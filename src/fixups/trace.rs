use crate::fixups::facts::{AstPath, BindingId, DefUseFact, FixupFacts, FunctionId, PathSegment};
use crate::rust_ast::{Block, IndentStmt, Item, Program, Stmt};
use std::fmt::Write;

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum Pass {
    Goto,
    EarlyInlineTemps,
    AnonymousStructs,
    ParamSpills,
    ZeroInit,
    StructFieldInit,
    SingletonScopes,
    CompoundAssign,
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
    SliceReduce,
    RangeLoop,
    VaList,
    RemoveMut,
    StringCopy,
    StringLibc,
    SortSearch,
    HeapOwnership,
    DeadLocals,
    PrintfFormat,
    CStrings,
    Stdio,
    MemchrPreludeFixupCalls,
    NullablePointer,
    StringLiftFixupCStrings,
    MemchrPrelude,
    MemchrPreludePruneUnusedHelper,
    ArrayElementPointerOrigin,
    BufferCursor,
    AtomicLocals,
    LateInlineTemps,
    AtomicCompareExchange,
    VarAliases,
    PruneUnusedExterns,
    UnusedItems,
    UnusedParams,
    MainZeroExit,
}

impl Pass {
    pub const ALL: &'static [Pass] = &[
        Pass::Goto,
        Pass::EarlyInlineTemps,
        Pass::AnonymousStructs,
        Pass::ParamSpills,
        Pass::ZeroInit,
        Pass::StructFieldInit,
        Pass::SingletonScopes,
        Pass::CompoundAssign,
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
        Pass::SliceReduce,
        Pass::RangeLoop,
        Pass::VaList,
        Pass::RemoveMut,
        Pass::StringCopy,
        Pass::StringLibc,
        Pass::SortSearch,
        Pass::HeapOwnership,
        Pass::DeadLocals,
        Pass::PrintfFormat,
        Pass::CStrings,
        Pass::Stdio,
        Pass::MemchrPreludeFixupCalls,
        Pass::NullablePointer,
        Pass::StringLiftFixupCStrings,
        Pass::MemchrPrelude,
        Pass::MemchrPreludePruneUnusedHelper,
        Pass::ArrayElementPointerOrigin,
        Pass::BufferCursor,
        Pass::AtomicLocals,
        Pass::LateInlineTemps,
        Pass::AtomicCompareExchange,
        Pass::VarAliases,
        Pass::PruneUnusedExterns,
        Pass::UnusedItems,
        Pass::UnusedParams,
        Pass::MainZeroExit,
    ];

    pub fn name(self) -> &'static str {
        match self {
            Pass::Goto => "goto",
            Pass::EarlyInlineTemps => "early_inline_temps",
            Pass::AnonymousStructs => "anonymous_structs",
            Pass::ParamSpills => "param_spills",
            Pass::ZeroInit => "zero_init",
            Pass::StructFieldInit => "struct_field_init",
            Pass::SingletonScopes => "singleton_scopes",
            Pass::CompoundAssign => "compound_assign",
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
            Pass::SliceReduce => "slice_reduce",
            Pass::RangeLoop => "range_loop",
            Pass::VaList => "va_list",
            Pass::RemoveMut => "remove_mut",
            Pass::StringCopy => "string_copy",
            Pass::StringLibc => "string_libc",
            Pass::SortSearch => "sort_search",
            Pass::HeapOwnership => "heap_ownership",
            Pass::DeadLocals => "dead_locals",
            Pass::PrintfFormat => "printf_format",
            Pass::CStrings => "c_strings",
            Pass::Stdio => "stdio",
            Pass::MemchrPreludeFixupCalls => "memchr_prelude::fixup_calls",
            Pass::NullablePointer => "nullable_pointer",
            Pass::StringLiftFixupCStrings => "string_lift::fixup_c_strings",
            Pass::MemchrPrelude => "memchr_prelude",
            Pass::MemchrPreludePruneUnusedHelper => "memchr_prelude::prune_unused_helper",
            Pass::ArrayElementPointerOrigin => "array_element_pointer_origin",
            Pass::BufferCursor => "buffer_cursor",
            Pass::AtomicLocals => "atomic_locals",
            Pass::LateInlineTemps => "late_inline_temps",
            Pass::AtomicCompareExchange => "atomic_compare_exchange",
            Pass::VarAliases => "var_aliases",
            Pass::PruneUnusedExterns => "prune_unused_externs",
            Pass::UnusedItems => "unused_items",
            Pass::UnusedParams => "unused_params",
            Pass::MainZeroExit => "main_zero_exit",
        }
    }

    pub fn parse(name: &str) -> Option<Self> {
        Some(match name {
            "goto" => Pass::Goto,
            "early_inline_temps" => Pass::EarlyInlineTemps,
            "anonymous_structs" => Pass::AnonymousStructs,
            "param_spills" => Pass::ParamSpills,
            "zero_init" => Pass::ZeroInit,
            "struct_field_init" => Pass::StructFieldInit,
            "singleton_scopes" => Pass::SingletonScopes,
            "compound_assign" => Pass::CompoundAssign,
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
            "slice_reduce" => Pass::SliceReduce,
            "range_loop" => Pass::RangeLoop,
            "va_list" => Pass::VaList,
            "remove_mut" => Pass::RemoveMut,
            "string_copy" => Pass::StringCopy,
            "string_libc" => Pass::StringLibc,
            "sort_search" => Pass::SortSearch,
            "heap_ownership" => Pass::HeapOwnership,
            "dead_locals" => Pass::DeadLocals,
            "printf_format" => Pass::PrintfFormat,
            "c_strings" => Pass::CStrings,
            "stdio" => Pass::Stdio,
            "memchr_prelude::fixup_calls" => Pass::MemchrPreludeFixupCalls,
            "nullable_pointer" => Pass::NullablePointer,
            "string_lift::fixup_c_strings" => Pass::StringLiftFixupCStrings,
            "memchr_prelude" => Pass::MemchrPrelude,
            "memchr_prelude::prune_unused_helper" => Pass::MemchrPreludePruneUnusedHelper,
            "array_element_pointer_origin" => Pass::ArrayElementPointerOrigin,
            "buffer_cursor" => Pass::BufferCursor,
            "atomic_locals" => Pass::AtomicLocals,
            "late_inline_temps" => Pass::LateInlineTemps,
            "atomic_compare_exchange" => Pass::AtomicCompareExchange,
            "var_aliases" => Pass::VarAliases,
            "prune_unused_externs" => Pass::PruneUnusedExterns,
            "unused_items" => Pass::UnusedItems,
            "unused_params" => Pass::UnusedParams,
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

pub(in crate::fixups) fn stmt_snippet(label: impl Into<String>, stmt: &Stmt) -> TraceSnippet {
    TraceSnippet::new(label, stmt.render().trim_end())
}

pub(in crate::fixups) fn fact(key: impl Into<String>, value: impl Into<String>) -> TraceFact {
    TraceFact::new(key, value)
}

pub(in crate::fixups) fn binding_facts(facts: &FixupFacts, binding: BindingId) -> Vec<TraceFact> {
    let mut out = vec![fact("binding_id", format!("{binding:?}"))];
    if let Some(name) = facts.binding_name(binding) {
        out.push(fact("binding_name", name));
    }
    if let Some(def_use) = facts.def_use(binding) {
        out.extend(def_use_facts(def_use));
    }
    out
}

pub(in crate::fixups) fn path_fact(key: impl Into<String>, path: &[PathSegment]) -> TraceFact {
    fact(key, path_to_string(path))
}

pub(in crate::fixups) fn ast_path_fact(key: impl Into<String>, path: &AstPath) -> TraceFact {
    fact(key, path_to_string(&path.0))
}

fn def_use_facts(def_use: &DefUseFact) -> Vec<TraceFact> {
    vec![
        fact("reads", def_use.reads.len().to_string()),
        fact("writes", def_use.writes.len().to_string()),
        ast_path_fact("definition", &def_use.definition),
    ]
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
            self.after.stmts as isize - self.before.stmts as isize,
            self.after.temp_lets as isize - self.before.temp_lets as isize,
            self.after.items as isize - self.before.items as isize
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, Program, Stmt};

    fn sample_program() -> Program {
        Program {
            items: vec![Item::Fn(func(
                vec![],
                None,
                vec![
                    temp("_v1", "i32", int(1)),
                    Stmt::Expr(var("_v1")),
                    let_mut("kept", "i32", int(2)),
                ],
            ))],
        }
    }

    #[test]
    fn pass_names_round_trip() {
        for &pass in Pass::ALL {
            assert_eq!(Pass::parse(pass.name()), Some(pass));
        }
        assert_eq!(Pass::parse("not_a_pass"), None);
    }

    #[test]
    fn noop_logger_is_disabled_and_discards_events() {
        let mut logger = NoopLogger;
        assert!(!logger.is_enabled());
        logger.begin_pass(
            Pass::ZeroInit,
            ProgramSummary::from_program(&sample_program()),
            sample_program().emit(),
        );
        logger.rewrite(RewriteEvent {
            pass: Pass::ZeroInit,
            kind: "folded init".into(),
            location: TraceLocation::default(),
            before: Vec::new(),
            after: Vec::new(),
            facts: Vec::new(),
        });
        logger.end_pass(
            ProgramSummary::from_program(&sample_program()),
            sample_program().emit(),
        );
    }

    #[test]
    fn collecting_logger_records_passes_and_rewrite_events() {
        let before = sample_program();
        let after = Program {
            items: vec![Item::Fn(func(
                vec![],
                None,
                vec![Stmt::Expr(int(1)), let_mut("kept", "i32", int(2))],
            ))],
        };
        let mut logger = CollectingLogger::default();
        logger.begin_pass(
            Pass::LateInlineTemps,
            ProgramSummary::from_program(&before),
            before.emit(),
        );
        logger.rewrite(RewriteEvent {
            pass: Pass::LateInlineTemps,
            kind: "inlined temp".into(),
            location: TraceLocation {
                file: Some("tests/fixtures/mem_memchr.c".into()),
                function: Some("main".into()),
                line: Some(4),
                ast_path: Some("stmt[0]".into()),
            },
            before: vec![TraceSnippet::new("producer", "let _v1: i32 = 1;")],
            after: vec![TraceSnippet::new("consumer", "1;")],
            facts: vec![TraceFact::new("reads", "1")],
        });
        logger.end_pass(ProgramSummary::from_program(&after), after.emit());

        let log = logger.finish(ProgramSummary::from_program(&after));
        assert_eq!(log.passes.len(), 1);
        assert_eq!(log.passes[0].pass, Pass::LateInlineTemps);
        assert!(log.passes[0].changed);
        assert_eq!(log.passes[0].events[0].facts[0].key, "reads");
        let rendered = log.render_human();
        assert!(rendered.contains("late_inline_temps"));
        assert!(rendered.contains("function main:"));
        assert!(rendered.contains("inlined temp"));
        assert!(rendered.contains("tests/fixtures/mem_memchr.c:4"));
        assert!(rendered.contains("reads=1"));
    }

    #[test]
    fn human_renderer_groups_events_by_function_and_marks_empty_passes() {
        let summary = ProgramSummary {
            items: 1,
            stmts: 2,
            temp_lets: 0,
        };
        let log = TraceLog {
            passes: vec![
                PassInvocation {
                    pass: Pass::Goto,
                    before: summary.clone(),
                    after: summary.clone(),
                    changed: false,
                    events: Vec::new(),
                },
                PassInvocation {
                    pass: Pass::ZeroInit,
                    before: summary.clone(),
                    after: ProgramSummary {
                        items: 1,
                        stmts: 1,
                        temp_lets: 0,
                    },
                    changed: true,
                    events: vec![
                        RewriteEvent {
                            pass: Pass::ZeroInit,
                            kind: "fold_zero_init_assignment".into(),
                            location: TraceLocation {
                                function: Some("main".into()),
                                ast_path: Some("stmt[1]".into()),
                                ..TraceLocation::default()
                            },
                            before: vec![TraceSnippet::new("assignment", "x = 10;")],
                            after: vec![TraceSnippet::new("declaration", "let mut x: i32 = 10;")],
                            facts: vec![TraceFact::new("binding_is_zero", "true")],
                        },
                        RewriteEvent {
                            pass: Pass::ZeroInit,
                            kind: "fold_zero_init_assignment".into(),
                            location: TraceLocation {
                                function: Some("helper".into()),
                                ast_path: Some("stmt[2]".into()),
                                ..TraceLocation::default()
                            },
                            before: Vec::new(),
                            after: Vec::new(),
                            facts: Vec::new(),
                        },
                    ],
                },
            ],
            final_summary: summary,
        };

        let rendered = log.render_human();
        assert!(rendered.contains("goto                               skipped;"));
        assert!(!rendered.contains("\n  skipped\n"));
        assert!(rendered.contains("  function main:"));
        assert!(rendered.contains("      at fn main, ast stmt[1]"));
        assert!(rendered.contains("      before:"));
        assert!(rendered.contains("        assignment:"));
        assert!(rendered.contains("          x = 10;"));
        assert!(rendered.contains("        binding_is_zero=true"));
        assert!(rendered.contains("  function helper:"));
        assert!(rendered.contains("      at fn helper, ast stmt[2]"));
    }

    #[test]
    fn human_renderer_prints_unknown_location_when_no_location_is_available() {
        let summary = ProgramSummary {
            items: 0,
            stmts: 0,
            temp_lets: 0,
        };
        let log = TraceLog {
            passes: vec![PassInvocation {
                pass: Pass::VarAliases,
                before: summary.clone(),
                after: summary.clone(),
                changed: true,
                events: vec![RewriteEvent {
                    pass: Pass::VarAliases,
                    kind: "inline_var_alias".into(),
                    location: TraceLocation::default(),
                    before: Vec::new(),
                    after: Vec::new(),
                    facts: Vec::new(),
                }],
            }],
            final_summary: summary,
        };

        let rendered = log.render_human();
        assert!(rendered.contains("  function <unknown>:"));
        assert!(rendered.contains("      at <unknown>"));
    }
}
