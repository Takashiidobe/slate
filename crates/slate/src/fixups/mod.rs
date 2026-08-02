//! Rust cleanup passes that run after faithful CIR lowering.

pub(crate) mod facts;
mod idents;
mod query;
mod rewrite;
mod runtime;
mod support;
pub mod trace;

#[cfg(test)]
mod test_support;

use crate::fixups::trace::{CollectingLogger, NoopLogger, ProgramSummary, TraceLog, TraceLogger};
use crate::rust_ast::{FnDef, IndentStmt, Item, Program};

use crate::fixups::rewrite::for_continue::ForContinue;
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

fn splice_incremental_facts(
    pre_edit_facts: &facts::FixupFacts,
    program: &Program,
    touched: &query::TouchedItems,
) -> facts::FixupFacts {
    let mut updated = pre_edit_facts.clone();
    if !touched.removed.is_empty() {
        updated.remove_items(&touched.removed);
    }
    for &item_index in &touched.in_place {
        if let Some(function) = pre_edit_facts.function_by_item_index(item_index) {
            updated.splice_function(program, function);
        }
    }
    if !touched.in_place.is_empty() || !touched.removed.is_empty() {
        facts::casts::collect_facts(program, &mut updated);
    }
    updated
}

struct IncrementalFacts {
    facts: facts::FixupFacts,
    dirty: Dirty,
}

enum Dirty {
    Clean,
    Touched(query::TouchedItems),
    Everything,
}

impl IncrementalFacts {
    fn new(facts: facts::FixupFacts) -> Self {
        Self {
            facts,
            dirty: Dirty::Clean,
        }
    }

    fn mark_touched(&mut self, touched: &query::TouchedItems) {
        if touched.unbounded {
            self.dirty = Dirty::Everything;
            return;
        }
        if touched.in_place.is_empty() && touched.removed.is_empty() {
            return;
        }
        match &mut self.dirty {
            Dirty::Everything => {}
            Dirty::Clean => self.dirty = Dirty::Touched(touched.clone()),
            Dirty::Touched(existing) => existing.merge(touched.clone()),
        }
    }

    fn mark_everything_dirty(&mut self) {
        self.dirty = Dirty::Everything;
    }

    fn resolve(&mut self, program: &Program) -> facts::FixupFacts {
        match std::mem::replace(&mut self.dirty, Dirty::Clean) {
            Dirty::Clean => {}
            Dirty::Everything => {
                self.facts = facts::analyze(program).facts;
            }
            Dirty::Touched(touched) => {
                self.facts = splice_incremental_facts(&self.facts, program, &touched);
            }
        }
        self.facts.clone()
    }
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

    let facts::AnalyzedProgram { program, facts } = facts::analyze(&input);
    let mut program = program.clone();
    let mut incremental = IncrementalFacts::new(facts);
    step!(program, Pass::Goto, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            let plan = {
                let query = query::QueryContext::new(program, facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::goto::rewrite());
                builder.finish()
            };
            plan.apply(program, facts, logger).changed
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::Switch, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::switch::Switch::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::EarlyInlineTemps, {
        let limit = inline_temp_fixpoint_limit(&program);
        to_fixpoint_program_with_facts(&mut program, limit, |program, facts| {
            let plan = {
                let query = query::QueryContext::new(program, facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::inline_temps::early());
                builder.finish()
            };
            plan.apply(program, facts, logger).changed
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::AnonymousStructs, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::anonymous_structs::program());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::ParamSpills, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::param_spills::ParamSpills::new(function, &facts, logger).fixup(f)
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::ZeroInit, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            let plan = {
                let query = query::QueryContext::new(program, facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::zero_init::direct());
                builder.finish()
            };
            plan.apply(program, facts, logger).changed
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::StructFieldInit, {
        loop {
            let facts = incremental.resolve(&program);
            let plan = {
                let query = query::QueryContext::new(&program, &facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::struct_field_init::rewrite());
                builder.finish()
            };
            let report = plan.apply(&mut program, &facts, logger);
            incremental.mark_touched(&report.touched);
            if !report.changed {
                break;
            }
        }
    });
    step!(program, Pass::SingletonScopes, {
        to_fixpoint_items(&mut program, FixpointLimit::Unlimited, |_, f| {
            let mut fixup = rewrite::singleton_scopes::SingletonScopes::new(f.name.clone(), logger);
            run_once(&mut f.body, &mut fixup)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::CompoundAssign, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::compound_assign::direct());
            builder.add_rule(&query, &query::rules::compound_assign::temp_backed());
            builder.finish()
        };
        plan.apply(&mut program, &facts, logger);
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::ForContinue, {
        to_fixpoint_items(&mut program, FixpointLimit::Unlimited, |_, f| {
            let mut fixup = ForContinue::new(f.name.clone(), logger);
            run_once(&mut f.body, &mut fixup)
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::SingletonScopes, {
        to_fixpoint_items(&mut program, FixpointLimit::Unlimited, |_, f| {
            let mut fixup = rewrite::singleton_scopes::SingletonScopes::new(f.name.clone(), logger);
            run_once(&mut f.body, &mut fixup)
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::ConstantIndexCasts, {
        let facts = incremental.resolve(&program);
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::constant_index_casts::rewrite());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::UnnecessaryCasts, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::unnecessary_casts::rewrite());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
    });
    step!(program, Pass::CallArgs, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            let plan = {
                let query = query::QueryContext::new(program, facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::call_args::rewrite());
                builder.finish()
            };
            plan.apply(program, facts, logger).changed
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::Retval, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup =
                rewrite::retval::Retval::new(f.name == "main", function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::FinalReturnTemps, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::final_return_temps::rewrite());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::LazySingleton, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::lazy_singleton::program());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::DropCallResults, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::drop_call_results::rewrite());
            builder.finish()
        };
        plan.apply(&mut program, &facts, logger);
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::StringLift, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::string_lift::rewrite());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
    });
    step!(program, Pass::StringParams, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            rewrite::string_params::StringParams::new(facts, logger).fixup(program)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::PtrLen, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::ptr_len::program());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::SliceIndex, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::slice_index::SliceIndex::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
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
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::SliceReduce, {
        if run_once_items(&mut program, |_, f| {
            let mut fixup = rewrite::slice_reduce::SliceReduce::new(f.name.clone(), logger);
            run_once(&mut f.body, &mut fixup)
        }) {
            late_loop_cleanup(&mut program, Pass::SliceReduce, logger);
        }
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::RangeLoop, {
        if !skip.contains(Pass::RangeLoop) {
            let plan = {
                let query = query::QueryContext::new(&program, &facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::range_loop::rewrite());
                builder.finish()
            };
            let report = plan.apply(&mut program, &facts, logger);
            if report.changed {
                late_loop_cleanup(&mut program, Pass::RangeLoop, logger);
                incremental.mark_everything_dirty();
            } else {
                incremental.mark_touched(&report.touched);
            }
        }
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::VaList, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::va_list::VaList::new(function, &facts, logger).fixup(f)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::RemoveMut, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::remove_mut::RemoveMut::new(function, &facts, logger).fixup(f)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::StringCopy, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::string_copy::StringCopy::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::StringParams, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            rewrite::string_params::StringParams::new(facts, logger).fixup(program)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::RemoveMut, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::remove_mut::RemoveMut::new(function, &facts, logger).fixup(f)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::StringLibc, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::string_libc::StringLibc::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
        runtime::ensure_numeric_parse(&mut program);
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::SortSearch, {
        run_once_program(&mut program, |program| {
            rewrite::sort_search::SortSearch::new(logger).fixup(program)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::HeapOwnership, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::heap_ownership::rewrite());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
    });
    step!(program, Pass::DeadLocals, {
        loop {
            let facts = incremental.resolve(&program);
            let plan = {
                let query = query::QueryContext::new(&program, &facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(
                    &query,
                    &query::rules::dead_locals::rewrite(Pass::DeadLocals),
                );
                builder.finish()
            };
            let report = plan.apply(&mut program, &facts, logger);
            incremental.mark_touched(&report.touched);
            if !report.changed {
                break;
            }
        }
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::RemoveMut, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::remove_mut::RemoveMut::new(function, &facts, logger).fixup(f)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::PrintfFormat, {
        run_once_program(&mut program, |program| {
            rewrite::printf_format::PrintfFormat::new(&facts, logger).fixup(program)
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::StringParams, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            rewrite::string_params::StringParams::new(facts, logger).fixup(program)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::RemoveMut, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::remove_mut::RemoveMut::new(function, &facts, logger).fixup(f)
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::StringLibc, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::string_libc::StringLibc::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
        runtime::ensure_numeric_parse(&mut program);
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::CStrings, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::c_strings::rewrite());
            builder.finish()
        };
        plan.apply(&mut program, &facts, logger);
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::Stdio, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            let mut fixup = rewrite::stdio::Stdio::new(function, &facts, logger);
            run_once(&mut f.body, &mut fixup)
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::MemchrPreludeFixupCalls, {
        let facts = incremental.resolve(&program);
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::memchr::calls());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
        report.changed
    });
    step!(program, Pass::NullablePointer, {
        to_fixpoint_items_with_facts(
            &mut program,
            FixpointLimit::Unlimited,
            |item_index, f, facts| {
                let Some(function) = facts.function_by_item_index(item_index) else {
                    return false;
                };
                let mut fixup =
                    rewrite::nullable_pointer::NullablePointer::new(function, facts, logger);
                run_once(&mut f.body, &mut fixup)
            },
        );
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::StringLiftFixupCStrings, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::string_lift::rewrite_c_strings());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
    });
    step!(program, Pass::MemchrPrelude, {
        let facts = incremental.resolve(&program);
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::memchr::helper());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
        report.changed
    });
    step!(program, Pass::LateInlineTemps, {
        let limit = inline_temp_fixpoint_limit(&program);
        to_fixpoint_program_with_facts(&mut program, limit, |program, facts| {
            let plan = {
                let query = query::QueryContext::new(program, facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::inline_temps::late());
                builder.finish()
            };
            plan.apply(program, facts, logger).changed
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::PtrCopy, {
        to_fixpoint_items(&mut program, FixpointLimit::Unlimited, |_, f| {
            let mut fixup = rewrite::ptr_copy::PtrCopy::new(&f.name, logger);
            run_once(&mut f.body, &mut fixup)
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::DeadLocals, {
        loop {
            let facts = incremental.resolve(&program);
            let plan = {
                let query = query::QueryContext::new(&program, &facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(
                    &query,
                    &query::rules::dead_locals::rewrite(Pass::DeadLocals),
                );
                builder.finish()
            };
            let report = plan.apply(&mut program, &facts, logger);
            incremental.mark_touched(&report.touched);
            if !report.changed {
                break;
            }
        }
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::ArrayElementPointerOrigin, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(
                &query,
                &query::rules::array_element_pointer_origin::rewrite(),
            );
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::BufferCursor, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::buffer_cursor::rewrite());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::AtomicLocals, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::atomic_locals::rewrite());
            builder.finish()
        };
        plan.apply(&mut program, &facts, logger);
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::LateInlineTemps, {
        let limit = inline_temp_fixpoint_limit(&program);
        to_fixpoint_program_with_facts(&mut program, limit, |program, facts| {
            let plan = {
                let query = query::QueryContext::new(program, facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::inline_temps::late());
                builder.finish()
            };
            plan.apply(program, facts, logger).changed
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::ZeroInit, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            let plan = {
                let query = query::QueryContext::new(program, facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::zero_init::deferred());
                builder.finish()
            };
            plan.apply(program, facts, logger).changed
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::AtomicCompareExchange, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            let plan = {
                let query = query::QueryContext::new(program, facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::atomic_compare_exchange::rewrite());
                builder.finish()
            };
            plan.apply(program, facts, logger).changed
        });
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::RemoveMut, {
        run_once_items(&mut program, |item_index, f| {
            let Some(function) = facts.function_by_item_index(item_index) else {
                return false;
            };
            rewrite::remove_mut::RemoveMut::new(function, &facts, logger).fixup(f)
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::VarAliases, {
        to_fixpoint_items(&mut program, FixpointLimit::Unlimited, |_, f| {
            let mut fixup = VarAliases::new(logger);
            run_once(&mut f.body, &mut fixup)
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::ConstantConditions, {
        to_fixpoint_program_with_facts(&mut program, FixpointLimit::Unlimited, |program, facts| {
            let plan = {
                let query = query::QueryContext::new(program, facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::constant_conditions::rewrite());
                builder.finish()
            };
            plan.apply(program, facts, logger).changed
        });
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::LibcExit, {
        let facts = incremental.resolve(&program);
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::libc_exit::calls());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
        report.changed
    });
    step!(program, Pass::UnusedItems, {
        let facts = incremental.resolve(&program);
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::unused_items::rewrite());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
        report.changed
    });
    step!(program, Pass::UnusedParams, {
        loop {
            let facts = incremental.resolve(&program);
            let plan = {
                let query = query::QueryContext::new(&program, &facts);
                let mut builder = query::ItemPlanBuilder::new();
                builder.add_rule(&query, &query::rules::unused_params::rewrite());
                builder.finish()
            };
            let report = plan.apply(&mut program, &facts, logger);
            incremental.mark_touched(&report.touched);
            if !report.changed {
                break;
            }
        }
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
        incremental.mark_everything_dirty();
    });
    step!(program, Pass::MainZeroExit, {
        for item in &mut program.items {
            if let Item::Fn(f) = item {
                let mut fixup =
                    rewrite::main_zero_exit::MainZeroExit::new(f.name == "main", logger);
                run_once(&mut f.body, &mut fixup);
            }
        }
        incremental.mark_everything_dirty();
    });
    let facts = incremental.resolve(&program);
    step!(program, Pass::PruneUnusedDefinitions, {
        let plan = {
            let query = query::QueryContext::new(&program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::externs::unused_known_declarations());
            builder.add_rule(&query, &query::rules::support::unused_numeric_parse());
            builder.finish()
        };
        let report = plan.apply(&mut program, &facts, logger);
        incremental.mark_touched(&report.touched);
        report.changed
    });
    let _ = debug_done;
    program.clone()
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
        for item in &mut program.items {
            if let Item::Fn(f) = item {
                changed |= rewrite::singleton_scopes::SingletonScopes::with_pass(
                    pass,
                    f.name.clone(),
                    logger,
                )
                .fixup(&mut f.body);
            }
        }
        let plan = {
            let query = query::QueryContext::new(program, &facts);
            let mut builder = query::ItemPlanBuilder::new();
            builder.add_rule(&query, &query::rules::dead_locals::rewrite(pass));
            builder.finish()
        };
        let report = plan.apply(program, &facts, logger);
        changed |= report.changed;
        if !changed {
            break;
        }
    }
}

fn inline_temp_fixpoint_limit(program: &Program) -> FixpointLimit {
    if ProgramSummary::from_program(program).stmts > 2_000 {
        FixpointLimit::Rounds(5)
    } else {
        FixpointLimit::Unlimited
    }
}
