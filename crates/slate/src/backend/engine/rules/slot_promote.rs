use super::for_range::subtree_ids;
use super::remove_mut::invariant_within;
use super::walk;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Block, Expr, Ident, Type, UnaryOp};
use crate::function_identity::CallBinding;

struct Slot {
    slot: Ident,
    storage: Ident,
    storage_id: NodeId,
    ty: Type,
}

fn zeroable(ty: &Type) -> bool {
    match ty {
        Type::Prim(_) | Type::Ptr { .. } => true,
        Type::Array { elem, .. } => zeroable(elem),
        _ => false,
    }
}

fn is_var(expr: &Expr, name: Ident) -> bool {
    matches!(expr, Expr::Var(v) if *v == name)
}

fn is_call_to(expr: &Expr, path: &str) -> bool {
    matches!(expr, Expr::Call { func, .. } if matches!(func.as_ref(), Expr::Var(v) if v.as_str() == path))
}

fn slot_write(expr: &Expr, slot: Ident) -> Option<&Expr> {
    match expr {
        Expr::Call { func, args, .. } if matches!(func.as_ref(), Expr::Var(v) if v.as_str() == "std::ptr::write") => {
            match &args[..] {
                [target, value] if is_var(target, slot) => Some(value),
                _ => None,
            }
        }
        _ => None,
    }
}

fn occurrences(expr: &Expr, slot: Ident) -> usize {
    usize::from(is_var(expr, slot))
        + walk::child_exprs(expr)
            .into_iter()
            .map(|child| occurrences(child, slot))
            .sum::<usize>()
}

fn derefs(expr: &Expr, slot: Ident) -> usize {
    match expr {
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } if is_var(expr, slot) => 1,
        _ => walk::child_exprs(expr)
            .into_iter()
            .map(|child| derefs(child, slot))
            .sum(),
    }
}

fn use_is_place(kind: &NodeKind, slot: Ident) -> bool {
    if matches!(kind, NodeKind::InlineAsm(_)) {
        return false;
    }
    if let NodeKind::Expr(expr) = kind
        && let Some(value) = slot_write(expr, slot)
    {
        return occurrences(value, slot) == derefs(value, slot);
    }
    let mut ok = true;
    walk::visit_kind_exprs(kind, |expr| {
        ok &= occurrences(expr, slot) == derefs(expr, slot)
    });
    ok
}

fn plan(arena: &FunctionOptimizer, id: NodeId) -> Option<Slot> {
    let Some(NodeKind::Let {
        name: slot,
        ty: Some(Type::Ptr {
            mutable: true,
            inner,
        }),
        init: Some(Expr::Cast { expr, .. }),
        ..
    }) = arena.get(id)
    else {
        return None;
    };
    if !slot.as_str().starts_with("__slate_slot_") || !zeroable(inner) {
        return None;
    }
    let Expr::AddrOf {
        mutable: true,
        expr: storage,
    } = expr.as_ref()
    else {
        return None;
    };
    let Expr::Var(storage) = **storage else {
        return None;
    };
    let storage_id = arena.definition(storage)?;
    let Some(NodeKind::Let {
        ty: Some(Type::Generic { name, args }),
        init: Some(init),
        ..
    }) = arena.get(storage_id)
    else {
        return None;
    };
    if name.as_str() != "std::mem::MaybeUninit"
        || !matches!(&args[..], [stored] if stored == inner.as_ref())
        || !is_call_to(init, "std::mem::MaybeUninit::uninit")
        || arena.def_use_neighbors(storage) != [id]
    {
        return None;
    }
    arena
        .def_use_neighbors(*slot)
        .iter()
        .all(|&user| {
            user != id
                && arena
                    .get(user)
                    .is_some_and(|kind| use_is_place(kind, *slot))
        })
        .then(|| Slot {
            slot: *slot,
            storage,
            storage_id,
            ty: (**inner).clone(),
        })
}

fn replace_derefs(expr: &mut Expr, slot: Ident, storage: Ident) -> bool {
    walk::fold_bottom_up(expr, &mut |expr| match expr {
        Expr::Unary {
            op: UnaryOp::Deref,
            expr: inner,
        } if is_var(inner, slot) => {
            *expr = Expr::Var(storage);
            true
        }
        _ => false,
    })
}

fn zeroed() -> Expr {
    Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            func: Box::new(Expr::Var("std::mem::zeroed".into())),
            args: Vec::new(),
            binding: CallBinding::Generated,
        })),
    }))
}

fn is_zeroed(expr: &Expr) -> bool {
    matches!(expr, Expr::Unsafe(block) if block.stmts.is_empty() && block.tail.as_deref().is_some_and(|tail| is_call_to(tail, "std::mem::zeroed")))
}

fn takes_address(expr: &Expr, name: Ident) -> bool {
    match expr {
        Expr::AddrOf { expr, .. } | Expr::Ref { expr, .. } if expr.reads_var(name.as_str()) => true,
        _ => walk::child_exprs(expr)
            .into_iter()
            .any(|child| takes_address(child, name)),
    }
}

fn kind_takes_address(kind: &NodeKind, name: Ident) -> bool {
    let mut found = matches!(kind, NodeKind::InlineAsm(_));
    walk::visit_kind_exprs(kind, |expr| found |= takes_address(expr, name));
    found
}

fn sole_assignment(arena: &FunctionOptimizer, id: NodeId) -> Option<(Ident, NodeId)> {
    let Some(NodeKind::Let {
        name,
        init: Some(init),
        ty: Some(_),
        ..
    }) = arena.get(id)
    else {
        return None;
    };
    if !is_zeroed(init) {
        return None;
    }
    let neighbors = arena.def_use_neighbors(*name);
    let mut assigns = neighbors.iter().copied().filter(|&user| {
        matches!(
            arena.get(user),
            Some(NodeKind::Assign { target, value }) if is_var(target, *name) && !value.reads_var(name.as_str())
        )
    });
    let assign = assigns.next()?;
    if assigns.next().is_some() {
        return None;
    }
    let parent = arena.parent(assign)?;
    let later = arena
        .get(parent)?
        .child_lists()
        .into_iter()
        .find_map(|list| {
            let pos = list.iter().position(|&child| child == assign)?;
            Some(list[pos + 1..].to_vec())
        })?;
    let mut region = Vec::new();
    for sibling in later {
        subtree_ids(arena, sibling, &mut region);
    }
    let readers: Vec<NodeId> = neighbors
        .iter()
        .copied()
        .filter(|&user| user != assign)
        .collect();
    (readers.iter().all(|&reader| {
        region.contains(&reader)
            && arena
                .get(reader)
                .is_some_and(|kind| !kind_takes_address(kind, *name))
    }) && invariant_within(arena, *name, &readers))
    .then_some((*name, assign))
}

pub(in crate::backend::engine) struct DeclSink;

impl NodeRule for DeclSink {
    fn name(&self) -> &'static str {
        "slot_promote::sink_decl"
    }

    fn priority(&self) -> u32 {
        4
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Let]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        sole_assignment(arena, id).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some((name, assign)) = sole_assignment(arena, id) else {
            return false;
        };
        let Some(NodeKind::Let { ty, .. }) = arena.get(id) else {
            return false;
        };
        let ty = ty.clone();
        let Some(NodeKind::Assign { value, .. }) = arena.get(assign) else {
            return false;
        };
        let value = value.clone();
        arena.retire(id, Some(assign));
        arena.set_kind(
            assign,
            NodeKind::Let {
                name,
                mutable: true,
                ty,
                init: Some(value),
            },
        );
        true
    }
}

pub(in crate::backend::engine) struct SlotPromote;

impl NodeRule for SlotPromote {
    fn name(&self) -> &'static str {
        "slot_promote"
    }

    fn priority(&self) -> u32 {
        3
    }

    fn requeues_producers(&self) -> bool {
        true
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Let]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        plan(arena, id).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(plan) = plan(arena, id) else {
            return false;
        };
        let users = arena.def_use_neighbors(plan.slot).to_vec();
        for user in users {
            if let Some(NodeKind::Expr(expr)) = arena.get(user)
                && let Some(value) = slot_write(expr, plan.slot)
            {
                let value = value.clone();
                arena.set_kind(
                    user,
                    NodeKind::Assign {
                        target: Expr::Var(plan.storage),
                        value,
                    },
                );
            }
            if let Some(kind) = arena.get_mut(user) {
                walk::visit_kind_exprs_mut(kind, |expr| {
                    replace_derefs(expr, plan.slot, plan.storage);
                });
            }
            arena.touch(user);
        }
        arena.set_kind(
            plan.storage_id,
            NodeKind::Let {
                name: plan.storage,
                mutable: true,
                ty: Some(plan.ty),
                init: Some(zeroed()),
            },
        );
        arena.retire(id, None);
        true
    }
}
