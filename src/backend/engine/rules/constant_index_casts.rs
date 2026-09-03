use super::walk::{child_exprs, child_exprs_mut};
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
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

fn nonnegative_bound(prim: Prim) -> Option<i128> {
    match prim {
        Prim::I8 => Some(i8::MAX as i128),
        Prim::I16 => Some(i16::MAX as i128),
        Prim::I32 => Some(i32::MAX as i128),
        Prim::I64 | Prim::Isize => Some(i64::MAX as i128),
        Prim::I128 => Some(i128::MAX),
        Prim::Usize | Prim::U8 | Prim::U16 | Prim::U32 | Prim::U64 | Prim::U128 => {
            Some(u64::MAX as i128)
        }
        _ => None,
    }
}

fn literal_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(value)) if *value >= 0 => Some(*value as i128),
        Expr::Value(RustValue::I128(value)) if *value >= 0 => Some(*value),
        _ => None,
    }
}

fn representable_literal(expr: &Expr) -> Option<Expr> {
    if literal_value(expr).is_some() {
        return Some(expr.clone());
    }
    let Expr::Cast {
        expr: inner,
        ty: Type::Prim(prim),
    } = expr
    else {
        return None;
    };
    let bound = nonnegative_bound(*prim)?;
    let literal = representable_literal(inner)?;
    (literal_value(&literal)? <= bound).then_some(literal)
}

fn constant_index_literal(index: &Expr) -> Option<Expr> {
    let Expr::Cast {
        expr: inner,
        ty: Type::Prim(Prim::Usize),
    } = index
    else {
        return None;
    };
    representable_literal(inner)
}

fn collapse_here(expr: &mut Expr) -> bool {
    let Expr::Index { index, .. } = expr else {
        return false;
    };
    let Some(literal) = constant_index_literal(index) else {
        return false;
    };
    **index = literal;
    true
}

fn fold_expr(expr: &mut Expr) -> bool {
    let mut changed = false;
    for child in child_exprs_mut(expr) {
        changed |= fold_expr(child);
    }
    changed | collapse_here(expr)
}

fn has_collapsible(expr: &Expr) -> bool {
    (matches!(expr, Expr::Index { index, .. } if constant_index_literal(index).is_some()))
        || child_exprs(expr).iter().any(|child| has_collapsible(child))
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

pub(in crate::backend::engine) struct ConstantIndexCasts;

impl NodeRule for ConstantIndexCasts {
    fn name(&self) -> &'static str {
        "constant_index_casts"
    }

    fn priority(&self) -> u32 {
        45
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        KINDS
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        arena.get(id).is_some_and(kind_has_collapsible)
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
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
