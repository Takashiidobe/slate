use crate::backend::facts::PathSegment;
use crate::backend::rust_ast::{IndentStmt, Stmt};
use crate::backend::trace::Pass;
use crate::function_identity::Known;

use super::super::item::StatementMatch;
use super::super::{
    CallRecord, CallTarget, EditSet, ExprSite, ExpressionRef, FnCall, ItemCaseContext, QueryRule,
    Rejection, StatementSequence,
};

pub(in crate::backend) fn rewrite() -> QueryRule<StatementSequence<1>> {
    QueryRule::new(Pass::MemSet, "rewrite_mem_set", StatementSequence::new())
        .case("mem_set_plan", rewrite_case)
        .ordered_non_overlapping()
}

fn rewrite_case(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<1>,
) -> Result<EditSet, Rejection> {
    let [stmt] = case.statements(matched)?;
    let statement = matched.statement(0);
    let body = case.fact(|query| query.enclosing_statements(&statement))?;
    let env = super::super::array_env::CopyEnv::from_body(body);
    let plan =
        super::super::mem_set::mem_set_plan(&stmt.stmt, &env).ok_or_else(|| case.reject())?;
    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![IndentStmt {
            depth: stmt.depth,
            stmt: Stmt::Expr(plan.expr),
        }],
    ))
}

pub(in crate::backend) fn calls() -> QueryRule<FnCall> {
    QueryRule::new(Pass::MemSet, "rewrite_mem_set_call", FnCall::default())
        .case("mem_set_call", call_case)
}

fn edit_target(call: &CallRecord) -> ExprSite {
    call.trivial_unsafe_site
        .clone()
        .unwrap_or_else(|| call.site.clone())
}

fn call_case(case: &mut ItemCaseContext<'_, '_>, call: &CallRecord) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::MemSet)))?;
    case.require(call.args.len() == 3)?;
    let dst = case
        .expr(&call.args[0])
        .cloned()
        .ok_or_else(|| case.reject())?;
    let val = case
        .expr(&call.args[1])
        .cloned()
        .ok_or_else(|| case.reject())?;
    let count = case
        .expr(&call.args[2])
        .cloned()
        .ok_or_else(|| case.reject())?;

    let target = edit_target(call);
    let statement = case.fact(|query| {
        query.enclosing_statement(&ExpressionRef {
            site: target.clone(),
        })
    })?;
    case.require(
        target.path.0.len() == statement.path.0.len() + 1
            && target.path.0.last() == Some(&PathSegment::Expr(0)),
    )?;
    let body = case.fact(|query| query.enclosing_statements(&statement))?;
    let env = super::super::array_env::CopyEnv::from_body(body);
    let plan = super::super::mem_set::plan_from_parts(&dst, &val, &count, &env)
        .ok_or_else(|| case.reject())?;
    Ok(EditSet::replace_expression(target, plan.expr))
}
