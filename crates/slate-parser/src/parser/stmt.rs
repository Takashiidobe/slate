use super::decl::{
    matching_brace, matching_paren, specifiers_with_type, split_top_level, top_level_semi,
};
use super::{Annotation, Cursor, Parser, TokenCursor, span_tokens, synthetic_span};
use crate::ast::*;
use crate::const_expr;
use crate::error::ParseError;
use crate::lexer::{Keyword, Token, TokenSpanExt};
use std::collections::HashMap;

impl Parser {
    pub(super) fn parse_function_body(
        &self,
        tokens: &[Span<Token>],
        position: &mut usize,
        declarator: &Declarator,
    ) -> Result<Vec<Stmt>, ParseError> {
        let close = matching_brace(tokens, *position)
            .ok_or_else(|| self.error_at_tokens(tokens, *position, "expected `}`"))?;
        let _scope = self.enter_scope();
        shadow_parameter_names(self, declarator.function_parameters());
        let mut body = self.parse_stmts_from_tokens(&tokens[*position + 1..close])?;
        body.extend(self.statement_annotations(tokens, close, close)?);
        *position = close + 1;
        Ok(body)
    }

    fn statement_annotations(
        &self,
        tokens: &[Span<Token>],
        start: usize,
        end: usize,
    ) -> Result<Vec<Stmt>, ParseError> {
        self.input
            .take(tokens, start, end)
            .into_iter()
            .map(|annotation| {
                let kind = match &annotation.value {
                    Annotation::Comment(group) => StmtKind::Comment(group.clone()),
                    Annotation::Pragma(tokens) => {
                        StmtKind::Pragma(self.parse_pragma_tokens(tokens)?)
                    }
                };
                Ok(annotation.with_value(kind))
            })
            .collect()
    }

    pub(super) fn parse_kr_parameter_declarations(
        &self,
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
                .map_err(|error| self.error_at_tokens(tokens, parser.pos, error.to_string()))?;
            loop {
                let declarator_start = parser.pos;
                let declarator = parser
                    .parse_declarator(false)
                    .map_err(|error| self.error_at_tokens(tokens, parser.pos, error.to_string()))?;
                let Some(name) = declarator.name() else {
                    return Err(self.error_at_tokens(
                        tokens,
                        parser.pos,
                        "expected parameter name in K&R parameter declaration",
                    ));
                };
                if !names.iter().any(|param_name| param_name == name) {
                    return Err(self.error_at_tokens(
                        tokens,
                        parser.pos,
                        format!("`{name}` is not a parameter of this function"),
                    ));
                }
                declared.insert(
                    name.to_string(),
                    (
                        specifiers.clone(),
                        declarator,
                        self.cover_tokens((), tokens, declarator_start, parser.pos),
                    ),
                );
                if !parser.matches(Token::Comma) {
                    break;
                }
            }
            if !parser.matches(Token::Semi) {
                return Err(self.error_at_tokens(
                    tokens,
                    parser.pos,
                    "expected `;` in K&R parameter declaration",
                ));
            }
            pos = parser.pos;
        }
        let parameters = names
            .iter()
            .map(|name| match declared.remove(name) {
                Some((specifiers, declarator, span)) => span.with_value(ParameterDeclarationKind {
                    specifiers,
                    declarator,
                    attributes: Vec::new(),
                    comments: Vec::new(),
                }),
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
                        attributes: Vec::new(),
                        comments: Vec::new(),
                    })
                }
            })
            .collect();
        Ok((parameters, pos))
    }

    pub(super) fn parse_stmts_from_tokens(
        &self,
        tokens: &[Span<Token>],
    ) -> Result<Vec<Stmt>, ParseError> {
        let _scope = self.enter_scope();
        let mut position = 0;
        let mut stmts = Vec::new();
        while position < tokens.len() {
            stmts.extend(self.statement_annotations(tokens, position, position)?);
            let start = position;
            let mut cursor = TokenCursor::new(self, tokens, position);
            let stmt = self.parse_one_stmt(&mut cursor)?;
            position = cursor.pos;
            let (pragmas, comments): (Vec<_>, Vec<_>) = self
                .statement_annotations(tokens, start, position - 1)?
                .into_iter()
                .map(|stmt| {
                    stmt.map(|kind| match kind {
                        StmtKind::Comment(group) => StmtKind::Comment(group.into_trailing()),
                        kind => kind,
                    })
                })
                .partition(|stmt| matches!(stmt.value, StmtKind::Pragma(_)));
            stmts.extend(pragmas);
            stmts.push(span_tokens(stmt, &tokens[start..position], self.context()));
            stmts.extend(comments);
        }
        stmts.extend(self.statement_annotations(tokens, tokens.len(), tokens.len())?);
        Ok(stmts)
    }

    pub(crate) fn parse_statement_expression_body(
        &self,
        tokens: &[Span<Token>],
    ) -> Result<Vec<Stmt>, ParseError> {
        self.parse_stmts_from_tokens(tokens)
    }

    pub(super) fn parse_body(&self, cursor: &mut TokenCursor) -> Result<Box<Stmt>, ParseError> {
        let _scope = (self.features().control_statement_scopes
            && cursor.peek() != Some(&Token::LBrace))
        .then(|| self.enter_scope());
        let start = cursor.pos;
        let stmt = self.parse_one_stmt(cursor)?;
        Ok(Box::new(span_tokens(
            stmt,
            &cursor.tokens[start..cursor.pos],
            self.context(),
        )))
    }

    fn parse_labeled_body(&self, cursor: &mut TokenCursor) -> Result<Box<Stmt>, ParseError> {
        if matches!(cursor.peek(), None | Some(Token::RBrace)) {
            return Ok(Box::new(synthetic_span(StmtKind::Null)));
        }
        let start = cursor.pos;
        let stmt = self.parse_one_stmt(cursor)?;
        Ok(Box::new(span_tokens(
            stmt,
            &cursor.tokens[start..cursor.pos],
            self.context(),
        )))
    }

    fn parse_loop_jump(
        &self,
        cursor: &mut TokenCursor,
        is_continue: bool,
    ) -> Result<StmtKind, ParseError> {
        cursor.pos += 1;
        let label = if let Some(Token::Ident(name)) = cursor.peek() {
            if !self.features().named_loops {
                return Err(cursor.error("named loop jumps require C2y"));
            }
            let label = span_tokens(
                name.to_string(),
                &cursor.tokens[cursor.pos..cursor.pos + 1],
                self.context(),
            );
            cursor.pos += 1;
            Some(label)
        } else {
            None
        };
        cursor.expect(Token::Semi, "expected `;`")?;
        Ok(match (is_continue, label) {
            (false, None) => StmtKind::Break,
            (true, None) => StmtKind::Continue,
            (false, Some(label)) => StmtKind::NamedBreak(label),
            (true, Some(label)) => StmtKind::NamedContinue(label),
        })
    }

    fn parse_selection_declaration(
        &self,
        tokens: &[Span<Token>],
    ) -> Result<(Box<Span<Declaration>>, Option<Expr>), ParseError> {
        if !self.features().selection_declarations {
            return Err(self.error_at_tokens(
                tokens,
                0,
                "selection statement declarations require C2y",
            ));
        }
        let semi = top_level_semi(tokens);
        let end = semi.unwrap_or(tokens.len());
        let mut declaration_tokens = tokens[..end].to_vec();
        let terminator = tokens
            .get(end)
            .or_else(|| tokens.last())
            .ok_or_else(|| self.error_at_tokens(tokens, 0, "expected declaration"))?;
        declaration_tokens.push(terminator.clone().with_value(Token::Semi));
        let declaration = self.parse_declaration_tokens(&declaration_tokens)?;
        let declaration = Box::new(span_tokens(declaration, &tokens[..end], self.context()));
        let condition = if let Some(semi) = semi {
            if semi + 1 == tokens.len() {
                return Err(self.error_at_tokens(
                    tokens,
                    semi,
                    "expected expression after selection declaration",
                ));
            }
            Some(self.parse_expression(&tokens[semi + 1..])?)
        } else {
            None
        };
        Ok((declaration, condition))
    }

    pub(super) fn starts_declaration(&self, tokens: &[Span<Token>], pos: usize) -> bool {
        let pos = if tokens.value_at(pos) == Some(&Token::Ident("__extension__".into())) {
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
            Some(token) => const_expr::starts_type_name(token, self.context()),
            None => false,
        }
    }

    pub(super) fn parse_one_stmt(&self, cursor: &mut TokenCursor) -> Result<StmtKind, ParseError> {
        let depth = self.nesting().get();
        if depth >= super::NESTING_LIMIT {
            return Err(cursor.error("nesting level exceeded maximum"));
        }
        self.nesting().set(depth + 1);
        let statement = self.one_stmt(cursor);
        self.nesting().set(depth);
        statement
    }

    fn one_stmt(&self, cursor: &mut TokenCursor) -> Result<StmtKind, ParseError> {
        let tokens = cursor.tokens;

        if tokens.value_at(cursor.pos) == Some(&Token::Semi) {
            cursor.pos += 1;
            return Ok(StmtKind::Null);
        }

        if tokens.value_at(cursor.pos) == Some(&Token::Keyword(Keyword::StaticAssert)) {
            let end = top_level_semi(&tokens[cursor.pos..])
                .map(|position| cursor.pos + position)
                .ok_or_else(|| self.error_at_tokens(tokens, cursor.pos, "expected `;`"))?;
            let assertion = self.parse_static_assert(&tokens[cursor.pos..=end])?;
            cursor.pos = end + 1;
            return Ok(StmtKind::StaticAssert(assertion));
        }

        if let Some(Token::Ident(name)) = tokens.value_at(cursor.pos)
            && tokens.value_at(cursor.pos + 1) == Some(&Token::Colon)
        {
            let label = span_tokens(
                name.to_string(),
                &tokens[cursor.pos..cursor.pos + 1],
                self.context(),
            );
            cursor.pos += 2;
            let body = self.parse_labeled_body(cursor)?;
            return Ok(StmtKind::Labeled { label, body });
        }

        if tokens.value_at(cursor.pos) == Some(&Token::Ident("__label__".into())) {
            let end = top_level_semi(&tokens[cursor.pos..])
                .map(|position| cursor.pos + position)
                .ok_or_else(|| self.error_at_tokens(tokens, cursor.pos, "expected `;`"))?;
            let names = split_top_level(&tokens[cursor.pos + 1..end], &Token::Comma)
                .into_iter()
                .filter(|part| !part.is_empty())
                .map(|part| match part {
                    [single] => match &single.value {
                        Token::Ident(name) => Ok(span_tokens(
                            name.to_string(),
                            std::slice::from_ref(single),
                            self.context(),
                        )),
                        _ => Err(self.error_at_tokens(tokens, cursor.pos, "expected label name")),
                    },
                    _ => Err(self.error_at_tokens(tokens, cursor.pos, "expected label name")),
                })
                .collect::<Result<Vec<_>, _>>()?;
            cursor.pos = end + 1;
            return Ok(StmtKind::LocalLabelDecl(names));
        }

        if (tokens.value_at(cursor.pos) == Some(&Token::LBracket)
            && tokens.value_at(cursor.pos + 1) == Some(&Token::LBracket))
            || matches!(
                tokens.value_at(cursor.pos),
                Some(Token::Ident(name)) if matches!(name.as_str(), "__attribute__" | "__attribute")
            )
        {
            let checkpoint = self.checkpoint();
            let (attributes, position) = self
                .parse_attribute_groups(tokens, cursor.pos)
                .map_err(|error| cursor.error(error))?;
            if tokens.value_at(position) == Some(&Token::Semi) {
                cursor.pos = position + 1;
                checkpoint.commit();
                return Ok(StmtKind::Attribute(attributes));
            }
            if !self.starts_declaration(tokens, position)
                || (matches!(tokens.value_at(position), Some(Token::Ident(_)))
                    && tokens.value_at(position + 1) == Some(&Token::Colon))
            {
                cursor.pos = position;
                let statement = self.parse_one_stmt(cursor)?;
                let body = Box::new(span_tokens(
                    statement,
                    &tokens[position..cursor.pos],
                    self.context(),
                ));
                checkpoint.commit();
                return Ok(StmtKind::Attributed { attributes, body });
            }
        }

        if let Some(asm) = self.parse_ms_asm_stmt(cursor)? {
            return Ok(StmtKind::MsAsm(asm));
        }

        if let Some(asm) = self.parse_asm_stmt(cursor)? {
            return Ok(StmtKind::Asm(asm));
        }

        if tokens.value_at(cursor.pos) == Some(&Token::LBrace) {
            let close = matching_brace(tokens, cursor.pos)
                .ok_or_else(|| self.error_at_tokens(tokens, cursor.pos, "expected `}`"))?;
            let mut body = self.parse_stmts_from_tokens(&tokens[cursor.pos + 1..close])?;
            body.extend(self.statement_annotations(tokens, close, close)?);
            cursor.pos = close + 1;
            return Ok(StmtKind::Block(body));
        }

        if self.starts_declaration(tokens, cursor.pos) {
            return match self.parse_external_item(tokens, &mut cursor.pos)? {
                DeclKind::Declaration(declaration) => Ok(StmtKind::Decl(declaration)),
                DeclKind::Function(function) => Ok(StmtKind::NestedFunction(Box::new(function))),
                _ => Err(cursor.error("expected declaration")),
            };
        }

        match tokens.value_at(cursor.pos) {
            Some(Token::Keyword(Keyword::Return)) => {
                let start = cursor.pos + 1;
                let end = top_level_semi(&tokens[start..])
                    .map(|position| start + position)
                    .ok_or_else(|| self.error_at_tokens(tokens, cursor.pos, "expected `;`"))?;
                cursor.pos = end + 1;
                if start == end {
                    Ok(StmtKind::ReturnVoid)
                } else {
                    Ok(StmtKind::Return(
                        self.parse_expression(&tokens[start..end])?,
                    ))
                }
            }
            Some(Token::Keyword(Keyword::Break)) => self.parse_loop_jump(cursor, false),
            Some(Token::Keyword(Keyword::Continue)) => self.parse_loop_jump(cursor, true),
            Some(Token::Keyword(Keyword::Goto))
                if tokens.value_at(cursor.pos + 1) == Some(&Token::Star) =>
            {
                let start = cursor.pos + 2;
                let end = top_level_semi(&tokens[start..])
                    .map(|position| start + position)
                    .ok_or_else(|| self.error_at_tokens(tokens, cursor.pos, "expected `;`"))?;
                let target = self.parse_expression(&tokens[start..end])?;
                cursor.pos = end + 1;
                Ok(StmtKind::ComputedGoto(target))
            }
            Some(Token::Keyword(Keyword::Goto)) => {
                cursor.pos += 1;
                let label_start = cursor.pos;
                let label = cursor.expect_ident("expected label after `goto`")?;
                let label = span_tokens(label, &tokens[label_start..cursor.pos], self.context());
                cursor.expect(Token::Semi, "expected `;` after `goto` label")?;
                Ok(StmtKind::Goto(label))
            }
            Some(Token::Keyword(Keyword::Case)) => {
                let start = cursor.pos + 1;
                let (value, end) = const_expr::Parser::parse_one(tokens, start, self.context())
                    .map_err(|error| self.error_at_tokens(tokens, start, error.to_string()))?;
                cursor.pos = end;
                let label = if cursor.peek() == Some(&Token::Ellipsis) {
                    let start = cursor.pos + 1;
                    let (end_value, end) =
                        const_expr::Parser::parse_one(tokens, start, self.context()).map_err(
                            |error| self.error_at_tokens(tokens, start, error.to_string()),
                        )?;
                    cursor.pos = end;
                    SwitchLabel::CaseRange {
                        start: value,
                        end: end_value,
                    }
                } else {
                    SwitchLabel::Case(value)
                };
                cursor.expect(Token::Colon, "expected `:` after `case`")?;
                let body = self.parse_labeled_body(cursor)?;
                Ok(StmtKind::SwitchLabel { label, body })
            }
            Some(Token::Keyword(Keyword::Default)) => {
                cursor.pos += 1;
                cursor.expect(Token::Colon, "expected `:` after `default`")?;
                let body = self.parse_labeled_body(cursor)?;
                Ok(StmtKind::SwitchLabel {
                    label: SwitchLabel::Default,
                    body,
                })
            }
            Some(Token::Keyword(Keyword::If)) => {
                let _scope = self
                    .features()
                    .control_statement_scopes
                    .then(|| self.enter_scope());
                let open = cursor.pos + 1;
                if tokens.value_at(open) != Some(&Token::LParen) {
                    return Err(self.error_at_tokens(
                        tokens,
                        cursor.pos,
                        "expected `(` after `if`",
                    ));
                }
                let close = matching_paren(tokens, open)
                    .ok_or_else(|| self.error_at_tokens(tokens, cursor.pos, "expected `)`"))?;
                let clause = &tokens[open + 1..close];
                let (declaration, condition) = if self.starts_declaration(clause, 0) {
                    let (declaration, condition) = self.parse_selection_declaration(clause)?;
                    (Some(declaration), condition)
                } else {
                    (None, Some(self.parse_expression(clause)?))
                };
                cursor.pos = close + 1;
                let then_branch = self.parse_body(cursor)?;
                let else_branch = if cursor.peek() == Some(&Token::Keyword(Keyword::Else)) {
                    cursor.pos += 1;
                    Some(self.parse_body(cursor)?)
                } else {
                    None
                };
                if let Some(declaration) = declaration {
                    Ok(StmtKind::IfDeclaration {
                        declaration,
                        condition,
                        then_branch,
                        else_branch,
                    })
                } else {
                    Ok(StmtKind::If {
                        condition: condition.ok_or_else(|| {
                            self.error_at_tokens(tokens, open + 1, "expected expression")
                        })?,
                        then_branch,
                        else_branch,
                    })
                }
            }
            Some(Token::Keyword(Keyword::While)) => {
                let _scope = self
                    .features()
                    .control_statement_scopes
                    .then(|| self.enter_scope());
                let open = cursor.pos + 1;
                if tokens.value_at(open) != Some(&Token::LParen) {
                    return Err(self.error_at_tokens(
                        tokens,
                        cursor.pos,
                        "expected `(` after `while`",
                    ));
                }
                let close = matching_paren(tokens, open)
                    .ok_or_else(|| self.error_at_tokens(tokens, cursor.pos, "expected `)`"))?;
                let condition = self.parse_expression(&tokens[open + 1..close])?;
                cursor.pos = close + 1;
                let body = self.parse_body(cursor)?;
                Ok(StmtKind::While { condition, body })
            }
            Some(Token::Keyword(Keyword::Do)) => {
                let _scope = self
                    .features()
                    .control_statement_scopes
                    .then(|| self.enter_scope());
                cursor.pos += 1;
                let body = self.parse_body(cursor)?;
                if tokens.value_at(cursor.pos) != Some(&Token::Keyword(Keyword::While)) {
                    return Err(self.error_at_tokens(
                        tokens,
                        cursor.pos,
                        "expected `while` after `do` body",
                    ));
                }
                let open = cursor.pos + 1;
                if tokens.value_at(open) != Some(&Token::LParen) {
                    return Err(self.error_at_tokens(
                        tokens,
                        cursor.pos,
                        "expected `(` after `while`",
                    ));
                }
                let close = matching_paren(tokens, open)
                    .ok_or_else(|| self.error_at_tokens(tokens, cursor.pos, "expected `)`"))?;
                let condition = self.parse_expression(&tokens[open + 1..close])?;
                if tokens.value_at(close + 1) != Some(&Token::Semi) {
                    return Err(self.error_at_tokens(
                        tokens,
                        cursor.pos,
                        "expected `;` after `do`-`while`",
                    ));
                }
                cursor.pos = close + 2;
                Ok(StmtKind::DoWhile { body, condition })
            }
            Some(Token::Keyword(Keyword::For)) => {
                let _scope = self
                    .features()
                    .control_statement_scopes
                    .then(|| self.enter_scope());
                let open = cursor.pos + 1;
                if tokens.value_at(open) != Some(&Token::LParen) {
                    return Err(self.error_at_tokens(
                        tokens,
                        cursor.pos,
                        "expected `(` after `for`",
                    ));
                }
                let close = matching_paren(tokens, open)
                    .ok_or_else(|| self.error_at_tokens(tokens, cursor.pos, "expected `)`"))?;
                let clause = &tokens[open + 1..close];
                let first_semi = top_level_semi(clause).ok_or_else(|| {
                    self.error_at_tokens(tokens, cursor.pos, "expected `;` in `for`")
                })?;
                let init_tokens = &clause[..first_semi];
                let rest = &clause[first_semi + 1..];
                let second_semi = top_level_semi(rest).ok_or_else(|| {
                    self.error_at_tokens(tokens, cursor.pos, "expected `;` in `for`")
                })?;
                let condition_tokens = &rest[..second_semi];
                let increment_tokens = &rest[second_semi + 1..];
                let init = if init_tokens.is_empty() {
                    None
                } else if self.starts_declaration(init_tokens, 0) {
                    let declaration = self.parse_declaration_tokens(&clause[..=first_semi])?;
                    Some(Box::new(span_tokens(
                        StmtKind::Decl(declaration),
                        init_tokens,
                        self.context(),
                    )))
                } else {
                    Some(Box::new(span_tokens(
                        StmtKind::Expr(self.parse_expression(init_tokens)?),
                        init_tokens,
                        self.context(),
                    )))
                };
                let condition = if condition_tokens.is_empty() {
                    None
                } else {
                    Some(self.parse_expression(condition_tokens)?)
                };
                let increment = if increment_tokens.is_empty() {
                    None
                } else {
                    Some(self.parse_expression(increment_tokens)?)
                };
                cursor.pos = close + 1;
                let body = self.parse_body(cursor)?;
                Ok(StmtKind::For {
                    init,
                    condition,
                    increment,
                    body,
                })
            }
            Some(Token::Keyword(Keyword::Switch)) => {
                let _scope = self
                    .features()
                    .control_statement_scopes
                    .then(|| self.enter_scope());
                let open = cursor.pos + 1;
                if tokens.value_at(open) != Some(&Token::LParen) {
                    return Err(self.error_at_tokens(
                        tokens,
                        cursor.pos,
                        "expected `(` after `switch`",
                    ));
                }
                let close = matching_paren(tokens, open)
                    .ok_or_else(|| self.error_at_tokens(tokens, cursor.pos, "expected `)`"))?;
                let clause = &tokens[open + 1..close];
                let (declaration, discriminant) = if self.starts_declaration(clause, 0) {
                    let (declaration, discriminant) = self.parse_selection_declaration(clause)?;
                    (Some(declaration), discriminant)
                } else {
                    (None, Some(self.parse_expression(clause)?))
                };
                cursor.pos = close + 1;
                let body = self.parse_body(cursor)?;
                if let Some(declaration) = declaration {
                    Ok(StmtKind::SwitchDeclaration {
                        declaration,
                        discriminant,
                        body,
                    })
                } else {
                    Ok(StmtKind::Switch {
                        discriminant: discriminant.ok_or_else(|| {
                            self.error_at_tokens(tokens, open + 1, "expected expression")
                        })?,
                        body,
                    })
                }
            }
            _ => {
                let end = top_level_semi(&tokens[cursor.pos..])
                    .map(|position| cursor.pos + position)
                    .ok_or_else(|| self.error_at_tokens(tokens, cursor.pos, "expected `;`"))?;
                let expression = self.parse_expression(&tokens[cursor.pos..end])?;
                cursor.pos = end + 1;
                Ok(StmtKind::Expr(expression))
            }
        }
    }

    pub(super) fn parse_expression(&self, tokens: &[Span<Token>]) -> Result<Expr, ParseError> {
        if tokens.is_empty() {
            return Err(self.error_at_tokens(tokens, 0, "expected expression"));
        }
        const_expr::Parser::parse_expression(tokens, self.context())
            .map_err(|error| self.error_at_tokens(tokens, 0, error.to_string()))
    }
}

fn shadow_parameter_names(parser: &Parser, parameters: Option<&ParameterList>) {
    if let Some(parameters) = parameters {
        for parameter in parameters.parameters() {
            if let Some(name) = parameter.declarator.name() {
                parser.names.bind(name, false);
            }
        }
    }
}
