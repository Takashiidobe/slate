mod define;
mod error;
mod expand;
mod has_checks;
mod include;
mod syntax;

use crate::ast::{FileId, HeaderKind, Loc, Provenance, Span};
use crate::attribute_support;
use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use crate::const_expr;
use crate::files::{Files, SearchPaths, display_path};
use crate::lexer::{Lexer, Token, TokenSpanExt, keyword_token};
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
    pub macros: foldhash::HashMap<String, MacroEntry>,
    pub main_file: Option<FileId>,
    outermost_system_header: Option<FileId>,
    search: &'a SearchPaths,
    open_stack: Vec<PathBuf>,
    sources: HashMap<FileId, String>,
    pub(crate) line_starts: HashMap<FileId, Vec<usize>>,
    pragma_once: HashSet<PathBuf>,
    pushed_macros: HashMap<String, Vec<Option<MacroEntry>>>,
    pub directive_diagnostics: Vec<DirectiveDiagnostic>,
    line_overrides: HashMap<FileId, Vec<LineOverride>>,
    counter: Cell<i64>,
    build_time: SystemTime,
    standard: LanguageStandard,
    flavor: CompilerFlavor,
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
            macros: foldhash::HashMap::default(),
            main_file: None,
            outermost_system_header: None,
            search,
            open_stack: Vec::new(),
            sources: HashMap::new(),
            line_starts: HashMap::new(),
            pragma_once: HashSet::new(),
            pushed_macros: HashMap::new(),
            directive_diagnostics: Vec::new(),
            line_overrides: HashMap::new(),
            counter: Cell::new(0),
            build_time: SystemTime::now(),
            standard,
            flavor: CompilerFlavor::default(),
            features,
            target: crate::target_info::TargetInfo::default(),
        }
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
        let gnu_namespace = if self.standard.is_gnu() {
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
            let inline_semantics = if self.standard.stdc_version() >= Some(199901) {
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

    pub fn configure(
        &mut self,
        target: crate::target_info::TargetInfo,
        options: &crate::compiler_options::CompilerOptions,
        flavor: CompilerFlavor,
    ) -> Result<(), PPError> {
        self.target = target.clone();
        self.flavor = flavor;
        self.features = self
            .features
            .with_microsoft_extensions(microsoft_extensions_enabled(flavor, &target));
        if flavor == CompilerFlavor::Gcc {
            self.features = self.features.with_gcc_keywords(
                self.standard,
                matches!(
                    target.family,
                    crate::target_info::TargetFamily::X86_64
                        | crate::target_info::TargetFamily::X86
                ),
            );
        }
        self.seed_builtin_macros(&target, flavor)?;
        if self.macros.contains_key("__GNUC__") {
            use crate::compiler_options::InlineSemantics;
            let (selected, other) = match options.effective_inline_semantics(self.standard) {
                InlineSemantics::SupressDef => ("__GNUC_GNU_INLINE__", "__GNUC_STDC_INLINE__"),
                InlineSemantics::ProvideDef => ("__GNUC_STDC_INLINE__", "__GNUC_GNU_INLINE__"),
            };
            if let Some(entry) = self.macros.remove(other) {
                self.macros.insert(selected.to_owned(), entry);
            }
        }
        let mut defines = if target.os == crate::target_info::TargetOs::Windows
            && flavor == CompilerFlavor::Msvc
        {
            Vec::new()
        } else {
            let mut defines = target.long_double.predefines();
            defines.extend(
                target
                    .isa
                    .predefines(target.family, flavor, !self.standard.is_gnu()),
            );
            defines
        };
        if flavor == CompilerFlavor::Gcc
            && options.operations.floating.rounding == crate::ir::Rounding::Environment
        {
            defines.push("__ROUNDING_MATH__=1".into());
        }
        if flavor == CompilerFlavor::Msvc
            && options.explicit_standard
            && let Some(version) = self.standard.stdc_version()
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
        let file = self.files.intern(canon.clone(), HeaderKind::User);
        self.main_file = Some(file);
        self.open_stack.push(canon);
        let nodes = self
            .parse_source(&src, file)
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
        let tokens = Lexer::new(file, src)
            .with_newlines()
            .with_features(self.features)
            .tokenize();
        let items = syntax::parse(src, tokens)?;
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
        let expanded = self.expand_macros(&directive.arguments, &mut foldhash::HashSet::default());
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
                            expanded.arguments = self.expand_macros(
                                &directive.arguments,
                                &mut foldhash::HashSet::default(),
                            );
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
            .expand_macros(source_tokens, &mut foldhash::HashSet::default())
            .into_iter()
            .map(|token| self.classify_keyword(token).with_provenance(provenance))
            .collect::<Vec<_>>();
        let mut nodes = Vec::new();
        let mut code_start = 0;
        let mut index = 0;
        while index < expanded.len() {
            if let Some((pragma_tokens, end)) = self.pragma_operator(&expanded, index) {
                if code_start < index {
                    let code = expanded[code_start..index].to_vec();
                    nodes.push(
                        Span::new(
                            PPNodeKind::Code {
                                tokens: code,
                                provenance,
                            },
                            loc,
                            loc,
                        )
                        .with_provenance(provenance),
                    );
                }
                let origin = Span::cover((), &expanded[index..end]);
                let classified_tokens = pragma_tokens
                    .iter()
                    .cloned()
                    .map(|token| self.classify_keyword(token))
                    .collect::<Vec<_>>();
                let pragma_loc = expanded[index].spelling.through(expanded[end - 1].spelling);
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
                            tokens: classified_tokens,
                            provenance,
                        },
                        pragma_loc,
                        origin.expansion,
                    )
                    .with_provenance(provenance),
                );
                index = end;
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

    fn pragma_operator(
        &self,
        tokens: &[Span<Token>],
        index: usize,
    ) -> Option<(Vec<Span<Token>>, usize)> {
        let Some(Token::Ident(name)) = tokens.value_at(index) else {
            return None;
        };
        if tokens.value_at(index + 1) != Some(&Token::LParen) {
            return None;
        }
        match name.as_str() {
            "_Pragma" => {
                let Some(Token::StringLit(value)) = tokens.value_at(index + 2) else {
                    return None;
                };
                if tokens.value_at(index + 3) != Some(&Token::RParen) {
                    return None;
                }
                let origin = Span::cover((), &tokens[index..index + 4]);
                let decoded = value.replace("\\\"", "\"").replace("\\\\", "\\");
                let pragma_tokens = self
                    .lex(&decoded)
                    .into_iter()
                    .map(|token| origin.clone().with_value(token))
                    .collect();
                Some((pragma_tokens, index + 4))
            }
            "__pragma" if self.features.microsoft_extensions => {
                let mut depth = 0usize;
                let close = (index + 1..tokens.len()).find(|&at| {
                    match tokens[at].value {
                        Token::LParen => depth += 1,
                        Token::RParen => depth -= 1,
                        _ => {}
                    }
                    depth == 0
                })?;
                Some((tokens[index + 2..close].to_vec(), close + 1))
            }
            _ => None,
        }
    }

    fn expand_embed(&self, directive: &Directive) -> Result<PPNode, PPFailure> {
        let arguments = self.expand_macros(&directive.arguments, &mut foldhash::HashSet::default());
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
        Ok(
            Span::new(PPNodeKind::Code { tokens, provenance }, loc, loc)
                .with_provenance(provenance),
        )
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
        Ok(self.is_defined(&name))
    }

    fn is_defined(&self, name: &str) -> bool {
        self.macros.contains_key(name) || is_defined_operator(name, self.flavor)
    }

    fn evaluate_condition(
        &self,
        directive: &Directive,
        name: &'static str,
    ) -> Result<bool, PPFailure> {
        let expanded = self.expand_condition(&directive.arguments);
        let expanded = self.expand_has_embed(&expanded, directive.loc.file);
        let expanded = self.expand_has_include(&expanded, directive.loc.file);
        let expanded = expand_has_checks(&expanded, self.flavor, self.standard, &self.target);
        const_expr::Parser::evaluate_with_defined(
            &expanded,
            &self.target,
            self.flavor,
            &|macro_name| self.is_defined(macro_name),
        )
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
            Token::Ident(word) => match keyword_token(word, &self.features) {
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

fn microsoft_extensions_enabled(
    flavor: CompilerFlavor,
    target: &crate::target_info::TargetInfo,
) -> bool {
    match flavor {
        CompilerFlavor::Msvc => true,
        CompilerFlavor::Clang => target.environment == crate::target_info::TargetEnvironment::Msvc,
        CompilerFlavor::Gcc => false,
    }
}

enum HeaderName {
    Angled(String),
    Quoted(String),
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

fn expand_has_checks(
    tokens: &[Span<Token>],
    flavor: CompilerFlavor,
    standard: LanguageStandard,
    target: &crate::target_info::TargetInfo,
) -> Vec<Span<Token>> {
    let has_attribute = |name: &str| attribute_support::has_attribute(name, flavor, target) as i64;
    let has_c_attribute =
        |name: &str| attribute_support::has_c_attribute(name, flavor, standard, target);
    let has_builtin = |name: &str| has_checks::has_builtin(name) as i64;
    let has_feature = |name: &str| has_checks::has_feature(name) as i64;
    let has_extension = |name: &str| has_checks::has_extension(name) as i64;
    let mut expanded = Vec::with_capacity(tokens.len());
    let mut index = 0;
    while index < tokens.len() {
        let check: Option<&dyn Fn(&str) -> i64> = match tokens.value_at(index) {
            Some(Token::Ident(name)) if name == "__has_attribute" => Some(&has_attribute),
            Some(Token::Ident(name)) if name == "__has_c_attribute" => Some(&has_c_attribute),
            Some(Token::Ident(name)) if name == "__has_builtin" => Some(&has_builtin),
            Some(Token::Ident(name)) if name == "__has_feature" => Some(&has_feature),
            Some(Token::Ident(name)) if name == "__has_extension" => Some(&has_extension),
            _ => None,
        };
        let scoped = matches!(
            tokens.value_at(index),
            Some(Token::Ident(name)) if name == "__has_c_attribute"
        );
        if let Some(check) = check
            && let Some((name, end)) = has_check_argument(tokens, index + 1, scoped)
        {
            expanded.push(
                tokens[index]
                    .clone()
                    .with_value(Token::IntLit(check(&name).to_string())),
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
                CompilerFlavor::Clang => name == "__building_module",
                _ => name == "__has_cpp_attribute",
            }
        }
    }
}

fn has_check_argument(
    tokens: &[Span<Token>],
    start: usize,
    scoped: bool,
) -> Option<(String, usize)> {
    let word = |index: usize| match tokens.value_at(index) {
        Some(Token::Ident(name)) => Some(name.clone()),
        Some(Token::Keyword(keyword)) => Some(<&str>::from(*keyword).to_string()),
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
