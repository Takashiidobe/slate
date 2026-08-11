use crate::backend::idents::expr_ident;
use crate::backend::rust_ast::{Expr, IndentStmt, Stmt};
use crate::backend::trace::Pass;

use super::super::item::StatementMatch;
use super::super::{
    EditSet, Field, ItemCaseContext, Local, QueryRule, Rejection, StatementSequence, Usage, Value,
};

pub(in crate::backend) fn rewrite() -> QueryRule<StatementSequence<3>> {
    QueryRule::new(
        Pass::SliceSwap,
        "rewrite_slice_index_swap",
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
    .case("indexed_slice", swap_case)
}

fn index_var(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Cast { expr, .. } => index_var(expr),
        _ => expr_ident(expr),
    }
}

fn slice_index(expr: &Expr) -> Option<(&str, &str, &Expr)> {
    let Expr::Index { base, index } = expr else {
        return None;
    };
    Some((expr_ident(base)?, index_var(index)?, index))
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
    let (slice_name, i_name, i_expr) = slice_index(tmp_init).ok_or_else(|| case.reject())?;

    let Stmt::Assign {
        target: a_target,
        value: b_value,
    } = &first.stmt
    else {
        return Err(case.reject());
    };
    let (a_slice, a_index, _) = slice_index(a_target).ok_or_else(|| case.reject())?;
    case.require(a_slice == slice_name && a_index == i_name)?;
    let (b_slice, j_name, j_expr) = slice_index(b_value).ok_or_else(|| case.reject())?;
    case.require(b_slice == slice_name)?;
    case.require(i_name != j_name)?;

    let Stmt::Assign {
        target: b_target,
        value: tmp_value,
    } = &second.stmt
    else {
        return Err(case.reject());
    };
    let (c_slice, c_index, _) = slice_index(b_target).ok_or_else(|| case.reject())?;
    case.require(c_slice == slice_name && c_index == j_name)?;
    case.require(expr_ident(tmp_value) == Some(tmp_name.as_str()))?;

    let swap = Expr::MethodCall {
        recv: Box::new(Expr::Var(slice_name.into())),
        method: "swap".into(),
        args: vec![i_expr.clone(), j_expr.clone()],
    };
    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![IndentStmt {
            depth: tmp_stmt.depth,
            stmt: Stmt::Expr(swap),
        }],
    ))
}
