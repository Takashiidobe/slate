mod array_iter;
mod branch_init;
mod compound_assign;
mod cstr_literal;
mod dead_store;
mod for_range;
mod getenv_var;
mod inline_temps;
mod label_elide;
mod libc_call;
pub(super) mod literal_casts;
mod loop_to_while;
mod nested_unsafe;
mod null_compare;
mod pattern_range;
mod ptr_offset;
mod pure_stmts;
mod raw_ptr_alias;
mod remove_mut;
mod representable;
mod return_cleanup;
mod singleton_scopes;
mod slot_promote;
mod structure_dispatch;
mod structure_goto;
pub(super) mod walk;

use super::NodeRule;
use crate::backend::rust_ast::Prim;
pub(super) use remove_mut::demote_params;

pub(super) fn control_flow_registry() -> Vec<Box<dyn NodeRule>> {
    vec![
        Box::new(structure_dispatch::StructureDispatch),
        Box::new(structure_goto::reducible::StructureReducible),
        Box::new(label_elide::BreakToElse),
        Box::new(label_elide::TailBreakDrop),
        Box::new(label_elide::LoopTailBreak),
        Box::new(label_elide::LabelElide),
        Box::new(label_elide::LoopLabelElide),
        Box::new(singleton_scopes::ScopeFlatten),
        Box::new(for_range::ForRangeRecover),
        Box::new(loop_to_while::LoopToWhile),
        Box::new(loop_to_while::LoopElseBreakToWhile),
    ]
}

pub(super) fn registry(
    target: &slate_parser::target_info::TargetInfo,
    runtime: crate::backend::rust_ast::Runtime,
) -> Vec<Box<dyn NodeRule>> {
    let mut rules = control_flow_registry();
    rules.extend(vec![
        Box::new(remove_mut::RemoveMut) as Box<dyn NodeRule>,
        Box::new(slot_promote::SlotPromote),
        Box::new(slot_promote::DeclSink),
        Box::new(branch_init::BranchInitFold),
        Box::new(raw_ptr_alias::RawPtrAliasElide),
        Box::new(array_iter::ForArrayIterRecover),
        Box::new(return_cleanup::UnreachableAfterReturn),
        Box::new(return_cleanup::ReturnSlotFold),
        Box::new(inline_temps::LateInlineTemps),
        Box::new(inline_temps::InlineConstArgTemps),
        Box::new(inline_temps::AsmOutputTempFold),
        Box::new(null_compare::NullCompare),
        Box::new(ptr_offset::PtrOffset {
            pointer_width: target.pointer_width,
        }),
        Box::new(pattern_range::MatchRangeFold),
        Box::new(cstr_literal::CStrLiteral {
            c_char: if slate_parser::target_info::TargetInfo::for_triple(&target.triple)
                .map_or(target.char_signed, |rust_abi| rust_abi.char_signed)
            {
                Prim::I8
            } else {
                Prim::U8
            },
        }),
    ]);
    if runtime.has_std() {
        rules.push(Box::new(getenv_var::GetenvVar));
    }
    rules.extend(vec![
        Box::new(compound_assign::CompoundAssignRecover) as Box<dyn NodeRule>,
        Box::new(dead_store::DeadStore),
        Box::new(pure_stmts::PureStmtDrop),
        Box::new(pure_stmts::EmptyElseDrop),
        Box::new(pure_stmts::EmptyThenInvert),
        Box::new(return_cleanup::FinalReturnTail),
    ]);
    rules.extend(libc_call::rules(target, runtime));
    rules
}

pub(super) fn late_registry() -> Vec<Box<dyn NodeRule>> {
    vec![Box::new(literal_casts::LiteralCasts)]
}

pub(super) fn unsafe_registry() -> Vec<Box<dyn NodeRule>> {
    vec![
        Box::new(nested_unsafe::NestedUnsafeStmt),
        Box::new(nested_unsafe::NestedUnsafeExpr),
        Box::new(singleton_scopes::ScopeFlatten),
    ]
}
