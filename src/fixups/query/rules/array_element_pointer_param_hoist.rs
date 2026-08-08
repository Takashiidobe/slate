use crate::fixups::facts::AstPath;
use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, Ident, Type};

use super::super::{
    BindingAccess, CallRecord, EditSet, ItemCaseContext, Parameter, ParameterRef, QueryRule,
    Rejection, ValueSite,
};

struct ScalarOrigin {
    base_name: String,
    index: Expr,
}

pub(in crate::fixups) fn rewrite() -> QueryRule<Parameter> {
    QueryRule::new(
        Pass::ArrayElementPointerParamHoist,
        "hoist_single_element_pointer_param",
        Parameter::default(),
    )
    .case("proven_scalar_origin", rewrite_case)
}

fn rewrite_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    parameter: &ParameterRef<'db>,
) -> Result<EditSet, Rejection> {
    let Some(Type::Ptr { mutable, inner }) = parameter.binding.ty.clone() else {
        return Err(case.reject());
    };

    let uses = case.fact(|query| query.parameter_uses(parameter))?;
    case.require(!uses.uses.is_empty())?;
    for use_ in &uses.uses {
        case.require(use_.access == BindingAccess::Read)?;
        let expression = use_.expression().ok_or_else(|| case.reject())?;
        case.require(case.is_bare_pointer_dereference(expression))?;
    }

    let function = case.fact(|query| query.parameter_function(parameter))?;
    let snapshot = case
        .fact(|query| query.function_snapshot(&function))?
        .clone();
    case.require(parameter.index < snapshot.params.len())?;

    let domain = case.fact(|query| query.function_call_domain(&function))?;
    case.require(!domain.calls.is_empty())?;
    case.require(
        domain
            .calls
            .iter()
            .all(|call| call.args.len() == snapshot.params.len()),
    )?;

    let mut call_origins = Vec::with_capacity(domain.calls.len());
    for call in &domain.calls {
        let origin = call_argument_origin(case, call, parameter)?;
        call_origins.push((call.clone(), origin));
    }

    let param_name = snapshot.params[parameter.index].name.clone();
    let mut function_replacement = snapshot;
    function_replacement.params[parameter.index].ty = Type::Ref { mutable, inner };
    crate::fixups::query::recipe::strip_redundant_unsafe(
        &mut function_replacement.body,
        &param_name,
    );
    let mut edits = EditSet::replace_function(domain.function, function_replacement);
    for (call, origin) in call_origins {
        let mut replacement = case
            .expr(&call.site)
            .cloned()
            .ok_or_else(|| case.reject())?;
        let Expr::Call { args, .. } = &mut replacement else {
            return Err(case.reject());
        };
        args[parameter.index] = Expr::Ref {
            mutable,
            expr: Box::new(Expr::Index {
                base: Box::new(Expr::Var(Ident::from(origin.base_name.as_str()))),
                index: Box::new(origin.index.clone()),
            }),
        };
        edits.push_replace_expression(call.site.clone(), replacement);
    }
    Ok(edits)
}

fn call_argument_origin<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
    parameter: &ParameterRef<'db>,
) -> Result<ScalarOrigin, Rejection> {
    let argument = case.fact(|query| query.call_argument(call, parameter.index))?;
    let expr = case
        .expr(&argument.site)
        .cloned()
        .ok_or_else(|| case.reject())?;
    if let Some(origin) = inline_scalar_origin(&expr) {
        return Ok(origin);
    }
    let Expr::Var(name) = peel_trivial(&expr) else {
        return Err(case.reject());
    };
    let origin = case.fact(|query| {
        query.pointer_origin(
            &ValueSite {
                item_index: call.site.item_index,
                path: AstPath::default(),
            },
            name.as_str(),
        )
    })?;
    Ok(ScalarOrigin {
        base_name: origin.base_name,
        index: origin.index,
    })
}

fn inline_scalar_origin(expr: &Expr) -> Option<ScalarOrigin> {
    let (Expr::AddrOf { expr, .. } | Expr::Ref { expr, .. }) = expr else {
        return None;
    };
    let Expr::Index { base, index } = peel_trivial(expr) else {
        return None;
    };
    let Expr::Var(base_name) = peel_trivial(base) else {
        return None;
    };
    Some(ScalarOrigin {
        base_name: base_name.as_str().into(),
        index: (**index).clone(),
    })
}

fn peel_trivial(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            block.tail.as_deref().map_or(expr, peel_trivial)
        }
        Expr::Cast { expr, .. } => peel_trivial(expr),
        _ => expr,
    }
}
