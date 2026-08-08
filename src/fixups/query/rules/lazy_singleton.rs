use crate::fixups::trace::Pass;

use super::super::{EditSet, QueryRule, WholeProgram, rewrite_lazy_singletons};

pub(in crate::fixups) fn program() -> QueryRule<WholeProgram> {
    QueryRule::new(
        Pass::LazySingleton,
        "rewrite_lazy_singletons",
        WholeProgram::when(|query| query.has_lazy_singletons()),
    )
    .case("complete_domain", |case, program| {
        let singletons = case.fact(|query| query.lazy_singletons())?;
        let rewrite = case.fact(|query| rewrite_lazy_singletons(query, singletons))?;
        Ok(EditSet::replace_program(
            program.clone(),
            rewrite.replacement,
            rewrite.removed,
        ))
    })
}
