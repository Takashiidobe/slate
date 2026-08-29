use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Ident, Path};
use crate::function_identity::{CallBinding, Known};

pub(super) enum ArgSpec {
    Passthrough,
    #[expect(
        dead_code,
        reason = "arg-transform extension point; first reordering/inserting libc idiom pending"
    )]
    Custom(fn(&[Expr]) -> Option<Vec<Expr>>),
}

impl ArgSpec {
    fn transform(&self, args: &[Expr]) -> Option<Vec<Expr>> {
        match self {
            ArgSpec::Passthrough => Some(args.to_vec()),
            ArgSpec::Custom(f) => f(args),
        }
    }
}

pub(super) struct LibcCall {
    known: Known,
    rust_path: &'static [&'static str],
    args: ArgSpec,
}

pub(super) fn libc_call(
    known: Known,
    rust_path: &'static [&'static str],
    args: ArgSpec,
) -> Box<dyn NodeRule> {
    Box::new(LibcCall {
        known,
        rust_path,
        args,
    })
}

fn as_known_call(expr: &Expr, known: Known) -> Option<&[Expr]> {
    let call = match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => block.tail.as_deref()?,
        other => other,
    };
    match call {
        Expr::Call { binding, args, .. } if binding.known() == Some(known) => Some(args),
        _ => None,
    }
}

impl NodeRule for LibcCall {
    fn name(&self) -> &'static str {
        self.known.symbol()
    }

    fn priority(&self) -> u32 {
        59
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Expr]
    }

    fn call_anchor(&self) -> Option<Ident> {
        Some(Ident::new(self.known.symbol()))
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        matches!(arena.get(id), Some(NodeKind::Expr(expr)) if as_known_call(expr, self.known).is_some())
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(NodeKind::Expr(expr)) = arena.get_mut(id) else {
            return false;
        };
        let Some(args) = as_known_call(expr, self.known) else {
            return false;
        };
        let Some(args) = self.args.transform(args) else {
            return false;
        };
        *expr = Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                self.rust_path.iter().copied().map(Ident::from),
            ))),
            args,
        };
        true
    }
}

pub(super) fn rules() -> Vec<Box<dyn NodeRule>> {
    vec![
        libc_call(
            Known::Exit,
            &["std", "process", "exit"],
            ArgSpec::Passthrough,
        ),
        libc_call(
            Known::Abort,
            &["std", "process", "abort"],
            ArgSpec::Passthrough,
        ),
    ]
}
