use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::Label;

pub(in crate::backend::engine) struct BreakToElse;
pub(in crate::backend::engine) struct TailBreakDrop;
pub(in crate::backend::engine) struct LabelElide;

fn reaches_label_end(arena: &Arena, container: NodeId, label: &Label) -> bool {
    let Some(kind) = arena.get(container) else {
        return false;
    };
    if matches!(kind, NodeKind::LabeledBlock { label: owner, .. } if owner == label) {
        return true;
    }
    if !matches!(
        kind,
        NodeKind::If { .. }
            | NodeKind::Match { .. }
            | NodeKind::Scope { .. }
            | NodeKind::LabeledBlock { .. }
            | NodeKind::Unsafe { tail: None, .. }
            | NodeKind::Block { tail: None, .. }
    ) {
        return false;
    }
    let Some(parent) = arena.parent(container) else {
        return false;
    };
    let Some(parent_kind) = arena.get(parent) else {
        return false;
    };
    if !parent_kind
        .child_lists()
        .iter()
        .any(|list| list.last() == Some(&container))
    {
        return false;
    }
    reaches_label_end(arena, parent, label)
}

fn position_in_parent(arena: &Arena, id: NodeId) -> Option<(NodeId, usize, usize, usize)> {
    let parent = arena.parent(id)?;
    let kind = arena.get(parent)?;
    let (list_index, list) = kind
        .child_lists()
        .into_iter()
        .enumerate()
        .find(|(_, list)| list.contains(&id))?;
    let pos = list.iter().position(|&child| child == id)?;
    Some((parent, list_index, pos, list.len()))
}

fn escaping_break(arena: &Arena, id: NodeId) -> Option<Label> {
    let NodeKind::If {
        then_body,
        else_body,
        ..
    } = arena.get(id)?
    else {
        return None;
    };
    if !else_body.is_empty() {
        return None;
    }
    match arena.get(*then_body.last()?)? {
        NodeKind::Break(Some(label)) => Some(label.clone()),
        _ => None,
    }
}

fn subtree_breaks(arena: &Arena, id: NodeId, label: &Label) -> bool {
    let Some(kind) = arena.get(id) else {
        return false;
    };
    if matches!(kind, NodeKind::Break(Some(target)) if target == label) {
        return true;
    }
    kind.child_lists()
        .into_iter()
        .flatten()
        .any(|&child| subtree_breaks(arena, child, label))
}

impl NodeRule for BreakToElse {
    fn name(&self) -> &'static str {
        "label_elide::break_to_else"
    }

    fn priority(&self) -> u32 {
        5
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::If]
    }

    fn requeues_moved_nodes(&self) -> bool {
        true
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        let Some(label) = escaping_break(arena, id) else {
            return false;
        };
        let Some((parent, _, pos, len)) = position_in_parent(arena, id) else {
            return false;
        };
        pos + 1 < len && reaches_label_end(arena, parent, &label)
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        if !self.matches(arena, id) {
            return false;
        }
        let Some((parent, list_index, pos, _)) = position_in_parent(arena, id) else {
            return false;
        };
        let Some(parent_kind) = arena.get_mut(parent) else {
            return false;
        };
        let mut lists = parent_kind.child_lists_mut();
        let Some(list) = lists.get_mut(list_index) else {
            return false;
        };
        let moved: Vec<NodeId> = list.split_off(pos + 1);
        let Some(NodeKind::If {
            then_body,
            else_body,
            ..
        }) = arena.get_mut(id)
        else {
            return false;
        };
        let dropped = then_body.pop();
        *else_body = moved.clone();
        if let Some(dropped) = dropped {
            arena.take(dropped);
        }
        for child in moved {
            arena.set_parent(child, Some(id));
        }
        true
    }
}

impl NodeRule for TailBreakDrop {
    fn name(&self) -> &'static str {
        "label_elide::tail_break_drop"
    }

    fn priority(&self) -> u32 {
        6
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Break]
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        let Some(NodeKind::Break(Some(label))) = arena.get(id) else {
            return false;
        };
        let Some((parent, _, pos, len)) = position_in_parent(arena, id) else {
            return false;
        };
        pos + 1 == len && reaches_label_end(arena, parent, label)
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        if !self.matches(arena, id) {
            return false;
        }
        let Some((parent, list_index, pos, _)) = position_in_parent(arena, id) else {
            return false;
        };
        let Some(parent_kind) = arena.get_mut(parent) else {
            return false;
        };
        let mut lists = parent_kind.child_lists_mut();
        let Some(list) = lists.get_mut(list_index) else {
            return false;
        };
        list.remove(pos);
        arena.take(id);
        true
    }
}

impl NodeRule for LabelElide {
    fn name(&self) -> &'static str {
        "label_elide::drop_label"
    }

    fn priority(&self) -> u32 {
        7
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::LabeledBlock]
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        let Some(NodeKind::LabeledBlock { label, body }) = arena.get(id) else {
            return false;
        };
        !body
            .iter()
            .any(|&child| subtree_breaks(arena, child, label))
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        if !self.matches(arena, id) {
            return false;
        }
        let Some(NodeKind::LabeledBlock { body, .. }) = arena.get(id) else {
            return false;
        };
        let body = body.clone();
        arena.set_kind(id, NodeKind::Scope { body });
        true
    }
}
