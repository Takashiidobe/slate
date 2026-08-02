use crate::fixups::trace::Pass;
use crate::rust_ast::{BinOp, Expr, IndentStmt, RustValue, Stmt};

use super::super::item::StatementMatch;
use super::super::{EditSet, ItemCaseContext, QueryRule, Rejection, StatementSequence};

pub(in crate::fixups) fn rewrite() -> QueryRule<StatementSequence<1>> {
    QueryRule::new(
        Pass::ConstantConditions,
        "fold_constant_condition",
        StatementSequence::new(),
    )
    .case("scoped_false_if", scoped_false_if_case)
    .case("direct_if", direct_if_case)
    .ordered_non_overlapping()
}

fn direct_if_case(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<1>,
) -> Result<EditSet, Rejection> {
    let [stmt] = case.statements(matched)?;
    let Stmt::If {
        cond,
        then_body,
        else_body,
    } = &stmt.stmt
    else {
        return Err(case.reject());
    };
    let value = bool_value(cond).ok_or_else(|| case.reject())?;
    let taken = if value { then_body } else { else_body };
    let mut replacement = taken.clone();
    for indent in &mut replacement {
        indent.depth = stmt.depth;
    }
    Ok(EditSet::replace_statements(
        matched.target().clone(),
        replacement,
    ))
}

fn scoped_false_if_case(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<1>,
) -> Result<EditSet, Rejection> {
    let [stmt] = case.statements(matched)?;
    case.require(scoped_constant_false_if(&stmt.stmt))?;
    Ok(EditSet::replace_statements(
        matched.target().clone(),
        Vec::new(),
    ))
}

fn scoped_constant_false_if(stmt: &Stmt) -> bool {
    let Stmt::Scope { body } = stmt else {
        return false;
    };
    let [
        IndentStmt {
            stmt:
                Stmt::Let {
                    name,
                    mutable: false,
                    init: Some(init),
                    ..
                },
            ..
        },
        IndentStmt {
            stmt:
                Stmt::If {
                    cond,
                    then_body: _,
                    else_body,
                },
            ..
        },
    ] = body.as_slice()
    else {
        return false;
    };
    else_body.is_empty()
        && int_value(init).is_some()
        && bool_value_with_temp(cond, name, init) == Some(false)
}

fn bool_value(expr: &Expr) -> Option<bool> {
    match expr {
        Expr::Value(RustValue::Bool(value)) => Some(*value),
        Expr::Value(RustValue::I64(value)) => Some(*value != 0),
        Expr::Value(RustValue::I128(value)) => Some(*value != 0),
        Expr::Binary { op, lhs, rhs } => compare_ints(*op, int_value(lhs)?, int_value(rhs)?),
        _ => None,
    }
}

fn bool_value_with_temp(expr: &Expr, name: &str, init: &Expr) -> Option<bool> {
    let mut expr = expr.clone();
    expr.substitute_var(name, init);
    bool_value(&expr)
}

fn compare_ints(op: BinOp, lhs: i128, rhs: i128) -> Option<bool> {
    Some(match op {
        BinOp::Eq => lhs == rhs,
        BinOp::Ne => lhs != rhs,
        BinOp::Lt => lhs < rhs,
        BinOp::Le => lhs <= rhs,
        BinOp::Gt => lhs > rhs,
        BinOp::Ge => lhs >= rhs,
        _ => return None,
    })
}

fn int_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(value)) => Some(*value as i128),
        Expr::Value(RustValue::I128(value)) => Some(*value),
        _ => None,
    }
}
