use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionKind, EditSet, Field, QueryRule};

fn type_definition_matcher() -> Definition {
    Definition {
        kind: Field::predicate(|kind: &DefinitionKind, _: &()| {
            matches!(
                kind,
                DefinitionKind::Struct | DefinitionKind::Record | DefinitionKind::Enum
            )
        }),
        ..Default::default()
    }
}

pub(in crate::fixups) fn rewrite() -> QueryRule<Definition> {
    QueryRule::new(
        Pass::UnusedItems,
        "prune_unused_type_definition",
        type_definition_matcher(),
    )
    .case("unreachable", |case, definition| {
        case.unused_type_definition(definition)?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
}
