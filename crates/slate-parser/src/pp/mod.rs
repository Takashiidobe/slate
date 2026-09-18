mod define;
mod error;
mod expand;
mod has_checks;
mod include;
mod syntax;

use crate::ast::{FileId, HeaderKind, Loc, Provenance, Span};
use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use crate::const_expr;
use crate::files::{Files, SearchPaths, display_path};
use crate::lexer::{Lexer, Token, TokenSpanExt};
use crate::standard_features::StandardFeatures;
pub use error::{DirectiveDiagnostic, DirectiveErrors, PPError};
use error::{PPErrorKind, PPFailure};
use include::{include_target, read_source};
use miette::Severity;
use std::cell::Cell;
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};
use std::time::SystemTime;
use syntax::{Directive, DirectiveName, IfSection, Item, directive_spelling, identifier};

pub type PPNode = Span<PPNodeKind>;

#[derive(Debug, Clone, PartialEq)]
pub enum PPNodeKind {
    Comment {
        text: String,
        provenance: Provenance,
    },
    Code {
        text: String,
        tokens: Vec<Span<Token>>,
        provenance: Provenance,
    },
    Pragma {
        text: String,
        tokens: Vec<Span<Token>>,
        provenance: Provenance,
    },
}

#[derive(Debug, Clone, PartialEq)]
pub struct MacroDef {
    pub parameters: Option<Vec<String>>,
    pub variadic: bool,
    pub replacement: Vec<Span<Token>>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct MacroEntry {
    pub definition: MacroDef,
    pub provenance: Provenance,
}

#[derive(Debug, Clone)]
struct LineOverride {
    at_line: usize,
    presumed_line: i64,
    presumed_file: String,
}

pub struct Preprocessor<'a> {
    pub files: Files,
    pub macros: HashMap<String, MacroEntry>,
    pub main_file: Option<FileId>,
    outermost_system_header: Option<FileId>,
    search: &'a SearchPaths,
    open_stack: Vec<PathBuf>,
    open_macro_states: Vec<(PathBuf, Vec<String>)>,
    sources: HashMap<FileId, String>,
    pub(crate) line_starts: HashMap<FileId, Vec<usize>>,
    pragma_once: HashSet<PathBuf>,
    include_guards: HashMap<PathBuf, String>,
    pushed_macros: HashMap<String, Vec<Option<MacroEntry>>>,
    pub directive_diagnostics: Vec<DirectiveDiagnostic>,
    line_overrides: HashMap<FileId, Vec<LineOverride>>,
    counter: Cell<i64>,
    build_time: SystemTime,
    standard: LanguageStandard,
    features: StandardFeatures,
    target: crate::target_info::TargetInfo,
}

impl<'a> Preprocessor<'a> {
    fn lex(&self, src: &str) -> Vec<Token> {
        Lexer::new(FileId(0), src)
            .with_features(self.features)
            .tokenize()
            .into_iter()
            .map(|span| span.value)
            .collect()
    }

    pub fn new(
        search: &'a SearchPaths,
        standard: LanguageStandard,
        features: StandardFeatures,
    ) -> Self {
        Preprocessor {
            files: Files::new(),
            macros: HashMap::new(),
            main_file: None,
            outermost_system_header: None,
            search,
            open_stack: Vec::new(),
            open_macro_states: Vec::new(),
            sources: HashMap::new(),
            line_starts: HashMap::new(),
            pragma_once: HashSet::new(),
            include_guards: HashMap::new(),
            pushed_macros: HashMap::new(),
            directive_diagnostics: Vec::new(),
            line_overrides: HashMap::new(),
            counter: Cell::new(0),
            build_time: SystemTime::now(),
            standard,
            features,
            target: crate::target_info::TargetInfo::default(),
        }
    }

    fn seed_builtin_macros(
        &mut self,
        target: &crate::target_info::TargetInfo,
        flavor: CompilerFlavor,
    ) -> Result<(), PPError> {
        use crate::target_info::{TargetEnvironment, TargetFamily, TargetOs};
        let (name, source, defaults, gnu_namespace) =
            match (target.os, target.environment, target.family, flavor) {
                (
                    TargetOs::Windows,
                    TargetEnvironment::Msvc,
                    TargetFamily::X86_64,
                    CompilerFlavor::Msvc,
                ) => (
                    "<msvc-x86_64-windows-predefines>",
                    include_str!("../predefines/msvc_19.51.36256_x86_64_windows.h"),
                    "",
                    "",
                ),
                (
                    TargetOs::Windows,
                    TargetEnvironment::Msvc,
                    TargetFamily::AArch64,
                    CompilerFlavor::Msvc,
                ) => (
                    "<msvc-aarch64-windows-predefines>",
                    include_str!("../predefines/msvc_19.51.36256_aarch64_windows.h"),
                    "",
                    "",
                ),
                (
                    TargetOs::Windows,
                    TargetEnvironment::Msvc,
                    TargetFamily::X86_64,
                    CompilerFlavor::Clang,
                ) => (
                    "<clang-x86_64-windows-msvc-predefines>",
                    include_str!("../predefines/clang-22.1.8_x86_64_windows_msvc.h"),
                    "",
                    "",
                ),
                (
                    TargetOs::Windows,
                    TargetEnvironment::Msvc,
                    TargetFamily::AArch64,
                    CompilerFlavor::Clang,
                ) => (
                    "<clang-aarch64-windows-msvc-predefines>",
                    include_str!("../predefines/clang-22.1.8_aarch64_windows_msvc.h"),
                    "",
                    "",
                ),
                (TargetOs::Linux, TargetEnvironment::Gnu, TargetFamily::X86_64, _) => (
                    "<clang-x86_64-linux-gnu-predefines>",
                    include_str!("../predefines/clang-22.1.8_x86_64_linux_gnu.h"),
                    include_str!("../predefines/slate_target_defaults.h"),
                    include_str!("../predefines/slate_gnu_namespace_linux.h"),
                ),
                (TargetOs::Linux, TargetEnvironment::Gnu, TargetFamily::X86, _) => (
                    "<clang-i386-linux-gnu-predefines>",
                    include_str!("../predefines/clang-22.1.8_i686_linux_gnu.h"),
                    include_str!("../predefines/slate_x86_linux_defaults.h"),
                    include_str!("../predefines/slate_gnu_namespace_i386_linux.h"),
                ),
                (TargetOs::Linux, TargetEnvironment::Gnu, TargetFamily::AArch64, _) => (
                    "<clang-aarch64-linux-gnu-predefines>",
                    include_str!("../predefines/clang-22.1.8_aarch64_linux_gnu.h"),
                    include_str!("../predefines/slate_aarch64_linux_defaults.h"),
                    include_str!("../predefines/slate_gnu_namespace_linux.h"),
                ),
                (TargetOs::Linux, TargetEnvironment::GnuEabiHf, TargetFamily::Arm32, _) => (
                    "<clang-armv7-linux-gnueabihf-predefines>",
                    include_str!("../predefines/clang-22.1.8_armv7_linux_gnueabihf.h"),
                    include_str!("../predefines/slate_arm32_linux_defaults.h"),
                    include_str!("../predefines/slate_gnu_namespace_linux.h"),
                ),
                (TargetOs::Linux, TargetEnvironment::GnuEabi, TargetFamily::Arm32, _) => (
                    "<clang-armv7-linux-gnueabi-predefines>",
                    include_str!("../predefines/clang-22.1.8_armv7_linux_gnueabi.h"),
                    include_str!("../predefines/slate_arm32_linux_defaults.h"),
                    include_str!("../predefines/slate_gnu_namespace_linux.h"),
                ),
                _ => {
                    return Err(
                        self.render_error(PPFailure::unlocated(PPErrorKind::Directive(format!(
                            "no predefines for {flavor:?} on {}",
                            target.triple
                        )))),
                    );
                }
            };
        let gnu_namespace = if self.standard.is_gnu() {
            gnu_namespace
        } else {
            ""
        };
        for (name, source) in [
            (name, source),
            ("<slate-target-defaults>", defaults),
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
        self.seed_standard_predefines()
    }

    fn seed_standard_predefines(&mut self) -> Result<(), PPError> {
        match (
            self.standard.stdc_version(),
            self.macros.get_mut("__STDC_VERSION__"),
        ) {
            (Some(version), Some(entry)) => {
                let replacement = &mut entry.definition.replacement;
                if let Some(token) = replacement.first_mut() {
                    token.value = Token::IntLit(format!("{version}L"));
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
            if !self.standard.is_gnu() {
                defines.push(("__STRICT_ANSI__".to_string(), "1".to_string()));
            }
            let inline_semantics = if self.standard.stdc_version().is_some() {
                "__GNUC_STDC_INLINE__"
            } else {
                "__GNUC_GNU_INLINE__"
            };
            defines.push((inline_semantics.to_string(), "1".to_string()));
        }
        if self
            .standard
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
                Token::IntLit(text) => Some(text.clone()),
                _ => None,
            },
            _ => None,
        }
    }

    fn string_macro_text(&self, name: &str) -> Option<String> {
        match self.macros.get(name)?.definition.replacement.as_slice() {
            [only] => match &only.value {
                Token::StringLit(text) => Some(text.clone()),
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

    pub fn configure(
        &mut self,
        target: crate::target_info::TargetInfo,
        options: &crate::compiler_options::CompilerOptions,
        flavor: CompilerFlavor,
    ) -> Result<(), PPError> {
        self.target = target.clone();
        self.seed_builtin_macros(&target, flavor)?;
        let mut defines = if target.os == crate::target_info::TargetOs::Windows
            && flavor == CompilerFlavor::Msvc
        {
            Vec::new()
        } else {
            target.long_double.predefines()
        };
        if flavor == CompilerFlavor::Gcc
            && options.operations.floating.rounding == crate::ir::Rounding::Environment
        {
            defines.push("__ROUNDING_MATH__=1".into());
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
        let file = self.files.intern(canon.clone(), HeaderKind::User);
        self.main_file = Some(file);
        self.open_macro_states
            .push((canon.clone(), self.macro_state()));
        self.open_stack.push(canon);
        let nodes = self
            .parse_source(&src, file)
            .map_err(|failure| self.render_error(failure))?;
        self.open_stack.pop();
        self.open_macro_states.pop();
        Ok(nodes)
    }

    pub(super) fn macro_state(&self) -> Vec<String> {
        let mut state = self
            .macros
            .iter()
            .map(|(name, entry)| format!("{name}:{:?}", entry.definition))
            .collect::<Vec<_>>();
        state.sort();
        state
    }

    pub fn parse_str(&mut self, name: &str, src: &str) -> Result<Vec<PPNode>, PPError> {
        let file = self.files.intern(PathBuf::from(name), HeaderKind::User);
        self.main_file = Some(file);
        self.parse_source(src, file)
            .map_err(|failure| self.render_error(failure))
    }

    fn parse_source(&mut self, src: &str, file: FileId) -> Result<Vec<PPNode>, PPFailure> {
        self.sources.insert(file, src.to_string());
        self.line_starts.insert(
            file,
            std::iter::once(0)
                .chain(src.match_indices('\n').map(|(index, _)| index + 1))
                .collect(),
        );
        let tokens = Lexer::new(file, src)
            .with_newlines()
            .with_features(self.features)
            .tokenize();
        let items = syntax::parse(src, tokens)?;
        if let Some(guard) = include_guard(src, &items) {
            let path = self.files.path(file);
            let key = path.canonicalize().unwrap_or_else(|_| path.to_path_buf());
            self.include_guards.insert(key, guard);
        }
        self.walk_group(&items)
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
        let expanded = self.expand_macros(&directive.arguments, &mut HashSet::new());
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
            }) => name.clone(),
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
            }) => name.clone(),
            _ => self.presumed_location(directive.loc).1,
        };
        self.push_line_override(directive.loc, presumed_line, presumed_file);
    }

    fn walk_group(&mut self, items: &[Item]) -> Result<Vec<PPNode>, PPFailure> {
        let mut nodes = Vec::new();
        for item in items {
            match item {
                Item::Comment(comment) => {
                    let provenance = self.provenance(comment.spelling);
                    nodes.push(
                        Span::new(
                            PPNodeKind::Comment {
                                text: comment.value.clone(),
                                provenance,
                            },
                            comment.spelling,
                            comment.spelling,
                        )
                        .with_provenance(provenance),
                    );
                }
                Item::Text(tokens) => nodes.extend(self.expand_line(tokens)),
                Item::Conditional(section) => nodes.extend(self.walk_conditional(section)?),
                Item::Directive(directive) => match directive.name {
                    DirectiveName::Define => self.record_define(directive)?,
                    DirectiveName::Include | DirectiveName::IncludeNext => {
                        let mut expanded = directive.clone();
                        if !matches!(
                            directive.arguments.value_at(0),
                            Some(Token::StringLit(_) | Token::Less)
                        ) {
                            expanded.arguments =
                                self.expand_macros(&directive.arguments, &mut HashSet::new());
                        }
                        let include = include_target(self.source(directive.loc.file), &expanded)?;
                        nodes.extend(
                            self.resolve_and_parse_include(&include, directive.arguments_loc())?,
                        );
                    }
                    DirectiveName::Embed => nodes.push(self.expand_embed(directive)?),
                    DirectiveName::Undef => self.record_undef(directive)?,
                    DirectiveName::Pragma => {
                        self.record_pragma(directive);
                        nodes.push(self.pragma_node(directive));
                    }
                    DirectiveName::Error => {
                        self.record_directive_diagnostic(directive, Severity::Error)
                    }
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
                },
            }
        }
        Ok(nodes)
    }

    fn expand_line(&mut self, source_tokens: &[Span<Token>]) -> Vec<PPNode> {
        let loc = Span::cover((), source_tokens).spelling;
        let provenance = self.provenance(loc);
        let expanded = self
            .expand_macros(source_tokens, &mut HashSet::new())
            .into_iter()
            .map(|token| token.with_provenance(provenance))
            .collect::<Vec<_>>();
        let mut nodes = Vec::new();
        let mut code_start = 0;
        let mut index = 0;
        while index < expanded.len() {
            if expanded.value_at(index) == Some(&Token::Ident("_Pragma".into()))
                && expanded.value_at(index + 1) == Some(&Token::LParen)
                && let Some(Token::StringLit(value)) = expanded.value_at(index + 2)
                && expanded.value_at(index + 3) == Some(&Token::RParen)
            {
                if code_start < index {
                    let code = expanded[code_start..index].to_vec();
                    nodes.push(
                        Span::new(
                            PPNodeKind::Code {
                                text: tokens_source(code.values()),
                                tokens: code,
                                provenance,
                            },
                            loc,
                            loc,
                        )
                        .with_provenance(provenance),
                    );
                }
                let decoded = value.replace("\\\"", "\"").replace("\\\\", "\\");
                let origin = Span::cover((), &expanded[index..index + 4]);
                let pragma_tokens = self
                    .lex(&decoded)
                    .into_iter()
                    .map(|token| origin.clone().with_value(token))
                    .collect::<Vec<_>>();
                let pragma_loc = expanded[index]
                    .spelling
                    .through(expanded[index + 3].spelling);
                self.record_pragma(&Directive {
                    name: DirectiveName::Pragma,
                    arguments: pragma_tokens.clone(),
                    name_loc: origin.expansion,
                    loc: origin.expansion,
                });
                nodes.push(
                    Span::new(
                        PPNodeKind::Pragma {
                            text: tokens_source(pragma_tokens.values()),
                            tokens: pragma_tokens,
                            provenance,
                        },
                        pragma_loc,
                        origin.expansion,
                    )
                    .with_provenance(provenance),
                );
                index += 4;
                code_start = index;
            } else {
                index += 1;
            }
        }
        if code_start < expanded.len() {
            let code = expanded[code_start..].to_vec();
            nodes.push(
                Span::new(
                    PPNodeKind::Code {
                        text: tokens_source(code.values()),
                        tokens: code,
                        provenance,
                    },
                    loc,
                    loc,
                )
                .with_provenance(provenance),
            );
        }
        nodes
    }

    fn expand_embed(&self, directive: &Directive) -> Result<PPNode, PPFailure> {
        let arguments = self.expand_macros(&directive.arguments, &mut HashSet::new());
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
        let include = include::IncludeDirective::Quoted(name.clone());
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
                Token::IntLit(i64::from(byte).to_string()),
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
        Ok(Span::new(
            PPNodeKind::Code {
                text: tokens_source(tokens.values()),
                tokens,
                provenance,
            },
            loc,
            loc,
        )
        .with_provenance(provenance))
    }

    fn walk_conditional(&mut self, section: &IfSection) -> Result<Vec<PPNode>, PPFailure> {
        for branch in &section.branches {
            let directive = &branch.directive;
            let taken = match directive.name {
                DirectiveName::Ifdef | DirectiveName::Elifdef => {
                    self.names_defined_macro(directive)?
                }
                DirectiveName::Ifndef | DirectiveName::Elifndef => {
                    !self.names_defined_macro(directive)?
                }
                DirectiveName::Else => true,
                _ => self.evaluate_condition(directive, directive_spelling(directive.name))?,
            };
            if taken {
                return self.walk_group(&branch.body);
            }
        }
        Ok(Vec::new())
    }

    fn names_defined_macro(&self, directive: &Directive) -> Result<bool, PPFailure> {
        let (name, _) = self.macro_name(directive, directive_spelling(directive.name))?;
        Ok(self.macros.contains_key(&name))
    }

    fn evaluate_condition(
        &self,
        directive: &Directive,
        name: &'static str,
    ) -> Result<bool, PPFailure> {
        let expanded = self.expand_condition(&directive.arguments);
        let expanded = self.expand_has_embed(&expanded, directive.loc.file);
        let expanded = self.expand_has_include(&expanded, directive.loc.file);
        let expanded = expand_has_checks(&expanded);
        const_expr::Parser::evaluate_with_defined(&expanded, &self.target, &|macro_name| {
            self.macros.contains_key(macro_name)
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
            if tokens.value_at(index) == Some(&Token::Ident("__has_embed".to_string()))
                && tokens.value_at(index + 1) == Some(&Token::LParen)
                && let Some(Token::StringLit(name)) = tokens.value_at(index + 2)
                && tokens.value_at(index + 3) == Some(&Token::RParen)
            {
                let found = self
                    .resolve_include(&include::IncludeDirective::Quoted(name.clone()), from)
                    .is_some();
                expanded.push(
                    tokens[index]
                        .clone()
                        .with_value(Token::IntLit((found as i64).to_string())),
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
                    (true, header) => include::IncludeDirective::Next(header.into_name()),
                    (false, HeaderName::Angled(name)) => include::IncludeDirective::Angled(name),
                    (false, HeaderName::Quoted(name)) => include::IncludeDirective::Quoted(name),
                };
                let found = self.resolve_include(&directive, from).is_some();
                expanded.push(
                    tokens[index]
                        .clone()
                        .with_value(Token::IntLit((found as i64).to_string())),
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
        let message = if directive.arguments.is_empty() {
            keyword.to_string()
        } else {
            format!("{keyword} {}", self.spelling(directive.arguments_loc()))
        };
        let error = self.render_error(PPFailure::at(
            directive.loc,
            PPErrorKind::Directive(message),
        ));
        self.directive_diagnostics
            .push(DirectiveDiagnostic { severity, error });
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
                tokens: directive.arguments.clone(),
                provenance,
            },
            directive.loc,
            directive.loc,
        )
        .with_provenance(provenance)
    }

    fn spelling(&self, loc: Loc) -> &str {
        self.source(loc.file)
            .get(loc.offset..loc.offset + loc.length)
            .unwrap_or_default()
    }
}

fn include_guard(src: &str, items: &[Item]) -> Option<String> {
    let mut items = items
        .iter()
        .filter(|item| !matches!(item, Item::Comment(_)));
    let Item::Conditional(section) = items.next()? else {
        return None;
    };
    if items.next().is_some() || section.branches.len() != 1 {
        return None;
    }
    let branch = &section.branches[0];
    if branch.directive.name != DirectiveName::Ifndef {
        return None;
    }
    let guard = identifier(src, branch.directive.arguments.first()?)?;
    let define = branch
        .body
        .iter()
        .find(|item| !matches!(item, Item::Comment(_)))?;
    let Item::Directive(define) = define else {
        return None;
    };
    if define.name != DirectiveName::Define
        || identifier(src, define.arguments.first()?).as_deref() != Some(&guard)
    {
        return None;
    }
    Some(guard)
}

enum HeaderName {
    Angled(String),
    Quoted(String),
}

impl HeaderName {
    fn into_name(self) -> String {
        match self {
            HeaderName::Angled(name) | HeaderName::Quoted(name) => name,
        }
    }
}

fn parse_header_name(tokens: &[Span<Token>], start: usize) -> Option<(HeaderName, usize)> {
    if let Some(Token::StringLit(text)) = tokens.value_at(start) {
        return (tokens.value_at(start + 1) == Some(&Token::RParen))
            .then_some((HeaderName::Quoted(text.clone()), start + 2));
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

fn expand_has_checks(tokens: &[Span<Token>]) -> Vec<Span<Token>> {
    let mut expanded = Vec::with_capacity(tokens.len());
    let mut index = 0;
    while index < tokens.len() {
        let check = match tokens.value_at(index) {
            Some(Token::Ident(name)) if name == "__has_attribute" => {
                Some(has_checks::has_attribute as fn(&str) -> bool)
            }
            Some(Token::Ident(name)) if name == "__has_builtin" => {
                Some(has_checks::has_builtin as fn(&str) -> bool)
            }
            Some(Token::Ident(name)) if name == "__has_feature" => {
                Some(has_checks::has_feature as fn(&str) -> bool)
            }
            Some(Token::Ident(name)) if name == "__has_extension" => {
                Some(has_checks::has_extension as fn(&str) -> bool)
            }
            _ => None,
        };
        if let Some(check) = check
            && tokens.value_at(index + 1) == Some(&Token::LParen)
            && let Some(Token::Ident(name)) = tokens.value_at(index + 2)
            && tokens.value_at(index + 3) == Some(&Token::RParen)
        {
            expanded.push(
                tokens[index]
                    .clone()
                    .with_value(Token::IntLit((check(name) as i64).to_string())),
            );
            index += 4;
        } else {
            expanded.push(tokens[index].clone());
            index += 1;
        }
    }
    expanded
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

fn stringized_source(tokens: &[Span<Token>]) -> String {
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
