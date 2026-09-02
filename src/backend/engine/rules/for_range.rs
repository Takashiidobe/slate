use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, Ident, RustValue, UnaryOp};

struct RecoveredFor {
    ind_var: Ident,
    start: Expr,
    end: Expr,
    mid_body: Vec<NodeId>,
    let_id: NodeId,
    assign_start_id: NodeId,
    bound_let_id: Option<NodeId>,
    guard_if_id: NodeId,
    increment_id: NodeId,
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

fn negated_lt_guard(arena: &Arena, id: NodeId, ind_var: Ident) -> Option<Expr> {
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
            let end = negated_lt_guard(arena, body[1], ind_var)?;
            if !is_var(&end, tmp) {
                return None;
            }
            let neighbors = arena.def_use_neighbors(tmp);
            if neighbors != [body[1]] {
                return None;
            }
            (Some(body[0]), body[1], bound_init.clone(), 1usize)
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
    if index < 2 {
        return None;
    }
    let let_id = list[index - 2];
    let assign_start_id = list[index - 1];
    let Some(NodeKind::Let { name: decl, .. }) = arena.get(let_id) else {
        return None;
    };
    if *decl != ind_var {
        return None;
    }
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

        if let Some(parent_kind) = arena.get_mut(parent_id)
            && let Some(list) = parent_kind.child_lists_mut().get_mut(list_index)
        {
            list.retain(|&x| x != plan.let_id && x != plan.assign_start_id);
        }
        true
    }
}
