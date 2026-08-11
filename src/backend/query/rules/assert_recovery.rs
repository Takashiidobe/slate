use crate::backend::rust_ast::{Expr, IndentStmt, Stmt, Type};
use crate::backend::trace::Pass;

use super::super::item::StatementMatch;
use super::super::{
    BindingRef, EditSet, Field, ItemCaseContext, Local, QueryRule, Rejection, StatementRef,
    StatementSequence, preserve_assert_result, recover_assert,
};

pub(in crate::backend) fn rewrite() -> QueryRule<StatementSequence<2>> {
    QueryRule::new(
        Pass::AssertRecovery,
        "recover_assert_from_abort_guard",
        StatementSequence::new().starting_with(Local {
            mutable: Field::eq(false),
            ..Default::default()
        }),
    )
    .case("dense_match", rewrite_guard)
    .ordered_non_overlapping()
}

struct AssertAbortGuard {
    cond: Expr,
    depth: usize,
    result_name: String,
    result_ty: Option<Type>,
    result_literal: Expr,
}

fn rewrite_guard<'db>(
    case: &mut ItemCaseContext<'_, 'db>,
    matched: &StatementMatch<2>,
) -> Result<EditSet, Rejection> {
    let [cond_stmt, guard_stmt] = case.statements(matched)?;
    let guard = assert_abort_guard(&cond_stmt, &guard_stmt).ok_or_else(|| case.reject())?;

    let statements: [StatementRef; 2] = std::array::from_fn(|offset| matched.statement(offset));
    only_used_within(case, &statements[0], &statements[1])?;

    let result_binding: BindingRef<'db> =
        case.fact(|query| query.statement_binding(&statements[1]))?;
    let result_uses = case.fact(|query| query.binding_uses(&result_binding))?;

    let mut replacement = vec![recover_assert(guard.cond, guard.depth)];
    if !result_uses.uses.is_empty() {
        replacement.push(preserve_assert_result(
            guard.result_name,
            guard.result_ty,
            guard.result_literal,
            guard.depth,
        ));
    }

    Ok(EditSet::replace_statements(
        matched.target().clone(),
        replacement,
    ))
}

fn assert_abort_guard(cond_stmt: &IndentStmt, guard_stmt: &IndentStmt) -> Option<AssertAbortGuard> {
    let Stmt::Let {
        name: cond_name,
        mutable: false,
        init: Some(cond),
        ..
    } = &cond_stmt.stmt
    else {
        return None;
    };
    if !is_temp_name(cond_name) {
        return None;
    }
    let Stmt::LetIf {
        name: result_name,
        mutable: false,
        ty: result_ty,
        cond: guard_cond,
        then_body,
        then_value,
        else_body,
        ..
    } = &guard_stmt.stmt
    else {
        return None;
    };
    if !is_temp_name(result_name) {
        return None;
    }
    let Expr::Var(guard_cond_name) = guard_cond else {
        return None;
    };
    if guard_cond_name.as_str() != cond_name.as_str() {
        return None;
    }
    let result_literal = pure_literal_arm(then_body, then_value)?;
    if !diverges_via_abort(else_body) {
        return None;
    }
    Some(AssertAbortGuard {
        cond: cond.clone(),
        depth: cond_stmt.depth,
        result_name: result_name.clone(),
        result_ty: result_ty.clone(),
        result_literal,
    })
}

fn pure_literal_arm(body: &[IndentStmt], value: &Expr) -> Option<Expr> {
    match body {
        [] => matches!(value, Expr::Value(_)).then(|| value.clone()),
        [
            IndentStmt {
                stmt:
                    Stmt::Let {
                        name,
                        mutable: false,
                        init: Some(init @ Expr::Value(_)),
                        ..
                    },
                ..
            },
        ] => matches!(value, Expr::Var(v) if v.as_str() == name.as_str()).then(|| init.clone()),
        _ => None,
    }
}

fn diverges_via_abort(body: &[IndentStmt]) -> bool {
    let Some(first) = body.first() else {
        return false;
    };
    let Stmt::Expr(Expr::Unsafe(block)) = &first.stmt else {
        return false;
    };
    if !block.stmts.is_empty() {
        return false;
    }
    let Some(tail) = &block.tail else {
        return false;
    };
    matches!(
        &**tail,
        Expr::Call { func, args, .. }
            if args.is_empty() && matches!(&**func, Expr::Var(name) if name.as_str() == "abort")
    )
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

fn only_used_within<'db>(
    case: &mut ItemCaseContext<'_, 'db>,
    definition: &StatementRef,
    only_use: &StatementRef,
) -> Result<(), Rejection> {
    let binding: BindingRef<'db> = case.fact(|query| query.statement_binding(definition))?;
    let total = case.fact(|query| query.binding_uses(&binding))?;
    let within = case.fact(|query| query.binding_uses_in_statement(&binding, only_use))?;
    case.require(within.uses.len() == total.uses.len())
}
