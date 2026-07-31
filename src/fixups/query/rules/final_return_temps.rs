use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, IndentStmt, Stmt};

use super::super::StmtWindowRule;

pub(in crate::fixups) fn rewrite() -> StmtWindowRule {
    StmtWindowRule::new(Pass::FinalReturnTemps, "inline_final_return_temp", 2).case(
        "temp_return",
        |case| {
            let [temp_stmt, return_stmt] = case.stmts();
            let Stmt::Let {
                name,
                mutable: false,
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
            case.sole_use(name)?;
            Ok(vec![IndentStmt {
                depth: return_stmt.depth,
                stmt: Stmt::Return(Some(init.clone())),
            }])
        },
    )
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}
