use crate::backend::rust_ast::{Expr, Stmt};
use crate::backend::trace::Pass;
use crate::function_identity::Known;

use super::super::{
    BindingAccess, CallRecord, CallTarget, EditSet, ExprSite, ExpressionRef, Field, FnCall,
    ItemCaseContext, Predicate, PrintfStream, QueryRule, Rejection, RejectionReason, WholeProgram,
    printf_macro, rewrite_printf_fallback,
};

pub(in crate::backend) fn rewrite() -> QueryRule<FnCall> {
    QueryRule::new(
        Pass::PrintfFormat,
        "rewrite_printf_format_call",
        FnCall {
            target: Field::eq(CallTarget::Known(Known::Printf)),
            ..Default::default()
        },
    )
    .case("known_format", rewrite_printf_call)
}

fn rewrite_printf_call(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    let fact = case.fact(|query| query.printf_call_at(&call.site))?;
    let rest = call
        .args
        .get(1..)
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
    let macro_call = printf_macro(format, &rest, &fact.arg_facts, PrintfStream::Stdout)
        .ok_or_else(|| {
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

pub(in crate::backend) fn replace_int_returning_call(
    case: &mut ItemCaseContext<'_, '_>,
    target: ExprSite,
    replacement: Expr,
) -> Result<EditSet, Rejection> {
    let statement = case.fact(|query| {
        query.enclosing_statement(&ExpressionRef {
            site: target.clone(),
        })
    })?;
    let stmt_root = case.fact(|query| query.statement_expression(&statement, 0))?;
    case.require(stmt_root.site == target)?;
    let indent = case.fact(|query| query.statement(&statement))?;

    match &indent.stmt {
        Stmt::Expr(_) => Ok(EditSet::replace_expression(target, replacement)),
        Stmt::Let { mutable: false, .. } => {
            let binding = case.fact(|query| query.statement_binding(&statement))?;
            let uses = case.fact(|query| query.binding_uses(&binding))?;
            let unused = !uses.uses.iter().any(|usage| {
                matches!(usage.access, BindingAccess::Read | BindingAccess::ReadWrite)
            });
            case.require(unused)?;
            let mut edits = EditSet::new();
            edits.push_replace_statement(
                statement.item_index,
                statement.path.clone(),
                Some(Stmt::Expr(replacement)),
            );
            Ok(edits)
        }
        _ => Err(case.reject()),
    }
}

pub(in crate::backend) fn fallback() -> QueryRule<WholeProgram> {
    QueryRule::new(
        Pass::PrintfFormat,
        "wrap_or_prune_printf_fallback",
        WholeProgram::when(|query| query.has_printf_extern()),
    )
    .case("complete_domain", |case, program| {
        let rewrite = case.fact(|query| rewrite_printf_fallback(query))?;
        Ok(EditSet::replace_program(
            program.clone(),
            rewrite.replacement,
            rewrite.removed,
        ))
    })
}
