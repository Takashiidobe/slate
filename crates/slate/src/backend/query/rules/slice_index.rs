use crate::backend::trace::Pass;

use super::super::{EditSet, Function, FunctionRef, ItemCaseContext, QueryRule, Rejection};

pub(in crate::backend) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(
        Pass::SliceIndex,
        "rewrite_slice_pointer_indexes",
        Function::default(),
    )
    .case("rewrite", rewrite_case)
}

fn rewrite_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef<'db>,
) -> Result<EditSet, Rejection> {
    let mut replacement = case
        .fact(|query| query.function_snapshot(function))?
        .clone();
    let plans = super::super::slice_index::plans_for_function(&replacement);
    if plans.is_empty() {
        return Err(case.reject());
    }

    let mut changed =
        super::super::slice_index::rewrite_body(&mut replacement.body, &plans, &mut Vec::new());
    changed |= super::super::slice_index::prune_dead_fact_temps(&mut replacement.body, &plans);
    if !changed {
        return Err(case.reject());
    }
    Ok(EditSet::replace_function(function.clone(), replacement))
}
