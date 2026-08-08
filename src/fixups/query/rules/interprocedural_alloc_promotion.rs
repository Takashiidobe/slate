use std::collections::BTreeMap;

use crate::fixups::facts::{AllocProvenance, HeapExtent, HeapOwnershipKind};
use crate::fixups::trace::Pass;

use super::super::{
    EditSet, Function, FunctionRef, InterproceduralAllocCalleePlan,
    InterproceduralAllocCallerInput, InterproceduralAllocCallerPlan, ItemCaseContext, QueryRule,
    Rejection, rewrite_interprocedural_alloc_callee, rewrite_interprocedural_alloc_caller,
    rewrite_interprocedural_alloc_wrapper,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(
        Pass::InterproceduralAllocPromotion,
        "promote_interprocedural_alloc",
        Function::default(),
    )
    .case("eligible_allocator", |case, function| {
        let summary = case.fact(|query| query.callee_alloc_summary(function))?;
        let AllocProvenance::Direct {
            elem_ty,
            extent,
            return_path,
            alloc_source_path,
            ..
        } = summary.provenance.clone()
        else {
            return Err(case.reject());
        };
        let eligibility = case.fact(|query| query.interprocedural_alloc_eligibility(function))?;
        case.require(eligibility.eligible)?;
        let callers = case.fact(|query| query.interprocedural_alloc_callers(function))?;
        case.require(!callers.is_empty())?;

        let (kind, count) = match &extent {
            HeapExtent::Scalar => (HeapOwnershipKind::ScalarBox, None),
            HeapExtent::Elements { count } => (HeapOwnershipKind::VecBuffer, Some(count.clone())),
            HeapExtent::Unknown => return Err(case.reject()),
        };

        let callee_fndef = case
            .fact(|query| query.function_snapshot(function))?
            .clone();
        let callee_plan = InterproceduralAllocCalleePlan {
            elem_ty: elem_ty.clone(),
            kind,
            count,
            return_path,
            alloc_source_path,
        };
        let new_callee = rewrite_interprocedural_alloc_callee(callee_fndef, &callee_plan);
        let mut edits = EditSet::replace_function(function.clone(), new_callee);

        let chain = case.fact(|query| query.interprocedural_alloc_chain(function))?;
        for wrapper_ref in &chain {
            let wrapper_fndef = case
                .fact(|query| query.function_snapshot(wrapper_ref))?
                .clone();
            let new_wrapper = rewrite_interprocedural_alloc_wrapper(wrapper_fndef, kind, &elem_ty);
            edits.extend(EditSet::replace_function(wrapper_ref.clone(), new_wrapper));
        }

        for (caller_ref, plans) in group_callers_by_function(case, &callers, kind, &elem_ty)? {
            let caller_fndef = case
                .fact(|query| query.function_snapshot(&caller_ref))?
                .clone();
            let mut new_caller = caller_fndef;
            new_caller.body = rewrite_interprocedural_alloc_caller(new_caller.body, &plans).lower();
            edits.extend(EditSet::replace_function(caller_ref, new_caller));
        }

        Ok(edits)
    })
}

fn group_callers_by_function<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    callers: &[InterproceduralAllocCallerInput<'db>],
    kind: HeapOwnershipKind,
    elem_ty: &crate::rust_ast::Type,
) -> Result<Vec<(FunctionRef<'db>, Vec<InterproceduralAllocCallerPlan>)>, Rejection> {
    let mut grouped: BTreeMap<usize, (FunctionRef<'db>, Vec<InterproceduralAllocCallerPlan>)> =
        BTreeMap::new();
    for caller in callers {
        let _ = case.fact(|query| query.statement(&caller.decl_stmt))?;
        let _ = case.fact(|query| query.statement(&caller.call_temp_stmt))?;
        if let Some(free_stmt) = &caller.free_stmt {
            let _ = case.fact(|query| query.statement(free_stmt))?;
        }
        let plan = InterproceduralAllocCallerPlan {
            pointer_name: caller.pointer_name.clone(),
            kind,
            elem_ty: elem_ty.clone(),
            decl_path: caller.decl_stmt.path.clone(),
            call_temp_path: caller.call_temp_stmt.path.clone(),
            free_path: caller.free_stmt.as_ref().map(|stmt| stmt.path.clone()),
        };
        grouped
            .entry(caller.caller.item_index)
            .or_insert_with(|| (caller.caller.clone(), Vec::new()))
            .1
            .push(plan);
    }
    Ok(grouped.into_values().collect())
}
