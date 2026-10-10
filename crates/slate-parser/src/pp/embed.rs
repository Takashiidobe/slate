use super::error::{PPErrorKind, PPFailure};
use super::include::IncludeDirective;
use super::syntax::{Directive, identifier};
use super::{PPNode, PPNodeKind, Preprocessor};
use crate::ast::{FileId, Loc, Span};
use crate::compiler_args::CompilerFlavor;
use crate::const_expr;
use crate::files::display_path;
use crate::lexer::{Token, TokenSpanExt};
use num_bigint::Sign;

#[derive(Default)]
struct EmbedParameters {
    prefix: Vec<Span<Token>>,
    suffix: Vec<Span<Token>>,
    if_empty: Vec<Span<Token>>,
    limit: Option<u64>,
    offset: u64,
    unknown: Option<(String, Loc)>,
}

impl EmbedParameters {
    fn window(&self, length: u64) -> (u64, u64) {
        let start = self.offset.min(length);
        let rest = length - start;
        (start, self.limit.map_or(rest, |limit| rest.min(limit)))
    }
}

struct EmbedParameter<'t> {
    vendor: Option<String>,
    name: String,
    clause: Option<&'t [Span<Token>]>,
    loc: Loc,
}

impl EmbedParameter<'_> {
    fn spelling(&self) -> String {
        match &self.vendor {
            Some(vendor) => format!("{vendor}::{}", self.name),
            None => self.name.clone(),
        }
    }
}

fn standard_spelling(name: &str) -> &str {
    name.strip_prefix("__")
        .and_then(|name| name.strip_suffix("__"))
        .filter(|name| !name.is_empty())
        .unwrap_or(name)
}

fn vendor_prefix(flavor: CompilerFlavor) -> Option<&'static str> {
    match flavor {
        CompilerFlavor::Gcc => Some("gnu"),
        CompilerFlavor::Clang => Some("clang"),
        CompilerFlavor::Msvc => None,
    }
}

fn split_parameters<'t>(
    src: &str,
    tokens: &'t [Span<Token>],
) -> Result<Vec<EmbedParameter<'t>>, PPFailure> {
    let mut parameters = Vec::new();
    let mut index = 0;
    while index < tokens.len() {
        let loc = tokens[index].spelling;
        let invalid = || PPFailure::at(loc, PPErrorKind::InvalidEmbedParameter);
        let first = identifier(src, &tokens[index]).ok_or_else(invalid)?;
        index += 1;
        let (vendor, name) = if tokens.value_at(index) == Some(&Token::Colon)
            && tokens.value_at(index + 1) == Some(&Token::Colon)
        {
            let name = tokens
                .get(index + 2)
                .and_then(|token| identifier(src, token))
                .ok_or_else(invalid)?;
            index += 3;
            (Some(first), name)
        } else {
            (None, first)
        };
        let clause = if tokens.value_at(index) == Some(&Token::LParen) {
            let start = index + 1;
            let mut depth = 1usize;
            index = start;
            while index < tokens.len() && depth != 0 {
                match tokens[index].value {
                    Token::LParen => depth += 1,
                    Token::RParen => depth -= 1,
                    _ => {}
                }
                index += 1;
            }
            if depth != 0 {
                return Err(invalid());
            }
            Some(&tokens[start..index - 1])
        } else {
            None
        };
        parameters.push(EmbedParameter {
            vendor,
            name,
            clause,
            loc,
        });
    }
    Ok(parameters)
}

impl Preprocessor<'_> {
    fn embed_parameters(
        &self,
        tokens: &[Span<Token>],
        file: FileId,
    ) -> Result<EmbedParameters, PPFailure> {
        let vendor = vendor_prefix(self.dialect.flavor());
        let mut parameters = EmbedParameters::default();
        for parameter in split_parameters(self.source(file), tokens)? {
            let name = standard_spelling(&parameter.name);
            let standard = match parameter.vendor.as_deref().map(standard_spelling) {
                None => matches!(name, "limit" | "prefix" | "suffix" | "if_empty"),
                Some(prefix) => Some(prefix) == vendor && name == "offset",
            };
            if !standard {
                parameters
                    .unknown
                    .get_or_insert_with(|| (parameter.spelling(), parameter.loc));
                continue;
            }
            let Some(clause) = parameter.clause else {
                return Err(PPFailure::at(
                    parameter.loc,
                    PPErrorKind::InvalidEmbedParameter,
                ));
            };
            match name {
                "prefix" => parameters.prefix = clause.to_vec(),
                "suffix" => parameters.suffix = clause.to_vec(),
                "if_empty" => parameters.if_empty = clause.to_vec(),
                "limit" => parameters.limit = Some(self.embed_operand(clause, parameter.loc)?),
                _ => parameters.offset = self.embed_operand(clause, parameter.loc)?,
            }
        }
        Ok(parameters)
    }

    fn embed_operand(&self, clause: &[Span<Token>], loc: Loc) -> Result<u64, PPFailure> {
        let value = const_expr::Parser::evaluate_directive_operand(clause, self.dialect, &|name| {
            self.is_defined(name)
        })
        .map_err(|located| {
            PPFailure::at(
                loc,
                PPErrorKind::InvalidExpression {
                    directive: "#embed",
                    message: located.error.to_string(),
                },
            )
        })?;
        if value.signed && value.value.sign() == Sign::Minus {
            return Err(PPFailure::at(loc, PPErrorKind::NegativeEmbedOperand));
        }
        Ok(u64::try_from(&value.value).unwrap_or(u64::MAX))
    }

    pub(super) fn expand_embed(&mut self, directive: &Directive) -> Result<PPNode, PPFailure> {
        let arguments = self.expand_macros(&directive.arguments)?;
        let Some(Span {
            value: Token::StringLit(name),
            ..
        }) = arguments.first()
        else {
            return Err(PPFailure::at(
                directive.arguments_loc(),
                PPErrorKind::ExpectedEmbedResource,
            ));
        };
        let include = IncludeDirective::Quoted(name.to_string());
        let (path, _) = self
            .resolve_include(&include, directive.loc.file)
            .ok_or_else(|| {
                PPFailure::at(
                    arguments[0].spelling,
                    PPErrorKind::HeaderNotFound(include.to_string()),
                )
            })?;
        let parameters = self.embed_parameters(&arguments[1..], directive.loc.file)?;
        if let Some((name, loc)) = parameters.unknown {
            return Err(PPFailure::at(loc, PPErrorKind::UnknownEmbedParameter(name)));
        }
        let bytes = std::fs::read(&path).map_err(|error| {
            PPFailure::at(
                arguments[0].spelling,
                PPErrorKind::ReadFailed {
                    path: display_path(&path),
                    message: error.to_string(),
                },
            )
        })?;
        let (start, length) = parameters.window(bytes.len() as u64);
        let bytes = &bytes[start as usize..(start + length) as usize];
        let loc = directive.loc;
        let mut tokens = if bytes.is_empty() {
            parameters.if_empty
        } else {
            parameters.prefix
        };
        for (index, byte) in bytes.iter().enumerate() {
            if index != 0 {
                tokens.push(Span::new(Token::Comma, loc, loc));
            }
            tokens.push(Span::new(
                Token::IntLit(i64::from(*byte).to_string().into()),
                loc,
                loc,
            ));
        }
        if !bytes.is_empty() {
            tokens.extend(parameters.suffix);
        }
        let provenance = self.provenance(loc);
        let tokens: Vec<_> = tokens
            .into_iter()
            .map(|token| token.with_provenance(provenance))
            .collect();
        Ok(
            Span::new(PPNodeKind::Code { tokens, provenance }, loc, loc)
                .with_provenance(provenance),
        )
    }

    pub(super) fn expand_has_embed(
        &self,
        tokens: &[Span<Token>],
        from: FileId,
    ) -> Result<Vec<Span<Token>>, PPFailure> {
        let mut expanded = Vec::with_capacity(tokens.len());
        let mut index = 0;
        while index < tokens.len() {
            let operand = if tokens.value_at(index) == Some(&Token::Ident("__has_embed".into()))
                && tokens.value_at(index + 1) == Some(&Token::LParen)
                && let Some(Token::StringLit(name)) = tokens.value_at(index + 2)
            {
                closing_paren(tokens, index + 2).map(|close| (name, close))
            } else {
                None
            };
            let Some((name, close)) = operand else {
                expanded.push(tokens[index].clone());
                index += 1;
                continue;
            };
            let value = self.has_embed(name, &tokens[index + 3..close], from)?;
            expanded.push(
                tokens[index]
                    .clone()
                    .with_value(Token::IntLit(value.to_string().into())),
            );
            index = close + 1;
        }
        Ok(expanded)
    }

    fn has_embed(
        &self,
        name: &str,
        parameters: &[Span<Token>],
        from: FileId,
    ) -> Result<u8, PPFailure> {
        let Some((path, _)) =
            self.resolve_include(&IncludeDirective::Quoted(name.to_string()), from)
        else {
            return Ok(0);
        };
        let parameters = self.embed_parameters(parameters, from)?;
        if parameters.unknown.is_some() {
            return Ok(0);
        }
        let Ok(metadata) = std::fs::metadata(&path) else {
            return Ok(0);
        };
        Ok(match parameters.window(metadata.len()) {
            (_, 0) => 2,
            _ => 1,
        })
    }
}

fn closing_paren(tokens: &[Span<Token>], start: usize) -> Option<usize> {
    let mut depth = 1usize;
    for (index, token) in tokens.iter().enumerate().skip(start) {
        match token.value {
            Token::LParen => depth += 1,
            Token::RParen => {
                depth -= 1;
                if depth == 0 {
                    return Some(index);
                }
            }
            _ => {}
        }
    }
    None
}
