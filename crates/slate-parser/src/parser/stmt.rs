use super::decl::{
    bare_identifier_names, join_node_text, matching_brace, matching_paren, signature_node_span,
    split_top_level, top_level_semi,
};
use super::declarator::DeclaratorParser;
use super::{Cursor, Fragment, Loc, Parser, coalesce_string_literals, lex, span_tokens, synthetic};
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
    ) -> Result<(FunctionDecl, usize), ParseError> {
        let provenance = self.node_provenance(&nodes[0]);
        let sig_node_count = signature_node_span(nodes, &self.typedef_names);
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
        let (mut attributes, mut index) = self
            .parse_attribute_groups(&sig_tokens, leading)
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
            let (more_attributes, position) = self
                .parse_attribute_groups(&sig_tokens, index)
                .map_err(|error| self.error_at(Loc::whole(code), error))?;
            attributes.extend(more_attributes);
            index = position;
        }
        let implicit_int = matches!(
            (sig_tokens.value_at(index), sig_tokens.value_at(index + 1)),
            (Some(Token::Ident(candidate)), Some(Token::LParen))
                if !self.typedef_names.contains(candidate)
        );
        let (ret_type, name, name_index) = if implicit_int {
            let Some(Token::Ident(name)) = sig_tokens.value_at(index) else {
                unreachable!()
            };
            (
                CType::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Int,
                    signed: true,
                }),
                name.clone(),
                index,
            )
        } else {
            let mut return_type_parser = DeclaratorParser {
                tokens: &sig_tokens,
                pos: index,
                typedef_names: &self.typedef_names,
                biggest_alignment: self.biggest_alignment,
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
            loop {
                match sig_tokens.value_at(return_type_parser.pos) {
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
                return_type_parser.pos += 1;
                let (more_attributes, position) = self
                    .parse_attribute_groups(&sig_tokens, return_type_parser.pos)
                    .map_err(|error| self.error_at(Loc::whole(code), error))?;
                attributes.extend(more_attributes);
                return_type_parser.pos = position;
            }
            let (mid_attributes, name_index) = self
                .parse_attribute_groups(&sig_tokens, return_type_parser.pos)
                .map_err(|error| self.error_at(Loc::whole(code), error))?;
            attributes.extend(mid_attributes);
            let name = match sig_tokens.value_at(name_index) {
                Some(Token::Ident(n)) => n.clone(),
                _ => return Err(self.error_at(Loc::whole(code), "expected function name")),
            };
            (ret_type, name, name_index)
        };
        if sig_tokens.value_at(name_index + 1) != Some(&Token::LParen) {
            return Err(self.error_at(
                Loc::at(code, code.len().saturating_sub(1), 1),
                "expected `(`",
            ));
        }
        let Some(close_paren) = matching_paren(&sig_tokens, name_index + 1) else {
            return Err(self.error_at(
                Loc::at(code, code.find('{').unwrap_or(0), 1),
                "expected `)`",
            ));
        };
        let kr_names = bare_identifier_names(
            sig_tokens[name_index + 2..close_paren]
                .iter()
                .map(|t| &t.value),
            &self.typedef_names,
        );
        let (parameters, variadic, body_index) = if let Some(names) = kr_names {
            let (parameters, body_index) =
                self.parse_kr_parameter_declarations(code, &sig_tokens, close_paren + 1, &names)?;
            (parameters, false, body_index)
        } else {
            let mut declarator_parser = DeclaratorParser {
                tokens: &sig_tokens,
                pos: name_index + 1,
                typedef_names: &self.typedef_names,
                biggest_alignment: self.biggest_alignment,
            };
            let (parameters, variadic) = declarator_parser
                .parse_parameters()
                .map_err(|error| self.error_at(Loc::whole(code), error.to_string()))?;
            let (signature_attributes, body_index) = self
                .parse_attribute_groups(&sig_tokens, declarator_parser.pos)
                .map_err(|error| self.error_at(Loc::whole(code), error))?;
            attributes.extend(signature_attributes);
            (parameters, variadic, body_index)
        };
        if sig_tokens.value_at(body_index) != Some(&Token::LBrace) {
            return Err(self.error_at(
                Loc::at(code, code.len().saturating_sub(1), 1),
                "expected function body",
            ));
        }

        if let Some(same_line_close) = matching_brace(&sig_tokens, body_index) {
            let body =
                self.parse_stmts_from_tokens(code, &sig_tokens[body_index + 1..same_line_close])?;
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
            let (trailing_attributes, _) = self
                .parse_attribute_groups(&closing_tokens, 1)
                .map_err(|error| self.error_at(Loc::whole(text), error))?;
            attributes.extend(trailing_attributes);
        }

        let body = self.parse_stmt_list(&nodes[sig_node_count..close_idx])?;
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

    pub(super) fn parse_kr_parameter_declarations(
        &self,
        code: &str,
        tokens: &[Span<Token>],
        mut pos: usize,
        names: &[String],
    ) -> Result<(Vec<Parameter>, usize), ParseError> {
        let mut declared: HashMap<String, (CType, Declarator)> = HashMap::new();
        while tokens.value_at(pos) != Some(&Token::LBrace) {
            let mut parser = DeclaratorParser {
                tokens,
                pos,
                typedef_names: &self.typedef_names,
                biggest_alignment: self.biggest_alignment,
            };
            parser.matches(Token::Keyword(Keyword::Register));
            let base_ty = parser
                .parse_base_type()
                .map_err(|error| self.error_at(Loc::whole(code), error.to_string()))?;
            loop {
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
                declared.insert(name.to_string(), (base_ty.clone(), declarator));
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
            .map(|name| {
                if let Some((ty, declarator)) = declared.remove(name) {
                    Parameter {
                        ty,
                        declarator: Some(declarator),
                        attributes: Vec::new(),
                    }
                } else {
                    Parameter {
                        ty: CType::Integer(IntegerType::Ranked {
                            rank: IntegerRank::Int,
                            signed: true,
                        }),
                        declarator: Some(Declarator::Name(name.clone())),
                        attributes: Vec::new(),
                    }
                }
            })
            .collect();
        Ok((parameters, pos))
    }

    pub(super) fn parse_stmt_list(&self, nodes: &[PPNode]) -> Result<Vec<SpannedStmt>, ParseError> {
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
                    let comment = group.map(Stmt::Comment);
                    if run_tokens.is_empty() {
                        stmts.push(comment);
                    } else {
                        pending_comments.push(comment);
                    }
                    index += consumed;
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
    ) -> Result<Vec<SpannedStmt>, ParseError> {
        let mut parser = self.clone();
        let mut position = 0;
        let mut stmts = Vec::new();
        while position < tokens.len() {
            let start = position;
            let mut fragment = Fragment::new(&parser, code, tokens, position);
            let stmt = parser.parse_one_stmt(&mut fragment)?;
            position = fragment.pos;
            if let Stmt::Decl(declaration) = &stmt {
                parser.record_declaration_typedefs(declaration);
            }
            stmts.push(span_tokens(stmt, &tokens[start..position]));
        }
        Ok(stmts)
    }

    pub(super) fn parse_body(
        &self,
        fragment: &mut Fragment,
    ) -> Result<Vec<SpannedStmt>, ParseError> {
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

    pub(super) fn parse_simple_keyword_stmt(
        &self,
        fragment: &mut Fragment,
        stmt: Stmt,
    ) -> Result<Stmt, ParseError> {
        fragment.pos += 1;
        fragment.expect(Token::Semi, "expected `;`")?;
        Ok(stmt)
    }

    pub(super) fn try_parse_nested_function(
        &self,
        code: &str,
        tokens: &[Span<Token>],
        start: usize,
    ) -> Result<Option<(FunctionDecl, usize)>, ParseError> {
        let fragment = Fragment::new(self, code, tokens, start);
        let (mut attributes, mut index) = self
            .parse_attribute_groups(tokens, start)
            .map_err(|error| fragment.error(error))?;
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
        let mut return_type_parser = DeclaratorParser::with_biggest_alignment(
            tokens,
            index,
            &self.typedef_names,
            self.biggest_alignment,
        );
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
        let mut declarator_parser = DeclaratorParser::with_biggest_alignment(
            tokens,
            name_index + 1,
            &self.typedef_names,
            self.biggest_alignment,
        );
        let Ok((parameters, variadic)) = declarator_parser.parse_parameters() else {
            return Ok(None);
        };
        let (signature_attributes, body_index) = self
            .parse_attribute_groups(tokens, declarator_parser.position())
            .map_err(|error| fragment.error(error))?;
        if tokens.value_at(body_index) != Some(&Token::LBrace) {
            return Ok(None);
        }
        attributes.extend(signature_attributes);
        let close =
            matching_brace(tokens, body_index).ok_or_else(|| fragment.error("expected `}`"))?;
        let body = self.parse_stmts_from_tokens(code, &tokens[body_index + 1..close])?;
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

    pub(super) fn starts_declaration(&self, tokens: &[Span<Token>], pos: usize) -> bool {
        let pos = if tokens.value_at(pos) == Some(&Token::Ident("__extension__".to_string())) {
            pos + 1
        } else {
            pos
        };
        match tokens.value_at(pos) {
            Some(Token::Keyword(keyword)) if keyword.is_storage_class_or_specifier() => true,
            Some(Token::Ident(name))
                if matches!(
                    name.as_str(),
                    "_Alignas" | "alignas" | "__auto_type" | "__attribute__" | "__attribute"
                ) =>
            {
                true
            }
            Some(token) => const_expr::starts_type_name(token, &self.typedef_names),
            None => false,
        }
    }

    pub(super) fn parse_one_stmt(&self, fragment: &mut Fragment) -> Result<Stmt, ParseError> {
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
                return Ok(Stmt::Attribute(attributes));
            }
        }

        if let Some(asm) = self.parse_asm_stmt(fragment)? {
            return Ok(Stmt::Asm(asm));
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
            let declaration = self.parse_declaration_tokens(code, &tokens[fragment.pos..=end])?;
            fragment.pos = end + 1;
            return Ok(Stmt::Decl(declaration));
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

    pub(super) fn parse_expression(
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
