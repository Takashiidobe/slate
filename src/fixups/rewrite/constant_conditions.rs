use crate::fixups::Fixup;
use crate::fixups::facts::PathSegment;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, named_path_location, path_fact,
    stmt_snippet,
};
use crate::rust_ast::{BinOp, Expr, IndentStmt, RustValue, Stmt};

pub(in crate::fixups) struct ConstantConditions<'a> {
    function_name: String,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for ConstantConditions<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        self.fixup_at(body, &mut Vec::new())
    }
}

impl<'a> ConstantConditions<'a> {
    pub(in crate::fixups) fn new(
        function_name: impl Into<String>,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            function_name: function_name.into(),
            logger,
        }
    }

    fn fixup_at(&mut self, body: &mut Vec<IndentStmt>, path: &mut Vec<PathSegment>) -> bool {
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
            if let Some(outcome) = fold_stmt_at(body, index) {
                if let Some(before) = before {
                    let after = match outcome {
                        FoldOutcome::Removed => None,
                        FoldOutcome::Replaced => body.get(index).map(|stmt| &stmt.stmt),
                    };
                    self.log_fold(path, index, before, after);
                }
                return true;
            }
        }
        false
    }

    fn log_fold(&mut self, path: &[PathSegment], index: usize, before: Stmt, after: Option<&Stmt>) {
        let mut stmt_path = path.to_vec();
        stmt_path.push(PathSegment::Stmt(index));
        self.logger.rewrite(RewriteEvent {
            pass: TracePass::ConstantConditions,
            kind: "fold_constant_condition".into(),
            location: named_path_location(self.function_name.clone(), &stmt_path),
            before: vec![stmt_snippet("stmt", &before)],
            after: after
                .map(|stmt| vec![stmt_snippet("stmt", stmt)])
                .unwrap_or_default(),
            facts: vec![
                path_fact("stmt_path", &stmt_path),
                fact("condition", "constant"),
            ],
        });
    }
}

#[derive(Clone, Copy)]
enum FoldOutcome {
    Removed,
    Replaced,
}

fn fold_stmt_at(body: &mut Vec<IndentStmt>, index: usize) -> Option<FoldOutcome> {
    if scoped_constant_false_if(&body[index].stmt) {
        body.remove(index);
        return Some(FoldOutcome::Removed);
    }

    let Stmt::If {
        cond,
        then_body,
        else_body,
    } = &body[index].stmt
    else {
        return None;
    };
    let value = bool_value(cond)?;
    let parent_depth = body[index].depth;
    let replacement = if value { then_body } else { else_body };
    if replacement.is_empty() {
        body.remove(index);
        Some(FoldOutcome::Removed)
    } else {
        let mut replacement = replacement.clone();
        for stmt in &mut replacement {
            stmt.depth = parent_depth;
        }
        body.splice(index..=index, replacement.drain(..));
        Some(FoldOutcome::Replaced)
    }
}

fn scoped_constant_false_if(stmt: &Stmt) -> bool {
    let Stmt::Scope { body } = stmt else {
        return false;
    };
    let [
        IndentStmt {
            stmt:
                Stmt::Let {
                    name,
                    mutable: false,
                    init: Some(init),
                    ..
                },
            ..
        },
        IndentStmt {
            stmt:
                Stmt::If {
                    cond,
                    then_body: _,
                    else_body,
                },
            ..
        },
    ] = body.as_slice()
    else {
        return false;
    };
    else_body.is_empty()
        && int_value(init).is_some()
        && bool_value_with_temp(cond, name, init) == Some(false)
}

fn bool_value(expr: &Expr) -> Option<bool> {
    match expr {
        Expr::Value(RustValue::Bool(value)) => Some(*value),
        Expr::Value(RustValue::I64(value)) => Some(*value != 0),
        Expr::Value(RustValue::I128(value)) => Some(*value != 0),
        Expr::Binary { op, lhs, rhs } => compare_ints(*op, int_value(lhs)?, int_value(rhs)?),
        _ => None,
    }
}

fn bool_value_with_temp(expr: &Expr, name: &str, init: &Expr) -> Option<bool> {
    let mut expr = expr.clone();
    expr.substitute_var(name, init);
    bool_value(&expr)
}

fn compare_ints(op: BinOp, lhs: i128, rhs: i128) -> Option<bool> {
    Some(match op {
        BinOp::Eq => lhs == rhs,
        BinOp::Ne => lhs != rhs,
        BinOp::Lt => lhs < rhs,
        BinOp::Le => lhs <= rhs,
        BinOp::Gt => lhs > rhs,
        BinOp::Ge => lhs >= rhs,
        _ => return None,
    })
}

fn int_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(value)) => Some(*value as i128),
        Expr::Value(RustValue::I128(value)) => Some(*value),
        _ => None,
    }
}
