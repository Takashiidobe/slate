//! Rust cleanup passes that run after faithful CIR lowering.

use crate::rust_ast::{
    Block, Expr, FnDef, IndentStmt, Item, Pattern, Program, RustValue, Stmt, UnaryOp,
};

pub fn apply(program: Program) -> Program {
    Program {
        items: program
            .items
            .into_iter()
            .map(|item| match item {
                Item::Fn(mut f) => {
                    inline_single_use_temps(&mut f.body);
                    eliminate_param_spills_ast(&mut f);
                    fuse_zero_init_ast(&mut f.body);
                    collapse_retval_ast(&mut f.body);
                    Item::Fn(f)
                }
                item => item,
            })
            .collect(),
    }
}

fn eliminate_param_spills_ast(f: &mut FnDef) {
    let param_names: Vec<String> = f.params.iter().map(|p| p.name.clone()).collect();
    let mut claimed_locals: Vec<String> = Vec::new();
    let mut removed: Vec<usize> = Vec::new();

    for param_index in 0..f.params.len() {
        if f.params[param_index].mutable {
            continue;
        }
        let param_name = f.params[param_index].name.clone();
        let param_ty = f.params[param_index].ty.render();
        let body_uses: usize = f
            .body
            .iter()
            .map(|stmt| stmt_ident_count(&stmt.stmt, &param_name))
            .sum();
        if body_uses != 1 {
            continue;
        }

        let Some((store_index, local)) =
            f.body
                .iter()
                .enumerate()
                .find_map(|(index, stmt)| match &stmt.stmt {
                    Stmt::Assign { target, value } => {
                        let local = expr_ident(target)?;
                        (expr_ident(value) == Some(param_name.as_str()))
                            .then(|| (index, local.to_string()))
                    }
                    _ => None,
                })
        else {
            continue;
        };
        if param_names.iter().any(|name| name == &local)
            || claimed_locals.iter().any(|name| name == &local)
        {
            continue;
        }

        let Some(decl_index) =
            f.body
                .iter()
                .enumerate()
                .take(store_index)
                .find_map(|(index, stmt)| match &stmt.stmt {
                    Stmt::Let {
                        name,
                        mutable: true,
                        ty: Some(ty),
                        ..
                    } if name == &local && ty.render() == param_ty => Some(index),
                    _ => None,
                })
        else {
            continue;
        };

        if f.body[decl_index + 1..store_index]
            .iter()
            .any(|stmt| stmt_ident_count(&stmt.stmt, &local) > 0)
        {
            continue;
        }

        f.params[param_index].name = local.clone();
        f.params[param_index].mutable = true;
        claimed_locals.push(local);
        removed.push(decl_index);
        removed.push(store_index);
    }

    removed.sort_unstable();
    removed.dedup();
    for index in removed.into_iter().rev() {
        f.body.remove(index);
    }
}

fn collapse_retval_ast(body: &mut Vec<IndentStmt>) {
    let Some((ret_index, name)) =
        body.iter()
            .enumerate()
            .find_map(|(index, stmt)| match &stmt.stmt {
                Stmt::Return(Some(expr)) => expr_ident(expr).map(|name| (index, name.to_string())),
                _ => None,
            })
    else {
        return;
    };
    if ret_index == 0 {
        return;
    }

    let store_index = ret_index - 1;
    let value = match &body[store_index].stmt {
        Stmt::Assign { target, value } if expr_ident(target) == Some(name.as_str()) => {
            value.clone()
        }
        _ => return,
    };

    let mentions: usize = body
        .iter()
        .map(|stmt| stmt_ident_count(&stmt.stmt, &name))
        .sum();
    if mentions != 3 {
        return;
    }

    let Some(decl_index) = body
        .iter()
        .position(|stmt| matches!(&stmt.stmt, Stmt::Let { name: n, .. } if n == &name))
    else {
        return;
    };

    body[ret_index].stmt = Stmt::Return(Some(value));
    let mut remove = [store_index, decl_index];
    remove.sort_unstable();
    for index in remove.into_iter().rev() {
        body.remove(index);
    }
}

fn fuse_zero_init_ast(body: &mut Vec<IndentStmt>) {
    for stmt in body.iter_mut() {
        for_nested_body(&mut stmt.stmt, fuse_zero_init_ast);
    }

    loop {
        let mut changed = false;
        for i in 0..body.len().saturating_sub(1) {
            let Stmt::Let {
                name,
                mutable: true,
                ty: Some(_),
                init: Some(init),
            } = &body[i].stmt
            else {
                continue;
            };
            if !is_zero_expr(init) {
                continue;
            }
            let Stmt::Assign { target, value } = &body[i + 1].stmt else {
                continue;
            };
            if expr_ident(target) != Some(name.as_str()) || expr_ident_count(value, name) != 0 {
                continue;
            }
            let value = value.clone();
            if let Stmt::Let { init, .. } = &mut body[i].stmt {
                *init = Some(value);
            }
            body.remove(i + 1);
            changed = true;
            break;
        }
        if !changed {
            break;
        }
    }
}

fn for_nested_body(stmt: &mut Stmt, f: fn(&mut Vec<IndentStmt>)) {
    match stmt {
        Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            f(then_body);
            f(else_body);
        }
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        } => {
            f(then_body);
            f(else_body);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => f(body),
        Stmt::Unsafe { body } => f(&mut body.stmts),
        _ => {}
    }
}

fn is_zero_expr(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Value(RustValue::I64(0)) | Expr::Value(RustValue::I128(0))
    ) || matches!(expr, Expr::Lit(s) if matches!(s.as_str(), "0" | "0.0" | "false"))
}

fn expr_ident(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(s) if is_ident(s.as_str()) => Some(s.as_str()),
        _ => None,
    }
}

// Inline single-use pure temps directly on the statement list. The temp's init is
// spliced as an `Expr` subtree into its use site and precedence-aware rendering
// elides redundant parens.
fn inline_single_use_temps(body: &mut Vec<IndentStmt>) {
    inline_nested_temps(body);
    loop {
        let mut applied = false;
        for i in 0..body.len() {
            let Stmt::Let {
                name,
                mutable: false,
                init: Some(init),
                ..
            } = &body[i].stmt
            else {
                continue;
            };
            if !is_temp_name(name) || !is_pure_expr(init) {
                continue;
            }
            let name = name.clone();
            let init = init.clone();
            let Some(use_index) = single_safe_use(body, i, &name) else {
                continue;
            };
            if body[use_index].stmt.substitute_var(&name, &init) {
                body.remove(i);
                applied = true;
                break;
            }
        }
        if !applied {
            break;
        }
        inline_nested_temps(body);
    }
}

fn inline_nested_temps(body: &mut [IndentStmt]) {
    for stmt in body {
        match &mut stmt.stmt {
            Stmt::If {
                then_body,
                else_body,
                ..
            } => {
                inline_single_use_temps(then_body);
                inline_single_use_temps(else_body);
            }
            Stmt::LetIf {
                then_body,
                else_body,
                ..
            } => {
                inline_single_use_temps(then_body);
                inline_single_use_temps(else_body);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                inline_single_use_temps(body);
            }
            Stmt::Unsafe { body } => inline_single_use_temps(&mut body.stmts),
            _ => {}
        }
    }
}

fn single_safe_use(body: &[IndentStmt], def_index: usize, name: &str) -> Option<usize> {
    let mut found = None;
    for (index, stmt) in body.iter().enumerate().skip(def_index + 1) {
        let stmt = &stmt.stmt;
        let uses = stmt_ident_count(stmt, name);
        if uses > 0 {
            if uses == 1
                && found.is_none()
                && !stmt_contains_call(stmt)
                && !is_receiver_use(stmt, name)
            {
                found = Some(index);
                continue;
            }
            return None;
        }
        if found.is_some() {
            continue;
        }
        if !is_pure_temp_let(stmt) {
            return None;
        }
    }
    found
}

fn is_pure_temp_let(stmt: &Stmt) -> bool {
    matches!(
        stmt,
        Stmt::Let { name, init: Some(init), .. } if is_temp_name(name) && is_pure_expr(init)
    )
}

// Conservative purity: only value/var arithmetic that has no side effects and no
// place dependence beyond its named operands. Matches (and never exceeds) what the
// prior text heuristic inlined, so inlining decisions are unchanged.
fn is_pure_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) => true,
        Expr::Lit(s) => s.bytes().all(|b| b.is_ascii_digit()),
        Expr::Var(_) => true,
        Expr::Unary { op, expr } => !matches!(op, UnaryOp::Not) && is_pure_expr(expr),
        Expr::Binary { lhs, rhs, .. } => is_pure_expr(lhs) && is_pure_expr(rhs),
        _ => false,
    }
}

fn stmt_contains_call(stmt: &Stmt) -> bool {
    let mut found = false;
    walk_stmt_exprs(stmt, &mut |expr| {
        found |= matches!(
            expr,
            Expr::Call { .. }
                | Expr::MethodCall { .. }
                | Expr::MethodCallGeneric { .. }
                | Expr::Macro { .. }
        );
    });
    found
}

fn is_receiver_use(stmt: &Stmt, name: &str) -> bool {
    let mut found = false;
    walk_stmt_exprs(stmt, &mut |expr| {
        let receiver = match expr {
            Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => Some(&**recv),
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => Some(&**base),
            _ => None,
        };
        if let Some(Expr::Var(v)) = receiver {
            found |= v.as_str() == name;
        }
    });
    found
}

fn walk_stmt_exprs(stmt: &Stmt, f: &mut impl FnMut(&Expr)) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(expr) = init {
                walk_expr(expr, f);
            }
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            walk_expr(cond, f);
            for stmt in then_body.iter().chain(else_body) {
                walk_stmt_exprs(&stmt.stmt, f);
            }
            walk_expr(then_value, f);
            walk_expr(else_value, f);
        }
        Stmt::Assign { target, value } => {
            walk_expr(target, f);
            walk_expr(value, f);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => walk_expr(expr, f),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            walk_expr(cond, f);
            for stmt in then_body.iter().chain(else_body) {
                walk_stmt_exprs(&stmt.stmt, f);
            }
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            for stmt in body {
                walk_stmt_exprs(&stmt.stmt, f);
            }
        }
        Stmt::Unsafe { body } => walk_block(body, f),
        Stmt::Match { expr, arms } => {
            walk_expr(expr, f);
            for arm in arms {
                for stmt in &arm.body {
                    walk_stmt_exprs(&stmt.stmt, f);
                }
            }
        }
        Stmt::While { cond, body } => {
            walk_expr(cond, f);
            walk_block(body, f);
        }
        Stmt::Block(body) => walk_block(body, f),
    }
}

fn walk_block(block: &Block, f: &mut impl FnMut(&Expr)) {
    for stmt in &block.stmts {
        walk_stmt_exprs(&stmt.stmt, f);
    }
    if let Some(tail) = &block.tail {
        walk_expr(tail, f);
    }
}

fn walk_expr(expr: &Expr, f: &mut impl FnMut(&Expr)) {
    f(expr);
    match expr {
        Expr::Value(_)
        | Expr::Lit(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => {}
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => walk_expr(expr, f),
        Expr::Binary { lhs, rhs, .. } => {
            walk_expr(lhs, f);
            walk_expr(rhs, f);
        }
        Expr::Call { func, args } => {
            walk_expr(func, f);
            for arg in args {
                walk_expr(arg, f);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            walk_expr(recv, f);
            for arg in args {
                walk_expr(arg, f);
            }
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => walk_expr(base, f),
        Expr::ArrayPtr { array, .. } => walk_expr(array, f),
        Expr::Index { base, index } => {
            walk_expr(base, f);
            walk_expr(index, f);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                walk_expr(value, f);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                walk_expr(elem, f);
            }
        }
        Expr::ArrayRepeat { elem, .. } => walk_expr(elem, f),
        Expr::Macro { args, .. } => {
            for arg in args {
                walk_expr(arg, f);
            }
        }
        Expr::Closure { body, .. } => walk_expr(body, f),
        Expr::Match { expr, arms } => {
            walk_expr(expr, f);
            for arm in arms {
                walk_expr(&arm.value, f);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            walk_expr(cond, f);
            walk_expr(then_expr, f);
            walk_expr(else_expr, f);
        }
        Expr::Block(block) | Expr::Unsafe(block) => walk_block(block, f),
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => walk_expr(ptr, f),
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            walk_expr(ptr, f);
            walk_expr(value, f);
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            walk_expr(ptr, f);
            walk_expr(expected, f);
            walk_expr(desired, f);
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            walk_expr(src, f);
            walk_expr(dst, f);
        }
    }
}

fn stmt_ident_count(stmt: &Stmt, name: &str) -> usize {
    match stmt {
        Stmt::Let { name: n, init, .. } => {
            usize::from(n == name) + init.as_ref().map_or(0, |expr| expr_ident_count(expr, name))
        }
        Stmt::LetIf {
            name: n,
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            usize::from(n == name)
                + expr_ident_count(cond, name)
                + then_body
                    .iter()
                    .chain(else_body)
                    .map(|stmt| stmt_ident_count(&stmt.stmt, name))
                    .sum::<usize>()
                + expr_ident_count(then_value, name)
                + expr_ident_count(else_value, name)
        }
        Stmt::Assign { target, value } => {
            expr_ident_count(target, name) + expr_ident_count(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_ident_count(expr, name),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => 0,
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_ident_count(cond, name)
                + then_body
                    .iter()
                    .chain(else_body)
                    .map(|stmt| stmt_ident_count(&stmt.stmt, name))
                    .sum::<usize>()
        }
        Stmt::Loop { label, body } => {
            label
                .as_ref()
                .map_or(0, |label| ident_count(label.as_str(), name))
                + body
                    .iter()
                    .map(|stmt| stmt_ident_count(&stmt.stmt, name))
                    .sum::<usize>()
        }
        Stmt::LabeledBlock { label, body } => {
            ident_count(label.as_str(), name)
                + body
                    .iter()
                    .map(|stmt| stmt_ident_count(&stmt.stmt, name))
                    .sum::<usize>()
        }
        Stmt::Scope { body } => body
            .iter()
            .map(|stmt| stmt_ident_count(&stmt.stmt, name))
            .sum(),
        Stmt::Unsafe { body } => block_ident_count(body, name),
        Stmt::Match { expr, arms } => {
            expr_ident_count(expr, name)
                + arms
                    .iter()
                    .map(|arm| {
                        pattern_ident_count(&arm.pattern, name)
                            + arm
                                .body
                                .iter()
                                .map(|stmt| stmt_ident_count(&stmt.stmt, name))
                                .sum::<usize>()
                    })
                    .sum::<usize>()
        }
        Stmt::While { cond, body } => expr_ident_count(cond, name) + block_ident_count(body, name),
        Stmt::Block(body) => block_ident_count(body, name),
    }
}

fn pattern_ident_count(pattern: &Pattern, name: &str) -> usize {
    match pattern {
        Pattern::Wildcard | Pattern::I64(_) | Pattern::I128(_) => 0,
        Pattern::Binding(binding) => usize::from(binding.as_str() == name),
        Pattern::TupleStruct { name: ctor, fields } => {
            usize::from(ctor.as_str() == name)
                + fields
                    .iter()
                    .map(|field| pattern_ident_count(field, name))
                    .sum::<usize>()
        }
    }
}

fn block_ident_count(block: &crate::rust_ast::Block, name: &str) -> usize {
    block
        .stmts
        .iter()
        .map(|stmt| stmt_ident_count(&stmt.stmt, name))
        .sum::<usize>()
        + block
            .tail
            .as_ref()
            .map_or(0, |tail| expr_ident_count(tail, name))
}

fn expr_ident_count(expr: &Expr, name: &str) -> usize {
    match expr {
        Expr::Value(_) => 0,
        Expr::Lit(s) => usize::from(s == name),
        Expr::Var(s) => usize::from(s.as_str() == name),
        Expr::Path(_) => 0,
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_ident_count(expr, name),
        Expr::Block(block) | Expr::Unsafe(block) => block_ident_count(block, name),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            expr_ident_count(src, name) + expr_ident_count(dst, name)
        }
        Expr::AtomicFence { .. } | Expr::Todo(_) => 0,
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => expr_ident_count(ptr, name),
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            expr_ident_count(ptr, name) + expr_ident_count(value, name)
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            expr_ident_count(ptr, name)
                + expr_ident_count(expected, name)
                + expr_ident_count(desired, name)
        }
        Expr::Binary { lhs, rhs, .. } => expr_ident_count(lhs, name) + expr_ident_count(rhs, name),
        Expr::Call { func, args } => {
            expr_ident_count(func, name)
                + args
                    .iter()
                    .map(|arg| expr_ident_count(arg, name))
                    .sum::<usize>()
        }
        Expr::MethodCall { recv, args, .. } => {
            expr_ident_count(recv, name)
                + args
                    .iter()
                    .map(|arg| expr_ident_count(arg, name))
                    .sum::<usize>()
        }
        Expr::MethodCallGeneric { recv, args, .. } => {
            expr_ident_count(recv, name)
                + args
                    .iter()
                    .map(|arg| expr_ident_count(arg, name))
                    .sum::<usize>()
        }
        Expr::Field { base, field } => expr_ident_count(base, name) + ident_count(field, name),
        Expr::TupleField { base, .. } => expr_ident_count(base, name),
        Expr::ArrayPtr { array, .. } => expr_ident_count(array, name),
        Expr::Index { base, index } => expr_ident_count(base, name) + expr_ident_count(index, name),
        Expr::StructLit {
            name: type_name,
            fields,
        } => {
            ident_count(type_name, name)
                + fields
                    .iter()
                    .map(|(field, value)| ident_count(field, name) + expr_ident_count(value, name))
                    .sum::<usize>()
        }
        Expr::ArrayLit(elems) => elems.iter().map(|elem| expr_ident_count(elem, name)).sum(),
        Expr::ArrayRepeat { elem, .. } => expr_ident_count(elem, name),
        Expr::Closure { params, body } => {
            if params.iter().any(|p| p.as_str() == name) {
                0
            } else {
                expr_ident_count(body, name)
            }
        }
        Expr::Macro {
            name: macro_name,
            args,
        } => {
            ident_count(macro_name, name)
                + args
                    .iter()
                    .map(|arg| expr_ident_count(arg, name))
                    .sum::<usize>()
        }
        Expr::Match { expr, arms } => {
            expr_ident_count(expr, name)
                + arms
                    .iter()
                    .map(|arm| {
                        pattern_ident_count(&arm.pattern, name) + expr_ident_count(&arm.value, name)
                    })
                    .sum::<usize>()
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_ident_count(cond, name)
                + expr_ident_count(then_expr, name)
                + expr_ident_count(else_expr, name)
        }
    }
}

fn is_ident(s: &str) -> bool {
    let bytes = s.as_bytes();
    !bytes.is_empty() && is_ident_start(bytes[0]) && bytes.iter().all(|&b| is_ident_continue(b))
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

fn ident_count(line: &str, name: &str) -> usize {
    let bytes = line.as_bytes();
    let name_bytes = name.as_bytes();
    let mut count = 0;
    let mut i = 0;
    while i + name_bytes.len() <= bytes.len() {
        if &bytes[i..i + name_bytes.len()] == name_bytes
            && (i == 0 || !is_ident_continue(bytes[i - 1]))
            && (i + name_bytes.len() == bytes.len()
                || !is_ident_continue(bytes[i + name_bytes.len()]))
        {
            count += 1;
            i += name_bytes.len();
        } else {
            i += 1;
        }
    }
    count
}

fn is_ident_start(b: u8) -> bool {
    b == b'_' || b.is_ascii_alphabetic()
}

fn is_ident_continue(b: u8) -> bool {
    is_ident_start(b) || b.is_ascii_digit()
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::rust_ast::{BinOp, FnParam, Prim, Type, Visibility};

    fn temp(name: &str, ty: &str, init: Expr) -> Stmt {
        Stmt::Let {
            name: name.to_string(),
            mutable: false,
            ty: Some(Type::parse(ty)),
            init: Some(init),
        }
    }

    fn let_mut(name: &str, ty: &str, init: Expr) -> Stmt {
        Stmt::Let {
            name: name.to_string(),
            mutable: true,
            ty: Some(Type::parse(ty)),
            init: Some(init),
        }
    }

    fn assign(target: &str, value: Expr) -> Stmt {
        Stmt::Assign {
            target: Expr::Var(target.into()),
            value,
        }
    }

    fn var(name: &str) -> Expr {
        Expr::Var(name.into())
    }

    fn int(n: i64) -> Expr {
        Expr::Value(RustValue::I64(n))
    }

    fn call(func: &str, args: Vec<Expr>) -> Expr {
        Expr::Call {
            func: Box::new(Expr::Var(func.into())),
            args,
        }
    }

    fn bin(op: BinOp, lhs: Expr, rhs: Expr) -> Expr {
        Expr::Binary {
            op,
            lhs: Box::new(lhs),
            rhs: Box::new(rhs),
        }
    }

    fn param(name: &str, ty: &str) -> FnParam {
        FnParam {
            name: name.into(),
            mutable: false,
            ty: Type::parse(ty),
        }
    }

    fn func(params: Vec<FnParam>, ret: Option<&str>, stmts: Vec<Stmt>) -> FnDef {
        FnDef {
            vis: Visibility::Private,
            unsafe_extern_c: false,
            name: "f".into(),
            params,
            ret: ret.map(Type::parse),
            body: stmts
                .into_iter()
                .map(|stmt| IndentStmt { depth: 1, stmt })
                .collect(),
        }
    }

    fn emit(f: FnDef) -> String {
        Program {
            items: vec![Item::Fn(f)],
        }
        .emit()
    }

    fn inlined(stmts: Vec<Stmt>) -> String {
        let mut f = func(vec![], None, stmts);
        inline_single_use_temps(&mut f.body);
        emit(f)
    }

    fn migrated_fn(body: Vec<Stmt>) -> FnDef {
        FnDef {
            vis: Visibility::Private,
            unsafe_extern_c: false,
            name: "add".into(),
            params: vec![
                FnParam {
                    name: "arg0".into(),
                    mutable: false,
                    ty: Type::Prim(Prim::I32),
                },
                FnParam {
                    name: "arg1".into(),
                    mutable: false,
                    ty: Type::Prim(Prim::I32),
                },
            ],
            ret: Some(Type::Prim(Prim::I32)),
            body: body
                .into_iter()
                .map(|stmt| IndentStmt { depth: 1, stmt })
                .collect(),
        }
    }

    #[test]
    fn apply_keeps_migrated_functions_structured() {
        let program = Program {
            items: vec![Item::Fn(migrated_fn(vec![
                Stmt::Let {
                    name: "a".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Let {
                    name: "b".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Let {
                    name: "__retval".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Let {
                    name: "c".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Assign {
                    target: Expr::Var("a".into()),
                    value: Expr::Var("arg0".into()),
                },
                Stmt::Assign {
                    target: Expr::Var("b".into()),
                    value: Expr::Var("arg1".into()),
                },
                Stmt::Assign {
                    target: Expr::Var("c".into()),
                    value: bin(BinOp::Add, Expr::Var("a".into()), Expr::Var("b".into())),
                },
                Stmt::Assign {
                    target: Expr::Var("__retval".into()),
                    value: Expr::Var("c".into()),
                },
                Stmt::Return(Some(Expr::Var("__retval".into()))),
            ]))],
        };

        let out = apply(program);
        let Item::Fn(f) = &out.items[0] else {
            panic!("migrated functions must remain structured");
        };
        assert_eq!(f.params[0].name, "a");
        assert!(f.params[0].mutable);
        assert_eq!(
            out.emit(),
            "\
fn add(mut a: i32, mut b: i32) -> i32 {
    let mut c: i32 = a + b;
    return c;
}
"
        );
    }

    fn after_body(
        pass: fn(&mut Vec<IndentStmt>),
        params: Vec<FnParam>,
        ret: Option<&str>,
        stmts: Vec<Stmt>,
    ) -> String {
        let mut f = func(params, ret, stmts);
        pass(&mut f.body);
        emit(f)
    }

    fn after_fn(pass: fn(&mut FnDef), f: FnDef) -> String {
        let mut f = f;
        pass(&mut f);
        emit(f)
    }

    #[test]
    fn collapses_retval_store_into_return() {
        let out = after_body(
            collapse_retval_ast,
            vec![],
            Some("i32"),
            vec![
                let_mut("__retval", "i32", int(0)),
                let_mut("c", "i32", int(0)),
                assign("c", bin(BinOp::Add, var("a"), var("b"))),
                assign("__retval", var("c")),
                Stmt::Return(Some(var("__retval"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let mut c: i32 = 0;
    c = a + b;
    return c;
}
"
        );
    }

    #[test]
    fn does_not_collapse_when_retval_read_elsewhere() {
        let stmts = vec![
            let_mut("__retval", "i32", int(0)),
            assign("__retval", int(1)),
            let_mut("x", "i32", var("__retval")),
            assign("__retval", var("x")),
            Stmt::Return(Some(var("__retval"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(
            after_body(collapse_retval_ast, vec![], Some("i32"), stmts),
            expected
        );
    }

    #[test]
    fn does_not_collapse_when_store_is_not_immediately_before_return() {
        let stmts = vec![
            let_mut("__retval", "i32", int(0)),
            assign("__retval", int(1)),
            let_mut("x", "i32", int(2)),
            Stmt::Return(Some(var("__retval"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(
            after_body(collapse_retval_ast, vec![], Some("i32"), stmts),
            expected
        );
    }

    #[test]
    fn folds_parameter_spills_into_direct_bindings() {
        let f = func(
            vec![param("arg0", "i32"), param("arg1", "i32")],
            Some("i32"),
            vec![
                let_mut("a", "i32", int(0)),
                let_mut("b", "i32", int(0)),
                let_mut("__retval", "i32", int(0)),
                let_mut("c", "i32", int(0)),
                assign("a", var("arg0")),
                assign("b", var("arg1")),
                assign("c", bin(BinOp::Add, var("a"), var("b"))),
                assign("__retval", var("c")),
                Stmt::Return(Some(var("__retval"))),
            ],
        );

        assert_eq!(
            after_fn(eliminate_param_spills_ast, f),
            "\
fn f(mut a: i32, mut b: i32) -> i32 {
    let mut __retval: i32 = 0;
    let mut c: i32 = 0;
    c = a + b;
    __retval = c;
    return __retval;
}
"
        );
    }

    #[test]
    fn does_not_fold_when_parameter_is_read_again() {
        let f = func(
            vec![param("arg0", "i32")],
            Some("i32"),
            vec![
                let_mut("a", "i32", int(0)),
                assign("a", var("arg0")),
                Stmt::Return(Some(var("arg0"))),
            ],
        );
        let expected = emit(f.clone());

        assert_eq!(after_fn(eliminate_param_spills_ast, f), expected);
    }

    #[test]
    fn does_not_fold_when_slot_read_before_spill() {
        let f = func(
            vec![param("arg0", "i32")],
            Some("i32"),
            vec![
                let_mut("a", "i32", int(0)),
                let_mut("b", "i32", var("a")),
                assign("a", var("arg0")),
                Stmt::Return(Some(var("b"))),
            ],
        );
        let expected = emit(f.clone());

        assert_eq!(after_fn(eliminate_param_spills_ast, f), expected);
    }

    #[test]
    fn does_not_fold_when_slot_type_differs_from_parameter() {
        let f = func(
            vec![param("arg0", "i32")],
            Some("i64"),
            vec![
                let_mut("a", "i64", int(0)),
                assign("a", var("arg0")),
                Stmt::Return(Some(var("a"))),
            ],
        );
        let expected = emit(f.clone());

        assert_eq!(after_fn(eliminate_param_spills_ast, f), expected);
    }

    #[test]
    fn fuses_zero_init_with_immediate_first_assignment() {
        let out = after_body(
            fuse_zero_init_ast,
            vec![],
            Some("i32"),
            vec![
                let_mut("c", "i32", int(0)),
                assign("c", bin(BinOp::Add, var("a"), var("b"))),
                Stmt::Return(Some(var("c"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let mut c: i32 = a + b;
    return c;
}
"
        );
    }

    #[test]
    fn does_not_fuse_when_first_assignment_reads_the_placeholder() {
        let stmts = vec![
            let_mut("c", "i32", int(0)),
            assign("c", bin(BinOp::Add, var("c"), int(1))),
            Stmt::Return(Some(var("c"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(
            after_body(fuse_zero_init_ast, vec![], Some("i32"), stmts),
            expected
        );
    }

    #[test]
    fn does_not_fuse_when_assignment_is_not_immediate() {
        let stmts = vec![
            let_mut("c", "i32", int(0)),
            Stmt::If {
                cond: var("cond"),
                then_body: vec![IndentStmt {
                    depth: 2,
                    stmt: assign("c", int(1)),
                }],
                else_body: vec![],
            },
            Stmt::Return(Some(var("c"))),
        ];
        let expected = emit(func(
            vec![param("cond", "bool")],
            Some("i32"),
            stmts.clone(),
        ));

        assert_eq!(
            after_body(
                fuse_zero_init_ast,
                vec![param("cond", "bool")],
                Some("i32"),
                stmts
            ),
            expected
        );
    }

    #[test]
    fn does_not_fuse_non_placeholder_initializers() {
        let stmts = vec![
            let_mut("c", "i32", int(7)),
            assign("c", int(1)),
            Stmt::Return(Some(var("c"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(
            after_body(fuse_zero_init_ast, vec![], Some("i32"), stmts),
            expected
        );
    }

    #[test]
    fn inlines_single_use_scalar_temps() {
        let out = inlined(vec![
            let_mut("a", "i32", int(0)),
            temp("_v0", "i32", int(20)),
            assign("a", var("_v0")),
            temp("_v1", "i32", int(5)),
            temp("_v2", "i32", var("a")),
            temp("_v3", "i32", bin(BinOp::Sub, var("_v2"), var("_v1"))),
            assign("a", var("_v3")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let mut a: i32 = 0;
    a = 20;
    a = a - 5;
}
"
        );
    }

    #[test]
    fn does_not_inline_call_results() {
        let out = inlined(vec![
            temp("_v0", "i32", call("g", vec![])),
            temp("_v1", "i32", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = g();
    let _v1: i32 = _v0;
}
"
        );
    }

    #[test]
    fn does_not_cross_side_effecting_statement() {
        let out = inlined(vec![
            temp("_v0", "i32", var("a")),
            Stmt::Expr(call("printf", vec![var("_v1")])),
            assign("b", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = a;
    printf(_v1);
    b = _v0;
}
"
        );
    }

    #[test]
    fn does_not_inline_impure_intrinsics() {
        let out = inlined(vec![
            temp(
                "_v0",
                "i32",
                call("std::ptr::read_volatile", vec![var("p")]),
            ),
            assign("b", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = std::ptr::read_volatile(p);
    b = _v0;
}
"
        );
    }

    #[test]
    fn does_not_inline_method_receivers_that_need_type_annotations() {
        let out = inlined(vec![
            temp("_v0", "i32", int(2147483647)),
            temp("_v1", "i32", int(1)),
            Stmt::Let {
                name: "_v2".into(),
                mutable: false,
                ty: None,
                init: Some(Expr::MethodCall {
                    recv: Box::new(var("_v0")),
                    method: "overflowing_add".into(),
                    args: vec![var("_v1")],
                }),
            },
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = 2147483647;
    let _v1: i32 = 1;
    let _v2 = _v0.overflowing_add(_v1);
}
"
        );
    }

    #[test]
    fn does_not_inline_call_arguments_that_need_type_annotations() {
        let out = inlined(vec![
            temp("_v0", "i64", int(9223372036854775807)),
            Stmt::Let {
                name: "_v1".into(),
                mutable: false,
                ty: Some(Type::parse("i32")),
                init: Some(call("printf", vec![var("_v0")])),
            },
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i64 = 9223372036854775807;
    let _v1: i32 = printf(_v0);
}
"
        );
    }
}
