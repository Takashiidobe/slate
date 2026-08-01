use crate::fixups::trace::Pass;
use crate::function_identity::Known;
use crate::rust_ast::Type;

use super::super::{CallRule, CallTarget, ExternFn, Field, FnCall, process_exit};

pub(in crate::fixups) fn calls() -> CallRule {
    CallRule::matches(
        Pass::LibcExit,
        "rewrite_libc_exit",
        FnCall {
            target: Field::eq(CallTarget::Known(Known::Exit)),
            arity: Field::eq(1),
            ..Default::default()
        },
    )
    .replace_trivial_unsafe()
    .case("never_returning_extern", |case| {
        case.extern_fn(&ExternFn {
            name: Field::eq(Known::Exit.symbol().into()),
            arity: Field::eq(1),
            returns: Field::eq(Some(Type::Never)),
        })?;
        Ok(process_exit())
    })
}
