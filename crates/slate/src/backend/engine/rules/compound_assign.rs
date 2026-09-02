use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, UnaryOp};

pub(in crate::backend::engine) struct CompoundAssignRecover;

fn foldable(op: BinOp) -> bool {
    matches!(
        op,
        BinOp::Add
            | BinOp::Sub
            | BinOp::Mul
            | BinOp::Div
            | BinOp::Rem
            | BinOp::BitAnd
            | BinOp::BitOr
            | BinOp::BitXor
            | BinOp::Shl
            | BinOp::Shr
    )
}

fn pure_operand(expr: &Expr) -> bool {
    match expr {
        Expr::Var(_) | Expr::Value(_) => true,
        Expr::Cast { expr, .. } => pure_operand(expr),
        _ => false,
    }
}

fn simple_place(expr: &Expr) -> bool {
    match expr {
        Expr::Var(_) => true,
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => simple_place(base),
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => simple_place(expr) || pure_operand(expr),
        Expr::Index { base, index } => {
            (simple_place(base) || pure_operand(base)) && pure_operand(index)
        }
        _ => false,
    }
}

fn split(kind: &NodeKind) -> Option<(Expr, BinOp, Expr)> {
    let NodeKind::Assign {
        target,
        value: Expr::Binary { op, lhs, rhs },
    } = kind
    else {
        return None;
    };
    if !foldable(*op) || !simple_place(target) || **lhs != *target {
        return None;
    }
    Some((target.clone(), *op, (**rhs).clone()))
}

impl NodeRule for CompoundAssignRecover {
    fn name(&self) -> &'static str {
        "compound_assign::recover"
    }

    fn priority(&self) -> u32 {
        70
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Assign]
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        arena.get(id).and_then(split).is_some()
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some((target, op, value)) = arena.get(id).and_then(split) else {
            return false;
        };
        arena.set_kind(id, NodeKind::CompoundAssign { target, op, value });
        true
    }
}
