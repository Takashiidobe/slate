//! Drop the binding of a synthetic temp that only holds an unused call result.
//!
//! Baseline lowering spills every call into its own `let` temp, even when the C
//! source discards the result: `let _v6 = unsafe { printf(...) };`. When such a
//! temp is never read, the binding is pure noise — the call still has to run for
//! its side effects, so the `let _v = <call>;` becomes a bare `<call>;`.
//!
//! Only temps whose initializer performs a call are touched (the side effect is
//! preserved), and only when the temp is unused in its entire lexical scope, so
//! a used call result is left materialized.

use crate::fixups::facts::{
    AstPath, EffectKind, EffectSubject, FixupFacts, FunctionId, PathSegment,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, binding_facts, function_path_location, path_fact,
    stmt_snippet,
};
use crate::rust_ast::{Block, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(body: &mut [IndentStmt], function: FunctionId, facts: &FixupFacts) {
    let mut logger = crate::fixups::trace::NoopLogger;
    DropCallResults::new(&mut logger).fixup(body, function, facts);
}

pub(in crate::fixups) struct DropCallResults<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> DropCallResults<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(
        &mut self,
        body: &mut [IndentStmt],
        function: FunctionId,
        facts: &FixupFacts,
    ) {
        self.scope(body, function, facts, &mut Vec::new());
    }

    fn scope(
        &mut self,
        stmts: &mut [IndentStmt],
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) {
        for (index, stmt) in stmts.iter_mut().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                self.recurse(&mut stmt.stmt, function, facts, path);
            });
        }
        for (i, stmt) in stmts.iter_mut().enumerate() {
            let def_path = stmt_path(path, i);
            let name = match &stmt.stmt {
                Stmt::Let {
                    name,
                    mutable: false,
                    init: Some(_),
                    ..
                } if is_temp_name(name) => name.clone(),
                _ => continue,
            };
            if !init_has_call(function, facts, &def_path) {
                continue;
            }
            let Some(binding) =
                facts.binding_by_local_path(function, &name, &AstPath(def_path.clone()))
            else {
                continue;
            };
            if facts
                .def_use(binding)
                .is_none_or(|fact| !fact.reads.is_empty())
            {
                continue;
            }
            let before = self.logger.is_enabled().then(|| stmt.stmt.clone());
            let init = match &mut stmt.stmt {
                Stmt::Let { init, .. } => init.take(),
                _ => None,
            };
            if let Some(init) = init {
                stmt.stmt = Stmt::Expr(init);
                if let Some(before) = before {
                    let mut event_facts = binding_facts(facts, binding);
                    event_facts.push(path_fact("stmt_path", &def_path));
                    self.logger.rewrite(RewriteEvent {
                        pass: TracePass::DropCallResults,
                        kind: "drop_unused_call_result_binding".into(),
                        location: function_path_location(facts, function, &def_path),
                        before: vec![stmt_snippet("binding", &before)],
                        after: vec![stmt_snippet("call", &stmt.stmt)],
                        facts: event_facts,
                    });
                }
            }
        }
    }

    fn recurse(
        &mut self,
        stmt: &mut Stmt,
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) {
        match stmt {
            Stmt::If {
                then_body,
                else_body,
                ..
            } => {
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.scope(then_body, function, facts, path);
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.scope(else_body, function, facts, path);
                });
            }
            Stmt::LetIf {
                then_body,
                else_body,
                ..
            } => {
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.scope(then_body, function, facts, path);
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.scope(else_body, function, facts, path);
                });
            }
            Stmt::Loop { body, .. } => {
                walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                    self.scope(body, function, facts, path);
                });
            }
            Stmt::Scope { body } => {
                walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                    self.scope(body, function, facts, path);
                });
            }
            Stmt::LabeledBlock { body, .. } => {
                walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                    self.scope(body, function, facts, path);
                });
            }
            Stmt::Unsafe { body } => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.scope_block(body, function, facts, path);
                });
            }
            Stmt::While { body, .. } => {
                walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                    self.scope_block(body, function, facts, path);
                });
            }
            Stmt::Block(body) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.scope_block(body, function, facts, path);
                });
            }
            Stmt::Match { arms, .. } => {
                for (index, arm) in arms.iter_mut().enumerate() {
                    walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                        self.scope(&mut arm.body, function, facts, path);
                    });
                }
            }
            _ => {}
        }
    }

    fn scope_block(
        &mut self,
        block: &mut Block,
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) {
        self.scope(&mut block.stmts, function, facts, path);
    }
}

fn init_has_call(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| {
            fact.effects.contains(&EffectKind::ReadOnlyCall)
                || fact.effects.contains(&EffectKind::UnknownCall)
                || fact.effects.contains(&EffectKind::MethodCall)
                || fact.effects.contains(&EffectKind::MacroExpansion)
        })
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

fn stmt_path(body_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = body_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}
