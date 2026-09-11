use crate::ast::{CType, Declarator, Designator, Expr, Initializer, InitializerItem};
use crate::lexer::{Keyword, Token};
use crate::parser::DeclaratorParser;
use miette::Diagnostic;
use std::collections::HashSet;
use thiserror::Error;

#[derive(Debug, Clone, PartialEq)]
pub enum ConstExpr {
    Integer(i64),
    Identifier(String),
    SizeOf(Box<Self>),
    AlignOf {
        ty: Box<CType>,
        declarator: Declarator,
    },
    Unary {
        op: UnaryOp,
        value: Box<Self>,
    },
    Binary {
        op: BinaryOp,
        left: Box<Self>,
        right: Box<Self>,
    },
    Assign {
        op: AssignOp,
        target: Box<Self>,
        value: Box<Self>,
    },
    Ternary {
        condition: Box<Self>,
        then_value: Box<Self>,
        else_value: Box<Self>,
    },
    Comma(Box<Self>, Box<Self>),
    Call {
        callee: Box<Self>,
        arguments: Vec<Self>,
    },
    Member {
        base: Box<Self>,
        field: String,
    },
    Arrow {
        base: Box<Self>,
        field: String,
    },
    Index {
        base: Box<Self>,
        index: Box<Self>,
    },
    PostIncrement(Box<Self>),
    PostDecrement(Box<Self>),
    PreIncrement(Box<Self>),
    PreDecrement(Box<Self>),
    AddrOf(Box<Self>),
    Deref(Box<Self>),
    Cast {
        ty: Box<CType>,
        declarator: Declarator,
        value: Box<Self>,
    },
    CompoundLiteral {
        ty: Box<CType>,
        declarator: Declarator,
        initializer: Vec<InitializerItem>,
    },
}

impl std::fmt::Display for ConstExpr {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Integer(value) => write!(formatter, "{value}"),
            Self::Identifier(value) => formatter.write_str(value),
            Self::SizeOf(value) => write!(formatter, "sizeof({value})"),
            Self::AlignOf { .. } => write!(formatter, "_Alignof(...)"),
            Self::Unary { op, value } => write!(formatter, "{}{}", <&str>::from(*op), value),
            Self::Binary { op, left, right } => {
                write!(formatter, "({left} {} {right})", <&str>::from(*op))
            }
            Self::Assign { op, target, value } => {
                write!(formatter, "({target} {} {value})", <&str>::from(*op))
            }
            Self::Ternary {
                condition,
                then_value,
                else_value,
            } => write!(formatter, "({condition} ? {then_value} : {else_value})"),
            Self::Comma(left, right) => write!(formatter, "({left}, {right})"),
            Self::Call { callee, arguments } => {
                write!(formatter, "{callee}(")?;
                for (index, argument) in arguments.iter().enumerate() {
                    if index > 0 {
                        write!(formatter, ", ")?;
                    }
                    write!(formatter, "{argument}")?;
                }
                write!(formatter, ")")
            }
            Self::Member { base, field } => write!(formatter, "{base}.{field}"),
            Self::Arrow { base, field } => write!(formatter, "{base}->{field}"),
            Self::Index { base, index } => write!(formatter, "{base}[{index}]"),
            Self::PostIncrement(value) => write!(formatter, "{value}++"),
            Self::PostDecrement(value) => write!(formatter, "{value}--"),
            Self::PreIncrement(value) => write!(formatter, "++{value}"),
            Self::PreDecrement(value) => write!(formatter, "--{value}"),
            Self::AddrOf(value) => write!(formatter, "&{value}"),
            Self::Deref(value) => write!(formatter, "*{value}"),
            Self::Cast { value, .. } => write!(formatter, "(cast){value}"),
            Self::CompoundLiteral { .. } => write!(formatter, "(compound literal)"),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum UnaryOp {
    Plus,
    Minus,
    BitNot,
    Not,
}

impl From<UnaryOp> for &'static str {
    fn from(op: UnaryOp) -> Self {
        match op {
            UnaryOp::Plus => "+",
            UnaryOp::Minus => "-",
            UnaryOp::BitNot => "~",
            UnaryOp::Not => "!",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum BinaryOp {
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Equal,
    NotEqual,
    BitAnd,
    BitXor,
    BitOr,
    And,
    Or,
    ShiftLeft,
    ShiftRight,
}

impl From<BinaryOp> for &'static str {
    fn from(op: BinaryOp) -> Self {
        match op {
            BinaryOp::Add => "+",
            BinaryOp::Sub => "-",
            BinaryOp::Mul => "*",
            BinaryOp::Div => "/",
            BinaryOp::Rem => "%",
            BinaryOp::Less => "<",
            BinaryOp::LessEqual => "<=",
            BinaryOp::Greater => ">",
            BinaryOp::GreaterEqual => ">=",
            BinaryOp::Equal => "==",
            BinaryOp::NotEqual => "!=",
            BinaryOp::BitAnd => "&",
            BinaryOp::BitXor => "^",
            BinaryOp::BitOr => "|",
            BinaryOp::And => "&&",
            BinaryOp::Or => "||",
            BinaryOp::ShiftLeft => "<<",
            BinaryOp::ShiftRight => ">>",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AssignOp {
    Assign,
    AddAssign,
    SubAssign,
    MulAssign,
    DivAssign,
    RemAssign,
    BitAndAssign,
    BitOrAssign,
    BitXorAssign,
    ShiftLeftAssign,
    ShiftRightAssign,
}

impl From<AssignOp> for &'static str {
    fn from(op: AssignOp) -> Self {
        match op {
            AssignOp::Assign => "=",
            AssignOp::AddAssign => "+=",
            AssignOp::SubAssign => "-=",
            AssignOp::MulAssign => "*=",
            AssignOp::DivAssign => "/=",
            AssignOp::RemAssign => "%=",
            AssignOp::BitAndAssign => "&=",
            AssignOp::BitOrAssign => "|=",
            AssignOp::BitXorAssign => "^=",
            AssignOp::ShiftLeftAssign => "<<=",
            AssignOp::ShiftRightAssign => ">>=",
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq, Error, Diagnostic)]
pub enum ConstExprError {
    #[error("unexpected tokens after expression")]
    UnexpectedTokens,
    #[error("unsupported identifier `{0}`")]
    UnsupportedIdentifier(String),
    #[error("sizeof is not supported here")]
    UnsupportedSizeOf,
    #[error("_Alignof is not supported here")]
    UnsupportedAlignOf,
    #[error("integer overflow")]
    IntegerOverflow,
    #[error("invalid integer constant expression")]
    InvalidIntegerConstant,
    #[error("expected `)`")]
    ExpectedRParen,
    #[error("expected `]`")]
    ExpectedRBracket,
    #[error("expected `:`")]
    ExpectedColon,
    #[error("expected `(`")]
    ExpectedLParen,
    #[error("expected identifier")]
    ExpectedIdentifier,
    #[error("expected type name")]
    ExpectedTypeName,
    #[error("expected `=`")]
    ExpectedEqual,
    #[error("expected `}}`")]
    ExpectedRBrace,
    #[error("unexpected token `{0:?}`")]
    UnexpectedToken(Token),
    #[error("expected integer expression")]
    ExpectedIntegerExpression,
    #[error("call to `{0}` is not a constant expression")]
    UnsupportedCall(String),
    #[error("{0} is not a constant expression")]
    NotConstant(&'static str),
}

pub struct Parser {
    tokens: Vec<Token>,
    position: usize,
    typedef_names: HashSet<String>,
}

impl Parser {
    pub fn parse(tokens: &[Token]) -> Result<ConstExpr, ConstExprError> {
        let mut parser = Self::new(tokens, &HashSet::new());
        let expression = parser.parse_conditional()?;
        if parser.peek().is_some() {
            return Err(ConstExprError::UnexpectedTokens);
        }
        Ok(expression)
    }

    pub fn parse_expression(
        tokens: &[Token],
        typedef_names: &HashSet<String>,
    ) -> Result<ConstExpr, ConstExprError> {
        let mut parser = Self::new(tokens, typedef_names);
        let expression = parser.parse_comma()?;
        if parser.peek().is_some() {
            return Err(ConstExprError::UnexpectedTokens);
        }
        Ok(expression)
    }

    pub(crate) fn parse_one(
        tokens: &[Token],
        start: usize,
        typedef_names: &HashSet<String>,
    ) -> Result<(ConstExpr, usize), ConstExprError> {
        let mut parser = Self::new(&tokens[start..], typedef_names);
        let expression = parser.parse_assignment()?;
        Ok((expression, start + parser.position))
    }

    pub fn evaluate(tokens: &[Token]) -> Result<i64, ConstExprError> {
        Self::evaluate_expr(&Self::parse(tokens)?)
    }

    fn evaluate_expr(expression: &ConstExpr) -> Result<i64, ConstExprError> {
        match expression {
            ConstExpr::Integer(value) => Ok(*value),
            ConstExpr::Identifier(name) => Err(ConstExprError::UnsupportedIdentifier(name.clone())),
            ConstExpr::SizeOf(_) => Err(ConstExprError::UnsupportedSizeOf),
            ConstExpr::AlignOf { .. } => Err(ConstExprError::UnsupportedAlignOf),
            ConstExpr::Call { callee, .. } => {
                Err(ConstExprError::UnsupportedCall(callee.to_string()))
            }
            ConstExpr::Cast { value, .. } => Self::evaluate_expr(value),
            ConstExpr::Unary { op, value } => {
                let value = Self::evaluate_expr(value)?;
                match op {
                    UnaryOp::Plus => Ok(value),
                    UnaryOp::Minus => value.checked_neg().ok_or(ConstExprError::IntegerOverflow),
                    UnaryOp::BitNot => Ok(!value),
                    UnaryOp::Not => Ok((value == 0) as i64),
                }
            }
            ConstExpr::Binary { op, left, right } => {
                let left = Self::evaluate_expr(left)?;
                if *op == BinaryOp::And && left == 0 || *op == BinaryOp::Or && left != 0 {
                    return Ok((*op == BinaryOp::Or) as i64);
                }
                let right = Self::evaluate_expr(right)?;
                let value = match op {
                    BinaryOp::Add => left.checked_add(right),
                    BinaryOp::Sub => left.checked_sub(right),
                    BinaryOp::Mul => left.checked_mul(right),
                    BinaryOp::Div => left.checked_div(right),
                    BinaryOp::Rem => left.checked_rem(right),
                    BinaryOp::Less => Some((left < right) as i64),
                    BinaryOp::LessEqual => Some((left <= right) as i64),
                    BinaryOp::Greater => Some((left > right) as i64),
                    BinaryOp::GreaterEqual => Some((left >= right) as i64),
                    BinaryOp::Equal => Some((left == right) as i64),
                    BinaryOp::NotEqual => Some((left != right) as i64),
                    BinaryOp::BitAnd => Some(left & right),
                    BinaryOp::BitXor => Some(left ^ right),
                    BinaryOp::BitOr => Some(left | right),
                    BinaryOp::And => Some(((left != 0) && (right != 0)) as i64),
                    BinaryOp::Or => Some(((left != 0) || (right != 0)) as i64),
                    BinaryOp::ShiftLeft => left.checked_shl(right as u32),
                    BinaryOp::ShiftRight => left.checked_shr(right as u32),
                };
                value.ok_or(ConstExprError::InvalidIntegerConstant)
            }
            ConstExpr::Ternary {
                condition,
                then_value,
                else_value,
            } => {
                if Self::evaluate_expr(condition)? != 0 {
                    Self::evaluate_expr(then_value)
                } else {
                    Self::evaluate_expr(else_value)
                }
            }
            ConstExpr::Comma(left, right) => {
                Self::evaluate_expr(left)?;
                Self::evaluate_expr(right)
            }
            ConstExpr::Assign { .. } => Err(ConstExprError::NotConstant("assignment")),
            ConstExpr::Member { .. } => Err(ConstExprError::NotConstant("member access")),
            ConstExpr::Arrow { .. } => Err(ConstExprError::NotConstant("member access")),
            ConstExpr::Index { .. } => Err(ConstExprError::NotConstant("array subscript")),
            ConstExpr::PostIncrement(_) => Err(ConstExprError::NotConstant("increment")),
            ConstExpr::PostDecrement(_) => Err(ConstExprError::NotConstant("decrement")),
            ConstExpr::PreIncrement(_) => Err(ConstExprError::NotConstant("increment")),
            ConstExpr::PreDecrement(_) => Err(ConstExprError::NotConstant("decrement")),
            ConstExpr::AddrOf(_) => Err(ConstExprError::NotConstant("address-of")),
            ConstExpr::Deref(_) => Err(ConstExprError::NotConstant("dereference")),
            ConstExpr::CompoundLiteral { .. } => {
                Err(ConstExprError::NotConstant("compound literal"))
            }
        }
    }

    fn new(tokens: &[Token], typedef_names: &HashSet<String>) -> Self {
        Self {
            tokens: tokens.to_vec(),
            position: 0,
            typedef_names: typedef_names.clone(),
        }
    }

    fn peek(&self) -> Option<&Token> {
        self.tokens.get(self.position)
    }

    fn take(&mut self) -> Option<Token> {
        let token = self.tokens.get(self.position).cloned();
        self.position += token.is_some() as usize;
        token
    }

    fn parse_comma(&mut self) -> Result<ConstExpr, ConstExprError> {
        let mut expression = self.parse_assignment()?;
        while self.peek() == Some(&Token::Comma) {
            self.take();
            let right = self.parse_assignment()?;
            expression = ConstExpr::Comma(Box::new(expression), Box::new(right));
        }
        Ok(expression)
    }

    fn parse_assignment(&mut self) -> Result<ConstExpr, ConstExprError> {
        let target = self.parse_conditional()?;
        if let Some(op) = self.assignment_operator() {
            self.take();
            let value = self.parse_assignment()?;
            return Ok(ConstExpr::Assign {
                op,
                target: Box::new(target),
                value: Box::new(value),
            });
        }
        Ok(target)
    }

    fn assignment_operator(&self) -> Option<AssignOp> {
        Some(match self.peek()? {
            Token::Equal => AssignOp::Assign,
            Token::PlusEqual => AssignOp::AddAssign,
            Token::MinusEqual => AssignOp::SubAssign,
            Token::StarEqual => AssignOp::MulAssign,
            Token::SlashEqual => AssignOp::DivAssign,
            Token::PercentEqual => AssignOp::RemAssign,
            Token::AmpEqual => AssignOp::BitAndAssign,
            Token::PipeEqual => AssignOp::BitOrAssign,
            Token::CaretEqual => AssignOp::BitXorAssign,
            Token::ShiftLeftEqual => AssignOp::ShiftLeftAssign,
            Token::ShiftRightEqual => AssignOp::ShiftRightAssign,
            _ => return None,
        })
    }

    fn parse_conditional(&mut self) -> Result<ConstExpr, ConstExprError> {
        let condition = self.parse_binary(0)?;
        if self.peek() == Some(&Token::Question) {
            self.take();
            let then_value = self.parse_comma()?;
            if self.take() != Some(Token::Colon) {
                return Err(ConstExprError::ExpectedColon);
            }
            let else_value = self.parse_conditional()?;
            return Ok(ConstExpr::Ternary {
                condition: Box::new(condition),
                then_value: Box::new(then_value),
                else_value: Box::new(else_value),
            });
        }
        Ok(condition)
    }

    fn parse_binary(&mut self, minimum_precedence: u8) -> Result<ConstExpr, ConstExprError> {
        let mut left = self.parse_cast()?;
        while let Some((op, precedence)) = self.binary_operator() {
            if precedence < minimum_precedence {
                break;
            }
            self.take();
            let right = self.parse_binary(precedence + 1)?;
            left = ConstExpr::Binary {
                op,
                left: Box::new(left),
                right: Box::new(right),
            };
        }
        Ok(left)
    }

    fn parse_cast(&mut self) -> Result<ConstExpr, ConstExprError> {
        if self.peek() == Some(&Token::LParen)
            && let Some(next) = self.tokens.get(self.position + 1)
            && starts_type_name(next, &self.typedef_names)
            && let Some((ty, declarator, end)) = self.try_parse_type_name(self.position + 1)
            && self.tokens.get(end) == Some(&Token::RParen)
            && self.tokens.get(end + 1) != Some(&Token::LBrace)
        {
            self.position = end + 1;
            let value = self.parse_cast()?;
            return Ok(ConstExpr::Cast {
                ty,
                declarator,
                value: Box::new(value),
            });
        }
        self.parse_unary()
    }

    fn try_parse_compound_literal(&mut self) -> Option<Result<ConstExpr, ConstExprError>> {
        if self.peek() != Some(&Token::LParen) {
            return None;
        }
        let next = self.tokens.get(self.position + 1)?;
        if !starts_type_name(next, &self.typedef_names) {
            return None;
        }
        let (ty, declarator, end) = self.try_parse_type_name(self.position + 1)?;
        if self.tokens.get(end) != Some(&Token::RParen)
            || self.tokens.get(end + 1) != Some(&Token::LBrace)
        {
            return None;
        }
        self.position = end + 1;
        Some(
            self.parse_initializer_list()
                .map(|initializer| ConstExpr::CompoundLiteral {
                    ty,
                    declarator,
                    initializer,
                }),
        )
    }

    fn parse_initializer_list(&mut self) -> Result<Vec<InitializerItem>, ConstExprError> {
        if self.take() != Some(Token::LBrace) {
            return Err(ConstExprError::UnexpectedToken(
                self.tokens[self.position - 1].clone(),
            ));
        }
        let mut items = Vec::new();
        while self.peek() != Some(&Token::RBrace) {
            let mut designators = Vec::new();
            loop {
                if self.peek() == Some(&Token::LBracket) {
                    self.take();
                    let Some(Token::IntLit(index)) = self.take() else {
                        return Err(ConstExprError::ExpectedIntegerExpression);
                    };
                    if self.take() != Some(Token::RBracket) {
                        return Err(ConstExprError::ExpectedRBracket);
                    }
                    designators.push(Designator::Array(index));
                } else if self.peek() == Some(&Token::Dot) {
                    self.take();
                    designators.push(Designator::Field(self.expect_field_name()?));
                } else {
                    break;
                }
            }
            if !designators.is_empty() && self.take() != Some(Token::Equal) {
                return Err(ConstExprError::ExpectedEqual);
            }
            let value = self.parse_initializer_value()?;
            items.push(InitializerItem { designators, value });
            if self.peek() == Some(&Token::Comma) {
                self.take();
            } else {
                break;
            }
        }
        if self.take() != Some(Token::RBrace) {
            return Err(ConstExprError::ExpectedRBrace);
        }
        Ok(items)
    }

    fn parse_initializer_value(&mut self) -> Result<Initializer, ConstExprError> {
        if self.peek() == Some(&Token::LBrace) {
            Ok(Initializer::List(self.parse_initializer_list()?))
        } else if let Some(Token::StringLit(value)) = self.peek().cloned() {
            self.take();
            Ok(Initializer::Expr(Expr::StringLit(value)))
        } else {
            let expression = self.parse_assignment()?;
            Ok(Initializer::Expr(Expr::Const(Box::new(expression))))
        }
    }

    fn try_parse_type_name(&self, start: usize) -> Option<(Box<CType>, Declarator, usize)> {
        let mut declarator_parser = DeclaratorParser::new(&self.tokens, start);
        let ty = declarator_parser.parse_base_type().ok()?;
        let declarator = declarator_parser.parse_declarator(true).ok()?;
        Some((Box::new(ty), declarator, declarator_parser.position()))
    }

    fn parse_unary(&mut self) -> Result<ConstExpr, ConstExprError> {
        if self.peek() == Some(&Token::Sizeof)
            && self.tokens.get(self.position + 1) == Some(&Token::LParen)
        {
            self.take();
            self.take();
            let value = self.parse_comma()?;
            if self.take() != Some(Token::RParen) {
                return Err(ConstExprError::ExpectedRParen);
            }
            return Ok(ConstExpr::SizeOf(Box::new(value)));
        }
        if self.peek() == Some(&Token::Alignof) {
            self.take();
            if self.take() != Some(Token::LParen) {
                return Err(ConstExprError::ExpectedLParen);
            }
            let (ty, declarator, end) = self
                .try_parse_type_name(self.position)
                .ok_or(ConstExprError::ExpectedTypeName)?;
            if self.tokens.get(end) != Some(&Token::RParen) {
                return Err(ConstExprError::ExpectedRParen);
            }
            self.position = end + 1;
            return Ok(ConstExpr::AlignOf { ty, declarator });
        }
        match self.peek() {
            Some(Token::Plus) => {
                self.take();
                Ok(ConstExpr::Unary {
                    op: UnaryOp::Plus,
                    value: Box::new(self.parse_cast()?),
                })
            }
            Some(Token::Minus) => {
                self.take();
                Ok(ConstExpr::Unary {
                    op: UnaryOp::Minus,
                    value: Box::new(self.parse_cast()?),
                })
            }
            Some(Token::Tilde) => {
                self.take();
                Ok(ConstExpr::Unary {
                    op: UnaryOp::BitNot,
                    value: Box::new(self.parse_cast()?),
                })
            }
            Some(Token::Bang) => {
                self.take();
                Ok(ConstExpr::Unary {
                    op: UnaryOp::Not,
                    value: Box::new(self.parse_cast()?),
                })
            }
            Some(Token::PlusPlus) => {
                self.take();
                Ok(ConstExpr::PreIncrement(Box::new(self.parse_unary()?)))
            }
            Some(Token::MinusMinus) => {
                self.take();
                Ok(ConstExpr::PreDecrement(Box::new(self.parse_unary()?)))
            }
            Some(Token::Amp) => {
                self.take();
                Ok(ConstExpr::AddrOf(Box::new(self.parse_cast()?)))
            }
            Some(Token::Star) => {
                self.take();
                Ok(ConstExpr::Deref(Box::new(self.parse_cast()?)))
            }
            _ => self.parse_postfix(),
        }
    }

    fn parse_postfix(&mut self) -> Result<ConstExpr, ConstExprError> {
        let mut expression = match self.try_parse_compound_literal() {
            Some(result) => result?,
            None => self.parse_primary()?,
        };
        loop {
            expression = match self.peek() {
                Some(Token::LParen) => {
                    self.take();
                    let mut arguments = Vec::new();
                    if self.peek() != Some(&Token::RParen) {
                        loop {
                            arguments.push(self.parse_assignment()?);
                            if self.peek() != Some(&Token::Comma) {
                                break;
                            }
                            self.take();
                        }
                    }
                    if self.take() != Some(Token::RParen) {
                        return Err(ConstExprError::ExpectedRParen);
                    }
                    ConstExpr::Call {
                        callee: Box::new(expression),
                        arguments,
                    }
                }
                Some(Token::Dot) => {
                    self.take();
                    ConstExpr::Member {
                        base: Box::new(expression),
                        field: self.expect_field_name()?,
                    }
                }
                Some(Token::Arrow) => {
                    self.take();
                    ConstExpr::Arrow {
                        base: Box::new(expression),
                        field: self.expect_field_name()?,
                    }
                }
                Some(Token::LBracket) => {
                    self.take();
                    let index = self.parse_comma()?;
                    if self.take() != Some(Token::RBracket) {
                        return Err(ConstExprError::ExpectedRBracket);
                    }
                    ConstExpr::Index {
                        base: Box::new(expression),
                        index: Box::new(index),
                    }
                }
                Some(Token::PlusPlus) => {
                    self.take();
                    ConstExpr::PostIncrement(Box::new(expression))
                }
                Some(Token::MinusMinus) => {
                    self.take();
                    ConstExpr::PostDecrement(Box::new(expression))
                }
                _ => break,
            };
        }
        Ok(expression)
    }

    fn expect_field_name(&mut self) -> Result<String, ConstExprError> {
        match self.take() {
            Some(Token::Ident(name)) => Ok(name),
            Some(token) => Err(ConstExprError::UnexpectedToken(token)),
            None => Err(ConstExprError::ExpectedIdentifier),
        }
    }

    fn parse_primary(&mut self) -> Result<ConstExpr, ConstExprError> {
        if self.take() == Some(Token::LParen) {
            let expression = self.parse_comma()?;
            if self.take() != Some(Token::RParen) {
                return Err(ConstExprError::ExpectedRParen);
            }
            return Ok(expression);
        }
        match self.tokens.get(self.position.saturating_sub(1)) {
            Some(Token::IntLit(value)) => Ok(ConstExpr::Integer(*value)),
            Some(
                Token::CharLit(raw)
                | Token::Utf8CharLit(raw)
                | Token::Utf16CharLit(raw)
                | Token::Utf32CharLit(raw)
                | Token::WideCharLit(raw),
            ) => Ok(ConstExpr::Integer(crate::lexer::decode_char_literal(raw))),
            Some(Token::Ident(value)) => Ok(ConstExpr::Identifier(value.clone())),
            Some(Token::Keyword(keyword)) => {
                Ok(ConstExpr::Identifier(<&str>::from(*keyword).into()))
            }
            Some(token) => Err(ConstExprError::UnexpectedToken(token.clone())),
            None => Err(ConstExprError::ExpectedIntegerExpression),
        }
    }

    fn binary_operator(&self) -> Option<(BinaryOp, u8)> {
        Some(match self.peek()? {
            Token::Star => (BinaryOp::Mul, 6),
            Token::Slash => (BinaryOp::Div, 6),
            Token::Percent => (BinaryOp::Rem, 6),
            Token::Plus => (BinaryOp::Add, 5),
            Token::Minus => (BinaryOp::Sub, 5),
            Token::Less => (BinaryOp::Less, 4),
            Token::LessEqual => (BinaryOp::LessEqual, 4),
            Token::Greater => (BinaryOp::Greater, 4),
            Token::GreaterEqual => (BinaryOp::GreaterEqual, 4),
            Token::EqualEqual => (BinaryOp::Equal, 3),
            Token::NotEqual => (BinaryOp::NotEqual, 3),
            Token::Amp => (BinaryOp::BitAnd, 2),
            Token::Caret => (BinaryOp::BitXor, 1),
            Token::Pipe => (BinaryOp::BitOr, 1),
            Token::AndAnd => (BinaryOp::And, 1),
            Token::OrOr => (BinaryOp::Or, 0),
            Token::ShiftLeft => (BinaryOp::ShiftLeft, 4),
            Token::ShiftRight => (BinaryOp::ShiftRight, 4),
            _ => return None,
        })
    }
}

pub(crate) fn starts_type_name(token: &Token, typedef_names: &HashSet<String>) -> bool {
    match token {
        Token::Keyword(keyword) => matches!(
            keyword,
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
                | Keyword::Long
                | Keyword::Short
                | Keyword::Signed
                | Keyword::Unsigned
                | Keyword::Void
                | Keyword::Complex
                | Keyword::Struct
                | Keyword::Union
                | Keyword::Enum
                | Keyword::Const
                | Keyword::Volatile
                | Keyword::Restrict
                | Keyword::Atomic
                | Keyword::Int128
                | Keyword::BitInt
                | Keyword::Accum
                | Keyword::Fract
                | Keyword::Saturated
                | Keyword::Typeof
        ),
        Token::Ident(name) => typedef_names.contains(name),
        _ => false,
    }
}
