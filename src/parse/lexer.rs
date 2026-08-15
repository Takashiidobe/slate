use crate::parse::ast::Type;
use crate::parse::error::{CompileError, CompileResult, SourceLocation};
use std::collections::{HashMap, HashSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::sync::{Mutex, OnceLock};

#[derive(Debug, Clone)]
pub struct File {
    pub name: PathBuf,
    pub file_no: usize,
    pub contents: String,
    pub display_name: String,
    pub line_delta: i32,
    pub included_from: Option<(usize, SourceLocation)>,
}

#[derive(Debug, Clone, PartialEq, Default)]
pub struct HideSet {
    names: HashSet<String>,
}

impl HideSet {
    pub fn union(&self, other: &Self) -> Self {
        Self {
            names: self.names.union(&other.names).cloned().collect(),
        }
    }

    pub fn intersection(&self, other: &Self) -> Self {
        Self {
            names: self.names.intersection(&other.names).cloned().collect(),
        }
    }

    pub fn contains(&self, name: &str) -> bool {
        self.names.contains(name)
    }

    pub fn add(&mut self, name: impl Into<String>) {
        self.names.insert(name.into());
    }

    pub fn add_tokens(&self, tokens: &mut [Token]) {
        for tok in tokens {
            tok.hideset = tok.hideset.union(self);
        }
    }
}

static INPUT_FILES: OnceLock<Mutex<Vec<File>>> = OnceLock::new();
static BASE_FILE: OnceLock<String> = OnceLock::new();

fn input_files() -> &'static Mutex<Vec<File>> {
    INPUT_FILES.get_or_init(|| Mutex::new(Vec::new()))
}

pub fn get_input_files() -> Vec<File> {
    input_files()
        .lock()
        .map(|files| files.clone())
        .unwrap_or_default()
}

pub fn get_input_file(file_no: usize) -> Option<File> {
    if file_no == 0 {
        return None;
    }
    input_files()
        .lock()
        .ok()
        .and_then(|files| files.get(file_no - 1).cloned())
}

pub fn get_base_file() -> Option<String> {
    BASE_FILE.get().cloned()
}

fn set_base_file(path: &Path) {
    let normalized = path.strip_prefix(".").unwrap_or(path);
    BASE_FILE.get_or_init(|| normalized.to_string_lossy().into_owned());
}

pub fn set_line_marker(file_no: usize, line_delta: i32, display_name: Option<String>) {
    if file_no == 0 {
        return;
    }
    if let Ok(mut files) = input_files().lock()
        && let Some(file) = files.get_mut(file_no - 1)
    {
        file.line_delta = line_delta;
        if let Some(name) = display_name {
            file.display_name = name;
        }
    }
}

fn register_file(
    path: PathBuf,
    contents: String,
    included_from: Option<(usize, SourceLocation)>,
) -> File {
    let mut files = input_files().lock().expect("input files lock poisoned");
    let file_no = files.len() + 1;
    let display_name = path.to_string_lossy().into_owned();
    let file = File {
        name: path,
        file_no,
        contents,
        display_name,
        line_delta: 0,
        included_from,
    };
    files.push(file.clone());
    file
}

pub fn tokenize_file(path: &Path) -> CompileResult<Vec<Token>> {
    tokenize_file_with_provenance(path, None)
}

pub fn tokenize_included_file(
    path: &Path,
    included_from: (usize, SourceLocation),
) -> CompileResult<Vec<Token>> {
    tokenize_file_with_provenance(path, Some(included_from))
}

fn tokenize_file_with_provenance(
    path: &Path,
    included_from: Option<(usize, SourceLocation)>,
) -> CompileResult<Vec<Token>> {
    let mut contents = fs::read_to_string(path)
        .map_err(|err| CompileError::new(format!("failed to read {}: {err}", path.display())))?;
    if contents.as_bytes().starts_with(b"\xEF\xBB\xBF") {
        contents = contents[3..].to_string();
    }
    let contents = Lexer::canonicalize_newline(&contents);
    let contents = Lexer::remove_backslash_newline(&contents);
    let contents = Lexer::convert_universal_chars(&contents);
    set_base_file(path);
    let file = register_file(path.to_path_buf(), contents, included_from);
    tokenize(&file.contents, file.file_no)
}

pub fn tokenize_builtin(name: &str, contents: &str) -> CompileResult<Vec<Token>> {
    let file = register_file(PathBuf::from(name), contents.to_string(), None);
    tokenize(&file.contents, file.file_no)
}

pub fn include_chain(file_no: usize) -> Vec<PathBuf> {
    let mut chain = Vec::new();
    let mut current = file_no;
    while let Some(file) = get_input_file(current) {
        let parent = file.included_from.map(|(parent_no, _)| parent_no);
        chain.push(file.name);
        match parent {
            Some(parent_no) => current = parent_no,
            None => break,
        }
    }
    chain.reverse();
    chain
}

pub fn tokenize_string_literal(token: &Token, base: &Type) -> CompileResult<Token> {
    let file = get_input_file(token.location.file_no).expect("file for token");
    let input = file.contents.as_bytes();
    let start = token.location.byte;
    if start >= input.len() {
        return Err(CompileError::at("invalid string literal", token.location));
    }
    let location = token.location;
    let first = input[start];
    let is_u8 = first == b'u'
        && start + 2 < input.len()
        && input[start + 1] == b'8'
        && input[start + 2] == b'"';
    let is_u = first == b'u' && start + 1 < input.len() && input[start + 1] == b'"';
    let is_u32 = first == b'U' && start + 1 < input.len() && input[start + 1] == b'"';
    let is_wide = first == b'L' && start + 1 < input.len() && input[start + 1] == b'"';
    let quote = if is_u8 {
        start + 2
    } else if is_u || is_u32 || is_wide {
        start + 1
    } else {
        start
    };
    let mut new_token = if base.size() == 1 {
        Lexer::read_string_literal(input, start, quote, location)?
    } else if base.size() == 2 {
        Lexer::read_utf16_string_literal(input, start, quote, location)?
    } else {
        Lexer::read_utf32_string_literal(input, start, quote, location, base.clone())?
    };
    new_token.location = location;
    new_token.at_bol = token.at_bol;
    new_token.has_space = token.has_space;
    new_token.hideset = token.hideset.clone();
    new_token.origin = token.origin;
    Ok(new_token)
}
/// Parse hexadecimal floating-point literal (e.g., 0x1.2p3)
/// Format: 0x[hex_digits][.hex_digits]p[+/-][decimal_exponent]
fn parse_hex_float(s: &str) -> Option<f64> {
    let s = s.strip_prefix("0x").or_else(|| s.strip_prefix("0X"))?;

    // Find the 'p' or 'P' that separates mantissa from exponent
    let (mantissa_str, exp_str) = if let Some(pos) = s.find(['p', 'P']) {
        (&s[..pos], &s[pos + 1..])
    } else {
        (s, "0")
    };

    // Parse the mantissa (integer and fractional parts)
    let (int_part, frac_part) = if let Some(dot_pos) = mantissa_str.find('.') {
        (&mantissa_str[..dot_pos], &mantissa_str[dot_pos + 1..])
    } else {
        (mantissa_str, "")
    };

    // Parse integer part as hex
    let int_value = if int_part.is_empty() {
        0.0
    } else {
        u64::from_str_radix(int_part, 16).ok()? as f64
    };

    // Parse fractional part as hex
    let mut frac_value = 0.0;
    if !frac_part.is_empty() {
        let frac_int = u64::from_str_radix(frac_part, 16).ok()? as f64;
        frac_value = frac_int / 16f64.powi(frac_part.len() as i32);
    }

    // Parse binary exponent (power of 2)
    let exponent: i32 = exp_str.parse().ok()?;

    // Combine: (integer + fraction) * 2^exponent
    Some((int_value + frac_value) * 2f64.powi(exponent))
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Keyword {
    Void,
    Bool,
    True,
    False,
    NullPtr,
    Char,
    Short,
    Int,
    Long,
    Float,
    Double,
    Enum,
    Struct,
    Union,
    Typedef,
    Static,
    Extern,
    Return,
    If,
    Else,
    For,
    While,
    Do,
    Sizeof,
    Alignof,
    Alignas,
    Goto,
    Break,
    Continue,
    Switch,
    Case,
    Default,
    Signed,
    Unsigned,
    Const,
    Volatile,
    Auto,
    Register,
    Restrict,
    Noreturn,
    Generic,
    Typeof,
    TypeofUnqual,
    Constexpr,
    Inline,
    Asm,
    ThreadLocal,
    Atomic,
    Complex,
    Real,
    Imag,
    Attribute,
    StaticAssert,
    BitInt,
}

impl std::fmt::Display for Keyword {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let text = match self {
            Keyword::Void => "void",
            Keyword::Bool => "bool",
            Keyword::True => "true",
            Keyword::False => "false",
            Keyword::NullPtr => "nullptr",
            Keyword::Char => "char",
            Keyword::Short => "short",
            Keyword::Int => "int",
            Keyword::Long => "long",
            Keyword::Float => "float",
            Keyword::Double => "double",
            Keyword::Enum => "enum",
            Keyword::Struct => "struct",
            Keyword::Union => "union",
            Keyword::Typedef => "typedef",
            Keyword::Static => "static",
            Keyword::Extern => "extern",
            Keyword::Return => "return",
            Keyword::If => "if",
            Keyword::Else => "else",
            Keyword::For => "for",
            Keyword::While => "while",
            Keyword::Do => "do",
            Keyword::Sizeof => "sizeof",
            Keyword::Alignof => "_Alignof",
            Keyword::Alignas => "_Alignas",
            Keyword::Goto => "goto",
            Keyword::Break => "break",
            Keyword::Continue => "continue",
            Keyword::Switch => "switch",
            Keyword::Case => "case",
            Keyword::Default => "default",
            Keyword::Signed => "signed",
            Keyword::Unsigned => "unsigned",
            Keyword::Const => "const",
            Keyword::Volatile => "volatile",
            Keyword::Auto => "auto",
            Keyword::Register => "register",
            Keyword::Restrict => "restrict",
            Keyword::Noreturn => "_Noreturn",
            Keyword::Generic => "_Generic",
            Keyword::Typeof => "typeof",
            Keyword::TypeofUnqual => "typeof_unqual",
            Keyword::Constexpr => "constexpr",
            Keyword::Inline => "inline",
            Keyword::Asm => "asm",
            Keyword::ThreadLocal => "_Thread_local",
            Keyword::Atomic => "_Atomic",
            Keyword::Complex => "_Complex",
            Keyword::Real => "__real__",
            Keyword::Imag => "__imag__",
            Keyword::Attribute => "__attribute__",
            Keyword::StaticAssert => "_Static_assert",
            Keyword::BitInt => "_BitInt",
        };
        f.write_str(text)
    }
}

#[derive(Debug, Clone, PartialEq, Default)]
pub enum TokenKind {
    Keyword(Keyword),
    Ident(String),
    Num {
        value: i64,
        fval: f64,
        ty: Type,
        ldouble: Option<rustc_apfloat::ieee::X87DoubleExtended>,
    },
    PPNum, // Preprocessing numbers
    Str {
        bytes: Vec<u8>,
        ty: Type,
    },
    Punct(Punct),
    #[default]
    Eof,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Punct {
    Plus,
    Minus,
    Star,
    Amp,
    Hash,
    HashHash,
    Pipe,
    Caret,
    Slash,
    Mod,
    Backtick,
    Dot,
    Ellipsis,
    Arrow,
    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,
    Semicolon,
    Comma,
    Colon,
    Question,
    Assign,
    AddAssign,
    SubAssign,
    MulAssign,
    DivAssign,
    ModAssign,
    AndAssign,
    OrAssign,
    XorAssign,
    ShlAssign,
    ShrAssign,
    Inc,
    Dec,
    Not,
    BitNot,
    LogAnd,
    LogOr,
    Shl,
    Shr,
    EqEq,
    NotEq,
    Less,
    LessEq,
    Greater,
    GreaterEq,
}

impl std::fmt::Display for Punct {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let text = match self {
            Punct::Plus => "+",
            Punct::Minus => "-",
            Punct::Star => "*",
            Punct::Amp => "&",
            Punct::Hash => "#",
            Punct::HashHash => "##",
            Punct::Pipe => "|",
            Punct::Caret => "^",
            Punct::Slash => "/",
            Punct::Mod => "%",
            Punct::Backtick => "`",
            Punct::Dot => ".",
            Punct::Ellipsis => "...",
            Punct::Arrow => "->",
            Punct::LParen => "(",
            Punct::RParen => ")",
            Punct::LBrace => "{",
            Punct::RBrace => "}",
            Punct::LBracket => "[",
            Punct::RBracket => "]",
            Punct::Semicolon => ";",
            Punct::Comma => ",",
            Punct::Colon => ":",
            Punct::Question => "?",
            Punct::Assign => "=",
            Punct::AddAssign => "+=",
            Punct::SubAssign => "-=",
            Punct::MulAssign => "*=",
            Punct::DivAssign => "/=",
            Punct::ModAssign => "%=",
            Punct::AndAssign => "&=",
            Punct::OrAssign => "|=",
            Punct::XorAssign => "^=",
            Punct::ShlAssign => "<<=",
            Punct::ShrAssign => ">>=",
            Punct::Inc => "++",
            Punct::Dec => "--",
            Punct::Not => "!",
            Punct::BitNot => "~",
            Punct::LogAnd => "&&",
            Punct::LogOr => "||",
            Punct::Shl => "<<",
            Punct::Shr => ">>",
            Punct::EqEq => "==",
            Punct::NotEq => "!=",
            Punct::Less => "<",
            Punct::LessEq => "<=",
            Punct::Greater => ">",
            Punct::GreaterEq => ">=",
        };
        f.write_str(text)
    }
}

#[derive(Debug, Clone, PartialEq, Default)]
pub struct Token {
    pub kind: TokenKind,
    pub location: SourceLocation,
    pub at_bol: bool,
    pub has_space: bool, // True if this token follows a space character
    pub len: usize,
    pub hideset: HideSet,               // For macro expansion
    pub origin: Option<SourceLocation>, // Macro expansion origin
    pub line_delta: i32,
}

fn integer_literal_bit_width(digits: &str, base: u32) -> u32 {
    if base == 2 {
        return digits.trim_start_matches('0').len().max(1) as u32;
    }
    if base == 8 || base == 16 {
        let bits_per_digit = if base == 8 { 3 } else { 4 };
        let trimmed = digits.trim_start_matches('0');
        if trimmed.is_empty() {
            return 1;
        }
        let first_value = trimmed.as_bytes()[0] as char;
        let first_value = first_value.to_digit(base).unwrap_or(0);
        let leading_bits = 32 - first_value.max(1).leading_zeros();
        return (trimmed.len() as u32 - 1) * bits_per_digit + leading_bits;
    }

    let mut digits: Vec<u8> = digits.bytes().map(|b| b - b'0').collect();
    if digits.iter().all(|&d| d == 0) {
        return 1;
    }
    let mut bits = 0u32;
    while !(digits.len() == 1 && digits[0] == 0) {
        let mut carry = 0u32;
        let mut quotient = Vec::with_capacity(digits.len());
        for &d in &digits {
            let cur = carry * 10 + d as u32;
            quotient.push((cur / 2) as u8);
            carry = cur % 2;
        }
        while quotient.len() > 1 && quotient[0] == 0 {
            quotient.remove(0);
        }
        digits = quotient;
        bits += 1;
    }
    bits
}

impl Token {
    pub fn text(&self) -> String {
        let file = get_input_file(self.location.file_no).expect("file for token");
        file.contents[self.location.byte..self.location.byte + self.len].to_string()
    }

    // Try to convert a pp-number token to an integer.
    fn convert_pp_int(&mut self) -> CompileResult<()> {
        let input = self.text();
        let bytes = input.as_bytes();
        let mut i = 0;

        // Determine base from prefix
        let (base, prefix_len) = if i + 1 < bytes.len() && bytes[i] == b'0' {
            if bytes[i + 1] == b'x' || bytes[i + 1] == b'X' {
                if i + 2 < bytes.len() && bytes[i + 2].is_ascii_hexdigit() {
                    (16, 2)
                } else {
                    (10, 0)
                }
            } else if bytes[i + 1] == b'b' || bytes[i + 1] == b'B' {
                if i + 2 < bytes.len() && (bytes[i + 2] == b'0' || bytes[i + 2] == b'1') {
                    (2, 2)
                } else {
                    (10, 0)
                }
            } else {
                (8, 0)
            }
        } else {
            (10, 0)
        };

        i += prefix_len;
        // Parse digits with optional separators.
        let is_valid_digit = |ch: u8| match base {
            2 => ch == b'0' || ch == b'1',
            8 => (b'0'..=b'7').contains(&ch),
            10 => ch.is_ascii_digit(),
            16 => ch.is_ascii_hexdigit(),
            _ => false,
        };

        let mut digits = Vec::new();
        let mut saw_digit = false;
        while i < bytes.len() {
            let ch = bytes[i];
            if ch == b'\'' {
                let next = bytes.get(i + 1).copied();
                let next_is_digit = next.is_some_and(is_valid_digit);
                if !saw_digit || !next_is_digit {
                    return Err(CompileError::at("invalid integer constant", self.location));
                }
                i += 1;
                continue;
            }
            if is_valid_digit(ch) {
                digits.push(ch);
                saw_digit = true;
                i += 1;
                continue;
            }
            break;
        }
        if !saw_digit {
            return Err(CompileError::at("invalid integer constant", self.location));
        }

        let num_str = String::from_utf8(digits).expect("digit bytes");

        {
            let suffix = &input[i..];
            let (has_u_prefix, rest) = if suffix.starts_with(['u', 'U']) {
                (true, &suffix[1..])
            } else {
                (false, suffix)
            };
            if rest.eq_ignore_ascii_case("wb") {
                let is_signed = !has_u_prefix;
                let width =
                    (integer_literal_bit_width(&num_str, base) + u32::from(is_signed)).min(512);
                self.kind = TokenKind::Num {
                    value: 0,
                    fval: 0.0,
                    ty: Type::BitInt {
                        width: width as i32,
                        is_signed,
                    },
                    ldouble: None,
                };
                return Ok(());
            }
        }

        let value = u128::from_str_radix(&num_str, base)
            .map_err(|_| CompileError::at("invalid integer constant", self.location))?;
        let value = value.min(u64::MAX as u128) as u64;

        // Read U, L or LL suffixes
        let mut has_long = false;
        let mut has_unsigned = false;
        let suffix = &input[i..];

        if suffix.starts_with("LLU")
            || suffix.starts_with("LLu")
            || suffix.starts_with("llU")
            || suffix.starts_with("llu")
            || suffix.starts_with("ULL")
            || suffix.starts_with("Ull")
            || suffix.starts_with("uLL")
            || suffix.starts_with("ull")
        {
            i += 3;
            has_long = true;
            has_unsigned = true;
        } else if suffix.len() >= 2
            && (suffix[..2].eq_ignore_ascii_case("lu") || suffix[..2].eq_ignore_ascii_case("ul"))
        {
            i += 2;
            has_long = true;
            has_unsigned = true;
        } else if suffix.starts_with("LL") || suffix.starts_with("ll") {
            i += 2;
            has_long = true;
        } else if suffix.starts_with('L') || suffix.starts_with('l') {
            i += 1;
            has_long = true;
        } else if suffix.starts_with('U') || suffix.starts_with('u') {
            i += 1;
            has_unsigned = true;
        }

        // Check we consumed the entire token
        if i != input.len() {
            return Err(CompileError::at("invalid integer constant", self.location));
        }

        // Infer type
        let ty = if base == 10 {
            if has_long && has_unsigned {
                Type::ULong
            } else if has_long {
                Type::Long
            } else if has_unsigned {
                if (value >> 32) != 0 {
                    Type::ULong
                } else {
                    Type::UInt
                }
            } else if (value >> 31) != 0 {
                Type::Long
            } else {
                Type::Int
            }
        } else if has_long && has_unsigned {
            Type::ULong
        } else if has_long {
            if (value >> 63) != 0 {
                Type::ULong
            } else {
                Type::Long
            }
        } else if has_unsigned {
            if (value >> 32) != 0 {
                Type::ULong
            } else {
                Type::UInt
            }
        } else if (value >> 63) != 0 {
            Type::ULong
        } else if (value >> 32) != 0 {
            Type::Long
        } else if (value >> 31) != 0 {
            Type::UInt
        } else {
            Type::Int
        };

        self.kind = TokenKind::Num {
            value: value as i64,
            fval: 0.0,
            ty,
            ldouble: None,
        };
        Ok(())
    }

    // Convert a pp-number token to a regular number token.
    fn convert_pp_number(&mut self) -> CompileResult<()> {
        // Try to parse as an integer constant
        if self.convert_pp_int().is_ok() {
            return Ok(());
        }

        // If it's not an integer, it must be a floating point constant
        let input = self.text();
        let mut parse_str = input.as_str();

        // Check for imaginary suffix (i or I) - must come last
        let is_imaginary = parse_str.ends_with('i') || parse_str.ends_with('I');
        if is_imaginary {
            parse_str = &parse_str[..parse_str.len() - 1];
        }

        // Check for float/long double suffix
        let base_ty = if parse_str.ends_with('f') || parse_str.ends_with('F') {
            parse_str = &parse_str[..parse_str.len() - 1];
            Type::Float
        } else if parse_str.ends_with('l') || parse_str.ends_with('L') {
            parse_str = &parse_str[..parse_str.len() - 1];
            Type::LDouble
        } else if parse_str.ends_with('q') || parse_str.ends_with('Q') {
            parse_str = &parse_str[..parse_str.len() - 1];
            Type::F128
        } else {
            Type::Double
        };

        // If imaginary, convert base type to complex type
        let ty = if is_imaginary {
            match base_ty {
                Type::Float => Type::FloatComplex,
                Type::Double => Type::DoubleComplex,
                Type::LDouble => Type::LDoubleComplex,
                _ => base_ty,
            }
        } else {
            base_ty
        };

        let cleaned = parse_str.replace('\'', "");
        let parse_clean = cleaned.as_str();

        if matches!(ty, Type::LDouble | Type::LDoubleComplex) {
            let value = parse_clean
                .parse::<rustc_apfloat::ieee::X87DoubleExtended>()
                .map_err(|_| {
                    CompileError::at("invalid numeric constant".to_string(), self.location)
                })?;
            self.kind = TokenKind::Num {
                value: 0,
                fval: 0.0,
                ty,
                ldouble: Some(value),
            };
            return Ok(());
        }

        let fval = if parse_clean.starts_with("0x") || parse_clean.starts_with("0X") {
            parse_hex_float(parse_clean).ok_or_else(|| {
                CompileError::at("invalid numeric constant".to_string(), self.location)
            })?
        } else {
            parse_clean.parse::<f64>().map_err(|_| {
                CompileError::at("invalid numeric constant".to_string(), self.location)
            })?
        };

        self.kind = TokenKind::Num {
            value: 0,
            fval,
            ty,
            ldouble: None,
        };
        Ok(())
    }
}

struct Lexer<'a> {
    input: &'a [u8],
    input_str: &'a str,
    file_no: usize,
    pos: usize,
    line: usize,
    column: usize,
    at_bol: bool,
    has_space: bool,
}

impl Default for Lexer<'_> {
    fn default() -> Self {
        Self {
            input: &[],
            input_str: "",
            file_no: 0,
            pos: 0,
            line: 1,
            column: 1,
            at_bol: true,
            has_space: false,
        }
    }
}

impl<'a> Lexer<'a> {
    fn new(input: &'a str, file_no: usize) -> Self {
        Self {
            input: input.as_bytes(),
            input_str: input,
            file_no,
            ..Default::default()
        }
    }

    fn location(&self) -> SourceLocation {
        SourceLocation {
            line: self.line,
            column: self.column,
            byte: self.pos,
            file_no: self.file_no,
        }
    }

    fn make_token(&self, kind: TokenKind, start: usize, start_column: usize) -> Token {
        Token {
            kind,
            location: SourceLocation {
                line: self.line,
                column: start_column,
                byte: start,
                file_no: self.file_no,
            },
            at_bol: self.at_bol,
            has_space: self.has_space,
            len: self.pos - start,
            ..Default::default()
        }
    }

    fn eof_token(&self) -> Token {
        Token {
            kind: TokenKind::Eof,
            location: self.location(),
            at_bol: self.at_bol,
            ..Default::default()
        }
    }

    // Text preprocessing functions (called before tokenization)

    fn remove_backslash_newline(input: &str) -> String {
        let bytes = input.as_bytes();
        let mut out = Vec::with_capacity(bytes.len());
        let mut i = 0;
        let mut pending_newlines = 0;

        while i < bytes.len() {
            if bytes[i] == b'\\' && i + 1 < bytes.len() && bytes[i + 1] == b'\n' {
                i += 2;
                pending_newlines += 1;
                continue;
            }

            if bytes[i] == b'\n' {
                out.push(b'\n');
                i += 1;
                if pending_newlines > 0 {
                    out.extend(std::iter::repeat_n(b'\n', pending_newlines));
                    pending_newlines = 0;
                }
                continue;
            }

            out.push(bytes[i]);
            i += 1;
        }

        if pending_newlines > 0 {
            out.extend(std::iter::repeat_n(b'\n', pending_newlines));
        }

        String::from_utf8(out).expect("line continuation removal")
    }

    fn canonicalize_newline(input: &str) -> String {
        let bytes = input.as_bytes();
        let mut out = Vec::with_capacity(bytes.len());
        let mut i = 0;

        while i < bytes.len() {
            if bytes[i] == b'\r' {
                if i + 1 < bytes.len() && bytes[i + 1] == b'\n' {
                    i += 2;
                } else {
                    i += 1;
                }
                out.push(b'\n');
                continue;
            }

            out.push(bytes[i]);
            i += 1;
        }

        String::from_utf8(out).expect("newline canonicalization")
    }

    fn read_universal_char(bytes: &[u8]) -> Option<u32> {
        let mut c = 0u32;
        for &b in bytes {
            if !b.is_ascii_hexdigit() {
                return None;
            }
            c = (c << 4) | Self::from_hex(b);
        }
        Some(c)
    }

    fn convert_universal_chars(input: &str) -> String {
        let bytes = input.as_bytes();
        let mut out = Vec::with_capacity(bytes.len());
        let mut i = 0;

        while i < bytes.len() {
            if bytes[i] == b'\\' && i + 1 < bytes.len() {
                if bytes[i + 1] == b'u' {
                    if i + 6 <= bytes.len()
                        && let Some(c) = Self::read_universal_char(&bytes[i + 2..i + 6])
                        && c != 0
                    {
                        i += 6;
                        Self::encode_utf8(&mut out, c);
                        continue;
                    }
                    out.push(bytes[i]);
                    i += 1;
                    continue;
                }

                if bytes[i + 1] == b'U' {
                    if i + 10 <= bytes.len()
                        && let Some(c) = Self::read_universal_char(&bytes[i + 2..i + 10])
                        && c != 0
                    {
                        i += 10;
                        Self::encode_utf8(&mut out, c);
                        continue;
                    }
                    out.push(bytes[i]);
                    i += 1;
                    continue;
                }

                out.push(bytes[i]);
                out.push(bytes[i + 1]);
                i += 2;
                continue;
            }

            out.push(bytes[i]);
            i += 1;
        }

        String::from_utf8(out).expect("universal character conversion")
    }

    // Character utility functions

    fn is_hex_digit(b: u8) -> bool {
        b.is_ascii_hexdigit()
    }

    fn from_hex(b: u8) -> u32 {
        match b {
            b'0'..=b'9' => (b - b'0') as u32,
            b'a'..=b'f' => (b - b'a' + 10) as u32,
            _ => (b - b'A' + 10) as u32,
        }
    }

    fn encode_utf8(out: &mut Vec<u8>, c: u32) -> usize {
        if c <= 0x7F {
            out.push(c as u8);
            return 1;
        }

        if c <= 0x7FF {
            out.push((0b1100_0000 | (c >> 6)) as u8);
            out.push((0b1000_0000 | (c & 0b0011_1111)) as u8);
            return 2;
        }

        if c <= 0xFFFF {
            out.push((0b1110_0000 | (c >> 12)) as u8);
            out.push((0b1000_0000 | ((c >> 6) & 0b0011_1111)) as u8);
            out.push((0b1000_0000 | (c & 0b0011_1111)) as u8);
            return 3;
        }

        out.push((0b1111_0000 | (c >> 18)) as u8);
        out.push((0b1000_0000 | ((c >> 12) & 0b0011_1111)) as u8);
        out.push((0b1000_0000 | ((c >> 6) & 0b0011_1111)) as u8);
        out.push((0b1000_0000 | (c & 0b0011_1111)) as u8);
        4
    }

    fn decode_utf8(input: &[u8], pos: &mut usize, location: SourceLocation) -> CompileResult<u32> {
        if *pos >= input.len() {
            return Ok(0);
        }

        let b = input[*pos];
        if b < 0x80 {
            *pos += 1;
            return Ok(b as u32);
        }

        let (len, mut c) = if b >= 0b1111_0000 {
            (4, (b & 0b0000_0111) as u32)
        } else if b >= 0b1110_0000 {
            (3, (b & 0b0000_1111) as u32)
        } else if b >= 0b1100_0000 {
            (2, (b & 0b0001_1111) as u32)
        } else {
            return Err(CompileError::at("invalid UTF-8 sequence", location));
        };

        if *pos + len > input.len() {
            return Err(CompileError::at("invalid UTF-8 sequence", location));
        }

        for i in 1..len {
            let byte = input[*pos + i];
            if byte >> 6 != 0b10 {
                return Err(CompileError::at("invalid UTF-8 sequence", location));
            }
            c = (c << 6) | (byte & 0b0011_1111) as u32;
        }

        *pos += len;
        Ok(c)
    }

    fn tokenize(mut self) -> CompileResult<Vec<Token>> {
        let mut tokens = Vec::new();

        while self.pos < self.input.len() {
            let b = self.input[self.pos];
            if b == b'/' && self.pos + 1 < self.input.len() {
                if self.input[self.pos + 1] == b'/' {
                    self.pos += 2;
                    self.column += 2;
                    while self.pos < self.input.len() && self.input[self.pos] != b'\n' {
                        self.pos += 1;
                        self.column += 1;
                    }
                    self.has_space = true;
                    continue;
                }
                if self.input[self.pos + 1] == b'*' {
                    let start_location = self.location();
                    self.pos += 2;
                    self.column += 2;
                    let mut closed = false;
                    while self.pos + 1 < self.input.len() {
                        if self.input[self.pos] == b'*' && self.input[self.pos + 1] == b'/' {
                            self.pos += 2;
                            self.column += 2;
                            closed = true;
                            break;
                        }
                        if self.input[self.pos] == b'\n' {
                            self.line += 1;
                            self.column = 1;
                            self.pos += 1;
                            self.at_bol = true;
                        } else {
                            self.pos += 1;
                            self.column += 1;
                        }
                    }
                    if !closed {
                        return Err(CompileError::at("unclosed block comment", start_location));
                    }
                    self.has_space = true;
                    continue;
                }
            }
            if b == b'\n' {
                self.line += 1;
                self.column = 1;
                self.pos += 1;
                self.at_bol = true;
                self.has_space = false;
                continue;
            }
            if b.is_ascii_whitespace() {
                self.column += 1;
                self.pos += 1;
                self.has_space = true;
                continue;
            }

            // Numeric literal
            if b.is_ascii_digit()
                || (b == b'.'
                    && self.pos + 1 < self.input.len()
                    && self.input[self.pos + 1].is_ascii_digit())
            {
                let start = self.pos;
                let start_column = self.column;
                self.pos += 1;

                // Read a pp-number following C preprocessing rules
                loop {
                    if self.pos + 1 < self.input.len()
                        && matches!(self.input[self.pos], b'e' | b'E' | b'p' | b'P')
                        && matches!(self.input[self.pos + 1], b'+' | b'-')
                    {
                        self.pos += 2;
                    } else if self.pos < self.input.len()
                        && (self.input[self.pos].is_ascii_alphanumeric()
                            || self.input[self.pos] == b'.'
                            || self.input[self.pos] == b'\'')
                    {
                        self.pos += 1;
                    } else {
                        break;
                    }
                }

                self.column += self.pos - start;
                tokens.push(self.make_token(TokenKind::PPNum, start, start_column));
                self.at_bol = false;
                self.has_space = false;
                continue;
            }

            if b == b'L' && self.pos + 1 < self.input.len() && self.input[self.pos + 1] == b'\'' {
                let start = self.pos;
                let location = self.location();
                let (value, end) = Self::read_char_literal(self.input, start, start + 1, location)?;
                self.pos = end;
                self.column += self.pos - start;
                tokens.push(Token {
                    kind: TokenKind::Num {
                        value,
                        fval: 0.0,
                        ty: Type::Int,
                        ldouble: None,
                    },
                    location,
                    at_bol: self.at_bol,
                    has_space: self.has_space,
                    len: end - start,
                    ..Default::default()
                });
                self.at_bol = false;
                self.has_space = false;
                continue;
            }

            if b == b'u' && self.pos + 1 < self.input.len() && self.input[self.pos + 1] == b'\'' {
                let start = self.pos;
                let location = self.location();
                let (value, end) = Self::read_char_literal(self.input, start, start + 1, location)?;
                let value = value & 0xffff;
                self.pos = end;
                self.column += self.pos - start;
                tokens.push(Token {
                    kind: TokenKind::Num {
                        value,
                        fval: 0.0,
                        ty: Type::UShort,
                        ldouble: None,
                    },
                    location,
                    at_bol: self.at_bol,
                    has_space: self.has_space,
                    len: end - start,
                    ..Default::default()
                });
                self.at_bol = false;
                self.has_space = false;
                continue;
            }

            if b == b'U' && self.pos + 1 < self.input.len() && self.input[self.pos + 1] == b'\'' {
                let start = self.pos;
                let location = self.location();
                let (value, end) = Self::read_char_literal(self.input, start, start + 1, location)?;
                self.pos = end;
                self.column += self.pos - start;
                tokens.push(Token {
                    kind: TokenKind::Num {
                        value,
                        fval: 0.0,
                        ty: Type::UInt,
                        ldouble: None,
                    },
                    location,
                    at_bol: self.at_bol,
                    has_space: self.has_space,
                    len: end - start,
                    ..Default::default()
                });
                self.at_bol = false;
                self.has_space = false;
                continue;
            }

            if b == b'"' {
                let start = self.pos;
                let location = self.location();
                let mut token = Self::read_string_literal(self.input, start, start, location)?;
                token.at_bol = self.at_bol;
                token.has_space = self.has_space;
                let len = token.len;
                tokens.push(token);
                self.at_bol = false;
                self.has_space = false;
                self.pos += len;
                self.column += len;
                continue;
            }

            if b == b'u' && self.pos + 1 < self.input.len() && self.input[self.pos + 1] == b'"' {
                let start = self.pos;
                let location = self.location();
                let mut token =
                    Self::read_utf16_string_literal(self.input, start, start + 1, location)?;
                token.at_bol = self.at_bol;
                token.has_space = self.has_space;
                let len = token.len;
                tokens.push(token);
                self.at_bol = false;
                self.has_space = false;
                self.pos += len;
                self.column += len;
                continue;
            }

            if b == b'U' && self.pos + 1 < self.input.len() && self.input[self.pos + 1] == b'"' {
                let start = self.pos;
                let location = self.location();
                let mut token = Self::read_utf32_string_literal(
                    self.input,
                    start,
                    start + 1,
                    location,
                    Type::UInt,
                )?;
                token.at_bol = self.at_bol;
                token.has_space = self.has_space;
                let len = token.len;
                tokens.push(token);
                self.at_bol = false;
                self.has_space = false;
                self.pos += len;
                self.column += len;
                continue;
            }

            if b == b'u'
                && self.pos + 2 < self.input.len()
                && self.input[self.pos + 1] == b'8'
                && self.input[self.pos + 2] == b'"'
            {
                let start = self.pos;
                let location = self.location();
                let mut token = Self::read_string_literal(self.input, start, start + 2, location)?;
                token.at_bol = self.at_bol;
                token.has_space = self.has_space;
                let len = token.len;
                tokens.push(token);
                self.at_bol = false;
                self.has_space = false;
                self.pos += len;
                self.column += len;
                continue;
            }

            if b == b'L' && self.pos + 1 < self.input.len() && self.input[self.pos + 1] == b'"' {
                let start = self.pos;
                let location = self.location();
                let mut token = Self::read_utf32_string_literal(
                    self.input,
                    start,
                    start + 1,
                    location,
                    Type::Int,
                )?;
                token.at_bol = self.at_bol;
                token.has_space = self.has_space;
                let len = token.len;
                tokens.push(token);
                self.at_bol = false;
                self.has_space = false;
                self.pos += len;
                self.column += len;
                continue;
            }

            {
                let location = self.location();
                let ident_len = read_ident(self.input, self.pos, location)?;
                if ident_len > 0 {
                    let word = &self.input_str[self.pos..self.pos + ident_len];
                    let kind = TokenKind::Ident(word.to_string());
                    tokens.push(Token {
                        kind,
                        location,
                        at_bol: self.at_bol,
                        has_space: self.has_space,
                        len: ident_len,
                        ..Default::default()
                    });
                    self.at_bol = false;
                    self.has_space = false;
                    self.pos += ident_len;
                    self.column += ident_len;
                    continue;
                }
            }

            if b == b'\'' {
                let start = self.pos;
                let location = self.location();
                let (value, end) = Self::read_char_literal(self.input, start, start, location)?;
                let value = value as i8 as i64;
                self.pos = end;
                self.column += self.pos - start;
                tokens.push(Token {
                    kind: TokenKind::Num {
                        value,
                        fval: 0.0,
                        ty: Type::Int,
                        ldouble: None,
                    },
                    location,
                    at_bol: self.at_bol,
                    has_space: self.has_space,
                    len: end - start,
                    ..Default::default()
                });
                self.at_bol = false;
                self.has_space = false;
                continue;
            }

            if let Some((punct, len)) = read_punct(&self.input_str[self.pos..]) {
                let start_column = self.column;
                let start = self.pos;
                self.pos += len;
                self.column += len;
                tokens.push(Token {
                    kind: TokenKind::Punct(punct),
                    location: SourceLocation {
                        line: self.line,
                        column: start_column,
                        byte: start,
                        file_no: self.file_no,
                    },
                    at_bol: self.at_bol,
                    has_space: self.has_space,
                    len,
                    ..Default::default()
                });
                self.at_bol = false;
                self.has_space = false;
                continue;
            }

            return Err(CompileError::at(
                format!("invalid token: {}", b as char),
                self.location(),
            ));
        }

        tokens.push(self.eof_token());
        Ok(tokens)
    }

    // Literal parsing functions

    fn read_escaped_char(
        input: &[u8],
        pos: &mut usize,
        location: SourceLocation,
    ) -> CompileResult<u32> {
        if *pos >= input.len() {
            return Ok(0);
        }

        let b = input[*pos];
        if b.is_ascii_digit() && b <= b'7' {
            let mut value = (b - b'0') as u32;
            *pos += 1;
            if *pos < input.len() && input[*pos].is_ascii_digit() && input[*pos] <= b'7' {
                value = (value << 3) + (input[*pos] - b'0') as u32;
                *pos += 1;
                if *pos < input.len() && input[*pos].is_ascii_digit() && input[*pos] <= b'7' {
                    value = (value << 3) + (input[*pos] - b'0') as u32;
                    *pos += 1;
                }
            }
            return Ok(value);
        }

        if b == b'x' {
            *pos += 1;
            if *pos >= input.len() || !Self::is_hex_digit(input[*pos]) {
                return Err(CompileError::at("invalid hex escape sequence", location));
            }
            let mut value: u32 = 0;
            while *pos < input.len() && Self::is_hex_digit(input[*pos]) {
                value = (value << 4) + Self::from_hex(input[*pos]);
                *pos += 1;
            }
            return Ok(value);
        }

        *pos += 1;
        Ok(match b {
            b'a' => b'\x07',
            b'b' => b'\x08',
            b't' => b'\t',
            b'n' => b'\n',
            b'v' => b'\x0b',
            b'f' => b'\x0c',
            b'r' => b'\r',
            b'e' => 27,
            _ => b,
        } as u32)
    }

    fn read_char_literal(
        input: &[u8],
        _start: usize,
        quote: usize,
        location: SourceLocation,
    ) -> CompileResult<(i64, usize)> {
        let mut i = quote + 1;
        if i >= input.len() {
            return Err(CompileError::at("unclosed char literal", location));
        }

        let value = if input[i] == b'\\' {
            let mut escaped_pos = i + 1;
            let escaped = Self::read_escaped_char(input, &mut escaped_pos, location)?;
            i = escaped_pos;
            escaped as i64
        } else {
            let mut utf8_pos = i;
            let decoded = Self::decode_utf8(input, &mut utf8_pos, location)?;
            i = utf8_pos;
            decoded as i64
        };

        if i >= input.len() || input[i] != b'\'' {
            return Err(CompileError::at("unclosed char literal", location));
        }
        i += 1;

        Ok((value, i))
    }

    fn string_literal_end(
        input: &[u8],
        mut pos: usize,
        start_location: SourceLocation,
    ) -> CompileResult<usize> {
        while pos < input.len() {
            match input[pos] {
                b'"' => return Ok(pos),
                b'\n' | b'\0' => {
                    return Err(CompileError::at("unclosed string literal", start_location));
                }
                b'\\' => {
                    pos += 1;
                }
                _ => {}
            }
            pos += 1;
        }
        Err(CompileError::at("unclosed string literal", start_location))
    }

    fn read_string_literal(
        input: &[u8],
        start: usize,
        quote: usize,
        location: SourceLocation,
    ) -> CompileResult<Token> {
        let end = Self::string_literal_end(input, quote + 1, location)?;
        let mut str_bytes = Vec::with_capacity(end - (quote + 1) + 1);
        let mut p = quote + 1;
        while p < end {
            if input[p] == b'\\' {
                let mut escaped_pos = p + 1;
                let escaped = Self::read_escaped_char(input, &mut escaped_pos, location)?;
                str_bytes.push(escaped as u8);
                p = escaped_pos;
            } else {
                str_bytes.push(input[p]);
                p += 1;
            }
        }
        str_bytes.push(0);
        let ty = Type::Array {
            base: Box::new(Type::Char),
            len: str_bytes.len() as i32,
        };
        Ok(Token {
            kind: TokenKind::Str {
                bytes: str_bytes,
                ty,
            },
            location,
            at_bol: false,
            has_space: false,
            len: end + 1 - start,
            hideset: HideSet::default(),
            origin: None,
            line_delta: 0,
        })
    }

    fn encode_utf16(buf: &mut Vec<u8>, c: u32) {
        if c < 0x10000 {
            buf.extend_from_slice(&(c as u16).to_le_bytes());
        } else {
            let c = c - 0x10000;
            let high = 0xd800 + ((c >> 10) & 0x3ff);
            let low = 0xdc00 + (c & 0x3ff);
            buf.extend_from_slice(&(high as u16).to_le_bytes());
            buf.extend_from_slice(&(low as u16).to_le_bytes());
        }
    }

    fn read_utf16_string_literal(
        input: &[u8],
        start: usize,
        quote: usize,
        location: SourceLocation,
    ) -> CompileResult<Token> {
        let end = Self::string_literal_end(input, quote + 1, location)?;
        let mut buf = Vec::with_capacity((end - quote + 1) * 2);
        let mut p = quote + 1;
        while p < end {
            if input[p] == b'\\' {
                let mut escaped_pos = p + 1;
                let escaped = Self::read_escaped_char(input, &mut escaped_pos, location)?;
                p = escaped_pos;
                Self::encode_utf16(&mut buf, escaped);
                continue;
            }

            let mut utf8_pos = p;
            let decoded = Self::decode_utf8(input, &mut utf8_pos, location)?;
            p = utf8_pos;
            Self::encode_utf16(&mut buf, decoded);
        }
        buf.extend_from_slice(&0u16.to_le_bytes());
        let ty = Type::Array {
            base: Box::new(Type::UShort),
            len: (buf.len() / 2) as i32,
        };
        Ok(Token {
            kind: TokenKind::Str { bytes: buf, ty },
            location,
            at_bol: false,
            has_space: false,
            len: end + 1 - start,
            hideset: HideSet::default(),
            origin: None,
            line_delta: 0,
        })
    }

    fn read_utf32_string_literal(
        input: &[u8],
        start: usize,
        quote: usize,
        location: SourceLocation,
        base: Type,
    ) -> CompileResult<Token> {
        let end = Self::string_literal_end(input, quote + 1, location)?;
        let mut buf = Vec::with_capacity((end - quote + 1) * 4);
        let mut p = quote + 1;
        while p < end {
            if input[p] == b'\\' {
                let mut escaped_pos = p + 1;
                let escaped = Self::read_escaped_char(input, &mut escaped_pos, location)?;
                p = escaped_pos;
                buf.extend_from_slice(&escaped.to_le_bytes());
            } else {
                let mut utf8_pos = p;
                let decoded = Self::decode_utf8(input, &mut utf8_pos, location)?;
                p = utf8_pos;
                buf.extend_from_slice(&decoded.to_le_bytes());
            }
        }
        buf.extend_from_slice(&0u32.to_le_bytes());
        let ty = Type::Array {
            base: Box::new(base),
            len: (buf.len() / 4) as i32,
        };
        Ok(Token {
            kind: TokenKind::Str { bytes: buf, ty },
            location,
            at_bol: false,
            has_space: false,
            len: end + 1 - start,
            hideset: HideSet::default(),
            origin: None,
            line_delta: 0,
        })
    }

    // Identifier/punct parsing functions

    fn in_range(ranges: &[u32], c: u32) -> bool {
        let mut i = 0;
        while i + 1 < ranges.len() {
            if c >= ranges[i] && c <= ranges[i + 1] {
                return true;
            }
            i += 2;
        }
        false
    }

    fn char_width(c: u32) -> usize {
        const RANGE1: &[u32] = &[
            0x0000, 0x001F, 0x007F, 0x00A0, 0x0300, 0x036F, 0x0483, 0x0486, 0x0488, 0x0489, 0x0591,
            0x05BD, 0x05BF, 0x05BF, 0x05C1, 0x05C2, 0x05C4, 0x05C5, 0x05C7, 0x05C7, 0x0600, 0x0603,
            0x0610, 0x0615, 0x064B, 0x065E, 0x0670, 0x0670, 0x06D6, 0x06E4, 0x06E7, 0x06E8, 0x06EA,
            0x06ED, 0x070F, 0x070F, 0x0711, 0x0711, 0x0730, 0x074A, 0x07A6, 0x07B0, 0x07EB, 0x07F3,
            0x0901, 0x0902, 0x093C, 0x093C, 0x0941, 0x0948, 0x094D, 0x094D, 0x0951, 0x0954, 0x0962,
            0x0963, 0x0981, 0x0981, 0x09BC, 0x09BC, 0x09C1, 0x09C4, 0x09CD, 0x09CD, 0x09E2, 0x09E3,
            0x0A01, 0x0A02, 0x0A3C, 0x0A3C, 0x0A41, 0x0A42, 0x0A47, 0x0A48, 0x0A4B, 0x0A4D, 0x0A70,
            0x0A71, 0x0A81, 0x0A82, 0x0ABC, 0x0ABC, 0x0AC1, 0x0AC5, 0x0AC7, 0x0AC8, 0x0ACD, 0x0ACD,
            0x0AE2, 0x0AE3, 0x0B01, 0x0B01, 0x0B3C, 0x0B3C, 0x0B3F, 0x0B3F, 0x0B41, 0x0B43, 0x0B4D,
            0x0B4D, 0x0B56, 0x0B56, 0x0B82, 0x0B82, 0x0BC0, 0x0BC0, 0x0BCD, 0x0BCD, 0x0C3E, 0x0C40,
            0x0C46, 0x0C48, 0x0C4A, 0x0C4D, 0x0C55, 0x0C56, 0x0CBC, 0x0CBC, 0x0CBF, 0x0CBF, 0x0CC6,
            0x0CC6, 0x0CCC, 0x0CCD, 0x0CE2, 0x0CE3, 0x0D41, 0x0D43, 0x0D4D, 0x0D4D, 0x0DCA, 0x0DCA,
            0x0DD2, 0x0DD4, 0x0DD6, 0x0DD6, 0x0E31, 0x0E31, 0x0E34, 0x0E3A, 0x0E47, 0x0E4E, 0x0EB1,
            0x0EB1, 0x0EB4, 0x0EB9, 0x0EBB, 0x0EBC, 0x0EC8, 0x0ECD, 0x0F18, 0x0F19, 0x0F35, 0x0F35,
            0x0F37, 0x0F37, 0x0F39, 0x0F39, 0x0F71, 0x0F7E, 0x0F80, 0x0F84, 0x0F86, 0x0F87, 0x0F90,
            0x0F97, 0x0F99, 0x0FBC, 0x0FC6, 0x0FC6, 0x102D, 0x1030, 0x1032, 0x1032, 0x1036, 0x1037,
            0x1039, 0x1039, 0x1058, 0x1059, 0x1160, 0x11FF, 0x135F, 0x135F, 0x1712, 0x1714, 0x1732,
            0x1734, 0x1752, 0x1753, 0x1772, 0x1773, 0x17B4, 0x17B5, 0x17B7, 0x17BD, 0x17C6, 0x17C6,
            0x17C9, 0x17D3, 0x17DD, 0x17DD, 0x180B, 0x180D, 0x18A9, 0x18A9, 0x1920, 0x1922, 0x1927,
            0x1928, 0x1932, 0x1932, 0x1939, 0x193B, 0x1A17, 0x1A18, 0x1B00, 0x1B03, 0x1B34, 0x1B34,
            0x1B36, 0x1B3A, 0x1B3C, 0x1B3C, 0x1B42, 0x1B42, 0x1B6B, 0x1B73, 0x1DC0, 0x1DCA, 0x1DFE,
            0x1DFF, 0x200B, 0x200F, 0x202A, 0x202E, 0x2060, 0x2063, 0x206A, 0x206F, 0x20D0, 0x20EF,
            0x302A, 0x302F, 0x3099, 0x309A, 0xA806, 0xA806, 0xA80B, 0xA80B, 0xA825, 0xA826, 0xFB1E,
            0xFB1E, 0xFE00, 0xFE0F, 0xFE20, 0xFE23, 0xFEFF, 0xFEFF, 0xFFF9, 0xFFFB, 0x10A01,
            0x10A03, 0x10A05, 0x10A06, 0x10A0C, 0x10A0F, 0x10A38, 0x10A3A, 0x10A3F, 0x10A3F,
            0x1D167, 0x1D169, 0x1D173, 0x1D182, 0x1D185, 0x1D18B, 0x1D1AA, 0x1D1AD, 0x1D242,
            0x1D244, 0xE0001, 0xE0001, 0xE0020, 0xE007F, 0xE0100, 0xE01EF,
        ];

        if Self::in_range(RANGE1, c) {
            return 0;
        }

        const RANGE2: &[u32] = &[
            0x1100, 0x115F, 0x2329, 0x2329, 0x232A, 0x232A, 0x2E80, 0x303E, 0x3040, 0xA4CF, 0xAC00,
            0xD7A3, 0xF900, 0xFAFF, 0xFE10, 0xFE19, 0xFE30, 0xFE6F, 0xFF00, 0xFF60, 0xFFE0, 0xFFE6,
            0x1F000, 0x1F644, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD,
        ];

        if Self::in_range(RANGE2, c) {
            return 2;
        }

        1
    }
}

pub fn tokenize(input: &str, file_no: usize) -> CompileResult<Vec<Token>> {
    Lexer::new(input, file_no).tokenize()
}

pub fn display_width(input: &str, len: usize) -> usize {
    let bytes = input.as_bytes();
    let limit = len.min(bytes.len());
    let mut pos = 0;
    let mut width = 0;
    let location = SourceLocation {
        line: 0,
        column: 0,
        byte: 0,
        file_no: 0,
    };

    while pos < limit {
        let start = pos;
        match Lexer::decode_utf8(bytes, &mut pos, location) {
            Ok(c) => width += Lexer::char_width(c),
            Err(_) => {
                pos = start + 1;
                width += 1;
            }
        }
    }

    width
}

fn is_ident1(c: u32) -> bool {
    const RANGES: &[u32] = &[
        b'_' as u32,
        b'_' as u32,
        b'a' as u32,
        b'z' as u32,
        b'A' as u32,
        b'Z' as u32,
        b'$' as u32,
        b'$' as u32,
        0x00A8,
        0x00A8,
        0x00AA,
        0x00AA,
        0x00AD,
        0x00AD,
        0x00AF,
        0x00AF,
        0x00B2,
        0x00B5,
        0x00B7,
        0x00BA,
        0x00BC,
        0x00BE,
        0x00C0,
        0x00D6,
        0x00D8,
        0x00F6,
        0x00F8,
        0x00FF,
        0x0100,
        0x02FF,
        0x0370,
        0x167F,
        0x1681,
        0x180D,
        0x180F,
        0x1DBF,
        0x1E00,
        0x1FFF,
        0x200B,
        0x200D,
        0x202A,
        0x202E,
        0x203F,
        0x2040,
        0x2054,
        0x2054,
        0x2060,
        0x206F,
        0x2070,
        0x20CF,
        0x2100,
        0x218F,
        0x2460,
        0x24FF,
        0x2776,
        0x2793,
        0x2C00,
        0x2DFF,
        0x2E80,
        0x2FFF,
        0x3004,
        0x3007,
        0x3021,
        0x302F,
        0x3031,
        0x303F,
        0x3040,
        0xD7FF,
        0xF900,
        0xFD3D,
        0xFD40,
        0xFDCF,
        0xFDF0,
        0xFE1F,
        0xFE30,
        0xFE44,
        0xFE47,
        0xFFFD,
        0x10000,
        0x1FFFD,
        0x20000,
        0x2FFFD,
        0x30000,
        0x3FFFD,
        0x40000,
        0x4FFFD,
        0x50000,
        0x5FFFD,
        0x60000,
        0x6FFFD,
        0x70000,
        0x7FFFD,
        0x80000,
        0x8FFFD,
        0x90000,
        0x9FFFD,
        0xA0000,
        0xAFFFD,
        0xB0000,
        0xBFFFD,
        0xC0000,
        0xCFFFD,
        0xD0000,
        0xDFFFD,
        0xE0000,
        0xEFFFD,
    ];
    Lexer::in_range(RANGES, c)
}

fn is_ident2(c: u32) -> bool {
    const RANGES: &[u32] = &[
        b'0' as u32,
        b'9' as u32,
        b'$' as u32,
        b'$' as u32,
        0x0300,
        0x036F,
        0x1DC0,
        0x1DFF,
        0x20D0,
        0x20FF,
        0xFE20,
        0xFE2F,
    ];
    is_ident1(c) || Lexer::in_range(RANGES, c)
}

fn read_ident(input: &[u8], pos: usize, location: SourceLocation) -> CompileResult<usize> {
    let mut p = pos;
    let mut decoded_pos = p;
    let c = Lexer::decode_utf8(input, &mut decoded_pos, location)?;
    if !is_ident1(c) {
        return Ok(0);
    }
    p = decoded_pos;

    loop {
        let mut next_pos = p;
        let c = Lexer::decode_utf8(input, &mut next_pos, location)?;
        if !is_ident2(c) {
            return Ok(p - pos);
        }
        p = next_pos;
    }
}

fn read_punct(input: &str) -> Option<(Punct, usize)> {
    if input.starts_with("...") {
        return Some((Punct::Ellipsis, 3));
    }
    if input.starts_with("<<=") {
        return Some((Punct::ShlAssign, 3));
    }
    if input.starts_with(">>=") {
        return Some((Punct::ShrAssign, 3));
    }
    if input.starts_with("##") {
        return Some((Punct::HashHash, 2));
    }
    if input.starts_with("==") {
        return Some((Punct::EqEq, 2));
    }
    if input.starts_with("!=") {
        return Some((Punct::NotEq, 2));
    }
    if input.starts_with("<=") {
        return Some((Punct::LessEq, 2));
    }
    if input.starts_with(">=") {
        return Some((Punct::GreaterEq, 2));
    }
    if input.starts_with("->") {
        return Some((Punct::Arrow, 2));
    }
    if input.starts_with("<<") {
        return Some((Punct::Shl, 2));
    }
    if input.starts_with(">>") {
        return Some((Punct::Shr, 2));
    }
    if input.starts_with("+=") {
        return Some((Punct::AddAssign, 2));
    }
    if input.starts_with("-=") {
        return Some((Punct::SubAssign, 2));
    }
    if input.starts_with("*=") {
        return Some((Punct::MulAssign, 2));
    }
    if input.starts_with("/=") {
        return Some((Punct::DivAssign, 2));
    }
    if input.starts_with("%=") {
        return Some((Punct::ModAssign, 2));
    }
    if input.starts_with("&&") {
        return Some((Punct::LogAnd, 2));
    }
    if input.starts_with("&=") {
        return Some((Punct::AndAssign, 2));
    }
    if input.starts_with("||") {
        return Some((Punct::LogOr, 2));
    }
    if input.starts_with("|=") {
        return Some((Punct::OrAssign, 2));
    }
    if input.starts_with("^=") {
        return Some((Punct::XorAssign, 2));
    }
    if input.starts_with("++") {
        return Some((Punct::Inc, 2));
    }
    if input.starts_with("--") {
        return Some((Punct::Dec, 2));
    }
    if input.starts_with("=") {
        return Some((Punct::Assign, 1));
    }

    let ch = input.as_bytes().first().copied()?;
    let punct = match ch {
        b'+' => Punct::Plus,
        b'-' => Punct::Minus,
        b'*' => Punct::Star,
        b'&' => Punct::Amp,
        b'#' => Punct::Hash,
        b'|' => Punct::Pipe,
        b'^' => Punct::Caret,
        b'/' => Punct::Slash,
        b'%' => Punct::Mod,
        b'`' => Punct::Backtick,
        b'.' => Punct::Dot,
        b'!' => Punct::Not,
        b'~' => Punct::BitNot,
        b'(' => Punct::LParen,
        b')' => Punct::RParen,
        b'{' => Punct::LBrace,
        b'}' => Punct::RBrace,
        b'[' => Punct::LBracket,
        b']' => Punct::RBracket,
        b';' => Punct::Semicolon,
        b',' => Punct::Comma,
        b':' => Punct::Colon,
        b'?' => Punct::Question,
        b'<' => Punct::Less,
        b'>' => Punct::Greater,
        _ => return None,
    };
    Some((punct, 1))
}

fn keyword_map() -> &'static HashMap<&'static str, ()> {
    static KEYWORDS: OnceLock<HashMap<&'static str, ()>> = OnceLock::new();
    KEYWORDS.get_or_init(|| {
        let keywords = [
            "void",
            "_Bool",
            "bool",
            "true",
            "false",
            "nullptr",
            "char",
            "short",
            "int",
            "long",
            "float",
            "double",
            "enum",
            "struct",
            "union",
            "typedef",
            "static",
            "extern",
            "return",
            "if",
            "else",
            "for",
            "while",
            "do",
            "sizeof",
            "_Alignof",
            "_Alignas",
            "goto",
            "break",
            "continue",
            "switch",
            "case",
            "default",
            "signed",
            "unsigned",
            "const",
            "volatile",
            "auto",
            "register",
            "restrict",
            "__restrict",
            "__restrict__",
            "_Noreturn",
            "_Generic",
            "typeof",
            "typeof_unqual",
            "constexpr",
            "inline",
            "asm",
            "_Thread_local",
            "__thread",
            "_Atomic",
            "_Complex",
            "__complex__",
            "__real__",
            "__imag__",
            "__attribute__",
            "_Static_assert",
            "_BitInt",
        ];
        let mut map = HashMap::with_capacity(keywords.len());
        for keyword in keywords {
            map.insert(keyword, ());
        }
        map
    })
}

fn is_keyword(name: &str) -> bool {
    keyword_map().contains_key(name)
}

fn convert_keyword(tok: &mut Token, name: &str) {
    let keyword = match name {
        "void" => Keyword::Void,
        "_Bool" | "bool" => Keyword::Bool,
        "true" => Keyword::True,
        "false" => Keyword::False,
        "nullptr" => Keyword::NullPtr,
        "char" => Keyword::Char,
        "short" => Keyword::Short,
        "int" => Keyword::Int,
        "long" => Keyword::Long,
        "float" => Keyword::Float,
        "double" => Keyword::Double,
        "enum" => Keyword::Enum,
        "struct" => Keyword::Struct,
        "union" => Keyword::Union,
        "typedef" => Keyword::Typedef,
        "static" => Keyword::Static,
        "extern" => Keyword::Extern,
        "return" => Keyword::Return,
        "if" => Keyword::If,
        "else" => Keyword::Else,
        "for" => Keyword::For,
        "while" => Keyword::While,
        "do" => Keyword::Do,
        "sizeof" => Keyword::Sizeof,
        "_Alignof" => Keyword::Alignof,
        "_Alignas" => Keyword::Alignas,
        "goto" => Keyword::Goto,
        "break" => Keyword::Break,
        "continue" => Keyword::Continue,
        "switch" => Keyword::Switch,
        "case" => Keyword::Case,
        "default" => Keyword::Default,
        "signed" => Keyword::Signed,
        "unsigned" => Keyword::Unsigned,
        "const" => Keyword::Const,
        "volatile" => Keyword::Volatile,
        "auto" => Keyword::Auto,
        "register" => Keyword::Register,
        "restrict" | "__restrict" | "__restrict__" => Keyword::Restrict,
        "_Noreturn" => Keyword::Noreturn,
        "_Generic" => Keyword::Generic,
        "typeof" => Keyword::Typeof,
        "typeof_unqual" => Keyword::TypeofUnqual,
        "constexpr" => Keyword::Constexpr,
        "inline" => Keyword::Inline,
        "asm" => Keyword::Asm,
        "_Thread_local" | "__thread" => Keyword::ThreadLocal,
        "_Atomic" => Keyword::Atomic,
        "_Complex" | "__complex__" => Keyword::Complex,
        "__real__" => Keyword::Real,
        "__imag__" => Keyword::Imag,
        "__attribute__" => Keyword::Attribute,
        "_Static_assert" => Keyword::StaticAssert,
        "_BitInt" => Keyword::BitInt,
        _ => return,
    };
    tok.kind = TokenKind::Keyword(keyword);
}

/// Convert pp-tokens to proper tokens.
/// This converts pp-numbers to numbers and identifiers to keywords.
pub fn convert_pp_tokens(tokens: &mut [Token]) -> CompileResult<()> {
    for tok in tokens {
        if let TokenKind::Ident(ref name) = tok.kind.clone() {
            if is_keyword(name) {
                convert_keyword(tok, name);
            }
        } else if matches!(tok.kind, TokenKind::PPNum) {
            tok.convert_pp_number()?;
        }
    }
    Ok(())
}
