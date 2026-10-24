use std::collections::HashMap;

use super::constant_index_casts::representable_literal;
use super::walk;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, Ident, Prim, RustValue, Type, UnaryOp};

type VarTypes = HashMap<Ident, Type>;

fn is_comparison(op: BinOp) -> bool {
    matches!(
        op,
        BinOp::Eq | BinOp::Ne | BinOp::Lt | BinOp::Le | BinOp::Gt | BinOp::Ge
    )
}

fn is_int_prim(prim: Prim) -> bool {
    !matches!(
        prim,
        Prim::Bool | Prim::F16 | Prim::F32 | Prim::F64 | Prim::F128
    )
}

fn is_bool(expr: &Expr, vars: &VarTypes) -> bool {
    match expr {
        Expr::Binary { op, .. } => is_comparison(*op) || matches!(op, BinOp::And | BinOp::Or),
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => is_bool(expr, vars),
        Expr::Value(RustValue::Bool(_)) => true,
        Expr::Var(name) => matches!(vars.get(name), Some(Type::Prim(Prim::Bool))),
        Expr::MethodCall { method, args, .. } => method == "is_null" && args.is_empty(),
        _ => false,
    }
}

fn int_literal(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::TypedInt(value, _)) => Some(*value),
        Expr::Value(RustValue::TypedUInt(value, _)) => i128::try_from(*value).ok(),
        _ => match representable_literal(expr)? {
            Expr::Value(RustValue::I64(value)) => Some(value as i128),
            Expr::Value(RustValue::I128(value)) => Some(value),
            Expr::Value(RustValue::U128(value)) => i128::try_from(value).ok(),
            _ => None,
        },
    }
}

fn is_integral(expr: &Expr, vars: &VarTypes) -> bool {
    match expr {
        Expr::Var(name) => matches!(vars.get(name), Some(Type::Prim(prim)) if is_int_prim(*prim)),
        Expr::Cast {
            ty: Type::Prim(prim),
            ..
        } => is_int_prim(*prim),
        _ => int_literal(expr).is_some(),
    }
}

fn inverse(op: BinOp) -> BinOp {
    match op {
        BinOp::Eq => BinOp::Ne,
        BinOp::Ne => BinOp::Eq,
        BinOp::Lt => BinOp::Ge,
        BinOp::Le => BinOp::Gt,
        BinOp::Gt => BinOp::Le,
        BinOp::Ge => BinOp::Lt,
        other => other,
    }
}

fn fold_literals(op: BinOp, lhs: i128, rhs: i128) -> bool {
    match op {
        BinOp::Eq => lhs == rhs,
        BinOp::Ne => lhs != rhs,
        BinOp::Lt => lhs < rhs,
        BinOp::Le => lhs <= rhs,
        BinOp::Gt => lhs > rhs,
        _ => lhs >= rhs,
    }
}

fn take(expr: &mut Expr) -> Expr {
    std::mem::replace(expr, Expr::Todo(String::new()))
}

fn bool_operand_of_zero_compare(expr: &Expr, vars: &VarTypes) -> Option<bool> {
    let Expr::Binary { op, lhs, rhs } = expr else {
        return None;
    };
    let negated = match op {
        BinOp::Ne => false,
        BinOp::Eq => true,
        _ => return None,
    };
    if int_literal(rhs) != Some(0) {
        return None;
    }
    let Expr::Cast {
        expr: inner,
        ty: Type::Prim(prim),
    } = &**lhs
    else {
        return None;
    };
    (is_int_prim(*prim) && is_bool(inner, vars)).then_some(negated)
}

fn rewritable(expr: &Expr, vars: &VarTypes) -> bool {
    match expr {
        Expr::Cast {
            expr: inner,
            ty: Type::Prim(Prim::Bool),
        } => matches!(**inner, Expr::Value(RustValue::Bool(_))),
        Expr::Binary { op, lhs, rhs } if is_comparison(*op) => {
            bool_operand_of_zero_compare(expr, vars).is_some()
                || (int_literal(lhs).is_some() && int_literal(rhs).is_some())
        }
        Expr::Unary {
            op: UnaryOp::Not,
            expr: inner,
        } => match &**inner {
            Expr::Value(RustValue::Bool(_)) => true,
            Expr::Unary {
                op: UnaryOp::Not,
                expr: innermost,
            } => is_bool(innermost, vars),
            Expr::Binary { op, lhs, rhs } if is_comparison(*op) => {
                matches!(op, BinOp::Eq | BinOp::Ne)
                    || is_integral(lhs, vars)
                    || is_integral(rhs, vars)
            }
            _ => false,
        },
        _ => false,
    }
}

fn rewrite_here(expr: &mut Expr, vars: &VarTypes) -> bool {
    if !rewritable(expr, vars) {
        return false;
    }
    match take(expr) {
        Expr::Cast { expr: inner, .. } => *expr = *inner,
        Expr::Binary { op, lhs, rhs } => {
            if let (Some(l), Some(r)) = (int_literal(&lhs), int_literal(&rhs)) {
                *expr = Expr::Value(RustValue::Bool(fold_literals(op, l, r)));
            } else {
                let negated = bool_operand_of_zero_compare(
                    &Expr::Binary {
                        op,
                        lhs: lhs.clone(),
                        rhs: rhs.clone(),
                    },
                    vars,
                )
                .unwrap_or(false);
                let Expr::Cast { expr: inner, .. } = *lhs else {
                    unreachable!();
                };
                *expr = if negated {
                    Expr::Unary {
                        op: UnaryOp::Not,
                        expr: inner,
                    }
                } else {
                    *inner
                };
            }
        }
        Expr::Unary { expr: inner, .. } => match *inner {
            Expr::Value(RustValue::Bool(value)) => *expr = Expr::Value(RustValue::Bool(!value)),
            Expr::Unary {
                expr: innermost, ..
            } => *expr = *innermost,
            Expr::Binary { op, lhs, rhs } => {
                *expr = Expr::Binary {
                    op: inverse(op),
                    lhs,
                    rhs,
                }
            }
            _ => unreachable!(),
        },
        _ => unreachable!(),
    }
    true
}

fn read_var_types(arena: &FunctionOptimizer, id: NodeId) -> VarTypes {
    arena
        .reads(id)
        .iter()
        .filter_map(|&name| arena.var_type(name).map(|ty| (name, ty.clone())))
        .collect()
}

pub(in crate::backend::engine) struct BoolSimplify;

impl NodeRule for BoolSimplify {
    fn name(&self) -> &'static str {
        "bool_simplify"
    }

    fn priority(&self) -> u32 {
        45
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        NodeKindTag::ALL_EXPRS
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let vars = read_var_types(arena, id);
        let collapsible_here = |expr: &Expr| rewritable(expr, &vars);
        arena.get(id).is_some_and(|kind| {
            let mut found = false;
            walk::visit_kind_exprs(kind, |expr| {
                found |= walk::any_collapsible(expr, &collapsible_here)
            });
            found
        })
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let vars = read_var_types(arena, id);
        let mut rewrite_here = |expr: &mut Expr| rewrite_here(expr, &vars);
        let Some(kind) = arena.get_mut(id) else {
            return false;
        };
        let mut changed = false;
        walk::visit_kind_exprs_mut(kind, |expr| {
            changed |= walk::fold_bottom_up(expr, &mut rewrite_here)
        });
        if !changed {
            return false;
        }
        arena.touch(id);
        true
    }
}
