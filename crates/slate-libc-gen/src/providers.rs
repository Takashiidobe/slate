use anyhow::{Context, Result, bail};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;
use std::process::Command;

const GLIBC32_FILE_OFFSET_FUNCTIONS: &[&str] = &[
    "fallocate",
    "fseeko",
    "ftello",
    "ftruncate",
    "lockf",
    "lseek",
    "mmap",
    "posix_fadvise",
    "posix_fallocate",
    "pread",
    "preadv",
    "pwrite",
    "pwritev",
    "readahead",
    "sendfile",
    "sync_file_range",
    "truncate",
];

#[derive(Default)]
pub struct Inventory {
    support_functions: BTreeSet<String>,
    paths: BTreeMap<String, BTreeSet<String>>,
    pub headers: BTreeSet<String>,
}

impl Inventory {
    pub fn collect(target: &str, work: &Path) -> Result<Self> {
        let (package, namespace) = if target.contains("-apple-") {
            ("mach2", "mach")
        } else {
            let mut inventory = Self::default();
            if target.contains("-linux-gnu")
                && slate_parser::target_info::TargetInfo::for_triple(target)
                    .is_ok_and(|info| info.pointer_width == 32)
            {
                inventory.support_functions = GLIBC32_FILE_OFFSET_FUNCTIONS
                    .iter()
                    .map(|name| name.to_string())
                    .collect();
            }
            return Ok(inventory);
        };
        let directory = work.join(format!("{package}-doc-{target}"));
        std::fs::create_dir_all(&directory)?;
        std::fs::write(
            directory.join("Cargo.toml"),
            format!(
                "[package]\nname = \"binding-inventory\"\nversion = \"0.0.0\"\nedition = \"2024\"\n[lib]\npath = \"lib.rs\"\n[dependencies]\n{package} = {{ path = \"../{package}\" }}\n[workspace]\n"
            ),
        )?;
        std::fs::write(directory.join("lib.rs"), "")?;
        let output = Command::new("cargo")
            .args([
                "rustdoc",
                "--lib",
                "-p",
                package,
                "--target",
                target,
                "--manifest-path",
            ])
            .arg(directory.join("Cargo.toml"))
            .env("CARGO_TARGET_DIR", directory.join("target"))
            .args(["--", "-Z", "unstable-options", "--output-format", "json"])
            .output()
            .context("document binding provider")?;
        if !output.status.success() {
            bail!(
                "{target}: {package} rustdoc failed: {}",
                String::from_utf8_lossy(&output.stderr)
            );
        }
        let document: serde_json::Value = serde_json::from_slice(&std::fs::read(
            directory
                .join("target")
                .join(target)
                .join(format!("doc/{}.json", package.replace('-', "_"))),
        )?)?;
        let mut inventory = Self::default();
        let paths = document["paths"].as_object().context("rustdoc paths")?;
        for (id, item) in paths {
            if item["crate_id"] != 0 || document["index"][id]["visibility"] != "public" {
                continue;
            }
            if item["kind"] == "module" {
                if let Some(docs) = document["index"][id]["docs"].as_str() {
                    for header in docs.split('`').skip(1).step_by(2) {
                        let header = header
                            .strip_suffix(".defs")
                            .map_or_else(|| header.to_string(), |header| format!("{header}.h"));
                        if header.ends_with(".h") {
                            inventory.headers.insert(header);
                        }
                    }
                }
                continue;
            }
            if !matches!(
                item["kind"].as_str(),
                Some(
                    "function" | "struct" | "union" | "enum" | "type_alias" | "constant" | "static"
                )
            ) {
                continue;
            }
            let path = item["path"]
                .as_array()
                .context("rustdoc item path")?
                .iter()
                .map(|segment| segment.as_str().context("rustdoc path segment"))
                .collect::<Result<Vec<_>>>()?;
            let name = path.last().context("empty rustdoc path")?;
            let path = format!(
                "slate_support::{namespace}::{}",
                path[1..]
                    .iter()
                    .map(|name| crate::rust_side::ident(name))
                    .collect::<Vec<_>>()
                    .join("::")
            );
            inventory
                .paths
                .entry((*name).into())
                .or_default()
                .insert(path);
        }
        Ok(inventory)
    }

    pub fn path(&self, name: &str) -> Option<String> {
        match self.paths.get(name) {
            Some(paths) if paths.len() == 1 => paths.first().cloned(),
            Some(_) => None,
            None if self.support_functions.contains(name) => {
                Some(format!("slate_support::{}", crate::rust_side::ident(name)))
            }
            None => Some(format!("libc::{}", crate::rust_side::ident(name))),
        }
    }
}
