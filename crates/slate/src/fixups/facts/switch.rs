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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts::analyze;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{ExprMatchArm, Label, MatchArm};

    fn ind(stmt: Stmt) -> IndentStmt {
        IndentStmt { depth: 1, stmt }
    }

    fn out_assign(v: i64) -> IndentStmt {
        ind(assign("out", int(v)))
    }

    fn scoped_switch(
        selector: Expr,
        selector_arms: Vec<ExprMatchArm>,
        case_arms: Vec<MatchArm>,
    ) -> Stmt {
        Stmt::Scope {
            body: vec![
                ind(Stmt::Let {
                    name: "__switch_value0".into(),
                    mutable: false,
                    ty: None,
                    init: Some(selector),
                }),
                ind(Stmt::Let {
                    name: "__switch_case0".into(),
                    mutable: true,
                    ty: None,
                    init: Some(Expr::Match {
                        expr: Box::new(var("__switch_value0")),
                        arms: selector_arms,
                    }),
                }),
                ind(Stmt::Loop {
                    label: Some(Label::new("__switch0")),
                    body: vec![ind(Stmt::Match {
                        expr: var("__switch_case0"),
                        arms: case_arms,
                    })],
                }),
            ],
        }
    }

    fn selector_arm(value: i64, index: i64) -> ExprMatchArm {
        ExprMatchArm {
            pattern: Pattern::I64(value),
            value: int(index),
        }
    }

    fn selector_wildcard(fallback: i64) -> ExprMatchArm {
        ExprMatchArm {
            pattern: Pattern::Wildcard,
            value: int(fallback),
        }
    }

    fn case_arm(index: i64, mut body: Vec<IndentStmt>, terminal_break: bool) -> MatchArm {
        if terminal_break {
            body.push(ind(Stmt::Break(Some(Label::new("__switch0")))));
        }
        MatchArm {
            pattern: Pattern::I64(index),
            body,
        }
    }

    fn case_wildcard() -> MatchArm {
        MatchArm {
            pattern: Pattern::Wildcard,
            body: vec![ind(Stmt::Break(Some(Label::new("__switch0"))))],
        }
    }

    fn analyzed_with(switch_stmt: Stmt) -> crate::fixups::facts::AnalyzedProgram {
        let f = func(
            vec![param("x", "i32")],
            Some("i32"),
            vec![
                let_mut("out", "i32", int(0)),
                switch_stmt,
                Stmt::Return(Some(var("out"))),
            ],
        );
        analyze(Program {
            items: vec![Item::Fn(f)],
        })
    }

    #[test]
    fn recognizes_break_only_switch_as_fallthrough_free() {
        let stmt = scoped_switch(
            var("x"),
            vec![selector_arm(1, 0), selector_arm(2, 1), selector_wildcard(2)],
            vec![
                case_arm(0, vec![out_assign(11)], true),
                case_arm(1, vec![out_assign(22)], true),
                case_arm(2, vec![out_assign(99)], true),
                case_wildcard(),
            ],
        );
        let analyzed = analyzed_with(stmt);
        assert_eq!(analyzed.facts.switch_dispatches.len(), 1);
        let dispatch = &analyzed.facts.switch_dispatches[0];
        assert!(dispatch.fallthrough_free);
        assert_eq!(dispatch.consumed, 1);
        assert_eq!(dispatch.cases.len(), 3);
        assert!(
            dispatch
                .cases
                .iter()
                .any(|c| c.is_default && c.values.is_empty())
        );
        for case in &dispatch.cases {
            assert!(!case.falls_through);
            assert!(!case.body.iter().any(|s| matches!(s.stmt, Stmt::Break(_))));
        }
    }

    #[test]
    fn recognizes_fallthrough_case_as_not_fallthrough_free() {
        let mut fallthrough_body = vec![out_assign(11)];
        fallthrough_body.push(ind(Stmt::Assign {
            target: var("__switch_case0"),
            value: int(1),
        }));
        fallthrough_body.push(ind(Stmt::Continue(Some(Label::new("__switch0")))));

        let stmt = scoped_switch(
            var("x"),
            vec![
                selector_arm(1, 0),
                selector_arm(2, 1),
                selector_wildcard(-1),
            ],
            vec![
                case_arm(0, fallthrough_body, false),
                case_arm(1, vec![out_assign(22)], true),
                case_wildcard(),
            ],
        );
        let analyzed = analyzed_with(stmt);
        let dispatch = &analyzed.facts.switch_dispatches[0];
        assert!(!dispatch.fallthrough_free);
        assert!(dispatch.cases[0].falls_through);
        assert!(!dispatch.cases[1].falls_through);
        assert!(dispatch.cases.iter().all(|c| !c.is_default));
    }

    #[test]
    fn default_only_switch_is_fallthrough_free() {
        let stmt = scoped_switch(
            var("x"),
            vec![selector_wildcard(0)],
            vec![case_arm(0, vec![out_assign(99)], true), case_wildcard()],
        );
        let analyzed = analyzed_with(stmt);
        let dispatch = &analyzed.facts.switch_dispatches[0];
        assert!(dispatch.fallthrough_free);
        assert_eq!(dispatch.cases.len(), 1);
        assert!(dispatch.cases[0].is_default);
        assert!(dispatch.cases[0].values.is_empty());
    }
}
