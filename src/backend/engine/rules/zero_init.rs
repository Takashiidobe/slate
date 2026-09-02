use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Ident, Pattern, RustValue, UnaryOp};

fn expr_is_zero(expr: &Expr) -> bool {
    match expr {
        Expr::Value(
            RustValue::I64(0)
            | RustValue::Usize(0)
            | RustValue::I128(0)
            | RustValue::U128(0)
            | RustValue::TypedInt(0, _)
            | RustValue::TypedUInt(0, _)
            | RustValue::NullPtr
            | RustValue::None,
        ) => true,
        Expr::Cast { expr, .. } => expr_is_zero(expr),
        Expr::Unary {
            op: UnaryOp::Neg,
            expr,
        } => expr_is_zero(expr),
        Expr::ArrayRepeat { elem, .. } => expr_is_zero(elem),
        Expr::StructLit { fields, .. } => {
            !fields.is_empty() && fields.iter().all(|(_, value)| expr_is_zero(value))
        }
        Expr::TupleStructLit { fields, .. } => {
            !fields.is_empty() && fields.iter().all(expr_is_zero)
        }
        _ => false,
    }
}

fn expr_is_pure_and_closed(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) | Expr::Str(_) | Expr::HexFloat(_) | Expr::ByteStr(_) | Expr::CStr(_) => {
            true
        }
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

fn qualifying_write_value(arena: &Arena, id: NodeId, name: Ident) -> Option<&Expr> {
    match arena.get(id)? {
        NodeKind::Assign { target, value } => (matches!(target, Expr::Var(v) if *v == name)
            && !value.reads_var(name.as_str()))
        .then_some(value),
        _ => None,
    }
}

fn intervening_ok(arena: &Arena, intervening: &[NodeId], value: &Expr) -> bool {
    let value_is_pure = expr_is_pure_and_closed(value);
    for &id in intervening {
        let Some(kind) = arena.get(id) else {
            return false;
        };
        if !value_is_pure && !matches!(kind, NodeKind::Let { .. }) {
            return false;
        }
        if let NodeKind::Let { name: declared, .. } = kind
            && value.reads_var(declared.as_str())
        {
            return false;
        }
    }
    true
}

struct Found {
    parent: NodeId,
    list_index: usize,
    write_pos: usize,
    write_id: NodeId,
}

fn locate(arena: &Arena, decl_id: NodeId, name: Ident) -> Option<Found> {
    let parent = arena.parent(decl_id)?;
    let lists = arena.get(parent)?.child_lists();
    for (list_index, list) in lists.iter().enumerate() {
        let Some(decl_pos) = list.iter().position(|&child| child == decl_id) else {
            continue;
        };
        for (offset, &sibling) in list.iter().enumerate().skip(decl_pos + 1) {
            if !node_touches_name(arena, sibling, name) {
                continue;
            }
            let value = qualifying_write_value(arena, sibling, name)?;
            let intervening = &list[decl_pos + 1..offset];
            return intervening_ok(arena, intervening, value).then_some(Found {
                parent,
                list_index,
                write_pos: offset,
                write_id: sibling,
            });
        }
        return None;
    }
    None
}

pub(in crate::backend::engine) struct ZeroInitFold;

impl NodeRule for ZeroInitFold {
    fn name(&self) -> &'static str {
        "zero_init::fold"
    }

    fn priority(&self) -> u32 {
        5
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Let]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        matches!(
            arena.get(id),
            Some(NodeKind::Let {
                mutable: true,
                ty: Some(_),
                init: Some(init),
                ..
            }) if expr_is_zero(init)
        )
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::Let {
            name,
            mutable: true,
            ty: Some(_),
            init: Some(init),
        }) = arena.get(id)
        else {
            return false;
        };
        if !expr_is_zero(init) {
            return false;
        }
        let name = *name;
        let Some(found) = locate(arena, id, name) else {
            return false;
        };
        let Some(mut write_kind) = arena.take(found.write_id) else {
            return false;
        };
        let NodeKind::Assign { value, .. } = &mut write_kind else {
            unreachable!("locate only returns Assign nodes")
        };
        let value = std::mem::replace(value, Expr::Value(RustValue::I64(0)));

        let Some(NodeKind::Let { init, .. }) = arena.get_mut(id) else {
            unreachable!("zero_init: id invalidated by taking an unrelated sibling slot")
        };
        *init = Some(value);

        let Some(parent_kind) = arena.get_mut(found.parent) else {
            unreachable!("zero_init: found.parent invalidated by taking an unrelated sibling slot")
        };
        let mut child_lists = parent_kind.child_lists_mut();
        let Some(list) = child_lists.get_mut(found.list_index) else {
            unreachable!("zero_init: list_index no longer valid on found.parent")
        };
        list.remove(found.write_pos);
        true
    }
}
