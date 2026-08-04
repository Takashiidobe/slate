use crate::fixups::facts::{FixupFacts, FunctionId, PathSegment};
use crate::rust_ast::{Block, IndentStmt, Item, Program, Stmt};
use std::fmt::Write;

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum Pass {
    Goto,
    Switch,
    EarlyInlineTemps,
    AnonymousStructs,
    ParamSpills,
    ZeroInit,
    StructFieldInit,
    SingletonScopes,
    CompoundAssign,
    Swap,
    ForContinue,
    ConstantIndexCasts,
    UnnecessaryCasts,
    CallArgs,
    Retval,
    FinalReturnTemps,
    LazySingleton,
    DropCallResults,
    StringLift,
    StringParams,
    PtrLen,
    SliceIndex,
    SliceLoop,
    SliceSwap,
    SliceReduce,
    RangeLoop,
    VaList,
    RemoveMut,
    StringCopy,
    StringLibc,
    CTypeLibc,
    SortSearch,
    HeapOwnership,
    OptionBoxLocals,
    InterproceduralAllocPromotion,
    DeadLocals,
    SprintfFormat,
    PrintfFormat,
    PrintfStream,
    CStrings,
    Stdio,
    Perror,
    MemchrPreludeFixupCalls,
    NullablePointer,
    StringLiftFixupCStrings,
    MemchrPrelude,
    PtrCopy,
    MemMove,
    MemSet,
    MemCmp,
    ArrayElementPointerOrigin,
    BufferCursor,
    AtomicLocals,
    LateInlineTemps,
    AtomicCompareExchange,
    AssertRecovery,
    VarAliases,
    ConstantConditions,
    LibcExit,
    UnusedItems,
    UnusedParams,
    FinalReturns,
    MainZeroExit,
    PruneUnusedDefinitions,
}

impl Pass {
    pub const ALL: &'static [Pass] = &[
        Pass::Goto,
        Pass::Switch,
        Pass::EarlyInlineTemps,
        Pass::AnonymousStructs,
        Pass::ParamSpills,
        Pass::ZeroInit,
        Pass::StructFieldInit,
        Pass::SingletonScopes,
        Pass::CompoundAssign,
        Pass::Swap,
        Pass::ForContinue,
        Pass::ConstantIndexCasts,
        Pass::UnnecessaryCasts,
        Pass::CallArgs,
        Pass::Retval,
        Pass::FinalReturnTemps,
        Pass::LazySingleton,
        Pass::DropCallResults,
        Pass::StringLift,
        Pass::StringParams,
        Pass::PtrLen,
        Pass::SliceIndex,
        Pass::SliceLoop,
        Pass::SliceSwap,
        Pass::SliceReduce,
        Pass::RangeLoop,
        Pass::VaList,
        Pass::RemoveMut,
        Pass::StringCopy,
        Pass::StringLibc,
        Pass::CTypeLibc,
        Pass::SortSearch,
        Pass::HeapOwnership,
        Pass::OptionBoxLocals,
        Pass::InterproceduralAllocPromotion,
        Pass::DeadLocals,
        Pass::SprintfFormat,
        Pass::PrintfFormat,
        Pass::PrintfStream,
        Pass::CStrings,
        Pass::Stdio,
        Pass::Perror,
        Pass::MemchrPreludeFixupCalls,
        Pass::NullablePointer,
        Pass::StringLiftFixupCStrings,
        Pass::MemchrPrelude,
        Pass::PtrCopy,
        Pass::MemMove,
        Pass::MemSet,
        Pass::MemCmp,
        Pass::ArrayElementPointerOrigin,
        Pass::BufferCursor,
        Pass::AtomicLocals,
        Pass::LateInlineTemps,
        Pass::AtomicCompareExchange,
        Pass::AssertRecovery,
        Pass::VarAliases,
        Pass::ConstantConditions,
        Pass::LibcExit,
        Pass::UnusedItems,
        Pass::UnusedParams,
        Pass::FinalReturns,
        Pass::MainZeroExit,
        Pass::PruneUnusedDefinitions,
    ];

    pub fn name(self) -> &'static str {
        match self {
            Pass::Goto => "goto",
            Pass::Switch => "switch",
            Pass::EarlyInlineTemps => "early_inline_temps",
            Pass::AnonymousStructs => "anonymous_structs",
            Pass::ParamSpills => "param_spills",
            Pass::ZeroInit => "zero_init",
            Pass::StructFieldInit => "struct_field_init",
            Pass::SingletonScopes => "singleton_scopes",
            Pass::CompoundAssign => "compound_assign",
            Pass::Swap => "swap",
            Pass::ForContinue => "for_continue",
            Pass::ConstantIndexCasts => "constant_index_casts",
            Pass::UnnecessaryCasts => "unnecessary_casts",
            Pass::CallArgs => "call_args",
            Pass::Retval => "retval",
            Pass::FinalReturnTemps => "final_return_temps",
            Pass::LazySingleton => "lazy_singleton",
            Pass::DropCallResults => "drop_call_results",
            Pass::StringLift => "string_lift",
            Pass::StringParams => "string_params",
            Pass::PtrLen => "ptr_len",
            Pass::SliceIndex => "slice_index",
            Pass::SliceLoop => "slice_loop",
            Pass::SliceSwap => "slice_swap",
            Pass::SliceReduce => "slice_reduce",
            Pass::RangeLoop => "range_loop",
            Pass::VaList => "va_list",
            Pass::RemoveMut => "remove_mut",
            Pass::StringCopy => "string_copy",
            Pass::StringLibc => "string_libc",
            Pass::CTypeLibc => "ctype_libc",
            Pass::SortSearch => "sort_search",
            Pass::HeapOwnership => "heap_ownership",
            Pass::OptionBoxLocals => "option_box_locals",
            Pass::InterproceduralAllocPromotion => "interprocedural_alloc_promotion",
            Pass::DeadLocals => "dead_locals",
            Pass::SprintfFormat => "sprintf_format",
            Pass::PrintfFormat => "printf_format",
            Pass::PrintfStream => "printf_stream",
            Pass::CStrings => "c_strings",
            Pass::Stdio => "stdio",
            Pass::Perror => "perror",
            Pass::MemchrPreludeFixupCalls => "memchr_prelude::fixup_calls",
            Pass::NullablePointer => "nullable_pointer",
            Pass::StringLiftFixupCStrings => "string_lift::fixup_c_strings",
            Pass::MemchrPrelude => "memchr_prelude",
            Pass::PtrCopy => "ptr_copy",
            Pass::MemMove => "mem_move",
            Pass::MemSet => "mem_set",
            Pass::MemCmp => "mem_cmp",
            Pass::ArrayElementPointerOrigin => "array_element_pointer_origin",
            Pass::BufferCursor => "buffer_cursor",
            Pass::AtomicLocals => "atomic_locals",
            Pass::LateInlineTemps => "late_inline_temps",
            Pass::AtomicCompareExchange => "atomic_compare_exchange",
            Pass::AssertRecovery => "assert_recovery",
            Pass::VarAliases => "var_aliases",
            Pass::ConstantConditions => "constant_conditions",
            Pass::LibcExit => "libc_exit",
            Pass::PruneUnusedDefinitions => "prune_unused_definitions",
            Pass::UnusedItems => "unused_items",
            Pass::UnusedParams => "unused_params",
            Pass::FinalReturns => "final_returns",
            Pass::MainZeroExit => "main_zero_exit",
        }
    }

    pub fn parse(name: &str) -> Option<Self> {
        Some(match name {
            "goto" => Pass::Goto,
            "switch" => Pass::Switch,
            "early_inline_temps" => Pass::EarlyInlineTemps,
            "anonymous_structs" => Pass::AnonymousStructs,
            "param_spills" => Pass::ParamSpills,
            "zero_init" => Pass::ZeroInit,
            "struct_field_init" => Pass::StructFieldInit,
            "singleton_scopes" => Pass::SingletonScopes,
            "compound_assign" => Pass::CompoundAssign,
            "swap" => Pass::Swap,
            "for_continue" => Pass::ForContinue,
            "constant_index_casts" => Pass::ConstantIndexCasts,
            "unnecessary_casts" => Pass::UnnecessaryCasts,
            "call_args" => Pass::CallArgs,
            "retval" => Pass::Retval,
            "final_return_temps" => Pass::FinalReturnTemps,
            "lazy_singleton" => Pass::LazySingleton,
            "drop_call_results" => Pass::DropCallResults,
            "string_lift" => Pass::StringLift,
            "string_params" => Pass::StringParams,
            "ptr_len" => Pass::PtrLen,
            "slice_index" => Pass::SliceIndex,
            "slice_loop" => Pass::SliceLoop,
            "slice_swap" => Pass::SliceSwap,
            "slice_reduce" => Pass::SliceReduce,
            "range_loop" => Pass::RangeLoop,
            "va_list" => Pass::VaList,
            "remove_mut" => Pass::RemoveMut,
            "string_copy" => Pass::StringCopy,
            "string_libc" => Pass::StringLibc,
            "ctype_libc" => Pass::CTypeLibc,
            "sort_search" => Pass::SortSearch,
            "heap_ownership" => Pass::HeapOwnership,
            "option_box_locals" => Pass::OptionBoxLocals,
            "interprocedural_alloc_promotion" => Pass::InterproceduralAllocPromotion,
            "dead_locals" => Pass::DeadLocals,
            "sprintf_format" => Pass::SprintfFormat,
            "printf_format" => Pass::PrintfFormat,
            "printf_stream" => Pass::PrintfStream,
            "c_strings" => Pass::CStrings,
            "stdio" => Pass::Stdio,
            "perror" => Pass::Perror,
            "memchr_prelude::fixup_calls" => Pass::MemchrPreludeFixupCalls,
            "nullable_pointer" => Pass::NullablePointer,
            "string_lift::fixup_c_strings" => Pass::StringLiftFixupCStrings,
            "memchr_prelude" => Pass::MemchrPrelude,
            "ptr_copy" => Pass::PtrCopy,
            "mem_move" => Pass::MemMove,
            "mem_set" => Pass::MemSet,
            "mem_cmp" => Pass::MemCmp,
            "array_element_pointer_origin" => Pass::ArrayElementPointerOrigin,
            "buffer_cursor" => Pass::BufferCursor,
            "atomic_locals" => Pass::AtomicLocals,
            "late_inline_temps" => Pass::LateInlineTemps,
            "atomic_compare_exchange" => Pass::AtomicCompareExchange,
            "assert_recovery" => Pass::AssertRecovery,
            "var_aliases" => Pass::VarAliases,
            "constant_conditions" => Pass::ConstantConditions,
            "libc_exit" => Pass::LibcExit,
            "prune_unused_definitions" => Pass::PruneUnusedDefinitions,
            "unused_items" => Pass::UnusedItems,
            "unused_params" => Pass::UnusedParams,
            "final_returns" => Pass::FinalReturns,
            "main_zero_exit" => Pass::MainZeroExit,
            _ => return None,
        })
    }
}

#[derive(Debug, Clone, PartialEq, Eq, Default)]
pub struct TraceLocation {
    pub file: Option<String>,
    pub function: Option<String>,
    pub line: Option<usize>,
    pub ast_path: Option<String>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct TraceFact {
    pub key: String,
    pub value: String,
}

impl TraceFact {
    pub fn new(key: impl Into<String>, value: impl Into<String>) -> Self {
        Self {
            key: key.into(),
            value: value.into(),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct TraceSnippet {
    pub label: String,
    pub code: String,
}

impl TraceSnippet {
    pub fn new(label: impl Into<String>, code: impl Into<String>) -> Self {
        Self {
            label: label.into(),
            code: code.into(),
        }
    }
}

pub(in crate::fixups) fn path_location(path: &[PathSegment]) -> TraceLocation {
    TraceLocation {
        ast_path: Some(path_to_string(path)),
        ..TraceLocation::default()
    }
}

pub(in crate::fixups) fn function_path_location(
    facts: &FixupFacts,
    function: FunctionId,
    path: &[PathSegment],
) -> TraceLocation {
    TraceLocation {
        function: facts.function_name(function).map(str::to_owned),
        ast_path: Some(path_to_string(path)),
        ..TraceLocation::default()
    }
}

pub(in crate::fixups) fn fact(key: impl Into<String>, value: impl Into<String>) -> TraceFact {
    TraceFact::new(key, value)
}

fn path_to_string(path: &[PathSegment]) -> String {
    let mut out = String::new();
    for segment in path {
        match segment {
            PathSegment::Stmt(index) => {
                write!(&mut out, ".stmt[{index}]").unwrap();
            }
            PathSegment::Then => out.push_str(".then"),
            PathSegment::Else => out.push_str(".else"),
            PathSegment::LoopBody => out.push_str(".loop_body"),
            PathSegment::ForBody => out.push_str(".for_body"),
            PathSegment::ScopeBody => out.push_str(".scope_body"),
            PathSegment::LabeledBody => out.push_str(".labeled_body"),
            PathSegment::MatchArm(index) => {
                write!(&mut out, ".match_arm[{index}]").unwrap();
            }
            PathSegment::UnsafeBody => out.push_str(".unsafe_body"),
            PathSegment::WhileBody => out.push_str(".while_body"),
            PathSegment::BlockBody => out.push_str(".block_body"),
            PathSegment::BlockTail => out.push_str(".block_tail"),
            PathSegment::Expr(index) => {
                write!(&mut out, ".expr[{index}]").unwrap();
            }
        }
    }
    out.strip_prefix('.').unwrap_or(out.as_str()).to_string()
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct RewriteEvent {
    pub pass: Pass,
    pub kind: String,
    pub location: TraceLocation,
    pub before: Vec<TraceSnippet>,
    pub after: Vec<TraceSnippet>,
    pub facts: Vec<TraceFact>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct PassInvocation {
    pub pass: Pass,
    pub before: ProgramSummary,
    pub after: ProgramSummary,
    pub changed: bool,
    pub events: Vec<RewriteEvent>,
}

impl PassInvocation {
    fn summary(&self) -> String {
        let status = if self.changed { "changed" } else { "skipped" };
        format!(
            "{status}; stmts {:+}, temp_lets {:+}, items {:+}",
            self.after.stmts as i64 - self.before.stmts as i64,
            self.after.temp_lets as i64 - self.before.temp_lets as i64,
            self.after.items as i64 - self.before.items as i64
        )
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct TraceLog {
    pub passes: Vec<PassInvocation>,
    pub final_summary: ProgramSummary,
}

impl TraceLog {
    pub fn render_human(&self) -> String {
        let mut out = String::new();
        for pass in &self.passes {
            writeln!(out, "{:<34} {}", pass.pass.name(), pass.summary()).unwrap();
            if pass.events.is_empty() {
                continue;
            }
            for group in function_groups(&pass.events) {
                writeln!(out, "  function {}:", group.label).unwrap();
                for event in group.events {
                    writeln!(out, "    {}", event.kind).unwrap();
                    writeln!(
                        out,
                        "      at {}",
                        render_location(&event.location).unwrap_or_else(|| "<unknown>".into())
                    )
                    .unwrap();
                    render_snippets(&mut out, "before", &event.before);
                    render_snippets(&mut out, "after", &event.after);
                    if !event.facts.is_empty() {
                        writeln!(out, "      facts:").unwrap();
                        for fact in &event.facts {
                            writeln!(out, "        {}={}", fact.key, fact.value).unwrap();
                        }
                    }
                }
            }
        }
        writeln!(
            out,
            "final: items={}, stmts={}, temp_lets={}",
            self.final_summary.items, self.final_summary.stmts, self.final_summary.temp_lets
        )
        .unwrap();
        out
    }
}

struct FunctionGroup<'a> {
    label: String,
    events: Vec<&'a RewriteEvent>,
}

fn function_groups(events: &[RewriteEvent]) -> Vec<FunctionGroup<'_>> {
    let mut groups: Vec<FunctionGroup<'_>> = Vec::new();
    for event in events {
        let label = event
            .location
            .function
            .clone()
            .unwrap_or_else(|| "<unknown>".into());
        if let Some(group) = groups.iter_mut().find(|group| group.label == label) {
            group.events.push(event);
        } else {
            groups.push(FunctionGroup {
                label,
                events: vec![event],
            });
        }
    }
    groups
}

fn render_location(location: &TraceLocation) -> Option<String> {
    let mut parts = Vec::new();
    if let Some(file) = &location.file {
        parts.push(match location.line {
            Some(line) => format!("{file}:{line}"),
            None => file.clone(),
        });
    } else if let Some(line) = location.line {
        parts.push(format!("line {line}"));
    }
    if let Some(function) = &location.function {
        parts.push(format!("fn {function}"));
    }
    if let Some(path) = &location.ast_path {
        parts.push(format!("ast {path}"));
    }
    if parts.is_empty() {
        None
    } else {
        Some(parts.join(", "))
    }
}

fn render_snippets(out: &mut String, title: &str, snippets: &[TraceSnippet]) {
    if snippets.is_empty() {
        return;
    }
    writeln!(out, "      {title}:").unwrap();
    for snippet in snippets {
        writeln!(out, "        {}:", snippet.label).unwrap();
        for line in snippet.code.lines() {
            writeln!(out, "          {line}").unwrap();
        }
    }
}

pub trait TraceLogger {
    fn is_enabled(&self) -> bool;
    fn begin_pass(&mut self, pass: Pass, before: ProgramSummary, before_emit: String);
    fn end_pass(&mut self, after: ProgramSummary, after_emit: String);
    fn rewrite(&mut self, event: RewriteEvent);
}

#[derive(Debug, Default)]
pub struct NoopLogger;

impl TraceLogger for NoopLogger {
    fn is_enabled(&self) -> bool {
        false
    }

    fn begin_pass(&mut self, _pass: Pass, _before: ProgramSummary, _before_emit: String) {}

    fn end_pass(&mut self, _after: ProgramSummary, _after_emit: String) {}

    fn rewrite(&mut self, _event: RewriteEvent) {}
}

#[derive(Debug, Default)]
pub struct CollectingLogger {
    completed: Vec<PassInvocation>,
    current: Option<ActivePass>,
}

#[derive(Debug)]
struct ActivePass {
    pass: Pass,
    before: ProgramSummary,
    before_emit: String,
    events: Vec<RewriteEvent>,
}

impl CollectingLogger {
    pub fn finish(mut self, final_summary: ProgramSummary) -> TraceLog {
        if let Some(active) = self.current.take() {
            self.completed.push(PassInvocation {
                pass: active.pass,
                before: active.before.clone(),
                after: active.before,
                changed: false,
                events: active.events,
            });
        }
        TraceLog {
            passes: self.completed,
            final_summary,
        }
    }
}

impl TraceLogger for CollectingLogger {
    fn is_enabled(&self) -> bool {
        true
    }

    fn begin_pass(&mut self, pass: Pass, before: ProgramSummary, before_emit: String) {
        assert!(self.current.is_none(), "trace pass already active");
        self.current = Some(ActivePass {
            pass,
            before,
            before_emit,
            events: Vec::new(),
        });
    }

    fn end_pass(&mut self, after: ProgramSummary, after_emit: String) {
        let active = self.current.take().expect("trace pass must be active");
        let changed = active.before_emit != after_emit;
        let mut events = active.events;
        if changed && events.is_empty() {
            events.push(generic_pass_changed_event(
                active.pass,
                &active.before,
                &after,
                &active.before_emit,
                &after_emit,
            ));
        }
        self.completed.push(PassInvocation {
            pass: active.pass,
            before: active.before,
            after,
            changed,
            events,
        });
    }

    fn rewrite(&mut self, event: RewriteEvent) {
        if let Some(active) = &mut self.current {
            assert_eq!(
                event.pass, active.pass,
                "rewrite event pass does not match active trace pass"
            );
            active.events.push(event);
        }
    }
}

fn generic_pass_changed_event(
    pass: Pass,
    before: &ProgramSummary,
    after: &ProgramSummary,
    before_emit: &str,
    after_emit: &str,
) -> RewriteEvent {
    RewriteEvent {
        pass,
        kind: "pass_changed".into(),
        location: TraceLocation {
            function: Some("program".into()),
            ast_path: Some("program".into()),
            ..TraceLocation::default()
        },
        before: vec![TraceSnippet::new("program", before_emit.trim_end())],
        after: vec![TraceSnippet::new("program", after_emit.trim_end())],
        facts: vec![
            TraceFact::new("instrumentation", "generic_pass_delta"),
            TraceFact::new("before_items", before.items.to_string()),
            TraceFact::new("after_items", after.items.to_string()),
            TraceFact::new("before_stmts", before.stmts.to_string()),
            TraceFact::new("after_stmts", after.stmts.to_string()),
            TraceFact::new("before_temp_lets", before.temp_lets.to_string()),
            TraceFact::new("after_temp_lets", after.temp_lets.to_string()),
        ],
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ProgramSummary {
    pub items: usize,
    pub stmts: usize,
    pub temp_lets: usize,
}

impl ProgramSummary {
    pub fn from_program(program: &Program) -> Self {
        Self {
            items: program.items.len(),
            stmts: program_stmt_count(program),
            temp_lets: program_temp_let_count(program),
        }
    }
}

fn program_stmt_count(program: &Program) -> usize {
    program
        .items
        .iter()
        .map(|item| match item {
            Item::Fn(f) => stmt_count(&f.body),
            _ => 0,
        })
        .sum()
}

fn stmt_count(stmts: &[IndentStmt]) -> usize {
    stmts
        .iter()
        .map(|stmt| {
            1 + match &stmt.stmt {
                Stmt::LetIf {
                    then_body,
                    else_body,
                    ..
                }
                | Stmt::If {
                    then_body,
                    else_body,
                    ..
                } => stmt_count(then_body) + stmt_count(else_body),
                Stmt::Loop { body, .. }
                | Stmt::For { body, .. }
                | Stmt::Scope { body }
                | Stmt::LabeledBlock { body, .. } => stmt_count(body),
                Stmt::Unsafe { body } | Stmt::Block(body) | Stmt::While { body, .. } => {
                    block_stmt_count(body)
                }
                Stmt::Match { arms, .. } => arms.iter().map(|arm| stmt_count(&arm.body)).sum(),
                _ => 0,
            }
        })
        .sum()
}

fn block_stmt_count(block: &Block) -> usize {
    stmt_count(&block.stmts)
}

fn program_temp_let_count(program: &Program) -> usize {
    program
        .items
        .iter()
        .map(|item| match item {
            Item::Fn(f) => temp_let_count(&f.body),
            _ => 0,
        })
        .sum()
}

fn temp_let_count(stmts: &[IndentStmt]) -> usize {
    stmts
        .iter()
        .map(|stmt| {
            let here = match &stmt.stmt {
                Stmt::Let { name, .. } if is_temp_name(name) => 1,
                _ => 0,
            };
            here + match &stmt.stmt {
                Stmt::LetIf {
                    then_body,
                    else_body,
                    ..
                }
                | Stmt::If {
                    then_body,
                    else_body,
                    ..
                } => temp_let_count(then_body) + temp_let_count(else_body),
                Stmt::Loop { body, .. }
                | Stmt::For { body, .. }
                | Stmt::Scope { body }
                | Stmt::LabeledBlock { body, .. } => temp_let_count(body),
                Stmt::Unsafe { body } | Stmt::Block(body) | Stmt::While { body, .. } => {
                    block_temp_let_count(body)
                }
                Stmt::Match { arms, .. } => arms.iter().map(|arm| temp_let_count(&arm.body)).sum(),
                _ => 0,
            }
        })
        .sum()
}

fn block_temp_let_count(block: &Block) -> usize {
    temp_let_count(&block.stmts)
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}
