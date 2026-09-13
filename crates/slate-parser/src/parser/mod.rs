mod asm;
mod attributes;
mod decl;
mod declarator;
mod stmt;

use crate::ast::*;
use crate::compiler_args::CompilerFlavor;
use crate::const_expr;
use crate::error::{FrontendError, ParseError};
use crate::files::{Files, SearchPaths, decode_source_bytes, display_path};
use crate::lexer::{Lexer, Token};
use crate::pp::{DirectiveDiagnostic, MacroEntry, PPNode, PPNodeKind, Preprocessor};
pub(crate) use attributes::apply_vector_attributes;
pub(crate) use decl::matching_brace;
pub(crate) use declarator::{DeclaratorParser, is_target_builtin_name};
use std::collections::{HashMap, HashSet};
use std::path::Path;

fn lex(code: &str) -> Vec<Span<Token>> {
    Lexer::new(FileId(0), code).tokenize()
}

fn synthetic(token: Token) -> Span<Token> {
    let loc = crate::ast::Loc::new(FileId(0), 0, 0);
    Span::new(token, loc, loc)
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

    fn at(code: &'a str, offset: usize, length: usize) -> Self {
        Self {
            code,
            offset,
            length,
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
    typedef_names: HashSet<String>,
    directive_diagnostics: Vec<DirectiveDiagnostic>,
    defines: Vec<String>,
    biggest_alignment: i64,
    flavor: CompilerFlavor,
}

pub(crate) const FALLBACK_BIGGEST_ALIGNMENT: i64 = 16;

fn resolve_biggest_alignment(macros: &HashMap<String, MacroEntry>) -> i64 {
    macros
        .get("__BIGGEST_ALIGNMENT__")
        .and_then(|entry| const_expr::Parser::evaluate(&entry.definition.replacement).ok())
        .unwrap_or(FALLBACK_BIGGEST_ALIGNMENT)
}

impl Parser {
    pub fn new(search: SearchPaths) -> Self {
        Self {
            search,
            source_name: "<source>".into(),
            source: String::new(),
            files: Files::new(),
            typedef_names: HashSet::new(),
            directive_diagnostics: Vec::new(),
            defines: Vec::new(),
            biggest_alignment: FALLBACK_BIGGEST_ALIGNMENT,
            flavor: CompilerFlavor::default(),
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

    pub fn directive_diagnostics(&self) -> &[DirectiveDiagnostic] {
        &self.directive_diagnostics
    }

    pub fn parse_source(&mut self, src: &str) -> Result<TranslationUnit, FrontendError> {
        self.source_name = "<main>".into();
        self.source = src.into();
        let search = self.search.clone();
        let mut pp = Preprocessor::new(&search);
        pp.define_all(&self.defines).map_err(FrontendError::PP)?;
        let nodes = pp.parse_str("<main>", src).map_err(FrontendError::PP)?;
        self.directive_diagnostics = std::mem::take(&mut pp.directive_diagnostics);
        self.biggest_alignment = resolve_biggest_alignment(&pp.macros);
        let root_file = pp.main_file.expect("parse_str sets main_file");
        self.parse_nodes(&nodes, root_file)
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
        let mut pp = Preprocessor::new(&search);
        pp.define_all(&self.defines).map_err(FrontendError::PP)?;
        let nodes = pp.parse_file(path).map_err(FrontendError::PP)?;
        self.files = pp.files.clone();
        self.directive_diagnostics = std::mem::take(&mut pp.directive_diagnostics);
        self.biggest_alignment = resolve_biggest_alignment(&pp.macros);
        let root_file = pp.main_file.expect("parse_file sets main_file");
        let ast = self.parse_nodes(&nodes, root_file);
        ast.map(|ast| (ast, pp.files)).map_err(FrontendError::Parse)
    }

    pub fn parse_declaration(&self, code: &str) -> Result<Declaration, ParseError> {
        let tokens = lex(code);
        self.parse_declaration_tokens(code, &tokens)
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

    fn node_text<'a>(&self, node: &'a PPNode) -> &'a str {
        match &node.value {
            PPNodeKind::Comment { text, .. } => text,
            PPNodeKind::Code { text, .. } => text,
        }
    }

    fn node_tokens(&self, node: &PPNode) -> Vec<Span<Token>> {
        let PPNodeKind::Code { tokens, .. } = &node.value else {
            return lex(self.node_text(node));
        };
        tokens.clone()
    }

    fn nodes_tokens(&self, nodes: &[PPNode]) -> Vec<Span<Token>> {
        nodes
            .iter()
            .filter(|node| matches!(node.value, PPNodeKind::Code { .. }))
            .flat_map(|node| self.node_tokens(node))
            .collect()
    }

    fn node_provenance(&self, node: &PPNode) -> Provenance {
        match &node.value {
            PPNodeKind::Comment { provenance, .. } => *provenance,
            PPNodeKind::Code { provenance, .. } => *provenance,
        }
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

pub(crate) fn concatenated_string_literal(
    previous: &Token,
    next: &Token,
    content: String,
) -> Token {
    match (previous, next) {
        (Token::StringLit(_), Token::StringLit(_)) => Token::StringLit(content),
        (Token::StringLit(_), other) => match other {
            Token::Utf8StringLit(_) => Token::Utf8StringLit(content),
            Token::Utf16StringLit(_) => Token::Utf16StringLit(content),
            Token::Utf32StringLit(_) => Token::Utf32StringLit(content),
            Token::WideStringLit(_) => Token::WideStringLit(content),
            _ => unreachable!("caller only merges string literal tokens"),
        },
        (Token::Utf8StringLit(_), _) => Token::Utf8StringLit(content),
        (Token::Utf16StringLit(_), _) => Token::Utf16StringLit(content),
        (Token::Utf32StringLit(_), _) => Token::Utf32StringLit(content),
        (Token::WideStringLit(_), _) => Token::WideStringLit(content),
        _ => unreachable!("caller only merges string literal tokens"),
    }
}

fn coalesce_string_literals(tokens: &[Span<Token>]) -> Vec<Span<Token>> {
    let mut result: Vec<Span<Token>> = Vec::with_capacity(tokens.len());
    for token in tokens {
        let Some(previous) = result.last_mut() else {
            result.push(token.clone());
            continue;
        };
        match (
            string_literal_content(&previous.value),
            string_literal_content(&token.value),
        ) {
            (Some(left), Some(right)) => {
                let content = format!("{left}{right}");
                previous.value =
                    concatenated_string_literal(&previous.value, &token.value, content);
                previous.spelling = previous.spelling.through(token.spelling);
                previous.expansion = previous.expansion.through(token.expansion);
            }
            _ => result.push(token.clone()),
        }
    }
    result
}

fn span_tokens<T>(value: T, tokens: &[Span<Token>]) -> Span<T> {
    Span::cover(value, tokens)
}

fn span_pp_nodes<T>(value: T, nodes: &[PPNode]) -> Span<T> {
    let Some(first) = nodes.first() else {
        return synthetic_span(value);
    };
    let last = nodes.last().unwrap();
    Span::new(
        value,
        first.spelling.through(last.spelling),
        first.expansion.through(last.expansion),
    )
}

fn span_decl_result(result: (Vec<Decl>, usize), nodes: &[PPNode]) -> (Vec<SpannedDecl>, usize) {
    let (decls, consumed) = result;
    (
        decls
            .into_iter()
            .map(|decl| span_pp_nodes(decl, &nodes[..consumed]))
            .collect(),
        consumed,
    )
}

fn synthetic_span<T>(value: T) -> Span<T> {
    let loc = crate::ast::Loc::new(FileId(0), 0, 0);
    Span::new(value, loc, loc)
}
