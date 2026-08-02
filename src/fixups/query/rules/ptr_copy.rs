use crate::fixups::trace::Pass;
use crate::rust_ast::{IndentStmt, Stmt};

use super::super::item::StatementMatch;
use super::super::{EditSet, ItemCaseContext, QueryRule, Rejection, StatementSequence};

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
    let env = super::super::ptr_copy::CopyEnv::from_body(body);
    let plan = super::super::ptr_copy::copy_plan(&stmt.stmt, &env).ok_or_else(|| case.reject())?;
    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![IndentStmt {
            depth: stmt.depth,
            stmt: Stmt::Expr(plan.expr),
        }],
    ))
}
