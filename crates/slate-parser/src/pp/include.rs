use super::error::{PPErrorKind, PPFailure};
use super::syntax::{Directive, DirectiveName};
use super::{PPNode, Preprocessor};
use crate::ast::{FileId, HeaderKind, Loc, Span};
use crate::files::display_path;
use crate::lexer::Token;
use std::fmt;
use std::path::{Path, PathBuf};

pub(super) enum IncludeDirective {
    Angled(String),
    Quoted(String),
    Next(String),
}

impl fmt::Display for IncludeDirective {
    fn fmt(&self, formatter: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            IncludeDirective::Angled(name) | IncludeDirective::Next(name) => {
                write!(formatter, "<{name}>")
            }
            IncludeDirective::Quoted(name) => write!(formatter, "\"{name}\""),
        }
    }
}

pub(super) fn include_target(
    src: &str,
    directive: &Directive,
) -> Result<IncludeDirective, PPFailure> {
    let target = match directive.arguments.as_slice() {
        [
            Span {
                value: Token::StringLit(name),
                ..
            },
        ] => Some((name.clone(), false)),
        [open, .., close] if open.value == Token::Less && close.value == Token::Greater => src
            .get(open.spelling.offset + open.spelling.length..close.spelling.offset)
            .map(|name| (name.to_string(), true)),
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
        (DirectiveName::IncludeNext, _) => IncludeDirective::Next(name),
        (_, true) => IncludeDirective::Angled(name),
        (_, false) => IncludeDirective::Quoted(name),
    })
}

pub(super) fn read_source(path: &Path) -> Result<String, PPErrorKind> {
    std::fs::read_to_string(path).map_err(|error| PPErrorKind::ReadFailed {
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
        if self.open_stack.contains(&resolved) {
            return Err(PPFailure::at(
                directive,
                PPErrorKind::IncludeCycle(display_path(&resolved)),
            ));
        }
        let src = read_source(&resolved).map_err(|kind| PPFailure::at(directive, kind))?;
        let file = self.files.intern(resolved.clone(), kind);
        self.open_stack.push(resolved);
        let enclosing_header = self.outermost_header;
        self.outermost_header.get_or_insert(file);
        let nodes = self.parse_source(&src, file);
        self.outermost_header = enclosing_header;
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
            IncludeDirective::Angled(name) => {
                find_in(&self.search.system, name).map(|path| (path, HeaderKind::System))
            }
            IncludeDirective::Next(name) => {
                let current_dir = self.files.path(from).parent();
                let start = current_dir
                    .and_then(|dir| {
                        self.search
                            .system
                            .iter()
                            .position(|candidate| candidate == dir)
                    })
                    .map_or(0, |index| index + 1);
                find_in(&self.search.system[start..], name).map(|path| (path, HeaderKind::System))
            }
            IncludeDirective::Quoted(name) => self
                .files
                .path(from)
                .parent()
                .map(|dir| dir.join(name))
                .filter(|candidate| candidate.is_file())
                .or_else(|| find_in(&self.search.user, name))
                .map(|path| (path, HeaderKind::User))
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
