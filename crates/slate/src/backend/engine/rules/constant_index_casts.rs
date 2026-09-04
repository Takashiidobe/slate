use super::walk;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKindTag};
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

fn collapsible_here(expr: &Expr) -> bool {
    matches!(expr, Expr::Index { index, .. } if constant_index_literal(index).is_some())
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
        arena.get(id).is_some_and(|kind| {
            let mut found = false;
            walk::visit_kind_exprs(kind, |expr| {
                found |= walk::any_collapsible(expr, &collapsible_here)
            });
            found
        })
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
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
