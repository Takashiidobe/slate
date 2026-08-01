use crate::fixups::trace::Pass;

use super::super::{
    Definition, DefinitionKind, DefinitionRule, Field, replace_body,
    rewrite_array_element_pointer_origins,
};

pub(in crate::fixups) fn rewrite() -> DefinitionRule {
    DefinitionRule::matches(
        Pass::ArrayElementPointerOrigin,
        "rewrite_array_element_pointer_origins",
        Definition {
            kind: Field::eq(DefinitionKind::Function),
            ..Default::default()
        },
    )
    .case("known_origins", |case| {
        let origins = case.array_element_pointer_origins()?;
        let body = case.function_body();
        let body = rewrite_array_element_pointer_origins(body, origins)
            .ok_or_else(|| case.unsupported_array_element_pointer_origin())?;
        Ok(replace_body(body))
    })
}
