use crate::backend::rust_ast::{BinOp, Expr, Prim, RustValue, Type, UnaryOp};

pub(super) type IsIntegral<'a> = &'a dyn Fn(&Expr) -> bool;

pub(super) fn negate(expr: Expr, integral: IsIntegral<'_>) -> Expr {
    match expr {
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => *expr,
        Expr::Binary { op, lhs, rhs } => match flip(op, &lhs, &rhs, integral) {
            Some(op) => Expr::Binary { op, lhs, rhs },
            None => not(Expr::Binary { op, lhs, rhs }),
        },
        other => not(other),
    }
}

pub(super) fn not(expr: Expr) -> Expr {
    Expr::Unary {
        op: UnaryOp::Not,
        expr: Box::new(expr),
    }
}

pub(super) fn flip(op: BinOp, lhs: &Expr, rhs: &Expr, integral: IsIntegral<'_>) -> Option<BinOp> {
    let total_order = integral(lhs) || integral(rhs);
    match op {
        BinOp::Eq => Some(BinOp::Ne),
        BinOp::Ne => Some(BinOp::Eq),
        BinOp::Lt if total_order => Some(BinOp::Ge),
        BinOp::Le if total_order => Some(BinOp::Gt),
        BinOp::Gt if total_order => Some(BinOp::Le),
        BinOp::Ge if total_order => Some(BinOp::Lt),
        _ => None,
    }
}

pub(super) fn is_integral_literal(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Value(
            RustValue::I64(_)
                | RustValue::I128(_)
                | RustValue::U128(_)
                | RustValue::Usize(_)
                | RustValue::TypedInt(..)
                | RustValue::TypedUInt(..)
                | RustValue::NullPtr
        )
    )
}

pub(super) fn is_integral_type(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Prim(
            Prim::Bool
                | Prim::I8
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
        ) | Type::Ptr { .. }
    )
}
