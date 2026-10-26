use super::Parser;
use super::decl::{matching_paren, split_top_level};
use crate::ast::*;
use crate::error::ParseError;
use crate::lexer::Token;

const DIRECTIVES: &[&str] = &[
    "allocate",
    "assume",
    "assumes",
    "atomic",
    "barrier",
    "begin",
    "cancel",
    "cancellation",
    "critical",
    "declare",
    "depobj",
    "dispatch",
    "distribute",
    "end",
    "error",
    "flush",
    "for",
    "interop",
    "loop",
    "masked",
    "master",
    "metadirective",
    "nothing",
    "ordered",
    "parallel",
    "requires",
    "scan",
    "scope",
    "section",
    "sections",
    "simd",
    "single",
    "target",
    "task",
    "taskgroup",
    "taskloop",
    "taskwait",
    "taskyield",
    "teams",
    "threadprivate",
    "tile",
    "unroll",
    "workshare",
];

const PARALLEL_COMBINED: &[&str] = &["for", "loop", "masked", "master", "sections", "workshare"];

pub(super) enum OmpPragma {
    Region(OmpDirectiveKind),
    Unsupported(String),
    Unknown,
}

fn ident(token: Option<&Span<Token>>) -> Option<&str> {
    match token.map(|token| &token.value) {
        Some(Token::Ident(name)) => Some(name),
        Some(Token::Keyword(keyword)) => Some((*keyword).into()),
        _ => None,
    }
}

pub(super) fn classify(tokens: &[Span<Token>]) -> Option<OmpPragma> {
    if ident(tokens.first()) != Some("omp") {
        return None;
    }
    let Some(name) = ident(tokens.get(1)).filter(|name| DIRECTIVES.contains(name)) else {
        return Some(OmpPragma::Unknown);
    };
    Some(match (name, ident(tokens.get(2))) {
        ("parallel", Some(next)) if PARALLEL_COMBINED.contains(&next) => {
            OmpPragma::Unsupported(format!("parallel {next}"))
        }
        ("parallel", _) => OmpPragma::Region(OmpDirectiveKind::Parallel),
        (name, _) => OmpPragma::Unsupported(name.to_string()),
    })
}

impl Parser {
    pub(super) fn is_omp_region(&self, tokens: &[Span<Token>]) -> bool {
        self.dialect().options().openmp && matches!(classify(tokens), Some(OmpPragma::Region(_)))
    }

    pub(super) fn parse_omp_directive(
        &self,
        tokens: &[Span<Token>],
    ) -> Result<OmpDirective, ParseError> {
        let Some(OmpPragma::Region(kind)) = classify(tokens) else {
            return Err(self.error_at_tokens(tokens, 1, "expected an OpenMP directive"));
        };
        let directive = format!("#pragma omp {}", kind.spelling());
        let mut clauses: Vec<Span<OmpClause>> = Vec::new();
        let mut position = 2;
        while position < tokens.len() {
            if tokens[position].value == Token::Comma && !clauses.is_empty() {
                position += 1;
                continue;
            }
            let start = position;
            let Some(name) = ident(tokens.get(position)) else {
                return Err(self.error_at_tokens(
                    tokens,
                    position,
                    format!("expected an OpenMP clause in `{directive}`"),
                ));
            };
            if tokens.get(position + 1).map(|token| &token.value) != Some(&Token::LParen) {
                return Err(self.error_at_tokens(
                    tokens,
                    position,
                    format!("unexpected OpenMP clause `{name}` in `{directive}`"),
                ));
            }
            let close = matching_paren(tokens, position + 1)
                .ok_or_else(|| self.error_at_tokens(tokens, position + 1, "expected `)`"))?;
            let arguments = &tokens[position + 2..close];
            let clause = match name {
                "private" | "firstprivate" | "shared" => {
                    let sharing = match name {
                        "private" => OmpDataSharing::Private,
                        "firstprivate" => OmpDataSharing::Firstprivate,
                        _ => OmpDataSharing::Shared,
                    };
                    let variables = split_top_level(arguments, &Token::Comma)
                        .into_iter()
                        .map(|variable| self.parse_omp_variable(tokens, position, variable))
                        .collect::<Result<_, _>>()?;
                    OmpClause::DataSharing { sharing, variables }
                }
                "default" => {
                    let default = match arguments {
                        [argument] => match ident(Some(argument)) {
                            Some("shared") => Some(OmpDefault::Shared),
                            Some("none") => Some(OmpDefault::None),
                            Some("private") if !self.flavor().is_msvc() => {
                                Some(OmpDefault::Private)
                            }
                            Some("firstprivate") if !self.flavor().is_msvc() => {
                                Some(OmpDefault::Firstprivate)
                            }
                            _ => None,
                        },
                        _ => None,
                    };
                    OmpClause::Default(default.ok_or_else(|| {
                        self.error_at_tokens(
                            tokens,
                            position + 2,
                            "invalid argument in OpenMP `default` clause",
                        )
                    })?)
                }
                "num_threads" => OmpClause::NumThreads(self.parse_expression(arguments)?),
                "if" => {
                    let condition = match arguments {
                        [modifier, colon, condition @ ..]
                            if !self.flavor().is_msvc() && colon.value == Token::Colon =>
                        {
                            if ident(Some(modifier)) != Some(kind.spelling()) {
                                return Err(self.error_at_tokens(
                                    tokens,
                                    position + 2,
                                    format!(
                                        "directive name modifier of `if` must be `{}`",
                                        kind.spelling()
                                    ),
                                ));
                            }
                            condition
                        }
                        condition => condition,
                    };
                    OmpClause::If(self.parse_expression(condition)?)
                }
                _ => {
                    return Err(self.error_at_tokens(
                        tokens,
                        position,
                        format!("unexpected OpenMP clause `{name}` in `{directive}`"),
                    ));
                }
            };
            let unique = matches!(
                clause,
                OmpClause::Default(_) | OmpClause::NumThreads(_) | OmpClause::If(_)
            );
            if unique
                && clauses.iter().any(|existing| {
                    std::mem::discriminant(&existing.value) == std::mem::discriminant(&clause)
                })
            {
                return Err(self.error_at_tokens(
                    tokens,
                    position,
                    format!("`{directive}` cannot contain more than one `{name}` clause"),
                ));
            }
            clauses.push(self.cover_tokens(clause, tokens, start, close + 1));
            position = close + 1;
        }
        Ok(OmpDirective { kind, clauses })
    }

    fn parse_omp_variable(
        &self,
        tokens: &[Span<Token>],
        clause: usize,
        variable: &[Span<Token>],
    ) -> Result<Expr, ParseError> {
        match variable {
            [name] if matches!(name.value, Token::Ident(_)) => self.parse_expression(variable),
            _ => Err(self.error_at_tokens(tokens, clause, "expected variable name")),
        }
    }

    pub(super) fn omp_statement_expected(&self, tokens: &[Span<Token>]) -> ParseError {
        let name = tokens
            .get(1)
            .and_then(|token| ident(Some(token)))
            .unwrap_or_default();
        self.error_at_tokens(
            tokens,
            1,
            format!("expected a statement after `#pragma omp {name}`"),
        )
    }

    pub(super) fn omp_pragma_error(&self, tokens: &[Span<Token>]) -> Option<ParseError> {
        if !self.dialect().options().openmp {
            return None;
        }
        match classify(tokens)? {
            OmpPragma::Region(kind) => Some(self.error_at_tokens(
                tokens,
                1,
                format!(
                    "unexpected OpenMP directive `#pragma omp {}`",
                    kind.spelling()
                ),
            )),
            OmpPragma::Unsupported(name) if name == "simd" => None,
            OmpPragma::Unsupported(name) => Some(self.error_at_tokens(
                tokens,
                1,
                format!("OpenMP directive `#pragma omp {name}` is not supported yet"),
            )),
            OmpPragma::Unknown if self.flavor().is_gcc() => None,
            OmpPragma::Unknown => {
                Some(self.error_at_tokens(tokens, 1, "expected an OpenMP directive"))
            }
        }
    }
}
