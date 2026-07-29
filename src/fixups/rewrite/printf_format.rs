use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, PathSegment, PrintfArgFact, PrintfCallFact,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::function_identity::{Known, known_call, known_declaration};
use crate::rust_ast::{BinOp, RustValue};
use crate::rust_ast::{
    Block, Expr, ExternDecl, ExternFnDecl, FnParam, IndentStmt, Item, Prim, Program, Stmt, Type,
};

pub(in crate::fixups) struct PrintfFormat<'a> {
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> PrintfFormat<'a> {
    pub(in crate::fixups) fn new(facts: &'a FixupFacts, logger: &'a mut dyn TraceLogger) -> Self {
        Self { facts, logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program) -> bool {
        let before = self.logger.is_enabled().then(|| program.emit());
        let changed = fixup_impl(program, self.facts);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::PrintfFormat,
                kind: "rewrite_printf_format_calls".into(),
                location: TraceLocation::default(),
                before: vec![TraceSnippet::new("program", before.trim_end())],
                after: vec![TraceSnippet::new("program", program.emit().trim_end())],
                facts: vec![fact(
                    "printf_calls",
                    self.facts.printf_calls.len().to_string(),
                )],
            });
        }
        changed
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut changed = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            fixup_body(&mut f.body, function, facts, &mut changed);
        }
    }
    let has_remaining_raw_calls = program_has_printf_call(program);
    if has_remaining_raw_calls {
        changed |= ensure_stdout_and_fflush_externs(program);
        changed |= wrap_remaining_raw_printf_calls(program);
    } else {
        changed |= prune_printf_extern(program);
    }
    changed
}

fn fixup_body(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    changed: &mut bool,
) {
    fixup_body_with_env(body, function, facts, &mut Vec::new(), changed);
}

fn fixup_body_with_env(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    changed: &mut bool,
) {
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            fixup_stmt(&mut indent.stmt, function, facts, path, changed);
        });
    }
}

fn fixup_stmt(
    stmt: &mut Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    changed: &mut bool,
) {
    match stmt {
        Stmt::Expr(expr) => {
            if let Some(replacement) = rewrite_printf_expr(expr, function, facts, path) {
                *expr = replacement;
                *changed = true;
            }
        }
        Stmt::Let {
            name,
            mutable: false,
            init: Some(init),
            ..
        } if unused_local(function, facts, name, path) => {
            if let Some(replacement) = rewrite_printf_expr(init, function, facts, path) {
                *stmt = Stmt::Expr(replacement);
                *changed = true;
            }
        }
        Stmt::If {
            then_body,
            else_body,
            ..
        }
        | Stmt::LetIf {
            then_body,
            else_body,
            ..
        } => {
            walk::with_path_segment(path, PathSegment::Then, |path| {
                fixup_body_with_env(then_body, function, facts, path, changed);
            });
            walk::with_path_segment(path, PathSegment::Else, |path| {
                fixup_body_with_env(else_body, function, facts, path, changed);
            });
        }
        Stmt::Loop { body, .. } => {
            walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                fixup_body_with_env(body, function, facts, path, changed);
            });
        }
        Stmt::For { body, .. } => {
            walk::with_path_segment(path, PathSegment::ForBody, |path| {
                fixup_body_with_env(body, function, facts, path, changed);
            });
        }
        Stmt::Scope { body } => {
            walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                fixup_body_with_env(body, function, facts, path, changed);
            });
        }
        Stmt::LabeledBlock { body, .. } => {
            walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                fixup_body_with_env(body, function, facts, path, changed);
            });
        }
        Stmt::Unsafe { body } => {
            walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                fixup_block(body, function, facts, path, changed);
            });
        }
        Stmt::While { body, .. } => {
            walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                fixup_block(body, function, facts, path, changed);
            });
        }
        Stmt::Block(body) => {
            walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                fixup_block(body, function, facts, path, changed);
            });
        }
        Stmt::Match { arms, .. } => {
            for (index, arm) in arms.iter_mut().enumerate() {
                walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                    fixup_body_with_env(&mut arm.body, function, facts, path, changed);
                });
            }
        }
        _ => {}
    }
}

fn fixup_block(
    block: &mut Block,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    changed: &mut bool,
) {
    fixup_body_with_env(&mut block.stmts, function, facts, path, changed);
    if let Some(tail) = &mut block.tail
        && let Some(replacement) = walk::with_path_segment(path, PathSegment::BlockTail, |path| {
            rewrite_printf_expr(tail, function, facts, path)
        })
    {
        **tail = replacement;
        *changed = true;
    }
}

fn rewrite_printf_expr(
    expr: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<Expr> {
    let fact = printf_fact(function, facts, path)?;
    let call = peel_empty_unsafe(expr);
    let Expr::Call { args, .. } = call else {
        return None;
    };
    if known_call(call) != Some(Known::Printf) {
        return None;
    }
    let rest = args.get(1..)?;
    if fact.arg_paths.len() != rest.len() {
        return None;
    }
    let macro_call = printf_macro(fact.format.as_deref()?, rest, &fact.arg_facts)?;
    Some(macro_call)
}

fn printf_fact<'a>(
    function: FunctionId,
    facts: &'a FixupFacts,
    path: &[PathSegment],
) -> Option<&'a PrintfCallFact> {
    facts
        .printf_call(function, &AstPath(path.to_vec()))
        .or_else(|| facts.printf_call(function, &unsafe_tail_path(path)))
}

fn unsafe_tail_path(path: &[PathSegment]) -> AstPath {
    let mut path = path.to_vec();
    path.push(PathSegment::UnsafeBody);
    path.push(PathSegment::BlockTail);
    AstPath(path)
}

fn unused_local(
    function: FunctionId,
    facts: &FixupFacts,
    name: &str,
    path: &[PathSegment],
) -> bool {
    facts
        .binding_by_local_path(function, name, &AstPath(path.to_vec()))
        .and_then(|binding| facts.def_use(binding))
        .is_some_and(|def_use| def_use.reads.is_empty())
}

fn peel_empty_unsafe(expr: &Expr) -> &Expr {
    if let Expr::Unsafe(block) = expr
        && block.stmts.is_empty()
        && let Some(tail) = &block.tail
    {
        return tail;
    }
    expr
}

fn const_c_string(expr: &Expr) -> Option<Vec<u8>> {
    match expr {
        Expr::Str(s) => Some(s.as_bytes().to_vec()),
        Expr::ByteStr(bytes) => Some(trim_c_nul(bytes)),
        Expr::CStr(bytes) => Some(bytes.clone()),
        Expr::Cast { expr, .. } => const_c_string(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            const_c_string(recv)
        }
        _ => None,
    }
}

fn trim_c_nul(bytes: &[u8]) -> Vec<u8> {
    bytes.strip_suffix(&[0]).unwrap_or(bytes).to_vec()
}

fn printf_macro(format: &[u8], args: &[Expr], arg_facts: &[PrintfArgFact]) -> Option<Expr> {
    let parsed = parse_printf_format(format)?;
    if parsed.conversions.len() != args.len() {
        return None;
    }
    let mut macro_args = Vec::new();
    let name = if parsed.trailing_newline {
        if parsed.format.is_empty() && args.is_empty() {
            return Some(format_macro("println", vec![]));
        }
        "println"
    } else {
        "print"
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
        ConversionKind::String(StringArg::Value) => printf_string_arg(arg, fact),
        ConversionKind::String(StringArg::Sized(format)) => sized_printf_string_arg(fact, format),
        ConversionKind::Char(CharArg::Value) => fact.const_char.clone().map(Expr::Str),
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

fn ensure_stdout_and_fflush_externs(program: &mut Program) -> bool {
    let has_stdout = program.items.iter().any(|item| {
        matches!(item, Item::ExternBlock { decls, .. } if decls.iter().any(|decl| matches!(decl, ExternDecl::Static { name, .. } if name == "stdout")))
    });
    let has_fflush = program.items.iter().any(|item| {
        matches!(item, Item::ExternBlock { decls, .. } if decls.iter().any(|decl| matches!(decl, ExternDecl::Fn(f) if f.name == "fflush")))
    });
    if has_stdout && has_fflush {
        return false;
    }
    let mut new_decls = Vec::new();
    if !has_stdout {
        new_decls.push(stdout_static_decl());
    }
    if !has_fflush {
        new_decls.push(fflush_fn_decl());
    }
    if let Some(Item::ExternBlock { decls, .. }) = program
        .items
        .iter_mut()
        .find(|item| matches!(item, Item::ExternBlock { abi, .. } if abi == "C"))
    {
        decls.extend(new_decls);
    } else {
        program.items.insert(
            0,
            Item::ExternBlock {
                abi: "C".into(),
                decls: new_decls,
            },
        );
    }
    true
}

fn stdout_static_decl() -> ExternDecl {
    ExternDecl::Static {
        attrs: Vec::new(),
        mutable: true,
        name: "stdout".into(),
        ty: Type::parse("*mut libc::FILE"),
    }
}

fn fflush_fn_decl() -> ExternDecl {
    ExternDecl::Fn(ExternFnDecl {
        identity: crate::function_identity::FunctionIdentity::Unknown,
        name: "fflush".into(),
        params: vec![FnParam {
            name: "_0".into(),
            mutable: false,
            ty: Type::parse("*mut libc::FILE"),
            nonnull: false,
        }],
        variadic: false,
        ret: Some(Type::parse("i32")),
        returns_nonnull: false,
    })
}

fn wrap_remaining_raw_printf_calls(program: &mut Program) -> bool {
    let mut changed = false;
    for item in &mut program.items {
        if let Item::Fn(f) = item {
            changed |= wrap_raw_printf_in_body(&mut f.body);
        }
    }
    changed
}

fn wrap_raw_printf_in_body(body: &mut Vec<IndentStmt>) -> bool {
    let mut changed = false;
    let mut index = 0;
    while index < body.len() {
        let mut path = Vec::new();
        walk::nested_body_vecs_mut_with_path(&mut body[index].stmt, &mut path, &mut |body, _| {
            changed |= wrap_raw_printf_in_body(body);
        });
        if is_raw_printf_call_stmt(&body[index].stmt) {
            let depth = body[index].depth;
            body.insert(
                index + 1,
                IndentStmt {
                    depth,
                    stmt: fflush_after_stmt(),
                },
            );
            body.insert(
                index,
                IndentStmt {
                    depth,
                    stmt: flush_before_stmt(),
                },
            );
            changed = true;
            index += 3;
        } else {
            index += 1;
        }
    }
    changed
}

fn is_raw_printf_call_stmt(stmt: &Stmt) -> bool {
    let Stmt::Expr(expr) = stmt else {
        return false;
    };
    known_call(peel_empty_unsafe(expr)) == Some(Known::Printf)
}

fn flush_before_stmt() -> Stmt {
    Stmt::Expr(Expr::MethodCall {
        recv: Box::new(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("std::io::Write::flush".into())),
            args: vec![Expr::Ref {
                mutable: true,
                expr: Box::new(Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var("std::io::stdout".into())),
                    args: Vec::new(),
                }),
            }],
        }),
        method: "unwrap".into(),
        args: Vec::new(),
    })
}

fn fflush_after_stmt() -> Stmt {
    Stmt::Expr(Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("fflush".into())),
            args: vec![Expr::Cast {
                expr: Box::new(Expr::Unsafe(Box::new(Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(Expr::Var("stdout".into()))),
                }))),
                ty: Type::parse("*mut libc::FILE"),
            }],
        })),
    })))
}

fn prune_printf_extern(program: &mut Program) -> bool {
    let before_items = program.items.len();
    let before_decls = program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::ExternBlock { decls, .. } => Some(decls.len()),
            _ => None,
        })
        .sum::<usize>();
    program.items.retain_mut(|item| match item {
        Item::ExternBlock { decls, .. } => {
            decls.retain(|decl| {
                !matches!(decl, ExternDecl::Fn(f) if known_declaration(f.identity, &f.name) == Some(Known::Printf))
            });
            !decls.is_empty()
        }
        _ => true,
    });
    let after_decls = program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::ExternBlock { decls, .. } => Some(decls.len()),
            _ => None,
        })
        .sum::<usize>();
    program.items.len() != before_items || after_decls != before_decls
}

fn program_has_printf_call(program: &Program) -> bool {
    program.items.iter().any(|item| match item {
        Item::Fn(f) => body_has_printf_call(&f.body),
        _ => false,
    })
}

fn body_has_printf_call(body: &[IndentStmt]) -> bool {
    body.iter().any(|indent| stmt_has_printf_call(&indent.stmt))
}

fn block_has_printf_call(block: &Block) -> bool {
    body_has_printf_call(&block.stmts) || block.tail.as_deref().is_some_and(expr_has_printf_call)
}

fn stmt_has_printf_call(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init.as_ref().is_some_and(expr_has_printf_call),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_has_printf_call(cond)
                || body_has_printf_call(then_body)
                || expr_has_printf_call(then_value)
                || body_has_printf_call(else_body)
                || expr_has_printf_call(else_value)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_has_printf_call(target) || expr_has_printf_call(value)
        }
        Stmt::Expr(expr) => expr_has_printf_call(expr),
        Stmt::Return(expr) => expr.as_ref().is_some_and(expr_has_printf_call),
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_has_printf_call(body)
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_has_printf_call(cond)
                || body_has_printf_call(then_body)
                || body_has_printf_call(else_body)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_has_printf_call(body)
        }
        Stmt::For { iter, body, .. } => expr_has_printf_call(iter) || body_has_printf_call(body),
        Stmt::Match { expr, arms } => {
            expr_has_printf_call(expr) || arms.iter().any(|arm| body_has_printf_call(&arm.body))
        }
        Stmt::InlineAsm(_) | Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}

fn expr_has_printf_call(expr: &Expr) -> bool {
    match expr {
        Expr::Call { func, args, .. } => {
            known_call(expr) == Some(Known::Printf)
                || expr_has_printf_call(func)
                || args.iter().any(expr_has_printf_call)
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_has_printf_call(expr),
        Expr::Binary { lhs, rhs, .. } => expr_has_printf_call(lhs) || expr_has_printf_call(rhs),
        Expr::Range { start, end } => expr_has_printf_call(start) || expr_has_printf_call(end),
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_has_printf_call(recv) || args.iter().any(expr_has_printf_call)
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => expr_has_printf_call(base),
        Expr::ArrayPtr { array, .. } => expr_has_printf_call(array),
        Expr::Index { base, index } => expr_has_printf_call(base) || expr_has_printf_call(index),
        Expr::StructLit { fields, .. } => {
            fields.iter().any(|(_, value)| expr_has_printf_call(value))
        }
        Expr::TupleStructLit { fields, .. } => fields.iter().any(expr_has_printf_call),
        Expr::ArrayLit(elems) => elems.iter().any(expr_has_printf_call),
        Expr::ArrayRepeat { elem, .. } => expr_has_printf_call(elem),
        Expr::VecLit(elems) => elems.iter().any(expr_has_printf_call),
        Expr::VecRepeat { elem, len } => expr_has_printf_call(elem) || expr_has_printf_call(len),
        Expr::Macro { args, .. } => args.iter().any(expr_has_printf_call),
        Expr::Closure { body, .. } => expr_has_printf_call(body),
        Expr::Match { expr, arms } => {
            expr_has_printf_call(expr) || arms.iter().any(|arm| expr_has_printf_call(&arm.value))
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_has_printf_call(cond)
                || expr_has_printf_call(then_expr)
                || expr_has_printf_call(else_expr)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block_has_printf_call(block),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            expr_has_printf_call(src) || expr_has_printf_call(dst)
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => expr_has_printf_call(src) || expr_has_printf_call(dst) || expr_has_printf_call(count),
        Expr::WriteBytes { dst, val, count } => {
            expr_has_printf_call(dst) || expr_has_printf_call(val) || expr_has_printf_call(count)
        }
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            place.ptr_expr().is_some_and(expr_has_printf_call)
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            place.ptr_expr().is_some_and(expr_has_printf_call) || expr_has_printf_call(value)
        }
        Expr::AtomicNew { value, .. } => expr_has_printf_call(value),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            place.ptr_expr().is_some_and(expr_has_printf_call)
                || expr_has_printf_call(expected)
                || expr_has_printf_call(desired)
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => false,
    }
}
