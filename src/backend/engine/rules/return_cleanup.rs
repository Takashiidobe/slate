use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Ident, Type};

fn position(arena: &Arena, id: NodeId) -> Option<(NodeId, usize, usize)> {
    let parent = arena.parent(id)?;
    arena
        .get(parent)?
        .child_lists()
        .iter()
        .enumerate()
        .find_map(|(list_index, list)| {
            list.iter()
                .position(|&child| child == id)
                .map(|pos| (parent, list_index, pos))
        })
}

fn copy_type(ty: &Type) -> bool {
    match ty {
        Type::Prim(_)
        | Type::LongDouble
        | Type::CLib(_)
        | Type::Ref { .. }
        | Type::Ptr { .. }
        | Type::FnPtr { .. }
        | Type::Unit
        | Type::Never => true,
        Type::Complex(inner) | Type::Array { elem: inner, .. } => copy_type(inner),
        _ => false,
    }
}

fn synthetic_slot(arena: &Arena, name: Ident) -> bool {
    if name.as_str() != "__retval"
        || !matches!(
            arena.definition(name).and_then(|id| arena.get(id)),
            Some(NodeKind::Let { name: declared, .. }) if *declared == name
        )
    {
        return false;
    }
    arena
        .def_use_neighbors(name)
        .iter()
        .all(|&use_id| match arena.get(use_id) {
            Some(NodeKind::Assign {
                target: Expr::Var(target),
                value,
            }) => *target == name && !value.reads_var(name.as_str()),
            Some(NodeKind::Return(Some(Expr::Var(returned)))) => *returned == name,
            Some(NodeKind::Let {
                name: temp,
                mutable: false,
                init: Some(Expr::Var(source)),
                ..
            }) if *source == name => matches!(
                arena.def_use_neighbors(*temp),
                [return_id]
                    if matches!(
                        arena.get(*return_id),
                        Some(NodeKind::Return(Some(Expr::Var(returned)))) if *returned == *temp
                    )
            ),
            _ => false,
        })
}

fn preceding_slot_store(arena: &Arena, id: NodeId) -> Option<(NodeId, usize, usize, Expr)> {
    let NodeKind::Return(Some(Expr::Var(name))) = arena.get(id)? else {
        return None;
    };
    if !synthetic_slot(arena, *name) {
        return None;
    }
    let (parent, list_index, pos) = position(arena, id)?;
    let store = *arena
        .get(parent)?
        .child_lists()
        .get(list_index)?
        .get(pos.checked_sub(1)?)?;
    let NodeKind::Assign {
        target: Expr::Var(target),
        value,
    } = arena.get(store)?
    else {
        return None;
    };
    (*target == *name).then(|| (store, list_index, pos, value.clone()))
}

fn final_return(arena: &Arena, id: NodeId) -> bool {
    let Some((parent, list_index, pos)) = position(arena, id) else {
        return false;
    };
    arena.parent(parent).is_none()
        && arena
            .get(parent)
            .and_then(|kind| {
                kind.child_lists()
                    .get(list_index)
                    .map(|list| pos + 1 == list.len())
            })
            .unwrap_or(false)
}

pub(in crate::backend::engine) struct ReturnSlotFold;

impl NodeRule for ReturnSlotFold {
    fn name(&self) -> &'static str {
        "return_cleanup::slot"
    }

    fn priority(&self) -> u32 {
        40
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Return]
    }

    fn requeues_producers(&self) -> bool {
        true
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        arena.return_type().is_some_and(copy_type) && preceding_slot_store(arena, id).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if !arena.return_type().is_some_and(copy_type) {
            return false;
        }
        if let Some((store, list_index, pos, value)) = preceding_slot_store(arena, id) {
            let Some(parent) = arena.parent(id) else {
                return false;
            };
            arena.set_kind(id, NodeKind::Return(Some(value)));
            let _ = arena.take(store);
            if let Some(list) = arena
                .get_mut(parent)
                .and_then(|kind| kind.child_lists_mut().into_iter().nth(list_index))
            {
                list.remove(pos - 1);
            }
            return true;
        }
        false
    }
}

pub(in crate::backend::engine) struct FinalReturnTail;

impl NodeRule for FinalReturnTail {
    fn name(&self) -> &'static str {
        "return_cleanup::tail"
    }

    fn priority(&self) -> u32 {
        90
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Return]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        arena.return_type().is_some_and(copy_type) && final_return(arena, id)
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if !self.matches(arena, id) {
            return false;
        }
        let Some(NodeKind::Return(Some(expr))) = arena.get(id) else {
            return false;
        };
        let expr = expr.clone();
        arena.set_kind(id, NodeKind::Expr(expr));
        true
    }
}
