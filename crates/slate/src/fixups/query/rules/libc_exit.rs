use crate::fixups::trace::Pass;
use crate::function_identity::Known;

use super::super::{CallRule, CallTarget, Field, FnCall, process_exit};

pub(in crate::fixups) fn calls() -> CallRule {
    CallRule::matching(
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
        case.never_returning_extern()?;
        Ok(process_exit())
    })
}
