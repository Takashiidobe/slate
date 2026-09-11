#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Keyword {
    Char,
    Int,
    Return,
    Typedef,
    Void,
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
}

impl From<Keyword> for &'static str {
    fn from(keyword: Keyword) -> Self {
        match keyword {
            Keyword::Char => "char",
            Keyword::Int => "int",
            Keyword::Return => "return",
            Keyword::Typedef => "typedef",
            Keyword::Void => "void",
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
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Token {
    Keyword(Keyword),
    Sizeof,
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
                "char" => Token::Keyword(Keyword::Char),
                "int" => Token::Keyword(Keyword::Int),
                "return" => Token::Keyword(Keyword::Return),
                "typedef" => Token::Keyword(Keyword::Typedef),
                "void" => Token::Keyword(Keyword::Void),
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
                _ => Token::Ident(word),
            });
        } else if c == '.' && chars.get(i..i + 3) == Some(&['.', '.', '.'][..]) {
            tokens.push(Token::Ellipsis);
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
                _ => unreachable!(),
            };
            tokens.push(tok);
            i += 2;
        } else {
            let tok = match c {
                '(' => Token::LParen,
                ')' => Token::RParen,
                '{' => Token::LBrace,
                '}' => Token::RBrace,
                '[' => Token::LBracket,
                ']' => Token::RBracket,
                ':' => Token::Colon,
                ',' => Token::Comma,
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
