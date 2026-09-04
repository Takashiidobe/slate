use std::collections::HashMap;

use super::walk;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKindTag};
use crate::backend::rust_ast::{Expr, Ident, Prim, RustValue, Type};

type VarTypes = HashMap<Ident, Type>;

const KINDS: &[NodeKindTag] = &[
    NodeKindTag::Let,
    NodeKindTag::LetIf,
    NodeKindTag::Assign,
    NodeKindTag::CompoundAssign,
    NodeKindTag::InlineAsm,
    NodeKindTag::Expr,
    NodeKindTag::Return,
    NodeKindTag::Unsafe,
    NodeKindTag::If,
    NodeKindTag::For,
    NodeKindTag::Match,
    NodeKindTag::While,
    NodeKindTag::Block,
];

fn is_sized_pointee(ty: &Type) -> bool {
    !matches!(
        ty,
        Type::Slice(_) | Type::Str | Type::VaList | Type::Variadic
    )
}

fn is_thin_raw_ptr(ty: &Type) -> bool {
    matches!(ty, Type::Ptr { inner, .. } if is_sized_pointee(inner))
}

fn is_int_prim(prim: Prim) -> bool {
    !matches!(
        prim,
        Prim::F16 | Prim::F32 | Prim::F64 | Prim::F128 | Prim::Bool
    )
}

fn is_ptr_or_int_type(ty: &Type) -> bool {
    is_thin_raw_ptr(ty) || matches!(ty, Type::Prim(p) if is_int_prim(*p))
}

fn yields_ptr_or_int(expr: &Expr, vars: &VarTypes) -> bool {
    match expr {
        Expr::Cast { ty, .. } => is_ptr_or_int_type(ty),
        Expr::MethodCall { method, .. } => matches!(method.as_str(), "as_ptr" | "as_mut_ptr"),
        Expr::ArrayPtr { .. } => true,
        Expr::Value(
            RustValue::NullPtr
            | RustValue::I64(_)
            | RustValue::I128(_)
            | RustValue::U128(_)
            | RustValue::Usize(_),
        ) => true,
        Expr::Var(name) => vars.get(name).is_some_and(is_ptr_or_int_type),
        _ => false,
    }
}

fn collapsible_pair(expr: &Expr, vars: &VarTypes) -> bool {
    let Expr::Cast {
        expr: inner,
        ty: outer_ty,
    } = expr
    else {
        return false;
    };
    let Expr::Cast {
        ty: inner_ty,
        expr: innermost,
    } = inner.as_ref()
    else {
        return false;
    };
    if inner_ty == outer_ty {
        return true;
    }
    is_thin_raw_ptr(inner_ty) && is_thin_raw_ptr(outer_ty) && yields_ptr_or_int(innermost, vars)
}

fn collapse_here(expr: &mut Expr, vars: &VarTypes) -> bool {
    let mut changed = false;
    while collapsible_pair(expr, vars) {
        let Expr::Cast { expr: inner, .. } = expr else {
            break;
        };
        let Expr::Cast {
            expr: innermost, ..
        } = inner.as_mut()
        else {
            break;
        };
        let innermost = std::mem::replace(innermost.as_mut(), Expr::Todo(String::new()));
        **inner = innermost;
        changed = true;
    }
    changed
}

fn read_var_types(arena: &FunctionOptimizer, id: NodeId) -> VarTypes {
    arena
        .reads(id)
        .iter()
        .filter_map(|&name| arena.var_type(name).map(|ty| (name, ty.clone())))
        .collect()
}

pub(in crate::backend::engine) struct PeelCasts;

impl NodeRule for PeelCasts {
    fn name(&self) -> &'static str {
        "peel_casts"
    }

    fn priority(&self) -> u32 {
        45
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        KINDS
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let vars = read_var_types(arena, id);
        let collapsible_here = |expr: &Expr| collapsible_pair(expr, &vars);
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
        let mut collapse_here = |expr: &mut Expr| collapse_here(expr, &vars);
        let Some(kind) = arena.get_mut(id) else {
            return false;
        };
        let mut changed = false;
        walk::visit_kind_exprs_mut(kind, |expr| {
            changed |= walk::fold_bottom_up(expr, &mut collapse_here)
        });
        if !changed {
            return false;
        }
        arena.touch(id);
        true
    }
}
