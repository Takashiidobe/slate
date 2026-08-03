use crate::fixups::trace::Pass;
use crate::rust_ast::IndentStmt;

use super::super::{FileOwnership, Function, ItemCaseContext, QueryRule, rewrite_file_ownership};

pub(in crate::fixups) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(
        Pass::Stdio,
        "rewrite_stdio_file_ownership",
        Function::default(),
    )
    .case("file_ownership", |case, function| {
        let facts = case.fact(|query| query.file_ownership_facts(function))?;
        case.require(!facts.owners.is_empty())?;
        let body = case
            .fact(|query| query.function_snapshot(function))?
            .body
            .clone();
        let plans = facts
            .owners
            .iter()
            .filter_map(|owner| owner_plan(case, &body, owner))
            .collect::<Vec<_>>();
        case.require(!plans.is_empty())?;
        case.replace_function_body(function.clone(), rewrite_file_ownership(body, plans))
    })
}

fn owner_plan(
    case: &mut ItemCaseContext<'_, '_>,
    body: &[IndentStmt],
    owner: &FileOwnership,
) -> Option<super::super::stdio::Plan> {
    for statement in [
        &owner.handle_statement,
        &owner.open_statement,
        &owner.assign_statement,
    ] {
        case.fact(|query| query.statement(statement)).ok()?;
    }
    case.fact(|query| query.binding_uses(&owner.handle)).ok()?;
    for use_ in &owner.uses {
        case.fact(|query| query.statement(&use_.statement)).ok()?;
    }
    super::super::stdio::plan_for_owner(body, owner)
}
