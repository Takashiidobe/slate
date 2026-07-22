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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, MatchArm, Pattern, Program};

    fn after(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        };
        let Item::Fn(f) = &mut program.items[0] else {
            unreachable!();
        };
        while fixup(&mut f.body) {}
        program.emit()
    }

    fn stmt(stmt: Stmt) -> IndentStmt {
        IndentStmt { depth: 0, stmt }
    }

    #[test]
    fn unwraps_singleton_if_scope() {
        let got = after(vec![Stmt::Scope {
            body: vec![stmt(Stmt::If {
                cond: bin(crate::rust_ast::BinOp::Lt, var("n"), int(10)),
                then_body: vec![stmt(assign("n", int(10)))],
                else_body: vec![],
            })],
        }]);

        assert!(got.contains("    if n < 10 {\n        n = 10;\n    }\n"));
        assert!(!got.contains("    {\n        if n < 10"));
    }

    #[test]
    fn unwraps_nested_singleton_if_scope() {
        let got = after(vec![Stmt::If {
            cond: var("ok"),
            then_body: vec![stmt(Stmt::Scope {
                body: vec![stmt(Stmt::If {
                    cond: var("inner"),
                    then_body: vec![stmt(assign("x", int(1)))],
                    else_body: vec![],
                })],
            })],
            else_body: vec![],
        }]);

        assert!(
            got.contains("    if ok {\n        if inner {\n            x = 1;\n        }\n    }\n")
        );
        assert!(!got.contains("    if ok {\n        {\n"));
    }

    #[test]
    fn unwraps_singleton_loop_and_match_scopes() {
        let got = after(vec![
            Stmt::Scope {
                body: vec![stmt(Stmt::Loop {
                    label: None,
                    body: vec![stmt(Stmt::Break(None))],
                })],
            },
            Stmt::Scope {
                body: vec![stmt(Stmt::Match {
                    expr: var("x"),
                    arms: vec![MatchArm {
                        pattern: Pattern::Wildcard,
                        body: vec![stmt(assign("x", int(1)))],
                    }],
                })],
            },
        ]);

        assert!(got.contains("    loop {\n        break;\n    }\n"));
        assert!(
            got.contains("    match x {\n        _ => {\n            x = 1;\n        }\n    }\n")
        );
        assert!(!got.contains("    {\n        loop"));
        assert!(!got.contains("    {\n        match"));
    }

    #[test]
    fn keeps_scope_with_declaration() {
        let got = after(vec![Stmt::Scope {
            body: vec![stmt(let_mut("x", "i32", int(1)))],
        }]);

        assert!(got.contains("    {\n        let mut x: i32 = 1;\n    }\n"));
    }

    #[test]
    fn unwraps_singleton_assignment_scope() {
        let got = after(vec![Stmt::For {
            pat: "item".into(),
            iter: call("items", vec![]),
            body: vec![stmt(Stmt::Scope {
                body: vec![stmt(crate::rust_ast::Stmt::CompoundAssign {
                    target: var("total"),
                    op: crate::rust_ast::BinOp::Add,
                    value: crate::rust_ast::Expr::Unary {
                        op: crate::rust_ast::UnaryOp::Deref,
                        expr: Box::new(var("item")),
                    },
                })],
            })],
        }]);

        assert!(got.contains("    for item in items() {\n        total += *item;\n    }\n"));
        assert!(!got.contains("    for item in items() {\n        {\n"));
    }

    #[test]
    fn unwraps_do_while_loop_body_scope() {
        let got = after(vec![Stmt::Loop {
            label: None,
            body: vec![
                stmt(Stmt::Scope {
                    body: vec![
                        stmt(crate::rust_ast::Stmt::CompoundAssign {
                            target: var("total"),
                            op: crate::rust_ast::BinOp::Add,
                            value: var("i"),
                        }),
                        stmt(crate::rust_ast::Stmt::CompoundAssign {
                            target: var("i"),
                            op: crate::rust_ast::BinOp::Add,
                            value: int(1),
                        }),
                    ],
                }),
                stmt(Stmt::If {
                    cond: crate::rust_ast::Expr::Unary {
                        op: crate::rust_ast::UnaryOp::Not,
                        expr: Box::new(bin(crate::rust_ast::BinOp::Le, var("i"), var("n"))),
                    },
                    then_body: vec![stmt(Stmt::Break(None))],
                    else_body: vec![],
                }),
            ],
        }]);

        assert!(got.contains(
            "    loop {\n        total += i;\n        i += 1;\n        if !(i <= n) {\n            break;\n        }\n    }\n"
        ));
        assert!(!got.contains("    loop {\n        {\n"));
    }

    #[test]
    fn unwraps_while_loop_body_scope() {
        let got = after(vec![Stmt::Loop {
            label: None,
            body: vec![
                stmt(Stmt::If {
                    cond: crate::rust_ast::Expr::Unary {
                        op: UnaryOp::Not,
                        expr: Box::new(bin(crate::rust_ast::BinOp::Le, var("i"), var("n"))),
                    },
                    then_body: vec![stmt(Stmt::Break(None))],
                    else_body: vec![],
                }),
                stmt(Stmt::Scope {
                    body: vec![
                        stmt(crate::rust_ast::Stmt::CompoundAssign {
                            target: var("total"),
                            op: crate::rust_ast::BinOp::Add,
                            value: var("i"),
                        }),
                        stmt(crate::rust_ast::Stmt::CompoundAssign {
                            target: var("i"),
                            op: crate::rust_ast::BinOp::Add,
                            value: int(1),
                        }),
                    ],
                }),
            ],
        }]);

        assert!(got.contains(
            "    loop {\n        if !(i <= n) {\n            break;\n        }\n        total += i;\n        i += 1;\n    }\n"
        ));
        assert!(!got.contains(
            "    loop {\n        if !(i <= n) {\n            break;\n        }\n        {\n"
        ));
    }

    #[test]
    fn keeps_scope_with_multiple_statements() {
        let got = after(vec![Stmt::Scope {
            body: vec![stmt(assign("x", int(1))), stmt(assign("y", int(2)))],
        }]);

        assert!(got.contains("    {\n        x = 1;\n        y = 2;\n    }\n"));
    }

    #[test]
    fn keeps_singleton_labeled_block_scope() {
        let got = after(vec![Stmt::Scope {
            body: vec![stmt(Stmt::LabeledBlock {
                label: "done".into(),
                body: vec![stmt(Stmt::Break(Some("done".into())))],
            })],
        }]);

        assert!(
            got.contains("    {\n        'done: {\n            break 'done;\n        }\n    }\n")
        );
    }
}
