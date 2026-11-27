mod array_iter;
mod branch_init;
mod dead_store;
mod for_range;
mod inline_temps;
mod label_elide;
pub(super) mod literal_casts;
mod mutation;
mod pure_stmts;
mod raw_ptr_alias;
mod return_cleanup;
mod singleton_scopes;
mod slot_promote;
pub(super) mod walk;

use super::NodeRule;

pub(super) fn control_flow_registry() -> Vec<Box<dyn NodeRule>> {
    vec![
        Box::new(label_elide::BreakToElse),
        Box::new(label_elide::TailBreakDrop),
        Box::new(label_elide::LoopTailBreak),
        Box::new(label_elide::LabelElide),
        Box::new(label_elide::LoopLabelElide),
        Box::new(singleton_scopes::ScopeFlatten),
        Box::new(for_range::ForRangeRecover),
    ]
}

pub(super) fn registry() -> Vec<Box<dyn NodeRule>> {
    let mut rules = control_flow_registry();
    rules.extend(vec![
        Box::new(slot_promote::SlotPromote) as Box<dyn NodeRule>,
        Box::new(slot_promote::DeclSink),
        Box::new(branch_init::BranchInitFold),
        Box::new(raw_ptr_alias::RawPtrAliasElide),
        Box::new(array_iter::ForArrayIterRecover),
        Box::new(return_cleanup::UnreachableAfterReturn),
        Box::new(inline_temps::LateInlineTemps),
    ]);
    rules.extend(vec![
        Box::new(dead_store::DeadStore) as Box<dyn NodeRule>,
        Box::new(pure_stmts::PureStmtDrop),
        Box::new(pure_stmts::EmptyElseDrop),
        Box::new(pure_stmts::EmptyThenInvert),
        Box::new(return_cleanup::FinalReturnTail),
    ]);
    rules
}

pub(super) fn late_registry() -> Vec<Box<dyn NodeRule>> {
    vec![Box::new(literal_casts::LiteralCasts)]
}

pub(super) fn unsafe_registry() -> Vec<Box<dyn NodeRule>> {
    vec![Box::new(singleton_scopes::ScopeFlatten)]
}
