use std::collections::BTreeMap;

use crate::backend::interproc::pointer_lattice::{PointerBinding, PointerFact, ResolvedPtrType};
use crate::backend::rust_ast::{
    BinOp, Block, Expr, FnDef, Ident, IndentStmt, Item, Path, Prim, Program, Stmt, Type, UnaryOp,
};
use crate::function_identity::CallBinding;

pub(in crate::backend) struct Pairing {
    len_param: String,
    idx_name: String,
}

pub(in crate::backend) fn compute(
    program: &Program,
    facts: &BTreeMap<PointerBinding, PointerFact>,
) -> BTreeMap<PointerBinding, Pairing> {
    let mut out = BTreeMap::new();
    collect_items(&program.items, facts, &mut out);
    out
}

pub(in crate::backend) fn apply(
    program: &mut Program,
    facts: &BTreeMap<PointerBinding, PointerFact>,
) {
    let pairings = compute(program, facts);
    if pairings.is_empty() {
        return;
    }
    apply_items(&mut program.items, &pairings);
}

fn collect_items(
    items: &[Item],
    facts: &BTreeMap<PointerBinding, PointerFact>,
    out: &mut BTreeMap<PointerBinding, Pairing>,
) {
    for item in items {
        match item {
            Item::Fn(f) => collect_fn(f, facts, out),
            Item::InlineMod { items, .. } => collect_items(items, facts, out),
            _ => {}
        }
    }
}

fn apply_items(items: &mut [Item], pairings: &BTreeMap<PointerBinding, Pairing>) {
    for item in items {
        match item {
            Item::Fn(f) => apply_fn(f, pairings),
            Item::InlineMod { items, .. } => apply_items(items, pairings),
            _ => {}
        }
    }
}

fn is_integer_type(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Prim(
            Prim::I8
                | Prim::I16
                | Prim::I32
                | Prim::I64
                | Prim::I128
                | Prim::Isize
                | Prim::U8
                | Prim::U16
                | Prim::U32
                | Prim::U64
                | Prim::U128
                | Prim::Usize
        )
    )
}

fn collect_fn(
    f: &FnDef,
    facts: &BTreeMap<PointerBinding, PointerFact>,
    out: &mut BTreeMap<PointerBinding, Pairing>,
) {
    let local_name_of = |param: &str| -> String {
        shadow_local_name(&f.body, param).unwrap_or_else(|| param.to_string())
    };

    let buffer_ptrs: Vec<(String, String)> = f
        .params
        .iter()
        .filter(|p| matches!(p.ty, Type::Ptr { .. }))
        .filter(|p| {
            let binding = PointerBinding {
                function: f.name.clone(),
                name: p.name.clone(),
            };
            facts
                .get(&binding)
                .is_some_and(|fact| fact.resolved().base == ResolvedPtrType::Slice)
        })
        .map(|p| (p.name.clone(), local_name_of(&p.name)))
        .collect();
    if buffer_ptrs.is_empty() {
        return;
    }
    let int_params: Vec<(String, String)> = f
        .params
        .iter()
        .filter(|p| is_integer_type(&p.ty))
        .map(|p| (p.name.clone(), local_name_of(&p.name)))
        .collect();
    if int_params.is_empty() {
        return;
    }

    let let_exprs = collect_let_exprs(&f.body);
    let mut bound_pairs: Vec<(String, String)> = Vec::new();
    collect_loop_bounds(&f.body, &let_exprs, &mut bound_pairs);

    let mut offsets = Vec::new();
    for indent in &f.body {
        indent.stmt.collect_offset_calls(&mut offsets);
    }

    if bound_pairs.is_empty() {
        return;
    }

    for (ptr_param, ptr_local) in &buffer_ptrs {
        for (len_param, len_local) in &int_params {
            if ptr_param == len_param {
                continue;
            }
            let matched_idx = bound_pairs.iter().find_map(|(idx, len)| {
                let matches_len = len == len_local;
                let matches_offset = offsets.iter().any(|(recv, _, index)| {
                    resolve_var(&let_exprs, recv.as_str()) == ptr_local.as_str()
                        && peeled_var(index).map(|v| resolve_var(&let_exprs, v))
                            == Some(idx.as_str())
                });
                (matches_len && matches_offset).then(|| idx.clone())
            });
            let Some(idx_name) = matched_idx else {
                continue;
            };
            let guard_reads = bound_pairs
                .iter()
                .filter(|(_, len)| len == len_local)
                .count();
            if count_var_reads(&f.body, len_local) != guard_reads {
                continue;
            }
            if assigned_more_than_once(&f.body, len_local)
                || assigned_more_than_once(&f.body, ptr_local)
            {
                continue;
            }
            let temp_names = matched_offset_temps(&f.body, &let_exprs, ptr_local, &idx_name);
            if temp_names.is_empty() || is_written_through(&f.body, &temp_names) {
                continue;
            }
            out.insert(
                PointerBinding {
                    function: f.name.clone(),
                    name: ptr_param.clone(),
                },
                Pairing {
                    len_param: len_param.clone(),
                    idx_name,
                },
            );
        }
    }
}

fn shadow_local_name(body: &[IndentStmt], param: &str) -> Option<String> {
    body.iter().find_map(|indent| match &indent.stmt {
        Stmt::Assign {
            target: Expr::Var(t),
            value: Expr::Var(v),
        } if v.as_str() == param => Some(t.as_str().to_string()),
        _ => None,
    })
}

fn apply_fn(f: &mut FnDef, pairings: &BTreeMap<PointerBinding, Pairing>) {
    for (ptr_name, elem_ty) in f
        .params
        .iter()
        .filter_map(|p| match &p.ty {
            Type::Ptr { inner, .. } => Some((p.name.clone(), (**inner).clone())),
            _ => None,
        })
        .collect::<Vec<_>>()
    {
        let binding = PointerBinding {
            function: f.name.clone(),
            name: ptr_name.clone(),
        };
        let Some(pairing) = pairings.get(&binding) else {
            continue;
        };
        apply_view(f, &ptr_name, &elem_ty, pairing);
    }
}

fn matched_offset_temps(
    body: &[IndentStmt],
    let_exprs: &BTreeMap<String, Expr>,
    ptr_local: &str,
    idx_name: &str,
) -> Vec<String> {
    let mut offset_lets = Vec::new();
    collect_offset_lets(body, &mut offset_lets);
    offset_lets
        .iter()
        .filter(|(_, recv, idx)| {
            resolve_var(let_exprs, recv.as_str()) == ptr_local
                && peeled_var(idx).map(|v| resolve_var(let_exprs, v)) == Some(idx_name)
        })
        .map(|(temp, ..)| temp.clone())
        .collect()
}

fn is_written_through(body: &[IndentStmt], temp_names: &[String]) -> bool {
    body.iter()
        .any(|indent| stmt_writes_temp(&indent.stmt, temp_names))
}

fn stmt_writes_temp(stmt: &Stmt, temp_names: &[String]) -> bool {
    let is_temp_deref = |target: &Expr| matches!(peel_cast(target), Expr::Unary{op: UnaryOp::Deref, expr} if matches!(&**expr, Expr::Var(v) if temp_names.iter().any(|t| t == v.as_str())));
    match stmt {
        Stmt::Assign { target, .. } | Stmt::CompoundAssign { target, .. } => is_temp_deref(target),
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => is_written_through(then_body, temp_names) || is_written_through(else_body, temp_names),
        Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. }
        | Stmt::For { body, .. } => is_written_through(body, temp_names),
        Stmt::Match { arms, .. } => arms
            .iter()
            .any(|arm| is_written_through(&arm.body, temp_names)),
        Stmt::While { body, .. } | Stmt::Unsafe { body } => {
            is_written_through(&body.stmts, temp_names)
        }
        Stmt::Block(body) => is_written_through(&body.stmts, temp_names),
        _ => false,
    }
}

fn peel_cast(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_cast(expr),
        other => other,
    }
}

fn apply_view(f: &mut FnDef, ptr_name: &str, elem_ty: &Type, pairing: &Pairing) {
    let let_exprs = collect_let_exprs(&f.body);
    let ptr_local = shadow_local_name(&f.body, ptr_name).unwrap_or_else(|| ptr_name.to_string());
    let temp_names = matched_offset_temps(&f.body, &let_exprs, &ptr_local, &pairing.idx_name);
    if temp_names.is_empty() {
        return;
    }

    let view_name = format!("__{ptr_name}_view");
    let insert_at = 0;
    let depth = f.body.first().map(|indent| indent.depth).unwrap_or(0);

    let view_let = IndentStmt {
        depth,
        stmt: Stmt::Let {
            name: view_name.clone(),
            mutable: false,
            ty: None,
            init: Some(Expr::Unsafe(Box::new(Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::Call {
                    binding: CallBinding::Generated,
                    func: Box::new(Expr::Path(Path::new(
                        ["std", "slice", "from_raw_parts"].map(Ident::from),
                    ))),
                    args: vec![
                        Expr::Cast {
                            expr: Box::new(Expr::Var(Ident::new(ptr_name))),
                            ty: Type::Ptr {
                                mutable: false,
                                inner: Box::new(elem_ty.clone()),
                            },
                        },
                        Expr::Cast {
                            expr: Box::new(Expr::Var(Ident::new(pairing.len_param.as_str()))),
                            ty: Type::Prim(Prim::Usize),
                        },
                    ],
                })),
            }))),
        },
    };
    f.body.insert(insert_at, view_let);

    for indent in &mut f.body {
        rewrite_stmt(&mut indent.stmt, &temp_names, &view_name, &pairing.idx_name);
    }
}

fn rewrite_stmt(stmt: &mut Stmt, temp_names: &[String], view_name: &str, idx_name: &str) {
    match stmt {
        Stmt::Let {
            init: Some(init), ..
        } => rewrite_expr(init, temp_names, view_name, idx_name),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            rewrite_expr(target, temp_names, view_name, idx_name);
            rewrite_expr(value, temp_names, view_name, idx_name);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            rewrite_expr(expr, temp_names, view_name, idx_name);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            rewrite_expr(cond, temp_names, view_name, idx_name);
            for indent in then_body.iter_mut().chain(else_body.iter_mut()) {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
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
            rewrite_expr(cond, temp_names, view_name, idx_name);
            for indent in then_body.iter_mut() {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
            }
            rewrite_expr(then_value, temp_names, view_name, idx_name);
            for indent in else_body.iter_mut() {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
            }
            rewrite_expr(else_value, temp_names, view_name, idx_name);
        }
        Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. }
        | Stmt::For { body, .. } => {
            for indent in body {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
            }
        }
        Stmt::While { cond, body } => {
            rewrite_expr(cond, temp_names, view_name, idx_name);
            for indent in &mut body.stmts {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
            }
            if let Some(tail) = &mut body.tail {
                rewrite_expr(tail, temp_names, view_name, idx_name);
            }
        }
        Stmt::Unsafe { body } | Stmt::Block(body) => {
            let body: &mut Block = body;
            for indent in &mut body.stmts {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
            }
            if let Some(tail) = &mut body.tail {
                rewrite_expr(tail, temp_names, view_name, idx_name);
            }
        }
        Stmt::Match { expr, arms } => {
            rewrite_expr(expr, temp_names, view_name, idx_name);
            for arm in arms {
                for indent in &mut arm.body {
                    rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
                }
            }
        }
        _ => {}
    }
}

fn rewrite_expr(expr: &mut Expr, temp_names: &[String], view_name: &str, idx_name: &str) {
    if let Expr::Unary {
        op: UnaryOp::Deref,
        expr: inner,
    } = expr
        && let Some(t) = peeled_var(inner)
        && temp_names.iter().any(|name| name == t)
    {
        *expr = Expr::Index {
            base: Box::new(Expr::Var(Ident::new(view_name))),
            index: Box::new(Expr::Cast {
                expr: Box::new(Expr::Var(Ident::new(idx_name))),
                ty: Type::Prim(Prim::Usize),
            }),
        };
        return;
    }
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. } => rewrite_expr(expr, temp_names, view_name, idx_name),
        Expr::Block(block) | Expr::Unsafe(block) => {
            for stmt in &mut block.stmts {
                rewrite_stmt(&mut stmt.stmt, temp_names, view_name, idx_name);
            }
            if let Some(tail) = &mut block.tail {
                rewrite_expr(tail, temp_names, view_name, idx_name);
            }
        }
        Expr::Binary { lhs, rhs, .. } => {
            rewrite_expr(lhs, temp_names, view_name, idx_name);
            rewrite_expr(rhs, temp_names, view_name, idx_name);
        }
        Expr::Call { func, args, .. } => {
            rewrite_expr(func, temp_names, view_name, idx_name);
            for arg in args {
                rewrite_expr(arg, temp_names, view_name, idx_name);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            rewrite_expr(recv, temp_names, view_name, idx_name);
            for arg in args {
                rewrite_expr(arg, temp_names, view_name, idx_name);
            }
        }
        Expr::Index { base, index } => {
            rewrite_expr(base, temp_names, view_name, idx_name);
            rewrite_expr(index, temp_names, view_name, idx_name);
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
            rewrite_expr(base, temp_names, view_name, idx_name);
        }
        _ => {}
    }
}

fn collect_loop_bounds(
    body: &[IndentStmt],
    let_exprs: &BTreeMap<String, Expr>,
    out: &mut Vec<(String, String)>,
) {
    for indent in body {
        collect_loop_bounds_stmt(&indent.stmt, let_exprs, out);
    }
}

fn collect_loop_bounds_stmt(
    stmt: &Stmt,
    let_exprs: &BTreeMap<String, Expr>,
    out: &mut Vec<(String, String)>,
) {
    match stmt {
        Stmt::Loop { body, .. } => {
            if let Some(pair) = loop_break_guard(body, let_exprs) {
                out.push(pair);
            }
            collect_loop_bounds(body, let_exprs, out);
        }
        Stmt::While { cond, body } => {
            if let Some(pair) = lt_pair(cond, let_exprs) {
                out.push(pair);
            }
            collect_loop_bounds(&body.stmts, let_exprs, out);
        }
        Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            collect_loop_bounds(then_body, let_exprs, out);
            collect_loop_bounds(else_body, let_exprs, out);
        }
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        } => {
            collect_loop_bounds(then_body, let_exprs, out);
            collect_loop_bounds(else_body, let_exprs, out);
        }
        Stmt::For { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            collect_loop_bounds(body, let_exprs, out);
        }
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_loop_bounds(&arm.body, let_exprs, out);
            }
        }
        Stmt::Unsafe { body } => collect_loop_bounds(&body.stmts, let_exprs, out),
        Stmt::Block(body) => collect_loop_bounds(&body.stmts, let_exprs, out),
        _ => {}
    }
}

fn loop_break_guard(
    body: &[IndentStmt],
    let_exprs: &BTreeMap<String, Expr>,
) -> Option<(String, String)> {
    body.iter().find_map(|indent| {
        let Stmt::If {
            cond,
            then_body,
            else_body,
        } = &indent.stmt
        else {
            return None;
        };
        if !else_body.is_empty() {
            return None;
        }
        let [only] = then_body.as_slice() else {
            return None;
        };
        if !matches!(only.stmt, Stmt::Break(None)) {
            return None;
        }
        let Expr::Unary {
            op: UnaryOp::Not,
            expr: inner,
        } = cond
        else {
            return None;
        };
        lt_pair(inner, let_exprs)
    })
}

fn lt_pair(expr: &Expr, let_exprs: &BTreeMap<String, Expr>) -> Option<(String, String)> {
    let resolved = resolve_full(expr, let_exprs, 0);
    let Expr::Binary {
        op: BinOp::Lt,
        lhs,
        rhs,
    } = &resolved
    else {
        return None;
    };
    let idx = peeled_var(lhs)?;
    let len = peeled_var(rhs)?;
    Some((
        resolve_var(let_exprs, idx).to_string(),
        resolve_var(let_exprs, len).to_string(),
    ))
}

fn peeled_var(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(v) => Some(v.as_str()),
        Expr::Cast { expr, .. } => peeled_var(expr),
        _ => None,
    }
}

fn count_var_reads(body: &[IndentStmt], name: &str) -> usize {
    body.iter()
        .map(|indent| stmt_read_count(&indent.stmt, name))
        .sum()
}

fn expr_read_count(expr: &Expr, name: &str) -> usize {
    let mut vars = Vec::new();
    expr.collect_vars(&mut vars);
    vars.iter().filter(|v| v.as_str() == name).count()
}

fn stmt_read_count(stmt: &Stmt, name: &str) -> usize {
    match stmt {
        Stmt::Let {
            init: Some(init), ..
        } => expr_read_count(init, name),
        Stmt::Let { init: None, .. } => 0,
        Stmt::Assign { value, .. } => expr_read_count(value, name),
        Stmt::CompoundAssign { target, value, .. } => {
            expr_read_count(target, name) + expr_read_count(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_read_count(expr, name),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) | Stmt::InlineAsm(_) => 0,
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_read_count(cond, name)
                + count_var_reads(then_body, name)
                + count_var_reads(else_body, name)
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_read_count(cond, name)
                + count_var_reads(then_body, name)
                + expr_read_count(then_value, name)
                + count_var_reads(else_body, name)
                + expr_read_count(else_value, name)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            count_var_reads(body, name)
        }
        Stmt::For { iter, body, .. } => expr_read_count(iter, name) + count_var_reads(body, name),
        Stmt::Match { expr, arms } => {
            expr_read_count(expr, name)
                + arms
                    .iter()
                    .map(|arm| count_var_reads(&arm.body, name))
                    .sum::<usize>()
        }
        Stmt::While { cond, body } => {
            expr_read_count(cond, name)
                + count_var_reads(&body.stmts, name)
                + body
                    .tail
                    .as_deref()
                    .map(|t| expr_read_count(t, name))
                    .unwrap_or(0)
        }
        Stmt::Unsafe { body } => {
            count_var_reads(&body.stmts, name)
                + body
                    .tail
                    .as_deref()
                    .map(|t| expr_read_count(t, name))
                    .unwrap_or(0)
        }
        Stmt::Block(body) => {
            count_var_reads(&body.stmts, name)
                + body
                    .tail
                    .as_deref()
                    .map(|t| expr_read_count(t, name))
                    .unwrap_or(0)
        }
    }
}

fn assigned_more_than_once(body: &[IndentStmt], name: &str) -> bool {
    count_assigns(body, name) > 1
}

fn count_assigns(body: &[IndentStmt], name: &str) -> usize {
    body.iter()
        .map(|indent| stmt_assign_count(&indent.stmt, name))
        .sum()
}

fn stmt_assign_count(stmt: &Stmt, name: &str) -> usize {
    match stmt {
        Stmt::Assign { target, .. } | Stmt::CompoundAssign { target, .. } => {
            usize::from(matches!(target, Expr::Var(v) if v.as_str() == name))
        }
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => count_assigns(then_body, name) + count_assigns(else_body, name),
        Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. }
        | Stmt::For { body, .. } => count_assigns(body, name),
        Stmt::Match { arms, .. } => arms.iter().map(|arm| count_assigns(&arm.body, name)).sum(),
        Stmt::While { body, .. } | Stmt::Unsafe { body } => count_assigns(&body.stmts, name),
        Stmt::Block(body) => count_assigns(&body.stmts, name),
        _ => 0,
    }
}

fn collect_let_exprs(body: &[IndentStmt]) -> BTreeMap<String, Expr> {
    let mut out = BTreeMap::new();
    collect_let_exprs_stmts(body, &mut out);
    out
}

fn collect_let_exprs_stmts(body: &[IndentStmt], out: &mut BTreeMap<String, Expr>) {
    for indent in body {
        collect_let_exprs_stmt(&indent.stmt, out);
    }
}

fn collect_let_exprs_stmt(stmt: &Stmt, out: &mut BTreeMap<String, Expr>) {
    if let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
    {
        out.insert(name.clone(), init.clone());
    }
    match stmt {
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            collect_let_exprs_stmts(then_body, out);
            collect_let_exprs_stmts(else_body, out);
        }
        Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. }
        | Stmt::For { body, .. } => collect_let_exprs_stmts(body, out),
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_let_exprs_stmts(&arm.body, out);
            }
        }
        Stmt::While { body, .. } | Stmt::Unsafe { body } => {
            collect_let_exprs_stmts(&body.stmts, out)
        }
        Stmt::Block(body) => collect_let_exprs_stmts(&body.stmts, out),
        _ => {}
    }
}

fn resolve_var<'a>(let_exprs: &'a BTreeMap<String, Expr>, name: &'a str) -> &'a str {
    let mut cur = name;
    for _ in 0..32 {
        match let_exprs.get(cur).and_then(peeled_var) {
            Some(next) if next != cur => cur = next,
            _ => break,
        }
    }
    cur
}

fn resolve_full(expr: &Expr, let_exprs: &BTreeMap<String, Expr>, depth: u32) -> Expr {
    if depth > 32 {
        return expr.clone();
    }
    if let Expr::Var(v) = expr
        && let Some(next) = let_exprs.get(v.as_str())
    {
        return resolve_full(next, let_exprs, depth + 1);
    }
    expr.clone()
}

fn collect_offset_lets(body: &[IndentStmt], out: &mut Vec<(String, String, Expr)>) {
    for indent in body {
        collect_offset_lets_stmt(&indent.stmt, out);
    }
}

fn collect_offset_lets_stmt(stmt: &Stmt, out: &mut Vec<(String, String, Expr)>) {
    if let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
        && let Some((recv, idx)) = peeled_offset_call(init)
    {
        out.push((name.clone(), recv.to_string(), idx.clone()));
    }
    match stmt {
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            collect_offset_lets(then_body, out);
            collect_offset_lets(else_body, out);
        }
        Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. }
        | Stmt::For { body, .. } => collect_offset_lets(body, out),
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_offset_lets(&arm.body, out);
            }
        }
        Stmt::While { body, .. } | Stmt::Unsafe { body } => collect_offset_lets(&body.stmts, out),
        Stmt::Block(body) => collect_offset_lets(&body.stmts, out),
        _ => {}
    }
}

fn peeled_offset_call(expr: &Expr) -> Option<(&str, &Expr)> {
    match expr {
        Expr::MethodCall { recv, method, args }
            if matches!(method.as_str(), "offset" | "add" | "wrapping_add") =>
        {
            let Expr::Var(r) = &**recv else {
                return None;
            };
            let [idx] = args.as_slice() else {
                return None;
            };
            Some((r.as_str(), idx))
        }
        Expr::Unsafe(block) | Expr::Block(block) => {
            block.tail.as_deref().and_then(peeled_offset_call)
        }
        Expr::Cast { expr, .. } => peeled_offset_call(expr),
        _ => None,
    }
}
