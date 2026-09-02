use super::negate;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, UnaryOp};

pub(in crate::backend::engine) struct NotFold;

fn folded(arena: &Arena, cond: &Expr) -> Option<Expr> {
    let Expr::Unary {
        op: UnaryOp::Not,
        expr,
    } = cond
    else {
        return None;
    };
    let Expr::Binary { op, lhs, rhs } = expr.as_ref() else {
        return None;
    };
    let integral = |e: &Expr| match e {
        Expr::Var(name) => arena.var_type(*name).is_some_and(negate::is_integral_type),
        other => negate::is_integral_literal(other),
    };
    let op = negate::flip(*op, lhs, rhs, &integral)?;
    Some(Expr::Binary {
        op,
        lhs: lhs.clone(),
        rhs: rhs.clone(),
    })
}

fn cond_of(arena: &Arena, id: NodeId) -> Option<&Expr> {
    match arena.get(id)? {
        NodeKind::If { cond, .. } | NodeKind::While { cond, .. } => Some(cond),
        _ => None,
    }
}

impl NodeRule for NotFold {
    fn name(&self) -> &'static str {
        "not_fold::flip_comparison"
    }

    fn priority(&self) -> u32 {
        50
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::If, NodeKindTag::While]
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        cond_of(arena, id).is_some_and(|cond| folded(arena, cond).is_some())
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(replacement) = cond_of(arena, id).and_then(|cond| folded(arena, cond)) else {
            return false;
        };
        match arena.get_mut(id) {
            Some(NodeKind::If { cond, .. } | NodeKind::While { cond, .. }) => {
                *cond = replacement;
                true
            }
            _ => false,
        }
    }
}
