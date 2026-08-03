use crate::fixups::trace::Pass;

use super::super::{
    Definition, DefinitionGroup, DefinitionKind, EditSet, Field, Predicate, QueryRule,
};

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
        let users = case.fact(|query| query.definition_users(definition))?;
        case.require_at(users.users == 0, Predicate::ZeroUsers, &users.site)?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
}

#[allow(dead_code)]
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
        let users = case.fact(|query| query.definition_group_users(group))?;
        case.require_at(users.users == 0, Predicate::ZeroGroupUsers, &users.site)?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
}
