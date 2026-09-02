use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};

pub(in crate::backend::engine) struct ScopeFlatten;

impl NodeRule for ScopeFlatten {
    fn name(&self) -> &'static str {
        "singleton_scopes::flatten"
    }

    fn priority(&self) -> u32 {
        10
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Scope]
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        arena.parent(id).is_some()
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(parent_id) = arena.parent(id) else {
            return false;
        };
        let Some(NodeKind::Scope { body: children }) = arena.take(id) else {
            return false;
        };
        for &child in &children {
            arena.set_parent(child, Some(parent_id));
        }
        let Some(parent_kind) = arena.get_mut(parent_id) else {
            return false;
        };
        for list in parent_kind.child_lists_mut() {
            if let Some(index) = list.iter().position(|&x| x == id) {
                list.splice(index..=index, children);
                return true;
            }
        }
        false
    }
}
