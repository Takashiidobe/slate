//! Collapse a fallthrough-free `__switch_value`/`__switch_case` dispatch loop
//! (see `src/fixups/facts/switch.rs`) into a direct Rust `match` over the
//! original selector expression. The dispatch loop is a correctness fallback
//! for fallthrough, which a plain `match` arm cannot express; when no case
//! falls through, the loop and its synthetic case index are unnecessary.

use crate::fixups::Fixup;
use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, PathSegment, SwitchCaseFact, SwitchDispatchFact,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, function_path_location, path_fact,
    stmt_snippet, stmts_snippet,
};
use crate::rust_ast::{IndentStmt, MatchArm, Pattern, Stmt};

pub(in crate::fixups) struct Switch<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for Switch<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        collapse_dispatches(
            body,
            self.function,
            self.facts,
            &mut Vec::new(),
            self.logger,
        )
    }
}

impl<'a> Switch<'a> {
    pub(in crate::fixups) fn new(
        function: FunctionId,
        facts: &'a FixupFacts,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            function,
            facts,
            logger,
        }
    }
}

fn collapse_dispatches(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    logger: &mut dyn TraceLogger,
) -> bool {
    let mut changed = false;
    for (index, stmt) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut stmt.stmt, path, &mut |nested, path| {
                changed |= collapse_dispatches(nested, function, facts, path, logger);
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
        changed = true;
    }
    changed
}

fn is_eligible(dispatch: &SwitchDispatchFact) -> bool {
    dispatch.fallthrough_free
        && dispatch
            .cases
            .iter()
            .all(|case| case.is_default || case.patterns.len() == 1)
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
        case.patterns[0].clone()
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
