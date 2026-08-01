use crate::fixups::trace::Pass;

use super::super::{
    Definition, DefinitionKind, DefinitionRule, Field, replace_body, rewrite_buffer_cursor,
};

fn function_matcher() -> Definition {
    Definition {
        kind: Field::eq(DefinitionKind::Function),
        ..Default::default()
    }
}

pub(in crate::fixups) fn rewrite() -> DefinitionRule {
    DefinitionRule::matches(
        Pass::BufferCursor,
        "rewrite_buffer_cursor",
        function_matcher(),
    )
    .case("resolved", |case| {
        let plan = case.buffer_cursor_plan()?;
        Ok(replace_body(rewrite_buffer_cursor(plan)))
    })
}
