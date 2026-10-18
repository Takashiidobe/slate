use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{BinOp, Expr, UnaryOp};

use super::loop_to_while::negate;

fn is_pure(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_) => true,
        Expr::Unary {
            op: UnaryOp::Not | UnaryOp::Raw(_),
            expr,
        }
        | Expr::Cast { expr, .. }
        | Expr::Field { base: expr, .. }
        | Expr::TupleField { base: expr, .. } => is_pure(expr),
        Expr::Binary { op, lhs, rhs } => {
            matches!(
                op,
                BinOp::BitAnd
                    | BinOp::BitOr
                    | BinOp::BitXor
                    | BinOp::Eq
                    | BinOp::Ne
                    | BinOp::Lt
                    | BinOp::Le
                    | BinOp::Gt
                    | BinOp::Ge
                    | BinOp::And
                    | BinOp::Or
            ) && is_pure(lhs)
                && is_pure(rhs)
        }
        Expr::Block(block) | Expr::Unsafe(block) => {
            block.stmts.is_empty() && block.tail.as_deref().is_none_or(is_pure)
        }
        _ => false,
    }
}

fn only_comments(arena: &Arena, body: &[NodeId]) -> bool {
    body.iter()
        .all(|&child| matches!(arena.get(child), Some(NodeKind::Comment(_))))
}

fn is_function_tail(arena: &FunctionOptimizer, id: NodeId) -> bool {
    arena.return_type().is_some()
        && arena.parent(id) == Some(arena.root)
        && arena
            .get(arena.root)
            .and_then(|root| root.child_lists().into_iter().flatten().last().copied())
            == Some(id)
}

fn is_inert(arena: &FunctionOptimizer, id: NodeId) -> bool {
    if arena.parent(id).is_none() {
        return false;
    }
    match arena.get(id) {
        Some(NodeKind::Expr(expr)) => is_pure(expr) && !is_function_tail(arena, id),
        Some(NodeKind::Scope { body }) => only_comments(arena, body),
        Some(NodeKind::Unsafe { stmts, tail } | NodeKind::Block { stmts, tail }) => {
            tail.is_none() && only_comments(arena, stmts)
        }
        Some(NodeKind::If {
            cond,
            then_body,
            else_body,
        }) => is_pure(cond) && only_comments(arena, then_body) && only_comments(arena, else_body),
        _ => false,
    }
}

pub(in crate::backend::engine) struct PureStmtDrop;

impl NodeRule for PureStmtDrop {
    fn name(&self) -> &'static str {
        "pure_stmts::drop"
    }

    fn priority(&self) -> u32 {
        55
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[
            NodeKindTag::Expr,
            NodeKindTag::Scope,
            NodeKindTag::Unsafe,
            NodeKindTag::Block,
            NodeKindTag::If,
        ]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        is_inert(arena, id)
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if !is_inert(arena, id) {
            return false;
        }
        arena.retire_subtree(id, None);
        true
    }
}

fn invert(cond: &Expr) -> Expr {
    match cond {
        Expr::Binary {
            op: op @ (BinOp::Eq | BinOp::Ne),
            lhs,
            rhs,
        } => Expr::Binary {
            op: if *op == BinOp::Eq {
                BinOp::Ne
            } else {
                BinOp::Eq
            },
            lhs: lhs.clone(),
            rhs: rhs.clone(),
        },
        _ => negate(cond),
    }
}

pub(in crate::backend::engine) struct EmptyThenInvert;

impl NodeRule for EmptyThenInvert {
    fn name(&self) -> &'static str {
        "pure_stmts::empty_then"
    }

    fn priority(&self) -> u32 {
        55
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::If]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        matches!(
            arena.get(id),
            Some(NodeKind::If { then_body, else_body, .. })
                if then_body.is_empty() && !only_comments(arena, else_body)
        )
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if !self.matches(arena, id) {
            return false;
        }
        let Some(NodeKind::If {
            cond,
            then_body,
            else_body,
        }) = arena.get_mut(id)
        else {
            return false;
        };
        *cond = invert(cond);
        std::mem::swap(then_body, else_body);
        true
    }
}

pub(in crate::backend::engine) struct EmptyElseDrop;

impl NodeRule for EmptyElseDrop {
    fn name(&self) -> &'static str {
        "pure_stmts::empty_else"
    }

    fn priority(&self) -> u32 {
        55
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::If]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        matches!(
            arena.get(id),
            Some(NodeKind::If { then_body, else_body, .. })
                if !else_body.is_empty() && !only_comments(arena, then_body) && only_comments(arena, else_body)
        ) && arena.parent(id).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if !self.matches(arena, id) {
            return false;
        }
        let Some(NodeKind::If { else_body, .. }) = arena.get(id) else {
            return false;
        };
        let comments = else_body.clone();
        arena.move_comments_after(&comments, id);
        true
    }
}
