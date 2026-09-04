mod array_iter;
mod compound_assign;
mod constant_index_casts;
mod cstr_literal;
mod dead_store;
mod for_range;
mod getenv_var;
mod inline_temps;
mod label_elide;
mod libc_call;
mod loop_to_while;
mod param_spills;
mod pattern_range;
mod peel_casts;
mod raw_ptr_alias;
mod return_cleanup;
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
        Box::new(structure_goto::reducible::StructureReducible),
        Box::new(label_elide::BreakToElse),
        Box::new(label_elide::TailBreakDrop),
        Box::new(label_elide::LabelElide),
        Box::new(zero_init::ZeroInitFold),
        Box::new(param_spills::ParamSpillFold),
        Box::new(raw_ptr_alias::RawPtrAliasElide),
        Box::new(singleton_scopes::ScopeFlatten),
        Box::new(for_range::ForRangeRecover),
        Box::new(array_iter::ForArrayIterRecover),
        Box::new(loop_to_while::LoopToWhile),
        Box::new(return_cleanup::ReturnSlotFold),
        Box::new(inline_temps::LateInlineTemps),
        Box::new(inline_temps::EffectfulTempForward),
        Box::new(inline_temps::InlineConstArgTemps),
        Box::new(peel_casts::PeelCasts),
        Box::new(constant_index_casts::ConstantIndexCasts),
        Box::new(pattern_range::MatchRangeFold),
        Box::new(cstr_literal::CStrLiteral),
        Box::new(getenv_var::GetenvVar),
        Box::new(compound_assign::CompoundAssignRecover),
        Box::new(dead_store::DeadStore),
        Box::new(return_cleanup::FinalReturnTail),
    ];
    rules.extend(libc_call::rules());
    rules
}
