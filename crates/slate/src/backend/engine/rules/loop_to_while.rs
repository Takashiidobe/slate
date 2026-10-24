use super::inline_temps;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, UnaryOp};

pub(in crate::backend::engine) struct LoopToWhile;

pub(in crate::backend::engine) struct LoopElseBreakToWhile;

pub(super) fn negate(cond: &Expr) -> Expr {
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

fn tail_pending_inline(arena: &Arena, body: &[NodeId]) -> bool {
    let Some(&last) = body.last() else {
        return false;
    };
    let Some(NodeKind::Assign {
        value: Expr::Var(tmp),
        ..
    }) = arena.get(last)
    else {
        return false;
    };
    let tmp = *tmp;
    let Some(def_id) = arena.definition(tmp) else {
        return false;
    };
    if !body.contains(&def_id) {
        return false;
    }
    let Some(NodeKind::Let {
        mutable: false,
        init: Some(init),
        ..
    }) = arena.get(def_id)
    else {
        return false;
    };
    !inline_temps::expr_effects(init).is_side_effect() && arena.def_use_neighbors(tmp) == [last]
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
        if tail_pending_inline(arena, body) {
            return false;
        }
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
        arena.retire_subtree(guard_id, Some(id));
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

struct ElseBreakHead {
    guard: NodeId,
    exit: NodeId,
    cond: Expr,
    stmts: Vec<NodeId>,
}

fn else_break_head(arena: &Arena, body: &[NodeId]) -> Option<ElseBreakHead> {
    let guard = *body.first()?;
    let NodeKind::If {
        cond,
        then_body,
        else_body,
    } = arena.get(guard)?
    else {
        return None;
    };
    let [exit] = else_body[..] else {
        return None;
    };
    if then_body.is_empty() || !matches!(arena.get(exit), Some(NodeKind::Break(None))) {
        return None;
    }
    Some(ElseBreakHead {
        guard,
        exit,
        cond: cond.clone(),
        stmts: then_body.iter().chain(&body[1..]).copied().collect(),
    })
}

impl NodeRule for LoopElseBreakToWhile {
    fn name(&self) -> &'static str {
        "loop_to_while::else_break"
    }

    fn priority(&self) -> u32 {
        20
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Loop]
    }

    fn requeues_moved_nodes(&self) -> bool {
        true
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::Loop { body, .. }) = arena.get(id) else {
            return false;
        };
        else_break_head(arena, body).is_some_and(|head| !tail_pending_inline(arena, &head.stmts))
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::Loop { label, body }) = arena.get(id) else {
            return false;
        };
        let Some(head) = else_break_head(arena, body) else {
            return false;
        };
        let label = label.clone();
        arena.retire_subtree(head.exit, Some(head.guard));
        arena.release_comments(head.guard, None);
        arena.take(head.guard);
        for &child in &head.stmts {
            arena.set_parent(child, Some(id));
        }
        arena.set_kind(
            id,
            NodeKind::While {
                label,
                cond: head.cond,
                stmts: head.stmts,
                tail: None,
            },
        );
        arena.touch_subtree(id);
        true
    }
}
