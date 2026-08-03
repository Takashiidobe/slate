use crate::fixups::facts::Purity;
use crate::fixups::trace::Pass;
use crate::function_identity::Known;
use crate::rust_ast::{BinOp, Expr, Prim, RustValue, Type, UnaryOp};

use super::super::ctype_classify::{self, Classification, ClassifyEnv};
use super::super::{
    CallRecord, CallTarget, EditSet, ExprPattern, ExprSite, ExpressionKind, ExpressionRef, Field,
    FnCall, ItemCaseContext, Proof, QueryRule, Rejection,
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

pub(in crate::fixups) fn classify() -> QueryRule<ExprPattern> {
    QueryRule::new(
        Pass::CTypeLibc,
        "rewrite_ctype_classify_idioms",
        ExprPattern {
            kind: Field::eq(ExpressionKind::Other),
            ..Default::default()
        },
    )
    .case("bitmask_classify", classify_case)
}

fn classify_case(
    case: &mut ItemCaseContext<'_, '_>,
    expression: &ExpressionRef,
) -> Result<EditSet, Rejection> {
    let site = &expression.site;
    let expr = case.expr(site).cloned().ok_or_else(|| case.reject())?;

    // A bare `mask_expr != 0` nested directly under a `!` is the same
    // logical match as the outer `!(...)` node; defer to that outer
    // candidate so the two don't produce overlapping edits.
    if !matches!(
        expr,
        Expr::Unary {
            op: UnaryOp::Not,
            ..
        }
    ) && let Ok(parent) = case.fact(|query| query.parent_expression(expression))
        && matches!(
            case.expr(&parent.site),
            Some(Expr::Unary {
                op: UnaryOp::Not,
                ..
            })
        )
    {
        return Err(case.reject());
    }

    let statement = case.fact(|query| query.enclosing_statement(expression))?;
    let body = case.fact(|query| query.enclosing_statements(&statement))?;
    let env = ClassifyEnv::from_body(body);
    let plan = ctype_classify::classify_plan(&expr, &env).ok_or_else(|| case.reject())?;

    if plan.classification.requires_locale_check() {
        let locale_stable =
            case.fact(|query| Ok(Proof::new(query.setlocale_calls_stay_c(), Vec::new())))?;
        case.require(locale_stable)?;
    }

    let replacement = classify_replacement(plan.classification, plan.arg, plan.negate);
    Ok(EditSet::replace_expression(site.clone(), replacement))
}

fn classify_replacement(classification: Classification, arg: Expr, negate: bool) -> Expr {
    let byte = Expr::Cast {
        expr: Box::new(arg),
        ty: Type::Prim(Prim::U8),
    };
    let positive = match classification {
        Classification::Alpha => is_ascii_method(byte, "is_ascii_alphabetic"),
        Classification::Digit => is_ascii_method(byte, "is_ascii_digit"),
        Classification::Upper => is_ascii_method(byte, "is_ascii_uppercase"),
        Classification::Lower => is_ascii_method(byte, "is_ascii_lowercase"),
        Classification::Alnum => is_ascii_method(byte, "is_ascii_alphanumeric"),
        Classification::XDigit => is_ascii_method(byte, "is_ascii_hexdigit"),
        Classification::Punct => is_ascii_method(byte, "is_ascii_punctuation"),
        Classification::Cntrl => is_ascii_method(byte, "is_ascii_control"),
        Classification::Graph => is_ascii_method(byte, "is_ascii_graphic"),
        // Rust's `is_ascii_whitespace` (the WHATWG definition) omits vertical
        // tab (0x0B), which C's `isspace` includes in the "C" locale.
        Classification::Space => is_ascii_method_or_byte(byte, "is_ascii_whitespace", 0x0B),
        // `is_ascii_graphic` excludes space, but `isprint` includes it.
        Classification::Print => is_ascii_method_or_byte(byte, "is_ascii_graphic", b' '),
    };
    if negate {
        Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(positive),
        }
    } else {
        positive
    }
}

fn is_ascii_method(recv: Expr, method: &str) -> Expr {
    Expr::MethodCall {
        recv: Box::new(recv),
        method: method.into(),
        args: Vec::new(),
    }
}

fn is_ascii_method_or_byte(recv: Expr, method: &str, extra_byte: u8) -> Expr {
    Expr::Binary {
        op: BinOp::Or,
        lhs: Box::new(is_ascii_method(recv.clone(), method)),
        rhs: Box::new(Expr::Binary {
            op: BinOp::Eq,
            lhs: Box::new(recv),
            rhs: Box::new(Expr::Value(RustValue::I64(extra_byte as i64))),
        }),
    }
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
