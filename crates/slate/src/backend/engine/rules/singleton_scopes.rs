use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind};
use crate::backend::rust_ast::{Expr, UnaryOp};

fn is_scope(arena: &Arena, id: NodeId) -> bool {
    matches!(arena.get(id), Some(NodeKind::Scope { .. }))
}

fn is_negated_break_guard(arena: &Arena, id: NodeId) -> bool {
    match arena.get(id) {
        Some(NodeKind::If {
            cond,
            then_body,
            else_body,
        }) => {
            matches!(
                cond,
                Expr::Unary {
                    op: UnaryOp::Not,
                    ..
                }
            ) && else_body.is_empty()
                && then_body.len() == 1
                && matches!(arena.get(then_body[0]), Some(NodeKind::Break(None)))
        }
        _ => false,
    }
}

fn is_unwrappable_kind(kind: &NodeKind) -> bool {
    matches!(
        kind,
        NodeKind::If { .. }
            | NodeKind::Loop { .. }
            | NodeKind::For { .. }
            | NodeKind::Match { .. }
            | NodeKind::Assign { .. }
            | NodeKind::CompoundAssign { .. }
            | NodeKind::Expr(_)
    )
}

fn splice_scope_into_loop(arena: &mut Arena, loop_id: NodeId, scope_index: usize) -> bool {
    let Some(NodeKind::Loop { body, .. }) = arena.get(loop_id) else {
        return false;
    };
    let Some(&scope_id) = body.get(scope_index) else {
        return false;
    };
    let Some(mut scope_kind) = arena.take(scope_id) else {
        return false;
    };
    let NodeKind::Scope { body: children } = &mut scope_kind else {
        unreachable!("scope_index only reached via is_scope check")
    };
    let children = std::mem::take(children);
    for &child in &children {
        arena.set_parent(child, Some(loop_id));
    }
    let Some(NodeKind::Loop { body, .. }) = arena.get_mut(loop_id) else {
        return false;
    };
    body.remove(scope_index);
    for (offset, child) in children.into_iter().enumerate() {
        body.insert(scope_index + offset, child);
    }
    true
}

pub(in crate::backend::engine) struct WhileLoopUnwrap;

impl NodeRule for WhileLoopUnwrap {
    fn name(&self) -> &'static str {
        "singleton_scopes::while_loop"
    }

    fn priority(&self) -> u32 {
        10
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        let Some(NodeKind::Loop { body, .. }) = arena.get(id) else {
            return false;
        };
        body.len() >= 2 && is_negated_break_guard(arena, body[0]) && is_scope(arena, body[1])
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        if !self.matches(arena, id) {
            return false;
        }
        splice_scope_into_loop(arena, id, 1)
    }
}

pub(in crate::backend::engine) struct DoWhileLoopUnwrap;

impl NodeRule for DoWhileLoopUnwrap {
    fn name(&self) -> &'static str {
        "singleton_scopes::do_while_loop"
    }

    fn priority(&self) -> u32 {
        11
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        let Some(NodeKind::Loop { body, .. }) = arena.get(id) else {
            return false;
        };
        body.len() >= 2 && is_negated_break_guard(arena, body[1]) && is_scope(arena, body[0])
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        if !self.matches(arena, id) {
            return false;
        }
        splice_scope_into_loop(arena, id, 0)
    }
}

pub(in crate::backend::engine) struct SingletonUnwrap;

impl NodeRule for SingletonUnwrap {
    fn name(&self) -> &'static str {
        "singleton_scopes::singleton"
    }

    fn priority(&self) -> u32 {
        12
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        let Some(NodeKind::Scope { body }) = arena.get(id) else {
            return false;
        };
        body.len() == 1 && arena.get(body[0]).is_some_and(is_unwrappable_kind)
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(NodeKind::Scope { body }) = arena.get(id) else {
            return false;
        };
        if body.len() != 1 {
            return false;
        }
        let child_id = body[0];
        if !arena.get(child_id).is_some_and(is_unwrappable_kind) {
            return false;
        }
        let Some(mut child_kind) = arena.take(child_id) else {
            return false;
        };
        arena.reparent_children(&mut child_kind, Some(id));
        let Some(slot) = arena.get_mut(id) else {
            return false;
        };
        *slot = child_kind;
        true
    }
}
