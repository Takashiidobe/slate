use super::attributes::{apply_vector_attributes, parse_attribute_groups};
use super::decl::specifiers_with_type;
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
        let (attributes, position) =
            parse_attribute_groups(self.tokens, self.pos, self.biggest_alignment)?;
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

    pub(super) fn parse_record_type(&mut self, kind: TagKind) -> Result<CType, DeclaratorError> {
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
    ) -> Result<CType, DeclaratorError> {
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
        Ok(CType::Tag(TagSpecifier::Definition(id)))
    }

    fn provenance_at(&self, index: usize) -> Provenance {
        self.statements.map_or_else(Provenance::default, |parser| {
            parser.token_provenance(&self.tokens[index])
        })
    }

    pub(super) fn parse_enum_type(&mut self) -> Result<CType, DeclaratorError> {
        let start = self.pos - 1;
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
        if self.peek() == Some(&Token::LBrace) {
            let body = TagBody::Enum(self.parse_enumerator_list()?);
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

    pub(super) fn parse_enumerator_list(&mut self) -> Result<Vec<Enumerator>, DeclaratorError> {
        self.pos += 1;
        let mut enumerators = Vec::new();
        loop {
            if self.matches(Token::RBrace) {
                break;
            }
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
            enumerators.push(Enumerator {
                name,
                value,
                provenance,
            });
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

    pub(super) fn parse_fixed_point(
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

    pub(super) fn fixed_point(
        &self,
        kind: FixedPointKind,
        saturated: bool,
        rank: FixedPointRank,
    ) -> CType {
        CType::FixedPoint(FixedPointType {
            kind,
            rank,
            saturated,
        })
    }

    pub(super) fn parse_bit_int(&mut self, is_unsigned: bool) -> CType {
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

    pub(super) fn parse_typeof(&mut self) -> Result<CType, DeclaratorError> {
        self.parse_typeof_operand().map(CType::TypeOf)
    }

    pub(super) fn parse_typeof_operand(&mut self) -> Result<TypeOfOperand, DeclaratorError> {
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
        let mut pointer_qualifiers = Vec::new();
        while self.matches(Token::Star) {
            pointer_qualifiers.push(self.take_qualifiers());
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

    fn opens_parameter_list(&self, pos: usize) -> bool {
        let pos = parse_attribute_groups(self.tokens, pos, self.biggest_alignment)
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

    pub(super) fn parse_parameters(&mut self) -> Result<(Vec<Parameter>, bool), DeclaratorError> {
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
            let mut attributes = self.parse_attributes()?;
            self.matches(Token::Keyword(Keyword::Register));
            let leading_qualifiers = self.take_qualifiers();
            self.matches(Token::Keyword(Keyword::Register));
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
            attributes.extend(self.parse_attributes()?);
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

pub fn apply_abstract_declarator(ty: CType, declarator: Declarator) -> CType {
    match declarator {
        Declarator::Abstract | Declarator::Name(_) => ty,
        Declarator::Grouped(inner) => apply_abstract_declarator(ty, *inner),
        Declarator::Attributed { inner, .. } => apply_abstract_declarator(ty, *inner),
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

fn tag_reference(kind: TagKind, name: Option<String>) -> Result<CType, DeclaratorError> {
    let name = name.ok_or(DeclaratorError::ExpectedTagNameOrBrace)?;
    Ok(CType::Tag(TagSpecifier::Reference { kind, name }))
}
