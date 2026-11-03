use anyhow::{Context, Result, bail};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;
use std::process::Command;

#[derive(Default)]
pub struct Inventory {
    paths: BTreeMap<String, BTreeSet<String>>,
    pub headers: BTreeSet<String>,
    system_functions: BTreeSet<String>,
    item_paths: BTreeMap<String, String>,
    path_items: BTreeMap<String, Vec<String>>,
    document: Option<serde_json::Value>,
}

impl Inventory {
    pub fn collect(target: &str, work: &Path) -> Result<Self> {
        let (package, namespace) = if target.contains("-apple-") {
            ("mach2", "mach")
        } else if target.ends_with("-windows-msvc") {
            ("windows-sys", "windows")
        } else {
            return Ok(Self::default());
        };
        let directory = work.join(format!("{package}-doc-{target}"));
        std::fs::create_dir_all(&directory)?;
        let mut features: Vec<String> = if namespace == "windows" {
            serde_json::from_slice(&std::fs::read(work.join("windows-features.json"))?)?
        } else {
            Vec::new()
        };
        if namespace == "windows" {
            features.push("docs".into());
        }
        std::fs::write(
            directory.join("Cargo.toml"),
            format!(
                "[package]\nname = \"binding-inventory\"\nversion = \"0.0.0\"\nedition = \"2024\"\n[lib]\npath = \"lib.rs\"\n[dependencies]\n{package} = {{ path = \"../{package}\", features = {features:?} }}\n[workspace]\n"
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
        if namespace == "windows" {
            inventory.headers.insert("windows.h".into());
        }
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
            if namespace == "windows" && !matches!(path.get(1), Some(&"Win32" | &"core")) {
                continue;
            }
            let name = path.last().context("empty rustdoc path")?;
            let path = format!(
                "slate_support::{namespace}::{}",
                path[1..]
                    .iter()
                    .map(|name| crate::rust_side::ident(name))
                    .collect::<Vec<_>>()
                    .join("::")
            );
            if document["index"][id]["inner"]["function"]["header"]["abi"]
                .get("System")
                .is_some()
            {
                inventory.system_functions.insert((*name).into());
            }
            inventory.item_paths.insert(id.clone(), path.clone());
            inventory
                .path_items
                .entry(path.clone())
                .or_default()
                .push(id.clone());
            inventory
                .paths
                .entry((*name).into())
                .or_default()
                .insert(path);
        }
        if namespace == "windows" {
            inventory.document = Some(document);
        }
        Ok(inventory)
    }

    pub fn system_abi(&self, name: &str) -> bool {
        self.system_functions.contains(name)
    }

    pub fn features(&self, path: &str) -> BTreeSet<String> {
        let mut features = BTreeSet::new();
        let Some(document) = &self.document else {
            return features;
        };
        let mut pending = self.path_items.get(path).cloned().unwrap_or_default();
        let mut visited = BTreeSet::new();
        while let Some(id) = pending.pop() {
            if !visited.insert(id.clone()) {
                continue;
            }
            if let Some(path) = self
                .item_paths
                .get(&id)
                .and_then(|path| path.strip_prefix("slate_support::windows::Win32::"))
                && let Some((module, _)) = path.rsplit_once("::")
            {
                features.insert(format!("Win32_{}", module.replace("::", "_")));
            }
            referenced_items(&document["index"][&id]["inner"], &mut pending);
        }
        features
    }

    pub fn path(&self, name: &str) -> Option<String> {
        match self.paths.get(name) {
            Some(paths) if paths.len() == 1 => paths.first().cloned(),
            Some(_) => None,
            None => Some(format!("libc::{}", crate::rust_side::ident(name))),
        }
    }
}

fn referenced_items(value: &serde_json::Value, out: &mut Vec<String>) {
    match value {
        serde_json::Value::Object(fields) => {
            for (key, value) in fields {
                if key == "id" {
                    if let Some(id) = value.as_u64() {
                        out.push(id.to_string());
                    }
                } else if matches!(key.as_str(), "fields" | "variants") {
                    for id in value
                        .as_array()
                        .into_iter()
                        .flatten()
                        .filter_map(serde_json::Value::as_u64)
                    {
                        out.push(id.to_string());
                    }
                }
                referenced_items(value, out);
            }
        }
        serde_json::Value::Array(values) => {
            for value in values {
                referenced_items(value, out);
            }
        }
        _ => {}
    }
}

pub fn with_windows_features(manifest: &str, features: &[String]) -> String {
    manifest
        .lines()
        .map(|line| {
            if line.starts_with("windows-sys = ") {
                format!("windows-sys = {{ path = \"../windows-sys\", features = {features:?} }}")
            } else {
                line.to_string()
            }
        })
        .collect::<Vec<_>>()
        .join("\n")
        + "\n"
}
