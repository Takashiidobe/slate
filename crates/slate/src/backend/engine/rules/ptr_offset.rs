use std::collections::HashMap;

use super::constant_index_casts::representable_literal;
use super::walk;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, Ident, Prim, RustValue, Type, UnaryOp};

type VarTypes = HashMap<Ident, Type>;

fn is_unsigned_prim(prim: Prim) -> bool {
    matches!(
        prim,
        Prim::U8 | Prim::U16 | Prim::U32 | Prim::U64 | Prim::U128 | Prim::Usize
    )
}

fn is_unsigned(expr: &Expr, vars: &VarTypes) -> bool {
    match expr {
        Expr::Cast {
            ty: Type::Prim(prim),
            ..
        } => is_unsigned_prim(*prim),
        Expr::Var(name) => {
            matches!(vars.get(name), Some(Type::Prim(prim)) if is_unsigned_prim(*prim))
        }
        Expr::Value(RustValue::Usize(_) | RustValue::TypedUInt(..)) => true,
        Expr::Binary { op, lhs, rhs } => match op {
            BinOp::Add
            | BinOp::Mul
            | BinOp::Div
            | BinOp::Rem
            | BinOp::BitAnd
            | BinOp::BitOr
            | BinOp::BitXor => is_unsigned(lhs, vars) && is_unsigned(rhs, vars),
            BinOp::Shl | BinOp::Shr => is_unsigned(lhs, vars),
            _ => false,
        },
        _ => false,
    }
}

fn unsigned_index(expr: &Expr, vars: &VarTypes) -> Option<Expr> {
    let Expr::Cast {
        expr: inner,
        ty: Type::Prim(Prim::Isize),
    } = expr
    else {
        return None;
    };
    if let Some(literal) = representable_literal(inner) {
        return Some(literal);
    }
    if !is_unsigned(inner, vars) {
        return None;
    }
    Some(match &**inner {
        Expr::Cast {
            ty: Type::Prim(Prim::Usize),
            ..
        } => (**inner).clone(),
        other => Expr::Cast {
            expr: Box::new(other.clone()),
            ty: Type::Prim(Prim::Usize),
        },
    })
}

fn rewrite_target(expr: &Expr, vars: &VarTypes) -> Option<(&'static str, Expr)> {
    let Expr::MethodCall { method, args, .. } = expr else {
        return None;
    };
    if method != "offset" {
        return None;
    }
    let [arg] = args.as_slice() else {
        return None;
    };
    if let Expr::Unary {
        op: UnaryOp::Neg,
        expr: negated,
    } = arg
    {
        return unsigned_index(negated, vars).map(|index| ("sub", index));
    }
    unsigned_index(arg, vars).map(|index| ("add", index))
}

fn rewrite_here(expr: &mut Expr, vars: &VarTypes) -> bool {
    let Some((method, index)) = rewrite_target(expr, vars) else {
        return false;
    };
    let Expr::MethodCall {
        method: name, args, ..
    } = expr
    else {
        unreachable!();
    };
    *name = method.into();
    args[0] = index;
    true
}

fn read_var_types(arena: &FunctionOptimizer, id: NodeId) -> VarTypes {
    arena
        .reads(id)
        .iter()
        .filter_map(|&name| arena.var_type(name).map(|ty| (name, ty.clone())))
        .collect()
}

pub(in crate::backend::engine) struct PtrOffset;

impl NodeRule for PtrOffset {
    fn name(&self) -> &'static str {
        "ptr_offset"
    }

    fn priority(&self) -> u32 {
        45
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        NodeKindTag::ALL_EXPRS
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let vars = read_var_types(arena, id);
        let collapsible_here = |expr: &Expr| rewrite_target(expr, &vars).is_some();
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
