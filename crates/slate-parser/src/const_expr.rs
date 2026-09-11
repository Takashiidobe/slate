use crate::lexer::Token;
use miette::Diagnostic;
use thiserror::Error;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum ConstExpr {
    Integer(i64),
    Identifier(String),
    SizeOf(Box<Self>),
    Unary {
        op: UnaryOp,
        value: Box<Self>,
    },
    Binary {
        op: BinaryOp,
        left: Box<Self>,
        right: Box<Self>,
    },
}

impl std::fmt::Display for ConstExpr {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Integer(value) => write!(formatter, "{value}"),
            Self::Identifier(value) => formatter.write_str(value),
            Self::SizeOf(value) => write!(formatter, "sizeof({value})"),
            Self::Unary { op, value } => write!(formatter, "{}{}", <&str>::from(*op), value),
            Self::Binary { op, left, right } => {
                write!(formatter, "({left} {} {right})", <&str>::from(*op))
            }
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

#[derive(Debug, Clone, PartialEq, Eq, Error, Diagnostic)]
pub enum ConstExprError {
    #[error("unexpected tokens after expression")]
    UnexpectedTokens,
    #[error("unsupported identifier `{0}`")]
    UnsupportedIdentifier(String),
    #[error("sizeof is not supported here")]
    UnsupportedSizeOf,
    #[error("integer overflow")]
    IntegerOverflow,
    #[error("invalid integer constant expression")]
    InvalidIntegerConstant,
    #[error("expected `)`")]
    ExpectedRParen,
    #[error("unexpected token `{0:?}`")]
    UnexpectedToken(Token),
    #[error("expected integer expression")]
    ExpectedIntegerExpression,
}

pub struct Parser {
    tokens: Vec<Token>,
    position: usize,
}

impl Parser {
    pub fn parse(tokens: &[Token]) -> Result<ConstExpr, ConstExprError> {
        let mut parser = Self::new(tokens);
        let expression = parser.parse_binary(0)?;
        if parser.peek().is_some() {
            return Err(ConstExprError::UnexpectedTokens);
        }
        Ok(expression)
    }

    pub fn evaluate(tokens: &[Token]) -> Result<i64, ConstExprError> {
        Self::evaluate_expr(&Self::parse(tokens)?)
    }

    fn evaluate_expr(expression: &ConstExpr) -> Result<i64, ConstExprError> {
        match expression {
            ConstExpr::Integer(value) => Ok(*value),
            ConstExpr::Identifier(name) => Err(ConstExprError::UnsupportedIdentifier(name.clone())),
            ConstExpr::SizeOf(_) => Err(ConstExprError::UnsupportedSizeOf),
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
        }
    }

    fn new(tokens: &[Token]) -> Self {
        Self {
            tokens: tokens.to_vec(),
            position: 0,
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

    fn parse_binary(&mut self, minimum_precedence: u8) -> Result<ConstExpr, ConstExprError> {
        let mut left = self.parse_unary()?;
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

    fn parse_unary(&mut self) -> Result<ConstExpr, ConstExprError> {
        if self.peek() == Some(&Token::Sizeof)
            && self.tokens.get(self.position + 1) == Some(&Token::LParen)
        {
            self.take();
            self.take();
            let value = self.parse_binary(0)?;
            if self.take() != Some(Token::RParen) {
                return Err(ConstExprError::ExpectedRParen);
            }
            return Ok(ConstExpr::SizeOf(Box::new(value)));
        }
        let op = match self.peek() {
            Some(Token::Plus) => Some(UnaryOp::Plus),
            Some(Token::Minus) => Some(UnaryOp::Minus),
            Some(Token::Tilde) => Some(UnaryOp::BitNot),
            Some(Token::Bang) => Some(UnaryOp::Not),
            _ => None,
        };
        if let Some(op) = op {
            self.take();
            return Ok(ConstExpr::Unary {
                op,
                value: Box::new(self.parse_unary()?),
            });
        }
        if self.take() == Some(Token::LParen) {
            let expression = self.parse_binary(0)?;
            if self.take() != Some(Token::RParen) {
                return Err(ConstExprError::ExpectedRParen);
            }
            return Ok(expression);
        }
        match self.tokens.get(self.position.saturating_sub(1)) {
            Some(Token::IntLit(value)) => Ok(ConstExpr::Integer(*value)),
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
