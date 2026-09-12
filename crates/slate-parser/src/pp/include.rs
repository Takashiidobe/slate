use super::error::{PPErrorKind, PPFailure};
use super::{PPNode, Preprocessor};
use crate::ast::{Condition, FileId, HeaderKind, Loc};
use crate::files::display_path;
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

pub(super) fn parse_include_directive(trimmed: &str) -> Option<IncludeDirective> {
    if let Some(rest) = trimmed.strip_prefix("#include_next") {
        let rest = rest.trim();
        let name = rest
            .strip_prefix('<')
            .and_then(|s| s.strip_suffix('>'))
            .or_else(|| rest.strip_prefix('"').and_then(|s| s.strip_suffix('"')))?;
        return Some(IncludeDirective::Next(name.to_string()));
    }
    let rest = trimmed.strip_prefix("#include")?.trim();
    if let Some(inner) = rest.strip_prefix('<').and_then(|s| s.strip_suffix('>')) {
        Some(IncludeDirective::Angled(inner.to_string()))
    } else {
        rest.strip_prefix('"')
            .and_then(|s| s.strip_suffix('"'))
            .map(|inner| IncludeDirective::Quoted(inner.to_string()))
    }
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
        active: &Condition,
    ) -> Result<Vec<PPNode>, PPFailure> {
        let (resolved, kind) = self
            .resolve_include(include, directive.file)
            .ok_or_else(|| {
                PPFailure::at(directive, PPErrorKind::HeaderNotFound(include.to_string()))
            })?;
        if self.open_stack.contains(&resolved) {
            return Err(PPFailure::at(
                directive,
                PPErrorKind::IncludeCycle(display_path(&resolved)),
            ));
        }
        let src = read_source(&resolved).map_err(|kind| PPFailure::at(directive, kind))?;
        let file = self.files.intern(resolved.clone(), kind);
        self.open_stack.push(resolved);
        let nodes = self.parse_source(&src, file, active.clone())?;
        self.open_stack.pop();
        Ok(nodes)
    }

    fn resolve_include(
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
                }),
        }
    }
}
