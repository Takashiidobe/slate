use super::{PPError, PPNode, Preprocessor};
use crate::ast::{Condition, FileId, HeaderKind};
use crate::files::display_path;
use std::path::PathBuf;

pub(super) enum IncludeDirective {
    Angled(String),
    Quoted(String),
    Next(String),
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

impl Preprocessor<'_> {
    pub(super) fn resolve_and_parse_include(
        &mut self,
        include: &IncludeDirective,
        from: FileId,
        active: &Condition,
    ) -> Result<Vec<PPNode>, PPError> {
        let (resolved, kind) = self.resolve_include(include, from);
        assert!(
            !self.open_stack.contains(&resolved),
            "include cycle detected: {}",
            resolved.display()
        );

        let src = std::fs::read_to_string(&resolved)
            .unwrap_or_else(|e| panic!("failed to read {}: {e}", resolved.display()));
        let file = self.files.intern(resolved.clone(), kind);
        self.open_stack.push(resolved.clone());
        let nodes = self.parse_source(&display_path(&resolved), &src, file, active.clone())?;
        self.open_stack.pop();
        Ok(nodes)
    }

    pub(super) fn resolve_include(
        &self,
        include: &IncludeDirective,
        from: FileId,
    ) -> (PathBuf, HeaderKind) {
        match include {
            IncludeDirective::Angled(name) => self
                .search
                .system
                .iter()
                .map(|dir| dir.join(name))
                .find(|candidate| candidate.is_file())
                .map(|path| (path, HeaderKind::System))
                .unwrap_or_else(|| panic!("system header not found in search path: <{name}>")),
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
                self.search.system[start..]
                    .iter()
                    .map(|dir| dir.join(name))
                    .find(|candidate| candidate.is_file())
                    .map(|path| (path, HeaderKind::System))
                    .unwrap_or_else(|| {
                        panic!("system header not found via #include_next in search path: <{name}>")
                    })
            }
            IncludeDirective::Quoted(name) => {
                let same_dir = self.files.path(from).parent().map(|dir| dir.join(name));
                same_dir
                    .filter(|c| c.is_file())
                    .map(|path| (path, HeaderKind::User))
                    .or_else(|| {
                        self.search
                            .user
                            .iter()
                            .map(|dir| dir.join(name))
                            .find(|c| c.is_file())
                            .map(|path| (path, HeaderKind::User))
                    })
                    .or_else(|| {
                        self.search
                            .system
                            .iter()
                            .map(|dir| dir.join(name))
                            .find(|c| c.is_file())
                            .map(|path| (path, HeaderKind::System))
                    })
                    .unwrap_or_else(|| panic!("header not found in search path: \"{name}\""))
            }
        }
    }
}
