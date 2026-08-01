use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionKind, DefinitionRule, Field, delete_definition};

pub(in crate::fixups) fn unused_numeric_parse() -> DefinitionRule {
    DefinitionRule::matches(
        Pass::PruneUnusedDefinitions,
        "prune_unused_support_module",
        Definition {
            kind: Field::eq(DefinitionKind::SupportModule),
            name: Field::eq("__slate_runtime".into()),
            ..Default::default()
        },
    )
    .case("unused", |case| {
        case.zero_users()?;
        Ok(delete_definition())
    })
}
