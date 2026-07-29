use crate::fixups::trace::Pass;

use super::super::{
    CallRule, DefinitionRule, byte_position, delete_definition, known_index, memchr_fallback_body,
    pointer_at_or_null, replace_body,
};

pub(in crate::fixups) fn calls() -> CallRule {
    CallRule::generated(
        Pass::MemchrPreludeFixupCalls,
        "rewrite_memchr_call",
        "__slate_memchr",
        3,
    )
    .case("known_nul", |case| {
        let [source, needle, count] = case.args();
        let source = case.byte_source(source)?;
        case.u8_eq(needle, 0)?;
        case.pure(needle)?;
        let nul = case.first_nul(&source)?;
        case.prefix_contains(count, nul)?;
        Ok(pointer_at_or_null(source, known_index(nul)))
    })
    .case("byte_position", |case| {
        let [source, needle, count] = case.args();
        let source = case.byte_source(source)?;
        case.full_byte_view(&source, count)?;
        let needle = case.pure(needle)?;
        Ok(pointer_at_or_null(source, byte_position(needle)))
    })
}

pub(in crate::fixups) fn helper() -> DefinitionRule {
    DefinitionRule::function(
        Pass::MemchrPrelude,
        "manage_memchr_helper",
        "__slate_memchr",
    )
    .case("unused", |case| {
        case.zero_users()?;
        Ok(delete_definition())
    })
    .case("retained", |_| Ok(replace_body(memchr_fallback_body())))
}
