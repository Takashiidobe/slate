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
//! Scope is deliberately narrow: predicates are normalized only for the known
//! target/debug macros; anything else is kept as an opaque diagnostic rather
//! than guessed.

use crate::rust_ast::Cfg;
use std::collections::BTreeMap;

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

/// Recorded preprocessing metadata for a translation unit.
#[derive(Debug, Clone, Default)]
pub struct Preprocessing {
    pub chains: Vec<CondChain>,
    pub diagnostics: Vec<Diagnostic>,
}

/// Scan `source` for conditional regions and resolve active branches against
/// `macros` (the predefined macro environment of the intended invocation).
pub fn record(source: &str, macros: &BTreeMap<String, String>) -> Preprocessing {
    let mut pp = scan(source);
    for chain in &mut pp.chains {
        resolve_active(chain, macros);
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
            if known_cfg(name).is_none() && !out.contains(name) {
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

fn scan(source: &str) -> Preprocessing {
    let mut stack: Vec<ChainBuilder> = Vec::new();
    let mut chains: Vec<CondChain> = Vec::new();
    let mut diagnostics: Vec<Diagnostic> = Vec::new();

    for (idx, line) in source.lines().enumerate() {
        let lineno = idx + 1;
        let Some((kind, arg)) = directive(line) else {
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
                    directive_line: lineno,
                    body_start: lineno + 1,
                    body_end: lineno,
                    active: None,
                };
                stack.push(ChainBuilder {
                    depth: stack.len(),
                    open_line: lineno,
                    branches: vec![branch],
                });
            }
            Directive::Cont(kind) => {
                let Some(top) = stack.last_mut() else {
                    diagnostics.push(Diagnostic {
                        kind: DiagnosticKind::StrayDirective,
                        line: lineno,
                        message: format!("line {lineno}: #{} without #if", kind.as_str()),
                    });
                    continue;
                };
                close_body(top, lineno);
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
                    directive_line: lineno,
                    body_start: lineno + 1,
                    body_end: lineno,
                    active: None,
                });
            }
            Directive::Endif => {
                let Some(mut top) = stack.pop() else {
                    diagnostics.push(Diagnostic {
                        kind: DiagnosticKind::StrayDirective,
                        line: lineno,
                        message: format!("line {lineno}: #endif without #if"),
                    });
                    continue;
                };
                close_body(&mut top, lineno);
                chains.push(CondChain {
                    depth: top.depth,
                    open_line: top.open_line,
                    endif_line: lineno,
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
        diagnostics,
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

enum Directive {
    Open(DirectiveKind),
    Cont(DirectiveKind),
    Endif,
}

/// If `line` is a conditional preprocessor directive, return its kind and the
/// trimmed argument text (comment-stripped; `None` when there is none).
fn directive(line: &str) -> Option<(Directive, Option<&str>)> {
    let rest = line.trim_start().strip_prefix('#')?;
    let rest = rest.trim_start();
    let (word, arg) = match rest.split_once(char::is_whitespace) {
        Some((w, a)) => (w, strip_comment(a).trim()),
        None => (rest, ""),
    };
    let arg = if arg.is_empty() { None } else { Some(arg) };
    let dir = match word {
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

fn strip_comment(s: &str) -> &str {
    let end = s.find("//").or_else(|| s.find("/*")).unwrap_or(s.len());
    &s[..end]
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
            let mut all = true;
            for item in items {
                all &= eval(item, macros)?;
            }
            Some(all)
        }
        PredExpr::Or(items) => {
            let mut any = false;
            for item in items {
                any |= eval(item, macros)?;
            }
            Some(any)
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
        "_WIN32" => Cfg::Flag("windows".into()),
        "__linux__" => opt("target_os", "linux"),
        "__APPLE__" => opt("target_vendor", "apple"),
        "__x86_64__" | "_M_X64" => opt("target_arch", "x86_64"),
        "__aarch64__" | "_M_ARM64" => opt("target_arch", "aarch64"),
        "__arm__" | "_M_ARM" => opt("target_arch", "arm"),
        "NDEBUG" => Cfg::Not(Box::new(Cfg::Flag("debug_assertions".into()))),
        _ => return None,
    })
}

pub(crate) fn pred_to_cfg(expr: &PredExpr) -> Option<Cfg> {
    match expr {
        PredExpr::Defined(name) => known_cfg(name),
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
    fn unknown_macro_has_no_cfg() {
        assert!(pred_to_cfg(&PredExpr::Defined("PROJECT_FOO".into())).is_none());
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
    fn unknown_macro_is_distinguished_from_opaque_shape() {
        let src = "#if defined(PROJECT_FEATURE_X)\nX\n#endif\n";
        let pp = record(src, &macros(&[]));
        let diag = &pp.diagnostics[0];
        assert_eq!(diag.kind, DiagnosticKind::UnmappedMacro);
        assert_eq!(diag.line, 1);
        assert!(
            diag.message.contains("PROJECT_FEATURE_X"),
            "should name the unmapped macro, got: {}",
            diag.message
        );
    }

    #[test]
    fn inactive_unmapped_branch_is_flagged_uncovered() {
        // linux branch is active (mapped); the unknown-macro branch is inactive.
        let src = "#if defined(__linux__)\nL\n#elif defined(PROJECT_X)\nP\n#endif\n";
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
