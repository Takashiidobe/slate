use super::asm::is_asm_keyword;
use super::attributes::apply_vector_attributes;
use super::declarator::{DeclaratorParser, apply_abstract_declarator};
use super::{Loc, Parser, lex, span_decl_result, span_pp_nodes, span_tokens, synthetic};
use crate::ast::*;
use crate::const_expr;
use crate::error::ParseError;
use crate::lexer::{Keyword, Token, TokenSpanExt};
use crate::pp::{PPNode, PPNodeKind};
use crate::reachability::filter_translation_unit;
use std::collections::{HashMap, HashSet};

impl Parser {
    pub(super) fn parse_declaration_tokens(
        &self,
        code: &str,
        tokens: &[Span<Token>],
    ) -> Result<Declaration, ParseError> {
        self.parse_declaration_tokens_with(code, tokens, true)
    }

    fn parse_declaration_tokens_with(
        &self,
        _code: &str,
        tokens: &[Span<Token>],
        allow_asm_label: bool,
    ) -> Result<Declaration, ParseError> {
        let mut parser = DeclaratorParser {
            tokens,
            pos: 0,
            typedef_names: &self.typedef_names,
            biggest_alignment: self.biggest_alignment,
        };
        while parser.peek() == Some(&Token::Ident("__extension__".to_string())) {
            parser.pos += 1;
        }
        let (mut attributes, position) = self
            .parse_attribute_groups(parser.tokens, parser.pos)
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
            let (more_attributes, position) = self
                .parse_attribute_groups(parser.tokens, parser.pos)
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
        let (mid_attributes, position) = self
            .parse_attribute_groups(parser.tokens, parser.pos)
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
        if !allow_asm_label && is_asm_keyword(parser.peek()) {
            return Err(self.error_at_tokens(
                tokens,
                parser.pos,
                "expected `;` at end of declaration list",
            ));
        }
        let asm_label = parser
            .parse_asm_label(storage)
            .map_err(|error| self.error_at_tokens(tokens, parser.pos, error))?;
        let (trailing_attributes, position) = self
            .parse_attribute_groups(parser.tokens, parser.pos)
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
                Some(
                    parser
                        .parse_initializer(&self.typedef_names)
                        .map_err(|error| {
                            self.error_at_tokens(tokens, parser.pos, error.to_string())
                        })?,
                )
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
            asm_label,
            initializer,
            attributes,
        })
    }

    pub(super) fn parse_field_declaration_tokens(
        &self,
        code: &str,
        tokens: &[Span<Token>],
    ) -> Result<(Declaration, Option<SpannedExpr>), ParseError> {
        let Some(colon) = top_level_token(tokens, &Token::Colon) else {
            return self
                .parse_declaration_tokens_with(code, tokens, false)
                .map(|declaration| (declaration, None));
        };
        let Some(semi) = top_level_token(tokens, &Token::Semi) else {
            return Err(self.error_at_tokens(tokens, tokens.len(), "expected `;`"));
        };
        let (width, width_end) =
            const_expr::Parser::parse_one(tokens, colon + 1, &self.typedef_names)
                .map_err(|error| self.error_at_tokens(tokens, colon + 1, error.to_string()))?;
        let mut declaration_tokens = tokens[..colon].to_vec();
        declaration_tokens.extend_from_slice(&tokens[width_end..=semi]);
        let declaration = self.parse_declaration_tokens_with(code, &declaration_tokens, false)?;
        let bit_width = span_tokens(Expr::Const(Box::new(width)), &tokens[colon + 1..width_end]);
        Ok((declaration, Some(bit_width)))
    }

    pub(super) fn parse_static_assert(
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

    pub(super) fn parse_nodes(
        &mut self,
        nodes: &[PPNode],
        root_file: FileId,
    ) -> Result<TranslationUnit, ParseError> {
        let ast = filter_translation_unit(
            &TranslationUnit {
                decls: self.parse_decls(nodes)?,
                flavor: self.flavor(),
            },
            root_file,
        );
        Ok(ast)
    }

    pub(super) fn parse_decls(&mut self, nodes: &[PPNode]) -> Result<Vec<SpannedDecl>, ParseError> {
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

    pub(super) fn parse_top_level_item(
        &mut self,
        nodes: &[PPNode],
    ) -> Result<(Vec<SpannedDecl>, usize), ParseError> {
        if let PPNodeKind::Code { .. } = &nodes[0].value {
            let tokens = self.node_tokens(&nodes[0]);
            let top_level_items = split_top_level_items(&tokens);
            if top_level_items.len() > 1
                && tokens.iter().any(|token| token.spelling != token.expansion)
            {
                let provenance = self.node_provenance(&nodes[0]);
                let mut declarations = Vec::new();
                for item in top_level_items {
                    if item.as_tokens() == [Token::Semi] {
                        continue;
                    }
                    let text = item
                        .values()
                        .map(String::from)
                        .collect::<Vec<_>>()
                        .join(" ");
                    let item_node = Span::cover(
                        PPNodeKind::Code {
                            text,
                            tokens: item,
                            provenance,
                        },
                        &tokens,
                    );
                    let (decls, _) = self.parse_top_level_item(std::slice::from_ref(&item_node))?;
                    declarations.extend(decls);
                }
                return Ok((declarations, 1));
            }
        }
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
            } if text_starts_with_typedef(text) => {
                let tokens = self.node_tokens(&nodes[0]);
                let has_inline_body = matches!(
                    tokens.values().find(|token| {
                        !matches!(token, Token::Keyword(Keyword::Typedef))
                            && **token != Token::Ident("__extension__".to_string())
                    }),
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
                let mut typedefs = Vec::new();
                for statement_tokens in split_top_level(&typedef_tokens, &Token::Semi) {
                    if statement_tokens.is_empty() {
                        continue;
                    }
                    let parts = split_top_level(&statement_tokens, &Token::Comma);
                    let prefix = parts.first().map_or(Vec::new(), |part| {
                        part[..self.declaration_prefix_end(part)].to_vec()
                    });
                    for (index, mut part) in parts.into_iter().enumerate() {
                        if index != 0 {
                            let mut with_prefix = prefix.clone();
                            with_prefix.append(&mut part);
                            part = with_prefix;
                        }
                        part.push(semi.clone());
                        let typedef = self.parse_typedef_line(
                            &typedef_text,
                            &part,
                            self.node_provenance(&nodes[0]),
                        )?;
                        typedefs.push(span_pp_nodes(typedef, &nodes[..span]));
                    }
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
                if let Some(asm) = self.parse_file_scope_asm(self.node_text(&nodes[0]), &tokens)? {
                    return Ok((
                        vec![nodes[0].clone().with_value(Decl::Asm {
                            asm,
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
                    ) || tokens.value_at(index)
                        == Some(&Token::Ident("__extension__".to_string()))
                    {
                        index += 1;
                    }
                    index
                };
                let tag_body_follows = first_equal.is_none()
                    && {
                        let after_name_index = if matches!(
                            tokens.value_at(tag_keyword_index + 1),
                            Some(Token::Ident(_))
                        ) {
                            tag_keyword_index + 2
                        } else {
                            tag_keyword_index + 1
                        };
                        tokens.len() == after_name_index
                    }
                    && nodes.get(1).is_some_and(|node| {
                        self.node_tokens(node).value_at(0) == Some(&Token::LBrace)
                    });
                if matches!(
                    tokens.value_at(tag_keyword_index),
                    Some(Token::Keyword(
                        Keyword::Struct | Keyword::Union | Keyword::Enum
                    ))
                ) && (tag_body_follows
                    || first_lbrace.is_some_and(|brace_index| {
                        first_equal.is_none_or(|equal_index| brace_index < equal_index)
                            && brace_index
                                .checked_sub(1)
                                .and_then(|index| tokens.value_at(index))
                                != Some(&Token::RParen)
                    }))
                {
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
                    signature_node_span(nodes, &self.typedef_names)
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

    pub(super) fn parse_tag_definition(
        &self,
        nodes: &[PPNode],
    ) -> Result<(Vec<Decl>, usize), ParseError> {
        let code = self.node_text(&nodes[0]);
        let all_tokens = self.node_tokens(&nodes[0]);
        let mut extension_prefix = 0usize;
        while all_tokens.value_at(extension_prefix)
            == Some(&Token::Ident("__extension__".to_string()))
        {
            extension_prefix += 1;
        }
        let tokens = &all_tokens[extension_prefix..];
        let is_typedef = tokens.value_at(0) == Some(&Token::Keyword(Keyword::Typedef));
        let tokens = if is_typedef { &tokens[1..] } else { tokens };
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
        let node0_offset = extension_prefix + (is_typedef as usize) + skip;
        let kind = match tokens.value_at(0) {
            Some(Token::Keyword(Keyword::Struct)) => TagKind::Struct,
            Some(Token::Keyword(Keyword::Union)) => TagKind::Union,
            Some(Token::Keyword(Keyword::Enum)) => TagKind::Enum,
            _ => return Err(self.error_at(Loc::whole(code), "expected record or enum")),
        };
        let (mut attributes, name_index) = self
            .parse_record_attributes(tokens)
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
            let (trailing_tokens, consumed) =
                tag_trailing_tokens(nodes, 0, node0_offset + same_line_close + 1)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
            let (trailing_attributes, alias_position) = self
                .parse_attribute_groups(&trailing_tokens, 0)
                .map_err(|error| self.error_at(Loc::whole(code), error))?;
            attributes.extend(trailing_attributes);
            let trailing_alias = if matches!(
                trailing_tokens.value_at(alias_position),
                Some(Token::Ident(_) | Token::Star)
            ) {
                let mut declarator_parser = DeclaratorParser::with_biggest_alignment(
                    &trailing_tokens,
                    alias_position,
                    &self.typedef_names,
                    self.biggest_alignment,
                );
                match declarator_parser.parse_declarator(false) {
                    Ok(declarator) => declarator
                        .name()
                        .map(|name| name.to_string())
                        .map(|name| (name, declarator)),
                    Err(_) => None,
                }
            } else {
                None
            };
            let provenance = self.node_provenance(&nodes[0]);
            let tag_decl = if kind == TagKind::Enum {
                let mut enumerators = Vec::new();
                let mut values = HashMap::new();
                let mut next_value = 0i64;
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
                    let explicit_value = match segment.value_at(1) {
                        None => None,
                        Some(Token::Equal) => {
                            Some(evaluate_enum_expression(&segment[2..], &values).map_err(
                                |error| self.error_at_tokens(&segment[2..], 0, error.to_string()),
                            )?)
                        }
                        _ => {
                            return Err(
                                self.error_at(Loc::whole(code), "expected enumerator value")
                            );
                        }
                    };
                    record_enum_value(
                        &mut values,
                        &mut next_value,
                        enumerator_name,
                        explicit_value,
                    );
                    enumerators.push(Enumerator {
                        name: enumerator_name.clone(),
                        value: explicit_value
                            .map(|value| span_tokens(Expr::IntLit(value), &segment[2..])),
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
                        let (declaration, bit_width) =
                            self.parse_field_declaration_tokens(code, &part)?;
                        fields.push(span_tokens(
                            FieldItem::Field(FieldDecl {
                                declaration,
                                bit_width,
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
            if let Some((alias, declarator)) = trailing_alias {
                decls.push(build_tag_alias_decl(
                    is_typedef, kind, name, alias, declarator, provenance,
                ));
            }
            return Ok((decls, consumed));
        }

        let mut depth = 0i32;
        let mut opened = false;
        let mut close = None;
        for (offset, node) in nodes.iter().enumerate() {
            let PPNodeKind::Code { .. } = &node.value else {
                continue;
            };
            for token in self.node_tokens(node) {
                match token.value {
                    Token::LBrace => {
                        opened = true;
                        depth += 1;
                    }
                    Token::RBrace => {
                        if opened {
                            depth -= 1;
                        }
                        if opened && depth == 0 {
                            close = Some(offset);
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
        let closing_tokens = self.node_tokens(&nodes[close]);
        let close_token = closing_tokens
            .values()
            .position(|token| *token == Token::RBrace)
            .ok_or_else(|| self.error_at(Loc::whole(code), "expected `}`"))?;
        let (trailing_tokens, consumed) = tag_trailing_tokens(nodes, close, close_token + 1)
            .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
        let (trailing, position) = self
            .parse_attribute_groups(&trailing_tokens, 0)
            .map_err(|error| self.error_at(Loc::whole(code), error))?;
        attributes.extend(trailing);
        let trailing_alias = if matches!(
            trailing_tokens.value_at(position),
            Some(Token::Ident(_) | Token::Star)
        ) {
            let mut declarator_parser = DeclaratorParser::with_biggest_alignment(
                &trailing_tokens,
                position,
                &self.typedef_names,
                self.biggest_alignment,
            );
            declarator_parser
                .parse_declarator(false)
                .ok()
                .and_then(|declarator| {
                    let name = declarator.name()?.to_string();
                    Some((name, declarator))
                })
        } else {
            None
        };
        let body_start = if self.node_tokens(&nodes[1]).value_at(0) == Some(&Token::LBrace) {
            2
        } else {
            1
        };
        let provenance = self.node_provenance(&nodes[0]);
        let tag_decl = if kind == TagKind::Enum {
            let mut enumerators = Vec::new();
            let mut values = HashMap::new();
            let mut next_value = 0i64;
            let body_tokens = self.nodes_tokens(&nodes[body_start..close]);
            for segment in split_top_level(&body_tokens, &Token::Comma) {
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
                let explicit_value = match segment.value_at(1) {
                    None => None,
                    Some(Token::Equal) => Some(
                        evaluate_enum_expression(&segment[2..], &values).map_err(|error| {
                            self.error_at_tokens(&segment[2..], 0, error.to_string())
                        })?,
                    ),
                    _ => {
                        return Err(self.error_at(Loc::whole(code), "expected enumerator value"));
                    }
                };
                record_enum_value(
                    &mut values,
                    &mut next_value,
                    enumerator_name,
                    explicit_value,
                );
                enumerators.push(Enumerator {
                    name: enumerator_name.clone(),
                    value: explicit_value
                        .map(|value| span_tokens(Expr::IntLit(value), &segment[2..])),
                });
            }
            Decl::Enum(EnumDecl {
                name: name.clone(),
                enumerators,
                provenance,
            })
        } else {
            let fields = self.parse_field_items(&nodes[body_start..close])?;
            Decl::Record(RecordDecl {
                kind,
                name: name.clone(),
                fields,
                provenance,
                attributes,
            })
        };
        let mut decls = vec![tag_decl];
        if let Some((alias, declarator)) = trailing_alias {
            decls.push(build_tag_alias_decl(
                is_typedef, kind, name, alias, declarator, provenance,
            ));
        }
        Ok((decls, consumed))
    }

    pub(super) fn parse_linkage_spec_block(
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

    pub(super) fn parse_field_items(
        &self,
        nodes: &[PPNode],
    ) -> Result<Vec<SpannedFieldItem>, ParseError> {
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
                    for declaration_tokens in split_top_level(&all_tokens, &Token::Semi) {
                        if declaration_tokens.is_empty() {
                            continue;
                        }
                        let parts = split_top_level(&declaration_tokens, &Token::Comma);
                        let prefix = declaration_tokens
                            [..self.declaration_prefix_end(&declaration_tokens)]
                            .to_vec();
                        for (part_index, mut part) in parts.into_iter().enumerate() {
                            if part_index > 0 {
                                let mut with_prefix = prefix.clone();
                                with_prefix.append(&mut part);
                                part = with_prefix;
                            }
                            part.push(synthetic(Token::Semi));
                            let (declaration, bit_width) =
                                self.parse_field_declaration_tokens(&joined, &part)?;
                            fields.push(span_pp_nodes(
                                FieldItem::Field(FieldDecl {
                                    declaration,
                                    bit_width,
                                    provenance: self.node_provenance(&nodes[start]),
                                }),
                                &nodes[start..index],
                            ));
                        }
                    }
                }
            }
        }
        Ok(fields)
    }

    pub(super) fn record_typedefs(&mut self, decl: &Decl) {
        if let Decl::Typedef { name, .. } = decl {
            self.typedef_names.insert(name.clone());
        }
    }

    pub(super) fn parse_typedef_line(
        &self,
        code: &str,
        tokens: &[Span<Token>],
        provenance: Provenance,
    ) -> Result<Decl, ParseError> {
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
        Ok(Decl::Typedef {
            name: name.to_string(),
            ty,
            asm_label: declaration.asm_label,
            provenance,
            attributes: declaration.attributes,
        })
    }
}

pub(super) fn paren_depth(tokens: &[Span<Token>]) -> i32 {
    tokens.values().fold(0i32, |depth, token| match token {
        Token::LParen => depth + 1,
        Token::RParen => depth - 1,
        _ => depth,
    })
}

pub(super) fn signature_node_span(nodes: &[PPNode], typedef_names: &HashSet<String>) -> usize {
    let mut depth = 0i32;
    let mut paren_group = Vec::new();
    let mut kr_style = false;
    for (index, node) in nodes.iter().enumerate() {
        let PPNodeKind::Code { text, .. } = &node.value else {
            continue;
        };
        for token in lex(text) {
            match token.value {
                Token::LParen => {
                    if depth == 0 {
                        paren_group.clear();
                    } else {
                        paren_group.push(Token::LParen);
                    }
                    depth += 1;
                }
                Token::RParen => {
                    depth -= 1;
                    if depth == 0 {
                        kr_style =
                            bare_identifier_names(paren_group.iter(), typedef_names).is_some();
                    } else {
                        paren_group.push(Token::RParen);
                    }
                }
                Token::LBrace if depth <= 0 => return index + 1,
                Token::Semi if depth <= 0 => {
                    if !kr_style {
                        return index + 1;
                    }
                }
                ref other => {
                    if depth >= 1 {
                        paren_group.push(other.clone());
                    }
                }
            }
        }
    }
    nodes.len().max(1)
}

pub(super) fn bare_identifier_names<'a>(
    tokens: impl IntoIterator<Item = &'a Token>,
    typedef_names: &HashSet<String>,
) -> Option<Vec<String>> {
    let mut names = Vec::new();
    let mut expect_ident = true;
    for token in tokens {
        if expect_ident {
            match token {
                Token::Ident(name) if !typedef_names.contains(name) => names.push(name.clone()),
                _ => return None,
            }
        } else if *token != Token::Comma {
            return None;
        }
        expect_ident = !expect_ident;
    }
    (!names.is_empty() && !expect_ident).then_some(names)
}

pub(super) fn declaration_node_span(nodes: &[PPNode]) -> usize {
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

pub(super) fn tag_trailing_tokens(
    nodes: &[PPNode],
    start_node: usize,
    start_token: usize,
) -> Option<(Vec<Span<Token>>, usize)> {
    let mut tokens = Vec::new();
    let mut depth = 0i32;
    for (node_index, node) in nodes.iter().enumerate().skip(start_node) {
        let node_tokens = match &node.value {
            PPNodeKind::Code { tokens, .. } => tokens,
            PPNodeKind::Comment { .. } => continue,
        };
        let token_start = if node_index == start_node {
            start_token
        } else {
            0
        };
        for token in &node_tokens[token_start..] {
            match token.value {
                Token::LParen | Token::LBrace | Token::LBracket => depth += 1,
                Token::RParen | Token::RBrace | Token::RBracket => depth -= 1,
                Token::Semi if depth == 0 => {
                    tokens.push(token.clone());
                    return Some((tokens, node_index + 1));
                }
                _ => {}
            }
            tokens.push(token.clone());
        }
    }
    None
}

pub(super) fn join_node_text(nodes: &[PPNode]) -> String {
    nodes
        .iter()
        .map(|node| match &node.value {
            PPNodeKind::Comment { text, .. } => text.as_str(),
            PPNodeKind::Code { text, .. } => text.as_str(),
        })
        .collect::<Vec<_>>()
        .join("\n")
}

pub(crate) fn matching_brace(tokens: &[Span<Token>], open: usize) -> Option<usize> {
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

pub(super) fn split_top_level(tokens: &[Span<Token>], delimiter: &Token) -> Vec<Vec<Span<Token>>> {
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

pub(super) fn evaluate_enum_expression(
    tokens: &[Span<Token>],
    values: &HashMap<String, i64>,
) -> Result<i64, const_expr::ConstExprError> {
    let tokens = tokens
        .iter()
        .map(|token| match &token.value {
            Token::Ident(name) => values.get(name).map_or_else(
                || token.clone(),
                |value| token.clone().with_value(Token::IntLit(value.to_string())),
            ),
            _ => token.clone(),
        })
        .collect::<Vec<_>>();
    const_expr::Parser::evaluate(&tokens)
}

pub(super) fn record_enum_value(
    values: &mut HashMap<String, i64>,
    next_value: &mut i64,
    name: &str,
    explicit_value: Option<i64>,
) {
    let value = explicit_value.unwrap_or(*next_value);
    values.insert(name.to_string(), value);
    *next_value = value.wrapping_add(1);
}

pub(super) fn build_tag_alias_decl(
    is_typedef: bool,
    kind: TagKind,
    name: Option<String>,
    alias: String,
    declarator: Declarator,
    provenance: Provenance,
) -> Decl {
    let base = CType::Tagged {
        kind,
        name,
        body: None,
    };
    if is_typedef {
        Decl::Typedef {
            name: alias,
            ty: apply_abstract_declarator(base, declarator),
            asm_label: None,
            provenance,
            attributes: Vec::new(),
        }
    } else {
        Decl::Declaration {
            declaration: Declaration {
                specifiers: DeclarationSpecifiers {
                    ty: base,
                    qualifiers: Qualifiers::default(),
                    storage: StorageClass::None,
                    is_thread_local: false,
                    is_inline: false,
                    is_noreturn: false,
                    is_constexpr: false,
                },
                declarator,
                asm_label: None,
                initializer: None,
                attributes: Vec::new(),
            },
            provenance,
        }
    }
}

pub(super) fn matching_paren(tokens: &[Span<Token>], open: usize) -> Option<usize> {
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

pub(super) fn top_level_semi(tokens: &[Span<Token>]) -> Option<usize> {
    top_level_token(tokens, &Token::Semi)
}

pub(super) fn split_top_level_items(tokens: &[Span<Token>]) -> Vec<Vec<Span<Token>>> {
    let mut items = Vec::new();
    let mut start = 0;
    let mut depth = 0i32;
    let mut function_body = false;
    for (index, token) in tokens.iter().enumerate() {
        match token.value {
            Token::LParen | Token::LBracket => depth += 1,
            Token::LBrace => {
                function_body = depth == 0
                    && index > start
                    && tokens.value_at(index - 1) == Some(&Token::RParen);
                depth += 1;
            }
            Token::RParen | Token::RBracket | Token::RBrace => depth -= 1,
            _ => {}
        }
        let is_function_end = function_body && depth == 0 && token.value == Token::RBrace;
        if depth == 0 && (token.value == Token::Semi || is_function_end) {
            items.push(tokens[start..=index].to_vec());
            start = index + 1;
            function_body = false;
        }
    }
    if start < tokens.len() {
        items.push(tokens[start..].to_vec());
    }
    items
}

pub(super) fn top_level_token(tokens: &[Span<Token>], target: &Token) -> Option<usize> {
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

pub(super) fn text_starts_with_typedef(text: &str) -> bool {
    let text = text.trim_start();
    let text = text
        .strip_prefix("__extension__")
        .map_or(text, str::trim_start);
    text.starts_with("typedef")
}
