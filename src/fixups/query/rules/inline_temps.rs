use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionKind, Field, Phase, QueryRule, rewrite_inline_temp};

fn function_matcher() -> Definition {
    Definition {
        kind: Field::eq(DefinitionKind::Function),
        ..Default::default()
    }
}

pub(in crate::fixups) fn early() -> QueryRule<Definition> {
    QueryRule::new(Pass::EarlyInlineTemps, "inline_temp", function_matcher()).case(
        "early",
        |case, definition| {
            let plan = case.fact(|query| query.inline_temp_candidate(definition, Phase::Early))?;
            let function = case.fact(|query| query.definition_function(definition))?;
            let body = case
                .fact(|query| query.function_snapshot(&function))?
                .body
                .clone();
            case.replace_function_body(function, rewrite_inline_temp(body, plan))
        },
    )
}

pub(in crate::fixups) fn late() -> QueryRule<Definition> {
    QueryRule::new(Pass::LateInlineTemps, "inline_temp", function_matcher()).case(
        "late",
        |case, definition| {
            let plan = case.fact(|query| query.inline_temp_candidate(definition, Phase::Late))?;
            let function = case.fact(|query| query.definition_function(definition))?;
            let body = case
                .fact(|query| query.function_snapshot(&function))?
                .body
                .clone();
            case.replace_function_body(function, rewrite_inline_temp(body, plan))
        },
    )
}
