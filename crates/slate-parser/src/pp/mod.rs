mod define;
mod error;
mod expand;
mod include;
mod syntax;

use crate::ast::{FileId, HeaderKind, Loc, Provenance, Span};
use crate::const_expr;
use crate::files::{Files, SearchPaths};
use crate::lexer::{Lexer, Token, TokenSpanExt};
pub use error::{DirectiveDiagnostic, DirectiveErrors, PPError};
use error::{PPErrorKind, PPFailure};
use include::{include_target, read_source};
use miette::Severity;
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};
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

pub struct Preprocessor<'a> {
    pub files: Files,
    pub macros: HashMap<String, MacroEntry>,
    pub main_file: Option<FileId>,
    outermost_header: Option<FileId>,
    search: &'a SearchPaths,
    open_stack: Vec<PathBuf>,
    sources: HashMap<FileId, String>,
    line_starts: HashMap<FileId, Vec<usize>>,
    pragma_once: HashSet<PathBuf>,
    pushed_macros: HashMap<String, Vec<Option<MacroEntry>>>,
    pub directive_diagnostics: Vec<DirectiveDiagnostic>,
}

const BUILTIN_PREDEFINES: [(&str, &str); 2] = [
    (
        "<clang-x86_64-linux-gnu-predefines>",
        include_str!("../predefines/clang_x86_64_linux_gnu.h"),
    ),
    (
        "<slate-target-defaults>",
        include_str!("../predefines/slate_target_defaults.h"),
    ),
];

impl<'a> Preprocessor<'a> {
    pub fn new(search: &'a SearchPaths) -> Self {
        let mut pp = Preprocessor {
            files: Files::new(),
            macros: HashMap::new(),
            main_file: None,
            outermost_header: None,
            search,
            open_stack: Vec::new(),
            sources: HashMap::new(),
            line_starts: HashMap::new(),
            pragma_once: HashSet::new(),
            pushed_macros: HashMap::new(),
            directive_diagnostics: Vec::new(),
        };
        pp.seed_builtin_macros();
        pp
    }

    fn seed_builtin_macros(&mut self) {
        for (name, source) in BUILTIN_PREDEFINES {
            let file = self.files.intern(PathBuf::from(name), HeaderKind::System);
            let nodes = self
                .parse_source(source, file)
                .expect("builtin predefines must parse cleanly");
            debug_assert!(
                nodes.is_empty(),
                "predefines should only contain #define directives"
            );
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
        self.line_starts.insert(
            file,
            std::iter::once(0)
                .chain(src.match_indices('\n').map(|(index, _)| index + 1))
                .collect(),
        );
        let tokens = Lexer::new(file, src).with_newlines().tokenize();
        let items = syntax::parse(src, tokens)?;
        self.walk_group(&items)
    }

    fn source(&self, file: FileId) -> &str {
        self.sources.get(&file).map_or("", String::as_str)
    }

    fn provenance(&self, loc: Loc) -> Provenance {
        let line = self.line_starts.get(&loc.file).map_or(0, |starts| {
            starts
                .partition_point(|&start| start <= loc.offset)
                .saturating_sub(1)
        });
        Provenance {
            file: loc.file,
            kind: self.files.kind(loc.file),
            line,
            header: self.outermost_header,
        }
    }

    fn walk_group(&mut self, items: &[Item]) -> Result<Vec<PPNode>, PPFailure> {
        let mut nodes = Vec::new();
        for item in items {
            match item {
                Item::Comment(comment) => nodes.push(Span::new(
                    PPNodeKind::Comment {
                        text: comment.value.clone(),
                        provenance: self.provenance(comment.spelling),
                    },
                    comment.spelling,
                    comment.spelling,
                )),
                Item::Text(tokens) => nodes.push(self.expand_line(tokens)),
                Item::Conditional(section) => nodes.extend(self.walk_conditional(section)?),
                Item::Directive(directive) => match directive.name {
                    DirectiveName::Define => self.record_define(directive)?,
                    DirectiveName::Include | DirectiveName::IncludeNext => {
                        let include = include_target(self.source(directive.loc.file), directive)?;
                        nodes.extend(
                            self.resolve_and_parse_include(&include, directive.arguments_loc())?,
                        );
                    }
                    DirectiveName::Embed => nodes.push(self.expand_embed(directive)?),
                    DirectiveName::Undef => self.record_undef(directive)?,
                    DirectiveName::Pragma => self.record_pragma(directive),
                    DirectiveName::Error => {
                        self.record_directive_diagnostic(directive, Severity::Error)
                    }
                    DirectiveName::Warning => {
                        self.record_directive_diagnostic(directive, Severity::Warning)
                    }
                    DirectiveName::Line
                    | DirectiveName::LineMarker
                    | DirectiveName::Ident
                    | DirectiveName::Null => {}
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

    fn expand_line(&self, source_tokens: &[Span<Token>]) -> PPNode {
        let loc = Span::cover((), source_tokens).spelling;
        let expanded =
            Self::strip_pragma_operator(&self.expand_macros(source_tokens, &mut HashSet::new()));
        Span::new(
            PPNodeKind::Code {
                text: tokens_source(expanded.values()),
                tokens: expanded,
                provenance: self.provenance(loc),
            },
            loc,
            loc,
        )
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
                    limit = usize::try_from(*value).ok();
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
            tokens.push(Span::new(Token::IntLit(i64::from(byte)), loc, loc));
        }
        if !is_empty {
            tokens.extend(suffix);
        }
        Ok(Span::new(
            PPNodeKind::Code {
                text: tokens_source(tokens.values()),
                tokens,
                provenance: self.provenance(loc),
            },
            loc,
            loc,
        ))
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
        let expanded = self.expand_has_embed(
            &self.expand_condition(&directive.arguments),
            directive.loc.file,
        );
        const_expr::Parser::evaluate_with_defined(&expanded, &|macro_name| {
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
                        .with_value(Token::IntLit(found as i64)),
                );
                index += 4;
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

    fn spelling(&self, loc: Loc) -> &str {
        self.source(loc.file)
            .get(loc.offset..loc.offset + loc.length)
            .unwrap_or_default()
    }
}

fn tokens_source<'a>(tokens: impl IntoIterator<Item = &'a Token>) -> String {
    tokens
        .into_iter()
        .map(String::from)
        .collect::<Vec<_>>()
        .join(" ")
}

fn lex(src: &str) -> Vec<Token> {
    lex_spanned(src)
        .into_iter()
        .map(|span| span.value)
        .collect()
}

fn lex_spanned(src: &str) -> Vec<Span<Token>> {
    Lexer::new(FileId(0), src).tokenize()
}
