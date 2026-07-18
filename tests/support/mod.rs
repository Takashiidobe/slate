#![allow(dead_code)]

pub mod alive;
pub mod cgen;

use std::collections::BTreeMap;
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};

pub struct Run {
    stdout: Vec<u8>,
    stderr: Vec<u8>,
    exit: Option<i32>,
}

fn cc() -> String {
    std::env::var("SLATE_CC").unwrap_or_else(|_| "clang".into())
}

fn cargo() -> String {
    std::env::var("SLATE_CARGO").unwrap_or_else(|_| "cargo".into())
}

/// Path to the `alive-tv` binary used for translation-validation regression
/// tests (compares before/after LLVM IR for a skippable fixup pass).
pub fn alive_tv() -> String {
    std::env::var("SLATE_ALIVE_TV").unwrap_or_else(|_| {
        let home = std::env::var("HOME").expect("HOME not set");
        format!("{home}/alive2/build/alive-tv")
    })
}

pub fn compile_c(src: &Path, out: &Path) -> Result<(), String> {
    let o = Command::new(cc())
        .args(["-O0", "-std=c11", "-o"])
        .arg(out)
        .arg(src)
        .arg("-lm")
        .output()
        .map_err(|e| format!("spawn {}: {e}", cc()))?;
    if !o.status.success() {
        return Err(format!(
            "C compile failed:\n{}",
            String::from_utf8_lossy(&o.stderr)
        ));
    }
    Ok(())
}

/// Compile several C translation units together into one binary (cross-TU link).
pub fn compile_c_multi(srcs: &[PathBuf], out: &Path) -> Result<(), String> {
    let o = Command::new(cc())
        .args(["-O0", "-std=c11", "-o"])
        .arg(out)
        .args(srcs)
        .arg("-lm")
        .output()
        .map_err(|e| format!("spawn {}: {e}", cc()))?;
    if !o.status.success() {
        return Err(format!(
            "C compile failed:\n{}",
            String::from_utf8_lossy(&o.stderr)
        ));
    }
    Ok(())
}

/// Invoke `slate translate-project <dir> <out_dir>`, writing one Rust module per
/// C translation unit (the unit with `main` becomes `main.rs`).
pub fn translate_project(dir: &Path, out_dir: &Path) -> Result<(), String> {
    let o = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-project")
        .arg(dir)
        .arg(out_dir)
        .output()
        .map_err(|e| format!("spawn slate translate-project: {e}"))?;
    if !o.status.success() {
        return Err(format!(
            "translate-project failed:\n{}",
            String::from_utf8_lossy(&o.stderr)
        ));
    }
    Ok(())
}

/// Compile a directory of translated Rust modules as one crate rooted at
/// `main.rs`, so `mod`/`use crate::…` wiring across units is linked together.
pub fn compile_rs_project(
    rs_dir: &Path,
    work_dir: &Path,
    package: &str,
) -> Result<PathBuf, String> {
    let project = work_dir.join(format!("{package}_proj"));
    let _ = std::fs::remove_dir_all(&project);
    std::fs::create_dir_all(project.join("src"))
        .map_err(|e| format!("create {}: {e}", project.display()))?;
    std::fs::write(
        project.join("Cargo.toml"),
        format!(
            r#"[package]
name = "{package}"
version = "0.0.0"
edition = "2024"

[dependencies]
libc = "0.2"

[build-dependencies]
cc = "1"

[profile.dev]
overflow-checks = false
"#
        ),
    )
    .map_err(|e| format!("write Cargo.toml: {e}"))?;
    write_long_double_shim(&project)?;

    for entry in std::fs::read_dir(rs_dir).map_err(|e| format!("read {}: {e}", rs_dir.display()))? {
        let path = entry
            .map_err(|e| format!("read {} entry: {e}", rs_dir.display()))?
            .path();
        if path.extension().and_then(|e| e.to_str()) == Some("rs") {
            let name = path.file_name().expect("rs file name");
            std::fs::copy(&path, project.join("src").join(name))
                .map_err(|e| format!("copy {} to project: {e}", path.display()))?;
        }
    }

    let target_dir = project.join("target");
    let o = Command::new(cargo())
        .args(["build", "--quiet", "--manifest-path"])
        .arg(project.join("Cargo.toml"))
        .arg("--target-dir")
        .arg(&target_dir)
        .output()
        .map_err(|e| format!("spawn {}: {e}", cargo()))?;
    if !o.status.success() {
        return Err(format!(
            "Rust cargo build failed:\n{}",
            String::from_utf8_lossy(&o.stderr)
        ));
    }
    Ok(target_dir.join("debug").join(package))
}

pub fn compile_rs_cargo(src: &Path, work_dir: &Path, package: &str) -> Result<PathBuf, String> {
    let project = work_dir.join(format!("{package}_cargo"));
    if project.exists() {
        std::fs::remove_dir_all(&project)
            .map_err(|e| format!("remove {}: {e}", project.display()))?;
    }
    std::fs::create_dir_all(project.join("src"))
        .map_err(|e| format!("create {}: {e}", project.display()))?;
    std::fs::write(
        project.join("Cargo.toml"),
        format!(
            r#"[package]
name = "{package}"
version = "0.0.0"
edition = "2024"

[dependencies]
libc = "0.2"

[build-dependencies]
cc = "1"

[profile.dev]
overflow-checks = false
"#
        ),
    )
    .map_err(|e| format!("write Cargo.toml: {e}"))?;
    write_long_double_shim(&project)?;
    std::fs::copy(src, project.join("src/main.rs"))
        .map_err(|e| format!("copy {} to cargo project: {e}", src.display()))?;

    let target_dir = project.join("target");
    let o = Command::new(cargo())
        .args(["build", "--quiet", "--manifest-path"])
        .arg(project.join("Cargo.toml"))
        .arg("--target-dir")
        .arg(&target_dir)
        .output()
        .map_err(|e| format!("spawn {}: {e}", cargo()))?;
    if !o.status.success() {
        return Err(format!(
            "Rust cargo build failed:\n{}",
            String::from_utf8_lossy(&o.stderr)
        ));
    }
    Ok(target_dir.join("debug").join(package))
}

/// One differential case: a name plus its C source and translated Rust source.
pub struct Case {
    pub name: String,
    pub c_src: PathBuf,
    pub rs_src: PathBuf,
    pub config: RunConfig,
}

#[derive(Default, Clone)]
pub struct RunConfig {
    pub args: Vec<String>,
    pub stdin: Vec<u8>,
    pub env: BTreeMap<String, String>,
    pub compare_stderr: bool,
}

/// A cargo bin target name derived from a case name (alnum/underscore only).
fn bin_name(name: &str) -> String {
    name.chars()
        .map(|c| if c.is_ascii_alphanumeric() { c } else { '_' })
        .collect()
}

/// Compile every case's Rust as its own binary target inside a single shared
/// crate, so `libc` is built once and all programs compile in one parallel
/// `cargo build` instead of one cold crate (and one libc rebuild) per case.
/// Then compile each C source and compare stdout + exit code. Results are
/// returned in input order.
///
/// If the batched build fails (e.g. a program does not compile), it falls back
/// to the per-case path so the failing case gets an exact error.
pub fn compare_batch(cases: &[Case], work_dir: &Path) -> Vec<(String, Result<(), String>)> {
    if cases.is_empty() {
        return Vec::new();
    }

    // preserve the project so Cargo can reuse target artifacts across runs.
    let project = work_dir.join("batch_cargo");
    let bin_dir = project.join("src/bin");

    let batch_bins = build_batch(cases, &project, &bin_dir);
    let target_dir = project.join("target");

    cases
        .iter()
        .map(|case| {
            let result = (|| {
                let bn = bin_name(&case.name);
                let rs_bin = match &batch_bins {
                    Ok(()) => target_dir.join("debug").join(&bn),
                    // fall back to an isolated build to pinpoint this case
                    Err(_) => compile_rs_cargo(&case.rs_src, work_dir, &format!("{bn}_rs"))?,
                };
                let c_bin = work_dir.join(format!("{}_c", bn));
                compile_c(&case.c_src, &c_bin)?;
                let run_dir = work_dir.join("runs").join(&bn);
                if run_dir.exists() {
                    std::fs::remove_dir_all(&run_dir)
                        .map_err(|e| format!("remove {}: {e}", run_dir.display()))?;
                }
                std::fs::create_dir_all(&run_dir)
                    .map_err(|e| format!("create {}: {e}", run_dir.display()))?;
                compare_runs(
                    &run_with_config(&c_bin, &case.config, &run_dir)?,
                    &run_with_config(&rs_bin, &case.config, &run_dir)?,
                    case.config.compare_stderr,
                )
            })();
            (case.name.clone(), result)
        })
        .collect()
}

/// Write one crate with a `src/bin/<name>.rs` per case and build them together.
fn build_batch(cases: &[Case], project: &Path, bin_dir: &Path) -> Result<(), String> {
    std::fs::create_dir_all(bin_dir).map_err(|e| format!("create {}: {e}", bin_dir.display()))?;
    write_if_changed(
        project.join("Cargo.toml"),
        r#"[package]
name = "slate_batch"
version = "0.0.0"
edition = "2024"

[dependencies]
libc = "0.2"

[build-dependencies]
cc = "1"

# clang compiles the C side at -O0, where signed overflow wraps two's-complement
# (and unsigned wrap is defined). Disable Rust's overflow checks so both sides
# wrap identically instead of panicking. Division by zero / INT_MIN by -1 still
# trap on both sides, so the generator keeps divisors to nonzero constants.
[profile.dev]
overflow-checks = false
"#
        .as_bytes(),
    )
    .map_err(|e| format!("write Cargo.toml: {e}"))?;
    write_long_double_shim(project)?;

    let mut expected = BTreeMap::new();
    for case in cases {
        let dest = bin_dir.join(format!("{}.rs", bin_name(&case.name)));
        expected.insert(dest.clone(), ());
        let contents = std::fs::read(&case.rs_src)
            .map_err(|e| format!("read {} for batch crate: {e}", case.rs_src.display()))?;
        write_if_changed(&dest, &contents)
            .map_err(|e| format!("write {} to batch crate: {e}", dest.display()))?;
    }

    for entry in
        std::fs::read_dir(bin_dir).map_err(|e| format!("read {}: {e}", bin_dir.display()))?
    {
        let path = entry
            .map_err(|e| format!("read {} entry: {e}", bin_dir.display()))?
            .path();
        if path.extension().and_then(|e| e.to_str()) == Some("rs") && !expected.contains_key(&path)
        {
            std::fs::remove_file(&path)
                .map_err(|e| format!("remove stale {}: {e}", path.display()))?;
        }
    }

    let o = Command::new(cargo())
        .args(["build", "--quiet", "--manifest-path"])
        .arg(project.join("Cargo.toml"))
        .arg("--target-dir")
        .arg(project.join("target"))
        .output()
        .map_err(|e| format!("spawn {}: {e}", cargo()))?;
    if o.status.success() {
        Ok(())
    } else {
        Err(String::from_utf8_lossy(&o.stderr).into_owned())
    }
}

pub fn compare_runs(c: &Run, r: &Run, compare_stderr: bool) -> Result<(), String> {
    if c.exit != r.exit {
        return Err(format!(
            "exit code differs: C={:?} Rust={:?}",
            c.exit, r.exit
        ));
    }
    if c.stdout != r.stdout {
        return Err(format!(
            "stdout differs:\n--- C ---\n{}\n--- Rust ---\n{}",
            String::from_utf8_lossy(&c.stdout),
            String::from_utf8_lossy(&r.stdout),
        ));
    }
    if compare_stderr && c.stderr != r.stderr {
        return Err(format!(
            "stderr differs:\n--- C ---\n{}\n--- Rust ---\n{}",
            String::from_utf8_lossy(&c.stderr),
            String::from_utf8_lossy(&r.stderr),
        ));
    }
    Ok(())
}

fn write_long_double_shim(project: &Path) -> Result<(), String> {
    write_if_changed(
        project.join("build.rs"),
        r#"fn main() {
    cc::Build::new()
        .file("src/slate_long_double.c")
        .compile("slate_long_double");
}
"#
        .as_bytes(),
    )
    .map_err(|e| format!("write build.rs: {e}"))?;
    write_if_changed(
        project.join("src/slate_long_double.c"),
        r#"#include <stdio.h>
#include <stdlib.h>

void __slate_strtold(char *nptr, char **endptr, long double *out) {
    *out = strtold(nptr, endptr);
}

int __slate_printf_ld_i32(char *fmt, const long double *value, int arg) {
    return printf(fmt, *value, arg);
}
"#
        .as_bytes(),
    )
    .map(|_| ())
    .map_err(|e| format!("write slate_long_double.c: {e}"))
}

fn write_if_changed(path: impl AsRef<Path>, contents: &[u8]) -> std::io::Result<bool> {
    let path = path.as_ref();
    if std::fs::read(path).is_ok_and(|current| current == contents) {
        return Ok(false);
    }
    if let Some(parent) = path.parent() {
        std::fs::create_dir_all(parent)?;
    }
    std::fs::write(path, contents)?;
    Ok(true)
}

pub fn run_with_config(bin: &Path, config: &RunConfig, cwd: &Path) -> Result<Run, String> {
    let mut cmd = Command::new(bin);
    cmd.current_dir(cwd)
        .args(&config.args)
        .env("LC_ALL", "C")
        .env("TZ", "UTC")
        .envs(&config.env)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped());
    if config.stdin.is_empty() {
        cmd.stdin(Stdio::null());
    } else {
        cmd.stdin(Stdio::piped());
    }
    let mut child = cmd
        .spawn()
        .map_err(|e| format!("run {}: {e}", bin.display()))?;
    if !config.stdin.is_empty() {
        let stdin = child
            .stdin
            .as_mut()
            .ok_or_else(|| format!("open stdin for {}", bin.display()))?;
        stdin
            .write_all(&config.stdin)
            .map_err(|e| format!("write stdin for {}: {e}", bin.display()))?;
    }
    let o = child
        .wait_with_output()
        .map_err(|e| format!("wait {}: {e}", bin.display()))?;
    Ok(Run {
        stdout: o.stdout,
        stderr: o.stderr,
        exit: o.status.code(),
    })
}

pub fn translate(c_src: &Path, rs_out: &Path) -> Result<(), String> {
    let o = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate")
        .arg(c_src)
        .output()
        .map_err(|e| format!("spawn slate translate: {e}"))?;
    if !o.status.success() {
        return Err(format!(
            "translate failed:\n{}",
            String::from_utf8_lossy(&o.stderr)
        ));
    }
    write_if_changed(rs_out, &o.stdout)
        .map(|_| ())
        .map_err(|e| format!("write {}: {e}", rs_out.display()))
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::time::{SystemTime, UNIX_EPOCH};

    fn temp_path(name: &str) -> PathBuf {
        let nonce = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .unwrap()
            .as_nanos();
        std::env::temp_dir().join(format!(
            "slate-support-test-{}-{nonce}-{name}",
            std::process::id()
        ))
    }

    #[test]
    fn write_if_changed_reports_unchanged_files() {
        let path = temp_path("unchanged");
        assert!(write_if_changed(&path, b"same").unwrap());
        assert!(!write_if_changed(&path, b"same").unwrap());
        assert_eq!(std::fs::read(&path).unwrap(), b"same");
        std::fs::remove_file(&path).unwrap();
    }

    #[test]
    fn write_if_changed_rewrites_changed_files() {
        let path = temp_path("changed");
        assert!(write_if_changed(&path, b"old").unwrap());
        assert!(write_if_changed(&path, b"new").unwrap());
        assert_eq!(std::fs::read(&path).unwrap(), b"new");
        std::fs::remove_file(&path).unwrap();
    }
}
