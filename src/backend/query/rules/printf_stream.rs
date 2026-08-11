use crate::backend::rust_ast::Expr;
use crate::backend::trace::Pass;
use crate::function_identity::Known;

use super::super::{
    CallRecord, CallTarget, EditSet, ExprSite, ExpressionRef, Field, FnCall, ItemCaseContext,
    Predicate, PrintfStream, QueryRule, Rejection, RejectionReason, printf_macro,
};

use super::printf_format::replace_int_returning_call;

pub(in crate::backend) fn fprintf_calls() -> QueryRule<FnCall> {
    QueryRule::new(
        Pass::PrintfStream,
        "rewrite_fprintf_stream_call",
        FnCall {
            target: Field::eq(CallTarget::Known(Known::FPrintf)),
            ..Default::default()
        },
    )
    .case("known_stream_format", rewrite_fprintf_call)
}

pub(in crate::backend) fn fputs_calls() -> QueryRule<FnCall> {
    QueryRule::new(
        Pass::PrintfStream,
        "rewrite_fputs_stream_call",
        FnCall {
            target: Field::eq(CallTarget::Known(Known::FPuts)),
            arity: Field::eq(2),
            ..Default::default()
        },
    )
    .case("known_stream_literal", rewrite_fputs_call)
}

fn rewrite_fprintf_call(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    let stream_site = call.args.first().ok_or_else(|| case.reject())?.clone();
    let stream = resolve_stream(case, &stream_site)?;

    let fact = case.fact(|query| query.printf_call_at(&call.site))?;
    let rest = call
        .args
        .get(2..)
        .ok_or_else(|| case.reject())?
        .iter()
        .map(|site| case.expr(site).cloned())
        .collect::<Option<Vec<_>>>()
        .ok_or_else(|| case.reject())?;
    case.require_at(
        fact.arg_paths.len() == rest.len(),
        Predicate::PrintfCall,
        &call.site,
    )?;
    let format = fact.format.as_ref().ok_or_else(|| {
        case.reject_at(
            Predicate::PrintfCall,
            &call.site,
            RejectionReason::UnsupportedShape,
        )
    })?;
    let macro_call = printf_macro(format, &rest, &fact.arg_facts, stream).ok_or_else(|| {
        case.reject_at(
            Predicate::PrintfCall,
            &call.site,
            RejectionReason::UnsupportedShape,
        )
    })?;

    let target = call
        .trivial_unsafe_site
        .clone()
        .unwrap_or_else(|| call.site.clone());
    replace_int_returning_call(case, target, macro_call)
}

fn rewrite_fputs_call(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    let [message_site, stream_site] = case.call_args::<2>(call);
    let stream = resolve_stream(case, &stream_site)?;
    let message = case
        .expr(&message_site)
        .and_then(literal_c_string)
        .and_then(|bytes| String::from_utf8(bytes).ok())
        .ok_or_else(|| case.reject())?;

    let target = call
        .trivial_unsafe_site
        .clone()
        .unwrap_or_else(|| call.site.clone());
    replace_int_returning_call(case, target, plain_print_macro(stream, &message))
}

fn resolve_stream(
    case: &mut ItemCaseContext<'_, '_>,
    site: &ExprSite,
) -> Result<PrintfStream, Rejection> {
    let mut current = site.clone();
    for _ in 0..8 {
        let Some(expr) = case.expr(&current) else {
            return Err(case.reject());
        };
        if let Some(stream) = classify_stream(expr) {
            return Ok(stream);
        }
        while matches!(case.expr(&current), Some(Expr::Cast { .. })) {
            let child = case.fact(|query| query.expression(&query.child(&current, 0)))?;
            current = child.site;
        }
        let Some(Expr::Var(_)) = case.expr(&current) else {
            return Err(case.reject());
        };
        let binding = case.fact(|query| {
            query.expression_binding(&ExpressionRef {
                site: current.clone(),
            })
        })?;
        let initializer = case.fact(|query| query.binding_initializer(&binding))?;
        current = initializer.site;
    }
    Err(case.reject())
}

fn classify_stream(expr: &Expr) -> Option<PrintfStream> {
    match expr {
        Expr::Cast { expr, .. } => classify_stream(expr),
        Expr::Unsafe(block) if block.stmts.is_empty() => classify_stream(block.tail.as_deref()?),
        Expr::Var(name) if name.as_str() == "stdout" => Some(PrintfStream::Stdout),
        Expr::Var(name) if name.as_str() == "stderr" => Some(PrintfStream::Stderr),
        _ => None,
    }
}

fn literal_c_string(expr: &Expr) -> Option<Vec<u8>> {
    match expr {
        Expr::CStr(bytes) => Some(bytes.clone()),
        Expr::ByteStr(bytes) => Some(bytes.strip_suffix(&[0]).unwrap_or(bytes).to_vec()),
        Expr::Str(text) => Some(text.as_bytes().to_vec()),
        Expr::Cast { expr, .. } => literal_c_string(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            literal_c_string(recv)
        }
        _ => None,
    }
}

fn plain_print_macro(stream: PrintfStream, message: &str) -> Expr {
    Expr::Macro {
        name: stream.plain_macro_name().into(),
        args: vec![Expr::Str(escape_format_braces(message))],
    }
}

fn escape_format_braces(message: &str) -> String {
    message.replace('{', "{{").replace('}', "}}")
}
