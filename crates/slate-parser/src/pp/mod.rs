mod define;
mod error;
mod expand;
mod has_checks;
mod hide_set;
mod include;
mod print;
mod syntax;

use crate::ast::{FileId, HeaderKind, Loc, Provenance, Span};
use crate::attribute_support;
use crate::compiler_args::CompilerFlavor;
use crate::const_expr;
use crate::dialect::Dialect;
use crate::files::{Files, SearchPaths, display_path};
use crate::lexer::{Lexer, Token, TokenSpanExt, keyword_token};
pub use error::{DirectiveDiagnostic, DirectiveErrors, PPError};
use error::{PPErrorKind, PPFailure};
use expand::{PPToken, Piece, Stream};
use hide_set::HideSets;
use include::{include_target, read_source};
use miette::Severity;
pub use print::write_preprocessed;
use std::cell::Cell;
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};
use std::rc::Rc;
use std::time::SystemTime;
use syntax::{Directive, DirectiveName, Line, TokenSource, directive_spelling, identifier};

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum MacroOption {
    Define(String),
    Undef(String),
}

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub struct PreprocessorInputs {
    pub macros: Vec<MacroOption>,
    pub imacros: Vec<PathBuf>,
    pub includes: Vec<PathBuf>,
}

pub type PPNode = Span<PPNodeKind>;

#[derive(Debug, Clone, PartialEq)]
pub enum PPNodeKind {
    Comment {
        text: String,
        provenance: Provenance,
    },
    Code {
        tokens: Vec<Span<Token>>,
        provenance: Provenance,
    },
    Pragma {
        text: String,
        tokens: Vec<Span<Token>>,
        provenance: Provenance,
    },
}

const GNU_COMPATIBILITY_MACROS: [(&str, &str); 5] = [
    ("__GNUC__", "4"),
    ("__GNUC_MINOR__", "2"),
    ("__GNUC_PATCHLEVEL__", "1"),
    ("__GXX_ABI_VERSION", "1002"),
    ("__STDC__", "1"),
];

// expanded by expand_builtin_macro; msvc lacks the gnu-only ones
const BUILTIN_MACROS: [&str; 6] = [
    "__LINE__",
    "__FILE__",
    "__COUNTER__",
    "__DATE__",
    "__TIME__",
    "__TIMESTAMP__",
];
const GNU_BUILTIN_MACROS: [&str; 3] = ["__FILE_NAME__", "__BASE_FILE__", "__INCLUDE_LEVEL__"];

const MSC_VERSION_MACROS: [&str; 5] = [
    "_MSC_VER",
    "_MSC_FULL_VER",
    "_MSC_BUILD",
    "_MSVC_CONSTEXPR_ATTRIBUTE",
    "_CRT_USE_BUILTIN_OFFSETOF",
];

#[derive(Debug, Clone, PartialEq)]
pub struct MacroDef {
    pub parameters: Option<Vec<String>>,
    pub variadic: bool,
    pub replacement: Vec<Span<Token>>,
    pub builtin: bool,
}

#[derive(Debug, Clone, PartialEq)]
pub struct MacroEntry {
    pub definition: Rc<MacroDef>,
    pub provenance: Provenance,
}

struct Conditional {
    opening: Loc,
    in_else: bool,
    taken: bool,
}

impl Conditional {
    fn new(directive: &Directive) -> Self {
        Self {
            opening: directive.loc,
            in_else: false,
            taken: false,
        }
    }

    fn advance(&mut self, directive: &Directive) -> Result<(), PPFailure> {
        match directive.name {
            DirectiveName::Else if self.in_else => {
                Err(PPFailure::at(directive.loc, PPErrorKind::MultipleElse))
            }
            DirectiveName::Else => {
                self.in_else = true;
                Ok(())
            }
            name if self.in_else => Err(PPFailure::at(
                directive.loc,
                PPErrorKind::ElifAfterElse(directive_spelling(name)),
            )),
            _ => Ok(()),
        }
    }
}

pub(super) struct FileInput<'s> {
    source: TokenSource<'s>,
    conditionals: Vec<Conditional>,
    group: Group,
}

enum GuardScan {
    Start,
    Open(String),
    Closed(String),
    Unguarded,
}

// the physical lines one text line's expansion read, and what they carried besides code
#[derive(Default)]
struct Group {
    start: Option<Loc>,
    end: Option<Loc>,
    trailing: Vec<Span<String>>,
    deferred: Vec<PPNode>,
}

impl Group {
    fn extend(&mut self, tokens: &[Span<Token>]) {
        if let (Some(first), Some(last)) = (tokens.first(), tokens.last()) {
            self.start.get_or_insert(first.spelling);
            self.end = Some(last.spelling);
        }
    }

    fn loc(&self) -> Option<Loc> {
        Some(self.start?.through(self.end?))
    }
}

#[derive(Debug, Clone)]
struct LineOverride {
    at_line: usize,
    presumed_line: i64,
    presumed_file: String,
}

pub struct Preprocessor<'a> {
    pub files: Files,
    pub macros: foldhash::HashMap<String, MacroEntry>,
    pub main_file: Option<FileId>,
    outermost_system_header: Option<FileId>,
    search: &'a SearchPaths,
    open_stack: Vec<PathBuf>,
    sources: HashMap<FileId, String>,
    pub(crate) line_starts: HashMap<FileId, Vec<usize>>,
    pragma_once: HashSet<PathBuf>,
    include_guards: HashMap<FileId, String>,
    pushed_macros: HashMap<String, Vec<Option<MacroEntry>>>,
    pub directive_diagnostics: Vec<DirectiveDiagnostic>,
    line_overrides: HashMap<FileId, Vec<LineOverride>>,
    counter: Cell<i64>,
    source_position: Option<Loc>,
    hide_sets: HideSets,
    build_time: SystemTime,
    dialect: &'a Dialect,
}

impl<'a> Preprocessor<'a> {
    fn lex(&self, src: &str) -> Vec<Token> {
        Lexer::new(FileId(0), src, self.dialect.features())
            .tokenize()
            .into_iter()
            .map(|span| span.value)
            .collect()
    }

    pub fn new(search: &'a SearchPaths, dialect: &'a Dialect) -> Result<Self, PPError> {
        let mut preprocessor = Preprocessor {
            files: Files::new(),
            macros: foldhash::HashMap::default(),
            main_file: None,
            outermost_system_header: None,
            search,
            open_stack: Vec::new(),
            sources: HashMap::new(),
            line_starts: HashMap::new(),
            pragma_once: HashSet::new(),
            include_guards: HashMap::new(),
            pushed_macros: HashMap::new(),
            directive_diagnostics: Vec::new(),
            line_overrides: HashMap::new(),
            counter: Cell::new(0),
            source_position: None,
            hide_sets: HideSets::new(),
            build_time: SystemTime::now(),
            dialect,
        };
        preprocessor.configure()?;
        Ok(preprocessor)
    }

    fn seed_builtin_macros(
        &mut self,
        target: &crate::target_info::TargetInfo,
        flavor: CompilerFlavor,
    ) -> Result<(), PPError> {
        let Some(predefines) = target.profile.predefines(flavor) else {
            return Err(
                self.render_error(PPFailure::unlocated(PPErrorKind::Directive(format!(
                    "no predefines for {flavor:?} on {}",
                    target.triple
                )))),
            );
        };
        let gnu_namespace = if self.dialect.standard().is_gnu() {
            predefines.gnu_namespace
        } else {
            ""
        };
        for (name, source) in [
            (predefines.name, predefines.source),
            ("<slate-target-defaults>", predefines.defaults),
            ("<slate-gnu-namespace-predefines>", gnu_namespace),
        ]
        .into_iter()
        .filter(|(_, source)| !source.is_empty())
        {
            let file = self.files.intern(PathBuf::from(name), HeaderKind::System);
            let Ok(nodes) = self.parse_source(source, file) else {
                continue;
            };
            if !nodes.is_empty() {
                continue;
            }
        }
        if flavor.is_clang() {
            self.seed_microsoft_mode_predefines(target)?;
        }
        self.seed_standard_predefines()
    }

    fn seed_microsoft_mode_predefines(
        &mut self,
        target: &crate::target_info::TargetInfo,
    ) -> Result<(), PPError> {
        let captured = target.environment == crate::target_info::TargetEnvironment::Msvc;
        let features = self.dialect.features();
        let mut defines = Vec::new();
        let mut removed = Vec::new();
        match (features.microsoft_compatibility, captured) {
            (true, false) => {
                removed.extend(GNU_COMPATIBILITY_MACROS.map(|(name, _)| name.to_string()));
                removed.extend(
                    self.macros
                        .keys()
                        .filter(|name| name.starts_with("__GCC_ATOMIC_"))
                        .cloned(),
                );
            }
            (false, true) => {
                defines.extend(
                    GNU_COMPATIBILITY_MACROS.map(|(name, value)| (name.into(), value.into())),
                );
                defines.push(("__GCC_ATOMIC_TEST_AND_SET_TRUEVAL".into(), "1".into()));
                for name in self.macros.keys() {
                    if let Some(kind) = name.strip_prefix("__CLANG_ATOMIC_")
                        && let Some(value) = self.scalar_macro_spelling(name)
                    {
                        defines.push((format!("__GCC_ATOMIC_{kind}"), value));
                    }
                }
            }
            _ => {}
        }
        let i686 = target.family == crate::target_info::TargetFamily::X86;
        match (features.microsoft_extensions, captured) {
            (true, false) if i686 => defines.push(("_M_IX86_FP".into(), "2".into())),
            (false, true) => removed.extend(["_MSC_EXTENSIONS".into(), "_M_IX86_FP".into()]),
            _ => {}
        }
        if captured && self.dialect.options().microsoft.extensions == Some(false) {
            removed.extend(MSC_VERSION_MACROS.map(String::from));
        }
        for name in &removed {
            self.macros.remove(name);
        }
        if defines.is_empty() {
            return Ok(());
        }
        let source: String = defines
            .iter()
            .map(|(name, value)| format!("#define {name} {value}\n"))
            .collect();
        let file = self
            .files
            .intern(PathBuf::from("<microsoft modes>"), HeaderKind::System);
        self.parse_source(&source, file)
            .map(drop)
            .map_err(|failure| self.render_error(failure))
    }

    fn seed_standard_predefines(&mut self) -> Result<(), PPError> {
        match (
            self.dialect
                .standard()
                .predefined_stdc_version(self.dialect.flavor()),
            self.macros.get_mut("__STDC_VERSION__"),
        ) {
            (Some(version), Some(entry)) => {
                let replacement = &mut Rc::make_mut(&mut entry.definition).replacement;
                if let Some(token) = replacement.first_mut() {
                    token.value = Token::IntLit(format!("{version}L").into());
                    replacement.truncate(1);
                }
            }
            (None, _) => {
                self.macros.remove("__STDC_VERSION__");
            }
            (Some(_), None) => {}
        }

        let mut defines = Vec::new();
        if self.macros.contains_key("__GNUC__") {
            if !self.dialect.standard().is_gnu() {
                defines.push(("__STRICT_ANSI__".to_string(), "1".to_string()));
            }
            let inline_semantics = if self.dialect.standard().at_least_c99() {
                "__GNUC_STDC_INLINE__"
            } else {
                "__GNUC_GNU_INLINE__"
            };
            defines.push((inline_semantics.to_string(), "1".to_string()));
        }
        if self
            .dialect
            .standard()
            .stdc_version()
            .is_some_and(|version| version >= 202311)
        {
            defines.extend(self.c23_predefines());
        }

        for name in [
            "__STRICT_ANSI__",
            "__GNUC_STDC_INLINE__",
            "__GNUC_GNU_INLINE__",
            "__BOOL_DEFINED",
            "__CHAR8_TYPE__",
            "__CLANG_ATOMIC_CHAR8_T_LOCK_FREE",
            "__GCC_ATOMIC_CHAR8_T_LOCK_FREE",
        ] {
            self.macros.remove(name);
        }
        for name in self
            .macros
            .keys()
            .filter(|name| Self::is_binary_format_macro(name))
            .cloned()
            .collect::<Vec<_>>()
        {
            self.macros.remove(&name);
        }

        if defines.is_empty() {
            return Ok(());
        }
        let source: String = defines
            .iter()
            .map(|(name, value)| format!("#define {name} {value}\n"))
            .collect();
        let file = self
            .files
            .intern(PathBuf::from("<standard predefines>"), HeaderKind::System);
        self.parse_source(&source, file)
            .map(drop)
            .map_err(|failure| self.render_error(failure))
    }

    fn is_binary_format_macro(name: &str) -> bool {
        name.ends_with("_FMTb__") || name.ends_with("_FMTB__")
    }

    fn c23_predefines(&self) -> Vec<(String, String)> {
        let mut defines = vec![("__CHAR8_TYPE__".to_string(), "unsigned char".to_string())];
        if self.macros.contains_key("_MSC_VER") {
            defines.push(("__BOOL_DEFINED".to_string(), "1".to_string()));
        }
        for prefix in ["__CLANG_ATOMIC", "__GCC_ATOMIC"] {
            if let Some(value) = self.scalar_macro_spelling(&format!("{prefix}_CHAR_LOCK_FREE")) {
                defines.push((format!("{prefix}_CHAR8_T_LOCK_FREE"), value));
            }
        }
        for (hex, binary) in [('x', 'b'), ('X', 'B')] {
            let suffix = format!("_FMT{hex}__");
            for name in self.macros.keys().filter(|name| name.ends_with(&suffix)) {
                let Some(format) = self.string_macro_text(name) else {
                    continue;
                };
                let Some(modifier) = format.strip_suffix(hex) else {
                    continue;
                };
                defines.push((
                    format!("{}_FMT{binary}__", &name[..name.len() - suffix.len()]),
                    format!("\"{modifier}{binary}\""),
                ));
            }
        }
        defines
    }

    fn scalar_macro_spelling(&self, name: &str) -> Option<String> {
        match self.macros.get(name)?.definition.replacement.as_slice() {
            [only] => match &only.value {
                Token::IntLit(text) => Some(text.to_string()),
                _ => None,
            },
            _ => None,
        }
    }

    fn string_macro_text(&self, name: &str) -> Option<String> {
        match self.macros.get(name)?.definition.replacement.as_slice() {
            [only] => match &only.value {
                Token::StringLit(text) => Some(text.to_string()),
                _ => None,
            },
            _ => None,
        }
    }

    pub fn define_all(&mut self, defines: &[String]) -> Result<(), PPError> {
        let source: String = defines
            .iter()
            .map(|define| {
                let define = define.trim_start_matches("-D");
                match define.split_once('=') {
                    Some((name, value)) => format!("#define {name} {value}\n"),
                    None => format!("#define {define} 1\n"),
                }
            })
            .collect();
        let file = self
            .files
            .intern(PathBuf::from("<command line>"), HeaderKind::User);
        self.parse_source(&source, file)
            .map(drop)
            .map_err(|failure| self.render_error(failure))
    }

    pub fn apply_macro_options(&mut self, options: &[MacroOption]) -> Result<(), PPError> {
        let source: String = options
            .iter()
            .map(|option| match option {
                MacroOption::Define(define) => match define.split_once('=') {
                    Some((name, value)) => format!("#define {name} {value}\n"),
                    None => format!("#define {define} 1\n"),
                },
                MacroOption::Undef(name) => format!("#undef {name}\n"),
            })
            .collect();
        let file = self
            .files
            .intern(PathBuf::from("<command line>"), HeaderKind::User);
        self.parse_source(&source, file)
            .map(drop)
            .map_err(|failure| self.render_error(failure))
    }

    pub fn process_forced_file(&mut self, path: &Path) -> Result<(FileId, Vec<PPNode>), PPError> {
        let file = self
            .files
            .intern(PathBuf::from("<command line>"), HeaderKind::User);
        let include = include::IncludeDirective::Quoted(path.to_string_lossy().into_owned());
        let (resolved, kind) = self.resolve_include(&include, file).ok_or_else(|| {
            self.render_error(PPFailure::unlocated(PPErrorKind::HeaderNotFound(
                include.to_string(),
            )))
        })?;
        let included = self.files.intern(resolved, kind);
        let nodes = self
            .resolve_and_parse_include(&include, Loc::new(file, 0, 0))
            .map_err(|failure| self.render_error(failure))?;
        Ok((included, nodes))
    }

    fn seed_builtin_names(&mut self, flavor: CompilerFlavor) {
        let file = self
            .files
            .intern(PathBuf::from("<built-in>"), HeaderKind::System);
        let provenance = self.provenance(Loc::new(file, 0, 0));
        let gnu = match flavor {
            CompilerFlavor::Msvc => &[][..],
            CompilerFlavor::Clang | CompilerFlavor::Gcc => &GNU_BUILTIN_MACROS[..],
        };
        for name in BUILTIN_MACROS.iter().chain(gnu) {
            let definition = MacroDef {
                parameters: None,
                variadic: false,
                replacement: Vec::new(),
                builtin: true,
            };
            self.macros.insert(
                name.to_string(),
                MacroEntry {
                    definition: Rc::new(definition),
                    provenance,
                },
            );
        }
    }

    // msvc ignores #define and #undef of its builtin macros (warning C4117)
    fn is_reserved_macro(&self, name: &str) -> bool {
        self.dialect.flavor().is_msvc()
            && self
                .macros
                .get(name)
                .is_some_and(|entry| entry.definition.builtin)
    }

    fn configure(&mut self) -> Result<(), PPError> {
        let dialect = self.dialect;
        let (target, options, flavor) = (dialect.target(), dialect.options(), dialect.flavor());
        self.seed_builtin_names(flavor);
        self.seed_builtin_macros(target, flavor)?;
        if self.macros.contains_key("__GNUC__") {
            use crate::compiler_options::InlineSemantics;
            let (selected, other) = match dialect.inline_semantics() {
                InlineSemantics::SupressDef => ("__GNUC_GNU_INLINE__", "__GNUC_STDC_INLINE__"),
                InlineSemantics::ProvideDef => ("__GNUC_STDC_INLINE__", "__GNUC_GNU_INLINE__"),
            };
            if let Some(entry) = self.macros.remove(other) {
                self.macros.insert(selected.to_owned(), entry);
            }
        }
        let mut defines = if target.os == crate::target_info::TargetOs::Windows && flavor.is_msvc()
        {
            Vec::new()
        } else {
            let mut defines = target.long_double.predefines();
            defines.extend(target.isa.predefines(
                target.family,
                flavor,
                !self.dialect.standard().is_gnu(),
            ));
            defines
        };
        if flavor.is_gcc()
            && options.operations.floating.rounding == crate::ir::Rounding::Environment
        {
            defines.push("__ROUNDING_MATH__=1".into());
        }
        if flavor.is_msvc()
            && options.explicit_standard
            && let Some(version) = self.dialect.standard().stdc_version()
        {
            let version = if version == 202311 { 202312 } else { version };
            defines.push(format!("__STDC_VERSION__={version}L"));
        }
        for define in &defines {
            if let Some((name, _)) = define.split_once('=') {
                self.macros.remove(name);
            }
        }
        if defines.is_empty() {
            return Ok(());
        }
        let source: String = defines
            .iter()
            .filter_map(|define| {
                define
                    .split_once('=')
                    .map(|(name, value)| format!("#define {name} {value}\n"))
            })
            .collect();
        let file = self
            .files
            .intern(PathBuf::from("<target options>"), HeaderKind::User);
        self.parse_source(&source, file)
            .map(drop)
            .map_err(|failure| self.render_error(failure))
    }

    pub fn parse_file(&mut self, path: &Path) -> Result<Vec<PPNode>, PPError> {
        let canon = path.canonicalize().unwrap_or_else(|_| path.to_path_buf());
        let src =
            read_source(&canon).map_err(|kind| self.render_error(PPFailure::unlocated(kind)))?;
        self.parse_file_with_source(path, &src)
    }

    pub fn parse_file_with_source(
        &mut self,
        path: &Path,
        src: &str,
    ) -> Result<Vec<PPNode>, PPError> {
        let canon = path.canonicalize().unwrap_or_else(|_| path.to_path_buf());
        let file = self.files.intern(canon.clone(), HeaderKind::User);
        self.main_file = Some(file);
        self.open_stack.push(canon);
        let nodes = self
            .parse_source(src, file)
            .map_err(|failure| self.render_error(failure))?;
        self.open_stack.pop();
        Ok(nodes)
    }

    pub fn parse_str(&mut self, name: &str, src: &str) -> Result<Vec<PPNode>, PPError> {
        let file = self.files.intern(PathBuf::from(name), HeaderKind::User);
        self.main_file = Some(file);
        self.parse_source(src, file)
            .map_err(|failure| self.render_error(failure))
    }

    fn parse_source(&mut self, src: &str, file: FileId) -> Result<Vec<PPNode>, PPFailure> {
        self.sources.insert(file, src.to_string());
        let starts: Vec<usize> = std::iter::once(0)
            .chain(src.match_indices('\n').map(|(index, _)| index + 1))
            .collect();
        self.files.set_line_starts(file, starts.clone());
        self.line_starts.insert(file, starts);
        let tokens = Lexer::new(file, src, self.dialect.features()).tokenize_lines();
        self.process(file, TokenSource::new(src, tokens))
    }

    fn source(&self, file: FileId) -> &str {
        self.sources.get(&file).map_or("", String::as_str)
    }

    fn line_number(&self, loc: Loc) -> usize {
        self.line_starts.get(&loc.file).map_or(0, |starts| {
            starts
                .partition_point(|&start| start <= loc.offset)
                .saturating_sub(1)
        })
    }

    fn provenance(&self, loc: Loc) -> Provenance {
        Provenance {
            file: loc.file,
            kind: self.files.kind(loc.file),
            line: self.line_number(loc),
            system_header: self.outermost_system_header,
        }
    }

    pub(super) fn presumed_location(&self, loc: Loc) -> (i64, String) {
        let actual = self.line_number(loc);
        let overrides = self.line_overrides.get(&loc.file);
        let applicable = overrides.and_then(|overrides| {
            let index = overrides.partition_point(|entry| entry.at_line <= actual);
            index.checked_sub(1).map(|index| &overrides[index])
        });
        match applicable {
            Some(entry) => (
                entry.presumed_line + (actual - entry.at_line) as i64,
                entry.presumed_file.clone(),
            ),
            None => (actual as i64 + 1, display_path(self.files.path(loc.file))),
        }
    }

    fn push_line_override(&mut self, loc: Loc, presumed_line: i64, presumed_file: String) {
        let at_line = self.line_number(loc) + 1;
        self.line_overrides
            .entry(loc.file)
            .or_default()
            .push(LineOverride {
                at_line,
                presumed_line,
                presumed_file,
            });
    }

    fn record_line_directive(&mut self, directive: &Directive) -> Result<(), PPFailure> {
        let expanded = self.expand_macros(&directive.arguments)?;
        let Some(Span {
            value: Token::IntLit(number),
            ..
        }) = expanded.first()
        else {
            return Err(PPFailure::at(
                directive.arguments_loc(),
                PPErrorKind::InvalidLineDirective,
            ));
        };
        let Ok(presumed_line) = number.replace('\'', "").parse::<i64>() else {
            return Err(PPFailure::at(
                directive.arguments_loc(),
                PPErrorKind::InvalidLineDirective,
            ));
        };
        let presumed_file = match expanded.get(1) {
            Some(Span {
                value: Token::StringLit(name),
                ..
            }) => name.to_string(),
            _ => self.presumed_location(directive.loc).1,
        };
        self.push_line_override(directive.loc, presumed_line, presumed_file);
        Ok(())
    }

    fn record_line_marker(&mut self, directive: &Directive) {
        let Ok(presumed_line) = self
            .spelling(directive.name_loc)
            .replace('\'', "")
            .parse::<i64>()
        else {
            return;
        };
        let presumed_file = match directive.arguments.first() {
            Some(Span {
                value: Token::StringLit(name),
                ..
            }) => name.to_string(),
            _ => self.presumed_location(directive.loc).1,
        };
        self.push_line_override(directive.loc, presumed_line, presumed_file);
    }

    fn process(&mut self, id: FileId, source: TokenSource<'_>) -> Result<Vec<PPNode>, PPFailure> {
        let mut nodes = Vec::new();
        let mut file = FileInput {
            source,
            conditionals: Vec::new(),
            group: Group::default(),
        };
        let mut guard = GuardScan::Start;
        while let Some(line) = file.source.next_line() {
            let line = match line {
                Line::Directive(directive, comments) => {
                    self.push_comments(&mut nodes, comments);
                    self.source_position = Some(directive.loc);
                    let name = directive.name;
                    let depth = file.conditionals.len();
                    let guard_macro = match guard {
                        GuardScan::Start => self
                            .guard_macro(&directive)
                            .filter(|guard| !self.is_defined(guard)),
                        _ => None,
                    };
                    self.directive(
                        &mut file.source,
                        &mut file.conditionals,
                        directive,
                        &mut nodes,
                    )?;
                    guard = match guard {
                        GuardScan::Start if depth == 0 && file.conditionals.len() == 1 => {
                            guard_macro.map_or(GuardScan::Unguarded, GuardScan::Open)
                        }
                        GuardScan::Open(_)
                            if depth == 1
                                && matches!(
                                    name,
                                    DirectiveName::Elif
                                        | DirectiveName::Elifdef
                                        | DirectiveName::Elifndef
                                        | DirectiveName::Else
                                ) =>
                        {
                            GuardScan::Unguarded
                        }
                        GuardScan::Open(guard) if file.conditionals.is_empty() => {
                            GuardScan::Closed(guard)
                        }
                        GuardScan::Open(guard) => GuardScan::Open(guard),
                        _ => GuardScan::Unguarded,
                    };
                    continue;
                }
                Line::Text(line) => line,
            };
            self.push_comments(&mut nodes, line.comments);
            if line.tokens.is_empty() {
                continue;
            }
            if !matches!(guard, GuardScan::Open(_)) {
                guard = GuardScan::Unguarded;
            }
            file.group.extend(&line.tokens);
            self.source_position = line.tokens.last().map(|token| token.spelling);
            let tokens = line.tokens.into_iter().map(PPToken::from).collect();
            let mut stream = Stream::new(tokens, Some(&mut file));
            let mut pieces = Vec::new();
            while self.read_piece(&mut stream, &mut pieces)? {}
            let group = std::mem::take(&mut file.group);
            if let Some(loc) = group.loc() {
                nodes.extend(self.emit_line(loc, pieces));
            }
            self.push_comments(&mut nodes, group.trailing);
            nodes.extend(group.deferred);
        }
        if let Some(open) = file.conditionals.last() {
            return Err(PPFailure::at(
                open.opening,
                PPErrorKind::UnterminatedConditional,
            ));
        }
        if let GuardScan::Closed(guard) = guard {
            self.include_guards.insert(id, guard);
        }
        Ok(nodes)
    }

    fn guard_macro(&self, directive: &Directive) -> Option<String> {
        let source = self.source(directive.loc.file);
        let name = match (directive.name, directive.arguments.as_slice()) {
            (DirectiveName::Ifndef, [name]) => name,
            (DirectiveName::If, [bang, defined, name])
                if bang.value == Token::Bang
                    && identifier(source, defined).as_deref() == Some("defined") =>
            {
                name
            }
            (DirectiveName::If, [bang, defined, open, name, close])
                if bang.value == Token::Bang
                    && identifier(source, defined).as_deref() == Some("defined")
                    && open.value == Token::LParen
                    && close.value == Token::RParen =>
            {
                name
            }
            _ => return None,
        };
        identifier(source, name)
    }

    fn directive(
        &mut self,
        source: &mut TokenSource<'_>,
        conditionals: &mut Vec<Conditional>,
        directive: Directive,
        nodes: &mut Vec<PPNode>,
    ) -> Result<(), PPFailure> {
        match directive.name {
            DirectiveName::If | DirectiveName::Ifdef | DirectiveName::Ifndef => {
                let mut open = Conditional::new(&directive);
                open.taken = self.branch_taken(&directive)?;
                if open.taken || !self.skip_group(source, &mut open)? {
                    conditionals.push(open);
                }
            }
            DirectiveName::Elif
            | DirectiveName::Elifdef
            | DirectiveName::Elifndef
            | DirectiveName::Else => {
                let Some(mut open) = conditionals.pop() else {
                    return Err(PPFailure::at(
                        directive.loc,
                        PPErrorKind::UnexpectedConditional,
                    ));
                };
                open.advance(&directive)?;
                if !self.skip_group(source, &mut open)? {
                    conditionals.push(open);
                }
            }
            DirectiveName::Endif => {
                if conditionals.pop().is_none() {
                    return Err(PPFailure::at(
                        directive.loc,
                        PPErrorKind::UnexpectedConditional,
                    ));
                }
            }
            _ => self.run_directive(&directive, nodes)?,
        }
        Ok(())
    }

    // scans a group whose branch is not taken; true when it ends at #endif, false when a later branch is taken
    fn skip_group(
        &mut self,
        source: &mut TokenSource<'_>,
        open: &mut Conditional,
    ) -> Result<bool, PPFailure> {
        let mut nested: Vec<Conditional> = Vec::new();
        loop {
            let Some(directive) = source.next_directive() else {
                let innermost = nested.last().unwrap_or(open);
                return Err(PPFailure::at(
                    innermost.opening,
                    PPErrorKind::UnterminatedConditional,
                ));
            };
            match directive.name {
                DirectiveName::If | DirectiveName::Ifdef | DirectiveName::Ifndef => {
                    nested.push(Conditional::new(&directive));
                }
                DirectiveName::Elif
                | DirectiveName::Elifdef
                | DirectiveName::Elifndef
                | DirectiveName::Else => match nested.last_mut() {
                    Some(inner) => inner.advance(&directive)?,
                    None => {
                        open.advance(&directive)?;
                        if !open.taken && self.branch_taken(&directive)? {
                            open.taken = true;
                            return Ok(false);
                        }
                    }
                },
                DirectiveName::Endif if nested.pop().is_none() => {
                    return Ok(true);
                }
                _ => {}
            }
        }
    }

    fn branch_taken(&mut self, directive: &Directive) -> Result<bool, PPFailure> {
        match directive.name {
            DirectiveName::Ifdef | DirectiveName::Elifdef => self.names_defined_macro(directive),
            DirectiveName::Ifndef | DirectiveName::Elifndef => {
                Ok(!self.names_defined_macro(directive)?)
            }
            DirectiveName::Else => Ok(true),
            _ => self.evaluate_condition(directive, directive_spelling(directive.name)),
        }
    }

    fn push_comments(&self, nodes: &mut Vec<PPNode>, comments: Vec<Span<String>>) {
        for comment in comments {
            let provenance = self.provenance(comment.spelling);
            nodes.push(
                Span::new(
                    PPNodeKind::Comment {
                        text: comment.value,
                        provenance,
                    },
                    comment.spelling,
                    comment.spelling,
                )
                .with_provenance(provenance),
            );
        }
    }

    fn run_directive(
        &mut self,
        directive: &Directive,
        nodes: &mut Vec<PPNode>,
    ) -> Result<(), PPFailure> {
        match directive.name {
            DirectiveName::Define => self.record_define(directive)?,
            DirectiveName::Include | DirectiveName::IncludeNext => {
                let mut expanded = directive.clone();
                if !include::spells_header_name(&directive.arguments) {
                    expanded.arguments = self.expand_macros(&directive.arguments)?;
                }
                let include = include_target(self.source(directive.loc.file), &expanded)?;
                nodes.extend(self.resolve_and_parse_include(&include, directive.arguments_loc())?);
            }
            DirectiveName::Embed => nodes.push(self.expand_embed(directive)?),
            DirectiveName::Undef => self.record_undef(directive)?,
            DirectiveName::Pragma => {
                self.record_pragma(directive);
                nodes.push(self.pragma_node(directive));
            }
            DirectiveName::Error => self.record_directive_diagnostic(directive, Severity::Error),
            DirectiveName::Warning => {
                self.record_directive_diagnostic(directive, Severity::Warning)
            }
            DirectiveName::Line => self.record_line_directive(directive)?,
            DirectiveName::LineMarker => self.record_line_marker(directive),
            DirectiveName::Ident | DirectiveName::Null => {}
            _ => {
                return Err(PPFailure::at(
                    directive.name_loc,
                    PPErrorKind::UnsupportedDirective,
                ));
            }
        }
        Ok(())
    }

    fn emit_line(&self, loc: Loc, pieces: Vec<Piece>) -> Vec<PPNode> {
        let provenance = self.provenance(loc);
        let finish = |token: Span<Token>| self.classify_keyword(token).with_provenance(provenance);
        let code_node = |code: Vec<Span<Token>>| {
            Span::new(
                PPNodeKind::Code {
                    tokens: code,
                    provenance,
                },
                loc,
                loc,
            )
            .with_provenance(provenance)
        };
        let mut nodes = Vec::new();
        let mut code = Vec::new();
        for piece in pieces {
            match piece {
                Piece::Code(token) => code.push(finish(token)),
                Piece::Pragma {
                    tokens,
                    spelling,
                    expansion,
                } => {
                    if !code.is_empty() {
                        nodes.push(code_node(std::mem::take(&mut code)));
                    }
                    let text = tokens_source(tokens.values());
                    nodes.push(
                        Span::new(
                            PPNodeKind::Pragma {
                                text,
                                tokens: tokens.into_iter().map(finish).collect(),
                                provenance,
                            },
                            spelling,
                            expansion,
                        )
                        .with_provenance(provenance),
                    );
                }
            }
        }
        if !code.is_empty() {
            nodes.push(code_node(code));
        }
        nodes
    }

    fn expand_embed(&mut self, directive: &Directive) -> Result<PPNode, PPFailure> {
        let arguments = self.expand_macros(&directive.arguments)?;
        let Some(Span {
            value: Token::StringLit(name),
            ..
        }) = arguments.first()
        else {
            return Err(PPFailure::at(
                directive.arguments_loc(),
                PPErrorKind::ExpectedEmbedResource,
            ));
        };
        let include = include::IncludeDirective::Quoted(name.to_string());
        let (path, _) = self
            .resolve_include(&include, directive.loc.file)
            .ok_or_else(|| {
                PPFailure::at(
                    arguments[0].spelling,
                    PPErrorKind::HeaderNotFound(include.to_string()),
                )
            })?;
        let mut bytes = std::fs::read(&path).map_err(|error| {
            PPFailure::at(
                arguments[0].spelling,
                PPErrorKind::ReadFailed {
                    path: crate::files::display_path(&path),
                    message: error.to_string(),
                },
            )
        })?;
        let mut prefix = Vec::new();
        let mut suffix = Vec::new();
        let mut if_empty = Vec::new();
        let mut limit = None;
        let mut index = 1;
        while index < arguments.len() {
            let Some(parameter) = identifier(self.source(directive.loc.file), &arguments[index])
            else {
                return Err(PPFailure::at(
                    arguments[index].spelling,
                    PPErrorKind::InvalidEmbedParameter,
                ));
            };
            if arguments.value_at(index + 1) != Some(&Token::LParen) {
                return Err(PPFailure::at(
                    arguments[index].spelling,
                    PPErrorKind::InvalidEmbedParameter,
                ));
            }
            let start = index + 2;
            let mut depth = 1usize;
            index = start;
            while index < arguments.len() && depth != 0 {
                match arguments[index].value {
                    Token::LParen => depth += 1,
                    Token::RParen => depth -= 1,
                    _ => {}
                }
                index += 1;
            }
            if depth != 0 {
                return Err(PPFailure::at(
                    arguments[start - 1].spelling,
                    PPErrorKind::InvalidEmbedParameter,
                ));
            }
            let replacement = arguments[start..index - 1].to_vec();
            match parameter.as_str() {
                "prefix" => prefix = replacement,
                "suffix" => suffix = replacement,
                "if_empty" => if_empty = replacement,
                "limit" => {
                    let [
                        Span {
                            value: Token::IntLit(value),
                            ..
                        },
                    ] = replacement.as_slice()
                    else {
                        return Err(PPFailure::at(
                            arguments[start - 2].spelling,
                            PPErrorKind::InvalidEmbedParameter,
                        ));
                    };
                    limit = value.parse::<usize>().ok();
                    if limit.is_none() {
                        return Err(PPFailure::at(
                            arguments[start - 2].spelling,
                            PPErrorKind::InvalidEmbedParameter,
                        ));
                    }
                }
                _ => {
                    return Err(PPFailure::at(
                        arguments[start - 2].spelling,
                        PPErrorKind::InvalidEmbedParameter,
                    ));
                }
            }
        }
        if let Some(limit) = limit {
            bytes.truncate(limit);
        }
        let loc = directive.loc;
        let is_empty = bytes.is_empty();
        let mut tokens = if is_empty { if_empty } else { prefix };
        for (index, byte) in bytes.into_iter().enumerate() {
            if index != 0 {
                tokens.push(Span::new(Token::Comma, loc, loc));
            }
            tokens.push(Span::new(
                Token::IntLit(i64::from(byte).to_string().into()),
                loc,
                loc,
            ));
        }
        if !is_empty {
            tokens.extend(suffix);
        }
        let provenance = self.provenance(loc);
        let tokens: Vec<_> = tokens
            .into_iter()
            .map(|token| token.with_provenance(provenance))
            .collect();
        Ok(
            Span::new(PPNodeKind::Code { tokens, provenance }, loc, loc)
                .with_provenance(provenance),
        )
    }

    fn names_defined_macro(&self, directive: &Directive) -> Result<bool, PPFailure> {
        let (name, _) = self.macro_name(directive, directive_spelling(directive.name))?;
        Ok(self.is_defined(&name))
    }

    fn is_defined(&self, name: &str) -> bool {
        self.macros.contains_key(name) || is_defined_operator(name, self.dialect.flavor())
    }

    fn evaluate_condition(
        &mut self,
        directive: &Directive,
        name: &'static str,
    ) -> Result<bool, PPFailure> {
        let expanded = self.expand_condition(&directive.arguments)?;
        let expanded = self.expand_has_embed(&expanded, directive.loc.file);
        let expanded = self.expand_has_include(&expanded, directive.loc.file);
        let expanded = expand_has_checks(&expanded, self.dialect);
        const_expr::Parser::evaluate_with_defined(&expanded, self.dialect, &|macro_name| {
            self.is_defined(macro_name)
        })
        .map(|value| value != 0)
        .map_err(|located| {
            let loc = match located.token {
                Some(index) => expanded
                    .get(index)
                    .map_or_else(|| directive.end_loc(), |token| token.expansion),
                None => directive.arguments_loc(),
            };
            PPFailure::at(
                loc,
                PPErrorKind::InvalidExpression {
                    directive: name,
                    message: located.error.to_string(),
                },
            )
        })
    }

    fn expand_has_embed(&self, tokens: &[Span<Token>], from: FileId) -> Vec<Span<Token>> {
        let mut expanded = Vec::with_capacity(tokens.len());
        let mut index = 0;
        while index < tokens.len() {
            if tokens.value_at(index) == Some(&Token::Ident("__has_embed".into()))
                && tokens.value_at(index + 1) == Some(&Token::LParen)
                && let Some(Token::StringLit(name)) = tokens.value_at(index + 2)
                && tokens.value_at(index + 3) == Some(&Token::RParen)
            {
                let found = self
                    .resolve_include(&include::IncludeDirective::Quoted(name.to_string()), from)
                    .is_some();
                expanded.push(
                    tokens[index]
                        .clone()
                        .with_value(Token::IntLit((found as i64).to_string().into())),
                );
                index += 4;
            } else {
                expanded.push(tokens[index].clone());
                index += 1;
            }
        }
        expanded
    }

    fn expand_has_include(&self, tokens: &[Span<Token>], from: FileId) -> Vec<Span<Token>> {
        let mut expanded = Vec::with_capacity(tokens.len());
        let mut index = 0;
        while index < tokens.len() {
            let next_kind = match tokens.value_at(index) {
                Some(Token::Ident(name)) if name == "__has_include" => Some(false),
                Some(Token::Ident(name)) if name == "__has_include_next" => Some(true),
                _ => None,
            };
            let parsed = next_kind
                .filter(|_| tokens.value_at(index + 1) == Some(&Token::LParen))
                .and_then(|is_next| {
                    parse_header_name(tokens, index + 2).map(|(h, e)| (is_next, h, e))
                });
            if let Some((is_next, header, end)) = parsed {
                let directive = match (is_next, header) {
                    (true, HeaderName::Angled(name)) => include::IncludeDirective::Next(name, true),
                    (true, HeaderName::Quoted(name)) => {
                        include::IncludeDirective::Next(name, false)
                    }
                    (false, HeaderName::Angled(name)) => include::IncludeDirective::Angled(name),
                    (false, HeaderName::Quoted(name)) => include::IncludeDirective::Quoted(name),
                };
                let found = self.resolve_include(&directive, from).is_some();
                expanded.push(
                    tokens[index]
                        .clone()
                        .with_value(Token::IntLit((found as i64).to_string().into())),
                );
                index = end;
            } else {
                expanded.push(tokens[index].clone());
                index += 1;
            }
        }
        expanded
    }

    fn record_directive_diagnostic(&mut self, directive: &Directive, severity: Severity) {
        let keyword = if severity == Severity::Warning {
            "#warning"
        } else {
            "#error"
        };
        let text = if directive.arguments.is_empty() {
            String::new()
        } else {
            self.spelling(directive.arguments_loc()).to_string()
        };
        let message = if text.is_empty() {
            keyword.to_string()
        } else {
            format!("{keyword} {text}")
        };
        let error = self.render_error(PPFailure::at(
            directive.loc,
            PPErrorKind::Directive(message),
        ));
        self.directive_diagnostics.push(DirectiveDiagnostic {
            severity,
            text,
            error,
        });
    }

    fn record_pragma(&mut self, directive: &Directive) {
        let pragma = directive
            .arguments
            .first()
            .and_then(|token| identifier(self.source(directive.loc.file), token));
        match pragma.as_deref() {
            Some("push_macro") => self.push_macro(directive),
            Some("pop_macro") => self.pop_macro(directive),
            Some("once") => {
                let path = self.files.path(directive.loc.file);
                let key = path.canonicalize().unwrap_or_else(|_| path.to_path_buf());
                self.pragma_once.insert(key);
            }
            _ => {}
        }
    }

    fn pragma_node(&self, directive: &Directive) -> PPNode {
        let provenance = self.provenance(directive.loc);
        Span::new(
            PPNodeKind::Pragma {
                text: self.spelling(directive.loc).to_string(),
                tokens: directive
                    .arguments
                    .iter()
                    .cloned()
                    .map(|token| self.classify_keyword(token))
                    .collect(),
                provenance,
            },
            directive.loc,
            directive.loc,
        )
        .with_provenance(provenance)
    }

    fn classify_keyword(&self, token: Span<Token>) -> Span<Token> {
        match &token.value {
            Token::Ident(word) => match keyword_token(word, &self.dialect.features()) {
                Some(keyword) => token.with_value(keyword),
                None => token,
            },
            _ => token,
        }
    }

    fn spelling(&self, loc: Loc) -> &str {
        self.source(loc.file)
            .get(loc.offset..loc.offset + loc.length)
            .unwrap_or_default()
    }
}

enum HeaderName {
    Angled(String),
    Quoted(String),
}

fn parse_header_name(tokens: &[Span<Token>], start: usize) -> Option<(HeaderName, usize)> {
    if let Some(Token::StringLit(text)) = tokens.value_at(start) {
        return (tokens.value_at(start + 1) == Some(&Token::RParen))
            .then_some((HeaderName::Quoted(text.to_string()), start + 2));
    }
    if tokens.value_at(start) != Some(&Token::Less) {
        return None;
    }
    let mut text = String::new();
    let mut index = start + 1;
    loop {
        match tokens.value_at(index) {
            Some(Token::Greater) => break,
            Some(Token::Ident(part)) => text.push_str(part),
            Some(Token::Keyword(keyword)) => text.push_str(<&str>::from(*keyword)),
            Some(Token::Dot) => text.push('.'),
            Some(Token::Slash) => text.push('/'),
            Some(Token::Minus) => text.push('-'),
            _ => return None,
        }
        index += 1;
    }
    (tokens.value_at(index + 1) == Some(&Token::RParen))
        .then_some((HeaderName::Angled(text), index + 2))
}

fn expand_has_checks(tokens: &[Span<Token>], dialect: &Dialect) -> Vec<Span<Token>> {
    let (flavor, standard, target) = (dialect.flavor(), dialect.standard(), dialect.target());
    let microsoft = dialect.features().microsoft_extensions;
    let has_attribute = |name: &str| attribute_support::has_attribute(name, flavor, target) as i64;
    let has_c_attribute =
        |name: &str| attribute_support::has_c_attribute(name, flavor, standard, target);
    let has_cpp_attribute = |name: &str| match has_c_attribute(name) {
        0 if !name.contains("::") => has_attribute(name),
        value => value,
    };
    let has_builtin = |name: &str| has_checks::has_builtin(name) as i64;
    let has_feature = |name: &str| has_checks::has_feature(name) as i64;
    let has_extension = |name: &str| has_checks::has_extension(name) as i64;
    let has_declspec_attribute = |name: &str| {
        (microsoft && attribute_support::declspec_registered(name, flavor, target)) as i64
    };
    let has_warning = |option: &str| has_checks::has_warning(option) as i64;
    let is_identifier = |name: &str| has_checks::is_identifier(name, standard, microsoft) as i64;
    let mut expanded = Vec::with_capacity(tokens.len());
    let mut index = 0;
    while index < tokens.len() {
        let check: Option<&dyn Fn(&str) -> i64> = match tokens.value_at(index) {
            Some(Token::Ident(name)) if name == "__has_attribute" => Some(&has_attribute),
            Some(Token::Ident(name)) if name == "__has_c_attribute" => Some(&has_c_attribute),
            Some(Token::Ident(name)) if name == "__has_cpp_attribute" && flavor.is_gcc() => {
                Some(&has_cpp_attribute)
            }
            Some(Token::Ident(name)) if name == "__has_builtin" => Some(&has_builtin),
            Some(Token::Ident(name)) if name == "__has_feature" => Some(&has_feature),
            Some(Token::Ident(name)) if name == "__has_extension" => Some(&has_extension),
            Some(Token::Ident(name)) if flavor.is_clang() => match name.as_ref() {
                "__has_declspec_attribute" => Some(&has_declspec_attribute),
                "__has_warning" => Some(&has_warning),
                "__is_identifier" => Some(&is_identifier),
                _ => None,
            },
            _ => None,
        };
        let scoped = matches!(
            tokens.value_at(index),
            Some(Token::Ident(name)) if name == "__has_c_attribute" || name == "__has_cpp_attribute"
        );
        let argument = if tokens.value_at(index) == Some(&Token::Ident("__has_warning".into())) {
            has_check_string_argument(tokens, index + 1)
        } else {
            has_check_argument(tokens, index + 1, scoped)
        };
        if let Some(check) = check
            && let Some((name, end)) = argument
        {
            expanded.push(
                tokens[index]
                    .clone()
                    .with_value(Token::IntLit(check(&name).to_string().into())),
            );
            index = end;
        } else {
            expanded.push(tokens[index].clone());
            index += 1;
        }
    }
    expanded
}

// the `__has_*` operators slate evaluates that each compiler also reports to `#ifdef`
fn is_defined_operator(name: &str, flavor: CompilerFlavor) -> bool {
    match flavor {
        CompilerFlavor::Msvc => matches!(name, "__has_include" | "__has_c_attribute"),
        CompilerFlavor::Clang | CompilerFlavor::Gcc => {
            matches!(
                name,
                "__has_include"
                    | "__has_include_next"
                    | "__has_embed"
                    | "__has_attribute"
                    | "__has_c_attribute"
                    | "__has_builtin"
                    | "__has_feature"
                    | "__has_extension"
            ) || match flavor {
                CompilerFlavor::Clang => matches!(
                    name,
                    "__building_module"
                        | "__has_declspec_attribute"
                        | "__has_warning"
                        | "__is_identifier"
                ),
                _ => name == "__has_cpp_attribute",
            }
        }
    }
}

fn has_check_string_argument(tokens: &[Span<Token>], start: usize) -> Option<(String, usize)> {
    match (
        tokens.value_at(start),
        tokens.value_at(start + 1),
        tokens.value_at(start + 2),
    ) {
        (Some(Token::LParen), Some(Token::StringLit(option)), Some(Token::RParen)) => {
            Some((option.to_string(), start + 3))
        }
        _ => None,
    }
}

fn has_check_argument(
    tokens: &[Span<Token>],
    start: usize,
    scoped: bool,
) -> Option<(String, usize)> {
    let word = |index: usize| match tokens.value_at(index) {
        Some(Token::Ident(name)) => Some(name.to_string()),
        Some(Token::Keyword(keyword)) => Some(<&str>::from(*keyword).to_string()),
        Some(token @ (Token::Sizeof | Token::Alignof | Token::Countof)) => {
            Some(String::from(token))
        }
        _ => None,
    };
    if tokens.value_at(start) != Some(&Token::LParen) {
        return None;
    }
    let mut name = word(start + 1)?;
    let mut end = start + 2;
    if scoped
        && tokens.value_at(end) == Some(&Token::Colon)
        && tokens.value_at(end + 1) == Some(&Token::Colon)
    {
        name = format!("{name}::{}", word(end + 2)?);
        end += 3;
    }
    (tokens.value_at(end) == Some(&Token::RParen)).then_some((name, end + 1))
}

fn tokens_source<'a>(tokens: impl IntoIterator<Item = &'a Token>) -> String {
    tokens
        .into_iter()
        .map(String::from)
        .collect::<Vec<_>>()
        .join(" ")
}

fn is_quoted_literal(token: &Token) -> bool {
    matches!(
        token,
        Token::CharLit(..)
            | Token::Utf8CharLit(..)
            | Token::Utf16CharLit(..)
            | Token::Utf32CharLit(..)
            | Token::WideCharLit(..)
            | Token::StringLit(_)
            | Token::Utf8StringLit(_)
            | Token::Utf16StringLit(_)
            | Token::Utf32StringLit(_)
            | Token::WideStringLit(_)
    )
}

fn stringized_source<'t>(tokens: impl IntoIterator<Item = &'t Span<Token>>) -> String {
    let mut text = String::new();
    let mut previous: Option<&Span<Token>> = None;
    for token in tokens {
        if previous.is_some() && token.leading_space {
            text.push(' ');
        }
        let spelling = String::from(&token.value);
        if is_quoted_literal(&token.value) {
            text.extend(spelling.chars().flat_map(|c| match c {
                '"' | '\\' => vec!['\\', c],
                _ => vec![c],
            }));
        } else {
            text.push_str(&spelling);
        }
        previous = Some(token);
    }
    text
}
