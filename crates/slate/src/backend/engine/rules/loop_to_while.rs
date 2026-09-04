use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, UnaryOp};

pub(in crate::backend::engine) struct LoopToWhile;

fn negate(cond: &Expr) -> Expr {
    match cond {
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => (**expr).clone(),
        _ => Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(cond.clone()),
        },
    }
}

fn head_test(arena: &Arena, body: &[NodeId]) -> Option<Expr> {
    let Some(NodeKind::If {
        cond,
        then_body,
        else_body,
    }) = arena.get(*body.first()?)
    else {
        return None;
    };
    if !else_body.is_empty() {
        return None;
    }
    let [only] = then_body[..] else {
        return None;
    };
    matches!(arena.get(only), Some(NodeKind::Break(None))).then(|| cond.clone())
}

impl NodeRule for LoopToWhile {
    fn name(&self) -> &'static str {
        "loop_to_while::recover"
    }

    fn priority(&self) -> u32 {
        20
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Loop]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::Loop { body, .. }) = arena.get(id) else {
            return false;
        };
        head_test(arena, body).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::Loop { label, body }) = arena.get(id) else {
            return false;
        };
        let Some(cond) = head_test(arena, body) else {
            return false;
        };
        let label = label.clone();
        let cond = negate(&cond);
        let guard_id = body[0];
        let stmts = body[1..].to_vec();
        arena.discard_subtree(guard_id);
        arena.set_kind(
            id,
            NodeKind::While {
                label,
                cond,
                stmts,
                tail: None,
            },
        );
        arena.touch_subtree(id);
        true
    }
}
