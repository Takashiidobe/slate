use crate::fixups::trace::Pass;

use super::super::{
    CallTarget, Definition, DefinitionKind, EditSet, Field, FnCall, QueryRule, byte_position,
    known_index, memchr_fallback_body, pointer_at_or_null,
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
        let source = case.byte_source(&source)?;
        case.u8_eq(&needle, 0)?;
        case.pure(&needle)?;
        let nul = case.first_nul(&source)?;
        case.prefix_contains(&count, nul)?;
        let replacement =
            case.lower_expr(pointer_at_or_null(source, known_index(nul)), &call.site)?;
        Ok(EditSet::replace_expression(call.site.clone(), replacement))
    })
    .case("byte_position", |case, call| {
        let [source, needle, count] = case.call_args(call);
        let source = case.byte_source(&source)?;
        case.full_byte_view(&source, &count)?;
        let needle = case.pure(&needle)?;
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
        case.zero_users(definition)?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
    .case("retained", |_, definition| {
        Ok(EditSet::replace_function_body(
            definition.clone(),
            memchr_fallback_body(),
        ))
    })
}
