use super::walk;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, Ident, Prim, RustValue, Type, UnaryOp};

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

const INT_KINDS: &[NodeKindTag] = &[
    NodeKindTag::Let,
    NodeKindTag::Assign,
    NodeKindTag::CompoundAssign,
    NodeKindTag::Expr,
    NodeKindTag::Return,
    NodeKindTag::If,
    NodeKindTag::While,
    NodeKindTag::Match,
];

fn is_int(ty: &Type) -> bool {
    matches!(ty, Type::Prim(prim) if !matches!(prim, Prim::Bool | Prim::F16 | Prim::F32 | Prim::F64 | Prim::F128))
}

fn int_literal(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some((*n).into()),
        Expr::Value(RustValue::I128(n) | RustValue::TypedInt(n, _)) => Some(*n),
        Expr::Value(RustValue::U128(n) | RustValue::TypedUInt(n, _)) => i128::try_from(*n).ok(),
        Expr::Value(RustValue::Usize(n)) => i128::try_from(*n).ok(),
        Expr::Cast { expr, ty } if is_int(ty) => int_literal(expr),
        _ => None,
    }
}

fn is_bool(expr: &Expr, bool_vars: &[Ident]) -> bool {
    match expr {
        Expr::Value(RustValue::Bool(_)) => true,
        Expr::Cast {
            expr,
            ty: Type::Prim(Prim::Bool),
        } => is_bool(expr, bool_vars),
        Expr::Var(name) => bool_vars.contains(name),
        Expr::Binary { op, .. } => matches!(
            op,
            BinOp::Eq
                | BinOp::Ne
                | BinOp::Lt
                | BinOp::Le
                | BinOp::Gt
                | BinOp::Ge
                | BinOp::And
                | BinOp::Or
        ),
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => is_bool(expr, bool_vars),
        _ => false,
    }
}

fn bool_arm(expr: &Expr, bool_vars: &[Ident]) -> Option<Expr> {
    if let Some(value) = int_literal(expr) {
        return Some(Expr::Value(RustValue::Bool(value != 0)));
    }
    match expr {
        Expr::Cast { expr, ty } if is_int(ty) && is_bool(expr, bool_vars) => {
            Some(strip_bool_casts(expr).clone())
        }
        _ => None,
    }
}

fn strip_bool_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast {
            expr,
            ty: Type::Prim(Prim::Bool),
        } => strip_bool_casts(expr),
        other => other,
    }
}

fn int_ternary_test(expr: &Expr, bool_vars: &[Ident]) -> Option<Expr> {
    let Expr::Binary {
        op: op @ (BinOp::Ne | BinOp::Eq),
        lhs,
        rhs,
    } = expr
    else {
        return None;
    };
    let (ternary, zero) = match (lhs.as_ref(), rhs.as_ref()) {
        (ternary @ Expr::If { .. }, zero) | (zero, ternary @ Expr::If { .. }) => (ternary, zero),
        _ => return None,
    };
    if int_literal(zero) != Some(0) {
        return None;
    }
    let Expr::If {
        cond,
        then_expr,
        else_expr,
    } = ternary
    else {
        return None;
    };
    let then_value = bool_arm(then_expr, bool_vars)?;
    let else_value = bool_arm(else_expr, bool_vars)?;
    let fold = classify(&then_value, &else_value)?;
    let folded = build(fold, (**cond).clone(), then_value, else_value);
    Some(match op {
        BinOp::Eq => Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(folded),
        },
        _ => folded,
    })
}

fn bool_vars(arena: &FunctionOptimizer, id: NodeId) -> Vec<Ident> {
    arena
        .reads(id)
        .iter()
        .copied()
        .filter(|&name| matches!(arena.var_type(name), Some(Type::Prim(Prim::Bool))))
        .collect()
}

pub(in crate::backend::engine) struct BoolIntTernaryFold;

impl NodeRule for BoolIntTernaryFold {
    fn name(&self) -> &'static str {
        "bool_ternary::int_arms"
    }

    fn priority(&self) -> u32 {
        41
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        INT_KINDS
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(kind) = arena.get(id) else {
            return false;
        };
        let vars = bool_vars(arena, id);
        let mut found = false;
        walk::visit_kind_exprs(kind, |expr| {
            found |= walk::any_collapsible(expr, &|expr| int_ternary_test(expr, &vars).is_some())
        });
        found
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let vars = bool_vars(arena, id);
        let Some(kind) = arena.get_mut(id) else {
            return false;
        };
        let mut changed = false;
        walk::visit_kind_exprs_mut(kind, |expr| {
            changed |= walk::fold_bottom_up(expr, &mut |expr| match int_ternary_test(expr, &vars) {
                Some(folded) => {
                    *expr = folded;
                    true
                }
                None => false,
            })
        });
        if changed {
            arena.touch(id);
        }
        changed
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
            arena.retire_subtree(stmt, Some(id));
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
