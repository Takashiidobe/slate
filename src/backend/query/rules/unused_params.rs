use crate::backend::facts::Purity;
use crate::backend::rust_ast::{Expr, Type};
use crate::backend::trace::Pass;

use super::super::{
    BindingCategory, CallRecord, EditSet, FunctionRef, ItemCaseContext, Parameter, ParameterRef,
    QueryRule, Rejection,
};

pub(in crate::backend) fn rewrite() -> QueryRule<Parameter> {
    QueryRule::new(
        Pass::UnusedParams,
        "remove_unused_param",
        Parameter::default(),
    )
    .case("unreachable_param", rewrite_case)
}

fn rewrite_case(
    case: &mut ItemCaseContext<'_, '_>,
    parameter: &ParameterRef,
) -> Result<EditSet, Rejection> {
    case.require(
        parameter
            .binding
            .ty
            .as_ref()
            .is_some_and(trivially_droppable),
    )?;
    let uses = case.fact(|query| query.parameter_uses(parameter))?;
    case.require(uses.uses.is_empty())?;
    let function = case.fact(|query| query.parameter_function(parameter))?;
    let domain = case.fact(|query| query.function_call_domain(&function))?;
    let snapshot = case
        .fact(|query| query.function_snapshot(&function))?
        .clone();
    case.require(parameter.index < snapshot.params.len())?;
    case.require(
        domain
            .calls
            .iter()
            .all(|call| call.args.len() == snapshot.params.len()),
    )?;
    defer_to_later_parameter(case, parameter, &function, &domain.calls)?;

    let mut replacements = Vec::new();
    for call in &domain.calls {
        let argument = case.fact(|query| query.call_argument(call, parameter.index))?;
        let effects = case.fact(|query| query.expression_effects(&argument))?;
        case.require(effects.purity == Purity::MovablePure)?;
        let mut replacement = case
            .expr(&call.site)
            .cloned()
            .ok_or_else(|| case.reject())?;
        let Expr::Call { args, .. } = &mut replacement else {
            return Err(case.reject());
        };
        args.remove(parameter.index);
        replacements.push((call.site.clone(), replacement));
    }

    let mut function_replacement = snapshot;
    function_replacement.params.remove(parameter.index);
    let mut edits = EditSet::replace_function(domain.function, function_replacement);
    for (site, replacement) in replacements {
        edits.push_replace_expression(site, replacement);
    }
    Ok(edits)
}

fn defer_to_later_parameter<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    parameter: &ParameterRef,
    function: &FunctionRef<'db>,
    calls: &[CallRecord],
) -> Result<(), Rejection> {
    let bindings = case.fact(|query| query.function_bindings(function))?;
    for binding in bindings {
        let BindingCategory::Parameter { index } = binding.kind else {
            continue;
        };
        if index <= parameter.index || !binding.ty.as_ref().is_some_and(trivially_droppable) {
            continue;
        }
        let later = ParameterRef { binding, index };
        let uses = case.fact(|query| query.parameter_uses(&later))?;
        if !uses.uses.is_empty() {
            continue;
        }
        let mut removable = true;
        for call in calls {
            let argument = case.fact(|query| query.call_argument(call, index))?;
            let effects = case.fact(|query| query.expression_effects(&argument))?;
            removable &= effects.purity == Purity::MovablePure;
        }
        case.require(!removable)?;
    }
    Ok(())
}

fn trivially_droppable(ty: &Type) -> bool {
    match ty {
        Type::Prim(_) | Type::Unit | Type::Ptr { .. } | Type::Ref { .. } => true,
        Type::Array { elem, .. } => trivially_droppable(elem),
        Type::Custom(_)
        | Type::LongDouble
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::Complex(_)
        | Type::Generic { .. }
        | Type::VaList
        | Type::Str
        | Type::Slice(_)
        | Type::FnPtr { .. }
        | Type::Variadic
        | Type::Never => false,
    }
}
