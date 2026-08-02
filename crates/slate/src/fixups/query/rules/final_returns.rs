use crate::fixups::facts::{AstPath, PathSegment};
use crate::fixups::trace::Pass;
use crate::rust_ast::{Stmt, Type};

use super::super::{EditSet, Field, Function, FunctionRef, ItemCaseContext, QueryRule, Rejection};

pub(in crate::fixups) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(
        Pass::FinalReturns,
        "rewrite_final_return",
        Function {
            returns: Field::predicate(|returns: &Option<Type>, _| returns.is_some()),
            ..Default::default()
        },
    )
    .case("tail_return", tail_return_case)
}

fn tail_return_case(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef,
) -> Result<EditSet, Rejection> {
    let definition = case.fact(|query| query.function_snapshot(function))?;
    let Some(last_index) = definition.body.len().checked_sub(1) else {
        return Err(case.reject());
    };
    let Stmt::Return(Some(expr)) = &definition.body[last_index].stmt else {
        return Err(case.reject());
    };
    let mut edits = EditSet::new();
    edits.push_replace_statement(
        function.item_index,
        AstPath(vec![PathSegment::Stmt(last_index)]),
        Some(Stmt::Expr(expr.clone())),
    );
    Ok(edits)
}
