use crate::fixups::trace::Pass;

use super::super::{
    Definition, DefinitionKind, DefinitionRule, Field, replace_body, rewrite_heap_ownership,
};

pub(in crate::fixups) fn rewrite() -> DefinitionRule {
    DefinitionRule::matches(
        Pass::HeapOwnership,
        "rewrite_heap_ownership",
        Definition {
            kind: Field::eq(DefinitionKind::Function),
            ..Default::default()
        },
    )
    .case("owned_heap", |case| {
        let plans = case.heap_ownership_plans()?;
        let body = case.function_body();
        Ok(replace_body(rewrite_heap_ownership(body, plans)))
    })
}
