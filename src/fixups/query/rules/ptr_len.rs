use crate::fixups::trace::Pass;

use super::super::{ProgramRule, rewrite_ptr_len};

pub(in crate::fixups) fn program() -> ProgramRule {
    ProgramRule::ptr_len(Pass::PtrLen, "rewrite_ptr_len_slice_params").case(
        "ptr_len_slices",
        |case| {
            let plans = case.ptr_len_slices()?;
            Ok(rewrite_ptr_len(plans))
        },
    )
}
