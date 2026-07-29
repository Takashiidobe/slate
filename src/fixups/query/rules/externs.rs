use crate::fixups::trace::Pass;

use super::super::{DefinitionRule, delete_definition};

pub(in crate::fixups) fn unused_known_declarations() -> DefinitionRule {
    DefinitionRule::known_extern_functions(Pass::PruneUnusedDefinitions, "prune_unused_extern_decl")
        .case("unused", |case| {
            case.zero_users()?;
            Ok(delete_definition())
        })
}

pub(in crate::fixups) fn unused_header(header: impl Into<String>) -> DefinitionRule {
    DefinitionRule::header(Pass::PruneUnusedDefinitions, "prune_unused_header", header).case(
        "unused",
        |case| {
            case.zero_group_users()?;
            Ok(delete_definition())
        },
    )
}
