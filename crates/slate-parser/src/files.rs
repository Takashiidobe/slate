use crate::ast::{FileId, HeaderKind};
use std::path::{Path, PathBuf};

pub fn display_path(path: &Path) -> String {
    path.strip_prefix(env!("CARGO_MANIFEST_DIR")).map_or_else(
        |_| path.display().to_string(),
        |path| path.display().to_string(),
    )
}

#[derive(Debug, Default, Clone)]
pub struct SearchPaths {
    pub user: Vec<PathBuf>,
    pub system: Vec<PathBuf>,
}

#[derive(Debug, Default, Clone)]
pub struct Files {
    entries: Vec<(PathBuf, HeaderKind)>,
}

impl Files {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn paths(&self) -> impl Iterator<Item = &Path> {
        self.entries.iter().map(|(path, _)| path.as_path())
    }

    pub fn intern(&mut self, path: PathBuf, kind: HeaderKind) -> FileId {
        if let Some(pos) = self.entries.iter().position(|(p, _)| *p == path) {
            return FileId(pos as u32);
        }
        self.entries.push((path, kind));
        FileId((self.entries.len() - 1) as u32)
    }

    pub fn path(&self, id: FileId) -> &Path {
        &self.entries[id.0 as usize].0
    }

    pub fn get_path(&self, id: FileId) -> Option<&Path> {
        self.entries
            .get(id.0 as usize)
            .map(|(path, _)| path.as_path())
    }

    pub fn kind(&self, id: FileId) -> HeaderKind {
        self.entries[id.0 as usize].1
    }
}
