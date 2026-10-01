use super::error::{PPErrorKind, PPFailure};
use super::syntax::{Directive, DirectiveName};
use super::{PPNode, Preprocessor};
use crate::ast::{FileId, HeaderKind, Loc, Span};
use crate::files::{decode_source_bytes, display_path};
use crate::lexer::Token;
use std::fmt;
use std::path::{Path, PathBuf};

// clang and gcc both stop at 200; they detect cycles only by hitting this limit
const MAX_INCLUDE_DEPTH: usize = 200;

pub(super) enum IncludeDirective {
    Angled(String),
    Quoted(String),
    Next(String, bool),
}

impl fmt::Display for IncludeDirective {
    fn fmt(&self, formatter: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            IncludeDirective::Angled(name) | IncludeDirective::Next(name, true) => {
                write!(formatter, "<{name}>")
            }
            IncludeDirective::Quoted(name) | IncludeDirective::Next(name, false) => {
                write!(formatter, "\"{name}\"")
            }
        }
    }
}

// a header name spelled out before expansion; tokens after it are ignored, as in clang and gcc
pub(super) fn spells_header_name(arguments: &[Span<Token>]) -> bool {
    match arguments.first().map(|token| &token.value) {
        Some(Token::StringLit(_)) => true,
        Some(Token::Less) => arguments.iter().any(|token| token.value == Token::Greater),
        _ => false,
    }
}

pub(super) fn include_target(
    src: &str,
    directive: &Directive,
) -> Result<IncludeDirective, PPFailure> {
    let arguments = directive.arguments.as_slice();
    let close = arguments
        .iter()
        .position(|token| token.value == Token::Greater);
    let target = match (arguments.first().map(|token| &token.value), close) {
        (Some(Token::StringLit(name)), _) => Some((name.to_string(), false)),
        (Some(Token::Less), Some(close_at)) => {
            let (open, close) = (&arguments[0], &arguments[close_at]);
            let name = if open.macro_origin.is_none() && close.macro_origin.is_none() {
                src.get(open.spelling.offset + open.spelling.length..close.spelling.offset)
                    .map(str::to_string)
            } else {
                Some(joined_spelling(&arguments[1..close_at]))
            };
            name.map(|name| (name, true))
        }
        _ => None,
    };
    let Some((name, angled)) = target else {
        return Err(PPFailure::at(
            directive
                .arguments
                .first()
                .map_or(directive.name_loc, |token| token.spelling),
            PPErrorKind::ExpectedHeaderName,
        ));
    };
    Ok(match (directive.name, angled) {
        (DirectiveName::IncludeNext, _) => IncludeDirective::Next(name, angled),
        (_, true) => IncludeDirective::Angled(name),
        (_, false) => IncludeDirective::Quoted(name),
    })
}

fn joined_spelling(tokens: &[Span<Token>]) -> String {
    let mut name = String::new();
    for (index, token) in tokens.iter().enumerate() {
        if index > 0 && token.leading_space {
            name.push(' ');
        }
        name.push_str(&String::from(&token.value));
    }
    name
}

pub(super) fn read_source(path: &Path) -> Result<String, PPErrorKind> {
    std::fs::read(path)
        .map(|bytes| decode_source_bytes(&bytes))
        .map_err(|error| PPErrorKind::ReadFailed {
            path: display_path(path),
            message: error.to_string(),
        })
}

impl Preprocessor<'_> {
    pub(super) fn resolve_and_parse_include(
        &mut self,
        include: &IncludeDirective,
        directive: Loc,
    ) -> Result<Vec<PPNode>, PPFailure> {
        let (resolved, kind) = self
            .resolve_include(include, directive.file)
            .ok_or_else(|| {
                PPFailure::at(directive, PPErrorKind::HeaderNotFound(include.to_string()))
            })?;
        let once_key = resolved.canonicalize().unwrap_or_else(|_| resolved.clone());
        if self.pragma_once.contains(&once_key) {
            return Ok(Vec::new());
        }
        let file = self.files.intern(resolved.clone(), kind);
        if self
            .include_guards
            .get(&file)
            .is_some_and(|guard| self.is_defined(guard))
        {
            return Ok(Vec::new());
        }
        if self.open_stack.len() >= MAX_INCLUDE_DEPTH {
            return Err(PPFailure::at(directive, PPErrorKind::IncludeTooDeep));
        }
        let src = read_source(&resolved).map_err(|kind| PPFailure::at(directive, kind))?;
        self.open_stack.push(once_key);
        let enclosing_system_header = self.outermost_system_header;
        if kind == HeaderKind::System {
            self.outermost_system_header.get_or_insert(file);
        }
        let nodes = self.parse_source(&src, file);
        self.outermost_system_header = enclosing_system_header;
        self.open_stack.pop();
        nodes
    }

    pub(super) fn resolve_include(
        &self,
        include: &IncludeDirective,
        from: FileId,
    ) -> Option<(PathBuf, HeaderKind)> {
        let find_in = |dirs: &[PathBuf], name: &str| {
            dirs.iter()
                .map(|dir| dir.join(name))
                .find(|candidate| candidate.is_file())
        };
        match include {
            IncludeDirective::Angled(name) => find_in(&self.search.user, name)
                .map(|path| (path, HeaderKind::User))
                .or_else(|| {
                    find_in(&self.search.system, name).map(|path| (path, HeaderKind::System))
                }),
            IncludeDirective::Next(name, angled) => {
                let directories = self
                    .search
                    .quote
                    .iter()
                    .filter(|_| !angled)
                    .map(|path| (path, HeaderKind::User))
                    .chain(self.search.user.iter().map(|path| (path, HeaderKind::User)))
                    .chain(
                        self.search
                            .system
                            .iter()
                            .map(|path| (path, HeaderKind::System)),
                    )
                    .collect::<Vec<_>>();
                let current_dir = self.files.path(from).parent();
                let start = current_dir
                    .and_then(|dir| {
                        directories
                            .iter()
                            .position(|(candidate, _)| *candidate == dir)
                    })
                    .map_or(0, |index| index + 1);
                directories[start..].iter().find_map(|(dir, kind)| {
                    let path = dir.join(name);
                    path.is_file().then_some((path, *kind))
                })
            }
            IncludeDirective::Quoted(name) => self
                .files
                .path(from)
                .parent()
                .map(|dir| dir.join(name))
                .filter(|candidate| candidate.is_file())
                .map(|path| (path, self.files.kind(from)))
                .or_else(|| find_in(&self.search.quote, name).map(|path| (path, HeaderKind::User)))
                .or_else(|| find_in(&self.search.user, name).map(|path| (path, HeaderKind::User)))
                .or_else(|| {
                    find_in(&self.search.system, name).map(|path| (path, HeaderKind::System))
                })
                .or_else(|| {
                    let candidate = PathBuf::from(name);
                    candidate.is_file().then_some((candidate, HeaderKind::User))
                }),
        }
    }
}
