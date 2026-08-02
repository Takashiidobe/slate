use crate::fixups::facts::{AstPath, FixupFacts, FunctionId, PathSegment};
pub(in crate::fixups) use crate::fixups::support::walk::{
    nested_bodies_with_path, nested_body_vecs_with_path, with_path_segment,
};
use crate::rust_ast::{AsmOperand, Block, Expr, IndentStmt, InlineAsm, Item, Program, Stmt};

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
        Stmt::InlineAsm(asm) => {
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| exprs(expr, f));
            }
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
        Stmt::For { iter, body, .. } => {
            exprs(iter, f);
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
        | Expr::CStr(_)
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
        Expr::Range { start, end } => {
            exprs(start, f);
            exprs(end, f);
        }
        Expr::Call { func, args, .. } => {
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
        Expr::TupleStructLit { fields, .. } => {
            for value in fields {
                exprs(value, f);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                exprs(elem, f);
            }
        }
        Expr::ArrayRepeat { elem, .. } => exprs(elem, f),
        Expr::VecLit(elems) => {
            for elem in elems {
                exprs(elem, f);
            }
        }
        Expr::VecRepeat { elem, len } => {
            exprs(elem, f);
            exprs(len, f);
        }
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
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            if let Some(ptr) = place.ptr_expr() {
                exprs(ptr, f);
            }
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            if let Some(ptr) = place.ptr_expr() {
                exprs(ptr, f);
            }
            exprs(value, f);
        }
        Expr::AtomicNew { value, .. } => exprs(value, f),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            if let Some(ptr) = place.ptr_expr() {
                exprs(ptr, f);
            }
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

pub(in crate::fixups) fn body_exprs_with_path(
    body: &[IndentStmt],
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&Expr, &mut Vec<PathSegment>),
) {
    for (index, indent) in body.iter().enumerate() {
        with_path_segment(path, PathSegment::Stmt(index), |path| {
            stmt_exprs_with_path(&indent.stmt, path, f);
        });
    }
}

pub(in crate::fixups) fn block_exprs_with_path(
    block: &Block,
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&Expr, &mut Vec<PathSegment>),
) {
    body_exprs_with_path(&block.stmts, path, f);
    if let Some(tail) = &block.tail {
        with_path_segment(path, PathSegment::BlockTail, |path| {
            exprs_with_path(tail, path, f);
        });
    }
}

pub(in crate::fixups) fn stmt_exprs_with_path(
    stmt: &Stmt,
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&Expr, &mut Vec<PathSegment>),
) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(expr) = init {
                stmt_root_expr_with_path(expr, 0, path, f);
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
            stmt_root_expr_with_path(cond, 0, path, f);
            with_path_segment(path, PathSegment::Then, |path| {
                body_exprs_with_path(then_body, path, f);
                stmt_root_expr_with_path(then_value, 0, path, f);
            });
            with_path_segment(path, PathSegment::Else, |path| {
                body_exprs_with_path(else_body, path, f);
                stmt_root_expr_with_path(else_value, 0, path, f);
            });
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            stmt_root_expr_with_path(target, 0, path, f);
            stmt_root_expr_with_path(value, 1, path, f);
        }
        Stmt::InlineAsm(asm) => {
            let mut index = 0;
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| {
                    stmt_root_expr_with_path(expr, index, path, f);
                    index += 1;
                });
            }
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => stmt_root_expr_with_path(expr, 0, path, f),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            stmt_root_expr_with_path(cond, 0, path, f);
            with_path_segment(path, PathSegment::Then, |path| {
                body_exprs_with_path(then_body, path, f);
            });
            with_path_segment(path, PathSegment::Else, |path| {
                body_exprs_with_path(else_body, path, f);
            });
        }
        Stmt::Loop { body, .. } => {
            with_path_segment(path, PathSegment::LoopBody, |path| {
                body_exprs_with_path(body, path, f);
            });
        }
        Stmt::For { iter, body, .. } => {
            stmt_root_expr_with_path(iter, 0, path, f);
            with_path_segment(path, PathSegment::ForBody, |path| {
                body_exprs_with_path(body, path, f);
            });
        }
        Stmt::Scope { body } => {
            with_path_segment(path, PathSegment::ScopeBody, |path| {
                body_exprs_with_path(body, path, f);
            });
        }
        Stmt::LabeledBlock { body, .. } => {
            with_path_segment(path, PathSegment::LabeledBody, |path| {
                body_exprs_with_path(body, path, f);
            });
        }
        Stmt::Unsafe { body } => {
            with_path_segment(path, PathSegment::UnsafeBody, |path| {
                block_exprs_with_path(body, path, f);
            });
        }
        Stmt::While { cond, body } => {
            stmt_root_expr_with_path(cond, 0, path, f);
            with_path_segment(path, PathSegment::WhileBody, |path| {
                block_exprs_with_path(body, path, f);
            });
        }
        Stmt::Block(body) => {
            with_path_segment(path, PathSegment::BlockBody, |path| {
                block_exprs_with_path(body, path, f);
            });
        }
        Stmt::Match { expr, arms } => {
            stmt_root_expr_with_path(expr, 0, path, f);
            for (index, arm) in arms.iter().enumerate() {
                with_path_segment(path, PathSegment::MatchArm(index), |path| {
                    body_exprs_with_path(&arm.body, path, f);
                });
            }
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

fn stmt_root_expr_with_path(
    expr: &Expr,
    index: usize,
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&Expr, &mut Vec<PathSegment>),
) {
    with_path_segment(path, PathSegment::Expr(index), |path| {
        exprs_with_path(expr, path, f);
    });
}

pub(in crate::fixups) fn exprs_with_path(
    expr: &Expr,
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&Expr, &mut Vec<PathSegment>),
) {
    f(expr, path);
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => {}
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(expr, path, f);
            });
        }
        Expr::Binary { lhs, rhs, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(lhs, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_with_path(rhs, path, f);
            });
        }
        Expr::Range { start, end } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(start, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_with_path(end, path, f);
            });
        }
        Expr::Call { func, args, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(func, path, f);
            });
            for (index, arg) in args.iter().enumerate() {
                with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    exprs_with_path(arg, path, f);
                });
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(recv, path, f);
            });
            for (index, arg) in args.iter().enumerate() {
                with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    exprs_with_path(arg, path, f);
                });
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(base, path, f);
            });
        }
        Expr::Index { base, index } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(base, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_with_path(index, path, f);
            });
        }
        Expr::StructLit { fields, .. } => {
            for (index, (_, value)) in fields.iter().enumerate() {
                with_path_segment(path, PathSegment::Expr(index), |path| {
                    exprs_with_path(value, path, f);
                });
            }
        }
        Expr::TupleStructLit { fields, .. } => {
            for (index, value) in fields.iter().enumerate() {
                with_path_segment(path, PathSegment::Expr(index), |path| {
                    exprs_with_path(value, path, f);
                });
            }
        }
        Expr::ArrayLit(elems) => {
            for (index, elem) in elems.iter().enumerate() {
                with_path_segment(path, PathSegment::Expr(index), |path| {
                    exprs_with_path(elem, path, f);
                });
            }
        }
        Expr::ArrayRepeat { elem, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(elem, path, f);
            });
        }
        Expr::VecLit(elems) => {
            for (index, elem) in elems.iter().enumerate() {
                with_path_segment(path, PathSegment::Expr(index), |path| {
                    exprs_with_path(elem, path, f);
                });
            }
        }
        Expr::VecRepeat { elem, len } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(elem, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_with_path(len, path, f);
            });
        }
        Expr::Macro { args, .. } => {
            for (index, arg) in args.iter().enumerate() {
                with_path_segment(path, PathSegment::Expr(index), |path| {
                    exprs_with_path(arg, path, f);
                });
            }
        }
        Expr::Closure { body, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(body, path, f);
            });
        }
        Expr::Match { expr, arms } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(expr, path, f);
            });
            for (index, arm) in arms.iter().enumerate() {
                with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    exprs_with_path(&arm.value, path, f);
                });
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(cond, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_with_path(then_expr, path, f);
            });
            with_path_segment(path, PathSegment::Expr(2), |path| {
                exprs_with_path(else_expr, path, f);
            });
        }
        Expr::Block(block) => {
            with_path_segment(path, PathSegment::BlockBody, |path| {
                block_exprs_with_path(block, path, f);
            });
        }
        Expr::Unsafe(block) => {
            with_path_segment(path, PathSegment::UnsafeBody, |path| {
                block_exprs_with_path(block, path, f);
            });
        }
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            if let Some(ptr) = place.ptr_expr() {
                with_path_segment(path, PathSegment::Expr(0), |path| {
                    exprs_with_path(ptr, path, f);
                });
            }
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            if let Some(ptr) = place.ptr_expr() {
                with_path_segment(path, PathSegment::Expr(0), |path| {
                    exprs_with_path(ptr, path, f);
                });
            }
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_with_path(value, path, f);
            });
        }
        Expr::AtomicNew { value, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(value, path, f);
            });
        }
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            if let Some(ptr) = place.ptr_expr() {
                with_path_segment(path, PathSegment::Expr(0), |path| {
                    exprs_with_path(ptr, path, f);
                });
            }
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_with_path(expected, path, f);
            });
            with_path_segment(path, PathSegment::Expr(2), |path| {
                exprs_with_path(desired, path, f);
            });
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(src, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_with_path(dst, path, f);
            });
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(src, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_with_path(dst, path, f);
            });
            with_path_segment(path, PathSegment::Expr(2), |path| {
                exprs_with_path(count, path, f);
            });
        }
        Expr::WriteBytes { dst, val, count } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_with_path(dst, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_with_path(val, path, f);
            });
            with_path_segment(path, PathSegment::Expr(2), |path| {
                exprs_with_path(count, path, f);
            });
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
        Stmt::InlineAsm(asm) => asm.operands.iter().any(|operand| {
            let mut found = false;
            operand.visit_exprs(&mut |expr| found |= exprs_any(expr, pred));
            found
        }),
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
        Stmt::For { iter, body, .. } => exprs_any(iter, pred) || body_exprs_any(body, pred),
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
        | Expr::CStr(_)
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
        Expr::Range { start, end } => exprs_any(start, pred) || exprs_any(end, pred),
        Expr::Call { func, args, .. } => {
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
        Expr::TupleStructLit { fields, .. } => fields.iter().any(|value| exprs_any(value, pred)),
        Expr::ArrayLit(elems) => elems.iter().any(|elem| exprs_any(elem, pred)),
        Expr::ArrayRepeat { elem, .. } => exprs_any(elem, pred),
        Expr::VecLit(elems) => elems.iter().any(|elem| exprs_any(elem, pred)),
        Expr::VecRepeat { elem, len } => exprs_any(elem, pred) || exprs_any(len, pred),
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
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            place.ptr_expr().is_some_and(|ptr| exprs_any(ptr, pred))
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            place.ptr_expr().is_some_and(|ptr| exprs_any(ptr, pred)) || exprs_any(value, pred)
        }
        Expr::AtomicNew { value, .. } => exprs_any(value, pred),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            place.ptr_expr().is_some_and(|ptr| exprs_any(ptr, pred))
                || exprs_any(expected, pred)
                || exprs_any(desired, pred)
        }
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
        Stmt::InlineAsm(asm) => asm.operands.iter().all(|operand| {
            let mut all = true;
            operand.visit_exprs(&mut |expr| {
                all &= exprs_all_with_hooks(expr, stmt_hook, expr_hook);
            });
            all
        }),
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
        Stmt::For { iter, body, .. } => {
            exprs_all_with_hooks(iter, stmt_hook, expr_hook)
                && body_exprs_all_with(body, stmt_hook, expr_hook)
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
        | Expr::CStr(_)
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
        Expr::Range { start, end } => {
            exprs_all_with_hooks(start, stmt_hook, expr_hook)
                && exprs_all_with_hooks(end, stmt_hook, expr_hook)
        }
        Expr::Call { func, args, .. } => {
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
        Expr::TupleStructLit { fields, .. } => fields
            .iter()
            .all(|value| exprs_all_with_hooks(value, stmt_hook, expr_hook)),
        Expr::ArrayLit(elems) => elems
            .iter()
            .all(|elem| exprs_all_with_hooks(elem, stmt_hook, expr_hook)),
        Expr::ArrayRepeat { elem, .. } => exprs_all_with_hooks(elem, stmt_hook, expr_hook),
        Expr::VecLit(elems) => elems
            .iter()
            .all(|elem| exprs_all_with_hooks(elem, stmt_hook, expr_hook)),
        Expr::VecRepeat { elem, len } => {
            exprs_all_with_hooks(elem, stmt_hook, expr_hook)
                && exprs_all_with_hooks(len, stmt_hook, expr_hook)
        }
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
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => place
            .ptr_expr()
            .is_none_or(|ptr| exprs_all_with_hooks(ptr, stmt_hook, expr_hook)),
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            place
                .ptr_expr()
                .is_none_or(|ptr| exprs_all_with_hooks(ptr, stmt_hook, expr_hook))
                && exprs_all_with_hooks(value, stmt_hook, expr_hook)
        }
        Expr::AtomicNew { value, .. } => exprs_all_with_hooks(value, stmt_hook, expr_hook),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            place
                .ptr_expr()
                .is_none_or(|ptr| exprs_all_with_hooks(ptr, stmt_hook, expr_hook))
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
        Stmt::InlineAsm(asm) => {
            for operand in &mut asm.operands {
                operand.visit_exprs_mut(&mut |expr| exprs_mut_with(expr, f));
            }
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
        Stmt::For { iter, body, .. } => {
            exprs_mut_with(iter, f);
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
        | Expr::CStr(_)
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
        Expr::Range { start, end } => {
            exprs_mut_with(start, f);
            exprs_mut_with(end, f);
        }
        Expr::Call { func, args, .. } => {
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
        Expr::TupleStructLit { fields, .. } => {
            for value in fields {
                exprs_mut_with(value, f);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                exprs_mut_with(elem, f);
            }
        }
        Expr::ArrayRepeat { elem, .. } => exprs_mut_with(elem, f),
        Expr::VecLit(elems) => {
            for elem in elems {
                exprs_mut_with(elem, f);
            }
        }
        Expr::VecRepeat { elem, len } => {
            exprs_mut_with(elem, f);
            exprs_mut_with(len, f);
        }
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
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            if let Some(ptr) = place.ptr_expr_mut() {
                exprs_mut_with(ptr, f);
            }
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            if let Some(ptr) = place.ptr_expr_mut() {
                exprs_mut_with(ptr, f);
            }
            exprs_mut_with(value, f);
        }
        Expr::AtomicNew { value, .. } => exprs_mut_with(value, f),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            if let Some(ptr) = place.ptr_expr_mut() {
                exprs_mut_with(ptr, f);
            }
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

pub(in crate::fixups) fn path_starts_with(path: &[PathSegment], prefix: &[PathSegment]) -> bool {
    path.len() >= prefix.len() && path[..prefix.len()] == *prefix
}

pub(in crate::fixups) fn paths_overlap(a: &[PathSegment], b: &[PathSegment]) -> bool {
    path_starts_with(a, b) || path_starts_with(b, a)
}

pub(in crate::fixups) fn target_expr_at_path<'a>(
    program: &'a Program,
    item_index: usize,
    path: &AstPath,
) -> Option<&'a Expr> {
    let Item::Fn(function) = program.items.get(item_index)? else {
        return None;
    };
    target_expr_in_body(&function.body, &path.0)
}

fn target_expr_in_body<'a>(body: &'a [IndentStmt], path: &[PathSegment]) -> Option<&'a Expr> {
    let [PathSegment::Stmt(index), rest @ ..] = path else {
        return None;
    };
    target_stmt_expr_at(&body.get(*index)?.stmt, rest)
}

fn target_stmt_expr_at<'a>(stmt: &'a Stmt, path: &[PathSegment]) -> Option<&'a Expr> {
    match (stmt, path) {
        (
            Stmt::Let {
                init: Some(init), ..
            },
            [PathSegment::Expr(0), rest @ ..],
        ) => target_expr_at(init, rest),
        (
            Stmt::LetIf {
                cond,
                then_body,
                then_value,
                else_body,
                else_value,
                ..
            },
            path,
        ) => match path {
            [PathSegment::Expr(0), rest @ ..] => target_expr_at(cond, rest),
            [PathSegment::Then, PathSegment::Expr(0), rest @ ..] => {
                target_expr_at(then_value, rest)
            }
            [PathSegment::Then, rest @ ..] => target_expr_in_body(then_body, rest),
            [PathSegment::Else, PathSegment::Expr(0), rest @ ..] => {
                target_expr_at(else_value, rest)
            }
            [PathSegment::Else, rest @ ..] => target_expr_in_body(else_body, rest),
            _ => None,
        },
        (Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. }, path) => {
            match path {
                [PathSegment::Expr(0), rest @ ..] => target_expr_at(target, rest),
                [PathSegment::Expr(1), rest @ ..] => target_expr_at(value, rest),
                _ => None,
            }
        }
        (Stmt::InlineAsm(asm), [PathSegment::Expr(index), rest @ ..]) => {
            target_inline_asm_expr_at(asm, *index, rest)
        }
        (Stmt::Expr(expr) | Stmt::Return(Some(expr)), [PathSegment::Expr(0), rest @ ..]) => {
            target_expr_at(expr, rest)
        }
        (
            Stmt::If {
                cond,
                then_body,
                else_body,
            },
            path,
        ) => match path {
            [PathSegment::Expr(0), rest @ ..] => target_expr_at(cond, rest),
            [PathSegment::Then, rest @ ..] => target_expr_in_body(then_body, rest),
            [PathSegment::Else, rest @ ..] => target_expr_in_body(else_body, rest),
            _ => None,
        },
        (Stmt::Loop { body, .. }, [PathSegment::LoopBody, rest @ ..]) => {
            target_expr_in_body(body, rest)
        }
        (Stmt::For { iter, .. }, [PathSegment::Expr(0), rest @ ..]) => target_expr_at(iter, rest),
        (Stmt::For { body, .. }, [PathSegment::ForBody, rest @ ..]) => {
            target_expr_in_body(body, rest)
        }
        (Stmt::Scope { body }, [PathSegment::ScopeBody, rest @ ..]) => {
            target_expr_in_body(body, rest)
        }
        (Stmt::LabeledBlock { body, .. }, [PathSegment::LabeledBody, rest @ ..]) => {
            target_expr_in_body(body, rest)
        }
        (Stmt::Unsafe { body }, [PathSegment::UnsafeBody, rest @ ..]) => {
            target_expr_in_block(body, rest)
        }
        (Stmt::While { cond, .. }, [PathSegment::Expr(0), rest @ ..]) => target_expr_at(cond, rest),
        (Stmt::While { body, .. }, [PathSegment::WhileBody, rest @ ..]) => {
            target_expr_in_block(body, rest)
        }
        (Stmt::Block(body), [PathSegment::BlockBody, rest @ ..]) => {
            target_expr_in_block(body, rest)
        }
        (Stmt::Match { expr, .. }, [PathSegment::Expr(0), rest @ ..]) => target_expr_at(expr, rest),
        (Stmt::Match { arms, .. }, [PathSegment::MatchArm(index), rest @ ..]) => {
            target_expr_in_body(&arms.get(*index)?.body, rest)
        }
        _ => None,
    }
}

fn target_expr_in_block<'a>(block: &'a Block, path: &[PathSegment]) -> Option<&'a Expr> {
    match path {
        [PathSegment::BlockTail] => block.tail.as_deref(),
        [PathSegment::BlockTail, rest @ ..] => target_expr_at(block.tail.as_deref()?, rest),
        _ => target_expr_in_body(&block.stmts, path),
    }
}

fn target_inline_asm_expr_at<'a>(
    asm: &'a InlineAsm,
    index: usize,
    path: &[PathSegment],
) -> Option<&'a Expr> {
    let mut current = 0;
    for operand in &asm.operands {
        let exprs = match operand {
            AsmOperand::In { value, .. }
            | AsmOperand::Out { value, .. }
            | AsmOperand::Const(value) => [Some(value), None],
            AsmOperand::InOut { input, output, .. } => [Some(input), Some(output)],
            AsmOperand::Label { state, value, .. } => [Some(state), Some(value)],
        };
        for expr in exprs.into_iter().flatten() {
            if current == index {
                return target_expr_at(expr, path);
            }
            current += 1;
        }
    }
    None
}

fn target_expr_at<'a>(expr: &'a Expr, path: &[PathSegment]) -> Option<&'a Expr> {
    if path.is_empty() {
        return Some(expr);
    }
    match (expr, path) {
        (Expr::Call { func, .. }, [PathSegment::Expr(0), rest @ ..]) => target_expr_at(func, rest),
        (Expr::Call { args, .. }, [PathSegment::Expr(index), rest @ ..]) if *index > 0 => {
            target_expr_at(args.get(index - 1)?, rest)
        }
        (
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. },
            [PathSegment::Expr(0), rest @ ..],
        ) => target_expr_at(expr, rest),
        (Expr::Range { start, .. }, [PathSegment::Expr(0), rest @ ..]) => {
            target_expr_at(start, rest)
        }
        (Expr::Range { end, .. }, [PathSegment::Expr(1), rest @ ..]) => target_expr_at(end, rest),
        (
            Expr::Binary { lhs, .. }
            | Expr::Index { base: lhs, .. }
            | Expr::Field { base: lhs, .. }
            | Expr::TupleField { base: lhs, .. }
            | Expr::ArrayPtr { array: lhs, .. },
            [PathSegment::Expr(0), rest @ ..],
        ) => target_expr_at(lhs, rest),
        (
            Expr::Binary { rhs, .. } | Expr::Index { index: rhs, .. },
            [PathSegment::Expr(1), rest @ ..],
        ) => target_expr_at(rhs, rest),
        (
            Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. },
            [PathSegment::Expr(0), rest @ ..],
        ) => target_expr_at(recv, rest),
        (
            Expr::MethodCall { args, .. } | Expr::MethodCallGeneric { args, .. },
            [PathSegment::Expr(index), rest @ ..],
        ) if *index > 0 => target_expr_at(args.get(index - 1)?, rest),
        (Expr::StructLit { fields, .. }, [PathSegment::Expr(index), rest @ ..]) => {
            target_expr_at(&fields.get(*index)?.1, rest)
        }
        (
            Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) | Expr::VecLit(fields),
            [PathSegment::Expr(index), rest @ ..],
        ) => target_expr_at(fields.get(*index)?, rest),
        (Expr::Macro { args, .. }, [PathSegment::Expr(index), rest @ ..]) => {
            target_expr_at(args.get(*index)?, rest)
        }
        (
            Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. },
            [PathSegment::Expr(0), rest @ ..],
        ) => target_expr_at(elem, rest),
        (Expr::VecRepeat { elem, .. }, [PathSegment::Expr(0), rest @ ..]) => {
            target_expr_at(elem, rest)
        }
        (Expr::VecRepeat { len, .. }, [PathSegment::Expr(1), rest @ ..]) => {
            target_expr_at(len, rest)
        }
        (Expr::Match { expr, .. }, [PathSegment::Expr(0), rest @ ..]) => target_expr_at(expr, rest),
        (Expr::Match { arms, .. }, [PathSegment::Expr(index), rest @ ..]) if *index > 0 => {
            target_expr_at(&arms.get(index - 1)?.value, rest)
        }
        (Expr::If { cond, .. }, [PathSegment::Expr(0), rest @ ..]) => target_expr_at(cond, rest),
        (Expr::If { then_expr, .. }, [PathSegment::Expr(1), rest @ ..]) => {
            target_expr_at(then_expr, rest)
        }
        (Expr::If { else_expr, .. }, [PathSegment::Expr(2), rest @ ..]) => {
            target_expr_at(else_expr, rest)
        }
        (Expr::Block(block), [PathSegment::BlockBody, rest @ ..])
        | (Expr::Unsafe(block), [PathSegment::UnsafeBody, rest @ ..]) => {
            target_expr_in_block(block, rest)
        }
        (
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. },
            [PathSegment::Expr(0), rest @ ..],
        ) => target_expr_at(place.ptr_expr()?, rest),
        (
            Expr::AtomicStore { place, .. }
            | Expr::AtomicFetch { place, .. }
            | Expr::AtomicSwap { place, .. },
            [PathSegment::Expr(0), rest @ ..],
        ) => target_expr_at(place.ptr_expr()?, rest),
        (
            Expr::AtomicStore { value, .. }
            | Expr::AtomicFetch { value, .. }
            | Expr::AtomicSwap { value, .. },
            [PathSegment::Expr(1), rest @ ..],
        ) => target_expr_at(value, rest),
        (Expr::AtomicNew { value, .. }, [PathSegment::Expr(0), rest @ ..]) => {
            target_expr_at(value, rest)
        }
        (Expr::AtomicCompareExchange { place, .. }, [PathSegment::Expr(0), rest @ ..]) => {
            target_expr_at(place.ptr_expr()?, rest)
        }
        (Expr::AtomicCompareExchange { expected, .. }, [PathSegment::Expr(1), rest @ ..]) => {
            target_expr_at(expected, rest)
        }
        (Expr::AtomicCompareExchange { desired, .. }, [PathSegment::Expr(2), rest @ ..]) => {
            target_expr_at(desired, rest)
        }
        (Expr::CopyNonoverlapping { src, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::PtrCopy { src, .. }, [PathSegment::Expr(0), rest @ ..]) => {
            target_expr_at(src, rest)
        }
        (Expr::CopyNonoverlapping { dst, .. }, [PathSegment::Expr(1), rest @ ..])
        | (Expr::PtrCopy { dst, .. }, [PathSegment::Expr(1), rest @ ..]) => {
            target_expr_at(dst, rest)
        }
        (Expr::PtrCopy { count, .. }, [PathSegment::Expr(2), rest @ ..]) => {
            target_expr_at(count, rest)
        }
        (Expr::WriteBytes { dst, .. }, [PathSegment::Expr(0), rest @ ..]) => {
            target_expr_at(dst, rest)
        }
        (Expr::WriteBytes { val, .. }, [PathSegment::Expr(1), rest @ ..]) => {
            target_expr_at(val, rest)
        }
        (Expr::WriteBytes { count, .. }, [PathSegment::Expr(2), rest @ ..]) => {
            target_expr_at(count, rest)
        }
        _ => None,
    }
}

/// Resolves an `AstPath` back to the `Expr` it was recorded from. Needed
/// because the callback walkers above use HRTB closures, which can't hand
/// back a reference tied to the caller's lifetime.
pub(in crate::fixups) fn expr_at_path<'a>(
    facts: &FixupFacts,
    program: &'a Program,
    function: FunctionId,
    path: &AstPath,
) -> Option<&'a Expr> {
    let item_index = facts.function_item_index(function)?;
    let Item::Fn(f) = &program.items[item_index] else {
        return None;
    };
    expr_in_body(&f.body, &path.0)
}

fn expr_in_body<'a>(body: &'a [IndentStmt], path: &[PathSegment]) -> Option<&'a Expr> {
    let [PathSegment::Stmt(index), rest @ ..] = path else {
        return None;
    };
    stmt_expr_at(&body.get(*index)?.stmt, rest)
}

fn stmt_expr_at<'a>(stmt: &'a Stmt, path: &[PathSegment]) -> Option<&'a Expr> {
    match stmt {
        Stmt::Let {
            init: Some(init), ..
        } => expr_at(init, path),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => match path {
            [PathSegment::Expr(0), rest @ ..] => expr_at(target, rest),
            [PathSegment::Expr(1), rest @ ..] => expr_at(value, rest),
            _ => None,
        },
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_at(expr, path),
        Stmt::If {
            then_body,
            else_body,
            ..
        } => match path {
            [PathSegment::Then, rest @ ..] => expr_in_body(then_body, rest),
            [PathSegment::Else, rest @ ..] => expr_in_body(else_body, rest),
            _ => None,
        },
        Stmt::Loop { body, .. } => match path {
            [PathSegment::LoopBody, rest @ ..] => expr_in_body(body, rest),
            _ => None,
        },
        Stmt::Scope { body } => match path {
            [PathSegment::ScopeBody, rest @ ..] => expr_in_body(body, rest),
            _ => None,
        },
        Stmt::LabeledBlock { body, .. } => match path {
            [PathSegment::LabeledBody, rest @ ..] => expr_in_body(body, rest),
            _ => None,
        },
        Stmt::For { body, .. } => match path {
            [PathSegment::ForBody, rest @ ..] => expr_in_body(body, rest),
            _ => None,
        },
        Stmt::Unsafe { body } => match path {
            [PathSegment::UnsafeBody, rest @ ..] => expr_in_block(body, rest),
            _ => None,
        },
        Stmt::While { body, .. } => match path {
            [PathSegment::WhileBody, rest @ ..] => expr_in_block(body, rest),
            _ => None,
        },
        Stmt::Block(body) => match path {
            [PathSegment::BlockBody, rest @ ..] => expr_in_block(body, rest),
            _ => None,
        },
        Stmt::Match { arms, .. } => match path {
            [PathSegment::MatchArm(index), rest @ ..] => {
                expr_in_body(&arms.get(*index)?.body, rest)
            }
            _ => None,
        },
        _ => None,
    }
}

fn expr_in_block<'a>(block: &'a Block, path: &[PathSegment]) -> Option<&'a Expr> {
    match path {
        [PathSegment::BlockTail] => block.tail.as_deref(),
        [PathSegment::BlockTail, rest @ ..] => expr_at(block.tail.as_deref()?, rest),
        _ => expr_in_body(&block.stmts, path),
    }
}

fn expr_at<'a>(expr: &'a Expr, path: &[PathSegment]) -> Option<&'a Expr> {
    if path.is_empty() {
        return Some(expr);
    }
    match (expr, path) {
        (Expr::Call { func, .. }, [PathSegment::Expr(0), rest @ ..]) => expr_at(func, rest),
        (Expr::Call { args, .. }, [PathSegment::Expr(index), rest @ ..]) if *index > 0 => {
            expr_at(args.get(index - 1)?, rest)
        }
        (Expr::Cast { expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::Unary { expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::Ref { expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::AddrOf { expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::Transmute { expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::MethodCall { recv: expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::ArrayPtr { array: expr, .. }, [PathSegment::Expr(0), rest @ ..]) => {
            expr_at(expr, rest)
        }
        (Expr::Binary { lhs, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::Index { base: lhs, .. }, [PathSegment::Expr(0), rest @ ..]) => expr_at(lhs, rest),
        (Expr::Binary { rhs, .. }, [PathSegment::Expr(1), rest @ ..])
        | (Expr::Index { index: rhs, .. }, [PathSegment::Expr(1), rest @ ..]) => expr_at(rhs, rest),
        (Expr::MethodCall { args, .. }, [PathSegment::Expr(index), rest @ ..]) if *index > 0 => {
            expr_at(args.get(index - 1)?, rest)
        }
        (Expr::Macro { args, .. }, [PathSegment::Expr(index), rest @ ..])
        | (Expr::ArrayLit(args), [PathSegment::Expr(index), rest @ ..]) => {
            expr_at(args.get(*index)?, rest)
        }
        (Expr::Block(block), [PathSegment::BlockBody, rest @ ..])
        | (Expr::Unsafe(block), [PathSegment::UnsafeBody, rest @ ..]) => expr_in_block(block, rest),
        _ => None,
    }
}
