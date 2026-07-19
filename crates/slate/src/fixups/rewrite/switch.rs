//! Collapse a fallthrough-free `__switch_value`/`__switch_case` dispatch loop
//! (see `src/fixups/facts/switch.rs`) into a direct Rust `match` over the
//! original selector expression. The dispatch loop is a correctness fallback
//! for fallthrough, which a plain `match` arm cannot express; when no case
//! falls through, the loop and its synthetic case index are unnecessary.

use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, PathSegment, SwitchCaseFact, SwitchDispatchFact,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, function_path_location, path_fact,
    stmt_snippet, stmts_snippet,
};
use crate::rust_ast::{IndentStmt, MatchArm, Pattern, Stmt};

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) {
    let mut logger = crate::fixups::trace::NoopLogger;
    Switch::new(&mut logger).fixup(body, function, facts);
}

pub(in crate::fixups) struct Switch<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> Switch<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(
        &mut self,
        body: &mut Vec<IndentStmt>,
        function: FunctionId,
        facts: &FixupFacts,
    ) {
        collapse_dispatches(body, function, facts, &mut Vec::new(), self.logger);
    }
}

fn collapse_dispatches(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    logger: &mut dyn TraceLogger,
) {
    for (index, stmt) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut stmt.stmt, path, &mut |nested, path| {
                collapse_dispatches(nested, function, facts, path, logger);
            });
        });
    }

    let mut dispatches: Vec<_> = facts
        .switch_dispatches
        .iter()
        .filter(|dispatch| dispatch.function == function && is_eligible(dispatch))
        .filter_map(|dispatch| Some((direct_stmt_index(path, &dispatch.path)?, dispatch)))
        .collect();
    dispatches.sort_by_key(|(index, _)| *index);
    for (index, dispatch) in dispatches.into_iter().rev() {
        if index >= body.len() {
            continue;
        }
        collapse_dispatch(body, path, index, dispatch, function, facts, logger);
    }
}

fn is_eligible(dispatch: &SwitchDispatchFact) -> bool {
    dispatch.fallthrough_free
        && dispatch
            .cases
            .iter()
            .all(|case| case.is_default || case.values.len() == 1)
}

fn collapse_dispatch(
    body: &mut Vec<IndentStmt>,
    body_path: &[PathSegment],
    index: usize,
    dispatch: &SwitchDispatchFact,
    function: FunctionId,
    facts: &FixupFacts,
    logger: &mut dyn TraceLogger,
) {
    let end = (index + dispatch.consumed).min(body.len());
    let depth = body[index].depth;
    let stmt = Stmt::Match {
        expr: dispatch.selector.clone(),
        arms: build_arms(&dispatch.cases),
    };

    let before = logger.is_enabled().then(|| body[index..end].to_vec());
    body.splice(index..end, [IndentStmt { depth, stmt }]);

    if let Some(before) = before {
        let resolved_path = stmt_path(body_path, index);
        logger.rewrite(RewriteEvent {
            pass: TracePass::Switch,
            kind: "collapse_switch_dispatch".into(),
            location: function_path_location(facts, function, &resolved_path),
            before: vec![stmts_snippet("dispatch_region", &before)],
            after: vec![stmt_snippet("match", &body[index].stmt)],
            facts: vec![
                path_fact("switch_path", &resolved_path),
                fact("switch_label", dispatch.switch_label.clone()),
                fact("case_count", dispatch.cases.len().to_string()),
            ],
        });
    }
}

fn build_arms(cases: &[SwitchCaseFact]) -> Vec<MatchArm> {
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

fn case_pattern(case: &SwitchCaseFact) -> Pattern {
    if case.is_default {
        Pattern::Wildcard
    } else {
        int_pattern(case.values[0])
    }
}

fn int_pattern(value: i128) -> Pattern {
    match i64::try_from(value) {
        Ok(v) => Pattern::I64(v),
        Err(_) => Pattern::I128(value),
    }
}

fn stmt_path(body_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = body_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

fn direct_stmt_index(body_path: &[PathSegment], path: &AstPath) -> Option<usize> {
    let rest = path.0.strip_prefix(body_path)?;
    match rest {
        [PathSegment::Stmt(index)] => Some(*index),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts::analyze;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Expr, ExprMatchArm, Item, Label, Program};

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

    fn fixed_body(switch_stmt: Stmt) -> String {
        let f = func(
            vec![param("x", "i32")],
            Some("i32"),
            vec![
                let_mut("out", "i32", int(0)),
                switch_stmt,
                Stmt::Return(Some(var("out"))),
            ],
        );
        let program = Program {
            items: vec![Item::Fn(f.clone())],
        };
        let analyzed = analyze(program);
        let Item::Fn(mut f) = analyzed.program.items.into_iter().next().unwrap() else {
            unreachable!();
        };
        fixup(&mut f.body, FunctionId(0), &analyzed.facts);
        emit(f)
    }

    #[test]
    fn collapses_fallthrough_free_switch_into_match() {
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
        let out = fixed_body(stmt);
        assert!(!out.contains("__switch_value0"));
        assert!(!out.contains("__switch_case0"));
        assert!(!out.contains("loop"));
        assert!(out.contains("match x {"));
        assert!(out.contains("1 =>"));
        assert!(out.contains("2 =>"));
        assert!(out.contains("_ =>"));
    }

    #[test]
    fn leaves_fallthrough_switch_untouched() {
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
        let out = fixed_body(stmt);
        assert!(out.contains("__switch_value0"));
        assert!(out.contains("__switch_case0"));
    }

    #[test]
    fn no_default_switch_gets_synthetic_wildcard_arm() {
        let stmt = scoped_switch(
            var("x"),
            vec![selector_arm(4, 0), selector_wildcard(-1)],
            vec![case_arm(0, vec![out_assign(4)], true), case_wildcard()],
        );
        let out = fixed_body(stmt);
        assert!(out.contains("match x {"));
        assert!(out.contains("4 =>"));
        assert!(out.contains("_ =>"));
    }
}
