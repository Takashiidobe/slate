use crate::fixups::facts::PathSegment;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, named_path_location, path_fact,
    stmt_snippet,
};
use crate::rust_ast::{IndentStmt, Stmt, UnaryOp};

pub(in crate::fixups) fn fixup(body: &mut [IndentStmt]) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    SingletonScopes::new("<unknown>", &mut logger).fixup(body)
}

pub(in crate::fixups) struct SingletonScopes<'a> {
    pass: TracePass,
    function_name: String,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> SingletonScopes<'a> {
    pub(in crate::fixups) fn new(
        function_name: impl Into<String>,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self::with_pass(TracePass::SingletonScopes, function_name, logger)
    }

    pub(in crate::fixups) fn with_pass(
        pass: TracePass,
        function_name: impl Into<String>,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            pass,
            function_name: function_name.into(),
            logger,
        }
    }

    pub(in crate::fixups) fn fixup(&mut self, body: &mut [IndentStmt]) -> bool {
        self.fixup_at(body, &mut Vec::new())
    }

    fn fixup_at(&mut self, body: &mut [IndentStmt], path: &mut Vec<PathSegment>) -> bool {
        for index in 0..body.len() {
            let mut changed = false;
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                walk::nested_body_vecs_mut_with_path(
                    &mut body[index].stmt,
                    path,
                    &mut |body, path| {
                        if !changed {
                            changed = self.fixup_at(body, path);
                        }
                    },
                );
            });
            if changed {
                return true;
            }

            let before = self.logger.is_enabled().then(|| body[index].stmt.clone());
            if unwrap_while_loop_scope(&mut body[index]) {
                if let Some(before) = before {
                    let mut stmt_path = path.clone();
                    stmt_path.push(PathSegment::Stmt(index));
                    self.logger.rewrite(RewriteEvent {
                        pass: self.pass,
                        kind: "unwrap_while_loop_scope".into(),
                        location: named_path_location(self.function_name.clone(), &stmt_path),
                        before: vec![stmt_snippet("loop", &before)],
                        after: vec![stmt_snippet("loop", &body[index].stmt)],
                        facts: vec![path_fact("stmt_path", &stmt_path)],
                    });
                }
                return true;
            }

            let before = self.logger.is_enabled().then(|| body[index].stmt.clone());
            if unwrap_do_while_loop_scope(&mut body[index]) {
                if let Some(before) = before {
                    let mut stmt_path = path.clone();
                    stmt_path.push(PathSegment::Stmt(index));
                    self.logger.rewrite(RewriteEvent {
                        pass: self.pass,
                        kind: "unwrap_do_while_loop_scope".into(),
                        location: named_path_location(self.function_name.clone(), &stmt_path),
                        before: vec![stmt_snippet("loop", &before)],
                        after: vec![stmt_snippet("loop", &body[index].stmt)],
                        facts: vec![path_fact("stmt_path", &stmt_path)],
                    });
                }
                return true;
            }

            let before = self.logger.is_enabled().then(|| body[index].stmt.clone());
            if unwrap_singleton_scope(&mut body[index]) {
                if let Some(before) = before {
                    let mut stmt_path = path.clone();
                    stmt_path.push(PathSegment::Stmt(index));
                    self.logger.rewrite(RewriteEvent {
                        pass: self.pass,
                        kind: "unwrap_singleton_scope".into(),
                        location: named_path_location(self.function_name.clone(), &stmt_path),
                        before: vec![stmt_snippet("scope", &before)],
                        after: vec![stmt_snippet("statement", &body[index].stmt)],
                        facts: vec![path_fact("stmt_path", &stmt_path), fact("scope_len", "1")],
                    });
                }
                return true;
            }
        }
        false
    }
}

fn unwrap_while_loop_scope(indent: &mut IndentStmt) -> bool {
    let Stmt::Loop { body, .. } = &mut indent.stmt else {
        return false;
    };
    if body.len() < 2 || !is_negated_break_guard(&body[0].stmt) {
        return false;
    }
    if !matches!(body[1].stmt, Stmt::Scope { .. }) {
        return false;
    }

    let Stmt::Scope { body: scoped } = body.remove(1).stmt else {
        unreachable!();
    };
    body.splice(1..1, scoped);
    true
}

fn unwrap_do_while_loop_scope(indent: &mut IndentStmt) -> bool {
    let Stmt::Loop { body, .. } = &mut indent.stmt else {
        return false;
    };
    if body.len() < 2 || !is_negated_break_guard(&body[1].stmt) {
        return false;
    }
    if !matches!(body[0].stmt, Stmt::Scope { .. }) {
        return false;
    }

    let Stmt::Scope { body: scoped } = body.remove(0).stmt else {
        unreachable!();
    };
    body.splice(0..0, scoped);
    true
}

fn is_negated_break_guard(stmt: &Stmt) -> bool {
    let Stmt::If {
        cond,
        then_body,
        else_body,
    } = stmt
    else {
        return false;
    };
    matches!(
        cond,
        crate::rust_ast::Expr::Unary {
            op: UnaryOp::Not,
            ..
        }
    ) && else_body.is_empty()
        && then_body.len() == 1
        && matches!(then_body[0].stmt, Stmt::Break(None))
}

fn unwrap_singleton_scope(indent: &mut IndentStmt) -> bool {
    let Stmt::Scope { body } = &mut indent.stmt else {
        return false;
    };
    if body.len() != 1 || !is_unwrappable(&body[0].stmt) {
        return false;
    }

    let child = body.pop().expect("singleton scope body");
    indent.stmt = child.stmt;
    true
}

fn is_unwrappable(stmt: &Stmt) -> bool {
    matches!(
        stmt,
        Stmt::If { .. }
            | Stmt::Loop { .. }
            | Stmt::For { .. }
            | Stmt::Match { .. }
            | Stmt::Assign { .. }
            | Stmt::CompoundAssign { .. }
            | Stmt::Expr(_)
    )
}
