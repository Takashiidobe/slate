use crate::fixups::facts::{CountedLoopIndexUse, CountedLoopStart, CountedLoopStep};
use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, IndentStmt, RustValue, Stmt};

use super::super::{EditSet, LetStmtPattern, LoopStmtPattern, QueryRule, StatementSequence};

pub(in crate::fixups) fn rewrite() -> QueryRule<StatementSequence> {
    QueryRule::new(
        Pass::RangeLoop,
        "rewrite_counted_loop_to_range",
        StatementSequence::new(2),
    )
    .case("zero_step_one", |case, matched| {
        let [index_stmt, loop_stmt] = matched.stmts();
        let Some(index_name) = LetStmtPattern::any().matches(&index_stmt.stmt, &()) else {
            return Err(case.reject());
        };
        let Some(loop_body) = LoopStmtPattern::unlabeled().matches(&loop_stmt.stmt, &()) else {
            return Err(case.reject());
        };
        case.require(loop_body.len() >= 2)?;
        let fact = case.counted_loop(&matched.statement(1))?;
        case.require(fact.start == CountedLoopStart::Zero)?;
        case.require(fact.step == CountedLoopStep::One)?;
        let pat = match fact.index_use {
            CountedLoopIndexUse::Unused => "_".to_string(),
            CountedLoopIndexUse::Other => index_name.to_string(),
            CountedLoopIndexUse::SliceIndexOnly | CountedLoopIndexUse::SliceIndexAndValue => {
                return Err(case.reject());
            }
        };
        let body = flatten_single_scope(loop_body[1..loop_body.len() - 1].to_vec());
        Ok(EditSet::replace_statements(
            matched.target().clone(),
            vec![IndentStmt {
                depth: loop_stmt.depth,
                stmt: Stmt::For {
                    pat,
                    iter: Expr::Range {
                        start: Box::new(Expr::Value(RustValue::I64(0))),
                        end: Box::new(fact.bound),
                    },
                    body,
                },
            }],
        ))
    })
}

fn flatten_single_scope(body: Vec<IndentStmt>) -> Vec<IndentStmt> {
    match body.as_slice() {
        [
            IndentStmt {
                stmt: Stmt::Scope { body },
                ..
            },
        ] => body.clone(),
        _ => body,
    }
}
