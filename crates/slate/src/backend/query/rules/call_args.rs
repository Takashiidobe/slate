use crate::backend::facts::{CallArgPinning, CallCallee, Purity};
use crate::backend::rust_ast::{Expr, Stmt};
use crate::backend::trace::Pass;

use super::super::item::StatementRef;
use super::super::{
    Binding, BindingAccess, BindingCategory, BindingRef, EditSet, Field, ItemCaseContext,
    QueryRule, Rejection,
};

fn matcher() -> Binding {
    Binding {
        kind: Field::eq(BindingCategory::Local),
        name: Field::predicate(|name: &String, _| is_temp_name(name)),
        ..Default::default()
    }
}

pub(in crate::backend) fn rewrite() -> QueryRule<Binding> {
    QueryRule::new(Pass::CallArgs, "inline_call_arg_temp", matcher())
        .case("inline", inline_call_arg)
        .ordered_non_overlapping()
}

fn inline_call_arg<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
) -> Result<EditSet, Rejection> {
    let initializer = case.fact(|query| query.binding_initializer(binding))?;
    let uses = case.fact(|query| query.binding_uses(binding))?;
    let reads = uses
        .uses
        .iter()
        .filter(|usage| usage.access == BindingAccess::Read)
        .collect::<Vec<_>>();
    case.require(reads.len() == 1 && uses.uses.len() == 1)?;
    let use_expression = reads[0]
        .expression()
        .cloned()
        .ok_or_else(|| case.reject())?;

    let (call, _slot) = case.fact(|query| query.argument_position(&use_expression))?;
    let Some(Expr::Call { func, .. }) = case.expr(&call.site) else {
        return Err(case.reject());
    };
    let function_pointer = if matches!(func.as_ref(), Expr::Var(_)) {
        false
    } else if is_option_unwrap_callee(func) {
        true
    } else {
        return Err(case.reject());
    };

    let producer = StatementRef {
        item_index: binding.item_index,
        path: binding.definition.clone(),
    };
    let producer_statement = case.fact(|query| query.statement(&producer))?;
    case.require(matches!(
        producer_statement.stmt,
        Stmt::Let { mutable: false, .. }
    ))?;
    let producer_container = producer.container().ok_or_else(|| case.reject())?;
    let consumer =
        case.fact(|query| query.statement_in_container(&producer_container, &use_expression))?;
    let producer_index = producer.index().ok_or_else(|| case.reject())?;
    let consumer_index = consumer.index().ok_or_else(|| case.reject())?;
    case.require(consumer_index > producer_index)?;

    for statement in case.fact(|query| query.statements_between(&producer, &consumer))? {
        let indent = case.fact(|query| query.statement(&statement))?;
        let Stmt::Let {
            name,
            mutable: false,
            init: Some(_),
            ..
        } = &indent.stmt
        else {
            return Err(case.reject());
        };
        case.require(is_temp_name(name))?;
        let intermediate = case.fact(|query| query.statement_binding(&statement))?;
        let intermediate_init = case.fact(|query| query.binding_initializer(&intermediate))?;
        let effects = case.fact(|query| query.expression_effects(&intermediate_init))?;
        case.require(effects.purity == Purity::MovablePure)?;
    }

    let producer_effects = case.fact(|query| query.expression_effects(&initializer))?;
    if function_pointer {
        case.require(producer_effects.purity == Purity::MovablePure)?;
    } else {
        let known_return = case
            .fact(|query| query.callsite_at(&initializer.site))
            .is_ok_and(|callee| {
                matches!(
                    callee,
                    CallCallee::Direct {
                        signature: Some(_),
                        ..
                    }
                )
            });
        if !known_return {
            let (pinning, variadic, _) =
                case.fact(|query| query.call_argument_pinning(&use_expression.site))?;
            case.require(
                pinning == CallArgPinning::DeclaredParam
                    && !variadic
                    && producer_effects.purity == Purity::MovablePure,
            )?;
        }
    }

    let init = case
        .expr(&initializer.site)
        .cloned()
        .ok_or_else(|| case.reject())?;
    let mut edits = EditSet::new();
    edits.push_replace_expression(use_expression.site, init);
    edits.push_replace_statement(binding.item_index, binding.definition.clone(), None);
    Ok(edits)
}

fn is_option_unwrap_callee(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::MethodCall { method, args, .. } if method == "unwrap" && args.is_empty()
    )
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|byte| byte.is_ascii_digit()))
}
