use crate::fixups::facts::goto;
use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, PathSegment, SwitchCaseFact, SwitchDispatchFact,
};
use crate::fixups::idents;
use crate::rust_ast::{Expr, IndentStmt, Item, Pattern, Program, RustValue, Stmt};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.switch_dispatches.clear();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        Collector { function, facts }.body(&f.body, &mut Vec::new());
    }
}

struct Collector<'a> {
    function: FunctionId,
    facts: &'a mut FixupFacts,
}

impl<'a> Collector<'a> {
    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>) {
        let mut i = 0;
        while i < body.len() {
            if let Some(parts) = goto::lowered_switch_parts(body, i) {
                let consumed = parts.6;
                walk::with_path_segment(path, PathSegment::Stmt(i), |path| {
                    self.record(parts, AstPath(path.clone()));
                });
                i += consumed;
                continue;
            }
            walk::with_path_segment(path, PathSegment::Stmt(i), |path| {
                walk::nested_bodies_with_path(&body[i].stmt, path, &mut |nested, path| {
                    self.body(nested, path);
                });
            });
            i += 1;
        }
    }

    fn record(&mut self, parts: goto::LoweredSwitchParts<'_>, path: AstPath) {
        let (
            _selector_name,
            selector_expr,
            case_name,
            selector_arms,
            switch_label,
            case_arms,
            consumed,
        ) = parts;

        let mut values_by_index: std::collections::BTreeMap<i64, Vec<i128>> =
            std::collections::BTreeMap::new();
        let mut default_index: Option<i64> = None;
        for arm in selector_arms {
            let Some(index) = arm_case_index(&arm.value) else {
                return;
            };
            match &arm.pattern {
                Pattern::Wildcard => default_index = Some(index),
                Pattern::I64(v) => values_by_index.entry(index).or_default().push(*v as i128),
                Pattern::I128(v) => values_by_index.entry(index).or_default().push(*v),
                _ => return,
            }
        }
        let default_index = default_index.filter(|index| *index >= 0);

        let mut cases = Vec::new();
        for arm in case_arms {
            let index = match &arm.pattern {
                Pattern::I64(v) => *v,
                Pattern::Wildcard => continue,
                _ => return,
            };
            let is_default = default_index == Some(index);
            let values = values_by_index.get(&index).cloned().unwrap_or_default();
            if !is_default && values.is_empty() {
                return;
            }
            let (body, falls_through) = classify_case_body(&arm.body, case_name, switch_label);
            cases.push(SwitchCaseFact {
                values,
                is_default,
                body,
                falls_through,
            });
        }
        if cases.is_empty() {
            return;
        }

        let fallthrough_free = cases.iter().all(|case| !case.falls_through);
        self.facts.switch_dispatches.push(SwitchDispatchFact {
            function: self.function,
            path,
            selector: selector_expr.clone(),
            switch_label: switch_label.to_string(),
            consumed,
            fallthrough_free,
            cases,
        });
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
