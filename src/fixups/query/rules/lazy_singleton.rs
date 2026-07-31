use crate::fixups::trace::Pass;

use super::super::{ProgramRule, rewrite_lazy_singletons};

pub(in crate::fixups) fn program() -> ProgramRule {
    ProgramRule::lazy_singletons(Pass::LazySingleton, "rewrite_lazy_singletons").case(
        "complete_domain",
        |case| {
            let singletons = case.lazy_singletons()?;
            Ok(rewrite_lazy_singletons(singletons))
        },
    )
}
