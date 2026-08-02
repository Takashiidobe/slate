use crate::fixups::facts::{
    HeapAllocationKind, HeapExtent, HeapInitKind, HeapOwnershipKind, HeapReadSafety, HeapUseKind,
    PathSegment,
};
use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, RustValue};

use super::super::{
    BindingRef, Function, HeapOwnership, HeapOwnershipPlan, HeapOwnershipReallocPlan,
    ItemCaseContext, QueryRule, Rejection, StatementRef, default_value, rewrite_heap_ownership,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(
        Pass::HeapOwnership,
        "rewrite_heap_ownership",
        Function::default(),
    )
    .case("owned_heap", |case, function| {
        let facts = case.fact(|query| query.heap_ownership_facts(function))?;
        let plans = facts
            .owners
            .iter()
            .filter_map(|owner| ownership_plan(case, owner).ok())
            .collect::<Vec<_>>();
        case.require(!plans.is_empty())?;
        let body = case
            .fact(|query| query.function_snapshot(function))?
            .body
            .clone();
        case.replace_function_body(function.clone(), rewrite_heap_ownership(body, plans))
    })
}

fn ownership_plan(
    case: &mut ItemCaseContext<'_, '_>,
    owner: &HeapOwnership,
) -> Result<HeapOwnershipPlan, Rejection> {
    for statement in [
        &owner.pointer_statement,
        &owner.allocation_statement,
        &owner.assignment_statement,
        &owner.free_statement,
    ] {
        let _ = case.fact(|query| query.statement(statement))?;
    }
    let _ = case.fact(|query| query.binding_uses(&owner.pointer))?;
    let _ = case.fact(|query| query.binding_uses(&owner.allocation_temp))?;
    if let Some(binding) = &owner.size_temp {
        let _ = case.fact(|query| query.binding_uses(binding))?;
    }
    if let Some(binding) = &owner.free_temp {
        let _ = case.fact(|query| query.binding_uses(binding))?;
    }
    for alias in &owner.aliases {
        let _ = case.fact(|query| query.binding_initializer(alias))?;
        let _ = case.fact(|query| query.binding_uses(alias))?;
    }
    for usage in &owner.uses {
        let _ = case.fact(|query| query.statement(&usage.statement))?;
    }
    case.require(owner.uses.iter().any(|usage| {
        matches!(usage.kind, HeapUseKind::Free) && usage.statement == owner.free_statement
    }))?;
    case.require(matches!(
        (&owner.allocation, owner.init),
        (HeapAllocationKind::Malloc, HeapInitKind::Uninitialized)
            | (HeapAllocationKind::Calloc, HeapInitKind::Zeroed)
    ))?;

    let (kind, count) = match &owner.extent {
        HeapExtent::Scalar => (
            HeapOwnershipKind::ScalarBox,
            Some(Expr::Value(RustValue::I64(1))),
        ),
        HeapExtent::Elements { count }
            if owner.read_safety != HeapReadSafety::MayReadUninitialized =>
        {
            (HeapOwnershipKind::VecBuffer, Some(count.clone()))
        }
        HeapExtent::Elements { .. } | HeapExtent::Unknown => {
            return Err(case.reject());
        }
    };
    let reallocs = owner
        .reallocations
        .iter()
        .map(|realloc| {
            case.require(realloc.init == HeapInitKind::Uninitialized)?;
            let _ = case.fact(|query| query.statement(&realloc.allocation_statement))?;
            let _ = case.fact(|query| query.statement(&realloc.assignment_statement))?;
            let _ = case.fact(|query| query.binding_uses(&realloc.allocation_temp))?;
            if let Some(binding) = &realloc.source_temp {
                let _ = case.fact(|query| query.binding_uses(binding))?;
            }
            if let Some(binding) = &realloc.size_temp {
                let _ = case.fact(|query| query.binding_uses(binding))?;
            }
            let count = match &realloc.new_extent {
                HeapExtent::Scalar => Expr::Value(RustValue::I64(1)),
                HeapExtent::Elements { count } => count.clone(),
                HeapExtent::Unknown => return Err(case.reject()),
            };
            Ok(HeapOwnershipReallocPlan {
                source_temp_stmt: optional_binding_index(case, &realloc.source_temp)?,
                size_stmt: optional_binding_index(case, &realloc.size_temp)?,
                allocation_stmt: Some(statement_index(case, &realloc.allocation_statement)?),
                assign_stmt: Some(statement_index(case, &realloc.assignment_statement)?),
                resize: realloc.resize,
                count,
            })
        })
        .collect::<Result<Vec<_>, Rejection>>()?;
    Ok(HeapOwnershipPlan {
        pointer_name: owner.pointer.name.clone(),
        kind,
        pointer_stmt: Some(statement_index(case, &owner.pointer_statement)?),
        size_stmt: optional_binding_index(case, &owner.size_temp)?,
        allocation_stmt: Some(statement_index(case, &owner.allocation_statement)?),
        assign_stmt: Some(statement_index(case, &owner.assignment_statement)?),
        free_temp_stmt: optional_binding_index(case, &owner.free_temp)?,
        free_stmt: Some(statement_index(case, &owner.free_statement)?),
        reallocs,
        elem_ty: owner.elem_ty.clone(),
        init: default_value(&owner.elem_ty),
        count,
    })
}

fn optional_binding_index(
    case: &ItemCaseContext<'_, '_>,
    binding: &Option<BindingRef>,
) -> Result<Option<usize>, Rejection> {
    let Some(binding) = binding else {
        return Ok(None);
    };
    match binding.definition.0.as_slice() {
        [PathSegment::Stmt(index)] => Ok(Some(*index)),
        _ => Err(case.reject()),
    }
}

fn statement_index(
    case: &ItemCaseContext<'_, '_>,
    statement: &StatementRef,
) -> Result<usize, Rejection> {
    match statement.path.0.as_slice() {
        [PathSegment::Stmt(index)] => Ok(*index),
        _ => Err(case.reject()),
    }
}
