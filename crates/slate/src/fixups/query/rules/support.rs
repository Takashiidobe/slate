use crate::fixups::trace::Pass;

use super::super::{DefinitionRule, delete_definition};

pub(in crate::fixups) fn unused_numeric_parse() -> DefinitionRule {
    DefinitionRule::support_module(
        Pass::PruneUnusedDefinitions,
        "prune_unused_support_module",
        "__slate_runtime",
    )
    .case("unused", |case| {
        case.zero_users()?;
        Ok(delete_definition())
    })
}
