use anyhow::{Context, Result, bail};
use object::{Object, ObjectSection, ObjectSymbol};
use std::collections::{BTreeMap, HashMap, HashSet};
use std::path::{Path, PathBuf};
use std::process::Command;

#[derive(Clone, Debug)]
pub struct RustLayout {
    pub size: u64,
    pub align: u64,
    pub offsets: HashMap<String, u64>,
}

const KEYWORDS: &[&str] = &[
    "as", "break", "const", "continue", "crate", "else", "enum", "extern", "false", "fn", "for",
    "if", "impl", "in", "let", "loop", "match", "mod", "move", "mut", "pub", "ref", "return",
    "self", "Self", "static", "struct", "super", "trait", "true", "type", "unsafe", "use", "where",
    "while", "async", "await", "dyn", "abstract", "become", "box", "do", "final", "macro",
    "override", "priv", "typeof", "unsized", "virtual", "yield", "try", "gen",
];

pub fn ident(name: &str) -> String {
    if KEYWORDS.contains(&name) {
        format!("r#{name}")
    } else {
        name.to_string()
    }
}

pub fn prepare_libc(libc: &Path, work: &Path) -> Result<PathBuf> {
    let dir = work.join("libc");
    std::fs::create_dir_all(&dir)?;
    let libc = std::fs::canonicalize(libc).context("vendored libc directory")?;
    std::fs::copy(libc.join("Cargo.toml.template"), dir.join("Cargo.toml"))?;
    for entry in ["build.rs", "src"] {
        let link = dir.join(entry);
        let _ = std::fs::remove_file(&link);
        std::os::unix::fs::symlink(libc.join(entry), link)?;
    }
    for name in ["mach2", "slate-support"] {
        let destination = work.join(name);
        std::fs::create_dir_all(&destination)?;
        let source = Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("../slate/vendor")
            .join(name)
            .canonicalize()?;
        for entry in std::fs::read_dir(&source)? {
            let entry = entry?;
            let name = entry.file_name();
            if name == "src" {
                let link = destination.join("src");
                let _ = std::fs::remove_file(&link);
                std::os::unix::fs::symlink(entry.path(), link)?;
            } else if entry.file_type()?.is_file() {
                let name = if name == "Cargo.toml.template" {
                    "Cargo.toml".into()
                } else {
                    name
                };
                std::fs::copy(entry.path(), destination.join(name))?;
            }
        }
    }
    let support = work.join("slate-support/Cargo.toml");
    let manifest = std::fs::read_to_string(&support)?;
    std::fs::write(&support, format!("{manifest}\n[workspace]\n"))?;
    Ok(dir)
}

struct Probe {
    key: String,
    type_index: usize,
    line: String,
}

pub fn collect(
    libc_dir: &Path,
    work: &Path,
    target: &str,
    candidates: &[(String, Vec<String>)],
    providers: &crate::providers::Inventory,
) -> Result<BTreeMap<String, RustLayout>> {
    let dir = work.join(target);
    std::fs::create_dir_all(&dir)?;
    std::fs::write(
        dir.join("Cargo.toml"),
        format!(
            "[package]\nname = \"probe\"\nversion = \"0.0.0\"\nedition = \"2024\"\n\n\
             [lib]\npath = \"lib.rs\"\n\n\
             [dependencies]\nlibc = {{ path = {libc:?}, default-features = false }}\nslate_support = {{ package = \"slate-support\", path = \"../slate-support\" }}\n\n\
             [profile.dev]\nopt-level = 0\ndebug = false\n\n[workspace]\n",
            libc = libc_dir.display().to_string()
        ),
    )?;

    let mut probes = Vec::new();
    for (index, (name, fields)) in candidates.iter().enumerate() {
        let Some(ty) = providers.path(name) else {
            continue;
        };
        probes.push(Probe {
            key: format!("L{index}_s"),
            type_index: index,
            line: format!("core::mem::size_of::<{ty}>() as u32"),
        });
        probes.push(Probe {
            key: format!("L{index}_a"),
            type_index: index,
            line: format!("core::mem::align_of::<{ty}>() as u32"),
        });
        for (field_index, field) in fields.iter().enumerate() {
            probes.push(Probe {
                key: format!("L{index}_o{field_index}"),
                type_index: index,
                line: format!("core::mem::offset_of!({ty}, {}) as u32", ident(field)),
            });
        }
    }

    const HEADER_LINES: usize = 3;
    let lib = dir.join("lib.rs");
    let target_dir = dir.join("target");
    let mut object_path = None;
    for _ in 0..8 {
        let mut source =
            String::from("#![no_std]\n#![allow(non_upper_case_globals)]\nextern crate libc;\n");
        for probe in &probes {
            source.push_str(&format!(
                "#[unsafe(no_mangle)] pub static {}: u32 = {};\n",
                probe.key, probe.line
            ));
        }
        std::fs::write(&lib, source)?;
        let output = Command::new("cargo")
            .args([
                "rustc",
                "--lib",
                "--message-format=json",
                "--target",
                target,
            ])
            .arg("--manifest-path")
            .arg(dir.join("Cargo.toml"))
            .env("CARGO_TARGET_DIR", &target_dir)
            .args(["--", "--emit=obj"])
            .output()
            .context("running cargo")?;
        let stdout = String::from_utf8_lossy(&output.stdout);
        let mut bad_lines = HashSet::new();
        let mut unmapped = Vec::new();
        for line in stdout.lines() {
            let Ok(value) = serde_json::from_str::<serde_json::Value>(line) else {
                continue;
            };
            if value["reason"] != "compiler-message" || value["message"]["level"] != "error" {
                continue;
            }
            let mut mapped = false;
            for span in value["message"]["spans"].as_array().into_iter().flatten() {
                let file = span["file_name"].as_str().unwrap_or("");
                if (file == "lib.rs" || Path::new(file).ends_with(Path::new(target).join("lib.rs")))
                    && let Some(line) = span["line_start"].as_u64()
                {
                    bad_lines.insert(line as usize);
                    mapped = true;
                }
            }
            if !mapped {
                unmapped.push(
                    value["message"]["rendered"]
                        .as_str()
                        .unwrap_or("")
                        .to_string(),
                );
            }
        }
        if !unmapped.is_empty() {
            bail!("{target}: probe build failed:\n{}", unmapped.join("\n"));
        }
        if bad_lines.is_empty() {
            if !output.status.success() {
                bail!(
                    "{target}: cargo failed:\n{}",
                    String::from_utf8_lossy(&output.stderr)
                );
            }
            object_path = newest_object(&target_dir.join(target).join("debug"));
            break;
        }
        let bad_types: HashSet<usize> = bad_lines
            .iter()
            .filter_map(|line| line.checked_sub(HEADER_LINES + 1))
            .filter_map(|index| probes.get(index))
            .filter(|probe| probe.key.ends_with("_s"))
            .map(|probe| probe.type_index)
            .collect();
        let before = probes.len();
        let mut index = 0;
        probes.retain(|probe| {
            index += 1;
            !bad_lines.contains(&(HEADER_LINES + index)) && !bad_types.contains(&probe.type_index)
        });
        if probes.len() == before {
            bail!("{target}: probe errors did not map to probe lines");
        }
    }
    let object_path = object_path.with_context(|| format!("{target}: probe did not converge"))?;
    read_object(&object_path, candidates)
}

fn newest_object(root: &Path) -> Option<PathBuf> {
    let mut found: Vec<PathBuf> = Vec::new();
    let mut pending = vec![root.to_path_buf()];
    while let Some(dir) = pending.pop() {
        let Ok(entries) = std::fs::read_dir(&dir) else {
            continue;
        };
        for entry in entries.filter_map(|entry| entry.ok()) {
            let path = entry.path();
            if path.is_dir() {
                pending.push(path);
            } else if path.extension().is_some_and(|ext| ext == "o")
                && path
                    .file_name()
                    .is_some_and(|name| name.to_string_lossy().starts_with("probe-"))
            {
                found.push(path);
            }
        }
    }
    found
        .into_iter()
        .max_by_key(|path| std::fs::metadata(path).and_then(|m| m.modified()).ok())
}

fn read_object(
    path: &Path,
    candidates: &[(String, Vec<String>)],
) -> Result<BTreeMap<String, RustLayout>> {
    let bytes = std::fs::read(path)?;
    let file = object::File::parse(&*bytes)?;
    let mut values: HashMap<String, u32> = HashMap::new();
    for symbol in file.symbols() {
        let Ok(name) = symbol.name() else { continue };
        let name = name.strip_prefix('_').unwrap_or(name);
        if !name.starts_with('L') || !name.contains('_') {
            continue;
        }
        let Some(section) = symbol
            .section_index()
            .and_then(|i| file.section_by_index(i).ok())
        else {
            continue;
        };
        let Ok(data) = section.data() else { continue };
        let start = (symbol.address() - section.address()) as usize;
        let Some(raw) = data.get(start..start + 4) else {
            continue;
        };
        values.insert(name.to_string(), u32::from_le_bytes(raw.try_into()?));
    }
    let mut result = BTreeMap::new();
    for (index, (name, fields)) in candidates.iter().enumerate() {
        let (Some(&size), Some(&align)) = (
            values.get(&format!("L{index}_s")),
            values.get(&format!("L{index}_a")),
        ) else {
            continue;
        };
        let offsets = fields
            .iter()
            .enumerate()
            .filter_map(|(field_index, field)| {
                values
                    .get(&format!("L{index}_o{field_index}"))
                    .map(|&offset| (field.clone(), u64::from(offset)))
            })
            .collect();
        result.insert(
            name.clone(),
            RustLayout {
                size: u64::from(size),
                align: u64::from(align),
                offsets,
            },
        );
    }
    Ok(result)
}

pub fn compile_lines(
    libc: &Path,
    work: &Path,
    target: &str,
    lines: &[String],
) -> Result<(PathBuf, HashSet<usize>)> {
    let dir = work.join(format!("symbol-probe-{target}"));
    std::fs::create_dir_all(&dir)?;
    std::fs::write(
        dir.join("Cargo.toml"),
        format!(
            "[package]\nname = \"probe\"\nversion = \"0.0.0\"\nedition = \"2024\"\n[lib]\npath = \"lib.rs\"\n[dependencies]\nlibc = {{ path = {:?}, default-features = false }}\nslate_support = {{ package = \"slate-support\", path = \"../slate-support\" }}\n[profile.dev]\ndebug = false\n[workspace]\n",
            libc.display().to_string()
        ),
    )?;
    let mut active: Vec<usize> = (0..lines.len()).collect();
    for _ in 0..16 {
        let mut source = String::from("#![no_std]\n#![allow(deprecated, unused_comparisons)]\n");
        for index in &active {
            source.push_str(&lines[*index]);
            source.push('\n');
        }
        std::fs::write(dir.join("lib.rs"), source)?;
        let output = Command::new("cargo")
            .args([
                "rustc",
                "--lib",
                "--message-format=json",
                "--target",
                target,
            ])
            .arg("--manifest-path")
            .arg(dir.join("Cargo.toml"))
            .env("CARGO_TARGET_DIR", dir.join("target"))
            .args(["--", "--emit=obj"])
            .output()?;
        let mut bad = HashSet::new();
        let mut errors = Vec::new();
        for line in String::from_utf8_lossy(&output.stdout).lines() {
            let Ok(value) = serde_json::from_str::<serde_json::Value>(line) else {
                continue;
            };
            if value["reason"] != "compiler-message" || value["message"]["level"] != "error" {
                continue;
            }
            let mut mapped = false;
            for span in value["message"]["spans"].as_array().into_iter().flatten() {
                if span["is_primary"] == true
                    && span["file_name"]
                        .as_str()
                        .is_some_and(|f| f == "lib.rs" || Path::new(f) == dir.join("lib.rs"))
                    && let Some(index) = span["line_start"].as_u64().and_then(|n| n.checked_sub(3))
                    && let Some(index) = active.get(index as usize)
                {
                    bad.insert(*index);
                    mapped = true;
                }
            }
            if !mapped {
                errors.push(
                    value["message"]["rendered"]
                        .as_str()
                        .unwrap_or("")
                        .to_string(),
                );
            }
        }
        if !errors.is_empty() {
            bail!("{target}: symbol probe failed: {}", errors.join("\n"));
        }
        if output.status.success() {
            let object = newest_object(&dir.join("target").join(target).join("debug"))
                .context("symbol probe object")?;
            return Ok((object, active.into_iter().collect()));
        }
        if bad.is_empty() {
            bail!(
                "{target}: symbol cargo failed: {}",
                String::from_utf8_lossy(&output.stderr)
            );
        }
        active.retain(|index| !bad.contains(index));
    }
    bail!("{target}: symbol probe did not converge")
}
