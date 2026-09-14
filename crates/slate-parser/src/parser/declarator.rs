use super::attributes::{apply_vector_attributes, parse_attribute_groups};
use super::decl::{bare_identifier_names, matching_paren, set_qualifier, specifiers_with_type};
use super::{Cursor, FALLBACK_BIGGEST_ALIGNMENT, Parser, coalesce_string_literals, span_tokens};
use crate::ast::*;
use crate::const_expr;
use crate::lexer::{Keyword, Token, TokenSpanExt};
use miette::Diagnostic;
use std::collections::HashSet;
use thiserror::Error;

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
    pub(super) typedef_names: &'a HashSet<String>,
    pub(super) biggest_alignment: i64,
    pub(super) statements: Option<&'a Parser>,
    pub(super) identifier_list: IdentifierList,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum IdentifierList {
    Rejected,
    Accepted,
    Parsed(Vec<String>),
}

impl<'a> DeclaratorParser<'a> {
    pub(crate) fn new(
        tokens: &'a [Span<Token>],
        pos: usize,
        typedef_names: &'a HashSet<String>,
    ) -> Self {
        Self::with_biggest_alignment(tokens, pos, typedef_names, FALLBACK_BIGGEST_ALIGNMENT)
    }

    pub(crate) fn with_biggest_alignment(
        tokens: &'a [Span<Token>],
        pos: usize,
        typedef_names: &'a HashSet<String>,
        biggest_alignment: i64,
    ) -> Self {
        Self {
            tokens,
            pos,
            typedef_names,
            biggest_alignment,
            statements: None,
            identifier_list: IdentifierList::Rejected,
        }
    }

    pub(crate) fn with_statements(mut self, statements: Option<&'a Parser>) -> Self {
        self.statements = statements;
        self
    }

    pub(crate) fn position(&self) -> usize {
        self.pos
    }

    pub(crate) fn parse_attributes(&mut self) -> Result<Vec<Attribute>, String> {
        let (attributes, position) = parse_attribute_groups(
            self.tokens,
            self.pos,
            self.biggest_alignment,
            self.typedef_names,
            self.statements,
        )?;
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

    pub(crate) fn parse_base_type(&mut self) -> Result<TypeSpecifier, DeclaratorError> {
        let Some(token) = self.peek().cloned() else {
            return Err(DeclaratorError::ExpectedDeclarationType);
        };
        self.pos += 1;
        Ok(match token {
            Token::Keyword(Keyword::Bool) => TypeSpecifier::Bool,
            Token::Keyword(Keyword::BFloat16) => TypeSpecifier::Floating(FloatingType::BFloat16),
            Token::Keyword(Keyword::Char) => {
                TypeSpecifier::Integer(IntegerType::Char { signed: None })
            }
            Token::Keyword(Keyword::Double) => {
                if self.matches(Token::Keyword(Keyword::Complex)) {
                    TypeSpecifier::Complex(Box::new(TypeSpecifier::Floating(FloatingType::Double)))
                } else if self.matches(Token::Keyword(Keyword::Imaginary)) {
                    TypeSpecifier::Imaginary(Box::new(TypeSpecifier::Floating(
                        FloatingType::Double,
                    )))
                } else {
                    TypeSpecifier::Floating(FloatingType::Double)
                }
            }
            Token::Keyword(Keyword::Float) => {
                if self.matches(Token::Keyword(Keyword::Complex)) {
                    TypeSpecifier::Complex(Box::new(TypeSpecifier::Floating(FloatingType::Float)))
                } else if self.matches(Token::Keyword(Keyword::Imaginary)) {
                    TypeSpecifier::Imaginary(Box::new(TypeSpecifier::Floating(FloatingType::Float)))
                } else {
                    TypeSpecifier::Floating(FloatingType::Float)
                }
            }
            Token::Keyword(Keyword::Float16) => TypeSpecifier::Floating(FloatingType::Float16),
            Token::Keyword(Keyword::Fp16) => TypeSpecifier::Floating(FloatingType::Fp16),
            Token::Keyword(Keyword::Float64x) => TypeSpecifier::Floating(FloatingType::Float64x),
            Token::Keyword(Keyword::Float128) => TypeSpecifier::Floating(FloatingType::Float128),
            Token::Keyword(Keyword::Float128Ext) => {
                TypeSpecifier::Floating(FloatingType::Float128Ext)
            }
            Token::Keyword(Keyword::Int) => TypeSpecifier::Integer(IntegerType::Ranked {
                rank: IntegerRank::Int,
                signed: true,
            }),
            Token::Keyword(Keyword::Int128) => TypeSpecifier::Integer(IntegerType::Ranked {
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
                    TypeSpecifier::Integer(IntegerType::Ranked {
                        rank: IntegerRank::LongLong,
                        signed,
                    })
                } else if self.matches(Token::Keyword(Keyword::Double)) {
                    if self.matches(Token::Keyword(Keyword::Complex)) {
                        TypeSpecifier::Complex(Box::new(TypeSpecifier::Floating(
                            FloatingType::LongDouble,
                        )))
                    } else if self.matches(Token::Keyword(Keyword::Imaginary)) {
                        TypeSpecifier::Imaginary(Box::new(TypeSpecifier::Floating(
                            FloatingType::LongDouble,
                        )))
                    } else {
                        TypeSpecifier::Floating(FloatingType::LongDouble)
                    }
                } else {
                    let signed = !self.matches(Token::Keyword(Keyword::Unsigned));
                    if signed {
                        self.matches(Token::Keyword(Keyword::Signed));
                    }
                    self.matches(Token::Keyword(Keyword::Int));
                    TypeSpecifier::Integer(IntegerType::Ranked {
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
                TypeSpecifier::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Short,
                    signed,
                })
            }
            Token::Keyword(Keyword::Signed) => match self.peek() {
                Some(Token::Keyword(Keyword::Char)) => {
                    self.pos += 1;
                    TypeSpecifier::Integer(IntegerType::Char { signed: Some(true) })
                }
                Some(Token::Keyword(Keyword::Short)) => {
                    self.pos += 1;
                    self.matches(Token::Keyword(Keyword::Int));
                    TypeSpecifier::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Short,
                        signed: true,
                    })
                }
                Some(Token::Keyword(Keyword::Long)) => {
                    self.pos += 1;
                    if self.matches(Token::Keyword(Keyword::Long)) {
                        self.matches(Token::Keyword(Keyword::Int));
                        TypeSpecifier::Integer(IntegerType::Ranked {
                            rank: IntegerRank::LongLong,
                            signed: true,
                        })
                    } else {
                        self.matches(Token::Keyword(Keyword::Int));
                        TypeSpecifier::Integer(IntegerType::Ranked {
                            rank: IntegerRank::Long,
                            signed: true,
                        })
                    }
                }
                Some(Token::Keyword(Keyword::Int128)) => {
                    self.pos += 1;
                    TypeSpecifier::Integer(IntegerType::Ranked {
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
                    TypeSpecifier::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Int,
                        signed: true,
                    })
                }
                _ => TypeSpecifier::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Int,
                    signed: true,
                }),
            },
            Token::Keyword(Keyword::Unsigned) => match self.peek() {
                Some(Token::Keyword(Keyword::Char)) => {
                    self.pos += 1;
                    TypeSpecifier::Integer(IntegerType::Char {
                        signed: Some(false),
                    })
                }
                Some(Token::Keyword(Keyword::Short)) => {
                    self.pos += 1;
                    self.matches(Token::Keyword(Keyword::Int));
                    TypeSpecifier::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Short,
                        signed: false,
                    })
                }
                Some(Token::Keyword(Keyword::Long)) => {
                    self.pos += 1;
                    if self.matches(Token::Keyword(Keyword::Long)) {
                        self.matches(Token::Keyword(Keyword::Int));
                        TypeSpecifier::Integer(IntegerType::Ranked {
                            rank: IntegerRank::LongLong,
                            signed: false,
                        })
                    } else {
                        self.matches(Token::Keyword(Keyword::Int));
                        TypeSpecifier::Integer(IntegerType::Ranked {
                            rank: IntegerRank::Long,
                            signed: false,
                        })
                    }
                }
                Some(Token::Keyword(Keyword::Int128)) => {
                    self.pos += 1;
                    TypeSpecifier::Integer(IntegerType::Ranked {
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
                    TypeSpecifier::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Int,
                        signed: false,
                    })
                }
                _ => TypeSpecifier::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Int,
                    signed: false,
                }),
            },
            Token::Keyword(Keyword::Void) => TypeSpecifier::Void,
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
                let ty = self.parse_type_name()?;
                self.expect(
                    Token::RParen,
                    DeclaratorError::ExpectedToken(Token::RParen, "after `_Atomic` type"),
                )?;
                TypeSpecifier::Atomic(Box::new(ty))
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
                    TypeSpecifier::Floating(FloatingType::Double)
                };
                TypeSpecifier::Complex(Box::new(element))
            }
            Token::Keyword(Keyword::Imaginary) => {
                TypeSpecifier::Imaginary(Box::new(TypeSpecifier::Floating(FloatingType::Double)))
            }
            Token::Keyword(Keyword::BitInt) => self.parse_bit_int(false),
            Token::Keyword(Keyword::Typeof) => self.parse_typeof()?,
            Token::Keyword(Keyword::TypeofUnqual) => {
                TypeSpecifier::TypeOfUnqual(self.parse_typeof_operand()?)
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
            Token::Ident(name) if name == "__int128_t" => {
                TypeSpecifier::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Int128,
                    signed: true,
                })
            }
            Token::Ident(name) if name == "__uint128_t" => {
                TypeSpecifier::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Int128,
                    signed: false,
                })
            }
            Token::Ident(name) if is_target_builtin_name(&name) => {
                TypeSpecifier::TargetBuiltin(name)
            }
            Token::Ident(name) => TypeSpecifier::Named(name),
            other => return Err(DeclaratorError::UnexpectedToken(other)),
        })
    }

    pub(super) fn parse_record_type(
        &mut self,
        kind: TagKind,
    ) -> Result<TypeSpecifier, DeclaratorError> {
        let start = self.pos - 1;
        let name = match self.peek() {
            Some(Token::Ident(name)) => {
                let name = name.clone();
                self.pos += 1;
                Some(name)
            }
            _ => None,
        };
        if self.peek() == Some(&Token::LBrace) {
            let body = TagBody::Record(self.parse_field_list()?);
            return self.define_tag(kind, name, body, start);
        }
        tag_reference(kind, name)
    }

    fn define_tag(
        &self,
        kind: TagKind,
        name: Option<String>,
        body: TagBody,
        start: usize,
    ) -> Result<TypeSpecifier, DeclaratorError> {
        let parser = self
            .statements
            .ok_or(DeclaratorError::TagDefinitionNotAllowed)?;
        let definition = TagDefinition {
            id: TagId(0),
            kind,
            name,
            attributes: Vec::new(),
            body,
            provenance: parser.token_provenance(&self.tokens[start]),
        };
        let id = parser.define_tag(span_tokens(definition, &self.tokens[start..self.pos]));
        Ok(TypeSpecifier::Tag(TagSpecifier::Definition(id)))
    }

    fn provenance_at(&self, index: usize) -> Provenance {
        self.statements.map_or_else(Provenance::default, |parser| {
            parser.token_provenance(&self.tokens[index])
        })
    }

    pub(super) fn parse_enum_type(&mut self) -> Result<TypeSpecifier, DeclaratorError> {
        let start = self.pos - 1;
        let name = match self.peek() {
            Some(Token::Ident(name)) => {
                let name = name.clone();
                self.pos += 1;
                Some(name)
            }
            _ => None,
        };
        let mut fixed_type = None;
        if self.peek() == Some(&Token::Colon) {
            let checkpoint = self.pos;
            self.pos += 1;
            match self.parse_type_name() {
                Ok(type_name) => fixed_type = Some(type_name),
                Err(_) => self.pos = checkpoint,
            }
        }
        if self.peek() == Some(&Token::LBrace) {
            let body = TagBody::Enum {
                fixed_type,
                enumerators: self.parse_enumerator_list()?,
            };
            return self.define_tag(TagKind::Enum, name, body, start);
        }
        tag_reference(TagKind::Enum, name)
    }

    pub(super) fn parse_field_list(&mut self) -> Result<Vec<SpannedFieldItem>, DeclaratorError> {
        self.pos += 1;
        let mut fields = Vec::new();
        while self.peek() != Some(&Token::RBrace) {
            if self.peek().is_none() {
                return Err(DeclaratorError::ExpectedToken(
                    Token::RBrace,
                    "in struct/union body",
                ));
            }
            let start = self.pos;
            while self.peek() == Some(&Token::Ident("__extension__".to_string())) {
                self.pos += 1;
            }
            let qualifiers = self.take_qualifiers();
            let mut specifiers = specifiers_with_type(self.parse_base_type()?);
            specifiers.qualifiers = qualifiers;
            let mut declarators = Vec::new();
            while !self.matches(Token::Semi) {
                let declarator = if self.peek() == Some(&Token::Colon) {
                    Declarator::Abstract
                } else {
                    self.parse_declarator(true)?
                };
                let bit_width = if self.matches(Token::Colon) {
                    let start = self.pos;
                    let (expression, end) = const_expr::Parser::parse_one(
                        self.tokens,
                        start,
                        self.typedef_names,
                        self.statements,
                    )
                    .map_err(|error| DeclaratorError::Other(error.to_string()))?;
                    self.pos = end;
                    Some(expression)
                } else {
                    None
                };
                let attributes = self.parse_attributes()?;
                declarators.push(FieldDeclarator {
                    declarator,
                    bit_width,
                    attributes,
                });
                if self.matches(Token::Comma) {
                    continue;
                }
                self.expect(
                    Token::Semi,
                    DeclaratorError::ExpectedToken(Token::Semi, "in struct/union field"),
                )?;
                break;
            }
            let provenance = self.provenance_at(start);
            fields.push(span_tokens(
                FieldItem::Field(FieldDecl {
                    specifiers,
                    declarators,
                    provenance,
                }),
                &self.tokens[start..self.pos],
            ));
        }
        self.pos += 1;
        Ok(fields)
    }

    pub(super) fn parse_enumerator_list(
        &mut self,
    ) -> Result<Vec<SpannedEnumItem>, DeclaratorError> {
        self.pos += 1;
        let mut items = Vec::new();
        loop {
            if self.matches(Token::RBrace) {
                break;
            }
            let start = self.pos;
            let Some(Token::Ident(name)) = self.peek().cloned() else {
                return Err(DeclaratorError::ExpectedEnumerator);
            };
            let provenance = self.provenance_at(self.pos);
            self.pos += 1;
            let value = if self.matches(Token::Equal) {
                let (value, end) = const_expr::Parser::parse_one(
                    self.tokens,
                    self.pos,
                    self.typedef_names,
                    self.statements,
                )
                .map_err(|error| error.to_string())?;
                self.pos = end;
                Some(value)
            } else {
                None
            };
            items.push(span_tokens(
                EnumItem::Enumerator(Enumerator {
                    name,
                    value,
                    provenance,
                }),
                &self.tokens[start..self.pos],
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

    pub(super) fn parse_fixed_point(
        &mut self,
        rank: FixedPointRank,
        saturated: bool,
    ) -> Result<TypeSpecifier, DeclaratorError> {
        let kind = match self.peek() {
            Some(Token::Keyword(Keyword::Fract)) => FixedPointKind::Fract,
            Some(Token::Keyword(Keyword::Accum)) => FixedPointKind::Accum,
            _ => return Err(DeclaratorError::ExpectedFractOrAccum),
        };
        self.pos += 1;
        Ok(self.fixed_point(kind, saturated, rank))
    }

    pub(super) fn fixed_point(
        &self,
        kind: FixedPointKind,
        saturated: bool,
        rank: FixedPointRank,
    ) -> TypeSpecifier {
        TypeSpecifier::FixedPoint(FixedPointType {
            kind,
            rank,
            saturated,
        })
    }

    pub(super) fn parse_bit_int(&mut self, is_unsigned: bool) -> TypeSpecifier {
        assert!(self.matches(Token::LParen), "expected `(` after _BitInt");
        let start = self.pos;
        while self.peek() != Some(&Token::RParen) {
            assert!(self.peek().is_some(), "expected `)` after _BitInt width");
            self.pos += 1;
        }
        let width = const_expr::Parser::parse(&self.tokens[start..self.pos])
            .expect("invalid _BitInt width expression");
        self.pos += 1;
        TypeSpecifier::Integer(IntegerType::BitInt {
            width,
            signed: !is_unsigned,
        })
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
        let tokens = coalesce_string_literals(tokens);
        let expression =
            const_expr::Parser::parse_expression(&tokens, self.typedef_names, self.statements)
                .map_err(|error| DeclaratorError::Other(error.to_string()))?;
        Ok(TypeOfOperand::Expression(expression))
    }

    pub(super) fn typeof_type_start(&self) -> bool {
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

    fn parse_designator_index(&mut self) -> Result<Expr, const_expr::ConstExprError> {
        let (index, end) = const_expr::Parser::parse_one(
            self.tokens,
            self.pos,
            self.typedef_names,
            self.statements,
        )?;
        self.pos = end;
        Ok(index)
    }

    pub(super) fn parse_initializer(
        &mut self,
        typedef_names: &HashSet<String>,
    ) -> Result<Initializer, const_expr::ConstExprError> {
        if self.matches(Token::LBrace) {
            let mut items = Vec::new();
            while !self.matches(Token::RBrace) {
                let mut designators = Vec::new();
                loop {
                    if self.matches(Token::LBracket) {
                        let index = self.parse_designator_index()?;
                        if self.matches(Token::Ellipsis) {
                            let end = self.parse_designator_index()?;
                            designators.push(Designator::ArrayRange { start: index, end });
                        } else {
                            designators.push(Designator::Array(index));
                        }
                        assert!(self.matches(Token::RBracket), "expected `]` in designator");
                    } else if self.matches(Token::Dot) {
                        let Some(Token::Ident(name)) = self.peek().cloned() else {
                            panic!("field designator must name a field")
                        };
                        let start = self.pos;
                        self.pos += 1;
                        designators.push(Designator::Field(span_tokens(
                            name,
                            &self.tokens[start..self.pos],
                        )));
                    } else if let Some(Token::Ident(name)) = self.peek().cloned()
                        && self.tokens.value_at(self.pos + 1) == Some(&Token::Colon)
                    {
                        let start = self.pos;
                        self.pos += 2;
                        designators.push(Designator::Field(span_tokens(
                            name,
                            &self.tokens[start..start + 1],
                        )));
                    } else {
                        break;
                    }
                }
                if !designators.is_empty() {
                    self.matches(Token::Equal);
                }
                items.push(InitializerItem {
                    designators,
                    value: self.parse_initializer(typedef_names)?,
                });
                if !self.matches(Token::Comma) {
                    assert!(
                        self.peek() == Some(&Token::RBrace),
                        "expected `,` in initializer"
                    );
                }
            }
            Ok(Initializer::List(items))
        } else {
            let (expression, end) = const_expr::Parser::parse_one(
                self.tokens,
                self.pos,
                typedef_names,
                self.statements,
            )?;
            self.pos = end;
            Ok(Initializer::Expr(expression))
        }
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
                Declarator::Name(name)
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

        for (qualifiers, attributes) in pointers {
            declarator = Declarator::Pointer {
                qualifiers,
                attributes,
                inner: Box::new(declarator),
            };
        }

        loop {
            declarator = match self.peek() {
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
                            assert!(self.peek().is_some(), "expected `]` in array declarator");
                            match self.peek() {
                                Some(Token::LParen | Token::LBrace | Token::LBracket) => depth += 1,
                                Some(Token::RParen | Token::RBrace | Token::RBracket) => depth -= 1,
                                _ => {}
                            }
                            self.pos += 1;
                        }
                        let bound_tokens = &self.tokens[start..self.pos];
                        let size = const_expr::Parser::parse_expression(
                            bound_tokens,
                            self.typedef_names,
                            self.statements,
                        )
                        .unwrap_or_else(|error| panic!("invalid array bound: {error}"));
                        ArraySize::Expression(size)
                    };
                    assert!(
                        self.matches(Token::RBracket),
                        "expected `]` in array declarator"
                    );
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
        while self.peek() == Some(&Token::Ident("__extension__".to_string())) {
            self.pos += 1;
        }
        specifiers.attributes = self.parse_attributes()?;
        let gnu_auto_type = self.matches(Token::Ident("__auto_type".into()));
        self.parse_specifier_keywords(&mut specifiers)?;
        let c23_auto_inference = self
            .statements
            .is_some_and(|parser| parser.standard().is_c23_or_later())
            && specifiers.storage == StorageClass::Auto
            && matches!(self.peek(), Some(Token::Ident(_)));
        if c23_auto_inference {
            specifiers.storage = StorageClass::None;
        }
        let implicit_int = implicit_int_function
            && matches!(
                (self.peek(), self.tokens.value_at(self.pos + 1)),
                (Some(Token::Ident(name)), Some(Token::LParen)) if !self.typedef_names.contains(name)
            );
        specifiers.ty = if gnu_auto_type || c23_auto_inference {
            TypeSpecifier::TargetBuiltin("__auto_type".into())
        } else if implicit_int {
            TypeSpecifier::Integer(IntegerType::Ranked {
                rank: IntegerRank::Int,
                signed: true,
            })
        } else {
            self.parse_base_type()?
        };
        self.parse_specifier_keywords(&mut specifiers)?;
        Ok(specifiers)
    }

    pub(crate) fn parse_specifier_keywords(
        &mut self,
        specifiers: &mut DeclarationSpecifiers,
    ) -> Result<(), DeclaratorError> {
        loop {
            while self.peek() == Some(&Token::Ident("__extension__".to_string())) {
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
            if specifiers.storage != StorageClass::None {
                return Err(DeclaratorError::MultipleStorageClasses);
            }
            specifiers.storage = storage;
            self.pos += 1;
        }
    }

    pub(crate) fn parse_type_name(&mut self) -> Result<TypeName, DeclaratorError> {
        let mut specifiers = self.parse_specifiers(false)?;
        let ty = std::mem::replace(&mut specifiers.ty, TypeSpecifier::Void);
        specifiers.ty = apply_vector_attributes(ty, &specifiers.attributes);
        let declarator = self.parse_declarator(true)?;
        Ok(TypeName {
            specifiers,
            declarator,
        })
    }

    fn parse_pointer_qualifiers(
        &mut self,
    ) -> Result<(Qualifiers, Vec<Attribute>), DeclaratorError> {
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

    fn opens_parameter_list(&self, pos: usize) -> bool {
        let pos = parse_attribute_groups(
            self.tokens,
            pos,
            self.biggest_alignment,
            self.typedef_names,
            self.statements,
        )
        .map_or(pos, |(_, after_attributes)| after_attributes);
        match self.tokens.value_at(pos) {
            Some(Token::RParen | Token::Ellipsis | Token::Keyword(Keyword::Register)) => true,
            Some(token) => const_expr::starts_type_name(token, self.typedef_names),
            None => false,
        }
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
            Keyword::Const | Keyword::Volatile | Keyword::Restrict | Keyword::Atomic
        ) {
            return None;
        }
        let keyword = *keyword;
        self.matches(Token::Keyword(keyword));
        Some(keyword)
    }

    pub(super) fn parse_parameters(&mut self) -> Result<ParameterList, DeclaratorError> {
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
                self.typedef_names,
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
            let mut specifiers = self.parse_specifiers(false)?;
            let declarator = match self.peek() {
                Some(Token::Comma) | Some(Token::RParen) => Declarator::Abstract,
                _ => self.parse_declarator(true)?,
            };
            let attributes = self.parse_attributes()?;
            let vector_attributes = specifiers
                .attributes
                .iter()
                .chain(&attributes)
                .cloned()
                .collect::<Vec<_>>();
            let ty = std::mem::replace(&mut specifiers.ty, TypeSpecifier::Void);
            specifiers.ty = apply_vector_attributes(ty, &vector_attributes);
            parameters.push(ParameterDeclaration {
                specifiers,
                declarator,
                declared_specifiers: None,
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

pub(crate) fn is_target_builtin_name(name: &str) -> bool {
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
            | "__builtin_va_list"
            | "char8_t"
            | "atomic_char8_t"
            | "nullptr_t"
    )
}

fn tag_reference(kind: TagKind, name: Option<String>) -> Result<TypeSpecifier, DeclaratorError> {
    let name = name.ok_or(DeclaratorError::ExpectedTagNameOrBrace)?;
    Ok(TypeSpecifier::Tag(TagSpecifier::Reference { kind, name }))
}
