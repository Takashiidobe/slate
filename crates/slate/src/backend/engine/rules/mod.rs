mod array_iter;
mod bool_ternary;
mod compound_assign;
mod constant_index_casts;
mod cstr_literal;
mod dead_store;
mod for_range;
mod getenv_var;
mod inline_temps;
mod label_elide;
mod libc_call;
mod literal_casts;
mod loop_to_while;
mod param_spills;
mod pattern_range;
mod peel_casts;
mod raw_ptr_alias;
mod remove_mut;
mod return_cleanup;
mod singleton_scopes;
mod slot_promote;
mod structure_dispatch;
mod structure_goto;
mod walk;
mod zero_init;

use super::NodeRule;
use crate::backend::rust_ast::Prim;
pub(super) use remove_mut::demote_params;

pub(super) fn control_flow_registry() -> Vec<Box<dyn NodeRule>> {
    vec![
        Box::new(structure_dispatch::StructureDispatch),
        Box::new(structure_goto::StructureGoto),
        Box::new(structure_goto::reducible::StructureReducible),
        Box::new(label_elide::BreakToElse),
        Box::new(label_elide::TailBreakDrop),
        Box::new(label_elide::LabelElide),
        Box::new(label_elide::LoopLabelElide),
        Box::new(singleton_scopes::ScopeFlatten),
        Box::new(for_range::ForRangeRecover),
        Box::new(loop_to_while::LoopToWhile),
    ]
}

pub(super) fn registry(target: &slate_parser::target_info::TargetInfo) -> Vec<Box<dyn NodeRule>> {
    let mut rules = control_flow_registry();
    rules.extend(vec![
        Box::new(remove_mut::RemoveMut) as Box<dyn NodeRule>,
        Box::new(slot_promote::SlotPromote),
        Box::new(slot_promote::DeclSink),
        Box::new(zero_init::ZeroInitFold),
        Box::new(param_spills::ParamSpillFold),
        Box::new(raw_ptr_alias::RawPtrAliasElide),
        Box::new(array_iter::ForArrayIterRecover),
        Box::new(return_cleanup::UnreachableAfterReturn),
        Box::new(return_cleanup::ReturnSlotFold),
        Box::new(bool_ternary::BoolTernaryFold),
        Box::new(inline_temps::LateInlineTemps),
        Box::new(inline_temps::EffectfulTempForward),
        Box::new(inline_temps::InlineConstArgTemps),
        Box::new(inline_temps::AsmOutputTempFold),
        Box::new(peel_casts::PeelCasts),
        Box::new(constant_index_casts::ConstantIndexCasts),
        Box::new(pattern_range::MatchRangeFold),
        Box::new(cstr_literal::CStrLiteral {
            c_char: if target.char_signed {
                Prim::I8
            } else {
                Prim::U8
            },
        }),
        Box::new(getenv_var::GetenvVar),
        Box::new(compound_assign::CompoundAssignRecover),
        Box::new(dead_store::DeadStore),
        Box::new(return_cleanup::FinalReturnTail),
    ]);
    rules.extend(libc_call::rules(target));
    rules
}

pub(super) fn late_registry() -> Vec<Box<dyn NodeRule>> {
    vec![Box::new(literal_casts::LiteralCasts)]
}
