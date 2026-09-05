use crate::backend::rust_ast::Cfg;
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicU64, Ordering};
use thiserror::Error;

static NEXT_SANITIZED_INPUT: AtomicU64 = AtomicU64::new(0);

pub fn read_source(path: &Path) -> std::io::Result<(String, Vec<u8>)> {
    let raw = std::fs::read(path)?;
    Ok((sanitize_utf8_preserving_layout(&raw), raw))
}

fn sanitize_utf8_preserving_layout(bytes: &[u8]) -> String {
    let mut out = Vec::with_capacity(bytes.len());
    let mut rest = bytes;
    loop {
        match std::str::from_utf8(rest) {
            Ok(valid) => {
                out.extend_from_slice(valid.as_bytes());
                break;
            }
            Err(error) => {
                let valid_up_to = error.valid_up_to();
                out.extend_from_slice(&rest[..valid_up_to]);
                let bad_len = error.error_len().unwrap_or(rest.len() - valid_up_to);
                out.resize(out.len() + bad_len, b'?');
                rest = &rest[valid_up_to + bad_len..];
            }
        }
    }
    String::from_utf8(out).expect("invalid UTF-8 runs replaced with ASCII placeholders")
}

#[derive(Debug, Error)]
pub enum PreprocessError {
    #[error("directive span {start}..{end} is outside {source_len} bytes of source")]
    DirectiveSpan {
        start: usize,
        end: usize,
        source_len: usize,
    },
    #[error("create {path}: {source}")]
    CreateTempDir {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("could not allocate a temporary directory for sanitized Clang input")]
    TempDirExhausted,
    #[error("write sanitized Clang input {path}: {source}")]
    WriteSanitizedInput {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("get current directory: {source}")]
    CurrentDir {
        #[source]
        source: std::io::Error,
    },
    #[error("encode VFS overlay: {source}")]
    EncodeOverlay {
        #[source]
        source: serde_json::Error,
    },
    #[error("write VFS overlay {path}: {source}")]
    WriteOverlay {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("query predefined macros: {source}")]
    PredefinedMacros {
        #[source]
        source: crate::frontend::toolchain::EmitError,
    },
    #[error("run preprocessing diagnostics for {path}: {source}")]
    Diagnostics {
        path: PathBuf,
        #[source]
        source: crate::frontend::toolchain::EmitError,
    },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum DirectiveKind {
    If,
    Ifdef,
    Ifndef,
    Elif,
    Elifdef,
    Elifndef,
    Else,
}

impl DirectiveKind {
    pub fn as_str(self) -> &'static str {
        match self {
            DirectiveKind::If => "if",
            DirectiveKind::Ifdef => "ifdef",
            DirectiveKind::Ifndef => "ifndef",
            DirectiveKind::Elif => "elif",
            DirectiveKind::Elifdef => "elifdef",
            DirectiveKind::Elifndef => "elifndef",
            DirectiveKind::Else => "else",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum DiagnosticKind {
    UnmappedMacro,
    OpaquePredicate,
    StrayDirective,
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

#[derive(Debug, Clone)]
pub struct Diagnostic {
    pub kind: DiagnosticKind,
    pub line: usize,
    pub message: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum PredExpr {
    Constant(bool),
    Defined(String),
    Not(Box<PredExpr>),
    And(Vec<PredExpr>),
    Or(Vec<PredExpr>),
    Opaque(String),
}

#[derive(Debug, Clone)]
pub struct Branch {
    pub kind: DirectiveKind,
    pub raw_predicate: Option<String>,
    pub predicate: PredExpr,
    pub directive_line: usize,
    pub body_start: usize,
    pub body_end: usize,
    pub rust_cfg: Option<Cfg>,
    pub active: Option<bool>,
}

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
    Elifdef,
    Elifndef,
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
        if !name.is_empty() && name.bytes().all(|byte| byte.is_ascii_digit()) {
            return Self::Line;
        }
        match name.as_str() {
            "" => Self::Null,
            "if" => Self::If,
            "ifdef" => Self::Ifdef,
            "ifndef" => Self::Ifndef,
            "elif" => Self::Elif,
            "elifdef" => Self::Elifdef,
            "elifndef" => Self::Elifndef,
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
            Self::Elifdef => "elifdef",
            Self::Elifndef => "elifndef",
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
            | Self::Elifdef
            | Self::Elifndef
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
        if self.name == DirectiveName::Pragma {
            if self.raw_payload.trim() == "once" {
                return DirectiveDisposition::NoOutput;
            }
            if is_diagnostic_pragma(&self.raw_payload) {
                return DirectiveDisposition::DiagnosticOnly;
            }
        }
        self.name.disposition()
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

    pub fn is_clang_resolved_pragma(&self) -> bool {
        self.name == DirectiveName::Pragma
            && (is_pack_pragma(&self.raw_payload)
                || is_visibility_pragma(&self.raw_payload)
                || is_weak_pragma(&self.raw_payload)
                || is_redefine_extname_pragma(&self.raw_payload)
                || is_macro_state_pragma(&self.raw_payload)
                || is_poison_pragma(&self.raw_payload)
                || is_cx_limited_range_pragma(&self.raw_payload)
                || is_fp_contract_pragma(&self.raw_payload)
                || is_fenv_access_pragma(&self.raw_payload)
                || is_fenv_round_pragma(&self.raw_payload)
                || is_fenv_dec_round_pragma(&self.raw_payload)
                || is_unroll_pragma(&self.raw_payload)
                || is_clang_optimize_pragma(&self.raw_payload)
                || is_clang_loop_pragma(&self.raw_payload)
                || is_clang_attribute_pragma(&self.raw_payload)
                || is_clang_fp_pragma(&self.raw_payload))
    }

    pub fn is_poison_pragma(&self) -> bool {
        self.name == DirectiveName::Pragma && is_poison_pragma(&self.raw_payload)
    }
}

fn is_diagnostic_pragma(payload: &str) -> bool {
    let payload = payload.trim_start();
    payload.starts_with("GCC diagnostic ")
        || payload.starts_with("clang diagnostic ")
        || payload.starts_with("warning(")
        || payload.starts_with("message(")
        || payload.starts_with("GCC warning")
}

fn is_pack_pragma(payload: &str) -> bool {
    let Some(args) = payload
        .trim()
        .strip_prefix("pack")
        .map(str::trim_start)
        .and_then(|rest| rest.strip_prefix('('))
        .and_then(|rest| rest.strip_suffix(')'))
    else {
        return false;
    };
    let args: Vec<_> = args.split(',').map(str::trim).collect();
    match args.as_slice() {
        [""] => true,
        ["push"] | ["pop"] => true,
        [alignment] => pack_alignment(alignment),
        ["push", value] | ["pop", value] => pack_alignment(value) || c_identifier(value),
        ["push", label, alignment] | ["pop", label, alignment] => {
            c_identifier(label) && pack_alignment(alignment)
        }
        _ => false,
    }
}

fn is_visibility_pragma(payload: &str) -> bool {
    let Some(rest) = payload
        .trim()
        .strip_prefix("GCC visibility")
        .map(str::trim_start)
    else {
        return false;
    };
    if rest == "pop" {
        return true;
    }
    let Some(value) = rest
        .strip_prefix("push")
        .map(str::trim_start)
        .and_then(|rest| rest.strip_prefix('('))
        .and_then(|rest| rest.strip_suffix(')'))
        .map(str::trim)
    else {
        return false;
    };
    matches!(value, "default" | "hidden" | "protected" | "internal")
}

fn is_weak_pragma(payload: &str) -> bool {
    let Some(rest) = payload.trim().strip_prefix("weak") else {
        return false;
    };
    if !rest.starts_with(char::is_whitespace) {
        return false;
    }
    let rest = rest.trim();
    if let Some((alias, target)) = rest.split_once('=') {
        return !target.contains('=') && c_identifier(alias.trim()) && c_identifier(target.trim());
    }
    c_identifier(rest)
}

fn is_redefine_extname_pragma(payload: &str) -> bool {
    let Some(rest) = payload.trim().strip_prefix("redefine_extname") else {
        return false;
    };
    if !rest.starts_with(char::is_whitespace) {
        return false;
    }
    let parts: Vec<_> = rest.split_whitespace().collect();
    matches!(parts.as_slice(), [name, target] if c_identifier(name) && c_identifier(target))
}

fn is_macro_state_pragma(payload: &str) -> bool {
    ["push_macro", "pop_macro"].iter().any(|name| {
        payload
            .trim()
            .strip_prefix(name)
            .map(str::trim_start)
            .and_then(|rest| rest.strip_prefix('('))
            .and_then(|rest| rest.strip_suffix(')'))
            .map(str::trim)
            .and_then(|name| name.strip_prefix('"'))
            .and_then(|name| name.strip_suffix('"'))
            .is_some_and(c_identifier)
    })
}

fn is_poison_pragma(payload: &str) -> bool {
    let Some(rest) = payload.trim().strip_prefix("GCC poison") else {
        return false;
    };
    if !rest.starts_with(char::is_whitespace) {
        return false;
    }
    let names: Vec<_> = rest.split_whitespace().collect();
    !names.is_empty() && names.into_iter().all(c_identifier)
}

fn pack_alignment(value: &str) -> bool {
    !value.is_empty() && value.bytes().all(|byte| byte.is_ascii_digit())
}

fn is_cx_limited_range_pragma(payload: &str) -> bool {
    let Some(rest) = payload.trim().strip_prefix("STDC CX_LIMITED_RANGE") else {
        return false;
    };
    if !rest.starts_with(char::is_whitespace) {
        return false;
    }
    matches!(rest.trim(), "ON" | "OFF" | "DEFAULT")
}

fn is_fp_contract_pragma(payload: &str) -> bool {
    let Some(rest) = payload.trim().strip_prefix("STDC FP_CONTRACT") else {
        return false;
    };
    if !rest.starts_with(char::is_whitespace) {
        return false;
    }
    matches!(rest.trim(), "ON" | "OFF" | "DEFAULT")
}

fn is_fenv_access_pragma(payload: &str) -> bool {
    let Some(rest) = payload.trim().strip_prefix("STDC FENV_ACCESS") else {
        return false;
    };
    if !rest.starts_with(char::is_whitespace) {
        return false;
    }
    matches!(rest.trim(), "ON" | "OFF" | "DEFAULT")
}

fn is_fenv_round_pragma(payload: &str) -> bool {
    let Some(rest) = payload.trim().strip_prefix("STDC FENV_ROUND") else {
        return false;
    };
    if !rest.starts_with(char::is_whitespace) {
        return false;
    }
    c_identifier(rest.trim())
}

fn is_fenv_dec_round_pragma(payload: &str) -> bool {
    let Some(rest) = payload.trim().strip_prefix("STDC FENV_DEC_ROUND") else {
        return false;
    };
    if !rest.starts_with(char::is_whitespace) {
        return false;
    }
    c_identifier(rest.trim())
}

fn is_unroll_pragma(payload: &str) -> bool {
    let payload = payload.trim();
    if payload == "unroll" || payload == "nounroll" {
        return true;
    }
    let Some(rest) = payload.strip_prefix("unroll") else {
        return false;
    };
    if !rest.starts_with(char::is_whitespace) && !rest.starts_with('(') {
        return false;
    }
    let rest = rest.trim();
    let arg = rest
        .strip_prefix('(')
        .and_then(|rest| rest.strip_suffix(')'))
        .unwrap_or(rest);
    !arg.is_empty() && arg.bytes().all(|byte| byte.is_ascii_digit())
}

fn is_clang_optimize_pragma(payload: &str) -> bool {
    matches!(payload.trim(), "clang optimize off" | "clang optimize on")
}

fn is_clang_loop_pragma(payload: &str) -> bool {
    let Some(rest) = payload.trim().strip_prefix("clang loop") else {
        return false;
    };
    if !rest.starts_with(char::is_whitespace) {
        return false;
    }
    const HINTS: &[&str] = &[
        "unroll_count",
        "unroll",
        "vectorize_width",
        "vectorize_predicate",
        "vectorize",
        "interleave_count",
        "interleave",
        "distribute",
        "pipeline_initiation_interval",
        "pipeline",
    ];
    let mut rest = rest.trim();
    if rest.is_empty() {
        return false;
    }
    while !rest.is_empty() {
        let Some(hint) = HINTS.iter().find(|hint| rest.starts_with(**hint)) else {
            return false;
        };
        rest = rest[hint.len()..].trim_start();
        let Some(args_start) = rest.strip_prefix('(') else {
            return false;
        };
        let Some(close) = args_start.find(')') else {
            return false;
        };
        rest = args_start[close + 1..].trim_start();
    }
    true
}

fn is_clang_attribute_pragma(payload: &str) -> bool {
    let payload = payload.trim();
    payload == "clang attribute pop" || payload.starts_with("clang attribute push")
}

fn is_clang_fp_pragma(payload: &str) -> bool {
    let Some(rest) = payload.trim().strip_prefix("clang fp") else {
        return false;
    };
    if !rest.starts_with(char::is_whitespace) {
        return false;
    }
    let rest = rest.trim();
    for keyword in ["contract", "reassociate", "exceptions"] {
        let Some(value) = rest
            .strip_prefix(keyword)
            .map(str::trim_start)
            .and_then(|rest| rest.strip_prefix('('))
            .and_then(|rest| rest.strip_suffix(')'))
            .map(str::trim)
        else {
            continue;
        };
        return matches!(value, "on" | "off" | "default");
    }
    false
}

fn c_identifier(value: &str) -> bool {
    let mut bytes = value.bytes();
    bytes
        .next()
        .is_some_and(|byte| byte == b'_' || byte.is_ascii_alphabetic())
        && bytes.all(|byte| byte == b'_' || byte.is_ascii_alphanumeric())
}

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
    raw: &[u8],
    directives: &[&DirectiveRecord],
) -> Result<ClangInput, PreprocessError> {
    if directives.is_empty() {
        return Ok(ClangInput {
            extra_args: Vec::new(),
            temp_dir: None,
        });
    }

    let mut bytes = raw.to_vec();
    let source_len = bytes.len();
    for directive in directives {
        for byte in bytes
            .get_mut(directive.byte_start..directive.byte_end)
            .ok_or(PreprocessError::DirectiveSpan {
                start: directive.byte_start,
                end: directive.byte_end,
                source_len,
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
    std::fs::write(&sanitized_path, bytes).map_err(|source| {
        PreprocessError::WriteSanitizedInput {
            path: sanitized_path.clone(),
            source,
        }
    })?;
    let virtual_path = if path.is_absolute() {
        path.to_path_buf()
    } else {
        std::env::current_dir()
            .map_err(|source| PreprocessError::CurrentDir { source })?
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
        serde_json::to_vec(&overlay).map_err(|source| PreprocessError::EncodeOverlay { source })?,
    )
    .map_err(|source| PreprocessError::WriteOverlay {
        path: overlay_path.clone(),
        source,
    })?;

    input.extra_args = vec![
        "-ivfsoverlay".to_string(),
        overlay_path.to_string_lossy().into_owned(),
    ];
    Ok(input)
}

fn create_sanitized_temp_dir() -> Result<PathBuf, PreprocessError> {
    for _ in 0..100 {
        let id = NEXT_SANITIZED_INPUT.fetch_add(1, Ordering::Relaxed);
        let path =
            std::env::temp_dir().join(format!("slate-sanitized-{}-{id}", std::process::id()));
        match std::fs::create_dir(&path) {
            Ok(()) => return Ok(path),
            Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => {}
            Err(source) => return Err(PreprocessError::CreateTempDir { path, source }),
        }
    }
    Err(PreprocessError::TempDirExhausted)
}

pub fn record(source: &str, macros: &BTreeMap<String, String>) -> Preprocessing {
    let mut pp = scan(source);
    resolve_directive_activity(&mut pp.directives, macros);
    let activity: BTreeMap<_, _> = pp
        .directives
        .iter()
        .map(|directive| (directive.line_start, directive.active))
        .collect();
    for chain in &mut pp.chains {
        for branch in &mut chain.branches {
            branch.active = activity.get(&branch.directive_line).copied().flatten();
        }
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

struct ConditionalState {
    parent: Option<bool>,
    matched: Option<bool>,
    active: Option<bool>,
}

fn resolve_directive_activity(
    directives: &mut [DirectiveRecord],
    initial_macros: &BTreeMap<String, String>,
) {
    let mut macros = initial_macros.clone();
    for macro_definition in crate::frontend::macros::MACROS {
        macros.entry(macro_definition.name.to_string()).or_default();
    }
    let mut stack: Vec<ConditionalState> = Vec::new();
    for directive in directives {
        let parsed = conditional_directive(directive.name.as_str(), &directive.raw_payload);
        match parsed {
            Some((Directive::Open(kind), arg)) => {
                let parent = stack.last().map_or(Some(true), |state| state.active);
                let selected = eval(&open_predicate(kind, arg), &macros);
                let active = truth_and(parent, selected);
                stack.push(ConditionalState {
                    parent,
                    matched: selected,
                    active,
                });
                directive.active = active;
            }
            Some((Directive::Cont(kind), arg)) => {
                let Some(state) = stack.last_mut() else {
                    directive.active = Some(true);
                    continue;
                };
                let selected = if kind == DirectiveKind::Else {
                    truth_not(state.matched)
                } else {
                    truth_and(
                        truth_not(state.matched),
                        eval(&open_predicate(kind, arg), &macros),
                    )
                };
                state.matched = truth_or(state.matched, selected);
                state.active = truth_and(state.parent, selected);
                directive.active = state.active;
            }
            Some((Directive::Endif, _)) => {
                stack.pop();
                directive.active = stack.last().map_or(Some(true), |state| state.active);
            }
            None => {
                directive.active = stack.last().map_or(Some(true), |state| state.active);
                if directive.active == Some(true)
                    && let Some(name) = directive_macro_name(&directive.raw_payload)
                {
                    match directive.name {
                        DirectiveName::Define => {
                            macros.insert(name.to_string(), String::new());
                        }
                        DirectiveName::Undef => {
                            macros.remove(name);
                        }
                        _ => {}
                    }
                }
            }
        }
    }
}

fn directive_macro_name(payload: &str) -> Option<&str> {
    let payload = payload.trim_start();
    let end = payload
        .bytes()
        .position(|byte| byte != b'_' && !byte.is_ascii_alphanumeric())
        .unwrap_or(payload.len());
    (end > 0).then_some(&payload[..end])
}

fn truth_and(lhs: Option<bool>, rhs: Option<bool>) -> Option<bool> {
    match (lhs, rhs) {
        (Some(false), _) | (_, Some(false)) => Some(false),
        (Some(true), Some(true)) => Some(true),
        _ => None,
    }
}

fn truth_or(lhs: Option<bool>, rhs: Option<bool>) -> Option<bool> {
    match (lhs, rhs) {
        (Some(true), _) | (_, Some(true)) => Some(true),
        (Some(false), Some(false)) => Some(false),
        _ => None,
    }
}

fn truth_not(value: Option<bool>) -> Option<bool> {
    value.map(|value| !value)
}

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

fn has_opaque(expr: &PredExpr) -> bool {
    match expr {
        PredExpr::Opaque(_) => true,
        PredExpr::Constant(_) | PredExpr::Defined(_) => false,
        PredExpr::Not(inner) => has_opaque(inner),
        PredExpr::And(items) | PredExpr::Or(items) => items.iter().any(has_opaque),
    }
}

fn unmapped_atoms(expr: &PredExpr) -> Vec<String> {
    let mut out = Vec::new();
    collect_unmapped(expr, &mut out);
    out
}

fn collect_unmapped(expr: &PredExpr, out: &mut Vec<String>) {
    match expr {
        PredExpr::Constant(_) => {}
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
pub fn record_file(source: &str, clang_args: &[String]) -> Result<Preprocessing, PreprocessError> {
    let macros = crate::frontend::toolchain::predefined_macros(clang_args)
        .map_err(|source| PreprocessError::PredefinedMacros { source })?;
    Ok(record(source, &macros))
}

pub fn record_translation_unit(
    path: &Path,
    source: &str,
    clang_args: &[String],
) -> Result<Preprocessing, PreprocessError> {
    let mut pp = record_file(source, clang_args)?;
    if pp
        .directives
        .iter()
        .any(|directive| directive.name == DirectiveName::Error && directive.active.is_none())
    {
        let (success, stderr) = crate::frontend::toolchain::preprocess_diagnostics(
            path, clang_args,
        )
        .map_err(|source| PreprocessError::Diagnostics {
            path: path.to_path_buf(),
            source,
        })?;
        let path = path
            .canonicalize()
            .unwrap_or_else(|_| path.to_path_buf())
            .to_string_lossy()
            .into_owned();
        for directive in &mut pp.directives {
            if directive.name != DirectiveName::Error || directive.active.is_some() {
                continue;
            }
            directive.active = if success {
                Some(false)
            } else {
                let prefix = format!("{path}:{}:", directive.line_start);
                stderr
                    .lines()
                    .any(|line| line.starts_with(&prefix))
                    .then_some(true)
            };
        }
    }
    Ok(pp)
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
        "elifdef" => Directive::Cont(DirectiveKind::Elifdef),
        "elifndef" => Directive::Cont(DirectiveKind::Elifndef),
        "else" => Directive::Cont(DirectiveKind::Else),
        "endif" => Directive::Endif,
        _ => return None,
    };
    Some((dir, arg))
}

fn open_predicate(kind: DirectiveKind, arg: Option<&str>) -> PredExpr {
    let arg = arg.unwrap_or("");
    match kind {
        DirectiveKind::Ifdef | DirectiveKind::Elifdef => PredExpr::Defined(arg.to_string()),
        DirectiveKind::Ifndef | DirectiveKind::Elifndef => {
            PredExpr::Not(Box::new(PredExpr::Defined(arg.to_string())))
        }
        _ => parse_predicate(arg),
    }
}

fn eval(expr: &PredExpr, macros: &BTreeMap<String, String>) -> Option<bool> {
    match expr {
        PredExpr::Constant(value) => Some(*value),
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
        "__OpenBSD__" => opt("target_os", "freebsd"),
        "__sun" | "sun" => opt("target_os", "solaris"),
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
        "__s390x__" => opt("target_arch", "s390x"),
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

fn slate_target_cfg(macro_name: &str) -> Option<Cfg> {
    if let Some(value) = macro_name.strip_prefix("__SLATE_ARCH_") {
        return Some(opt(
            "target_arch",
            match value {
                "X86_64" => "x86_64",
                "X86" => "x86",
                "ARM" => "arm",
                "AARCH64" => "aarch64",
                "RISCV64" => "riscv64",
                "RISCV32" => "riscv32",
                _ => return None,
            },
        ));
    }
    if let Some(value) = macro_name.strip_prefix("__SLATE_VENDOR_") {
        return Some(opt(
            "target_vendor",
            match value {
                "UNKNOWN" => "unknown",
                "PC" => "pc",
                "APPLE" => "apple",
                _ => return None,
            },
        ));
    }
    if let Some(value) = macro_name.strip_prefix("__SLATE_KERNEL_") {
        return match value {
            "LINUX" => Some(Cfg::Any(vec![
                opt("target_os", "linux"),
                opt("target_os", "android"),
            ])),
            "WINDOWS" => Some(opt("target_os", "windows")),
            "DARWIN" => Some(opt("target_os", "macos")),
            _ => None,
        };
    }
    if let Some(value) = macro_name.strip_prefix("__SLATE_PLATFORM_") {
        return match value {
            "ANDROID" => Some(opt("target_os", "android")),
            "MACOS" => Some(opt("target_os", "macos")),
            _ => None,
        };
    }
    if let Some(value) = macro_name.strip_prefix("__SLATE_LIBC_") {
        return match value {
            "GLIBC" => Some(opt("target_env", "gnu")),
            "MUSL" => Some(opt("target_env", "musl")),
            "MINGW" => Some(opt("target_env", "gnu")),
            "MSVC" => Some(opt("target_env", "msvc")),
            "BIONIC" => Some(Cfg::All(Vec::new())),
            "DARWIN" => Some(opt("target_os", "macos")),
            // no Rust target_env distinguishes a generic libc; treat as unconstrained
            "GENERIC" => Some(Cfg::All(Vec::new())),
            _ => None,
        };
    }
    if macro_name.starts_with("__SLATE_OBJ_") {
        // object format (ELF/COFF/Mach-O) has no matching Rust cfg; unconstrained
        return Some(Cfg::All(Vec::new()));
    }
    if let Some(value) = macro_name.strip_prefix("__SLATE_WORDSIZE_") {
        return Some(opt(
            "target_pointer_width",
            match value {
                "64" => "64",
                "32" => "32",
                _ => return None,
            },
        ));
    }
    if let Some(value) = macro_name.strip_prefix("__SLATE_ENDIAN_") {
        return Some(opt(
            "target_endian",
            match value {
                "LITTLE" => "little",
                "BIG" => "big",
                _ => return None,
            },
        ));
    }
    None
}

fn macro_cfg(macro_name: &str) -> Option<Cfg> {
    known_cfg(macro_name)
        .or_else(|| slate_target_cfg(macro_name))
        .or_else(|| feature_cfg(macro_name))
}

pub fn pred_to_cfg(expr: &PredExpr) -> Option<Cfg> {
    match expr {
        PredExpr::Constant(true) => Some(Cfg::All(Vec::new())),
        PredExpr::Constant(false) => Some(Cfg::Any(Vec::new())),
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
    let is_true = |cfg: &Cfg| matches!(cfg, Cfg::All(v) if v.is_empty());
    let is_false = |cfg: &Cfg| matches!(cfg, Cfg::Any(v) if v.is_empty());
    let mut atoms: Vec<Cfg> = Vec::new();
    for item in items {
        let cfg = pred_to_cfg(item)?;
        match keyword {
            CfgList::All if is_true(&cfg) => continue,
            CfgList::All if is_false(&cfg) => return Some(Cfg::Any(Vec::new())),
            CfgList::Any if is_false(&cfg) => continue,
            CfgList::Any if is_true(&cfg) => return Some(Cfg::All(Vec::new())),
            _ => {}
        }
        if !atoms.contains(&cfg) {
            atoms.push(cfg);
        }
    }
    match atoms.len() {
        0 => Some(match keyword {
            CfgList::All => Cfg::All(Vec::new()),
            CfgList::Any => Cfg::Any(Vec::new()),
        }),
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
    Other,
}

fn tokenize(s: &str) -> Vec<(Tok, usize, usize)> {
    let bytes = s.as_bytes();
    let mut toks = Vec::new();
    let mut i = 0;
    while i < bytes.len() {
        let b = bytes[i];
        match b {
            b' ' | b'\t' => i += 1,
            b'(' => {
                toks.push((Tok::LParen, i, i + 1));
                i += 1;
            }
            b')' => {
                toks.push((Tok::RParen, i, i + 1));
                i += 1;
            }
            b'!' if bytes.get(i + 1) != Some(&b'=') => {
                toks.push((Tok::Bang, i, i + 1));
                i += 1;
            }
            b'&' if bytes.get(i + 1) == Some(&b'&') => {
                toks.push((Tok::And, i, i + 2));
                i += 2;
            }
            b'|' if bytes.get(i + 1) == Some(&b'|') => {
                toks.push((Tok::Or, i, i + 2));
                i += 2;
            }
            b'_' | b'a'..=b'z' | b'A'..=b'Z' => {
                let start = i;
                while i < bytes.len() && (bytes[i] == b'_' || bytes[i].is_ascii_alphanumeric()) {
                    i += 1;
                }
                toks.push((Tok::Ident(s[start..i].to_string()), start, i));
            }
            _ => {
                let start = i;
                i += 1;
                toks.push((Tok::Other, start, i));
            }
        }
    }
    toks
}

struct Parser<'a> {
    toks: Vec<(Tok, usize, usize)>,
    pos: usize,
    src: &'a str,
}

impl<'a> Parser<'a> {
    fn peek(&self) -> Option<&Tok> {
        self.toks.get(self.pos).map(|(tok, ..)| tok)
    }

    fn bump(&mut self) -> Option<(Tok, usize, usize)> {
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
        match self.peek()? {
            Tok::LParen => {
                self.bump();
                let inner = self.parse_or()?;
                match self.bump()?.0 {
                    Tok::RParen => Some(inner),
                    _ => None,
                }
            }
            Tok::Ident(name) if name == "defined" => {
                self.bump();
                if self.peek() == Some(&Tok::LParen) {
                    self.bump();
                    let name = match self.bump()?.0 {
                        Tok::Ident(n) => n,
                        _ => return None,
                    };
                    match self.bump()?.0 {
                        Tok::RParen => Some(PredExpr::Defined(name)),
                        _ => None,
                    }
                } else {
                    match self.bump()?.0 {
                        Tok::Ident(n) => Some(PredExpr::Defined(n)),
                        _ => None,
                    }
                }
            }
            _ => self.parse_opaque_atom(),
        }
    }

    fn parse_opaque_atom(&mut self) -> Option<PredExpr> {
        let &(_, start, mut end) = self.toks.get(self.pos)?;
        let mut depth = 0usize;
        while let Some((tok, _, tend)) = self.toks.get(self.pos) {
            match tok {
                Tok::LParen => {
                    depth += 1;
                    end = *tend;
                    self.pos += 1;
                }
                Tok::RParen => {
                    if depth == 0 {
                        break;
                    }
                    depth -= 1;
                    end = *tend;
                    self.pos += 1;
                }
                Tok::And | Tok::Or if depth == 0 => break,
                _ => {
                    end = *tend;
                    self.pos += 1;
                }
            }
        }
        Some(PredExpr::Opaque(self.src[start..end].trim().to_string()))
    }
}

fn parse_predicate(raw: &str) -> PredExpr {
    match raw.trim() {
        "0" => return PredExpr::Constant(false),
        "1" => return PredExpr::Constant(true),
        _ => {}
    }
    let opaque = || PredExpr::Opaque(raw.trim().to_string());
    let toks = tokenize(raw);
    if toks.is_empty() {
        return opaque();
    }
    let mut parser = Parser {
        toks,
        pos: 0,
        src: raw,
    };
    match parser.parse_or() {
        Some(expr) if parser.pos == parser.toks.len() => expr,
        _ => opaque(),
    }
}

pub fn predicate_text(expr: &PredExpr) -> String {
    match expr {
        PredExpr::Constant(true) => "1".into(),
        PredExpr::Constant(false) => "0".into(),
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
