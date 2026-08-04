use std::collections::BTreeMap;

use crate::fixups::facts::NullCheckProof;
use crate::fixups::trace::Pass;
use crate::function_identity::{Known, known_call};
use crate::rust_ast::{Expr, IndentStmt, Stmt, Type};

use super::super::{
    Function, ItemCaseContext, OptionBoxComparisonPlan, OptionBoxLocalPlan,
    OptionBoxLocalPlanInput, QueryRule, Rejection, rewrite_option_box_locals,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(
        Pass::OptionBoxLocals,
        "rewrite_option_box_locals",
        Function::default(),
    )
    .case("guarded_owned_pointer", |case, function| {
        let candidates = case.fact(|query| query.option_box_local_candidates(function))?;
        let plans = candidates
            .iter()
            .filter_map(|candidate| option_box_plan(case, candidate).ok())
            .collect::<Vec<_>>();
        case.require(!plans.is_empty())?;

        let converted: BTreeMap<String, Type> = plans
            .iter()
            .map(|plan| (plan.name.clone(), plan.elem_ty.clone()))
            .collect();
        let comparisons = case.fact(|query| query.option_box_comparisons(function))?;
        let comparison_plans = comparisons
            .iter()
            .filter_map(|comparison| {
                let elem_ty = converted.get(&comparison.lhs)?;
                if converted.get(&comparison.rhs)? != elem_ty {
                    return None;
                }
                Some(OptionBoxComparisonPlan {
                    if_stmt_path: comparison.if_stmt.path.clone(),
                    lhs: comparison.lhs.clone(),
                    rhs: comparison.rhs.clone(),
                    negate: comparison.negate,
                    elem_ty: elem_ty.clone(),
                })
            })
            .collect::<Vec<_>>();

        let body = case
            .fact(|query| query.function_snapshot(function))?
            .body
            .clone();
        case.replace_function_body(
            function.clone(),
            rewrite_option_box_locals(body, plans, comparison_plans),
        )
    })
}

fn option_box_plan(
    case: &mut ItemCaseContext<'_, '_>,
    candidate: &OptionBoxLocalPlanInput,
) -> Result<OptionBoxLocalPlan, Rejection> {
    case.fact(|query| query.pointer_option_eligible(&candidate.binding))?;
    case.require(!candidate.deref_sites.is_empty())?;

    let mut guard_stmt = None;
    for deref_site in &candidate.deref_sites {
        let dominance =
            case.fact(|query| query.null_check_dominates(&candidate.binding, deref_site))?;
        case.require(dominance.proof == NullCheckProof::StructuredGuard)?;
        let Some(stmt) = dominance.guard_stmt.clone() else {
            return Err(case.reject());
        };
        match &guard_stmt {
            Some(existing) if *existing == stmt => {}
            Some(_) => return Err(case.reject()),
            None => guard_stmt = Some(stmt),
        }
    }
    let Some(guard_stmt) = guard_stmt else {
        return Err(case.reject());
    };

    let guard = case.fact(|query| query.statement(&guard_stmt))?;
    let Stmt::If { then_body, .. } = &guard.stmt else {
        return Err(case.reject());
    };
    let aliases = local_pointer_aliases(then_body);
    let ends_with_free = then_body.last().is_some_and(|indent| {
        matches!(&indent.stmt, Stmt::Expr(expr) if is_free_call(expr, &candidate.binding.name, &aliases))
    });
    case.require(ends_with_free)?;

    for assignment in &candidate.assignments {
        let _ = case.fact(|query| query.statement(&assignment.stmt))?;
        if let Some(alloc_source) = &assignment.alloc_source {
            let _ = case.fact(|query| query.statement(alloc_source))?;
        }
    }

    Ok(OptionBoxLocalPlan {
        name: candidate.binding.name.clone(),
        elem_ty: candidate.elem_ty.clone(),
        decl_path: candidate.decl_stmt.path.clone(),
        assignments: candidate
            .assignments
            .iter()
            .map(|assignment| {
                (
                    assignment.stmt.path.clone(),
                    assignment.kind,
                    assignment
                        .alloc_source
                        .as_ref()
                        .map(|stmt| stmt.path.clone()),
                )
            })
            .collect(),
        guard_path: guard_stmt.path.clone(),
    })
}

fn is_free_call(expr: &Expr, name: &str, aliases: &BTreeMap<String, String>) -> bool {
    let Expr::Call { args, .. } = peel_wrappers(expr) else {
        return false;
    };
    if known_call(peel_wrappers(expr)) != Some(Known::Free) || args.len() != 1 {
        return false;
    }
    matches!(peel_wrappers(&args[0]), Expr::Var(arg_name) if resolve_local_alias(aliases, arg_name.as_str(), 4) == name)
}

fn local_pointer_aliases(body: &[IndentStmt]) -> BTreeMap<String, String> {
    let mut aliases = BTreeMap::new();
    for indent in body {
        if let Stmt::Let {
            name,
            init: Some(init),
            ..
        } = &indent.stmt
            && let Expr::Var(target) = peel_wrappers(init)
        {
            aliases.insert(name.clone(), target.as_str().to_string());
        }
    }
    aliases
}

fn resolve_local_alias<'a>(
    aliases: &'a BTreeMap<String, String>,
    name: &'a str,
    depth: u32,
) -> &'a str {
    if depth == 0 {
        return name;
    }
    match aliases.get(name) {
        Some(next) => resolve_local_alias(aliases, next.as_str(), depth - 1),
        None => name,
    }
}

fn peel_wrappers(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_wrappers(expr),
        Expr::Unsafe(block) | Expr::Block(block) if block.stmts.is_empty() => {
            match block.tail.as_deref() {
                Some(inner) => peel_wrappers(inner),
                None => expr,
            }
        }
        _ => expr,
    }
}
