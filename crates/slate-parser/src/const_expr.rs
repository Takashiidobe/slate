use crate::ast::{
    Designator, Expr, ExprKind, FixedPointKind, FixedPointRank, GenericAssociation, GenericControl,
    Initializer, InitializerItem, IntegerType, Span, SpanRangeIndex, TypeName, TypeSpecifier,
};
use crate::lexer::{Keyword, Lexer, Token, TokenSpanExt};
use crate::parser::DeclaratorParser;
use crate::standard_features::StandardFeatures;
use crate::target_info::TargetInfo;
use miette::Diagnostic;
use num_bigint::{BigInt, BigUint};
use std::cell::Cell;
use thiserror::Error;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct WideInt {
    pub value: BigInt,
    pub width: u32,
    pub signed: bool,
}

impl WideInt {
    pub fn wrap(value: BigInt, width: u32, signed: bool) -> Self {
        let modulus = BigInt::from(1) << width;
        let mut value = ((value % &modulus) + &modulus) % &modulus;
        if signed && value.bit(u64::from(width) - 1) {
            value -= modulus;
        }
        Self {
            value,
            width,
            signed,
        }
    }

    pub fn from_i64(value: i64) -> Self {
        Self {
            value: BigInt::from(value),
            width: 64,
            signed: true,
        }
    }

    fn combined_width_signed(&self, rhs: &Self) -> (u32, bool) {
        match (self.signed, rhs.signed) {
            (true, true) | (false, false) => (self.width.max(rhs.width), self.signed),
            (true, false) if self.width > rhs.width => (self.width, true),
            (false, true) if rhs.width > self.width => (rhs.width, true),
            _ => (self.width.max(rhs.width), false),
        }
    }

    fn binary(&self, rhs: &Self, op: impl FnOnce(&BigInt, &BigInt) -> BigInt) -> Self {
        let (width, signed) = self.combined_width_signed(rhs);
        let left = Self::wrap(self.value.clone(), width, signed);
        let right = Self::wrap(rhs.value.clone(), width, signed);
        Self::wrap(op(&left.value, &right.value), width, signed)
    }

    pub fn wrapping_add(&self, rhs: &Self) -> Self {
        self.binary(rhs, |left, right| left + right)
    }

    pub fn wrapping_sub(&self, rhs: &Self) -> Self {
        self.binary(rhs, |left, right| left - right)
    }

    pub fn wrapping_mul(&self, rhs: &Self) -> Self {
        self.binary(rhs, |left, right| left * right)
    }

    pub fn bitand(&self, rhs: &Self) -> Self {
        self.binary(rhs, |left, right| left & right)
    }

    pub fn bitor(&self, rhs: &Self) -> Self {
        self.binary(rhs, |left, right| left | right)
    }

    pub fn bitxor(&self, rhs: &Self) -> Self {
        self.binary(rhs, |left, right| left ^ right)
    }

    pub fn checked_div(&self, rhs: &Self) -> Option<Self> {
        if rhs.value == BigInt::from(0) {
            return None;
        }
        Some(self.binary(rhs, |left, right| left / right))
    }

    pub fn checked_rem(&self, rhs: &Self) -> Option<Self> {
        if rhs.value == BigInt::from(0) {
            return None;
        }
        Some(self.binary(rhs, |left, right| left % right))
    }

    pub fn shift_left(&self, shift: u32) -> Self {
        Self::wrap(&self.value << shift, self.width, self.signed)
    }

    pub fn shift_right(&self, shift: u32) -> Self {
        Self::wrap(&self.value >> shift, self.width, self.signed)
    }

    pub fn compare(&self, rhs: &Self, op: BinaryOp) -> Option<i64> {
        let (width, signed) = self.combined_width_signed(rhs);
        let left = Self::wrap(self.value.clone(), width, signed);
        let right = Self::wrap(rhs.value.clone(), width, signed);
        let result = match op {
            BinaryOp::Less => left.value < right.value,
            BinaryOp::LessEqual => left.value <= right.value,
            BinaryOp::Greater => left.value > right.value,
            BinaryOp::GreaterEqual => left.value >= right.value,
            BinaryOp::Equal => left.value == right.value,
            BinaryOp::NotEqual => left.value != right.value,
            _ => return None,
        };
        Some(result as i64)
    }

    pub fn is_zero(&self) -> bool {
        self.value == BigInt::from(0)
    }

    pub fn truncate_to_i64(&self) -> i64 {
        let mut bytes = self.value.to_signed_bytes_le();
        bytes.resize(
            8,
            if self.value.sign() == num_bigint::Sign::Minus {
                0xFF
            } else {
                0
            },
        );
        let mut result = [0u8; 8];
        let length = bytes.len().min(result.len());
        result[..length].copy_from_slice(&bytes[..length]);
        i64::from_le_bytes(result)
    }
}

impl std::fmt::Display for WideInt {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(formatter, "{}", self.value)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum UnaryOp {
    Plus,
    Minus,
    BitNot,
    Not,
    AddrOf,
    Deref,
    PreIncrement,
    PreDecrement,
    Real,
    Imag,
}

impl From<UnaryOp> for &'static str {
    fn from(op: UnaryOp) -> Self {
        match op {
            UnaryOp::Plus => "+",
            UnaryOp::Minus => "-",
            UnaryOp::BitNot => "~",
            UnaryOp::Not => "!",
            UnaryOp::AddrOf => "&",
            UnaryOp::Deref => "*",
            UnaryOp::PreIncrement => "++",
            UnaryOp::PreDecrement => "--",
            UnaryOp::Real => "__real__ ",
            UnaryOp::Imag => "__imag__ ",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PostfixOp {
    Increment,
    Decrement,
}

impl From<PostfixOp> for &'static str {
    fn from(op: PostfixOp) -> Self {
        match op {
            PostfixOp::Increment => "++",
            PostfixOp::Decrement => "--",
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
    #[error("{0}")]
    CharacterConstant(&'static str),
    #[error("{0} requires semantic type evaluation")]
    RequiresSemanticEvaluation(&'static str),
    #[error("invalid floating literal `{0}`")]
    InvalidFloatLiteral(String),
    #[error("{0}")]
    StatementExpression(String),
    #[error("nesting level exceeded maximum")]
    NestingTooDeep,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Radix {
    Decimal,
    Hex,
    Octal,
    Binary,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum IntegerSizeSuffix {
    None,
    Long,
    LongLong,
    BitInt,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct IntegerSuffix {
    pub unsigned: bool,
    pub size: IntegerSizeSuffix,
}

#[derive(custom_debug::Debug, Clone, PartialEq, Eq)]
pub struct IntegerLiteral {
    #[debug(format = "{}")]
    pub value: BigUint,
    pub radix: Radix,
    pub suffix: IntegerSuffix,
    pub spelling: String,
    #[debug(skip_if = crate::ast::is_false)]
    pub imaginary: bool,
}

impl IntegerLiteral {
    pub fn decimal(value: i64) -> Self {
        let value = BigUint::try_from(value).unwrap_or_default();
        Self {
            spelling: value.to_string(),
            value,
            radix: Radix::Decimal,
            suffix: IntegerSuffix {
                unsigned: false,
                size: IntegerSizeSuffix::None,
            },
            imaginary: false,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum FloatRadix {
    Decimal,
    Hex,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum FloatSuffix {
    None,
    F,
    L,
    BF16,
    F16,
    F32,
    F64,
    F128,
    F32x,
    F64x,
    Q,
    DecimalF32,
    DecimalF64,
    DecimalF128,
}

#[derive(custom_debug::Debug, Clone, PartialEq)]
pub struct FloatLiteral {
    pub spelling: String,
    pub radix: FloatRadix,
    pub suffix: FloatSuffix,
    #[debug(skip_if = Option::is_none)]
    pub fixed_suffix: Option<FixedPointLiteralSuffix>,
    #[debug(skip_if = crate::ast::is_false)]
    pub imaginary: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct FixedPointLiteralSuffix {
    pub kind: FixedPointKind,
    pub rank: FixedPointRank,
    pub unsigned: bool,
}

#[derive(custom_debug::Debug, Clone, PartialEq)]
pub struct ResolvedFloat {
    pub value: FloatValue,
    #[debug(skip_if = crate::ast::is_false)]
    pub imaginary: bool,
}

#[derive(custom_debug::Debug, Clone, PartialEq)]
pub enum FloatValue {
    BFloat16(#[debug(format = "{:#x}")] u16),
    Half(#[debug(format = "{:#x}")] u16),
    Single(f32),
    Double(f64),
    Quad(#[debug(format = "{:#x}")] u128),
    LongDouble(#[debug(format = "{:#x}")] u128),
    Decimal32(String),
    Decimal64(String),
    Decimal128(String),
}

// scanned in order by ends_with, so bf16 must precede f16
const FLOAT_SUFFIXES: [&str; 13] = [
    "bf16", "f128", "f64x", "f32x", "f16", "f32", "f64", "df", "dd", "dl", "f", "l", "q",
];

fn float_suffix_from_token(suffix: &str) -> FloatSuffix {
    match suffix {
        "" => FloatSuffix::None,
        "f" => FloatSuffix::F,
        "l" => FloatSuffix::L,
        "bf16" => FloatSuffix::BF16,
        "f16" => FloatSuffix::F16,
        "f32" => FloatSuffix::F32,
        "f64" => FloatSuffix::F64,
        "f128" => FloatSuffix::F128,
        "f32x" => FloatSuffix::F32x,
        "f64x" => FloatSuffix::F64x,
        "q" => FloatSuffix::Q,
        "df" => FloatSuffix::DecimalF32,
        "dd" => FloatSuffix::DecimalF64,
        _ => FloatSuffix::DecimalF128,
    }
}

fn split_float_spelling(spelling: &str) -> (String, &'static str, bool) {
    let spelling = spelling.replace('\'', "");
    let imaginary_start = spelling
        .char_indices()
        .next_back()
        .filter(|(_, ch)| matches!(ch, 'i' | 'I' | 'j' | 'J'))
        .map(|(index, _)| index);
    let (digits, imaginary) = match imaginary_start {
        Some(index) => (&spelling[..index], true),
        None => (spelling.as_str(), false),
    };
    let suffix = FLOAT_SUFFIXES
        .into_iter()
        .find(|suffix| {
            digits.ends_with(suffix)
                || digits.ends_with(&suffix.to_ascii_uppercase())
                || match *suffix {
                    "f32x" => digits.ends_with("F32x"),
                    "f64x" => digits.ends_with("F64x"),
                    _ => false,
                }
        })
        .unwrap_or("");
    let digits = &digits[..digits.len() - suffix.len()];
    let (digits, imaginary) = if imaginary {
        (digits.to_string(), imaginary)
    } else {
        let (digits, imaginary) = strip_imaginary(digits);
        (digits.to_string(), imaginary)
    };
    (digits.to_ascii_lowercase(), suffix, imaginary)
}

impl FloatLiteral {
    pub fn parse_unevaluated(spelling: &str) -> Self {
        let (fixed_suffix, base_spelling) = split_fixed_suffix(spelling);
        let (digits, suffix, imaginary) = split_float_spelling(&base_spelling);
        let radix = if digits.starts_with("0x") || digits.starts_with("0X") {
            FloatRadix::Hex
        } else {
            FloatRadix::Decimal
        };
        Self {
            spelling: spelling.to_string(),
            radix,
            suffix: if fixed_suffix.is_some() {
                FloatSuffix::None
            } else {
                float_suffix_from_token(suffix)
            },
            fixed_suffix,
            imaginary,
        }
    }
}

fn split_fixed_suffix(spelling: &str) -> (Option<FixedPointLiteralSuffix>, String) {
    let lower = spelling.to_ascii_lowercase();
    let chars: Vec<char> = lower.chars().collect();
    let Some(last) = chars.last().copied() else {
        return (None, spelling.to_string());
    };
    let kind = match last {
        'r' => FixedPointKind::Fract,
        'k' => FixedPointKind::Accum,
        _ => return (None, spelling.to_string()),
    };
    let marker = chars.len() - 1;
    let suffix_start = chars[..marker]
        .iter()
        .rposition(|c| !matches!(c, 'u' | 'h' | 'l'))
        .map_or(0, |i| i + 1);
    let mut letters: String = chars[suffix_start..marker].iter().collect();
    let unsigned = letters.contains('u');
    letters.retain(|c| c != 'u');
    let rank = match letters.as_str() {
        "" => FixedPointRank::Default,
        "h" => FixedPointRank::Short,
        "l" => FixedPointRank::Long,
        "ll" => FixedPointRank::LongLong,
        _ => return (None, spelling.to_string()),
    };
    let start = suffix_start;
    let prefix: String = chars[..start].iter().collect();
    if prefix.is_empty() || !prefix.chars().any(|c| c.is_ascii_digit()) {
        return (None, spelling.to_string());
    }
    (
        Some(FixedPointLiteralSuffix {
            kind,
            rank,
            unsigned,
        }),
        prefix,
    )
}

pub fn resolve_float(literal: &FloatLiteral) -> Result<ResolvedFloat, ConstExprError> {
    let invalid = || ConstExprError::InvalidFloatLiteral(literal.spelling.clone());
    let (digits, _, _) = split_float_spelling(&literal.spelling);
    let token = Token::FloatLit(digits.clone());
    let value = match literal.suffix {
        FloatSuffix::None | FloatSuffix::F64 | FloatSuffix::F32x => {
            FloatValue::Double(token.float_value_f64().ok_or_else(invalid)?)
        }
        FloatSuffix::F | FloatSuffix::F32 => {
            FloatValue::Single(token.float_value_f32().ok_or_else(invalid)?)
        }
        FloatSuffix::BF16 => FloatValue::BFloat16(token.float_value_bf16().ok_or_else(invalid)?),
        FloatSuffix::F16 => FloatValue::Half(token.float_value_f16().ok_or_else(invalid)?),
        FloatSuffix::F128 | FloatSuffix::Q => {
            FloatValue::Quad(token.float_value_f128().ok_or_else(invalid)?)
        }
        FloatSuffix::L | FloatSuffix::F64x => {
            FloatValue::LongDouble(token.float_value_f80().ok_or_else(invalid)?)
        }
        FloatSuffix::DecimalF32 => FloatValue::Decimal32(digits),
        FloatSuffix::DecimalF64 => FloatValue::Decimal64(digits),
        FloatSuffix::DecimalF128 => FloatValue::Decimal128(digits),
    };
    Ok(ResolvedFloat {
        value,
        imaginary: literal.imaginary,
    })
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Encoding {
    Plain,
    Utf8,
    Utf16,
    Utf32,
    Wide,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct CharLiteral {
    pub encoding: Encoding,
    pub code_units: Vec<u32>,
    pub spelling: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct StringLiteral {
    pub encoding: Encoding,
    pub code_units: Vec<u32>,
    pub pieces: Vec<Span<String>>,
}

impl Encoding {
    pub fn unit_width(self, wchar_width: u32) -> u32 {
        match self {
            Encoding::Plain | Encoding::Utf8 => 8,
            Encoding::Utf16 => 16,
            Encoding::Utf32 => 32,
            Encoding::Wide => wchar_width,
        }
    }
}

impl CharLiteral {
    pub fn execution_units(&self, wchar_width: u32) -> Vec<u32> {
        execution_units(&self.spelling, self.encoding.unit_width(wchar_width))
    }

    // signedness of the character type behind the constant, not of its C type:
    // a plain constant is int in C but follows char in preprocessor arithmetic
    pub fn char_type_is_signed(&self, target: &TargetInfo) -> bool {
        match self.encoding {
            Encoding::Plain => target.char_signed,
            Encoding::Wide => target.wchar_signed,
            Encoding::Utf8 | Encoding::Utf16 | Encoding::Utf32 => false,
        }
    }

    pub fn value(&self, target: &TargetInfo) -> Result<i64, ConstExprError> {
        let units = self.execution_units(target.wchar_width);
        if self.encoding == Encoding::Plain {
            return match units.as_slice() {
                [] => Err(ConstExprError::CharacterConstant(
                    "empty character constant",
                )),
                [single] => Ok(wrap_to_width(u64::from(*single), 8, target.char_signed)),
                multiple => {
                    let packed = multiple
                        .iter()
                        .fold(0u64, |acc, &byte| (acc << 8) | u64::from(byte & 0xFF));
                    Ok(wrap_to_width(packed, target.int_width, true))
                }
            };
        }
        let [single] = units.as_slice() else {
            return Err(ConstExprError::CharacterConstant(match self.encoding {
                Encoding::Wide => "wide character literals may not contain multiple characters",
                _ => "Unicode character literals may not contain multiple characters",
            }));
        };
        let width = self.encoding.unit_width(target.wchar_width);
        let signed = self.encoding == Encoding::Wide && target.wchar_signed;
        Ok(wrap_to_width(u64::from(*single), width, signed))
    }
}

pub(crate) fn wrap_to_width(value: u64, width: u32, signed: bool) -> i64 {
    let value = value & (u64::MAX >> (u64::BITS - width));
    if signed && value >> (width - 1) != 0 {
        value as i64 - (1i64 << width)
    } else {
        value as i64
    }
}

#[derive(Clone, Copy, Default)]
pub struct EvalContext<'a> {
    is_defined: Option<&'a dyn Fn(&str) -> bool>,
    target: Option<&'a TargetInfo>,
}

impl StringLiteral {
    pub fn unit_width(&self, wchar_width: u32) -> u32 {
        self.encoding.unit_width(wchar_width)
    }

    // code_units holds characters; sizes and contents need execution-charset units
    pub fn execution_units(&self, wchar_width: u32) -> Vec<u32> {
        let width = self.unit_width(wchar_width);
        let mut units = Vec::with_capacity(self.code_units.len());
        for piece in &self.pieces {
            units.extend(execution_units(&piece.value, width));
        }
        units
    }
}

fn execution_units(spelling: &str, width: u32) -> Vec<u32> {
    let chars: Vec<char> = spelling.chars().collect();
    let mut units = Vec::with_capacity(chars.len());
    for unit in crate::lexer::Lexer::decode_source_units(&chars, 0, chars.len()) {
        match unit {
            crate::lexer::SourceUnit::CodeUnit(value) => units.push(truncate(value, width)),
            crate::lexer::SourceUnit::Character(value) => encode(value, width, &mut units),
        }
    }
    units
}

fn truncate(value: u32, width: u32) -> u32 {
    match width {
        32 => value,
        width => value & ((1 << width) - 1),
    }
}

fn encode(value: u32, width: u32, units: &mut Vec<u32>) {
    match (width, value) {
        (8, 0..=0x7F) | (16, 0..=0xFFFF) | (32, _) => units.push(value),
        (8, 0x80..=0x7FF) => units.extend([0xC0 | value >> 6, 0x80 | value & 0x3F]),
        (8, 0x800..=0xFFFF) => units.extend([
            0xE0 | value >> 12,
            0x80 | (value >> 6) & 0x3F,
            0x80 | value & 0x3F,
        ]),
        (8, _) => units.extend([
            0xF0 | value >> 18,
            0x80 | (value >> 12) & 0x3F,
            0x80 | (value >> 6) & 0x3F,
            0x80 | value & 0x3F,
        ]),
        (16, _) => {
            let value = value - 0x1_0000;
            units.extend([0xD800 | value >> 10, 0xDC00 | value & 0x3FF]);
        }
        _ => units.push(truncate(value, width)),
    }
}

fn strip_imaginary(digits: &str) -> (&str, bool) {
    match digits.strip_suffix(['i', 'I', 'j', 'J']) {
        Some(stripped) => (stripped, true),
        None => (digits, false),
    }
}

fn integer_literal_wide(literal: &IntegerLiteral) -> WideInt {
    let signed = !literal.suffix.unsigned;
    let width = literal.value.bits() as u32 + u32::from(signed);
    WideInt::wrap(BigInt::from(literal.value.clone()), width.max(2), signed)
}

fn integer_literal_i64(literal: &IntegerLiteral) -> i64 {
    if literal.value.bits() > 64 {
        return integer_literal_wide(literal).truncate_to_i64();
    }
    let bytes = literal.value.to_bytes_le();
    let mut buf = [0u8; 8];
    buf[..bytes.len()].copy_from_slice(&bytes);
    i64::from_le_bytes(buf)
}

fn parse_integer_suffix(spelling: &str, digits_len: usize) -> IntegerSuffix {
    let suffix = spelling[digits_len..].to_ascii_lowercase();
    let unsigned = suffix.contains('u');
    let size = if suffix.contains("wb") {
        IntegerSizeSuffix::BitInt
    } else {
        match suffix.matches('l').count() {
            0 => IntegerSizeSuffix::None,
            1 => IntegerSizeSuffix::Long,
            _ => IntegerSizeSuffix::LongLong,
        }
    };
    IntegerSuffix { unsigned, size }
}

fn parse_integer_literal(spelling: &str) -> IntegerLiteral {
    let digits = Lexer::integer_digits(spelling);
    let suffix = parse_integer_suffix(spelling, digits.len());
    let cleaned = digits.replace('\'', "");
    let (radix, radix_num, digits) = if cleaned.starts_with("0x") || cleaned.starts_with("0X") {
        (Radix::Hex, 16, &cleaned[2..])
    } else if cleaned.starts_with("0b") || cleaned.starts_with("0B") {
        (Radix::Binary, 2, &cleaned[2..])
    } else if cleaned.len() > 1 && cleaned.starts_with('0') {
        (Radix::Octal, 8, &cleaned[1..])
    } else {
        (Radix::Decimal, 10, cleaned.as_str())
    };
    let value = if digits.is_empty() {
        BigUint::default()
    } else {
        BigUint::parse_bytes(digits.as_bytes(), radix_num).unwrap_or_default()
    };
    IntegerLiteral {
        value,
        radix,
        suffix,
        spelling: spelling.to_string(),
        imaginary: Lexer::is_imaginary_integer(spelling),
    }
}

fn string_literal_encoding(token: &Token) -> Encoding {
    match token {
        Token::StringLit(_) => Encoding::Plain,
        Token::Utf8StringLit(_) => Encoding::Utf8,
        Token::Utf16StringLit(_) => Encoding::Utf16,
        Token::Utf32StringLit(_) => Encoding::Utf32,
        Token::WideStringLit(_) => Encoding::Wide,
        _ => Encoding::Plain,
    }
}

fn merge_string_encoding(current: Encoding, next: Encoding) -> Encoding {
    if current == Encoding::Plain {
        next
    } else {
        current
    }
}

fn fold_char_code_units(units: &[u32]) -> i64 {
    match units {
        [] => 0,
        [single] => i64::from(*single),
        multiple => multiple.iter().fold(0i64, |acc, &codepoint| {
            (acc << 8) | i64::from(codepoint as u8)
        }),
    }
}

fn contains_wide(expression: &Expr) -> bool {
    match &expression.value {
        ExprKind::IntegerLiteral(literal) => literal.value.bits() > 64,
        ExprKind::Paren(operand) | ExprKind::Unary { operand, .. } => contains_wide(operand),
        ExprKind::Cast { ty, value, .. } => bit_int_width(ty).is_some() || contains_wide(value),
        ExprKind::Binary { left, right, .. } => contains_wide(left) || contains_wide(right),
        ExprKind::Conditional {
            condition,
            then_value,
            else_value,
        } => contains_wide(then_value.as_ref().unwrap_or(condition)) || contains_wide(else_value),
        ExprKind::Comma { right, .. } => contains_wide(right),
        _ => false,
    }
}

fn identifier(expression: &Expr) -> Option<&str> {
    match &expression.value {
        ExprKind::Identifier(name) => Some(name),
        _ => None,
    }
}

impl std::fmt::Display for ResolvedFloat {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match &self.value {
            FloatValue::BFloat16(bits) => write!(formatter, "bf16:{bits:#x}")?,
            FloatValue::Half(bits) => write!(formatter, "f16:{bits:#x}")?,
            FloatValue::Single(value) => write!(formatter, "{value}f")?,
            FloatValue::Double(value) => write!(formatter, "{value}")?,
            FloatValue::Quad(bits) => write!(formatter, "f128:{bits:#x}")?,
            FloatValue::LongDouble(bits) => write!(formatter, "f80:{bits:#x}")?,
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

pub struct Parser<'a> {
    tokens: &'a [Span<Token>],
    span_ranges: Option<SpanRangeIndex>,
    position: usize,
    context: Option<&'a crate::parser::Parser>,
    nesting: Cell<u32>,
}

impl<'a> Parser<'a> {
    fn features(&self) -> StandardFeatures {
        self.context
            .map_or_else(StandardFeatures::default, |parser| parser.features())
    }

    pub fn parse(tokens: &'a [Span<Token>]) -> Result<Expr, ConstExprError> {
        let mut parser = Self::new(tokens, None);
        let expression = parser.parse_conditional()?;
        if parser.peek().is_some() {
            return Err(ConstExprError::UnexpectedTokens);
        }
        Ok(expression)
    }

    pub(crate) fn parse_expression(
        tokens: &'a [Span<Token>],
        context: Option<&'a crate::parser::Parser>,
    ) -> Result<Expr, ConstExprError> {
        let mut parser = Self::new(tokens, context);
        let expression = parser.parse_comma()?;
        if parser.peek().is_some() {
            return Err(ConstExprError::UnexpectedTokens);
        }
        Ok(expression)
    }

    pub(crate) fn parse_one(
        tokens: &'a [Span<Token>],
        start: usize,
        context: Option<&'a crate::parser::Parser>,
    ) -> Result<(Expr, usize), ConstExprError> {
        let mut parser = Self::new(tokens, context);
        parser.position = start;
        let expression = parser.parse_assignment()?;
        Ok((expression, parser.position))
    }

    pub(crate) fn parse_initializer(
        tokens: &'a [Span<Token>],
        start: usize,
        context: Option<&'a crate::parser::Parser>,
    ) -> Result<(Initializer, usize), ConstExprError> {
        let mut parser = Self::new(tokens, context);
        parser.position = start;
        let initializer = parser.parse_initializer_value()?;
        Ok((initializer, parser.position))
    }

    pub fn evaluate(tokens: &'a [Span<Token>]) -> Result<i64, ConstExprError> {
        Self::evaluate_expr(&Self::parse(tokens)?, EvalContext::default())
    }

    pub fn evaluate_ast(expression: &Expr) -> Result<i64, ConstExprError> {
        Self::evaluate_expr(expression, EvalContext::default())
    }

    pub fn evaluate_with_defined(
        tokens: &'a [Span<Token>],
        target: &TargetInfo,
        is_defined: &dyn Fn(&str) -> bool,
    ) -> Result<i64, LocatedConstExprError> {
        let mut parser = Self::new(tokens, None);
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
        let ctx = EvalContext {
            is_defined: Some(is_defined),
            target: Some(target),
        };
        Self::evaluate_wide(&expression, ctx)
            .map(|value| i64::from(!value.is_zero()))
            .map_err(|error| LocatedConstExprError { error, token: None })
    }

    fn evaluate_expr(expression: &Expr, ctx: EvalContext<'_>) -> Result<i64, ConstExprError> {
        match &expression.value {
            ExprKind::IntegerLiteral(literal) if literal.imaginary => {
                Err(ConstExprError::NotConstant("imaginary literal"))
            }
            ExprKind::IntegerLiteral(literal) => Ok(integer_literal_i64(literal)),
            ExprKind::CharLiteral(literal) => match ctx.target {
                Some(target) => literal.value(target),
                None => Ok(fold_char_code_units(&literal.code_units)),
            },
            ExprKind::StringLiteral(_) => Err(ConstExprError::NotConstant("string literal")),
            ExprKind::Generic { .. } => Err(ConstExprError::RequiresSemanticEvaluation(
                "generic selection",
            )),
            ExprKind::FloatLiteral(_) => Err(ConstExprError::NotConstant("floating literal")),
            ExprKind::Identifier(name) => match ctx.is_defined {
                Some(_) if name == "true" => Ok(1),
                Some(_) => Ok(0),
                None => Err(ConstExprError::UnsupportedIdentifier(name.clone())),
            },
            ExprKind::Paren(inner) => Self::evaluate_expr(inner, ctx),
            ExprKind::SizeOfExpr(_) => Err(ConstExprError::UnsupportedSizeOf),
            ExprKind::SizeOfType { .. } => Err(ConstExprError::UnsupportedSizeOf),
            ExprKind::AlignOf { .. } => Err(ConstExprError::UnsupportedAlignOf),
            ExprKind::AlignOfExpr(_) => Err(ConstExprError::UnsupportedAlignOf),
            ExprKind::OffsetOf { .. } => Err(ConstExprError::NotConstant("offsetof")),
            ExprKind::TypesCompatible { .. } => {
                Err(ConstExprError::NotConstant("types compatible"))
            }
            ExprKind::Call { callee, arguments } => {
                let argument = match arguments.as_slice() {
                    [argument] => identifier(argument),
                    _ => None,
                };
                match (ctx.is_defined, identifier(callee), argument) {
                    (Some(is_defined), Some("defined"), Some(macro_name)) => {
                        Ok(is_defined(macro_name) as i64)
                    }
                    (
                        Some(_),
                        Some("__has_c_attribute" | "__has_cpp_attribute" | "__building_module"),
                        Some(_),
                    ) => Ok(0),
                    _ => Err(ConstExprError::UnsupportedCall(callee.to_string())),
                }
            }
            ExprKind::Cast { ty, .. } if bit_int_width(ty).is_some() => {
                Self::evaluate_wide(expression, ctx).map(|wide| wide.truncate_to_i64())
            }
            ExprKind::Cast { value, .. } => Self::evaluate_expr(value, ctx),
            ExprKind::BitCast { .. } => Err(ConstExprError::NotConstant("bit cast")),
            ExprKind::ConvertVector { .. } => Err(ConstExprError::NotConstant("convertvector")),
            ExprKind::VaArg { .. } => Err(ConstExprError::NotConstant("va_arg")),
            ExprKind::Unary {
                op: UnaryOp::AddrOf,
                ..
            } => Err(ConstExprError::NotConstant("address-of")),
            ExprKind::Unary {
                op: UnaryOp::Deref, ..
            } => Err(ConstExprError::NotConstant("dereference")),
            ExprKind::Unary {
                op: UnaryOp::PreIncrement,
                ..
            }
            | ExprKind::Postfix {
                op: PostfixOp::Increment,
                ..
            } => Err(ConstExprError::NotConstant("increment")),
            ExprKind::Unary {
                op: UnaryOp::PreDecrement,
                ..
            }
            | ExprKind::Postfix {
                op: PostfixOp::Decrement,
                ..
            } => Err(ConstExprError::NotConstant("decrement")),
            ExprKind::Unary { op, operand } => {
                if contains_wide(operand) {
                    return Self::evaluate_wide(expression, ctx).map(|wide| wide.truncate_to_i64());
                }
                let value = Self::evaluate_expr(operand, ctx)?;
                match op {
                    UnaryOp::Plus | UnaryOp::Real => Ok(value),
                    UnaryOp::Minus => value.checked_neg().ok_or(ConstExprError::IntegerOverflow),
                    UnaryOp::BitNot => Ok(!value),
                    UnaryOp::Not => Ok((value == 0) as i64),
                    UnaryOp::Imag => Ok(0),
                    UnaryOp::AddrOf
                    | UnaryOp::Deref
                    | UnaryOp::PreIncrement
                    | UnaryOp::PreDecrement => Err(ConstExprError::InvalidIntegerConstant),
                }
            }
            ExprKind::Binary { op, left, right } => {
                if contains_wide(left) || contains_wide(right) {
                    return Self::evaluate_wide(expression, ctx).map(|wide| wide.truncate_to_i64());
                }
                let left = Self::evaluate_expr(left, ctx)?;
                if *op == BinaryOp::And && left == 0 || *op == BinaryOp::Or && left != 0 {
                    return Ok((*op == BinaryOp::Or) as i64);
                }
                let right = Self::evaluate_expr(right, ctx)?;
                match op {
                    BinaryOp::Add => Ok((left as i128 + right as i128) as i64),
                    BinaryOp::Sub => Ok((left as i128 - right as i128) as i64),
                    BinaryOp::Mul => Ok((left as i128 * right as i128) as i64),
                    BinaryOp::Div => left
                        .checked_div(right)
                        .ok_or(ConstExprError::InvalidIntegerConstant),
                    BinaryOp::Rem => left
                        .checked_rem(right)
                        .ok_or(ConstExprError::InvalidIntegerConstant),
                    BinaryOp::Less => Ok((left < right) as i64),
                    BinaryOp::LessEqual => Ok((left <= right) as i64),
                    BinaryOp::Greater => Ok((left > right) as i64),
                    BinaryOp::GreaterEqual => Ok((left >= right) as i64),
                    BinaryOp::Equal => Ok((left == right) as i64),
                    BinaryOp::NotEqual => Ok((left != right) as i64),
                    BinaryOp::BitAnd => Ok(left & right),
                    BinaryOp::BitXor => Ok(left ^ right),
                    BinaryOp::BitOr => Ok(left | right),
                    BinaryOp::And => Ok(((left != 0) && (right != 0)) as i64),
                    BinaryOp::Or => Ok(((left != 0) || (right != 0)) as i64),
                    BinaryOp::ShiftLeft => left
                        .checked_shl(right as u32)
                        .ok_or(ConstExprError::InvalidIntegerConstant),
                    BinaryOp::ShiftRight => left
                        .checked_shr(right as u32)
                        .ok_or(ConstExprError::InvalidIntegerConstant),
                }
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                let value = Self::evaluate_expr(condition, ctx)?;
                match (value != 0, then_value) {
                    (true, Some(then_value)) => Self::evaluate_expr(then_value, ctx),
                    (true, None) => Ok(value),
                    (false, _) => Self::evaluate_expr(else_value, ctx),
                }
            }
            ExprKind::Comma { left, right } => {
                Self::evaluate_expr(left, ctx)?;
                Self::evaluate_expr(right, ctx)
            }
            ExprKind::Assign { .. } => Err(ConstExprError::NotConstant("assignment")),
            ExprKind::Member { .. } => Err(ConstExprError::NotConstant("member access")),
            ExprKind::Index { .. } => Err(ConstExprError::NotConstant("array subscript")),
            ExprKind::CompoundLiteral { .. } => {
                Err(ConstExprError::NotConstant("compound literal"))
            }
            ExprKind::LabelAddress(_) => Err(ConstExprError::NotConstant("label address")),
            ExprKind::StatementExpression(_) => {
                Err(ConstExprError::NotConstant("statement expression"))
            }
            ExprKind::BoolLiteral(value) => Ok(i64::from(*value)),
            ExprKind::NullPtrLiteral => Err(ConstExprError::NotConstant("nullptr")),
        }
    }

    fn evaluate_wide(expression: &Expr, ctx: EvalContext<'_>) -> Result<WideInt, ConstExprError> {
        match &expression.value {
            ExprKind::IntegerLiteral(literal) if literal.imaginary => {
                Err(ConstExprError::NotConstant("imaginary literal"))
            }
            ExprKind::IntegerLiteral(literal) if ctx.is_defined.is_some() => {
                if literal.value.bits() > 64 {
                    return Err(ConstExprError::IntegerOverflow);
                }
                Ok(WideInt::wrap(
                    BigInt::from(literal.value.clone()),
                    64,
                    !literal.suffix.unsigned && literal.value.bits() < 64,
                ))
            }
            ExprKind::IntegerLiteral(literal) => Ok(integer_literal_wide(literal)),
            ExprKind::CharLiteral(literal) => {
                let Some(target) = ctx.target else {
                    return Ok(WideInt::from_i64(fold_char_code_units(&literal.code_units)));
                };
                Ok(WideInt::wrap(
                    BigInt::from(literal.value(target)?),
                    64,
                    literal.char_type_is_signed(target),
                ))
            }
            ExprKind::Paren(inner) => Self::evaluate_wide(inner, ctx),
            ExprKind::Unary {
                op:
                    op @ (UnaryOp::Plus
                    | UnaryOp::Minus
                    | UnaryOp::BitNot
                    | UnaryOp::Not
                    | UnaryOp::Real
                    | UnaryOp::Imag),
                operand,
            } => {
                let value = Self::evaluate_wide(operand, ctx)?;
                Ok(match op {
                    UnaryOp::Plus | UnaryOp::Real => value,
                    UnaryOp::Minus => {
                        WideInt::wrap(-value.value.clone(), value.width, value.signed)
                    }
                    UnaryOp::BitNot => {
                        WideInt::wrap(!value.value.clone(), value.width, value.signed)
                    }
                    UnaryOp::Imag => WideInt::wrap(BigInt::from(0), value.width, value.signed),
                    _ => WideInt::from_i64(value.is_zero() as i64),
                })
            }
            ExprKind::Binary { op, left, right } => {
                let left = Self::evaluate_wide(left, ctx)?;
                if *op == BinaryOp::And && left.is_zero() || *op == BinaryOp::Or && !left.is_zero()
                {
                    return Ok(WideInt::from_i64((*op == BinaryOp::Or) as i64));
                }
                let right = Self::evaluate_wide(right, ctx)?;
                match op {
                    BinaryOp::Add => Ok(left.wrapping_add(&right)),
                    BinaryOp::Sub => Ok(left.wrapping_sub(&right)),
                    BinaryOp::Mul => Ok(left.wrapping_mul(&right)),
                    BinaryOp::Div => left
                        .checked_div(&right)
                        .ok_or(ConstExprError::InvalidIntegerConstant),
                    BinaryOp::Rem => left
                        .checked_rem(&right)
                        .ok_or(ConstExprError::InvalidIntegerConstant),
                    BinaryOp::BitAnd => Ok(left.bitand(&right)),
                    BinaryOp::BitXor => Ok(left.bitxor(&right)),
                    BinaryOp::BitOr => Ok(left.bitor(&right)),
                    BinaryOp::ShiftLeft | BinaryOp::ShiftRight => {
                        let shift = u32::try_from(&right.value)
                            .map_err(|_| ConstExprError::InvalidIntegerConstant)?;
                        if shift >= left.width {
                            return Err(ConstExprError::InvalidIntegerConstant);
                        }
                        Ok(if *op == BinaryOp::ShiftLeft {
                            left.shift_left(shift)
                        } else {
                            left.shift_right(shift)
                        })
                    }
                    BinaryOp::And => Ok(WideInt::from_i64(
                        (!left.is_zero() && !right.is_zero()) as i64,
                    )),
                    BinaryOp::Or => Ok(WideInt::from_i64(
                        (!left.is_zero() || !right.is_zero()) as i64,
                    )),
                    BinaryOp::Less
                    | BinaryOp::LessEqual
                    | BinaryOp::Greater
                    | BinaryOp::GreaterEqual
                    | BinaryOp::Equal
                    | BinaryOp::NotEqual => left
                        .compare(&right, *op)
                        .map(WideInt::from_i64)
                        .ok_or(ConstExprError::InvalidIntegerConstant),
                }
            }
            ExprKind::Cast { ty, value, .. } => {
                let value = Self::evaluate_wide(value, ctx)?;
                match bit_int_width(ty) {
                    Some((width, signed)) => Ok(WideInt::wrap(value.value, width, signed)),
                    None => Ok(value),
                }
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                let value = Self::evaluate_wide(condition, ctx)?;
                let result = match (value.is_zero(), then_value) {
                    (false, Some(then_value)) => Self::evaluate_wide(then_value, ctx),
                    (false, None) => Ok(value),
                    (true, _) => Self::evaluate_wide(else_value, ctx),
                }?;
                if ctx.is_defined.is_some() {
                    let unsigned = Self::pp_unsigned(then_value.as_ref().unwrap_or(condition))?
                        || Self::pp_unsigned(else_value)?;
                    Ok(WideInt::wrap(result.value, 64, !unsigned))
                } else {
                    Ok(result)
                }
            }
            ExprKind::Comma { left, right } => {
                Self::evaluate_wide(left, ctx)?;
                Self::evaluate_wide(right, ctx)
            }
            _ => Self::evaluate_expr(expression, ctx).map(WideInt::from_i64),
        }
    }

    fn pp_unsigned(expression: &Expr) -> Result<bool, ConstExprError> {
        Ok(match &expression.value {
            ExprKind::IntegerLiteral(literal) => literal.suffix.unsigned,
            ExprKind::Paren(inner) => Self::pp_unsigned(inner)?,
            ExprKind::Unary { operand, .. } => Self::pp_unsigned(operand)?,
            ExprKind::Cast { ty, value, .. } => match bit_int_width(ty) {
                Some((_, signed)) => !signed,
                None => Self::pp_unsigned(value)?,
            },
            ExprKind::Conditional { condition, .. } => Self::pp_unsigned(condition)?,
            ExprKind::Comma { right, .. } => Self::pp_unsigned(right)?,
            ExprKind::Binary { op, .. } => matches!(
                op,
                BinaryOp::Less
                    | BinaryOp::LessEqual
                    | BinaryOp::Greater
                    | BinaryOp::GreaterEqual
                    | BinaryOp::Equal
                    | BinaryOp::NotEqual
                    | BinaryOp::And
                    | BinaryOp::Or
            ),
            ExprKind::Call { .. } => true,
            _ => false,
        })
    }

    fn new(tokens: &'a [Span<Token>], context: Option<&'a crate::parser::Parser>) -> Self {
        let indexed_by_context = context.is_some_and(|parser| parser.has_token_slice(tokens));
        Self {
            tokens,
            span_ranges: (!indexed_by_context).then(|| SpanRangeIndex::new(tokens)),
            position: 0,
            context,
            nesting: Cell::new(0),
        }
    }

    fn nested<T>(
        &mut self,
        parse: impl FnOnce(&mut Self) -> Result<T, ConstExprError>,
    ) -> Result<T, ConstExprError> {
        let depth = self.nesting().get();
        if depth >= crate::parser::NESTING_LIMIT {
            return Err(ConstExprError::NestingTooDeep);
        }
        self.nesting().set(depth + 1);
        let parsed = parse(self);
        self.nesting().set(depth);
        parsed
    }

    fn nesting(&self) -> &Cell<u32> {
        self.context
            .map_or(&self.nesting, crate::parser::Parser::nesting)
    }

    fn node(&self, kind: ExprKind, start: usize) -> Expr {
        Box::new(self.cover_span(kind, start, self.position))
    }

    fn cover_span<T>(&self, value: T, start: usize, end: usize) -> Span<T> {
        if let Some(span_ranges) = &self.span_ranges {
            span_ranges.cover(value, self.tokens, start, end)
        } else if let Some(context) = self.context {
            context.cover_tokens(value, self.tokens, start, end)
        } else {
            Span::cover(value, &self.tokens[start..end])
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

    fn parse_comma(&mut self) -> Result<Expr, ConstExprError> {
        let start = self.position;
        let mut expression = self.parse_assignment()?;
        while self.consume(&Token::Comma) {
            let right = self.parse_assignment()?;
            expression = self.node(
                ExprKind::Comma {
                    left: expression,
                    right,
                },
                start,
            );
        }
        Ok(expression)
    }

    fn parse_assignment(&mut self) -> Result<Expr, ConstExprError> {
        let start = self.position;
        let target = self.parse_conditional()?;
        if let Some(op) = self.assignment_operator() {
            self.take();
            let value = self.parse_assignment()?;
            return Ok(self.node(ExprKind::Assign { op, target, value }, start));
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

    fn parse_conditional(&mut self) -> Result<Expr, ConstExprError> {
        let start = self.position;
        let condition = self.parse_binary(0)?;
        if !self.consume(&Token::Question) {
            return Ok(condition);
        }
        let then_value = if self.peek() == Some(&Token::Colon) {
            None
        } else {
            Some(self.parse_comma()?)
        };
        self.expect(Token::Colon)?;
        let else_value = self.parse_conditional()?;
        Ok(self.node(
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            },
            start,
        ))
    }

    fn parse_binary(&mut self, minimum_precedence: u8) -> Result<Expr, ConstExprError> {
        let start = self.position;
        let mut left = self.parse_cast()?;
        while let Some((op, precedence)) = self.binary_operator() {
            if precedence < minimum_precedence {
                break;
            }
            self.take();
            let right = self.parse_binary(precedence + 1)?;
            left = self.node(ExprKind::Binary { op, left, right }, start);
        }
        Ok(left)
    }

    fn parse_cast(&mut self) -> Result<Expr, ConstExprError> {
        self.nested(Self::cast_expression)
    }

    fn cast_expression(&mut self) -> Result<Expr, ConstExprError> {
        let start = self.position;
        if self.peek() == Some(&Token::LParen)
            && let Some(next) = self.peek_at(1)
            && starts_type_name(next, self.context)
            && let Some((ty, end)) = self.try_parse_type_name(self.position + 1, |end| {
                self.token_at(end) == Some(&Token::RParen)
                    && self.token_at(end + 1) != Some(&Token::LBrace)
            })
        {
            self.position = end + 1;
            let value = self.parse_cast()?;
            return Ok(self.node(ExprKind::Cast { ty, value }, start));
        }
        self.parse_unary()
    }

    fn try_parse_compound_literal(&mut self) -> Result<Option<Expr>, ConstExprError> {
        let start = self.position;
        if self.peek() != Some(&Token::LParen) {
            return Ok(None);
        }
        let Some(next) = self.peek_at(1) else {
            return Ok(None);
        };
        if !starts_type_name(next, self.context) {
            return Ok(None);
        }
        let Some((ty, end)) = self.try_parse_type_name(self.position + 1, |end| {
            self.token_at(end) == Some(&Token::RParen)
                && self.token_at(end + 1) == Some(&Token::LBrace)
        }) else {
            return Ok(None);
        };
        self.position = end + 1;
        let initializer = self.parse_initializer_list()?;
        Ok(Some(self.node(
            ExprKind::CompoundLiteral { ty, initializer },
            start,
        )))
    }

    fn parse_initializer_list(&mut self) -> Result<Vec<InitializerItem>, ConstExprError> {
        let opening = self.take();
        if opening != Some(Token::LBrace) {
            return Err(opening.map_or(
                ConstExprError::Expected {
                    expected: Token::LBrace,
                    found: None,
                },
                ConstExprError::UnexpectedToken,
            ));
        }
        let mut items = Vec::new();
        while self.peek() != Some(&Token::RBrace) {
            let mut designators = Vec::new();
            loop {
                if self.peek() == Some(&Token::LBracket) {
                    self.take();
                    let index = self.parse_conditional()?;
                    if self.consume(&Token::Ellipsis) {
                        let end = self.parse_conditional()?;
                        designators.push(Designator::ArrayRange { start: index, end });
                    } else {
                        designators.push(Designator::Array(index));
                    }
                    self.expect(Token::RBracket)?;
                } else if self.peek() == Some(&Token::Dot) {
                    self.take();
                    designators.push(Designator::Field(self.expect_field_name()?));
                } else if let Some(Token::Ident(name)) = self.peek().cloned()
                    && self.peek_at(1) == Some(&Token::Colon)
                {
                    let start = self.position;
                    self.position += 2;
                    designators.push(Designator::Field(self.cover_span(name, start, start + 1)));
                } else {
                    break;
                }
            }
            if !designators.is_empty() {
                self.consume(&Token::Equal);
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
        self.nested(Self::initializer_value)
    }

    fn initializer_value(&mut self) -> Result<Initializer, ConstExprError> {
        if self.peek() == Some(&Token::LBrace) {
            Ok(Initializer::List(self.parse_initializer_list()?))
        } else {
            Ok(Initializer::Expr(self.parse_assignment()?))
        }
    }

    fn try_parse_type_name(
        &self,
        start: usize,
        accepts: impl FnOnce(usize) -> bool,
    ) -> Option<(Box<TypeName>, usize)> {
        let checkpoint = self.context.map(crate::parser::Parser::checkpoint);
        let mut declarator_parser = DeclaratorParser::new(self.tokens, start, self.context);
        let type_name = declarator_parser.parse_type_name().ok()?;
        let end = declarator_parser.position();
        if !accepts(end) {
            return None;
        }
        if let Some(checkpoint) = checkpoint {
            checkpoint.commit();
        }
        Some((Box::new(type_name), end))
    }

    pub(crate) fn try_parse_full_type_name(
        tokens: &'a [Span<Token>],
        context: Option<&'a crate::parser::Parser>,
    ) -> Option<Box<TypeName>> {
        let first = &tokens.first()?.value;
        if !starts_type_name(first, context) {
            return None;
        }
        let parser = Self::new(tokens, context);
        let (ty, _) = parser.try_parse_type_name(0, |end| end == tokens.len())?;
        Some(ty)
    }

    fn parse_unary(&mut self) -> Result<Expr, ConstExprError> {
        self.nested(Self::unary_expression)
    }

    fn unary_expression(&mut self) -> Result<Expr, ConstExprError> {
        let start = self.position;
        if let Some(Token::Ident(name)) = self.peek()
            && name == "__extension__"
        {
            self.take();
            return self.parse_cast();
        }
        if self.peek() == Some(&Token::Sizeof)
            && self.peek_at(1) == Some(&Token::LParen)
            && let Some(next) = self.peek_at(2)
            && starts_type_name(next, self.context)
            && let Some((ty, end)) = self.try_parse_type_name(self.position + 2, |end| {
                self.token_at(end) == Some(&Token::RParen)
                    && self.token_at(end + 1) != Some(&Token::LBrace)
            })
        {
            self.position = end + 1;
            return Ok(self.node(ExprKind::SizeOfType { ty }, start));
        }
        if self.consume(&Token::Sizeof) {
            let operand = self.parse_unary()?;
            return Ok(self.node(ExprKind::SizeOfExpr(operand), start));
        }
        if self.peek() == Some(&Token::Alignof)
            && self.peek_at(1) == Some(&Token::LParen)
            && let Some(next) = self.peek_at(2)
            && starts_type_name(next, self.context)
            && let Some((ty, end)) = self.try_parse_type_name(self.position + 2, |end| {
                self.token_at(end) == Some(&Token::RParen)
            })
        {
            self.position = end + 1;
            return Ok(self.node(ExprKind::AlignOf { ty }, start));
        }
        if self.consume(&Token::Alignof) {
            let operand = self.parse_unary()?;
            return Ok(self.node(ExprKind::AlignOfExpr(operand), start));
        }
        if let Some(Token::Ident(name)) = self.peek()
            && matches!(name.as_str(), "__real__" | "__imag__" | "__real" | "__imag")
        {
            let op = if name.starts_with("__real") {
                UnaryOp::Real
            } else {
                UnaryOp::Imag
            };
            self.take();
            let operand = self.parse_unary()?;
            return Ok(self.node(ExprKind::Unary { op, operand }, start));
        }
        let op = match self.peek() {
            Some(Token::Plus) => UnaryOp::Plus,
            Some(Token::Minus) => UnaryOp::Minus,
            Some(Token::Tilde) => UnaryOp::BitNot,
            Some(Token::Bang) => UnaryOp::Not,
            Some(Token::PlusPlus) => UnaryOp::PreIncrement,
            Some(Token::MinusMinus) => UnaryOp::PreDecrement,
            Some(Token::Amp) => UnaryOp::AddrOf,
            Some(Token::Star) => UnaryOp::Deref,
            Some(Token::AndAnd) => {
                self.take();
                let label_start = self.position;
                return match self.take() {
                    Some(Token::Ident(label)) => {
                        let label = self.cover_span(label, label_start, self.position);
                        Ok(self.node(ExprKind::LabelAddress(label), start))
                    }
                    Some(token) => Err(ConstExprError::UnexpectedToken(token)),
                    None => Err(ConstExprError::ExpectedIdentifier),
                };
            }
            _ => return self.parse_postfix(),
        };
        self.take();
        let operand = match op {
            UnaryOp::PreIncrement | UnaryOp::PreDecrement => self.parse_unary()?,
            _ => self.parse_cast()?,
        };
        Ok(self.node(ExprKind::Unary { op, operand }, start))
    }

    fn parse_postfix(&mut self) -> Result<Expr, ConstExprError> {
        let start = self.position;
        let mut expression = match self.try_parse_compound_literal()? {
            Some(result) => result,
            None => self.parse_primary()?,
        };
        loop {
            let kind = match self.peek() {
                Some(Token::LParen) => {
                    self.take();
                    let mut arguments = Vec::new();
                    if self.peek() != Some(&Token::RParen) {
                        loop {
                            arguments.push(self.parse_assignment()?);
                            if !self.consume(&Token::Comma) {
                                break;
                            }
                        }
                    }
                    self.expect(Token::RParen)?;
                    ExprKind::Call {
                        callee: expression,
                        arguments,
                    }
                }
                Some(Token::Dot | Token::Arrow) => {
                    let arrow = self.take() == Some(Token::Arrow);
                    ExprKind::Member {
                        base: expression,
                        field: self.expect_field_name()?,
                        arrow,
                    }
                }
                Some(Token::LBracket) => {
                    self.take();
                    let index = self.parse_comma()?;
                    self.expect(Token::RBracket)?;
                    ExprKind::Index {
                        base: expression,
                        index,
                    }
                }
                Some(Token::PlusPlus) => {
                    self.take();
                    ExprKind::Postfix {
                        op: PostfixOp::Increment,
                        operand: expression,
                    }
                }
                Some(Token::MinusMinus) => {
                    self.take();
                    ExprKind::Postfix {
                        op: PostfixOp::Decrement,
                        operand: expression,
                    }
                }
                _ => break,
            };
            expression = self.node(kind, start);
        }
        Ok(expression)
    }

    fn expect_field_name(&mut self) -> Result<Span<String>, ConstExprError> {
        let start = self.position;
        match self.take() {
            Some(Token::Ident(name)) => Ok(self.cover_span(name, start, self.position)),
            Some(token) => Err(ConstExprError::UnexpectedToken(token)),
            None => Err(ConstExprError::ExpectedIdentifier),
        }
    }

    fn parse_primary(&mut self) -> Result<Expr, ConstExprError> {
        let start = self.position;
        if self.peek() == Some(&Token::LParen) && self.peek_at(1) == Some(&Token::LBrace) {
            return self.parse_statement_expression();
        }
        if self.take() == Some(Token::LParen) {
            let expression = self.parse_comma()?;
            self.expect(Token::RParen)?;
            return Ok(self.node(ExprKind::Paren(expression), start));
        }
        let kind = match self.tokens.value_at(self.position.saturating_sub(1)) {
            Some(Token::IntLit(value)) => ExprKind::IntegerLiteral(parse_integer_literal(value)),
            Some(Token::FloatLit(value)) => {
                ExprKind::FloatLiteral(FloatLiteral::parse_unevaluated(value))
            }
            Some(
                token @ (Token::StringLit(_)
                | Token::Utf8StringLit(_)
                | Token::Utf16StringLit(_)
                | Token::Utf32StringLit(_)
                | Token::WideStringLit(_)),
            ) => {
                let first_index = self.position - 1;
                let mut encoding = string_literal_encoding(token);
                let Some(content) = crate::parser::string_literal_content(token) else {
                    return Err(ConstExprError::UnexpectedToken(token.clone()));
                };
                let mut pieces = vec![
                    self.tokens[first_index]
                        .clone()
                        .with_value(content.to_string()),
                ];
                while let Some(next) = self.peek()
                    && let Some(content) = crate::parser::string_literal_content(next)
                {
                    encoding = merge_string_encoding(encoding, string_literal_encoding(next));
                    let index = self.position;
                    pieces.push(self.tokens[index].clone().with_value(content.to_string()));
                    self.take();
                }
                let code_units = pieces
                    .iter()
                    .flat_map(|piece| {
                        let chars: Vec<char> = piece.value.chars().collect();
                        Lexer::decode_escapes(&chars, 0, chars.len())
                    })
                    .collect();
                ExprKind::StringLiteral(StringLiteral {
                    encoding,
                    code_units,
                    pieces,
                })
            }
            Some(
                token @ (Token::CharLit(_, _)
                | Token::Utf8CharLit(_, _)
                | Token::Utf16CharLit(_, _)
                | Token::Utf32CharLit(_, _)
                | Token::WideCharLit(_, _)),
            ) => {
                let (encoding, spelling, code_units) = match token {
                    Token::CharLit(spelling, units) => (Encoding::Plain, spelling, units),
                    Token::Utf8CharLit(spelling, units) => (Encoding::Utf8, spelling, units),
                    Token::Utf16CharLit(spelling, units) => (Encoding::Utf16, spelling, units),
                    Token::Utf32CharLit(spelling, units) => (Encoding::Utf32, spelling, units),
                    Token::WideCharLit(spelling, units) => (Encoding::Wide, spelling, units),
                    _ => return Err(ConstExprError::UnexpectedToken(token.clone())),
                };
                ExprKind::CharLiteral(CharLiteral {
                    encoding,
                    code_units: code_units.clone(),
                    spelling: spelling.clone(),
                })
            }
            Some(Token::Ident(value)) if value == "defined" => return self.parse_defined(start),
            Some(Token::Ident(value)) if value == "__builtin_offsetof" => {
                return self.parse_offsetof(start);
            }
            Some(Token::Ident(value)) if value == "__builtin_bit_cast" => {
                return self.parse_bit_cast(start);
            }
            Some(Token::Ident(value)) if value == "__builtin_va_arg" => {
                return self.parse_va_arg(start);
            }
            Some(Token::Ident(value)) if value == "__builtin_convertvector" => {
                return self.parse_convert_vector(start);
            }
            Some(Token::Ident(value)) if value == "__builtin_types_compatible_p" => {
                return self.parse_types_compatible(start);
            }
            Some(Token::Ident(value)) if value == "_Generic" => return self.parse_generic(start),
            Some(Token::Ident(value))
                if value == "true" && self.features().keyword_bool_true_false.is_accepted() =>
            {
                ExprKind::BoolLiteral(true)
            }
            Some(Token::Ident(value))
                if value == "false" && self.features().keyword_bool_true_false.is_accepted() =>
            {
                ExprKind::BoolLiteral(false)
            }
            Some(Token::Ident(value))
                if value == "nullptr" && self.features().keyword_nullptr.is_accepted() =>
            {
                ExprKind::NullPtrLiteral
            }
            Some(Token::Ident(value)) => ExprKind::Identifier(value.clone()),
            Some(Token::Keyword(keyword)) => ExprKind::Identifier(<&str>::from(*keyword).into()),
            Some(token) => return Err(ConstExprError::UnexpectedToken(token.clone())),
            None => return Err(ConstExprError::ExpectedIntegerExpression),
        };
        Ok(self.node(kind, start))
    }

    fn parse_statement_expression(&mut self) -> Result<Expr, ConstExprError> {
        let start = self.position;
        self.expect(Token::LParen)?;
        let open = self.position;
        let close = crate::parser::matching_brace(self.tokens, open)
            .ok_or(ConstExprError::ExpectedIntegerExpression)?;
        let context = self
            .context
            .ok_or(ConstExprError::NotConstant("statement expression"))?;
        let body = context
            .parse_statement_expression_body(&self.tokens[open + 1..close])
            .map_err(|error| ConstExprError::StatementExpression(error.to_string()))?;
        self.position = close + 1;
        self.expect(Token::RParen)?;
        Ok(self.node(ExprKind::StatementExpression(body), start))
    }

    fn parse_generic(&mut self, start: usize) -> Result<Expr, ConstExprError> {
        self.expect(Token::LParen)?;
        let controlling = if let Some(next) = self.peek()
            && starts_type_name(next, self.context)
            && let Some((ty, end)) = self.try_parse_type_name(self.position, |end| {
                self.token_at(end) == Some(&Token::Comma)
            }) {
            self.position = end;
            GenericControl::Type { ty }
        } else {
            GenericControl::Expr(self.parse_assignment()?)
        };
        self.expect(Token::Comma)?;
        let mut associations = Vec::new();
        loop {
            if self.consume(&Token::Keyword(Keyword::Default)) {
                self.expect(Token::Colon)?;
                associations.push(GenericAssociation::Default(self.parse_assignment()?));
            } else {
                let Some((ty, end)) = self.try_parse_type_name(self.position, |_| true) else {
                    return Err(ConstExprError::ExpectedTypeName);
                };
                self.position = end;
                self.expect(Token::Colon)?;
                associations.push(GenericAssociation::Type {
                    ty,
                    value: self.parse_assignment()?,
                });
            }
            if self.consume(&Token::Comma) {
                continue;
            }
            self.expect(Token::RParen)?;
            break;
        }
        Ok(self.node(
            ExprKind::Generic {
                controlling,
                associations,
            },
            start,
        ))
    }

    fn parse_defined(&mut self, start: usize) -> Result<Expr, ConstExprError> {
        let callee = self.node(ExprKind::Identifier("defined".to_string()), start);
        let parenthesized = self.consume(&Token::LParen);
        let name_start = self.position;
        let name = match self.take() {
            Some(Token::Ident(value)) => value,
            Some(token @ Token::Keyword(_)) => String::from(&token),
            Some(token) => return Err(ConstExprError::UnexpectedToken(token)),
            None => return Err(ConstExprError::ExpectedIntegerExpression),
        };
        let argument = self.node(ExprKind::Identifier(name), name_start);
        if parenthesized {
            self.expect(Token::RParen)?;
        }
        Ok(self.node(
            ExprKind::Call {
                callee,
                arguments: vec![argument],
            },
            start,
        ))
    }

    fn parse_offsetof(&mut self, start: usize) -> Result<Expr, ConstExprError> {
        self.expect(Token::LParen)?;
        let (ty, end) = self
            .try_parse_type_name(self.position, |_| true)
            .ok_or(ConstExprError::ExpectedTypeName)?;
        self.position = end;
        self.expect(Token::Comma)?;
        let member_start = self.position;
        let name = self.expect_field_name()?;
        let mut member = self.node(ExprKind::Identifier(name.value), member_start);
        loop {
            let kind = match self.peek() {
                Some(Token::Dot) => {
                    self.take();
                    ExprKind::Member {
                        base: member,
                        field: self.expect_field_name()?,
                        arrow: false,
                    }
                }
                Some(Token::LBracket) => {
                    self.take();
                    let index = self.parse_comma()?;
                    self.expect(Token::RBracket)?;
                    ExprKind::Index {
                        base: member,
                        index,
                    }
                }
                _ => break,
            };
            member = self.node(kind, member_start);
        }
        self.expect(Token::RParen)?;
        Ok(self.node(ExprKind::OffsetOf { ty, member }, start))
    }

    fn parse_bit_cast(&mut self, start: usize) -> Result<Expr, ConstExprError> {
        self.expect(Token::LParen)?;
        let (ty, end) = self
            .try_parse_type_name(self.position, |_| true)
            .ok_or(ConstExprError::ExpectedTypeName)?;
        self.position = end;
        self.expect(Token::Comma)?;
        let value = self.parse_assignment()?;
        self.expect(Token::RParen)?;
        Ok(self.node(ExprKind::BitCast { ty, value }, start))
    }

    fn parse_convert_vector(&mut self, start: usize) -> Result<Expr, ConstExprError> {
        self.expect(Token::LParen)?;
        let value = self.parse_assignment()?;
        self.expect(Token::Comma)?;
        let (ty, end) = self
            .try_parse_type_name(self.position, |_| true)
            .ok_or(ConstExprError::ExpectedTypeName)?;
        self.position = end;
        self.expect(Token::RParen)?;
        Ok(self.node(ExprKind::ConvertVector { ty, value }, start))
    }

    fn parse_va_arg(&mut self, start: usize) -> Result<Expr, ConstExprError> {
        self.expect(Token::LParen)?;
        let list = self.parse_assignment()?;
        self.expect(Token::Comma)?;
        let (ty, end) = self
            .try_parse_type_name(self.position, |_| true)
            .ok_or(ConstExprError::ExpectedTypeName)?;
        self.position = end;
        self.expect(Token::RParen)?;
        Ok(self.node(ExprKind::VaArg { list, ty }, start))
    }

    fn parse_types_compatible(&mut self, start: usize) -> Result<Expr, ConstExprError> {
        self.expect(Token::LParen)?;
        let (left_ty, end) = self
            .try_parse_type_name(self.position, |_| true)
            .ok_or(ConstExprError::ExpectedTypeName)?;
        self.position = end;
        self.expect(Token::Comma)?;
        let (right_ty, end) = self
            .try_parse_type_name(self.position, |_| true)
            .ok_or(ConstExprError::ExpectedTypeName)?;
        self.position = end;
        self.expect(Token::RParen)?;
        Ok(self.node(ExprKind::TypesCompatible { left_ty, right_ty }, start))
    }

    fn binary_operator(&self) -> Option<(BinaryOp, u8)> {
        Some(match self.peek()? {
            Token::Star => (BinaryOp::Mul, 9),
            Token::Slash => (BinaryOp::Div, 9),
            Token::Percent => (BinaryOp::Rem, 9),
            Token::Plus => (BinaryOp::Add, 8),
            Token::Minus => (BinaryOp::Sub, 8),
            Token::Less => (BinaryOp::Less, 6),
            Token::LessEqual => (BinaryOp::LessEqual, 6),
            Token::Greater => (BinaryOp::Greater, 6),
            Token::GreaterEqual => (BinaryOp::GreaterEqual, 6),
            Token::EqualEqual => (BinaryOp::Equal, 5),
            Token::NotEqual => (BinaryOp::NotEqual, 5),
            Token::Amp => (BinaryOp::BitAnd, 4),
            Token::Caret => (BinaryOp::BitXor, 3),
            Token::Pipe => (BinaryOp::BitOr, 2),
            Token::AndAnd => (BinaryOp::And, 1),
            Token::OrOr => (BinaryOp::Or, 0),
            Token::ShiftLeft => (BinaryOp::ShiftLeft, 7),
            Token::ShiftRight => (BinaryOp::ShiftRight, 7),
            _ => return None,
        })
    }
}

fn bit_int_width(ty: &TypeName) -> Option<(u32, bool)> {
    if ty.declarator.is_derived() {
        return None;
    }
    let TypeSpecifier::Integer(IntegerType::BitInt { width, signed }) = &ty.specifiers.ty else {
        return None;
    };
    let width = Parser::evaluate_expr(width, EvalContext::default()).ok()?;
    let width = u32::try_from(width).ok()?;
    (width > 0).then_some((width, *signed))
}

pub(crate) fn starts_type_name(token: &Token, context: Option<&crate::parser::Parser>) -> bool {
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
                | Keyword::Float128Ext
                | Keyword::Decimal32
                | Keyword::Decimal64
                | Keyword::Decimal128
                | Keyword::Int
                | Keyword::Long
                | Keyword::Short
                | Keyword::Signed
                | Keyword::Unsigned
                | Keyword::Void
                | Keyword::Complex
                | Keyword::Imaginary
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
            context.is_some_and(|parser| parser.is_typedef(name))
                || crate::parser::is_target_builtin_name(name)
                || matches!(name.as_str(), "__attribute__" | "__attribute")
        }
        _ => false,
    }
}
