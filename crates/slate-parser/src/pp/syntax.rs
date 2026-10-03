use crate::ast::{Loc, Span};
use crate::lexer::{LineToken, Token};

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

pub(super) fn identifier(src: &str, token: &Span<Token>) -> Option<String> {
    match &token.value {
        Token::Ident(name) => Some(name.to_string()),
        Token::Keyword(_) | Token::Sizeof | Token::Alignof | Token::Countof => src
            .get(token.spelling.offset..token.spelling.offset + token.spelling.length)
            .map(str::to_string),
        _ => None,
    }
}

pub(super) enum Line {
    Directive(Directive, Vec<Span<String>>),
    Text(PhysicalLine),
}

pub(super) struct PhysicalLine {
    pub(super) comments: Vec<Span<String>>,
    pub(super) tokens: Vec<Span<Token>>,
}

pub(super) struct TokenSource<'a> {
    src: &'a str,
    tokens: Vec<LineToken>,
    position: usize,
}

impl<'a> TokenSource<'a> {
    pub(super) fn new(src: &'a str, tokens: Vec<LineToken>) -> Self {
        Self {
            src,
            tokens,
            position: 0,
        }
    }

    pub(super) fn next_line(&mut self) -> Option<Line> {
        let line = self.physical_line()?;
        if starts_directive(&line.tokens) {
            return Some(Line::Directive(self.directive(line.tokens), line.comments));
        }
        Some(Line::Text(line))
    }

    pub(super) fn peek_token(&self) -> Option<&Span<Token>> {
        self.tokens[self.position.min(self.tokens.len())..]
            .iter()
            .map(|token| &token.token)
            .find(|token| !matches!(token.value, Token::Comment(_)))
    }

    pub(super) fn next_directive(&mut self) -> Option<Directive> {
        while self.position < self.tokens.len() {
            if self.at_directive() {
                let line = self.physical_line()?;
                return Some(self.directive(line.tokens));
            }
            self.position += 1;
            while self
                .tokens
                .get(self.position)
                .is_some_and(|token| !token.at_line_start)
            {
                self.position += 1;
            }
        }
        None
    }

    fn at_directive(&self) -> bool {
        self.tokens[self.position.min(self.tokens.len())..]
            .iter()
            .enumerate()
            .take_while(|(index, token)| *index == 0 || !token.at_line_start)
            .find(|(_, token)| !matches!(token.token.value, Token::Comment(_)))
            .is_some_and(|(_, token)| token.token.value == Token::Hash)
    }

    fn physical_line(&mut self) -> Option<PhysicalLine> {
        if self.position >= self.tokens.len() {
            return None;
        }
        let mut comments = Vec::new();
        let mut tokens = Vec::new();
        loop {
            let token = self.tokens[self.position].token.clone();
            self.position += 1;
            match token.value {
                Token::Comment(text) => {
                    comments.push(Span::new(text.to_string(), token.spelling, token.expansion))
                }
                _ => tokens.push(token),
            }
            if self
                .tokens
                .get(self.position)
                .is_none_or(|token| token.at_line_start)
            {
                return Some(PhysicalLine { comments, tokens });
            }
        }
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
}

fn starts_directive(tokens: &[Span<Token>]) -> bool {
    tokens
        .first()
        .is_some_and(|token| token.value == Token::Hash)
}
