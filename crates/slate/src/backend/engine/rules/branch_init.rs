use super::zero_init::{expr_is_pure_and_closed, node_touches_name};
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Ident, Type};

struct Plan {
    let_id: NodeId,
    then_assign: NodeId,
    else_assign: NodeId,
}

fn closing_assign(arena: &Arena, body: &[NodeId], name: Ident) -> Option<NodeId> {
    let (&last, earlier) = body.split_last()?;
    let Some(NodeKind::Assign {
        target: Expr::Var(target),
        value,
    }) = arena.get(last)
    else {
        return None;
    };
    (*target == name
        && !value.reads_var(name.as_str())
        && earlier
            .iter()
            .all(|&stmt| !node_touches_name(arena, stmt, name)))
    .then_some(last)
}

fn preceding_sibling(arena: &Arena, id: NodeId) -> Option<NodeId> {
    let parent = arena.parent(id)?;
    arena
        .get(parent)?
        .child_lists()
        .into_iter()
        .find_map(|list| {
            let pos = list.iter().position(|&child| child == id)?;
            list.get(pos.checked_sub(1)?).copied()
        })
}

fn plan(arena: &Arena, id: NodeId) -> Option<Plan> {
    let Some(NodeKind::If {
        cond,
        then_body,
        else_body,
    }) = arena.get(id)
    else {
        return None;
    };
    let let_id = preceding_sibling(arena, id)?;
    let Some(NodeKind::Let {
        name,
        ty: Some(_),
        init,
        ..
    }) = arena.get(let_id)
    else {
        return None;
    };
    if init
        .as_ref()
        .is_some_and(|init| !expr_is_pure_and_closed(init))
        || cond.reads_var(name.as_str())
    {
        return None;
    }
    Some(Plan {
        let_id,
        then_assign: closing_assign(arena, then_body, *name)?,
        else_assign: closing_assign(arena, else_body, *name)?,
    })
}

fn assigned_value(arena: &mut FunctionOptimizer, assign: NodeId, anchor: NodeId) -> Option<Expr> {
    match arena.retire(assign, Some(anchor))? {
        NodeKind::Assign { value, .. } => Some(value),
        _ => None,
    }
}

pub(in crate::backend::engine) struct BranchInitFold;

impl NodeRule for BranchInitFold {
    fn name(&self) -> &'static str {
        "branch_init::fold"
    }

    fn priority(&self) -> u32 {
        6
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::If]
    }

    fn requeues_moved_nodes(&self) -> bool {
        true
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        plan(arena, id).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(plan) = plan(arena, id) else {
            return false;
        };
        let let_id = plan.let_id;
        let Some(NodeKind::Let {
            name, mutable, ty, ..
        }) = arena.get(let_id)
        else {
            return false;
        };
        let (name, mutable, ty): (Ident, bool, Option<Type>) = (*name, *mutable, ty.clone());
        let Some(then_value) = assigned_value(arena, plan.then_assign, id) else {
            return false;
        };
        let Some(else_value) = assigned_value(arena, plan.else_assign, id) else {
            return false;
        };
        let Some(NodeKind::If {
            cond,
            then_body,
            else_body,
        }) = arena.get(id)
        else {
            return false;
        };
        let (cond, then_body, else_body) = (cond.clone(), then_body.clone(), else_body.clone());
        arena.retire(let_id, Some(id));
        arena.set_kind(
            id,
            NodeKind::LetIf {
                name,
                mutable,
                ty,
                cond,
                then_body,
                then_value,
                else_body,
                else_value,
            },
        );
        true
    }
}
