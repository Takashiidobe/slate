use crate::backend::trace::Pass;

use super::super::{EditSet, QueryRule, WholeProgram, rewrite_anonymous_structs};

pub(in crate::backend) fn program() -> QueryRule<WholeProgram> {
    QueryRule::new(
        Pass::AnonymousStructs,
        "rewrite_anonymous_structs",
        WholeProgram::when(|query| query.has_anonymous_structs()),
    )
    .case("complete_domain", |case, program| {
        let structs = case.fact(|query| query.anonymous_structs())?;
        let rewrite = case.fact(|query| rewrite_anonymous_structs(query, structs))?;
        Ok(EditSet::replace_program(
            program.clone(),
            rewrite.replacement,
            rewrite.removed,
        ))
    })
}
