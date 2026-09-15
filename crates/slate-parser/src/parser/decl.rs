use super::asm::is_asm_keyword;
use super::attributes::apply_vector_attributes;
use super::declarator::{DeclaratorError, DeclaratorParser, IdentifierList};
use super::{Annotation, Parser, ParserInput, span_tokens};
use crate::ast::*;
use crate::const_expr;
use crate::error::ParseError;
use crate::lexer::{Keyword, Token, TokenSpanExt};
use crate::reachability::filter_translation_unit;
use std::collections::HashSet;
use std::rc::Rc;

impl Parser {
    pub(super) fn parse_declaration_tokens(
        &self,
        tokens: &[Span<Token>],
    ) -> Result<Declaration, ParseError> {
        let mut parser = self.declarator_parser(tokens, 0);
        let mut specifiers =
            self.parse_declaration_specifiers(&mut parser, self.standard().allows_implicit_int())?;
        let declarators = self.parse_declarator_list(&mut parser, &mut specifiers, false)?;
        Ok(Declaration {
            specifiers,
            declarators: declarators.into_iter().map(into_init_declarator).collect(),
        })
    }

    pub(super) fn parse_pragma_tokens(&self, tokens: &[Span<Token>]) -> Result<Pragma, ParseError> {
        let text = tokens
            .values()
            .map(String::from)
            .collect::<Vec<_>>()
            .join(" ");
        let values = tokens.as_tokens();
        let kind = match values.as_slice() {
            [Token::Ident(name), Token::LParen, .., Token::RParen] if name == "pack" => {
                parse_pack(self, &tokens[2..tokens.len() - 1])?
            }
            [Token::Ident(name), Token::Ident(symbol)] if name == "weak" => PragmaKind::Weak {
                name: symbol.clone(),
                alias: None,
            },
            [
                Token::Ident(name),
                Token::Ident(symbol),
                Token::Equal,
                Token::Ident(alias),
            ] if name == "weak" => PragmaKind::Weak {
                name: symbol.clone(),
                alias: Some(alias.clone()),
            },
            [
                Token::Ident(gcc),
                Token::Ident(visibility),
                Token::Ident(action),
                rest @ ..,
            ] if gcc == "GCC" && visibility == "visibility" => {
                let value = match rest {
                    [Token::LParen, Token::Ident(value), Token::RParen] => Some(value.clone()),
                    _ => None,
                };
                PragmaKind::Visibility {
                    action: parse_stack_action(action)?,
                    visibility: value,
                }
            }
            [Token::Ident(std), Token::Ident(option), Token::Ident(value)] if std == "STDC" => {
                match (parse_stdc_option(option), parse_on_off(value)) {
                    (Some(option), Ok(enabled)) => PragmaKind::Stdc { option, enabled },
                    _ => PragmaKind::Opaque(text.clone()),
                }
            }
            [
                Token::Ident(name),
                Token::Ident(option),
                Token::Ident(value),
            ] if name == "float_control" => {
                match (parse_float_control_option(option), parse_on_off(value)) {
                    (Some(option), Ok(enabled)) => PragmaKind::FloatControl { option, enabled },
                    _ => PragmaKind::Opaque(text.clone()),
                }
            }
            [Token::Ident(name), Token::Ident(action)] if name == "ms_struct" => {
                PragmaKind::MsStruct {
                    action: parse_stack_action(action)?,
                }
            }
            _ => PragmaKind::Opaque(text),
        };
        Ok(Pragma { kind })
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

    pub(super) fn parse_declaration_specifiers(
        &self,
        parser: &mut DeclaratorParser,
        implicit_int_function: bool,
    ) -> Result<DeclarationSpecifiers, ParseError> {
        parser
            .parse_specifiers(implicit_int_function)
            .map_err(|error| match error {
                DeclaratorError::Parse(error) => error,
                error => self.error_at_tokens(parser.tokens, parser.pos, error.to_string()),
            })
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

    pub(super) fn parse_input(
        &mut self,
        input: ParserInput,
        root_file: FileId,
    ) -> Result<TranslationUnit, ParseError> {
        self.tags.borrow_mut().clear();
        self.input = Rc::new(input);
        let input = self.input.clone();
        let mut position = 0;
        let mut decls = self.parse_decls(&input.tokens, &mut position, false)?;
        for annotation in input.take_remaining() {
            decls.push(self.declaration_annotation(annotation)?);
        }
        let options = self.effective_options();
        Ok(filter_translation_unit(
            &TranslationUnit {
                options: options.clone(),
                decls,
                tags: self.tags.take(),
                flavor: self.flavor(),
                target: options.effective_target(self.target),
            },
            root_file,
        ))
    }

    fn declaration_annotation(&self, annotation: Span<Annotation>) -> Result<Decl, ParseError> {
        let kind = match &annotation.value {
            Annotation::Comment(group) => DeclKind::Comment(group.clone()),
            Annotation::Pragma(tokens) => DeclKind::Pragma(self.parse_pragma_tokens(tokens)?),
        };
        Ok(annotation.with_value(kind))
    }

    fn parse_decls(
        &mut self,
        tokens: &[Span<Token>],
        position: &mut usize,
        linkage: bool,
    ) -> Result<Vec<Decl>, ParseError> {
        let mut decls = Vec::new();
        while *position < tokens.len() {
            for annotation in self.input.take(tokens, *position, *position) {
                decls.push(self.declaration_annotation(annotation)?);
            }
            if linkage && tokens.value_at(*position) == Some(&Token::RBrace) {
                *position += 1;
                return Ok(decls);
            }
            if tokens.value_at(*position) == Some(&Token::Semi) {
                *position += 1;
                continue;
            }
            if tokens.value_at(*position) == Some(&Token::Keyword(Keyword::Extern))
                && matches!(tokens.value_at(*position + 1), Some(Token::StringLit(_)))
                && tokens.value_at(*position + 2) == Some(&Token::LBrace)
            {
                *position += 3;
                decls.extend(self.parse_decls(tokens, position, true)?);
                continue;
            }
            let start = *position;
            let decl = self.parse_external_item(tokens, position)?;
            self.record_typedefs(&decl);
            let mut comments = Vec::new();
            for annotation in self.input.take(tokens, start, position.saturating_sub(1)) {
                if matches!(annotation.value, Annotation::Comment(_)) {
                    comments.push(self.declaration_annotation(annotation)?);
                } else {
                    decls.push(self.declaration_annotation(annotation)?);
                }
            }
            decls.push(span_tokens(decl, &tokens[start..*position]));
            decls.extend(comments);
        }
        if linkage {
            return Err(self.error_at_tokens(tokens, *position, "expected `}`"));
        }
        Ok(decls)
    }

    pub(super) fn parse_external_item(
        &self,
        tokens: &[Span<Token>],
        position: &mut usize,
    ) -> Result<DeclKind, ParseError> {
        let start = *position;
        if tokens.value_at(start) == Some(&Token::Keyword(Keyword::StaticAssert))
            || is_asm_keyword(tokens.value_at(start))
        {
            let end = start
                + top_level_semi(&tokens[start..])
                    .ok_or_else(|| self.error_at_tokens(tokens, start, "expected `;`"))?
                + 1;
            *position = end;
            let item = &tokens[start..end];
            if tokens.value_at(start) == Some(&Token::Keyword(Keyword::StaticAssert)) {
                return self
                    .parse_static_assert("", item)
                    .map(DeclKind::StaticAssert);
            }
            return self
                .parse_file_scope_asm("", item)?
                .map(DeclKind::Asm)
                .ok_or_else(|| self.error_at_tokens(tokens, start, "expected asm"));
        }
        let mut parser = self.declarator_parser(tokens, start);
        parser.identifier_list = IdentifierList::Accepted;
        let mut specifiers =
            self.parse_declaration_specifiers(&mut parser, self.standard().allows_implicit_int())?;
        let mut declarators = Vec::new();
        if parser.peek() != Some(&Token::Semi) {
            let mut first = self.parse_one_declarator(&mut parser, &specifiers, false)?;
            let identifier_list =
                std::mem::replace(&mut parser.identifier_list, IdentifierList::Rejected);
            if let IdentifierList::Parsed(names) = identifier_list {
                let (parameters, end) =
                    self.parse_kr_parameter_declarations("", tokens, parser.pos, &names)?;
                if let Some(list) = first.value.declarator.function_parameters_mut() {
                    *list = ParameterList::Prototype {
                        parameters,
                        variadic: false,
                    };
                }
                parser.pos = end;
            }
            if parser.peek() == Some(&Token::LBrace)
                && first.declarator.function_parameters().is_some()
                && first.initializer.is_none()
            {
                let body = self.parse_function_body(tokens, &mut parser.pos, &first.declarator)?;
                *position = parser.pos;
                return Ok(DeclKind::Function(FunctionDefinition {
                    specifiers,
                    declarator: first.value.declarator,
                    attributes: first.value.attributes,
                    body,
                }));
            }
            declarators.push(first);
            while parser.matches(Token::Comma) {
                declarators.push(self.parse_one_declarator(&mut parser, &specifiers, false)?);
            }
        }
        if !parser.matches(Token::Semi) {
            return Err(self.error_at_tokens(tokens, parser.pos, "expected `;`"));
        }
        *position = parser.pos;
        let attributes = specifiers
            .attributes
            .iter()
            .chain(declarators.iter().flat_map(|parsed| &parsed.attributes))
            .cloned()
            .collect::<Vec<_>>();
        specifiers.ty = apply_vector_attributes(specifiers.ty, &attributes);
        Ok(DeclKind::Declaration(Declaration {
            specifiers,
            declarators: declarators.into_iter().map(into_init_declarator).collect(),
        }))
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
        } else {
            for name in declaration.names() {
                self.typedef_names.remove(name);
            }
        }
    }
}

fn parse_stack_action(token: &str) -> Result<PragmaStackAction, ParseError> {
    match token {
        "push" => Ok(PragmaStackAction::Push),
        "pop" => Ok(PragmaStackAction::Pop),
        "show" => Ok(PragmaStackAction::Show),
        _ => Ok(PragmaStackAction::Set),
    }
}

fn parse_stack_action_token(token: &Token) -> Result<PragmaStackAction, ParseError> {
    match token {
        Token::Ident(value) => parse_stack_action(value),
        _ => Err(ParseError::new(
            "<pragma>",
            "",
            0,
            1,
            "expected pragma stack action",
        )),
    }
}

fn parse_on_off(token: &str) -> Result<bool, ParseError> {
    match token {
        value if value.eq_ignore_ascii_case("on") => Ok(true),
        value if value.eq_ignore_ascii_case("off") => Ok(false),
        _ => Err(ParseError::new(
            "<pragma>",
            "",
            0,
            1,
            "expected `on` or `off`",
        )),
    }
}

fn parse_stdc_option(token: &str) -> Option<StdcPragmaOption> {
    match token {
        "FENV_ACCESS" => Some(StdcPragmaOption::FenvAccess),
        "FP_CONTRACT" => Some(StdcPragmaOption::FpContract),
        "CX_LIMITED_RANGE" => Some(StdcPragmaOption::CxLimitedRange),
        _ => None,
    }
}

fn parse_float_control_option(token: &str) -> Option<FloatControlOption> {
    match token {
        "precise" => Some(FloatControlOption::Precise),
        "except" => Some(FloatControlOption::Except),
        _ => None,
    }
}

fn parse_pack(parser: &Parser, tokens: &[Span<Token>]) -> Result<PragmaKind, ParseError> {
    let action = tokens.first().map_or(PragmaStackAction::Set, |token| {
        parse_stack_action_token(&token.value).unwrap_or(PragmaStackAction::Set)
    });
    let alignment_start = match action {
        PragmaStackAction::Push => 2,
        PragmaStackAction::Set => 0,
        PragmaStackAction::Pop | PragmaStackAction::Show => tokens.len(),
    };
    let alignment = tokens.get(alignment_start).and_then(|_token| {
        const_expr::Parser::parse_expression(
            &tokens[alignment_start..],
            &parser.typedef_names,
            Some(parser),
        )
        .ok()
    });
    Ok(PragmaKind::Pack { action, alignment })
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
        _ => {}
    }
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
