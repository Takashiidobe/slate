use super::Parser;
use crate::ast::*;
use crate::const_expr;
use crate::lexer::{Token, TokenSpanExt};

impl Parser {
    pub(super) fn parse_attribute_groups(
        &self,
        tokens: &[Span<Token>],
        position: usize,
    ) -> Result<(Vec<Attribute>, usize), String> {
        parse_attribute_groups(tokens, position, self.biggest_alignment)
    }

    pub(super) fn parse_record_attributes(
        &self,
        tokens: &[Span<Token>],
    ) -> Result<(Vec<Attribute>, usize), String> {
        self.parse_attribute_groups(tokens, 1)
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
    ) -> Result<Vec<Span<Token>>, String> {
        if !self.consume(&Token::LParen) {
            return Ok(Vec::new());
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
        let arguments = self.tokens[start..self.pos].to_vec();
        self.expect(Token::RParen, message)?;
        Ok(arguments)
    }
}

pub(super) fn parse_attribute_groups(
    tokens: &[Span<Token>],
    position: usize,
    biggest_alignment: i64,
) -> Result<(Vec<Attribute>, usize), String> {
    let mut cursor = AttrCursor::new(tokens, position);
    let mut attributes = Vec::new();
    loop {
        if cursor.consume(&Token::Ident("_Alignas".into()))
            || cursor.consume(&Token::Ident("alignas".into()))
        {
            let arguments =
                cursor.parse_parenthesized_arguments("expected `)` after `_Alignas` argument")?;
            if arguments.is_empty() {
                return Err("expected `(` after `_Alignas`".into());
            }
            attributes.push(Attribute::Aligned(parse_attribute_expression(&arguments)?));
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
                let name = cursor.expect_ident("expected attribute name")?;
                let arguments = cursor
                    .parse_parenthesized_arguments("expected `)` after attribute arguments")?;
                attributes.push(parse_attribute(&name, &arguments, biggest_alignment)?);
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
                let mut name = cursor.expect_ident("expected C23 attribute name")?;
                if cursor.consume(&Token::Colon) {
                    cursor.expect(Token::Colon, "expected `::` in attribute name")?;
                    let last = cursor.expect_ident("expected attribute name after `::`")?;
                    name.push_str("::");
                    name.push_str(&last);
                }
                let arguments = cursor
                    .parse_parenthesized_arguments("expected `)` after attribute arguments")?;
                attributes.push(parse_attribute(&name, &arguments, biggest_alignment)?);
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

pub(super) fn parse_attribute(
    name: &str,
    arguments: &[Span<Token>],
    biggest_alignment: i64,
) -> Result<Attribute, String> {
    let canonical_name = name
        .strip_prefix("__")
        .and_then(|name| name.strip_suffix("__"))
        .unwrap_or(name);
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
        "aligned" => Ok(match single_int() {
            Some(value) => Attribute::Aligned(integer_argument(value, arguments)),
            None if arguments.is_empty() => {
                Attribute::Aligned(integer_argument(biggest_alignment, arguments))
            }
            None => Attribute::Aligned(parse_attribute_expression(arguments)?),
        }),
        "vector_size" => Ok(match single_int() {
            Some(value) => Attribute::VectorSize(integer_argument(value, arguments)),
            None if !arguments.is_empty() => {
                Attribute::VectorSize(parse_attribute_expression(arguments)?)
            }
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
                .map(parse_attribute_expression)
                .collect::<Result<Vec<_>, _>>()
            {
                Ok(values) if !values.is_empty() => Attribute::AssumeAligned(values),
                _ => invalid_attribute(name, arguments),
            },
        ),
        "alloc_size" => Ok(
            match arguments
                .split(|token| token.value == Token::Comma)
                .map(parse_attribute_expression)
                .collect::<Result<Vec<_>, _>>()
            {
                Ok(values) if !values.is_empty() => Attribute::AllocSize(values),
                _ => invalid_attribute(name, arguments),
            },
        ),
        "alloc_align" => Ok(match parse_attribute_expression(arguments) {
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
        "weak_import" if arguments.is_empty() => Ok(Attribute::WeakImport),
        "tls_model" => Ok(single_string()
            .map(Attribute::TlsModel)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "ms_struct" if arguments.is_empty() => Ok(Attribute::MsStruct),
        "stdcall" if arguments.is_empty() => Ok(Attribute::Stdcall),
        "nomips16" if arguments.is_empty() => Ok(Attribute::NoMips16),
        "availability" => Ok(Attribute::Availability(attribute_arguments(arguments))),
        "ext_vector_type" => Ok(match parse_attribute_expression(arguments) {
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
        "maybe_unused" if arguments.is_empty() => Ok(Attribute::MaybeUnused),
        "fallthrough" if arguments.is_empty() => Ok(Attribute::Fallthrough),
        _ if canonical_name.is_attribute_name() => Ok(invalid_attribute(name, arguments)),
        _ => Ok(Attribute::Unknown {
            name: name.into(),
            arguments: arguments.values().map(String::from).collect(),
        }),
    }
}

pub(crate) fn apply_vector_attributes(mut ty: CType, attributes: &[Attribute]) -> CType {
    for attribute in attributes {
        let size = match attribute {
            Attribute::VectorSize(size) => VectorSize::Bytes(size.clone()),
            Attribute::ExtVectorType(size) => VectorSize::Lanes(size.clone()),
            _ => continue,
        };
        ty = CType::Vector(VectorType {
            element: Box::new(ty),
            size,
        });
    }
    ty
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
    Box::new(Span::cover(ExprKind::Integer(value), arguments))
}

pub(super) fn parse_attribute_expression(arguments: &[Span<Token>]) -> Result<Expr, String> {
    const_expr::Parser::parse(arguments).map_err(|error| error.to_string())
}

trait AttributeName {
    fn is_attribute_name(&self) -> bool;
}

impl AttributeName for str {
    fn is_attribute_name(&self) -> bool {
        matches!(
            self,
            "aligned"
                | "vector_size"
                | "mode"
                | "visibility"
                | "section"
                | "annotate"
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
                | "cpu_dispatch"
                | "cpu_specific"
                | "target_clones"
                | "ifunc"
                | "dllimport"
                | "weak_import"
                | "tls_model"
                | "ms_struct"
                | "stdcall"
                | "nomips16"
                | "availability"
                | "ext_vector_type"
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
                | "nodiscard"
                | "maybe_unused"
                | "fallthrough"
        )
    }
}
