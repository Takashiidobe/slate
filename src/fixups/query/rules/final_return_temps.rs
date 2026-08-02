use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, IndentStmt, Stmt};

use super::super::{
    BindingAccess, EditSet, Field, Local, QueryRule, StatementSequence, Usage, Value,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<StatementSequence<2>> {
    QueryRule::new(
        Pass::FinalReturnTemps,
        "inline_final_return_temp",
        StatementSequence::new().starting_with(Local {
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
        let [temp_stmt, return_stmt] = case.statements(matched)?;
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
        let statement = matched.statement(0);
        let binding = case.fact(|query| query.statement_binding(&statement))?;
        case.require(binding.name == *name)?;
        let uses = case.fact(|query| query.binding_uses(&binding))?;
        let [usage] = uses.uses.as_slice() else {
            return Err(case.reject());
        };
        let returned = case.fact(|query| {
            let statement = matched.statement(1);
            query.statement_expression(&statement, 0)
        })?;
        case.require(
            usage.access == BindingAccess::Read
                && usage
                    .expression()
                    .is_some_and(|expression| expression.site == returned.site),
        )?;
        Ok(EditSet::replace_statements(
            matched.target().clone(),
            vec![IndentStmt {
                depth: return_stmt.depth,
                stmt: Stmt::Return(Some(init.clone())),
            }],
        ))
    })
}
