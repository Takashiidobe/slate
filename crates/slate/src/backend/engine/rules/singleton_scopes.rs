use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};

fn later_siblings(arena: &Arena, id: NodeId) -> Option<(NodeId, Vec<NodeId>)> {
    let parent = arena.parent(id)?;
    let lists = arena.get(parent)?.child_lists();
    let list = lists.into_iter().find(|list| list.contains(&id))?;
    let pos = list.iter().position(|&child| child == id)?;
    Some((parent, list[pos + 1..].to_vec()))
}

fn shadow_outlives_scope(arena: &Arena, id: NodeId) -> bool {
    let Some(NodeKind::Scope { body }) = arena.get(id) else {
        return false;
    };
    let Some((parent, later)) = later_siblings(arena, id) else {
        return false;
    };
    body.iter()
        .filter_map(|&child| arena.get(child).and_then(NodeKind::declared_name))
        .any(|name| {
            arena.def_use_neighbors(name).iter().any(|&reader| {
                if reader == parent {
                    return true;
                }
                let mut node = reader;
                while let Some(up) = arena.parent(node) {
                    if up == parent {
                        return later.contains(&node);
                    }
                    node = up;
                }
                false
            })
        })
}

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

    fn requeues_moved_nodes(&self) -> bool {
        true
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        arena.parent(id).is_some() && !shadow_outlives_scope(arena, id)
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if shadow_outlives_scope(arena, id) {
            return false;
        }
        let Some(parent_id) = arena.parent(id) else {
            return false;
        };
        arena.release_comments(id, None);
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
