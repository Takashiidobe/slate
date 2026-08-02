use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionGroup, DefinitionKind, EditSet, Field, QueryRule};

pub(in crate::fixups) fn unused_known_declarations() -> QueryRule<Definition> {
    QueryRule::new(
        Pass::PruneUnusedDefinitions,
        "prune_unused_extern_decl",
        Definition {
            kind: Field::eq(DefinitionKind::ExternFunction),
            group: Field::predicate(|group: &Option<DefinitionGroup>, _| group.is_some()),
            ..Default::default()
        },
    )
    .case("unused", |case, definition| {
        case.fact(|query| query.zero_users(definition))?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
}

pub(in crate::fixups) fn unused_header(header: impl Into<String>) -> QueryRule<Definition> {
    QueryRule::new(
        Pass::PruneUnusedDefinitions,
        "prune_unused_header",
        Definition {
            group: Field::eq(Some(DefinitionGroup::Header(header.into()))),
            ..Default::default()
        },
    )
    .case("unused", |case, definition| {
        let group = definition.group.as_ref().ok_or_else(|| case.reject())?;
        case.fact(|query| query.zero_group_users(group))?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
}
