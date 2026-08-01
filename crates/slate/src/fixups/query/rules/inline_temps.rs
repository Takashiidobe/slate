use crate::fixups::trace::Pass;

use super::super::{
    Definition, DefinitionKind, DefinitionRule, Field, Phase, replace_body, rewrite_inline_temp,
};

fn function_matcher() -> Definition {
    Definition {
        kind: Field::eq(DefinitionKind::Function),
        ..Default::default()
    }
}

pub(in crate::fixups) fn early() -> DefinitionRule {
    DefinitionRule::matches(Pass::EarlyInlineTemps, "inline_temp", function_matcher()).case(
        "early",
        |case| {
            let plan = case.inline_temp_candidate(Phase::Early)?;
            let body = case.function_body();
            Ok(replace_body(rewrite_inline_temp(body, plan)))
        },
    )
}

pub(in crate::fixups) fn late() -> DefinitionRule {
    DefinitionRule::matches(Pass::LateInlineTemps, "inline_temp", function_matcher()).case(
        "late",
        |case| {
            let plan = case.inline_temp_candidate(Phase::Late)?;
            let body = case.function_body();
            Ok(replace_body(rewrite_inline_temp(body, plan)))
        },
    )
}
