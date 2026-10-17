use super::walk;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, Prim, RustValue, Type, UnaryOp};

const KINDS: &[NodeKindTag] = &[
    NodeKindTag::Let,
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

fn bounds(prim: Prim) -> Option<(i128, i128)> {
    Some(match prim {
        Prim::I8 => (i8::MIN.into(), i8::MAX.into()),
        Prim::I16 => (i16::MIN.into(), i16::MAX.into()),
        Prim::I32 | Prim::Isize => (i32::MIN.into(), i32::MAX.into()),
        Prim::I64 => (i64::MIN.into(), i64::MAX.into()),
        Prim::I128 => (i128::MIN, i128::MAX),
        Prim::U8 => (0, u8::MAX.into()),
        Prim::U16 => (0, u16::MAX.into()),
        Prim::U32 | Prim::Usize => (0, u32::MAX.into()),
        Prim::U64 => (0, u64::MAX.into()),
        Prim::U128 => (0, i128::MAX),
        _ => return None,
    })
}

fn fits(value: i128, ty: &Type) -> bool {
    matches!(ty, Type::Prim(prim) if bounds(*prim).is_some_and(|(lo, hi)| (lo..=hi).contains(&value)))
}

fn literal_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some((*n).into()),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Value(RustValue::U128(n)) => i128::try_from(*n).ok(),
        Expr::Value(RustValue::Usize(n)) => i128::try_from(*n).ok(),
        _ => None,
    }
}

fn eval(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Cast { expr, ty } => eval(expr).filter(|&value| fits(value, ty)),
        Expr::Unary {
            op: UnaryOp::Neg,
            expr,
        } => eval(expr)?.checked_neg(),
        other => literal_value(other),
    }
}

fn literal_type(expr: &Expr) -> Option<&Type> {
    match expr {
        Expr::Cast { ty, .. } => Some(ty),
        Expr::Unary {
            op: UnaryOp::Neg,
            expr,
        } => literal_type(expr),
        _ => None,
    }
}

fn is_bare(expr: &Expr) -> bool {
    match expr {
        Expr::Unary {
            op: UnaryOp::Neg,
            expr,
        } => literal_value(expr).is_some(),
        other => literal_value(other).is_some(),
    }
}

fn literal(value: i128) -> Expr {
    let magnitude = Expr::Value(match i64::try_from(value.unsigned_abs()) {
        Ok(small) => RustValue::I64(small),
        Err(_) => RustValue::U128(value.unsigned_abs()),
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

fn strippable(expr: &Expr) -> bool {
    !is_bare(expr)
        && eval(expr).is_some()
        && literal_type(expr).is_some_and(|ty| eval(expr).is_some_and(|value| fits(value, ty)))
}

fn fixed(expr: &Expr) -> bool {
    match expr {
        Expr::Call { func, .. } => {
            !matches!(func.as_ref(), Expr::Var(v) if v.as_str() == "std::mem::zeroed")
        }
        Expr::Transmute { .. } | Expr::Todo(_) => false,
        Expr::Block(block) | Expr::Unsafe(block) => block.tail.as_deref().is_some_and(fixed),
        other => eval(other).is_none(),
    }
}

fn arithmetic(op: BinOp) -> bool {
    matches!(
        op,
        BinOp::Add
            | BinOp::Sub
            | BinOp::Mul
            | BinOp::Div
            | BinOp::Rem
            | BinOp::BitAnd
            | BinOp::BitOr
            | BinOp::BitXor
    )
}

fn comparison(op: BinOp) -> bool {
    matches!(
        op,
        BinOp::Eq | BinOp::Ne | BinOp::Lt | BinOp::Le | BinOp::Gt | BinOp::Ge
    )
}

fn self_typed_method(method: &str) -> bool {
    matches!(method, "wrapping_add" | "wrapping_sub" | "wrapping_mul")
}

fn rewrite(expr: &mut Expr, typed: bool) -> bool {
    if typed && strippable(expr) {
        if let Some(value) = eval(expr) {
            *expr = literal(value);
        }
        return true;
    }
    if let Expr::Cast { expr: inner, ty } = expr
        && !is_bare(inner)
        && let Some(value) = eval(inner)
        && literal_type(inner).is_some()
        && fits(value, ty)
    {
        **inner = literal(value);
        return true;
    }
    match expr {
        Expr::Binary { op, lhs, rhs } if arithmetic(*op) || comparison(*op) => {
            let both = typed && arithmetic(*op);
            let lhs_typed = both || (fixed(rhs) && !strippable(rhs));
            let rhs_typed = both || (fixed(lhs) && !strippable(lhs));
            let left = rewrite(lhs, lhs_typed);
            rewrite(rhs, rhs_typed) || left
        }
        Expr::Range { start, end } => {
            let start_typed = fixed(end) && !strippable(end);
            let end_typed = fixed(start) && !strippable(start);
            let left = rewrite(start, start_typed);
            rewrite(end, end_typed) || left
        }
        Expr::Unary {
            op: UnaryOp::Neg | UnaryOp::Not,
            expr,
        } => rewrite(expr, typed),
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            let mut changed = rewrite(cond, false);
            changed |= rewrite(then_expr, typed);
            changed |= rewrite(else_expr, typed);
            changed
        }
        Expr::Block(block) | Expr::Unsafe(block) if block.stmts.is_empty() => block
            .tail
            .as_deref_mut()
            .is_some_and(|tail| rewrite(tail, typed)),
        Expr::StructLit { fields, .. } => {
            let mut changed = false;
            for (_, value) in fields {
                changed |= rewrite(value, true);
            }
            changed
        }
        Expr::ArrayLit(elems) => {
            let mut changed = false;
            for elem in elems {
                changed |= rewrite(elem, typed);
            }
            changed
        }
        Expr::MethodCall { recv, method, args } if self_typed_method(method.as_str()) => {
            let args_typed = fixed(recv) && !strippable(recv);
            let mut changed = rewrite(recv, false);
            for arg in args {
                changed |= rewrite(arg, args_typed);
            }
            changed
        }
        other => {
            let mut changed = false;
            for child in walk::child_exprs_mut(other) {
                changed |= rewrite(child, false);
            }
            changed
        }
    }
}

fn rewrite_kind(kind: &mut NodeKind, tail: bool) -> bool {
    match kind {
        NodeKind::Let {
            ty,
            init: Some(init),
            ..
        } => rewrite(init, ty.is_some()),
        NodeKind::Assign { target, value } => {
            let target_changed = rewrite(target, false);
            rewrite(value, true) || target_changed
        }
        NodeKind::CompoundAssign { target, op, value } => {
            let target_changed = rewrite(target, false);
            rewrite(value, !matches!(op, BinOp::Shl | BinOp::Shr)) || target_changed
        }
        NodeKind::Return(Some(expr)) => rewrite(expr, true),
        NodeKind::Expr(expr) if tail => rewrite(expr, true),
        NodeKind::LetIf { .. } | NodeKind::InlineAsm(_) => false,
        other => {
            let mut changed = false;
            walk::visit_kind_exprs_mut(other, |expr| changed |= rewrite(expr, false));
            changed
        }
    }
}

fn function_tail(arena: &FunctionOptimizer, id: NodeId) -> bool {
    if arena.return_type().is_none_or(Type::is_unit) {
        return false;
    }
    let Some(parent) = arena.parent(id) else {
        return false;
    };
    arena.parent(parent).is_none()
        && arena.get(parent).and_then(|kind| {
            kind.child_lists()
                .first()
                .and_then(|list| list.last().copied())
        }) == Some(id)
}

pub(in crate::backend::engine) struct LiteralCasts;

impl NodeRule for LiteralCasts {
    fn name(&self) -> &'static str {
        "literal_casts"
    }

    fn priority(&self) -> u32 {
        47
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
            found |= walk::any_collapsible(
                expr,
                &|expr| matches!(expr, Expr::Cast { expr: inner, .. } if eval(expr).is_some() || (!is_bare(inner) && eval(inner).is_some())),
            )
        });
        found
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let tail = function_tail(arena, id);
        let Some(kind) = arena.get_mut(id) else {
            return false;
        };
        if !rewrite_kind(kind, tail) {
            return false;
        }
        arena.touch(id);
        true
    }
}
