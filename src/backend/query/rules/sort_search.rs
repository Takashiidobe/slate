use crate::backend::trace::Pass;

use super::super::{EditSet, QueryRule, WholeProgram, rewrite_sort_search};

pub(in crate::backend) fn program() -> QueryRule<WholeProgram> {
    QueryRule::new(
        Pass::SortSearch,
        "rewrite_sort_search_calls",
        WholeProgram::when(|query| query.has_sort_search_calls()),
    )
    .case("sort_search", |case, program| {
        let rewrite = case.fact(|query| rewrite_sort_search(query))?;
        Ok(EditSet::replace_program(
            program.clone(),
            rewrite.replacement,
            rewrite.removed,
        ))
    })
}
