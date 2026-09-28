use super::error::{PPErrorKind, PPFailure};
use crate::ast::{Loc, Span};
use crate::lexer::Token;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum DirectiveName {
    If,
    Ifdef,
    Ifndef,
    Elif,
    Elifdef,
    Elifndef,
    Else,
    Endif,
    Define,
    Undef,
    Include,
    IncludeNext,
    Embed,
    Error,
    Warning,
    Pragma,
    Line,
    LineMarker,
    Ident,
    Null,
    Unknown,
}

impl DirectiveName {
    fn from_spelling(spelling: &str) -> Self {
        match spelling {
            "if" => Self::If,
            "ifdef" => Self::Ifdef,
            "ifndef" => Self::Ifndef,
            "elif" => Self::Elif,
            "elifdef" => Self::Elifdef,
            "elifndef" => Self::Elifndef,
            "else" => Self::Else,
            "endif" => Self::Endif,
            "define" => Self::Define,
            "undef" => Self::Undef,
            "include" => Self::Include,
            "include_next" => Self::IncludeNext,
            "embed" => Self::Embed,
            "error" => Self::Error,
            "warning" => Self::Warning,
            "pragma" => Self::Pragma,
            "line" => Self::Line,
            "ident" | "sccs" => Self::Ident,
            _ => Self::Unknown,
        }
    }
}

pub(super) fn directive_spelling(name: DirectiveName) -> &'static str {
    match name {
        DirectiveName::Ifdef => "#ifdef",
        DirectiveName::Ifndef => "#ifndef",
        DirectiveName::Elif => "#elif",
        DirectiveName::Elifdef => "#elifdef",
        DirectiveName::Elifndef => "#elifndef",
        _ => "#if",
    }
}

#[derive(Debug, Clone)]
pub(super) struct Directive {
    pub(super) name: DirectiveName,
    pub(super) arguments: Vec<Span<Token>>,
    pub(super) name_loc: Loc,
    pub(super) loc: Loc,
}

impl Directive {
    pub(super) fn arguments_loc(&self) -> Loc {
        if self.arguments.is_empty() {
            self.name_loc
        } else {
            Span::cover((), &self.arguments).spelling
        }
    }

    pub(super) fn end_loc(&self) -> Loc {
        Loc::new(self.loc.file, self.loc.offset + self.loc.length, 0)
    }
}

#[derive(Debug, Clone)]
pub(super) enum Item {
    Comment(Span<String>),
    Text(Vec<Span<Token>>),
    Directive(Directive),
    Conditional(IfSection),
}

#[derive(Debug, Clone)]
pub(super) struct Branch {
    pub(super) directive: Directive,
    pub(super) body: Vec<Item>,
}

#[derive(Debug, Clone)]
pub(super) struct IfSection {
    pub(super) branches: Vec<Branch>,
}

#[derive(Default)]
struct LogicalLine {
    comments: Vec<Span<String>>,
    tokens: Vec<Span<Token>>,
}

pub(super) fn identifier(src: &str, token: &Span<Token>) -> Option<String> {
    match &token.value {
        Token::Ident(name) => Some(name.to_string()),
        Token::Keyword(_) | Token::Sizeof | Token::Alignof => src
            .get(token.spelling.offset..token.spelling.offset + token.spelling.length)
            .map(str::to_string),
        _ => None,
    }
}

pub(super) fn parse(src: &str, tokens: Vec<Span<Token>>) -> Result<Vec<Item>, PPFailure> {
    let mut parser = GroupParser {
        src,
        lines: logical_lines(tokens).into_iter(),
    };
    let mut items = Vec::new();
    match parser.group(&mut items)? {
        Some(stray) => Err(PPFailure::at(stray.loc, PPErrorKind::UnexpectedConditional)),
        None => Ok(items),
    }
}

fn logical_lines(tokens: Vec<Span<Token>>) -> Vec<LogicalLine> {
    let mut lines = Vec::new();
    let mut current = LogicalLine::default();
    for token in tokens {
        match token.value {
            Token::Newline => {
                if !current.comments.is_empty() || !current.tokens.is_empty() {
                    lines.push(std::mem::take(&mut current));
                }
            }
            Token::Comment(text) => {
                current
                    .comments
                    .push(Span::new(text.to_string(), token.spelling, token.expansion))
            }
            _ => current.tokens.push(token),
        }
    }
    if !current.comments.is_empty() || !current.tokens.is_empty() {
        lines.push(current);
    }
    merge_open_lines(lines)
}

fn is_directive_line(line: &LogicalLine) -> bool {
    line.tokens
        .first()
        .is_some_and(|token| token.value == Token::Hash)
}

fn paren_depth(line: &LogicalLine) -> i32 {
    line.tokens
        .iter()
        .fold(0i32, |depth, token| match token.value {
            Token::LParen | Token::LBracket => depth + 1,
            Token::RParen | Token::RBracket => depth - 1,
            _ => depth,
        })
}

fn merge_open_lines(lines: Vec<LogicalLine>) -> Vec<LogicalLine> {
    let mut merged = Vec::with_capacity(lines.len());
    let mut lines = lines.into_iter();
    while let Some(mut line) = lines.next() {
        if is_directive_line(&line) {
            merged.push(line);
            continue;
        }
        let mut depth = paren_depth(&line);
        let mut deferred_comments = Vec::new();
        while depth > 0 {
            let Some(next) = lines.next() else { break };
            if is_directive_line(&next) {
                merged.push(line);
                merged.push(LogicalLine {
                    comments: deferred_comments,
                    tokens: Vec::new(),
                });
                line = next;
                depth = 0;
                deferred_comments = Vec::new();
                break;
            }
            depth += paren_depth(&next);
            deferred_comments.extend(next.comments);
            line.tokens.extend(next.tokens);
        }
        merged.push(line);
        if !deferred_comments.is_empty() {
            merged.push(LogicalLine {
                comments: deferred_comments,
                tokens: Vec::new(),
            });
        }
    }
    merged
}

struct GroupParser<'a> {
    src: &'a str,
    lines: std::vec::IntoIter<LogicalLine>,
}

impl GroupParser<'_> {
    fn group(&mut self, items: &mut Vec<Item>) -> Result<Option<Directive>, PPFailure> {
        while let Some(line) = self.lines.next() {
            items.extend(line.comments.into_iter().map(Item::Comment));
            if line
                .tokens
                .first()
                .is_none_or(|token| token.value != Token::Hash)
            {
                if !line.tokens.is_empty() {
                    items.push(Item::Text(line.tokens));
                }
                continue;
            }
            let directive = self.directive(line.tokens);
            match directive.name {
                DirectiveName::Elif
                | DirectiveName::Elifdef
                | DirectiveName::Elifndef
                | DirectiveName::Else
                | DirectiveName::Endif => {
                    return Ok(Some(directive));
                }
                DirectiveName::If | DirectiveName::Ifdef | DirectiveName::Ifndef => {
                    let section = self.if_section(directive)?;
                    items.push(Item::Conditional(section));
                }
                _ => items.push(Item::Directive(directive)),
            }
        }
        Ok(None)
    }

    fn directive(&self, tokens: Vec<Span<Token>>) -> Directive {
        let loc = Span::cover((), &tokens).spelling;
        let mut tokens = tokens.into_iter().skip(1);
        let Some(name) = tokens.next() else {
            return Directive {
                name: DirectiveName::Null,
                arguments: Vec::new(),
                name_loc: loc,
                loc,
            };
        };
        Directive {
            name: match &name.value {
                Token::IntLit(_) => DirectiveName::LineMarker,
                _ => identifier(self.src, &name).map_or(DirectiveName::Unknown, |name| {
                    DirectiveName::from_spelling(&name)
                }),
            },
            arguments: tokens.collect(),
            name_loc: name.spelling,
            loc,
        }
    }

    fn if_section(&mut self, opening: Directive) -> Result<IfSection, PPFailure> {
        let opening_loc = opening.loc;
        let mut branches = Vec::new();
        let mut directive = opening;
        loop {
            let mut body = Vec::new();
            let terminator = self.group(&mut body)?;
            let in_else = directive.name == DirectiveName::Else;
            branches.push(Branch { directive, body });
            let Some(next) = terminator else {
                return Err(PPFailure::at(
                    opening_loc,
                    PPErrorKind::UnterminatedConditional,
                ));
            };
            match next.name {
                DirectiveName::Endif => {
                    return Ok(IfSection { branches });
                }
                DirectiveName::Else if in_else => {
                    return Err(PPFailure::at(next.loc, PPErrorKind::MultipleElse));
                }
                DirectiveName::Elif | DirectiveName::Elifdef | DirectiveName::Elifndef
                    if in_else =>
                {
                    return Err(PPFailure::at(
                        next.loc,
                        PPErrorKind::ElifAfterElse(directive_spelling(next.name)),
                    ));
                }
                _ => directive = next,
            }
        }
    }
}
