use crate::rust_ast::{Block, Expr, IndentStmt, Stmt};

pub(in crate::fixups) fn body_exprs(body: &[IndentStmt], f: &mut impl FnMut(&Expr)) {
    for stmt in body {
        stmt_exprs(&stmt.stmt, f);
    }
}

pub(in crate::fixups) fn block_exprs(block: &Block, f: &mut impl FnMut(&Expr)) {
    body_exprs(&block.stmts, f);
    if let Some(tail) = &block.tail {
        exprs(tail, f);
    }
}

pub(in crate::fixups) fn stmt_exprs(stmt: &Stmt, f: &mut impl FnMut(&Expr)) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(expr) = init {
                exprs(expr, f);
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
            exprs(cond, f);
            body_exprs(then_body, f);
            exprs(then_value, f);
            body_exprs(else_body, f);
            exprs(else_value, f);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            exprs(target, f);
            exprs(value, f);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => exprs(expr, f),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            exprs(cond, f);
            body_exprs(then_body, f);
            body_exprs(else_body, f);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_exprs(body, f);
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_exprs(body, f);
        }
        Stmt::Match { expr, arms } => {
            exprs(expr, f);
            for arm in arms {
                body_exprs(&arm.body, f);
            }
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

pub(in crate::fixups) fn exprs(expr: &Expr, f: &mut impl FnMut(&Expr)) {
    f(expr);
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => {}
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => exprs(expr, f),
        Expr::Binary { lhs, rhs, .. } => {
            exprs(lhs, f);
            exprs(rhs, f);
        }
        Expr::Call { func, args } => {
            exprs(func, f);
            for arg in args {
                exprs(arg, f);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            exprs(recv, f);
            for arg in args {
                exprs(arg, f);
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => exprs(base, f),
        Expr::Index { base, index } => {
            exprs(base, f);
            exprs(index, f);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                exprs(value, f);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                exprs(elem, f);
            }
        }
        Expr::ArrayRepeat { elem, .. } => exprs(elem, f),
        Expr::Macro { args, .. } => {
            for arg in args {
                exprs(arg, f);
            }
        }
        Expr::Closure { body, .. } => exprs(body, f),
        Expr::Match { expr, arms } => {
            exprs(expr, f);
            for arm in arms {
                exprs(&arm.value, f);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            exprs(cond, f);
            exprs(then_expr, f);
            exprs(else_expr, f);
        }
        Expr::Block(block) | Expr::Unsafe(block) => block_exprs(block, f),
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => exprs(ptr, f),
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            exprs(ptr, f);
            exprs(value, f);
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            exprs(ptr, f);
            exprs(expected, f);
            exprs(desired, f);
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            exprs(src, f);
            exprs(dst, f);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            exprs(src, f);
            exprs(dst, f);
            exprs(count, f);
        }
        Expr::WriteBytes { dst, val, count } => {
            exprs(dst, f);
            exprs(val, f);
            exprs(count, f);
        }
    }
}

pub(in crate::fixups) fn body_exprs_any(
    body: &[IndentStmt],
    pred: &mut impl FnMut(&Expr) -> bool,
) -> bool {
    body.iter().any(|stmt| stmt_exprs_any(&stmt.stmt, pred))
}

pub(in crate::fixups) fn block_exprs_any(
    block: &Block,
    pred: &mut impl FnMut(&Expr) -> bool,
) -> bool {
    body_exprs_any(&block.stmts, pred)
        || block
            .tail
            .as_deref()
            .is_some_and(|tail| exprs_any(tail, pred))
}

pub(in crate::fixups) fn stmt_exprs_any(stmt: &Stmt, pred: &mut impl FnMut(&Expr) -> bool) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init.as_ref().is_some_and(|expr| exprs_any(expr, pred)),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            exprs_any(cond, pred)
                || body_exprs_any(then_body, pred)
                || exprs_any(then_value, pred)
                || body_exprs_any(else_body, pred)
                || exprs_any(else_value, pred)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            exprs_any(target, pred) || exprs_any(value, pred)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => exprs_any(expr, pred),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            exprs_any(cond, pred)
                || body_exprs_any(then_body, pred)
                || body_exprs_any(else_body, pred)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_exprs_any(body, pred)
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_exprs_any(body, pred)
        }
        Stmt::Match { expr, arms } => {
            exprs_any(expr, pred) || arms.iter().any(|arm| body_exprs_any(&arm.body, pred))
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}

pub(in crate::fixups) fn exprs_any(expr: &Expr, pred: &mut impl FnMut(&Expr) -> bool) -> bool {
    if pred(expr) {
        return true;
    }
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => false,
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => exprs_any(expr, pred),
        Expr::Binary { lhs, rhs, .. } => exprs_any(lhs, pred) || exprs_any(rhs, pred),
        Expr::Call { func, args } => {
            exprs_any(func, pred) || args.iter().any(|arg| exprs_any(arg, pred))
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            exprs_any(recv, pred) || args.iter().any(|arg| exprs_any(arg, pred))
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => exprs_any(base, pred),
        Expr::Index { base, index } => exprs_any(base, pred) || exprs_any(index, pred),
        Expr::StructLit { fields, .. } => fields.iter().any(|(_, value)| exprs_any(value, pred)),
        Expr::ArrayLit(elems) => elems.iter().any(|elem| exprs_any(elem, pred)),
        Expr::ArrayRepeat { elem, .. } => exprs_any(elem, pred),
        Expr::Macro { args, .. } => args.iter().any(|arg| exprs_any(arg, pred)),
        Expr::Closure { body, .. } => exprs_any(body, pred),
        Expr::Match { expr, arms } => {
            exprs_any(expr, pred) || arms.iter().any(|arm| exprs_any(&arm.value, pred))
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => exprs_any(cond, pred) || exprs_any(then_expr, pred) || exprs_any(else_expr, pred),
        Expr::Block(block) | Expr::Unsafe(block) => block_exprs_any(block, pred),
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => exprs_any(ptr, pred),
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => exprs_any(ptr, pred) || exprs_any(value, pred),
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => exprs_any(ptr, pred) || exprs_any(expected, pred) || exprs_any(desired, pred),
        Expr::CopyNonoverlapping { src, dst, .. } => exprs_any(src, pred) || exprs_any(dst, pred),
        Expr::PtrCopy {
            src, dst, count, ..
        } => exprs_any(src, pred) || exprs_any(dst, pred) || exprs_any(count, pred),
        Expr::WriteBytes { dst, val, count } => {
            exprs_any(dst, pred) || exprs_any(val, pred) || exprs_any(count, pred)
        }
    }
}

pub(in crate::fixups) fn body_exprs_all(
    body: &[IndentStmt],
    pred: &mut impl FnMut(&Expr) -> bool,
) -> bool {
    !body_exprs_any(body, &mut |expr| !pred(expr))
}

pub(in crate::fixups) fn block_exprs_all(
    block: &Block,
    pred: &mut impl FnMut(&Expr) -> bool,
) -> bool {
    !block_exprs_any(block, &mut |expr| !pred(expr))
}

pub(in crate::fixups) fn stmt_exprs_all(stmt: &Stmt, pred: &mut impl FnMut(&Expr) -> bool) -> bool {
    !stmt_exprs_any(stmt, &mut |expr| !pred(expr))
}

pub(in crate::fixups) fn exprs_all(expr: &Expr, pred: &mut impl FnMut(&Expr) -> bool) -> bool {
    !exprs_any(expr, &mut |expr| !pred(expr))
}

pub(in crate::fixups) fn body_exprs_all_with(
    body: &[IndentStmt],
    stmt_hook: &mut impl FnMut(&Stmt) -> Option<bool>,
    expr_hook: &mut impl FnMut(&Expr) -> Option<bool>,
) -> bool {
    body.iter()
        .all(|stmt| stmt_exprs_all_with(&stmt.stmt, stmt_hook, expr_hook))
}

pub(in crate::fixups) fn block_exprs_all_with(
    block: &Block,
    stmt_hook: &mut impl FnMut(&Stmt) -> Option<bool>,
    expr_hook: &mut impl FnMut(&Expr) -> Option<bool>,
) -> bool {
    body_exprs_all_with(&block.stmts, stmt_hook, expr_hook)
        && block
            .tail
            .as_deref()
            .is_none_or(|tail| exprs_all_with_hooks(tail, stmt_hook, expr_hook))
}

pub(in crate::fixups) fn stmt_exprs_all_with(
    stmt: &Stmt,
    stmt_hook: &mut impl FnMut(&Stmt) -> Option<bool>,
    expr_hook: &mut impl FnMut(&Expr) -> Option<bool>,
) -> bool {
    if let Some(result) = stmt_hook(stmt) {
        return result;
    }
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_none_or(|expr| exprs_all_with_hooks(expr, stmt_hook, expr_hook)),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            exprs_all_with_hooks(cond, stmt_hook, expr_hook)
                && body_exprs_all_with(then_body, stmt_hook, expr_hook)
                && exprs_all_with_hooks(then_value, stmt_hook, expr_hook)
                && body_exprs_all_with(else_body, stmt_hook, expr_hook)
                && exprs_all_with_hooks(else_value, stmt_hook, expr_hook)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            exprs_all_with_hooks(target, stmt_hook, expr_hook)
                && exprs_all_with_hooks(value, stmt_hook, expr_hook)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            exprs_all_with_hooks(expr, stmt_hook, expr_hook)
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            exprs_all_with_hooks(cond, stmt_hook, expr_hook)
                && body_exprs_all_with(then_body, stmt_hook, expr_hook)
                && body_exprs_all_with(else_body, stmt_hook, expr_hook)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_exprs_all_with(body, stmt_hook, expr_hook)
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_exprs_all_with(body, stmt_hook, expr_hook)
        }
        Stmt::Match { expr, arms } => {
            exprs_all_with_hooks(expr, stmt_hook, expr_hook)
                && arms
                    .iter()
                    .all(|arm| body_exprs_all_with(&arm.body, stmt_hook, expr_hook))
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => true,
    }
}

pub(in crate::fixups) fn exprs_all_with(
    expr: &Expr,
    hook: &mut impl FnMut(&Expr) -> Option<bool>,
) -> bool {
    exprs_all_with_hooks(expr, &mut |_| None, hook)
}

fn exprs_all_with_hooks(
    expr: &Expr,
    stmt_hook: &mut impl FnMut(&Stmt) -> Option<bool>,
    expr_hook: &mut impl FnMut(&Expr) -> Option<bool>,
) -> bool {
    if let Some(result) = expr_hook(expr) {
        return result;
    }
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => true,
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => exprs_all_with_hooks(expr, stmt_hook, expr_hook),
        Expr::Binary { lhs, rhs, .. } => {
            exprs_all_with_hooks(lhs, stmt_hook, expr_hook)
                && exprs_all_with_hooks(rhs, stmt_hook, expr_hook)
        }
        Expr::Call { func, args } => {
            exprs_all_with_hooks(func, stmt_hook, expr_hook)
                && args
                    .iter()
                    .all(|arg| exprs_all_with_hooks(arg, stmt_hook, expr_hook))
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            exprs_all_with_hooks(recv, stmt_hook, expr_hook)
                && args
                    .iter()
                    .all(|arg| exprs_all_with_hooks(arg, stmt_hook, expr_hook))
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => exprs_all_with_hooks(base, stmt_hook, expr_hook),
        Expr::Index { base, index } => {
            exprs_all_with_hooks(base, stmt_hook, expr_hook)
                && exprs_all_with_hooks(index, stmt_hook, expr_hook)
        }
        Expr::StructLit { fields, .. } => fields
            .iter()
            .all(|(_, value)| exprs_all_with_hooks(value, stmt_hook, expr_hook)),
        Expr::ArrayLit(elems) => elems
            .iter()
            .all(|elem| exprs_all_with_hooks(elem, stmt_hook, expr_hook)),
        Expr::ArrayRepeat { elem, .. } => exprs_all_with_hooks(elem, stmt_hook, expr_hook),
        Expr::Macro { args, .. } => args
            .iter()
            .all(|arg| exprs_all_with_hooks(arg, stmt_hook, expr_hook)),
        Expr::Closure { body, .. } => exprs_all_with_hooks(body, stmt_hook, expr_hook),
        Expr::Match { expr, arms } => {
            exprs_all_with_hooks(expr, stmt_hook, expr_hook)
                && arms
                    .iter()
                    .all(|arm| exprs_all_with_hooks(&arm.value, stmt_hook, expr_hook))
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            exprs_all_with_hooks(cond, stmt_hook, expr_hook)
                && exprs_all_with_hooks(then_expr, stmt_hook, expr_hook)
                && exprs_all_with_hooks(else_expr, stmt_hook, expr_hook)
        }
        Expr::Block(block) | Expr::Unsafe(block) => {
            block_exprs_all_with(block, stmt_hook, expr_hook)
        }
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
            exprs_all_with_hooks(ptr, stmt_hook, expr_hook)
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            exprs_all_with_hooks(ptr, stmt_hook, expr_hook)
                && exprs_all_with_hooks(value, stmt_hook, expr_hook)
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            exprs_all_with_hooks(ptr, stmt_hook, expr_hook)
                && exprs_all_with_hooks(expected, stmt_hook, expr_hook)
                && exprs_all_with_hooks(desired, stmt_hook, expr_hook)
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            exprs_all_with_hooks(src, stmt_hook, expr_hook)
                && exprs_all_with_hooks(dst, stmt_hook, expr_hook)
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            exprs_all_with_hooks(src, stmt_hook, expr_hook)
                && exprs_all_with_hooks(dst, stmt_hook, expr_hook)
                && exprs_all_with_hooks(count, stmt_hook, expr_hook)
        }
        Expr::WriteBytes { dst, val, count } => {
            exprs_all_with_hooks(dst, stmt_hook, expr_hook)
                && exprs_all_with_hooks(val, stmt_hook, expr_hook)
                && exprs_all_with_hooks(count, stmt_hook, expr_hook)
        }
    }
}

pub(in crate::fixups) fn body_exprs_mut_with(
    body: &mut [IndentStmt],
    f: &mut impl FnMut(&mut Expr) -> bool,
) {
    for stmt in body {
        stmt_exprs_mut_with(&mut stmt.stmt, f);
    }
}

pub(in crate::fixups) fn block_exprs_mut_with(
    block: &mut Block,
    f: &mut impl FnMut(&mut Expr) -> bool,
) {
    body_exprs_mut_with(&mut block.stmts, f);
    if let Some(tail) = &mut block.tail {
        exprs_mut_with(tail, f);
    }
}

pub(in crate::fixups) fn stmt_exprs_mut_with(
    stmt: &mut Stmt,
    f: &mut impl FnMut(&mut Expr) -> bool,
) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(expr) = init {
                exprs_mut_with(expr, f);
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
            exprs_mut_with(cond, f);
            body_exprs_mut_with(then_body, f);
            exprs_mut_with(then_value, f);
            body_exprs_mut_with(else_body, f);
            exprs_mut_with(else_value, f);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            exprs_mut_with(target, f);
            exprs_mut_with(value, f);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => exprs_mut_with(expr, f),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            exprs_mut_with(cond, f);
            body_exprs_mut_with(then_body, f);
            body_exprs_mut_with(else_body, f);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_exprs_mut_with(body, f);
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_exprs_mut_with(body, f);
        }
        Stmt::Match { expr, arms } => {
            exprs_mut_with(expr, f);
            for arm in arms {
                body_exprs_mut_with(&mut arm.body, f);
            }
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

pub(in crate::fixups) fn exprs_mut_with(expr: &mut Expr, f: &mut impl FnMut(&mut Expr) -> bool) {
    if !f(expr) {
        return;
    }
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => {}
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => exprs_mut_with(expr, f),
        Expr::Binary { lhs, rhs, .. } => {
            exprs_mut_with(lhs, f);
            exprs_mut_with(rhs, f);
        }
        Expr::Call { func, args } => {
            exprs_mut_with(func, f);
            for arg in args {
                exprs_mut_with(arg, f);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            exprs_mut_with(recv, f);
            for arg in args {
                exprs_mut_with(arg, f);
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => exprs_mut_with(base, f),
        Expr::Index { base, index } => {
            exprs_mut_with(base, f);
            exprs_mut_with(index, f);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                exprs_mut_with(value, f);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                exprs_mut_with(elem, f);
            }
        }
        Expr::ArrayRepeat { elem, .. } => exprs_mut_with(elem, f),
        Expr::Macro { args, .. } => {
            for arg in args {
                exprs_mut_with(arg, f);
            }
        }
        Expr::Closure { body, .. } => exprs_mut_with(body, f),
        Expr::Match { expr, arms } => {
            exprs_mut_with(expr, f);
            for arm in arms {
                exprs_mut_with(&mut arm.value, f);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            exprs_mut_with(cond, f);
            exprs_mut_with(then_expr, f);
            exprs_mut_with(else_expr, f);
        }
        Expr::Block(block) | Expr::Unsafe(block) => block_exprs_mut_with(block, f),
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => exprs_mut_with(ptr, f),
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            exprs_mut_with(ptr, f);
            exprs_mut_with(value, f);
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            exprs_mut_with(ptr, f);
            exprs_mut_with(expected, f);
            exprs_mut_with(desired, f);
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            exprs_mut_with(src, f);
            exprs_mut_with(dst, f);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            exprs_mut_with(src, f);
            exprs_mut_with(dst, f);
            exprs_mut_with(count, f);
        }
        Expr::WriteBytes { dst, val, count } => {
            exprs_mut_with(dst, f);
            exprs_mut_with(val, f);
            exprs_mut_with(count, f);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Expr, IndentStmt, Stmt};

    #[test]
    fn stmt_exprs_any_reaches_nested_block_tail() {
        let stmt = Stmt::Expr(Expr::Block(Box::new(Block {
            stmts: vec![IndentStmt {
                depth: 1,
                stmt: Stmt::Expr(bin(BinOp::Add, var("a"), int(1))),
            }],
            tail: Some(Box::new(var("needle"))),
        })));

        assert!(stmt_exprs_any(&stmt, &mut |expr| {
            matches!(expr, Expr::Var(name) if name.as_str() == "needle")
        }));
    }

    #[test]
    fn stmt_exprs_all_checks_every_expression() {
        let stmt = assign("x", bin(BinOp::Add, var("a"), int(1)));

        assert!(!stmt_exprs_all(&stmt, &mut |expr| {
            !matches!(expr, Expr::Var(name) if name.as_str() == "a")
        }));
    }

    #[test]
    fn exprs_all_with_uses_default_recursion_when_hook_declines() {
        let expr = bin(BinOp::Add, var("ok"), var("bad"));

        assert!(!exprs_all_with(&expr, &mut |expr| match expr {
            Expr::Var(name) if name.as_str() == "bad" => Some(false),
            _ => None,
        }));
    }

    #[test]
    fn exprs_all_with_can_accept_subtree_without_descending() {
        let expr = call("allowed", vec![var("bad")]);

        assert!(exprs_all_with(&expr, &mut |expr| match expr {
            Expr::Call { func, .. } if matches!(&**func, Expr::Var(name) if name.as_str() == "allowed") =>
            {
                Some(true)
            }
            Expr::Var(name) if name.as_str() == "bad" => Some(false),
            _ => None,
        }));
    }

    #[test]
    fn stmt_exprs_all_with_can_override_statement_policy() {
        let stmt = assign("target", var("bad"));

        assert!(stmt_exprs_all_with(
            &stmt,
            &mut |stmt| match stmt {
                Stmt::Assign { target, value } if matches!(target, Expr::Var(name) if name.as_str() == "target") =>
                {
                    Some(exprs_all_with(value, &mut |expr| match expr {
                        Expr::Var(name) if name.as_str() == "bad" => Some(true),
                        _ => None,
                    }))
                }
                _ => None,
            },
            &mut |expr| match expr {
                Expr::Var(name) if name.as_str() == "bad" => Some(false),
                _ => None,
            },
        ));
    }

    #[test]
    fn stmt_exprs_all_with_rejects_from_statement_hook() {
        let stmt = assign("target", int(1));

        assert!(!stmt_exprs_all_with(
            &stmt,
            &mut |stmt| match stmt {
                Stmt::Assign { target, .. } if matches!(target, Expr::Var(name) if name.as_str() == "target") =>
                {
                    Some(false)
                }
                _ => None,
            },
            &mut |_| None,
        ));
    }

    #[test]
    fn stmt_exprs_all_with_applies_statement_hook_inside_expression_blocks() {
        let stmt = Stmt::Expr(Expr::Block(Box::new(Block {
            stmts: vec![IndentStmt {
                depth: 1,
                stmt: assign("target", int(1)),
            }],
            tail: None,
        })));

        assert!(!stmt_exprs_all_with(
            &stmt,
            &mut |stmt| match stmt {
                Stmt::Assign { target, .. } if matches!(target, Expr::Var(name) if name.as_str() == "target") =>
                {
                    Some(false)
                }
                _ => None,
            },
            &mut |_| None,
        ));
    }

    #[test]
    fn stmt_exprs_mut_with_rewrites_nested_expression_blocks() {
        let mut stmt = Stmt::Expr(Expr::Block(Box::new(Block {
            stmts: vec![IndentStmt {
                depth: 1,
                stmt: Stmt::Expr(var("old")),
            }],
            tail: Some(Box::new(var("old"))),
        })));

        stmt_exprs_mut_with(&mut stmt, &mut |expr| {
            if matches!(expr, Expr::Var(name) if name.as_str() == "old") {
                *expr = var("new");
            }
            true
        });

        assert!(!stmt_exprs_any(&stmt, &mut |expr| {
            matches!(expr, Expr::Var(name) if name.as_str() == "old")
        }));
        assert!(stmt_exprs_any(&stmt, &mut |expr| {
            matches!(expr, Expr::Var(name) if name.as_str() == "new")
        }));
    }

    #[test]
    fn exprs_mut_with_can_skip_replaced_subtree() {
        let mut expr = call("wrap", vec![var("old")]);

        exprs_mut_with(&mut expr, &mut |expr| {
            if matches!(expr, Expr::Call { .. }) {
                *expr = var("new");
                return false;
            }
            if matches!(expr, Expr::Var(name) if name.as_str() == "new") {
                *expr = var("visited");
            }
            true
        });

        assert!(matches!(expr, Expr::Var(name) if name.as_str() == "new"));
    }
}
