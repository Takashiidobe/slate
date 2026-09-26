use super::Parser;
use crate::ast::*;
use crate::attribute_support;
use crate::const_expr;
use crate::lexer::{Token, TokenSpanExt};

impl Parser {
    pub(super) fn parse_attribute_groups(
        &self,
        tokens: &[Span<Token>],
        position: usize,
    ) -> Result<(Vec<Span<Attribute>>, usize), String> {
        parse_attribute_groups(tokens, position, self.biggest_alignment, Some(self))
    }
}

pub(super) struct AttrCursor<'a> {
    tokens: &'a [Span<Token>],
    pos: usize,
}

impl<'a> AttrCursor<'a> {
    pub(super) fn new(tokens: &'a [Span<Token>], pos: usize) -> Self {
        Self { tokens, pos }
    }

    pub(super) fn peek(&self) -> Option<&Token> {
        self.tokens.value_at(self.pos)
    }

    pub(super) fn consume(&mut self, token: &Token) -> bool {
        if self.peek() == Some(token) {
            self.pos += 1;
            true
        } else {
            false
        }
    }

    pub(super) fn expect(&mut self, token: Token, message: &str) -> Result<(), String> {
        if self.consume(&token) {
            Ok(())
        } else {
            Err(message.into())
        }
    }

    pub(super) fn expect_ident(&mut self, message: &str) -> Result<String, String> {
        match self.peek() {
            Some(Token::Ident(name)) => {
                let name = name.clone();
                self.pos += 1;
                Ok(name)
            }
            Some(Token::Keyword(keyword)) => {
                let name = <&str>::from(*keyword).to_string();
                self.pos += 1;
                Ok(name)
            }
            _ => Err(message.into()),
        }
    }

    pub(super) fn parse_parenthesized_arguments(
        &mut self,
        message: &str,
    ) -> Result<&'a [Span<Token>], String> {
        if !self.consume(&Token::LParen) {
            return Ok(&[]);
        }
        let start = self.pos;
        let mut depth = 0;
        while let Some(token) = self.peek() {
            match token {
                Token::LParen => depth += 1,
                Token::RParen if depth == 0 => break,
                Token::RParen => depth -= 1,
                _ => {}
            }
            self.pos += 1;
        }
        let arguments = &self.tokens[start..self.pos];
        self.expect(Token::RParen, message)?;
        Ok(arguments)
    }
}

pub(super) fn parse_attribute_groups(
    tokens: &[Span<Token>],
    position: usize,
    biggest_alignment: i64,
    context: Option<&Parser>,
) -> Result<(Vec<Span<Attribute>>, usize), String> {
    let mut cursor = AttrCursor::new(tokens, position);
    let mut attributes = Vec::new();
    loop {
        if let Some(convention) = keyword_calling_convention(cursor.peek()) {
            let start = cursor.pos;
            cursor.pos += 1;
            attributes.push(locate_attribute(
                tokens,
                start,
                cursor.pos,
                Attribute::CallingConvention(convention),
            ));
        } else if cursor.consume(&Token::Ident("__declspec".into())) {
            cursor.expect(Token::LParen, "expected `(` after __declspec")?;
            while !cursor.consume(&Token::RParen) {
                let start = cursor.pos;
                let name = cursor.expect_ident("expected declspec name")?;
                let end = cursor.pos;
                let arguments = cursor
                    .parse_parenthesized_arguments("expected `)` after declspec arguments")?;
                let canonical = match name.as_str() {
                    "align" => "aligned",
                    "allocate" => "section",
                    _ => &name,
                };
                attributes.push(locate_attribute(
                    tokens,
                    start,
                    end,
                    parse_attribute_spelling(
                        &name,
                        canonical,
                        arguments,
                        biggest_alignment,
                        context,
                    )?,
                ));
                cursor.consume(&Token::Comma);
            }
        } else if cursor.consume(&Token::Ident("_Alignas".into()))
            || cursor.consume(&Token::Ident("alignas".into()))
        {
            let end = cursor.pos;
            let start = end - 1;
            let arguments =
                cursor.parse_parenthesized_arguments("expected `)` after `_Alignas` argument")?;
            if arguments.is_empty() {
                return Err("expected `(` after `_Alignas`".into());
            }
            let operand = match const_expr::Parser::try_parse_full_type_name(arguments, context) {
                Some(ty) => AlignAsOperand::Type { ty },
                None => AlignAsOperand::Expr(parse_attribute_expression(arguments, context)?),
            };
            attributes.push(locate_attribute(
                tokens,
                start,
                end,
                Attribute::AlignAs(operand),
            ));
        } else if cursor.consume(&Token::Ident("__attribute__".into()))
            || cursor.consume(&Token::Ident("__attribute".into()))
        {
            cursor.expect(Token::LParen, "expected `((` after __attribute__")?;
            cursor.expect(Token::LParen, "expected `((` after __attribute__")?;
            if cursor.consume(&Token::RParen) {
                cursor.expect(Token::RParen, "expected `))` after attributes")?;
                continue;
            }
            loop {
                let start = cursor.pos;
                let name = cursor.expect_ident("expected attribute name")?;
                let end = cursor.pos;
                let arguments = cursor
                    .parse_parenthesized_arguments("expected `)` after attribute arguments")?;
                let attribute = if gnu_registered(&name, context) {
                    parse_attribute(&name, arguments, biggest_alignment, context)?
                } else {
                    unknown_attribute(&name, arguments)
                };
                attributes.push(locate_attribute(tokens, start, end, attribute));
                if cursor.consume(&Token::Comma) {
                    continue;
                }
                cursor.expect(Token::RParen, "expected `))` after attributes")?;
                cursor.expect(Token::RParen, "expected `))` after attributes")?;
                break;
            }
        } else if cursor.peek() == Some(&Token::LBracket)
            && cursor.tokens.value_at(cursor.pos + 1) == Some(&Token::LBracket)
        {
            cursor.pos += 2;
            loop {
                if cursor.consume(&Token::RBracket) {
                    cursor.expect(Token::RBracket, "expected `]]` after C23 attributes")?;
                    break;
                }
                if cursor.consume(&Token::Comma) {
                    continue;
                }
                let start = cursor.pos;
                let mut name = cursor.expect_ident("expected C23 attribute name")?;
                if cursor.consume(&Token::Colon) {
                    cursor.expect(Token::Colon, "expected `::` in attribute name")?;
                    let last = cursor.expect_ident("expected attribute name after `::`")?;
                    name.push_str("::");
                    name.push_str(&last);
                }
                let end = cursor.pos;
                let arguments = cursor
                    .parse_parenthesized_arguments("expected `)` after attribute arguments")?;
                let attribute =
                    match c23_attribute_name(&name).filter(|_| c23_registered(&name, context)) {
                        Some(canonical) => parse_attribute_spelling(
                            &name,
                            canonical,
                            arguments,
                            biggest_alignment,
                            context,
                        )?,
                        None => unknown_attribute(&name, arguments),
                    };
                attributes.push(locate_attribute(tokens, start, end, attribute));
                if cursor.consume(&Token::Comma) {
                    continue;
                }
                cursor.expect(Token::RBracket, "expected `]]` after C23 attributes")?;
                cursor.expect(Token::RBracket, "expected `]]` after C23 attributes")?;
                break;
            }
        } else {
            break;
        }
    }
    Ok((attributes, cursor.pos))
}

fn locate_attribute(
    tokens: &[Span<Token>],
    start: usize,
    end: usize,
    attribute: Attribute,
) -> Span<Attribute> {
    let mut span = tokens[start].derive(attribute);
    if let Some(last) = tokens.get(end.saturating_sub(1)) {
        span.spelling = span.spelling.through(last.spelling);
        span.expansion = span.expansion.through(last.expansion);
    }
    span
}

pub(super) fn parse_attribute(
    name: &str,
    arguments: &[Span<Token>],
    biggest_alignment: i64,
    context: Option<&Parser>,
) -> Result<Attribute, String> {
    let checkpoint = context.map(Parser::checkpoint);
    let attribute = parse_attribute_value(name, arguments, biggest_alignment, context)?;
    if !matches!(attribute, Attribute::Invalid { .. })
        && let Some(checkpoint) = checkpoint
    {
        checkpoint.commit();
    }
    Ok(attribute)
}

fn parse_attribute_value(
    name: &str,
    arguments: &[Span<Token>],
    biggest_alignment: i64,
    context: Option<&Parser>,
) -> Result<Attribute, String> {
    let parse_expression =
        |arguments: &[Span<Token>]| parse_attribute_expression(arguments, context);
    let canonical_name = unwrapped_attribute_name(name);
    let single_string = || match arguments {
        [single] => match &single.value {
            Token::StringLit(value) => Some(value.clone()),
            _ => None,
        },
        _ => None,
    };
    let single_ident = || match arguments {
        [single] => match &single.value {
            Token::Ident(value) => Some(value.clone()),
            _ => None,
        },
        _ => None,
    };
    let single_int = || match arguments {
        [single] => match &single.value {
            token @ Token::IntLit(_) => token.integer_value(),
            _ => None,
        },
        _ => None,
    };
    let integers = || {
        arguments
            .split(|token| token.value == Token::Comma)
            .map(|tokens| match tokens {
                [single] => match &single.value {
                    token @ Token::IntLit(_) => token.integer_value().ok_or(()),
                    _ => Err(()),
                },
                _ => Err(()),
            })
            .collect::<Result<Vec<_>, _>>()
            .ok()
    };
    match canonical_name {
        "packed" if arguments.is_empty() => Ok(Attribute::Packed),
        "address_space" | "pass_object_size" | "pass_dynamic_object_size" => {
            Ok(match parse_expression(arguments) {
                Ok(value) => match canonical_name {
                    "address_space" => Attribute::AddressSpace(value),
                    _ => Attribute::PassObjectSize {
                        size_type: value,
                        dynamic: canonical_name == "pass_dynamic_object_size",
                    },
                },
                Err(_) => invalid_attribute(name, arguments),
            })
        }
        "lifetimebound" if arguments.is_empty() => Ok(Attribute::LifetimeBound),
        "overloadable" if arguments.is_empty() => Ok(Attribute::Overloadable),
        "gnu_inline" if arguments.is_empty() => Ok(Attribute::GnuInline),
        "nothrow" if arguments.is_empty() => Ok(Attribute::NoThrow),
        "selectany" if arguments.is_empty() => Ok(Attribute::SelectAny),
        "thread" if arguments.is_empty() => Ok(Attribute::ThreadLocal),
        "noalias" if arguments.is_empty() => Ok(Attribute::NoAlias),
        "restrict" if arguments.is_empty() => Ok(Attribute::RestrictReturn),
        "optnone" if arguments.is_empty() => Ok(Attribute::OptimizeNone),
        "code_seg" => Ok(single_string()
            .map(Attribute::CodeSeg)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "aligned" => Ok(match single_int() {
            Some(value) => Attribute::Aligned(integer_argument(value, arguments)),
            None if arguments.is_empty() => {
                Attribute::Aligned(integer_argument(biggest_alignment, arguments))
            }
            None => Attribute::Aligned(parse_expression(arguments)?),
        }),
        "vector_size" => Ok(match single_int() {
            Some(value) => Attribute::VectorSize(integer_argument(value, arguments)),
            None if !arguments.is_empty() => Attribute::VectorSize(parse_expression(arguments)?),
            None => invalid_attribute(name, arguments),
        }),
        "mode" => Ok(single_ident()
            .map(Attribute::Mode)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "visibility" => Ok(single_string()
            .map(Attribute::Visibility)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "section" => Ok(single_string()
            .map(Attribute::Section)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "annotate" => Ok(single_string()
            .map(Attribute::Annotate)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "target" => Ok(single_string()
            .map(Attribute::Target)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "alias" => Ok(single_string()
            .map(Attribute::Alias)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "weakref" => Ok(single_string()
            .map(Attribute::WeakRef)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "nonnull" if arguments.is_empty() => Ok(Attribute::NonNull(Vec::new())),
        "nonnull" => Ok(integers()
            .map(Attribute::NonNull)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "assume_aligned" => Ok(
            match arguments
                .split(|token| token.value == Token::Comma)
                .map(parse_expression)
                .collect::<Result<Vec<_>, _>>()
            {
                Ok(values) if !values.is_empty() => Attribute::AssumeAligned(values),
                _ => invalid_attribute(name, arguments),
            },
        ),
        "alloc_size" => Ok(
            match arguments
                .split(|token| token.value == Token::Comma)
                .map(parse_expression)
                .collect::<Result<Vec<_>, _>>()
            {
                Ok(values) if !values.is_empty() => Attribute::AllocSize(values),
                _ => invalid_attribute(name, arguments),
            },
        ),
        "alloc_align" => Ok(match parse_expression(arguments) {
            Ok(value) if !arguments.is_empty() => Attribute::AllocAlign(value),
            _ => invalid_attribute(name, arguments),
        }),
        "cleanup" => Ok(single_ident()
            .map(Attribute::Cleanup)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "weak" if arguments.is_empty() => Ok(Attribute::Weak),
        "used" if arguments.is_empty() => Ok(Attribute::Used),
        "retain" if arguments.is_empty() => Ok(Attribute::Retain),
        "noinline" if arguments.is_empty() => Ok(Attribute::NoInline),
        "always_inline" if arguments.is_empty() => Ok(Attribute::AlwaysInline),
        "noreturn" if arguments.is_empty() => Ok(Attribute::NoReturn),
        "constructor" => Ok(match arguments {
            [] => Attribute::Constructor(None),
            [single] => single
                .value
                .integer_value()
                .map(|value| Attribute::Constructor(Some(value)))
                .unwrap_or_else(|| invalid_attribute(name, arguments)),
            _ => invalid_attribute(name, arguments),
        }),
        "destructor" => Ok(match arguments {
            [] => Attribute::Destructor(None),
            [single] => single
                .value
                .integer_value()
                .map(|value| Attribute::Destructor(Some(value)))
                .unwrap_or_else(|| invalid_attribute(name, arguments)),
            _ => invalid_attribute(name, arguments),
        }),
        "malloc" if arguments.is_empty() => Ok(Attribute::Malloc),
        "returns_nonnull" if arguments.is_empty() => Ok(Attribute::ReturnsNonNull),
        "warn_unused_result" if arguments.is_empty() => Ok(Attribute::WarnUnusedResult),
        "sentinel" if arguments.is_empty() => Ok(Attribute::Sentinel(None)),
        "sentinel" => Ok(single_int()
            .map(|value| Attribute::Sentinel(Some(value)))
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "cold" if arguments.is_empty() => Ok(Attribute::Cold),
        "flatten" if arguments.is_empty() => Ok(Attribute::Flatten),
        "hot" if arguments.is_empty() => Ok(Attribute::Hot),
        "leaf" if arguments.is_empty() => Ok(Attribute::Leaf),
        "noipa" if arguments.is_empty() => Ok(Attribute::NoIpa),
        "noclone" if arguments.is_empty() => Ok(Attribute::NoClone),
        "optimize" if !arguments.is_empty() => Ok(Attribute::Optimize(
            arguments.values().map(String::from).collect(),
        )),
        "naked" if arguments.is_empty() => Ok(Attribute::Naked),
        "interrupt" if arguments.is_empty() => Ok(Attribute::Interrupt),
        "no_split_stack" if arguments.is_empty() => Ok(Attribute::NoSplitStack),
        "returns_twice" if arguments.is_empty() => Ok(Attribute::ReturnsTwice),
        "cpu_dispatch" => Ok(Attribute::CpuDispatch(attribute_arguments(arguments))),
        "cpu_specific" => Ok(Attribute::CpuSpecific(attribute_arguments(arguments))),
        "target_clones" => Ok(Attribute::TargetClones(attribute_arguments(arguments))),
        "ifunc" => Ok(single_string()
            .map(Attribute::Ifunc)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "dllimport" if arguments.is_empty() => Ok(Attribute::DllImport),
        "dllexport" if arguments.is_empty() => Ok(Attribute::DllExport),
        "weak_import" if arguments.is_empty() => Ok(Attribute::WeakImport),
        "tls_model" => Ok(single_string()
            .map(Attribute::TlsModel)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "ms_struct" if arguments.is_empty() => Ok(Attribute::MsStruct),
        "cdecl" | "stdcall" | "fastcall" | "vectorcall" | "thiscall" | "ms_abi" | "sysv_abi"
        | "preserve_most" | "preserve_all" | "preserve_none"
            if arguments.is_empty() =>
        {
            let convention = match canonical_name {
                "cdecl" => CallingConvention::Cdecl,
                "stdcall" => CallingConvention::Stdcall,
                "fastcall" => CallingConvention::Fastcall,
                "vectorcall" => CallingConvention::Vectorcall,
                "thiscall" => CallingConvention::Thiscall,
                "ms_abi" => CallingConvention::MsAbi,
                "preserve_most" => CallingConvention::PreserveMost,
                "preserve_all" => CallingConvention::PreserveAll,
                "preserve_none" => CallingConvention::PreserveNone,
                _ => CallingConvention::SysVAbi,
            };
            Ok(Attribute::CallingConvention(convention))
        }
        "regparm" => Ok(match parse_expression(arguments) {
            Ok(value) => Attribute::CallingConvention(CallingConvention::RegParm(value)),
            Err(_) => invalid_attribute(name, arguments),
        }),
        "pcs" => Ok(match single_string().as_deref() {
            Some("aapcs") => {
                Attribute::CallingConvention(CallingConvention::Pcs(PcsConvention::Aapcs))
            }
            Some("aapcs-vfp") => {
                Attribute::CallingConvention(CallingConvention::Pcs(PcsConvention::AapcsVfp))
            }
            _ => invalid_attribute(name, arguments),
        }),
        "nomips16" if arguments.is_empty() => Ok(Attribute::NoMips16),
        "availability" => Ok(Attribute::Availability(attribute_arguments(arguments))),
        "ext_vector_type" => Ok(match parse_expression(arguments) {
            Ok(value) => Attribute::ExtVectorType(value),
            Err(_) => invalid_attribute(name, arguments),
        }),
        "scalar_storage_order" => Ok(single_string()
            .map(Attribute::ScalarStorageOrder)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "transparent_union" if arguments.is_empty() => Ok(Attribute::TransparentUnion),
        "format" => Ok(Attribute::Format(attribute_arguments(arguments))),
        "format_arg" => Ok(Attribute::FormatArg(attribute_arguments(arguments))),
        "gcc_struct" if arguments.is_empty() => Ok(Attribute::GccStruct),
        "common" if arguments.is_empty() => Ok(Attribute::Common),
        "nocommon" if arguments.is_empty() => Ok(Attribute::NoCommon),
        "pure" if arguments.is_empty() => Ok(Attribute::Pure),
        "const" if arguments.is_empty() => Ok(Attribute::Const),
        "may_alias" if arguments.is_empty() => Ok(Attribute::MayAlias),
        "deprecated" if arguments.is_empty() => Ok(Attribute::Deprecated(None)),
        "deprecated" => Ok(single_string()
            .map(|value| Attribute::Deprecated(Some(value)))
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "nodiscard" if arguments.is_empty() => Ok(Attribute::NoDiscard(None)),
        "nodiscard" => Ok(single_string()
            .map(|value| Attribute::NoDiscard(Some(value)))
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "maybe_unused" | "unused" if arguments.is_empty() => Ok(Attribute::MaybeUnused),
        "fallthrough" if arguments.is_empty() => Ok(Attribute::Fallthrough),
        _ if attribute_support::is_modeled(canonical_name) => {
            Ok(invalid_attribute(name, arguments))
        }
        _ => Ok(unknown_attribute(name, arguments)),
    }
}

pub(super) fn invalid_attribute(name: &str, arguments: &[Span<Token>]) -> Attribute {
    Attribute::Invalid {
        name: name.into(),
        arguments: arguments.values().map(String::from).collect(),
    }
}

pub(super) fn attribute_arguments(arguments: &[Span<Token>]) -> Vec<String> {
    arguments
        .split(|token| token.value == Token::Comma)
        .map(|tokens| {
            tokens
                .values()
                .map(String::from)
                .collect::<Vec<_>>()
                .join(" ")
        })
        .collect()
}

fn integer_argument(value: i64, arguments: &[Span<Token>]) -> Expr {
    Box::new(Span::cover(
        ExprKind::IntegerLiteral(const_expr::IntegerLiteral::decimal(value)),
        arguments,
    ))
}

fn parse_attribute_expression(
    arguments: &[Span<Token>],
    context: Option<&Parser>,
) -> Result<Expr, String> {
    const_expr::Parser::parse_expression(arguments, context).map_err(|error| error.to_string())
}

fn keyword_calling_convention(token: Option<&Token>) -> Option<CallingConvention> {
    let Token::Ident(name) = token? else {
        return None;
    };
    Some(match name.as_str() {
        "__cdecl" | "_cdecl" => CallingConvention::Cdecl,
        "__stdcall" | "_stdcall" => CallingConvention::Stdcall,
        "__fastcall" | "_fastcall" => CallingConvention::Fastcall,
        "__vectorcall" => CallingConvention::Vectorcall,
        "__thiscall" | "_thiscall" => CallingConvention::Thiscall,
        _ => return None,
    })
}

fn unwrapped_attribute_name(name: &str) -> &str {
    name.strip_prefix("__")
        .and_then(|name| name.strip_suffix("__"))
        .unwrap_or(name)
}

fn c23_attribute_name(name: &str) -> Option<&str> {
    let (namespace, local) = name.split_once("::").unwrap_or(("", name));
    let local = unwrapped_attribute_name(local);
    match (unwrapped_attribute_name(namespace), local) {
        ("", "deprecated" | "nodiscard" | "maybe_unused" | "noreturn" | "fallthrough") => {
            Some(local)
        }
        ("", "_Noreturn") => Some("noreturn"),
        (
            "gnu",
            "packed"
            | "gnu_inline"
            | "nothrow"
            | "selectany"
            | "unused"
            | "aligned"
            | "vector_size"
            | "mode"
            | "visibility"
            | "section"
            | "target"
            | "alias"
            | "weakref"
            | "nonnull"
            | "weak"
            | "used"
            | "retain"
            | "noinline"
            | "always_inline"
            | "noreturn"
            | "constructor"
            | "destructor"
            | "malloc"
            | "assume_aligned"
            | "alloc_size"
            | "alloc_align"
            | "cleanup"
            | "returns_nonnull"
            | "warn_unused_result"
            | "sentinel"
            | "cold"
            | "flatten"
            | "hot"
            | "leaf"
            | "noipa"
            | "noclone"
            | "optimize"
            | "naked"
            | "interrupt"
            | "no_split_stack"
            | "returns_twice"
            | "target_clones"
            | "ifunc"
            | "dllimport"
            | "tls_model"
            | "ms_struct"
            | "stdcall"
            | "cdecl"
            | "fastcall"
            | "thiscall"
            | "ms_abi"
            | "sysv_abi"
            | "regparm"
            | "pcs"
            | "dllexport"
            | "scalar_storage_order"
            | "transparent_union"
            | "format"
            | "format_arg"
            | "gcc_struct"
            | "common"
            | "nocommon"
            | "pure"
            | "const"
            | "may_alias"
            | "deprecated"
            | "fallthrough",
        ) => Some(local),
        (
            "clang",
            "address_space"
            | "pass_object_size"
            | "pass_dynamic_object_size"
            | "lifetimebound"
            | "overloadable"
            | "optnone"
            | "preserve_most"
            | "preserve_all"
            | "preserve_none"
            | "annotate"
            | "availability"
            | "cpu_dispatch"
            | "cpu_specific"
            | "ext_vector_type"
            | "weak_import"
            | "vectorcall"
            | "always_inline"
            | "noinline",
        ) => Some(local),
        ("msvc", "noinline") => Some("noinline"),
        ("msvc", "forceinline") => Some("always_inline"),
        _ => None,
    }
}

fn gnu_registered(name: &str, context: Option<&Parser>) -> bool {
    context
        .is_none_or(|parser| attribute_support::gnu_registered(name, parser.flavor, &parser.target))
}

fn c23_registered(name: &str, context: Option<&Parser>) -> bool {
    !name.contains("::")
        || context.is_none_or(|parser| {
            attribute_support::spelling_registered(name, parser.flavor, &parser.target)
        })
}

fn parse_attribute_spelling(
    spelling: &str,
    canonical: &str,
    arguments: &[Span<Token>],
    biggest_alignment: i64,
    context: Option<&Parser>,
) -> Result<Attribute, String> {
    Ok(
        match parse_attribute(canonical, arguments, biggest_alignment, context)? {
            Attribute::Invalid { .. } => invalid_attribute(spelling, arguments),
            Attribute::Unknown { .. } => unknown_attribute(spelling, arguments),
            attribute => attribute,
        },
    )
}

fn unknown_attribute(name: &str, arguments: &[Span<Token>]) -> Attribute {
    Attribute::Unknown {
        name: name.into(),
        arguments: arguments.values().map(String::from).collect(),
    }
}
