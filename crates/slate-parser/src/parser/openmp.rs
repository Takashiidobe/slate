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
    Some(match (name, ident(tokens.get(2)), ident(tokens.get(3))) {
        ("for", Some("simd"), _) => OmpPragma::Unsupported("for simd".into()),
        ("for", ..) => OmpPragma::Region(OmpDirectiveKind::For),
        ("parallel", Some("for"), Some("simd")) => {
            OmpPragma::Unsupported("parallel for simd".into())
        }
        ("parallel", Some("for"), _) => OmpPragma::Region(OmpDirectiveKind::ParallelFor),
        ("parallel", Some(next), _) if PARALLEL_COMBINED.contains(&next) => {
            OmpPragma::Unsupported(format!("parallel {next}"))
        }
        ("parallel", ..) => OmpPragma::Region(OmpDirectiveKind::Parallel),
        (name, ..) => OmpPragma::Unsupported(name.to_string()),
    })
}

fn allows(kind: OmpDirectiveKind, clause: &str) -> bool {
    match clause {
        "private" | "firstprivate" => true,
        "shared" | "default" | "num_threads" | "if" => kind.is_parallel(),
        "schedule" | "collapse" => kind.is_loop(),
        "nowait" => kind == OmpDirectiveKind::For,
        _ => false,
    }
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
        let mut associated_loops = usize::from(kind.is_loop());
        let mut position = 1 + kind.spelling().split(' ').count();
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
            let unexpected = || {
                self.error_at_tokens(
                    tokens,
                    position,
                    format!("unexpected OpenMP clause `{name}` in `{directive}`"),
                )
            };
            if !allows(kind, name) {
                return Err(unexpected());
            }
            let (clause, end) = if name == "nowait" {
                (OmpClause::Nowait, position + 1)
            } else {
                if tokens.get(position + 1).map(|token| &token.value) != Some(&Token::LParen) {
                    return Err(unexpected());
                }
                let close = matching_paren(tokens, position + 1)
                    .ok_or_else(|| self.error_at_tokens(tokens, position + 1, "expected `)`"))?;
                let clause = self.parse_omp_clause(tokens, position, close)?;
                if let OmpClause::Collapse(expression) = &clause
                    && !self.flavor().is_msvc()
                {
                    associated_loops = crate::const_expr::Parser::evaluate_ast(expression)
                        .ok()
                        .and_then(|value| usize::try_from(value).ok())
                        .filter(|&value| value > 0)
                        .ok_or_else(|| {
                            self.error_at_tokens(
                                tokens,
                                position + 2,
                                "collapse argument needs positive constant integer expression",
                            )
                        })?;
                }
                (clause, close + 1)
            };
            let unique = match clause {
                OmpClause::DataSharing { .. } => false,
                OmpClause::Collapse(_) => !self.flavor().is_msvc(),
                _ => true,
            };
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
            clauses.push(self.cover_tokens(clause, tokens, start, end));
            position = end;
        }
        Ok(OmpDirective {
            kind,
            clauses,
            associated_loops,
        })
    }

    fn parse_omp_clause(
        &self,
        tokens: &[Span<Token>],
        position: usize,
        close: usize,
    ) -> Result<OmpClause, ParseError> {
        let name = ident(tokens.get(position)).unwrap_or_default();
        let arguments = &tokens[position + 2..close];
        Ok(match name {
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
                        Some("private") if !self.flavor().is_msvc() => Some(OmpDefault::Private),
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
                        if ident(Some(modifier)) != Some("parallel") {
                            return Err(self.error_at_tokens(
                                tokens,
                                position + 2,
                                "directive name modifier of `if` must be `parallel`",
                            ));
                        }
                        condition
                    }
                    condition => condition,
                };
                OmpClause::If(self.parse_expression(condition)?)
            }
            "schedule" => OmpClause::Schedule(self.parse_omp_schedule(tokens, position, close)?),
            _ => OmpClause::Collapse(self.parse_expression(arguments)?),
        })
    }

    fn parse_omp_schedule(
        &self,
        tokens: &[Span<Token>],
        position: usize,
        close: usize,
    ) -> Result<OmpSchedule, ParseError> {
        let mut cursor = position + 2;
        let invalid = |at: usize| {
            self.error_at_tokens(
                tokens,
                at,
                "invalid schedule kind in OpenMP `schedule` clause",
            )
        };
        let colon = matches!(
            ident(tokens.get(cursor)),
            Some("monotonic" | "nonmonotonic" | "simd")
        )
        .then(|| (cursor..close).find(|&index| tokens[index].value == Token::Colon))
        .flatten();
        let mut monotonic = None;
        if let Some(colon) = colon {
            if self.flavor().is_msvc() {
                return Err(invalid(cursor));
            }
            for (index, modifier) in tokens[cursor..colon].iter().enumerate() {
                let at = cursor + index;
                if index % 2 == 1 {
                    if modifier.value != Token::Comma {
                        return Err(invalid(at));
                    }
                    continue;
                }
                let value = match ident(Some(modifier)) {
                    Some("monotonic") => OmpScheduleModifier::Monotonic,
                    Some("nonmonotonic") => OmpScheduleModifier::Nonmonotonic,
                    Some("simd") => continue,
                    _ => return Err(invalid(at)),
                };
                if monotonic.is_some_and(|existing| existing != value) {
                    return Err(self.error_at_tokens(
                        tokens,
                        at,
                        "both `monotonic` and `nonmonotonic` modifiers specified",
                    ));
                }
                monotonic = Some(value);
            }
            cursor = colon + 1;
        }
        let kind = match ident(tokens.get(cursor).filter(|_| cursor < close)) {
            Some("static") => OmpScheduleKind::Static,
            Some("dynamic") => OmpScheduleKind::Dynamic,
            Some("guided") => OmpScheduleKind::Guided,
            Some("auto") if !self.flavor().is_msvc() => OmpScheduleKind::Auto,
            Some("runtime") => OmpScheduleKind::Runtime,
            _ => return Err(invalid(cursor)),
        };
        let chunk = match &tokens[cursor + 1..close] {
            [] => None,
            [comma, chunk @ ..] if comma.value == Token::Comma && !chunk.is_empty() => {
                if matches!(kind, OmpScheduleKind::Auto | OmpScheduleKind::Runtime) {
                    return Err(self.error_at_tokens(
                        tokens,
                        cursor,
                        format!(
                            "schedule `{}` does not take a `chunk_size` parameter",
                            kind.spelling()
                        ),
                    ));
                }
                Some(self.parse_expression(chunk)?)
            }
            _ => return Err(self.error_at_tokens(tokens, cursor + 1, "expected `)`")),
        };
        Ok(OmpSchedule {
            kind,
            modifier: monotonic,
            chunk,
        })
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
