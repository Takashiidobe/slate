use super::walk::kind_has_stmt_exprs;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::Label;

pub(in crate::backend::engine) struct BreakToElse;
pub(in crate::backend::engine) struct TailBreakDrop;
pub(in crate::backend::engine) struct LoopTailBreak;
pub(in crate::backend::engine) struct LabelElide;
pub(in crate::backend::engine) struct LoopLabelElide;

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

fn enclosing_loop(arena: &Arena, id: NodeId) -> Option<NodeId> {
    let mut current = arena.parent(id)?;
    loop {
        match arena.get(current)? {
            NodeKind::Loop { .. } | NodeKind::While { .. } | NodeKind::For { .. } => {
                return Some(current);
            }
            NodeKind::If { .. }
            | NodeKind::Match { .. }
            | NodeKind::Scope { .. }
            | NodeKind::Unsafe { tail: None, .. }
            | NodeKind::Block { tail: None, .. } => current = arena.parent(current)?,
            _ => return None,
        }
    }
}

fn loop_ends_label(arena: &Arena, looped: NodeId, label: &Label) -> bool {
    let Some(parent) = arena.parent(looped) else {
        return false;
    };
    let Some(kind) = arena.get(parent) else {
        return false;
    };
    kind.child_lists()
        .iter()
        .any(|list| list.last() == Some(&looped))
        && reaches_label_end(arena, parent, label)
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

fn loop_label_jumps(
    arena: &Arena,
    id: NodeId,
    label: &Label,
    nested: bool,
    jumps: &mut Vec<NodeId>,
) -> bool {
    let Some(kind) = arena.get(id) else {
        return true;
    };
    if kind_has_stmt_exprs(kind) {
        return false;
    }
    match kind {
        NodeKind::Break(Some(target)) | NodeKind::Continue(Some(target)) if target == label => {
            jumps.push(id);
            return !nested;
        }
        _ => {}
    }
    let nested = nested
        || matches!(
            kind,
            NodeKind::Loop { .. }
                | NodeKind::While { .. }
                | NodeKind::For { .. }
                | NodeKind::LabeledBlock { .. }
        );
    kind.child_lists()
        .into_iter()
        .flatten()
        .all(|&child| loop_label_jumps(arena, child, label, nested, jumps))
}

fn unlabeled_loop_jumps(arena: &Arena, id: NodeId) -> Option<Vec<NodeId>> {
    let kind = arena.get(id)?;
    let (NodeKind::Loop {
        label: Some(label), ..
    }
    | NodeKind::While {
        label: Some(label), ..
    }
    | NodeKind::For {
        label: Some(label), ..
    }) = kind
    else {
        return None;
    };
    if kind_has_stmt_exprs(kind) {
        return None;
    }
    let mut jumps = Vec::new();
    kind.child_lists()
        .into_iter()
        .flatten()
        .all(|&child| loop_label_jumps(arena, child, label, false, &mut jumps))
        .then_some(jumps)
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

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(label) = escaping_break(arena, id) else {
            return false;
        };
        let Some((parent, _, pos, len)) = position_in_parent(arena, id) else {
            return false;
        };
        pos + 1 < len && reaches_label_end(arena, parent, &label)
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
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

    fn requeues_moved_nodes(&self) -> bool {
        true
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::Break(Some(label))) = arena.get(id) else {
            return false;
        };
        let Some((parent, _, pos, len)) = position_in_parent(arena, id) else {
            return false;
        };
        pos + 1 == len && reaches_label_end(arena, parent, label)
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if !self.matches(arena, id) {
            return false;
        }
        arena.retire(id, None);
        true
    }
}

impl NodeRule for LoopTailBreak {
    fn name(&self) -> &'static str {
        "label_elide::loop_tail_break"
    }

    fn priority(&self) -> u32 {
        6
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Break]
    }

    fn requeues_moved_nodes(&self) -> bool {
        true
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::Break(Some(label))) = arena.get(id) else {
            return false;
        };
        enclosing_loop(arena, id).is_some_and(|looped| loop_ends_label(arena, looped, label))
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if !self.matches(arena, id) {
            return false;
        }
        if let Some(NodeKind::Break(label)) = arena.get_mut(id) {
            *label = None;
        }
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

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::LabeledBlock { label, body }) = arena.get(id) else {
            return false;
        };
        !body
            .iter()
            .any(|&child| subtree_breaks(arena, child, label))
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
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

impl NodeRule for LoopLabelElide {
    fn name(&self) -> &'static str {
        "label_elide::drop_loop_label"
    }

    fn priority(&self) -> u32 {
        8
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Loop, NodeKindTag::While, NodeKindTag::For]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        unlabeled_loop_jumps(arena, id).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(jumps) = unlabeled_loop_jumps(arena, id) else {
            return false;
        };
        for jump in jumps {
            if let Some(NodeKind::Break(label) | NodeKind::Continue(label)) = arena.get_mut(jump) {
                *label = None;
            }
        }
        if let Some(
            NodeKind::Loop { label, .. }
            | NodeKind::While { label, .. }
            | NodeKind::For { label, .. },
        ) = arena.get_mut(id)
        {
            *label = None;
        }
        true
    }
}
