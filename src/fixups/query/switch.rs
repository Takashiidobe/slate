use crate::fixups::facts::goto::{LoweredSwitchParts, lowered_switch_flat_parts};
use crate::fixups::idents;
use crate::rust_ast::{Expr, IndentStmt, MatchArm, Pattern, RustValue, Stmt};

use super::views::{SwitchCase, SwitchDispatch};

pub(super) fn flat_dispatch(body: &[IndentStmt]) -> Option<SwitchDispatch> {
    build(lowered_switch_flat_parts(body, 0)?)
}

fn build(parts: LoweredSwitchParts<'_>) -> Option<SwitchDispatch> {
    let (_selector_name, selector_expr, case_name, selector_arms, switch_label, case_arms, _) =
        parts;

    let mut patterns_by_index: std::collections::BTreeMap<i64, Vec<Pattern>> =
        std::collections::BTreeMap::new();
    let mut default_index: Option<i64> = None;
    for arm in selector_arms {
        let index = arm_case_index(&arm.value)?;
        match &arm.pattern {
            Pattern::Wildcard => default_index = Some(index),
            Pattern::I64(_)
            | Pattern::I128(_)
            | Pattern::U128(_)
            | Pattern::InclusiveRange { .. } => {
                patterns_by_index
                    .entry(index)
                    .or_default()
                    .push(arm.pattern.clone());
            }
            _ => return None,
        }
    }
    let default_index = default_index.filter(|index| *index >= 0);

    let mut cases = Vec::new();
    for arm in case_arms {
        let index = match &arm.pattern {
            Pattern::I64(v) => *v,
            Pattern::Wildcard => continue,
            _ => return None,
        };
        let is_default = default_index == Some(index);
        let patterns = patterns_by_index.get(&index).cloned().unwrap_or_default();
        if !is_default && patterns.is_empty() {
            return None;
        }
        let (body, falls_through) = classify_case_body(&arm.body, case_name, switch_label);
        if body
            .iter()
            .any(|stmt| idents::stmt_ident_count(&stmt.stmt, switch_label) > 0)
        {
            return None;
        }
        cases.push(SwitchCase {
            patterns,
            is_default,
            body,
            falls_through,
        });
    }
    if cases.is_empty() {
        return None;
    }

    Some(SwitchDispatch {
        selector: selector_expr.clone(),
        cases,
    })
}

pub(super) fn is_eligible(dispatch: &SwitchDispatch) -> bool {
    dispatch.cases.iter().all(|case| !case.falls_through)
        && dispatch
            .cases
            .iter()
            .all(|case| case.is_default || case.patterns.len() == 1)
}

pub(super) fn build_match(dispatch: &SwitchDispatch) -> Stmt {
    Stmt::Match {
        expr: dispatch.selector.clone(),
        arms: build_arms(&dispatch.cases),
    }
}

fn build_arms(cases: &[SwitchCase]) -> Vec<MatchArm> {
    let mut arms = Vec::new();
    let mut default_arm = None;
    for case in cases {
        let arm = MatchArm {
            pattern: case_pattern(case),
            body: case.body.clone(),
        };
        if case.is_default {
            default_arm = Some(arm);
        } else {
            arms.push(arm);
        }
    }
    arms.push(default_arm.unwrap_or(MatchArm {
        pattern: Pattern::Wildcard,
        body: Vec::new(),
    }));
    arms
}

fn case_pattern(case: &SwitchCase) -> Pattern {
    if case.is_default {
        Pattern::Wildcard
    } else {
        case.patterns[0].clone()
    }
}

fn arm_case_index(expr: &Expr) -> Option<i64> {
    match expr {
        Expr::Value(RustValue::I64(v)) => Some(*v),
        _ => None,
    }
}

fn classify_case_body(
    body: &[IndentStmt],
    case_var: &str,
    switch_label: &str,
) -> (Vec<IndentStmt>, bool) {
    if let [
        rest @ ..,
        IndentStmt {
            stmt: Stmt::Assign { target, .. },
            ..
        },
        IndentStmt {
            stmt: Stmt::Continue(Some(label)),
            ..
        },
    ] = body
        && idents::expr_ident(target) == Some(case_var)
        && label.as_str() == switch_label
    {
        return (rest.to_vec(), true);
    }
    if let [
        rest @ ..,
        IndentStmt {
            stmt: Stmt::Break(Some(label)),
            ..
        },
    ] = body
        && label.as_str() == switch_label
    {
        return (rest.to_vec(), false);
    }
    (body.to_vec(), false)
}
