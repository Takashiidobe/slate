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
use crate::rust_ast::{Item, Program};

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

    let facts::AnalyzedProgram { program, .. } = facts::analyze(input);
    let mut program = program;
    step!(program, Pass::Goto, {
        structure_goto(&mut program, logger);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::Switch, {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::switch::Switch::new(logger).fixup(&mut f.body, function, &facts);
            }
        }
    });
    step!(program, Pass::EarlyInlineTemps, {
        inline_temps_to_fixpoint(&mut program, InlinePass::Early, logger);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::AnonymousStructs, {
        rewrite::anonymous_structs::AnonymousStructs::new(logger).fixup(&mut program, &facts);
    });
    step!(program, Pass::ParamSpills, {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::param_spills::ParamSpills::new(logger).fixup(f, function, &facts);
            }
        }
    });
    step!(program, Pass::ZeroInit, {
        zero_init_to_fixpoint(&mut program, false, logger);
    });
    step!(program, Pass::StructFieldInit, {
        struct_field_init_to_fixpoint(&mut program, logger);
    });
    step!(program, Pass::SingletonScopes, {
        singleton_scopes_to_fixpoint(&mut program, logger);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::CompoundAssign, {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::compound_assign::CompoundAssign::new(logger).fixup(
                    &mut f.body,
                    function,
                    &facts,
                );
            }
        }
    });
    step!(program, Pass::ForContinue, {
        cleanup_for_continues(&mut program, logger);
    });
    step!(program, Pass::SingletonScopes, {
        singleton_scopes_to_fixpoint(&mut program, logger);
    });
    step!(program, Pass::ConstantIndexCasts, {
        for item in &mut program.items {
            if let Item::Fn(f) = item {
                rewrite::constant_index_casts::ConstantIndexCasts::new(logger).fixup(&mut f.body);
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::UnnecessaryCasts, {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::unnecessary_casts::UnnecessaryCasts::new(logger).fixup(
                    &mut f.body,
                    function,
                    &facts,
                );
            }
        }
    });
    step!(program, Pass::CallArgs, {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            let mut changed = false;
            for (item_index, item) in program.items.iter_mut().enumerate() {
                if let Item::Fn(f) = item
                    && let Some(function) = facts.function_by_item_index(item_index)
                    && rewrite::call_args::CallArgs::new(logger).fixup(
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
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::Retval, {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::retval::Retval::new(logger).fixup(f, function, &facts);
            }
        }
    });
    step!(program, Pass::FinalReturnTemps, {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            let mut changed = false;
            for (item_index, item) in program.items.iter_mut().enumerate() {
                if let Item::Fn(f) = item
                    && let Some(function) = facts.function_by_item_index(item_index)
                    && rewrite::final_return_temps::FinalReturnTemps::new(logger).fixup(
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
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::LazySingleton, {
        rewrite::lazy_singleton::LazySingleton::new(logger).fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::DropCallResults, {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::drop_call_results::DropCallResults::new(logger).fixup(
                    &mut f.body,
                    function,
                    &facts,
                );
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::StringLift, {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::string_lift::StringLift::new(Pass::StringLift, logger)
                    .fixup_with_recoveries(
                        &mut f.body,
                        function,
                        &facts,
                        &[
                            facts::StringRecoveryCandidate::BorrowedStr,
                            facts::StringRecoveryCandidate::BorrowedBytes,
                        ],
                    );
            }
        }
    });
    step!(program, Pass::StringParams, {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            if !rewrite::string_params::StringParams::new(logger).fixup(&mut program, &facts) {
                break;
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::PtrLen, {
        rewrite::ptr_len::PtrLen::new(logger).fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::SliceIndex, {
        rewrite::slice_index::SliceIndex::new(logger).fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::SliceLoop, {
        if rewrite::slice_loop::SliceLoop::new(logger).fixup(&mut program, &facts) {
            late_loop_cleanup(&mut program, Pass::SliceLoop, logger);
        }
    });
    step!(program, Pass::SliceReduce, {
        if rewrite::slice_reduce::SliceReduce::new(logger).fixup(&mut program) {
            late_loop_cleanup(&mut program, Pass::SliceReduce, logger);
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::RangeLoop, {
        if !skip.contains(Pass::RangeLoop)
            && rewrite::range_loop::RangeLoop::new(logger).fixup(&mut program, &facts)
        {
            late_loop_cleanup(&mut program, Pass::RangeLoop, logger);
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::VaList, {
        rewrite::va_list::VaList::new(logger).fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::RemoveMut, {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::remove_mut::RemoveMut::new(logger).fixup(f, function, &facts);
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::StringCopy, {
        rewrite::string_copy::StringCopy::new(logger).fixup(&mut program, &facts);
    });
    step!(program, Pass::StringParams, {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            if !rewrite::string_params::StringParams::new(logger).fixup(&mut program, &facts) {
                break;
            }
        }
    });
    step!(program, Pass::RemoveMut, {
        remove_mut(&mut program, logger);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::StringLibc, {
        rewrite::string_libc::StringLibc::new(logger).fixup(&mut program, &facts);
    });
    step!(program, Pass::SortSearch, {
        rewrite::sort_search::SortSearch::new(logger).fixup(&mut program);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::HeapOwnership, {
        rewrite::heap_ownership::HeapOwnership::new(logger).fixup(&mut program, &facts);
    });
    step!(program, Pass::DeadLocals, {
        dead_locals_to_fixpoint(&mut program, logger);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::RemoveMut, {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::remove_mut::RemoveMut::new(logger).fixup(f, function, &facts);
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::PrintfFormat, {
        rewrite::printf_format::PrintfFormat::new(logger).fixup(&mut program, &facts);
    });
    step!(program, Pass::StringParams, {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            if !rewrite::string_params::StringParams::new(logger).fixup(&mut program, &facts) {
                break;
            }
        }
    });
    step!(program, Pass::RemoveMut, {
        remove_mut(&mut program, logger);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::StringLibc, {
        rewrite::string_libc::StringLibc::new(logger).fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::CStrings, {
        rewrite::c_strings::CStrings::new(logger).fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::Stdio, {
        rewrite::stdio::Stdio::new(logger).fixup(&mut program, &facts);
    });
    step!(program, Pass::MemchrPreludeFixupCalls, {
        rewrite::memchr_prelude::MemchrPrelude::new(logger).fixup_calls(&mut program, &facts);
    });
    step!(program, Pass::NullablePointer, {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
            if !rewrite::nullable_pointer::NullablePointer::new(logger).fixup(&mut program, &facts)
            {
                break;
            }
        }
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::StringLiftFixupCStrings, {
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
            {
                rewrite::string_lift::StringLift::new(Pass::StringLiftFixupCStrings, logger)
                    .fixup_with_recoveries(
                        &mut f.body,
                        function,
                        &facts,
                        &[facts::StringRecoveryCandidate::BorrowedCStr],
                    );
            }
        }
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
        inline_temps_to_fixpoint(&mut program, InlinePass::Late, logger);
    });
    step!(program, Pass::PtrCopy, {
        ptr_copy_to_fixpoint(&mut program, logger);
    });
    step!(program, Pass::DeadLocals, {
        dead_locals_to_fixpoint(&mut program, logger);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::ArrayElementPointerOrigin, {
        rewrite::array_element_pointer_origin::ArrayElementPointerOrigin::new(logger)
            .fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::BufferCursor, {
        rewrite::buffer_cursor::BufferCursor::new(logger).fixup(&mut program, &facts);
    });
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::AtomicLocals, {
        rewrite::atomic_locals::AtomicLocals::new(logger).fixup(&mut program, &facts);
    });
    step!(program, Pass::LateInlineTemps, {
        inline_temps_to_fixpoint(&mut program, InlinePass::Late, logger);
    });
    step!(program, Pass::ZeroInit, {
        zero_init_to_fixpoint(&mut program, true, logger);
    });
    step!(program, Pass::AtomicCompareExchange, {
        loop {
            let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
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
    step!(program, Pass::RemoveMut, {
        remove_mut(&mut program, logger);
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
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    step!(program, Pass::PruneUnusedExterns, {
        rewrite::prune_unused_externs::PruneUnusedExterns::new(logger).fixup(&mut program, &facts);
    });
    step!(program, Pass::UnusedItems, {
        rewrite::unused_items::UnusedItems::new(logger).fixup(&mut program);
    });
    step!(program, Pass::UnusedParams, {
        rewrite::unused_params::UnusedParams::new(logger).fixup(&mut program);
    });
    step!(program, Pass::FinalReturns, {
        for item in &mut program.items {
            if let Item::Fn(f) = item {
                rewrite::final_returns::FinalReturns::new(logger).fixup(f);
            }
        }
    });
    step!(program, Pass::MainZeroExit, {
        for item in &mut program.items {
            if let Item::Fn(f) = item {
                rewrite::main_zero_exit::MainZeroExit::new(logger).fixup(f);
            }
        }
    });
    let _ = debug_done;
    program
}

fn structure_goto(program: &mut Program, logger: &mut impl TraceLogger) {
    for item in &mut program.items {
        if let Item::Fn(f) = item {
            while rewrite::goto::Goto::new(f.name.clone(), logger).fixup(&mut f.body) {}
        }
    }
}

fn constant_conditions_to_fixpoint(program: &mut Program, logger: &mut impl TraceLogger) {
    loop {
        let mut changed = false;
        for item in &mut program.items {
            if let Item::Fn(f) = item
                && rewrite::constant_conditions::ConstantConditions::new(&f.name, logger)
                    .fixup(&mut f.body)
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

fn cleanup_for_continues(program: &mut Program, logger: &mut impl TraceLogger) {
    loop {
        let mut changed = false;
        for item in &mut program.items {
            if let Item::Fn(f) = item
                && rewrite::for_continue::ForContinue::new(f.name.clone(), logger)
                    .fixup(&mut f.body)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn singleton_scopes_to_fixpoint(program: &mut Program, logger: &mut impl TraceLogger) {
    loop {
        let mut changed = false;
        for item in &mut program.items {
            if let Item::Fn(f) = item
                && rewrite::singleton_scopes::SingletonScopes::new(f.name.clone(), logger)
                    .fixup(&mut f.body)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn zero_init_to_fixpoint(
    program: &mut Program,
    cross_effects: bool,
    logger: &mut impl TraceLogger,
) {
    loop {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        let mut changed = false;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
                && run_zero_init_pass(&mut f.body, function, &facts, cross_effects, logger)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn struct_field_init_to_fixpoint(program: &mut Program, logger: &mut impl TraceLogger) {
    loop {
        let mut changed = false;
        for item in &mut program.items {
            if let Item::Fn(f) = item
                && rewrite::struct_field_init::StructFieldInit::new(logger).fixup(&mut f.body)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn late_loop_cleanup(program: &mut Program, pass: Pass, logger: &mut impl TraceLogger) {
    loop {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        let mut changed = false;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item {
                if rewrite::singleton_scopes::SingletonScopes::with_pass(
                    pass,
                    f.name.clone(),
                    logger,
                )
                .fixup(&mut f.body)
                {
                    changed = true;
                }
                if let Some(function) = facts.function_by_item_index(item_index)
                    && rewrite::dead_locals::DeadLocals::with_pass(pass, logger).fixup(
                        &mut f.body,
                        function,
                        &facts,
                    )
                {
                    changed = true;
                }
            }
        }
        if !changed {
            break;
        }
    }
}

fn dead_locals_to_fixpoint(program: &mut Program, logger: &mut impl TraceLogger) {
    loop {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        let mut changed = false;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
                && run_dead_locals_pass(&mut f.body, function, &facts, logger)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn run_dead_locals_pass(
    body: &mut Vec<crate::rust_ast::IndentStmt>,
    function: facts::FunctionId,
    facts: &facts::FixupFacts,
    logger: &mut impl TraceLogger,
) -> bool {
    rewrite::dead_locals::DeadLocals::new(logger).fixup(body, function, facts)
}

#[derive(Clone, Copy)]
enum InlinePass {
    Early,
    Late,
}

fn inline_temps_to_fixpoint(
    program: &mut Program,
    pass: InlinePass,
    logger: &mut impl TraceLogger,
) {
    let inline_round_limit = if ProgramSummary::from_program(program).stmts > 2_000 {
        5
    } else {
        usize::MAX
    };
    let mut rounds = 0;
    while rounds < inline_round_limit {
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        rounds += 1;
        let mut changed = false;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            if let Item::Fn(f) = item
                && let Some(function) = facts.function_by_item_index(item_index)
                && match pass {
                    InlinePass::Early => run_inline_temps_pass(
                        &mut f.body,
                        function,
                        &facts,
                        rewrite::inline_temps::Phase::Early,
                        logger,
                    ),
                    InlinePass::Late => run_inline_temps_pass(
                        &mut f.body,
                        function,
                        &facts,
                        rewrite::inline_temps::Phase::Late,
                        logger,
                    ),
                }
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn run_inline_temps_pass(
    body: &mut Vec<crate::rust_ast::IndentStmt>,
    function: facts::FunctionId,
    facts: &facts::FixupFacts,
    phase: rewrite::inline_temps::Phase,
    logger: &mut impl TraceLogger,
) -> bool {
    rewrite::inline_temps::InlineTemps::new(phase, logger).fixup(body, function, facts)
}

fn remove_mut(program: &mut Program, logger: &mut impl TraceLogger) {
    let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            rewrite::remove_mut::RemoveMut::new(logger).fixup(f, function, &facts);
        }
    }
}

fn run_zero_init_pass(
    body: &mut Vec<crate::rust_ast::IndentStmt>,
    function: facts::FunctionId,
    facts: &facts::FixupFacts,
    cross_effects: bool,
    logger: &mut impl TraceLogger,
) -> bool {
    rewrite::zero_init::ZeroInit::new(cross_effects, logger).fixup(body, function, facts)
}

fn inline_var_aliases_to_fixpoint(program: &mut Program, logger: &mut impl TraceLogger) {
    loop {
        let mut changed = false;
        for item in &mut program.items {
            if let Item::Fn(f) = item
                && run_var_aliases_pass(&mut f.body, logger)
            {
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
}

fn run_var_aliases_pass(
    body: &mut Vec<crate::rust_ast::IndentStmt>,
    logger: &mut impl TraceLogger,
) -> bool {
    rewrite::var_aliases::VarAliases::new(logger).fixup(body)
}
