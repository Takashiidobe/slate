use crate::backend::rust_ast::{Expr, Stmt};

pub(super) fn child_exprs(expr: &Expr) -> Vec<&Expr> {
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. }
        | Expr::Field { base: expr, .. }
        | Expr::TupleField { base: expr, .. }
        | Expr::ArrayPtr { array: expr, .. }
        | Expr::ArrayRepeat { elem: expr, .. }
        | Expr::AtomicNew { value: expr, .. }
        | Expr::Closure { body: expr, .. } => vec![expr],
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        }
        | Expr::Index {
            base: lhs,
            index: rhs,
        }
        | Expr::VecRepeat {
            elem: lhs,
            len: rhs,
        } => vec![lhs, rhs],
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => vec![cond, then_expr, else_expr],
        Expr::Call { func, args, .. } => {
            let mut out = vec![func.as_ref()];
            out.extend(args.iter());
            out
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            let mut out = vec![recv.as_ref()];
            out.extend(args.iter());
            out
        }
        Expr::StructLit { fields, .. } => fields.iter().map(|(_, value)| value).collect(),
        Expr::TupleStructLit { fields, .. } => fields.iter().collect(),
        Expr::ArrayLit(elems) | Expr::VecLit(elems) => elems.iter().collect(),
        Expr::Macro { args, .. } => args.iter().collect(),
        Expr::Match { expr, arms } => {
            let mut out = vec![expr.as_ref()];
            out.extend(arms.iter().map(|arm| &arm.value));
            out
        }
        Expr::CopyNonoverlapping { src, dst, .. } => vec![src, dst],
        Expr::PtrCopy {
            src, dst, count, ..
        } => vec![src, dst, count],
        Expr::WriteBytes { dst, val, count } => vec![dst, val, count],
        Expr::AtomicStore { value, .. }
        | Expr::AtomicFetch { value, .. }
        | Expr::AtomicSwap { value, .. } => vec![value],
        Expr::AtomicCompareExchange {
            expected, desired, ..
        } => vec![expected, desired],
        Expr::Block(block) | Expr::Unsafe(block) => {
            let mut out: Vec<&Expr> = Vec::new();
            for stmt in &block.stmts {
                stmt_exprs(&stmt.stmt, &mut out);
            }
            if let Some(tail) = &block.tail {
                out.push(tail);
            }
            out
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. }
        | Expr::AtomicRef { .. }
        | Expr::AtomicLoad { .. } => Vec::new(),
    }
}

pub(super) fn child_exprs_mut(expr: &mut Expr) -> Vec<&mut Expr> {
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. }
        | Expr::Field { base: expr, .. }
        | Expr::TupleField { base: expr, .. }
        | Expr::ArrayPtr { array: expr, .. }
        | Expr::ArrayRepeat { elem: expr, .. }
        | Expr::AtomicNew { value: expr, .. }
        | Expr::Closure { body: expr, .. } => vec![expr],
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        }
        | Expr::Index {
            base: lhs,
            index: rhs,
        }
        | Expr::VecRepeat {
            elem: lhs,
            len: rhs,
        } => vec![lhs, rhs],
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => vec![cond, then_expr, else_expr],
        Expr::Call { func, args, .. } => {
            let mut out = vec![func.as_mut()];
            out.extend(args.iter_mut());
            out
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            let mut out = vec![recv.as_mut()];
            out.extend(args.iter_mut());
            out
        }
        Expr::StructLit { fields, .. } => fields.iter_mut().map(|(_, value)| value).collect(),
        Expr::TupleStructLit { fields, .. } => fields.iter_mut().collect(),
        Expr::ArrayLit(elems) | Expr::VecLit(elems) => elems.iter_mut().collect(),
        Expr::Macro { args, .. } => args.iter_mut().collect(),
        Expr::Match { expr, arms } => {
            let mut out = vec![expr.as_mut()];
            out.extend(arms.iter_mut().map(|arm| &mut arm.value));
            out
        }
        Expr::CopyNonoverlapping { src, dst, .. } => vec![src, dst],
        Expr::PtrCopy {
            src, dst, count, ..
        } => vec![src, dst, count],
        Expr::WriteBytes { dst, val, count } => vec![dst, val, count],
        Expr::AtomicStore { value, .. }
        | Expr::AtomicFetch { value, .. }
        | Expr::AtomicSwap { value, .. } => vec![value],
        Expr::AtomicCompareExchange {
            expected, desired, ..
        } => vec![expected, desired],
        Expr::Block(block) | Expr::Unsafe(block) => {
            let mut out: Vec<&mut Expr> = Vec::new();
            for stmt in &mut block.stmts {
                stmt_exprs_mut(&mut stmt.stmt, &mut out);
            }
            if let Some(tail) = &mut block.tail {
                out.push(tail);
            }
            out
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. }
        | Expr::AtomicRef { .. }
        | Expr::AtomicLoad { .. } => Vec::new(),
    }
}

pub(super) fn stmt_exprs<'a>(stmt: &'a Stmt, out: &mut Vec<&'a Expr>) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => out.push(expr),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            out.push(target);
            out.push(value);
        }
        _ => {}
    }
}

pub(super) fn stmt_exprs_mut<'a>(stmt: &'a mut Stmt, out: &mut Vec<&'a mut Expr>) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => out.push(expr),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            out.push(target);
            out.push(value);
        }
        _ => {}
    }
}
