use crate::fixups::facts::{AstPath, FixupFacts, FunctionId, PathSegment};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, function_path_location, path_fact, stmt_snippet,
};
use crate::rust_ast::{
    Block, Expr, ExprMatchArm, IndentStmt, Pattern, Prim, RustValue, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    AtomicCompareExchange::new(&mut logger).fixup(body, function, facts)
}

pub(in crate::fixups) struct AtomicCompareExchange<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> AtomicCompareExchange<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(
        &mut self,
        body: &mut Vec<IndentStmt>,
        function: FunctionId,
        facts: &FixupFacts,
    ) -> bool {
        fixup_at(body, function, facts, &mut Vec::new(), self.logger)
    }
}

fn fixup_at(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    logger: &mut dyn TraceLogger,
) -> bool {
    for index in 0..body.len() {
        let mut changed = false;
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut body[index].stmt, path, &mut |body, path| {
                if !changed {
                    changed = fixup_at(body, function, facts, path, logger);
                }
            });
        });
        if changed {
            return true;
        }
    }

    for index in 0..body.len().saturating_sub(5) {
        let Some(rewrite) = compare_exchange_chain(body, index, function, facts, path) else {
            continue;
        };
        let before = logger.is_enabled().then(|| {
            body[index..index + 6]
                .iter()
                .map(|indent| indent.stmt.clone())
                .collect::<Vec<_>>()
        });
        body[index + 5].stmt = rewrite;
        body.drain(index..index + 5);
        if let Some(before) = before {
            let stmt_path = stmt_path(path, index);
            logger.rewrite(RewriteEvent {
                pass: TracePass::AtomicCompareExchange,
                kind: "collapse_compare_exchange_chain".into(),
                location: function_path_location(facts, function, &stmt_path),
                before: before
                    .iter()
                    .enumerate()
                    .map(|(i, stmt)| stmt_snippet(format!("stmt[{i}]"), stmt))
                    .collect(),
                after: vec![stmt_snippet("stmt", &body[index].stmt)],
                facts: vec![path_fact("stmt_path", &stmt_path)],
            });
        }
        return true;
    }
    false
}

fn compare_exchange_chain(
    body: &[IndentStmt],
    index: usize,
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
) -> Option<Stmt> {
    let Stmt::Let {
        name: result_name,
        mutable: false,
        init: Some(compare_exchange),
        ..
    } = &body[index].stmt
    else {
        return None;
    };
    if !is_temp_name(result_name) || !matches!(compare_exchange, Expr::AtomicCompareExchange { .. })
    {
        return None;
    }
    let Expr::AtomicCompareExchange { expected, .. } = compare_exchange else {
        unreachable!();
    };
    let Expr::Var(expected_name) = &**expected else {
        return None;
    };

    let value_name = match_value_temp(&body[index + 1].stmt, result_name)?;
    let ok_name = is_ok_temp(&body[index + 2].stmt, result_name)?;
    let not_name = not_temp(&body[index + 3].stmt, ok_name)?;
    failure_write(
        &body[index + 4].stmt,
        not_name,
        expected_name.as_str(),
        value_name,
    )?;
    let (final_name, mutable, ty, needs_cast) = final_bool_binding(&body[index + 5].stmt, ok_name)?;

    if !temp_uses_are_only(
        facts,
        function,
        result_name,
        body_path,
        index,
        &[
            stmt_path(body_path, index + 1),
            stmt_path(body_path, index + 2),
        ],
    ) || !temp_uses_are_only(
        facts,
        function,
        value_name,
        body_path,
        index + 1,
        &[stmt_path(body_path, index + 4)],
    ) || !temp_uses_are_only(
        facts,
        function,
        ok_name,
        body_path,
        index + 2,
        &[
            stmt_path(body_path, index + 3),
            stmt_path(body_path, index + 5),
        ],
    ) || !temp_uses_are_only(
        facts,
        function,
        not_name,
        body_path,
        index + 3,
        &[stmt_path(body_path, index + 4)],
    ) {
        return None;
    }

    let mut init = dense_match(compare_exchange.clone(), expected_name.as_str());
    if needs_cast {
        init = Expr::Cast {
            expr: Box::new(init),
            ty: Type::Prim(Prim::I32),
        };
    }
    Some(Stmt::Let {
        name: final_name.to_string(),
        mutable,
        ty: ty.cloned(),
        init: Some(init),
    })
}

fn match_value_temp<'a>(stmt: &'a Stmt, result_name: &str) -> Option<&'a str> {
    let Stmt::Let {
        name,
        mutable: false,
        init: Some(Expr::Match { expr, arms }),
        ..
    } = stmt
    else {
        return None;
    };
    if !is_temp_name(name) || !matches!(&**expr, Expr::Var(var) if var.as_str() == result_name) {
        return None;
    }
    let [ok, err] = arms.as_slice() else {
        return None;
    };
    if !matches!(
        &ok.pattern,
        Pattern::TupleStruct { name, fields }
            if name.as_str() == "Ok" && matches!(fields.as_slice(), [Pattern::Binding(v)] if expr_is_var(&ok.value, v.as_str()))
    ) || !matches!(
        &err.pattern,
        Pattern::TupleStruct { name, fields }
            if name.as_str() == "Err" && matches!(fields.as_slice(), [Pattern::Binding(v)] if expr_is_var(&err.value, v.as_str()))
    ) {
        return None;
    }
    Some(name)
}

fn is_ok_temp<'a>(stmt: &'a Stmt, result_name: &str) -> Option<&'a str> {
    let Stmt::Let {
        name,
        mutable: false,
        init: Some(Expr::MethodCall { recv, method, args }),
        ..
    } = stmt
    else {
        return None;
    };
    if is_temp_name(name)
        && method == "is_ok"
        && args.is_empty()
        && matches!(&**recv, Expr::Var(var) if var.as_str() == result_name)
    {
        Some(name)
    } else {
        None
    }
}

fn not_temp<'a>(stmt: &'a Stmt, ok_name: &str) -> Option<&'a str> {
    let Stmt::Let {
        name,
        mutable: false,
        init: Some(Expr::Unary {
            op: UnaryOp::Not,
            expr,
        }),
        ..
    } = stmt
    else {
        return None;
    };
    if is_temp_name(name) && matches!(&**expr, Expr::Var(var) if var.as_str() == ok_name) {
        Some(name)
    } else {
        None
    }
}

fn failure_write(stmt: &Stmt, not_name: &str, expected_name: &str, value_name: &str) -> Option<()> {
    let Stmt::If {
        cond: Expr::Var(cond),
        then_body,
        else_body,
    } = stmt
    else {
        return None;
    };
    if cond.as_str() != not_name || !else_body.is_empty() {
        return None;
    }
    let [
        IndentStmt {
            stmt: Stmt::Assign { target, value },
            ..
        },
    ] = then_body.as_slice()
    else {
        return None;
    };
    if matches!(target, Expr::Var(var) if var.as_str() == expected_name)
        && matches!(value, Expr::Var(var) if var.as_str() == value_name)
    {
        Some(())
    } else {
        None
    }
}

fn final_bool_binding<'a>(
    stmt: &'a Stmt,
    ok_name: &str,
) -> Option<(&'a str, bool, Option<&'a Type>, bool)> {
    let Stmt::Let {
        name,
        mutable,
        ty,
        init: Some(init),
    } = stmt
    else {
        return None;
    };
    match init {
        Expr::Cast { expr, ty } if matches!(ty, Type::Prim(Prim::I32)) => {
            if matches!(&**expr, Expr::Var(var) if var.as_str() == ok_name) {
                Some((name, *mutable, Some(ty), true))
            } else {
                None
            }
        }
        Expr::Var(var) if var.as_str() == ok_name => Some((name, *mutable, ty.as_ref(), false)),
        _ => None,
    }
}

fn dense_match(compare_exchange: Expr, expected_name: &str) -> Expr {
    Expr::Match {
        expr: Box::new(compare_exchange),
        arms: vec![
            ExprMatchArm {
                pattern: Pattern::TupleStruct {
                    name: "Ok".into(),
                    fields: vec![Pattern::Wildcard],
                },
                value: Expr::Value(RustValue::Bool(true)),
            },
            ExprMatchArm {
                pattern: Pattern::TupleStruct {
                    name: "Err".into(),
                    fields: vec![Pattern::Binding("v".into())],
                },
                value: Expr::Block(Box::new(Block {
                    stmts: vec![IndentStmt {
                        depth: 0,
                        stmt: Stmt::Assign {
                            target: Expr::Var(expected_name.into()),
                            value: Expr::Var("v".into()),
                        },
                    }],
                    tail: Some(Box::new(Expr::Value(RustValue::Bool(false)))),
                })),
            },
        ],
    }
}

fn temp_uses_are_only(
    facts: &FixupFacts,
    function: FunctionId,
    name: &str,
    body_path: &[PathSegment],
    def_index: usize,
    allowed: &[Vec<PathSegment>],
) -> bool {
    let def_path = AstPath(stmt_path(body_path, def_index));
    let Some(binding) = facts.binding_by_local_path(function, name, &def_path) else {
        return false;
    };
    facts.def_use(binding).is_some_and(|fact| {
        fact.writes.is_empty()
            && fact.reads.len() == allowed.len()
            && fact
                .reads
                .iter()
                .all(|read| allowed.iter().any(|path| read.0.starts_with(path)))
    })
}

fn expr_is_var(expr: &Expr, name: &str) -> bool {
    matches!(expr, Expr::Var(var) if var.as_str() == name)
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{AtomicOrdering, AtomicPlace, AtomicType, Item, Program};

    fn after(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        };
        loop {
            let analyzed = crate::fixups::facts::analyze(program.clone());
            let Item::Fn(f) = &mut program.items[0] else {
                unreachable!();
            };
            if !fixup(&mut f.body, FunctionId(0), &analyzed.facts) {
                break;
            }
        }
        program.emit()
    }

    fn cmpxchg(expected: &str) -> Expr {
        Expr::AtomicCompareExchange {
            ty: AtomicType::I32,
            place: AtomicPlace::Local("a".into()),
            expected: Box::new(var(expected)),
            desired: Box::new(int(42)),
            success: AtomicOrdering::SeqCst,
            failure: AtomicOrdering::SeqCst,
        }
    }

    fn extracted_value(result: &str) -> Expr {
        Expr::Match {
            expr: Box::new(var(result)),
            arms: vec![
                ExprMatchArm {
                    pattern: Pattern::TupleStruct {
                        name: "Ok".into(),
                        fields: vec![Pattern::Binding("v".into())],
                    },
                    value: var("v"),
                },
                ExprMatchArm {
                    pattern: Pattern::TupleStruct {
                        name: "Err".into(),
                        fields: vec![Pattern::Binding("v".into())],
                    },
                    value: var("v"),
                },
            ],
        }
    }

    fn success_bool(result: &str) -> Expr {
        Expr::MethodCall {
            recv: Box::new(var(result)),
            method: "is_ok".into(),
            args: vec![],
        }
    }

    fn chain(extra: Vec<Stmt>) -> Vec<Stmt> {
        let mut stmts = vec![
            temp("_v0", "Result<i32, i32>", cmpxchg("expected")),
            temp("_v1", "i32", extracted_value("_v0")),
            temp("_v2", "bool", success_bool("_v0")),
            temp(
                "_v3",
                "bool",
                Expr::Unary {
                    op: UnaryOp::Not,
                    expr: Box::new(var("_v2")),
                },
            ),
            Stmt::If {
                cond: var("_v3"),
                then_body: vec![IndentStmt {
                    depth: 2,
                    stmt: assign("expected", var("_v1")),
                }],
                else_body: vec![],
            },
            Stmt::Let {
                name: "ok".into(),
                mutable: false,
                ty: Some(Type::Prim(Prim::I32)),
                init: Some(Expr::Cast {
                    expr: Box::new(var("_v2")),
                    ty: Type::Prim(Prim::I32),
                }),
            },
        ];
        stmts.extend(extra);
        stmts
    }

    #[test]
    fn rewrites_compare_exchange_temp_chain_to_match() {
        let out = after(chain(vec![]));

        assert!(out.contains("let ok: i32 = match a.compare_exchange(expected, 42, std::sync::atomic::Ordering::SeqCst, std::sync::atomic::Ordering::SeqCst)"));
        assert!(out.contains("Ok(_) => true"));
        assert!(out.contains("Err(v) =>"));
        assert!(out.contains("expected = v;"));
        assert!(!out.contains("let _v0"));
        assert!(!out.contains(".is_ok()"));
    }

    #[test]
    fn keeps_chain_when_success_temp_has_extra_read() {
        let out = after(chain(vec![Stmt::Expr(var("_v2"))]));

        assert!(out.contains("let _v0: Result<i32, i32> ="));
        assert!(out.contains("let _v2: bool = _v0.is_ok();"));
    }
}
