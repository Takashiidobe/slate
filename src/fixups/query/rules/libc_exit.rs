use crate::fixups::trace::Pass;
use crate::function_identity::Known;

use super::super::{CallRule, process_exit};

pub(in crate::fixups) fn calls() -> CallRule {
    CallRule::known(Pass::LibcExit, "rewrite_libc_exit", Known::Exit, 1)
        .replace_trivial_unsafe()
        .case("never_returning_extern", |case| {
            case.never_returning_extern()?;
            Ok(process_exit())
        })
}
