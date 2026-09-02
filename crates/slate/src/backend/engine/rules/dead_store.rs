use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, RustValue};

use super::inline_temps::expr_effects;

fn is_dead_let(arena: &Arena, id: NodeId) -> bool {
    matches!(
        arena.get(id),
        Some(NodeKind::Let { name, init: Some(_), .. }) if arena.def_use_neighbors(*name).is_empty()
    )
}

pub(in crate::backend::engine) struct DeadStore;

impl NodeRule for DeadStore {
    fn name(&self) -> &'static str {
        "dead_store::eliminate"
    }

    fn priority(&self) -> u32 {
        60
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Let]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        is_dead_let(arena, id)
    }

    fn requeues_producers(&self) -> bool {
        true
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if !is_dead_let(arena, id) {
            return false;
        }
        let Some(NodeKind::Let {
            init: Some(init), ..
        }) = arena.get(id)
        else {
            return false;
        };

        if expr_effects(init).is_side_effect() {
            let Some(NodeKind::Let {
                init: Some(init), ..
            }) = arena.get_mut(id)
            else {
                return false;
            };
            let init = std::mem::replace(init, Expr::Value(RustValue::I64(0)));
            arena.set_kind(id, NodeKind::Expr(init));
            return true;
        }

        let Some(parent) = arena.parent(id) else {
            return false;
        };
        let Some(parent_kind) = arena.get(parent) else {
            return false;
        };
        let lists = parent_kind.child_lists();
        let Some((list_index, pos)) = lists.iter().enumerate().find_map(|(list_index, list)| {
            list.iter()
                .position(|&child| child == id)
                .map(|pos| (list_index, pos))
        }) else {
            return false;
        };

        let _ = arena.take(id);
        if let Some(parent_kind) = arena.get_mut(parent)
            && let Some(list) = parent_kind.child_lists_mut().get_mut(list_index)
        {
            list.remove(pos);
        }
        true
    }
}
