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
    CharLit(String),
    Utf8CharLit(String),
    Utf16CharLit(String),
    Utf32CharLit(String),
    WideCharLit(String),
    StringLit(String),
    Utf8StringLit(String),
    Utf16StringLit(String),
    Utf32StringLit(String),
    WideStringLit(String),
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

impl From<&Token> for String {
    fn from(token: &Token) -> Self {
        match token {
            Token::Sizeof => "sizeof".into(),
            Token::Alignof => "_Alignof".into(),
            Token::Keyword(keyword) => <&str>::from(*keyword).into(),
            Token::Ident(name) => name.clone(),
            Token::IntLit(value) => value.to_string(),
            Token::FloatLit(value) => value.clone(),
            Token::CharLit(value) => format!("'{value}'"),
            Token::Utf8CharLit(value) => format!("u8'{value}'"),
            Token::Utf16CharLit(value) => format!("u'{value}'"),
            Token::Utf32CharLit(value) => format!("U'{value}'"),
            Token::WideCharLit(value) => format!("L'{value}'"),
            Token::StringLit(value) => format!("\"{value}\""),
            Token::Utf8StringLit(value) => format!("u8\"{value}\""),
            Token::Utf16StringLit(value) => format!("u\"{value}\""),
            Token::Utf32StringLit(value) => format!("U\"{value}\""),
            Token::WideStringLit(value) => format!("L\"{value}\""),
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

pub fn lex(src: &str) -> Vec<Token> {
    let chars: Vec<char> = src.chars().collect();
    let mut tokens = Vec::new();
    let mut i = 0;

    while i < chars.len() {
        let c = chars[i];

        if c.is_whitespace() {
            i += 1;
        } else if c == '/' && chars.get(i + 1) == Some(&'/') {
            while i < chars.len() && chars[i] != '\n' {
                i += 1;
            }
        } else if c == '/' && chars.get(i + 1) == Some(&'*') {
            i += 2;
            while i + 1 < chars.len() && !(chars[i] == '*' && chars[i + 1] == '/') {
                i += 1;
            }
            i += 2;
        } else if c.is_ascii_digit() {
            let start = i;
            i = numeric_end(&chars, i);
            let spelling: String = chars[start..i].iter().collect();
            if spelling.contains('.') || spelling.contains('e') || spelling.contains('E') {
                tokens.push(Token::FloatLit(spelling));
            } else {
                let digits = integer_digits(&spelling).replace('\'', "");
                let (radix, digits) = if digits.starts_with("0x") || digits.starts_with("0X") {
                    (16, &digits[2..])
                } else if digits.starts_with("0b") || digits.starts_with("0B") {
                    (2, &digits[2..])
                } else if digits.len() > 1 && digits.starts_with('0') {
                    (8, &digits[1..])
                } else {
                    (10, digits.as_str())
                };
                tokens.push(u64::from_str_radix(digits, radix).map_or_else(
                    |_| Token::FloatLit(spelling),
                    |value| Token::IntLit(value.min(i64::MAX as u64) as i64),
                ));
            }
        } else if let Some(prefix_len) = string_prefix_len(&chars, i) {
            let prefix = chars[i];
            let start = i + prefix_len;
            i = literal_end(&chars, start, '"');
            let value: String = chars[start + 1..i.min(chars.len())].iter().collect();
            tokens.push(match prefix_len {
                0 => Token::StringLit(value),
                1 if prefix == 'u' => Token::Utf16StringLit(value),
                1 if prefix == 'U' => Token::Utf32StringLit(value),
                1 => Token::WideStringLit(value),
                _ => Token::Utf8StringLit(value),
            });
            i = i.saturating_add(1).min(chars.len());
        } else if let Some(prefix_len) = char_prefix_len(&chars, i) {
            let start = i + prefix_len;
            let end = literal_end(&chars, start, '\'');
            let value: String = chars[start + 1..end.min(chars.len())].iter().collect();
            tokens.push(match prefix_len {
                0 => Token::CharLit(value),
                1 if chars[i] == 'u' => Token::Utf16CharLit(value),
                1 if chars[i] == 'U' => Token::Utf32CharLit(value),
                1 => Token::WideCharLit(value),
                _ => Token::Utf8CharLit(value),
            });
            i = end.saturating_add(1).min(chars.len());
        } else if c == '\'' {
            let end = literal_end(&chars, i, '\'');
            let value: String = chars[i + 1..end.min(chars.len())].iter().collect();
            tokens.push(Token::CharLit(value));
            i = end.saturating_add(1).min(chars.len());
        } else if c == '"' {
            let end = literal_end(&chars, i, '"');
            let value: String = chars[i + 1..end.min(chars.len())].iter().collect();
            tokens.push(Token::StringLit(value));
            i = end.saturating_add(1).min(chars.len());
        } else if c.is_ascii_alphabetic() || c == '_' || c == '\\' {
            let start = i;
            while i < chars.len()
                && (chars[i].is_ascii_alphanumeric() || chars[i] == '_' || chars[i] == '\\')
            {
                if chars[i] == '\\' {
                    i = universal_character_name_end(&chars, i);
                } else {
                    i += 1;
                }
            }
            let word: String = chars[start..i].iter().collect();
            tokens.push(match word.as_str() {
                "sizeof" => Token::Sizeof,
                "_Alignof" | "__alignof" | "__alignof__" => Token::Alignof,
                "_Bool" => Token::Keyword(Keyword::Bool),
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
                "_Complex" => Token::Keyword(Keyword::Complex),
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
                "inline" => Token::Keyword(Keyword::Inline),
                "__int128" => Token::Keyword(Keyword::Int128),
                "_Noreturn" => Token::Keyword(Keyword::Noreturn),
                "_Thread_local" | "__thread" => Token::Keyword(Keyword::ThreadLocal),
                "__restrict" | "__restrict__" => Token::Keyword(Keyword::Restrict),
                "_BitInt" => Token::Keyword(Keyword::BitInt),
                "_Accum" => Token::Keyword(Keyword::Accum),
                "_Fract" => Token::Keyword(Keyword::Fract),
                "_Sat" => Token::Keyword(Keyword::Saturated),
                "typeof" | "__typeof__" => Token::Keyword(Keyword::Typeof),
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
                _ => Token::Ident(word),
            });
        } else if c == '.' && chars.get(i..i + 3) == Some(&['.', '.', '.'][..]) {
            tokens.push(Token::Ellipsis);
            i += 3;
        } else if i + 2 < chars.len()
            && matches!(
                (c, chars[i + 1], chars[i + 2]),
                ('<', '<', '=') | ('>', '>', '=')
            )
        {
            let tok = match c {
                '<' => Token::ShiftLeftEqual,
                '>' => Token::ShiftRightEqual,
                _ => unreachable!(),
            };
            tokens.push(tok);
            i += 3;
        } else if i + 1 < chars.len()
            && matches!(
                (c, chars[i + 1]),
                ('<', '=')
                    | ('>', '=')
                    | ('=', '=')
                    | ('!', '=')
                    | ('&', '&')
                    | ('|', '|')
                    | ('<', '<')
                    | ('>', '>')
                    | ('-', '>')
                    | ('+', '+')
                    | ('-', '-')
                    | ('+', '=')
                    | ('-', '=')
                    | ('*', '=')
                    | ('/', '=')
                    | ('%', '=')
                    | ('&', '=')
                    | ('|', '=')
                    | ('^', '=')
            )
        {
            let tok = match (c, chars[i + 1]) {
                ('<', '=') => Token::LessEqual,
                ('>', '=') => Token::GreaterEqual,
                ('=', '=') => Token::EqualEqual,
                ('!', '=') => Token::NotEqual,
                ('&', '&') => Token::AndAnd,
                ('|', '|') => Token::OrOr,
                ('<', '<') => Token::ShiftLeft,
                ('>', '>') => Token::ShiftRight,
                ('-', '>') => Token::Arrow,
                ('+', '+') => Token::PlusPlus,
                ('-', '-') => Token::MinusMinus,
                ('+', '=') => Token::PlusEqual,
                ('-', '=') => Token::MinusEqual,
                ('*', '=') => Token::StarEqual,
                ('/', '=') => Token::SlashEqual,
                ('%', '=') => Token::PercentEqual,
                ('&', '=') => Token::AmpEqual,
                ('|', '=') => Token::PipeEqual,
                ('^', '=') => Token::CaretEqual,
                _ => unreachable!(),
            };
            tokens.push(tok);
            i += 2;
        } else {
            if c == '#' && chars.get(i + 1) == Some(&'#') {
                tokens.push(Token::HashHash);
                i += 2;
                continue;
            }
            let tok = match c {
                '(' => Token::LParen,
                ')' => Token::RParen,
                '{' => Token::LBrace,
                '}' => Token::RBrace,
                '[' => Token::LBracket,
                ']' => Token::RBracket,
                ':' => Token::Colon,
                ',' => Token::Comma,
                '#' => Token::Hash,
                '*' => Token::Star,
                ';' => Token::Semi,
                '=' => Token::Equal,
                '.' => Token::Dot,
                '+' => Token::Plus,
                '-' => Token::Minus,
                '/' => Token::Slash,
                '%' => Token::Percent,
                '!' => Token::Bang,
                '~' => Token::Tilde,
                '<' => Token::Less,
                '>' => Token::Greater,
                '&' => Token::Amp,
                '^' => Token::Caret,
                '|' => Token::Pipe,
                '?' => Token::Question,
                other => Token::Ident(other.to_string()),
            };
            tokens.push(tok);
            i += 1;
        }
    }

    tokens
}

fn numeric_end(chars: &[char], mut i: usize) -> usize {
    while i < chars.len()
        && (chars[i].is_ascii_alphanumeric()
            || matches!(chars[i], '\'' | '.')
            || (chars[i] == '+' || chars[i] == '-')
                && i > 0
                && matches!(chars[i - 1], 'e' | 'E' | 'p' | 'P'))
    {
        i += 1;
    }
    i
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

fn string_prefix_len(chars: &[char], i: usize) -> Option<usize> {
    if chars.get(i) == Some(&'"') {
        return Some(0);
    }
    if chars.get(i) == Some(&'u') && chars.get(i + 1) == Some(&'8') {
        return (chars.get(i + 2) == Some(&'"')).then_some(2);
    }
    if matches!(chars.get(i), Some('u' | 'U' | 'L')) {
        return (chars.get(i + 1) == Some(&'"')).then_some(1);
    }
    None
}

fn char_prefix_len(chars: &[char], i: usize) -> Option<usize> {
    if chars.get(i) == Some(&'\'') {
        return Some(0);
    }
    if chars.get(i) == Some(&'u') && chars.get(i + 1) == Some(&'8') {
        return (chars.get(i + 2) == Some(&'\'')).then_some(2);
    }
    if matches!(chars.get(i), Some('u' | 'U' | 'L')) {
        return (chars.get(i + 1) == Some(&'\'')).then_some(1);
    }
    None
}

fn literal_end(chars: &[char], start: usize, delimiter: char) -> usize {
    let mut i = start + 1;
    while i < chars.len() {
        if chars[i] == '\\' {
            i = universal_character_name_end(chars, i).max(i + 2);
        } else if chars[i] == delimiter {
            break;
        } else {
            i += 1;
        }
    }
    i
}

pub fn decode_char_literal(raw: &str) -> i64 {
    let chars: Vec<char> = raw.chars().collect();
    let mut codepoints = Vec::new();
    let mut i = 0;
    while i < chars.len() {
        let (codepoint, next) = decode_char_escape(&chars, i);
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

fn decode_char_escape(chars: &[char], i: usize) -> (u32, usize) {
    if chars[i] != '\\' {
        return (chars[i] as u32, i + 1);
    }
    let j = i + 1;
    match chars.get(j) {
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
        Some('x') => hex_char_escape(chars, j + 1, usize::MAX),
        Some('u') => hex_char_escape(chars, j + 1, 4),
        Some('U') => hex_char_escape(chars, j + 1, 8),
        Some(digit) if digit.is_digit(8) => {
            let mut end = j;
            let mut value = 0u32;
            let mut count = 0;
            while count < 3 && chars.get(end).is_some_and(|c| c.is_digit(8)) {
                value = value * 8 + chars[end].to_digit(8).unwrap();
                end += 1;
                count += 1;
            }
            (value, end)
        }
        Some(&other) => (other as u32, j + 1),
        None => (0, j),
    }
}

fn hex_char_escape(chars: &[char], start: usize, max_digits: usize) -> (u32, usize) {
    let mut end = start;
    let mut value = 0u32;
    while end - start < max_digits && chars.get(end).is_some_and(char::is_ascii_hexdigit) {
        value = value * 16 + chars[end].to_digit(16).unwrap();
        end += 1;
    }
    (value, end)
}

fn universal_character_name_end(chars: &[char], i: usize) -> usize {
    match (chars.get(i + 1), chars.get(i + 2)) {
        (Some('u'), _) => (i + 6).min(chars.len()),
        (Some('U'), _) => (i + 10).min(chars.len()),
        (Some('N'), Some('{')) => {
            let mut end = i + 3;
            while end < chars.len() && chars[end] != '}' {
                end += 1;
            }
            end.saturating_add(1).min(chars.len())
        }
        _ => (i + 2).min(chars.len()),
    }
}
