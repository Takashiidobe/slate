use crate::ast::{FileId, Loc, Span};

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
    IntLit(i64),
    FloatLit(String),
    CharLit(String, i64),
    Utf8CharLit(String, i64),
    Utf16CharLit(String, i64),
    Utf32CharLit(String, i64),
    WideCharLit(String, i64),
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
            Token::IntLit(value) => value.to_string(),
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
    tokens: Vec<Span<Token>>,
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
            tokens: Vec::new(),
        }
    }

    pub fn with_newlines(mut self) -> Self {
        self.emit_newlines = true;
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
        self.tokens.push(Span::new(token, loc, loc));
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
        } else if c.is_whitespace() {
            self.pos += 1;
        } else if self.try_consume("//") {
            while self.peek().is_some_and(|c| c != '\n') {
                self.pos += 1;
            }
            let text: String = self.chars[i..self.pos].iter().collect();
            self.emit(Token::Comment(text));
        } else if self.try_consume("/*") {
            while self.pos < self.chars.len() && !self.peek_str("*/") {
                self.pos += 1;
            }
            self.pos = (self.pos + 2).min(self.chars.len());
            let text: String = self.chars[i..self.pos].iter().collect();
            self.emit(Token::Comment(text));
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
                let token = u64::from_str_radix(digits, radix).map_or_else(
                    |_| Token::FloatLit(spelling.clone()),
                    |value| Token::IntLit(value.min(i64::MAX as u64) as i64),
                );
                self.emit(token);
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
            let decoded = self.decode_char_literal(start + 1, literal_close);
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
            let decoded = self.decode_char_literal(i + 1, literal_close);
            self.pos = self.past_literal(literal_close);
            self.emit(Token::CharLit(value, decoded));
        } else if c == '"' {
            let literal_close = self.literal_end(i, '"');
            let value: String = self.chars[i + 1..literal_close.min(self.chars.len())]
                .iter()
                .collect();
            self.pos = self.past_literal(literal_close);
            self.emit(Token::StringLit(value));
        } else if c.is_ascii_alphabetic() || c == '_' || c == '\\' {
            while self.pos < self.chars.len()
                && (self.chars[self.pos].is_ascii_alphanumeric()
                    || self.chars[self.pos] == '_'
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
                "_Bool" | "bool" => Token::Keyword(Keyword::Bool),
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
                "int" => Token::Keyword(Keyword::Int),
                "long" => Token::Keyword(Keyword::Long),
                "return" => Token::Keyword(Keyword::Return),
                "short" => Token::Keyword(Keyword::Short),
                "signed" => Token::Keyword(Keyword::Signed),
                "typedef" => Token::Keyword(Keyword::Typedef),
                "unsigned" => Token::Keyword(Keyword::Unsigned),
                "void" => Token::Keyword(Keyword::Void),
                "_Complex" | "__complex__" => Token::Keyword(Keyword::Complex),
                "struct" => Token::Keyword(Keyword::Struct),
                "union" => Token::Keyword(Keyword::Union),
                "enum" => Token::Keyword(Keyword::Enum),
                "const" => Token::Keyword(Keyword::Const),
                "volatile" => Token::Keyword(Keyword::Volatile),
                "restrict" => Token::Keyword(Keyword::Restrict),
                "_Atomic" => Token::Keyword(Keyword::Atomic),
                "extern" => Token::Keyword(Keyword::Extern),
                "static" => Token::Keyword(Keyword::Static),
                "auto" => Token::Keyword(Keyword::Auto),
                "register" => Token::Keyword(Keyword::Register),
                "inline" | "__inline" | "__inline__" => Token::Keyword(Keyword::Inline),
                "__int128" => Token::Keyword(Keyword::Int128),
                "_Noreturn" => Token::Keyword(Keyword::Noreturn),
                "_Thread_local" | "thread_local" | "__thread" => {
                    Token::Keyword(Keyword::ThreadLocal)
                }
                "__restrict" | "__restrict__" => Token::Keyword(Keyword::Restrict),
                "_BitInt" => Token::Keyword(Keyword::BitInt),
                "_Accum" => Token::Keyword(Keyword::Accum),
                "_Fract" => Token::Keyword(Keyword::Fract),
                "_Sat" => Token::Keyword(Keyword::Saturated),
                "typeof" | "__typeof__" => Token::Keyword(Keyword::Typeof),
                "typeof_unqual" | "__typeof_unqual__" => Token::Keyword(Keyword::TypeofUnqual),
                "constexpr" => Token::Keyword(Keyword::Constexpr),
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
                "_Static_assert" | "static_assert" => Token::Keyword(Keyword::StaticAssert),
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
                || matches!(self.chars[i], '\'' | '.')
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
        if self.peek_str("u8") {
            return (self.peek_at(2) == Some(quote)).then_some(2);
        }
        if matches!(self.peek(), Some('u' | 'U' | 'L')) {
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

    fn integer_digits(spelling: &str) -> String {
        let mut end = spelling.len();
        let bytes = spelling.as_bytes();
        if end >= 2 && matches!(&bytes[end - 2..], b"wb" | b"WB") {
            end -= 2;
        }
        while end > 0 && matches!(bytes[end - 1], b'u' | b'U' | b'l' | b'L' | b'w' | b'W') {
            end -= 1;
        }
        spelling[..end].to_string()
    }

    fn decode_char_literal(&self, start: usize, end: usize) -> i64 {
        let mut codepoints = Vec::new();
        let mut i = start;
        while i < end {
            let (codepoint, next) = self.decode_char_escape(i, end);
            codepoints.push(codepoint);
            i = next;
        }
        match codepoints.as_slice() {
            [] => 0,
            [single] => i64::from(*single),
            multiple => multiple.iter().fold(0i64, |acc, &codepoint| {
                (acc << 8) | i64::from(codepoint as u8)
            }),
        }
    }

    fn char_in(&self, i: usize, end: usize) -> Option<char> {
        (i < end).then(|| self.chars[i])
    }

    fn decode_char_escape(&self, i: usize, end: usize) -> (u32, usize) {
        if self.chars[i] != '\\' {
            return (self.chars[i] as u32, i + 1);
        }
        let j = i + 1;
        match self.char_in(j, end) {
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
            Some('x') => self.hex_char_escape(j + 1, end, usize::MAX),
            Some('u') => self.hex_char_escape(j + 1, end, 4),
            Some('U') => self.hex_char_escape(j + 1, end, 8),
            Some(digit) if digit.is_digit(8) => {
                let mut e = j;
                let mut value = 0u32;
                let mut count = 0;
                while count < 3 && self.char_in(e, end).is_some_and(|c| c.is_digit(8)) {
                    value = value * 8 + self.chars[e].to_digit(8).unwrap();
                    e += 1;
                    count += 1;
                }
                (value, e)
            }
            Some(other) => (other as u32, j + 1),
            None => (0, j),
        }
    }

    fn hex_char_escape(&self, start: usize, end: usize, max_digits: usize) -> (u32, usize) {
        let mut e = start;
        let mut value = 0u32;
        while e - start < max_digits && self.char_in(e, end).is_some_and(|c| c.is_ascii_hexdigit())
        {
            value = value * 16 + self.chars[e].to_digit(16).unwrap();
            e += 1;
        }
        (value, e)
    }
}
