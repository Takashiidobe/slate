use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, IndentStmt, Stmt};

use super::super::{Field, Local, StmtWindowRule, Usage, Value};

pub(in crate::fixups) fn rewrite() -> StmtWindowRule {
    StmtWindowRule::new(Pass::FinalReturnTemps, "inline_final_return_temp", 2)
        .matching_local(Local {
            mutable: Field::eq(false),
            value: Value {
                usage: Field::eq(Some(Usage {
                    reads: 1,
                    writes: 0,
                })),
                ..Default::default()
            },
            ..Default::default()
        })
        .case("temp_return", |case| {
            let [temp_stmt, return_stmt] = case.stmts();
            let Stmt::Let {
                name,
                init: Some(init),
                ..
            } = &temp_stmt.stmt
            else {
                return Err(case.reject());
            };
            case.require(is_temp_name(name))?;
            let Stmt::Return(Some(Expr::Var(returned))) = &return_stmt.stmt else {
                return Err(case.reject());
            };
            case.require(returned.as_str() == name)?;
            let read_path = case.read_path(name)?;
            case.require(read_path == case.stmt_path(1))?;
            Ok(vec![IndentStmt {
                depth: return_stmt.depth,
                stmt: Stmt::Return(Some(init.clone())),
            }])
        })
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}
