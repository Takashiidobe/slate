use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, IndentStmt, Stmt};

use super::super::{EditSet, Field, Local, QueryRule, StatementSequence, Usage, Value};

pub(in crate::fixups) fn rewrite() -> QueryRule<StatementSequence> {
    QueryRule::new(
        Pass::FinalReturnTemps,
        "inline_final_return_temp",
        StatementSequence::new(2).starting_with(Local {
            mutable: Field::eq(false),
            value: Value {
                usage: Field::eq(Some(Usage {
                    reads: 1,
                    writes: 0,
                })),
                ..Default::default()
            },
            ..Default::default()
        }),
    )
    .case("temp_return", |case, matched| {
        let [temp_stmt, return_stmt] = matched.stmts();
        let Stmt::Let {
            name,
            init: Some(init),
            ..
        } = &temp_stmt.stmt
        else {
            return Err(case.reject());
        };
        let Stmt::Return(Some(Expr::Var(returned))) = &return_stmt.stmt else {
            return Err(case.reject());
        };
        case.require(returned.as_str() == name)?;
        let binding = case.local_binding(&matched.statement(0), name)?;
        let uses = case.def_use(&binding)?;
        let [read_path] = uses.reads.as_slice() else {
            return Err(case.reject());
        };
        case.require(uses.writes.is_empty() && read_path == &matched.statement(1).path)?;
        Ok(EditSet::replace_statements(
            matched.target().clone(),
            vec![IndentStmt {
                depth: return_stmt.depth,
                stmt: Stmt::Return(Some(init.clone())),
            }],
        ))
    })
}
