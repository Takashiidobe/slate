use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionKind, EditSet, Field, Predicate, QueryRule};

pub(in crate::fixups) fn unused_numeric_parse() -> QueryRule<Definition> {
    QueryRule::new(
        Pass::PruneUnusedDefinitions,
        "prune_unused_support_module",
        Definition {
            kind: Field::eq(DefinitionKind::SupportModule),
            name: Field::eq("__slate_runtime".into()),
            ..Default::default()
        },
    )
    .case("unused", |case, definition| {
        let users = case.fact(|query| query.definition_users(definition))?;
        case.require_at(users.users == 0, Predicate::ZeroUsers, &users.site)?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
}
