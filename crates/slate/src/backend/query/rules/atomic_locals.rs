use crate::backend::trace::Pass;

use super::super::{EditSet, QueryRule, WholeProgram, rewrite_atomic_locals};

pub(in crate::backend) fn rewrite() -> QueryRule<WholeProgram> {
    QueryRule::new(
        Pass::AtomicLocals,
        "promote_atomic_locals",
        WholeProgram::when(|query| query.has_atomic_promotions()),
    )
    .case("complete_domain", |case, program| {
        let promotions = case.fact(|query| query.atomic_promotions())?;
        let rewrite = case.fact(|query| rewrite_atomic_locals(query, promotions))?;
        Ok(EditSet::replace_program(
            program.clone(),
            rewrite.replacement,
            rewrite.removed,
        ))
    })
}
