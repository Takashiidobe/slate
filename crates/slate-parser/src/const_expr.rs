use crate::ast::{
    ArraySize, CType, Declarator, Designator, Expr, FloatingType, Initializer, InitializerItem,
    IntegerRank, IntegerType, IntegerValue, Span,
};
use crate::lexer::{Keyword, Token, TokenSpanExt};
use crate::parser::DeclaratorParser;
use miette::Diagnostic;
use rustc_apfloat::ieee;
use std::collections::HashSet;
use thiserror::Error;

#[derive(Debug, Clone, PartialEq)]
pub enum ConstExpr {
    Integer(i64),
    IntegerLiteral(String),
    Float(FloatLiteral),
    Identifier(String),
    StringLit(String),
    Utf8StringLit(String),
    Utf16StringLit(String),
    Utf32StringLit(String),
    WideStringLit(String),
    Generic {
        controlling: Box<Self>,
        associations: Vec<ConstGenericAssociation>,
    },
    SizeOf(Box<Self>),
    SizeOfType {
        ty: Box<CType>,
        declarator: Declarator,
    },
    AlignOf {
        ty: Box<CType>,
        declarator: Declarator,
    },
    OffsetOf {
        ty: Box<CType>,
        declarator: Declarator,
        member: Box<Self>,
    },
    TypesCompatible {
        left_ty: Box<CType>,
        left_declarator: Declarator,
        right_ty: Box<CType>,
        right_declarator: Declarator,
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
    BitCast {
        ty: Box<CType>,
        declarator: Declarator,
        value: Box<Self>,
    },
    CompoundLiteral {
        ty: Box<CType>,
        declarator: Declarator,
        initializer: Vec<InitializerItem>,
    },
    LabelAddr(String),
}

#[derive(Debug, Clone, PartialEq)]
pub struct ConstGenericAssociation {
    pub type_name: Option<String>,
    pub expression: ConstExpr,
}

impl std::fmt::Display for ConstExpr {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Integer(value) => write!(formatter, "{value}"),
            Self::IntegerLiteral(value) => formatter.write_str(value),
            Self::Float(value) => write!(formatter, "{value}"),
            Self::Identifier(value) => formatter.write_str(value),
            Self::StringLit(value) => write!(formatter, "\"{value}\""),
            Self::Utf8StringLit(value) => write!(formatter, "u8\"{value}\""),
            Self::Utf16StringLit(value) => write!(formatter, "u\"{value}\""),
            Self::Utf32StringLit(value) => write!(formatter, "U\"{value}\""),
            Self::WideStringLit(value) => write!(formatter, "L\"{value}\""),
            Self::Generic { .. } => formatter.write_str("_Generic(...)"),
            Self::SizeOf(value) => write!(formatter, "sizeof({value})"),
            Self::SizeOfType { .. } => write!(formatter, "sizeof(...)"),
            Self::AlignOf { .. } => write!(formatter, "_Alignof(...)"),
            Self::OffsetOf { member, .. } => write!(formatter, "__builtin_offsetof(..., {member})"),
            Self::TypesCompatible { .. } => {
                formatter.write_str("__builtin_types_compatible_p(...)")
            }
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
            Self::BitCast { value, .. } => write!(formatter, "__builtin_bit_cast(..., {value})"),
            Self::CompoundLiteral { .. } => write!(formatter, "(compound literal)"),
            Self::LabelAddr(label) => write!(formatter, "&&{label}"),
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
    #[error("cannot compute size of this type")]
    UnsupportedTypeSize,
    #[error("_Alignof is not supported here")]
    UnsupportedAlignOf,
    #[error("integer overflow")]
    IntegerOverflow,
    #[error("invalid integer constant expression")]
    InvalidIntegerConstant,
    #[error("expected `{expected}`, found {}", found.as_ref().map(|span| format!("`{}`", span.value)).unwrap_or_else(|| "end of input".to_string()))]
    Expected {
        expected: Token,
        found: Option<Span<Token>>,
    },
    #[error("expected identifier")]
    ExpectedIdentifier,
    #[error("expected type name")]
    ExpectedTypeName,
    #[error("unexpected token `{0:?}`")]
    UnexpectedToken(Token),
    #[error("expected integer expression")]
    ExpectedIntegerExpression,
    #[error("call to `{0}` is not a constant expression")]
    UnsupportedCall(String),
    #[error("{0} is not a constant expression")]
    NotConstant(&'static str),
    #[error("invalid floating literal `{0}`")]
    InvalidFloatLiteral(String),
}

#[derive(custom_debug::Debug, Clone, PartialEq)]
pub struct FloatLiteral {
    pub value: FloatValue,
    #[debug(skip_if = crate::ast::is_false)]
    pub imaginary: bool,
}

#[derive(custom_debug::Debug, Clone, PartialEq)]
pub enum FloatValue {
    Half(#[debug(format = "{:#x}")] u16),
    Single(f32),
    Double(f64),
    Quad(#[debug(format = "{:#x}")] u128),
    LongDouble(String),
    Decimal32(String),
    Decimal64(String),
    Decimal128(String),
}

impl FloatLiteral {
    const SUFFIXES: [&str; 12] = [
        "f128", "f64x", "f32x", "f16", "f32", "f64", "df", "dd", "dl", "f", "l", "q",
    ];

    pub fn parse(spelling: &str) -> Result<Self, ConstExprError> {
        let invalid = || ConstExprError::InvalidFloatLiteral(spelling.to_string());
        let lowered = spelling.replace('\'', "").to_ascii_lowercase();
        let (mut digits, mut imaginary) = strip_imaginary(&lowered);
        let suffix = Self::SUFFIXES
            .into_iter()
            .find(|suffix| digits.ends_with(suffix))
            .unwrap_or("");
        digits = &digits[..digits.len() - suffix.len()];
        if !imaginary {
            (digits, imaginary) = strip_imaginary(digits);
        }
        let parse_bits = |bits: fn(&str) -> Option<u128>| bits(digits).ok_or_else(invalid);
        let value = match suffix {
            "" | "f64" | "f32x" => FloatValue::Double(f64::from_bits(parse_bits(
                apfloat_bits::<ieee::Double>,
            )? as u64)),
            "f" | "f32" => FloatValue::Single(f32::from_bits(parse_bits(
                apfloat_bits::<ieee::Single>,
            )? as u32)),
            "f16" => FloatValue::Half(parse_bits(apfloat_bits::<ieee::Half>)? as u16),
            "f128" | "q" => FloatValue::Quad(parse_bits(apfloat_bits::<ieee::Quad>)?),
            "l" | "f64x" => FloatValue::LongDouble(digits.to_string()),
            "df" => FloatValue::Decimal32(digits.to_string()),
            "dd" => FloatValue::Decimal64(digits.to_string()),
            _ => FloatValue::Decimal128(digits.to_string()),
        };
        Ok(Self { value, imaginary })
    }
}

fn strip_imaginary(digits: &str) -> (&str, bool) {
    match digits.strip_suffix(['i', 'j']) {
        Some(stripped) => (stripped, true),
        None => (digits, false),
    }
}

fn integer_value(token: &Token) -> IntegerValue {
    token.integer_value_i128().map_or_else(
        || IntegerValue::Arbitrary(String::from(token)),
        IntegerValue::I128,
    )
}

fn apfloat_bits<F: rustc_apfloat::Float>(digits: &str) -> Option<u128> {
    F::from_str_r(digits, rustc_apfloat::Round::NearestTiesToEven)
        .ok()
        .map(|parsed| parsed.value.to_bits())
}

impl std::fmt::Display for FloatLiteral {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match &self.value {
            FloatValue::Half(bits) => write!(formatter, "f16:{bits:#x}")?,
            FloatValue::Single(value) => write!(formatter, "{value}f")?,
            FloatValue::Double(value) => write!(formatter, "{value}")?,
            FloatValue::Quad(bits) => write!(formatter, "f128:{bits:#x}")?,
            FloatValue::LongDouble(digits) => write!(formatter, "{digits}L")?,
            FloatValue::Decimal32(digits) => write!(formatter, "{digits}DF")?,
            FloatValue::Decimal64(digits) => write!(formatter, "{digits}DD")?,
            FloatValue::Decimal128(digits) => write!(formatter, "{digits}DL")?,
        }
        if self.imaginary {
            formatter.write_str("i")?;
        }
        Ok(())
    }
}

#[derive(Debug)]
pub struct LocatedConstExprError {
    pub error: ConstExprError,
    pub token: Option<usize>,
}

pub struct Parser {
    tokens: Vec<Span<Token>>,
    position: usize,
    typedef_names: HashSet<String>,
}

impl Parser {
    pub fn parse(tokens: &[Span<Token>]) -> Result<ConstExpr, ConstExprError> {
        let mut parser = Self::new(tokens, &HashSet::new());
        let expression = parser.parse_conditional()?;
        if parser.peek().is_some() {
            return Err(ConstExprError::UnexpectedTokens);
        }
        Ok(expression)
    }

    pub fn parse_expression(
        tokens: &[Span<Token>],
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
        tokens: &[Span<Token>],
        start: usize,
        typedef_names: &HashSet<String>,
    ) -> Result<(ConstExpr, usize), ConstExprError> {
        let mut parser = Self::new(&tokens[start..], typedef_names);
        let expression = parser.parse_assignment()?;
        Ok((expression, start + parser.position))
    }

    pub fn evaluate(tokens: &[Span<Token>]) -> Result<i64, ConstExprError> {
        Self::evaluate_expr(&Self::parse(tokens)?, None)
    }

    pub fn evaluate_with_defined(
        tokens: &[Span<Token>],
        is_defined: &dyn Fn(&str) -> bool,
    ) -> Result<i64, LocatedConstExprError> {
        let mut parser = Self::new(tokens, &HashSet::new());
        let at_position = |parser: &Self, error| LocatedConstExprError {
            token: Some(parser.failure_position(&error)),
            error,
        };
        let expression = parser
            .parse_conditional()
            .map_err(|error| at_position(&parser, error))?;
        if parser.peek().is_some() {
            return Err(at_position(&parser, ConstExprError::UnexpectedTokens));
        }
        Self::evaluate_expr(&expression, Some(is_defined))
            .map_err(|error| LocatedConstExprError { error, token: None })
    }

    fn evaluate_expr(
        expression: &ConstExpr,
        is_defined: Option<&dyn Fn(&str) -> bool>,
    ) -> Result<i64, ConstExprError> {
        match expression {
            ConstExpr::Integer(value) => Ok(*value),
            ConstExpr::IntegerLiteral(_) => {
                Err(ConstExprError::NotConstant("wide integer literal"))
            }
            ConstExpr::StringLit(_)
            | ConstExpr::Utf8StringLit(_)
            | ConstExpr::Utf16StringLit(_)
            | ConstExpr::Utf32StringLit(_)
            | ConstExpr::WideStringLit(_) => Err(ConstExprError::NotConstant("string literal")),
            ConstExpr::Generic { .. } => Err(ConstExprError::NotConstant("generic selection")),
            ConstExpr::Float(_) => Err(ConstExprError::NotConstant("floating literal")),
            ConstExpr::Identifier(name) => match is_defined {
                Some(_) => Ok(0),
                None => Err(ConstExprError::UnsupportedIdentifier(name.clone())),
            },
            ConstExpr::SizeOf(_) => Err(ConstExprError::UnsupportedSizeOf),
            ConstExpr::SizeOfType { ty, declarator } => {
                declarator_size(ty, declarator).map(|size| size as i64)
            }
            ConstExpr::AlignOf { .. } => Err(ConstExprError::UnsupportedAlignOf),
            ConstExpr::OffsetOf { .. } => Err(ConstExprError::NotConstant("offsetof")),
            ConstExpr::TypesCompatible { .. } => {
                Err(ConstExprError::NotConstant("types compatible"))
            }
            ConstExpr::Call { callee, arguments } => {
                match (is_defined, callee.as_ref(), arguments.as_slice()) {
                    (
                        Some(is_defined),
                        ConstExpr::Identifier(name),
                        [ConstExpr::Identifier(macro_name)],
                    ) if name == "defined" => Ok(is_defined(macro_name) as i64),
                    (Some(_), ConstExpr::Identifier(name), [ConstExpr::Identifier(_)])
                        if matches!(
                            name.as_str(),
                            "__has_include"
                                | "__has_include_next"
                                | "__has_feature"
                                | "__has_extension"
                                | "__has_builtin"
                                | "__has_attribute"
                                | "__has_c_attribute"
                                | "__has_cpp_attribute"
                                | "__building_module"
                        ) =>
                    {
                        Ok(0)
                    }
                    _ => Err(ConstExprError::UnsupportedCall(callee.to_string())),
                }
            }
            ConstExpr::Cast { value, .. } => Self::evaluate_expr(value, is_defined),
            ConstExpr::BitCast { .. } => Err(ConstExprError::NotConstant("bit cast")),
            ConstExpr::Unary { op, value } => {
                let value = Self::evaluate_expr(value, is_defined)?;
                match op {
                    UnaryOp::Plus => Ok(value),
                    UnaryOp::Minus => value.checked_neg().ok_or(ConstExprError::IntegerOverflow),
                    UnaryOp::BitNot => Ok(!value),
                    UnaryOp::Not => Ok((value == 0) as i64),
                }
            }
            ConstExpr::Binary { op, left, right } => {
                let left = Self::evaluate_expr(left, is_defined)?;
                if *op == BinaryOp::And && left == 0 || *op == BinaryOp::Or && left != 0 {
                    return Ok((*op == BinaryOp::Or) as i64);
                }
                let right = Self::evaluate_expr(right, is_defined)?;
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
                if Self::evaluate_expr(condition, is_defined)? != 0 {
                    Self::evaluate_expr(then_value, is_defined)
                } else {
                    Self::evaluate_expr(else_value, is_defined)
                }
            }
            ConstExpr::Comma(left, right) => {
                Self::evaluate_expr(left, is_defined)?;
                Self::evaluate_expr(right, is_defined)
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
            ConstExpr::LabelAddr(_) => Err(ConstExprError::NotConstant("label address")),
        }
    }

    fn new(tokens: &[Span<Token>], typedef_names: &HashSet<String>) -> Self {
        Self {
            tokens: tokens.to_vec(),
            position: 0,
            typedef_names: typedef_names.clone(),
        }
    }

    fn failure_position(&self, error: &ConstExprError) -> usize {
        match error {
            // some errors fire after `take()` has already consumed the offending token
            ConstExprError::UnexpectedToken(token)
                if self.position > 0 && self.token_at(self.position - 1) == Some(token) =>
            {
                self.position - 1
            }
            _ => self.position,
        }
    }

    fn peek(&self) -> Option<&Token> {
        self.tokens.value_at(self.position)
    }

    fn peek_at(&self, offset: usize) -> Option<&Token> {
        self.tokens.value_at(self.position + offset)
    }

    fn token_at(&self, index: usize) -> Option<&Token> {
        self.tokens.value_at(index)
    }

    fn consume(&mut self, token: &Token) -> bool {
        if self.peek() == Some(token) {
            self.position += 1;
            true
        } else {
            false
        }
    }

    fn expect(&mut self, token: Token) -> Result<(), ConstExprError> {
        if self.consume(&token) {
            Ok(())
        } else {
            Err(ConstExprError::Expected {
                expected: token,
                found: self.tokens.get(self.position).cloned(),
            })
        }
    }

    fn take(&mut self) -> Option<Token> {
        let token = self.tokens.value_owned(self.position);
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
            self.expect(Token::Colon)?;
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
            && let Some(next) = self.peek_at(1)
            && starts_type_name(next, &self.typedef_names)
            && let Some((ty, declarator, end)) = self.try_parse_type_name(self.position + 1)
            && self.token_at(end) == Some(&Token::RParen)
            && self.token_at(end + 1) != Some(&Token::LBrace)
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

    fn try_parse_compound_literal(&mut self) -> Result<Option<ConstExpr>, ConstExprError> {
        if self.peek() != Some(&Token::LParen) {
            return Ok(None);
        }
        let Some(next) = self.peek_at(1) else {
            return Ok(None);
        };
        if !starts_type_name(next, &self.typedef_names) {
            return Ok(None);
        }
        let Some((ty, declarator, end)) = self.try_parse_type_name(self.position + 1) else {
            return Ok(None);
        };
        if self.token_at(end) != Some(&Token::RParen)
            || self.token_at(end + 1) != Some(&Token::LBrace)
        {
            return Ok(None);
        }
        self.position = end + 1;
        let initializer = self.parse_initializer_list()?;
        Ok(Some(ConstExpr::CompoundLiteral {
            ty,
            declarator,
            initializer,
        }))
    }

    fn parse_initializer_list(&mut self) -> Result<Vec<InitializerItem>, ConstExprError> {
        let opening = self.take();
        if opening != Some(Token::LBrace) {
            return Err(ConstExprError::UnexpectedToken(opening.unwrap()));
        }
        let mut items = Vec::new();
        while self.peek() != Some(&Token::RBrace) {
            let mut designators = Vec::new();
            loop {
                if self.peek() == Some(&Token::LBracket) {
                    self.take();
                    let Some(token @ Token::IntLit(_)) = self.take() else {
                        return Err(ConstExprError::ExpectedIntegerExpression);
                    };
                    let index_value = integer_value(&token);
                    let Some(index) = token.integer_value() else {
                        return Err(ConstExprError::ExpectedIntegerExpression);
                    };
                    if self.consume(&Token::Ellipsis) {
                        let Some(Token::IntLit(end)) = self.take() else {
                            return Err(ConstExprError::ExpectedIntegerExpression);
                        };
                        let end = integer_value(&Token::IntLit(end));
                        designators.push(Designator::ArrayRange {
                            start: index_value,
                            end,
                        });
                    } else {
                        designators.push(Designator::Array(index));
                    }
                    self.expect(Token::RBracket)?;
                } else if self.peek() == Some(&Token::Dot) {
                    self.take();
                    designators.push(Designator::Field(self.expect_field_name()?));
                } else {
                    break;
                }
            }
            if !designators.is_empty() {
                self.expect(Token::Equal)?;
            }
            let value = self.parse_initializer_value()?;
            items.push(InitializerItem { designators, value });
            if !self.consume(&Token::Comma) {
                break;
            }
        }
        self.expect(Token::RBrace)?;
        Ok(items)
    }

    fn parse_initializer_value(&mut self) -> Result<Initializer, ConstExprError> {
        let start = self.position;
        if self.peek() == Some(&Token::LBrace) {
            Ok(Initializer::List(self.parse_initializer_list()?))
        } else if let Some(expression) = string_literal_expr(self.peek()) {
            self.take();
            Ok(Initializer::Expr(Span::cover(
                expression,
                &self.tokens[start..self.position],
            )))
        } else {
            let expression = self.parse_assignment()?;
            Ok(Initializer::Expr(Span::cover(
                Expr::Const(Box::new(expression)),
                &self.tokens[start..self.position],
            )))
        }
    }

    fn try_parse_type_name(&self, start: usize) -> Option<(Box<CType>, Declarator, usize)> {
        let mut declarator_parser = DeclaratorParser::new(&self.tokens, start);
        let leading = declarator_parser.take_qualifiers();
        let mut ty = declarator_parser.parse_base_type().ok()?;
        let trailing = declarator_parser.take_qualifiers();
        let qualifiers = crate::ast::Qualifiers {
            is_const: leading.is_const || trailing.is_const,
            is_volatile: leading.is_volatile || trailing.is_volatile,
            is_restrict: leading.is_restrict || trailing.is_restrict,
            is_atomic: leading.is_atomic || trailing.is_atomic,
        };
        if qualifiers != crate::ast::Qualifiers::default() {
            ty = CType::Qualified {
                qualifiers,
                ty: Box::new(ty),
            };
        }
        let declarator = declarator_parser.parse_declarator(true).ok()?;
        Some((Box::new(ty), declarator, declarator_parser.position()))
    }

    fn parse_unary(&mut self) -> Result<ConstExpr, ConstExprError> {
        if self.peek() == Some(&Token::Sizeof)
            && self.peek_at(1) == Some(&Token::LParen)
            && let Some(next) = self.peek_at(2)
            && starts_type_name(next, &self.typedef_names)
            && let Some((ty, declarator, end)) = self.try_parse_type_name(self.position + 2)
            && self.token_at(end) == Some(&Token::RParen)
        {
            self.position = end + 1;
            return Ok(ConstExpr::SizeOfType { ty, declarator });
        }
        if self.peek() == Some(&Token::Sizeof) && self.peek_at(1) == Some(&Token::LParen) {
            self.take();
            self.take();
            let value = self.parse_comma()?;
            self.expect(Token::RParen)?;
            return Ok(ConstExpr::SizeOf(Box::new(value)));
        }
        if self.peek() == Some(&Token::Sizeof) {
            self.take();
            return Ok(ConstExpr::SizeOf(Box::new(self.parse_unary()?)));
        }
        if self.peek() == Some(&Token::Alignof) {
            self.take();
            self.expect(Token::LParen)?;
            let (ty, declarator, end) = self
                .try_parse_type_name(self.position)
                .ok_or(ConstExprError::ExpectedTypeName)?;
            if self.token_at(end) != Some(&Token::RParen) {
                return Err(ConstExprError::Expected {
                    expected: Token::RParen,
                    found: self.tokens.get(end).cloned(),
                });
            }
            self.position = end + 1;
            return Ok(ConstExpr::AlignOf { ty, declarator });
        }
        if let Some(Token::Ident(name)) = self.peek()
            && matches!(name.as_str(), "__real__" | "__imag__")
        {
            let name = name.clone();
            self.take();
            return Ok(ConstExpr::Call {
                callee: Box::new(ConstExpr::Identifier(name)),
                arguments: vec![self.parse_unary()?],
            });
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
            Some(Token::AndAnd) => {
                self.take();
                match self.take() {
                    Some(Token::Ident(label)) => Ok(ConstExpr::LabelAddr(label)),
                    Some(token) => Err(ConstExprError::UnexpectedToken(token)),
                    None => Err(ConstExprError::ExpectedIdentifier),
                }
            }
            _ => self.parse_postfix(),
        }
    }

    fn parse_postfix(&mut self) -> Result<ConstExpr, ConstExprError> {
        let mut expression = match self.try_parse_compound_literal()? {
            Some(result) => result,
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
                    self.expect(Token::RParen)?;
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
                    self.expect(Token::RBracket)?;
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
            self.expect(Token::RParen)?;
            return Ok(expression);
        }
        match self.tokens.value_at(self.position.saturating_sub(1)) {
            Some(Token::IntLit(value)) => match Token::IntLit(value.clone()).integer_value() {
                Some(value) => Ok(ConstExpr::Integer(value)),
                None => Ok(ConstExpr::IntegerLiteral(value.clone())),
            },
            Some(Token::FloatLit(value)) => FloatLiteral::parse(value).map(ConstExpr::Float),
            Some(Token::StringLit(value)) => Ok(ConstExpr::StringLit(value.clone())),
            Some(Token::Utf8StringLit(value)) => Ok(ConstExpr::Utf8StringLit(value.clone())),
            Some(Token::Utf16StringLit(value)) => Ok(ConstExpr::Utf16StringLit(value.clone())),
            Some(Token::Utf32StringLit(value)) => Ok(ConstExpr::Utf32StringLit(value.clone())),
            Some(Token::WideStringLit(value)) => Ok(ConstExpr::WideStringLit(value.clone())),
            Some(
                Token::CharLit(_, value)
                | Token::Utf8CharLit(_, value)
                | Token::Utf16CharLit(_, value)
                | Token::Utf32CharLit(_, value)
                | Token::WideCharLit(_, value),
            ) => Ok(ConstExpr::Integer(*value)),
            Some(Token::Ident(value)) if value == "defined" => self.parse_defined(),
            Some(Token::Ident(value))
                if value == "__has_include" || value == "__has_include_next" =>
            {
                self.parse_has_include(value.clone())
            }
            Some(Token::Ident(value)) if value == "__builtin_offsetof" => self.parse_offsetof(),
            Some(Token::Ident(value)) if value == "__builtin_bit_cast" => self.parse_bit_cast(),
            Some(Token::Ident(value)) if value == "__builtin_types_compatible_p" => {
                self.parse_types_compatible()
            }
            Some(Token::Ident(value)) if value == "_Generic" => self.parse_generic(),
            Some(Token::Ident(value)) => Ok(ConstExpr::Identifier(value.clone())),
            Some(Token::Keyword(keyword)) => {
                Ok(ConstExpr::Identifier(<&str>::from(*keyword).into()))
            }
            Some(token) => Err(ConstExprError::UnexpectedToken(token.clone())),
            None => Err(ConstExprError::ExpectedIntegerExpression),
        }
    }

    fn parse_generic(&mut self) -> Result<ConstExpr, ConstExprError> {
        self.expect(Token::LParen)?;
        let controlling = if let Some((_, _, end)) = self.try_parse_type_name(self.position)
            && self.token_at(end) == Some(&Token::Comma)
        {
            self.position = end;
            ConstExpr::Identifier("<type-name>".into())
        } else {
            self.parse_assignment()?
        };
        self.expect(Token::Comma)?;
        let mut associations = Vec::new();
        loop {
            let type_start = self.position;
            let mut depth = 0i32;
            while let Some(token) = self.peek() {
                match token {
                    Token::LParen | Token::LBracket => depth += 1,
                    Token::RParen | Token::RBracket if depth > 0 => depth -= 1,
                    Token::Colon if depth == 0 => break,
                    _ => {}
                }
                self.take();
            }
            let type_tokens = self.tokens[type_start..self.position].to_vec();
            self.expect(Token::Colon)?;
            let type_name = match type_tokens.as_tokens().as_slice() {
                [Token::Keyword(Keyword::Default)] => None,
                _ => Some(
                    type_tokens
                        .values()
                        .map(String::from)
                        .collect::<Vec<_>>()
                        .join(" "),
                ),
            };
            associations.push(ConstGenericAssociation {
                type_name,
                expression: self.parse_assignment()?,
            });
            if self.consume(&Token::Comma) {
                continue;
            }
            self.expect(Token::RParen)?;
            break;
        }
        Ok(ConstExpr::Generic {
            controlling: Box::new(controlling),
            associations,
        })
    }

    fn parse_defined(&mut self) -> Result<ConstExpr, ConstExprError> {
        let parenthesized = self.consume(&Token::LParen);
        let name = match self.take() {
            Some(Token::Ident(value)) => value,
            Some(token) => return Err(ConstExprError::UnexpectedToken(token)),
            None => return Err(ConstExprError::ExpectedIntegerExpression),
        };
        if parenthesized {
            self.expect(Token::RParen)?;
        }
        Ok(ConstExpr::Call {
            callee: Box::new(ConstExpr::Identifier("defined".to_string())),
            arguments: vec![ConstExpr::Identifier(name)],
        })
    }

    fn parse_has_include(&mut self, name: String) -> Result<ConstExpr, ConstExprError> {
        self.expect(Token::LParen)?;
        let header = if self.peek() == Some(&Token::Less) {
            self.take();
            let mut text = String::new();
            loop {
                match self.take() {
                    Some(Token::Greater) => break,
                    Some(Token::Ident(part)) => text.push_str(&part),
                    Some(Token::Keyword(keyword)) => text.push_str(<&str>::from(keyword)),
                    Some(Token::Dot) => text.push('.'),
                    Some(Token::Slash) => text.push('/'),
                    Some(Token::Minus) => text.push('-'),
                    Some(token) => return Err(ConstExprError::UnexpectedToken(token)),
                    None => {
                        return Err(ConstExprError::Expected {
                            expected: Token::Greater,
                            found: None,
                        });
                    }
                }
            }
            format!("<{text}>")
        } else {
            match self.take() {
                Some(Token::StringLit(text)) => format!("\"{text}\""),
                Some(token) => return Err(ConstExprError::UnexpectedToken(token)),
                None => return Err(ConstExprError::ExpectedIntegerExpression),
            }
        };
        self.expect(Token::RParen)?;
        Ok(ConstExpr::Call {
            callee: Box::new(ConstExpr::Identifier(name)),
            arguments: vec![ConstExpr::Identifier(header)],
        })
    }

    fn parse_offsetof(&mut self) -> Result<ConstExpr, ConstExprError> {
        self.expect(Token::LParen)?;
        let (ty, declarator, end) = self
            .try_parse_type_name(self.position)
            .ok_or(ConstExprError::ExpectedTypeName)?;
        self.position = end;
        self.expect(Token::Comma)?;
        let mut member = ConstExpr::Identifier(self.expect_field_name()?);
        loop {
            member = match self.peek() {
                Some(Token::Dot) => {
                    self.take();
                    ConstExpr::Member {
                        base: Box::new(member),
                        field: self.expect_field_name()?,
                    }
                }
                Some(Token::LBracket) => {
                    self.take();
                    let index = self.parse_comma()?;
                    self.expect(Token::RBracket)?;
                    ConstExpr::Index {
                        base: Box::new(member),
                        index: Box::new(index),
                    }
                }
                _ => break,
            };
        }
        self.expect(Token::RParen)?;
        Ok(ConstExpr::OffsetOf {
            ty,
            declarator,
            member: Box::new(member),
        })
    }

    fn parse_bit_cast(&mut self) -> Result<ConstExpr, ConstExprError> {
        self.expect(Token::LParen)?;
        let (ty, declarator, end) = self
            .try_parse_type_name(self.position)
            .ok_or(ConstExprError::ExpectedTypeName)?;
        self.position = end;
        self.expect(Token::Comma)?;
        let value = self.parse_assignment()?;
        self.expect(Token::RParen)?;
        Ok(ConstExpr::BitCast {
            ty,
            declarator,
            value: Box::new(value),
        })
    }

    fn parse_types_compatible(&mut self) -> Result<ConstExpr, ConstExprError> {
        self.expect(Token::LParen)?;
        let (left_ty, left_declarator, end) = self
            .try_parse_type_name(self.position)
            .ok_or(ConstExprError::ExpectedTypeName)?;
        self.position = end;
        self.expect(Token::Comma)?;
        let (right_ty, right_declarator, end) = self
            .try_parse_type_name(self.position)
            .ok_or(ConstExprError::ExpectedTypeName)?;
        self.position = end;
        self.expect(Token::RParen)?;
        Ok(ConstExpr::TypesCompatible {
            left_ty,
            left_declarator,
            right_ty,
            right_declarator,
        })
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

fn declarator_size(ty: &CType, declarator: &Declarator) -> Result<u64, ConstExprError> {
    match declarator {
        Declarator::Abstract | Declarator::Name(_) => ctype_size(ty),
        Declarator::Grouped(inner) => declarator_size(ty, inner),
        Declarator::Pointer { .. } => Ok(8),
        Declarator::Array { inner, size } => {
            let element = declarator_size(ty, inner)?;
            let count = match size {
                ArraySize::Expression(expr) => match &expr.value {
                    Expr::IntLit(value) => *value as u64,
                    _ => return Err(ConstExprError::UnsupportedTypeSize),
                },
                ArraySize::Unspecified | ArraySize::Star => {
                    return Err(ConstExprError::UnsupportedTypeSize);
                }
            };
            Ok(element * count)
        }
        Declarator::Function { .. } => Err(ConstExprError::UnsupportedTypeSize),
    }
}

fn ctype_size(ty: &CType) -> Result<u64, ConstExprError> {
    match ty {
        CType::Void => Ok(1),
        CType::Bool => Ok(1),
        CType::Pointer { .. } => Ok(8),
        CType::Integer(IntegerType::Char { .. }) => Ok(1),
        CType::Integer(IntegerType::Ranked { rank, .. }) => Ok(match rank {
            IntegerRank::Short => 2,
            IntegerRank::Int => 4,
            IntegerRank::Long => 8,
            IntegerRank::LongLong => 8,
            IntegerRank::Int128 => 16,
        }),
        CType::Integer(IntegerType::BitInt { .. }) => Err(ConstExprError::UnsupportedTypeSize),
        CType::Floating(kind) => Ok(match kind {
            FloatingType::BFloat16 | FloatingType::Float16 | FloatingType::Fp16 => 2,
            FloatingType::Float => 4,
            FloatingType::Double | FloatingType::Float64x => 8,
            FloatingType::LongDouble | FloatingType::Float128 | FloatingType::Float128Ext => 16,
        }),
        CType::Complex(inner) => Ok(ctype_size(inner)? * 2),
        CType::Imaginary(inner) => ctype_size(inner),
        CType::Qualified { ty, .. } => ctype_size(ty),
        CType::Atomic(inner) => ctype_size(inner),
        CType::Array { element, size } => {
            let element_size = ctype_size(element)?;
            let count = match size {
                ArraySize::Expression(expr) => match &expr.value {
                    Expr::IntLit(value) => *value as u64,
                    _ => return Err(ConstExprError::UnsupportedTypeSize),
                },
                ArraySize::Unspecified | ArraySize::Star => {
                    return Err(ConstExprError::UnsupportedTypeSize);
                }
            };
            Ok(element_size * count)
        }
        CType::TypeOf(_)
        | CType::TypeOfUnqual(_)
        | CType::TargetBuiltin(_)
        | CType::Named(_)
        | CType::Tagged { .. }
        | CType::Function { .. }
        | CType::Vector(_)
        | CType::FixedPoint(_) => Err(ConstExprError::UnsupportedTypeSize),
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
                | Keyword::TypeofUnqual
                | Keyword::Constexpr
        ),
        Token::Ident(name) => {
            typedef_names.contains(name)
                || matches!(name.as_str(), "char8_t" | "atomic_char8_t" | "nullptr_t")
        }
        _ => false,
    }
}

pub(crate) fn string_literal_expr(token: Option<&Token>) -> Option<Expr> {
    match token? {
        Token::StringLit(value) => Some(Expr::StringLit(value.clone())),
        Token::Utf8StringLit(value) => Some(Expr::Utf8StringLit(value.clone())),
        Token::Utf16StringLit(value) => Some(Expr::Utf16StringLit(value.clone())),
        Token::Utf32StringLit(value) => Some(Expr::Utf32StringLit(value.clone())),
        Token::WideStringLit(value) => Some(Expr::WideStringLit(value.clone())),
        _ => None,
    }
}
