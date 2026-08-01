use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionKind, DefinitionRule, Field, delete_definition};

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

pub(in crate::fixups) fn rewrite() -> DefinitionRule {
    DefinitionRule::matches(
        Pass::UnusedItems,
        "prune_unused_type_definition",
        type_definition_matcher(),
    )
    .case("unreachable", |case| {
        case.unused_type_definition()?;
        Ok(delete_definition())
    })
}
