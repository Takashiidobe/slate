use crate::ast::{FileId, HeaderKind};
use std::path::{Path, PathBuf};

#[derive(Debug, Default, Clone)]
pub struct SearchPaths {
    pub user: Vec<PathBuf>,
    pub system: Vec<PathBuf>,
}

#[derive(Debug, Default)]
pub struct Files {
    entries: Vec<(PathBuf, HeaderKind)>,
}

impl Files {
    pub fn new() -> Self {
        Self::default()
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

    pub fn kind(&self, id: FileId) -> HeaderKind {
        self.entries[id.0 as usize].1
    }
}
