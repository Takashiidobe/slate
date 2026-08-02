use crate::fixups::facts::Purity;
use crate::fixups::trace::Pass;

use super::super::{
    CallTarget, Definition, DefinitionKind, EditSet, Field, FnCall, Predicate, QueryRule,
    StableExpr, byte_position, known_index, memchr_fallback_body, pointer_at_or_null,
};

pub(in crate::fixups) fn calls() -> QueryRule<FnCall> {
    QueryRule::new(
        Pass::MemchrPreludeFixupCalls,
        "rewrite_memchr_call",
        FnCall {
            target: Field::eq(CallTarget::Generated("__slate_memchr".into())),
            arity: Field::eq(3),
            ..Default::default()
        },
    )
    .case("known_nul", |case, call| {
        let [source, needle, count] = case.call_args(call);
        let source = case.fact(|query| query.byte_source(&source))?;
        let needle_value = case.fact(|query| query.const_u8(&needle))?;
        case.require_at(needle_value == 0, Predicate::ConstantU8, &needle)?;
        let needle_ref = case.fact(|query| query.expression(&needle))?;
        let needle_effects = case.fact(|query| query.expression_effects(&needle_ref))?;
        case.require(needle_effects.purity == Purity::MovablePure)?;
        let nul = case.fact(|query| query.first_nul(&source))?;
        case.fact(|query| query.prefix_contains(&count, nul))?;
        let replacement =
            case.lower_expr(pointer_at_or_null(source, known_index(nul)), &call.site)?;
        Ok(EditSet::replace_expression(call.site.clone(), replacement))
    })
    .case("byte_position", |case, call| {
        let [source, needle, count] = case.call_args(call);
        let source = case.fact(|query| query.byte_source(&source))?;
        case.fact(|query| query.full_byte_view(&source, &count))?;
        let needle_ref = case.fact(|query| query.expression(&needle))?;
        let needle_effects = case.fact(|query| query.expression_effects(&needle_ref))?;
        case.require(needle_effects.purity == Purity::MovablePure)?;
        let needle = StableExpr {
            site: needle_ref.site,
        };
        let replacement = case.lower_expr(
            pointer_at_or_null(source, byte_position(needle)),
            &call.site,
        )?;
        Ok(EditSet::replace_expression(call.site.clone(), replacement))
    })
}

pub(in crate::fixups) fn helper() -> QueryRule<Definition> {
    QueryRule::new(
        Pass::MemchrPrelude,
        "manage_memchr_helper",
        Definition {
            kind: Field::eq(DefinitionKind::Function),
            name: Field::eq("__slate_memchr".into()),
            ..Default::default()
        },
    )
    .case("unused", |case, definition| {
        case.fact(|query| query.zero_users(definition))?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
    .case("retained", |case, definition| {
        let function = case.function(definition)?;
        Ok(EditSet::replace_function_body(
            function,
            memchr_fallback_body(),
        ))
    })
}
