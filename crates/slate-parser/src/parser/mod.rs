mod asm;
mod attributes;
mod decl;
mod declarator;
mod input;
mod stmt;

use crate::ast::*;
use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use crate::const_expr;
use crate::error::{FrontendError, ParseError};
use crate::files::{Files, SearchPaths, decode_source_bytes, display_path};
use crate::lexer::{Lexer, Token};
use crate::pp::{DirectiveDiagnostic, MacroEntry, Preprocessor};
use crate::target_info::TargetInfo;
pub(crate) use decl::matching_brace;
pub(crate) use declarator::{DeclaratorParser, is_target_builtin_name};
use input::{Annotation, ParserInput};
use std::cell::RefCell;
use std::collections::{HashMap, HashSet};
use std::path::Path;
use std::rc::Rc;

fn lex(code: &str) -> Vec<Span<Token>> {
    Lexer::new(FileId(0), code).tokenize()
}

struct Loc<'a> {
    code: &'a str,
    offset: usize,
    length: usize,
}

impl<'a> Loc<'a> {
    fn whole(code: &'a str) -> Self {
        Self {
            code,
            offset: 0,
            length: code.len(),
        }
    }
}

trait Cursor {
    type Error;

    fn tokens(&self) -> &[Span<Token>];
    fn pos(&self) -> usize;
    fn set_pos(&mut self, pos: usize);

    fn peek(&self) -> Option<&Token> {
        self.tokens().get(self.pos()).map(|span| &span.value)
    }

    fn consume(&mut self, token: Token) -> bool {
        if self.peek() == Some(&token) {
            self.set_pos(self.pos() + 1);
            true
        } else {
            false
        }
    }
}

struct Fragment<'p, 'a> {
    parser: &'p Parser,
    code: &'a str,
    tokens: &'a [Span<Token>],
    pos: usize,
}

impl<'p, 'a> Fragment<'p, 'a> {
    fn new(parser: &'p Parser, code: &'a str, tokens: &'a [Span<Token>], pos: usize) -> Self {
        Self {
            parser,
            code,
            tokens,
            pos,
        }
    }

    fn error(&self, message: impl Into<String>) -> ParseError {
        self.parser.error_at_tokens(self.tokens, self.pos, message)
    }

    fn expect(&mut self, token: Token, message: &str) -> Result<(), ParseError> {
        if self.consume(token) {
            Ok(())
        } else {
            Err(self.error(message))
        }
    }

    fn expect_ident(&mut self, message: &str) -> Result<String, ParseError> {
        match self.peek() {
            Some(Token::Ident(name)) => {
                let name = name.clone();
                self.pos += 1;
                Ok(name)
            }
            _ => Err(self.error(message)),
        }
    }
}

impl<'p, 'a> Cursor for Fragment<'p, 'a> {
    type Error = ParseError;

    fn tokens(&self) -> &[Span<Token>] {
        self.tokens
    }

    fn pos(&self) -> usize {
        self.pos
    }

    fn set_pos(&mut self, pos: usize) {
        self.pos = pos;
    }
}

#[derive(Clone)]
pub struct Parser {
    search: SearchPaths,
    source_name: String,
    source: String,
    files: Files,
    names: Rc<RefCell<NameEnvironment>>,
    input: Rc<ParserInput>,
    directive_diagnostics: Vec<DirectiveDiagnostic>,
    defines: Vec<String>,
    biggest_alignment: i64,
    flavor: CompilerFlavor,
    standard: LanguageStandard,
    target: TargetInfo,
    options: Option<crate::compiler_options::CompilerOptions>,
    tags: Rc<RefCell<Vec<Span<TagDefinition>>>>,
    line_starts: HashMap<FileId, Vec<usize>>,
}

#[derive(Default)]
struct NameEnvironment {
    scopes: Vec<HashMap<String, bool>>,
}

pub(super) struct ScopeGuard {
    names: Rc<RefCell<NameEnvironment>>,
}

impl Drop for ScopeGuard {
    fn drop(&mut self) {
        self.names.borrow_mut().leave();
    }
}

impl NameEnvironment {
    fn typedef_names(&self) -> HashSet<String> {
        let mut visible = HashMap::new();
        for scope in &self.scopes {
            for (name, is_typedef) in scope {
                visible.insert(name.clone(), *is_typedef);
            }
        }
        visible
            .into_iter()
            .filter_map(|(name, is_typedef)| is_typedef.then_some(name))
            .collect()
    }

    fn enter(&mut self) {
        self.scopes.push(HashMap::new());
    }

    fn leave(&mut self) {
        self.scopes.pop();
    }

    fn bind(&mut self, name: &str, is_typedef: bool) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.insert(name.to_string(), is_typedef);
        }
    }
}

pub(crate) const FALLBACK_BIGGEST_ALIGNMENT: i64 = 16;

fn resolve_biggest_alignment(macros: &HashMap<String, MacroEntry>) -> i64 {
    macros
        .get("__BIGGEST_ALIGNMENT__")
        .and_then(|entry| const_expr::Parser::evaluate(&entry.definition.replacement).ok())
        .unwrap_or(FALLBACK_BIGGEST_ALIGNMENT)
}

impl Parser {
    pub(super) fn enter_scope(&self) -> ScopeGuard {
        self.names.borrow_mut().enter();
        ScopeGuard {
            names: Rc::clone(&self.names),
        }
    }

    pub fn new(search: SearchPaths) -> Self {
        Self {
            search,
            source_name: "<source>".into(),
            source: String::new(),
            files: Files::new(),
            names: Rc::new(RefCell::new(NameEnvironment {
                scopes: vec![HashMap::new()],
            })),
            input: Rc::default(),
            directive_diagnostics: Vec::new(),
            defines: Vec::new(),
            biggest_alignment: FALLBACK_BIGGEST_ALIGNMENT,
            flavor: CompilerFlavor::default(),
            standard: LanguageStandard::default(),
            target: TargetInfo::default(),
            options: None,
            tags: Rc::default(),
            line_starts: HashMap::new(),
        }
    }

    pub fn with_defines(mut self, defines: impl IntoIterator<Item = String>) -> Self {
        self.defines = defines.into_iter().collect();
        self
    }

    pub fn with_flavor(mut self, flavor: CompilerFlavor) -> Self {
        self.flavor = flavor;
        self
    }

    pub fn flavor(&self) -> CompilerFlavor {
        self.flavor
    }

    pub fn with_standard(mut self, standard: LanguageStandard) -> Self {
        self.standard = standard;
        self
    }

    pub fn standard(&self) -> LanguageStandard {
        self.standard
    }

    pub fn with_target(mut self, target: TargetInfo) -> Self {
        self.target = target;
        self
    }

    pub fn with_options(mut self, options: crate::compiler_options::CompilerOptions) -> Self {
        self.options = Some(options);
        self
    }

    fn effective_options(&self) -> crate::compiler_options::CompilerOptions {
        self.options
            .clone()
            .unwrap_or_else(|| crate::compiler_options::CompilerOptions::for_flavor(self.flavor))
    }

    pub fn directive_diagnostics(&self) -> &[DirectiveDiagnostic] {
        &self.directive_diagnostics
    }

    pub fn parse_source(&mut self, src: &str) -> Result<TranslationUnit, FrontendError> {
        self.source_name = "<main>".into();
        self.source = src.into();
        let search = self.search.clone();
        let mut pp = Preprocessor::new(&search, self.standard);
        let options = self.effective_options();
        pp.configure(options.effective_target(self.target), &options, self.flavor)
            .map_err(FrontendError::PP)?;
        pp.define_all(&self.defines).map_err(FrontendError::PP)?;
        let nodes = pp.parse_str("<main>", src).map_err(FrontendError::PP)?;
        self.directive_diagnostics = std::mem::take(&mut pp.directive_diagnostics);
        self.biggest_alignment = resolve_biggest_alignment(&pp.macros);
        self.line_starts = pp.line_starts.clone();
        let root_file = pp.main_file.ok_or_else(|| {
            FrontendError::Parse(ParseError::new(
                self.source_name.clone(),
                self.source.clone(),
                0,
                0,
                "preprocessor did not produce a main file",
            ))
        })?;
        self.parse_input(ParserInput::new(nodes), root_file)
            .map_err(FrontendError::Parse)
    }

    pub fn parse_file(&mut self, path: &Path) -> Result<(TranslationUnit, Files), FrontendError> {
        self.source_name = display_path(path);
        self.source = std::fs::read(path)
            .map(|bytes| decode_source_bytes(&bytes))
            .map_err(|error| {
                ParseError::new(
                    self.source_name.clone(),
                    "",
                    0,
                    0,
                    format!("failed to read source: {error}"),
                )
            })
            .map_err(FrontendError::Parse)?;
        let search = self.search.clone();
        let mut pp = Preprocessor::new(&search, self.standard);
        let options = self.effective_options();
        pp.configure(options.effective_target(self.target), &options, self.flavor)
            .map_err(FrontendError::PP)?;
        pp.define_all(&self.defines).map_err(FrontendError::PP)?;
        let nodes = pp.parse_file(path).map_err(FrontendError::PP)?;
        self.files = pp.files.clone();
        self.directive_diagnostics = std::mem::take(&mut pp.directive_diagnostics);
        self.biggest_alignment = resolve_biggest_alignment(&pp.macros);
        self.line_starts = pp.line_starts.clone();
        let root_file = pp.main_file.ok_or_else(|| {
            FrontendError::Parse(ParseError::new(
                self.source_name.clone(),
                self.source.clone(),
                0,
                0,
                "preprocessor did not produce a main file",
            ))
        })?;
        let ast = self.parse_input(ParserInput::new(nodes), root_file);
        ast.map(|ast| (ast, pp.files)).map_err(FrontendError::Parse)
    }

    pub fn parse_declaration(&self, code: &str) -> Result<Declaration, ParseError> {
        let tokens = lex(code);
        self.parse_declaration_tokens(&tokens)
    }

    fn error_at(&self, loc: Loc<'_>, message: impl Into<String>) -> ParseError {
        let Loc {
            code,
            offset,
            length,
        } = loc;
        if let Some(base) = self.source.find(code) {
            return ParseError::new(
                self.source_name.clone(),
                self.source.clone(),
                base + offset,
                length.max(1),
                message,
            );
        }
        for path in self.files.paths() {
            let Ok(contents) = std::fs::read(path).map(|bytes| decode_source_bytes(&bytes)) else {
                continue;
            };
            if let Some(base) = contents.find(code) {
                return ParseError::new(
                    display_path(path),
                    contents,
                    base + offset,
                    length.max(1),
                    message,
                );
            }
        }
        ParseError::new(
            self.source_name.clone(),
            self.source.clone(),
            offset,
            length.max(1),
            message,
        )
    }

    fn error_at_tokens(
        &self,
        tokens: &[Span<Token>],
        position: usize,
        message: impl Into<String>,
    ) -> ParseError {
        let message = message.into();
        if tokens.is_empty() {
            return self.error_at(Loc::whole(""), message);
        }
        let end = position.min(tokens.len() - 1) + 1;
        for token in tokens[..end].iter().rev() {
            let loc = token.expansion;
            let Some(path) = self.files.get_path(loc.file) else {
                continue;
            };
            let Ok(source) = std::fs::read(path).map(|bytes| decode_source_bytes(&bytes)) else {
                continue;
            };
            if loc.offset >= source.len() {
                continue;
            }
            let length = loc.length.max(1).min(source.len() - loc.offset);
            return ParseError::new(display_path(path), source, loc.offset, length, message);
        }
        let offset = self.source.len().saturating_sub(1);
        ParseError::new(
            self.source_name.clone(),
            self.source.clone(),
            offset,
            usize::from(!self.source.is_empty()),
            message,
        )
    }

    pub(crate) fn define_tag(&self, mut definition: Span<TagDefinition>) -> TagId {
        let mut tags = self.tags.borrow_mut();
        let id = TagId(tags.len());
        definition.value.id = id;
        tags.push(definition);
        id
    }
}

pub(crate) fn string_literal_content(token: &Token) -> Option<&str> {
    match token {
        Token::StringLit(value)
        | Token::Utf8StringLit(value)
        | Token::Utf16StringLit(value)
        | Token::Utf32StringLit(value)
        | Token::WideStringLit(value) => Some(value),
        _ => None,
    }
}

fn span_tokens<T>(value: T, tokens: &[Span<Token>]) -> Span<T> {
    Span::cover(value, tokens)
}

fn synthetic_span<T>(value: T) -> Span<T> {
    let loc = crate::ast::Loc::new(FileId(0), 0, 0);
    Span::new(value, loc, loc)
}
