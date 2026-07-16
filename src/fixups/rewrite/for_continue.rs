use crate::fixups::support::walk;
use crate::rust_ast::{BinOp, Expr, IndentStmt, Label, Stmt, UnaryOp};

pub(in crate::fixups) fn fixup(body: &mut Vec<IndentStmt>) -> bool {
    fixup_at(body)
}

fn fixup_at(body: &mut Vec<IndentStmt>) -> bool {
    for indent in body {
        match &mut indent.stmt {
            Stmt::Loop { label, body } => {
                if fixup_at(body) {
                    return true;
                }
                let Some(loop_label) = synthetic_label_name(label, "__loop") else {
                    continue;
                };
                if rewrite_continue_blocks(body) {
                    rewrite_current_loop_breaks(body, &loop_label);
                    if label_ref_count(body, &loop_label) == 0 {
                        *label = None;
                    }
                    return true;
                }
            }
            stmt => {
                let mut changed = false;
                walk::nested_body_vecs_mut_with_path(stmt, &mut Vec::new(), &mut |body, _path| {
                    if !changed && fixup_at(body) {
                        changed = true;
                    }
                });
                if changed {
                    return true;
                }
            }
        }
    }
    false
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
        | Stmt::Expr(_)
        | Stmt::Return(_)
        | Stmt::Break(_)
        | Stmt::Continue(_) => 0,
    }
}

fn synthetic_label_name(label: &mut Option<Label>, prefix: &str) -> Option<String> {
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Expr, IndentStmt, Stmt};

    fn run(stmts: Vec<Stmt>) -> String {
        after_body(
            |body| {
                while fixup(body) {}
            },
            vec![],
            None,
            stmts,
        )
    }

    fn ind(stmt: Stmt) -> IndentStmt {
        IndentStmt { depth: 1, stmt }
    }

    #[test]
    fn inverts_synthetic_for_continue_block() {
        let out = run(vec![Stmt::Loop {
            label: Some(Label::new("__loop0")),
            body: vec![
                ind(Stmt::LabeledBlock {
                    label: Label::new("__continue0"),
                    body: vec![ind(Stmt::Scope {
                        body: vec![
                            ind(Stmt::If {
                                cond: bin(BinOp::Eq, var("i"), int(0)),
                                then_body: vec![ind(Stmt::Break(Some(Label::new("__continue0"))))],
                                else_body: vec![],
                            }),
                            ind(assign("total", var("i"))),
                        ],
                    })],
                }),
                ind(assign("i", bin(BinOp::Add, var("i"), int(1)))),
            ],
        }]);

        assert!(out.contains("if i != 0 {"), "{out}");
        assert!(out.contains("total = i;"), "{out}");
        assert!(!out.contains("__continue0"));
        assert!(!out.contains("__loop0"));
    }

    #[test]
    fn rewrites_direct_break_to_unlabeled_break_when_loop_label_is_removed() {
        let out = run(vec![Stmt::Loop {
            label: Some(Label::new("__loop0")),
            body: vec![ind(Stmt::LabeledBlock {
                label: Label::new("__continue0"),
                body: vec![ind(Stmt::Scope {
                    body: vec![
                        ind(Stmt::If {
                            cond: bin(BinOp::Eq, var("skip"), int(1)),
                            then_body: vec![ind(Stmt::Break(Some(Label::new("__continue0"))))],
                            else_body: vec![],
                        }),
                        ind(Stmt::If {
                            cond: bin(BinOp::Gt, var("i"), int(4)),
                            then_body: vec![ind(Stmt::Break(Some(Label::new("__loop0"))))],
                            else_body: vec![],
                        }),
                    ],
                })],
            })],
        }]);

        assert!(out.contains("if skip != 1 {"));
        assert!(out.contains("break;"));
        assert!(!out.contains("break '__loop0"));
        assert!(!out.contains("__continue0"));
    }

    #[test]
    fn leaves_multiple_continue_exits_alone() {
        let out = run(vec![Stmt::Loop {
            label: Some(Label::new("__loop0")),
            body: vec![ind(Stmt::LabeledBlock {
                label: Label::new("__continue0"),
                body: vec![ind(Stmt::Scope {
                    body: vec![
                        ind(Stmt::If {
                            cond: var("a"),
                            then_body: vec![ind(Stmt::Break(Some(Label::new("__continue0"))))],
                            else_body: vec![],
                        }),
                        ind(Stmt::If {
                            cond: var("b"),
                            then_body: vec![ind(Stmt::Break(Some(Label::new("__continue0"))))],
                            else_body: vec![],
                        }),
                        ind(Stmt::Expr(Expr::Var("work".into()))),
                    ],
                })],
            })],
        }]);

        assert!(out.contains("__continue0"));
    }
}
