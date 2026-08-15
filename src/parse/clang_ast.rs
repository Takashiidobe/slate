use clang_ast::{SourceLocation, SourceRange};
use serde::Deserialize;
use std::path::{Path, PathBuf};
use std::process::{Command, ExitStatus};
use thiserror::Error;

#[derive(Deserialize, Debug, Default)]
pub struct Clang {
    #[serde(default)]
    pub kind: Option<String>,
    pub name: Option<String>,
    #[serde(rename = "tagUsed")]
    pub tag_used: Option<String>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    pub loc: Option<SourceLocation>,
    pub range: Option<SourceRange>,
}

#[derive(Deserialize, Debug)]
pub struct QualType {
    #[serde(rename = "qualType")]
    pub qual_type: String,
}

pub type Node = clang_ast::Node<Clang>;

#[derive(Debug, Error)]
pub enum ClangAstError {
    #[error("configure target for clang AST dump of {path}: {source}")]
    Target {
        path: PathBuf,
        #[source]
        source: crate::cir::TargetError,
    },
    #[error("spawn {clang} for clang AST dump of {path}: {source}")]
    Spawn {
        clang: String,
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("clang AST dump failed for {path} with {status}:\n{stderr}")]
    ClangFailed {
        path: PathBuf,
        status: ExitStatus,
        stderr: String,
    },
    #[error("parse clang AST JSON for {path}: {source}")]
    ParseJson {
        path: PathBuf,
        #[source]
        source: serde_json::Error,
    },
}

fn clang() -> String {
    std::env::var("SLATE_CC").unwrap_or_else(|_| "clang".to_string())
}

pub fn dump_json(src: &Path, extra_args: &[String]) -> Result<String, ClangAstError> {
    let clang = clang();
    let target_args = crate::cir::emit::target_args().map_err(|source| ClangAstError::Target {
        path: src.to_path_buf(),
        source,
    })?;
    let out = Command::new(&clang)
        .args(["-std=gnu23", "-Xclang", "-ast-dump=json", "-fsyntax-only"])
        .args(target_args)
        .args(extra_args)
        .arg(src)
        .output()
        .map_err(|source| ClangAstError::Spawn {
            clang: clang.clone(),
            path: src.to_path_buf(),
            source,
        })?;
    if !out.status.success() {
        return Err(ClangAstError::ClangFailed {
            path: src.to_path_buf(),
            status: out.status,
            stderr: String::from_utf8_lossy(&out.stderr).into_owned(),
        });
    }
    Ok(String::from_utf8_lossy(&out.stdout).into_owned())
}

pub fn parse_file(src: &Path, extra_args: &[String]) -> Result<Node, ClangAstError> {
    let json = dump_json(src, extra_args)?;
    serde_json::from_str(&json).map_err(|source| ClangAstError::ParseJson {
        path: src.to_path_buf(),
        source,
    })
}

pub fn dump_tree(node: &Node, depth: usize, out: &mut String) {
    use std::fmt::Write;

    let indent = "  ".repeat(depth);
    write!(
        out,
        "{indent}[{}] {}",
        node.id,
        node.kind.kind.as_deref().unwrap_or("<unknown>")
    )
    .unwrap();
    if let Some(name) = &node.kind.name {
        write!(out, " name={name:?}").unwrap();
    }
    if let Some(tag_used) = &node.kind.tag_used {
        write!(out, " tagUsed={tag_used:?}").unwrap();
    }
    if let Some(ty) = &node.kind.qual_type {
        write!(out, " type={:?}", ty.qual_type).unwrap();
    }
    if let Some(loc) = &node.kind.loc
        && let Some(spelling) = &loc.spelling_loc
    {
        write!(
            out,
            " @ {}:{}:{}",
            spelling.file, spelling.line, spelling.col
        )
        .unwrap();
    }
    out.push('\n');
    for child in &node.inner {
        dump_tree(child, depth + 1, out);
    }
}
