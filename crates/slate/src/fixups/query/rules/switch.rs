use crate::fixups::trace::Pass;
use crate::rust_ast::IndentStmt;

use super::super::item::StatementMatch;
use super::super::{
    EditSet, ItemCaseContext, QueryRule, Rejection, StatementRef, StatementSequence,
};

pub(in crate::fixups) fn flat() -> QueryRule<StatementSequence<3>> {
    QueryRule::new(
        Pass::Switch,
        "collapse_switch_dispatch",
        StatementSequence::new(),
    )
    .case("known_dispatch", rewrite_flat)
    .ordered_non_overlapping()
}

fn rewrite_flat(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<3>,
) -> Result<EditSet, Rejection> {
    let statements: [StatementRef; 3] = std::array::from_fn(|offset| matched.statement(offset));
    let (dispatch, target, depth) = case.fact(|query| query.switch_dispatch_flat(&statements))?;
    Ok(EditSet::replace_statements(
        target,
        vec![IndentStmt {
            depth,
            stmt: super::super::switch::build_match(&dispatch),
        }],
    ))
}
