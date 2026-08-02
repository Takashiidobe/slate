use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionKind, EditSet, Field, QueryRule, rewrite_heap_ownership};

pub(in crate::fixups) fn rewrite() -> QueryRule<Definition> {
    QueryRule::new(
        Pass::HeapOwnership,
        "rewrite_heap_ownership",
        Definition {
            kind: Field::eq(DefinitionKind::Function),
            ..Default::default()
        },
    )
    .case("owned_heap", |case, definition| {
        let plans = case.fact(|query| query.heap_ownership_plans(definition))?;
        let body = case.function_body(definition);
        Ok(EditSet::replace_function_body(
            definition.clone(),
            rewrite_heap_ownership(body, plans),
        ))
    })
}
