use crate::ast::{FileId, Loc, Span};
use crate::files::raw_byte_for_char;
use crate::standard_features::StandardFeatures;

// a numeric escape or a raw source byte names a code unit directly; anything
// else names a character that the execution encoding still has to encode
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum SourceUnit {
    Character(u32),
    CodeUnit(u32),
}

impl SourceUnit {
    pub(crate) fn value(self) -> u32 {
        match self {
            Self::Character(value) | Self::CodeUnit(value) => value,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Keyword {
    Bool,
    BFloat16,
    Char,
    Double,
    Float,
    Float16,
    Fp16,
    Float64x,
    Float128,
    Float128Ext,
    Decimal32,
    Decimal64,
    Decimal128,
    Int,
    Long,
    Return,
    Short,
    Signed,
    Typedef,
    Unsigned,
    Void,
    Complex,
    Struct,
    Union,
    Enum,
    Const,
    Volatile,
    Restrict,
    Atomic,
    Extern,
    Static,
    Auto,
    Register,
    Inline,
    Int128,
    Noreturn,
    ThreadLocal,
    BitInt,
    Accum,
    Fract,
    Saturated,
    Typeof,
    TypeofUnqual,
    Constexpr,
    Imaginary,
    If,
    Else,
    While,
    Do,
    For,
    Switch,
    Case,
    Default,
    Break,
    Continue,
    Goto,
    StaticAssert,
}

impl Keyword {
    pub fn is_storage_class_or_specifier(self) -> bool {
        matches!(
            self,
            Keyword::Typedef
                | Keyword::Extern
                | Keyword::Static
                | Keyword::Auto
                | Keyword::Register
                | Keyword::ThreadLocal
                | Keyword::Inline
                | Keyword::Noreturn
                | Keyword::Constexpr
        )
    }
}

impl From<Keyword> for &'static str {
    fn from(keyword: Keyword) -> Self {
        match keyword {
            Keyword::Bool => "_Bool",
            Keyword::BFloat16 => "__bf16",
            Keyword::Char => "char",
            Keyword::Double => "double",
            Keyword::Float => "float",
            Keyword::Float16 => "_Float16",
            Keyword::Fp16 => "__fp16",
            Keyword::Float64x => "_Float64x",
            Keyword::Float128 => "_Float128",
            Keyword::Float128Ext => "__float128",
            Keyword::Decimal32 => "_Decimal32",
            Keyword::Decimal64 => "_Decimal64",
            Keyword::Decimal128 => "_Decimal128",
            Keyword::Int => "int",
            Keyword::Long => "long",
            Keyword::Return => "return",
            Keyword::Short => "short",
            Keyword::Signed => "signed",
            Keyword::Typedef => "typedef",
            Keyword::Unsigned => "unsigned",
            Keyword::Void => "void",
            Keyword::Complex => "_Complex",
            Keyword::Struct => "struct",
            Keyword::Union => "union",
            Keyword::Enum => "enum",
            Keyword::Const => "const",
            Keyword::Volatile => "volatile",
            Keyword::Restrict => "restrict",
            Keyword::Atomic => "_Atomic",
            Keyword::Extern => "extern",
            Keyword::Static => "static",
            Keyword::Auto => "auto",
            Keyword::Register => "register",
            Keyword::Inline => "inline",
            Keyword::Int128 => "__int128",
            Keyword::Noreturn => "_Noreturn",
            Keyword::ThreadLocal => "_Thread_local",
            Keyword::BitInt => "_BitInt",
            Keyword::Accum => "_Accum",
            Keyword::Fract => "_Fract",
            Keyword::Saturated => "_Sat",
            Keyword::Typeof => "typeof",
            Keyword::TypeofUnqual => "typeof_unqual",
            Keyword::Constexpr => "constexpr",
            Keyword::Imaginary => "_Imaginary",
            Keyword::If => "if",
            Keyword::Else => "else",
            Keyword::While => "while",
            Keyword::Do => "do",
            Keyword::For => "for",
            Keyword::Switch => "switch",
            Keyword::Case => "case",
            Keyword::Default => "default",
            Keyword::Break => "break",
            Keyword::Continue => "continue",
            Keyword::Goto => "goto",
            Keyword::StaticAssert => "_Static_assert",
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Token {
    Keyword(Keyword),
    Sizeof,
    Alignof,
    Ident(String),
    IntLit(String),
    FloatLit(String),
    CharLit(String, Vec<u32>),
    Utf8CharLit(String, Vec<u32>),
    Utf16CharLit(String, Vec<u32>),
    Utf32CharLit(String, Vec<u32>),
    WideCharLit(String, Vec<u32>),
    StringLit(String),
    Utf8StringLit(String),
    Utf16StringLit(String),
    Utf32StringLit(String),
    WideStringLit(String),
    Comment(String),
    Newline,
    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,
    Colon,
    Comma,
    Hash,
    HashHash,
    Star,
    Ellipsis,
    Semi,
    Equal,
    Dot,
    Plus,
    Minus,
    Slash,
    Percent,
    Bang,
    Tilde,
    Less,
    Greater,
    LessEqual,
    GreaterEqual,
    EqualEqual,
    NotEqual,
    Amp,
    Caret,
    Pipe,
    AndAnd,
    OrOr,
    ShiftLeft,
    ShiftRight,
    Question,
    Arrow,
    PlusPlus,
    MinusMinus,
    PlusEqual,
    MinusEqual,
    StarEqual,
    SlashEqual,
    PercentEqual,
    AmpEqual,
    PipeEqual,
    CaretEqual,
    ShiftLeftEqual,
    ShiftRightEqual,
}

impl Token {
    pub fn integer_value_i128(&self) -> Option<i128> {
        let Token::IntLit(spelling) = self else {
            return None;
        };
        let digits = Lexer::integer_digits(spelling).replace('\'', "");
        let (radix, digits) = if digits.starts_with("0x") || digits.starts_with("0X") {
            (16, &digits[2..])
        } else if digits.starts_with("0b") || digits.starts_with("0B") {
            (2, &digits[2..])
        } else if digits.len() > 1 && digits.starts_with('0') {
            (8, &digits[1..])
        } else {
            (10, digits.as_str())
        };
        u128::from_str_radix(digits, radix)
            .ok()
            .and_then(|value| i128::try_from(value).ok())
    }

    pub fn integer_value(&self) -> Option<i64> {
        let Token::IntLit(spelling) = self else {
            return None;
        };
        let digits = Lexer::integer_digits(spelling).replace('\'', "");
        let (radix, digits) = if digits.starts_with("0x") || digits.starts_with("0X") {
            (16, &digits[2..])
        } else if digits.starts_with("0b") || digits.starts_with("0B") {
            (2, &digits[2..])
        } else if digits.len() > 1 && digits.starts_with('0') {
            (8, &digits[1..])
        } else {
            (10, digits.as_str())
        };
        u64::from_str_radix(digits, radix)
            .ok()
            .map(|value| value as i64)
    }

    pub fn float_value_f16(&self) -> Option<u16> {
        self.apfloat_bits::<rustc_apfloat::ieee::Half>()
            .map(|bits| bits as u16)
    }

    pub fn float_value_f32(&self) -> Option<f32> {
        self.apfloat_bits::<rustc_apfloat::ieee::Single>()
            .map(|bits| f32::from_bits(bits as u32))
    }

    pub fn float_value_f64(&self) -> Option<f64> {
        self.apfloat_bits::<rustc_apfloat::ieee::Double>()
            .map(|bits| f64::from_bits(bits as u64))
    }

    pub fn float_value_f80(&self) -> Option<u128> {
        self.apfloat_bits::<rustc_apfloat::ieee::X87DoubleExtended>()
    }

    pub fn float_value_f128(&self) -> Option<u128> {
        self.apfloat_bits::<rustc_apfloat::ieee::Quad>()
    }

    fn apfloat_bits<F: rustc_apfloat::Float>(&self) -> Option<u128> {
        let Token::FloatLit(spelling) = self else {
            return None;
        };
        F::from_str_r(spelling, rustc_apfloat::Round::NearestTiesToEven)
            .ok()
            .map(|parsed| parsed.value.to_bits())
    }
}

impl std::fmt::Display for Token {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        formatter.write_str(&String::from(self))
    }
}

impl From<&Token> for String {
    fn from(token: &Token) -> Self {
        match token {
            Token::Sizeof => "sizeof".into(),
            Token::Alignof => "_Alignof".into(),
            Token::Keyword(keyword) => <&str>::from(*keyword).into(),
            Token::Ident(name) => name.clone(),
            Token::IntLit(value) => value.clone(),
            Token::FloatLit(value) => value.clone(),
            Token::CharLit(value, _) => format!("'{value}'"),
            Token::Utf8CharLit(value, _) => format!("u8'{value}'"),
            Token::Utf16CharLit(value, _) => format!("u'{value}'"),
            Token::Utf32CharLit(value, _) => format!("U'{value}'"),
            Token::WideCharLit(value, _) => format!("L'{value}'"),
            Token::StringLit(value) => format!("\"{value}\""),
            Token::Utf8StringLit(value) => format!("u8\"{value}\""),
            Token::Utf16StringLit(value) => format!("u\"{value}\""),
            Token::Utf32StringLit(value) => format!("U\"{value}\""),
            Token::WideStringLit(value) => format!("L\"{value}\""),
            Token::Comment(text) => text.clone(),
            Token::Newline => "\n".into(),
            Token::LParen => "(".into(),
            Token::RParen => ")".into(),
            Token::LBrace => "{".into(),
            Token::RBrace => "}".into(),
            Token::LBracket => "[".into(),
            Token::RBracket => "]".into(),
            Token::Colon => ":".into(),
            Token::Comma => ",".into(),
            Token::Hash => "#".into(),
            Token::HashHash => "##".into(),
            Token::Star => "*".into(),
            Token::Ellipsis => "...".into(),
            Token::Semi => ";".into(),
            Token::Equal => "=".into(),
            Token::Dot => ".".into(),
            Token::Plus => "+".into(),
            Token::Minus => "-".into(),
            Token::Slash => "/".into(),
            Token::Percent => "%".into(),
            Token::Bang => "!".into(),
            Token::Tilde => "~".into(),
            Token::Less => "<".into(),
            Token::Greater => ">".into(),
            Token::LessEqual => "<=".into(),
            Token::GreaterEqual => ">=".into(),
            Token::EqualEqual => "==".into(),
            Token::NotEqual => "!=".into(),
            Token::Amp => "&".into(),
            Token::Caret => "^".into(),
            Token::Pipe => "|".into(),
            Token::AndAnd => "&&".into(),
            Token::OrOr => "||".into(),
            Token::ShiftLeft => "<<".into(),
            Token::ShiftRight => ">>".into(),
            Token::Question => "?".into(),
            Token::Arrow => "->".into(),
            Token::PlusPlus => "++".into(),
            Token::MinusMinus => "--".into(),
            Token::PlusEqual => "+=".into(),
            Token::MinusEqual => "-=".into(),
            Token::StarEqual => "*=".into(),
            Token::SlashEqual => "/=".into(),
            Token::PercentEqual => "%=".into(),
            Token::AmpEqual => "&=".into(),
            Token::PipeEqual => "|=".into(),
            Token::CaretEqual => "^=".into(),
            Token::ShiftLeftEqual => "<<=".into(),
            Token::ShiftRightEqual => ">>=".into(),
        }
    }
}

const MULTI_CHAR_OPS: &[(&str, Token)] = &[
    ("<<=", Token::ShiftLeftEqual),
    (">>=", Token::ShiftRightEqual),
    ("...", Token::Ellipsis),
    ("<=", Token::LessEqual),
    (">=", Token::GreaterEqual),
    ("==", Token::EqualEqual),
    ("!=", Token::NotEqual),
    ("&&", Token::AndAnd),
    ("||", Token::OrOr),
    ("<<", Token::ShiftLeft),
    (">>", Token::ShiftRight),
    ("->", Token::Arrow),
    ("++", Token::PlusPlus),
    ("--", Token::MinusMinus),
    ("+=", Token::PlusEqual),
    ("-=", Token::MinusEqual),
    ("*=", Token::StarEqual),
    ("/=", Token::SlashEqual),
    ("%=", Token::PercentEqual),
    ("&=", Token::AmpEqual),
    ("|=", Token::PipeEqual),
    ("^=", Token::CaretEqual),
    ("##", Token::HashHash),
];

const SINGLE_CHAR_OPS: &[(char, Token)] = &[
    ('(', Token::LParen),
    (')', Token::RParen),
    ('{', Token::LBrace),
    ('}', Token::RBrace),
    ('[', Token::LBracket),
    (']', Token::RBracket),
    (':', Token::Colon),
    (',', Token::Comma),
    ('#', Token::Hash),
    ('*', Token::Star),
    (';', Token::Semi),
    ('=', Token::Equal),
    ('.', Token::Dot),
    ('+', Token::Plus),
    ('-', Token::Minus),
    ('/', Token::Slash),
    ('%', Token::Percent),
    ('!', Token::Bang),
    ('~', Token::Tilde),
    ('<', Token::Less),
    ('>', Token::Greater),
    ('&', Token::Amp),
    ('^', Token::Caret),
    ('|', Token::Pipe),
    ('?', Token::Question),
];

pub trait TokenSpanExt {
    fn value_at(&self, index: usize) -> Option<&Token>;
    fn value_owned(&self, index: usize) -> Option<Token> {
        self.value_at(index).cloned()
    }
    fn values(&self) -> impl Iterator<Item = &Token>;
    fn contains_value(&self, token: &Token) -> bool {
        self.values().any(|value| value == token)
    }
    fn as_tokens(&self) -> Vec<Token> {
        self.values().cloned().collect()
    }
}

impl TokenSpanExt for [Span<Token>] {
    fn value_at(&self, index: usize) -> Option<&Token> {
        self.get(index).map(|span| &span.value)
    }

    fn values(&self) -> impl Iterator<Item = &Token> {
        self.iter().map(|span| &span.value)
    }
}

pub struct Lexer {
    file: FileId,
    base_offset: usize,
    chars: Vec<char>,
    byte_offsets: Vec<usize>,
    pos: usize,
    mark: usize,
    emit_newlines: bool,
    space_before: bool,
    tokens: Vec<Span<Token>>,
    features: StandardFeatures,
}

impl Lexer {
    pub fn new(file: FileId, src: &str) -> Self {
        Self::with_offset(file, src, 0)
    }

    pub fn with_offset(file: FileId, src: &str, base_offset: usize) -> Self {
        let mut chars = Vec::with_capacity(src.len());
        let mut byte_offsets = Vec::with_capacity(src.len() + 1);
        let mut indices = src.char_indices();
        while let Some((byte, c)) = indices.next() {
            let rest = &src[byte + c.len_utf8()..];
            let splice_len = match c {
                '\\' if rest.starts_with('\n') => 1,
                '\\' if rest.starts_with("\r\n") => 2,
                _ => 0,
            };
            if splice_len > 0 {
                indices.nth(splice_len - 1);
                continue;
            }
            chars.push(c);
            byte_offsets.push(byte);
        }
        byte_offsets.push(src.len());
        Self {
            file,
            base_offset,
            chars,
            byte_offsets,
            pos: 0,
            mark: 0,
            emit_newlines: false,
            space_before: false,
            tokens: Vec::new(),
            features: StandardFeatures::default(),
        }
    }

    pub fn with_newlines(mut self) -> Self {
        self.emit_newlines = true;
        self
    }

    pub fn with_features(mut self, features: StandardFeatures) -> Self {
        self.features = features;
        self
    }

    fn byte_of(&self, char_index: usize) -> usize {
        self.base_offset + self.byte_offsets[char_index]
    }

    fn char_at(&self, index: usize) -> Option<char> {
        self.chars.get(index).copied()
    }

    fn peek(&self) -> Option<char> {
        self.peek_at(0)
    }

    fn peek_at(&self, offset: usize) -> Option<char> {
        self.char_at(self.pos + offset)
    }

    fn peek_str(&self, s: &str) -> bool {
        s.chars()
            .enumerate()
            .all(|(offset, c)| self.peek_at(offset) == Some(c))
    }

    fn try_consume(&mut self, s: &str) -> bool {
        if self.peek_str(s) {
            self.pos += s.chars().count();
            true
        } else {
            false
        }
    }

    fn try_consume_op(&mut self) -> Option<Token> {
        for (op, token) in MULTI_CHAR_OPS {
            if self.try_consume(op) {
                return Some(token.clone());
            }
        }
        None
    }

    fn current_loc(&self) -> Loc {
        let start = self.byte_of(self.mark);
        let end = match self.pos.checked_sub(1) {
            Some(last) if last >= self.mark => self.byte_of(last) + self.chars[last].len_utf8(),
            _ => start,
        };
        Loc::new(self.file, start, end - start)
    }

    fn emit(&mut self, token: Token) {
        let loc = self.current_loc();
        let leading_space = std::mem::take(&mut self.space_before);
        self.tokens
            .push(Span::new(token, loc, loc).with_leading_space(leading_space));
    }

    pub fn tokenize(mut self) -> Vec<Span<Token>> {
        while self.pos < self.chars.len() {
            self.mark = self.pos;
            self.scan_one();
        }
        self.tokens
    }

    fn scan_one(&mut self) {
        let i = self.pos;
        let c = self.chars[i];

        if c == '\n' && self.emit_newlines {
            self.pos += 1;
            self.emit(Token::Newline);
            self.space_before = true;
        } else if c.is_whitespace() {
            self.pos += 1;
            self.space_before = true;
        } else if self.try_consume("//") {
            while self.peek().is_some_and(|c| c != '\n') {
                self.pos += 1;
            }
            let text: String = self.chars[i..self.pos].iter().collect();
            self.emit(Token::Comment(text));
            self.space_before = true;
        } else if self.try_consume("/*") {
            while self.pos < self.chars.len() && !self.peek_str("*/") {
                self.pos += 1;
            }
            self.pos = (self.pos + 2).min(self.chars.len());
            let text: String = self.chars[i..self.pos].iter().collect();
            self.emit(Token::Comment(text));
            self.space_before = true;
        } else if c.is_ascii_digit()
            || (c == '.' && self.peek_at(1).is_some_and(|next| next.is_ascii_digit()))
        {
            self.pos = self.numeric_end(self.pos);
            let spelling: String = self.chars[i..self.pos].iter().collect();
            let exponent: &[char] = if spelling.starts_with("0x") || spelling.starts_with("0X") {
                &['p', 'P']
            } else {
                &['e', 'E']
            };
            if spelling.contains('.') || spelling.contains(exponent) {
                self.emit(Token::FloatLit(spelling));
            } else {
                let digits = Self::integer_digits(&spelling).replace('\'', "");
                let (radix, digits) = if digits.starts_with("0x") || digits.starts_with("0X") {
                    (16, &digits[2..])
                } else if digits.starts_with("0b") || digits.starts_with("0B") {
                    (2, &digits[2..])
                } else if digits.len() > 1 && digits.starts_with('0') {
                    (8, &digits[1..])
                } else {
                    (10, digits.as_str())
                };
                if !digits.is_empty() && digits.chars().all(|digit| digit.is_digit(radix)) {
                    self.emit(Token::IntLit(spelling));
                } else {
                    self.emit(Token::FloatLit(spelling));
                }
            }
        } else if let Some(prefix_len) = self.string_prefix_len() {
            let start = i + prefix_len;
            self.pos = self.literal_end(start, '"');
            let value: String = self.chars[start + 1..self.pos.min(self.chars.len())]
                .iter()
                .collect();
            self.pos = self.past_literal(self.pos);
            let token = match prefix_len {
                0 => Token::StringLit(value),
                1 if c == 'u' => Token::Utf16StringLit(value),
                1 if c == 'U' => Token::Utf32StringLit(value),
                1 => Token::WideStringLit(value),
                _ => Token::Utf8StringLit(value),
            };
            self.emit(token);
        } else if let Some(prefix_len) = self.char_prefix_len() {
            let start = i + prefix_len;
            let literal_close = self.literal_end(start, '\'').min(self.chars.len());
            let value: String = self.chars[start + 1..literal_close].iter().collect();
            let decoded = Self::decode_escapes(&self.chars, start + 1, literal_close);
            let token = match prefix_len {
                0 => Token::CharLit(value, decoded),
                1 if c == 'u' => Token::Utf16CharLit(value, decoded),
                1 if c == 'U' => Token::Utf32CharLit(value, decoded),
                1 => Token::WideCharLit(value, decoded),
                _ => Token::Utf8CharLit(value, decoded),
            };
            self.pos = self.past_literal(literal_close);
            self.emit(token);
        } else if c == '\'' {
            let literal_close = self.literal_end(i, '\'').min(self.chars.len());
            let value: String = self.chars[i + 1..literal_close].iter().collect();
            let decoded = Self::decode_escapes(&self.chars, i + 1, literal_close);
            self.pos = self.past_literal(literal_close);
            self.emit(Token::CharLit(value, decoded));
        } else if c == '"' {
            let literal_close = self.literal_end(i, '"');
            let value: String = self.chars[i + 1..literal_close.min(self.chars.len())]
                .iter()
                .collect();
            self.pos = self.past_literal(literal_close);
            self.emit(Token::StringLit(value));
        } else if c.is_ascii_alphabetic() || matches!(c, '_' | '$' | '\\') {
            while self.pos < self.chars.len()
                && (self.chars[self.pos].is_ascii_alphanumeric()
                    || matches!(self.chars[self.pos], '_' | '$')
                    || self.chars[self.pos] == '\\')
            {
                if self.chars[self.pos] == '\\' {
                    self.pos = self.universal_character_name_end(self.pos);
                } else {
                    self.pos += 1;
                }
            }
            let word: String = self.chars[i..self.pos].iter().collect();
            let token = match word.as_str() {
                "sizeof" => Token::Sizeof,
                "_Alignof" | "__alignof" | "__alignof__" => Token::Alignof,
                "_Bool" => Token::Keyword(Keyword::Bool),
                "bool" if self.features.keyword_bool_true_false.is_accepted() => {
                    Token::Keyword(Keyword::Bool)
                }
                "__bf16" => Token::Keyword(Keyword::BFloat16),
                "char" => Token::Keyword(Keyword::Char),
                "double" => Token::Keyword(Keyword::Double),
                "float" => Token::Keyword(Keyword::Float),
                "_Float16" => Token::Keyword(Keyword::Float16),
                "__fp16" => Token::Keyword(Keyword::Fp16),
                "_Float32" => Token::Keyword(Keyword::Float),
                "_Float64" => Token::Keyword(Keyword::Double),
                "_Float32x" => Token::Keyword(Keyword::Double),
                "_Float64x" => Token::Keyword(Keyword::Float64x),
                "_Float128" | "_Float128x" => Token::Keyword(Keyword::Float128),
                "__float128" => Token::Keyword(Keyword::Float128Ext),
                "_Decimal32" if self.features.decimal_floating_point.is_accepted() => {
                    Token::Keyword(Keyword::Decimal32)
                }
                "_Decimal64" if self.features.decimal_floating_point.is_accepted() => {
                    Token::Keyword(Keyword::Decimal64)
                }
                "_Decimal128" if self.features.decimal_floating_point.is_accepted() => {
                    Token::Keyword(Keyword::Decimal128)
                }
                "int" => Token::Keyword(Keyword::Int),
                "long" => Token::Keyword(Keyword::Long),
                "return" => Token::Keyword(Keyword::Return),
                "short" => Token::Keyword(Keyword::Short),
                "signed" => Token::Keyword(Keyword::Signed),
                "typedef" => Token::Keyword(Keyword::Typedef),
                "unsigned" => Token::Keyword(Keyword::Unsigned),
                "void" => Token::Keyword(Keyword::Void),
                "_Complex" | "__complex__" | "__complex" => Token::Keyword(Keyword::Complex),
                "struct" => Token::Keyword(Keyword::Struct),
                "union" => Token::Keyword(Keyword::Union),
                "enum" => Token::Keyword(Keyword::Enum),
                "const" | "__const" | "__const__" => Token::Keyword(Keyword::Const),
                "volatile" | "__volatile" | "__volatile__" => Token::Keyword(Keyword::Volatile),
                "restrict" if self.features.keyword_restrict.is_accepted() => {
                    Token::Keyword(Keyword::Restrict)
                }
                "_Atomic" => Token::Keyword(Keyword::Atomic),
                "extern" => Token::Keyword(Keyword::Extern),
                "static" => Token::Keyword(Keyword::Static),
                "auto" => Token::Keyword(Keyword::Auto),
                "register" => Token::Keyword(Keyword::Register),
                "__inline" | "__inline__" => Token::Keyword(Keyword::Inline),
                "inline" if self.features.keyword_inline.is_accepted() => {
                    Token::Keyword(Keyword::Inline)
                }
                "__int128" => Token::Keyword(Keyword::Int128),
                "_Noreturn" => Token::Keyword(Keyword::Noreturn),
                "_Thread_local" | "__thread" => Token::Keyword(Keyword::ThreadLocal),
                "thread_local" if self.features.keyword_thread_local.is_accepted() => {
                    Token::Keyword(Keyword::ThreadLocal)
                }
                "__restrict" | "__restrict__" => Token::Keyword(Keyword::Restrict),
                "_BitInt" => Token::Keyword(Keyword::BitInt),
                "_Accum" => Token::Keyword(Keyword::Accum),
                "_Fract" => Token::Keyword(Keyword::Fract),
                "_Sat" => Token::Keyword(Keyword::Saturated),
                "__typeof" | "__typeof__" => Token::Keyword(Keyword::Typeof),
                "typeof" if self.features.keyword_typeof.is_accepted() => {
                    Token::Keyword(Keyword::Typeof)
                }
                "__typeof_unqual" | "__typeof_unqual__" => Token::Keyword(Keyword::TypeofUnqual),
                "typeof_unqual" if self.features.keyword_typeof_unqual.is_accepted() => {
                    Token::Keyword(Keyword::TypeofUnqual)
                }
                "constexpr" if self.features.keyword_constexpr.is_accepted() => {
                    Token::Keyword(Keyword::Constexpr)
                }
                "_Imaginary" => Token::Keyword(Keyword::Imaginary),
                "if" => Token::Keyword(Keyword::If),
                "else" => Token::Keyword(Keyword::Else),
                "while" => Token::Keyword(Keyword::While),
                "do" => Token::Keyword(Keyword::Do),
                "for" => Token::Keyword(Keyword::For),
                "switch" => Token::Keyword(Keyword::Switch),
                "case" => Token::Keyword(Keyword::Case),
                "default" => Token::Keyword(Keyword::Default),
                "break" => Token::Keyword(Keyword::Break),
                "continue" => Token::Keyword(Keyword::Continue),
                "goto" => Token::Keyword(Keyword::Goto),
                "_Static_assert" => Token::Keyword(Keyword::StaticAssert),
                "static_assert" if self.features.keyword_static_assert.is_accepted() => {
                    Token::Keyword(Keyword::StaticAssert)
                }
                _ => Token::Ident(word),
            };
            self.emit(token);
        } else if let Some(token) = self.try_consume_op() {
            self.emit(token);
        } else {
            let token = SINGLE_CHAR_OPS
                .iter()
                .find(|(ch, _)| *ch == c)
                .map(|(_, token)| token.clone())
                .unwrap_or_else(|| Token::Ident(c.to_string()));
            self.pos += 1;
            self.emit(token);
        }
    }

    fn numeric_end(&self, start: usize) -> usize {
        let mut i = start;
        while i < self.chars.len()
            && (self.chars[i].is_ascii_alphanumeric()
                || self.chars[i] == '.'
                || (self.chars[i] == '\''
                    && self
                        .chars
                        .get(i + 1)
                        .is_some_and(char::is_ascii_alphanumeric))
                || (self.chars[i] == '+' || self.chars[i] == '-')
                    && i > 0
                    && matches!(self.chars[i - 1], 'e' | 'E' | 'p' | 'P'))
        {
            i += 1;
        }
        i
    }

    fn string_prefix_len(&self) -> Option<usize> {
        self.literal_prefix_len('"')
    }

    fn char_prefix_len(&self) -> Option<usize> {
        self.literal_prefix_len('\'')
    }

    fn literal_prefix_len(&self, quote: char) -> Option<usize> {
        if self.peek() == Some(quote) {
            return Some(0);
        }
        let unicode = self.features.unicode_literal_prefixes.is_accepted();
        if self.peek_str("u8") {
            let accepted = match quote {
                '\'' => self.features.u8_character_constant.is_accepted(),
                _ => unicode,
            };
            return (accepted && self.peek_at(2) == Some(quote)).then_some(2);
        }
        if matches!(self.peek(), Some('u' | 'U')) {
            return (unicode && self.peek_at(1) == Some(quote)).then_some(1);
        }
        if self.peek() == Some('L') {
            return (self.peek_at(1) == Some(quote)).then_some(1);
        }
        None
    }

    fn literal_end(&self, start: usize, delimiter: char) -> usize {
        let mut i = start + 1;
        while i < self.chars.len() {
            if self.chars[i] == '\\' {
                i = self.universal_character_name_end(i).max(i + 2);
            } else if self.chars[i] == delimiter || self.chars[i] == '\n' {
                break;
            } else {
                i += 1;
            }
        }
        i
    }

    fn past_literal(&self, close: usize) -> usize {
        match self.char_at(close) {
            Some('\n') | None => close,
            Some(_) => close + 1,
        }
    }

    fn universal_character_name_end(&self, at: usize) -> usize {
        let len = self.chars.len();
        match (self.char_at(at + 1), self.char_at(at + 2)) {
            (Some('u'), _) => (at + 6).min(len),
            (Some('U'), _) => (at + 10).min(len),
            (Some('N'), Some('{')) => {
                let mut end = at + 3;
                while self.char_at(end).is_some_and(|c| c != '}') {
                    end += 1;
                }
                end.saturating_add(1).min(len)
            }
            _ => (at + 2).min(len),
        }
    }

    pub(crate) fn integer_digits(spelling: &str) -> String {
        let mut end = spelling.len();
        let bytes = spelling.as_bytes();
        loop {
            let previous = end;
            if end >= 2 && bytes[end - 2..end].eq_ignore_ascii_case(b"wb") {
                end -= 2;
            }
            while end > 0 && matches!(bytes[end - 1], b'u' | b'U' | b'l' | b'L' | b'w' | b'W') {
                end -= 1;
            }
            if end == previous {
                break;
            }
        }
        spelling[..end].to_string()
    }

    pub(crate) fn decode_escapes(chars: &[char], start: usize, end: usize) -> Vec<u32> {
        Self::decode_source_units(chars, start, end)
            .into_iter()
            .map(SourceUnit::value)
            .collect()
    }

    pub(crate) fn decode_source_units(chars: &[char], start: usize, end: usize) -> Vec<SourceUnit> {
        let mut units = Vec::new();
        let mut i = start;
        while i < end {
            let (unit, next) = Self::decode_char_escape(chars, i, end);
            units.push(unit);
            i = next;
        }
        units
    }

    fn char_in(chars: &[char], i: usize, end: usize) -> Option<char> {
        (i < end).then(|| chars[i])
    }

    fn decode_char_escape(chars: &[char], i: usize, end: usize) -> (SourceUnit, usize) {
        if chars[i] != '\\' {
            return match raw_byte_for_char(chars[i]) {
                Some(byte) => (SourceUnit::CodeUnit(u32::from(byte)), i + 1),
                None => (SourceUnit::Character(chars[i] as u32), i + 1),
            };
        }
        let j = i + 1;
        let (value, next) = match Self::char_in(chars, j, end) {
            Some('n') => (0x0A, j + 1),
            Some('t') => (0x09, j + 1),
            Some('r') => (0x0D, j + 1),
            Some('a') => (0x07, j + 1),
            Some('b') => (0x08, j + 1),
            Some('f') => (0x0C, j + 1),
            Some('v') => (0x0B, j + 1),
            Some('e') => (0x1B, j + 1),
            Some('\\') => (0x5C, j + 1),
            Some('\'') => (0x27, j + 1),
            Some('"') => (0x22, j + 1),
            Some('?') => (0x3F, j + 1),
            Some('x') => {
                let (value, next) = Self::hex_char_escape(chars, j + 1, end, usize::MAX);
                return (SourceUnit::CodeUnit(value), next);
            }
            Some('u') => Self::hex_char_escape(chars, j + 1, end, 4),
            Some('U') => Self::hex_char_escape(chars, j + 1, end, 8),
            Some(digit) if digit.is_digit(8) => {
                let mut e = j;
                let mut value = 0u32;
                let mut count = 0;
                while count < 3 && Self::char_in(chars, e, end).is_some_and(|c| c.is_digit(8)) {
                    value = value * 8 + chars[e].to_digit(8).unwrap();
                    e += 1;
                    count += 1;
                }
                return (SourceUnit::CodeUnit(value), e);
            }
            Some(other) => (other as u32, j + 1),
            None => (0, j),
        };
        (SourceUnit::Character(value), next)
    }

    fn hex_char_escape(
        chars: &[char],
        start: usize,
        end: usize,
        max_digits: usize,
    ) -> (u32, usize) {
        let mut e = start;
        let mut value = 0u32;
        while e - start < max_digits
            && Self::char_in(chars, e, end).is_some_and(|c| c.is_ascii_hexdigit())
        {
            value = value * 16 + chars[e].to_digit(16).unwrap();
            e += 1;
        }
        (value, e)
    }
}
