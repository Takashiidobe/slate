use crate::fixups::Fixup;
use crate::fixups::facts::{
    AstPath, CountedLoopFact, CountedLoopIndexUse, CountedLoopStart, CountedLoopStep, FixupFacts,
    FunctionId, PathSegment,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact as trace_fact, function_path_location,
    path_fact, stmt_snippet, stmts_snippet,
};
use crate::rust_ast::{Expr, IndentStmt, RustValue, Stmt};

pub(in crate::fixups) struct RangeLoop<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for RangeLoop<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        self.rewrite_body(body, &mut Vec::new())
    }
}

impl<'a> RangeLoop<'a> {
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

    fn rewrite_body(&mut self, body: &mut Vec<IndentStmt>, path: &mut Vec<PathSegment>) -> bool {
        let mut changed = false;
        for (index, indent) in body.iter_mut().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                    changed |= self.rewrite_body(body, path);
                });
            });
        }

        for index in (0..body.len().saturating_sub(1)).rev() {
            let mut loop_path = path.to_vec();
            loop_path.push(PathSegment::Stmt(index + 1));
            let Some(fact) = loop_fact(self.function, self.facts, &AstPath(loop_path.clone()))
            else {
                continue;
            };
            let Some(replacement) = replacement_for_pair(&body[index..index + 2], fact) else {
                continue;
            };
            let trace_before = self
                .logger
                .is_enabled()
                .then(|| body[index..index + 2].to_vec());
            let trace_after = self.logger.is_enabled().then(|| replacement.stmt.clone());
            body.splice(index..index + 2, [replacement]);
            if let Some(before) = trace_before {
                let after = trace_after.expect("trace after");
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::RangeLoop,
                    kind: "rewrite_counted_loop_to_range_for".into(),
                    location: function_path_location(self.facts, self.function, &loop_path),
                    before: vec![stmts_snippet("counted_loop_pair", &before)],
                    after: vec![stmt_snippet("for_loop", &after)],
                    facts: vec![
                        path_fact("loop_path", &loop_path),
                        trace_fact("index_use", format!("{:?}", fact.index_use)),
                        trace_fact("start", format!("{:?}", fact.start)),
                        trace_fact("step", format!("{:?}", fact.step)),
                    ],
                });
            }
            changed = true;
        }
        changed
    }
}

fn loop_fact<'a>(
    function: FunctionId,
    facts: &'a FixupFacts,
    loop_path: &AstPath,
) -> Option<&'a CountedLoopFact> {
    facts.counted_loops.iter().find(|fact| {
        fact.site.function == function
            && fact.site.loop_path == *loop_path
            && fact.start == CountedLoopStart::Zero
            && fact.step == CountedLoopStep::One
    })
}

fn replacement_for_pair(pair: &[IndentStmt], fact: &CountedLoopFact) -> Option<IndentStmt> {
    let Stmt::Let {
        name: index_name, ..
    } = &pair[0].stmt
    else {
        return None;
    };
    let Stmt::Loop {
        body: loop_body, ..
    } = &pair[1].stmt
    else {
        return None;
    };
    if loop_body.len() < 2 {
        return None;
    }
    let pat = match fact.index_use {
        CountedLoopIndexUse::Unused => "_".to_string(),
        CountedLoopIndexUse::Other => index_name.clone(),
        CountedLoopIndexUse::SliceIndexOnly | CountedLoopIndexUse::SliceIndexAndValue => {
            return None;
        }
    };
    let bound = fact.bound.clone();
    let body = flatten_single_scope(loop_body[1..loop_body.len() - 1].to_vec());
    Some(IndentStmt {
        depth: pair[1].depth,
        stmt: Stmt::For {
            pat,
            iter: Expr::Range {
                start: Box::new(Expr::Value(RustValue::I64(0))),
                end: Box::new(bound),
            },
            body,
        },
    })
}

fn flatten_single_scope(body: Vec<IndentStmt>) -> Vec<IndentStmt> {
    match body.as_slice() {
        [
            IndentStmt {
                stmt: Stmt::Scope { body },
                ..
            },
        ] => body.clone(),
        _ => body,
    }
}
