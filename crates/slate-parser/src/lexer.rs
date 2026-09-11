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

#[derive(Debug, Clone, PartialEq)]
pub enum Token {
    Keyword(Keyword),
    Ident(String),
    IntLit(i64),
    StringLit(String),
    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,
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
            while i < chars.len() && chars[i].is_ascii_digit() {
                i += 1;
            }
            let n: i64 = chars[start..i].iter().collect::<String>().parse().unwrap();
            tokens.push(Token::IntLit(n));
        } else if c == '"' {
            i += 1;
            let start = i;
            while i < chars.len() && chars[i] != '"' {
                i += 1;
            }
            assert!(i < chars.len(), "unterminated string literal");
            tokens.push(Token::StringLit(chars[start..i].iter().collect()));
            i += 1;
        } else if c.is_ascii_alphabetic() || c == '_' {
            let start = i;
            while i < chars.len() && (chars[i].is_ascii_alphanumeric() || chars[i] == '_') {
                i += 1;
            }
            let word: String = chars[start..i].iter().collect();
            tokens.push(match word.as_str() {
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
                other => panic!("unexpected character in phase 0/1 lexer: {other:?}"),
            };
            tokens.push(tok);
            i += 1;
        }
    }

    tokens
}
