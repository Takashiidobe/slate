use crate::fixups::facts::PathSegment;
use crate::fixups::trace::Pass;
use crate::function_identity::Known;
use crate::rust_ast::IndentStmt;

use super::super::item::StatementMatch;
use super::super::{
    CallRecord, CallTarget, EditSet, ExprSite, ExpressionRef, FnCall, ItemCaseContext, QueryRule,
    Rejection, StatementSequence,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<StatementSequence<1>> {
    QueryRule::new(
        Pass::PtrCopy,
        "rewrite_pointer_copy",
        StatementSequence::new(),
    )
    .case("copy_plan", rewrite_case)
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
    let plan = super::super::ptr_copy::copy_plan(&stmt.stmt, &env).ok_or_else(|| case.reject())?;
    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![IndentStmt {
            depth: stmt.depth,
            stmt: plan.stmt,
        }],
    ))
}

pub(in crate::fixups) fn calls() -> QueryRule<FnCall> {
    QueryRule::new(Pass::PtrCopy, "rewrite_memcpy_call", FnCall::default())
        .case("memcpy_call", call_case)
}

fn edit_target(call: &CallRecord) -> ExprSite {
    call.trivial_unsafe_site
        .clone()
        .unwrap_or_else(|| call.site.clone())
}

fn call_case(case: &mut ItemCaseContext<'_, '_>, call: &CallRecord) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::MemCpy)))?;
    case.require(call.args.len() == 3)?;
    let dst = case
        .expr(&call.args[0])
        .cloned()
        .ok_or_else(|| case.reject())?;
    let src = case
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
    let indent_stmt = case.fact(|query| query.statement(&statement))?;
    let depth = indent_stmt.depth;
    let body = case.fact(|query| query.enclosing_statements(&statement))?;
    let env = super::super::array_env::CopyEnv::from_body(body);
    let plan = super::super::ptr_copy::memcpy_call_plan(&dst, &src, &count, &env)
        .ok_or_else(|| case.reject())?;
    Ok(EditSet::replace_statements(
        statement.range(),
        vec![IndentStmt {
            depth,
            stmt: plan.stmt,
        }],
    ))
}
