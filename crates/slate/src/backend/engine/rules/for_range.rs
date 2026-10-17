use super::inline_temps;
use super::remove_mut::{binding_invariant_within, invariant_within};
use super::walk::kind_has_stmt_exprs;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, Ident, Prim, RustValue, Type, UnaryOp};
use std::collections::HashSet;

struct RecoveredFor {
    ind_var: Ident,
    start: Expr,
    end: Expr,
    body: Vec<NodeId>,
    start_id: NodeId,
    decl_id: Option<NodeId>,
    increment_id: NodeId,
}

pub(super) fn int_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some(*n as i128),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Value(RustValue::TypedInt(n, _)) => Some(*n),
        Expr::Value(RustValue::Usize(n)) => i128::try_from(*n).ok(),
        Expr::Value(RustValue::U128(n) | RustValue::TypedUInt(n, _)) => i128::try_from(*n).ok(),
        Expr::Cast { expr, .. } => int_value(expr),
        _ => None,
    }
}

fn is_var(expr: &Expr, name: Ident) -> bool {
    matches!(expr, Expr::Var(v) if *v == name)
}

fn is_unit_increment(arena: &Arena, id: NodeId, ind_var: Ident) -> bool {
    match arena.get(id) {
        Some(NodeKind::CompoundAssign {
            target,
            op: BinOp::Add,
            value,
        }) => is_var(target, ind_var) && int_value(value) == Some(1),
        Some(NodeKind::Assign {
            target,
            value:
                Expr::Binary {
                    op: BinOp::Add,
                    lhs,
                    rhs,
                },
        }) => is_var(target, ind_var) && is_var(lhs, ind_var) && int_value(rhs) == Some(1),
        Some(NodeKind::Assign {
            target,
            value: Expr::MethodCall { recv, method, args },
        }) => {
            is_var(target, ind_var)
                && is_var(recv, ind_var)
                && method.as_str() == "wrapping_add"
                && matches!(&args[..], [one] if int_value(one) == Some(1))
        }
        _ => false,
    }
}

fn is_integer(ty: Option<&Type>) -> bool {
    matches!(ty, Some(Type::Prim(prim)) if !matches!(prim, Prim::Bool | Prim::F16 | Prim::F32 | Prim::F64 | Prim::F128))
}

fn continues_here(arena: &Arena, id: NodeId) -> bool {
    let Some(kind) = arena.get(id) else {
        return false;
    };
    match kind {
        NodeKind::Continue(_) => true,
        NodeKind::Loop { .. } | NodeKind::While { .. } | NodeKind::For { .. } => false,
        _ => {
            kind_has_stmt_exprs(kind)
                || kind
                    .child_lists()
                    .into_iter()
                    .flatten()
                    .any(|&child| continues_here(arena, child))
        }
    }
}

pub(super) fn subtree_ids(arena: &Arena, id: NodeId, out: &mut Vec<NodeId>) {
    out.push(id);
    let Some(kind) = arena.get(id) else { return };
    for &child in kind.child_lists().into_iter().flatten() {
        subtree_ids(arena, child, out);
    }
}

fn loop_invariant(arena: &FunctionOptimizer, expr: &Expr, region: &[NodeId]) -> bool {
    match expr {
        Expr::Value(_) => true,
        Expr::Var(name) => invariant_within(arena, *name, region),
        Expr::Cast { expr, .. }
        | Expr::Unary {
            op: UnaryOp::Neg | UnaryOp::Not,
            expr,
        } => loop_invariant(arena, expr, region),
        Expr::Binary {
            op:
                BinOp::Add
                | BinOp::Sub
                | BinOp::Mul
                | BinOp::Div
                | BinOp::Rem
                | BinOp::BitAnd
                | BinOp::BitOr
                | BinOp::BitXor
                | BinOp::Shl
                | BinOp::Shr,
            lhs,
            rhs,
        } => loop_invariant(arena, lhs, region) && loop_invariant(arena, rhs, region),
        _ => false,
    }
}

fn earlier_siblings(arena: &Arena, id: NodeId) -> Option<Vec<NodeId>> {
    let parent = arena.get(arena.parent(id)?)?;
    parent.child_lists().into_iter().find_map(|list| {
        let index = list.iter().position(|&child| child == id)?;
        Some(list[..index].to_vec())
    })
}

fn plan(arena: &FunctionOptimizer, while_id: NodeId) -> Option<RecoveredFor> {
    let Some(NodeKind::While {
        label: None,
        cond:
            Expr::Binary {
                op: BinOp::Lt,
                lhs,
                rhs: end,
            },
        stmts,
        tail: None,
    }) = arena.get(while_id)
    else {
        return None;
    };
    let Expr::Var(ind_var) = **lhs else {
        return None;
    };
    let (&increment_id, body) = stmts.split_last()?;
    let mut body_region = Vec::new();
    for &id in body {
        subtree_ids(arena, id, &mut body_region);
    }
    if !is_unit_increment(arena, increment_id, ind_var)
        || body.iter().any(|&id| continues_here(arena, id))
    {
        return None;
    }

    let earlier = earlier_siblings(arena, while_id)?;
    let local_definition =
        earlier.iter().rev().copied().find(
            |&id| matches!(arena.get(id), Some(NodeKind::Let { name, .. }) if *name == ind_var),
        );
    let definition = local_definition.or_else(|| arena.definition(ind_var))?;
    let Some(NodeKind::Let { ty, init, .. }) = arena.get(definition) else {
        return None;
    };
    if !is_integer(ty.as_ref())
        || !binding_invariant_within(arena, ind_var, ty.as_ref(), &body_region)
    {
        return None;
    }
    let (start, start_id, decl_id) = match arena.get(*earlier.last()?)? {
        NodeKind::Assign {
            target: Expr::Var(name),
            value: start,
        } if *name == ind_var => {
            if init
                .as_ref()
                .is_some_and(|init| inline_temps::expr_effects(init).is_side_effect())
            {
                return None;
            }
            (start.clone(), *earlier.last()?, Some(definition))
        }
        _ => {
            let position = earlier.iter().position(|&id| id == definition)?;
            let start = init.as_ref()?;
            let mut between = Vec::new();
            for &id in &earlier[position + 1..] {
                subtree_ids(arena, id, &mut between);
            }
            if inline_temps::expr_effects(start).is_side_effect()
                || !loop_invariant(arena, start, &between)
            {
                return None;
            }
            (start.clone(), definition, None)
        }
    };

    let mut region = Vec::new();
    subtree_ids(arena, while_id, &mut region);
    if !loop_invariant(arena, end, &region) {
        return None;
    }
    let mut binding_scope = Vec::new();
    if let Some(local) = local_definition {
        let siblings = arena
            .get(arena.parent(while_id)?)?
            .child_lists()
            .into_iter()
            .find(|list| list.contains(&local))?;
        let position = siblings.iter().position(|&id| id == local)?;
        for &id in &siblings[position..] {
            subtree_ids(arena, id, &mut binding_scope);
        }
    }
    let binding_scope: HashSet<NodeId> = binding_scope.into_iter().collect();
    let outside = [Some(start_id), decl_id];
    if !arena.def_use_neighbors(ind_var).iter().all(|n| {
        region.contains(n)
            || outside.contains(&Some(*n))
            || (local_definition.is_some() && !binding_scope.contains(n))
    }) {
        return None;
    }

    Some(RecoveredFor {
        ind_var,
        start,
        end: (**end).clone(),
        body: body.to_vec(),
        start_id,
        decl_id,
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
        &[NodeKindTag::While]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        plan(arena, id).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(plan) = plan(arena, id) else {
            return false;
        };
        arena.retire(plan.start_id, Some(id));
        if let Some(decl_id) = plan.decl_id {
            arena.retire(decl_id, Some(id));
        }
        arena.retire(plan.increment_id, Some(id));
        arena.set_kind(
            id,
            NodeKind::For {
                pat: plan.ind_var,
                iter: Expr::Range {
                    start: Box::new(plan.start),
                    end: Box::new(plan.end),
                },
                body: plan.body,
            },
        );
        true
    }
}
