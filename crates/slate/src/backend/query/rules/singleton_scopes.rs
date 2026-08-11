use crate::backend::rust_ast::{Expr, IndentStmt, Stmt, UnaryOp};
use crate::backend::trace::Pass;

use super::super::item::StatementMatch;
use super::super::{EditSet, ItemCaseContext, QueryRule, Rejection, StatementSequence};

pub(in crate::backend) fn rewrite(pass: Pass) -> QueryRule<StatementSequence<1>> {
    QueryRule::new(pass, "unwrap_scope", StatementSequence::new())
        .case("while_loop", while_loop_case)
        .case("do_while_loop", do_while_loop_case)
        .case("singleton", singleton_case)
        .ordered_non_overlapping()
}

fn while_loop_case(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<1>,
) -> Result<EditSet, Rejection> {
    let [mut stmt] = case.statements(matched)?;
    if !unwrap_while_loop_scope(&mut stmt) {
        return Err(case.reject());
    }
    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![stmt],
    ))
}

fn do_while_loop_case(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<1>,
) -> Result<EditSet, Rejection> {
    let [mut stmt] = case.statements(matched)?;
    if !unwrap_do_while_loop_scope(&mut stmt) {
        return Err(case.reject());
    }
    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![stmt],
    ))
}

fn singleton_case(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<1>,
) -> Result<EditSet, Rejection> {
    let [mut stmt] = case.statements(matched)?;
    if !unwrap_singleton_scope(&mut stmt) {
        return Err(case.reject());
    }
    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![stmt],
    ))
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
        Expr::Unary {
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
