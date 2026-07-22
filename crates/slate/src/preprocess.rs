//! Preprocessing oracle: record the `#if`/`#ifdef`/`#ifndef`/`#elif`/`#else`/
//! `#endif` regions of a C source before CIR lowering.
//!
//! A single Clang invocation only sees the active preprocessed branch, so CIR
//! and the Clang AST cannot describe portable, target-gated source. This module
//! scans the raw source for conditional directives and records, per branch, the
//! normalized predicate, its source ranges, the mapped Rust `cfg`, and whether
//! the branch is active for a given macro environment. Later stages join this
//! metadata to lowered items to recover `#[cfg(...)]` gates.
//!
//! Scope is deliberately narrow: predicates are normalized only for known
//! target/debug macros and project-style feature macros; anything else is kept
//! as an opaque diagnostic rather than guessed.

use crate::rust_ast::Cfg;
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicU64, Ordering};

static NEXT_SANITIZED_INPUT: AtomicU64 = AtomicU64::new(0);

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum DirectiveKind {
    If,
    Ifdef,
    Ifndef,
    Elif,
    Else,
}

impl DirectiveKind {
    pub fn as_str(self) -> &'static str {
        match self {
            DirectiveKind::If => "if",
            DirectiveKind::Ifdef => "ifdef",
            DirectiveKind::Ifndef => "ifndef",
            DirectiveKind::Elif => "elif",
            DirectiveKind::Else => "else",
        }
    }
}

/// Why a conditional region cannot be recovered as a portable Rust `cfg`. The
/// two predicate classes are kept distinct so users can tell an *unsupported but
/// recorded* predicate (a clean `defined(...)` shape over an unknown macro —
/// fixable with a config-matrix or feature mapping) from a predicate *shape* we
/// cannot normalize at all.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum DiagnosticKind {
    /// A boolean-over-`defined()` predicate that names a macro with no known
    /// target/debug cfg mapping. Recorded, but not mappable without user input.
    UnmappedMacro,
    /// A predicate whose shape is outside the `defined()` subset (arithmetic,
    /// comparisons, bare macros), so it cannot be normalized at all.
    OpaquePredicate,
    /// A `#elif`/`#else`/`#endif` with no open `#if`.
    StrayDirective,
    /// An `#if` chain with no matching `#endif`.
    UnterminatedIf,
}

impl DiagnosticKind {
    pub fn as_str(self) -> &'static str {
        match self {
            DiagnosticKind::UnmappedMacro => "unmapped-macro",
            DiagnosticKind::OpaquePredicate => "opaque-predicate",
            DiagnosticKind::StrayDirective => "stray-directive",
            DiagnosticKind::UnterminatedIf => "unterminated-if",
        }
    }
}

/// A recorded reason a conditional region could not be mapped, with enough
/// source context (line, and for predicate cases the predicate text) for a user
/// to add a config-matrix entry.
#[derive(Debug, Clone)]
pub struct Diagnostic {
    pub kind: DiagnosticKind,
    /// 1-based source line of the offending directive.
    pub line: usize,
    pub message: String,
}

/// A normalized preprocessor condition, restricted to the boolean-over-`defined`
/// shape used by target/debug gates. Anything richer is [`PredExpr::Opaque`].
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum PredExpr {
    Defined(String),
    Not(Box<PredExpr>),
    And(Vec<PredExpr>),
    Or(Vec<PredExpr>),
    Opaque(String),
}

/// One branch of a conditional chain.
#[derive(Debug, Clone)]
pub struct Branch {
    pub kind: DirectiveKind,
    /// Directive argument text (`None` for `#else`).
    pub raw_predicate: Option<String>,
    /// Normalized predicate. For `#else` this is the negation of every prior
    /// branch predicate in the chain.
    pub predicate: PredExpr,
    /// 1-based line of the branch directive itself.
    pub directive_line: usize,
    /// 1-based first line of the guarded body (may exceed `body_end` if empty).
    pub body_start: usize,
    /// 1-based last line of the guarded body.
    pub body_end: usize,
    /// Mapped Rust `cfg(...)` predicate, or `None` when the predicate is opaque
    /// or references an unknown macro.
    pub rust_cfg: Option<Cfg>,
    /// Whether this branch is active for the queried macro environment.
    /// `None` when the predicate could not be evaluated.
    pub active: Option<bool>,
}

/// A full `#if ... #endif` group at one nesting depth.
#[derive(Debug, Clone)]
pub struct CondChain {
    pub depth: usize,
    pub open_line: usize,
    pub endif_line: usize,
    pub branches: Vec<Branch>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum DirectiveName {
    Null,
    If,
    Ifdef,
    Ifndef,
    Elif,
    Else,
    Endif,
    Define,
    Undef,
    Include,
    IncludeNext,
    Import,
    Line,
    Error,
    Warning,
    Pragma,
    Embed,
    Ident,
    Sccs,
    Unknown(String),
}

impl DirectiveName {
    fn parse(name: String) -> Self {
        match name.as_str() {
            "" => Self::Null,
            "if" => Self::If,
            "ifdef" => Self::Ifdef,
            "ifndef" => Self::Ifndef,
            "elif" => Self::Elif,
            "else" => Self::Else,
            "endif" => Self::Endif,
            "define" => Self::Define,
            "undef" => Self::Undef,
            "include" => Self::Include,
            "include_next" => Self::IncludeNext,
            "import" => Self::Import,
            "line" => Self::Line,
            "error" => Self::Error,
            "warning" => Self::Warning,
            "pragma" => Self::Pragma,
            "embed" => Self::Embed,
            "ident" => Self::Ident,
            "sccs" => Self::Sccs,
            _ => Self::Unknown(name),
        }
    }

    pub fn as_str(&self) -> &str {
        match self {
            Self::Null => "",
            Self::If => "if",
            Self::Ifdef => "ifdef",
            Self::Ifndef => "ifndef",
            Self::Elif => "elif",
            Self::Else => "else",
            Self::Endif => "endif",
            Self::Define => "define",
            Self::Undef => "undef",
            Self::Include => "include",
            Self::IncludeNext => "include_next",
            Self::Import => "import",
            Self::Line => "line",
            Self::Error => "error",
            Self::Warning => "warning",
            Self::Pragma => "pragma",
            Self::Embed => "embed",
            Self::Ident => "ident",
            Self::Sccs => "sccs",
            Self::Unknown(name) => name,
        }
    }

    pub fn disposition(&self) -> DirectiveDisposition {
        match self {
            Self::If
            | Self::Ifdef
            | Self::Ifndef
            | Self::Elif
            | Self::Else
            | Self::Endif
            | Self::Error => DirectiveDisposition::RepresentedInRust,
            Self::Define
            | Self::Undef
            | Self::Include
            | Self::IncludeNext
            | Self::Import
            | Self::Line
            | Self::Embed => DirectiveDisposition::ConsumedByClang,
            Self::Warning => DirectiveDisposition::DiagnosticOnly,
            Self::Null | Self::Ident | Self::Sccs => DirectiveDisposition::NoOutput,
            Self::Pragma | Self::Unknown(_) => DirectiveDisposition::UnsupportedSemantic,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum DirectiveDisposition {
    RepresentedInRust,
    ConsumedByClang,
    DiagnosticOnly,
    NoOutput,
    UnsupportedSemantic,
}

impl DirectiveDisposition {
    pub fn as_str(self) -> &'static str {
        match self {
            Self::RepresentedInRust => "represented-in-rust",
            Self::ConsumedByClang => "consumed-by-clang",
            Self::DiagnosticOnly => "diagnostic-only",
            Self::NoOutput => "no-output",
            Self::UnsupportedSemantic => "unsupported-semantic",
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct DirectiveRecord {
    pub name: DirectiveName,
    pub raw_payload: String,
    pub byte_start: usize,
    pub byte_end: usize,
    pub line_start: usize,
    pub line_end: usize,
    pub depth: usize,
    pub condition: Option<PredExpr>,
    pub active: Option<bool>,
}

impl DirectiveRecord {
    pub fn disposition(&self) -> DirectiveDisposition {
        if self.name == DirectiveName::Pragma && is_diagnostic_pragma(&self.raw_payload) {
            DirectiveDisposition::DiagnosticOnly
        } else {
            self.name.disposition()
        }
    }

    pub fn unsupported_message(&self) -> String {
        let mut message = format!(
            "unsupported semantic directive #{} at line {}",
            self.name.as_str(),
            self.line_start
        );
        if !self.raw_payload.is_empty() {
            message.push_str(": ");
            message.push_str(&self.raw_payload);
        }
        message
    }
}

fn is_diagnostic_pragma(payload: &str) -> bool {
    let payload = payload.trim_start();
    payload.starts_with("GCC diagnostic ")
        || payload.starts_with("clang diagnostic ")
        || payload.starts_with("warning(")
}

/// Recorded preprocessing metadata for a translation unit.
#[derive(Debug, Clone, Default)]
pub struct Preprocessing {
    pub chains: Vec<CondChain>,
    pub directives: Vec<DirectiveRecord>,
    pub diagnostics: Vec<Diagnostic>,
}

pub struct ClangInput {
    extra_args: Vec<String>,
    temp_dir: Option<PathBuf>,
}

impl ClangInput {
    pub fn extra_args(&self) -> &[String] {
        &self.extra_args
    }
}

impl Drop for ClangInput {
    fn drop(&mut self) {
        if let Some(path) = &self.temp_dir {
            let _ = std::fs::remove_dir_all(path);
        }
    }
}

pub fn clang_input(
    path: &Path,
    source: &str,
    directives: &[&DirectiveRecord],
) -> Result<ClangInput, String> {
    if directives.is_empty() {
        return Ok(ClangInput {
            extra_args: Vec::new(),
            temp_dir: None,
        });
    }

    let mut bytes = source.as_bytes().to_vec();
    let source_len = bytes.len();
    for directive in directives {
        for byte in bytes
            .get_mut(directive.byte_start..directive.byte_end)
            .ok_or_else(|| {
                format!(
                    "directive span {}..{} is outside {} bytes of source",
                    directive.byte_start, directive.byte_end, source_len
                )
            })?
        {
            if *byte != b'\n' && *byte != b'\r' {
                *byte = b' ';
            }
        }
    }

    let temp_dir = create_sanitized_temp_dir()?;
    let mut input = ClangInput {
        extra_args: Vec::new(),
        temp_dir: Some(temp_dir.clone()),
    };
    let sanitized_path = temp_dir.join("source.c");
    let overlay_path = temp_dir.join("overlay.json");
    std::fs::write(&sanitized_path, bytes)
        .map_err(|e| format!("write {}: {e}", sanitized_path.display()))?;
    let virtual_path = if path.is_absolute() {
        path.to_path_buf()
    } else {
        std::env::current_dir()
            .map_err(|e| format!("get current directory: {e}"))?
            .join(path)
    };
    let overlay = serde_json::json!({
        "version": 0,
        "use-external-names": false,
        "roots": [{
            "type": "file",
            "name": virtual_path,
            "external-contents": sanitized_path,
        }],
    });
    std::fs::write(
        &overlay_path,
        serde_json::to_vec(&overlay).map_err(|e| format!("encode VFS overlay: {e}"))?,
    )
    .map_err(|e| format!("write {}: {e}", overlay_path.display()))?;

    input.extra_args = vec![
        "-ivfsoverlay".to_string(),
        overlay_path.to_string_lossy().into_owned(),
    ];
    Ok(input)
}

fn create_sanitized_temp_dir() -> Result<PathBuf, String> {
    for _ in 0..100 {
        let id = NEXT_SANITIZED_INPUT.fetch_add(1, Ordering::Relaxed);
        let path =
            std::env::temp_dir().join(format!("slate-sanitized-{}-{id}", std::process::id()));
        match std::fs::create_dir(&path) {
            Ok(()) => return Ok(path),
            Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => {}
            Err(error) => return Err(format!("create {}: {error}", path.display())),
        }
    }
    Err("could not allocate a temporary directory for sanitized Clang input".into())
}

/// Scan `source` for conditional regions and resolve active branches against
/// `macros` (the predefined macro environment of the intended invocation).
pub fn record(source: &str, macros: &BTreeMap<String, String>) -> Preprocessing {
    let mut pp = scan(source);
    for chain in &mut pp.chains {
        resolve_active(chain, macros);
    }
    for directive in &mut pp.directives {
        directive.active = match &directive.condition {
            Some(condition) => eval(condition, macros),
            None => Some(true),
        };
    }
    for chain in &pp.chains {
        for branch in &chain.branches {
            if let Some(diag) = classify_branch(branch) {
                pp.diagnostics.push(diag);
            }
        }
    }
    pp
}

/// Diagnose a branch whose predicate does not map to a Rust `cfg`. Returns
/// `None` for a cleanly mapped branch. An inactive unmapped branch is flagged as
/// *uncovered* — it will silently vanish from the output rather than being gated.
fn classify_branch(branch: &Branch) -> Option<Diagnostic> {
    if branch.rust_cfg.is_some() {
        return None;
    }
    let raw = branch.raw_predicate.as_deref().unwrap_or("(else)");
    let uncovered = branch.active != Some(true);
    if has_opaque(&branch.predicate) {
        return Some(Diagnostic {
            kind: DiagnosticKind::OpaquePredicate,
            line: branch.directive_line,
            message: format!(
                "line {}: #{} predicate `{}` is not a boolean-over-defined() gate; \
                 cannot be normalized to a Rust cfg{}",
                branch.directive_line,
                branch.kind.as_str(),
                raw,
                coverage_note(uncovered),
            ),
        });
    }
    let unknown = unmapped_atoms(&branch.predicate);
    Some(Diagnostic {
        kind: DiagnosticKind::UnmappedMacro,
        line: branch.directive_line,
        message: format!(
            "line {}: #{} predicate `{}` references macro(s) {} with no known target/debug \
             cfg mapping{}",
            branch.directive_line,
            branch.kind.as_str(),
            raw,
            unknown.join(", "),
            coverage_note(uncovered),
        ),
    })
}

fn coverage_note(uncovered: bool) -> &'static str {
    if uncovered {
        "; branch left uncovered (supply a config-matrix entry or cfg mapping)"
    } else {
        ""
    }
}

/// True when any leaf of `expr` is an [`PredExpr::Opaque`] shape.
fn has_opaque(expr: &PredExpr) -> bool {
    match expr {
        PredExpr::Opaque(_) => true,
        PredExpr::Defined(_) => false,
        PredExpr::Not(inner) => has_opaque(inner),
        PredExpr::And(items) | PredExpr::Or(items) => items.iter().any(has_opaque),
    }
}

/// The `defined(MACRO)` atoms of `expr` that have no known cfg mapping, in
/// source order and de-duplicated.
fn unmapped_atoms(expr: &PredExpr) -> Vec<String> {
    let mut out = Vec::new();
    collect_unmapped(expr, &mut out);
    out
}

fn collect_unmapped(expr: &PredExpr, out: &mut Vec<String>) {
    match expr {
        PredExpr::Defined(name) => {
            if macro_cfg(name).is_none() && !out.contains(name) {
                out.push(name.clone());
            }
        }
        PredExpr::Not(inner) => collect_unmapped(inner, out),
        PredExpr::And(items) | PredExpr::Or(items) => {
            for item in items {
                collect_unmapped(item, out);
            }
        }
        PredExpr::Opaque(_) => {}
    }
}

/// Convenience: query Clang's predefined macros for `clang_args` and record.
pub fn record_file(source: &str, clang_args: &[String]) -> Result<Preprocessing, String> {
    let macros = crate::cir::emit::predefined_macros(clang_args)?;
    Ok(record(source, &macros))
}

struct ChainBuilder {
    depth: usize,
    open_line: usize,
    branches: Vec<Branch>,
}

struct LogicalLine {
    text: String,
    offsets: Vec<usize>,
    byte_end: usize,
    line_start: usize,
    line_end: usize,
}

struct SourceDirective {
    name: String,
    raw_payload: String,
    payload: String,
    byte_start: usize,
}

fn scan(source: &str) -> Preprocessing {
    let mut stack: Vec<ChainBuilder> = Vec::new();
    let mut chains: Vec<CondChain> = Vec::new();
    let mut directives = Vec::new();
    let mut diagnostics: Vec<Diagnostic> = Vec::new();
    let mut in_block_comment = false;

    for line in logical_lines(source) {
        let scrubbed = scrub_comments(&line.text, &mut in_block_comment);
        let Some(source_directive) = source_directive(&line, &scrubbed) else {
            continue;
        };
        let parsed = conditional_directive(&source_directive.name, &source_directive.payload);
        let (condition, depth) = directive_context(&stack, parsed.as_ref());
        directives.push(DirectiveRecord {
            name: DirectiveName::parse(source_directive.name.clone()),
            raw_payload: source_directive.raw_payload.clone(),
            byte_start: source_directive.byte_start,
            byte_end: line.byte_end,
            line_start: line.line_start,
            line_end: line.line_end,
            depth,
            condition,
            active: None,
        });
        let Some((kind, arg)) = parsed else {
            continue;
        };
        match kind {
            Directive::Open(kind) => {
                let predicate = open_predicate(kind, arg);
                let branch = Branch {
                    kind,
                    raw_predicate: arg.map(str::to_string),
                    rust_cfg: pred_to_cfg(&predicate),
                    predicate,
                    directive_line: line.line_start,
                    body_start: line.line_end + 1,
                    body_end: line.line_end,
                    active: None,
                };
                stack.push(ChainBuilder {
                    depth: stack.len(),
                    open_line: line.line_start,
                    branches: vec![branch],
                });
            }
            Directive::Cont(kind) => {
                let Some(top) = stack.last_mut() else {
                    diagnostics.push(Diagnostic {
                        kind: DiagnosticKind::StrayDirective,
                        line: line.line_start,
                        message: format!(
                            "line {}: #{} without #if",
                            line.line_start,
                            kind.as_str()
                        ),
                    });
                    continue;
                };
                close_body(top, line.line_start);
                let predicate = match kind {
                    DirectiveKind::Else => Box::new(PredExpr::Not(Box::new(or_of_priors(top)))),
                    _ => Box::new(open_predicate(kind, arg)),
                };
                let predicate = *predicate;
                top.branches.push(Branch {
                    kind,
                    raw_predicate: arg.map(str::to_string),
                    rust_cfg: pred_to_cfg(&predicate),
                    predicate,
                    directive_line: line.line_start,
                    body_start: line.line_end + 1,
                    body_end: line.line_end,
                    active: None,
                });
            }
            Directive::Endif => {
                let Some(mut top) = stack.pop() else {
                    diagnostics.push(Diagnostic {
                        kind: DiagnosticKind::StrayDirective,
                        line: line.line_start,
                        message: format!("line {}: #endif without #if", line.line_start),
                    });
                    continue;
                };
                close_body(&mut top, line.line_start);
                chains.push(CondChain {
                    depth: top.depth,
                    open_line: top.open_line,
                    endif_line: line.line_end,
                    branches: top.branches,
                });
            }
        }
    }

    for leftover in &stack {
        diagnostics.push(Diagnostic {
            kind: DiagnosticKind::UnterminatedIf,
            line: leftover.open_line,
            message: format!(
                "line {}: unterminated #if (missing #endif)",
                leftover.open_line
            ),
        });
    }
    chains.sort_by_key(|c| c.open_line);
    Preprocessing {
        chains,
        directives,
        diagnostics,
    }
}

fn logical_lines(source: &str) -> Vec<LogicalLine> {
    let bytes = source.as_bytes();
    let mut lines = Vec::new();
    let mut offset = 0;
    let mut line_number = 1;

    while offset < bytes.len() {
        let line_start = line_number;
        let mut text = Vec::new();
        let mut offsets = Vec::new();

        let (byte_end, line_end) = loop {
            let physical_start = offset;
            let newline = bytes[offset..]
                .iter()
                .position(|byte| *byte == b'\n')
                .map(|index| offset + index);
            let physical_end = newline.unwrap_or(bytes.len());
            let content_end = if physical_end > physical_start && bytes[physical_end - 1] == b'\r' {
                physical_end - 1
            } else {
                physical_end
            };
            let continued = newline.is_some()
                && content_end > physical_start
                && bytes[content_end - 1] == b'\\';
            let append_end = if continued {
                content_end - 1
            } else {
                content_end
            };
            text.extend_from_slice(&bytes[physical_start..append_end]);
            offsets.extend(physical_start..append_end);
            offset = newline.map_or(bytes.len(), |index| index + 1);
            if newline.is_some() {
                line_number += 1;
            }
            if !continued || offset == bytes.len() {
                break (content_end, line_number - usize::from(newline.is_some()));
            }
        };

        lines.push(LogicalLine {
            text: String::from_utf8(text).expect("logical line came from UTF-8 source"),
            offsets,
            byte_end,
            line_start,
            line_end,
        });
    }
    lines
}

fn scrub_comments(line: &str, in_block: &mut bool) -> String {
    let mut bytes = line.as_bytes().to_vec();
    let mut quote = None;
    let mut index = 0;
    while index < bytes.len() {
        if *in_block {
            if bytes[index] == b'*' && bytes.get(index + 1) == Some(&b'/') {
                bytes[index] = b' ';
                bytes[index + 1] = b' ';
                *in_block = false;
                index += 2;
            } else {
                bytes[index] = b' ';
                index += 1;
            }
            continue;
        }
        if let Some(delimiter) = quote {
            if bytes[index] == b'\\' {
                index += 2;
            } else {
                if bytes[index] == delimiter {
                    quote = None;
                }
                index += 1;
            }
            continue;
        }
        match bytes[index] {
            b'\'' | b'"' => {
                quote = Some(bytes[index]);
                index += 1;
            }
            b'/' if bytes.get(index + 1) == Some(&b'/') => {
                bytes[index..].fill(b' ');
                break;
            }
            b'/' if bytes.get(index + 1) == Some(&b'*') => {
                bytes[index] = b' ';
                bytes[index + 1] = b' ';
                *in_block = true;
                index += 2;
            }
            _ => index += 1,
        }
    }
    String::from_utf8(bytes).expect("comment scrubbing preserves UTF-8")
}

fn source_directive(line: &LogicalLine, scrubbed: &str) -> Option<SourceDirective> {
    let bytes = scrubbed.as_bytes();
    let mut index = 0;
    while bytes.get(index).is_some_and(u8::is_ascii_whitespace) {
        index += 1;
    }
    if bytes.get(index) != Some(&b'#') {
        return None;
    }
    let hash_index = index;
    index += 1;
    while bytes.get(index).is_some_and(u8::is_ascii_whitespace) {
        index += 1;
    }
    let name_start = index;
    while bytes
        .get(index)
        .is_some_and(|byte| byte.is_ascii_alphanumeric() || *byte == b'_')
    {
        index += 1;
    }
    let name = scrubbed[name_start..index].to_string();
    let raw_payload = line.text[index..].trim().to_string();
    let payload = scrubbed[index..].trim().to_string();
    Some(SourceDirective {
        name,
        raw_payload,
        payload,
        byte_start: line.offsets[hash_index],
    })
}

fn directive_context(
    stack: &[ChainBuilder],
    directive: Option<&(Directive, Option<&str>)>,
) -> (Option<PredExpr>, usize) {
    let (count, branch) = match directive {
        Some((Directive::Open(kind), arg)) => (stack.len(), Some(open_predicate(*kind, *arg))),
        Some((Directive::Cont(kind), arg)) => (
            stack.len().saturating_sub(1),
            stack
                .last()
                .map(|chain| continuation_condition(chain, *kind, *arg)),
        ),
        Some((Directive::Endif, _)) => (stack.len().saturating_sub(1), None),
        None => (stack.len(), None),
    };
    let mut conditions: Vec<_> = stack[..count]
        .iter()
        .map(selected_branch_condition)
        .collect();
    if let Some(branch) = branch {
        conditions.push(branch);
    }
    (and_condition(conditions), count)
}

fn continuation_condition(
    chain: &ChainBuilder,
    kind: DirectiveKind,
    arg: Option<&str>,
) -> PredExpr {
    let prior = or_of_priors(chain);
    let no_prior = PredExpr::Not(Box::new(prior));
    if kind == DirectiveKind::Else {
        no_prior
    } else {
        PredExpr::And(vec![no_prior, open_predicate(kind, arg)])
    }
}

fn selected_branch_condition(chain: &ChainBuilder) -> PredExpr {
    let current = chain.branches.last().expect("chain has a branch");
    if chain.branches.len() == 1 {
        return current.predicate.clone();
    }
    let prior = chain.branches[..chain.branches.len() - 1]
        .iter()
        .map(|branch| branch.predicate.clone())
        .collect::<Vec<_>>();
    let prior = match prior.len() {
        1 => prior.into_iter().next().unwrap(),
        _ => PredExpr::Or(prior),
    };
    let no_prior = PredExpr::Not(Box::new(prior));
    if current.kind == DirectiveKind::Else {
        no_prior
    } else {
        PredExpr::And(vec![no_prior, current.predicate.clone()])
    }
}

fn and_condition(mut conditions: Vec<PredExpr>) -> Option<PredExpr> {
    match conditions.len() {
        0 => None,
        1 => conditions.pop(),
        _ => Some(PredExpr::And(conditions)),
    }
}

fn close_body(builder: &mut ChainBuilder, next_directive_line: usize) {
    if let Some(branch) = builder.branches.last_mut() {
        branch.body_end = next_directive_line.saturating_sub(1);
    }
}

fn or_of_priors(builder: &ChainBuilder) -> PredExpr {
    let priors: Vec<PredExpr> = builder
        .branches
        .iter()
        .map(|b| b.predicate.clone())
        .collect();
    match priors.len() {
        1 => priors.into_iter().next().unwrap(),
        _ => PredExpr::Or(priors),
    }
}

#[derive(Debug)]
enum Directive {
    Open(DirectiveKind),
    Cont(DirectiveKind),
    Endif,
}

fn conditional_directive<'a>(name: &str, payload: &'a str) -> Option<(Directive, Option<&'a str>)> {
    let arg = if payload.is_empty() {
        None
    } else {
        Some(payload)
    };
    let dir = match name {
        "if" => Directive::Open(DirectiveKind::If),
        "ifdef" => Directive::Open(DirectiveKind::Ifdef),
        "ifndef" => Directive::Open(DirectiveKind::Ifndef),
        "elif" => Directive::Cont(DirectiveKind::Elif),
        "else" => Directive::Cont(DirectiveKind::Else),
        "endif" => Directive::Endif,
        _ => return None,
    };
    Some((dir, arg))
}

fn open_predicate(kind: DirectiveKind, arg: Option<&str>) -> PredExpr {
    let arg = arg.unwrap_or("");
    match kind {
        DirectiveKind::Ifdef => PredExpr::Defined(arg.to_string()),
        DirectiveKind::Ifndef => PredExpr::Not(Box::new(PredExpr::Defined(arg.to_string()))),
        _ => parse_predicate(arg),
    }
}

fn resolve_active(chain: &mut CondChain, macros: &BTreeMap<String, String>) {
    let mut decided = false;
    let mut uncertain = false;
    for branch in &mut chain.branches {
        let active = if decided {
            Some(false)
        } else if uncertain {
            None
        } else {
            eval(&branch.predicate, macros)
        };
        match active {
            Some(true) => decided = true,
            Some(false) => {}
            None => uncertain = true,
        }
        branch.active = active;
    }
}

fn eval(expr: &PredExpr, macros: &BTreeMap<String, String>) -> Option<bool> {
    match expr {
        PredExpr::Defined(name) => Some(macros.contains_key(name)),
        PredExpr::Not(inner) => eval(inner, macros).map(|b| !b),
        PredExpr::And(items) => {
            let mut unknown = false;
            for item in items {
                match eval(item, macros) {
                    Some(false) => return Some(false),
                    Some(true) => {}
                    None => unknown = true,
                }
            }
            (!unknown).then_some(true)
        }
        PredExpr::Or(items) => {
            let mut unknown = false;
            for item in items {
                match eval(item, macros) {
                    Some(true) => return Some(true),
                    Some(false) => {}
                    None => unknown = true,
                }
            }
            (!unknown).then_some(false)
        }
        PredExpr::Opaque(_) => None,
    }
}

fn opt(key: impl Into<String>, value: impl Into<String>) -> Cfg {
    Cfg::Opt {
        key: key.into(),
        value: value.into(),
    }
}

fn known_cfg(macro_name: &str) -> Option<Cfg> {
    Some(match macro_name {
        "_WIN64" => Cfg::All(vec![
            Cfg::Flag("windows".into()),
            opt("target_pointer_width", "64"),
        ]),
        "_WIN32" => Cfg::Flag("windows".into()),
        "__linux__" | "__linux" | "linux" => opt("target_os", "linux"),
        "__ANDROID__" => opt("target_os", "android"),
        "__FreeBSD__" => opt("target_os", "freebsd"),
        "__unix__" | "__unix" => Cfg::Flag("unix".into()),
        "__APPLE__" => opt("target_vendor", "apple"),
        "__x86_64__" | "_M_X64" => opt("target_arch", "x86_64"),
        "__i386__" | "_M_IX86" => opt("target_arch", "x86"),
        "__aarch64__" | "_M_ARM64" => opt("target_arch", "aarch64"),
        "__arm__" | "_M_ARM" => opt("target_arch", "arm"),
        "__powerpc64__" | "__PPC64__" => opt("target_arch", "powerpc64"),
        "__powerpc__" | "__POWERPC__" | "_M_PPC" => opt("target_arch", "powerpc"),
        "__wasm64__" => opt("target_arch", "wasm64"),
        "__wasm32__" => opt("target_arch", "wasm32"),
        "_M_RISCV64" => opt("target_arch", "riscv64"),
        "_M_RISCV32" => opt("target_arch", "riscv32"),
        "__LP64__" | "_LP64" => opt("target_pointer_width", "64"),
        "__ILP32__" | "_ILP32" => opt("target_pointer_width", "32"),
        "__ARMEB__" => Cfg::All(vec![opt("target_arch", "arm"), opt("target_endian", "big")]),
        "__ARMEL__" => Cfg::All(vec![
            opt("target_arch", "arm"),
            opt("target_endian", "little"),
        ]),
        "__AARCH64EB__" => Cfg::All(vec![
            opt("target_arch", "aarch64"),
            opt("target_endian", "big"),
        ]),
        "__AARCH64EL__" => Cfg::All(vec![
            opt("target_arch", "aarch64"),
            opt("target_endian", "little"),
        ]),
        "NDEBUG" => Cfg::Not(Box::new(Cfg::Flag("debug_assertions".into()))),
        _ => return None,
    })
}

fn feature_cfg(macro_name: &str) -> Option<Cfg> {
    if macro_name.starts_with('_') {
        return None;
    }
    let mut feature = String::new();
    let mut prev_underscore = false;
    for ch in macro_name.chars() {
        if ch.is_ascii_alphanumeric() {
            feature.push(ch.to_ascii_lowercase());
            prev_underscore = false;
        } else if !prev_underscore {
            feature.push('_');
            prev_underscore = true;
        }
    }
    let feature = feature.trim_matches('_');
    if feature.is_empty() {
        None
    } else {
        Some(opt("feature", feature))
    }
}

fn macro_cfg(macro_name: &str) -> Option<Cfg> {
    known_cfg(macro_name).or_else(|| feature_cfg(macro_name))
}

pub(crate) fn pred_to_cfg(expr: &PredExpr) -> Option<Cfg> {
    match expr {
        PredExpr::Defined(name) => macro_cfg(name),
        PredExpr::Opaque(_) => None,
        PredExpr::Not(inner) => pred_to_cfg(inner).map(negate_cfg),
        PredExpr::Or(items) => combine_cfg(items, CfgList::Any),
        PredExpr::And(items) => combine_cfg(items, CfgList::All),
    }
}

enum CfgList {
    Any,
    All,
}

fn combine_cfg(items: &[PredExpr], keyword: CfgList) -> Option<Cfg> {
    let mut atoms: Vec<Cfg> = Vec::new();
    for item in items {
        let cfg = pred_to_cfg(item)?;
        if !atoms.contains(&cfg) {
            atoms.push(cfg);
        }
    }
    match atoms.len() {
        0 => None,
        1 => Some(atoms.remove(0)),
        _ => Some(match keyword {
            CfgList::Any => Cfg::Any(atoms),
            CfgList::All => Cfg::All(atoms),
        }),
    }
}

fn negate_cfg(cfg: Cfg) -> Cfg {
    match cfg {
        Cfg::Not(inner) => *inner,
        other => Cfg::Not(Box::new(other)),
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
enum Tok {
    Ident(String),
    LParen,
    RParen,
    Bang,
    And,
    Or,
}

fn tokenize(s: &str) -> Option<Vec<Tok>> {
    let bytes = s.as_bytes();
    let mut toks = Vec::new();
    let mut i = 0;
    while i < bytes.len() {
        let b = bytes[i];
        match b {
            b' ' | b'\t' => i += 1,
            b'(' => {
                toks.push(Tok::LParen);
                i += 1;
            }
            b')' => {
                toks.push(Tok::RParen);
                i += 1;
            }
            b'!' => {
                toks.push(Tok::Bang);
                i += 1;
            }
            b'&' if bytes.get(i + 1) == Some(&b'&') => {
                toks.push(Tok::And);
                i += 2;
            }
            b'|' if bytes.get(i + 1) == Some(&b'|') => {
                toks.push(Tok::Or);
                i += 2;
            }
            b'_' | b'a'..=b'z' | b'A'..=b'Z' => {
                let start = i;
                while i < bytes.len() && (bytes[i] == b'_' || bytes[i].is_ascii_alphanumeric()) {
                    i += 1;
                }
                toks.push(Tok::Ident(s[start..i].to_string()));
            }
            // anything else (digits, comparisons, arithmetic) is out of scope.
            _ => return None,
        }
    }
    Some(toks)
}

struct Parser {
    toks: Vec<Tok>,
    pos: usize,
}

impl Parser {
    fn peek(&self) -> Option<&Tok> {
        self.toks.get(self.pos)
    }

    fn bump(&mut self) -> Option<Tok> {
        let tok = self.toks.get(self.pos).cloned();
        if tok.is_some() {
            self.pos += 1;
        }
        tok
    }

    fn parse_or(&mut self) -> Option<PredExpr> {
        let mut items = vec![self.parse_and()?];
        while self.peek() == Some(&Tok::Or) {
            self.bump();
            items.push(self.parse_and()?);
        }
        Some(if items.len() == 1 {
            items.pop().unwrap()
        } else {
            PredExpr::Or(items)
        })
    }

    fn parse_and(&mut self) -> Option<PredExpr> {
        let mut items = vec![self.parse_unary()?];
        while self.peek() == Some(&Tok::And) {
            self.bump();
            items.push(self.parse_unary()?);
        }
        Some(if items.len() == 1 {
            items.pop().unwrap()
        } else {
            PredExpr::And(items)
        })
    }

    fn parse_unary(&mut self) -> Option<PredExpr> {
        if self.peek() == Some(&Tok::Bang) {
            self.bump();
            return Some(PredExpr::Not(Box::new(self.parse_unary()?)));
        }
        self.parse_primary()
    }

    fn parse_primary(&mut self) -> Option<PredExpr> {
        match self.bump()? {
            Tok::LParen => {
                let inner = self.parse_or()?;
                match self.bump()? {
                    Tok::RParen => Some(inner),
                    _ => None,
                }
            }
            Tok::Ident(name) if name == "defined" => {
                if self.peek() == Some(&Tok::LParen) {
                    self.bump();
                    let name = match self.bump()? {
                        Tok::Ident(n) => n,
                        _ => return None,
                    };
                    match self.bump()? {
                        Tok::RParen => Some(PredExpr::Defined(name)),
                        _ => None,
                    }
                } else {
                    match self.bump()? {
                        Tok::Ident(n) => Some(PredExpr::Defined(n)),
                        _ => None,
                    }
                }
            }
            // a bare macro or literal on its own is out of the recorded subset.
            _ => None,
        }
    }
}

fn parse_predicate(raw: &str) -> PredExpr {
    let opaque = || PredExpr::Opaque(raw.trim().to_string());
    let Some(toks) = tokenize(raw) else {
        return opaque();
    };
    let mut parser = Parser { toks, pos: 0 };
    match parser.parse_or() {
        Some(expr) if parser.pos == parser.toks.len() => expr,
        _ => opaque(),
    }
}

pub fn predicate_text(expr: &PredExpr) -> String {
    match expr {
        PredExpr::Defined(name) => format!("defined({name})"),
        PredExpr::Not(inner) => format!("!({})", predicate_text(inner)),
        PredExpr::And(items) => items
            .iter()
            .map(|item| format!("({})", predicate_text(item)))
            .collect::<Vec<_>>()
            .join(" && "),
        PredExpr::Or(items) => items
            .iter()
            .map(|item| format!("({})", predicate_text(item)))
            .collect::<Vec<_>>()
            .join(" || "),
        PredExpr::Opaque(raw) => raw.clone(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn macros(defined: &[&str]) -> BTreeMap<String, String> {
        defined
            .iter()
            .map(|m| (m.to_string(), "1".to_string()))
            .collect()
    }

    fn cfg_str(cfg: &Option<Cfg>) -> Option<String> {
        cfg.as_ref().map(Cfg::render)
    }

    #[test]
    fn records_directives_with_payload_spans_depth_and_activity() {
        let src = "  #define TOP 1\n#ifdef ENABLED\n#error disabled\n#endif\n";
        let pp = record(src, &macros(&[]));

        assert_eq!(pp.directives.len(), 4);
        let define = &pp.directives[0];
        assert_eq!(define.name, DirectiveName::Define);
        assert_eq!(define.raw_payload, "TOP 1");
        assert_eq!(define.byte_start, 2);
        assert_eq!(define.byte_end, 15);
        assert_eq!((define.line_start, define.line_end), (1, 1));
        assert_eq!(define.depth, 0);
        assert_eq!(define.condition, None);
        assert_eq!(define.active, Some(true));

        let error = &pp.directives[2];
        assert_eq!(error.name, DirectiveName::Error);
        assert_eq!(error.raw_payload, "disabled");
        assert_eq!(error.depth, 1);
        assert_eq!(error.condition, Some(PredExpr::Defined("ENABLED".into())));
        assert_eq!(error.active, Some(false));
    }

    #[test]
    fn splices_logical_directive_lines_and_preserves_physical_ranges() {
        let src = "#if defined(A) \\\n || defined(B)\n#error nope\n#endif\n";
        let pp = record(src, &macros(&["B"]));

        let opening = &pp.directives[0];
        assert_eq!(opening.name, DirectiveName::If);
        assert_eq!(opening.raw_payload, "defined(A)  || defined(B)");
        assert_eq!((opening.line_start, opening.line_end), (1, 2));
        assert_eq!(opening.byte_start, 0);
        assert_eq!(opening.byte_end, 31);
        assert_eq!(pp.chains[0].branches[0].body_start, 3);
        assert_eq!(pp.chains[0].branches[0].active, Some(true));

        let error = &pp.directives[1];
        assert_eq!(
            error.condition,
            Some(parse_predicate("defined(A) || defined(B)"))
        );
        assert_eq!(error.active, Some(true));
    }

    #[test]
    fn recognizes_directives_after_comments_without_matching_strings() {
        let src = "const char *s = \"#error not a directive\";\n/* lead */ #ifdef A // tail\n#error actual\n#endif\n";
        let pp = record(src, &macros(&["A"]));

        assert_eq!(
            pp.directives
                .iter()
                .map(|directive| directive.name.as_str())
                .collect::<Vec<_>>(),
            vec!["ifdef", "error", "endif"]
        );
        assert_eq!(pp.directives[0].raw_payload, "A // tail");
        assert_eq!(pp.chains[0].branches[0].raw_predicate.as_deref(), Some("A"));
    }

    #[test]
    fn records_nested_effective_conditions() {
        let src = "#ifdef A\n#define X 1\n#ifdef B\n#error nested\n#endif\n#endif\n";
        let pp = record(src, &macros(&["A"]));
        let error = &pp.directives[3];

        assert_eq!(
            error.condition,
            Some(PredExpr::And(vec![
                PredExpr::Defined("A".into()),
                PredExpr::Defined("B".into()),
            ]))
        );
        assert_eq!(error.depth, 2);
        assert_eq!(error.active, Some(false));
    }

    #[test]
    fn records_else_body_as_effective_negated_condition() {
        let src = "#ifdef A\n#else\n#error fallback\n#endif\n";
        let pp = record(src, &macros(&[]));
        let error = &pp.directives[2];

        assert_eq!(
            error.condition,
            Some(PredExpr::Not(Box::new(PredExpr::Defined("A".into()))))
        );
        assert_eq!(error.active, Some(true));
    }

    #[test]
    fn elif_activity_includes_prior_branch_negation() {
        let src = "#if defined(A)\n#error first\n#elif defined(B)\n#error second\n#endif\n";
        let pp = record(src, &macros(&["A", "B"]));
        let second = &pp.directives[3];

        assert_eq!(
            second.condition,
            Some(PredExpr::And(vec![
                PredExpr::Not(Box::new(PredExpr::Defined("A".into()))),
                PredExpr::Defined("B".into()),
            ]))
        );
        assert_eq!(second.active, Some(false));
    }

    #[test]
    fn block_comments_can_span_physical_lines_before_a_directive() {
        let src = "/* hidden\ncontinued */ #define V 1\n";
        let pp = record(src, &macros(&[]));

        assert_eq!(pp.directives.len(), 1);
        assert_eq!(pp.directives[0].name, DirectiveName::Define);
        assert_eq!(
            (pp.directives[0].line_start, pp.directives[0].line_end),
            (2, 2)
        );
    }

    #[test]
    fn records_stray_directives_as_well_as_diagnosing_them() {
        let pp = record("#endif\n", &macros(&[]));

        assert_eq!(pp.directives.len(), 1);
        assert_eq!(pp.directives[0].name, DirectiveName::Endif);
        assert_eq!(pp.directives[0].depth, 0);
        assert_eq!(pp.directives[0].active, Some(true));
        assert_eq!(pp.diagnostics[0].kind, DiagnosticKind::StrayDirective);
    }

    #[test]
    fn inactive_parent_short_circuits_an_unknown_nested_condition() {
        let src = "#ifdef A\n#if VERSION > 3\n#error unreachable\n#endif\n#endif\n";
        let pp = record(src, &macros(&[]));

        assert_eq!(pp.directives[2].active, Some(false));
    }

    #[test]
    fn trailing_backslash_without_newline_is_not_spliced() {
        let pp = record("#error trailing\\", &macros(&[]));

        assert_eq!(pp.directives[0].raw_payload, "trailing\\");
        assert_eq!(pp.directives[0].byte_end, 16);
    }

    #[test]
    fn parses_defined_disjunction() {
        assert_eq!(
            parse_predicate("defined(__x86_64__) || defined(_M_X64)"),
            PredExpr::Or(vec![
                PredExpr::Defined("__x86_64__".into()),
                PredExpr::Defined("_M_X64".into()),
            ])
        );
    }

    #[test]
    fn unknown_predicate_shape_is_opaque() {
        assert_eq!(
            parse_predicate("VERSION > 3"),
            PredExpr::Opaque("VERSION > 3".into())
        );
        assert!(pred_to_cfg(&parse_predicate("VERSION > 3")).is_none());
    }

    #[test]
    fn project_macro_maps_to_feature_cfg() {
        let cfg = pred_to_cfg(&PredExpr::Defined("PROJECT_FOO".into()));
        assert_eq!(cfg_str(&cfg).as_deref(), Some("feature = \"project_foo\""));
    }

    #[test]
    fn unknown_system_macro_has_no_cfg() {
        assert!(pred_to_cfg(&PredExpr::Defined("_FILE_OFFSET_BITS".into())).is_none());
    }

    #[test]
    fn disjunction_of_same_arch_dedups_to_one_cfg() {
        let cfg = pred_to_cfg(&parse_predicate("defined(__x86_64__) || defined(_M_X64)"));
        assert_eq!(cfg_str(&cfg).as_deref(), Some("target_arch = \"x86_64\""));
    }

    #[test]
    fn negated_ndebug_simplifies_double_not() {
        let cfg = pred_to_cfg(&PredExpr::Not(Box::new(PredExpr::Defined("NDEBUG".into()))));
        assert_eq!(cfg_str(&cfg).as_deref(), Some("debug_assertions"));
    }

    #[test]
    fn records_else_as_negation_of_priors_with_ranges() {
        let src = "#if defined(_WIN32)\nW\n#elif defined(__linux__)\nL\n#else\nO\n#endif\n";
        let pp = record(src, &macros(&["__linux__"]));
        assert_eq!(pp.chains.len(), 1);
        let chain = &pp.chains[0];
        assert_eq!(chain.open_line, 1);
        assert_eq!(chain.endif_line, 7);

        let win = &chain.branches[0];
        assert_eq!(cfg_str(&win.rust_cfg).as_deref(), Some("windows"));
        assert_eq!(win.body_start, 2);
        assert_eq!(win.body_end, 2);
        assert_eq!(win.active, Some(false));

        let lin = &chain.branches[1];
        assert_eq!(lin.kind, DirectiveKind::Elif);
        assert_eq!(
            cfg_str(&lin.rust_cfg).as_deref(),
            Some("target_os = \"linux\"")
        );
        assert_eq!(lin.active, Some(true));

        let other = &chain.branches[2];
        assert_eq!(other.kind, DirectiveKind::Else);
        assert_eq!(
            cfg_str(&other.rust_cfg).as_deref(),
            Some("not(any(windows, target_os = \"linux\"))")
        );
        assert_eq!(other.active, Some(false));
    }

    #[test]
    fn nested_chains_are_recorded_at_their_depth() {
        let src = "#ifdef A\n#ifdef B\nX\n#endif\n#endif\n";
        let pp = record(src, &macros(&["A", "B"]));
        assert_eq!(pp.chains.len(), 2);
        // sorted by open line: outer A first, inner B second.
        assert_eq!(pp.chains[0].depth, 0);
        assert_eq!(pp.chains[0].open_line, 1);
        assert_eq!(pp.chains[1].depth, 1);
        assert_eq!(pp.chains[1].open_line, 2);
        assert_eq!(pp.chains[1].branches[0].active, Some(true));
    }

    #[test]
    fn opaque_predicate_yields_undetermined_active_and_diagnostic() {
        let src = "#if FOO > 2\nX\n#endif\n";
        let pp = record(src, &macros(&[]));
        assert_eq!(pp.chains[0].branches[0].active, None);
        let diag = &pp.diagnostics[0];
        assert_eq!(diag.kind, DiagnosticKind::OpaquePredicate);
        assert_eq!(diag.line, 1);
        assert!(diag.message.contains("FOO > 2"));
    }

    #[test]
    fn reserved_macro_is_distinguished_from_opaque_shape() {
        let src = "#if defined(_FILE_OFFSET_BITS)\nX\n#endif\n";
        let pp = record(src, &macros(&[]));
        let diag = &pp.diagnostics[0];
        assert_eq!(diag.kind, DiagnosticKind::UnmappedMacro);
        assert_eq!(diag.line, 1);
        assert!(
            diag.message.contains("_FILE_OFFSET_BITS"),
            "should name the unmapped macro, got: {}",
            diag.message
        );
    }

    #[test]
    fn inactive_unmapped_branch_is_flagged_uncovered() {
        // linux branch is active (mapped); the reserved-macro branch is inactive.
        let src = "#if defined(__linux__)\nL\n#elif defined(_FILE_OFFSET_BITS)\nP\n#endif\n";
        let pp = record(src, &macros(&["__linux__"]));
        let unmapped = pp
            .diagnostics
            .iter()
            .find(|d| d.kind == DiagnosticKind::UnmappedMacro)
            .expect("unmapped macro diagnostic");
        assert!(
            unmapped.message.contains("uncovered"),
            "inactive unmapped branch should be uncovered, got: {}",
            unmapped.message
        );
    }

    #[test]
    fn unterminated_if_is_diagnosed() {
        let pp = record("#if defined(X)\nY\n", &macros(&[]));
        let diag = &pp.diagnostics[0];
        assert_eq!(diag.kind, DiagnosticKind::UnterminatedIf);
        assert!(diag.message.contains("unterminated"));
    }

    #[test]
    fn stray_directive_is_diagnosed() {
        let pp = record("#endif\n", &macros(&[]));
        assert_eq!(pp.diagnostics[0].kind, DiagnosticKind::StrayDirective);
    }
}
