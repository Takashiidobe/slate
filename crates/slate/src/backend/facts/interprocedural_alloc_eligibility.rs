use std::collections::{BTreeMap, BTreeSet};

use crate::backend::facts::heap_ownership;
use crate::backend::facts::{
    self, AllocProvenance, AstPath, BindingFact, CalleeAllocSummaryFact, FunctionId,
    HeapAllocationKind, HeapExtent, HeapInitKind, HeapReadSafety, InterproceduralAllocCallerFact,
    InterproceduralAllocEligibilityFact, PathSegment,
};
use crate::backend::rust_ast::{Expr, IndentStmt, Stmt, Type};

#[derive(Debug, Clone)]
struct ResolvedSummary {
    elem_ty: Type,
    allocation: HeapAllocationKind,
    extent: HeapExtent,
    init: HeapInitKind,
}

pub(in crate::backend) struct FunctionSummary<'db, 'a> {
    pub(in crate::backend) id: FunctionId<'db>,
    pub(in crate::backend) name: &'a str,
    pub(in crate::backend) body: &'a [IndentStmt],
    pub(in crate::backend) bindings: &'a [BindingFact<'db>],
    pub(in crate::backend) callee_alloc_summary: Option<&'a CalleeAllocSummaryFact<'db>>,
}
pub(in crate::backend) fn collect<'db, 'a>(
    functions: &[FunctionSummary<'db, 'a>],
) -> (
    Vec<InterproceduralAllocEligibilityFact<'db>>,
    Vec<InterproceduralAllocCallerFact<'db>>,
) {
    let mut by_name: BTreeMap<&str, FunctionId<'db>> = BTreeMap::new();
    let mut by_function: BTreeMap<FunctionId<'db>, &CalleeAllocSummaryFact<'db>> = BTreeMap::new();
    for summary in functions {
        by_name.insert(summary.name, summary.id);
        if let Some(fact) = summary.callee_alloc_summary {
            by_function.insert(summary.id, fact);
        }
    }

    // Resolve every function that has a provenance fact to its ultimate (root, summary),
    // walking Direct/PassThrough links -- the chain itself falls out as a by-product below,
    // rather than needing its own separately-tracked fact.
    let mut memo: BTreeMap<FunctionId<'db>, Option<(ResolvedSummary, FunctionId<'db>)>> =
        BTreeMap::new();
    for function in by_function.keys() {
        resolve_root(
            *function,
            &by_function,
            &by_name,
            &mut memo,
            &mut BTreeSet::new(),
        );
    }
    let resolved: BTreeMap<FunctionId<'db>, (ResolvedSummary, FunctionId<'db>)> = memo
        .into_iter()
        .filter_map(|(function, outcome)| outcome.map(|outcome| (function, outcome)))
        .collect();

    let mut chain_of: BTreeMap<FunctionId<'db>, Vec<FunctionId<'db>>> = BTreeMap::new();
    for (function, (_, root)) in &resolved {
        if function != root {
            chain_of.entry(*root).or_default().push(*function);
        }
    }

    let by_id: BTreeMap<FunctionId<'db>, &FunctionSummary<'db, 'a>> = functions
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

fn resolve_root<'db>(
    function: FunctionId<'db>,
    by_function: &BTreeMap<FunctionId<'db>, &CalleeAllocSummaryFact<'db>>,
    by_name: &BTreeMap<&str, FunctionId<'db>>,
    memo: &mut BTreeMap<FunctionId<'db>, Option<(ResolvedSummary, FunctionId<'db>)>>,
    visited: &mut BTreeSet<FunctionId<'db>>,
) -> Option<(ResolvedSummary, FunctionId<'db>)> {
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
            let mut agreed: Option<(ResolvedSummary, FunctionId<'db>)> = None;
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

struct CallerRewritePlan<'db> {
    caller: FunctionId<'db>,
    pointer_name: String,
    decl_path: AstPath,
    call_temp_path: AstPath,
    free_path: Option<AstPath>,
}

fn caller_calls_for_callee<'db>(
    caller: &FunctionSummary<'db, '_>,
    callee_name: &str,
    resolved: &BTreeMap<FunctionId<'db>, (ResolvedSummary, FunctionId<'db>)>,
    by_name: &BTreeMap<&str, FunctionId<'db>>,
) -> (bool, Vec<CallerRewritePlan<'db>>) {
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

fn call_allocation_temp<'db>(
    stmt: &Stmt,
    resolved: &BTreeMap<FunctionId<'db>, (ResolvedSummary, FunctionId<'db>)>,
    by_name: &BTreeMap<&str, FunctionId<'db>>,
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

fn find_call_allocation<'db>(
    caller: &FunctionSummary<'db, '_>,
    start: usize,
    pointer_name: &str,
    resolved: &BTreeMap<FunctionId<'db>, (ResolvedSummary, FunctionId<'db>)>,
    by_name: &BTreeMap<&str, FunctionId<'db>>,
) -> Option<CallAllocationOutcome> {
    let body = caller.body;
    for allocation_index in start..body.len() {
        let Some(call_alloc) =
            call_allocation_temp(&body[allocation_index].stmt, resolved, by_name)
        else {
            continue;
        };
        let allocation_path = AstPath(vec![PathSegment::Stmt(allocation_index)]);
        let Some(allocation_temp) = facts::binding_by_local_path(
            caller.bindings,
            caller.id,
            &call_alloc.temp_name,
            &allocation_path,
        ) else {
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
                caller.bindings,
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
