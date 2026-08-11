use crate::backend::facts::effects::is_movable_pure_expr;
use crate::backend::idents::{expr_ident, expr_ident_count, stmt_ident_count};
use crate::backend::rust_ast::{IndentStmt, Stmt};
use crate::backend::support::walk;

pub(super) fn remove_unused_retval_artifacts(body: &mut Vec<IndentStmt>) -> bool {
    if retval_read_count(body) != 0 {
        return false;
    }
    let mut changed = remove_unused_retval_writes(body);
    changed |= remove_unused_retval_decl(body);
    changed
}

fn remove_unused_retval_decl(body: &mut Vec<IndentStmt>) -> bool {
    let Some(index) = body.iter().position(|indent| {
        matches!(
            &indent.stmt,
            Stmt::Let {
                name,
                mutable: true,
                ..
            } if name == "__retval"
        )
    }) else {
        return false;
    };
    let uses = body
        .iter()
        .enumerate()
        .filter(|(i, _)| *i != index)
        .map(|(_, indent)| stmt_ident_count(&indent.stmt, "__retval"))
        .sum::<usize>();
    if uses == 0 {
        body.remove(index);
        return true;
    }
    false
}

fn remove_unused_retval_writes(body: &mut Vec<IndentStmt>) -> bool {
    let mut changed = false;
    for indent in body.iter_mut() {
        let mut path = Vec::new();
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut path, &mut |body, _| {
            changed |= remove_unused_retval_writes(body);
        });
    }
    let before = body.len();
    body.retain(|indent| !is_removable_retval_write(&indent.stmt));
    changed || body.len() != before
}

fn is_removable_retval_write(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Assign { target, value } if expr_ident(target) == Some("__retval") => {
            is_movable_pure_expr(value)
        }
        _ => false,
    }
}

fn retval_read_count(body: &[IndentStmt]) -> usize {
    body.iter()
        .map(|indent| stmt_retval_read_count(&indent.stmt))
        .sum()
}

fn stmt_retval_read_count(stmt: &Stmt) -> usize {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .map_or(0, |expr| expr_ident_count(expr, "__retval")),
        Stmt::Assign { target, value } if expr_ident(target) == Some("__retval") => {
            expr_ident_count(value, "__retval")
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_ident_count(cond, "__retval")
                + retval_read_count(then_body)
                + expr_ident_count(then_value, "__retval")
                + retval_read_count(else_body)
                + expr_ident_count(else_value, "__retval")
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_ident_count(cond, "__retval")
                + retval_read_count(then_body)
                + retval_read_count(else_body)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } => retval_read_count(body),
        Stmt::For { iter, body, .. } => {
            expr_ident_count(iter, "__retval") + retval_read_count(body)
        }
        Stmt::LabeledBlock { body, .. } => retval_read_count(body),
        Stmt::Match { expr, arms } => {
            expr_ident_count(expr, "__retval")
                + arms
                    .iter()
                    .map(|arm| retval_read_count(&arm.body))
                    .sum::<usize>()
        }
        Stmt::Unsafe { body } | Stmt::Block(body) => {
            retval_read_count(&body.stmts)
                + body
                    .tail
                    .as_ref()
                    .map_or(0, |tail| expr_ident_count(tail, "__retval"))
        }
        Stmt::While { cond, body } => {
            expr_ident_count(cond, "__retval")
                + retval_read_count(&body.stmts)
                + body
                    .tail
                    .as_ref()
                    .map_or(0, |tail| expr_ident_count(tail, "__retval"))
        }
        other => stmt_ident_count(other, "__retval"),
    }
}
