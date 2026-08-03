use crate::fixups::facts::Purity;
use crate::fixups::trace::Pass;
use crate::function_identity::Known;
use crate::rust_ast::{BinOp, Expr, RustValue};

use super::super::{
    CallRecord, CallTarget, EditSet, ExprSite, FnCall, ItemCaseContext, Proof, QueryRule, Rejection,
};

pub(in crate::fixups) fn calls() -> QueryRule<FnCall> {
    QueryRule::new(
        Pass::CTypeLibc,
        "rewrite_ctype_libc_idioms",
        FnCall::default(),
    )
    .case("toupper", toupper_case)
    .case("tolower", tolower_case)
}

fn edit_target(call: &CallRecord) -> ExprSite {
    call.trivial_unsafe_site
        .clone()
        .unwrap_or_else(|| call.site.clone())
}

fn toupper_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::ToUpper)))?;
    let replacement = ctype_classify_replacement(case, call, b'a' as i64, b'z' as i64, -32)?;
    Ok(EditSet::replace_expression(edit_target(call), replacement))
}

fn tolower_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::ToLower)))?;
    let replacement = ctype_classify_replacement(case, call, b'A' as i64, b'Z' as i64, 32)?;
    Ok(EditSet::replace_expression(edit_target(call), replacement))
}

fn ctype_classify_replacement(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
    low: i64,
    high: i64,
    delta: i64,
) -> Result<Expr, Rejection> {
    case.require(call.args.len() == 1)?;
    let locale_stable =
        case.fact(|query| Ok(Proof::new(query.setlocale_calls_stay_c(), Vec::new())))?;
    case.require(locale_stable)?;
    let [arg] = case.call_args(call);
    let arg_ref = case.fact(|query| query.expression(&arg))?;
    let effects = case.fact(|query| query.expression_effects(&arg_ref))?;
    case.require(effects.purity == Purity::MovablePure)?;
    let value = case.expr(&arg).cloned().ok_or_else(|| case.reject())?;
    Ok(Expr::If {
        cond: Box::new(Expr::Binary {
            op: BinOp::And,
            lhs: Box::new(Expr::Binary {
                op: BinOp::Ge,
                lhs: Box::new(value.clone()),
                rhs: Box::new(int_lit(low)),
            }),
            rhs: Box::new(Expr::Binary {
                op: BinOp::Le,
                lhs: Box::new(value.clone()),
                rhs: Box::new(int_lit(high)),
            }),
        }),
        then_expr: Box::new(Expr::Binary {
            op: BinOp::Add,
            lhs: Box::new(value.clone()),
            rhs: Box::new(int_lit(delta)),
        }),
        else_expr: Box::new(value),
    })
}

fn int_lit(value: i64) -> Expr {
    Expr::Value(RustValue::I64(value))
}
