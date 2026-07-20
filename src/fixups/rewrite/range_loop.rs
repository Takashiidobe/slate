use crate::fixups::facts::{
    AstPath, CountedLoopFact, CountedLoopIndexUse, CountedLoopStart, CountedLoopStep, FixupFacts,
    FunctionId, PathSegment,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact as trace_fact, function_path_location,
    path_fact, stmt_snippet, stmts_snippet,
};
use crate::rust_ast::{Expr, IndentStmt, Item, Program, RustValue, Stmt};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    RangeLoop::new(&mut logger).fixup(program, facts)
}

pub(in crate::fixups) struct RangeLoop<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> RangeLoop<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) -> bool {
        let mut changed = false;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            let Item::Fn(f) = item else {
                continue;
            };
            let Some(function) = facts.function_by_item_index(item_index) else {
                continue;
            };
            changed |= self.rewrite_body(&mut f.body, function, facts, &mut Vec::new());
        }
        changed
    }

    fn rewrite_body(
        &mut self,
        body: &mut Vec<IndentStmt>,
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) -> bool {
        let mut changed = false;
        for (index, indent) in body.iter_mut().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                    changed |= self.rewrite_body(body, function, facts, path);
                });
            });
        }

        for index in (0..body.len().saturating_sub(1)).rev() {
            let mut loop_path = path.to_vec();
            loop_path.push(PathSegment::Stmt(index + 1));
            let Some(fact) = loop_fact(function, facts, &AstPath(loop_path.clone())) else {
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
                    location: function_path_location(facts, function, &loop_path),
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, FnParam, Type, UnaryOp};

    fn stmt(stmt: Stmt) -> IndentStmt {
        IndentStmt { depth: 2, stmt }
    }

    fn counted_body(body_stmt: Stmt) -> Stmt {
        counted_body_with_bound(var("n"), body_stmt)
    }

    fn counted_body_with_bound(bound: Expr, body_stmt: Stmt) -> Stmt {
        Stmt::Scope {
            body: vec![
                stmt(let_mut("i", "i32", int(0))),
                stmt(Stmt::Loop {
                    label: None,
                    body: vec![
                        stmt(Stmt::If {
                            cond: Expr::Unary {
                                op: UnaryOp::Not,
                                expr: Box::new(bin(BinOp::Lt, var("i"), bound)),
                            },
                            then_body: vec![stmt(Stmt::Break(None))],
                            else_body: Vec::new(),
                        }),
                        stmt(body_stmt),
                        stmt(Stmt::CompoundAssign {
                            target: var("i"),
                            op: BinOp::Add,
                            value: int(1),
                        }),
                    ],
                }),
            ],
        }
    }

    #[test]
    fn rewrites_unused_index_counted_loop_to_range_for() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![FnParam {
                    name: "n".into(),
                    mutable: false,
                    ty: Type::parse("i32"),
                    nonnull: false,
                }],
                Some("i32"),
                vec![
                    let_mut("total", "i32", int(0)),
                    counted_body(Stmt::CompoundAssign {
                        target: var("total"),
                        op: BinOp::Add,
                        value: int(1),
                    }),
                    Stmt::Return(Some(var("total"))),
                ],
            ))],
        };
        let facts = facts::analyze(program.clone()).facts;

        assert!(fixup(&mut program, &facts));
        let out = program.emit();

        assert!(out.contains("for _ in 0..n"));
        assert!(out.contains("total += 1;"));
        assert!(!out.contains("loop {"));
    }

    #[test]
    fn preserves_index_name_when_body_uses_it() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![param("n", "i32")],
                Some("i32"),
                vec![
                    let_mut("total", "i32", int(0)),
                    counted_body(Stmt::CompoundAssign {
                        target: var("total"),
                        op: BinOp::Add,
                        value: var("i"),
                    }),
                    Stmt::Return(Some(var("total"))),
                ],
            ))],
        };
        let facts = facts::analyze(program.clone()).facts;

        assert!(fixup(&mut program, &facts));
        let out = program.emit();

        assert!(out.contains("for i in 0..n"));
        assert!(out.contains("total += i;"));
    }

    #[test]
    fn rewrites_literal_bound_counted_loop_to_range_for() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                Vec::new(),
                Some("i32"),
                vec![
                    let_mut("total", "i32", int(0)),
                    counted_body_with_bound(
                        int(5),
                        Stmt::CompoundAssign {
                            target: var("total"),
                            op: BinOp::Add,
                            value: var("i"),
                        },
                    ),
                    Stmt::Return(Some(var("total"))),
                ],
            ))],
        };
        let facts = facts::analyze(program.clone()).facts;

        assert!(fixup(&mut program, &facts));
        let out = program.emit();

        assert!(out.contains("for i in 0..5"));
        assert!(out.contains("total += i;"));
        assert!(!out.contains("loop {"));
    }

    #[test]
    fn rewrites_reducible_integer_bound_counted_loop_to_range_for() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                Vec::new(),
                Some("i32"),
                vec![
                    let_mut("total", "i32", int(0)),
                    counted_body_with_bound(
                        bin(BinOp::Add, int(2), int(3)),
                        Stmt::CompoundAssign {
                            target: var("total"),
                            op: BinOp::Add,
                            value: var("i"),
                        },
                    ),
                    Stmt::Return(Some(var("total"))),
                ],
            ))],
        };
        let facts = facts::analyze(program.clone()).facts;

        assert!(fixup(&mut program, &facts));
        let out = program.emit();

        assert!(out.contains("for i in 0..(2 + 3)"));
        assert!(!out.contains("loop {"));
    }
}
