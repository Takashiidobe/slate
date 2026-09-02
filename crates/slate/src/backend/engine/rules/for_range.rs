use super::inline_temps;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, Ident, RustValue, UnaryOp};

struct RecoveredFor {
    ind_var: Ident,
    start: Expr,
    end: Expr,
    mid_body: Vec<NodeId>,
    let_id: NodeId,
    let_parent: NodeId,
    let_list_index: usize,
    assign_start_id: NodeId,
    bound_let_id: Option<NodeId>,
    guard_if_id: NodeId,
    increment_id: NodeId,
}

fn owning_list(arena: &Arena, id: NodeId) -> Option<(NodeId, usize)> {
    let parent_id = arena.parent(id)?;
    let list_index = arena
        .get(parent_id)?
        .child_lists()
        .iter()
        .position(|list| list.contains(&id))?;
    Some((parent_id, list_index))
}

fn remove_from_list(arena: &mut Arena, parent: NodeId, list_index: usize, id: NodeId) {
    if let Some(parent_kind) = arena.get_mut(parent)
        && let Some(list) = parent_kind.child_lists_mut().get_mut(list_index)
    {
        list.retain(|&x| x != id);
    }
}

fn int_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some(*n as i128),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Value(RustValue::TypedInt(n, _)) => Some(*n),
        _ => None,
    }
}

fn is_var(expr: &Expr, name: Ident) -> bool {
    matches!(expr, Expr::Var(v) if *v == name)
}

fn break_guard_cond(arena: &Arena, id: NodeId) -> Option<&Expr> {
    let Some(NodeKind::If {
        cond,
        then_body,
        else_body,
    }) = arena.get(id)
    else {
        return None;
    };
    if !else_body.is_empty() || then_body.len() != 1 {
        return None;
    }
    if !matches!(arena.get(then_body[0]), Some(NodeKind::Break(None))) {
        return None;
    }
    Some(cond)
}

fn extract_negated_lt(cond: &Expr, ind_var: Ident) -> Option<Expr> {
    let Expr::Unary {
        op: UnaryOp::Not,
        expr: inner,
    } = cond
    else {
        return None;
    };
    let Expr::Binary {
        op: BinOp::Lt,
        lhs,
        rhs,
    } = inner.as_ref()
    else {
        return None;
    };
    if !is_var(lhs, ind_var) {
        return None;
    }
    Some(rhs.as_ref().clone())
}

fn negated_lt_guard(arena: &Arena, id: NodeId, ind_var: Ident) -> Option<Expr> {
    extract_negated_lt(break_guard_cond(arena, id)?, ind_var)
}

fn negated_lt_guard_through_temp(
    arena: &Arena,
    guard_if_id: NodeId,
    temp_let_id: NodeId,
    ind_var: Ident,
) -> Option<Expr> {
    let Some(NodeKind::Let {
        name: tmp,
        mutable: false,
        init: Some(init),
        ..
    }) = arena.get(temp_let_id)
    else {
        return None;
    };
    let tmp = *tmp;
    if inline_temps::expr_effects(init).is_side_effect() {
        return None;
    }
    if arena.def_use_neighbors(tmp) != [guard_if_id] {
        return None;
    }
    let cond = break_guard_cond(arena, guard_if_id)?;
    let mut substituted = cond.clone();
    substituted.substitute_var(tmp.as_str(), init);
    extract_negated_lt(&substituted, ind_var)
}

fn is_unit_increment(arena: &Arena, id: NodeId, ind_var: Ident) -> bool {
    let Some(NodeKind::Assign { target, value }) = arena.get(id) else {
        return false;
    };
    if !is_var(target, ind_var) {
        return false;
    }
    let Expr::Binary {
        op: BinOp::Add,
        lhs,
        rhs,
    } = value
    else {
        return false;
    };
    is_var(lhs, ind_var) && int_value(rhs) == Some(1)
}

fn assigns_to(arena: &Arena, id: NodeId, name: Ident) -> bool {
    let Some(kind) = arena.get(id) else {
        return false;
    };
    let direct = match kind {
        NodeKind::Assign { target, .. } | NodeKind::CompoundAssign { target, .. } => {
            is_var(target, name)
        }
        NodeKind::Let {
            name: decl,
            init: Some(_),
            ..
        } => *decl == name,
        NodeKind::For { pat, .. } => *pat == name,
        _ => false,
    };
    direct
        || kind
            .child_lists()
            .into_iter()
            .flatten()
            .any(|&child| assigns_to(arena, child, name))
}

fn subtree_ids(arena: &Arena, id: NodeId, out: &mut Vec<NodeId>) {
    out.push(id);
    let Some(kind) = arena.get(id) else { return };
    for &child in kind.child_lists().into_iter().flatten() {
        subtree_ids(arena, child, out);
    }
}

fn plan(arena: &Arena, loop_id: NodeId) -> Option<RecoveredFor> {
    let Some(NodeKind::Loop { label: None, body }) = arena.get(loop_id) else {
        return None;
    };
    let increment_id = *body.last()?;
    let Some(NodeKind::Assign {
        target: Expr::Var(ind_var),
        ..
    }) = arena.get(increment_id)
    else {
        return None;
    };
    let ind_var = *ind_var;
    if !is_unit_increment(arena, increment_id, ind_var) {
        return None;
    }

    let (bound_let_id, guard_if_id, end, guard_end_index) =
        if let Some(end) = negated_lt_guard(arena, body[0], ind_var) {
            (None, body[0], end, 0usize)
        } else if body.len() >= 2
            && let Some(NodeKind::Let {
                name: tmp,
                init: Some(bound_init),
                ..
            }) = arena.get(body[0])
        {
            let tmp = *tmp;
            let bound_init = bound_init.clone();
            if let Some(end) = negated_lt_guard(arena, body[1], ind_var)
                && is_var(&end, tmp)
                && arena.def_use_neighbors(tmp) == [body[1]]
            {
                (Some(body[0]), body[1], bound_init, 1usize)
            } else {
                let end = negated_lt_guard_through_temp(arena, body[1], body[0], ind_var)?;
                (Some(body[0]), body[1], end, 1usize)
            }
        } else {
            return None;
        };

    if guard_end_index + 1 > body.len() - 1 {
        return None;
    }
    let mid_body = body[guard_end_index + 1..body.len() - 1].to_vec();
    if mid_body.iter().any(|&id| assigns_to(arena, id, ind_var)) {
        return None;
    }

    let parent_id = arena.parent(loop_id)?;
    let parent_kind = arena.get(parent_id)?;
    let (index, list) = parent_kind
        .child_lists()
        .into_iter()
        .find_map(|list| list.iter().position(|&x| x == loop_id).map(|i| (i, list)))?;
    if index < 1 {
        return None;
    }
    let assign_start_id = list[index - 1];
    let Some(NodeKind::Assign {
        target: Expr::Var(assign_name),
        value: start,
    }) = arena.get(assign_start_id)
    else {
        return None;
    };
    if *assign_name != ind_var {
        return None;
    }
    let start = start.clone();

    let let_id = arena.definition(ind_var)?;
    let Some(NodeKind::Let { name: decl, .. }) = arena.get(let_id) else {
        return None;
    };
    if *decl != ind_var || let_id == assign_start_id {
        return None;
    }
    let (let_parent, let_list_index) = owning_list(arena, let_id)?;

    let mut allowed = vec![let_id, assign_start_id];
    subtree_ids(arena, loop_id, &mut allowed);
    if !arena
        .def_use_neighbors(ind_var)
        .iter()
        .all(|n| allowed.contains(n))
    {
        return None;
    }

    Some(RecoveredFor {
        ind_var,
        start,
        end,
        mid_body,
        let_id,
        let_parent,
        let_list_index,
        assign_start_id,
        bound_let_id,
        guard_if_id,
        increment_id,
    })
}

pub(in crate::backend::engine) struct ForRangeRecover;

impl NodeRule for ForRangeRecover {
    fn name(&self) -> &'static str {
        "for_range::recover"
    }

    fn priority(&self) -> u32 {
        13
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Loop]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        plan(arena, id).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(plan) = plan(arena, id) else {
            return false;
        };
        let Some(parent_id) = arena.parent(id) else {
            return false;
        };
        let Some(parent_kind) = arena.get(parent_id) else {
            return false;
        };
        let Some(list_index) = parent_kind
            .child_lists()
            .iter()
            .position(|list| list.contains(&id))
        else {
            return false;
        };

        arena.take(plan.let_id);
        arena.take(plan.assign_start_id);
        if let Some(bound_let_id) = plan.bound_let_id {
            arena.take(bound_let_id);
        }
        arena.discard_subtree(plan.guard_if_id);
        arena.take(plan.increment_id);

        arena.set_kind(
            id,
            NodeKind::For {
                pat: plan.ind_var,
                iter: Expr::Range {
                    start: Box::new(plan.start),
                    end: Box::new(plan.end),
                },
                body: plan.mid_body,
            },
        );

        let Some(parent_kind) = arena.get_mut(parent_id) else {
            unreachable!("for_range: parent_id invalidated by taking an unrelated sibling slot")
        };
        let mut child_lists = parent_kind.child_lists_mut();
        let Some(list) = child_lists.get_mut(list_index) else {
            unreachable!("for_range: list_index no longer valid on parent_id")
        };
        list.retain(|&x| x != plan.assign_start_id);
        remove_from_list(arena, plan.let_parent, plan.let_list_index, plan.let_id);
        true
    }
}
