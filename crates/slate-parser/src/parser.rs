use crate::ast::*;
use crate::const_expr;
use crate::error::{FrontendError, ParseError};
use crate::files::{Files, SearchPaths, display_path};
use crate::lexer::{Keyword, Lexer, Token, TokenSpanExt};
use crate::pp::{DirectiveDiagnostic, PPNode, PPNodeKind, Preprocessor};
use crate::reachability::{filter_translation_unit, mark_unreachable};
use miette::Diagnostic;
use std::collections::HashSet;
use std::path::Path;
use thiserror::Error;

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

pub struct Parser {
    search: SearchPaths,
    source_name: String,
    source: String,
    files: Files,
    typedef_names: HashSet<String>,
    directive_diagnostics: Vec<DirectiveDiagnostic>,
    defines: Vec<String>,
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
        }
    }

    pub fn with_defines(mut self, defines: impl IntoIterator<Item = String>) -> Self {
        self.defines = defines.into_iter().collect();
        self
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
        let root_file = pp.main_file.expect("parse_str sets main_file");
        self.parse_nodes(&nodes, root_file)
            .map_err(FrontendError::Parse)
    }

    pub fn parse_file(&mut self, path: &Path) -> Result<(TranslationUnit, Files), FrontendError> {
        self.source_name = display_path(path);
        self.source = std::fs::read_to_string(path)
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
        let root_file = pp.main_file.expect("parse_file sets main_file");
        let ast = self.parse_nodes(&nodes, root_file);
        ast.map(|ast| (ast, pp.files)).map_err(FrontendError::Parse)
    }

    pub fn parse_declaration(&self, code: &str) -> Result<Declaration, ParseError> {
        let tokens = lex(code);
        self.parse_declaration_tokens(code, &tokens)
    }

    fn parse_declaration_tokens(
        &self,
        _code: &str,
        tokens: &[Span<Token>],
    ) -> Result<Declaration, ParseError> {
        let mut parser = DeclaratorParser {
            tokens,
            pos: 0,
            typedef_names: &self.typedef_names,
        };
        while parser.peek() == Some(&Token::Ident("__extension__".to_string())) {
            parser.pos += 1;
        }
        let (mut attributes, position) = parse_attribute_groups(parser.tokens, parser.pos)
            .map_err(|error| self.error_at_tokens(tokens, parser.pos, error))?;
        parser.pos = position;
        let mut qualifiers = Qualifiers::default();
        let mut storage = StorageClass::None;
        let mut is_thread_local = false;
        let mut is_inline = false;
        let mut is_noreturn = false;
        let mut is_constexpr = false;
        let gnu_auto_type = parser.matches(Token::Ident("__auto_type".into()));
        loop {
            let (more_attributes, position) = parse_attribute_groups(parser.tokens, parser.pos)
                .map_err(|error| self.error_at_tokens(tokens, parser.pos, error))?;
            if position != parser.pos {
                attributes.extend(more_attributes);
                parser.pos = position;
                continue;
            }
            if let Some(qualifier) = parser.take_qualifier() {
                match qualifier {
                    Keyword::Const => qualifiers.is_const = true,
                    Keyword::Volatile => qualifiers.is_volatile = true,
                    Keyword::Restrict => qualifiers.is_restrict = true,
                    Keyword::Atomic => qualifiers.is_atomic = true,
                    _ => unreachable!(),
                }
                continue;
            }
            if parser.matches(Token::Keyword(Keyword::Inline)) {
                is_inline = true;
                continue;
            }
            if parser.matches(Token::Keyword(Keyword::Noreturn)) {
                is_noreturn = true;
                continue;
            }
            if parser.matches(Token::Keyword(Keyword::Constexpr)) {
                is_constexpr = true;
                continue;
            }
            if parser.matches(Token::Keyword(Keyword::ThreadLocal)) {
                if is_thread_local {
                    return Err(self.error_at_tokens(
                        tokens,
                        parser.pos - 1,
                        "duplicate `_Thread_local`",
                    ));
                }
                is_thread_local = true;
                continue;
            }
            if let Some(Token::Keyword(keyword)) = parser.peek() {
                let next_storage = match *keyword {
                    Keyword::Typedef => StorageClass::Typedef,
                    Keyword::Extern => StorageClass::Extern,
                    Keyword::Static => StorageClass::Static,
                    Keyword::Auto => StorageClass::Auto,
                    Keyword::Register => StorageClass::Register,
                    _ => break,
                };
                if storage != StorageClass::None {
                    return Err(self.error_at_tokens(
                        tokens,
                        parser.pos,
                        "multiple storage classes",
                    ));
                }
                storage = next_storage;
                parser.matches(Token::Keyword(*keyword));
            } else {
                break;
            }
        }
        let ty = if gnu_auto_type
            || storage == StorageClass::Auto && matches!(parser.peek(), Some(Token::Ident(_)))
        {
            CType::TargetBuiltin("__auto_type".into())
        } else {
            parser
                .parse_base_type()
                .map_err(|error| self.error_at_tokens(tokens, parser.pos, error.to_string()))?
        };
        while let Some(qualifier) = parser.take_qualifier() {
            match qualifier {
                Keyword::Const => qualifiers.is_const = true,
                Keyword::Volatile => qualifiers.is_volatile = true,
                Keyword::Restrict => qualifiers.is_restrict = true,
                Keyword::Atomic => qualifiers.is_atomic = true,
                _ => unreachable!(),
            }
        }
        let (mid_attributes, position) = parse_attribute_groups(parser.tokens, parser.pos)
            .map_err(|error| self.error_at_tokens(tokens, parser.pos, error))?;
        parser.pos = position;
        attributes.extend(mid_attributes);
        let declarator = if parser.peek() == Some(&Token::Semi) {
            Declarator::Abstract
        } else {
            if !matches!(
                parser.peek(),
                Some(&Token::Ident(_)) | Some(&Token::LParen) | Some(&Token::Star)
            ) {
                return Err(self.error_at_tokens(tokens, parser.pos, "expected declarator"));
            }
            parser
                .parse_declarator(false)
                .map_err(|error| self.error_at_tokens(tokens, parser.pos, error.to_string()))?
        };
        let (trailing_attributes, position) = parse_attribute_groups(parser.tokens, parser.pos)
            .map_err(|error| self.error_at_tokens(tokens, parser.pos, error))?;
        parser.pos = position;
        attributes.extend(trailing_attributes);
        let initializer = if parser.matches(Token::Equal) {
            if parser.peek() == Some(&Token::LParen)
                && parser.tokens.value_at(parser.pos + 1) == Some(&Token::LBrace)
            {
                let close = matching_brace(parser.tokens, parser.pos + 1)
                    .ok_or_else(|| self.error_at_tokens(tokens, parser.pos, "expected `}`"))?;
                if parser.tokens.value_at(close + 1) != Some(&Token::RParen) {
                    return Err(self.error_at_tokens(tokens, close, "expected `)`"));
                }
                let body =
                    self.parse_stmts_from_tokens(_code, &parser.tokens[parser.pos + 2..close])?;
                let start = parser.pos;
                parser.pos = close + 2;
                Some(Initializer::Expr(span_tokens(
                    Expr::StatementExpression(body),
                    &parser.tokens[start..parser.pos],
                )))
            } else {
                Some(parser.parse_initializer(&self.typedef_names))
            }
        } else {
            None
        };
        if parser.peek() != Some(&Token::Semi) {
            return Err(self.error_at_tokens(tokens, parser.pos, "expected `;`"));
        }
        parser.pos += 1;
        if parser.peek().is_some() {
            return Err(self.error_at_tokens(
                tokens,
                parser.pos,
                "unexpected tokens after declaration",
            ));
        }
        Ok(Declaration {
            specifiers: DeclarationSpecifiers {
                ty: apply_vector_attributes(ty, &attributes),
                qualifiers,
                storage,
                is_thread_local,
                is_inline,
                is_noreturn,
                is_constexpr,
            },
            declarator,
            initializer,
            attributes,
        })
    }

    fn parse_field_declaration_tokens(
        &self,
        code: &str,
        tokens: &[Span<Token>],
    ) -> Result<Declaration, ParseError> {
        let Some(colon) = top_level_token(tokens, &Token::Colon) else {
            return self.parse_declaration_tokens(code, tokens);
        };
        let Some(semi) = top_level_token(tokens, &Token::Semi) else {
            return Err(self.error_at_tokens(tokens, tokens.len(), "expected `;`"));
        };
        let mut declaration_tokens = tokens[..colon].to_vec();
        declaration_tokens.push(tokens[semi].clone());
        self.parse_declaration_tokens(code, &declaration_tokens)
    }

    fn parse_static_assert(
        &self,
        code: &str,
        tokens: &[Span<Token>],
    ) -> Result<StaticAssert, ParseError> {
        if tokens.value_at(1) != Some(&Token::LParen) {
            return Err(self.error_at_tokens(tokens, 1, "expected `(` after static assertion"));
        }
        let close = matching_paren(tokens, 1)
            .ok_or_else(|| self.error_at_tokens(tokens, 1, "expected `)`"))?;
        if tokens.value_at(close + 1) != Some(&Token::Semi) {
            return Err(self.error_at_tokens(tokens, close + 1, "expected `;`"));
        }
        let arguments = &tokens[2..close];
        let comma = top_level_token(arguments, &Token::Comma);
        let condition_end = comma.unwrap_or(arguments.len());
        let condition = self.parse_expression(code, &arguments[..condition_end])?;
        let message = comma
            .map(|comma| {
                if let [message] = &arguments[comma + 1..]
                    && let Token::StringLit(message) = &message.value
                {
                    Ok(message.clone())
                } else {
                    Err(self.error_at_tokens(
                        arguments,
                        comma + 1,
                        "expected static assertion message",
                    ))
                }
            })
            .transpose()?;
        Ok(StaticAssert { condition, message })
    }

    fn parse_nodes(
        &mut self,
        nodes: &[PPNode],
        root_file: FileId,
    ) -> Result<TranslationUnit, ParseError> {
        let ast = filter_translation_unit(
            &TranslationUnit {
                decls: self.parse_decls(nodes)?,
            },
            root_file,
        );
        Ok(ast)
    }

    fn parse_decls(&mut self, nodes: &[PPNode]) -> Result<Vec<SpannedDecl>, ParseError> {
        let mut decls = Vec::new();
        let mut i = 0;
        while i < nodes.len() {
            if matches!(&nodes[i].value, PPNodeKind::Code { text, .. } if lex(text).is_empty()) {
                i += 1;
                continue;
            }
            let (new_decls, consumed) = self.parse_top_level_item(&nodes[i..])?;
            for decl in new_decls {
                self.record_typedefs(&decl.value);
                decls.push(decl);
            }
            i += consumed;
        }
        Ok(decls)
    }

    fn parse_top_level_item(
        &mut self,
        nodes: &[PPNode],
    ) -> Result<(Vec<SpannedDecl>, usize), ParseError> {
        match &nodes[0].value {
            PPNodeKind::Comment { text, provenance } => Ok((
                vec![nodes[0].clone().with_value(Decl::Comment {
                    text: text.clone(),
                    loc: nodes[0].expansion,
                    provenance: *provenance,
                })],
                1,
            )),
            PPNodeKind::Code {
                text, provenance, ..
            } if text.trim_start().starts_with("typedef") => {
                let tokens = self.node_tokens(&nodes[0]);
                let has_inline_body = matches!(
                    tokens
                        .values()
                        .find(|token| !matches!(token, Token::Keyword(Keyword::Typedef))),
                    Some(Token::Keyword(
                        Keyword::Struct | Keyword::Union | Keyword::Enum
                    ))
                ) && tokens.contains_value(&Token::LBrace);
                if has_inline_body {
                    return self
                        .parse_tag_definition(nodes)
                        .map(|result| span_decl_result(result, nodes));
                }
                let span = declaration_node_span(nodes);
                let typedef_text = if span == 1 {
                    text.to_string()
                } else {
                    join_node_text(&nodes[..span])
                };
                let typedef_tokens = self.nodes_tokens(&nodes[..span]);
                let Some(semi) = typedef_tokens
                    .last()
                    .filter(|token| token.value == Token::Semi)
                    .cloned()
                else {
                    return Err(self.error_at_tokens(
                        &typedef_tokens,
                        typedef_tokens.len(),
                        "expected `;`",
                    ));
                };
                let declaration_tokens = &typedef_tokens[..typedef_tokens.len() - 1];
                let parts = split_top_level(declaration_tokens, &Token::Comma);
                let prefix = parts.first().map_or(Vec::new(), |part| {
                    part[..self.declaration_prefix_end(part)].to_vec()
                });
                let mut typedefs = Vec::new();
                for (index, mut part) in parts.into_iter().enumerate() {
                    if index != 0 {
                        let mut with_prefix = prefix.clone();
                        with_prefix.append(&mut part);
                        part = with_prefix;
                    }
                    part.push(semi.clone());
                    let (name, ty, attributes) = self.parse_typedef_line(&typedef_text, &part)?;
                    typedefs.push(span_pp_nodes(
                        Decl::Typedef {
                            name,
                            ty,
                            provenance: self.node_provenance(&nodes[0]),
                            attributes,
                        },
                        &nodes[..span],
                    ));
                }
                Ok((typedefs, span))
            }
            PPNodeKind::Code { .. } => {
                let tokens = self.node_tokens(&nodes[0]);
                if tokens.value_at(0) == Some(&Token::Keyword(Keyword::StaticAssert)) {
                    return Ok((
                        vec![nodes[0].clone().with_value(Decl::StaticAssert {
                            assertion:
                                self.parse_static_assert(self.node_text(&nodes[0]), &tokens)?,
                            provenance: self.node_provenance(&nodes[0]),
                        })],
                        1,
                    ));
                }
                let first_lbrace = top_level_token(&tokens, &Token::LBrace);
                let first_equal = top_level_token(&tokens, &Token::Equal);
                if matches!(
                    tokens.as_tokens().as_slice(),
                    [
                        Token::Keyword(Keyword::Extern),
                        Token::StringLit(_),
                        Token::LBrace
                    ]
                ) {
                    return self.parse_linkage_spec_block(nodes);
                }
                let tag_keyword_index = {
                    let mut index = 0;
                    while matches!(
                        tokens.value_at(index),
                        Some(Token::Keyword(
                            Keyword::Static
                                | Keyword::Extern
                                | Keyword::Auto
                                | Keyword::Register
                                | Keyword::Inline
                                | Keyword::Noreturn
                                | Keyword::Constexpr
                                | Keyword::ThreadLocal
                                | Keyword::Const
                                | Keyword::Volatile
                                | Keyword::Restrict
                                | Keyword::Atomic
                        ))
                    ) {
                        index += 1;
                    }
                    index
                };
                if matches!(
                    tokens.value_at(tag_keyword_index),
                    Some(Token::Keyword(
                        Keyword::Struct | Keyword::Union | Keyword::Enum
                    ))
                ) && first_lbrace.is_some_and(|brace_index| {
                    first_equal.is_none_or(|equal_index| brace_index < equal_index)
                        && brace_index
                            .checked_sub(1)
                            .and_then(|index| tokens.value_at(index))
                            != Some(&Token::RParen)
                }) {
                    return self
                        .parse_tag_definition(nodes)
                        .map(|result| span_decl_result(result, nodes));
                }
                let has_brace_initializer = matches!(
                    (first_lbrace, first_equal),
                    (Some(brace_index), Some(equal_index)) if equal_index < brace_index
                );
                let item_span = if has_brace_initializer {
                    declaration_node_span(nodes)
                } else if paren_depth(&tokens) > 0
                    || !tokens.contains_value(&Token::Semi)
                        && !tokens.contains_value(&Token::LBrace)
                {
                    signature_node_span(nodes)
                } else {
                    1
                };
                let joined_item_text;
                let (item_text, item_tokens): (&str, Vec<Span<Token>>) = if item_span == 1 {
                    (self.node_text(&nodes[0]), tokens)
                } else {
                    joined_item_text = join_node_text(&nodes[..item_span]);
                    (&joined_item_text, self.nodes_tokens(&nodes[..item_span]))
                };
                let first_lbrace = top_level_token(&item_tokens, &Token::LBrace).or_else(|| {
                    (paren_depth(&item_tokens) > 0)
                        .then(|| {
                            item_tokens
                                .values()
                                .position(|token| *token == Token::LBrace)
                        })
                        .flatten()
                });
                let first_equal = top_level_token(&item_tokens, &Token::Equal);
                let looks_like_declaration = match (first_lbrace, first_equal) {
                    (None, _) => true,
                    (Some(_), None) => false,
                    (Some(brace_index), Some(equal_index)) => equal_index < brace_index,
                };
                if item_tokens.contains_value(&Token::Semi) && looks_like_declaration {
                    let declaration_tokens = if item_tokens
                        .last()
                        .is_some_and(|token| token.value == Token::Semi)
                    {
                        &item_tokens[..item_tokens.len() - 1]
                    } else {
                        &item_tokens
                    };
                    let parts = split_top_level(declaration_tokens, &Token::Comma);
                    let prefix = parts.first().map_or(Vec::new(), |part| {
                        part[..self.declaration_prefix_end(part)].to_vec()
                    });
                    let mut declarations = Vec::new();
                    for (index, mut part) in parts
                        .into_iter()
                        .filter(|part| !part.is_empty())
                        .enumerate()
                    {
                        if index != 0 {
                            let mut with_prefix = prefix.clone();
                            with_prefix.append(&mut part);
                            part = with_prefix;
                        }
                        part.push(synthetic(Token::Semi));
                        declarations.push(span_pp_nodes(
                            Decl::Declaration {
                                declaration: self.parse_declaration_tokens(item_text, &part)?,
                                provenance: self.node_provenance(&nodes[0]),
                            },
                            &nodes[..item_span],
                        ));
                    }
                    return Ok((declarations, item_span));
                }
                let (func, consumed) = self.parse_function(nodes)?;
                Ok((
                    vec![span_pp_nodes(Decl::Function(func), &nodes[..consumed])],
                    consumed,
                ))
            }
        }
    }

    fn parse_tag_definition(&self, nodes: &[PPNode]) -> Result<(Vec<Decl>, usize), ParseError> {
        let code = self.node_text(&nodes[0]);
        let tokens = self.node_tokens(&nodes[0]);
        let is_typedef = tokens.value_at(0) == Some(&Token::Keyword(Keyword::Typedef));
        let tokens = if is_typedef {
            &tokens[1..]
        } else {
            &tokens[..]
        };
        let mut skip = 0;
        while matches!(
            tokens.value_at(skip),
            Some(Token::Keyword(
                Keyword::Static
                    | Keyword::Extern
                    | Keyword::Auto
                    | Keyword::Register
                    | Keyword::Inline
                    | Keyword::Noreturn
                    | Keyword::Constexpr
                    | Keyword::ThreadLocal
                    | Keyword::Const
                    | Keyword::Volatile
                    | Keyword::Restrict
                    | Keyword::Atomic
            ))
        ) {
            skip += 1;
        }
        let tokens = &tokens[skip..];
        let kind = match tokens.value_at(0) {
            Some(Token::Keyword(Keyword::Struct)) => TagKind::Struct,
            Some(Token::Keyword(Keyword::Union)) => TagKind::Union,
            Some(Token::Keyword(Keyword::Enum)) => TagKind::Enum,
            _ => return Err(self.error_at(Loc::whole(code), "expected record or enum")),
        };
        let (mut attributes, name_index) = parse_record_attributes(tokens)
            .map_err(|error| self.error_at(Loc::whole(code), error))?;
        let name = match tokens.value_at(name_index) {
            Some(Token::Ident(name)) => Some(name.clone()),
            Some(Token::LBrace) => None,
            _ => return Err(self.error_at(Loc::whole(code), "expected tag name or `{`")),
        };

        if let Some(open_brace_idx) = tokens[name_index..]
            .values()
            .position(|token| *token == Token::LBrace)
            .map(|position| name_index + position)
            && let Some(same_line_close) = matching_brace(tokens, open_brace_idx)
        {
            let body_tokens = &tokens[open_brace_idx + 1..same_line_close];
            let trailing_tokens = &tokens[same_line_close + 1..];
            let (trailing_attributes, alias_position) = parse_attribute_groups(trailing_tokens, 0)
                .map_err(|error| self.error_at(Loc::whole(code), error))?;
            attributes.extend(trailing_attributes);
            let trailing_name = match trailing_tokens.value_at(alias_position) {
                Some(Token::Ident(alias)) => Some(alias.clone()),
                _ => None,
            };
            let provenance = self.node_provenance(&nodes[0]);
            let tag_decl = if kind == TagKind::Enum {
                let mut enumerators = Vec::new();
                for segment in split_top_level(body_tokens, &Token::Comma) {
                    let segment = segment
                        .into_iter()
                        .filter(|token| !matches!(token.value, Token::Comment(_)))
                        .collect::<Vec<_>>();
                    if segment.is_empty() {
                        continue;
                    }
                    let Some(Token::Ident(enumerator_name)) = segment.value_at(0) else {
                        return Err(self.error_at(Loc::whole(code), "expected enumerator"));
                    };
                    let value = match segment.value_at(1) {
                        None => None,
                        Some(Token::Equal) => {
                            let value =
                                const_expr::Parser::evaluate(&segment[2..]).map_err(|error| {
                                    self.error_at(Loc::whole(code), error.to_string())
                                })?;
                            Some(span_tokens(Expr::IntLit(value), &segment[2..]))
                        }
                        _ => {
                            return Err(
                                self.error_at(Loc::whole(code), "expected enumerator value")
                            );
                        }
                    };
                    enumerators.push(Enumerator {
                        name: enumerator_name.clone(),
                        value,
                    });
                }
                Decl::Enum(EnumDecl {
                    name: name.clone(),
                    enumerators,
                    provenance,
                })
            } else {
                let mut fields = Vec::new();
                for segment in split_top_level(body_tokens, &Token::Semi) {
                    if segment.is_empty() {
                        continue;
                    }
                    let parts = split_top_level(&segment, &Token::Comma);
                    let prefix = parts.first().map_or(Vec::new(), |part| {
                        part[..self.declaration_prefix_end(part)].to_vec()
                    });
                    for (index, mut part) in parts.into_iter().enumerate() {
                        if index != 0 {
                            let mut with_prefix = prefix.clone();
                            with_prefix.append(&mut part);
                            part = with_prefix;
                        }
                        part.push(synthetic(Token::Semi));
                        fields.push(span_tokens(
                            FieldItem::Field(FieldDecl {
                                declaration: self.parse_field_declaration_tokens(code, &part)?,
                                provenance,
                            }),
                            &part,
                        ));
                    }
                }
                Decl::Record(RecordDecl {
                    kind,
                    name: name.clone(),
                    fields,
                    provenance,
                    attributes,
                })
            };
            let mut decls = vec![tag_decl];
            if let Some(alias) = trailing_name {
                decls.push(build_tag_alias_decl(
                    is_typedef, kind, name, alias, provenance,
                ));
            }
            return Ok((decls, 1));
        }

        let mut depth = 1i32;
        let mut close = None;
        for (offset, node) in nodes[1..].iter().enumerate() {
            let PPNodeKind::Code { .. } = &node.value else {
                continue;
            };
            for token in self.node_tokens(node) {
                match token.value {
                    Token::LBrace => depth += 1,
                    Token::RBrace => {
                        depth -= 1;
                        if depth == 0 {
                            close = Some(offset + 1);
                            break;
                        }
                    }
                    _ => {}
                }
            }
            if close.is_some() {
                break;
            }
        }
        let close = close.ok_or_else(|| self.error_at(Loc::whole(code), "expected `}`"))?;
        let mut trailing_name = None;
        if let PPNodeKind::Code { text, .. } = &nodes[close].value {
            let closing_tokens = self.node_tokens(&nodes[close]);
            if closing_tokens.value_at(0) == Some(&Token::RBrace) {
                let (trailing, position) = parse_attribute_groups(&closing_tokens, 1)
                    .map_err(|error| self.error_at(Loc::whole(text), error))?;
                attributes.extend(trailing);
                if let Some(Token::Ident(alias)) = closing_tokens.value_at(position) {
                    trailing_name = Some(alias.clone());
                }
            }
        }
        let consumed = close + 1;
        let provenance = self.node_provenance(&nodes[0]);
        let tag_decl = if kind == TagKind::Enum {
            let mut enumerators = Vec::new();
            for node in &nodes[1..close] {
                let text = self.node_text(node);
                if matches!(node.value, PPNodeKind::Comment { .. }) {
                    continue;
                }
                let tokens = self.node_tokens(node);
                if tokens.is_empty() {
                    continue;
                }
                let Some(Token::Ident(name)) = tokens.value_at(0) else {
                    return Err(self.error_at(Loc::whole(text), "expected enumerator"));
                };
                let value = match tokens.value_at(1) {
                    Some(Token::Comma) | None => None,
                    Some(Token::Equal) => {
                        let end = tokens
                            .values()
                            .position(|token| token == &Token::Comma)
                            .unwrap_or(tokens.len());
                        let value = const_expr::Parser::evaluate(&tokens[2..end])
                            .map_err(|error| self.error_at(Loc::whole(text), error.to_string()))?;
                        Some(span_tokens(Expr::IntLit(value), &tokens[2..end]))
                    }
                    _ => {
                        return Err(self.error_at(Loc::whole(text), "expected enumerator value"));
                    }
                };
                enumerators.push(Enumerator {
                    name: name.clone(),
                    value,
                });
            }
            Decl::Enum(EnumDecl {
                name: name.clone(),
                enumerators,
                provenance,
            })
        } else {
            let fields = self.parse_field_items(&nodes[1..close])?;
            Decl::Record(RecordDecl {
                kind,
                name: name.clone(),
                fields,
                provenance,
                attributes,
            })
        };
        let mut decls = vec![tag_decl];
        if let Some(alias) = trailing_name {
            decls.push(build_tag_alias_decl(
                is_typedef, kind, name, alias, provenance,
            ));
        }
        Ok((decls, consumed))
    }

    fn parse_linkage_spec_block(
        &mut self,
        nodes: &[PPNode],
    ) -> Result<(Vec<SpannedDecl>, usize), ParseError> {
        let code = self.node_text(&nodes[0]);
        let mut depth = 1i32;
        let mut close = None;
        for (offset, node) in nodes[1..].iter().enumerate() {
            let PPNodeKind::Code { .. } = &node.value else {
                continue;
            };
            for token in self.node_tokens(node) {
                match token.value {
                    Token::LBrace => depth += 1,
                    Token::RBrace => {
                        depth -= 1;
                        if depth == 0 {
                            close = Some(offset + 1);
                            break;
                        }
                    }
                    _ => {}
                }
            }
            if close.is_some() {
                break;
            }
        }
        let close = close.ok_or_else(|| self.error_at(Loc::whole(code), "expected `}`"))?;
        let decls = self.parse_decls(&nodes[1..close])?;
        Ok((decls, close + 1))
    }

    fn parse_field_items(&self, nodes: &[PPNode]) -> Result<Vec<SpannedFieldItem>, ParseError> {
        let mut fields = Vec::new();
        let mut index = 0;
        while index < nodes.len() {
            match &nodes[index].value {
                PPNodeKind::Comment { text, provenance } => {
                    fields.push(nodes[index].clone().with_value(FieldItem::Comment {
                        text: text.clone(),
                        loc: nodes[index].expansion,
                        provenance: *provenance,
                    }));
                    index += 1;
                }
                PPNodeKind::Code { .. } => {
                    if self.node_tokens(&nodes[index]).is_empty() {
                        index += 1;
                        continue;
                    }
                    let start = index;
                    let mut depth: i32 = 0;
                    let mut joined = String::new();
                    loop {
                        let text = self.node_text(&nodes[index]);
                        if !joined.is_empty() {
                            joined.push('\n');
                        }
                        joined.push_str(text);
                        for token in self.node_tokens(&nodes[index]) {
                            match token.value {
                                Token::LBrace => depth += 1,
                                Token::RBrace => depth -= 1,
                                _ => {}
                            }
                        }
                        index += 1;
                        if (depth <= 0 && joined.trim_end().ends_with(';')) || index >= nodes.len()
                        {
                            break;
                        }
                    }
                    let all_tokens = if index == start + 1 {
                        self.node_tokens(&nodes[start])
                    } else {
                        self.nodes_tokens(&nodes[start..index])
                    };
                    let declaration_tokens = all_tokens
                        .last()
                        .is_some_and(|token| token.value == Token::Semi)
                        .then(|| &all_tokens[..all_tokens.len() - 1])
                        .unwrap_or(&all_tokens);
                    let parts = split_top_level(declaration_tokens, &Token::Comma);
                    let prefix = declaration_tokens
                        [..self.declaration_prefix_end(declaration_tokens)]
                        .to_vec();
                    for (part_index, mut part) in parts.into_iter().enumerate() {
                        if part_index > 0 {
                            let mut with_prefix = prefix.clone();
                            with_prefix.append(&mut part);
                            part = with_prefix;
                        }
                        part.push(synthetic(Token::Semi));
                        fields.push(span_pp_nodes(
                            FieldItem::Field(FieldDecl {
                                declaration: self.parse_field_declaration_tokens(&joined, &part)?,
                                provenance: self.node_provenance(&nodes[start]),
                            }),
                            &nodes[start..index],
                        ));
                    }
                }
            }
        }
        Ok(fields)
    }

    fn record_typedefs(&mut self, decl: &Decl) {
        if let Decl::Typedef { name, .. } = decl {
            self.typedef_names.insert(name.clone());
        }
    }

    fn parse_typedef_line(
        &self,
        code: &str,
        tokens: &[Span<Token>],
    ) -> Result<(String, CType, Vec<Attribute>), ParseError> {
        let declaration = self.parse_declaration_tokens(code, tokens)?;
        let name = declaration
            .declarator
            .name()
            .ok_or_else(|| self.error_at(Loc::whole(code), "expected typedef name"))?;
        let ty = if declaration.specifiers.qualifiers == Qualifiers::default() {
            declaration.specifiers.ty
        } else {
            CType::Qualified {
                qualifiers: declaration.specifiers.qualifiers,
                ty: Box::new(declaration.specifiers.ty),
            }
        };
        Ok((name.to_string(), ty, declaration.attributes))
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
            let Ok(contents) = std::fs::read_to_string(path) else {
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
            let Ok(source) = std::fs::read_to_string(path) else {
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

fn parse_record_attributes(tokens: &[Span<Token>]) -> Result<(Vec<Attribute>, usize), String> {
    parse_attribute_groups(tokens, 1)
}

struct AttrCursor<'a> {
    tokens: &'a [Span<Token>],
    pos: usize,
}

impl<'a> AttrCursor<'a> {
    fn new(tokens: &'a [Span<Token>], pos: usize) -> Self {
        Self { tokens, pos }
    }

    fn peek(&self) -> Option<&Token> {
        self.tokens.value_at(self.pos)
    }

    fn consume(&mut self, token: &Token) -> bool {
        if self.peek() == Some(token) {
            self.pos += 1;
            true
        } else {
            false
        }
    }

    fn expect(&mut self, token: Token, message: &str) -> Result<(), String> {
        if self.consume(&token) {
            Ok(())
        } else {
            Err(message.into())
        }
    }

    fn expect_ident(&mut self, message: &str) -> Result<String, String> {
        match self.peek() {
            Some(Token::Ident(name)) => {
                let name = name.clone();
                self.pos += 1;
                Ok(name)
            }
            Some(Token::Keyword(keyword)) => {
                let name = <&str>::from(*keyword).to_string();
                self.pos += 1;
                Ok(name)
            }
            _ => Err(message.into()),
        }
    }

    fn parse_parenthesized_arguments(&mut self, message: &str) -> Result<Vec<Span<Token>>, String> {
        if !self.consume(&Token::LParen) {
            return Ok(Vec::new());
        }
        let start = self.pos;
        let mut depth = 0;
        while let Some(token) = self.peek() {
            match token {
                Token::LParen => depth += 1,
                Token::RParen if depth == 0 => break,
                Token::RParen => depth -= 1,
                _ => {}
            }
            self.pos += 1;
        }
        let arguments = self.tokens[start..self.pos].to_vec();
        self.expect(Token::RParen, message)?;
        Ok(arguments)
    }
}

fn parse_attribute_groups(
    tokens: &[Span<Token>],
    position: usize,
) -> Result<(Vec<Attribute>, usize), String> {
    let mut cursor = AttrCursor::new(tokens, position);
    let mut attributes = Vec::new();
    loop {
        if cursor.consume(&Token::Ident("_Alignas".into()))
            || cursor.consume(&Token::Ident("alignas".into()))
        {
            let arguments =
                cursor.parse_parenthesized_arguments("expected `)` after `_Alignas` argument")?;
            if arguments.is_empty() {
                return Err("expected `(` after `_Alignas`".into());
            }
            attributes.push(Attribute::Aligned(parse_attribute_expression(&arguments)?));
        } else if cursor.consume(&Token::Ident("__attribute__".into())) {
            cursor.expect(Token::LParen, "expected `((` after __attribute__")?;
            cursor.expect(Token::LParen, "expected `((` after __attribute__")?;
            loop {
                let name = cursor.expect_ident("expected attribute name")?;
                let arguments = cursor
                    .parse_parenthesized_arguments("expected `)` after attribute arguments")?;
                attributes.push(parse_attribute(&name, &arguments)?);
                if cursor.consume(&Token::Comma) {
                    continue;
                }
                cursor.expect(Token::RParen, "expected `))` after attributes")?;
                cursor.expect(Token::RParen, "expected `))` after attributes")?;
                break;
            }
        } else if cursor.peek() == Some(&Token::LBracket)
            && cursor.tokens.value_at(cursor.pos + 1) == Some(&Token::LBracket)
        {
            cursor.pos += 2;
            loop {
                let mut name = cursor.expect_ident("expected C23 attribute name")?;
                if cursor.consume(&Token::Colon) {
                    cursor.expect(Token::Colon, "expected `::` in attribute name")?;
                    let last = cursor.expect_ident("expected attribute name after `::`")?;
                    name.push_str("::");
                    name.push_str(&last);
                }
                let arguments = cursor
                    .parse_parenthesized_arguments("expected `)` after attribute arguments")?;
                attributes.push(parse_attribute(&name, &arguments)?);
                if cursor.consume(&Token::Comma) {
                    continue;
                }
                cursor.expect(Token::RBracket, "expected `]]` after C23 attributes")?;
                cursor.expect(Token::RBracket, "expected `]]` after C23 attributes")?;
                break;
            }
        } else {
            break;
        }
    }
    Ok((attributes, cursor.pos))
}

fn parse_attribute(name: &str, arguments: &[Span<Token>]) -> Result<Attribute, String> {
    let canonical_name = name
        .strip_prefix("__")
        .and_then(|name| name.strip_suffix("__"))
        .unwrap_or(name);
    let single_string = || match arguments {
        [single] => match &single.value {
            Token::StringLit(value) => Some(value.clone()),
            _ => None,
        },
        _ => None,
    };
    let single_ident = || match arguments {
        [single] => match &single.value {
            Token::Ident(value) => Some(value.clone()),
            _ => None,
        },
        _ => None,
    };
    let single_int = || match arguments {
        [single] => match &single.value {
            token @ Token::IntLit(_) => token.integer_value(),
            _ => None,
        },
        _ => None,
    };
    let integers = || {
        arguments
            .split(|token| token.value == Token::Comma)
            .map(|tokens| match tokens {
                [single] => match &single.value {
                    token @ Token::IntLit(_) => token.integer_value().ok_or(()),
                    _ => Err(()),
                },
                _ => Err(()),
            })
            .collect::<Result<Vec<_>, _>>()
            .ok()
    };
    match canonical_name {
        "packed" if arguments.is_empty() => Ok(Attribute::Packed),
        "aligned" => Ok(match single_int() {
            Some(value) => Attribute::Aligned(const_expr::ConstExpr::Integer(value)),
            None if !arguments.is_empty() => {
                Attribute::Aligned(parse_attribute_expression(arguments)?)
            }
            None => invalid_attribute(name, arguments),
        }),
        "vector_size" => Ok(match single_int() {
            Some(value) => Attribute::VectorSize(const_expr::ConstExpr::Integer(value)),
            None if !arguments.is_empty() => {
                Attribute::VectorSize(parse_attribute_expression(arguments)?)
            }
            None => invalid_attribute(name, arguments),
        }),
        "mode" => Ok(single_ident()
            .map(Attribute::Mode)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "visibility" => Ok(single_string()
            .map(Attribute::Visibility)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "section" => Ok(single_string()
            .map(Attribute::Section)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "annotate" => Ok(single_string()
            .map(Attribute::Annotate)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "target" => Ok(single_string()
            .map(Attribute::Target)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "alias" => Ok(single_string()
            .map(Attribute::Alias)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "weakref" => Ok(single_string()
            .map(Attribute::WeakRef)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "nonnull" if arguments.is_empty() => Ok(Attribute::NonNull(Vec::new())),
        "nonnull" => Ok(integers()
            .map(Attribute::NonNull)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "assume_aligned" => Ok(
            match arguments
                .split(|token| token.value == Token::Comma)
                .map(parse_attribute_expression)
                .collect::<Result<Vec<_>, _>>()
            {
                Ok(values) if !values.is_empty() => Attribute::AssumeAligned(values),
                _ => invalid_attribute(name, arguments),
            },
        ),
        "alloc_size" => Ok(
            match arguments
                .split(|token| token.value == Token::Comma)
                .map(parse_attribute_expression)
                .collect::<Result<Vec<_>, _>>()
            {
                Ok(values) if !values.is_empty() => Attribute::AllocSize(values),
                _ => invalid_attribute(name, arguments),
            },
        ),
        "alloc_align" => Ok(match parse_attribute_expression(arguments) {
            Ok(value) if !arguments.is_empty() => Attribute::AllocAlign(value),
            _ => invalid_attribute(name, arguments),
        }),
        "cleanup" => Ok(single_ident()
            .map(Attribute::Cleanup)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "weak" if arguments.is_empty() => Ok(Attribute::Weak),
        "used" if arguments.is_empty() => Ok(Attribute::Used),
        "retain" if arguments.is_empty() => Ok(Attribute::Retain),
        "noinline" if arguments.is_empty() => Ok(Attribute::NoInline),
        "always_inline" if arguments.is_empty() => Ok(Attribute::AlwaysInline),
        "noreturn" if arguments.is_empty() => Ok(Attribute::NoReturn),
        "constructor" => Ok(match arguments {
            [] => Attribute::Constructor(None),
            [single] => single
                .value
                .integer_value()
                .map(|value| Attribute::Constructor(Some(value)))
                .unwrap_or_else(|| invalid_attribute(name, arguments)),
            _ => invalid_attribute(name, arguments),
        }),
        "destructor" => Ok(match arguments {
            [] => Attribute::Destructor(None),
            [single] => single
                .value
                .integer_value()
                .map(|value| Attribute::Destructor(Some(value)))
                .unwrap_or_else(|| invalid_attribute(name, arguments)),
            _ => invalid_attribute(name, arguments),
        }),
        "malloc" if arguments.is_empty() => Ok(Attribute::Malloc),
        "returns_nonnull" if arguments.is_empty() => Ok(Attribute::ReturnsNonNull),
        "warn_unused_result" if arguments.is_empty() => Ok(Attribute::WarnUnusedResult),
        "sentinel" if arguments.is_empty() => Ok(Attribute::Sentinel(None)),
        "sentinel" => Ok(single_int()
            .map(|value| Attribute::Sentinel(Some(value)))
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "cold" if arguments.is_empty() => Ok(Attribute::Cold),
        "flatten" if arguments.is_empty() => Ok(Attribute::Flatten),
        "hot" if arguments.is_empty() => Ok(Attribute::Hot),
        "leaf" if arguments.is_empty() => Ok(Attribute::Leaf),
        "noipa" if arguments.is_empty() => Ok(Attribute::NoIpa),
        "noclone" if arguments.is_empty() => Ok(Attribute::NoClone),
        "optimize" if !arguments.is_empty() => Ok(Attribute::Optimize(
            arguments.values().map(String::from).collect(),
        )),
        "naked" if arguments.is_empty() => Ok(Attribute::Naked),
        "interrupt" if arguments.is_empty() => Ok(Attribute::Interrupt),
        "no_split_stack" if arguments.is_empty() => Ok(Attribute::NoSplitStack),
        "returns_twice" if arguments.is_empty() => Ok(Attribute::ReturnsTwice),
        "cpu_dispatch" => Ok(Attribute::CpuDispatch(attribute_arguments(arguments))),
        "cpu_specific" => Ok(Attribute::CpuSpecific(attribute_arguments(arguments))),
        "target_clones" => Ok(Attribute::TargetClones(attribute_arguments(arguments))),
        "ifunc" => Ok(single_string()
            .map(Attribute::Ifunc)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "dllimport" if arguments.is_empty() => Ok(Attribute::DllImport),
        "weak_import" if arguments.is_empty() => Ok(Attribute::WeakImport),
        "tls_model" => Ok(single_string()
            .map(Attribute::TlsModel)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "ms_struct" if arguments.is_empty() => Ok(Attribute::MsStruct),
        "stdcall" if arguments.is_empty() => Ok(Attribute::Stdcall),
        "nomips16" if arguments.is_empty() => Ok(Attribute::NoMips16),
        "availability" => Ok(Attribute::Availability(attribute_arguments(arguments))),
        "ext_vector_type" => Ok(match parse_attribute_expression(arguments) {
            Ok(value) => Attribute::ExtVectorType(value),
            Err(_) => invalid_attribute(name, arguments),
        }),
        "scalar_storage_order" => Ok(single_string()
            .map(Attribute::ScalarStorageOrder)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "transparent_union" if arguments.is_empty() => Ok(Attribute::TransparentUnion),
        "format" => Ok(Attribute::Format(attribute_arguments(arguments))),
        "format_arg" => Ok(Attribute::FormatArg(attribute_arguments(arguments))),
        "gcc_struct" if arguments.is_empty() => Ok(Attribute::GccStruct),
        "common" if arguments.is_empty() => Ok(Attribute::Common),
        "nocommon" if arguments.is_empty() => Ok(Attribute::NoCommon),
        "pure" if arguments.is_empty() => Ok(Attribute::Pure),
        "const" if arguments.is_empty() => Ok(Attribute::Const),
        "may_alias" if arguments.is_empty() => Ok(Attribute::MayAlias),
        "deprecated" if arguments.is_empty() => Ok(Attribute::Deprecated(None)),
        "deprecated" => Ok(single_string()
            .map(|value| Attribute::Deprecated(Some(value)))
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "nodiscard" if arguments.is_empty() => Ok(Attribute::NoDiscard(None)),
        "nodiscard" => Ok(single_string()
            .map(|value| Attribute::NoDiscard(Some(value)))
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "maybe_unused" if arguments.is_empty() => Ok(Attribute::MaybeUnused),
        "fallthrough" if arguments.is_empty() => Ok(Attribute::Fallthrough),
        _ if canonical_name.is_attribute_name() => Ok(invalid_attribute(name, arguments)),
        _ => Ok(Attribute::Unknown {
            name: name.into(),
            arguments: arguments.values().map(String::from).collect(),
        }),
    }
}

fn apply_vector_attributes(mut ty: CType, attributes: &[Attribute]) -> CType {
    for attribute in attributes {
        let size = match attribute {
            Attribute::VectorSize(size) => VectorSize::Bytes(size.clone()),
            Attribute::ExtVectorType(size) => VectorSize::Lanes(size.clone()),
            _ => continue,
        };
        ty = CType::Vector(VectorType {
            element: Box::new(ty),
            size,
        });
    }
    ty
}

fn invalid_attribute(name: &str, arguments: &[Span<Token>]) -> Attribute {
    Attribute::Invalid {
        name: name.into(),
        arguments: arguments.values().map(String::from).collect(),
    }
}

fn attribute_arguments(arguments: &[Span<Token>]) -> Vec<String> {
    arguments
        .split(|token| token.value == Token::Comma)
        .map(|tokens| {
            tokens
                .values()
                .map(String::from)
                .collect::<Vec<_>>()
                .join(" ")
        })
        .collect()
}

fn parse_attribute_expression(arguments: &[Span<Token>]) -> Result<const_expr::ConstExpr, String> {
    const_expr::Parser::parse(arguments).map_err(|error| error.to_string())
}

fn paren_depth(tokens: &[Span<Token>]) -> i32 {
    tokens.values().fold(0i32, |depth, token| match token {
        Token::LParen => depth + 1,
        Token::RParen => depth - 1,
        _ => depth,
    })
}

fn signature_node_span(nodes: &[PPNode]) -> usize {
    let mut depth = 0i32;
    for (index, node) in nodes.iter().enumerate() {
        let PPNodeKind::Code { text, .. } = &node.value else {
            continue;
        };
        for token in lex(text) {
            match token.value {
                Token::LParen => depth += 1,
                Token::RParen => depth -= 1,
                Token::LBrace | Token::Semi if depth <= 0 => return index + 1,
                _ => {}
            }
        }
    }
    nodes.len().max(1)
}

fn declaration_node_span(nodes: &[PPNode]) -> usize {
    let mut depth = 0i32;
    for (index, node) in nodes.iter().enumerate() {
        let PPNodeKind::Code { tokens, .. } = &node.value else {
            continue;
        };
        for token in tokens {
            match token.value {
                Token::LParen | Token::LBrace | Token::LBracket => depth += 1,
                Token::RParen | Token::RBrace | Token::RBracket => depth -= 1,
                Token::Semi if depth == 0 => return index + 1,
                _ => {}
            }
        }
    }
    nodes.len().max(1)
}

fn join_node_text(nodes: &[PPNode]) -> String {
    nodes
        .iter()
        .map(|node| match &node.value {
            PPNodeKind::Comment { text, .. } => text.as_str(),
            PPNodeKind::Code { text, .. } => text.as_str(),
        })
        .collect::<Vec<_>>()
        .join("\n")
}

fn matching_brace(tokens: &[Span<Token>], open: usize) -> Option<usize> {
    let mut depth = 0i32;
    for (offset, token) in tokens[open..].iter().enumerate() {
        match token.value {
            Token::LBrace => depth += 1,
            Token::RBrace => {
                depth -= 1;
                if depth == 0 {
                    return Some(open + offset);
                }
            }
            _ => {}
        }
    }
    None
}

fn split_top_level(tokens: &[Span<Token>], delimiter: &Token) -> Vec<Vec<Span<Token>>> {
    let mut segments = Vec::new();
    let mut depth = 0i32;
    let mut current = Vec::new();
    for token in tokens {
        match &token.value {
            Token::LBrace | Token::LParen | Token::LBracket => {
                depth += 1;
                current.push(token.clone());
            }
            Token::RBrace | Token::RParen | Token::RBracket => {
                depth -= 1;
                current.push(token.clone());
            }
            value if depth == 0 && value == delimiter => {
                segments.push(std::mem::take(&mut current));
            }
            _ => current.push(token.clone()),
        }
    }
    if !current.is_empty() {
        segments.push(current);
    }
    segments
}

fn build_tag_alias_decl(
    is_typedef: bool,
    kind: TagKind,
    name: Option<String>,
    alias: String,
    provenance: Provenance,
) -> Decl {
    let ty = CType::Tagged {
        kind,
        name,
        body: None,
    };
    if is_typedef {
        Decl::Typedef {
            name: alias,
            ty,
            provenance,
            attributes: Vec::new(),
        }
    } else {
        Decl::Declaration {
            declaration: Declaration {
                specifiers: DeclarationSpecifiers {
                    ty,
                    qualifiers: Qualifiers::default(),
                    storage: StorageClass::None,
                    is_thread_local: false,
                    is_inline: false,
                    is_noreturn: false,
                    is_constexpr: false,
                },
                declarator: Declarator::Name(alias),
                initializer: None,
                attributes: Vec::new(),
            },
            provenance,
        }
    }
}

fn matching_paren(tokens: &[Span<Token>], open: usize) -> Option<usize> {
    let mut depth = 0i32;
    for (offset, token) in tokens[open..].iter().enumerate() {
        match token.value {
            Token::LParen => depth += 1,
            Token::RParen => {
                depth -= 1;
                if depth == 0 {
                    return Some(open + offset);
                }
            }
            _ => {}
        }
    }
    None
}

fn top_level_semi(tokens: &[Span<Token>]) -> Option<usize> {
    top_level_token(tokens, &Token::Semi)
}

fn top_level_token(tokens: &[Span<Token>], target: &Token) -> Option<usize> {
    let mut depth = 0i32;
    for (offset, token) in tokens.iter().enumerate() {
        if depth == 0 && &token.value == target {
            return Some(offset);
        }
        match token.value {
            Token::LParen | Token::LBrace | Token::LBracket => depth += 1,
            Token::RParen | Token::RBrace | Token::RBracket => depth -= 1,
            _ => {}
        }
    }
    None
}

trait AttributeName {
    fn is_attribute_name(&self) -> bool;
}

impl AttributeName for str {
    fn is_attribute_name(&self) -> bool {
        matches!(
            self,
            "aligned"
                | "vector_size"
                | "mode"
                | "visibility"
                | "section"
                | "annotate"
                | "target"
                | "alias"
                | "weakref"
                | "nonnull"
                | "weak"
                | "used"
                | "retain"
                | "noinline"
                | "always_inline"
                | "noreturn"
                | "constructor"
                | "destructor"
                | "malloc"
                | "assume_aligned"
                | "alloc_size"
                | "alloc_align"
                | "cleanup"
                | "returns_nonnull"
                | "warn_unused_result"
                | "sentinel"
                | "cold"
                | "flatten"
                | "hot"
                | "leaf"
                | "noipa"
                | "noclone"
                | "optimize"
                | "naked"
                | "interrupt"
                | "no_split_stack"
                | "returns_twice"
                | "cpu_dispatch"
                | "cpu_specific"
                | "target_clones"
                | "ifunc"
                | "dllimport"
                | "weak_import"
                | "tls_model"
                | "ms_struct"
                | "stdcall"
                | "nomips16"
                | "availability"
                | "ext_vector_type"
                | "scalar_storage_order"
                | "transparent_union"
                | "format"
                | "format_arg"
                | "gcc_struct"
                | "common"
                | "nocommon"
                | "pure"
                | "const"
                | "may_alias"
                | "deprecated"
                | "nodiscard"
                | "maybe_unused"
                | "fallthrough"
        )
    }
}

#[derive(Debug, Clone, PartialEq, Eq, Error, Diagnostic)]
pub(crate) enum DeclaratorError {
    #[error("expected declaration type")]
    ExpectedDeclarationType,
    #[error("expected declaration type, found {0:?}")]
    UnexpectedToken(Token),
    #[error("expected `{0:?}` {1}")]
    ExpectedToken(Token, &'static str),
    #[error("expected tag name or `{{`")]
    ExpectedTagNameOrBrace,
    #[error("expected enumerator")]
    ExpectedEnumerator,
    #[error("expected `,` or `}}` in enum body")]
    ExpectedCommaOrRBrace,
    #[error("expected `_Fract` or `_Accum`")]
    ExpectedFractOrAccum,
    #[error("unsupported typeof expression")]
    UnsupportedTypeofExpression,
    #[error("expected declarator")]
    ExpectedDeclarator,
    #[error("{0}")]
    Other(String),
}

impl From<String> for DeclaratorError {
    fn from(message: String) -> Self {
        DeclaratorError::Other(message)
    }
}

pub(crate) struct DeclaratorParser<'a> {
    tokens: &'a [Span<Token>],
    pos: usize,
    typedef_names: &'a HashSet<String>,
}

impl<'a> DeclaratorParser<'a> {
    pub(crate) fn new(
        tokens: &'a [Span<Token>],
        pos: usize,
        typedef_names: &'a HashSet<String>,
    ) -> Self {
        Self {
            tokens,
            pos,
            typedef_names,
        }
    }

    pub(crate) fn position(&self) -> usize {
        self.pos
    }

    fn peek(&self) -> Option<&Token> {
        self.tokens.get(self.pos).map(|span| &span.value)
    }

    fn matches(&mut self, expected: Token) -> bool {
        if self.peek() == Some(&expected) {
            self.pos += 1;
            true
        } else {
            false
        }
    }

    pub(crate) fn parse_base_type(&mut self) -> Result<CType, DeclaratorError> {
        let Some(token) = self.peek().cloned() else {
            return Err(DeclaratorError::ExpectedDeclarationType);
        };
        self.pos += 1;
        Ok(match token {
            Token::Keyword(Keyword::Bool) => CType::Bool,
            Token::Keyword(Keyword::BFloat16) => CType::Floating(FloatingType::BFloat16),
            Token::Keyword(Keyword::Char) => CType::Integer(IntegerType::Char { signed: None }),
            Token::Keyword(Keyword::Double) => {
                if self.matches(Token::Keyword(Keyword::Complex)) {
                    CType::Complex(Box::new(CType::Floating(FloatingType::Double)))
                } else if self.matches(Token::Keyword(Keyword::Imaginary)) {
                    CType::Imaginary(Box::new(CType::Floating(FloatingType::Double)))
                } else {
                    CType::Floating(FloatingType::Double)
                }
            }
            Token::Keyword(Keyword::Float) => {
                if self.matches(Token::Keyword(Keyword::Complex)) {
                    CType::Complex(Box::new(CType::Floating(FloatingType::Float)))
                } else if self.matches(Token::Keyword(Keyword::Imaginary)) {
                    CType::Imaginary(Box::new(CType::Floating(FloatingType::Float)))
                } else {
                    CType::Floating(FloatingType::Float)
                }
            }
            Token::Keyword(Keyword::Float16) => CType::Floating(FloatingType::Float16),
            Token::Keyword(Keyword::Fp16) => CType::Floating(FloatingType::Fp16),
            Token::Keyword(Keyword::Float64x) => CType::Floating(FloatingType::Float64x),
            Token::Keyword(Keyword::Float128) => CType::Floating(FloatingType::Float128),
            Token::Keyword(Keyword::Float128Ext) => CType::Floating(FloatingType::Float128Ext),
            Token::Keyword(Keyword::Int) => CType::Integer(IntegerType::Ranked {
                rank: IntegerRank::Int,
                signed: true,
            }),
            Token::Keyword(Keyword::Int128) => CType::Integer(IntegerType::Ranked {
                rank: IntegerRank::Int128,
                signed: true,
            }),
            Token::Keyword(Keyword::Long) => {
                if matches!(
                    self.peek(),
                    Some(Token::Keyword(Keyword::Fract | Keyword::Accum))
                ) {
                    return self.parse_fixed_point(FixedPointRank::Long, false);
                }
                if self.matches(Token::Keyword(Keyword::Long)) {
                    if matches!(
                        self.peek(),
                        Some(Token::Keyword(Keyword::Fract | Keyword::Accum))
                    ) {
                        return self.parse_fixed_point(FixedPointRank::LongLong, false);
                    }
                    let signed = !self.matches(Token::Keyword(Keyword::Unsigned));
                    if signed {
                        self.matches(Token::Keyword(Keyword::Signed));
                    }
                    self.matches(Token::Keyword(Keyword::Int));
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::LongLong,
                        signed,
                    })
                } else if self.matches(Token::Keyword(Keyword::Double)) {
                    if self.matches(Token::Keyword(Keyword::Complex)) {
                        CType::Complex(Box::new(CType::Floating(FloatingType::LongDouble)))
                    } else if self.matches(Token::Keyword(Keyword::Imaginary)) {
                        CType::Imaginary(Box::new(CType::Floating(FloatingType::LongDouble)))
                    } else {
                        CType::Floating(FloatingType::LongDouble)
                    }
                } else {
                    let signed = !self.matches(Token::Keyword(Keyword::Unsigned));
                    if signed {
                        self.matches(Token::Keyword(Keyword::Signed));
                    }
                    self.matches(Token::Keyword(Keyword::Int));
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Long,
                        signed,
                    })
                }
            }
            Token::Keyword(Keyword::Short) => {
                if matches!(
                    self.peek(),
                    Some(Token::Keyword(Keyword::Fract | Keyword::Accum))
                ) {
                    return self.parse_fixed_point(FixedPointRank::Short, false);
                }
                let signed = !self.matches(Token::Keyword(Keyword::Unsigned));
                if signed {
                    self.matches(Token::Keyword(Keyword::Signed));
                }
                self.matches(Token::Keyword(Keyword::Int));
                CType::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Short,
                    signed,
                })
            }
            Token::Keyword(Keyword::Signed) => match self.peek() {
                Some(Token::Keyword(Keyword::Char)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Char { signed: Some(true) })
                }
                Some(Token::Keyword(Keyword::Short)) => {
                    self.pos += 1;
                    self.matches(Token::Keyword(Keyword::Int));
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Short,
                        signed: true,
                    })
                }
                Some(Token::Keyword(Keyword::Long)) => {
                    self.pos += 1;
                    if self.matches(Token::Keyword(Keyword::Long)) {
                        self.matches(Token::Keyword(Keyword::Int));
                        CType::Integer(IntegerType::Ranked {
                            rank: IntegerRank::LongLong,
                            signed: true,
                        })
                    } else {
                        self.matches(Token::Keyword(Keyword::Int));
                        CType::Integer(IntegerType::Ranked {
                            rank: IntegerRank::Long,
                            signed: true,
                        })
                    }
                }
                Some(Token::Keyword(Keyword::Int128)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Int128,
                        signed: true,
                    })
                }
                Some(Token::Keyword(Keyword::BitInt)) => {
                    self.pos += 1;
                    self.parse_bit_int(false)
                }
                Some(Token::Keyword(Keyword::Int)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Int,
                        signed: true,
                    })
                }
                _ => CType::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Int,
                    signed: true,
                }),
            },
            Token::Keyword(Keyword::Unsigned) => match self.peek() {
                Some(Token::Keyword(Keyword::Char)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Char {
                        signed: Some(false),
                    })
                }
                Some(Token::Keyword(Keyword::Short)) => {
                    self.pos += 1;
                    self.matches(Token::Keyword(Keyword::Int));
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Short,
                        signed: false,
                    })
                }
                Some(Token::Keyword(Keyword::Long)) => {
                    self.pos += 1;
                    if self.matches(Token::Keyword(Keyword::Long)) {
                        self.matches(Token::Keyword(Keyword::Int));
                        CType::Integer(IntegerType::Ranked {
                            rank: IntegerRank::LongLong,
                            signed: false,
                        })
                    } else {
                        self.matches(Token::Keyword(Keyword::Int));
                        CType::Integer(IntegerType::Ranked {
                            rank: IntegerRank::Long,
                            signed: false,
                        })
                    }
                }
                Some(Token::Keyword(Keyword::Int128)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Int128,
                        signed: false,
                    })
                }
                Some(Token::Keyword(Keyword::BitInt)) => {
                    self.pos += 1;
                    self.parse_bit_int(true)
                }
                Some(Token::Keyword(Keyword::Int)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Int,
                        signed: false,
                    })
                }
                _ => CType::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Int,
                    signed: false,
                }),
            },
            Token::Keyword(Keyword::Void) => CType::Void,
            Token::Keyword(Keyword::Saturated) => {
                let rank = if self.matches(Token::Keyword(Keyword::Short)) {
                    FixedPointRank::Short
                } else if self.matches(Token::Keyword(Keyword::Long)) {
                    if self.matches(Token::Keyword(Keyword::Long)) {
                        FixedPointRank::LongLong
                    } else {
                        FixedPointRank::Long
                    }
                } else {
                    FixedPointRank::Default
                };
                return self.parse_fixed_point(rank, true);
            }
            Token::Keyword(Keyword::Atomic) => {
                self.expect(
                    Token::LParen,
                    DeclaratorError::ExpectedToken(Token::LParen, "after `_Atomic`"),
                )?;
                let leading_qualifiers = self.take_qualifiers();
                let mut ty = self.parse_base_type()?;
                let trailing_qualifiers = self.take_qualifiers();
                let qualifiers = Qualifiers {
                    is_const: leading_qualifiers.is_const || trailing_qualifiers.is_const,
                    is_volatile: leading_qualifiers.is_volatile || trailing_qualifiers.is_volatile,
                    is_restrict: leading_qualifiers.is_restrict || trailing_qualifiers.is_restrict,
                    is_atomic: leading_qualifiers.is_atomic || trailing_qualifiers.is_atomic,
                };
                if qualifiers != Qualifiers::default() {
                    ty = CType::Qualified {
                        qualifiers,
                        ty: Box::new(ty),
                    };
                }
                let declarator = self.parse_declarator(true)?;
                ty = apply_abstract_declarator(ty, declarator);
                self.expect(
                    Token::RParen,
                    DeclaratorError::ExpectedToken(Token::RParen, "after `_Atomic` type"),
                )?;
                CType::Atomic(Box::new(ty))
            }
            Token::Keyword(Keyword::Complex) => {
                let element = if matches!(
                    self.peek(),
                    Some(Token::Keyword(
                        Keyword::Char
                            | Keyword::Double
                            | Keyword::Float
                            | Keyword::Int
                            | Keyword::Long
                            | Keyword::Short
                            | Keyword::Signed
                            | Keyword::Unsigned
                            | Keyword::Float16
                            | Keyword::Float64x
                            | Keyword::Float128
                            | Keyword::Float128Ext
                    ))
                ) {
                    self.parse_base_type()?
                } else {
                    CType::Floating(FloatingType::Double)
                };
                CType::Complex(Box::new(element))
            }
            Token::Keyword(Keyword::Imaginary) => {
                CType::Imaginary(Box::new(CType::Floating(FloatingType::Double)))
            }
            Token::Keyword(Keyword::BitInt) => self.parse_bit_int(false),
            Token::Keyword(Keyword::Typeof) => self.parse_typeof()?,
            Token::Keyword(Keyword::TypeofUnqual) => {
                CType::TypeOfUnqual(self.parse_typeof_operand()?)
            }
            Token::Keyword(Keyword::Fract) => {
                self.fixed_point(FixedPointKind::Fract, false, FixedPointRank::Default)
            }
            Token::Keyword(Keyword::Accum) => {
                self.fixed_point(FixedPointKind::Accum, false, FixedPointRank::Default)
            }
            Token::Keyword(Keyword::Struct) => self.parse_record_type(TagKind::Struct)?,
            Token::Keyword(Keyword::Union) => self.parse_record_type(TagKind::Union)?,
            Token::Keyword(Keyword::Enum) => self.parse_enum_type()?,
            Token::Ident(name) if name == "__int128_t" => CType::Integer(IntegerType::Ranked {
                rank: IntegerRank::Int128,
                signed: true,
            }),
            Token::Ident(name) if name == "__uint128_t" => CType::Integer(IntegerType::Ranked {
                rank: IntegerRank::Int128,
                signed: false,
            }),
            Token::Ident(name) if is_target_builtin_name(&name) => CType::TargetBuiltin(name),
            Token::Ident(name) => CType::Named(name),
            other => return Err(DeclaratorError::UnexpectedToken(other)),
        })
    }

    fn parse_record_type(&mut self, kind: TagKind) -> Result<CType, DeclaratorError> {
        let name = match self.peek() {
            Some(Token::Ident(name)) => {
                let name = name.clone();
                self.pos += 1;
                Some(name)
            }
            _ => None,
        };
        let body = if self.peek() == Some(&Token::LBrace) {
            Some(TagBody::Fields(self.parse_field_list()?))
        } else {
            None
        };
        if name.is_none() && body.is_none() {
            return Err(DeclaratorError::ExpectedTagNameOrBrace);
        }
        Ok(CType::Tagged { kind, name, body })
    }

    fn parse_enum_type(&mut self) -> Result<CType, DeclaratorError> {
        let name = match self.peek() {
            Some(Token::Ident(name)) => {
                let name = name.clone();
                self.pos += 1;
                Some(name)
            }
            _ => None,
        };
        if self.peek() == Some(&Token::Colon) {
            let checkpoint = self.pos;
            self.pos += 1;
            self.take_qualifiers();
            if self.parse_base_type().is_err() {
                self.pos = checkpoint;
            }
        }
        let body = if self.peek() == Some(&Token::LBrace) {
            Some(TagBody::Enumerators(self.parse_enumerator_list()?))
        } else {
            None
        };
        if name.is_none() && body.is_none() {
            return Err(DeclaratorError::ExpectedTagNameOrBrace);
        }
        Ok(CType::Tagged {
            kind: TagKind::Enum,
            name,
            body,
        })
    }

    fn parse_field_list(&mut self) -> Result<Vec<FieldDecl>, DeclaratorError> {
        self.pos += 1;
        let mut fields = Vec::new();
        while self.peek() != Some(&Token::RBrace) {
            if self.peek().is_none() {
                return Err(DeclaratorError::ExpectedToken(
                    Token::RBrace,
                    "in struct/union body",
                ));
            }
            while self.peek() == Some(&Token::Ident("__extension__".to_string())) {
                self.pos += 1;
            }
            let qualifiers = self.take_qualifiers();
            let ty = self.parse_base_type()?;
            let declarator = if matches!(self.peek(), Some(&Token::Semi) | Some(&Token::Colon)) {
                Declarator::Abstract
            } else {
                self.parse_declarator(true)?
            };
            if self.matches(Token::Colon) {
                while !matches!(self.peek(), Some(&Token::Semi) | None) {
                    self.pos += 1;
                }
            }
            self.expect(
                Token::Semi,
                DeclaratorError::ExpectedToken(Token::Semi, "in struct/union field"),
            )?;
            fields.push(FieldDecl {
                declaration: Declaration {
                    specifiers: DeclarationSpecifiers {
                        ty,
                        qualifiers,
                        storage: StorageClass::None,
                        is_thread_local: false,
                        is_inline: false,
                        is_noreturn: false,
                        is_constexpr: false,
                    },
                    declarator,
                    initializer: None,
                    attributes: Vec::new(),
                },
                provenance: Provenance::default(),
            });
        }
        self.pos += 1;
        Ok(fields)
    }

    fn parse_enumerator_list(&mut self) -> Result<Vec<Enumerator>, DeclaratorError> {
        self.pos += 1;
        let mut enumerators = Vec::new();
        loop {
            if self.matches(Token::RBrace) {
                break;
            }
            let Some(Token::Ident(name)) = self.peek().cloned() else {
                return Err(DeclaratorError::ExpectedEnumerator);
            };
            self.pos += 1;
            let value = if self.matches(Token::Equal) {
                let start = self.pos;
                while !matches!(
                    self.peek(),
                    Some(&Token::Comma) | Some(&Token::RBrace) | None
                ) {
                    self.pos += 1;
                }
                let value = const_expr::Parser::evaluate(&self.tokens[start..self.pos])
                    .map_err(|error| error.to_string())?;
                Some(span_tokens(
                    Expr::IntLit(value),
                    &self.tokens[start..self.pos],
                ))
            } else {
                None
            };
            enumerators.push(Enumerator { name, value });
            if self.matches(Token::Comma) {
                continue;
            }
            if self.matches(Token::RBrace) {
                break;
            }
            return Err(DeclaratorError::ExpectedCommaOrRBrace);
        }
        Ok(enumerators)
    }

    fn parse_fixed_point(
        &mut self,
        rank: FixedPointRank,
        saturated: bool,
    ) -> Result<CType, DeclaratorError> {
        let kind = match self.peek() {
            Some(Token::Keyword(Keyword::Fract)) => FixedPointKind::Fract,
            Some(Token::Keyword(Keyword::Accum)) => FixedPointKind::Accum,
            _ => return Err(DeclaratorError::ExpectedFractOrAccum),
        };
        self.pos += 1;
        Ok(self.fixed_point(kind, saturated, rank))
    }

    fn fixed_point(&self, kind: FixedPointKind, saturated: bool, rank: FixedPointRank) -> CType {
        CType::FixedPoint(FixedPointType {
            kind,
            rank,
            saturated,
        })
    }

    fn parse_bit_int(&mut self, is_unsigned: bool) -> CType {
        assert!(self.matches(Token::LParen), "expected `(` after _BitInt");
        let start = self.pos;
        while self.peek() != Some(&Token::RParen) {
            assert!(self.peek().is_some(), "expected `)` after _BitInt width");
            self.pos += 1;
        }
        let width = const_expr::Parser::parse(&self.tokens[start..self.pos])
            .expect("invalid _BitInt width expression");
        self.pos += 1;
        CType::Integer(IntegerType::BitInt {
            width,
            signed: !is_unsigned,
        })
    }

    fn parse_typeof(&mut self) -> Result<CType, DeclaratorError> {
        self.parse_typeof_operand().map(CType::TypeOf)
    }

    fn parse_typeof_operand(&mut self) -> Result<TypeOfOperand, DeclaratorError> {
        self.expect(
            Token::LParen,
            DeclaratorError::ExpectedToken(Token::LParen, "after `typeof`"),
        )?;
        if self.typeof_type_start() {
            let ty = self.parse_base_type()?;
            self.expect(
                Token::RParen,
                DeclaratorError::ExpectedToken(Token::RParen, "after typeof type-name"),
            )?;
            return Ok(TypeOfOperand::Type(Box::new(ty)));
        }
        let start = self.pos;
        let mut depth = 0;
        while let Some(token) = self.peek() {
            match token {
                Token::LParen => depth += 1,
                Token::RParen if depth == 0 => break,
                Token::RParen => depth -= 1,
                _ => {}
            }
            self.pos += 1;
        }
        self.expect(
            Token::RParen,
            DeclaratorError::ExpectedToken(Token::RParen, "after typeof expression"),
        )?;
        let tokens = &self.tokens[start..self.pos - 1];
        if tokens.is_empty() {
            return Err(DeclaratorError::UnsupportedTypeofExpression);
        }
        if let [single] = tokens
            && let Some(expression) = const_expr::string_literal_expr(Some(&single.value))
        {
            return Ok(TypeOfOperand::Expression(Box::new(
                single.clone().with_value(expression),
            )));
        }
        let tokens = coalesce_string_literals(tokens);
        let expression = const_expr::Parser::parse_expression(&tokens, self.typedef_names)
            .map_err(|error| DeclaratorError::Other(error.to_string()))?;
        Ok(TypeOfOperand::Expression(Box::new(span_tokens(
            Expr::Const(Box::new(expression)),
            &tokens,
        ))))
    }

    fn typeof_type_start(&self) -> bool {
        matches!(
            self.peek(),
            Some(Token::Keyword(
                Keyword::Bool
                    | Keyword::BFloat16
                    | Keyword::Char
                    | Keyword::Double
                    | Keyword::Float
                    | Keyword::Float16
                    | Keyword::Fp16
                    | Keyword::Float64x
                    | Keyword::Float128
                    | Keyword::Float128Ext
                    | Keyword::Int
                    | Keyword::Int128
                    | Keyword::Long
                    | Keyword::Short
                    | Keyword::Signed
                    | Keyword::Unsigned
                    | Keyword::Void
                    | Keyword::Complex
                    | Keyword::Imaginary
                    | Keyword::BitInt
                    | Keyword::Atomic
                    | Keyword::Struct
                    | Keyword::Union
                    | Keyword::Enum
                    | Keyword::Fract
                    | Keyword::Accum
                    | Keyword::Saturated
                    | Keyword::Typeof
                    | Keyword::TypeofUnqual
            ))
        )
    }

    fn parse_designator_index_expr(&mut self) -> i64 {
        let start = self.pos;
        let mut depth = 0i32;
        while let Some(token) = self.tokens.value_at(self.pos) {
            match token {
                Token::LBracket | Token::LParen => depth += 1,
                Token::RBracket if depth == 0 => break,
                Token::Ellipsis if depth == 0 => break,
                Token::RBracket | Token::RParen => depth -= 1,
                _ => {}
            }
            self.pos += 1;
        }

        const_expr::Parser::evaluate(&self.tokens[start..self.pos])
            .unwrap_or_else(|error| panic!("invalid array designator expression: {error:?}"))
    }

    fn parse_initializer(&mut self, typedef_names: &HashSet<String>) -> Initializer {
        if self.matches(Token::LBrace) {
            let mut items = Vec::new();
            while !self.matches(Token::RBrace) {
                let mut designators = Vec::new();
                loop {
                    if self.matches(Token::LBracket) {
                        let index = self.parse_designator_index_expr();
                        if self.matches(Token::Ellipsis) {
                            let end = self.parse_designator_index_expr();
                            designators.push(Designator::ArrayRange {
                                start: IntegerValue::I128(index as i128),
                                end: IntegerValue::I128(end as i128),
                            });
                        } else {
                            designators.push(Designator::Array(index));
                        }
                        assert!(self.matches(Token::RBracket), "expected `]` in designator");
                    } else if self.matches(Token::Dot) {
                        let Some(Token::Ident(name)) = self.peek().cloned() else {
                            panic!("field designator must name a field")
                        };
                        self.pos += 1;
                        designators.push(Designator::Field(name));
                    } else if let Some(Token::Ident(name)) = self.peek().cloned()
                        && self.tokens.value_at(self.pos + 1) == Some(&Token::Colon)
                    {
                        self.pos += 2;
                        designators.push(Designator::Field(name));
                    } else {
                        break;
                    }
                }
                if !designators.is_empty() {
                    self.matches(Token::Equal);
                }
                items.push(InitializerItem {
                    designators,
                    value: self.parse_initializer(typedef_names),
                });
                if !self.matches(Token::Comma) {
                    assert!(
                        self.peek() == Some(&Token::RBrace),
                        "expected `,` in initializer"
                    );
                }
            }
            Initializer::List(items)
        } else if let Some(expression) = const_expr::string_literal_expr(self.peek()) {
            let start = self.pos;
            self.pos += 1;
            Initializer::Expr(span_tokens(expression, &self.tokens[start..self.pos]))
        } else {
            let start = self.pos;
            let (expression, end) =
                const_expr::Parser::parse_one(self.tokens, self.pos, typedef_names)
                    .unwrap_or_else(|error| panic!("unsupported initializer expression: {error}"));
            self.pos = end;
            Initializer::Expr(span_tokens(
                Expr::Const(Box::new(expression)),
                &self.tokens[start..end],
            ))
        }
    }

    pub(crate) fn parse_declarator(
        &mut self,
        allow_abstract: bool,
    ) -> Result<Declarator, DeclaratorError> {
        let mut pointer_qualifiers = Vec::new();
        while self.matches(Token::Star) {
            pointer_qualifiers.push(self.take_qualifiers());
        }
        let mut declarator = match self.peek().cloned() {
            Some(Token::Ident(name)) => {
                self.pos += 1;
                Declarator::Name(name)
            }
            Some(Token::LParen) => {
                self.pos += 1;
                let declarator = self.parse_declarator(allow_abstract)?;
                if !self.matches(Token::RParen) {
                    return Err(DeclaratorError::ExpectedToken(
                        Token::RParen,
                        "in declarator",
                    ));
                }
                Declarator::Grouped(Box::new(declarator))
            }
            _ if allow_abstract => Declarator::Abstract,
            _ => return Err(DeclaratorError::ExpectedDeclarator),
        };

        for qualifiers in pointer_qualifiers {
            declarator = Declarator::Pointer {
                qualifiers,
                inner: Box::new(declarator),
            };
        }

        loop {
            declarator = match self.peek() {
                Some(Token::LBracket) => {
                    self.pos += 1;
                    while matches!(
                        self.peek(),
                        Some(Token::Keyword(
                            Keyword::Static
                                | Keyword::Const
                                | Keyword::Volatile
                                | Keyword::Restrict
                        ))
                    ) {
                        self.pos += 1;
                    }
                    let size = if self.peek() == Some(&Token::RBracket) {
                        ArraySize::Unspecified
                    } else if self.peek() == Some(&Token::Star)
                        && self.tokens.value_at(self.pos + 1) == Some(&Token::RBracket)
                    {
                        self.pos += 1;
                        ArraySize::Star
                    } else {
                        let start = self.pos;
                        let mut depth = 0i32;
                        while !matches!(self.peek(), Some(Token::RBracket) if depth == 0) {
                            assert!(self.peek().is_some(), "expected `]` in array declarator");
                            match self.peek() {
                                Some(Token::LParen | Token::LBrace | Token::LBracket) => depth += 1,
                                Some(Token::RParen | Token::RBrace | Token::RBracket) => depth -= 1,
                                _ => {}
                            }
                            self.pos += 1;
                        }
                        let bound_tokens = &self.tokens[start..self.pos];
                        let size = match const_expr::Parser::evaluate(bound_tokens) {
                            Ok(value) => Expr::IntLit(value),
                            Err(_) => {
                                let expression = const_expr::Parser::parse(bound_tokens)
                                    .unwrap_or_else(|error| panic!("invalid array bound: {error}"));
                                Expr::Const(Box::new(expression))
                            }
                        };
                        ArraySize::Expression(Box::new(span_tokens(size, bound_tokens)))
                    };
                    assert!(
                        self.matches(Token::RBracket),
                        "expected `]` in array declarator"
                    );
                    Declarator::Array {
                        inner: Box::new(declarator),
                        size,
                    }
                }
                Some(Token::LParen) => {
                    let (parameters, variadic) = self.parse_parameters()?;
                    Declarator::Function {
                        inner: Box::new(declarator),
                        parameters,
                        variadic,
                    }
                }
                _ => break,
            };
        }
        Ok(declarator)
    }

    pub(crate) fn take_qualifiers(&mut self) -> Qualifiers {
        let mut qualifiers = Qualifiers::default();
        while let Some(qualifier) = self.take_qualifier() {
            match qualifier {
                Keyword::Const => qualifiers.is_const = true,
                Keyword::Volatile => qualifiers.is_volatile = true,
                Keyword::Restrict => qualifiers.is_restrict = true,
                Keyword::Atomic => qualifiers.is_atomic = true,
                _ => unreachable!(),
            }
        }
        qualifiers
    }

    fn take_qualifier(&mut self) -> Option<Keyword> {
        let Some(Token::Keyword(keyword)) = self.peek() else {
            return None;
        };
        if *keyword == Keyword::Atomic && self.tokens.value_at(self.pos + 1) == Some(&Token::LParen)
        {
            return None;
        }
        if !matches!(
            keyword,
            Keyword::Const | Keyword::Volatile | Keyword::Restrict | Keyword::Atomic
        ) {
            return None;
        }
        let keyword = *keyword;
        self.matches(Token::Keyword(keyword));
        Some(keyword)
    }

    fn parse_parameters(&mut self) -> Result<(Vec<Parameter>, bool), DeclaratorError> {
        self.expect(
            Token::LParen,
            DeclaratorError::ExpectedToken(Token::LParen, "in function declarator"),
        )?;
        if self.matches(Token::RParen) {
            return Ok((vec![], false));
        }
        if self.peek() == Some(&Token::Keyword(Keyword::Void))
            && self.tokens.value_at(self.pos + 1) == Some(&Token::RParen)
        {
            self.pos += 2;
            return Ok((vec![], false));
        }

        let mut parameters = Vec::new();
        let mut variadic = false;
        loop {
            if self.matches(Token::Ellipsis) {
                self.expect(
                    Token::RParen,
                    DeclaratorError::ExpectedToken(Token::RParen, "after `...`"),
                )?;
                variadic = true;
                break;
            }
            let leading_qualifiers = self.take_qualifiers();
            let base_ty = self.parse_base_type()?;
            let trailing_qualifiers = self.take_qualifiers();
            let qualifiers = Qualifiers {
                is_const: leading_qualifiers.is_const || trailing_qualifiers.is_const,
                is_volatile: leading_qualifiers.is_volatile || trailing_qualifiers.is_volatile,
                is_restrict: leading_qualifiers.is_restrict || trailing_qualifiers.is_restrict,
                is_atomic: leading_qualifiers.is_atomic || trailing_qualifiers.is_atomic,
            };
            let ty = if qualifiers == Qualifiers::default() {
                base_ty
            } else {
                CType::Qualified {
                    qualifiers,
                    ty: Box::new(base_ty),
                }
            };
            let declarator = match self.peek() {
                Some(Token::Comma) | Some(Token::RParen) => None,
                _ => Some(self.parse_declarator(true)?),
            };
            let (attributes, position) = parse_attribute_groups(self.tokens, self.pos)?;
            self.pos = position;
            parameters.push(Parameter {
                ty: apply_vector_attributes(ty, &attributes),
                declarator,
                attributes,
            });
            if self.matches(Token::RParen) {
                break;
            }
            self.expect(
                Token::Comma,
                DeclaratorError::ExpectedToken(Token::Comma, "between parameters"),
            )?;
        }
        Ok((parameters, variadic))
    }

    fn expect(&mut self, token: Token, err: DeclaratorError) -> Result<(), DeclaratorError> {
        if self.consume(token) {
            Ok(())
        } else {
            Err(err)
        }
    }
}

fn apply_abstract_declarator(ty: CType, declarator: Declarator) -> CType {
    match declarator {
        Declarator::Abstract | Declarator::Name(_) => ty,
        Declarator::Grouped(inner) => apply_abstract_declarator(ty, *inner),
        Declarator::Pointer { qualifiers, inner } => CType::Pointer {
            qualifiers,
            pointee: Box::new(apply_abstract_declarator(ty, *inner)),
        },
        Declarator::Array { inner, size } => match *inner {
            Declarator::Grouped(grouped) => apply_abstract_declarator(
                CType::Array {
                    element: Box::new(ty),
                    size,
                },
                *grouped,
            ),
            inner => CType::Array {
                element: Box::new(apply_abstract_declarator(ty, inner)),
                size,
            },
        },
        Declarator::Function {
            inner,
            parameters,
            variadic,
        } => match *inner {
            Declarator::Grouped(grouped) => apply_abstract_declarator(
                CType::Function {
                    return_type: Box::new(ty),
                    parameters,
                    variadic,
                },
                *grouped,
            ),
            inner => CType::Function {
                return_type: Box::new(apply_abstract_declarator(ty, inner)),
                parameters,
                variadic,
            },
        },
    }
}

impl<'a> Cursor for DeclaratorParser<'a> {
    type Error = DeclaratorError;

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

fn is_target_builtin_name(name: &str) -> bool {
    matches!(
        name,
        "__m128"
            | "__m128d"
            | "__m128i"
            | "__m256"
            | "__m256d"
            | "__m256i"
            | "__m512"
            | "__m512d"
            | "__m512i"
            | "char8_t"
            | "atomic_char8_t"
            | "nullptr_t"
    )
}

impl Parser {
    fn parse_function(&self, nodes: &[PPNode]) -> Result<(FunctionDecl, usize), ParseError> {
        let provenance = self.node_provenance(&nodes[0]);
        let sig_node_count = signature_node_span(nodes);
        let joined_code;
        let code: &str = if sig_node_count == 1 {
            self.node_text(&nodes[0])
        } else {
            joined_code = join_node_text(&nodes[..sig_node_count]);
            &joined_code
        };
        let sig_tokens = self.nodes_tokens(&nodes[..sig_node_count]);
        let mut leading = 0;
        while sig_tokens.value_at(leading) == Some(&Token::Ident("__extension__".to_string())) {
            leading += 1;
        }
        let (mut attributes, mut index) = parse_attribute_groups(&sig_tokens, leading)
            .map_err(|error| self.error_at(Loc::whole(code), error))?;
        let mut qualifiers = Qualifiers::default();
        let mut storage = StorageClass::None;
        let mut is_inline = false;
        let mut is_noreturn = false;
        loop {
            match sig_tokens.value_at(index) {
                Some(Token::Keyword(Keyword::Const)) => qualifiers.is_const = true,
                Some(Token::Keyword(Keyword::Volatile)) => qualifiers.is_volatile = true,
                Some(Token::Keyword(Keyword::Restrict)) => qualifiers.is_restrict = true,
                Some(Token::Keyword(Keyword::Atomic)) => qualifiers.is_atomic = true,
                Some(Token::Keyword(Keyword::Inline)) => is_inline = true,
                Some(Token::Keyword(Keyword::Noreturn)) => is_noreturn = true,
                Some(Token::Keyword(keyword)) => {
                    let next_storage = match keyword {
                        Keyword::Extern => StorageClass::Extern,
                        Keyword::Static => StorageClass::Static,
                        _ => break,
                    };
                    if storage != StorageClass::None {
                        return Err(self.error_at(Loc::whole(code), "multiple storage classes"));
                    }
                    storage = next_storage;
                }
                _ => break,
            }
            index += 1;
            let (more_attributes, position) = parse_attribute_groups(&sig_tokens, index)
                .map_err(|error| self.error_at(Loc::whole(code), error))?;
            attributes.extend(more_attributes);
            index = position;
        }
        let mut return_type_parser = DeclaratorParser {
            tokens: &sig_tokens,
            pos: index,
            typedef_names: &self.typedef_names,
        };
        let mut ret_type = return_type_parser
            .parse_base_type()
            .map_err(|error| self.error_at(Loc::whole(code), error.to_string()))?;
        while return_type_parser.matches(Token::Star) {
            let qualifiers = return_type_parser.take_qualifiers();
            ret_type = CType::Pointer {
                qualifiers,
                pointee: Box::new(ret_type),
            };
        }
        let (mid_attributes, name_index) =
            parse_attribute_groups(&sig_tokens, return_type_parser.pos)
                .map_err(|error| self.error_at(Loc::whole(code), error))?;
        attributes.extend(mid_attributes);
        let name = match sig_tokens.value_at(name_index) {
            Some(Token::Ident(n)) => n.clone(),
            _ => return Err(self.error_at(Loc::whole(code), "expected function name")),
        };
        if sig_tokens.value_at(name_index + 1) != Some(&Token::LParen) {
            return Err(self.error_at(
                Loc::at(code, code.len().saturating_sub(1), 1),
                "expected `(`",
            ));
        }
        if matching_paren(&sig_tokens, name_index + 1).is_none() {
            return Err(self.error_at(
                Loc::at(code, code.find('{').unwrap_or(0), 1),
                "expected `)`",
            ));
        }
        let mut declarator_parser = DeclaratorParser {
            tokens: &sig_tokens,
            pos: name_index + 1,
            typedef_names: &self.typedef_names,
        };
        let (parameters, variadic) = declarator_parser
            .parse_parameters()
            .map_err(|error| self.error_at(Loc::whole(code), error.to_string()))?;
        let (signature_attributes, body_index) =
            parse_attribute_groups(&sig_tokens, declarator_parser.pos)
                .map_err(|error| self.error_at(Loc::whole(code), error))?;
        attributes.extend(signature_attributes);
        if sig_tokens.value_at(body_index) != Some(&Token::LBrace) {
            return Err(self.error_at(
                Loc::at(code, code.len().saturating_sub(1), 1),
                "expected function body",
            ));
        }

        if let Some(same_line_close) = matching_brace(&sig_tokens, body_index) {
            let body = mark_unreachable(
                self.parse_stmts_from_tokens(code, &sig_tokens[body_index + 1..same_line_close])?,
            );
            return Ok((
                FunctionDecl {
                    ret_type,
                    name,
                    parameters,
                    variadic,
                    body,
                    provenance,
                    qualifiers,
                    storage,
                    is_inline,
                    is_noreturn,
                    attributes,
                },
                sig_node_count,
            ));
        }

        let mut depth = 1i32;
        let mut close_idx = None;
        for (offset, node) in nodes[sig_node_count..].iter().enumerate() {
            if let PPNodeKind::Code { .. } = &node.value {
                for token in self.node_tokens(node) {
                    match token.value {
                        Token::LBrace => depth += 1,
                        Token::RBrace => depth -= 1,
                        _ => {}
                    }
                    if depth == 0 {
                        break;
                    }
                }
            }
            if depth == 0 {
                close_idx = Some(sig_node_count + offset);
                break;
            }
        }
        let close_idx = close_idx.ok_or_else(|| {
            self.error_at(
                Loc::at(code, code.len().saturating_sub(1), 1),
                "expected `}`",
            )
        })?;

        if let PPNodeKind::Code { text, .. } = &nodes[close_idx].value {
            let closing_tokens = self.node_tokens(&nodes[close_idx]);
            let (trailing_attributes, _) = parse_attribute_groups(&closing_tokens, 1)
                .map_err(|error| self.error_at(Loc::whole(text), error))?;
            attributes.extend(trailing_attributes);
        }

        let body = mark_unreachable(self.parse_stmt_list(&nodes[sig_node_count..close_idx])?);
        Ok((
            FunctionDecl {
                ret_type,
                name,
                parameters,
                variadic,
                body,
                provenance,
                qualifiers,
                storage,
                is_inline,
                is_noreturn,
                attributes,
            },
            close_idx + 1,
        ))
    }

    fn parse_stmt_list(&self, nodes: &[PPNode]) -> Result<Vec<SpannedStmt>, ParseError> {
        let mut stmts = Vec::new();
        let mut pending_comments = Vec::new();
        let mut run_text = String::new();
        let mut run_tokens = Vec::new();
        for node in nodes {
            match &node.value {
                PPNodeKind::Comment { text, provenance } => {
                    if !run_tokens.is_empty()
                        && let Ok(parsed) = self.parse_stmts_from_tokens(&run_text, &run_tokens)
                    {
                        stmts.extend(parsed);
                        stmts.append(&mut pending_comments);
                        run_text.clear();
                        run_tokens.clear();
                    }
                    let comment = node.clone().with_value(Stmt::Comment {
                        text: text.clone(),
                        loc: node.expansion,
                        provenance: *provenance,
                    });
                    if run_tokens.is_empty() {
                        stmts.push(comment);
                    } else {
                        pending_comments.push(comment);
                    }
                }
                PPNodeKind::Code { text, .. } if !lex(text).is_empty() => {
                    if !run_text.is_empty() {
                        run_text.push(' ');
                    }
                    run_text.push_str(text);
                    run_tokens.extend(self.node_tokens(node));
                }
                PPNodeKind::Code { .. } => {}
            }
        }
        if !run_tokens.is_empty() {
            stmts.extend(self.parse_stmts_from_tokens(&run_text, &run_tokens)?);
        }
        stmts.append(&mut pending_comments);
        Ok(stmts)
    }

    fn parse_stmts_from_tokens(
        &self,
        code: &str,
        tokens: &[Span<Token>],
    ) -> Result<Vec<SpannedStmt>, ParseError> {
        let mut fragment = Fragment::new(self, code, tokens, 0);
        let mut stmts = Vec::new();
        while fragment.pos < fragment.tokens.len() {
            let start = fragment.pos;
            let stmt = self.parse_one_stmt(&mut fragment)?;
            stmts.push(span_tokens(stmt, &fragment.tokens[start..fragment.pos]));
        }
        Ok(stmts)
    }

    fn parse_body(&self, fragment: &mut Fragment) -> Result<Vec<SpannedStmt>, ParseError> {
        if fragment.peek() == Some(&Token::LBrace) {
            let close = matching_brace(fragment.tokens, fragment.pos)
                .ok_or_else(|| fragment.error("expected `}`"))?;
            let body = self.parse_stmts_from_tokens(
                fragment.code,
                &fragment.tokens[fragment.pos + 1..close],
            )?;
            fragment.pos = close + 1;
            Ok(body)
        } else {
            let mut stmts = Vec::new();
            loop {
                let start = fragment.pos;
                let stmt = self.parse_one_stmt(fragment)?;
                let is_label = matches!(stmt, Stmt::Labeled(_));
                stmts.push(span_tokens(stmt, &fragment.tokens[start..fragment.pos]));
                if !is_label {
                    break;
                }
            }
            Ok(stmts)
        }
    }

    fn parse_simple_keyword_stmt(
        &self,
        fragment: &mut Fragment,
        stmt: Stmt,
    ) -> Result<Stmt, ParseError> {
        fragment.pos += 1;
        fragment.expect(Token::Semi, "expected `;`")?;
        Ok(stmt)
    }

    fn try_parse_nested_function(
        &self,
        code: &str,
        tokens: &[Span<Token>],
        start: usize,
    ) -> Result<Option<(FunctionDecl, usize)>, ParseError> {
        let fragment = Fragment::new(self, code, tokens, start);
        let (mut attributes, mut index) =
            parse_attribute_groups(tokens, start).map_err(|error| fragment.error(error))?;
        let mut qualifiers = Qualifiers::default();
        let mut storage = StorageClass::None;
        let mut is_inline = false;
        let mut is_noreturn = false;
        loop {
            match tokens.value_at(index) {
                Some(Token::Keyword(Keyword::Const)) => qualifiers.is_const = true,
                Some(Token::Keyword(Keyword::Volatile)) => qualifiers.is_volatile = true,
                Some(Token::Keyword(Keyword::Restrict)) => qualifiers.is_restrict = true,
                Some(Token::Keyword(Keyword::Atomic)) => qualifiers.is_atomic = true,
                Some(Token::Keyword(Keyword::Inline)) => is_inline = true,
                Some(Token::Keyword(Keyword::Noreturn)) => is_noreturn = true,
                Some(Token::Keyword(keyword)) => {
                    let next_storage = match keyword {
                        Keyword::Extern => StorageClass::Extern,
                        Keyword::Static => StorageClass::Static,
                        Keyword::Auto => StorageClass::Auto,
                        Keyword::Register => StorageClass::Register,
                        _ => break,
                    };
                    if storage != StorageClass::None {
                        return Ok(None);
                    }
                    storage = next_storage;
                }
                _ => break,
            }
            index += 1;
        }
        let mut return_type_parser = DeclaratorParser::new(tokens, index, &self.typedef_names);
        let Ok(mut ret_type) = return_type_parser.parse_base_type() else {
            return Ok(None);
        };
        while return_type_parser.matches(Token::Star) {
            let pointer_qualifiers = return_type_parser.take_qualifiers();
            ret_type = CType::Pointer {
                qualifiers: pointer_qualifiers,
                pointee: Box::new(ret_type),
            };
        }
        let name_index = return_type_parser.position();
        let Some(Token::Ident(name)) = tokens.value_at(name_index) else {
            return Ok(None);
        };
        if tokens.value_at(name_index + 1) != Some(&Token::LParen) {
            return Ok(None);
        }
        let mut declarator_parser =
            DeclaratorParser::new(tokens, name_index + 1, &self.typedef_names);
        let Ok((parameters, variadic)) = declarator_parser.parse_parameters() else {
            return Ok(None);
        };
        let (signature_attributes, body_index) =
            parse_attribute_groups(tokens, declarator_parser.position())
                .map_err(|error| fragment.error(error))?;
        if tokens.value_at(body_index) != Some(&Token::LBrace) {
            return Ok(None);
        }
        attributes.extend(signature_attributes);
        let close =
            matching_brace(tokens, body_index).ok_or_else(|| fragment.error("expected `}`"))?;
        let body =
            mark_unreachable(self.parse_stmts_from_tokens(code, &tokens[body_index + 1..close])?);
        Ok(Some((
            FunctionDecl {
                ret_type,
                name: name.clone(),
                parameters,
                variadic,
                body,
                provenance: Provenance::default(),
                qualifiers,
                storage,
                is_inline,
                is_noreturn,
                attributes,
            },
            close + 1,
        )))
    }

    fn starts_declaration(&self, tokens: &[Span<Token>], pos: usize) -> bool {
        match tokens.value_at(pos) {
            Some(Token::Keyword(keyword)) if keyword.is_storage_class_or_specifier() => true,
            Some(Token::Ident(name))
                if matches!(name.as_str(), "_Alignas" | "alignas" | "__auto_type") =>
            {
                true
            }
            Some(token) => const_expr::starts_type_name(token, &self.typedef_names),
            None => false,
        }
    }

    fn declaration_prefix_end(&self, tokens: &[Span<Token>]) -> usize {
        let fallback = tokens
            .values()
            .position(|token| matches!(token, Token::Ident(_)))
            .unwrap_or(0);
        let (_, position) = match parse_attribute_groups(tokens, 0) {
            Ok(result) => result,
            Err(_) => return fallback,
        };
        let mut parser = DeclaratorParser::new(tokens, position, &self.typedef_names);
        loop {
            if parser.take_qualifier().is_some() {
                continue;
            }
            match parser.peek() {
                Some(Token::Keyword(
                    Keyword::Inline
                    | Keyword::Noreturn
                    | Keyword::Constexpr
                    | Keyword::ThreadLocal
                    | Keyword::Typedef
                    | Keyword::Extern
                    | Keyword::Static
                    | Keyword::Auto
                    | Keyword::Register,
                )) => {
                    parser.pos += 1;
                }
                _ => break,
            }
        }
        if parser.parse_base_type().is_err() {
            return fallback;
        }
        while parser.take_qualifier().is_some() {}
        parser.position()
    }

    fn parse_one_stmt(&self, fragment: &mut Fragment) -> Result<Stmt, ParseError> {
        let code = fragment.code;
        let tokens = fragment.tokens;
        let stmt_start = fragment.pos;

        if tokens.value_at(fragment.pos) == Some(&Token::Semi) {
            fragment.pos += 1;
            return Ok(Stmt::Block(Vec::new()));
        }

        if tokens.value_at(fragment.pos) == Some(&Token::Keyword(Keyword::StaticAssert)) {
            let end = top_level_semi(&tokens[fragment.pos..])
                .map(|position| fragment.pos + position)
                .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
            let assertion = self.parse_static_assert(code, &tokens[fragment.pos..=end])?;
            fragment.pos = end + 1;
            return Ok(Stmt::StaticAssert(assertion));
        }

        if tokens.value_at(fragment.pos) == Some(&Token::LParen)
            && tokens.value_at(fragment.pos + 1) == Some(&Token::LBrace)
        {
            let close = matching_brace(tokens, fragment.pos + 1)
                .ok_or_else(|| self.error_at(Loc::whole(code), "expected `}`"))?;
            if tokens.value_at(close + 1) != Some(&Token::RParen) {
                return Err(self.error_at(Loc::whole(code), "expected `)`"));
            }
            let body = self.parse_stmts_from_tokens(code, &tokens[fragment.pos + 2..close])?;
            fragment.pos = close + 2;
            if tokens.value_at(fragment.pos) == Some(&Token::Semi) {
                fragment.pos += 1;
            }
            return Ok(Stmt::Expr(span_tokens(
                Expr::StatementExpression(body),
                &tokens[stmt_start..fragment.pos],
            )));
        }

        if let Some(Token::Ident(name)) = tokens.value_at(fragment.pos)
            && tokens.value_at(fragment.pos + 1) == Some(&Token::Colon)
        {
            let name = name.clone();
            fragment.pos += 2;
            return Ok(Stmt::Labeled(name));
        }

        if tokens.value_at(fragment.pos) == Some(&Token::Ident("__label__".into())) {
            let end = top_level_semi(&tokens[fragment.pos..])
                .map(|position| fragment.pos + position)
                .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
            let names = split_top_level(&tokens[fragment.pos + 1..end], &Token::Comma)
                .into_iter()
                .filter(|part| !part.is_empty())
                .map(|part| match part.as_slice() {
                    [single] => match &single.value {
                        Token::Ident(name) => Ok(name.clone()),
                        _ => Err(self.error_at(Loc::whole(code), "expected label name")),
                    },
                    _ => Err(self.error_at(Loc::whole(code), "expected label name")),
                })
                .collect::<Result<Vec<_>, _>>()?;
            fragment.pos = end + 1;
            return Ok(Stmt::LocalLabelDecl(names));
        }

        if tokens.value_at(fragment.pos) == Some(&Token::LBracket)
            && tokens.value_at(fragment.pos + 1) == Some(&Token::LBracket)
        {
            let (_, position) = parse_attribute_groups(tokens, fragment.pos)
                .map_err(|error| fragment.error(error))?;
            fragment.pos = position;
            fragment.expect(Token::Semi, "expected `;` after attributes")?;
            return self.parse_one_stmt(fragment);
        }

        if let Some(Token::Ident(name)) = tokens.value_at(fragment.pos)
            && matches!(name.as_str(), "asm" | "__asm__" | "__asm")
        {
            let mut cursor = fragment.pos + 1;
            while matches!(
                tokens.value_at(cursor),
                Some(Token::Keyword(
                    Keyword::Volatile | Keyword::Inline | Keyword::Goto
                ))
            ) {
                cursor += 1;
            }
            if tokens.value_at(cursor) == Some(&Token::LParen) {
                let close = matching_paren(tokens, cursor).ok_or_else(|| {
                    self.error_at(Loc::whole(code), "expected `)` in asm statement")
                })?;
                let end = if tokens.value_at(close + 1) == Some(&Token::Semi) {
                    close + 1
                } else {
                    close
                };
                let text = tokens[fragment.pos..=end]
                    .values()
                    .map(String::from)
                    .collect::<Vec<_>>()
                    .join(" ");
                fragment.pos = end + 1;
                return Ok(Stmt::Asm(text));
            }
        }

        if tokens.value_at(fragment.pos) == Some(&Token::LBrace) {
            let close = matching_brace(tokens, fragment.pos)
                .ok_or_else(|| self.error_at(Loc::whole(code), "expected `}`"))?;
            let body = self.parse_stmts_from_tokens(code, &tokens[fragment.pos + 1..close])?;
            fragment.pos = close + 1;
            return Ok(Stmt::Block(body));
        }

        if self.starts_declaration(tokens, fragment.pos)
            && let Some((function, next)) =
                self.try_parse_nested_function(code, tokens, fragment.pos)?
        {
            fragment.pos = next;
            return Ok(Stmt::NestedFunction(Box::new(function)));
        }

        if self.starts_declaration(tokens, fragment.pos) {
            let end = top_level_semi(&tokens[fragment.pos..])
                .map(|position| fragment.pos + position)
                .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
            let declaration_tokens = &tokens[fragment.pos..=end];
            let parts = if declaration_tokens
                .last()
                .is_some_and(|token| token.value == Token::Semi)
            {
                &declaration_tokens[..declaration_tokens.len() - 1]
            } else {
                declaration_tokens
            };
            let parts = split_top_level(parts, &Token::Comma);
            fragment.pos = end + 1;
            if parts.len() == 1 {
                return Ok(Stmt::Decl(
                    self.parse_declaration_tokens(code, declaration_tokens)?,
                ));
            }
            let prefix = parts[0][..self.declaration_prefix_end(&parts[0])].to_vec();
            let declarations = parts
                .into_iter()
                .filter(|part| !part.is_empty())
                .enumerate()
                .map(|(index, mut part)| {
                    if index != 0 {
                        let mut with_prefix = prefix.clone();
                        with_prefix.append(&mut part);
                        part = with_prefix;
                    }
                    part.push(synthetic(Token::Semi));
                    self.parse_declaration_tokens(code, &part)
                        .map(|declaration| span_tokens(Stmt::Decl(declaration), &part))
                })
                .collect::<Result<Vec<_>, _>>()?;
            return Ok(Stmt::Block(declarations));
        }

        match tokens.value_at(fragment.pos) {
            Some(Token::Keyword(Keyword::Return)) => {
                let start = fragment.pos + 1;
                let end = top_level_semi(&tokens[start..])
                    .map(|position| start + position)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
                fragment.pos = end + 1;
                if start == end {
                    Ok(Stmt::ReturnVoid)
                } else {
                    Ok(Stmt::Return(
                        self.parse_expression(code, &tokens[start..end])?,
                    ))
                }
            }
            Some(Token::Keyword(Keyword::Break)) => {
                self.parse_simple_keyword_stmt(fragment, Stmt::Break)
            }
            Some(Token::Keyword(Keyword::Continue)) => {
                self.parse_simple_keyword_stmt(fragment, Stmt::Continue)
            }
            Some(Token::Keyword(Keyword::Goto))
                if tokens.value_at(fragment.pos + 1) == Some(&Token::Star) =>
            {
                let start = fragment.pos + 2;
                let end = top_level_semi(&tokens[start..])
                    .map(|position| start + position)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
                let target = self.parse_expression(code, &tokens[start..end])?;
                fragment.pos = end + 1;
                Ok(Stmt::ComputedGoto(target))
            }
            Some(Token::Keyword(Keyword::Goto)) => {
                fragment.pos += 1;
                let label = fragment.expect_ident("expected label after `goto`")?;
                fragment.expect(Token::Semi, "expected `;` after `goto` label")?;
                Ok(Stmt::Goto(label))
            }
            Some(Token::Keyword(Keyword::Case)) => {
                let start = fragment.pos + 1;
                let colon = tokens[start..]
                    .values()
                    .position(|token| *token == Token::Colon)
                    .map(|position| start + position)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `:` after `case`"))?;
                let range = tokens[start..colon]
                    .values()
                    .position(|token| *token == Token::Ellipsis);
                let value = if let Some(range) = range {
                    let range_start = self.parse_expression(code, &tokens[start..start + range])?;
                    let range_end =
                        self.parse_expression(code, &tokens[start + range + 1..colon])?;
                    fragment.pos = colon + 1;
                    return Ok(Stmt::CaseRange {
                        start: range_start,
                        end: range_end,
                    });
                } else {
                    self.parse_expression(code, &tokens[start..colon])?
                };
                fragment.pos = colon + 1;
                Ok(Stmt::Case(value))
            }
            Some(Token::Keyword(Keyword::Default)) => {
                fragment.pos += 1;
                fragment.expect(Token::Colon, "expected `:` after `default`")?;
                Ok(Stmt::Default)
            }
            Some(Token::Keyword(Keyword::If)) => {
                let open = fragment.pos + 1;
                if tokens.value_at(open) != Some(&Token::LParen) {
                    return Err(self.error_at(Loc::whole(code), "expected `(` after `if`"));
                }
                let close = matching_paren(tokens, open)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `)`"))?;
                let condition = self.parse_expression(code, &tokens[open + 1..close])?;
                fragment.pos = close + 1;
                let then_branch = self.parse_body(fragment)?;
                let else_branch = if fragment.peek() == Some(&Token::Keyword(Keyword::Else)) {
                    fragment.pos += 1;
                    Some(self.parse_body(fragment)?)
                } else {
                    None
                };
                Ok(Stmt::If {
                    condition,
                    then_branch,
                    else_branch,
                })
            }
            Some(Token::Keyword(Keyword::While)) => {
                let open = fragment.pos + 1;
                if tokens.value_at(open) != Some(&Token::LParen) {
                    return Err(self.error_at(Loc::whole(code), "expected `(` after `while`"));
                }
                let close = matching_paren(tokens, open)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `)`"))?;
                let condition = self.parse_expression(code, &tokens[open + 1..close])?;
                fragment.pos = close + 1;
                let body = self.parse_body(fragment)?;
                Ok(Stmt::While { condition, body })
            }
            Some(Token::Keyword(Keyword::Do)) => {
                fragment.pos += 1;
                let body = self.parse_body(fragment)?;
                if tokens.value_at(fragment.pos) != Some(&Token::Keyword(Keyword::While)) {
                    return Err(self.error_at(Loc::whole(code), "expected `while` after `do` body"));
                }
                let open = fragment.pos + 1;
                if tokens.value_at(open) != Some(&Token::LParen) {
                    return Err(self.error_at(Loc::whole(code), "expected `(` after `while`"));
                }
                let close = matching_paren(tokens, open)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `)`"))?;
                let condition = self.parse_expression(code, &tokens[open + 1..close])?;
                if tokens.value_at(close + 1) != Some(&Token::Semi) {
                    return Err(self.error_at(Loc::whole(code), "expected `;` after `do`-`while`"));
                }
                fragment.pos = close + 2;
                Ok(Stmt::DoWhile { body, condition })
            }
            Some(Token::Keyword(Keyword::For)) => {
                let open = fragment.pos + 1;
                if tokens.value_at(open) != Some(&Token::LParen) {
                    return Err(self.error_at(Loc::whole(code), "expected `(` after `for`"));
                }
                let close = matching_paren(tokens, open)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `)`"))?;
                let clause = &tokens[open + 1..close];
                let first_semi = top_level_semi(clause)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;` in `for`"))?;
                let init_tokens = &clause[..first_semi];
                let rest = &clause[first_semi + 1..];
                let second_semi = top_level_semi(rest)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;` in `for`"))?;
                let condition_tokens = &rest[..second_semi];
                let increment_tokens = &rest[second_semi + 1..];
                let init = if init_tokens.is_empty() {
                    None
                } else if self.starts_declaration(init_tokens, 0) {
                    let mut decl_tokens = init_tokens.to_vec();
                    decl_tokens.push(synthetic(Token::Semi));
                    Some(Box::new(span_tokens(
                        Stmt::Decl(self.parse_declaration_tokens(code, &decl_tokens)?),
                        init_tokens,
                    )))
                } else {
                    Some(Box::new(span_tokens(
                        Stmt::Expr(self.parse_expression(code, init_tokens)?),
                        init_tokens,
                    )))
                };
                let condition = if condition_tokens.is_empty() {
                    None
                } else {
                    Some(self.parse_expression(code, condition_tokens)?)
                };
                let increment = if increment_tokens.is_empty() {
                    None
                } else {
                    Some(self.parse_expression(code, increment_tokens)?)
                };
                fragment.pos = close + 1;
                let body = self.parse_body(fragment)?;
                Ok(Stmt::For {
                    init,
                    condition,
                    increment,
                    body,
                })
            }
            Some(Token::Keyword(Keyword::Switch)) => {
                let open = fragment.pos + 1;
                if tokens.value_at(open) != Some(&Token::LParen) {
                    return Err(self.error_at(Loc::whole(code), "expected `(` after `switch`"));
                }
                let close = matching_paren(tokens, open)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `)`"))?;
                let discriminant = self.parse_expression(code, &tokens[open + 1..close])?;
                fragment.pos = close + 1;
                let body = self.parse_body(fragment)?;
                Ok(Stmt::Switch { discriminant, body })
            }
            _ => {
                let end = top_level_semi(&tokens[fragment.pos..])
                    .map(|position| fragment.pos + position)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
                let expression = self.parse_expression(code, &tokens[fragment.pos..end])?;
                fragment.pos = end + 1;
                Ok(Stmt::Expr(expression))
            }
        }
    }

    fn parse_expression(
        &self,
        code: &str,
        tokens: &[Span<Token>],
    ) -> Result<SpannedExpr, ParseError> {
        if tokens.is_empty() {
            return Err(self.error_at(Loc::whole(code), "expected expression"));
        }
        if let [single] = tokens
            && let Some(expression) = const_expr::string_literal_expr(Some(&single.value))
        {
            return Ok(single.clone().with_value(expression));
        }
        let tokens = coalesce_string_literals(tokens);
        const_expr::Parser::parse_expression(&tokens, &self.typedef_names)
            .map(|expression| span_tokens(Expr::Const(Box::new(expression)), &tokens))
            .map_err(|error| self.error_at(Loc::whole(code), error.to_string()))
    }
}

fn coalesce_string_literals(tokens: &[Span<Token>]) -> Vec<Span<Token>> {
    let mut result = Vec::with_capacity(tokens.len());
    for token in tokens {
        let Some(previous) = result.last_mut() else {
            result.push(token.clone());
            continue;
        };
        match (&mut previous.value, &token.value) {
            (Token::StringLit(left), Token::StringLit(right)) => {
                left.push_str(right);
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
