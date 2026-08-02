use std::collections::BTreeSet;

use crate::fixups::facts::{BorrowAliasReason, CallArgPinning};
use crate::fixups::trace::Pass;
use crate::rust_ast::Type;

use super::super::{
    BindingCategory, BindingRef, EditSet, Function, FunctionRef, ItemCaseContext, QueryRule,
    Rejection, UseSiteRef,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(Pass::RemoveMut, "drop_unneeded_mut", Function::default())
        .case("droppable_mut", rewrite_case)
}

fn rewrite_case(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef,
) -> Result<EditSet, Rejection> {
    let mut replacement = case
        .fact(|query| query.function_snapshot(function))?
        .clone();
    let bindings = case.fact(|query| query.function_bindings(function))?;
    let mut changed = false;

    for (index, param) in replacement.params.iter_mut().enumerate() {
        if matches!(param.ty, Type::Variadic | Type::VaList) {
            continue;
        }
        let Some(binding) = bindings.iter().find(|binding| {
            matches!(binding.kind, BindingCategory::Parameter { index: param_index } if param_index == index)
        }) else {
            continue;
        };
        if param_can_drop_mut(case, binding)? {
            changed |= param.mutable;
            param.mutable = false;
        }
    }

    let mut eligible_locals = BTreeSet::new();
    for local in bindings
        .iter()
        .filter(|binding| matches!(binding.kind, BindingCategory::Local))
    {
        if !case.fact(|query| query.binding_requires_mut(local))? {
            eligible_locals.insert(local.definition.clone());
        }
    }
    changed |= super::super::remove_mut::clear_local_mut(
        &mut replacement.body,
        &eligible_locals,
        &mut Vec::new(),
    );

    if !changed {
        return Err(case.reject());
    }
    Ok(EditSet::replace_function(function.clone(), replacement))
}

fn param_can_drop_mut(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef,
) -> Result<bool, Rejection> {
    if !case.fact(|query| query.binding_requires_mut(binding))? {
        return Ok(true);
    }
    let def_use = case.fact(|query| query.binding_def_use(binding))?;
    if !def_use.writes.is_empty() {
        return Ok(false);
    }
    let Some(reasons) = case.fact(|query| query.borrow_alias_reasons(binding))? else {
        return Ok(true);
    };
    if !reasons.iter().all(|reason| {
        matches!(
            reason,
            BorrowAliasReason::Read | BorrowAliasReason::UnknownCallEscape
        )
    }) {
        return Ok(false);
    }
    for read in &def_use.reads {
        let UseSiteRef::Expression(expr) = read else {
            continue;
        };
        let ok = match case.fact(|query| query.call_argument_pinning(&expr.site)) {
            Ok((pinning, _variadic, declared_ty)) => {
                pinning == CallArgPinning::DeclaredParam
                    && matches!(declared_ty, Some(Type::Ref { mutable: false, .. }))
            }
            Err(_) => true,
        };
        if !ok {
            return Ok(false);
        }
    }
    Ok(true)
}
