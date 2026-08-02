use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionKind, EditSet, Field, QueryRule, rewrite_buffer_cursor};

fn function_matcher() -> Definition {
    Definition {
        kind: Field::eq(DefinitionKind::Function),
        ..Default::default()
    }
}

pub(in crate::fixups) fn rewrite() -> QueryRule<Definition> {
    QueryRule::new(
        Pass::BufferCursor,
        "rewrite_buffer_cursor",
        function_matcher(),
    )
    .case("resolved", |case, definition| {
        let plan = case.buffer_cursor_plan(definition)?;
        Ok(EditSet::replace_function_body(
            definition.clone(),
            rewrite_buffer_cursor(plan),
        ))
    })
}
