use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, RustValue, UnaryOp};

fn is_true(expr: &Expr) -> bool {
    matches!(expr, Expr::Value(RustValue::Bool(true)))
}

fn is_false(expr: &Expr) -> bool {
    matches!(expr, Expr::Value(RustValue::Bool(false)))
}

fn resolve_tail(arena: &FunctionOptimizer, body: &[NodeId], value: &Expr) -> Option<Expr> {
    match body {
        [] => Some(value.clone()),
        [only] => {
            let Expr::Var(tail_name) = value else {
                return None;
            };
            let Some(NodeKind::Let {
                name,
                mutable: false,
                init: Some(init),
                ..
            }) = arena.get(*only)
            else {
                return None;
            };
            (name == tail_name).then(|| init.clone())
        }
        _ => None,
    }
}

enum Fold {
    Cond,
    NotCond,
    Or,
    And,
}

fn classify(then_value: &Expr, else_value: &Expr) -> Option<Fold> {
    if is_true(then_value) && is_false(else_value) {
        Some(Fold::Cond)
    } else if is_false(then_value) && is_true(else_value) {
        Some(Fold::NotCond)
    } else if is_true(then_value) {
        Some(Fold::Or)
    } else if is_false(else_value) {
        Some(Fold::And)
    } else {
        None
    }
}

fn build(fold: Fold, cond: Expr, then_value: Expr, else_value: Expr) -> Expr {
    match fold {
        Fold::Cond => cond,
        Fold::NotCond => Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(cond),
        },
        Fold::Or => Expr::Binary {
            op: BinOp::Or,
            lhs: Box::new(cond),
            rhs: Box::new(else_value),
        },
        Fold::And => Expr::Binary {
            op: BinOp::And,
            lhs: Box::new(cond),
            rhs: Box::new(then_value),
        },
    }
}

pub(in crate::backend::engine) struct BoolTernaryFold;

impl NodeRule for BoolTernaryFold {
    fn name(&self) -> &'static str {
        "bool_ternary::fold"
    }

    fn priority(&self) -> u32 {
        41
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::LetIf]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::LetIf {
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        }) = arena.get(id)
        else {
            return false;
        };
        let Some(then_resolved) = resolve_tail(arena, then_body, then_value) else {
            return false;
        };
        let Some(else_resolved) = resolve_tail(arena, else_body, else_value) else {
            return false;
        };
        classify(&then_resolved, &else_resolved).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::LetIf {
            name,
            mutable,
            ty,
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
        }) = arena.get(id)
        else {
            return false;
        };
        let Some(then_resolved) = resolve_tail(arena, then_body, then_value) else {
            return false;
        };
        let Some(else_resolved) = resolve_tail(arena, else_body, else_value) else {
            return false;
        };
        let Some(fold) = classify(&then_resolved, &else_resolved) else {
            return false;
        };
        let name = *name;
        let mutable = *mutable;
        let ty = ty.clone();
        let cond = cond.clone();
        let dead_stmts: Vec<NodeId> = then_body.iter().chain(else_body).copied().collect();

        let init = build(fold, cond, then_resolved, else_resolved);
        for stmt in dead_stmts {
            arena.discard_subtree(stmt);
        }
        arena.set_kind(
            id,
            NodeKind::Let {
                name,
                mutable,
                ty,
                init: Some(init),
            },
        );
        true
    }
}
