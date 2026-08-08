use crate::fixups::trace::Pass;

use super::super::{EditSet, QueryRule, WholeProgram, rewrite_ptr_len};

pub(in crate::fixups) fn program() -> QueryRule<WholeProgram> {
    QueryRule::new(
        Pass::PtrLen,
        "rewrite_ptr_len_slice_params",
        WholeProgram::when(|query| query.has_ptr_len_slices()),
    )
    .case("ptr_len_slices", |case, program| {
        let plans = case.fact(|query| query.ptr_len_slices())?;
        let rewrite = case.fact(|query| rewrite_ptr_len(query, plans))?;
        Ok(EditSet::replace_program(
            program.clone(),
            rewrite.replacement,
            rewrite.removed,
        ))
    })
}
