use std::collections::BTreeMap;
use std::env;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;

use clap::Parser;
use serde::Deserialize;

#[derive(Debug, Deserialize)]
struct RawParam {
    #[serde(rename = "type")]
    ty: String,
    immarg: bool,
}

#[derive(Debug, Deserialize)]
struct RawIntrinsic {
    name: String,
    #[serde(default)]
    builtins: Vec<String>,
    overloaded: bool,
    ret: Option<String>,
    params: Option<Vec<RawParam>>,
    #[serde(default)]
    overloaded_positions: Option<Vec<u32>>,
}

#[derive(Parser)]
#[command(about = "Generate Slate's intrinsic lowering catalog from LLVM")]
struct Config {
    #[arg(long)]
    llvm_build: PathBuf,
    #[arg(long)]
    llvm_src: Option<PathBuf>,
    #[arg(long = "prefix", value_parser = ["x86", "aarch64", "arm", "riscv"], default_values = ["x86", "aarch64", "arm", "riscv"])]
    prefixes: Vec<String>,
    #[arg(long)]
    out: PathBuf,
    #[arg(long)]
    stdarch_src: Option<PathBuf>,
}

#[derive(Debug)]
struct StdarchOverride {
    link_name: String,
    params: Vec<String>,
    ret: Option<String>,
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut config = Config::parse();
    config.prefixes.sort();
    config.prefixes.dedup();

    let llvm_config = config.llvm_build.join("bin/llvm-config");
    if !llvm_config.exists() {
        return Err(format!("llvm-config not found at {}", llvm_config.display()).into());
    }

    let work_dir = tempfile::tempdir()?;
    let extractor = build_extractor(&llvm_config, work_dir.path())?;
    let builtins = mine_builtin_names(&config, &llvm_config)?;

    let llvm_commit = config
        .llvm_src
        .as_ref()
        .and_then(|src| git_short_commit(src));

    let mut sections = Vec::new();
    let raw = run_extractor(&extractor, "__generic__")?;
    sections.push(("general".to_string(), raw));
    for prefix in &config.prefixes {
        let raw = run_extractor(&extractor, prefix)?;
        sections.push((prefix.clone(), raw));
    }

    let stdarch_overrides = match &config.stdarch_src {
        Some(dir) => mine_stdarch_overrides(dir)?,
        None => Vec::new(),
    };
    if config.stdarch_src.is_some() {
        println!(
            "mined {} stdarch link_name overrides",
            stdarch_overrides.len()
        );
    }

    for (_, intrinsics) in &mut sections {
        intrinsics.sort_by(|a, b| a.name.cmp(&b.name));
        for intrinsic in intrinsics {
            if let Some(metadata) = builtins.get(&intrinsic.name) {
                intrinsic.builtins = metadata.builtins.clone();
                if let Some(params) = &mut intrinsic.params {
                    for &index in &metadata.immargs {
                        if let Some(param) = params.get_mut(index) {
                            param.immarg = true;
                        }
                    }
                }
            }
        }
    }
    let source = generate_source(&sections, llvm_commit.as_deref(), &stdarch_overrides);
    if let Some(parent) = config.out.parent() {
        fs::create_dir_all(parent)?;
    }
    fs::write(&config.out, source)?;

    let total: usize = sections.iter().map(|(_, v)| v.len()).sum();
    println!(
        "wrote {} intrinsics across {} sections to {}",
        total,
        sections.len(),
        config.out.display()
    );
    Ok(())
}

fn build_extractor(
    llvm_config: &Path,
    work_dir: &Path,
) -> Result<PathBuf, Box<dyn std::error::Error>> {
    let cxxflags = llvm_config_output(llvm_config, &["--cxxflags"])?;
    let ldflags = llvm_config_output(llvm_config, &["--ldflags"])?;
    let libs = llvm_config_output(llvm_config, &["--libs", "core", "support"])?;
    let system_libs = llvm_config_output(llvm_config, &["--system-libs"])?;

    let cpp_source = Path::new(env!("CARGO_MANIFEST_DIR")).join("cpp/extract_intrinsics.cpp");
    let out_bin = work_dir.join("extract_intrinsics");

    let cxx = env::var("CXX").unwrap_or_else(|_| "c++".to_string());
    let mut cmd = Command::new(cxx);
    cmd.args(split_flags(&cxxflags)?);
    cmd.arg(&cpp_source);
    cmd.arg("-o").arg(&out_bin);
    cmd.args(split_flags(&ldflags)?);
    cmd.args(split_flags(&libs)?);
    cmd.args(split_flags(&system_libs)?);

    let status = cmd.status()?;
    if !status.success() {
        return Err(
            "failed to build cpp/extract_intrinsics.cpp against the given LLVM build".into(),
        );
    }
    Ok(out_bin)
}

fn split_flags(s: &str) -> Result<Vec<String>, Box<dyn std::error::Error>> {
    shlex::split(s).ok_or_else(|| "invalid llvm-config shell flags".into())
}

fn llvm_config_output(
    llvm_config: &Path,
    args: &[&str],
) -> Result<String, Box<dyn std::error::Error>> {
    let output = Command::new(llvm_config).args(args).output()?;
    if !output.status.success() {
        return Err(format!("llvm-config {args:?} failed").into());
    }
    Ok(String::from_utf8(output.stdout)?.trim().to_string())
}

fn run_extractor(
    extractor: &Path,
    prefix: &str,
) -> Result<Vec<RawIntrinsic>, Box<dyn std::error::Error>> {
    let output = Command::new(extractor).arg(prefix).output()?;
    if !output.status.success() {
        return Err(format!(
            "extractor failed for prefix {prefix}: {}",
            String::from_utf8_lossy(&output.stderr)
        )
        .into());
    }
    Ok(serde_json::from_slice(&output.stdout)?)
}

fn git_short_commit(llvm_src: &Path) -> Option<String> {
    let output = Command::new("git")
        .args(["rev-parse", "--short", "HEAD"])
        .current_dir(llvm_src)
        .output()
        .ok()?;
    if !output.status.success() {
        return None;
    }
    Some(String::from_utf8(output.stdout).ok()?.trim().to_string())
}

fn mine_stdarch_overrides(
    stdarch_src: &Path,
) -> Result<Vec<StdarchOverride>, Box<dyn std::error::Error>> {
    if !stdarch_src.is_dir() {
        return Err(format!(
            "stdarch source directory not found: {}",
            stdarch_src.display()
        )
        .into());
    }
    let mut overrides = Vec::new();
    for dir in ["x86", "x86_64"] {
        let dir_path = stdarch_src.join(dir);
        if !dir_path.is_dir() {
            continue;
        }
        for entry in fs::read_dir(&dir_path)? {
            let path = entry?.path();
            if path.extension().and_then(|e| e.to_str()) != Some("rs") {
                continue;
            }
            let text = fs::read_to_string(&path)?;
            mine_file(&text, &mut overrides);
        }
    }
    overrides.sort_by(|a: &StdarchOverride, b| {
        a.link_name
            .cmp(&b.link_name)
            .then_with(|| a.params.cmp(&b.params))
            .then_with(|| a.ret.cmp(&b.ret))
    });
    overrides.dedup_by(|a, b| a.link_name == b.link_name && a.params == b.params && a.ret == b.ret);
    Ok(overrides)
}

fn mine_file(text: &str, out: &mut Vec<StdarchOverride>) {
    let mut lines = text.lines().peekable();
    while let Some(line) = lines.next() {
        let Some(after) = line.trim_start().strip_prefix("#[link_name = \"") else {
            continue;
        };
        let Some(end) = after.find('"') else { continue };
        let link_name = &after[..end];
        if !link_name.starts_with("llvm.x86") {
            continue;
        }
        let mut sig = String::new();
        for sig_line in lines.by_ref() {
            sig.push_str(sig_line.trim());
            sig.push(' ');
            if sig_line.contains(';') {
                break;
            }
        }
        let Some(sig) = sig.trim().strip_suffix(';').map(str::trim) else {
            continue;
        };
        let Some(sig) = sig.strip_prefix("fn ") else {
            continue;
        };
        let Some(open) = sig.find('(') else { continue };
        let Some(close) = matching_paren(sig, open) else {
            continue;
        };
        let params_text = &sig[open + 1..close];
        let params: Vec<String> = split_top_level_commas(params_text)
            .into_iter()
            .filter_map(|param| {
                let param = param.trim();
                if param.is_empty() {
                    return None;
                }
                let colon = param.find(':')?;
                Some(param[colon + 1..].trim().to_string())
            })
            .collect();
        let rest = sig[close + 1..].trim();
        let ret = rest.strip_prefix("->").map(|r| r.trim().to_string());
        out.push(StdarchOverride {
            link_name: link_name.to_string(),
            params,
            ret,
        });
    }
}

fn matching_paren(s: &str, open: usize) -> Option<usize> {
    let mut depth = 0i32;
    for (i, ch) in s.char_indices().skip(open) {
        match ch {
            '(' => depth += 1,
            ')' => {
                depth -= 1;
                if depth == 0 {
                    return Some(i);
                }
            }
            _ => {}
        }
    }
    None
}

fn split_top_level_commas(s: &str) -> Vec<&str> {
    let mut parts = Vec::new();
    let mut depth = 0i32;
    let mut start = 0usize;
    for (i, ch) in s.char_indices() {
        match ch {
            '(' | '[' | '<' => depth += 1,
            ')' | ']' | '>' => depth -= 1,
            ',' if depth == 0 => {
                parts.push(&s[start..i]);
                start = i + 1;
            }
            _ => {}
        }
    }
    parts.push(&s[start..]);
    parts
}

fn generate_source(
    sections: &[(String, Vec<RawIntrinsic>)],
    llvm_commit: Option<&str>,
    stdarch_overrides: &[StdarchOverride],
) -> String {
    let mut out = String::new();
    out.push_str(
        "#![allow(dead_code, reason = \"catalog retains metadata for future lowering\")]\n",
    );
    out.push_str(&format!(
        "pub const LLVM_COMMIT: Option<&str> = {llvm_commit:?};\n\n"
    ));
    out.push_str("pub struct IntrinsicParam {\n    pub llvm_type: &'static str,\n    pub immarg: bool,\n}\n\n");
    out.push_str("pub struct IntrinsicSignature {\n    pub name: &'static str,\n    pub builtins: &'static [&'static str],\n    pub overloaded: bool,\n    pub ret: Option<&'static str>,\n    pub params: Option<&'static [IntrinsicParam]>,\n    pub overloaded_positions: Option<&'static [u32]>,\n}\n\n");

    for (prefix, intrinsics) in sections {
        let const_name = format!("{}_INTRINSICS", prefix.to_uppercase());
        out.push_str(&format!(
            "#[rustfmt::skip]\npub static {const_name}: &[IntrinsicSignature] = &[\n"
        ));
        for intr in intrinsics {
            let ret = match &intr.ret {
                Some(r) => format!("Some({:?})", r),
                None => "None".to_string(),
            };
            let params = match &intr.params {
                Some(params) => {
                    let entries: Vec<String> = params
                        .iter()
                        .map(|p| {
                            format!(
                                "IntrinsicParam {{ llvm_type: {:?}, immarg: {} }}",
                                p.ty, p.immarg
                            )
                        })
                        .collect();
                    format!("Some(&[{}])", entries.join(", "))
                }
                None => "None".to_string(),
            };
            let overloaded_positions = match &intr.overloaded_positions {
                Some(positions) => format!(
                    "Some(&[{}])",
                    positions
                        .iter()
                        .map(u32::to_string)
                        .collect::<Vec<_>>()
                        .join(", ")
                ),
                None => "None".to_string(),
            };
            out.push_str(&format!(
                "    IntrinsicSignature {{ name: {:?}, builtins: &{:?}, overloaded: {}, ret: {}, params: {}, overloaded_positions: {} }},\n",
                intr.name, intr.builtins, intr.overloaded, ret, params, overloaded_positions
            ));
        }
        out.push_str("];\n");
    }

    if !stdarch_overrides.is_empty() {
        out.push_str(
            "pub struct StdarchOverride {\n    \
             pub link_name: &'static str,\n    \
             pub params: &'static [&'static str],\n    \
             pub ret: Option<&'static str>,\n\
             }\n\n",
        );
        out.push_str(
            "#[rustfmt::skip]\npub static X86_STDARCH_OVERRIDES: &[StdarchOverride] = &[\n",
        );
        for entry in stdarch_overrides {
            let params: Vec<String> = entry.params.iter().map(|p| format!("{p:?}")).collect();
            let ret = match &entry.ret {
                Some(r) => format!("Some({r:?})"),
                None => "None".to_string(),
            };
            out.push_str(&format!(
                "    StdarchOverride {{ link_name: {:?}, params: &[{}], ret: {} }},\n",
                entry.link_name,
                params.join(", "),
                ret
            ));
        }
        out.push_str("];\n");
    }

    out
}

fn mine_builtin_names(
    config: &Config,
    llvm_config: &Path,
) -> Result<BTreeMap<String, SourceMetadata>, Box<dyn std::error::Error>> {
    let llvm_dir = match &config.llvm_src {
        Some(src) => src.join("llvm"),
        None => PathBuf::from(llvm_config_output(llvm_config, &["--src-root"])?),
    };
    let include = llvm_dir.join("include");
    let output = Command::new(config.llvm_build.join("bin/llvm-tblgen"))
        .arg("--dump-json")
        .arg("-I")
        .arg(&include)
        .arg(include.join("llvm/IR/Intrinsics.td"))
        .output()?;
    if !output.status.success() {
        return Err(format!(
            "llvm-tblgen failed: {}",
            String::from_utf8_lossy(&output.stderr)
        )
        .into());
    }
    let records: BTreeMap<String, serde_json::Value> = serde_json::from_slice(&output.stdout)?;
    let mut names = BTreeMap::<String, SourceMetadata>::new();
    for (record_name, record) in &records {
        let Some(suffix) = record_name.strip_prefix("int_") else {
            continue;
        };
        let llvm_name = record
            .get("LLVMName")
            .and_then(|v| v.as_str())
            .filter(|name| !name.is_empty())
            .map(str::to_owned)
            .unwrap_or_else(|| format!("llvm.{}", suffix.replace('_', ".")));
        for field in ["ClangBuiltinName", "GCCBuiltinName"] {
            if let Some(name) = record
                .get(field)
                .and_then(|v| v.as_str())
                .filter(|name| !name.is_empty())
            {
                names
                    .entry(llvm_name.clone())
                    .or_default()
                    .builtins
                    .push(name.to_owned());
            }
        }
        for property in record
            .get("IntrProperties")
            .and_then(|v| v.as_array())
            .into_iter()
            .flatten()
        {
            let Some(property) = property
                .get("def")
                .and_then(|v| v.as_str())
                .and_then(|name| records.get(name))
            else {
                continue;
            };
            if !property
                .get("!superclasses")
                .and_then(|v| v.as_array())
                .is_some_and(|classes| classes.iter().any(|class| class.as_str() == Some("ImmArg")))
            {
                continue;
            }
            if let Some(index) = property
                .get("ArgNo")
                .and_then(|v| v.as_u64())
                .and_then(|index| index.checked_sub(1))
            {
                names
                    .entry(llvm_name.clone())
                    .or_default()
                    .immargs
                    .push(index as usize);
            }
        }
    }
    for metadata in names.values_mut() {
        metadata.builtins.sort();
        metadata.builtins.dedup();
    }
    Ok(names)
}

#[derive(Default)]
struct SourceMetadata {
    builtins: Vec<String>,
    immargs: Vec<usize>,
}
