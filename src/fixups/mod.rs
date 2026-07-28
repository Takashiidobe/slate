//! Rust cleanup passes that run after faithful CIR lowering.

pub(crate) mod facts;
mod idents;
mod rewrite;
mod runtime;
mod support;
pub mod trace;

#[cfg(test)]
mod test_support;

use crate::fixups::trace::{CollectingLogger, NoopLogger, ProgramSummary, TraceLog, TraceLogger};
use crate::rust_ast::{FnDef, IndentStmt, Item, Program};

use crate::fixups::rewrite::constant_conditions::ConstantConditions;
use crate::fixups::rewrite::dead_locals::DeadLocals;
use crate::fixups::rewrite::for_continue::ForContinue;
use crate::fixups::rewrite::struct_field_init::StructFieldInit;
use crate::fixups::rewrite::var_aliases::VarAliases;
pub use trace::Pass;

#[derive(Debug, Clone, Default)]
pub struct SkipSet(std::collections::HashSet<Pass>);

impl SkipSet {
    pub fn none() -> Self {
        Self::default()
    }

    pub fn skip(pass: Pass) -> Self {
        let mut set = Self::default();
        set.0.insert(pass);
        set
    }

    fn contains(&self, pass: Pass) -> bool {
        self.0.contains(&pass)
    }
}

pub fn apply(program: Program) -> Program {
    apply_with(program, &SkipSet::none())
}

pub fn apply_with(program: Program, skip: &SkipSet) -> Program {
    let mut logger = NoopLogger;
    apply_with_logger(program, skip, &mut logger, DebugOptions::default())
}

pub fn debug(program: Program) -> String {
    let (_, log) = debug_log_with(program, DebugOptions::default());
    log.render_human()
}

pub fn debug_log(program: Program) -> (Program, TraceLog) {
    debug_log_with(program, DebugOptions::default())
}

pub fn debug_with(program: Program, options: DebugOptions) -> String {
    let (_, log) = debug_log_with(program, options);
    log.render_human()
}

pub fn debug_log_with(program: Program, options: DebugOptions) -> (Program, TraceLog) {
    let mut logger = CollectingLogger::default();
    let final_program = apply_with_logger(program, &SkipSet::none(), &mut logger, options);
    let final_summary = ProgramSummary::from_program(&final_program);
    let log = logger.finish(final_summary);
    (final_program, log)
}

#[derive(Debug, Clone, Copy, Default)]
pub struct DebugOptions {
    pub up_to_pass: Option<Pass>,
    pub only_pass: Option<Pass>,
    pub debug_only_pass: Option<Pass>,
}

impl DebugOptions {
    fn should_run(self, pass: Pass) -> bool {
        self.only_pass.is_none_or(|only| only == pass)
    }

    fn should_log(self, pass: Pass) -> bool {
        self.only_pass.is_none_or(|only| only == pass)
            && self
                .debug_only_pass
                .is_none_or(|debug_only| debug_only == pass)
    }

    fn stops_after(self, pass: Pass) -> bool {
        self.up_to_pass == Some(pass)
    }
}

pub fn valid_pass_names() -> String {
    Pass::ALL
        .iter()
        .map(|pass| pass.name())
        .collect::<Vec<_>>()
        .join(", ")
}

fn apply_with_logger(
    input: Program,
    skip: &SkipSet,
    logger: &mut impl TraceLogger,
    debug_options: DebugOptions,
) -> Program {
    let mut debug_done = false;

    macro_rules! step {
        ($program:ident, $pass:expr, $body:block) => {{
            let pass = $pass;
            let run = !debug_done && debug_options.should_run(pass);
            let tracing = !debug_done && logger.is_enabled() && debug_options.should_log(pass);
            if tracing {
                logger.begin_pass(
                    pass,
                    ProgramSummary::from_program(&$program),
                    $program.emit(),
                );
            }
            let result = if run { Some($body) } else { None };
            if tracing {
                logger.end_pass(ProgramSummary::from_program(&$program), $program.emit());
            }
            if !debug_done && debug_options.stops_after(pass) {
                debug_done = true;
            }
            result
        }};
    }

    let facts::AnalyzedProgram { program, .. } = facts::analyze(&input);
    let mut program = program.clone();
    step!(program, Pass::Goto, {
        to_fixpoint_items(&mut program, FixpointLimit::Unlimited, |_, function| {
            let mut fixup = rewrite::goto::Goto::new(function.name.clone(), logger);
            run_once(&mut function.body, &mut fixup)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::Switch, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::switch::Switch::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::EarlyInlineTemps, {
        let limit = inline_temp_fixpoint_limit(&program);
        to_fixpoint_items_with_facts(&mut program, limit, |item_index, f, facts| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::inline_temps::InlineTemps::new(
                rewrite::inline_temps::Phase::Early,
                function,
                facts,
                logger,
            );
            run_once(&mut f.body, &mut fixup)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::AnonymousStructs, {
        run_once_program(&mut program, |program| {
            rewrite::anonymous_structs::AnonymousStructs::new(&facts, logger).fixup(program)
        });
    });
    step!(program, Pass::ParamSpills, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::param_spills::ParamSpills::new(function, &facts, logger).fixup(f)
        });
    });
    step!(program, Pass::ZeroInit, {
        to_fixpoint_items_with_facts(
            &mut program,
            FixpointLimit::Unlimited,
            |item_index, f, facts| {
                let Some(function) = facts.function_by_item_index(item_index) else {
                    return false;
                };
                let mut fixup = rewrite::zero_init::ZeroInit::new(false, function, facts, logger);
                run_once(&mut f.body, &mut fixup)
            },
        );
    });
    step!(program, Pass::StructFieldInit, {
        let mut fixup = StructFieldInit::new(logger);
        to_fixpoint_items(&mut program, FixpointLimit::Unlimited, |_, f| {
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::SingletonScopes, {
        to_fixpoint_items(&mut program, FixpointLimit::Unlimited, |_, f| {
            let mut fixup = rewrite::singleton_scopes::SingletonScopes::new(f.name.clone(), logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::CompoundAssign, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::compound_assign::CompoundAssign::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::ForContinue, {
        to_fixpoint_items(&mut program, FixpointLimit::Unlimited, |_, f| {
            let mut fixup = ForContinue::new(f.name.clone(), logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::SingletonScopes, {
        to_fixpoint_items(&mut program, FixpointLimit::Unlimited, |_, f| {
            let mut fixup = rewrite::singleton_scopes::SingletonScopes::new(f.name.clone(), logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::ConstantIndexCasts, {
        let mut fixup = rewrite::constant_index_casts::ConstantIndexCasts::new(logger);
        run_once_items(&mut program, |_, f| run_once(&mut f.body, &mut fixup));
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::UnnecessaryCasts, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup =
                rewrite::unnecessary_casts::UnnecessaryCasts::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::CallArgs, {
        to_fixpoint_items_with_facts(
            &mut program,
            FixpointLimit::Unlimited,
            |item_index, f, facts| {
                let Some(function) = facts.function_by_item_index(item_index) else {
                    return false;
                };
                let mut fixup = rewrite::call_args::CallArgs::new(function, facts, logger);
                run_once(&mut f.body, &mut fixup)
            },
        );
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::Retval, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup =
                rewrite::retval::Retval::new(f.name == "main", function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::FinalReturnTemps, {
        to_fixpoint_items_with_facts(
            &mut program,
            FixpointLimit::Unlimited,
            |item_index, f, facts| {
                let Some(function) = facts.function_by_item_index(item_index) else {
                    return false;
                };
                let mut fixup =
                    rewrite::final_return_temps::FinalReturnTemps::new(function, facts, logger);
                run_once(&mut f.body, &mut fixup)
            },
        );
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::LazySingleton, {
        let mut fixup = rewrite::lazy_singleton::LazySingleton::new(&facts, logger);
        run_once_program(&mut program, |program| fixup.fixup(program));
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::DropCallResults, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup =
                rewrite::drop_call_results::DropCallResults::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::StringLift, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::string_lift::StringLift::new(
                Pass::StringLift,
                function,
                &facts,
                &[
                    facts::StringRecoveryCandidate::BorrowedStr,
                    facts::StringRecoveryCandidate::BorrowedBytes,
                ],
                logger,
            );
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::StringParams, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            rewrite::string_params::StringParams::new(facts, logger).fixup(program)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::PtrLen, {
        let mut fixup = rewrite::ptr_len::PtrLen::new(&facts, logger);
        run_once_program(&mut program, |program| fixup.fixup(program));
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::SliceIndex, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::slice_index::SliceIndex::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::SliceLoop, {
        if run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::slice_loop::SliceLoop::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        }) {
            late_loop_cleanup(&mut program, Pass::SliceLoop, logger);
        }
    });
    step!(program, Pass::SliceReduce, {
        if run_once_items(&mut program, |_, f| {
            let mut fixup = rewrite::slice_reduce::SliceReduce::new(f.name.clone(), logger);
            run_once(&mut f.body, &mut fixup)
        }) {
            late_loop_cleanup(&mut program, Pass::SliceReduce, logger);
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::RangeLoop, {
        if !skip.contains(Pass::RangeLoop)
            && run_once_items(&mut program, |item_index, f| {
                let Some(function) = facts.function_by_item_index(item_index) else {
                    return false;
                };
                let mut fixup = rewrite::range_loop::RangeLoop::new(function, &facts, logger);
                run_once(&mut f.body, &mut fixup)
            })
        {
            late_loop_cleanup(&mut program, Pass::RangeLoop, logger);
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::VaList, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::va_list::VaList::new(function, &facts, logger).fixup(f)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::RemoveMut, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::remove_mut::RemoveMut::new(function, &facts, logger).fixup(f)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::StringCopy, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::string_copy::StringCopy::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::StringParams, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            rewrite::string_params::StringParams::new(facts, logger).fixup(program)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::RemoveMut, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::remove_mut::RemoveMut::new(function, &facts, logger).fixup(f)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::StringLibc, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::string_libc::StringLibc::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
        runtime::ensure_numeric_parse(&mut program);
    });
    step!(program, Pass::SortSearch, {
        run_once_program(&mut program, |program| {
            rewrite::sort_search::SortSearch::new(logger).fixup(program)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::HeapOwnership, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::heap_ownership::HeapOwnership::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::DeadLocals, {
        to_fixpoint_items_with_facts(
            &mut program,
            FixpointLimit::Unlimited,
            |item_index, f, facts| {
                let Some(function) = facts.function_by_item_index(item_index) else {
                    return false;
                };
                let mut fixup = DeadLocals::new(Pass::DeadLocals, function, facts, logger);
                run_once(&mut f.body, &mut fixup)
            },
        );
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::RemoveMut, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::remove_mut::RemoveMut::new(function, &facts, logger).fixup(f)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::PrintfFormat, {
        run_once_program(&mut program, |program| {
            rewrite::printf_format::PrintfFormat::new(&facts, logger).fixup(program)
        });
    });
    step!(program, Pass::StringParams, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            rewrite::string_params::StringParams::new(facts, logger).fixup(program)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::RemoveMut, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::remove_mut::RemoveMut::new(function, &facts, logger).fixup(f)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::StringLibc, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::string_libc::StringLibc::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
        runtime::ensure_numeric_parse(&mut program);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::CStrings, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::c_strings::CStrings::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::Stdio, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::stdio::Stdio::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::MemchrPreludeFixupCalls, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup =
                rewrite::memchr_prelude::MemchrPreludeCalls::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::NullablePointer, {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
            if !rewrite::nullable_pointer::NullablePointer::new(logger).fixup(&mut program, &facts)
            {
                break;
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::StringLiftFixupCStrings, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::string_lift::StringLift::new(
                Pass::StringLiftFixupCStrings,
                function,
                &facts,
                &[facts::StringRecoveryCandidate::BorrowedCStr],
                logger,
            );
            run_once(&mut f.body, &mut fixup)
        });
    });
    step!(program, Pass::MemchrPrelude, {
        for item in &mut program.items {
            if let Item::Fn(f) = item {
                rewrite::memchr_prelude::MemchrPrelude::new(logger).fixup(f);
            }
        }
    });
    step!(program, Pass::MemchrPreludePruneUnusedHelper, {
        rewrite::memchr_prelude::MemchrPrelude::new(logger).prune_unused_helper(&mut program);
    });
    step!(program, Pass::LateInlineTemps, {
        inline_temps_to_fixpoint(&mut program, rewrite::inline_temps::Phase::Late, logger);
    });
    step!(program, Pass::PtrCopy, {
        ptr_copy_to_fixpoint(&mut program, logger);
    });
    step!(program, Pass::DeadLocals, {
        to_fixpoint_items_with_facts(
            &mut program,
            FixpointLimit::Unlimited,
            |item_index, f, facts| {
                let Some(function) = facts.function_by_item_index(item_index) else {
                    return false;
                };
                let mut fixup = DeadLocals::new(Pass::DeadLocals, function, facts, logger);
                run_once(&mut f.body, &mut fixup)
            },
        );
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::ArrayElementPointerOrigin, {
        rewrite::array_element_pointer_origin::ArrayElementPointerOrigin::new(logger)
            .fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::BufferCursor, {
        rewrite::buffer_cursor::BufferCursor::new(logger).fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::AtomicLocals, {
        rewrite::atomic_locals::AtomicLocals::new(logger).fixup(&mut program, &facts);
    });
    step!(program, Pass::LateInlineTemps, {
        inline_temps_to_fixpoint(&mut program, rewrite::inline_temps::Phase::Late, logger);
    });
    step!(program, Pass::ZeroInit, {
        to_fixpoint_items_with_facts(
            &mut program,
            FixpointLimit::Unlimited,
            |item_index, f, facts| {
                let Some(function) = facts.function_by_item_index(item_index) else {
                    return false;
                };
                let mut fixup = rewrite::zero_init::ZeroInit::new(true, function, facts, logger);
                run_once(&mut f.body, &mut fixup)
            },
        );
    });
    step!(program, Pass::AtomicCompareExchange, {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
            let mut changed = false;
            for (item_index, item) in program.items.iter_mut().enumerate() {
                if let Item::Fn(f) = item
                    && let Some(function) = facts.function_by_item_index(item_index)
                    && rewrite::atomic_compare_exchange::AtomicCompareExchange::new(logger).fixup(
                        &mut f.body,
                        function,
                        &facts,
                    )
                {
                    changed = true;
                }
            }
            if !changed {
                break;
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::RemoveMut, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::remove_mut::RemoveMut::new(function, &facts, logger).fixup(f)
        });
    });
    step!(program, Pass::VarAliases, {
        inline_var_aliases_to_fixpoint(&mut program, logger);
    });
    step!(program, Pass::ConstantConditions, {
        constant_conditions_to_fixpoint(&mut program, logger);
    });
    step!(program, Pass::LibcExit, {
        rewrite::libc_exit::LibcExit::new(logger).fixup(&mut program);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
    step!(program, Pass::PruneUnusedExterns, {
        rewrite::prune_unused_externs::PruneUnusedExterns::new(logger).fixup(&mut program, &facts);
    });
    step!(program, Pass::UnusedItems, {
        rewrite::unused_items::UnusedItems::new(logger).fixup(&mut program);
    });
    step!(program, Pass::UnusedParams, {
        to_fixpoint_program(&mut program, |program| {
            rewrite::unused_params::UnusedParams::new(logger).fixup(program)
        });
    });
    step!(program, Pass::FinalReturns, {
        for item in &mut program.items {
            if let Item::Fn(f) = item {
                let mut fixup = rewrite::final_returns::FinalReturns::new(
                    f.name.clone(),
                    f.ret.is_some(),
                    logger,
                );
                run_once(&mut f.body, &mut fixup);
            }
        }
    });
    step!(program, Pass::MainZeroExit, {
        for item in &mut program.items {
            if let Item::Fn(f) = item {
                let mut fixup =
                    rewrite::main_zero_exit::MainZeroExit::new(f.name == "main", logger);
                run_once(&mut f.body, &mut fixup);
            }
        }
    });
    let _ = debug_done;
    program.clone()
}

fn constant_conditions_to_fixpoint(program: &mut Program, logger: &mut impl TraceLogger) {
    loop {
        let mut changed = false;
        for item in &mut program.items {
            if let Item::Fn(f) = item
                && ConstantConditions::new(&f.name, logger).fixup(&mut f.body)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn ptr_copy_to_fixpoint(program: &mut Program, logger: &mut impl TraceLogger) {
    loop {
        let mut changed = false;
        for item in &mut program.items {
            if let Item::Fn(f) = item
                && rewrite::ptr_copy::PtrCopy::new(&f.name, logger).fixup(&mut f.body)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

pub trait Fixup {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool;
}

fn run_once(body: &mut Vec<IndentStmt>, fixup: &mut impl Fixup) -> bool {
    fixup.fixup(body)
}

fn run_once_program(program: &mut Program, mut fixup: impl FnMut(&mut Program) -> bool) -> bool {
    fixup(program)
}

fn to_fixpoint_program(program: &mut Program, mut fixup: impl FnMut(&mut Program) -> bool) {
    while fixup(program) {}
}

fn to_fixpoint_program_with_facts(
    program: &mut Program,
    limit: FixpointLimit,
    mut fixup: impl FnMut(&mut Program, &facts::FixupFacts) -> bool,
) {
    let mut completed_rounds = 0;
    while limit.permits(completed_rounds) {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program);
        completed_rounds += 1;
        if !fixup(program, &facts) {
            break;
        }
    }
}

fn run_once_items(program: &mut Program, mut fixup: impl FnMut(usize, &mut FnDef) -> bool) -> bool {
    let mut changed = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item {
            changed |= fixup(item_index, f);
        }
    }
    changed
}

#[derive(Clone, Copy)]
enum FixpointLimit {
    Unlimited,
    Rounds(usize),
}

impl FixpointLimit {
    fn permits(self, completed_rounds: usize) -> bool {
        match self {
            Self::Unlimited => true,
            Self::Rounds(limit) => completed_rounds < limit,
        }
    }
}

fn inline_var_aliases_to_fixpoint(program: &mut Program, logger: &mut impl TraceLogger) {
    let mut fixup = VarAliases::new(logger);
    to_fixpoint_items(program, FixpointLimit::Unlimited, |_, f| {
        run_once(&mut f.body, &mut fixup)
    });
}

fn to_fixpoint_items(
    program: &mut Program,
    limit: FixpointLimit,
    mut fixup: impl FnMut(usize, &mut FnDef) -> bool,
) {
    let mut completed_rounds = 0;
    while limit.permits(completed_rounds) {
        completed_rounds += 1;
        if !run_once_items(program, &mut fixup) {
            break;
        }
    }
}

fn to_fixpoint_items_with_facts(
    program: &mut Program,
    limit: FixpointLimit,
    mut fixup: impl FnMut(usize, &mut FnDef, &facts::FixupFacts) -> bool,
) {
    let mut completed_rounds = 0;
    while limit.permits(completed_rounds) {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program);
        completed_rounds += 1;
        if !run_once_items(program, |item_index, f| fixup(item_index, f, &facts)) {
            break;
        }
    }
}

fn late_loop_cleanup(program: &mut Program, pass: Pass, logger: &mut impl TraceLogger) {
    loop {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program);
        let mut changed = false;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item {
                changed |= rewrite::singleton_scopes::SingletonScopes::with_pass(
                    pass,
                    f.name.clone(),
                    logger,
                )
                .fixup(&mut f.body);
                if let Some(function) = facts.function_by_item_index(item_index) {
                    let mut fixup = DeadLocals::new(pass, function, &facts, logger);
                    changed |= run_once(&mut f.body, &mut fixup);
                }
            }
        }
        if !changed {
            break;
        }
    }
}

fn inline_temps_to_fixpoint(
    program: &mut Program,
    phase: rewrite::inline_temps::Phase,
    logger: &mut impl TraceLogger,
) {
    let limit = inline_temp_fixpoint_limit(program);
    to_fixpoint_items_with_facts(program, limit, |item_index, f, facts| {
        let Some(function) = facts.function_by_item_index(item_index) else {
            return false;
        };
        let mut fixup = rewrite::inline_temps::InlineTemps::new(phase, function, facts, logger);
        run_once(&mut f.body, &mut fixup)
    });
}

fn inline_temp_fixpoint_limit(program: &Program) -> FixpointLimit {
    if ProgramSummary::from_program(program).stmts > 2_000 {
        FixpointLimit::Rounds(5)
    } else {
        FixpointLimit::Unlimited
    }
}
