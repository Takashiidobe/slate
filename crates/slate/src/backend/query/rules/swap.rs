use crate::backend::idents::expr_ident;
use crate::backend::rust_ast::{Expr, Ident, IndentStmt, Path, Stmt};
use crate::backend::trace::Pass;

use super::super::item::StatementMatch;
use super::super::{
    EditSet, Field, ItemCaseContext, Local, QueryRule, Rejection, StatementSequence, Usage, Value,
};

pub(in crate::backend) fn rewrite() -> QueryRule<StatementSequence<3>> {
    QueryRule::new(
        Pass::Swap,
        "rewrite_temp_swap",
        StatementSequence::new().starting_with(Local {
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
    .case("plain_identifiers", swap_case)
}

fn swap_case(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<3>,
) -> Result<EditSet, Rejection> {
    let [tmp_stmt, first, second] = case.statements(matched)?;

    let Stmt::Let {
        name: tmp_name,
        init: Some(tmp_init),
        ..
    } = &tmp_stmt.stmt
    else {
        return Err(case.reject());
    };
    let a_name = expr_ident(tmp_init).ok_or_else(|| case.reject())?;

    let Stmt::Assign {
        target: a_target,
        value: b_value,
    } = &first.stmt
    else {
        return Err(case.reject());
    };
    case.require(expr_ident(a_target) == Some(a_name))?;
    let b_name = expr_ident(b_value).ok_or_else(|| case.reject())?;
    case.require(a_name != b_name)?;

    let Stmt::Assign {
        target: b_target,
        value: tmp_value,
    } = &second.stmt
    else {
        return Err(case.reject());
    };
    case.require(expr_ident(b_target) == Some(b_name))?;
    case.require(expr_ident(tmp_value) == Some(tmp_name.as_str()))?;

    let swap = Expr::Call {
        func: Box::new(Expr::Path(Path::new([
            Ident::from("std"),
            Ident::from("mem"),
            Ident::from("swap"),
        ]))),
        args: vec![
            Expr::Ref {
                mutable: true,
                expr: Box::new(Expr::Var(a_name.into())),
            },
            Expr::Ref {
                mutable: true,
                expr: Box::new(Expr::Var(b_name.into())),
            },
        ],
        binding: crate::function_identity::CallBinding::Generated,
    };
    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![IndentStmt {
            depth: tmp_stmt.depth,
            stmt: Stmt::Expr(swap),
        }],
    ))
}
