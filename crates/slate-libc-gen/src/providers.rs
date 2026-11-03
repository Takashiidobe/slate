use anyhow::{Context, Result, bail};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;
use std::process::Command;

#[derive(Default)]
pub struct Inventory {
    paths: BTreeMap<String, BTreeSet<String>>,
    pub headers: BTreeSet<String>,
}

impl Inventory {
    pub fn collect(target: &str, work: &Path) -> Result<Self> {
        if !target.contains("-apple-") {
            return Ok(Self::default());
        }
        let directory = work.join(format!("mach2-doc-{target}"));
        std::fs::create_dir_all(&directory)?;
        std::fs::write(
            directory.join("Cargo.toml"),
            r#"[package]
name = "mach2-inventory"
version = "0.0.0"
edition = "2024"
[lib]
path = "lib.rs"
[dependencies]
mach2 = { path = "../mach2" }
[workspace]
"#,
        )?;
        std::fs::write(directory.join("lib.rs"), "")?;
        let output = Command::new("cargo")
            .args([
                "rustdoc",
                "--lib",
                "-p",
                "mach2",
                "--target",
                target,
                "--manifest-path",
            ])
            .arg(directory.join("Cargo.toml"))
            .env("CARGO_TARGET_DIR", directory.join("target"))
            .args(["--", "-Z", "unstable-options", "--output-format", "json"])
            .output()
            .context("document mach2")?;
        if !output.status.success() {
            bail!(
                "{target}: mach2 rustdoc failed: {}",
                String::from_utf8_lossy(&output.stderr)
            );
        }
        let document: serde_json::Value = serde_json::from_slice(&std::fs::read(
            directory.join("target").join(target).join("doc/mach2.json"),
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
                "slate_support::mach::{}",
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
            None => Some(format!("libc::{}", crate::rust_side::ident(name))),
        }
    }
}
