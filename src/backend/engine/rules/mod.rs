mod cstr_literal;
mod dead_store;
mod inline_temps;
mod libc_call;
mod peel_casts;
mod raw_ptr_alias;
mod singleton_scopes;
mod walk;
mod zero_init;

use super::NodeRule;

pub(super) fn registry() -> Vec<Box<dyn NodeRule>> {
    let mut rules: Vec<Box<dyn NodeRule>> = vec![
        Box::new(zero_init::ZeroInitFold),
        Box::new(raw_ptr_alias::RawPtrAliasElide),
        Box::new(singleton_scopes::WhileLoopUnwrap),
        Box::new(singleton_scopes::DoWhileLoopUnwrap),
        Box::new(singleton_scopes::SingletonUnwrap),
        Box::new(inline_temps::LateInlineTemps),
        Box::new(inline_temps::EffectfulTempForward),
        Box::new(inline_temps::InlineConstArgTemps),
        Box::new(peel_casts::PeelCasts),
        Box::new(cstr_literal::CStrLiteral),
        Box::new(dead_store::DeadStore),
    ];
    rules.extend(libc_call::rules());
    rules
}
