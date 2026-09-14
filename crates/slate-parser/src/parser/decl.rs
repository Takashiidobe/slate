use super::asm::is_asm_keyword;
use super::attributes::apply_vector_attributes;
use super::declarator::{DeclaratorParser, IdentifierList};
use super::{Loc, Parser, lex, span_decl_result, span_pp_nodes, span_tokens, synthetic};
use crate::ast::*;
use crate::const_expr;
use crate::error::ParseError;
use crate::lexer::{Keyword, Token, TokenSpanExt};
use crate::pp::{PPNode, PPNodeKind};
use crate::reachability::filter_translation_unit;
use std::collections::HashSet;

impl Parser {
    pub(super) fn parse_declaration_tokens(
        &self,
        tokens: &[Span<Token>],
    ) -> Result<Declaration, ParseError> {
        let mut parser = self.declarator_parser(tokens, 0);
        let mut specifiers = self.parse_declaration_specifiers(&mut parser, false)?;
        let declarators = self.parse_declarator_list(&mut parser, &mut specifiers, false)?;
        Ok(Declaration {
            specifiers,
            declarators: declarators.into_iter().map(into_init_declarator).collect(),
        })
    }

    pub(super) fn parse_field_declaration_tokens(
        &self,
        tokens: &[Span<Token>],
    ) -> Result<(DeclarationSpecifiers, Vec<FieldDeclarator>), ParseError> {
        let mut parser = self.declarator_parser(tokens, 0);
        let mut specifiers = self.parse_declaration_specifiers(&mut parser, false)?;
        let declarators = self.parse_declarator_list(&mut parser, &mut specifiers, true)?;
        Ok((
            specifiers,
            declarators.into_iter().map(into_field_declarator).collect(),
        ))
    }

    pub(super) fn declarator_parser<'a>(
        &'a self,
        tokens: &'a [Span<Token>],
        pos: usize,
    ) -> DeclaratorParser<'a> {
        DeclaratorParser {
            tokens,
            pos,
            typedef_names: &self.typedef_names,
            biggest_alignment: self.biggest_alignment,
            statements: Some(self),
            identifier_list: IdentifierList::Rejected,
        }
    }

    fn parse_specifier_keywords(
        &self,
        parser: &mut DeclaratorParser,
        specifiers: &mut DeclarationSpecifiers,
    ) -> Result<(), ParseError> {
        parser
            .parse_specifier_keywords(specifiers)
            .map_err(|error| self.error_at_tokens(parser.tokens, parser.pos, error.to_string()))
    }

    pub(super) fn parse_declaration_specifiers(
        &self,
        parser: &mut DeclaratorParser,
        implicit_int_function: bool,
    ) -> Result<DeclarationSpecifiers, ParseError> {
        parser
            .parse_specifiers(implicit_int_function)
            .map_err(|error| self.error_at_tokens(parser.tokens, parser.pos, error.to_string()))
    }

    fn parse_declarator_list(
        &self,
        parser: &mut DeclaratorParser,
        specifiers: &mut DeclarationSpecifiers,
        is_field: bool,
    ) -> Result<Vec<Span<ParsedDeclarator>>, ParseError> {
        let tokens = parser.tokens;
        let mut declarators = Vec::new();
        if parser.peek() != Some(&Token::Semi) {
            loop {
                declarators.push(self.parse_one_declarator(parser, specifiers, is_field)?);
                if !parser.matches(Token::Comma) {
                    break;
                }
            }
        }
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
        let vector_attributes = specifiers
            .attributes
            .iter()
            .chain(declarators.iter().flat_map(|parsed| &parsed.attributes))
            .cloned()
            .collect::<Vec<_>>();
        specifiers.ty = apply_vector_attributes(specifiers.ty.clone(), &vector_attributes);
        Ok(declarators)
    }

    fn parse_one_declarator(
        &self,
        parser: &mut DeclaratorParser,
        specifiers: &DeclarationSpecifiers,
        is_field: bool,
    ) -> Result<Span<ParsedDeclarator>, ParseError> {
        let tokens = parser.tokens;
        let start = parser.pos;
        let (mut attributes, position) = self
            .parse_attribute_groups(tokens, parser.pos)
            .map_err(|error| self.error_at_tokens(tokens, parser.pos, error))?;
        parser.pos = position;
        let declarator = if is_field && parser.peek() == Some(&Token::Colon) {
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
        let bit_width = if is_field && parser.matches(Token::Colon) {
            let start = parser.pos;
            let (width, end) =
                const_expr::Parser::parse_one(tokens, start, &self.typedef_names, Some(self))
                    .map_err(|error| self.error_at_tokens(tokens, start, error.to_string()))?;
            parser.pos = end;
            Some(width)
        } else {
            None
        };
        if is_field && is_asm_keyword(parser.peek()) {
            return Err(self.error_at_tokens(
                tokens,
                parser.pos,
                "expected `;` at end of declaration list",
            ));
        }
        let asm_label = parser
            .parse_asm_label(specifiers.storage)
            .map_err(|error| self.error_at_tokens(tokens, parser.pos, error))?;
        let (trailing_attributes, position) = self
            .parse_attribute_groups(tokens, parser.pos)
            .map_err(|error| self.error_at_tokens(tokens, parser.pos, error))?;
        parser.pos = position;
        attributes.extend(trailing_attributes);
        let initializer = if parser.matches(Token::Equal) {
            Some(self.parse_declaration_initializer(parser)?)
        } else {
            None
        };
        Ok(Span::cover(
            ParsedDeclarator {
                declarator,
                bit_width,
                asm_label,
                attributes,
                initializer,
            },
            &tokens[start..parser.pos],
        ))
    }

    fn parse_declaration_initializer(
        &self,
        parser: &mut DeclaratorParser,
    ) -> Result<Initializer, ParseError> {
        let tokens = parser.tokens;
        parser
            .parse_initializer(&self.typedef_names)
            .map_err(|error| self.error_at_tokens(tokens, parser.pos, error.to_string()))
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
        self.tags.borrow_mut().clear();
        let decls = self.parse_decls(nodes)?;
        let options = self.effective_options();
        let ast = filter_translation_unit(
            &TranslationUnit {
                options: options.clone(),
                decls,
                tags: self.tags.take(),
                flavor: self.flavor(),
                target: options.effective_target(self.target),
            },
            root_file,
        );
        Ok(ast)
    }

    pub(super) fn parse_decls(&mut self, nodes: &[PPNode]) -> Result<Vec<Decl>, ParseError> {
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
    ) -> Result<(Vec<Decl>, usize), ParseError> {
        if let PPNodeKind::Code { .. } = &nodes[0].value {
            let tokens = self.node_tokens(&nodes[0]);
            let top_level_items = split_top_level_items(&tokens, &self.typedef_names);
            if top_level_items.len() > 1 {
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
            PPNodeKind::Comment { provenance, .. } => {
                let (group, consumed) = self.comment_group(nodes, *provenance);
                Ok((vec![group.map(DeclKind::Comment)], consumed))
            }
            PPNodeKind::Code { text, .. } if text_starts_with_typedef(text) => {
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
                for mut statement_tokens in split_top_level(&typedef_tokens, &Token::Semi) {
                    if statement_tokens.is_empty() {
                        continue;
                    }
                    statement_tokens.push(semi.clone());
                    let typedef =
                        DeclKind::Declaration(self.parse_declaration_tokens(&statement_tokens)?);
                    typedefs.push(span_pp_nodes(typedef, &nodes[..span]));
                }
                Ok((typedefs, span))
            }
            PPNodeKind::Code { .. } => {
                let tokens = self.node_tokens(&nodes[0]);
                if tokens.value_at(0) == Some(&Token::Keyword(Keyword::StaticAssert)) {
                    return Ok((
                        vec![nodes[0].clone().with_value(DeclKind::StaticAssert(
                            self.parse_static_assert(self.node_text(&nodes[0]), &tokens)?,
                        ))],
                        1,
                    ));
                }
                if let Some(asm) = self.parse_file_scope_asm(self.node_text(&nodes[0]), &tokens)? {
                    return Ok((vec![nodes[0].clone().with_value(DeclKind::Asm(asm))], 1));
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
                            || tokens.value_at(tag_keyword_index)
                                == Some(&Token::Keyword(Keyword::Enum))
                                && tokens.value_at(after_name_index) == Some(&Token::Colon)
                                && !tokens.contains_value(&Token::Semi)
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
                let item_tokens = if item_span == 1 {
                    tokens
                } else {
                    self.nodes_tokens(&nodes[..item_span])
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
                    let mut declaration_tokens = item_tokens;
                    if declaration_tokens
                        .last()
                        .is_none_or(|token| token.value != Token::Semi)
                    {
                        declaration_tokens.push(synthetic(Token::Semi));
                    }
                    let declaration =
                        DeclKind::Declaration(self.parse_declaration_tokens(&declaration_tokens)?);
                    return Ok((
                        vec![span_pp_nodes(declaration, &nodes[..item_span])],
                        item_span,
                    ));
                }
                let (func, consumed) = self.parse_function(nodes)?;
                Ok((
                    vec![span_pp_nodes(DeclKind::Function(func), &nodes[..consumed])],
                    consumed,
                ))
            }
        }
    }

    fn parse_enum_fixed_type(
        &self,
        tokens: &[Span<Token>],
        colon: usize,
        header_end: usize,
    ) -> Result<Option<TypeName>, ParseError> {
        if tokens.value_at(colon) != Some(&Token::Colon) {
            return Ok(None);
        }
        let header = &tokens[..header_end];
        let mut parser = self.declarator_parser(header, colon + 1);
        let type_name = parser
            .parse_type_name()
            .map_err(|error| self.error_at_tokens(header, parser.pos, error.to_string()))?;
        if parser.pos != header_end {
            return Err(self.error_at_tokens(
                header,
                parser.pos,
                "expected `{` after enum underlying type",
            ));
        }
        Ok(Some(type_name))
    }

    pub(super) fn parse_tag_definition(
        &self,
        nodes: &[PPNode],
    ) -> Result<(Vec<DeclKind>, usize), ParseError> {
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
            Some(Token::Colon) if kind == TagKind::Enum => None,
            _ => return Err(self.error_at(Loc::whole(code), "expected tag name or `{`")),
        };
        let fixed_type_index = name_index + usize::from(name.is_some());
        let header_end = tokens[name_index..]
            .values()
            .position(|token| *token == Token::LBrace)
            .map_or(tokens.len(), |position| name_index + position);
        let fixed_type = self.parse_enum_fixed_type(tokens, fixed_type_index, header_end)?;

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
            let (trailing_attributes, declarators_position) = self
                .parse_attribute_groups(&trailing_tokens, 0)
                .map_err(|error| self.error_at(Loc::whole(code), error))?;
            attributes.extend(trailing_attributes);
            let body = if kind == TagKind::Enum {
                TagBody::Enum {
                    fixed_type,
                    enumerators: self.parse_enumerators(code, body_tokens)?,
                }
            } else {
                let mut fields = Vec::new();
                for mut segment in split_top_level(body_tokens, &Token::Semi) {
                    if segment.is_empty() {
                        continue;
                    }
                    segment.push(synthetic(Token::Semi));
                    let (specifiers, declarators) =
                        self.parse_field_declaration_tokens(&segment)?;
                    fields.push(span_tokens(
                        FieldItemKind::Field(FieldDecl {
                            specifiers,
                            declarators,
                        }),
                        &segment,
                    ));
                }
                TagBody::Record(fields)
            };
            let id = self.define_tag(span_tokens(
                TagDefinition {
                    id: TagId(0),
                    kind,
                    name,
                    attributes,
                    body,
                },
                &tokens[..=same_line_close],
            ));
            let declaration = self.parse_tag_declaration(
                &all_tokens[..node0_offset],
                TypeSpecifier::Tag(TagSpecifier::Definition(id)),
                &trailing_tokens,
                declarators_position,
            )?;
            return Ok((vec![declaration], consumed));
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
        let body_start = if self.node_tokens(&nodes[1]).value_at(0) == Some(&Token::LBrace) {
            2
        } else {
            1
        };
        let body = if kind == TagKind::Enum {
            TagBody::Enum {
                fixed_type,
                enumerators: self.parse_enum_items(&nodes[body_start..close])?,
            }
        } else {
            TagBody::Record(self.parse_field_items(&nodes[body_start..close])?)
        };
        let id = self.define_tag(span_pp_nodes(
            TagDefinition {
                id: TagId(0),
                kind,
                name,
                attributes,
                body,
            },
            &nodes[..=close],
        ));
        let declaration = self.parse_tag_declaration(
            &all_tokens[..node0_offset],
            TypeSpecifier::Tag(TagSpecifier::Definition(id)),
            &trailing_tokens,
            position,
        )?;
        Ok((vec![declaration], consumed))
    }

    fn parse_enumerators(
        &self,
        code: &str,
        body_tokens: &[Span<Token>],
    ) -> Result<Vec<EnumItem>, ParseError> {
        let mut items = Vec::new();
        for raw_segment in split_top_level(body_tokens, &Token::Comma) {
            let segment = raw_segment
                .iter()
                .filter(|token| !matches!(token.value, Token::Comment(_)))
                .cloned()
                .collect::<Vec<_>>();
            if segment.is_empty() {
                continue;
            }
            let Some(Token::Ident(name)) = segment.value_at(0) else {
                return Err(self.error_at(Loc::whole(code), "expected enumerator"));
            };
            let value = match segment.value_at(1) {
                None => None,
                Some(Token::Equal) => Some(self.parse_enumerator_value(&segment[2..])?),
                _ => {
                    return Err(self.error_at(Loc::whole(code), "expected enumerator value"));
                }
            };
            items.push(span_tokens(
                EnumItemKind::Enumerator(Enumerator {
                    name: name.clone(),
                    value,
                }),
                &raw_segment,
            ));
        }
        Ok(items)
    }

    fn parse_enum_items(&self, nodes: &[PPNode]) -> Result<Vec<EnumItem>, ParseError> {
        let mut items = Vec::new();
        let mut index = 0;
        while index < nodes.len() {
            match &nodes[index].value {
                PPNodeKind::Comment { provenance, .. } => {
                    let (group, consumed) = self.comment_group(&nodes[index..], *provenance);
                    items.push(group.map(EnumItemKind::Comment));
                    index += consumed;
                }
                PPNodeKind::Code { .. } => {
                    let start = index;
                    while index < nodes.len()
                        && matches!(nodes[index].value, PPNodeKind::Code { .. })
                    {
                        index += 1;
                    }
                    let code = self.node_text(&nodes[start]);
                    let chunk_tokens = self.nodes_tokens(&nodes[start..index]);
                    items.extend(self.parse_enumerators(code, &chunk_tokens)?);
                }
            }
        }
        Ok(items)
    }

    fn parse_enumerator_value(&self, tokens: &[Span<Token>]) -> Result<Expr, ParseError> {
        const_expr::Parser::parse_expression(tokens, &self.typedef_names, Some(self))
            .map_err(|error| self.error_at_tokens(tokens, 0, error.to_string()))
    }

    fn parse_tag_declaration(
        &self,
        prefix: &[Span<Token>],
        ty: TypeSpecifier,
        trailing: &[Span<Token>],
        position: usize,
    ) -> Result<DeclKind, ParseError> {
        let mut specifiers = specifiers_with_type(ty);
        self.parse_specifier_keywords(&mut self.declarator_parser(prefix, 0), &mut specifiers)?;
        let declarators = if trailing.value_at(position) == Some(&Token::Semi) {
            Vec::new()
        } else {
            let mut parser = self.declarator_parser(trailing, position);
            while let Some(qualifier) = parser.take_qualifier() {
                set_qualifier(&mut specifiers.qualifiers, qualifier);
            }
            self.parse_declarator_list(&mut parser, &mut specifiers, false)?
                .into_iter()
                .map(into_init_declarator)
                .collect()
        };
        Ok(DeclKind::Declaration(Declaration {
            specifiers,
            declarators,
        }))
    }

    pub(super) fn parse_linkage_spec_block(
        &mut self,
        nodes: &[PPNode],
    ) -> Result<(Vec<Decl>, usize), ParseError> {
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

    pub(super) fn parse_field_items(&self, nodes: &[PPNode]) -> Result<Vec<FieldItem>, ParseError> {
        let mut fields = Vec::new();
        let mut index = 0;
        while index < nodes.len() {
            match &nodes[index].value {
                PPNodeKind::Comment { provenance, .. } => {
                    let (group, consumed) = self.comment_group(&nodes[index..], *provenance);
                    fields.push(group.map(FieldItemKind::Comment));
                    index += consumed;
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
                    for mut declaration_tokens in split_top_level(&all_tokens, &Token::Semi) {
                        if declaration_tokens.is_empty() {
                            continue;
                        }
                        declaration_tokens.push(synthetic(Token::Semi));
                        let (specifiers, declarators) =
                            self.parse_field_declaration_tokens(&declaration_tokens)?;
                        fields.push(span_pp_nodes(
                            FieldItemKind::Field(FieldDecl {
                                specifiers,
                                declarators,
                            }),
                            &nodes[start..index],
                        ));
                    }
                }
            }
        }
        Ok(fields)
    }

    pub(super) fn record_typedefs(&mut self, decl: &DeclKind) {
        if let DeclKind::Declaration(declaration) = decl {
            self.record_declaration_typedefs(declaration);
        }
    }

    pub(super) fn record_declaration_typedefs(&mut self, declaration: &Declaration) {
        if declaration.specifiers.storage == StorageClass::Typedef {
            self.typedef_names
                .extend(declaration.names().map(str::to_string));
        }
    }
}

struct ParsedDeclarator {
    declarator: Declarator,
    bit_width: Option<Expr>,
    asm_label: Option<Span<AsmLabel>>,
    attributes: Vec<Attribute>,
    initializer: Option<Initializer>,
}

fn into_init_declarator(parsed: Span<ParsedDeclarator>) -> InitDeclarator {
    parsed.map(|parsed| InitDeclaratorKind {
        declarator: parsed.declarator,
        asm_label: parsed.asm_label,
        attributes: parsed.attributes,
        initializer: parsed.initializer,
    })
}

fn into_field_declarator(parsed: Span<ParsedDeclarator>) -> FieldDeclarator {
    parsed.map(|parsed| FieldDeclaratorKind {
        declarator: parsed.declarator,
        bit_width: parsed.bit_width,
        attributes: parsed.attributes,
    })
}

pub(super) fn specifiers_with_type(ty: TypeSpecifier) -> DeclarationSpecifiers {
    DeclarationSpecifiers {
        ty,
        qualifiers: Qualifiers::default(),
        storage: StorageClass::None,
        is_thread_local: false,
        is_inline: false,
        is_noreturn: false,
        is_constexpr: false,
        attributes: Vec::new(),
    }
}

pub(super) fn set_qualifier(qualifiers: &mut Qualifiers, qualifier: Keyword) {
    match qualifier {
        Keyword::Const => qualifiers.is_const = true,
        Keyword::Volatile => qualifiers.is_volatile = true,
        Keyword::Restrict => qualifiers.is_restrict = true,
        Keyword::Atomic => qualifiers.is_atomic = true,
        _ => unreachable!(),
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
                        kr_style = kr_style
                            || bare_identifier_names(paren_group.iter(), typedef_names).is_some();
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

pub(super) fn split_top_level_items(
    tokens: &[Span<Token>],
    typedef_names: &HashSet<String>,
) -> Vec<Vec<Span<Token>>> {
    let mut items = Vec::new();
    let mut start = 0;
    let mut depth = 0i32;
    let mut function_body = false;
    let mut has_assignment = false;
    let mut kr_style = false;
    let mut paren_open = None;
    for (index, token) in tokens.iter().enumerate() {
        match token.value {
            Token::LParen => {
                if depth == 0 {
                    paren_open = Some(index);
                }
                depth += 1;
            }
            Token::LBracket => depth += 1,
            Token::LBrace => {
                function_body = depth == 0
                    && index > start
                    && !has_assignment
                    && tokens.value_at(index - 1) == Some(&Token::RParen);
                depth += 1;
            }
            Token::RParen => {
                depth -= 1;
                if depth == 0
                    && let Some(open) = paren_open.take()
                {
                    let preceded_by_name =
                        open > 0 && matches!(tokens.value_at(open - 1), Some(Token::Ident(_)));
                    if !has_assignment && preceded_by_name {
                        kr_style = bare_identifier_names(
                            tokens[open + 1..index].iter().map(|t| &t.value),
                            typedef_names,
                        )
                        .is_some_and(|names| !names.is_empty());
                    }
                }
            }
            Token::RBracket | Token::RBrace => depth -= 1,
            Token::Equal if depth == 0 => has_assignment = true,
            _ => {}
        }
        let is_function_end =
            (function_body || kr_style) && depth == 0 && token.value == Token::RBrace;
        if depth == 0 && ((token.value == Token::Semi && !kr_style) || is_function_end) {
            items.push(tokens[start..=index].to_vec());
            start = index + 1;
            function_body = false;
            has_assignment = false;
            kr_style = false;
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
