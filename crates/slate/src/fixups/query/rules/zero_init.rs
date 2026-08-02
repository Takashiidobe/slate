use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionKind, EditSet, Field, QueryRule, rewrite_zero_init};

fn function_matcher() -> Definition {
    Definition {
        kind: Field::eq(DefinitionKind::Function),
        ..Default::default()
    }
}

pub(in crate::fixups) fn direct() -> QueryRule<Definition> {
    QueryRule::new(
        Pass::ZeroInit,
        "fold_zero_init_assignment",
        function_matcher(),
    )
    .case("direct", |case, definition| {
        let plan = case.fact(|query| query.zero_init_candidate(definition, false))?;
        let function = case.function(definition)?;
        Ok(EditSet::replace_function_body(
            function,
            rewrite_zero_init(plan),
        ))
    })
}

pub(in crate::fixups) fn deferred() -> QueryRule<Definition> {
    QueryRule::new(
        Pass::ZeroInit,
        "fold_zero_init_assignment",
        function_matcher(),
    )
    .case("deferred", |case, definition| {
        let plan = case.fact(|query| query.zero_init_candidate(definition, true))?;
        let function = case.function(definition)?;
        Ok(EditSet::replace_function_body(
            function,
            rewrite_zero_init(plan),
        ))
    })
}
