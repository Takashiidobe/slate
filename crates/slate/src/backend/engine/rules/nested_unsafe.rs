use super::walk::{child_exprs, child_exprs_mut, visit_kind_exprs, visit_kind_exprs_mut};
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Block, Expr, UnaryOp};

fn inside_unsafe(arena: &Arena, id: NodeId) -> bool {
    let mut node = arena.parent(id);
    while let Some(ancestor) = node {
        if matches!(arena.get(ancestor), Some(NodeKind::Unsafe { .. })) {
            return true;
        }
        node = arena.parent(ancestor);
    }
    false
}

fn is_place(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Var(_)
            | Expr::Path(_)
            | Expr::Unary {
                op: UnaryOp::Deref,
                ..
            }
            | Expr::Field { .. }
            | Expr::TupleField { .. }
            | Expr::Index { .. }
    )
}

fn child_value_contexts(expr: &Expr) -> Vec<bool> {
    match expr {
        Expr::Ref { .. }
        | Expr::AddrOf { .. }
        | Expr::Unary {
            op: UnaryOp::Raw(_),
            ..
        }
        | Expr::Field { .. }
        | Expr::TupleField { .. }
        | Expr::ArrayPtr { .. } => vec![false],
        Expr::Index { .. } => vec![false, true],
        Expr::MethodCall { args, .. } | Expr::MethodCallGeneric { args, .. } => {
            std::iter::once(false)
                .chain(args.iter().map(|_| true))
                .collect()
        }
        Expr::Macro { args, .. } => vec![false; args.len()],
        Expr::Match { arms, .. } => std::iter::once(false)
            .chain(arms.iter().map(|_| true))
            .collect(),
        other => vec![true; child_exprs(other).len()],
    }
}

fn unwrappable(expr: &Expr, in_unsafe: bool, value_context: bool) -> bool {
    in_unsafe
        && matches!(expr, Expr::Unsafe(block)
            if value_context || !block.stmts.is_empty() || !block.tail.as_deref().is_some_and(is_place))
}

fn has_nested(expr: &Expr, in_unsafe: bool, value_context: bool) -> bool {
    if unwrappable(expr, in_unsafe, value_context) {
        return true;
    }
    let in_unsafe = in_unsafe || matches!(expr, Expr::Unsafe(_));
    child_exprs(expr)
        .into_iter()
        .zip(child_value_contexts(expr))
        .any(|(child, value_context)| has_nested(child, in_unsafe, value_context))
}

fn unwrap(block: Block) -> Expr {
    match block {
        Block {
            stmts,
            tail: Some(tail),
        } if stmts.is_empty() => *tail,
        block => Expr::Block(Box::new(block)),
    }
}

fn strip(expr: &mut Expr, in_unsafe: bool, value_context: bool) {
    while unwrappable(expr, in_unsafe, value_context) {
        let Expr::Unsafe(block) = expr else {
            break;
        };
        let block = std::mem::take(block);
        *expr = unwrap(*block);
    }
    let in_unsafe = in_unsafe || matches!(expr, Expr::Unsafe(_));
    let contexts = child_value_contexts(expr);
    for (child, value_context) in child_exprs_mut(expr).into_iter().zip(contexts) {
        strip(child, in_unsafe, value_context);
    }
}

pub(in crate::backend::engine) struct NestedUnsafeStmt;

impl NodeRule for NestedUnsafeStmt {
    fn name(&self) -> &'static str {
        "nested_unsafe::stmt"
    }

    fn priority(&self) -> u32 {
        5
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Unsafe]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        inside_unsafe(arena, id)
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if !inside_unsafe(arena, id) {
            return false;
        }
        let Some(NodeKind::Unsafe { stmts, tail }) = arena.get(id) else {
            return false;
        };
        let kind = match tail {
            None => NodeKind::Scope {
                body: stmts.clone(),
            },
            Some(tail) => NodeKind::Block {
                stmts: stmts.clone(),
                tail: Some(tail.clone()),
            },
        };
        arena.set_kind(id, kind);
        true
    }
}

pub(in crate::backend::engine) struct NestedUnsafeExpr;

impl NestedUnsafeExpr {
    fn context(arena: &FunctionOptimizer, id: NodeId) -> (bool, bool) {
        let kind = arena.get(id);
        let in_unsafe = matches!(kind, Some(NodeKind::Unsafe { .. })) || inside_unsafe(arena, id);
        (in_unsafe, !matches!(kind, Some(NodeKind::Match { .. })))
    }
}

impl NodeRule for NestedUnsafeExpr {
    fn name(&self) -> &'static str {
        "nested_unsafe::expr"
    }

    fn priority(&self) -> u32 {
        5
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        NodeKindTag::ALL_EXPRS
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(kind) = arena.get(id) else {
            return false;
        };
        let (in_unsafe, value_context) = Self::context(arena, id);
        let mut found = false;
        visit_kind_exprs(kind, |expr| {
            found |= has_nested(expr, in_unsafe, value_context)
        });
        found
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if !self.matches(arena, id) {
            return false;
        }
        let (in_unsafe, value_context) = Self::context(arena, id);
        let Some(kind) = arena.get_mut(id) else {
            return false;
        };
        visit_kind_exprs_mut(kind, |expr| strip(expr, in_unsafe, value_context));
        true
    }
}
