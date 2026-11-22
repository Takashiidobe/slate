use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Ident, LocalOrigin, Pattern, Type};

fn expr_is_pure_and_closed(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::LayoutQuery { .. } => true,
        Expr::Unary { expr, .. } | Expr::Cast { expr, .. } => expr_is_pure_and_closed(expr),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        } => expr_is_pure_and_closed(lhs) && expr_is_pure_and_closed(rhs),
        _ => false,
    }
}

fn kind_reads_var(kind: &NodeKind, name: Ident) -> bool {
    let name_str = name.as_str();
    match kind {
        NodeKind::Comment(_) => false,
        NodeKind::Let {
            name: declared,
            init,
            ..
        } => *declared == name || init.as_ref().is_some_and(|expr| expr.reads_var(name_str)),
        NodeKind::LetIf {
            name: declared,
            cond,
            then_value,
            else_value,
            ..
        } => {
            *declared == name
                || cond.reads_var(name_str)
                || then_value.reads_var(name_str)
                || else_value.reads_var(name_str)
        }
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            target.reads_var(name_str) || value.reads_var(name_str)
        }
        NodeKind::InlineAsm(asm) => {
            let mut found = false;
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| found |= expr.reads_var(name_str));
            }
            found
        }
        NodeKind::Expr(expr) => expr.reads_var(name_str),
        NodeKind::Return(expr) => expr.as_ref().is_some_and(|expr| expr.reads_var(name_str)),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            tail.as_ref().is_some_and(|tail| tail.reads_var(name_str))
        }
        NodeKind::While { cond, tail, .. } => {
            cond.reads_var(name_str) || tail.as_ref().is_some_and(|tail| tail.reads_var(name_str))
        }
        NodeKind::If { cond, .. } => cond.reads_var(name_str),
        NodeKind::For { pat, iter, .. } => *pat == name || iter.reads_var(name_str),
        NodeKind::Loop { .. } | NodeKind::Scope { .. } | NodeKind::LabeledBlock { .. } => false,
        NodeKind::Match { expr, arms } => expr.reads_var(name_str)
            || arms.iter().any(
                |arm| matches!(&arm.pattern, Pattern::Guarded { cond, .. } if cond.reads_var(name_str)),
            ),
        NodeKind::Break(_) | NodeKind::Continue(_) => false,
    }
}

fn node_touches_name(arena: &Arena, id: NodeId, name: Ident) -> bool {
    let Some(kind) = arena.get(id) else {
        return false;
    };
    kind_reads_var(kind, name)
        || kind.child_lists().iter().any(|list| {
            list.iter()
                .any(|&child| node_touches_name(arena, child, name))
        })
}

struct Plan {
    let_id: NodeId,
    then_assign: NodeId,
    else_assign: NodeId,
}

fn closing_assign(arena: &Arena, body: &[NodeId], name: Ident) -> Option<NodeId> {
    let (&last, earlier) = body.split_last()?;
    let Some(NodeKind::Assign {
        target: Expr::Var { name: target, .. },
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
        origin,
        ty: Some(_),
        init,
        ..
    }) = arena.get(let_id)
    else {
        return None;
    };
    let uninitialized = match origin {
        LocalOrigin::Synthetic => init.is_none(),
        LocalOrigin::Source => false,
        LocalOrigin::SourceUninitialized => init.as_ref().is_none_or(expr_is_pure_and_closed),
    };
    if !uninitialized || cond.reads_var(name.as_str()) {
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
