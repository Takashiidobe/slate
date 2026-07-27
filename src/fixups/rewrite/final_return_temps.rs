use crate::fixups::facts::{AstPath, FixupFacts, FunctionId, PathSegment};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, binding_facts, function_path_location, path_fact,
    stmt_snippet,
};
use crate::rust_ast::{Expr, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    FinalReturnTemps::new(&mut logger).fixup(body, function, facts)
}

pub(in crate::fixups) struct FinalReturnTemps<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> FinalReturnTemps<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(
        &mut self,
        body: &mut Vec<IndentStmt>,
        function: FunctionId,
        facts: &FixupFacts,
    ) -> bool {
        self.fixup_at(body, function, facts, &mut Vec::new())
    }

    fn fixup_at(
        &mut self,
        body: &mut Vec<IndentStmt>,
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) -> bool {
        for index in 0..body.len() {
            let mut changed = false;
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                walk::nested_body_vecs_mut_with_path(
                    &mut body[index].stmt,
                    path,
                    &mut |body, path| {
                        if !changed {
                            changed = self.fixup_at(body, function, facts, path);
                        }
                    },
                )
            });
            if changed {
                return true;
            }
        }

        if body.len() < 2 {
            return false;
        }

        for index in 0..body.len() - 1 {
            let Some((init, binding)) = final_return_temp(body, index, function, facts, path)
            else {
                continue;
            };
            let temp_path = stmt_path(path, index);
            let return_path = stmt_path(path, index + 1);
            let trace_before = self
                .logger
                .is_enabled()
                .then(|| (body[index].stmt.clone(), body[index + 1].stmt.clone()));
            body[index + 1].stmt = Stmt::Return(Some(init));
            if let Some((before_temp, before_return)) = trace_before {
                let after_return = body[index + 1].stmt.clone();
                let mut event_facts = binding_facts(facts, binding);
                event_facts.extend([
                    path_fact("temp_path", &temp_path),
                    path_fact("return_path", &return_path),
                ]);
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::FinalReturnTemps,
                    kind: "inline_final_return_temp".into(),
                    location: function_path_location(facts, function, &return_path),
                    before: vec![
                        stmt_snippet("temp", &before_temp),
                        stmt_snippet("return", &before_return),
                    ],
                    after: vec![stmt_snippet("return", &after_return)],
                    facts: event_facts,
                });
            }
            body.remove(index);
            return true;
        }

        false
    }
}

fn final_return_temp(
    body: &[IndentStmt],
    index: usize,
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
) -> Option<(Expr, crate::fixups::facts::BindingId)> {
    let Stmt::Let {
        name,
        mutable: false,
        init: Some(init),
        ..
    } = &body[index].stmt
    else {
        return None;
    };
    if !is_temp_name(name) {
        return None;
    }
    let Stmt::Return(Some(Expr::Var(returned))) = &body[index + 1].stmt else {
        return None;
    };
    if returned.as_str() != name {
        return None;
    }
    let def_path = AstPath(stmt_path(body_path, index));
    let return_path = AstPath(stmt_path(body_path, index + 1));
    let binding = facts.binding_by_local_path(function, name, &def_path)?;
    let uses = facts
        .def_use
        .iter()
        .find(|fact| fact.function == function && fact.binding == binding)?;
    if uses.reads != [return_path] || !uses.writes.is_empty() {
        return None;
    }

    Some((init.clone(), binding))
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
