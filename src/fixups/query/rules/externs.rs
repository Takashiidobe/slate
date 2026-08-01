use crate::fixups::trace::Pass;

use super::super::{
    Definition, DefinitionGroup, DefinitionKind, DefinitionRule, Field, delete_definition,
};

pub(in crate::fixups) fn unused_known_declarations() -> DefinitionRule {
    DefinitionRule::matches(
        Pass::PruneUnusedDefinitions,
        "prune_unused_extern_decl",
        Definition {
            kind: Field::eq(DefinitionKind::ExternFunction),
            group: Field::predicate(|group: &Option<DefinitionGroup>, _| group.is_some()),
            ..Default::default()
        },
    )
    .case("unused", |case| {
        case.zero_users()?;
        Ok(delete_definition())
    })
}

pub(in crate::fixups) fn unused_header(header: impl Into<String>) -> DefinitionRule {
    DefinitionRule::matches(
        Pass::PruneUnusedDefinitions,
        "prune_unused_header",
        Definition {
            group: Field::eq(Some(DefinitionGroup::Header(header.into()))),
            ..Default::default()
        },
    )
    .case("unused", |case| {
        case.zero_group_users()?;
        Ok(delete_definition())
    })
}
