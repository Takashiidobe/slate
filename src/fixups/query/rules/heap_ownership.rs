use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionKind, Field, QueryRule, rewrite_heap_ownership};

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
        let function = case.fact(|query| query.definition_function(definition))?;
        let body = case
            .fact(|query| query.function_snapshot(&function))?
            .body
            .clone();
        case.replace_function_body(function, rewrite_heap_ownership(body, plans))
    })
}
