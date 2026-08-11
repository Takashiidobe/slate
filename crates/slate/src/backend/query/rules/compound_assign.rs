use crate::backend::facts::Purity;
use crate::backend::idents::expr_ident;
use crate::backend::rust_ast::{BinOp, Expr, IndentStmt, Stmt};
use crate::backend::trace::Pass;

use super::super::item::StatementMatch;
use super::super::{
    AssignmentValue, EditSet, ExpressionRef, ItemCaseContext, Predicate, QueryRule, Rejection,
    RejectionReason, StatementSequence,
};

pub(in crate::backend) fn direct() -> QueryRule<AssignmentValue> {
    QueryRule::new(
        Pass::CompoundAssign,
        "recover_compound_assign",
        AssignmentValue,
    )
    .case("direct", direct_case)
}

fn direct_case(
    case: &mut ItemCaseContext<'_, '_>,
    expression: &ExpressionRef,
) -> Result<EditSet, Rejection> {
    let Some(Expr::Binary { op, lhs, rhs }) = case.expr(&expression.site).cloned() else {
        return Err(case.reject_at(
            Predicate::Expression,
            &expression.site,
            RejectionReason::UnsupportedShape,
        ));
    };
    case.require(is_compound_op(op))?;
    let stmt_ref = case.fact(|query| query.enclosing_statement(expression))?;
    let assign = case.fact(|query| query.statement(&stmt_ref))?;
    let Stmt::Assign { target, .. } = &assign.stmt else {
        return Err(case.reject());
    };
    let Some(name) = expr_ident(target) else {
        return Err(case.reject());
    };
    case.require(expr_ident(&lhs) == Some(name))?;
    let target = target.clone();
    let rhs_effects = case.fact(|query| {
        query.expression_effects(&ExpressionRef {
            site: query.child(&expression.site, 1),
        })
    })?;
    case.require(rhs_effects.purity == Purity::MovablePure)?;
    let mut edits = EditSet::new();
    edits.push_replace_statement(
        stmt_ref.item_index,
        stmt_ref.path.clone(),
        Some(Stmt::CompoundAssign {
            target,
            op,
            value: *rhs,
        }),
    );
    Ok(edits)
}

pub(in crate::backend) fn temp_backed() -> QueryRule<StatementSequence<2>> {
    QueryRule::new(
        Pass::CompoundAssign,
        "recover_temp_backed_compound_assign",
        StatementSequence::new(),
    )
    .case("post_update", post_update_case)
    .case("pre_update", pre_update_case)
    .ordered_non_overlapping()
}

fn post_update_case(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<2>,
) -> Result<EditSet, Rejection> {
    let [temp_stmt, assign_stmt] = case.statements(matched)?;
    let Stmt::Let {
        name: temp_name,
        init: Some(source_expr),
        ..
    } = &temp_stmt.stmt
    else {
        return Err(case.reject());
    };
    case.require(is_temp_name(temp_name))?;
    let source = expr_ident(source_expr).ok_or_else(|| case.reject())?;
    let Stmt::Assign { target, value } = &assign_stmt.stmt else {
        return Err(case.reject());
    };
    case.require(expr_ident(target) == Some(source))?;
    let Expr::Binary { op, lhs, rhs } = value else {
        return Err(case.reject());
    };
    case.require(is_compound_op(*op) && expr_ident(lhs) == Some(temp_name.as_str()))?;
    let value_expr = case.fact(|query| query.statement_expression(&matched.statement(1), 1))?;
    let rhs_effects = case.fact(|query| {
        query.expression_effects(&ExpressionRef {
            site: query.child(&value_expr.site, 1),
        })
    })?;
    case.require(rhs_effects.purity == Purity::MovablePure)?;
    let assign_ref = matched.statement(1);
    let mut edits = EditSet::new();
    edits.push_replace_statement(
        assign_ref.item_index,
        assign_ref.path,
        Some(Stmt::CompoundAssign {
            target: target.clone(),
            op: *op,
            value: (**rhs).clone(),
        }),
    );
    Ok(edits)
}

fn pre_update_case(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<2>,
) -> Result<EditSet, Rejection> {
    let [let_stmt, assign_stmt] = case.statements(matched)?;
    let Stmt::Let {
        name,
        mutable,
        ty,
        init: Some(value),
    } = &let_stmt.stmt
    else {
        return Err(case.reject());
    };
    let Expr::Binary { op, lhs, rhs } = value else {
        return Err(case.reject());
    };
    case.require(is_compound_op(*op))?;
    let target_name = expr_ident(lhs).ok_or_else(|| case.reject())?;
    let Stmt::Assign {
        target,
        value: assigned,
    } = &assign_stmt.stmt
    else {
        return Err(case.reject());
    };
    case.require(expr_ident(target) == Some(target_name))?;
    case.require(matches!(assigned, Expr::Var(v) if v.as_str() == name.as_str()))?;
    let value_expr = case.fact(|query| query.statement_expression(&matched.statement(0), 0))?;
    let rhs_effects = case.fact(|query| {
        query.expression_effects(&ExpressionRef {
            site: query.child(&value_expr.site, 1),
        })
    })?;
    case.require(rhs_effects.purity == Purity::MovablePure)?;
    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![
            IndentStmt {
                depth: let_stmt.depth,
                stmt: Stmt::CompoundAssign {
                    target: target.clone(),
                    op: *op,
                    value: (**rhs).clone(),
                },
            },
            IndentStmt {
                depth: assign_stmt.depth,
                stmt: Stmt::Let {
                    name: name.clone(),
                    mutable: *mutable,
                    ty: ty.clone(),
                    init: Some(target.clone()),
                },
            },
        ],
    ))
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|byte| byte.is_ascii_digit()))
}

fn is_compound_op(op: BinOp) -> bool {
    matches!(
        op,
        BinOp::Add
            | BinOp::Sub
            | BinOp::Mul
            | BinOp::Div
            | BinOp::Rem
            | BinOp::Shl
            | BinOp::Shr
            | BinOp::BitAnd
            | BinOp::BitOr
            | BinOp::BitXor
    )
}
