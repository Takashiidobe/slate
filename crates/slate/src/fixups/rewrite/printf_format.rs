use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, PathSegment, PrintfArgFact, PrintfCallFact,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{BinOp, RustValue};
use crate::rust_ast::{
    Block, Expr, ExternDecl, ExternFnDecl, FnParam, IndentStmt, Item, Program, Stmt, Type,
};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
    let mut logger = crate::fixups::trace::NoopLogger;
    PrintfFormat::new(&mut logger).fixup(program, facts);
}

pub(in crate::fixups) struct PrintfFormat<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> PrintfFormat<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) {
        let before = self.logger.is_enabled().then(|| program.emit());
        fixup_impl(program, facts);
        if let Some(before) = before {
            let after = program.emit();
            if before != after {
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::PrintfFormat,
                    kind: "rewrite_printf_format_calls".into(),
                    location: TraceLocation::default(),
                    before: vec![TraceSnippet::new("program", before.trim_end())],
                    after: vec![TraceSnippet::new("program", after.trim_end())],
                    facts: vec![fact("printf_calls", facts.printf_calls.len().to_string())],
                });
            }
        }
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) {
    let mut converted_any = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            fixup_body(&mut f.body, function, facts, &mut converted_any);
        }
    }
    let has_remaining_raw_calls = program_has_printf_call(program);
    if converted_any && has_remaining_raw_calls {
        ensure_stdout_and_fflush_externs(program);
        wrap_remaining_raw_printf_calls(program);
    }
    if !has_remaining_raw_calls {
        prune_printf_extern(program);
    }
}

fn fixup_body(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    converted: &mut bool,
) {
    fixup_body_with_env(body, function, facts, &mut Vec::new(), converted);
}

fn fixup_body_with_env(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    converted: &mut bool,
) {
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            fixup_stmt(&mut indent.stmt, function, facts, path, converted);
        });
    }
}

fn fixup_stmt(
    stmt: &mut Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    converted: &mut bool,
) {
    match stmt {
        Stmt::Expr(expr) => {
            if let Some(replacement) = rewrite_printf_expr(expr, function, facts, path) {
                *expr = replacement;
                *converted = true;
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
                *converted = true;
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
                fixup_body_with_env(then_body, function, facts, path, converted);
            });
            walk::with_path_segment(path, PathSegment::Else, |path| {
                fixup_body_with_env(else_body, function, facts, path, converted);
            });
        }
        Stmt::Loop { body, .. } => {
            walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                fixup_body_with_env(body, function, facts, path, converted);
            });
        }
        Stmt::For { body, .. } => {
            walk::with_path_segment(path, PathSegment::ForBody, |path| {
                fixup_body_with_env(body, function, facts, path, converted);
            });
        }
        Stmt::Scope { body } => {
            walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                fixup_body_with_env(body, function, facts, path, converted);
            });
        }
        Stmt::LabeledBlock { body, .. } => {
            walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                fixup_body_with_env(body, function, facts, path, converted);
            });
        }
        Stmt::Unsafe { body } => {
            walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                fixup_block(body, function, facts, path, converted);
            });
        }
        Stmt::While { body, .. } => {
            walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                fixup_block(body, function, facts, path, converted);
            });
        }
        Stmt::Block(body) => {
            walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                fixup_block(body, function, facts, path, converted);
            });
        }
        Stmt::Match { arms, .. } => {
            for (index, arm) in arms.iter_mut().enumerate() {
                walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                    fixup_body_with_env(&mut arm.body, function, facts, path, converted);
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
    converted: &mut bool,
) {
    fixup_body_with_env(&mut block.stmts, function, facts, path, converted);
    if let Some(tail) = &mut block.tail
        && let Some(replacement) = walk::with_path_segment(path, PathSegment::BlockTail, |path| {
            rewrite_printf_expr(tail, function, facts, path)
        })
    {
        **tail = replacement;
        *converted = true;
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
    let Expr::Call { func, args } = call else {
        return None;
    };
    if !matches!(&**func, Expr::Var(name) if name.as_str() == "printf") {
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
    Char,
    Float,
    Pointer,
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
}

#[derive(Clone, Copy)]
struct AlternateIntegerFormat {
    left: bool,
    zero: bool,
    width: Option<usize>,
    radix: char,
}

#[derive(Clone, Copy)]
struct PrecisionIntegerFormat {
    left: bool,
    plus: bool,
    width: Option<usize>,
    precision: usize,
    kind: PrecisionKind,
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

    match bytes.get(i).copied()? {
        b'l' => {
            i += 1;
            if bytes.get(i).copied() == Some(b'l') {
                i += 1;
            }
        }
        b'z' => i += 1,
        _ => {}
    }
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
        }))
    } else if alternate {
        let width = width.map(str::parse).transpose().ok()?;
        ConversionKind::Integer(IntegerArg::Alternate(AlternateIntegerFormat {
            left,
            zero,
            width,
            radix: format_kind?,
        }))
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
                    kind: ConversionKind::Char,
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
    parse_sized_string_conversion(bytes, i)
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
    let precision = if bytes.get(i).copied()? == b'.' {
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
        format!("{{:.{precision}}}"),
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
        ConversionKind::String(StringArg::Value) => printf_string_arg(arg, fact),
        ConversionKind::String(StringArg::Sized(format)) => sized_printf_string_arg(fact, format),
        ConversionKind::Char => fact.const_char.clone().map(Expr::Str),
        ConversionKind::Float => Some(arg.clone()),
        ConversionKind::Pointer if fact.pointer => Some(arg.clone()),
        ConversionKind::Pointer => None,
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
                init: Some(arg.clone()),
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
    let inner = match format.kind {
        PrecisionKind::SignedDecimal => signed_precision_expr(arg, format.precision, format.plus),
        PrecisionKind::UnsignedDecimal => {
            precision_format_call(format!("{{:0{}}}", format.precision), arg.clone())
        }
        PrecisionKind::Hex => {
            precision_format_call(format!("{{:0{}x}}", format.precision), arg.clone())
        }
        PrecisionKind::HexUpper => {
            precision_format_call(format!("{{:0{}X}}", format.precision), arg.clone())
        }
        PrecisionKind::Octal => {
            precision_format_call(format!("{{:0{}o}}", format.precision), arg.clone())
        }
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

fn ensure_stdout_and_fflush_externs(program: &mut Program) {
    let has_stdout = program.items.iter().any(|item| {
        matches!(item, Item::ExternBlock { decls, .. } if decls.iter().any(|decl| matches!(decl, ExternDecl::Static { name, .. } if name == "stdout")))
    });
    let has_fflush = program.items.iter().any(|item| {
        matches!(item, Item::ExternBlock { decls, .. } if decls.iter().any(|decl| matches!(decl, ExternDecl::Fn(f) if f.name == "fflush")))
    });
    if has_stdout && has_fflush {
        return;
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
}

fn stdout_static_decl() -> ExternDecl {
    ExternDecl::Static {
        mutable: true,
        name: "stdout".into(),
        ty: Type::parse("*mut libc::FILE"),
    }
}

fn fflush_fn_decl() -> ExternDecl {
    ExternDecl::Fn(ExternFnDecl {
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

fn wrap_remaining_raw_printf_calls(program: &mut Program) {
    for item in &mut program.items {
        if let Item::Fn(f) = item {
            wrap_raw_printf_in_body(&mut f.body);
        }
    }
}

fn wrap_raw_printf_in_body(body: &mut Vec<IndentStmt>) {
    let mut index = 0;
    while index < body.len() {
        let mut path = Vec::new();
        walk::nested_body_vecs_mut_with_path(&mut body[index].stmt, &mut path, &mut |body, _| {
            wrap_raw_printf_in_body(body);
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
            index += 3;
        } else {
            index += 1;
        }
    }
}

fn is_raw_printf_call_stmt(stmt: &Stmt) -> bool {
    let Stmt::Expr(expr) = stmt else {
        return false;
    };
    matches!(peel_empty_unsafe(expr), Expr::Call { func, .. } if matches!(&**func, Expr::Var(name) if name.as_str() == "printf"))
}

fn flush_before_stmt() -> Stmt {
    Stmt::Expr(Expr::MethodCall {
        recv: Box::new(Expr::Call {
            func: Box::new(Expr::Var("std::io::Write::flush".into())),
            args: vec![Expr::Ref {
                mutable: true,
                expr: Box::new(Expr::Call {
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

fn prune_printf_extern(program: &mut Program) {
    program.items.retain_mut(|item| match item {
        Item::ExternBlock { decls, .. } => {
            decls.retain(|decl| !matches!(decl, ExternDecl::Fn(f) if f.name == "printf"));
            !decls.is_empty()
        }
        _ => true,
    });
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
        Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}

fn expr_has_printf_call(expr: &Expr) -> bool {
    match expr {
        Expr::Call { func, args } => {
            matches!(&**func, Expr::Var(name) if name.as_str() == "printf")
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{
        Block, Expr, ExternDecl, ExternFnDecl, FnParam, IndentStmt, Item, MatchArm, Pattern, Prim,
        Program, Stmt, Type, Visibility,
    };

    fn printf_decl() -> ExternDecl {
        ExternDecl::Fn(ExternFnDecl {
            name: "printf".into(),
            params: vec![FnParam {
                name: "_0".into(),
                mutable: false,
                ty: Type::Ptr {
                    mutable: true,
                    inner: Box::new(Type::Prim(Prim::I8)),
                },
                nonnull: false,
            }],
            variadic: true,
            ret: Some(Type::Prim(Prim::I32)),
            returns_nonnull: false,
        })
    }

    fn fmt_arg(bytes: &[u8]) -> Expr {
        Expr::Cast {
            expr: Box::new(Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::ByteStr(bytes.to_vec())),
                    method: "as_ptr".into(),
                    args: vec![],
                }),
                ty: Type::parse("*mut libc::c_char"),
            }),
            ty: Type::Ptr {
                mutable: true,
                inner: Box::new(Type::Prim(Prim::I8)),
            },
        }
    }

    fn printf_stmt(bytes: &[u8], value: Expr) -> Stmt {
        Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: vec![],
            tail: Some(Box::new(call("printf", vec![fmt_arg(bytes), value]))),
        })))
    }

    fn printf_stmt_args(bytes: &[u8], args: Vec<Expr>) -> Stmt {
        let mut call_args = vec![fmt_arg(bytes)];
        call_args.extend(args);
        Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: vec![],
            tail: Some(Box::new(call("printf", call_args))),
        })))
    }

    fn program(stmt: Stmt) -> Program {
        program_with_body(vec![stmt])
    }

    fn program_with_body(stmts: Vec<Stmt>) -> Program {
        Program {
            items: vec![
                Item::ExternBlock {
                    abi: "C".into(),
                    decls: vec![printf_decl()],
                },
                Item::Fn(crate::rust_ast::FnDef {
                    attrs: Vec::new(),
                    vis: Visibility::Private,
                    unsafe_: false,
                    abi: None,
                    name: "main".into(),
                    params: vec![],
                    ret: None,
                    body: stmts
                        .into_iter()
                        .map(|stmt| IndentStmt { depth: 1, stmt })
                        .collect(),
                    returns_nonnull: false,
                }),
            ],
        }
    }

    fn run(stmt: Stmt) -> String {
        let mut program = program(stmt);
        run_fixup(&mut program);
        program.emit()
    }

    fn run_fixup(program: &mut Program) {
        let analyzed = crate::fixups::facts::analyze(program.clone());
        fixup(program, &analyzed.facts);
    }

    #[test]
    fn rewrites_percent_d_newline_to_println_and_removes_printf_extern() {
        let out = run(printf_stmt(b"%d\n\0", call("add", vec![int(2), int(3)])));

        assert_eq!(
            out,
            "\
fn main() {
    println!(\"{}\", add(2, 3));
}
"
        );
    }

    #[test]
    fn rewrites_plain_newline_to_println_without_arguments() {
        let out = run(Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: vec![],
            tail: Some(Box::new(call("printf", vec![fmt_arg(b"\n\0")]))),
        }))));

        assert_eq!(
            out,
            "\
fn main() {
    println!();
}
"
        );
    }

    #[test]
    fn rewrites_printf_calls_inside_match_arms() {
        let out = run(Stmt::Match {
            expr: var("state"),
            arms: vec![MatchArm {
                pattern: Pattern::I64(0),
                body: vec![IndentStmt {
                    depth: 0,
                    stmt: printf_stmt(b"%d\n\0", var("sum")),
                }],
            }],
        });

        assert_eq!(
            out,
            "\
fn main() {
    match state {
        0 => {
            println!(\"{}\", sum);
        }
    }
}
"
        );
    }

    #[test]
    fn rewrites_printf_calls_inside_loops_before_trailing_newline() {
        let mut program = program_with_body(vec![
            Stmt::Loop {
                label: None,
                body: vec![
                    IndentStmt {
                        depth: 0,
                        stmt: printf_stmt(b"%d \0", var("x")),
                    },
                    IndentStmt {
                        depth: 0,
                        stmt: Stmt::Break(None),
                    },
                ],
            },
            printf_stmt_args(b"\n\0", vec![]),
        ]);
        run_fixup(&mut program);
        let out = program.emit();

        assert_eq!(
            out,
            "\
fn main() {
    loop {
        print!(\"{} \", x);
        break;
    }
    println!();
}
"
        );
    }

    #[test]
    fn rewrites_printf_calls_inside_for_loops_before_trailing_newline() {
        let mut program = program_with_body(vec![
            Stmt::For {
                pat: "i".into(),
                iter: Expr::Range {
                    start: Box::new(int(0)),
                    end: Box::new(int(3)),
                },
                body: vec![IndentStmt {
                    depth: 0,
                    stmt: printf_stmt(b"%d \0", var("i")),
                }],
            },
            printf_stmt_args(b"\n\0", vec![]),
        ]);
        run_fixup(&mut program);
        let out = program.emit();

        assert_eq!(
            out,
            "\
fn main() {
    for i in 0..3 {
        print!(\"{} \", i);
    }
    println!();
}
"
        );
    }

    #[test]
    fn rewrites_multiple_percent_d_conversions() {
        let out = run(printf_stmt_args(
            b"%d %d %d\n\0",
            vec![var("a"), var("b"), call("add", vec![int(2), int(3)])],
        ));

        assert_eq!(
            out,
            "\
fn main() {
    println!(\"{} {} {}\", a, b, add(2, 3));
}
"
        );
    }

    #[test]
    fn rewrites_signed_and_unsigned_integer_conversions() {
        let out = run(printf_stmt_args(
            b"%i %u %ld %lu %lld %llu %zu\n\0",
            vec![
                var("i"),
                var("u"),
                var("l"),
                var("ul"),
                var("ll"),
                var("ull"),
                var("n"),
            ],
        ));

        assert_eq!(
            out,
            "\
fn main() {
    println!(\"{} {} {} {} {} {} {}\", i, u, l, ul, ll, ull, n);
}
"
        );
    }

    #[test]
    fn rewrites_static_width_zero_left_and_sign_integer_formats() {
        let out = run(printf_stmt_args(
            b"%05d|%-4d|%+d|%5u|%+06ld\n\0",
            vec![var("a"), var("b"), var("c"), var("u"), var("l")],
        ));

        assert_eq!(
            out,
            "\
fn main() {
    println!(\"{:05}|{:<4}|{:+}|{:5}|{:+06}\", a, b, c, u, l);
}
"
        );
    }

    #[test]
    fn rewrites_hex_and_octal_integer_conversions() {
        let out = run(printf_stmt_args(
            b"%x %X %o %08x %-4X %5o\n\0",
            vec![
                var("lo"),
                var("hi"),
                var("oct"),
                var("padded"),
                var("left"),
                var("wide"),
            ],
        ));

        assert_eq!(
            out,
            "\
fn main() {
    println!(\"{:x} {:X} {:o} {:08x} {:<4X} {:5o}\", lo, hi, oct, padded, left, wide);
}
"
        );
    }

    #[test]
    fn rewrites_alternate_hex_integer_conversions() {
        let out = run(printf_stmt_args(
            b"%#x %#X %#o %#08x %-#10X %#12lo\n\0",
            vec![
                var("lo"),
                var("hi"),
                var("oct"),
                var("padded"),
                var("left"),
                var("wide"),
            ],
        ));

        assert!(out.contains("println!(\"{} {} {} {} {} {}\","));
        assert!(out.contains("format!(\"{:#x}\", __slate_printf_arg)"));
        assert!(out.contains("format!(\"0X{:X}\", __slate_printf_arg)"));
        assert!(out.contains("format!(\"0{:o}\", __slate_printf_arg)"));
        assert!(out.contains("format!(\"{:#08x}\", __slate_printf_arg)"));
        assert!(out.contains("format!(\"{:<10}\", format!(\"0X{:X}\", __slate_printf_arg))"));
        assert!(out.contains("format!(\"{:>12}\", format!(\"0{:o}\", __slate_printf_arg))"));
    }

    #[test]
    fn rewrites_constant_string_and_ascii_char_conversions() {
        let mut program = program_with_body(vec![
            temp("ch", "i32", int(65)),
            printf_stmt_args(
                b"%s %c %d\n\0",
                vec![fmt_arg(b"tag\0"), var("ch"), var("n")],
            ),
        ]);
        run_fixup(&mut program);
        let out = program.emit();

        assert_eq!(
            out,
            "\
fn main() {
    let ch: i32 = 65;
    println!(\"{} {} {}\", \"tag\", \"A\", n);
}
"
        );
    }

    #[test]
    fn leaves_pointer_strings_and_non_ascii_chars_unsupported() {
        for stmt in [
            printf_stmt_args(b"%s\n\0", vec![var("ptr")]),
            printf_stmt_args(b"%c\n\0", vec![int(128)]),
            printf_stmt_args(b"%s\n\0", vec![fmt_arg(b"a\0b\0")]),
        ] {
            let out = run(stmt);
            assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
            assert!(out.contains("unsafe { printf("));
            assert!(!out.contains("println!"));
        }
    }

    #[test]
    fn rewrites_ascii_constant_string_width_and_precision_forms() {
        let out = run(printf_stmt_args(
            b"%5s|%-5s|%.1s|%6.1s\n\0",
            vec![
                fmt_arg(b"hi\0"),
                fmt_arg(b"hi\0"),
                fmt_arg(b"hi\0"),
                fmt_arg(b"hi\0"),
            ],
        ));

        assert_eq!(
            out,
            "\
fn main() {
    println!(\"{}|{}|{}|{}\", \"   hi\", \"hi   \", \"h\", \"     h\");
}
"
        );
    }

    #[test]
    fn leaves_dynamic_and_non_ascii_string_width_precision_unsupported() {
        let mut program = program_with_body(vec![
            Stmt::Let {
                name: "s".into(),
                mutable: false,
                ty: Some(Type::parse("String")),
                init: None,
            },
            printf_stmt_args(b"%5s\n\0", vec![var("s")]),
        ]);
        run_fixup(&mut program);
        let out = program.emit();
        assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
        assert!(out.contains("unsafe { printf("));
        assert!(!out.contains("println!"));

        for stmt in [
            printf_stmt(b"%5s\n\0", fmt_arg(b"caf\xc3\xa9\0")),
            printf_stmt(b"%.*s\n\0", fmt_arg(b"hi\0")),
        ] {
            let out = run(stmt);
            assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
            assert!(out.contains("unsafe { printf("));
            assert!(!out.contains("println!"));
        }
    }

    #[test]
    fn rewrites_percent_d_without_newline_to_print() {
        let out = run(printf_stmt(b"value=%d\0", var("x")));

        assert_eq!(
            out,
            "\
fn main() {
    print!(\"value={}\", x);
}
"
        );
    }

    #[test]
    fn rewrites_literal_percent_and_braces() {
        let out = run(Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: vec![],
            tail: Some(Box::new(call(
                "printf",
                vec![fmt_arg(b"ratio %% {ok}\n\0")],
            ))),
        }))));

        assert_eq!(
            out,
            "\
fn main() {
    println!(\"ratio % {{ok}}\");
}
"
        );
    }

    #[test]
    fn leaves_unsupported_formats_and_extern_declaration() {
        let out = run(printf_stmt(b"%e\n\0", var("x")));

        assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
        assert!(out.contains("unsafe { printf("));
        assert!(!out.contains("println!"));
    }

    #[test]
    fn rewrites_fixed_float_conversions() {
        let out = run(printf_stmt_args(
            b"%f %.2f %.0f\n\0",
            vec![var("x"), var("y"), var("z")],
        ));

        assert_eq!(
            out,
            "\
fn main() {
    println!(\"{:.6} {:.2} {:.0}\", x, y, z);
}
"
        );
    }

    #[test]
    fn rewrites_pointer_conversions_for_pointer_typed_arguments() {
        let pointer_ty = Type::Ptr {
            mutable: true,
            inner: Box::new(Type::CLib(crate::rust_ast::CLibType::Void)),
        };
        let mut program = program_with_body(vec![
            Stmt::Let {
                name: "ptr".into(),
                mutable: false,
                ty: Some(pointer_ty),
                init: None,
            },
            printf_stmt_args(b"%p %p\n\0", vec![var("ptr"), fmt_arg(b"tag\0")]),
        ]);
        run_fixup(&mut program);
        let out = program.emit();

        assert_eq!(
            out,
            "\
fn main() {
    let ptr: *mut core::ffi::c_void;
    println!(\"{:p} {:p}\", ptr, (b\"tag\\0\".as_ptr() as *mut libc::c_char) as *mut i8);
}
"
        );
    }

    #[test]
    fn leaves_ambiguous_float_formats_unsupported() {
        for fmt in [
            &b"%e\n\0"[..],
            &b"%g\n\0"[..],
            &b"%8.2f\n\0"[..],
            &b"%+.2f\n\0"[..],
            &b"%.*f\n\0"[..],
            &b"%Lf\n\0"[..],
        ] {
            let out = run(printf_stmt(fmt, var("x")));
            assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
            assert!(out.contains("unsafe { printf("));
            assert!(!out.contains("println!"));
        }
    }

    #[test]
    fn leaves_mismatched_argument_counts_unsupported() {
        let out = run(printf_stmt_args(b"%d %d\n\0", vec![var("x")]));

        assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
        assert!(out.contains("unsafe { printf("));
        assert!(!out.contains("println!"));
    }

    #[test]
    fn leaves_dynamic_width_precision_and_unsupported_flags() {
        for fmt in [
            &b"%*d\n\0"[..],
            &b"%.*d\n\0"[..],
            &b"%.0d\n\0"[..],
            &b"%#.3d\n\0"[..],
            &b"%#d\n\0"[..],
            &b"% d\n\0"[..],
            &b"%-05d\n\0"[..],
            &b"%+x\n\0"[..],
            &b"%20p\n\0"[..],
        ] {
            let out = run(printf_stmt(fmt, var("x")));
            assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
            assert!(out.contains("unsafe { printf("));
            assert!(!out.contains("println!"));
        }
    }

    #[test]
    fn rewrites_static_integer_precision_forms() {
        let out = run(printf_stmt_args(
            b"%.3d %.3u %.4x %.4X %.4o\n\0",
            vec![var("a"), var("b"), var("c"), var("d"), var("e")],
        ));

        assert!(out.contains("println!(\"{} {} {} {} {}\","));
        assert!(out.contains("let __slate_printf_arg = a;"));
        assert!(out.contains("format!(\"-{:03}\", __slate_printf_arg.unsigned_abs())"));
        assert!(out.contains("format!(\"{:03}\", __slate_printf_arg)"));
        assert!(out.contains("format!(\"{:03}\", b)"));
        assert!(out.contains("format!(\"{:04x}\", c)"));
        assert!(out.contains("format!(\"{:04X}\", d)"));
        assert!(out.contains("format!(\"{:04o}\", e)"));
    }

    #[test]
    fn rewrites_integer_precision_with_width_and_sign_interactions() {
        let out = run(printf_stmt_args(
            b"%8.3d %-8.3d %+.3d %08.3d\n\0",
            vec![var("a"), var("b"), var("c"), var("d")],
        ));

        assert!(out.contains("println!(\"{} {} {} {}\","));
        assert!(out.contains("format!(\"{:>8}\", {\n"));
        assert!(out.contains("format!(\"{:<8}\", {\n"));
        assert!(out.contains("format!(\"-{:03}\", __slate_printf_arg.unsigned_abs())"));
        assert!(out.contains("format!(\"+{:03}\", __slate_printf_arg)"));
        assert!(out.matches("let __slate_printf_arg = a;").count() == 1);
        assert!(out.matches("let __slate_printf_arg = b;").count() == 1);
        assert!(out.matches("let __slate_printf_arg = c;").count() == 1);
        assert!(out.matches("let __slate_printf_arg = d;").count() == 1);
    }

    #[test]
    fn converts_supported_calls_and_wraps_remaining_raw_calls_in_flush_barriers() {
        let mut program = program_with_body(vec![
            printf_stmt(b"%d\n\0", var("x")),
            printf_stmt(b"%e\n\0", var("y")),
        ]);
        run_fixup(&mut program);
        let out = program.emit();

        assert!(out.contains("static mut stdout: *mut libc::FILE;"));
        assert!(out.contains("fn fflush(_0: *mut libc::FILE) -> i32;"));
        assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
        assert!(out.contains("println!(\"{}\", x);"));
        assert!(out.contains("std::io::Write::flush(&mut std::io::stdout()).unwrap();"));
        assert!(out.contains("unsafe { printf("));
        assert!(out.contains("unsafe { fflush((unsafe { stdout }) as *mut libc::FILE) };"));

        let flush_index = out.find("std::io::Write::flush").unwrap();
        let printf_index = out.find("unsafe { printf(").unwrap();
        let fflush_index = out.find("unsafe { fflush(").unwrap();
        assert!(flush_index < printf_index && printf_index < fflush_index);
    }

    #[test]
    fn leaves_fully_unsupported_program_without_flush_barriers() {
        let out = run(printf_stmt(b"%e\n\0", var("x")));

        assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
        assert!(out.contains("unsafe { printf("));
        assert!(!out.contains("println!"));
        assert!(!out.contains("stdout"));
        assert!(!out.contains("fflush"));
    }
}
