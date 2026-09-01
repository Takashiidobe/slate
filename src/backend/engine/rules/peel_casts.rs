use super::walk::{child_exprs, child_exprs_mut};
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Prim, RustValue, Type};

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

fn yields_ptr_or_int(expr: &Expr) -> bool {
    match expr {
        Expr::Cast { ty, .. } => {
            is_thin_raw_ptr(ty) || matches!(ty, Type::Prim(p) if is_int_prim(*p))
        }
        Expr::MethodCall { method, .. } => matches!(method.as_str(), "as_ptr" | "as_mut_ptr"),
        Expr::ArrayPtr { .. } => true,
        Expr::Value(
            RustValue::NullPtr
            | RustValue::I64(_)
            | RustValue::I128(_)
            | RustValue::U128(_)
            | RustValue::Usize(_),
        ) => true,
        _ => false,
    }
}

fn collapsible_pair(expr: &Expr) -> bool {
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
    is_thin_raw_ptr(inner_ty) && is_thin_raw_ptr(outer_ty) && yields_ptr_or_int(innermost)
}

fn collapse_here(expr: &mut Expr) -> bool {
    let mut changed = false;
    while collapsible_pair(expr) {
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

fn fold_expr(expr: &mut Expr) -> bool {
    let mut changed = false;
    for child in child_exprs_mut(expr) {
        changed |= fold_expr(child);
    }
    changed | collapse_here(expr)
}

fn has_collapsible(expr: &Expr) -> bool {
    collapsible_pair(expr) || child_exprs(expr).iter().any(|child| has_collapsible(child))
}

fn fold_kind(kind: &mut NodeKind) -> bool {
    let mut changed = false;
    let mut fold = |expr: &mut Expr| changed |= fold_expr(expr);
    match kind {
        NodeKind::Let { init, .. } => init.as_mut().into_iter().for_each(&mut fold),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => [cond, then_value, else_value]
            .into_iter()
            .for_each(&mut fold),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            [target, value].into_iter().for_each(&mut fold)
        }
        NodeKind::InlineAsm(asm) => {
            for operand in &mut asm.operands {
                operand.visit_exprs_mut(&mut fold);
            }
        }
        NodeKind::Expr(expr) => fold(expr),
        NodeKind::Return(expr) => expr.as_mut().into_iter().for_each(&mut fold),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            tail.as_deref_mut().into_iter().for_each(&mut fold)
        }
        NodeKind::While { cond, tail, .. } => {
            fold(cond);
            tail.as_deref_mut().into_iter().for_each(&mut fold);
        }
        NodeKind::If { cond, .. } => fold(cond),
        NodeKind::For { iter, .. } => fold(iter),
        NodeKind::Match { expr, .. } => fold(expr),
        NodeKind::Loop { .. }
        | NodeKind::Scope { .. }
        | NodeKind::LabeledBlock { .. }
        | NodeKind::Break(_)
        | NodeKind::Continue(_) => {}
    }
    changed
}

fn kind_has_collapsible(kind: &NodeKind) -> bool {
    let mut found = false;
    let mut scan = |expr: &Expr| found |= has_collapsible(expr);
    match kind {
        NodeKind::Let { init, .. } => init.as_ref().into_iter().for_each(&mut scan),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => [cond, then_value, else_value]
            .into_iter()
            .for_each(&mut scan),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            [target, value].into_iter().for_each(&mut scan)
        }
        NodeKind::InlineAsm(asm) => {
            for operand in &asm.operands {
                operand.visit_exprs(&mut scan);
            }
        }
        NodeKind::Expr(expr) => scan(expr),
        NodeKind::Return(expr) => expr.as_ref().into_iter().for_each(&mut scan),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            tail.as_deref().into_iter().for_each(&mut scan)
        }
        NodeKind::While { cond, tail, .. } => {
            scan(cond);
            tail.as_deref().into_iter().for_each(&mut scan);
        }
        NodeKind::If { cond, .. } => scan(cond),
        NodeKind::For { iter, .. } => scan(iter),
        NodeKind::Match { expr, .. } => scan(expr),
        NodeKind::Loop { .. }
        | NodeKind::Scope { .. }
        | NodeKind::LabeledBlock { .. }
        | NodeKind::Break(_)
        | NodeKind::Continue(_) => {}
    }
    found
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

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        arena.get(id).is_some_and(kind_has_collapsible)
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(kind) = arena.get_mut(id) else {
            return false;
        };
        if !fold_kind(kind) {
            return false;
        }
        arena.touch(id);
        true
    }
}
