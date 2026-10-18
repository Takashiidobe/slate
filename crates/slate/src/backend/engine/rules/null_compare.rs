use std::collections::HashMap;

use super::walk;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, Ident, RustValue, Type, UnaryOp};

type VarTypes = HashMap<Ident, Type>;

fn is_null(expr: &Expr) -> bool {
    match expr {
        Expr::Value(RustValue::NullPtr) => true,
        Expr::Call { func, args, .. } if args.is_empty() => matches!(&**func, Expr::Var(name)
            if name.as_str().starts_with("std::ptr::null::<")
                || name.as_str().starts_with("std::ptr::null_mut::<")),
        _ => false,
    }
}

fn null_operand(expr: &Expr) -> Option<(bool, &Expr)> {
    let Expr::Binary { op, lhs, rhs } = expr else {
        return None;
    };
    let negated = match op {
        BinOp::Eq => false,
        BinOp::Ne => true,
        _ => return None,
    };
    match (is_null(lhs), is_null(rhs)) {
        (false, true) => Some((negated, lhs)),
        (true, false) => Some((negated, rhs)),
        _ => None,
    }
}

fn is_null_check(expr: &Expr) -> bool {
    matches!(expr, Expr::MethodCall { method, args, .. } if method == "is_null" && args.is_empty())
}

fn peel_ptr_cast(expr: Expr, vars: &VarTypes) -> Expr {
    match expr {
        Expr::Cast {
            expr: inner,
            ty: Type::Ptr { .. },
        } if matches!(&*inner, Expr::Var(name) if matches!(vars.get(name), Some(Type::Ptr { .. }))) => {
            *inner
        }
        other => other,
    }
}

fn rewrite_here(expr: &mut Expr, vars: &VarTypes) -> bool {
    if let Expr::Unary {
        op: UnaryOp::Not,
        expr: inner,
    } = expr
        && let Expr::Unary {
            op: UnaryOp::Not,
            expr: check,
        } = &mut **inner
        && is_null_check(check)
    {
        *expr = std::mem::replace(&mut **check, Expr::Todo(String::new()));
        return true;
    }
    let Some((negated, _)) = null_operand(expr) else {
        return false;
    };
    let Expr::Binary { lhs, rhs, .. } = std::mem::replace(expr, Expr::Todo(String::new())) else {
        unreachable!();
    };
    let operand = if is_null(&rhs) { *lhs } else { *rhs };
    let check = Expr::MethodCall {
        recv: Box::new(peel_ptr_cast(operand, vars)),
        method: "is_null".into(),
        args: Vec::new(),
    };
    *expr = if negated {
        Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(check),
        }
    } else {
        check
    };
    true
}

fn read_var_types(arena: &FunctionOptimizer, id: NodeId) -> VarTypes {
    arena
        .reads(id)
        .iter()
        .filter_map(|&name| arena.var_type(name).map(|ty| (name, ty.clone())))
        .collect()
}

pub(in crate::backend::engine) struct NullCompare;

impl NodeRule for NullCompare {
    fn name(&self) -> &'static str {
        "null_compare"
    }

    fn priority(&self) -> u32 {
        45
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        NodeKindTag::ALL_EXPRS
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let collapsible_here = |expr: &Expr| null_operand(expr).is_some();
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
