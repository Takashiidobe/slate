use crate::ast::{FileId, HeaderKind};
use std::path::{Path, PathBuf};

pub fn display_path(path: &Path) -> String {
    path.strip_prefix(env!("CARGO_MANIFEST_DIR")).map_or_else(
        |_| path.display().to_string(),
        |path| path.display().to_string(),
    )
}

const RAW_BYTE_ESCAPE_BASE: u32 = 0x10_FF00;

pub fn decode_source_bytes(bytes: &[u8]) -> String {
    let mut result = String::with_capacity(bytes.len());
    let mut rest = bytes;
    loop {
        match std::str::from_utf8(rest) {
            Ok(valid) => {
                result.push_str(valid);
                break;
            }
            Err(error) => {
                let valid_len = error.valid_up_to();
                result.push_str(std::str::from_utf8(&rest[..valid_len]).unwrap());
                let bad_len = error.error_len().unwrap_or(rest.len() - valid_len);
                for &byte in &rest[valid_len..valid_len + bad_len] {
                    result.push(char::from_u32(RAW_BYTE_ESCAPE_BASE + u32::from(byte)).unwrap());
                }
                rest = &rest[valid_len + bad_len..];
            }
        }
    }
    result
}

pub fn raw_byte_for_char(c: char) -> Option<u8> {
    let code = c as u32;
    (RAW_BYTE_ESCAPE_BASE..=RAW_BYTE_ESCAPE_BASE + 0xFF)
        .contains(&code)
        .then(|| (code - RAW_BYTE_ESCAPE_BASE) as u8)
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
