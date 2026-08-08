use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::heap_ownership;
use crate::fixups::facts::{
    AllocProvenance, AstPath, BindingFact, CalleeAllocSummaryFact, FixupFacts, FunctionId,
    HeapAllocationKind, HeapExtent, HeapInitKind, HeapReadSafety, InterproceduralAllocCallerFact,
    InterproceduralAllocEligibilityFact, PathSegment,
};
use crate::rust_ast::{Expr, IndentStmt, Item, Program, Stmt, Type};

#[derive(Debug, Clone)]
struct ResolvedSummary {
    elem_ty: Type,
    allocation: HeapAllocationKind,
    extent: HeapExtent,
    init: HeapInitKind,
}

pub(in crate::fixups) struct FunctionSummary<'a> {
    pub(in crate::fixups) id: FunctionId,
    pub(in crate::fixups) name: &'a str,
    pub(in crate::fixups) body: &'a [IndentStmt],
    pub(in crate::fixups) bindings: &'a [BindingFact],
    pub(in crate::fixups) callee_alloc_summary: Option<&'a CalleeAllocSummaryFact>,
}

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    let mut bindings_by_function: BTreeMap<FunctionId, Vec<BindingFact>> = BTreeMap::new();
    for binding in &facts.bindings {
        bindings_by_function
            .entry(binding.function)
            .or_default()
            .push(binding.clone());
    }
    let mut summaries_by_function: BTreeMap<FunctionId, &CalleeAllocSummaryFact> = facts
        .callee_alloc_summaries
        .iter()
        .map(|fact| (fact.function, fact))
        .collect();

    let mut functions = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        functions.push(FunctionSummary {
            id: function,
            name: f.name.as_str(),
            body: &f.body,
            bindings: bindings_by_function
                .get(&function)
                .map_or(&[][..], |v| v.as_slice()),
            callee_alloc_summary: summaries_by_function.remove(&function),
        });
    }
    let (eligibility, callers) = collect(&functions);
    facts.interprocedural_alloc_eligibility = eligibility;
    facts.interprocedural_alloc_callers = callers;
}

pub(in crate::fixups) fn collect(
    functions: &[FunctionSummary],
) -> (
    Vec<InterproceduralAllocEligibilityFact>,
    Vec<InterproceduralAllocCallerFact>,
) {
    let mut by_name: BTreeMap<&str, FunctionId> = BTreeMap::new();
    let mut by_function: BTreeMap<FunctionId, &CalleeAllocSummaryFact> = BTreeMap::new();
    for summary in functions {
        by_name.insert(summary.name, summary.id);
        if let Some(fact) = summary.callee_alloc_summary {
            by_function.insert(summary.id, fact);
        }
    }

    // Resolve every function that has a provenance fact to its ultimate (root, summary),
    // walking Direct/PassThrough links -- the chain itself falls out as a by-product below,
    // rather than needing its own separately-tracked fact.
    let mut memo: BTreeMap<FunctionId, Option<(ResolvedSummary, FunctionId)>> = BTreeMap::new();
    for function in by_function.keys() {
        resolve_root(
            *function,
            &by_function,
            &by_name,
            &mut memo,
            &mut BTreeSet::new(),
        );
    }
    let resolved: BTreeMap<FunctionId, (ResolvedSummary, FunctionId)> = memo
        .into_iter()
        .filter_map(|(function, outcome)| outcome.map(|outcome| (function, outcome)))
        .collect();

    let mut chain_of: BTreeMap<FunctionId, Vec<FunctionId>> = BTreeMap::new();
    for (function, (_, root)) in &resolved {
        if function != root {
            chain_of.entry(*root).or_default().push(*function);
        }
    }

    let by_id: BTreeMap<FunctionId, &FunctionSummary> = functions
        .iter()
        .map(|summary| (summary.id, summary))
        .collect();

    let mut all = Vec::new();
    let mut all_callers = Vec::new();
    for (root, (summary, resolved_root)) in &resolved {
        if root != resolved_root {
            continue;
        }
        let chain = chain_of.get(root).cloned().unwrap_or_default();
        let mut members: Vec<&str> = chain
            .iter()
            .filter_map(|id| by_id.get(id).map(|summary| summary.name))
            .collect();
        if let Some(root_summary) = by_id.get(root) {
            members.push(root_summary.name);
        }

        let mut eligible = true;
        let mut callers = Vec::new();
        for member_name in &members {
            for caller in functions {
                let (ok, mut found) =
                    caller_calls_for_callee(caller, member_name, &resolved, &by_name);
                eligible &= ok;
                callers.append(&mut found);
            }
        }

        if eligible {
            all_callers.extend(
                callers
                    .into_iter()
                    .map(|caller| InterproceduralAllocCallerFact {
                        callee: *root,
                        caller: caller.caller,
                        pointer_name: caller.pointer_name,
                        decl_path: caller.decl_path,
                        call_temp_path: caller.call_temp_path,
                        free_path: caller.free_path,
                    }),
            );
        }
        all.push(InterproceduralAllocEligibilityFact {
            function: *root,
            elem_ty: summary.elem_ty.clone(),
            allocation: summary.allocation,
            extent: summary.extent.clone(),
            init: summary.init,
            eligible,
            chain,
        });
    }
    (all, all_callers)
}

fn resolve_root(
    function: FunctionId,
    by_function: &BTreeMap<FunctionId, &CalleeAllocSummaryFact>,
    by_name: &BTreeMap<&str, FunctionId>,
    memo: &mut BTreeMap<FunctionId, Option<(ResolvedSummary, FunctionId)>>,
    visited: &mut BTreeSet<FunctionId>,
) -> Option<(ResolvedSummary, FunctionId)> {
    if let Some(cached) = memo.get(&function) {
        return cached.clone();
    }
    if !visited.insert(function) {
        return None;
    }
    let fact = by_function.get(&function)?;
    let result = match &fact.provenance {
        AllocProvenance::Direct {
            elem_ty,
            allocation,
            extent,
            init,
            ..
        } => Some((
            ResolvedSummary {
                elem_ty: elem_ty.clone(),
                allocation: *allocation,
                extent: extent.clone(),
                init: *init,
            },
            function,
        )),
        AllocProvenance::PassThrough { callees } => {
            let mut agreed: Option<(ResolvedSummary, FunctionId)> = None;
            for callee_name in callees {
                let callee = *by_name.get(callee_name.as_str())?;
                let resolved = resolve_root(callee, by_function, by_name, memo, visited)?;
                match &agreed {
                    None => agreed = Some(resolved),
                    Some((_, existing_root)) if resolved.1 == *existing_root => {}
                    Some(_) => return None,
                }
            }
            agreed
        }
    };
    memo.insert(function, result.clone());
    result
}

struct CallerRewritePlan {
    caller: FunctionId,
    pointer_name: String,
    decl_path: AstPath,
    call_temp_path: AstPath,
    free_path: Option<AstPath>,
}

fn caller_calls_for_callee(
    caller: &FunctionSummary,
    callee_name: &str,
    resolved: &BTreeMap<FunctionId, (ResolvedSummary, FunctionId)>,
    by_name: &BTreeMap<&str, FunctionId>,
) -> (bool, Vec<CallerRewritePlan>) {
    let mut ok = true;
    let mut plans = Vec::new();
    let mut index = 0;
    while index < caller.body.len() {
        let Some((pointer_name, _elem_ty)) =
            heap_ownership::null_pointer_decl(&caller.body[index].stmt)
        else {
            index += 1;
            continue;
        };
        let Some(outcome) =
            find_call_allocation(caller, index + 1, pointer_name, resolved, by_name)
        else {
            index += 1;
            continue;
        };
        if outcome.callee_name == callee_name {
            if outcome.eligible {
                plans.push(CallerRewritePlan {
                    caller: caller.id,
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
    resolved: &BTreeMap<FunctionId, (ResolvedSummary, FunctionId)>,
    by_name: &BTreeMap<&str, FunctionId>,
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
    let callee_id = *by_name.get(callee_name.as_str())?;
    let (summary, _) = resolved.get(&callee_id)?.clone();
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
    caller: &FunctionSummary,
    start: usize,
    pointer_name: &str,
    resolved: &BTreeMap<FunctionId, (ResolvedSummary, FunctionId)>,
    by_name: &BTreeMap<&str, FunctionId>,
) -> Option<CallAllocationOutcome> {
    let local_facts = FixupFacts {
        bindings: caller.bindings.to_vec(),
        ..FixupFacts::default()
    };
    let body = caller.body;
    for allocation_index in start..body.len() {
        let Some(call_alloc) =
            call_allocation_temp(&body[allocation_index].stmt, resolved, by_name)
        else {
            continue;
        };
        let allocation_path = AstPath(vec![PathSegment::Stmt(allocation_index)]);
        let Some(allocation_temp) =
            local_facts.binding_by_local_path(caller.id, &call_alloc.temp_name, &allocation_path)
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
                allocation: call_alloc.summary.allocation,
                extent: call_alloc.summary.extent.clone(),
                init: call_alloc.summary.init,
                elem_ty: call_alloc.summary.elem_ty.clone(),
                read_safety: HeapReadSafety::MayReadUninitialized,
                uses: Vec::new(),
                reallocations: Vec::new(),
            };
            let outcome = heap_ownership::heap_uses_are_owned(
                caller.id,
                body,
                &local_facts,
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
