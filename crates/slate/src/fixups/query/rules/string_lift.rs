use crate::fixups::facts::StringRecoveryCandidate;
use crate::fixups::trace::Pass;

use super::super::{
    Definition, DefinitionKind, DefinitionRule, Field, replace_body, rewrite_string_lift,
};

fn function_matcher() -> Definition {
    Definition {
        kind: Field::eq(DefinitionKind::Function),
        ..Default::default()
    }
}

pub(in crate::fixups) fn rewrite() -> DefinitionRule {
    DefinitionRule::matches(Pass::StringLift, "lift_string_buffer", function_matcher()).case(
        "borrowed",
        |case| {
            let plans = case.string_lift_plans(&Field::predicate(|recovery, _| {
                matches!(
                    recovery,
                    StringRecoveryCandidate::BorrowedStr | StringRecoveryCandidate::BorrowedBytes
                )
            }))?;
            let body = case.function_body();
            Ok(replace_body(rewrite_string_lift(body, plans)))
        },
    )
}

pub(in crate::fixups) fn rewrite_c_strings() -> DefinitionRule {
    DefinitionRule::matches(
        Pass::StringLiftFixupCStrings,
        "lift_string_buffer",
        function_matcher(),
    )
    .case("borrowed_cstr", |case| {
        let plans = case.string_lift_plans(&Field::eq(StringRecoveryCandidate::BorrowedCStr))?;
        let body = case.function_body();
        Ok(replace_body(rewrite_string_lift(body, plans)))
    })
}
