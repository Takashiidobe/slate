use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Ident, Path};
use crate::function_identity::CallBinding;

fn is_exit_call(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Call { func, args, .. }
            if args.len() == 1 && matches!(func.as_ref(), Expr::Var(name) if name.as_str() == "exit")
    )
}

fn exit_call_tail(kind: &NodeKind) -> bool {
    matches!(
        kind,
        NodeKind::Expr(Expr::Unsafe(block))
            if block.stmts.is_empty() && block.tail.as_deref().is_some_and(is_exit_call)
    )
}

pub(in crate::backend::engine) struct LibcExit;

impl NodeRule for LibcExit {
    fn name(&self) -> &'static str {
        "libc_exit"
    }

    fn priority(&self) -> u32 {
        59
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Expr]
    }

    fn call_anchor(&self) -> Option<Ident> {
        Some(Ident::new("exit"))
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        arena.get(id).is_some_and(exit_call_tail)
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(NodeKind::Expr(expr)) = arena.get_mut(id) else {
            return false;
        };
        let Expr::Unsafe(block) = expr else {
            return false;
        };
        if !block.stmts.is_empty() || !block.tail.as_deref().is_some_and(is_exit_call) {
            return false;
        }
        let Expr::Call { args, .. } = *block.tail.take().unwrap() else {
            unreachable!("is_exit_call only accepts Expr::Call")
        };
        *expr = Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                ["std", "process", "exit"].map(Ident::from),
            ))),
            args,
        };
        true
    }
}
