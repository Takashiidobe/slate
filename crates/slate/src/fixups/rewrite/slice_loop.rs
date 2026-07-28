use crate::fixups::Fixup;
use crate::fixups::facts::{
    AstPath, CountedLoopBound, CountedLoopIndexUse, CountedLoopStart, CountedLoopStep,
    CountedSliceLoopFact, FixupFacts, FunctionId, PathSegment, SliceLoopAccess,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact as trace_fact, function_path_location,
    path_fact, stmt_snippet, stmts_snippet,
};
use crate::rust_ast::{Expr, Ident, IndentStmt, RustValue, Stmt, Type, UnaryOp};

pub(in crate::fixups) struct SliceLoop<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for SliceLoop<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        self.rewrite_body(body, &mut Vec::new())
    }
}

impl<'a> SliceLoop<'a> {
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
            let Some(replacement) = replacement_for_pair(&body[index..index + 2], fact, self.facts)
            else {
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
                    pass: TracePass::SliceLoop,
                    kind: "rewrite_counted_loop_to_slice_iter".into(),
                    location: function_path_location(self.facts, self.function, &loop_path),
                    before: vec![stmts_snippet("counted_loop_pair", &before)],
                    after: vec![stmt_snippet("for_loop", &after)],
                    facts: vec![
                        path_fact("loop_path", &loop_path),
                        trace_fact("index_use", format!("{:?}", fact.index_use)),
                        trace_fact("access", format!("{:?}", fact.access)),
                        trace_fact("bound", format!("{:?}", fact.bound)),
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
) -> Option<&'a CountedSliceLoopFact> {
    facts.counted_slice_loops.iter().find(|fact| {
        fact.site.function == function
            && fact.site.loop_path == *loop_path
            && fact.start == CountedLoopStart::Zero
            && fact.bound == CountedLoopBound::SliceLen
            && fact.step == CountedLoopStep::One
            && matches!(
                fact.index_use,
                CountedLoopIndexUse::SliceIndexOnly | CountedLoopIndexUse::SliceIndexAndValue
            )
    })
}

fn replacement_for_pair(
    pair: &[IndentStmt],
    fact: &CountedSliceLoopFact,
    facts: &FixupFacts,
) -> Option<IndentStmt> {
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
    let slice_name = facts.binding_name(fact.slice)?.to_string();
    let method = match fact.access {
        SliceLoopAccess::ReadOnly => "iter",
        SliceLoopAccess::Mutable => "iter_mut",
    };
    let mut body = loop_body[1..loop_body.len() - 1].to_vec();
    let item_name = extract_materialized_item(&mut body, &slice_name, index_name.as_str())?;
    rewrite_index_uses(&mut body, &slice_name, index_name.as_str(), &item_name);

    let iter = Expr::MethodCall {
        recv: Box::new(Expr::Var(Ident::new(slice_name))),
        method: method.into(),
        args: Vec::new(),
    };

    let (pat, iter) = if fact.index_use == CountedLoopIndexUse::SliceIndexAndValue {
        let orig_ty = facts.binding_type(fact.index)?;
        if orig_ty != "usize" {
            body.insert(0, index_cast_shadow(index_name, orig_ty, &body));
        }
        (
            format!("({index_name}, {item_name})"),
            Expr::MethodCall {
                recv: Box::new(iter),
                method: "enumerate".into(),
                args: Vec::new(),
            },
        )
    } else {
        (item_name, iter)
    };

    Some(IndentStmt {
        depth: pair[1].depth,
        stmt: Stmt::For { pat, iter, body },
    })
}

fn index_cast_shadow(index_name: &str, orig_ty: &str, body: &[IndentStmt]) -> IndentStmt {
    let depth = body.first().map_or(0, |indent| indent.depth);
    let ty = Type::parse(orig_ty);
    IndentStmt {
        depth,
        stmt: Stmt::Let {
            name: index_name.to_string(),
            mutable: false,
            ty: Some(ty.clone()),
            init: Some(Expr::Cast {
                expr: Box::new(Expr::Var(Ident::new(index_name))),
                ty,
            }),
        },
    }
}

fn extract_materialized_item(
    body: &mut Vec<IndentStmt>,
    slice_name: &str,
    index_name: &str,
) -> Option<String> {
    for index in 0..body.len() {
        if let Some(name) = extract_assigned_materialized_item(body, index, slice_name, index_name)
        {
            return Some(name);
        }
        if let Stmt::Let {
            name,
            init: Some(init),
            ..
        } = &body[index].stmt
            && !is_synthetic_temp(name)
            && is_slice_index(init, slice_name, index_name)
        {
            let name = name.clone();
            body.remove(index);
            substitute_remaining(body, index, &name);
            return Some(name);
        }
        match &mut body[index].stmt {
            Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. }
            | Stmt::Loop { body, .. }
            | Stmt::For { body, .. } => {
                if let Some(name) = extract_materialized_item(body, slice_name, index_name) {
                    return Some(name);
                }
            }
            Stmt::If {
                then_body,
                else_body,
                ..
            }
            | Stmt::LetIf {
                then_body,
                else_body,
                ..
            } => {
                if let Some(name) = extract_materialized_item(then_body, slice_name, index_name) {
                    return Some(name);
                }
                if let Some(name) = extract_materialized_item(else_body, slice_name, index_name) {
                    return Some(name);
                }
            }
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                if let Some(name) =
                    extract_materialized_item(&mut body.stmts, slice_name, index_name)
                {
                    return Some(name);
                }
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    if let Some(name) =
                        extract_materialized_item(&mut arm.body, slice_name, index_name)
                    {
                        return Some(name);
                    }
                }
            }
            Stmt::Let { .. }
            | Stmt::Assign { .. }
            | Stmt::CompoundAssign { .. }
            | Stmt::InlineAsm(_)
            | Stmt::Expr(_)
            | Stmt::Return(_)
            | Stmt::Break(_)
            | Stmt::Continue(_) => {}
        }
    }
    None
}

fn extract_assigned_materialized_item(
    body: &mut Vec<IndentStmt>,
    index: usize,
    slice_name: &str,
    index_name: &str,
) -> Option<String> {
    let [
        IndentStmt {
            stmt:
                Stmt::Let {
                    name: item_name,
                    init: Some(item_init),
                    ..
                },
            ..
        },
        IndentStmt {
            stmt:
                Stmt::Let {
                    name: temp_name,
                    init: Some(temp_init),
                    ..
                },
            ..
        },
        IndentStmt {
            stmt:
                Stmt::Assign {
                    target,
                    value: assign_value,
                },
            ..
        },
    ] = body.get(index..index + 3)?
    else {
        return None;
    };
    if is_synthetic_temp(item_name)
        || !is_zero(item_init)
        || !is_synthetic_temp(temp_name)
        || !is_slice_index(temp_init, slice_name, index_name)
        || !matches!(target, Expr::Var(name) if name.as_str() == item_name)
        || !matches!(assign_value, Expr::Var(name) if name.as_str() == temp_name)
    {
        return None;
    }
    let item_name = item_name.clone();
    body.splice(index..index + 3, []);
    substitute_remaining(body, index, &item_name);
    Some(item_name)
}

fn substitute_remaining(body: &mut [IndentStmt], start: usize, name: &str) {
    let replacement = item_deref(name);
    for indent in &mut body[start..] {
        indent.stmt.substitute_var(name, &replacement);
    }
}

fn is_synthetic_temp(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|suffix| !suffix.is_empty() && suffix.chars().all(|ch| ch.is_ascii_digit()))
}

fn is_zero(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Value(RustValue::I64(0) | RustValue::Usize(0) | RustValue::I128(0))
    )
}

fn rewrite_index_uses(
    body: &mut [IndentStmt],
    slice_name: &str,
    index_name: &str,
    item_name: &str,
) {
    for indent in body {
        walk::stmt_exprs_mut_with(&mut indent.stmt, &mut |expr| {
            if is_slice_index(expr, slice_name, index_name) {
                *expr = item_deref(item_name);
                return false;
            }
            true
        });
    }
}

fn is_slice_index(expr: &Expr, slice_name: &str, index_name: &str) -> bool {
    let Expr::Index { base, index } = expr else {
        return false;
    };
    matches!(&**base, Expr::Var(name) if name.as_str() == slice_name)
        && is_index_expr(index, index_name)
}

fn is_index_expr(expr: &Expr, index_name: &str) -> bool {
    match expr {
        Expr::Var(name) => name.as_str() == index_name,
        Expr::Cast { expr, .. } => is_index_expr(expr, index_name),
        _ => false,
    }
}

fn item_deref(item_name: &str) -> Expr {
    Expr::Unary {
        op: UnaryOp::Deref,
        expr: Box::new(Expr::Var(Ident::new(item_name))),
    }
}
