use crate::fixups::support::walk;
use crate::rust_ast::{Block, Expr, IndentStmt, Pattern, Stmt};

pub(super) fn stmt_declares_name(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { name: binding, .. } | Stmt::LetIf { name: binding, .. } => binding == name,
        Stmt::For { pat, .. } => pat == name,
        Stmt::Match { arms, .. } => arms
            .iter()
            .any(|arm| pattern_declares_name(&arm.pattern, name)),
        _ => false,
    }
}

pub(super) fn stmt_changes_name(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Assign { target, .. } | Stmt::CompoundAssign { target, .. } => {
            expr_writes_name(target, name)
        }
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_some_and(|expr| expr_mutably_borrows_name(expr, name)),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_mutably_borrows_name(cond, name)
                || body_changes_name(then_body, name)
                || expr_mutably_borrows_name(then_value, name)
                || body_changes_name(else_body, name)
                || expr_mutably_borrows_name(else_value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_mutably_borrows_name(expr, name),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_mutably_borrows_name(cond, name)
                || body_changes_name(then_body, name)
                || body_changes_name(else_body, name)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_changes_name(body, name)
        }
        Stmt::For { iter, body, .. } => {
            expr_mutably_borrows_name(iter, name) || body_changes_name(body, name)
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_changes_name(body, name)
        }
        Stmt::Match { expr, arms } => {
            expr_mutably_borrows_name(expr, name)
                || arms.iter().any(|arm| body_changes_name(&arm.body, name))
        }
        Stmt::InlineAsm(_) => true,
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}

fn body_changes_name(body: &[IndentStmt], name: &str) -> bool {
    body.iter()
        .any(|indent| stmt_changes_name(&indent.stmt, name))
}

fn block_changes_name(block: &Block, name: &str) -> bool {
    body_changes_name(&block.stmts, name)
        || block
            .tail
            .as_ref()
            .is_some_and(|tail| expr_mutably_borrows_name(tail, name))
}

fn expr_mutably_borrows_name(expr: &Expr, name: &str) -> bool {
    walk::expr_any(expr, &mut |expr| match expr {
        Expr::Ref {
            mutable: true,
            expr,
        }
        | Expr::AddrOf {
            mutable: true,
            expr,
        } => matches!(&**expr, Expr::Var(var) if var.as_str() == name),
        _ => false,
    })
}

fn expr_writes_name(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(var) => var.as_str() == name,
        Expr::Unary { expr, .. }
        | Expr::Field { base: expr, .. }
        | Expr::TupleField { base: expr, .. }
        | Expr::ArrayPtr { array: expr, .. } => expr_writes_name(expr, name),
        Expr::Index { base, .. } => expr_writes_name(base, name),
        _ => false,
    }
}

fn pattern_declares_name(pattern: &Pattern, name: &str) -> bool {
    match pattern {
        Pattern::Binding(binding) => binding.as_str() == name,
        Pattern::TupleStruct { fields, .. } => fields
            .iter()
            .any(|field| pattern_declares_name(field, name)),
        Pattern::Wildcard | Pattern::I64(_) | Pattern::I128(_) | Pattern::U128(_) => false,
        Pattern::InclusiveRange { .. } => false,
    }
}
