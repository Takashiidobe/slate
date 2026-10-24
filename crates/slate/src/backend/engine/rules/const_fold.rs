use super::walk;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, Item, Prim, RustValue, Type, UnaryOp};

const KINDS: &[NodeKindTag] = &[
    NodeKindTag::Let,
    NodeKindTag::LetIf,
    NodeKindTag::Assign,
    NodeKindTag::CompoundAssign,
    NodeKindTag::Expr,
    NodeKindTag::Return,
    NodeKindTag::Unsafe,
    NodeKindTag::Block,
    NodeKindTag::If,
    NodeKindTag::For,
    NodeKindTag::Match,
    NodeKindTag::While,
];

#[derive(Clone, Copy)]
struct Val {
    value: i128,
    ty: Option<Prim>,
}

fn width(prim: Prim) -> Option<(u32, bool)> {
    Some(match prim {
        Prim::I8 => (8, true),
        Prim::I16 => (16, true),
        Prim::I32 => (32, true),
        Prim::I64 => (64, true),
        Prim::U8 => (8, false),
        Prim::U16 => (16, false),
        Prim::U32 => (32, false),
        Prim::U64 => (64, false),
        _ => return None,
    })
}

fn bounds(prim: Prim) -> Option<(i128, i128)> {
    let (bits, signed) = width(prim)?;
    Some(if signed {
        (-(1i128 << (bits - 1)), (1i128 << (bits - 1)) - 1)
    } else {
        (0, (1i128 << bits) - 1)
    })
}

fn fits(value: i128, prim: Prim) -> bool {
    bounds(prim).is_some_and(|(lo, hi)| (lo..=hi).contains(&value))
}

fn wrap(value: i128, prim: Prim) -> Option<i128> {
    let (bits, signed) = width(prim)?;
    let masked = value & ((1i128 << bits) - 1);
    Some(if signed && masked >> (bits - 1) == 1 {
        masked - (1i128 << bits)
    } else {
        masked
    })
}

fn untyped(value: i128) -> Val {
    Val { value, ty: None }
}

fn typed(value: i128, prim: Prim) -> Option<Val> {
    fits(value, prim).then_some(Val {
        value,
        ty: Some(prim),
    })
}

fn settle(lhs: Val, rhs: Val) -> Option<Prim> {
    let prim = match (lhs.ty, rhs.ty) {
        (Some(a), Some(b)) if a == b => a,
        (Some(a), None) if fits(rhs.value, a) => a,
        (None, Some(b)) if fits(lhs.value, b) => b,
        _ => return None,
    };
    (fits(lhs.value, prim) && fits(rhs.value, prim)).then_some(prim)
}

fn arithmetic(op: BinOp, lhs: Val, rhs: Val) -> Option<Val> {
    if matches!(op, BinOp::Shl | BinOp::Shr) {
        let prim = lhs.ty?;
        let (bits, _) = width(prim)?;
        let amount = u32::try_from(rhs.value).ok().filter(|&n| n < bits)?;
        let shifted = if op == BinOp::Shl {
            lhs.value << amount
        } else {
            lhs.value >> amount
        };
        return typed(wrap(shifted, prim)?, prim);
    }
    let prim = settle(lhs, rhs)?;
    let (a, b) = (lhs.value, rhs.value);
    let value = match op {
        BinOp::Add => a.checked_add(b)?,
        BinOp::Sub => a.checked_sub(b)?,
        BinOp::Mul => a.checked_mul(b)?,
        BinOp::Div if b != 0 => a.checked_div(b)?,
        BinOp::Rem if b != 0 => a.checked_rem(b)?,
        BinOp::BitAnd => a & b,
        BinOp::BitOr => a | b,
        BinOp::BitXor => a ^ b,
        _ => return None,
    };
    typed(value, prim)
}

fn wrapping(method: &str, lhs: Val, rhs: Val) -> Option<Val> {
    let prim = settle(lhs, rhs)?;
    lhs.ty?;
    let value = match method {
        "wrapping_add" => lhs.value.checked_add(rhs.value)?,
        "wrapping_sub" => lhs.value.checked_sub(rhs.value)?,
        "wrapping_mul" => lhs.value.checked_mul(rhs.value)?,
        _ => return None,
    };
    typed(wrap(value, prim)?, prim)
}

fn eval(expr: &Expr) -> Option<Val> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some(untyped((*n).into())),
        Expr::Value(RustValue::I128(n)) => Some(untyped(*n)),
        Expr::Value(RustValue::U128(n)) => i128::try_from(*n).ok().map(untyped),
        Expr::Value(RustValue::TypedInt(n, prim)) => typed(*n, *prim),
        Expr::Value(RustValue::TypedUInt(n, prim)) => typed(i128::try_from(*n).ok()?, *prim),
        Expr::Unary {
            op: UnaryOp::Neg,
            expr,
        } => {
            let inner = eval(expr)?;
            let value = inner.value.checked_neg()?;
            match inner.ty {
                Some(prim) if width(prim)?.1 => typed(value, prim),
                Some(_) => None,
                None => Some(untyped(value)),
            }
        }
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => {
            let inner = eval(expr)?;
            let prim = inner.ty?;
            typed(wrap(!inner.value, prim)?, prim)
        }
        Expr::Cast {
            expr,
            ty: Type::Prim(prim),
        } => {
            let inner = eval(expr)?;
            match inner.ty {
                Some(_) => typed(wrap(inner.value, *prim)?, *prim),
                None => typed(inner.value, *prim),
            }
        }
        Expr::Binary { op, lhs, rhs } => arithmetic(*op, eval(lhs)?, eval(rhs)?),
        Expr::MethodCall { recv, method, args } if args.len() == 1 => {
            wrapping(method.as_str(), eval(recv)?, eval(&args[0])?)
        }
        _ => None,
    }
}

fn is_literal(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) => true,
        Expr::Unary {
            op: UnaryOp::Neg,
            expr,
        } => matches!(expr.as_ref(), Expr::Value(_)),
        _ => false,
    }
}

fn foldable(expr: &Expr) -> Option<(i128, Prim)> {
    if is_literal(expr) {
        return None;
    }
    let val = eval(expr)?;
    Some((val.value, val.ty?))
}

fn literal(value: i128, prim: Prim, bare: bool) -> Expr {
    let magnitude = Expr::Value(if bare {
        RustValue::I64(value.unsigned_abs() as i64)
    } else if value < 0 {
        RustValue::TypedInt(-value, prim)
    } else {
        RustValue::TypedUInt(value as u128, prim)
    });
    if value < 0 {
        Expr::Unary {
            op: UnaryOp::Neg,
            expr: Box::new(magnitude),
        }
    } else {
        magnitude
    }
}

fn fold(expr: &mut Expr, expected: Option<Prim>) -> bool {
    if let Some((value, prim)) = foldable(expr) {
        let bare = expected == Some(prim) && i64::try_from(value.unsigned_abs()).is_ok();
        *expr = literal(value, prim, bare);
        return true;
    }
    let mut changed = false;
    for child in walk::child_exprs_mut(expr) {
        changed |= fold(child, None);
    }
    changed
}

fn fold_static(expr: &mut Expr, ty: &Type) -> bool {
    match (expr, ty) {
        (Expr::ArrayLit(elems), Type::Array { elem, .. }) => {
            let mut changed = false;
            for item in elems {
                changed |= fold_static(item, elem);
            }
            changed
        }
        (expr, Type::Prim(prim)) => fold(expr, Some(*prim)),
        (expr, _) => fold(expr, None),
    }
}

pub(in crate::backend::engine) fn fold_initializers(items: &mut [Item]) {
    for item in items {
        match item {
            Item::Static { ty, init, .. } | Item::Const { ty, init, .. } => {
                fold_static(init, ty);
            }
            Item::InlineMod { items, .. } => fold_initializers(items),
            _ => {}
        }
    }
}

pub(in crate::backend::engine) struct ConstFold;

impl NodeRule for ConstFold {
    fn name(&self) -> &'static str {
        "const_fold"
    }

    fn priority(&self) -> u32 {
        48
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        KINDS
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(kind) = arena.get(id) else {
            return false;
        };
        let mut found = false;
        walk::visit_kind_exprs(kind, |expr| {
            found |= walk::any_collapsible(expr, &|expr| foldable(expr).is_some())
        });
        found
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(kind) = arena.get_mut(id) else {
            return false;
        };
        if matches!(kind, NodeKind::InlineAsm(_)) {
            return false;
        }
        let mut changed = false;
        walk::visit_kind_exprs_mut(kind, |expr| changed |= fold(expr, None));
        if !changed {
            return false;
        }
        arena.touch(id);
        true
    }
}
