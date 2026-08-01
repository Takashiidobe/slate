use crate::fixups::trace::Pass;

use super::super::{
    Definition, DefinitionKind, DefinitionRule, Field, replace_body, rewrite_zero_init,
};

fn function_matcher() -> Definition {
    Definition {
        kind: Field::eq(DefinitionKind::Function),
        ..Default::default()
    }
}

pub(in crate::fixups) fn direct() -> DefinitionRule {
    DefinitionRule::matches(
        Pass::ZeroInit,
        "fold_zero_init_assignment",
        function_matcher(),
    )
    .case("direct", |case| {
        let plan = case.zero_init_candidate(false)?;
        Ok(replace_body(rewrite_zero_init(plan)))
    })
}

pub(in crate::fixups) fn deferred() -> DefinitionRule {
    DefinitionRule::matches(
        Pass::ZeroInit,
        "fold_zero_init_assignment",
        function_matcher(),
    )
    .case("deferred", |case| {
        let plan = case.zero_init_candidate(true)?;
        Ok(replace_body(rewrite_zero_init(plan)))
    })
}
