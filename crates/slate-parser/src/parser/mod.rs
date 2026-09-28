mod asm;
mod attributes;
mod context;
mod decl;
mod declarator;
mod input;
mod ms_asm;
mod stmt;
mod undo;

use crate::ast::*;
use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use crate::const_expr;
use crate::dialect::Dialect;
use crate::error::{FrontendError, ParseError};
use crate::files::{Files, SearchPaths, decode_source_bytes, display_path};
use crate::lexer::Token;
use crate::pp::{
    DirectiveDiagnostic, MacroEntry, MacroOption, PPNode, Preprocessor, PreprocessorInputs,
};
use crate::standard_features::StandardFeatures;
pub(crate) use context::ParseContext;
pub(crate) use decl::matching_brace;
pub(crate) use declarator::{DeclaratorParser, builtin_integer_typedef, is_target_builtin_name};
use input::{Annotation, ParserInput};
use std::cell::{Cell, RefCell};
use std::collections::HashMap;
use std::path::Path;
use std::rc::Rc;

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

struct TokenCursor<'p, 'a> {
    parser: &'p Parser,
    tokens: &'a [Span<Token>],
    pos: usize,
}

impl<'p, 'a> TokenCursor<'p, 'a> {
    fn new(parser: &'p Parser, tokens: &'a [Span<Token>], pos: usize) -> Self {
        Self {
            parser,
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
                let name = name.to_string();
                self.pos += 1;
                Ok(name)
            }
            _ => Err(self.error(message)),
        }
    }
}

impl<'p, 'a> Cursor for TokenCursor<'p, 'a> {
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

// recursive-descent frames, not source nesting levels; well above C's guaranteed 63 parens / 127 blocks
pub(crate) const NESTING_LIMIT: u32 = 1024;

pub struct Parser {
    search: SearchPaths,
    source_name: String,
    source: String,
    files: Files,
    names: NameEnvironment,
    input: Rc<ParserInput>,
    directive_diagnostics: Vec<DirectiveDiagnostic>,
    defines: Vec<String>,
    preprocessor_inputs: PreprocessorInputs,
    forced_roots: Vec<FileId>,
    biggest_alignment: i64,
    dialect: Dialect,
    tags: Rc<RefCell<Vec<Span<TagDefinition>>>>,
    line_starts: HashMap<FileId, Vec<usize>>,
    nesting: Cell<u32>,
}

#[derive(Clone, Copy)]
enum NameBinding {
    Typedef,
    Ordinary,
}

enum NameChange {
    Entered,
    Exited(HashMap<String, NameBinding>),
    Bound {
        name: String,
        previous: Option<NameBinding>,
    },
}

struct NameEnvironment {
    scopes: RefCell<Vec<HashMap<String, NameBinding>>>,
    undo: RefCell<undo::UndoLog<NameChange>>,
}

impl Default for NameEnvironment {
    fn default() -> Self {
        Self {
            scopes: RefCell::new(vec![HashMap::new()]),
            undo: RefCell::default(),
        }
    }
}

pub(super) struct ScopeGuard<'a> {
    names: &'a NameEnvironment,
}

impl Drop for ScopeGuard<'_> {
    fn drop(&mut self) {
        if let Some(scope) = self.names.scopes.borrow_mut().pop() {
            self.names
                .undo
                .borrow_mut()
                .record(|| NameChange::Exited(scope));
        }
    }
}

impl NameEnvironment {
    fn is_typedef(&self, name: &str) -> bool {
        matches!(
            self.scopes
                .borrow()
                .iter()
                .rev()
                .find_map(|scope| scope.get(name)),
            Some(NameBinding::Typedef)
        )
    }

    fn enter(&self) -> ScopeGuard<'_> {
        self.scopes.borrow_mut().push(HashMap::new());
        self.undo.borrow_mut().record(|| NameChange::Entered);
        ScopeGuard { names: self }
    }

    fn bind(&self, name: &str, is_typedef: bool) {
        if let Some(scope) = self.scopes.borrow_mut().last_mut() {
            let previous = scope.insert(
                name.to_string(),
                if is_typedef {
                    NameBinding::Typedef
                } else {
                    NameBinding::Ordinary
                },
            );
            self.undo.borrow_mut().record(|| NameChange::Bound {
                name: name.to_string(),
                previous,
            });
        }
    }

    fn rollback(&self, mark: usize) {
        let undone = self.undo.borrow_mut().rollback(mark);
        let mut scopes = self.scopes.borrow_mut();
        for change in undone {
            match change {
                NameChange::Entered => {
                    scopes.pop();
                }
                NameChange::Exited(scope) => scopes.push(scope),
                NameChange::Bound { name, previous } => {
                    if let Some(scope) = scopes.last_mut() {
                        match previous {
                            Some(binding) => scope.insert(name, binding),
                            None => scope.remove(&name),
                        };
                    }
                }
            }
        }
    }
}

pub(crate) struct ParseCheckpoint<'a> {
    parser: &'a Parser,
    names: usize,
    tags: usize,
    annotations: usize,
    committed: bool,
}

impl ParseCheckpoint<'_> {
    pub(crate) fn commit(mut self) {
        self.committed = true;
    }
}

impl Drop for ParseCheckpoint<'_> {
    fn drop(&mut self) {
        if self.committed {
            self.parser.names.undo.borrow_mut().commit();
            self.parser.input.commit_annotations();
        } else {
            self.parser.names.rollback(self.names);
            self.parser.tags.borrow_mut().truncate(self.tags);
            self.parser.input.rollback_annotations(self.annotations);
        }
    }
}

pub(crate) const FALLBACK_BIGGEST_ALIGNMENT: i64 = 16;

fn resolve_biggest_alignment(
    macros: &foldhash::HashMap<String, MacroEntry>,
    dialect: &Dialect,
) -> i64 {
    macros
        .get("__BIGGEST_ALIGNMENT__")
        .and_then(|entry| const_expr::Parser::evaluate(&entry.definition.replacement, dialect).ok())
        .unwrap_or(FALLBACK_BIGGEST_ALIGNMENT)
}

impl Parser {
    pub(crate) fn cover_tokens<T>(
        &self,
        value: T,
        tokens: &[Span<Token>],
        start: usize,
        end: usize,
    ) -> Span<T> {
        self.input.cover_tokens(value, tokens, start, end)
    }

    pub(crate) fn has_token_slice(&self, tokens: &[Span<Token>]) -> bool {
        self.input.has_token_slice(tokens)
    }

    pub(crate) fn checkpoint(&self) -> ParseCheckpoint<'_> {
        ParseCheckpoint {
            parser: self,
            names: self.names.undo.borrow_mut().mark(),
            tags: self.tags.borrow().len(),
            annotations: self.input.mark_annotations(),
            committed: false,
        }
    }

    pub(crate) fn is_typedef(&self, name: &str) -> bool {
        self.names.is_typedef(name)
    }

    pub(super) fn enter_scope(&self) -> ScopeGuard<'_> {
        self.names.enter()
    }

    pub(crate) fn nesting(&self) -> &Cell<u32> {
        &self.nesting
    }

    pub fn new(search: SearchPaths, dialect: Dialect) -> Self {
        Self {
            search,
            source_name: "<source>".into(),
            source: String::new(),
            files: Files::new(),
            names: NameEnvironment::default(),
            input: Rc::default(),
            directive_diagnostics: Vec::new(),
            defines: Vec::new(),
            preprocessor_inputs: PreprocessorInputs::default(),
            forced_roots: Vec::new(),
            biggest_alignment: FALLBACK_BIGGEST_ALIGNMENT,
            dialect,
            tags: Rc::default(),
            line_starts: HashMap::new(),
            nesting: Cell::new(0),
        }
    }

    pub fn with_defines(mut self, defines: impl IntoIterator<Item = String>) -> Self {
        self.defines = defines.into_iter().collect();
        self
    }

    pub fn with_preprocessor_inputs(mut self, inputs: PreprocessorInputs) -> Self {
        self.preprocessor_inputs = inputs;
        self
    }

    fn prepare_preprocessor(
        &mut self,
        pp: &mut Preprocessor<'_>,
    ) -> Result<Vec<PPNode>, FrontendError> {
        self.forced_roots.clear();
        let options = self
            .defines
            .iter()
            .cloned()
            .map(MacroOption::Define)
            .chain(self.preprocessor_inputs.macros.iter().cloned())
            .collect::<Vec<_>>();
        pp.apply_macro_options(&options)
            .map_err(FrontendError::PP)?;
        for path in &self.preprocessor_inputs.imacros {
            pp.process_forced_file(path).map_err(FrontendError::PP)?;
        }
        let mut nodes = Vec::new();
        for path in &self.preprocessor_inputs.includes {
            let (file, included) = pp.process_forced_file(path).map_err(FrontendError::PP)?;
            self.forced_roots.push(file);
            nodes.extend(included);
        }
        Ok(nodes)
    }

    pub fn dialect(&self) -> &Dialect {
        &self.dialect
    }

    pub fn flavor(&self) -> CompilerFlavor {
        self.dialect.flavor()
    }

    pub fn standard(&self) -> LanguageStandard {
        self.dialect.standard()
    }

    pub fn features(&self) -> StandardFeatures {
        self.dialect.features()
    }

    pub fn directive_diagnostics(&self) -> &[DirectiveDiagnostic] {
        &self.directive_diagnostics
    }

    pub fn parse_source(&mut self, src: &str) -> Result<TranslationUnit, FrontendError> {
        self.source_name = "<main>".into();
        self.source = src.into();
        let search = self.search.clone();
        let dialect = self.dialect.clone();
        let mut pp = Preprocessor::new(&search, &dialect).map_err(FrontendError::PP)?;
        let mut nodes = self.prepare_preprocessor(&mut pp)?;
        nodes.extend(pp.parse_str("<main>", src).map_err(FrontendError::PP)?);
        self.directive_diagnostics = std::mem::take(&mut pp.directive_diagnostics);
        self.biggest_alignment = resolve_biggest_alignment(&pp.macros, &dialect);
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
        let dialect = self.dialect.clone();
        let mut pp = Preprocessor::new(&search, &dialect).map_err(FrontendError::PP)?;
        let mut nodes = self.prepare_preprocessor(&mut pp)?;
        nodes.extend(pp.parse_file(path).map_err(FrontendError::PP)?);
        self.files = pp.files.clone();
        self.directive_diagnostics = std::mem::take(&mut pp.directive_diagnostics);
        self.biggest_alignment = resolve_biggest_alignment(&pp.macros, &dialect);
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

    fn error_at_tokens(
        &self,
        tokens: &[Span<Token>],
        position: usize,
        message: impl Into<String>,
    ) -> ParseError {
        let message = message.into();
        let end = position.saturating_add(1).min(tokens.len());
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

pub(crate) fn span_tokens<T>(
    value: T,
    tokens: &[Span<Token>],
    context: ParseContext<'_>,
) -> Span<T> {
    match context.parser() {
        Some(parser) => parser.cover_tokens(value, tokens, 0, tokens.len()),
        None => Span::cover(value, tokens),
    }
}

fn synthetic_span<T>(value: T) -> Span<T> {
    let loc = crate::ast::Loc::new(FileId(0), 0, 0);
    Span::new(value, loc, loc)
}
