use crate::fixups::facts::{
    AstPath, CallCallee, FixupFacts, FunctionId, PathSegment, StringBufferFact, StringBufferKind,
    StringLibcFunction,
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
            fixup_body(&mut f.body, function, facts, &mut Vec::new());
        }
    }
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
    path: &mut Vec<PathSegment>,
) {
    let mut temps = BTreeMap::new();
    let mut remove = Vec::new();
    for i in 0..body.len() {
        let mut stmt_path = stmt_path(path, i);
        let candidate = match &body[i].stmt {
            Stmt::Let {
                name,
                ty: Some(Type::Prim(Prim::I32)),
                init: Some(init),
                ..
            } => supported_compare_call(init, function, facts, &stmt_path)
                .filter(|_| temp_uses_are_zero_comparisons(&body[i + 1..], name))
                .map(|compare| (name.clone(), compare)),
            _ => None,
        };
        if let Some((name, compare)) = candidate {
            temps.insert(name, compare);
            remove.push(i);
        } else {
            fixup_stmt(&mut body[i].stmt, function, facts, &temps, &mut stmt_path);
        }
    }
    for i in remove.into_iter().rev() {
        body.remove(i);
    }
}

pub(in crate::fixups) fn prune_unused_externs(program: &mut Program, facts: &FixupFacts) {
    let used = direct_calls(facts);
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

fn direct_calls(facts: &FixupFacts) -> Vec<String> {
    let mut calls = facts
        .callsites
        .iter()
        .filter_map(|callsite| match &callsite.callee {
            CallCallee::Direct { name, .. } => Some(name.clone()),
            CallCallee::Indirect => None,
        })
        .collect::<Vec<_>>();
    calls.sort();
    calls.dedup();
    calls
}

fn fixup_stmt(
    stmt: &mut Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    temps: &BTreeMap<String, Compare>,
    path: &mut Vec<PathSegment>,
) {
    match stmt {
        Stmt::Let { ty, init, .. } => {
            if let Some(init) = init {
                if let Some(source) = supported_strlen_call(function, facts, path) {
                    *init = strlen_replacement(source);
                    if matches!(ty, Some(Type::Prim(Prim::U64))) {
                        *ty = Some(Type::Prim(Prim::Usize));
                    }
                    return;
                }
                fixup_expr(init, function, facts, temps, path);
            }
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            fixup_expr(target, function, facts, temps, path);
            fixup_expr(value, function, facts, temps, path);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            fixup_expr(expr, function, facts, temps, path)
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            fixup_expr(cond, function, facts, temps, path);
            walk::with_path_segment(path, PathSegment::Then, |path| {
                fixup_body(then_body, function, facts, path);
            });
            walk::with_path_segment(path, PathSegment::Else, |path| {
                fixup_body(else_body, function, facts, path);
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
            fixup_expr(cond, function, facts, temps, path);
            walk::with_path_segment(path, PathSegment::Then, |path| {
                fixup_body(then_body, function, facts, path);
                fixup_expr(then_value, function, facts, temps, path);
            });
            walk::with_path_segment(path, PathSegment::Else, |path| {
                fixup_body(else_body, function, facts, path);
                fixup_expr(else_value, function, facts, temps, path);
            });
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            fixup_body(body, function, facts, path);
        }
        Stmt::While { cond, body } => {
            fixup_expr(cond, function, facts, temps, path);
            fixup_block(body, function, facts, path);
        }
        Stmt::Block(body) | Stmt::Unsafe { body } => fixup_block(body, function, facts, path),
        Stmt::Match { expr, arms } => {
            fixup_expr(expr, function, facts, temps, path);
            for (index, arm) in arms.iter_mut().enumerate() {
                walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                    fixup_body(&mut arm.body, function, facts, path);
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
    path: &mut Vec<PathSegment>,
) {
    fixup_body(&mut block.stmts, function, facts, path);
    if let Some(tail) = &mut block.tail {
        walk::with_path_segment(path, PathSegment::BlockTail, |path| {
            fixup_expr(tail, function, facts, &BTreeMap::new(), path);
        });
    }
}

fn fixup_expr(
    expr: &mut Expr,
    function: FunctionId,
    facts: &FixupFacts,
    temps: &BTreeMap<String, Compare>,
    path: &mut Vec<PathSegment>,
) {
    if let Some(replacement) = replacement_expr(expr, function, facts, temps, path) {
        *expr = replacement;
        return;
    }
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(expr, function, facts, temps, path)
            });
        }
        Expr::Binary { lhs, rhs, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(lhs, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                fixup_expr(rhs, function, facts, temps, path)
            });
        }
        Expr::Call { func, args } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(func, function, facts, temps, path)
            });
            for (index, arg) in args.iter_mut().enumerate() {
                walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    fixup_expr(arg, function, facts, temps, path)
                });
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(recv, function, facts, temps, path)
            });
            for (index, arg) in args.iter_mut().enumerate() {
                walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    fixup_expr(arg, function, facts, temps, path)
                });
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(base, function, facts, temps, path)
            });
        }
        Expr::Index { base, index } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(base, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                fixup_expr(index, function, facts, temps, path)
            });
        }
        Expr::StructLit { fields, .. } => {
            for (index, (_, value)) in fields.iter_mut().enumerate() {
                walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                    fixup_expr(value, function, facts, temps, path)
                });
            }
        }
        Expr::ArrayLit(elems) | Expr::Macro { args: elems, .. } => {
            for (index, elem) in elems.iter_mut().enumerate() {
                walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                    fixup_expr(elem, function, facts, temps, path)
                });
            }
        }
        Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(elem, function, facts, temps, path)
            });
        }
        Expr::Match { expr, arms } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(expr, function, facts, temps, path)
            });
            for (index, arm) in arms.iter_mut().enumerate() {
                walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    fixup_expr(&mut arm.value, function, facts, temps, path)
                });
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(cond, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                fixup_expr(then_expr, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                fixup_expr(else_expr, function, facts, temps, path)
            });
        }
        Expr::Block(block) => {
            walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                fixup_block(block, function, facts, path)
            });
        }
        Expr::Unsafe(block) => {
            walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                fixup_block(block, function, facts, path)
            });
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(src, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                fixup_expr(dst, function, facts, temps, path)
            });
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(src, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                fixup_expr(dst, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                fixup_expr(count, function, facts, temps, path)
            });
        }
        Expr::WriteBytes { dst, val, count } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(dst, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                fixup_expr(val, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                fixup_expr(count, function, facts, temps, path)
            });
        }
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(ptr, function, facts, temps, path)
            });
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(ptr, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                fixup_expr(value, function, facts, temps, path)
            });
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                fixup_expr(ptr, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                fixup_expr(expected, function, facts, temps, path)
            });
            walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                fixup_expr(desired, function, facts, temps, path)
            });
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
    function: FunctionId,
    facts: &FixupFacts,
    temps: &BTreeMap<String, Compare>,
    path: &mut Vec<PathSegment>,
) -> Option<Expr> {
    if let Some((compare, op)) = temp_zero_comparison(expr, temps) {
        return Some(compare_to_bool(compare, op));
    }
    if let Expr::Binary { op, lhs, rhs } = expr
        && op.is_comparison()
    {
        let mut lhs_path = path.to_vec();
        lhs_path.push(PathSegment::Expr(0));
        let mut rhs_path = path.to_vec();
        rhs_path.push(PathSegment::Expr(1));
        if let Some(compare) = supported_compare_call(lhs, function, facts, &lhs_path)
            && is_zero(rhs)
        {
            return Some(compare_to_bool(&compare, *op));
        }
        if let Some(compare) = supported_compare_call(rhs, function, facts, &rhs_path)
            && is_zero(lhs)
        {
            return Some(compare_to_bool(&compare, flip_comparison(*op)));
        }
    }
    if let Some(source) = supported_strlen_call(function, facts, path) {
        return Some(strlen_replacement(source));
    }
    supported_compare_call(expr, function, facts, path)
        .map(|compare| cmp_to_i32(compare_expr(compare)))
}

fn strlen_replacement(source: Source) -> Expr {
    Expr::MethodCall {
        recv: Box::new(Expr::Var(source.name.into())),
        method: "len".into(),
        args: Vec::new(),
    }
}

fn supported_strlen_call(
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<Source> {
    let usage = facts.string_libc_use(function, &AstPath(path.to_vec()))?;
    if usage.callee != StringLibcFunction::StrLen || usage.pointer_args.len() != 1 {
        return None;
    }
    source_for_binding(facts, usage.pointer_args[0])
}

fn supported_compare_call(
    expr: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<Compare> {
    let Expr::Call { func, args } = peel_empty_unsafe(expr) else {
        return None;
    };
    let Expr::Var(name) = &**func else {
        return None;
    };
    let usage = facts.string_libc_use(function, &AstPath(path.to_vec()))?;
    match (name.as_str(), usage.callee) {
        ("strcmp", StringLibcFunction::StrCmp)
            if args.len() == 2 && usage.pointer_args.len() == 2 =>
        {
            let lhs = source_for_binding(facts, usage.pointer_args[0])?;
            let rhs = source_for_binding(facts, usage.pointer_args[1])?;
            comparable(lhs, rhs).map(|(lhs, rhs)| Compare { lhs, rhs })
        }
        ("strncmp", StringLibcFunction::StrNCmp)
            if args.len() == 3 && usage.pointer_args.len() == 2 =>
        {
            let lhs = source_for_binding(facts, usage.pointer_args[0])?;
            let rhs = source_for_binding(facts, usage.pointer_args[1])?;
            Some(Compare {
                lhs: prefix(lhs, args[2].clone()),
                rhs: prefix(rhs, args[2].clone()),
            })
        }
        ("memcmp", StringLibcFunction::MemCmp)
            if args.len() == 3 && usage.pointer_args.len() == 2 =>
        {
            let lhs = source_for_binding(facts, usage.pointer_args[0])?;
            let rhs = source_for_binding(facts, usage.pointer_args[1])?;
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

fn source_for_binding(
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
) -> Option<Source> {
    let buffer = facts.string_buffer(binding)?;
    Some(Source {
        name: facts.binding_name(binding)?.to_owned(),
        kind: string_kind_for_buffer(buffer)?,
    })
}

fn string_kind_for_buffer(buffer: &StringBufferFact) -> Option<StringKind> {
    match buffer.kind {
        StringBufferKind::BorrowedStr | StringBufferKind::OwnedString => Some(StringKind::Str),
        StringBufferKind::BorrowedBytes => Some(StringKind::Bytes),
        StringBufferKind::CharArray => None,
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
