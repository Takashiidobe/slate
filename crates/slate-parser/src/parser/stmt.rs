use super::decl::{
    join_node_text, matching_brace, matching_paren, signature_node_span, specifiers_with_type,
    split_top_level, top_level_semi,
};
use super::declarator::IdentifierList;
use super::{
    Cursor, Fragment, Loc, Parser, coalesce_string_literals, lex, span_tokens, synthetic,
    synthetic_span,
};
use crate::ast::*;
use crate::const_expr;
use crate::error::ParseError;
use crate::lexer::{Keyword, Token, TokenSpanExt};
use crate::pp::{PPNode, PPNodeKind};
use std::collections::HashMap;

impl Parser {
    pub(super) fn parse_function(
        &self,
        nodes: &[PPNode],
    ) -> Result<(FunctionDefinition, usize), ParseError> {
        let sig_node_count = signature_node_span(nodes, &self.typedef_names);
        let joined_code;
        let code: &str = if sig_node_count == 1 {
            self.node_text(&nodes[0])
        } else {
            joined_code = join_node_text(&nodes[..sig_node_count]);
            &joined_code
        };
        let sig_tokens = self.nodes_tokens(&nodes[..sig_node_count]);
        let mut parser = self.declarator_parser(&sig_tokens, 0);
        parser.identifier_list = IdentifierList::Accepted;
        let specifiers = self.parse_declaration_specifiers(&mut parser, true)?;
        let mut declarator = parser
            .parse_declarator(false)
            .map_err(|error| self.error_at_tokens(&sig_tokens, parser.pos, error.to_string()))?;
        if declarator.function_parameters().is_none() {
            return Err(self.error_at_tokens(
                &sig_tokens,
                parser.pos,
                "expected function declarator",
            ));
        }
        let (mut attributes, body_index) = if let IdentifierList::Parsed(names) =
            std::mem::replace(&mut parser.identifier_list, IdentifierList::Rejected)
        {
            let (parameters, body_index) =
                self.parse_kr_parameter_declarations(code, &sig_tokens, parser.pos, &names)?;
            if let Some(list) = declarator.function_parameters_mut() {
                *list = ParameterList::Prototype {
                    parameters,
                    variadic: false,
                };
            }
            (Vec::new(), body_index)
        } else {
            self.parse_attribute_groups(&sig_tokens, parser.pos)
                .map_err(|error| self.error_at(Loc::whole(code), error))?
        };
        if sig_tokens.value_at(body_index) != Some(&Token::LBrace) {
            return Err(self.error_at(
                Loc::at(code, code.len().saturating_sub(1), 1),
                "expected function body",
            ));
        }

        if let Some(same_line_close) = matching_brace(&sig_tokens, body_index) {
            let mut body_parser = self.clone();
            shadow_parameter_names(&mut body_parser, declarator.function_parameters());
            let body = body_parser
                .parse_stmts_from_tokens(code, &sig_tokens[body_index + 1..same_line_close])?;
            return Ok((
                FunctionDefinition {
                    specifiers,
                    declarator,
                    attributes,
                    body,
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
            let (trailing_attributes, _) = self
                .parse_attribute_groups(&closing_tokens, 1)
                .map_err(|error| self.error_at(Loc::whole(text), error))?;
            attributes.extend(trailing_attributes);
        }

        let mut body_parser = self.clone();
        shadow_parameter_names(&mut body_parser, declarator.function_parameters());
        let body = body_parser.parse_stmt_list(&nodes[sig_node_count..close_idx])?;
        Ok((
            FunctionDefinition {
                specifiers,
                declarator,
                attributes,
                body,
            },
            close_idx + 1,
        ))
    }

    pub(super) fn parse_kr_parameter_declarations(
        &self,
        code: &str,
        tokens: &[Span<Token>],
        mut pos: usize,
        names: &[String],
    ) -> Result<(Vec<ParameterDeclaration>, usize), ParseError> {
        let mut declared: HashMap<String, (DeclarationSpecifiers, Declarator, Span<()>)> =
            HashMap::new();
        while tokens.value_at(pos) != Some(&Token::LBrace) {
            let mut parser = self.declarator_parser(tokens, pos);
            let specifiers = parser
                .parse_specifiers(false)
                .map_err(|error| self.error_at(Loc::whole(code), error.to_string()))?;
            loop {
                let declarator_start = parser.pos;
                let declarator = parser
                    .parse_declarator(false)
                    .map_err(|error| self.error_at(Loc::whole(code), error.to_string()))?;
                let Some(name) = declarator.name() else {
                    return Err(self.error_at(
                        Loc::whole(code),
                        "expected parameter name in K&R parameter declaration",
                    ));
                };
                if !names.iter().any(|param_name| param_name == name) {
                    return Err(self.error_at(
                        Loc::whole(code),
                        format!("`{name}` is not a parameter of this function"),
                    ));
                }
                declared.insert(
                    name.to_string(),
                    (
                        specifiers.clone(),
                        declarator,
                        Span::cover((), &tokens[declarator_start..parser.pos]),
                    ),
                );
                if !parser.matches(Token::Comma) {
                    break;
                }
            }
            if !parser.matches(Token::Semi) {
                return Err(self.error_at(
                    Loc::whole(code),
                    "expected `;` in K&R parameter declaration",
                ));
            }
            pos = parser.pos;
        }
        let parameters = names
            .iter()
            .map(|name| match declared.remove(name) {
                Some((written, declarator, span)) => {
                    let promoted = if declarator.is_derived() {
                        None
                    } else {
                        default_argument_promotion(&written.ty)
                    };
                    let parameter = match promoted {
                        Some(ty) => ParameterDeclarationKind {
                            specifiers: DeclarationSpecifiers {
                                ty,
                                ..written.clone()
                            },
                            declarator,
                            declared_specifiers: Some(written),
                            attributes: Vec::new(),
                        },
                        None => ParameterDeclarationKind {
                            specifiers: written,
                            declarator,
                            declared_specifiers: None,
                            attributes: Vec::new(),
                        },
                    };
                    span.with_value(parameter)
                }
                None => {
                    let span = tokens
                        .iter()
                        .find(|token| matches!(&token.value, Token::Ident(found) if found == name))
                        .cloned()
                        .map(|token| token.with_value(()))
                        .unwrap_or_else(|| synthetic_span(()));
                    span.with_value(ParameterDeclarationKind {
                        specifiers: specifiers_with_type(TypeSpecifier::Integer(
                            IntegerType::Ranked {
                                rank: IntegerRank::Int,
                                signed: true,
                            },
                        )),
                        declarator: Declarator::Name(name.clone()),
                        declared_specifiers: None,
                        attributes: Vec::new(),
                    })
                }
            })
            .collect();
        Ok((parameters, pos))
    }

    pub(super) fn parse_stmt_list(&self, nodes: &[PPNode]) -> Result<Vec<Stmt>, ParseError> {
        let mut stmts = Vec::new();
        let mut pending_comments = Vec::new();
        let mut run_text = String::new();
        let mut run_tokens = Vec::new();
        let mut index = 0;
        while index < nodes.len() {
            let node = &nodes[index];
            match &node.value {
                PPNodeKind::Comment { provenance, .. } => {
                    if !run_tokens.is_empty()
                        && let Ok(parsed) = self.parse_stmts_from_tokens(&run_text, &run_tokens)
                    {
                        stmts.extend(parsed);
                        stmts.append(&mut pending_comments);
                        run_text.clear();
                        run_tokens.clear();
                    }
                    let (group, consumed) = self.comment_group(&nodes[index..], *provenance);
                    let comment = group.map(StmtKind::Comment);
                    if run_tokens.is_empty() {
                        stmts.push(comment);
                    } else {
                        pending_comments.push(comment);
                    }
                    index += consumed;
                }
                PPNodeKind::Pragma { tokens, .. } => {
                    stmts.push(
                        node.clone()
                            .with_value(StmtKind::Pragma(self.parse_pragma_tokens(tokens)?)),
                    );
                    index += 1;
                }
                PPNodeKind::Code { text, .. } if !lex(text).is_empty() => {
                    if !run_text.is_empty() {
                        run_text.push(' ');
                    }
                    run_text.push_str(text);
                    run_tokens.extend(self.node_tokens(node));
                    index += 1;
                }
                PPNodeKind::Code { .. } => index += 1,
            }
        }
        if !run_tokens.is_empty() {
            stmts.extend(self.parse_stmts_from_tokens(&run_text, &run_tokens)?);
        }
        stmts.append(&mut pending_comments);
        Ok(stmts)
    }

    pub(super) fn parse_stmts_from_tokens(
        &self,
        code: &str,
        tokens: &[Span<Token>],
    ) -> Result<Vec<Stmt>, ParseError> {
        let mut parser = self.clone();
        let mut position = 0;
        let mut stmts = Vec::new();
        while position < tokens.len() {
            let start = position;
            let mut fragment = Fragment::new(&parser, code, tokens, position);
            let stmt = parser.parse_one_stmt(&mut fragment)?;
            position = fragment.pos;
            if let StmtKind::Decl(declaration) = &stmt {
                parser.record_declaration_typedefs(declaration);
            }
            stmts.push(span_tokens(stmt, &tokens[start..position]));
        }
        Ok(stmts)
    }

    pub(crate) fn parse_statement_expression_body(
        &self,
        tokens: &[Span<Token>],
    ) -> Result<Vec<Stmt>, ParseError> {
        self.parse_stmts_from_tokens("", tokens)
    }

    pub(super) fn parse_body(&self, fragment: &mut Fragment) -> Result<Vec<Stmt>, ParseError> {
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
            let start = fragment.pos;
            let stmt = self.parse_one_stmt(fragment)?;
            Ok(vec![span_tokens(
                stmt,
                &fragment.tokens[start..fragment.pos],
            )])
        }
    }

    fn parse_labeled_body(&self, fragment: &mut Fragment) -> Result<Box<Stmt>, ParseError> {
        if matches!(fragment.peek(), None | Some(Token::RBrace)) {
            return Ok(Box::new(synthetic_span(StmtKind::Block(Vec::new()))));
        }
        let start = fragment.pos;
        let stmt = self.parse_one_stmt(fragment)?;
        Ok(Box::new(span_tokens(
            stmt,
            &fragment.tokens[start..fragment.pos],
        )))
    }

    pub(super) fn parse_simple_keyword_stmt(
        &self,
        fragment: &mut Fragment,
        stmt: StmtKind,
    ) -> Result<StmtKind, ParseError> {
        fragment.pos += 1;
        fragment.expect(Token::Semi, "expected `;`")?;
        Ok(stmt)
    }

    pub(super) fn try_parse_nested_function(
        &self,
        code: &str,
        tokens: &[Span<Token>],
        start: usize,
    ) -> Result<Option<(FunctionDefinition, usize)>, ParseError> {
        let fragment = Fragment::new(self, code, tokens, start);
        let mut parser = self.declarator_parser(tokens, start);
        let Ok(specifiers) = self.parse_declaration_specifiers(&mut parser, false) else {
            return Ok(None);
        };
        if !matches!(
            parser.peek(),
            Some(Token::Ident(_) | Token::LParen | Token::Star)
        ) {
            return Ok(None);
        }
        let Ok(declarator) = parser.parse_declarator(false) else {
            return Ok(None);
        };
        if declarator.function_parameters().is_none() {
            return Ok(None);
        }
        let (attributes, body_index) = self
            .parse_attribute_groups(tokens, parser.pos)
            .map_err(|error| fragment.error(error))?;
        if tokens.value_at(body_index) != Some(&Token::LBrace) {
            return Ok(None);
        }
        let close =
            matching_brace(tokens, body_index).ok_or_else(|| fragment.error("expected `}`"))?;
        let body = self.parse_stmts_from_tokens(code, &tokens[body_index + 1..close])?;
        Ok(Some((
            FunctionDefinition {
                specifiers,
                declarator,
                attributes,
                body,
            },
            close + 1,
        )))
    }

    pub(super) fn starts_declaration(&self, tokens: &[Span<Token>], pos: usize) -> bool {
        let pos = if tokens.value_at(pos) == Some(&Token::Ident("__extension__".to_string())) {
            pos + 1
        } else {
            pos
        };
        match tokens.value_at(pos) {
            Some(Token::LBracket) if tokens.value_at(pos + 1) == Some(&Token::LBracket) => true,
            Some(Token::Keyword(keyword)) if keyword.is_storage_class_or_specifier() => true,
            Some(Token::Ident(name))
                if matches!(
                    name.as_str(),
                    "_Alignas"
                        | "alignas"
                        | "__auto_type"
                        | "__attribute__"
                        | "__attribute"
                        | "__declspec"
                ) =>
            {
                true
            }
            Some(token) => const_expr::starts_type_name(token, &self.typedef_names),
            None => false,
        }
    }

    pub(super) fn parse_one_stmt(&self, fragment: &mut Fragment) -> Result<StmtKind, ParseError> {
        let code = fragment.code;
        let tokens = fragment.tokens;

        if tokens.value_at(fragment.pos) == Some(&Token::Semi) {
            fragment.pos += 1;
            return Ok(StmtKind::Block(Vec::new()));
        }

        if tokens.value_at(fragment.pos) == Some(&Token::Keyword(Keyword::StaticAssert)) {
            let end = top_level_semi(&tokens[fragment.pos..])
                .map(|position| fragment.pos + position)
                .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
            let assertion = self.parse_static_assert(code, &tokens[fragment.pos..=end])?;
            fragment.pos = end + 1;
            return Ok(StmtKind::StaticAssert(assertion));
        }

        if let Some(Token::Ident(name)) = tokens.value_at(fragment.pos)
            && tokens.value_at(fragment.pos + 1) == Some(&Token::Colon)
        {
            let label = span_tokens(name.clone(), &tokens[fragment.pos..fragment.pos + 1]);
            fragment.pos += 2;
            let body = self.parse_labeled_body(fragment)?;
            return Ok(StmtKind::Labeled { label, body });
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
                        Token::Ident(name) => {
                            Ok(span_tokens(name.clone(), std::slice::from_ref(single)))
                        }
                        _ => Err(self.error_at(Loc::whole(code), "expected label name")),
                    },
                    _ => Err(self.error_at(Loc::whole(code), "expected label name")),
                })
                .collect::<Result<Vec<_>, _>>()?;
            fragment.pos = end + 1;
            return Ok(StmtKind::LocalLabelDecl(names));
        }

        if (tokens.value_at(fragment.pos) == Some(&Token::LBracket)
            && tokens.value_at(fragment.pos + 1) == Some(&Token::LBracket))
            || matches!(
                tokens.value_at(fragment.pos),
                Some(Token::Ident(name)) if matches!(name.as_str(), "__attribute__" | "__attribute")
            )
        {
            let (attributes, position) = self
                .parse_attribute_groups(tokens, fragment.pos)
                .map_err(|error| fragment.error(error))?;
            if tokens.value_at(position) == Some(&Token::Semi) {
                fragment.pos = position + 1;
                return Ok(StmtKind::Attribute(attributes));
            }
        }

        if let Some(asm) = self.parse_asm_stmt(fragment)? {
            return Ok(StmtKind::Asm(asm));
        }

        if tokens.value_at(fragment.pos) == Some(&Token::LBrace) {
            let close = matching_brace(tokens, fragment.pos)
                .ok_or_else(|| self.error_at(Loc::whole(code), "expected `}`"))?;
            let body = self.parse_stmts_from_tokens(code, &tokens[fragment.pos + 1..close])?;
            fragment.pos = close + 1;
            return Ok(StmtKind::Block(body));
        }

        if self.starts_declaration(tokens, fragment.pos)
            && let Some((function, next)) =
                self.try_parse_nested_function(code, tokens, fragment.pos)?
        {
            fragment.pos = next;
            return Ok(StmtKind::NestedFunction(Box::new(function)));
        }

        if self.starts_declaration(tokens, fragment.pos) {
            let end = top_level_semi(&tokens[fragment.pos..])
                .map(|position| fragment.pos + position)
                .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
            let declaration = self.parse_declaration_tokens(&tokens[fragment.pos..=end])?;
            fragment.pos = end + 1;
            return Ok(StmtKind::Decl(declaration));
        }

        match tokens.value_at(fragment.pos) {
            Some(Token::Keyword(Keyword::Return)) => {
                let start = fragment.pos + 1;
                let end = top_level_semi(&tokens[start..])
                    .map(|position| start + position)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
                fragment.pos = end + 1;
                if start == end {
                    Ok(StmtKind::ReturnVoid)
                } else {
                    Ok(StmtKind::Return(
                        self.parse_expression(code, &tokens[start..end])?,
                    ))
                }
            }
            Some(Token::Keyword(Keyword::Break)) => {
                self.parse_simple_keyword_stmt(fragment, StmtKind::Break)
            }
            Some(Token::Keyword(Keyword::Continue)) => {
                self.parse_simple_keyword_stmt(fragment, StmtKind::Continue)
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
                Ok(StmtKind::ComputedGoto(target))
            }
            Some(Token::Keyword(Keyword::Goto)) => {
                fragment.pos += 1;
                let label_start = fragment.pos;
                let label = fragment.expect_ident("expected label after `goto`")?;
                let label = span_tokens(label, &tokens[label_start..fragment.pos]);
                fragment.expect(Token::Semi, "expected `;` after `goto` label")?;
                Ok(StmtKind::Goto(label))
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
                let label = if let Some(range) = range {
                    let range_start = self.parse_expression(code, &tokens[start..start + range])?;
                    let range_end =
                        self.parse_expression(code, &tokens[start + range + 1..colon])?;
                    fragment.pos = colon + 1;
                    SwitchLabel::CaseRange {
                        start: range_start,
                        end: range_end,
                    }
                } else {
                    let value = self.parse_expression(code, &tokens[start..colon])?;
                    fragment.pos = colon + 1;
                    SwitchLabel::Case(value)
                };
                let body = self.parse_labeled_body(fragment)?;
                Ok(StmtKind::SwitchLabel { label, body })
            }
            Some(Token::Keyword(Keyword::Default)) => {
                fragment.pos += 1;
                fragment.expect(Token::Colon, "expected `:` after `default`")?;
                let body = self.parse_labeled_body(fragment)?;
                Ok(StmtKind::SwitchLabel {
                    label: SwitchLabel::Default,
                    body,
                })
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
                Ok(StmtKind::If {
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
                Ok(StmtKind::While { condition, body })
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
                Ok(StmtKind::DoWhile { body, condition })
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
                        StmtKind::Decl(self.parse_declaration_tokens(&decl_tokens)?),
                        init_tokens,
                    )))
                } else {
                    Some(Box::new(span_tokens(
                        StmtKind::Expr(self.parse_expression(code, init_tokens)?),
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
                Ok(StmtKind::For {
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
                Ok(StmtKind::Switch { discriminant, body })
            }
            _ => {
                let end = top_level_semi(&tokens[fragment.pos..])
                    .map(|position| fragment.pos + position)
                    .ok_or_else(|| self.error_at(Loc::whole(code), "expected `;`"))?;
                let expression = self.parse_expression(code, &tokens[fragment.pos..end])?;
                fragment.pos = end + 1;
                Ok(StmtKind::Expr(expression))
            }
        }
    }

    pub(super) fn parse_expression(
        &self,
        code: &str,
        tokens: &[Span<Token>],
    ) -> Result<Expr, ParseError> {
        if tokens.is_empty() {
            return Err(self.error_at(Loc::whole(code), "expected expression"));
        }
        let tokens = coalesce_string_literals(tokens);
        const_expr::Parser::parse_expression(&tokens, &self.typedef_names, Some(self))
            .map_err(|error| self.error_at(Loc::whole(code), error.to_string()))
    }
}

fn shadow_parameter_names(parser: &mut Parser, parameters: Option<&ParameterList>) {
    if let Some(parameters) = parameters {
        for parameter in parameters.parameters() {
            if let Some(name) = parameter.declarator.name() {
                parser.typedef_names.remove(name);
            }
        }
    }
}

fn default_argument_promotion(ty: &TypeSpecifier) -> Option<TypeSpecifier> {
    match ty {
        TypeSpecifier::Bool
        | TypeSpecifier::Integer(
            IntegerType::Char { .. }
            | IntegerType::Ranked {
                rank: IntegerRank::Short,
                ..
            },
        ) => Some(TypeSpecifier::Integer(IntegerType::Ranked {
            rank: IntegerRank::Int,
            signed: true,
        })),
        TypeSpecifier::Floating(FloatingType::Float) => {
            Some(TypeSpecifier::Floating(FloatingType::Double))
        }
        _ => None,
    }
}
