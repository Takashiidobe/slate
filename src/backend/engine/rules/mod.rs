mod array_iter;
mod compound_assign;
mod cstr_literal;
mod dead_store;
mod for_range;
mod inline_temps;
mod label_elide;
mod libc_call;
mod pattern_range;
mod peel_casts;
mod raw_ptr_alias;
mod singleton_scopes;
mod structure_dispatch;
mod structure_goto;
mod walk;
mod zero_init;

use super::NodeRule;

pub(super) fn registry() -> Vec<Box<dyn NodeRule>> {
    let mut rules: Vec<Box<dyn NodeRule>> = vec![
        Box::new(structure_dispatch::StructureDispatch),
        Box::new(structure_goto::StructureGoto),
        Box::new(label_elide::BreakToElse),
        Box::new(label_elide::TailBreakDrop),
        Box::new(label_elide::LabelElide),
        Box::new(zero_init::ZeroInitFold),
        Box::new(raw_ptr_alias::RawPtrAliasElide),
        Box::new(singleton_scopes::ScopeFlatten),
        Box::new(for_range::ForRangeRecover),
        Box::new(array_iter::ForArrayIterRecover),
        Box::new(inline_temps::LateInlineTemps),
        Box::new(inline_temps::EffectfulTempForward),
        Box::new(inline_temps::InlineConstArgTemps),
        Box::new(peel_casts::PeelCasts),
        Box::new(pattern_range::MatchRangeFold),
        Box::new(cstr_literal::CStrLiteral),
        Box::new(compound_assign::CompoundAssignRecover),
        Box::new(dead_store::DeadStore),
    ];
    rules.extend(libc_call::rules());
    rules
}
