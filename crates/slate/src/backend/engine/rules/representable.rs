use crate::backend::rust_ast::{Expr, Prim, RustValue, Type};

fn nonnegative_bound(prim: Prim, pointer_width: u32) -> Option<i128> {
    let (bits, signed) = match prim.resolved() {
        Prim::I8 => (8, true),
        Prim::I16 => (16, true),
        Prim::I32 => (32, true),
        Prim::I64 => (64, true),
        Prim::I128 => (128, true),
        Prim::Isize => (pointer_width, true),
        Prim::U8 => (8, false),
        Prim::U16 => (16, false),
        Prim::U32 => (32, false),
        Prim::U64 => (64, false),
        Prim::U128 => (128, false),
        Prim::Usize => (pointer_width, false),
        _ => return None,
    };
    let bits = if signed { bits - 1 } else { bits };
    Some(if bits >= 127 {
        i128::MAX
    } else {
        (1i128 << bits) - 1
    })
}

fn literal_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(value)) if *value >= 0 => Some(*value as i128),
        Expr::Value(RustValue::I128(value)) if *value >= 0 => Some(*value),
        Expr::Value(RustValue::U128(value)) => i128::try_from(*value).ok(),
        _ => None,
    }
}

pub(super) fn representable_literal(expr: &Expr, pointer_width: u32) -> Option<Expr> {
    if literal_value(expr).is_some() {
        return Some(expr.clone());
    }
    let Expr::Cast {
        expr: inner,
        ty: Type::Prim(prim),
    } = expr
    else {
        return None;
    };
    let bound = nonnegative_bound(*prim, pointer_width)?;
    let literal = representable_literal(inner, pointer_width)?;
    (literal_value(&literal)? <= bound).then_some(literal)
}

pub(super) fn literal_within(literal: &Expr, prim: Prim, pointer_width: u32) -> bool {
    literal_value(literal)
        .zip(nonnegative_bound(prim, pointer_width))
        .is_some_and(|(value, bound)| value <= bound)
}
