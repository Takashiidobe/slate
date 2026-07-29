use crate::fixups::trace::Pass;

use super::super::{ProgramRule, rewrite_anonymous_structs};

pub(in crate::fixups) fn program() -> ProgramRule {
    ProgramRule::anonymous_structs(Pass::AnonymousStructs, "rewrite_anonymous_structs").case(
        "complete_domain",
        |case| {
            let structs = case.anonymous_structs()?;
            Ok(rewrite_anonymous_structs(structs))
        },
    )
}
