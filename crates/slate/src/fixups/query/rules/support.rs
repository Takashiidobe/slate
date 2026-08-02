use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionKind, EditSet, Field, QueryRule};

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
        case.fact(|query| query.zero_users(definition))?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
}
