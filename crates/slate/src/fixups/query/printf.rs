use crate::fixups::facts::{GENERATED_C_STRING_READ_CALLEE, PrintfArgFact};
use crate::rust_ast::{BinOp, Block, Expr, IndentStmt, Prim, RustValue, Stmt, Type};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum PrintfStream {
    Stdout,
    Stderr,
}

impl PrintfStream {
    fn macro_names(self) -> (&'static str, &'static str) {
        match self {
            PrintfStream::Stdout => ("println", "print"),
            PrintfStream::Stderr => ("eprintln", "eprint"),
        }
    }

    pub(in crate::fixups) fn plain_macro_name(self) -> &'static str {
        self.macro_names().1
    }
}

pub(in crate::fixups) fn printf_macro(
    format: &[u8],
    args: &[Expr],
    arg_facts: &[PrintfArgFact],
    stream: PrintfStream,
) -> Option<Expr> {
    let parsed = parse_printf_format(format)?;
    if parsed.conversions.len() != args.len() {
        return None;
    }
    let (line_macro, plain_macro) = stream.macro_names();
    let mut macro_args = Vec::new();
    let name = if parsed.trailing_newline {
        if parsed.format.is_empty() && args.is_empty() {
            return Some(format_macro(line_macro, vec![]));
        }
        line_macro
    } else {
        plain_macro
    };
    macro_args.push(Expr::Str(parsed.format));
    for ((arg, conversion), arg_fact) in args
        .iter()
        .zip(parsed.conversions.iter())
        .zip(arg_facts.iter())
    {
        macro_args.push(printf_macro_arg(arg, conversion.kind, arg_fact)?);
    }
    Some(format_macro(name, macro_args))
}

pub(in crate::fixups) fn sprintf_format_expr(
    format: &[u8],
    args: &[Expr],
    arg_facts: &[PrintfArgFact],
) -> Option<Expr> {
    let parsed = parse_printf_format(format)?;
    if parsed.conversions.len() != args.len() {
        return None;
    }
    let mut text = parsed.format;
    if parsed.trailing_newline {
        text.push('\n');
    }
    let mut macro_args = vec![Expr::Str(text)];
    for ((arg, conversion), arg_fact) in args
        .iter()
        .zip(parsed.conversions.iter())
        .zip(arg_facts.iter())
    {
        macro_args.push(printf_macro_arg(arg, conversion.kind, arg_fact)?);
    }
    Some(format_macro("format", macro_args))
}

const INTEGER_MAX_DIGITS: usize = 24;
const POINTER_MAX_DIGITS: usize = 20;

pub(in crate::fixups) fn sprintf_worst_case_len(
    format: &[u8],
    arg_facts: &[PrintfArgFact],
) -> Option<usize> {
    let parsed = parse_printf_format(format)?;
    if parsed.conversions.len() != arg_facts.len() {
        return None;
    }
    let mut total = parsed.format.len();
    if parsed.trailing_newline {
        total += 1;
    }
    for (conversion, arg_fact) in parsed.conversions.iter().zip(arg_facts.iter()) {
        total += conversion_max_len(conversion.kind, arg_fact)?;
    }
    Some(total)
}

fn conversion_max_len(kind: ConversionKind, arg_fact: &PrintfArgFact) -> Option<usize> {
    match kind {
        ConversionKind::Integer(int_arg) => Some(integer_max_len(int_arg)),
        ConversionKind::Char(CharArg::Value) => Some(1),
        ConversionKind::Char(CharArg::Sized(fmt)) => Some(fmt.width.max(1)),
        ConversionKind::String(StringArg::Value) => {
            arg_fact.const_string.as_ref().map(|value| value.len())
        }
        ConversionKind::String(StringArg::Sized(fmt)) => match fmt.precision {
            Some(precision) => Some(precision.max(fmt.width.unwrap_or(0))),
            None => arg_fact
                .const_string
                .as_ref()
                .map(|value| value.len().max(fmt.width.unwrap_or(0))),
        },
        ConversionKind::Pointer => Some(POINTER_MAX_DIGITS),
        ConversionKind::Float | ConversionKind::Exponent(_) | ConversionKind::General(_) => None,
    }
}

fn integer_max_len(arg: IntegerArg) -> usize {
    let requested = match arg {
        IntegerArg::Value | IntegerArg::Narrow { .. } => 0,
        IntegerArg::Alternate(fmt) => fmt.width.unwrap_or(0),
        IntegerArg::Precision(fmt) => fmt.width.unwrap_or(0).max(fmt.precision),
    };
    INTEGER_MAX_DIGITS.max(requested)
}

struct ParsedFormat {
    format: String,
    conversions: Vec<Conversion>,
    trailing_newline: bool,
}

struct Conversion {
    kind: ConversionKind,
}

#[derive(Clone, Copy)]
enum ConversionKind {
    Integer(IntegerArg),
    String(StringArg),
    Char(CharArg),
    Float,
    Exponent(ExponentFormat),
    General(GeneralFormat),
    Pointer,
}

#[derive(Clone, Copy)]
struct ExponentFormat {
    left: bool,
    plus: bool,
    upper: bool,
    width: Option<usize>,
    precision: usize,
}

#[derive(Clone, Copy)]
struct GeneralFormat {
    left: bool,
    plus: bool,
    alternate: bool,
    zero: bool,
    upper: bool,
    width: Option<usize>,
    precision: usize,
}

#[derive(Clone, Copy)]
enum CharArg {
    Value,
    Sized(CharSizeFormat),
}

#[derive(Clone, Copy)]
struct CharSizeFormat {
    left: bool,
    width: usize,
}

#[derive(Clone, Copy)]
enum StringArg {
    Value,
    Sized(StringSizeFormat),
}

#[derive(Clone, Copy)]
struct StringSizeFormat {
    left: bool,
    width: Option<usize>,
    precision: Option<usize>,
}

#[derive(Clone, Copy)]
enum IntegerArg {
    Value,
    Alternate(AlternateIntegerFormat),
    Precision(PrecisionIntegerFormat),
    Narrow { signed: bool, width: NarrowWidth },
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum NarrowWidth {
    Byte,
    Half,
}

#[derive(Clone, Copy)]
struct AlternateIntegerFormat {
    left: bool,
    zero: bool,
    width: Option<usize>,
    radix: char,
    narrow: Option<NarrowWidth>,
}

#[derive(Clone, Copy)]
struct PrecisionIntegerFormat {
    left: bool,
    plus: bool,
    width: Option<usize>,
    precision: usize,
    kind: PrecisionKind,
    narrow: Option<NarrowWidth>,
}

#[derive(Clone, Copy)]
enum PrecisionKind {
    SignedDecimal,
    UnsignedDecimal,
    Hex,
    HexUpper,
    Octal,
}

fn parse_printf_format(bytes: &[u8]) -> Option<ParsedFormat> {
    let mut format = String::new();
    let mut conversions = Vec::new();
    let mut i = 0;
    while i < bytes.len() {
        match bytes[i] {
            b'%' => match bytes.get(i + 1).copied() {
                Some(b'%') => {
                    format.push('%');
                    i += 2;
                }
                Some(_) => {
                    let (next, conversion, placeholder) = parse_conversion(bytes, i + 1)?;
                    format.push_str(&placeholder);
                    conversions.push(conversion);
                    i = next;
                }
                None => return None,
            },
            b'{' => {
                format.push_str("{{");
                i += 1;
            }
            b'}' => {
                format.push_str("}}");
                i += 1;
            }
            b'\n' => {
                format.push('\n');
                i += 1;
            }
            0x20..=0x7e => {
                format.push(bytes[i] as char);
                i += 1;
            }
            _ => return None,
        }
    }
    let trailing_newline = format.ends_with('\n');
    if trailing_newline {
        format.pop();
    }
    Some(ParsedFormat {
        format,
        conversions,
        trailing_newline,
    })
}

fn parse_conversion(bytes: &[u8], i: usize) -> Option<(usize, Conversion, String)> {
    parse_integer_conversion(bytes, i)
        .or_else(|| parse_string_char_conversion(bytes, i))
        .or_else(|| parse_float_conversion(bytes, i))
        .or_else(|| parse_exponent_conversion(bytes, i))
        .or_else(|| parse_general_conversion(bytes, i))
        .or_else(|| parse_pointer_conversion(bytes, i))
}

fn parse_integer_conversion(bytes: &[u8], mut i: usize) -> Option<(usize, Conversion, String)> {
    let mut left = false;
    let mut plus = false;
    let mut alternate = false;
    let mut zero = false;
    loop {
        match bytes.get(i).copied()? {
            b'-' if !left => {
                left = true;
                i += 1;
            }
            b'+' if !plus => {
                plus = true;
                i += 1;
            }
            b'#' if !alternate => {
                alternate = true;
                i += 1;
            }
            b'0' if !zero => {
                zero = true;
                i += 1;
            }
            b'-' | b'+' | b'0' | b' ' | b'#' => return None,
            _ => break,
        }
    }

    let width_start = i;
    while bytes.get(i).is_some_and(u8::is_ascii_digit) {
        i += 1;
    }
    let width = if i > width_start {
        Some(std::str::from_utf8(&bytes[width_start..i]).ok()?)
    } else {
        None
    };
    if (left || zero) && width.is_none() {
        return None;
    }
    if left && zero {
        return None;
    }

    let precision = if bytes.get(i).copied() == Some(b'.') {
        i += 1;
        if bytes.get(i).copied() == Some(b'*') {
            return None;
        }
        let precision_start = i;
        while bytes.get(i).is_some_and(u8::is_ascii_digit) {
            i += 1;
        }
        let text = std::str::from_utf8(&bytes[precision_start..i]).ok()?;
        let value: usize = if text.is_empty() {
            0
        } else {
            text.parse().ok()?
        };
        if value == 0 {
            return None;
        }
        Some(value)
    } else {
        None
    };
    if alternate && precision.is_some() {
        return None;
    }

    let narrow_width = match bytes.get(i).copied()? {
        b'l' => {
            i += 1;
            if bytes.get(i).copied() == Some(b'l') {
                i += 1;
            }
            None
        }
        b'z' | b'j' | b't' => {
            i += 1;
            None
        }
        b'h' => {
            i += 1;
            if bytes.get(i).copied() == Some(b'h') {
                i += 1;
                Some(NarrowWidth::Byte)
            } else {
                Some(NarrowWidth::Half)
            }
        }
        _ => None,
    };
    let conv = bytes.get(i).copied()?;
    if !matches!(conv, b'd' | b'i' | b'u' | b'x' | b'X' | b'o') {
        return None;
    }
    if plus && !matches!(conv, b'd' | b'i') {
        return None;
    }
    if alternate && !matches!(conv, b'x' | b'X' | b'o') {
        return None;
    }
    let format_kind = match conv {
        b'x' => Some('x'),
        b'X' => Some('X'),
        b'o' => Some('o'),
        _ => None,
    };
    let conversion_kind = if let Some(precision) = precision {
        let width = width.map(str::parse).transpose().ok()?;
        let kind = match conv {
            b'd' | b'i' => PrecisionKind::SignedDecimal,
            b'u' => PrecisionKind::UnsignedDecimal,
            b'x' => PrecisionKind::Hex,
            b'X' => PrecisionKind::HexUpper,
            b'o' => PrecisionKind::Octal,
            _ => unreachable!(),
        };
        ConversionKind::Integer(IntegerArg::Precision(PrecisionIntegerFormat {
            left,
            plus,
            width,
            precision,
            kind,
            narrow: narrow_width,
        }))
    } else if alternate {
        let width = width.map(str::parse).transpose().ok()?;
        ConversionKind::Integer(IntegerArg::Alternate(AlternateIntegerFormat {
            left,
            zero,
            width,
            radix: format_kind?,
            narrow: narrow_width,
        }))
    } else if let Some(width) = narrow_width {
        ConversionKind::Integer(IntegerArg::Narrow {
            signed: matches!(conv, b'd' | b'i'),
            width,
        })
    } else {
        ConversionKind::Integer(IntegerArg::Value)
    };
    Some((
        i + 1,
        Conversion {
            kind: conversion_kind,
        },
        if alternate || precision.is_some() {
            "{}".into()
        } else {
            integer_placeholder(left, plus, false, zero, width, format_kind)
        },
    ))
}

fn parse_string_char_conversion(bytes: &[u8], i: usize) -> Option<(usize, Conversion, String)> {
    match bytes.get(i).copied()? {
        b'c' => {
            return Some((
                i + 1,
                Conversion {
                    kind: ConversionKind::Char(CharArg::Value),
                },
                "{}".into(),
            ));
        }
        b's' => {
            return Some((
                i + 1,
                Conversion {
                    kind: ConversionKind::String(StringArg::Value),
                },
                "{}".into(),
            ));
        }
        _ => {}
    }
    parse_sized_char_conversion(bytes, i).or_else(|| parse_sized_string_conversion(bytes, i))
}

fn parse_sized_char_conversion(bytes: &[u8], mut i: usize) -> Option<(usize, Conversion, String)> {
    let left = bytes.get(i).copied() == Some(b'-');
    if left {
        i += 1;
    }
    let width_start = i;
    while bytes.get(i).is_some_and(u8::is_ascii_digit) {
        i += 1;
    }
    if i == width_start {
        return None;
    }
    let width: usize = std::str::from_utf8(&bytes[width_start..i])
        .ok()?
        .parse()
        .ok()?;
    if bytes.get(i).copied()? != b'c' {
        return None;
    }
    Some((
        i + 1,
        Conversion {
            kind: ConversionKind::Char(CharArg::Sized(CharSizeFormat { left, width })),
        },
        "{}".into(),
    ))
}

fn parse_sized_string_conversion(
    bytes: &[u8],
    mut i: usize,
) -> Option<(usize, Conversion, String)> {
    let left = bytes.get(i).copied() == Some(b'-');
    if left {
        i += 1;
    }
    let width_start = i;
    while bytes.get(i).is_some_and(u8::is_ascii_digit) {
        i += 1;
    }
    let width = if i > width_start {
        Some(
            std::str::from_utf8(&bytes[width_start..i])
                .ok()?
                .parse()
                .ok()?,
        )
    } else {
        None
    };
    let precision = if bytes.get(i).copied() == Some(b'.') {
        i += 1;
        if bytes.get(i).copied() == Some(b'*') {
            return None;
        }
        let precision_start = i;
        while bytes.get(i).is_some_and(u8::is_ascii_digit) {
            i += 1;
        }
        let text = std::str::from_utf8(&bytes[precision_start..i]).ok()?;
        Some(if text.is_empty() {
            0
        } else {
            text.parse().ok()?
        })
    } else {
        None
    };
    if width.is_none() && precision.is_none() {
        return None;
    }
    if left && width.is_none() {
        return None;
    }
    if bytes.get(i).copied()? != b's' {
        return None;
    }
    Some((
        i + 1,
        Conversion {
            kind: ConversionKind::String(StringArg::Sized(StringSizeFormat {
                left,
                width,
                precision,
            })),
        },
        "{}".into(),
    ))
}

fn parse_float_conversion(bytes: &[u8], mut i: usize) -> Option<(usize, Conversion, String)> {
    let mut left = false;
    let mut plus = false;
    let mut zero = false;
    loop {
        match bytes.get(i).copied()? {
            b'-' if !left => {
                left = true;
                i += 1;
            }
            b'+' if !plus => {
                plus = true;
                i += 1;
            }
            b'0' if !zero => {
                zero = true;
                i += 1;
            }
            b'-' | b'+' | b'0' | b' ' | b'#' => return None,
            _ => break,
        }
    }
    if left && zero {
        return None;
    }

    let width_start = i;
    while bytes.get(i).is_some_and(u8::is_ascii_digit) {
        i += 1;
    }
    let width = if i > width_start {
        Some(std::str::from_utf8(&bytes[width_start..i]).ok()?)
    } else {
        None
    };
    if (left || zero) && width.is_none() {
        return None;
    }

    let precision = if bytes.get(i).copied() == Some(b'.') {
        i += 1;
        let precision_start = i;
        while bytes.get(i).is_some_and(u8::is_ascii_digit) {
            i += 1;
        }
        if i == precision_start {
            return None;
        }
        std::str::from_utf8(&bytes[precision_start..i]).ok()?
    } else {
        "6"
    };
    if bytes.get(i).copied()? != b'f' {
        return None;
    }
    Some((
        i + 1,
        Conversion {
            kind: ConversionKind::Float,
        },
        float_placeholder(left, plus, zero, width, precision),
    ))
}

fn float_placeholder(
    left: bool,
    plus: bool,
    zero: bool,
    width: Option<&str>,
    precision: &str,
) -> String {
    let mut out = String::from("{:");
    if left {
        out.push('<');
    }
    if plus {
        out.push('+');
    }
    if zero {
        out.push('0');
    }
    if let Some(width) = width {
        out.push_str(width);
    }
    out.push('.');
    out.push_str(precision);
    out.push('}');
    out
}

fn parse_exponent_conversion(bytes: &[u8], mut i: usize) -> Option<(usize, Conversion, String)> {
    let mut left = false;
    let mut plus = false;
    loop {
        match bytes.get(i).copied()? {
            b'-' if !left => {
                left = true;
                i += 1;
            }
            b'+' if !plus => {
                plus = true;
                i += 1;
            }
            b'-' | b'+' | b'0' | b' ' | b'#' => return None,
            _ => break,
        }
    }

    let width_start = i;
    while bytes.get(i).is_some_and(u8::is_ascii_digit) {
        i += 1;
    }
    let width = if i > width_start {
        Some(
            std::str::from_utf8(&bytes[width_start..i])
                .ok()?
                .parse::<usize>()
                .ok()?,
        )
    } else {
        None
    };
    if left && width.is_none() {
        return None;
    }

    let precision = if bytes.get(i).copied() == Some(b'.') {
        i += 1;
        if bytes.get(i).copied() == Some(b'*') {
            return None;
        }
        let precision_start = i;
        while bytes.get(i).is_some_and(u8::is_ascii_digit) {
            i += 1;
        }
        let text = std::str::from_utf8(&bytes[precision_start..i]).ok()?;
        if text.is_empty() {
            0
        } else {
            text.parse().ok()?
        }
    } else {
        6
    };

    let conv = bytes.get(i).copied()?;
    if !matches!(conv, b'e' | b'E') {
        return None;
    }
    Some((
        i + 1,
        Conversion {
            kind: ConversionKind::Exponent(ExponentFormat {
                left,
                plus,
                upper: conv == b'E',
                width,
                precision,
            }),
        },
        "{}".into(),
    ))
}

fn parse_general_conversion(bytes: &[u8], mut i: usize) -> Option<(usize, Conversion, String)> {
    let mut left = false;
    let mut plus = false;
    let mut alternate = false;
    let mut zero = false;
    loop {
        match bytes.get(i).copied()? {
            b'-' if !left => {
                left = true;
                i += 1;
            }
            b'+' if !plus => {
                plus = true;
                i += 1;
            }
            b'#' if !alternate => {
                alternate = true;
                i += 1;
            }
            b'0' if !zero => {
                zero = true;
                i += 1;
            }
            b'-' | b'+' | b'0' | b' ' | b'#' => return None,
            _ => break,
        }
    }
    if left && zero {
        return None;
    }

    let width_start = i;
    while bytes.get(i).is_some_and(u8::is_ascii_digit) {
        i += 1;
    }
    let width = if i > width_start {
        Some(
            std::str::from_utf8(&bytes[width_start..i])
                .ok()?
                .parse::<usize>()
                .ok()?,
        )
    } else {
        None
    };
    if (left || zero) && width.is_none() {
        return None;
    }

    let precision = if bytes.get(i).copied() == Some(b'.') {
        i += 1;
        if bytes.get(i).copied() == Some(b'*') {
            return None;
        }
        let precision_start = i;
        while bytes.get(i).is_some_and(u8::is_ascii_digit) {
            i += 1;
        }
        let text = std::str::from_utf8(&bytes[precision_start..i]).ok()?;
        if text.is_empty() {
            0
        } else {
            text.parse().ok()?
        }
    } else {
        6
    };

    let conv = bytes.get(i).copied()?;
    if !matches!(conv, b'g' | b'G') {
        return None;
    }
    Some((
        i + 1,
        Conversion {
            kind: ConversionKind::General(GeneralFormat {
                left,
                plus,
                alternate,
                zero,
                upper: conv == b'G',
                width,
                precision,
            }),
        },
        "{}".into(),
    ))
}

fn parse_pointer_conversion(bytes: &[u8], i: usize) -> Option<(usize, Conversion, String)> {
    if bytes.get(i).copied()? != b'p' {
        return None;
    }
    Some((
        i + 1,
        Conversion {
            kind: ConversionKind::Pointer,
        },
        "{:p}".into(),
    ))
}

fn integer_placeholder(
    left: bool,
    plus: bool,
    alternate: bool,
    zero: bool,
    width: Option<&str>,
    format_kind: Option<char>,
) -> String {
    if !left && !plus && !alternate && !zero && width.is_none() && format_kind.is_none() {
        return "{}".into();
    }
    let mut out = String::from("{:");
    if left {
        out.push('<');
    }
    if plus {
        out.push('+');
    }
    if alternate {
        out.push('#');
    }
    if zero {
        out.push('0');
    }
    if let Some(width) = width {
        out.push_str(width);
    }
    if let Some(format_kind) = format_kind {
        out.push(format_kind);
    }
    out.push('}');
    out
}

fn format_macro(name: &str, args: Vec<Expr>) -> Expr {
    Expr::Macro {
        name: name.into(),
        args,
    }
}

fn runtime_c_string_arg(arg: &Expr) -> Expr {
    Expr::MethodCall {
        recv: Box::new(Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var(GENERATED_C_STRING_READ_CALLEE.into())),
                args: vec![Expr::Cast {
                    expr: Box::new(arg.clone()),
                    ty: Type::parse("*const i8"),
                }],
            })),
        }))),
        method: "to_string_lossy".into(),
        args: Vec::new(),
    }
}

fn char_from_int_arg(arg: &Expr) -> Expr {
    Expr::Cast {
        expr: Box::new(Expr::Cast {
            expr: Box::new(arg.clone()),
            ty: Type::Prim(Prim::U8),
        }),
        ty: Type::Custom("char".into()),
    }
}

fn printf_macro_arg(arg: &Expr, kind: ConversionKind, fact: &PrintfArgFact) -> Option<Expr> {
    match kind {
        ConversionKind::Integer(IntegerArg::Value) => Some(arg.clone()),
        ConversionKind::Integer(IntegerArg::Alternate(format)) => {
            Some(alternate_integer_arg(arg, format))
        }
        ConversionKind::Integer(IntegerArg::Precision(format)) => {
            Some(precision_integer_arg(arg, format))
        }
        ConversionKind::Integer(IntegerArg::Narrow { signed, width }) => {
            Some(narrow_integer_arg(arg, signed, width))
        }
        ConversionKind::String(StringArg::Value) => {
            Some(printf_string_arg(arg, fact).unwrap_or_else(|| runtime_c_string_arg(arg)))
        }
        ConversionKind::String(StringArg::Sized(format)) => sized_printf_string_arg(fact, format),
        ConversionKind::Char(CharArg::Value) => Some(
            fact.const_char
                .clone()
                .map(Expr::Str)
                .unwrap_or_else(|| char_from_int_arg(arg)),
        ),
        ConversionKind::Char(CharArg::Sized(format)) => sized_printf_char_arg(fact, format),
        ConversionKind::Float => Some(arg.clone()),
        ConversionKind::Exponent(format) => Some(exponent_arg(arg, format)),
        ConversionKind::General(format) => general_arg(format, fact),
        ConversionKind::Pointer if fact.pointer => Some(arg.clone()),
        ConversionKind::Pointer => None,
    }
}

fn narrow_integer_arg(arg: &Expr, signed: bool, width: NarrowWidth) -> Expr {
    let unsigned_ty = match width {
        NarrowWidth::Byte => Type::Prim(Prim::U8),
        NarrowWidth::Half => Type::Prim(Prim::U16),
    };
    let truncated = Expr::Cast {
        expr: Box::new(arg.clone()),
        ty: unsigned_ty,
    };
    if !signed {
        return truncated;
    }
    let signed_ty = match width {
        NarrowWidth::Byte => Type::Prim(Prim::I8),
        NarrowWidth::Half => Type::Prim(Prim::I16),
    };
    Expr::Cast {
        expr: Box::new(truncated),
        ty: signed_ty,
    }
}

fn printf_string_arg(arg: &Expr, fact: &PrintfArgFact) -> Option<Expr> {
    let stripped = strip_pointer_view(arg);
    if fact.rust_string {
        return Some(stripped.clone());
    }
    if let Some(value) = &fact.const_string {
        return Some(Expr::Str(value.clone()));
    }
    None
}

fn sized_printf_string_arg(fact: &PrintfArgFact, format: StringSizeFormat) -> Option<Expr> {
    let value = fact.const_string.as_ref()?;
    if !value.is_ascii() {
        return None;
    }
    Some(Expr::Str(apply_string_size_format(value, format)))
}

fn sized_printf_char_arg(fact: &PrintfArgFact, format: CharSizeFormat) -> Option<Expr> {
    let value = fact.const_char.as_ref()?;
    Some(Expr::Str(apply_string_size_format(
        value,
        StringSizeFormat {
            left: format.left,
            width: Some(format.width),
            precision: None,
        },
    )))
}

fn apply_string_size_format(value: &str, format: StringSizeFormat) -> String {
    let truncated = match format.precision {
        Some(precision) => value.chars().take(precision).collect::<String>(),
        None => value.to_string(),
    };
    let Some(width) = format.width else {
        return truncated;
    };
    let pad = width.saturating_sub(truncated.chars().count());
    if pad == 0 {
        return truncated;
    }
    let fill = " ".repeat(pad);
    if format.left {
        format!("{truncated}{fill}")
    } else {
        format!("{fill}{truncated}")
    }
}

fn strip_pointer_view(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => strip_pointer_view(expr),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            strip_pointer_view(recv)
        }
        Expr::ArrayPtr { array, .. } => strip_pointer_view(array),
        _ => expr,
    }
}

fn alternate_integer_arg(arg: &Expr, format: AlternateIntegerFormat) -> Expr {
    let arg = match format.narrow {
        Some(width) => narrow_integer_arg(arg, false, width),
        None => arg.clone(),
    };
    let tmp = "__slate_printf_arg";
    let tmp_expr = Expr::Var(tmp.into());
    let zero_cond = Expr::Binary {
        op: BinOp::Eq,
        lhs: Box::new(tmp_expr.clone()),
        rhs: Box::new(Expr::Value(RustValue::I64(0))),
    };
    Expr::Block(Box::new(Block {
        stmts: vec![IndentStmt {
            depth: 0,
            stmt: Stmt::Let {
                name: tmp.into(),
                mutable: false,
                ty: None,
                init: Some(arg),
            },
        }],
        tail: Some(Box::new(Expr::If {
            cond: Box::new(zero_cond),
            then_expr: Box::new(non_alternate_integer_format(format, tmp_expr.clone())),
            else_expr: Box::new(alternate_nonzero_integer_format(format, tmp_expr)),
        })),
    }))
}

fn non_alternate_integer_format(format: AlternateIntegerFormat, arg: Expr) -> Expr {
    let width = format.width.map(|width| width.to_string());
    let placeholder = integer_placeholder(
        format.left,
        false,
        false,
        format.zero,
        width.as_deref(),
        Some(format.radix),
    );
    format_macro("format", vec![Expr::Str(placeholder), arg])
}

fn alternate_nonzero_integer_format(format: AlternateIntegerFormat, arg: Expr) -> Expr {
    match format.radix {
        'x' => format_macro(
            "format",
            vec![
                Expr::Str(integer_placeholder(
                    format.left,
                    false,
                    true,
                    format.zero,
                    format.width.map(|width| width.to_string()).as_deref(),
                    Some('x'),
                )),
                arg,
            ],
        ),
        'X' => prefixed_nonzero_integer_format(format, "0X", "X", arg),
        'o' => prefixed_nonzero_integer_format(format, "0", "o", arg),
        _ => unreachable!(),
    }
}

fn prefixed_nonzero_integer_format(
    format: AlternateIntegerFormat,
    prefix: &str,
    radix: &str,
    arg: Expr,
) -> Expr {
    let base = if format.zero && radix == "X" {
        let digit_width = format.width.unwrap_or(0).saturating_sub(prefix.len());
        format_macro(
            "format",
            vec![
                Expr::Str(format!("{prefix}{{:0>{digit_width}{radix}}}")),
                arg,
            ],
        )
    } else {
        format_macro(
            "format",
            vec![Expr::Str(format!("{prefix}{{:{radix}}}")), arg],
        )
    };
    match format.width {
        Some(width) if format.left => {
            format_macro("format", vec![Expr::Str(format!("{{:<{width}}}")), base])
        }
        Some(width) if format.zero && radix != "X" => {
            format_macro("format", vec![Expr::Str(format!("{{:0>{width}}}")), base])
        }
        Some(width) if !format.zero => {
            format_macro("format", vec![Expr::Str(format!("{{:>{width}}}")), base])
        }
        _ => base,
    }
}

fn precision_integer_arg(arg: &Expr, format: PrecisionIntegerFormat) -> Expr {
    let arg = match format.narrow {
        Some(width) => narrow_integer_arg(
            arg,
            matches!(format.kind, PrecisionKind::SignedDecimal),
            width,
        ),
        None => arg.clone(),
    };
    let inner = match format.kind {
        PrecisionKind::SignedDecimal => signed_precision_expr(&arg, format.precision, format.plus),
        PrecisionKind::UnsignedDecimal => {
            precision_format_call(format!("{{:0{}}}", format.precision), arg)
        }
        PrecisionKind::Hex => precision_format_call(format!("{{:0{}x}}", format.precision), arg),
        PrecisionKind::HexUpper => {
            precision_format_call(format!("{{:0{}X}}", format.precision), arg)
        }
        PrecisionKind::Octal => precision_format_call(format!("{{:0{}o}}", format.precision), arg),
    };
    match format.width {
        Some(width) => {
            let placeholder = if format.left {
                format!("{{:<{width}}}")
            } else {
                format!("{{:>{width}}}")
            };
            precision_format_call(placeholder, inner)
        }
        None => inner,
    }
}

fn signed_precision_expr(arg: &Expr, precision: usize, plus: bool) -> Expr {
    let tmp = "__slate_printf_arg";
    let tmp_expr = Expr::Var(tmp.into());
    let neg_cond = Expr::Binary {
        op: BinOp::Lt,
        lhs: Box::new(tmp_expr.clone()),
        rhs: Box::new(Expr::Value(RustValue::I64(0))),
    };
    let abs_expr = Expr::MethodCall {
        recv: Box::new(tmp_expr.clone()),
        method: "unsigned_abs".into(),
        args: vec![],
    };
    let neg_branch = precision_format_call(format!("-{{:0{precision}}}"), abs_expr);
    let sign_prefix = if plus { "+" } else { "" };
    let pos_branch =
        precision_format_call(format!("{sign_prefix}{{:0{precision}}}"), tmp_expr.clone());
    Expr::Block(Box::new(Block {
        stmts: vec![IndentStmt {
            depth: 0,
            stmt: Stmt::Let {
                name: tmp.into(),
                mutable: false,
                ty: None,
                init: Some(arg.clone()),
            },
        }],
        tail: Some(Box::new(Expr::If {
            cond: Box::new(neg_cond),
            then_expr: Box::new(neg_branch),
            else_expr: Box::new(pos_branch),
        })),
    }))
}

fn precision_format_call(placeholder: String, arg: Expr) -> Expr {
    format_macro("format", vec![Expr::Str(placeholder), arg])
}

fn exponent_arg(arg: &Expr, format: ExponentFormat) -> Expr {
    let marker = if format.upper { 'E' } else { 'e' };

    let tmp = "__slate_printf_arg";
    let tmp_expr = Expr::Var(tmp.into());
    let tmp_stmt = IndentStmt {
        depth: 0,
        stmt: Stmt::Let {
            name: tmp.into(),
            mutable: false,
            ty: None,
            init: Some(arg.clone()),
        },
    };

    let formatted = "__slate_printf_e_formatted";
    let formatted_expr = Expr::Var(formatted.into());
    let formatted_stmt = IndentStmt {
        depth: 0,
        stmt: Stmt::Let {
            name: formatted.into(),
            mutable: false,
            ty: None,
            init: Some(format_macro(
                "format",
                vec![
                    Expr::Str(format!("{{:.{}{marker}}}", format.precision)),
                    tmp_expr.clone(),
                ],
            )),
        },
    };

    let idx = "__slate_printf_e_idx";
    let idx_expr = Expr::Var(idx.into());
    let idx_stmt = IndentStmt {
        depth: 0,
        stmt: Stmt::Let {
            name: idx.into(),
            mutable: false,
            ty: None,
            init: Some(Expr::MethodCall {
                recv: Box::new(Expr::MethodCall {
                    recv: Box::new(formatted_expr.clone()),
                    method: "find".into(),
                    args: vec![Expr::Str(marker.to_string())],
                }),
                method: "unwrap".into(),
                args: vec![],
            }),
        },
    };

    let parts = "__slate_printf_e_parts";
    let parts_expr = Expr::Var(parts.into());
    let parts_stmt = IndentStmt {
        depth: 0,
        stmt: Stmt::Let {
            name: parts.into(),
            mutable: false,
            ty: None,
            init: Some(Expr::MethodCall {
                recv: Box::new(formatted_expr),
                method: "split_at".into(),
                args: vec![idx_expr],
            }),
        },
    };

    let exp = "__slate_printf_e_exp";
    let exp_expr = Expr::Var(exp.into());
    let exp_stmt = IndentStmt {
        depth: 0,
        stmt: Stmt::Let {
            name: exp.into(),
            mutable: false,
            ty: None,
            init: Some(Expr::MethodCall {
                recv: Box::new(Expr::MethodCallGeneric {
                    recv: Box::new(Expr::TupleField {
                        base: Box::new(Expr::MethodCall {
                            recv: Box::new(Expr::TupleField {
                                base: Box::new(parts_expr.clone()),
                                index: 1,
                            }),
                            method: "split_at".into(),
                            args: vec![Expr::Value(RustValue::Usize(1))],
                        }),
                        index: 1,
                    }),
                    method: "parse".into(),
                    type_args: vec![Type::Prim(Prim::I32)],
                    args: vec![],
                }),
                method: "unwrap".into(),
                args: vec![],
            }),
        },
    };

    let exp_rendered = "__slate_printf_e_exp_str";
    let exp_rendered_expr = Expr::Var(exp_rendered.into());
    let exp_rendered_stmt = IndentStmt {
        depth: 0,
        stmt: Stmt::Let {
            name: exp_rendered.into(),
            mutable: false,
            ty: None,
            init: Some(Expr::If {
                cond: Box::new(Expr::Binary {
                    op: BinOp::Lt,
                    lhs: Box::new(exp_expr.clone()),
                    rhs: Box::new(Expr::Value(RustValue::I64(0))),
                }),
                then_expr: Box::new(format_macro(
                    "format",
                    vec![
                        Expr::Str("-{:02}".into()),
                        Expr::MethodCall {
                            recv: Box::new(exp_expr.clone()),
                            method: "unsigned_abs".into(),
                            args: vec![],
                        },
                    ],
                )),
                else_expr: Box::new(format_macro(
                    "format",
                    vec![Expr::Str("+{:02}".into()), exp_expr],
                )),
            }),
        },
    };

    let corrected = "__slate_printf_e_corrected";
    let corrected_expr = Expr::Var(corrected.into());
    let corrected_stmt = IndentStmt {
        depth: 0,
        stmt: Stmt::Let {
            name: corrected.into(),
            mutable: false,
            ty: None,
            init: Some(format_macro(
                "format",
                vec![
                    Expr::Str(format!("{{}}{marker}{{}}")),
                    Expr::TupleField {
                        base: Box::new(parts_expr),
                        index: 0,
                    },
                    exp_rendered_expr,
                ],
            )),
        },
    };

    let tail = if format.plus {
        Expr::If {
            cond: Box::new(Expr::Binary {
                op: BinOp::Lt,
                lhs: Box::new(tmp_expr),
                rhs: Box::new(Expr::Value(RustValue::Float(0.0))),
            }),
            then_expr: Box::new(corrected_expr.clone()),
            else_expr: Box::new(format_macro(
                "format",
                vec![Expr::Str("+{}".into()), corrected_expr],
            )),
        }
    } else {
        corrected_expr
    };

    let inner = Expr::Block(Box::new(Block {
        stmts: vec![
            tmp_stmt,
            formatted_stmt,
            idx_stmt,
            parts_stmt,
            exp_stmt,
            exp_rendered_stmt,
            corrected_stmt,
        ],
        tail: Some(Box::new(tail)),
    }));

    match format.width {
        Some(width) => {
            let placeholder = if format.left {
                format!("{{:<{width}}}")
            } else {
                format!("{{:>{width}}}")
            };
            precision_format_call(placeholder, inner)
        }
        None => inner,
    }
}

fn general_arg(format: GeneralFormat, fact: &PrintfArgFact) -> Option<Expr> {
    let value = fact.const_float?;
    let body = render_general_body(
        value,
        format.precision,
        format.upper,
        format.plus,
        format.alternate,
    );
    let padded = match format.width {
        Some(width) => pad_general_body(body, width, format.left, format.zero),
        None => body,
    };
    Some(Expr::Str(padded))
}

fn render_general_body(
    value: f64,
    precision: usize,
    upper: bool,
    plus: bool,
    alternate: bool,
) -> String {
    let precision = if precision == 0 { 1 } else { precision };
    let sci = format!("{:.*e}", precision - 1, value);
    let exp_marker = sci.find('e').expect("exponential formatting has a marker");
    let exp: i32 = sci[exp_marker + 1..]
        .parse()
        .expect("exponential formatting has a parseable exponent");
    let body = if exp >= -4 && exp < precision as i32 {
        let frac_digits = (precision as i32 - 1 - exp).max(0) as usize;
        if plus {
            format!("{value:+.frac_digits$}")
        } else {
            format!("{value:.frac_digits$}")
        }
    } else {
        let frac_digits = precision - 1;
        let marker = if upper { 'E' } else { 'e' };
        let sci = match (upper, plus) {
            (true, true) => format!("{value:+.frac_digits$E}"),
            (true, false) => format!("{value:.frac_digits$E}"),
            (false, true) => format!("{value:+.frac_digits$e}"),
            (false, false) => format!("{value:.frac_digits$e}"),
        };
        let idx = sci
            .find(marker)
            .expect("exponential formatting has a marker");
        let (mantissa, rest) = sci.split_at(idx);
        let exp: i32 = rest[1..]
            .parse()
            .expect("exponential formatting has a parseable exponent");
        let exp_rendered = if exp < 0 {
            format!("-{:02}", exp.unsigned_abs())
        } else {
            format!("+{exp:02}")
        };
        format!("{mantissa}{marker}{exp_rendered}")
    };
    if alternate {
        body
    } else {
        trim_general_trailing_zeros(&body)
    }
}

fn trim_general_trailing_zeros(body: &str) -> String {
    let (mantissa, suffix) = match body.find(['e', 'E']) {
        Some(idx) => (&body[..idx], &body[idx..]),
        None => (body, ""),
    };
    if !mantissa.contains('.') {
        return body.to_string();
    }
    let trimmed = mantissa.trim_end_matches('0').trim_end_matches('.');
    format!("{trimmed}{suffix}")
}

fn pad_general_body(body: String, width: usize, left: bool, zero: bool) -> String {
    let len = body.chars().count();
    if len >= width {
        return body;
    }
    let pad_len = width - len;
    if left {
        return format!("{body}{}", " ".repeat(pad_len));
    }
    if zero {
        if let Some(rest) = body.strip_prefix('-') {
            return format!("-{}{rest}", "0".repeat(pad_len));
        }
        if let Some(rest) = body.strip_prefix('+') {
            return format!("+{}{rest}", "0".repeat(pad_len));
        }
        return format!("{}{body}", "0".repeat(pad_len));
    }
    format!("{}{body}", " ".repeat(pad_len))
}
