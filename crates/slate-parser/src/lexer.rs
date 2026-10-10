use crate::ast::{FileId, Loc, Span};
use crate::files::raw_byte_for_char;
use crate::standard_features::StandardFeatures;
use std::rc::Rc;

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
    Float32,
    Float64,
    Float32x,
    Float64x,
    Float128,
    Float128Ext,
    Float80,
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
    ForceInline,
    Unaligned,
    Ptr32,
    Ptr64,
    Sptr,
    Uptr,
    SegFs,
    SegGs,
    Int128,
    Int64,
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
                | Keyword::ForceInline
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
            Keyword::Float32 => "_Float32",
            Keyword::Float64 => "_Float64",
            Keyword::Float32x => "_Float32x",
            Keyword::Float64x => "_Float64x",
            Keyword::Float128 => "_Float128",
            Keyword::Float128Ext => "__float128",
            Keyword::Float80 => "__float80",
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
            Keyword::ForceInline => "__forceinline",
            Keyword::Unaligned => "__unaligned",
            Keyword::Ptr32 => "__ptr32",
            Keyword::Ptr64 => "__ptr64",
            Keyword::Sptr => "__sptr",
            Keyword::Uptr => "__uptr",
            Keyword::SegFs => "__seg_fs",
            Keyword::SegGs => "__seg_gs",
            Keyword::Int128 => "__int128",
            Keyword::Int64 => "__int64",
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

#[derive(Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct TokenText(Rc<str>);

impl TokenText {
    pub fn as_str(&self) -> &str {
        &self.0
    }
}

impl std::ops::Deref for TokenText {
    type Target = str;

    fn deref(&self) -> &str {
        &self.0
    }
}

impl std::fmt::Debug for TokenText {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Debug::fmt(&*self.0, formatter)
    }
}

impl std::fmt::Display for TokenText {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        formatter.write_str(&self.0)
    }
}

impl From<String> for TokenText {
    fn from(text: String) -> Self {
        Self(text.into())
    }
}

impl From<&str> for TokenText {
    fn from(text: &str) -> Self {
        Self(text.into())
    }
}

impl From<TokenText> for String {
    fn from(text: TokenText) -> Self {
        text.0.to_string()
    }
}

impl PartialEq<str> for TokenText {
    fn eq(&self, other: &str) -> bool {
        &*self.0 == other
    }
}

impl PartialEq<&str> for TokenText {
    fn eq(&self, other: &&str) -> bool {
        &*self.0 == *other
    }
}

impl PartialEq<String> for TokenText {
    fn eq(&self, other: &String) -> bool {
        *self.0 == **other
    }
}

impl PartialEq<TokenText> for String {
    fn eq(&self, other: &TokenText) -> bool {
        **self == *other.0
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Token {
    Keyword(Keyword),
    Sizeof,
    Alignof,
    Countof,
    Maxof,
    Minof,
    Ident(TokenText),
    IntLit(TokenText),
    FloatLit(TokenText),
    CharLit(TokenText, Rc<[u32]>),
    Utf8CharLit(TokenText, Rc<[u32]>),
    Utf16CharLit(TokenText, Rc<[u32]>),
    Utf32CharLit(TokenText, Rc<[u32]>),
    WideCharLit(TokenText, Rc<[u32]>),
    StringLit(TokenText),
    Utf8StringLit(TokenText),
    Utf16StringLit(TokenText),
    Utf32StringLit(TokenText),
    WideStringLit(TokenText),
    Comment(TokenText),
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
        if Lexer::is_imaginary_integer(spelling) {
            return None;
        }
        let digits = Lexer::integer_digits(spelling).replace('\'', "");
        let (radix, digits) = Lexer::integer_radix(&digits);
        u128::from_str_radix(digits, radix)
            .ok()
            .and_then(|value| i128::try_from(value).ok())
    }

    pub fn integer_value(&self) -> Option<i64> {
        let Token::IntLit(spelling) = self else {
            return None;
        };
        if Lexer::is_imaginary_integer(spelling) {
            return None;
        }
        let digits = Lexer::integer_digits(spelling).replace('\'', "");
        let (radix, digits) = Lexer::integer_radix(&digits);
        u64::from_str_radix(digits, radix)
            .ok()
            .map(|value| value as i64)
    }

    pub fn float_value_bf16(&self) -> Option<u16> {
        self.apfloat_bits::<rustc_apfloat::ieee::BFloat>()
            .map(|bits| bits as u16)
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
            Token::Countof => "_Countof".into(),
            Token::Maxof => "_Maxof".into(),
            Token::Minof => "_Minof".into(),
            Token::Keyword(keyword) => <&str>::from(*keyword).into(),
            Token::Ident(name) => name.to_string(),
            Token::IntLit(value) => value.to_string(),
            Token::FloatLit(value) => value.to_string(),
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
            Token::Comment(text) => text.to_string(),
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

const DIGRAPHS: &[(&str, Token)] = &[
    ("%:%:", Token::HashHash),
    ("%:", Token::Hash),
    ("<:", Token::LBracket),
    (":>", Token::RBracket),
    ("<%", Token::LBrace),
    ("%>", Token::RBrace),
];

pub(crate) fn token_spelling(token: &Span<Token>) -> String {
    token
        .digraph
        .then(|| {
            DIGRAPHS
                .iter()
                .find(|(_, digraph)| *digraph == token.value)
                .map(|(spelling, _)| (*spelling).to_owned())
        })
        .flatten()
        .unwrap_or_else(|| String::from(&token.value))
}

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

#[derive(Debug, Clone)]
pub struct LineToken {
    pub token: Span<Token>,
    pub at_line_start: bool,
}

pub struct Lexer {
    file: FileId,
    base_offset: usize,
    chars: Vec<char>,
    byte_offsets: Vec<usize>,
    byte_ends: Vec<usize>,
    pos: usize,
    mark: usize,
    line_starts: Option<Vec<bool>>,
    at_line_start: bool,
    space_before: bool,
    tokens: Vec<Span<Token>>,
    features: StandardFeatures,
}

impl Lexer {
    pub fn new(file: FileId, src: &str, features: StandardFeatures) -> Self {
        Self::with_offset(file, src, 0, features)
    }

    pub fn with_offset(
        file: FileId,
        src: &str,
        base_offset: usize,
        features: StandardFeatures,
    ) -> Self {
        let mut chars = Vec::with_capacity(src.len());
        let mut byte_offsets = Vec::with_capacity(src.len() + 1);
        let mut byte_ends = Vec::with_capacity(src.len());
        let mut byte = 0;
        while let Some((c, len)) = source_char(src, byte, features.trigraphs) {
            let splice_len = match c {
                '\\' => splice_len(&src[byte + len..], features.whitespace_line_splice),
                _ => 0,
            };
            if splice_len == 0 {
                chars.push(c);
                byte_offsets.push(byte);
                byte_ends.push(byte + len);
            }
            byte += len + splice_len;
        }
        byte_offsets.push(src.len());
        Self {
            file,
            base_offset,
            chars,
            byte_offsets,
            byte_ends,
            pos: 0,
            mark: 0,
            line_starts: None,
            at_line_start: true,
            space_before: false,
            tokens: Vec::new(),
            features,
        }
    }

    pub fn tokenize_lines(mut self) -> Vec<LineToken> {
        self.line_starts = Some(Vec::new());
        let starts = self.run();
        let tokens = std::mem::take(&mut self.tokens);
        tokens
            .into_iter()
            .zip(starts)
            .map(|(token, at_line_start)| LineToken {
                token,
                at_line_start,
            })
            .collect()
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

    fn try_consume_digraph(&mut self) -> Option<Token> {
        if !self.features.digraphs {
            return None;
        }
        DIGRAPHS
            .iter()
            .find(|(digraph, _)| self.try_consume(digraph))
            .map(|(_, token)| token.clone())
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
            Some(last) if last >= self.mark => self.base_offset + self.byte_ends[last],
            _ => start,
        };
        Loc::new(self.file, start, end - start)
    }

    fn emit(&mut self, token: Token) {
        let loc = self.current_loc();
        let leading_space = std::mem::take(&mut self.space_before);
        if let Some(starts) = &mut self.line_starts {
            starts.push(std::mem::take(&mut self.at_line_start));
        }
        self.tokens
            .push(Span::new(token, loc, loc).with_leading_space(leading_space));
    }

    pub fn tokenize(mut self) -> Vec<Span<Token>> {
        self.run();
        self.tokens
    }

    fn run(&mut self) -> Vec<bool> {
        while self.pos < self.chars.len() {
            self.mark = self.pos;
            self.scan_one();
        }
        self.line_starts.take().unwrap_or_default()
    }

    fn scan_one(&mut self) {
        let i = self.pos;
        let c = self.chars[i];

        if c == '\n' && self.line_starts.is_some() {
            self.pos += 1;
            self.at_line_start = true;
            self.space_before = true;
        } else if c.is_whitespace() {
            self.pos += 1;
            self.space_before = true;
        } else if self.try_consume("//") {
            while self.peek().is_some_and(|c| c != '\n') {
                self.pos += 1;
            }
            let text: String = self.chars[i..self.pos].iter().collect();
            self.emit(Token::Comment(text.into()));
            self.space_before = true;
        } else if self.try_consume("/*") {
            while self.pos < self.chars.len() && !self.peek_str("*/") {
                self.pos += 1;
            }
            self.pos = (self.pos + 2).min(self.chars.len());
            let text: String = self.chars[i..self.pos].iter().collect();
            self.emit(Token::Comment(text.into()));
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
                self.emit(Token::FloatLit(spelling.into()));
            } else {
                let digits = Self::integer_digits(&spelling).replace('\'', "");
                let octal_prefix = digits
                    .get(..2)
                    .is_some_and(|p| p.eq_ignore_ascii_case("0o"));
                let (radix, digits) = Self::integer_radix(&digits);
                if !digits.is_empty()
                    && digits.chars().all(|digit| digit.is_digit(radix))
                    && (self.features.octal_prefix || !octal_prefix)
                {
                    self.emit(Token::IntLit(spelling.into()));
                } else {
                    self.emit(Token::FloatLit(spelling.into()));
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
                0 => Token::StringLit(value.into()),
                1 if c == 'u' => Token::Utf16StringLit(value.into()),
                1 if c == 'U' => Token::Utf32StringLit(value.into()),
                1 => Token::WideStringLit(value.into()),
                _ => Token::Utf8StringLit(value.into()),
            };
            self.emit(token);
        } else if let Some(prefix_len) = self.char_prefix_len() {
            let start = i + prefix_len;
            let literal_close = self.literal_end(start, '\'').min(self.chars.len());
            let value: String = self.chars[start + 1..literal_close].iter().collect();
            let decoded = Self::decode_escapes(&self.chars, start + 1, literal_close);
            let token = match prefix_len {
                0 => Token::CharLit(value.into(), decoded.into()),
                1 if c == 'u' => Token::Utf16CharLit(value.into(), decoded.into()),
                1 if c == 'U' => Token::Utf32CharLit(value.into(), decoded.into()),
                1 => Token::WideCharLit(value.into(), decoded.into()),
                _ => Token::Utf8CharLit(value.into(), decoded.into()),
            };
            self.pos = self.past_literal(literal_close);
            self.emit(token);
        } else if c == '\'' {
            let literal_close = self.literal_end(i, '\'').min(self.chars.len());
            let value: String = self.chars[i + 1..literal_close].iter().collect();
            let decoded = Self::decode_escapes(&self.chars, i + 1, literal_close);
            self.pos = self.past_literal(literal_close);
            self.emit(Token::CharLit(value.into(), decoded.into()));
        } else if c == '"' {
            let literal_close = self.literal_end(i, '"');
            let value: String = self.chars[i + 1..literal_close.min(self.chars.len())]
                .iter()
                .collect();
            self.pos = self.past_literal(literal_close);
            self.emit(Token::StringLit(value.into()));
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
            self.emit(Token::Ident(word.into()));
        } else if let Some(token) = self.try_consume_digraph() {
            self.emit(token);
            if let Some(last) = self.tokens.last_mut() {
                last.digraph = true;
            }
        } else if let Some(token) = self.try_consume_op() {
            self.emit(token);
        } else {
            let token = SINGLE_CHAR_OPS
                .iter()
                .find(|(ch, _)| *ch == c)
                .map(|(_, token)| token.clone())
                .unwrap_or_else(|| Token::Ident(c.to_string().into()));
            self.pos += 1;
            self.emit(token);
        }
    }

    fn numeric_end(&self, start: usize) -> usize {
        let mut i = start;
        while let Some(&c) = self.chars.get(i) {
            let next = self.chars.get(i + 1).copied();
            if matches!(c, 'e' | 'E' | 'p' | 'P') && matches!(next, Some('+' | '-')) {
                i += 2;
            } else if c.is_ascii_alphanumeric() || c == '_' || c == '.' {
                i += 1;
            } else if c == '\''
                && self.features.digit_separators
                && next.is_some_and(|next| next.is_ascii_alphanumeric())
            {
                i += 2;
            } else {
                break;
            }
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
            (Some('u' | 'o' | 'x' | 'N'), Some('{')) => {
                let mut end = at + 3;
                while self.char_at(end).is_some_and(|c| c != '}') {
                    end += 1;
                }
                end.saturating_add(1).min(len)
            }
            (Some('u'), _) => (at + 6).min(len),
            (Some('U'), _) => (at + 10).min(len),
            _ => (at + 2).min(len),
        }
    }

    pub(crate) fn is_imaginary_integer(spelling: &str) -> bool {
        spelling.contains(['i', 'I', 'j', 'J'])
    }

    pub(crate) fn integer_radix(digits: &str) -> (u32, &str) {
        match digits.get(..2).map(str::to_ascii_lowercase).as_deref() {
            Some("0x") => (16, &digits[2..]),
            Some("0b") => (2, &digits[2..]),
            Some("0o") => (8, &digits[2..]),
            _ if digits.len() > 1 && digits.starts_with('0') => (8, &digits[1..]),
            _ => (10, digits),
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
            while end > 0
                && matches!(
                    bytes[end - 1],
                    b'u' | b'U' | b'l' | b'L' | b'w' | b'W' | b'i' | b'I' | b'j' | b'J'
                )
            {
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
        if matches!(Self::char_in(chars, j, end), Some('o' | 'x' | 'u'))
            && Self::char_in(chars, j + 1, end) == Some('{')
        {
            let radix = if chars[j] == 'o' { 8 } else { 16 };
            let mut next = j + 2;
            let mut value = 0u32;
            while let Some(digit) = Self::char_in(chars, next, end).and_then(|c| c.to_digit(radix))
            {
                value = value.wrapping_mul(radix).wrapping_add(digit);
                next += 1;
            }
            if Self::char_in(chars, next, end) == Some('}') {
                next += 1;
            }
            let unit = if chars[j] == 'u' {
                SourceUnit::Character(value)
            } else {
                SourceUnit::CodeUnit(value)
            };
            return (unit, next);
        }
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

fn splice_len(after_backslash: &str, whitespace_line_splice: bool) -> usize {
    let whitespace = if whitespace_line_splice {
        after_backslash
            .bytes()
            .take_while(|byte| matches!(byte, b' ' | b'\t' | b'\x0b' | b'\x0c'))
            .count()
    } else {
        0
    };
    let rest = &after_backslash[whitespace..];
    if rest.starts_with('\n') {
        whitespace + 1
    } else if rest.starts_with("\r\n") {
        whitespace + 2
    } else {
        0
    }
}

fn source_char(src: &str, byte: usize, trigraphs: bool) -> Option<(char, usize)> {
    let rest = &src[byte..];
    let replacement = match rest.as_bytes() {
        [b'?', b'?', third, ..] if trigraphs => match third {
            b'=' => Some('#'),
            b'(' => Some('['),
            b'/' => Some('\\'),
            b')' => Some(']'),
            b'\'' => Some('^'),
            b'<' => Some('{'),
            b'!' => Some('|'),
            b'>' => Some('}'),
            b'-' => Some('~'),
            _ => None,
        },
        _ => None,
    };
    match replacement {
        Some(c) => Some((c, 3)),
        None => rest.chars().next().map(|c| (c, c.len_utf8())),
    }
}

pub fn keyword_token(word: &str, features: &StandardFeatures) -> Option<Token> {
    Some(match word {
        "sizeof" => Token::Sizeof,
        "_Alignof" | "__alignof" | "__alignof__" => Token::Alignof,
        "alignof" if features.keyword_alignof.is_accepted() => Token::Alignof,
        "_Countof" if features.keyword_countof => Token::Countof,
        "_Maxof" if features.keyword_type_limits => Token::Maxof,
        "_Minof" if features.keyword_type_limits => Token::Minof,
        "_Bool" => Token::Keyword(Keyword::Bool),
        "bool" if features.keyword_bool_true_false.is_accepted() => Token::Keyword(Keyword::Bool),
        "__bf16" => Token::Keyword(Keyword::BFloat16),
        "char" => Token::Keyword(Keyword::Char),
        "double" => Token::Keyword(Keyword::Double),
        "float" => Token::Keyword(Keyword::Float),
        "_Float16" if features.keyword_float16.is_accepted() => Token::Keyword(Keyword::Float16),
        "__fp16" => Token::Keyword(Keyword::Fp16),
        "_Float32" if features.gnu_floating_keywords => Token::Keyword(Keyword::Float32),
        "_Float64" if features.gnu_floating_keywords => Token::Keyword(Keyword::Float64),
        "_Float32x" if features.gnu_floating_keywords => Token::Keyword(Keyword::Float32x),
        "_Float64x" if features.gnu_floating_keywords => Token::Keyword(Keyword::Float64x),
        "_Float128" if features.gnu_floating_keywords => Token::Keyword(Keyword::Float128),
        "__float128" => Token::Keyword(Keyword::Float128Ext),
        "__float80" if features.keyword_float80 => Token::Keyword(Keyword::Float80),
        "_Decimal32" if features.decimal_floating_point.is_accepted() => {
            Token::Keyword(Keyword::Decimal32)
        }
        "_Decimal64" if features.decimal_floating_point.is_accepted() => {
            Token::Keyword(Keyword::Decimal64)
        }
        "_Decimal128" if features.decimal_floating_point.is_accepted() => {
            Token::Keyword(Keyword::Decimal128)
        }
        "int" => Token::Keyword(Keyword::Int),
        "long" => Token::Keyword(Keyword::Long),
        "return" => Token::Keyword(Keyword::Return),
        "short" => Token::Keyword(Keyword::Short),
        "signed" | "__signed" | "__signed__" => Token::Keyword(Keyword::Signed),
        "typedef" => Token::Keyword(Keyword::Typedef),
        "unsigned" | "__unsigned" | "__unsigned__" => Token::Keyword(Keyword::Unsigned),
        "void" => Token::Keyword(Keyword::Void),
        "_Complex" | "__complex__" | "__complex" => Token::Keyword(Keyword::Complex),
        "struct" => Token::Keyword(Keyword::Struct),
        "union" => Token::Keyword(Keyword::Union),
        "enum" => Token::Keyword(Keyword::Enum),
        "const" | "__const" | "__const__" => Token::Keyword(Keyword::Const),
        "volatile" | "__volatile" | "__volatile__" => Token::Keyword(Keyword::Volatile),
        "restrict" if features.keyword_restrict.is_accepted() => Token::Keyword(Keyword::Restrict),
        "_Atomic" => Token::Keyword(Keyword::Atomic),
        "extern" => Token::Keyword(Keyword::Extern),
        "static" => Token::Keyword(Keyword::Static),
        "auto" => Token::Keyword(Keyword::Auto),
        "register" => Token::Keyword(Keyword::Register),
        "__inline" | "__inline__" => Token::Keyword(Keyword::Inline),
        "__forceinline" if features.microsoft_extensions => Token::Keyword(Keyword::ForceInline),
        "__unaligned" if features.microsoft_extensions => Token::Keyword(Keyword::Unaligned),
        "__ptr32" if features.microsoft_extensions => Token::Keyword(Keyword::Ptr32),
        "__ptr64" if features.microsoft_extensions => Token::Keyword(Keyword::Ptr64),
        "__sptr" if features.microsoft_extensions => Token::Keyword(Keyword::Sptr),
        "__uptr" if features.microsoft_extensions => Token::Keyword(Keyword::Uptr),
        "__seg_fs" if features.x86_segment_keywords => Token::Keyword(Keyword::SegFs),
        "__seg_gs" if features.x86_segment_keywords => Token::Keyword(Keyword::SegGs),
        "inline" if features.keyword_inline.is_accepted() => Token::Keyword(Keyword::Inline),
        "__int128" => Token::Keyword(Keyword::Int128),
        "__int8" | "_int8" if features.microsoft_extensions => Token::Keyword(Keyword::Char),
        "__int16" | "_int16" if features.microsoft_extensions => Token::Keyword(Keyword::Short),
        "__int32" | "_int32" if features.microsoft_extensions => Token::Keyword(Keyword::Int),
        "__int64" | "_int64" if features.microsoft_extensions => Token::Keyword(Keyword::Int64),
        "_Noreturn" => Token::Keyword(Keyword::Noreturn),
        "_Thread_local" | "__thread" => Token::Keyword(Keyword::ThreadLocal),
        "thread_local" if features.keyword_thread_local.is_accepted() => {
            Token::Keyword(Keyword::ThreadLocal)
        }
        "__restrict" | "__restrict__" => Token::Keyword(Keyword::Restrict),
        "_BitInt" => Token::Keyword(Keyword::BitInt),
        "_Accum" if features.fixed_point_keywords => Token::Keyword(Keyword::Accum),
        "_Fract" if features.fixed_point_keywords => Token::Keyword(Keyword::Fract),
        "_Sat" if features.fixed_point_keywords => Token::Keyword(Keyword::Saturated),
        "__typeof" | "__typeof__" => Token::Keyword(Keyword::Typeof),
        "typeof" if features.keyword_typeof.is_accepted() => Token::Keyword(Keyword::Typeof),
        "__typeof_unqual" | "__typeof_unqual__" => Token::Keyword(Keyword::TypeofUnqual),
        "typeof_unqual" if features.keyword_typeof_unqual.is_accepted() => {
            Token::Keyword(Keyword::TypeofUnqual)
        }
        "constexpr" if features.keyword_constexpr.is_accepted() => {
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
        "static_assert" if features.keyword_static_assert.is_accepted() => {
            Token::Keyword(Keyword::StaticAssert)
        }
        _ => return None,
    })
}
