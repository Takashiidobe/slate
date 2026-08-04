use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::heap_ownership;
use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, HeapAllocationKind, HeapExtent, HeapInitKind, HeapReadSafety,
    InterproceduralAllocCallerFact, InterproceduralAllocEligibilityFact, PathSegment,
};
use crate::rust_ast::{Expr, FnDef, IndentStmt, Item, Program, Stmt, Type};

#[derive(Debug, Clone)]
struct ResolvedSummary {
    elem_ty: Type,
    allocation: HeapAllocationKind,
    extent: HeapExtent,
    init: HeapInitKind,
}

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.interprocedural_alloc_eligibility.clear();
    facts.interprocedural_alloc_callers.clear();

    let mut fn_defs: BTreeMap<String, (FunctionId, &FnDef)> = BTreeMap::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        fn_defs.insert(f.name.clone(), (function, f));
    }

    let mut base_summaries: BTreeMap<String, ResolvedSummary> = BTreeMap::new();
    for summary in &facts.callee_alloc_summaries {
        if let Some((name, _)) = fn_defs.iter().find(|(_, (id, _))| *id == summary.function) {
            base_summaries.insert(
                name.clone(),
                ResolvedSummary {
                    elem_ty: summary.elem_ty.clone(),
                    allocation: summary.allocation.clone(),
                    extent: summary.extent.clone(),
                    init: summary.init,
                },
            );
        }
    }

    let mut resolved: BTreeMap<String, ResolvedSummary> = BTreeMap::new();
    for name in fn_defs.keys() {
        let mut visited = BTreeSet::new();
        if let Some(summary) = resolve_summary(name, &fn_defs, &base_summaries, &mut visited) {
            resolved.insert(name.clone(), summary);
        }
    }

    let mut all = Vec::new();
    let mut all_callers = Vec::new();
    for (callee_name, summary) in &resolved {
        let Some((function, _)) = fn_defs.get(callee_name) else {
            continue;
        };
        let mut eligible = true;
        let mut callers = Vec::new();
        for (caller_id, caller_def) in fn_defs.values() {
            let (ok, mut found) =
                caller_calls_for_callee(*caller_id, caller_def, callee_name, &resolved, facts);
            eligible &= ok;
            callers.append(&mut found);
        }
        if eligible {
            all_callers.extend(
                callers
                    .into_iter()
                    .map(|caller| InterproceduralAllocCallerFact {
                        callee: *function,
                        caller: caller.caller,
                        pointer_name: caller.pointer_name,
                        decl_path: caller.decl_path,
                        call_temp_path: caller.call_temp_path,
                        free_path: caller.free_path,
                    }),
            );
        }
        all.push(InterproceduralAllocEligibilityFact {
            function: *function,
            elem_ty: summary.elem_ty.clone(),
            allocation: summary.allocation.clone(),
            extent: summary.extent.clone(),
            init: summary.init,
            eligible,
        });
    }
    facts.interprocedural_alloc_eligibility = all;
    facts.interprocedural_alloc_callers = all_callers;
}

fn resolve_summary(
    name: &str,
    fn_defs: &BTreeMap<String, (FunctionId, &FnDef)>,
    base_summaries: &BTreeMap<String, ResolvedSummary>,
    visited: &mut BTreeSet<String>,
) -> Option<ResolvedSummary> {
    if !visited.insert(name.to_string()) {
        return None;
    }
    if let Some(summary) = base_summaries.get(name) {
        return Some(summary.clone());
    }
    let (_, f) = fn_defs.get(name)?;
    let Some(Type::Ptr { .. }) = &f.ret else {
        return None;
    };
    if count_returns(&f.body) != 1 {
        return None;
    }
    let (_, return_expr) = top_level_return(&f.body)?;
    let Expr::Call { func, .. } = peel_casts(return_expr) else {
        return None;
    };
    let Expr::Var(callee_name) = func.as_ref() else {
        return None;
    };
    resolve_summary(callee_name.as_str(), fn_defs, base_summaries, visited)
}

struct CallerRewritePlan {
    caller: FunctionId,
    pointer_name: String,
    decl_path: AstPath,
    call_temp_path: AstPath,
    free_path: Option<AstPath>,
}

fn caller_calls_for_callee(
    caller_id: FunctionId,
    caller_def: &FnDef,
    callee_name: &str,
    resolved: &BTreeMap<String, ResolvedSummary>,
    facts: &FixupFacts,
) -> (bool, Vec<CallerRewritePlan>) {
    let mut ok = true;
    let mut plans = Vec::new();
    let mut index = 0;
    while index < caller_def.body.len() {
        let Some((pointer_name, _elem_ty)) =
            heap_ownership::null_pointer_decl(&caller_def.body[index].stmt)
        else {
            index += 1;
            continue;
        };
        let Some(outcome) = find_call_allocation(
            caller_id,
            &caller_def.body,
            facts,
            index + 1,
            pointer_name,
            resolved,
        ) else {
            index += 1;
            continue;
        };
        if outcome.callee_name == callee_name {
            if outcome.eligible {
                plans.push(CallerRewritePlan {
                    caller: caller_id,
                    pointer_name: pointer_name.to_string(),
                    decl_path: AstPath(vec![PathSegment::Stmt(index)]),
                    call_temp_path: AstPath(vec![PathSegment::Stmt(outcome.call_temp_index)]),
                    free_path: outcome
                        .free_index
                        .map(|free_index| AstPath(vec![PathSegment::Stmt(free_index)])),
                });
            } else {
                ok = false;
            }
        }
        index += 1;
    }
    (ok, plans)
}

struct CallAllocation {
    temp_name: String,
    callee_name: String,
    summary: ResolvedSummary,
}

fn call_allocation_temp(
    stmt: &Stmt,
    resolved: &BTreeMap<String, ResolvedSummary>,
) -> Option<CallAllocation> {
    let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
    else {
        return None;
    };
    let Expr::Call { func, .. } = init else {
        return None;
    };
    let Expr::Var(callee_name) = func.as_ref() else {
        return None;
    };
    let summary = resolved.get(callee_name.as_str())?.clone();
    Some(CallAllocation {
        temp_name: name.clone(),
        callee_name: callee_name.as_str().to_string(),
        summary,
    })
}

struct CallAllocationOutcome {
    callee_name: String,
    eligible: bool,
    call_temp_index: usize,
    free_index: Option<usize>,
}

fn find_call_allocation(
    function: FunctionId,
    body: &[IndentStmt],
    facts: &FixupFacts,
    start: usize,
    pointer_name: &str,
    resolved: &BTreeMap<String, ResolvedSummary>,
) -> Option<CallAllocationOutcome> {
    for allocation_index in start..body.len() {
        let Some(call_alloc) = call_allocation_temp(&body[allocation_index].stmt, resolved) else {
            continue;
        };
        let allocation_path = AstPath(vec![PathSegment::Stmt(allocation_index)]);
        let Some(allocation_temp) =
            facts.binding_by_local_path(function, &call_alloc.temp_name, &allocation_path)
        else {
            continue;
        };
        for assign_index in allocation_index + 1..body.len() {
            if !heap_ownership::assigns_allocated_pointer(
                &body[assign_index].stmt,
                pointer_name,
                &call_alloc.temp_name,
            ) {
                continue;
            }
            let candidate = heap_ownership::Candidate {
                allocation_index,
                assign_index,
                free_index: 0,
                allocation_temp,
                size_temp: None,
                free_temp: None,
                aliases: Vec::new(),
                allocation: call_alloc.summary.allocation.clone(),
                extent: call_alloc.summary.extent.clone(),
                init: call_alloc.summary.init,
                elem_ty: call_alloc.summary.elem_ty.clone(),
                read_safety: HeapReadSafety::MayReadUninitialized,
                uses: Vec::new(),
                reallocations: Vec::new(),
            };
            let outcome = heap_ownership::heap_uses_are_owned(
                function,
                body,
                facts,
                pointer_name,
                &candidate,
            );
            let (eligible, free_index) = match outcome {
                Some((free_index, _, _, _, _, read_safety)) => {
                    let extent_ok = match &call_alloc.summary.extent {
                        HeapExtent::Scalar => true,
                        HeapExtent::Elements { .. } => {
                            read_safety != HeapReadSafety::MayReadUninitialized
                        }
                        HeapExtent::Unknown => false,
                    };
                    (extent_ok, free_index)
                }
                None => (false, None),
            };
            return Some(CallAllocationOutcome {
                callee_name: call_alloc.callee_name,
                eligible,
                call_temp_index: allocation_index,
                free_index,
            });
        }
    }
    None
}

fn count_returns(body: &[IndentStmt]) -> usize {
    let mut count = 0;
    for indent in body {
        if matches!(&indent.stmt, Stmt::Return(Some(_))) {
            count += 1;
        }
        crate::fixups::facts::walk::nested_bodies_with_path(
            &indent.stmt,
            &mut Vec::new(),
            &mut |nested, _| {
                count += count_returns(nested);
            },
        );
    }
    count
}

fn top_level_return(body: &[IndentStmt]) -> Option<(usize, &Expr)> {
    body.iter()
        .enumerate()
        .find_map(|(index, indent)| match &indent.stmt {
            Stmt::Return(Some(expr)) => Some((index, expr)),
            _ => None,
        })
}

fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        _ => expr,
    }
}
