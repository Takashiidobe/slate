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

fn is_inert_init(expr: &Expr) -> bool {
    match expr {
        Expr::Unsafe(block) | Expr::Block(block) if block.stmts.is_empty() => {
            block.tail.as_deref().is_some_and(is_inert_init)
        }
        Expr::Call { func, args, .. } => match &**func {
            Expr::Var(name) if args.is_empty() => {
                matches!(
                    name.as_str(),
                    "std::mem::MaybeUninit::uninit" | "std::mem::zeroed"
                )
            }
            Expr::Var(name) if args.len() == 1 => {
                name.as_str().starts_with("__SlateAlign") && args.iter().all(is_inert_init)
            }
            _ => false,
        },
        _ => false,
    }
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

        if !is_inert_init(init) && expr_effects(init).is_side_effect() {
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

        arena.retire(id, None);
        true
    }
}
