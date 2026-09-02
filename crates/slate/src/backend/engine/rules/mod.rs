mod array_iter;
mod cstr_literal;
mod dead_store;
mod for_range;
mod inline_temps;
mod libc_call;
mod negate;
mod not_fold;
mod peel_casts;
mod raw_ptr_alias;
mod singleton_scopes;
mod structure_dispatch;
mod walk;
mod zero_init;

use super::NodeRule;

pub(super) fn registry() -> Vec<Box<dyn NodeRule>> {
    let mut rules: Vec<Box<dyn NodeRule>> = vec![
        Box::new(structure_dispatch::StructureDispatch),
        Box::new(zero_init::ZeroInitFold),
        Box::new(raw_ptr_alias::RawPtrAliasElide),
        Box::new(singleton_scopes::ScopeFlatten),
        Box::new(for_range::ForRangeRecover),
        Box::new(array_iter::ForArrayIterRecover),
        Box::new(inline_temps::LateInlineTemps),
        Box::new(inline_temps::EffectfulTempForward),
        Box::new(inline_temps::InlineConstArgTemps),
        Box::new(peel_casts::PeelCasts),
        Box::new(cstr_literal::CStrLiteral),
        Box::new(dead_store::DeadStore),
        Box::new(not_fold::NotFold),
    ];
    rules.extend(libc_call::rules());
    rules
}
