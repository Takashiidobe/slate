use crate::fixups::facts::{
    AstPath, BindingKind, FixupFacts, FunctionId, PathSegment, StringBufferKind,
};
use crate::fixups::support::walk;
use crate::rust_ast::{BinOp, Block, Expr, ExternDecl, Ident, IndentStmt, Item, Path, Prim, Type};
use crate::rust_ast::{Program, RustValue, Stmt};
use std::collections::BTreeMap;

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            let mut env = root_env(function, facts);
            fixup_body(&mut f.body, function, facts, &mut env, &mut Vec::new());
        }
    }
    prune_unused_externs(program);
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum StringKind {
    Str,
    Bytes,
}

#[derive(Clone)]
struct Source {
    name: String,
    kind: StringKind,
}

#[derive(Clone)]
struct Compare {
    lhs: Expr,
    rhs: Expr,
}

fn fixup_body(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    env: &mut BTreeMap<String, StringKind>,
    path: &mut Vec<PathSegment>,
) {
    let mut temps = BTreeMap::new();
    let mut remove = Vec::new();
    for i in 0..body.len() {
        let candidate = match &body[i].stmt {
            Stmt::Let {
                name,
                ty: Some(Type::Prim(Prim::I32)),
                init: Some(init),
                ..
            } => supported_compare_call(init, env)
                .filter(|_| temp_uses_are_zero_comparisons(&body[i + 1..], name))
                .map(|compare| (name.clone(), compare)),
            _ => None,
        };
        if let Some((name, compare)) = candidate {
            temps.insert(name, compare);
            remove.push(i);
        } else {
            let mut stmt_path = stmt_path(path, i);
            fixup_stmt(
                &mut body[i].stmt,
                function,
                facts,
                env,
                &temps,
                &mut stmt_path,
            );
            record_local_string_kind(&body[i].stmt, function, facts, env, &stmt_path);
        }
    }
    for i in remove.into_iter().rev() {
        body.remove(i);
    }
}

fn root_env(function: FunctionId, facts: &FixupFacts) -> BTreeMap<String, StringKind> {
    facts
        .bindings
        .iter()
        .filter(|binding| binding.function == function)
        .filter(|binding| matches!(binding.kind, BindingKind::Param { .. }))
        .filter_map(|binding| {
            let kind = facts
                .string_buffer(binding.id)
                .and_then(string_kind_for_buffer)?;
            Some((binding.name.clone(), kind))
        })
        .collect()
}

fn record_local_string_kind(
    stmt: &Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    env: &mut BTreeMap<String, StringKind>,
    path: &[PathSegment],
) {
    let Stmt::Let { name, .. } = stmt else {
        return;
    };
    let Some(binding) = facts.binding_by_local_path(function, name, &AstPath(path.to_vec())) else {
        return;
    };
    if let Some(kind) = facts
        .string_buffer(binding)
        .and_then(string_kind_for_buffer)
    {
        env.insert(name.clone(), kind);
    }
}

fn string_kind_for_buffer(buffer: &crate::fixups::facts::StringBufferFact) -> Option<StringKind> {
    match buffer.kind {
        StringBufferKind::BorrowedStr | StringBufferKind::OwnedString => Some(StringKind::Str),
        StringBufferKind::BorrowedBytes => Some(StringKind::Bytes),
        StringBufferKind::CharArray => None,
    }
}

fn prune_unused_externs(program: &mut Program) {
    let used = libc_string_calls(program);
    program.items.retain_mut(|item| match item {
        Item::ExternBlock { decls, .. } => {
            decls.retain(|decl| match decl {
                ExternDecl::Fn(f) if is_libc_string_func(&f.name) => used.contains(&f.name),
                _ => true,
            });
            !decls.is_empty()
        }
        _ => true,
    });
}

fn libc_string_calls(program: &Program) -> Vec<String> {
    let mut calls = Vec::new();
    for item in &program.items {
        if let Item::Fn(f) = item {
            body_libc_string_calls(&f.body, &mut calls);
        }
    }
    calls.sort();
    calls.dedup();
    calls
}

fn body_libc_string_calls(body: &[IndentStmt], calls: &mut Vec<String>) {
    for indent in body {
        stmt_libc_string_calls(&indent.stmt, calls);
    }
}

fn block_libc_string_calls(block: &Block, calls: &mut Vec<String>) {
    body_libc_string_calls(&block.stmts, calls);
    if let Some(tail) = &block.tail {
        expr_libc_string_calls(tail, calls);
    }
}

fn stmt_libc_string_calls(stmt: &Stmt, calls: &mut Vec<String>) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(init) = init {
                expr_libc_string_calls(init, calls);
            }
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_libc_string_calls(target, calls);
            expr_libc_string_calls(value, calls);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_libc_string_calls(expr, calls),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_libc_string_calls(cond, calls);
            body_libc_string_calls(then_body, calls);
            body_libc_string_calls(else_body, calls);
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_libc_string_calls(cond, calls);
            body_libc_string_calls(then_body, calls);
            expr_libc_string_calls(then_value, calls);
            body_libc_string_calls(else_body, calls);
            expr_libc_string_calls(else_value, calls);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_libc_string_calls(body, calls);
        }
        Stmt::While { cond, body } => {
            expr_libc_string_calls(cond, calls);
            block_libc_string_calls(body, calls);
        }
        Stmt::Block(body) | Stmt::Unsafe { body } => block_libc_string_calls(body, calls),
        Stmt::Match { expr, arms } => {
            expr_libc_string_calls(expr, calls);
            for arm in arms {
                body_libc_string_calls(&arm.body, calls);
            }
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

fn expr_libc_string_calls(expr: &Expr, calls: &mut Vec<String>) {
    match expr {
        Expr::Call { func, args } => {
            if let Expr::Var(name) = &**func
                && is_libc_string_func(name.as_str())
            {
                calls.push(name.as_str().into());
            }
            expr_libc_string_calls(func, calls);
            for arg in args {
                expr_libc_string_calls(arg, calls);
            }
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_libc_string_calls(expr, calls),
        Expr::Binary { lhs, rhs, .. } => {
            expr_libc_string_calls(lhs, calls);
            expr_libc_string_calls(rhs, calls);
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_libc_string_calls(recv, calls);
            for arg in args {
                expr_libc_string_calls(arg, calls);
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => expr_libc_string_calls(base, calls),
        Expr::Index { base, index } => {
            expr_libc_string_calls(base, calls);
            expr_libc_string_calls(index, calls);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                expr_libc_string_calls(value, calls);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                expr_libc_string_calls(elem, calls);
            }
        }
        Expr::ArrayRepeat { elem, .. } => expr_libc_string_calls(elem, calls),
        Expr::Macro { args, .. } => {
            for arg in args {
                expr_libc_string_calls(arg, calls);
            }
        }
        Expr::Closure { body, .. } => expr_libc_string_calls(body, calls),
        Expr::Match { expr, arms } => {
            expr_libc_string_calls(expr, calls);
            for arm in arms {
                expr_libc_string_calls(&arm.value, calls);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_libc_string_calls(cond, calls);
            expr_libc_string_calls(then_expr, calls);
            expr_libc_string_calls(else_expr, calls);
        }
        Expr::Block(block) | Expr::Unsafe(block) => block_libc_string_calls(block, calls),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            expr_libc_string_calls(src, calls);
            expr_libc_string_calls(dst, calls);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            expr_libc_string_calls(src, calls);
            expr_libc_string_calls(dst, calls);
            expr_libc_string_calls(count, calls);
        }
        Expr::WriteBytes { dst, val, count } => {
            expr_libc_string_calls(dst, calls);
            expr_libc_string_calls(val, calls);
            expr_libc_string_calls(count, calls);
        }
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
            expr_libc_string_calls(ptr, calls);
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            expr_libc_string_calls(ptr, calls);
            expr_libc_string_calls(value, calls);
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            expr_libc_string_calls(ptr, calls);
            expr_libc_string_calls(expected, calls);
            expr_libc_string_calls(desired, calls);
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => {}
    }
}

fn fixup_stmt(
    stmt: &mut Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    env: &BTreeMap<String, StringKind>,
    temps: &BTreeMap<String, Compare>,
    path: &mut Vec<PathSegment>,
) {
    match stmt {
        Stmt::Let { ty, init, .. } => {
            if let Some(init) = init {
                if let Some(source) = supported_strlen_call(init, env) {
                    *init = strlen_replacement(source);
                    if matches!(ty, Some(Type::Prim(Prim::U64))) {
                        *ty = Some(Type::Prim(Prim::Usize));
                    }
                    return;
                }
                fixup_expr(init, env, temps);
            }
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            fixup_expr(target, env, temps);
            fixup_expr(value, env, temps);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => fixup_expr(expr, env, temps),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            fixup_expr(cond, env, temps);
            let mut then_env = env.clone();
            let mut else_env = env.clone();
            walk::with_path_segment(path, PathSegment::Then, |path| {
                fixup_body(then_body, function, facts, &mut then_env, path);
            });
            walk::with_path_segment(path, PathSegment::Else, |path| {
                fixup_body(else_body, function, facts, &mut else_env, path);
            });
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            fixup_expr(cond, env, temps);
            let mut then_env = env.clone();
            let mut else_env = env.clone();
            walk::with_path_segment(path, PathSegment::Then, |path| {
                fixup_body(then_body, function, facts, &mut then_env, path);
            });
            fixup_expr(then_value, &then_env, temps);
            walk::with_path_segment(path, PathSegment::Else, |path| {
                fixup_body(else_body, function, facts, &mut else_env, path);
            });
            fixup_expr(else_value, &else_env, temps);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            let mut nested_env = env.clone();
            fixup_body(body, function, facts, &mut nested_env, path);
        }
        Stmt::While { cond, body } => {
            fixup_expr(cond, env, temps);
            fixup_block(body, function, facts, env, path);
        }
        Stmt::Block(body) | Stmt::Unsafe { body } => fixup_block(body, function, facts, env, path),
        Stmt::Match { expr, arms } => {
            fixup_expr(expr, env, temps);
            for (index, arm) in arms.iter_mut().enumerate() {
                let mut arm_env = env.clone();
                walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                    fixup_body(&mut arm.body, function, facts, &mut arm_env, path);
                });
            }
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

fn fixup_block(
    block: &mut Block,
    function: FunctionId,
    facts: &FixupFacts,
    env: &BTreeMap<String, StringKind>,
    path: &mut Vec<PathSegment>,
) {
    let mut block_env = env.clone();
    fixup_body(&mut block.stmts, function, facts, &mut block_env, path);
    if let Some(tail) = &mut block.tail {
        fixup_expr(tail, &block_env, &BTreeMap::new());
    }
}

fn fixup_expr(
    expr: &mut Expr,
    env: &BTreeMap<String, StringKind>,
    temps: &BTreeMap<String, Compare>,
) {
    if let Some(replacement) = replacement_expr(expr, env, temps) {
        *expr = replacement;
        return;
    }
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => fixup_expr(expr, env, temps),
        Expr::Binary { lhs, rhs, .. } => {
            fixup_expr(lhs, env, temps);
            fixup_expr(rhs, env, temps);
        }
        Expr::Call { func, args } => {
            fixup_expr(func, env, temps);
            for arg in args {
                fixup_expr(arg, env, temps);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            fixup_expr(recv, env, temps);
            for arg in args {
                fixup_expr(arg, env, temps);
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => fixup_expr(base, env, temps),
        Expr::Index { base, index } => {
            fixup_expr(base, env, temps);
            fixup_expr(index, env, temps);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                fixup_expr(value, env, temps);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                fixup_expr(elem, env, temps);
            }
        }
        Expr::ArrayRepeat { elem, .. } => fixup_expr(elem, env, temps),
        Expr::Macro { args, .. } => {
            for arg in args {
                fixup_expr(arg, env, temps);
            }
        }
        Expr::Closure { body, .. } => fixup_expr(body, env, temps),
        Expr::Match { expr, arms } => {
            fixup_expr(expr, env, temps);
            for arm in arms {
                fixup_expr(&mut arm.value, env, temps);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            fixup_expr(cond, env, temps);
            fixup_expr(then_expr, env, temps);
            fixup_expr(else_expr, env, temps);
        }
        Expr::Block(block) | Expr::Unsafe(block) => fixup_block(
            block,
            FunctionId(usize::MAX),
            &FixupFacts::default(),
            env,
            &mut Vec::new(),
        ),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            fixup_expr(src, env, temps);
            fixup_expr(dst, env, temps);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            fixup_expr(src, env, temps);
            fixup_expr(dst, env, temps);
            fixup_expr(count, env, temps);
        }
        Expr::WriteBytes { dst, val, count } => {
            fixup_expr(dst, env, temps);
            fixup_expr(val, env, temps);
            fixup_expr(count, env, temps);
        }
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => fixup_expr(ptr, env, temps),
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            fixup_expr(ptr, env, temps);
            fixup_expr(value, env, temps);
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            fixup_expr(ptr, env, temps);
            fixup_expr(expected, env, temps);
            fixup_expr(desired, env, temps);
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => {}
    }
}

fn replacement_expr(
    expr: &Expr,
    env: &BTreeMap<String, StringKind>,
    temps: &BTreeMap<String, Compare>,
) -> Option<Expr> {
    if let Some((compare, op)) = temp_zero_comparison(expr, temps) {
        return Some(compare_to_bool(compare, op));
    }
    if let Expr::Binary { op, lhs, rhs } = expr
        && op.is_comparison()
    {
        if let Some(compare) = supported_compare_call(lhs, env)
            && is_zero(rhs)
        {
            return Some(compare_to_bool(&compare, *op));
        }
        if let Some(compare) = supported_compare_call(rhs, env)
            && is_zero(lhs)
        {
            return Some(compare_to_bool(&compare, flip_comparison(*op)));
        }
    }
    if let Some(source) = supported_strlen_call(expr, env) {
        return Some(strlen_replacement(source));
    }
    supported_compare_call(expr, env).map(|compare| cmp_to_i32(compare_expr(compare)))
}

fn strlen_replacement(source: Source) -> Expr {
    Expr::MethodCall {
        recv: Box::new(Expr::Var(source.name.into())),
        method: "len".into(),
        args: Vec::new(),
    }
}

fn supported_strlen_call(expr: &Expr, env: &BTreeMap<String, StringKind>) -> Option<Source> {
    let Expr::Call { func, args } = peel_empty_unsafe(expr) else {
        return None;
    };
    if !matches!(&**func, Expr::Var(name) if name.as_str() == "strlen") || args.len() != 1 {
        return None;
    }
    pointer_source(&args[0], env)
}

fn supported_compare_call(expr: &Expr, env: &BTreeMap<String, StringKind>) -> Option<Compare> {
    let Expr::Call { func, args } = peel_empty_unsafe(expr) else {
        return None;
    };
    let Expr::Var(name) = &**func else {
        return None;
    };
    match name.as_str() {
        "strcmp" if args.len() == 2 => {
            let lhs = pointer_source(&args[0], env)?;
            let rhs = pointer_source(&args[1], env)?;
            comparable(lhs, rhs).map(|(lhs, rhs)| Compare { lhs, rhs })
        }
        "strncmp" if args.len() == 3 => {
            let lhs = pointer_source(&args[0], env)?;
            let rhs = pointer_source(&args[1], env)?;
            Some(Compare {
                lhs: prefix(lhs, args[2].clone()),
                rhs: prefix(rhs, args[2].clone()),
            })
        }
        "memcmp" if args.len() == 3 => {
            let lhs = pointer_source(&args[0], env)?;
            let rhs = pointer_source(&args[1], env)?;
            Some(Compare {
                lhs: prefix(lhs, args[2].clone()),
                rhs: prefix(rhs, args[2].clone()),
            })
        }
        _ => None,
    }
}

fn is_libc_string_func(name: &str) -> bool {
    matches!(
        name,
        "strlen" | "strcmp" | "strncmp" | "memcmp" | "strcpy" | "strncpy" | "strcat" | "strncat"
    )
}

fn peel_empty_unsafe(expr: &Expr) -> &Expr {
    if let Expr::Unsafe(block) = expr
        && block.stmts.is_empty()
        && let Some(tail) = &block.tail
    {
        return tail;
    }
    expr
}

fn pointer_source(expr: &Expr, env: &BTreeMap<String, StringKind>) -> Option<Source> {
    match expr {
        Expr::Var(name) => env.get(name.as_str()).map(|kind| Source {
            name: name.as_str().into(),
            kind: *kind,
        }),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            pointer_source(recv, env)
        }
        Expr::ArrayPtr { array, .. } => pointer_source(array, env),
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => pointer_source(expr, env),
        _ => None,
    }
}

fn comparable(lhs: Source, rhs: Source) -> Option<(Expr, Expr)> {
    if lhs.kind != rhs.kind {
        return None;
    }
    Some((Expr::Var(lhs.name.into()), Expr::Var(rhs.name.into())))
}

fn prefix(source: Source, count: Expr) -> Expr {
    let base = match source.kind {
        StringKind::Str => Expr::MethodCall {
            recv: Box::new(Expr::Var(source.name.clone().into())),
            method: "as_bytes".into(),
            args: Vec::new(),
        },
        StringKind::Bytes => Expr::Var(source.name.clone().into()),
    };
    let len = Expr::MethodCall {
        recv: Box::new(Expr::Var(source.name.into())),
        method: "len".into(),
        args: Vec::new(),
    };
    let n = Expr::Call {
        func: Box::new(path_expr(["std", "cmp", "min"])),
        args: vec![
            Expr::Cast {
                expr: Box::new(count),
                ty: Type::Prim(Prim::Usize),
            },
            len,
        ],
    };
    Expr::TupleField {
        base: Box::new(Expr::MethodCall {
            recv: Box::new(base),
            method: "split_at".into(),
            args: vec![n],
        }),
        index: 0,
    }
}

fn compare_expr(compare: Compare) -> Expr {
    Expr::MethodCall {
        recv: Box::new(compare.lhs),
        method: "cmp".into(),
        args: vec![compare.rhs],
    }
}

fn compare_to_bool(compare: &Compare, op: BinOp) -> Expr {
    match op {
        BinOp::Eq => Expr::Binary {
            op: BinOp::Eq,
            lhs: Box::new(compare.lhs.clone()),
            rhs: Box::new(compare.rhs.clone()),
        },
        BinOp::Ne => Expr::Binary {
            op: BinOp::Ne,
            lhs: Box::new(compare.lhs.clone()),
            rhs: Box::new(compare.rhs.clone()),
        },
        BinOp::Lt => ordering_compare(compare.clone(), BinOp::Eq, "Less"),
        BinOp::Le => ordering_compare(compare.clone(), BinOp::Ne, "Greater"),
        BinOp::Gt => ordering_compare(compare.clone(), BinOp::Eq, "Greater"),
        BinOp::Ge => ordering_compare(compare.clone(), BinOp::Ne, "Less"),
        _ => unreachable!(),
    }
}

fn ordering_compare(compare: Compare, op: BinOp, variant: &str) -> Expr {
    Expr::Binary {
        op,
        lhs: Box::new(compare_expr(compare)),
        rhs: Box::new(ordering_variant(variant)),
    }
}

fn cmp_to_i32(cmp: Expr) -> Expr {
    Expr::If {
        cond: Box::new(Expr::Binary {
            op: BinOp::Eq,
            lhs: Box::new(cmp.clone()),
            rhs: Box::new(ordering_variant("Less")),
        }),
        then_expr: Box::new(Expr::Value(RustValue::I64(-1))),
        else_expr: Box::new(Expr::If {
            cond: Box::new(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(cmp),
                rhs: Box::new(ordering_variant("Equal")),
            }),
            then_expr: Box::new(Expr::Value(RustValue::I64(0))),
            else_expr: Box::new(Expr::Value(RustValue::I64(1))),
        }),
    }
}

fn temp_zero_comparison<'a>(
    expr: &Expr,
    temps: &'a BTreeMap<String, Compare>,
) -> Option<(&'a Compare, BinOp)> {
    let Expr::Binary { op, lhs, rhs } = expr else {
        return None;
    };
    if !op.is_comparison() {
        return None;
    }
    match (&**lhs, &**rhs) {
        (Expr::Var(name), rhs) if is_zero(rhs) => Some((temps.get(name.as_str())?, *op)),
        (lhs, Expr::Var(name)) if is_zero(lhs) => {
            Some((temps.get(name.as_str())?, flip_comparison(*op)))
        }
        _ => None,
    }
}

fn temp_uses_are_zero_comparisons(body: &[IndentStmt], name: &str) -> bool {
    body.iter()
        .all(|indent| stmt_temp_uses_are_zero_comparisons(&indent.stmt, name))
}

fn stmt_temp_uses_are_zero_comparisons(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_none_or(|expr| expr_temp_uses_are_zero_comparisons(expr, name)),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_temp_uses_are_zero_comparisons(target, name)
                && expr_temp_uses_are_zero_comparisons(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            expr_temp_uses_are_zero_comparisons(expr, name)
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_temp_uses_are_zero_comparisons(cond, name)
                && temp_uses_are_zero_comparisons(then_body, name)
                && temp_uses_are_zero_comparisons(else_body, name)
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_temp_uses_are_zero_comparisons(cond, name)
                && temp_uses_are_zero_comparisons(then_body, name)
                && expr_temp_uses_are_zero_comparisons(then_value, name)
                && temp_uses_are_zero_comparisons(else_body, name)
                && expr_temp_uses_are_zero_comparisons(else_value, name)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            temp_uses_are_zero_comparisons(body, name)
        }
        Stmt::While { cond, body } => {
            expr_temp_uses_are_zero_comparisons(cond, name)
                && block_temp_uses_are_zero_comparisons(body, name)
        }
        Stmt::Block(body) | Stmt::Unsafe { body } => {
            block_temp_uses_are_zero_comparisons(body, name)
        }
        Stmt::Match { expr, arms } => {
            expr_temp_uses_are_zero_comparisons(expr, name)
                && arms
                    .iter()
                    .all(|arm| temp_uses_are_zero_comparisons(&arm.body, name))
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => true,
    }
}

fn block_temp_uses_are_zero_comparisons(block: &Block, name: &str) -> bool {
    temp_uses_are_zero_comparisons(&block.stmts, name)
        && block
            .tail
            .as_deref()
            .is_none_or(|tail| expr_temp_uses_are_zero_comparisons(tail, name))
}

fn expr_temp_uses_are_zero_comparisons(expr: &Expr, name: &str) -> bool {
    if is_zero_comparison_with_var(expr, name) {
        return true;
    }
    match expr {
        Expr::Var(v) if v.as_str() == name => false,
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_temp_uses_are_zero_comparisons(expr, name),
        Expr::Binary { lhs, rhs, .. } => {
            expr_temp_uses_are_zero_comparisons(lhs, name)
                && expr_temp_uses_are_zero_comparisons(rhs, name)
        }
        Expr::Call { func, args } => {
            expr_temp_uses_are_zero_comparisons(func, name)
                && args
                    .iter()
                    .all(|arg| expr_temp_uses_are_zero_comparisons(arg, name))
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_temp_uses_are_zero_comparisons(recv, name)
                && args
                    .iter()
                    .all(|arg| expr_temp_uses_are_zero_comparisons(arg, name))
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => expr_temp_uses_are_zero_comparisons(base, name),
        Expr::Index { base, index } => {
            expr_temp_uses_are_zero_comparisons(base, name)
                && expr_temp_uses_are_zero_comparisons(index, name)
        }
        Expr::StructLit { fields, .. } => fields
            .iter()
            .all(|(_, value)| expr_temp_uses_are_zero_comparisons(value, name)),
        Expr::ArrayLit(elems) => elems
            .iter()
            .all(|elem| expr_temp_uses_are_zero_comparisons(elem, name)),
        Expr::ArrayRepeat { elem, .. } => expr_temp_uses_are_zero_comparisons(elem, name),
        Expr::Macro { args, .. } => args
            .iter()
            .all(|arg| expr_temp_uses_are_zero_comparisons(arg, name)),
        Expr::Closure { body, .. } => expr_temp_uses_are_zero_comparisons(body, name),
        Expr::Match { expr, arms } => {
            expr_temp_uses_are_zero_comparisons(expr, name)
                && arms
                    .iter()
                    .all(|arm| expr_temp_uses_are_zero_comparisons(&arm.value, name))
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_temp_uses_are_zero_comparisons(cond, name)
                && expr_temp_uses_are_zero_comparisons(then_expr, name)
                && expr_temp_uses_are_zero_comparisons(else_expr, name)
        }
        Expr::Block(block) | Expr::Unsafe(block) => {
            block_temp_uses_are_zero_comparisons(block, name)
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            expr_temp_uses_are_zero_comparisons(src, name)
                && expr_temp_uses_are_zero_comparisons(dst, name)
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            expr_temp_uses_are_zero_comparisons(src, name)
                && expr_temp_uses_are_zero_comparisons(dst, name)
                && expr_temp_uses_are_zero_comparisons(count, name)
        }
        Expr::WriteBytes { dst, val, count } => {
            expr_temp_uses_are_zero_comparisons(dst, name)
                && expr_temp_uses_are_zero_comparisons(val, name)
                && expr_temp_uses_are_zero_comparisons(count, name)
        }
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
            expr_temp_uses_are_zero_comparisons(ptr, name)
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            expr_temp_uses_are_zero_comparisons(ptr, name)
                && expr_temp_uses_are_zero_comparisons(value, name)
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            expr_temp_uses_are_zero_comparisons(ptr, name)
                && expr_temp_uses_are_zero_comparisons(expected, name)
                && expr_temp_uses_are_zero_comparisons(desired, name)
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => true,
    }
}

fn is_zero_comparison_with_var(expr: &Expr, name: &str) -> bool {
    let Expr::Binary { op, lhs, rhs } = expr else {
        return false;
    };
    op.is_comparison()
        && ((matches!(&**lhs, Expr::Var(v) if v.as_str() == name) && is_zero(rhs))
            || (is_zero(lhs) && matches!(&**rhs, Expr::Var(v) if v.as_str() == name)))
}

fn lifted_kind(ty: &Type) -> Option<StringKind> {
    match ty {
        Type::Custom(name) if name == "String" => Some(StringKind::Str),
        Type::Ref {
            mutable: false,
            inner,
        } => match &**inner {
            Type::Str => Some(StringKind::Str),
            Type::Slice(elem) if matches!(&**elem, Type::Prim(Prim::U8)) => Some(StringKind::Bytes),
            _ => None,
        },
        _ => None,
    }
}

fn is_zero(expr: &Expr) -> bool {
    match expr {
        Expr::Value(RustValue::I64(0) | RustValue::I128(0)) => true,
        Expr::Cast { expr, .. } => is_zero(expr),
        _ => false,
    }
}

fn flip_comparison(op: BinOp) -> BinOp {
    match op {
        BinOp::Lt => BinOp::Gt,
        BinOp::Le => BinOp::Ge,
        BinOp::Gt => BinOp::Lt,
        BinOp::Ge => BinOp::Le,
        op => op,
    }
}

fn ordering_variant(variant: &str) -> Expr {
    path_expr(["std", "cmp", "Ordering", variant])
}

fn path_expr<const N: usize>(segments: [&str; N]) -> Expr {
    Expr::Path(Path::new(segments.into_iter().map(Ident::new)))
}

fn stmt_path(path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, Program};

    fn fixed(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        };
        let analyzed = crate::fixups::facts::analyze(program.clone());
        fixup(&mut program, &analyzed.facts);
        program.emit()
    }

    #[test]
    fn rewrites_strlen_on_lifted_str() {
        let out = fixed(vec![
            Stmt::Let {
                name: "s".into(),
                mutable: false,
                ty: Some(Type::parse("&str")),
                init: Some(Expr::Str("abc".into())),
            },
            temp(
                "n",
                "u64",
                Expr::Unsafe(Box::new(Block {
                    stmts: vec![],
                    tail: Some(Box::new(call(
                        "strlen",
                        vec![Expr::Cast {
                            expr: Box::new(Expr::MethodCall {
                                recv: Box::new(var("s")),
                                method: "as_ptr".into(),
                                args: vec![],
                            }),
                            ty: Type::parse("*mut i8"),
                        }],
                    ))),
                })),
            ),
        ]);

        assert!(out.contains("let n: usize = s.len();"));
        assert!(!out.contains("strlen("));
    }

    #[test]
    fn rewrites_strcmp_temp_comparison() {
        let out = fixed(vec![
            Stmt::Let {
                name: "a".into(),
                mutable: false,
                ty: Some(Type::parse("&str")),
                init: Some(Expr::Str("a".into())),
            },
            Stmt::Let {
                name: "b".into(),
                mutable: false,
                ty: Some(Type::parse("&str")),
                init: Some(Expr::Str("b".into())),
            },
            temp(
                "c",
                "i32",
                Expr::Unsafe(Box::new(Block {
                    stmts: vec![],
                    tail: Some(Box::new(call(
                        "strcmp",
                        vec![
                            Expr::Cast {
                                expr: Box::new(Expr::MethodCall {
                                    recv: Box::new(var("a")),
                                    method: "as_ptr".into(),
                                    args: vec![],
                                }),
                                ty: Type::parse("*mut i8"),
                            },
                            Expr::Cast {
                                expr: Box::new(Expr::MethodCall {
                                    recv: Box::new(var("b")),
                                    method: "as_ptr".into(),
                                    args: vec![],
                                }),
                                ty: Type::parse("*mut i8"),
                            },
                        ],
                    ))),
                })),
            ),
            temp("lt", "bool", bin(BinOp::Lt, var("c"), int(0))),
        ]);

        assert!(out.contains("let lt: bool = a.cmp(b) == std::cmp::Ordering::Less;"));
        assert!(!out.contains("strcmp("));
        assert!(!out.contains("let c:"));
    }
}
