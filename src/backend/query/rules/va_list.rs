use crate::backend::trace::Pass;

use super::super::{EditSet, Function, FunctionRef, ItemCaseContext, QueryRule, Rejection};

pub(in crate::backend) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(
        Pass::VaList,
        "remove_va_list_clone_alias",
        Function::default(),
    )
    .case("unique_alias", rewrite_case)
}

fn rewrite_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef<'db>,
) -> Result<EditSet, Rejection> {
    let alias = case.fact(|query| query.va_list_alias(function))?;
    let mut replacement = case
        .fact(|query| query.function_snapshot(function))?
        .clone();
    case.require(alias.param_index < replacement.params.len())?;
    if !super::super::va_list::remove_alias_stmts(
        &mut replacement.body,
        &alias.local_decl,
        &alias.clone_assign,
    ) {
        return Err(case.reject());
    }
    replacement.params[alias.param_index].name = alias.local_name.clone();
    Ok(EditSet::replace_function(function.clone(), replacement))
}
