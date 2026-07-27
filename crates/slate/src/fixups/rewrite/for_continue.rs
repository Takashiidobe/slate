use crate::fixups::facts::PathSegment;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, named_path_location, path_fact,
    stmt_snippet,
};
use crate::rust_ast::{BinOp, Expr, IndentStmt, Label, Stmt, UnaryOp};

pub(in crate::fixups) fn fixup(body: &mut [IndentStmt]) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    ForContinue::new("<unknown>", &mut logger).fixup(body)
}

pub(in crate::fixups) struct ForContinue<'a> {
    function_name: String,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> ForContinue<'a> {
    pub(in crate::fixups) fn new(
        function_name: impl Into<String>,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            function_name: function_name.into(),
            logger,
        }
    }

    pub(in crate::fixups) fn fixup(&mut self, body: &mut [IndentStmt]) -> bool {
        self.fixup_at(body, &Vec::new())
    }

    fn fixup_at(&mut self, body: &mut [IndentStmt], path: &[PathSegment]) -> bool {
        for index in 0..body.len() {
            let mut stmt_path = path.to_owned();
            stmt_path.push(PathSegment::Stmt(index));
            let trace_before = self.logger.is_enabled().then(|| body[index].stmt.clone());
            let mut rewrote_continue = false;
            let mut rewrite_event = None;
            match &mut body[index].stmt {
                Stmt::Loop {
                    label,
                    body: loop_body,
                } => {
                    let mut loop_body_path = stmt_path.clone();
                    loop_body_path.push(PathSegment::LoopBody);
                    if self.fixup_at(loop_body, &loop_body_path) {
                        return true;
                    }
                    let Some(loop_label) = synthetic_label_name(label, "__loop") else {
                        continue;
                    };
                    if rewrite_continue_blocks(loop_body) {
                        rewrote_continue = true;
                        rewrite_current_loop_breaks(loop_body, &loop_label);
                        let removed_loop_label = label_ref_count(loop_body, &loop_label) == 0;
                        if removed_loop_label {
                            *label = None;
                        }
                        if let Some(before) = trace_before {
                            rewrite_event = Some((before, loop_label, removed_loop_label));
                        }
                    }
                }
                stmt => {
                    let mut changed = false;
                    walk::nested_body_vecs_mut_with_path(
                        stmt,
                        &mut stmt_path,
                        &mut |body, path| {
                            if !changed && self.fixup_at(body, path) {
                                changed = true;
                            }
                        },
                    );
                    if changed {
                        return true;
                    }
                }
            }
            if let Some((before, loop_label, removed_loop_label)) = rewrite_event {
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::ForContinue,
                    kind: "rewrite_continue_block".into(),
                    location: named_path_location(self.function_name.clone(), &stmt_path),
                    before: vec![stmt_snippet("loop", &before)],
                    after: vec![stmt_snippet("loop", &body[index].stmt)],
                    facts: vec![
                        path_fact("loop_path", &stmt_path),
                        fact("loop_label", loop_label),
                        fact("removed_loop_label", removed_loop_label.to_string()),
                    ],
                });
                return true;
            }
            if rewrote_continue {
                return true;
            }
        }
        false
    }
}

fn rewrite_continue_blocks(body: &mut [IndentStmt]) -> bool {
    for indent in body {
        let Stmt::LabeledBlock { label, body } = &mut indent.stmt else {
            continue;
        };
        if !is_synthetic_label(label, "__continue") {
            continue;
        }
        if let Some(mut replacement) = continue_block_replacement(label.as_str(), body) {
            indent.stmt = if replacement.len() == 1 {
                replacement.remove(0).stmt
            } else {
                Stmt::Scope { body: replacement }
            };
            return true;
        }
    }
    false
}

fn continue_block_replacement(label: &str, body: &[IndentStmt]) -> Option<Vec<IndentStmt>> {
    let body = unwrapped_scope_body(body);
    let guard_index = body
        .iter()
        .position(|indent| is_continue_guard(&indent.stmt, label))?;
    let Stmt::If { cond, .. } = &body[guard_index].stmt else {
        unreachable!();
    };
    let prefix = body[..guard_index].to_vec();
    let rest = body[guard_index + 1..].to_vec();
    if label_ref_count(&rest, label) != 0 {
        return None;
    }

    let mut replacement = prefix;
    if !rest.is_empty() {
        replacement.push(IndentStmt {
            depth: body[guard_index].depth,
            stmt: Stmt::If {
                cond: not_expr(cond.clone()),
                then_body: rest,
                else_body: vec![],
            },
        });
    }
    Some(replacement)
}

fn unwrapped_scope_body(body: &[IndentStmt]) -> &[IndentStmt] {
    match body {
        [
            IndentStmt {
                stmt: Stmt::Scope { body },
                ..
            },
        ] => body,
        _ => body,
    }
}

fn is_continue_guard(stmt: &Stmt, label: &str) -> bool {
    let Stmt::If {
        then_body,
        else_body,
        ..
    } = stmt
    else {
        return false;
    };
    else_body.is_empty()
        && matches!(
            then_body.as_slice(),
            [IndentStmt {
                stmt: Stmt::Break(Some(break_label)),
                ..
            }] if break_label.as_str() == label
        )
}

fn rewrite_current_loop_breaks(body: &mut [IndentStmt], label: &str) {
    for indent in body {
        rewrite_current_loop_breaks_in_stmt(&mut indent.stmt, label);
    }
}

fn rewrite_current_loop_breaks_in_stmt(stmt: &mut Stmt, label: &str) {
    match stmt {
        Stmt::Break(Some(break_label)) if break_label.as_str() == label => {
            *stmt = Stmt::Break(None);
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
            rewrite_current_loop_breaks(then_body, label);
            rewrite_current_loop_breaks(else_body, label);
        }
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            rewrite_current_loop_breaks(body, label);
        }
        Stmt::Unsafe { body } | Stmt::Block(body) | Stmt::While { body, .. } => {
            rewrite_current_loop_breaks(&mut body.stmts, label);
        }
        Stmt::Match { arms, .. } => {
            for arm in arms {
                rewrite_current_loop_breaks(&mut arm.body, label);
            }
        }
        Stmt::Let { .. }
        | Stmt::Assign { .. }
        | Stmt::CompoundAssign { .. }
        | Stmt::InlineAsm(_)
        | Stmt::Expr(_)
        | Stmt::Return(_)
        | Stmt::Break(_)
        | Stmt::Continue(_)
        | Stmt::Loop { .. }
        | Stmt::For { .. } => {}
    }
}

fn label_ref_count(body: &[IndentStmt], label: &str) -> usize {
    body.iter()
        .map(|indent| stmt_label_ref_count(&indent.stmt, label))
        .sum()
}

fn stmt_label_ref_count(stmt: &Stmt, label: &str) -> usize {
    match stmt {
        Stmt::Break(Some(break_label)) | Stmt::Continue(Some(break_label))
            if break_label.as_str() == label =>
        {
            1
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
        } => label_ref_count(then_body, label) + label_ref_count(else_body, label),
        Stmt::Loop {
            label: loop_label,
            body,
        } => {
            usize::from(
                loop_label
                    .as_ref()
                    .is_some_and(|value| value.as_str() == label),
            ) + label_ref_count(body, label)
        }
        Stmt::For { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            label_ref_count(body, label)
        }
        Stmt::Unsafe { body } | Stmt::Block(body) | Stmt::While { body, .. } => {
            label_ref_count(&body.stmts, label)
        }
        Stmt::Match { arms, .. } => arms
            .iter()
            .map(|arm| label_ref_count(&arm.body, label))
            .sum(),
        Stmt::Let { .. }
        | Stmt::Assign { .. }
        | Stmt::CompoundAssign { .. }
        | Stmt::InlineAsm(_)
        | Stmt::Expr(_)
        | Stmt::Return(_)
        | Stmt::Break(_)
        | Stmt::Continue(_) => 0,
    }
}

fn synthetic_label_name(label: &Option<Label>, prefix: &str) -> Option<String> {
    let label = label.as_ref()?;
    is_synthetic_label(label, prefix).then(|| label.as_str().to_string())
}

fn is_synthetic_label(label: &Label, prefix: &str) -> bool {
    label
        .as_str()
        .strip_prefix(prefix)
        .is_some_and(|suffix| !suffix.is_empty() && suffix.bytes().all(|b| b.is_ascii_digit()))
}

fn not_expr(expr: Expr) -> Expr {
    match expr {
        Expr::Value(crate::rust_ast::RustValue::Bool(value)) => {
            Expr::Value(crate::rust_ast::RustValue::Bool(!value))
        }
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => *expr,
        Expr::Binary { op, lhs, rhs } => match inverse_comparison(op) {
            Some(op) => Expr::Binary { op, lhs, rhs },
            None => Expr::Unary {
                op: UnaryOp::Not,
                expr: Box::new(Expr::Binary { op, lhs, rhs }),
            },
        },
        expr => Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(expr),
        },
    }
}

fn inverse_comparison(op: BinOp) -> Option<BinOp> {
    Some(match op {
        BinOp::Eq => BinOp::Ne,
        BinOp::Ne => BinOp::Eq,
        BinOp::Lt => BinOp::Ge,
        BinOp::Le => BinOp::Gt,
        BinOp::Gt => BinOp::Le,
        BinOp::Ge => BinOp::Lt,
        _ => return None,
    })
}
