use super::attributes::{parse_attribute_groups, parse_c23_attribute_groups};
use super::decl::{bare_identifier_names, matching_paren, set_qualifier, specifiers_with_type};
use super::{Cursor, FALLBACK_BIGGEST_ALIGNMENT, ParseContext, Parser, span_tokens};
use crate::ast::*;
use crate::const_expr;
use crate::lexer::{Keyword, Token, TokenSpanExt};
use miette::Diagnostic;
use thiserror::Error;

#[derive(Debug, Error, Diagnostic)]
pub(crate) enum DeclaratorError {
    #[error(transparent)]
    #[diagnostic(transparent)]
    Parse(#[from] crate::error::ParseError),
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
    #[error("cannot combine `{0}` with previous declaration specifiers")]
    CannotCombine(String),
    #[error("unsupported typeof expression")]
    UnsupportedTypeofExpression,
    #[error("expected declarator")]
    ExpectedDeclarator,
    #[error("tag definition is not allowed here")]
    TagDefinitionNotAllowed,
    #[error("multiple storage classes")]
    MultipleStorageClasses,
    #[error("duplicate `_Thread_local`")]
    DuplicateThreadLocal,
    #[error("{0}")]
    Other(String),
}

impl From<String> for DeclaratorError {
    fn from(message: String) -> Self {
        DeclaratorError::Other(message)
    }
}

pub(crate) struct DeclaratorParser<'a> {
    pub(super) tokens: &'a [Span<Token>],
    pub(super) pos: usize,
    pub(super) biggest_alignment: i64,
    pub(super) context: ParseContext<'a>,
    pub(super) identifier_list: IdentifierList,
    pub(super) definition_bindings: std::collections::HashMap<String, super::NameBinding>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum IdentifierList {
    Rejected,
    Accepted,
    Parsed(Vec<String>),
}

impl<'a> DeclaratorParser<'a> {
    pub(crate) fn new(tokens: &'a [Span<Token>], pos: usize, context: ParseContext<'a>) -> Self {
        Self {
            tokens,
            pos,
            biggest_alignment: context
                .parser()
                .map_or(FALLBACK_BIGGEST_ALIGNMENT, |parser| {
                    parser.biggest_alignment
                }),
            context,
            identifier_list: IdentifierList::Rejected,
            definition_bindings: std::collections::HashMap::new(),
        }
    }

    pub(crate) fn position(&self) -> usize {
        self.pos
    }

    pub(crate) fn parse_attributes(&mut self) -> Result<Vec<Span<Attribute>>, String> {
        let (attributes, position) =
            parse_attribute_groups(self.tokens, self.pos, self.biggest_alignment, self.context)?;
        self.pos = position;
        Ok(attributes)
    }

    pub(super) fn peek(&self) -> Option<&Token> {
        self.tokens.get(self.pos).map(|span| &span.value)
    }

    pub(super) fn matches(&mut self, expected: Token) -> bool {
        if self.peek() == Some(&expected) {
            self.pos += 1;
            true
        } else {
            false
        }
    }

    fn parse_type_specifiers(
        &mut self,
        specifiers: &mut DeclarationSpecifiers,
    ) -> Result<TypeSpecifier, DeclaratorError> {
        let mut set = TypeSpecifierSet::default();
        while self.take_type_specifier(&mut set)? {
            self.parse_specifier_keywords(specifiers)?;
        }
        if set.shape.is_empty() {
            return Err(match self.peek().cloned() {
                Some(token) => {
                    self.pos += 1;
                    DeclaratorError::UnexpectedToken(token)
                }
                None => DeclaratorError::ExpectedDeclarationType,
            });
        }
        set.finish()
    }

    fn take_type_specifier(&mut self, set: &mut TypeSpecifierSet) -> Result<bool, DeclaratorError> {
        let Some(token) = self.peek().cloned() else {
            return Ok(false);
        };
        let shape = set.shape;
        let Some(piece) = specifier_piece(&token, shape.is_empty()) else {
            return Ok(false);
        };
        let Some(next) = shape.add(piece) else {
            return Err(DeclaratorError::CannotCombine(match token {
                Token::Keyword(keyword) => <&str>::from(keyword).to_string(),
                Token::Ident(name) => name.into(),
                other => format!("{other:?}"),
            }));
        };
        self.pos += 1;
        match piece {
            Piece::Base(BaseKind::BitInt) => {
                set.payload = Some(TypeSpecifier::Integer(IntegerType::BitInt {
                    width: self.parse_bit_int_width()?,
                    signed: true,
                }));
            }
            Piece::Base(BaseKind::Other) => set.payload = Some(self.parse_other_type(token)?),
            _ => {}
        }
        set.shape = next;
        Ok(true)
    }

    fn parse_other_type(&mut self, token: Token) -> Result<TypeSpecifier, DeclaratorError> {
        if let Token::Ident(name) = &token
            && let Some(integer) = builtin_integer_typedef(name)
        {
            return Ok(TypeSpecifier::Integer(integer));
        }
        Ok(match token {
            Token::Keyword(Keyword::Bool) => TypeSpecifier::Bool,
            Token::Keyword(Keyword::Void) => TypeSpecifier::Void,
            Token::Keyword(Keyword::Atomic) => {
                self.expect(
                    Token::LParen,
                    DeclaratorError::ExpectedToken(Token::LParen, "after `_Atomic`"),
                )?;
                let ty = self.parse_type_name()?;
                self.expect(
                    Token::RParen,
                    DeclaratorError::ExpectedToken(Token::RParen, "after `_Atomic` type"),
                )?;
                TypeSpecifier::Atomic(Box::new(ty))
            }
            Token::Keyword(Keyword::Typeof) => self.parse_typeof()?,
            Token::Keyword(Keyword::TypeofUnqual) => {
                TypeSpecifier::TypeOfUnqual(self.parse_typeof_operand()?)
            }
            Token::Keyword(Keyword::Struct) => self.parse_record_type(TagKind::Struct)?,
            Token::Keyword(Keyword::Union) => self.parse_record_type(TagKind::Union)?,
            Token::Keyword(Keyword::Enum) => self.parse_enum_type()?,
            Token::Ident(name) if is_target_builtin_name(&name) => {
                TypeSpecifier::TargetBuiltin(name.into())
            }
            Token::Ident(name) => TypeSpecifier::Named(span_tokens(
                name.into(),
                &self.tokens[self.pos - 1..self.pos],
                self.context,
            )),
            other => return Err(DeclaratorError::UnexpectedToken(other)),
        })
    }

    pub(super) fn parse_record_type(
        &mut self,
        kind: TagKind,
    ) -> Result<TypeSpecifier, DeclaratorError> {
        let start = self.pos - 1;
        let mut attributes = self.parse_attributes()?;
        let name = self.tag_name();
        if self.peek() == Some(&Token::LBrace) {
            let body = TagBody::Record(self.parse_field_list()?);
            attributes.extend(self.parse_attributes()?);
            return self.define_tag(kind, name.map(|name| name.value), attributes, body, start);
        }
        tag_reference(kind, name, None)
    }

    fn tag_name(&mut self) -> Option<Span<String>> {
        let Some(Token::Ident(name)) = self.peek() else {
            return None;
        };
        let name = span_tokens(
            name.to_string(),
            &self.tokens[self.pos..self.pos + 1],
            self.context,
        );
        self.pos += 1;
        Some(name)
    }

    fn define_tag(
        &self,
        kind: TagKind,
        name: Option<String>,
        attributes: Vec<Span<Attribute>>,
        body: TagBody,
        start: usize,
    ) -> Result<TypeSpecifier, DeclaratorError> {
        let parser = self
            .context
            .parser()
            .ok_or(DeclaratorError::TagDefinitionNotAllowed)?;
        let definition = TagDefinition {
            id: TagId(0),
            kind,
            name,
            attributes,
            body,
        };
        let id = parser.define_tag(span_tokens(
            definition,
            &self.tokens[start..self.pos],
            self.context,
        ));
        Ok(TypeSpecifier::Tag(TagSpecifier::Definition(id)))
    }

    pub(super) fn parse_enum_type(&mut self) -> Result<TypeSpecifier, DeclaratorError> {
        let start = self.pos - 1;
        let mut attributes = self.parse_attributes()?;
        let name = self.tag_name();
        let mut fixed_type = None;
        if self.peek() == Some(&Token::Colon)
            && self
                .tokens
                .value_at(self.pos + 1)
                .is_some_and(|token| const_expr::starts_type_name(token, self.context))
        {
            self.pos += 1;
            fixed_type = Some(self.parse_type_name()?);
        }
        if self.peek() == Some(&Token::LBrace) {
            let body = TagBody::Enum {
                fixed_type,
                enumerators: self.parse_enumerator_list()?,
            };
            attributes.extend(self.parse_attributes()?);
            return self.define_tag(
                TagKind::Enum,
                name.map(|name| name.value),
                attributes,
                body,
                start,
            );
        }
        tag_reference(TagKind::Enum, name, fixed_type.map(Box::new))
    }

    pub(super) fn parse_field_list(&mut self) -> Result<Vec<FieldItem>, DeclaratorError> {
        self.pos += 1;
        let mut fields = Vec::new();
        let parser = self
            .context
            .parser()
            .ok_or(DeclaratorError::TagDefinitionNotAllowed)?;
        loop {
            fields.extend(
                parser
                    .input
                    .take_comments(self.tokens, self.pos, self.pos)
                    .into_iter()
                    .filter_map(|annotation| match &annotation.value {
                        super::Annotation::Comment(group) => Some(
                            annotation
                                .clone()
                                .with_value(FieldItemKind::Comment(group.clone())),
                        ),
                        _ => None,
                    }),
            );
            if self.matches(Token::RBrace) {
                break;
            }
            let start = self.pos;
            let end = start
                + super::decl::top_level_semi(&self.tokens[start..]).ok_or(
                    DeclaratorError::ExpectedToken(Token::Semi, "in struct/union body"),
                )?
                + 1;
            let (specifiers, declarators) = parser
                .parse_field_declaration_tokens(&self.tokens[start..end])
                .map_err(DeclaratorError::Parse)?;
            self.pos = end;
            fields.push(span_tokens(
                FieldItemKind::Field(FieldDecl {
                    specifiers,
                    declarators,
                }),
                &self.tokens[start..end],
                self.context,
            ));
            fields.extend(
                parser
                    .input
                    .take_comments(self.tokens, start, end - 1)
                    .into_iter()
                    .filter_map(|annotation| match &annotation.value {
                        super::Annotation::Comment(group) => Some(
                            annotation
                                .clone()
                                .with_value(FieldItemKind::Comment(group.clone())),
                        ),
                        _ => None,
                    }),
            );
        }
        Ok(fields)
    }

    pub(super) fn parse_enumerator_list(&mut self) -> Result<Vec<EnumItem>, DeclaratorError> {
        self.pos += 1;
        let mut items = Vec::new();
        loop {
            if let Some(parser) = self.context.parser() {
                items.extend(
                    parser
                        .input
                        .take_comments(self.tokens, self.pos, self.pos)
                        .into_iter()
                        .filter_map(|annotation| match &annotation.value {
                            super::Annotation::Comment(group) => Some(
                                annotation
                                    .clone()
                                    .with_value(EnumItemKind::Comment(group.clone())),
                            ),
                            _ => None,
                        }),
                );
            }
            if self.matches(Token::RBrace) {
                break;
            }
            let start = self.pos;
            let Some(Token::Ident(name)) = self.peek().cloned() else {
                return Err(DeclaratorError::ExpectedEnumerator);
            };
            self.pos += 1;
            let attributes = self.parse_attributes()?;
            let value = if self.matches(Token::Equal) {
                let (value, end) =
                    const_expr::Parser::parse_one(self.tokens, self.pos, self.context)
                        .map_err(|error| error.to_string())?;
                self.pos = end;
                Some(value)
            } else {
                None
            };
            if let Some(parser) = self.context.parser() {
                parser.names.bind(&name, false);
            }
            items.push(span_tokens(
                EnumItemKind::Enumerator(Enumerator {
                    name: name.to_string(),
                    attributes,
                    value,
                }),
                &self.tokens[start..self.pos],
                self.context,
            ));
            if self.matches(Token::Comma) {
                continue;
            }
            if self.matches(Token::RBrace) {
                break;
            }
            return Err(DeclaratorError::ExpectedCommaOrRBrace);
        }
        Ok(items)
    }

    fn parse_bit_int_width(&mut self) -> Result<Expr, DeclaratorError> {
        self.expect(
            Token::LParen,
            DeclaratorError::ExpectedToken(Token::LParen, "after `_BitInt`"),
        )?;
        let (width, end) = const_expr::Parser::parse_one(self.tokens, self.pos, self.context)
            .map_err(|error| DeclaratorError::Other(error.to_string()))?;
        self.pos = end;
        self.expect(
            Token::RParen,
            DeclaratorError::ExpectedToken(Token::RParen, "after `_BitInt` width"),
        )?;
        Ok(width)
    }

    pub(super) fn parse_typeof(&mut self) -> Result<TypeSpecifier, DeclaratorError> {
        self.parse_typeof_operand().map(TypeSpecifier::TypeOf)
    }

    pub(super) fn parse_typeof_operand(&mut self) -> Result<TypeOfOperand, DeclaratorError> {
        self.expect(
            Token::LParen,
            DeclaratorError::ExpectedToken(Token::LParen, "after `typeof`"),
        )?;
        if self.typeof_type_start() {
            let ty = self.parse_type_name()?;
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
        let expression = const_expr::Parser::parse_expression(tokens, self.context)
            .map_err(|error| DeclaratorError::Other(error.to_string()))?;
        Ok(TypeOfOperand::Expression(expression))
    }

    pub(super) fn typeof_type_start(&self) -> bool {
        self.peek()
            .is_some_and(|token| const_expr::starts_type_name(token, self.context))
    }

    pub(super) fn parse_initializer(&mut self) -> Result<Initializer, const_expr::ConstExprError> {
        let (initializer, end) =
            const_expr::Parser::parse_initializer(self.tokens, self.pos, self.context)?;
        self.pos = end;
        Ok(initializer)
    }

    pub(crate) fn parse_declarator(
        &mut self,
        allow_abstract: bool,
    ) -> Result<Declarator, DeclaratorError> {
        let mut pointers = Vec::new();
        while self.matches(Token::Star) {
            pointers.push(self.parse_pointer_qualifiers()?);
        }
        let mut declarator = match self.peek().cloned() {
            Some(Token::Ident(name)) => {
                self.pos += 1;
                Declarator::Name(name.into())
            }
            Some(Token::Keyword(keyword @ Keyword::Float16)) => {
                self.pos += 1;
                let name: &'static str = keyword.into();
                Declarator::Name(name.into())
            }
            Some(Token::LParen) if allow_abstract && self.opens_parameter_list(self.pos + 1) => {
                Declarator::Abstract
            }
            Some(Token::LParen) => {
                self.pos += 1;
                let attributes = self.parse_attributes()?;
                let mut declarator = self.parse_declarator(allow_abstract)?;
                if !self.matches(Token::RParen) {
                    return Err(DeclaratorError::ExpectedToken(
                        Token::RParen,
                        "in declarator",
                    ));
                }
                if !attributes.is_empty() {
                    declarator = Declarator::Attributed {
                        inner: Box::new(declarator),
                        attributes,
                    };
                }
                Declarator::Grouped(Box::new(declarator))
            }
            _ if allow_abstract => Declarator::Abstract,
            _ => return Err(DeclaratorError::ExpectedDeclarator),
        };
        if matches!(declarator, Declarator::Name(_)) {
            declarator = self.wrap_c23_attributes(declarator)?;
        }

        for (qualifiers, attributes) in pointers.into_iter().rev() {
            declarator = Declarator::Pointer {
                qualifiers,
                attributes,
                inner: Box::new(declarator),
            };
        }

        loop {
            declarator = match self.peek() {
                Some(Token::LBracket)
                    if self.tokens.value_at(self.pos + 1) == Some(&Token::LBracket) =>
                {
                    self.wrap_c23_attributes(declarator)?
                }
                Some(Token::LBracket) => {
                    self.pos += 1;
                    let mut is_static = self.matches(Token::Keyword(Keyword::Static));
                    let qualifiers = self.take_qualifiers();
                    if !is_static {
                        is_static = self.matches(Token::Keyword(Keyword::Static));
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
                            if self.peek().is_none() {
                                return Err(DeclaratorError::ExpectedToken(
                                    Token::RBracket,
                                    "in array declarator",
                                ));
                            }
                            match self.peek() {
                                Some(Token::LParen | Token::LBrace | Token::LBracket) => depth += 1,
                                Some(Token::RParen | Token::RBrace | Token::RBracket) => depth -= 1,
                                _ => {}
                            }
                            self.pos += 1;
                        }
                        let bound_tokens = &self.tokens[start..self.pos];
                        let size = const_expr::Parser::parse_expression(bound_tokens, self.context)
                            .map_err(|error| DeclaratorError::Other(error.to_string()))?;
                        ArraySize::Expression(size)
                    };
                    self.expect(
                        Token::RBracket,
                        DeclaratorError::ExpectedToken(Token::RBracket, "in array declarator"),
                    )?;
                    Declarator::Array {
                        inner: Box::new(declarator),
                        size,
                        qualifiers,
                        is_static,
                    }
                }
                Some(Token::LParen) => Declarator::Function {
                    inner: Box::new(declarator),
                    parameters: self.parse_parameters()?,
                },
                _ => break,
            };
        }
        Ok(declarator)
    }

    pub(crate) fn parse_specifiers(
        &mut self,
        implicit_int_function: bool,
    ) -> Result<DeclarationSpecifiers, DeclaratorError> {
        let mut specifiers = specifiers_with_type(TypeSpecifier::Void);
        while self.peek() == Some(&Token::Ident("__extension__".into())) {
            self.pos += 1;
        }
        specifiers.attributes = self.parse_attributes()?;
        self.parse_specifier_keywords(&mut specifiers)?;
        let gnu_auto_type = self.matches(Token::Ident("__auto_type".into()));
        self.parse_specifier_keywords(&mut specifiers)?;
        let c23_auto_inference = self.context.features().auto_type_inference
            && specifiers.storage == StorageClass::Auto
            && match self.peek() {
                Some(Token::Ident(name)) => !self.context.is_typedef(name),
                Some(Token::Star | Token::LParen) => true,
                _ => false,
            };
        if c23_auto_inference {
            specifiers.storage = StorageClass::None;
        }
        let inferred =
            gnu_auto_type || c23_auto_inference || specifiers.ty == TypeSpecifier::Inferred;
        let implicit_int = implicit_int_function
            && self.context.features().implicit_int.is_accepted()
            && matches!(self.peek(), Some(Token::Ident(name)) if !self.context.is_typedef(name))
            && matches!(
                self.tokens.value_at(self.pos + 1),
                Some(Token::LParen | Token::Semi | Token::Comma | Token::Equal)
            );
        if !implicit_int
            && !inferred
            && matches!(self.peek(), Some(Token::Ident(name)) if !self.context.is_typedef(name))
            && matches!(
                self.tokens.value_at(self.pos + 1),
                Some(Token::LParen | Token::Semi)
            )
            && !self.context.features().implicit_int.is_accepted()
        {
            return Err(DeclaratorError::Other(
                "a type specifier is required for all declarations".into(),
            ));
        }
        specifiers.ty = if inferred {
            TypeSpecifier::Inferred
        } else if implicit_int {
            TypeSpecifier::Integer(IntegerType::Ranked {
                rank: IntegerRank::Int,
                signed: true,
            })
        } else {
            let ty = self.parse_type_specifiers(&mut specifiers)?;
            if specifiers.ty == TypeSpecifier::Inferred {
                TypeSpecifier::Inferred
            } else {
                ty
            }
        };
        self.parse_specifier_keywords(&mut specifiers)?;
        Ok(specifiers)
    }

    pub(crate) fn parse_specifier_keywords(
        &mut self,
        specifiers: &mut DeclarationSpecifiers,
    ) -> Result<(), DeclaratorError> {
        loop {
            while self.peek() == Some(&Token::Ident("__extension__".into())) {
                self.pos += 1;
            }
            let start = self.pos;
            let attributes = self.parse_attributes()?;
            if self.pos != start {
                specifiers.attributes.extend(attributes);
                continue;
            }
            if let Some(qualifier) = self.take_qualifier() {
                set_qualifier(&mut specifiers.qualifiers, qualifier);
                continue;
            }
            if self.matches(Token::Keyword(Keyword::Inline)) {
                specifiers.is_inline = true;
                continue;
            }
            if self.matches(Token::Keyword(Keyword::ForceInline)) {
                specifiers.is_inline = true;
                specifiers.attributes.push(span_tokens(
                    Attribute::AlwaysInline,
                    &self.tokens[self.pos - 1..self.pos],
                    self.context,
                ));
                continue;
            }
            if self.matches(Token::Keyword(Keyword::Noreturn)) {
                specifiers.is_noreturn = true;
                continue;
            }
            if self.matches(Token::Keyword(Keyword::Constexpr)) {
                specifiers.is_constexpr = true;
                continue;
            }
            if self.matches(Token::Keyword(Keyword::ThreadLocal)) {
                if specifiers.is_thread_local {
                    return Err(DeclaratorError::DuplicateThreadLocal);
                }
                specifiers.is_thread_local = true;
                continue;
            }
            let Some(Token::Keyword(keyword)) = self.peek() else {
                return Ok(());
            };
            let storage = match *keyword {
                Keyword::Typedef => StorageClass::Typedef,
                Keyword::Extern => StorageClass::Extern,
                Keyword::Static => StorageClass::Static,
                Keyword::Auto => StorageClass::Auto,
                Keyword::Register => StorageClass::Register,
                _ => return Ok(()),
            };
            let inference = self.context.features().auto_type_inference;
            match (specifiers.storage, storage) {
                (StorageClass::None, _) => specifiers.storage = storage,
                (StorageClass::Auto, other) | (other, StorageClass::Auto)
                    if inference && other != StorageClass::Auto =>
                {
                    specifiers.storage = other;
                    specifiers.ty = TypeSpecifier::Inferred;
                }
                _ => return Err(DeclaratorError::MultipleStorageClasses),
            }
            self.pos += 1;
        }
    }

    pub(crate) fn parse_type_name(&mut self) -> Result<TypeName, DeclaratorError> {
        let mut specifiers = self.parse_specifiers(false)?;
        let ty = std::mem::replace(&mut specifiers.ty, TypeSpecifier::Void);
        specifiers.ty = ty.with_type_attributes(&specifiers.attributes);
        let declarator = self.parse_declarator(true)?;
        Ok(TypeName {
            specifiers,
            declarator,
        })
    }

    fn parse_pointer_qualifiers(
        &mut self,
    ) -> Result<(Qualifiers, Vec<Span<Attribute>>), DeclaratorError> {
        let mut qualifiers = Qualifiers::default();
        let mut attributes = Vec::new();
        loop {
            let start = self.pos;
            while let Some(qualifier) = self.take_qualifier() {
                set_qualifier(&mut qualifiers, qualifier);
            }
            attributes.extend(self.parse_attributes()?);
            if self.pos == start {
                return Ok((qualifiers, attributes));
            }
        }
    }

    fn wrap_c23_attributes(&mut self, inner: Declarator) -> Result<Declarator, DeclaratorError> {
        let (attributes, position) = parse_c23_attribute_groups(
            self.tokens,
            self.pos,
            self.biggest_alignment,
            self.context,
        )?;
        self.pos = position;
        Ok(if attributes.is_empty() {
            inner
        } else {
            Declarator::Attributed {
                inner: Box::new(inner),
                attributes,
            }
        })
    }

    fn opens_parameter_list(&self, pos: usize) -> bool {
        let pos = parse_attribute_groups(self.tokens, pos, self.biggest_alignment, self.context)
            .map_or(pos, |(_, after_attributes)| after_attributes);
        match self.tokens.value_at(pos) {
            Some(Token::RParen | Token::Ellipsis | Token::Keyword(Keyword::Register)) => true,
            Some(token) => const_expr::starts_type_name(token, self.context),
            None => false,
        }
    }

    pub(crate) fn take_qualifiers(&mut self) -> Qualifiers {
        let mut qualifiers = Qualifiers::default();
        while let Some(qualifier) = self.take_qualifier() {
            set_qualifier(&mut qualifiers, qualifier);
        }
        qualifiers
    }

    pub(super) fn take_qualifier(&mut self) -> Option<Keyword> {
        let Some(Token::Keyword(keyword)) = self.peek() else {
            return None;
        };
        if *keyword == Keyword::Atomic && self.tokens.value_at(self.pos + 1) == Some(&Token::LParen)
        {
            return None;
        }
        if !matches!(
            keyword,
            Keyword::Const
                | Keyword::Volatile
                | Keyword::Restrict
                | Keyword::Atomic
                | Keyword::Unaligned
                | Keyword::Ptr32
                | Keyword::Ptr64
                | Keyword::Sptr
                | Keyword::Uptr
                | Keyword::SegFs
                | Keyword::SegGs
        ) {
            return None;
        }
        let keyword = *keyword;
        self.matches(Token::Keyword(keyword));
        Some(keyword)
    }

    pub(super) fn parse_parameters(&mut self) -> Result<ParameterList, DeclaratorError> {
        let _scope = self.context.parser().map(Parser::enter_scope);
        let open = self.pos;
        self.expect(
            Token::LParen,
            DeclaratorError::ExpectedToken(Token::LParen, "in function declarator"),
        )?;
        let accepts_identifier_list = self.identifier_list == IdentifierList::Accepted;
        if accepts_identifier_list {
            self.identifier_list = IdentifierList::Rejected;
        }
        if self.matches(Token::RParen) {
            return Ok(ParameterList::Empty);
        }
        if self.peek() == Some(&Token::Keyword(Keyword::Void))
            && self.tokens.value_at(self.pos + 1) == Some(&Token::RParen)
        {
            self.pos += 2;
            return Ok(ParameterList::Void);
        }
        if accepts_identifier_list
            && let Some(close) = matching_paren(self.tokens, open)
            && let Some(names) = bare_identifier_names(
                self.tokens[open + 1..close]
                    .iter()
                    .map(|token| &token.value),
                self.context,
            )
        {
            self.pos = close + 1;
            self.identifier_list = IdentifierList::Parsed(names);
            return Ok(ParameterList::Empty);
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
            let parameter_start = self.pos;
            let mut specifiers = self.parse_specifiers(false)?;
            let declarator = match self.peek() {
                Some(Token::Comma) | Some(Token::RParen) => Declarator::Abstract,
                _ => self.parse_declarator(true)?,
            };
            if let Some(parser) = self.context.parser()
                && let Some(name) = declarator.name()
            {
                parser.names.bind(name, false);
            }
            let attributes = self.parse_attributes()?;
            let vector_attributes = specifiers
                .attributes
                .iter()
                .chain(&attributes)
                .cloned()
                .collect::<Vec<_>>();
            let ty = std::mem::replace(&mut specifiers.ty, TypeSpecifier::Void);
            specifiers.ty = ty.with_type_attributes(&vector_attributes);
            parameters.push(span_tokens(
                ParameterDeclarationKind {
                    specifiers,
                    declarator,
                    attributes,
                },
                &self.tokens[parameter_start..self.pos],
                self.context,
            ));
            if self.matches(Token::RParen) {
                break;
            }
            self.expect(
                Token::Comma,
                DeclaratorError::ExpectedToken(Token::Comma, "between parameters"),
            )?;
        }
        if accepts_identifier_list && let Some(parser) = self.context.parser() {
            self.definition_bindings = parser
                .names
                .scopes
                .borrow()
                .last()
                .cloned()
                .unwrap_or_default();
        }
        Ok(ParameterList::Prototype {
            parameters,
            variadic,
        })
    }

    pub(super) fn expect(
        &mut self,
        token: Token,
        err: DeclaratorError,
    ) -> Result<(), DeclaratorError> {
        if self.consume(token) {
            Ok(())
        } else {
            Err(err)
        }
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

pub(crate) fn builtin_integer_typedef(name: &str) -> Option<IntegerType> {
    let signed = match name {
        "__int128_t" => true,
        "__uint128_t" => false,
        _ => return None,
    };
    Some(IntegerType::Ranked {
        rank: IntegerRank::Int128,
        signed,
    })
}

pub(crate) fn is_target_builtin_name(name: &str) -> bool {
    name == "__builtin_va_list"
}

fn tag_reference(
    kind: TagKind,
    name: Option<Span<String>>,
    fixed_type: Option<Box<TypeName>>,
) -> Result<TypeSpecifier, DeclaratorError> {
    let name = name.ok_or(DeclaratorError::ExpectedTagNameOrBrace)?;
    Ok(TypeSpecifier::Tag(TagSpecifier::Reference {
        kind,
        name,
        fixed_type,
    }))
}

#[derive(Default)]
struct TypeSpecifierSet {
    shape: SpecifierShape,
    payload: Option<TypeSpecifier>,
}

impl TypeSpecifierSet {
    fn finish(self) -> Result<TypeSpecifier, DeclaratorError> {
        let shape = self.shape;
        let signed = shape.sign != Some(false);
        let rank = match shape.width {
            SpecifierWidth::None => IntegerRank::Int,
            SpecifierWidth::Short => IntegerRank::Short,
            SpecifierWidth::Long => IntegerRank::Long,
            SpecifierWidth::LongLong => IntegerRank::LongLong,
        };
        let ranked = TypeSpecifier::Integer(IntegerType::Ranked { rank, signed });
        let ty = match shape.base {
            None if shape.sign.is_some() || shape.width != SpecifierWidth::None => ranked,
            None if shape.complex || shape.imaginary => {
                TypeSpecifier::Floating(FloatingType::Double)
            }
            None => return Err(DeclaratorError::ExpectedFractOrAccum),
            Some(BaseKind::Int) => ranked,
            Some(BaseKind::Char) => {
                TypeSpecifier::Integer(IntegerType::Char { signed: shape.sign })
            }
            Some(BaseKind::Int128) => TypeSpecifier::Integer(IntegerType::Ranked {
                rank: IntegerRank::Int128,
                signed,
            }),
            Some(BaseKind::Floating(FloatingType::Double))
                if shape.width == SpecifierWidth::Long =>
            {
                TypeSpecifier::Floating(FloatingType::LongDouble)
            }
            Some(BaseKind::Floating(float)) => TypeSpecifier::Floating(float),
            Some(BaseKind::FixedPoint(kind)) => TypeSpecifier::FixedPoint(FixedPointType {
                kind,
                rank: match shape.width {
                    SpecifierWidth::None => FixedPointRank::Default,
                    SpecifierWidth::Short => FixedPointRank::Short,
                    SpecifierWidth::Long => FixedPointRank::Long,
                    SpecifierWidth::LongLong => FixedPointRank::LongLong,
                },
                signed,
                saturated: shape.saturated,
            }),
            Some(BaseKind::BitInt) => match self.payload {
                Some(TypeSpecifier::Integer(IntegerType::BitInt { width, .. })) => {
                    TypeSpecifier::Integer(IntegerType::BitInt { width, signed })
                }
                _ => return Err(DeclaratorError::ExpectedDeclarationType),
            },
            Some(BaseKind::Other) => self
                .payload
                .ok_or(DeclaratorError::ExpectedDeclarationType)?,
        };
        Ok(if shape.complex {
            TypeSpecifier::Complex(Box::new(ty))
        } else if shape.imaginary {
            TypeSpecifier::Imaginary(Box::new(ty))
        } else {
            ty
        })
    }
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
enum SpecifierWidth {
    #[default]
    None,
    Short,
    Long,
    LongLong,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum BaseKind {
    Char,
    Int,
    Int128,
    Floating(FloatingType),
    FixedPoint(FixedPointKind),
    BitInt,
    Other,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Piece {
    Sign(bool),
    Short,
    Long,
    Int64,
    Complex,
    Imaginary,
    Saturated,
    Base(BaseKind),
}

#[derive(Debug, Clone, Copy, Default)]
struct SpecifierShape {
    base: Option<BaseKind>,
    sign: Option<bool>,
    width: SpecifierWidth,
    complex: bool,
    imaginary: bool,
    saturated: bool,
}

impl SpecifierShape {
    fn is_empty(self) -> bool {
        self.base.is_none()
            && self.sign.is_none()
            && self.width == SpecifierWidth::None
            && !self.complex
            && !self.imaginary
            && !self.saturated
    }

    fn add(self, piece: Piece) -> Option<Self> {
        let mut next = self;
        match piece {
            Piece::Sign(signed) => {
                if self.sign.is_some_and(|sign| sign != signed) {
                    return None;
                }
                next.sign = Some(signed);
            }
            Piece::Short => {
                if self.width != SpecifierWidth::None {
                    return None;
                }
                next.width = SpecifierWidth::Short;
            }
            Piece::Long => {
                next.width = match self.width {
                    SpecifierWidth::None => SpecifierWidth::Long,
                    SpecifierWidth::Long => SpecifierWidth::LongLong,
                    SpecifierWidth::Short | SpecifierWidth::LongLong => return None,
                };
            }
            // clang-cl reads `__int64` as a `long long` width, so `long __int64` and `int __int64` combine
            Piece::Int64 => {
                if self.width == SpecifierWidth::Short {
                    return None;
                }
                next.width = SpecifierWidth::LongLong;
            }
            Piece::Complex => next.complex = true,
            Piece::Imaginary => next.imaginary = true,
            Piece::Saturated => next.saturated = true,
            Piece::Base(base) => {
                if self.base.is_some() {
                    return None;
                }
                next.base = Some(base);
            }
        }
        next.is_valid().then_some(next)
    }

    fn is_valid(self) -> bool {
        if self.complex && self.imaginary {
            return false;
        }
        let Some(base) = self.base else {
            return true;
        };
        let no_width = self.width == SpecifierWidth::None;
        let integer = !self.imaginary && !self.saturated;
        match base {
            BaseKind::Int => integer,
            BaseKind::Char | BaseKind::Int128 | BaseKind::BitInt => integer && no_width,
            BaseKind::Floating(float) => {
                self.sign.is_none()
                    && !self.saturated
                    && (no_width
                        || float == FloatingType::Double && self.width == SpecifierWidth::Long)
                    && (!self.complex || has_complex_form(float))
                    && (!self.imaginary
                        || matches!(float, FloatingType::Float | FloatingType::Double))
            }
            BaseKind::FixedPoint(_) => !self.complex && !self.imaginary,
            BaseKind::Other => {
                self.sign.is_none()
                    && no_width
                    && !self.complex
                    && !self.imaginary
                    && !self.saturated
            }
        }
    }
}

fn specifier_piece(token: &Token, nothing_before: bool) -> Option<Piece> {
    let Token::Keyword(keyword) = token else {
        return (nothing_before && matches!(token, Token::Ident(_)))
            .then_some(Piece::Base(BaseKind::Other));
    };
    Some(match keyword {
        Keyword::Signed => Piece::Sign(true),
        Keyword::Unsigned => Piece::Sign(false),
        Keyword::Short => Piece::Short,
        Keyword::Long => Piece::Long,
        Keyword::Int64 => Piece::Int64,
        Keyword::Complex => Piece::Complex,
        Keyword::Imaginary => Piece::Imaginary,
        Keyword::Saturated => Piece::Saturated,
        Keyword::Char => Piece::Base(BaseKind::Char),
        Keyword::Int => Piece::Base(BaseKind::Int),
        Keyword::Int128 => Piece::Base(BaseKind::Int128),
        Keyword::BitInt => Piece::Base(BaseKind::BitInt),
        Keyword::Fract => Piece::Base(BaseKind::FixedPoint(FixedPointKind::Fract)),
        Keyword::Accum => Piece::Base(BaseKind::FixedPoint(FixedPointKind::Accum)),
        Keyword::Float => Piece::Base(BaseKind::Floating(FloatingType::Float)),
        Keyword::Double => Piece::Base(BaseKind::Floating(FloatingType::Double)),
        Keyword::Float16 => Piece::Base(BaseKind::Floating(FloatingType::Float16)),
        Keyword::Fp16 => Piece::Base(BaseKind::Floating(FloatingType::Fp16)),
        Keyword::BFloat16 => Piece::Base(BaseKind::Floating(FloatingType::BFloat16)),
        Keyword::Float32 => Piece::Base(BaseKind::Floating(FloatingType::Float32)),
        Keyword::Float64 => Piece::Base(BaseKind::Floating(FloatingType::Float64)),
        Keyword::Float32x => Piece::Base(BaseKind::Floating(FloatingType::Float32x)),
        Keyword::Float64x => Piece::Base(BaseKind::Floating(FloatingType::Float64x)),
        Keyword::Float128 => Piece::Base(BaseKind::Floating(FloatingType::Float128)),
        Keyword::Float128Ext => Piece::Base(BaseKind::Floating(FloatingType::Float128Ext)),
        Keyword::Float80 => Piece::Base(BaseKind::Floating(FloatingType::Float80)),
        Keyword::Decimal32 => Piece::Base(BaseKind::Floating(FloatingType::Decimal32)),
        Keyword::Decimal64 => Piece::Base(BaseKind::Floating(FloatingType::Decimal64)),
        Keyword::Decimal128 => Piece::Base(BaseKind::Floating(FloatingType::Decimal128)),
        Keyword::Bool
        | Keyword::Void
        | Keyword::Atomic
        | Keyword::Typeof
        | Keyword::TypeofUnqual
        | Keyword::Struct
        | Keyword::Union
        | Keyword::Enum => Piece::Base(BaseKind::Other),
        _ => return None,
    })
}

fn has_complex_form(float: FloatingType) -> bool {
    !matches!(
        float,
        FloatingType::Fp16
            | FloatingType::Decimal32
            | FloatingType::Decimal64
            | FloatingType::Decimal128
    )
}
