#![allow(
    dead_code,
    reason = "test helper toolbox; helpers may sit unused between runs"
)]
pub mod libc_shim;

use std::collections::BTreeMap;
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::mpsc;
use std::thread;

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

fn c23_clang_args() -> String {
    let existing = std::env::var("SLATE_CLANG_ARGS").unwrap_or_default();
    format!("{existing} -std=c23").trim().to_string()
}

fn ensure_c23_clang_args() {
    static ONCE: std::sync::Once = std::sync::Once::new();
    ONCE.call_once(|| {
        unsafe { std::env::set_var("SLATE_CLANG_ARGS", c23_clang_args()) };
    });
}

fn aligned_path() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("vendor/aligned")
}

fn generated_crate_manifest(name: &str) -> String {
    format!(
        r#"[package]
name = "{name}"
version = "0.0.0"
edition = "2024"

[dependencies]
libc = "0.2"
aligned = {{ path = "{}" }}

[build-dependencies]
cc = "1"

[profile.dev]
overflow-checks = false
panic = "unwind"
codegen-units = 256
"#,
        aligned_path().display()
    )
}

pub fn test_cache_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("target/test-cache")
}

fn test_target_dir_for_project(project: &std::path::Path) -> std::path::PathBuf {
    let manifest_dir = Path::new(env!("CARGO_MANIFEST_DIR"));
    let key: String = project
        .strip_prefix(manifest_dir)
        .unwrap_or(project)
        .to_string_lossy()
        .chars()
        .map(|c| if c.is_ascii_alphanumeric() { c } else { '-' })
        .collect();
    let td = test_cache_root().join(key);
    if let Err(e) = std::fs::create_dir_all(&td) {
        eprintln!("could not create test target dir {}: {}", td.display(), e);
    }
    unsafe { std::env::set_var("CARGO_TARGET_DIR", td.as_os_str()) }
    td
}

pub fn test_jobs() -> usize {
    std::env::var("SLATE_TEST_JOBS")
        .ok()
        .and_then(|value| value.parse().ok())
        .filter(|jobs| *jobs > 0)
        .unwrap_or_else(|| {
            thread::available_parallelism()
                .map(usize::from)
                .unwrap_or(1)
                .div_ceil(4)
                .clamp(1, 8)
        })
}

pub fn parallel_map<T, R, F>(items: &[T], f: F) -> Vec<R>
where
    T: Sync,
    R: Send,
    F: Fn(&T) -> R + Sync,
{
    parallel_map_with_jobs(items, test_jobs(), f)
}

pub fn parallel_map_with_jobs<T, R, F>(items: &[T], jobs: usize, f: F) -> Vec<R>
where
    T: Sync,
    R: Send,
    F: Fn(&T) -> R + Sync,
{
    if items.is_empty() {
        return Vec::new();
    }
    let jobs = jobs.max(1).min(items.len());
    let next = AtomicUsize::new(0);
    let (tx, rx) = mpsc::channel();
    thread::scope(|scope| {
        for _ in 0..jobs {
            let tx = tx.clone();
            let next = &next;
            let f = &f;
            scope.spawn(move || {
                loop {
                    let index = next.fetch_add(1, Ordering::Relaxed);
                    if index >= items.len() {
                        break;
                    }
                    if tx.send((index, f(&items[index]))).is_err() {
                        break;
                    }
                }
            });
        }
    });
    drop(tx);
    let mut results: Vec<Option<R>> = (0..items.len()).map(|_| None).collect();
    for (index, result) in rx {
        results[index] = Some(result);
    }
    results
        .into_iter()
        .map(|result| result.expect("parallel worker did not produce a result"))
        .collect()
}

pub fn compile_c(src: &Path, out: &Path) -> Result<(), String> {
    compile_c_with_args(src, out, &[])
}

pub fn compile_c_with_args(src: &Path, out: &Path, extra_args: &[String]) -> Result<(), String> {
    let o = Command::new(cc())
        .args(["-O0", "-std=c23"])
        .args(extra_args)
        .arg("-o")
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
        .args(["-O0", "-std=c23", "-o"])
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

/// Invoke `slate translate-project <dir> <crate_dir>`, writing a Cargo crate
/// (Cargo.toml, vendored `aligned`, one Rust module per C translation unit
/// under `src/`; the unit with `main` becomes `src/main.rs`) at `crate_dir`.
pub fn translate_project(dir: &Path, crate_dir: &Path) -> Result<(), String> {
    let o = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-project")
        .arg(dir)
        .arg(crate_dir)
        .env("SLATE_CLANG_ARGS", c23_clang_args())
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

/// Build the Cargo crate `translate-project` wrote at `crate_dir`. The crate
/// is already self-contained (Cargo.toml, vendored `aligned`, a shim build.rs
/// if needed), so this just invokes `cargo build` and reads back the produced
/// binary's path from cargo's own JSON build messages, rather than guessing
/// the binary name from the crate's package name.
pub fn compile_rs_project(crate_dir: &Path) -> Result<PathBuf, String> {
    let target_dir = test_target_dir_for_project(crate_dir);
    std::fs::create_dir_all(&target_dir)
        .map_err(|e| format!("create {}: {e}", target_dir.display()))?;
    let o = Command::new(cargo())
        .args([
            "build",
            "--quiet",
            "--message-format=json",
            "--manifest-path",
        ])
        .arg(crate_dir.join("Cargo.toml"))
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
    for line in String::from_utf8_lossy(&o.stdout).lines() {
        let Ok(message) = serde_json::from_str::<serde_json::Value>(line) else {
            continue;
        };
        if message.get("reason").and_then(serde_json::Value::as_str) != Some("compiler-artifact") {
            continue;
        }
        let is_bin = message["target"]["kind"]
            .as_array()
            .is_some_and(|kinds| kinds.iter().any(|kind| kind.as_str() == Some("bin")));
        if let (true, Some(executable)) = (
            is_bin,
            message
                .get("executable")
                .and_then(serde_json::Value::as_str),
        ) {
            return Ok(PathBuf::from(executable));
        }
    }
    Err(format!(
        "cargo build at {} did not report a binary artifact",
        crate_dir.display()
    ))
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
        generated_crate_manifest(package),
    )
    .map_err(|e| format!("write Cargo.toml: {e}"))?;
    std::fs::copy(src, project.join("src/main.rs"))
        .map_err(|e| format!("copy {} to cargo project: {e}", src.display()))?;
    write_long_double_shim(&project)?;

    let target_dir = test_target_dir_for_project(&project);
    std::fs::create_dir_all(&target_dir)
        .map_err(|e| format!("create {}: {e}", target_dir.display()))?;
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

pub struct RustCase {
    pub name: String,
    pub rs_src: PathBuf,
}

#[derive(Default, Clone)]
pub struct RunConfig {
    pub args: Vec<String>,
    pub stdin: Vec<u8>,
    pub env: BTreeMap<String, String>,
    pub compare_stderr: bool,
    pub timeout_seconds: Option<u64>,
    pub c_args: Vec<String>,
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
pub fn compare_batch(cases: &[Case], work_dir: &Path) -> Vec<(String, Result<(), String>)> {
    compare_batch_with_jobs(cases, work_dir, test_jobs())
}

pub fn compare_batch_with_jobs(
    cases: &[Case],
    work_dir: &Path,
    jobs: usize,
) -> Vec<(String, Result<(), String>)> {
    if cases.is_empty() {
        return Vec::new();
    }

    // preserve the project so Cargo can reuse target artifacts across runs.
    let project = work_dir.join("batch_cargo");
    let bin_dir = project.join("src/bin");
    let rust_cases: Vec<RustCase> = cases
        .iter()
        .map(|case| RustCase {
            name: case.name.clone(),
            rs_src: case.rs_src.clone(),
        })
        .collect();

    let batch_bins = build_batch(&rust_cases, &project, &bin_dir, jobs);
    let target_dir = test_target_dir_for_project(&project);

    parallel_map_with_jobs(cases, jobs, |case| {
        let result = (|| {
            let bn = bin_name(&case.name);
            let batch_bin = target_dir.join("debug").join(&bn);
            let rs_bin = if batch_bin.is_file() {
                batch_bin
            } else {
                return Err(match &batch_bins {
                    Ok(()) => format!("Rust batch build did not produce {}", batch_bin.display()),
                    Err(error) => format!("Rust batch build failed:\n{error}"),
                });
            };
            let c_bin = work_dir.join(format!("{}_c", bn));
            compile_c_with_args(&case.c_src, &c_bin, &case.config.c_args)?;
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
}

pub fn compile_rs_batch(cases: &[RustCase], work_dir: &Path) -> Vec<(String, Result<(), String>)> {
    if cases.is_empty() {
        return Vec::new();
    }
    let project = work_dir.join("batch_cargo");
    let bin_dir = project.join("src/bin");
    let batch = build_batch(cases, &project, &bin_dir, test_jobs());
    let target_dir = test_target_dir_for_project(&project);
    parallel_map(cases, |case| {
        let bn = bin_name(&case.name);
        let bin = target_dir.join("debug").join(&bn);
        let result = if bin.is_file() {
            Ok(())
        } else {
            Err(match &batch {
                Ok(()) => format!("Rust batch build did not produce {}", bin.display()),
                Err(error) => format!("Rust batch build failed:\n{error}"),
            })
        };
        (case.name.clone(), result)
    })
}

/// Write one crate with a `src/bin/<name>.rs` per case and build them together.
fn build_batch(
    cases: &[RustCase],
    project: &Path,
    bin_dir: &Path,
    jobs: usize,
) -> Result<(), String> {
    std::fs::create_dir_all(bin_dir).map_err(|e| format!("create {}: {e}", bin_dir.display()))?;
    write_if_changed(
        project.join("Cargo.toml"),
        generated_crate_manifest("slate_batch").as_bytes(),
    )
    .map_err(|e| format!("write Cargo.toml: {e}"))?;

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
    write_long_double_shim(project)?;

    let target_dir = test_target_dir_for_project(project);
    std::fs::create_dir_all(&target_dir)
        .map_err(|e| format!("create {}: {e}", target_dir.display()))?;
    let o = Command::new(cargo())
        .args(["build", "--quiet", "--keep-going", "--manifest-path"])
        .arg(project.join("Cargo.toml"))
        .arg("--jobs")
        .arg(jobs.max(1).to_string())
        .arg("--target-dir")
        .arg(&target_dir)
        .output()
        .map_err(|e| format!("spawn {}: {e}", cargo()))?;
    if o.status.success() {
        Ok(())
    } else {
        Err(String::from_utf8_lossy(&o.stderr).into_owned())
    }
}

pub struct MultiBinCase {
    pub name: String,
    pub main_rs: PathBuf,
    pub common_rs: PathBuf,
}

pub fn multi_bin_batch_path(project: &Path, name: &str) -> PathBuf {
    test_target_dir_for_project(project)
        .join("debug")
        .join(bin_name(name))
}

pub fn build_multi_bin_batch(cases: &[MultiBinCase], project: &Path) -> Result<String, String> {
    let bin_dir = project.join("src/bin");
    std::fs::create_dir_all(&bin_dir).map_err(|e| format!("create {}: {e}", bin_dir.display()))?;
    write_if_changed(
        project.join("Cargo.toml"),
        generated_crate_manifest("slate_multi_batch").as_bytes(),
    )
    .map_err(|e| format!("write Cargo.toml: {e}"))?;

    let mut expected = BTreeMap::new();
    for case in cases {
        let case_dir = bin_dir.join(bin_name(&case.name));
        std::fs::create_dir_all(&case_dir)
            .map_err(|e| format!("create {}: {e}", case_dir.display()))?;
        for (src, file) in [(&case.main_rs, "main.rs"), (&case.common_rs, "common.rs")] {
            let dest = case_dir.join(file);
            expected.insert(dest.clone(), ());
            let contents = std::fs::read(src)
                .map_err(|e| format!("read {} for batch crate: {e}", src.display()))?;
            write_if_changed(&dest, &contents)
                .map_err(|e| format!("write {} to batch crate: {e}", dest.display()))?;
        }
    }

    let expected_dirs: std::collections::BTreeSet<PathBuf> = expected
        .keys()
        .filter_map(|dest| dest.parent().map(Path::to_path_buf))
        .collect();
    for entry in
        std::fs::read_dir(&bin_dir).map_err(|e| format!("read {}: {e}", bin_dir.display()))?
    {
        let path = entry
            .map_err(|e| format!("read {} entry: {e}", bin_dir.display()))?
            .path();
        if path.is_dir() && !expected_dirs.contains(&path) {
            std::fs::remove_dir_all(&path)
                .map_err(|e| format!("remove stale {}: {e}", path.display()))?;
        }
    }
    write_long_double_shim(project)?;

    for case in cases {
        let _ = std::fs::remove_file(multi_bin_batch_path(project, &case.name));
    }

    let target_dir = test_target_dir_for_project(project);
    std::fs::create_dir_all(&target_dir)
        .map_err(|e| format!("create {}: {e}", target_dir.display()))?;
    let o = Command::new(cargo())
        .args(["build", "--quiet", "--keep-going", "--manifest-path"])
        .arg(project.join("Cargo.toml"))
        .arg("--jobs")
        .arg(test_jobs().to_string())
        .arg("--target-dir")
        .arg(&target_dir)
        .output()
        .map_err(|e| format!("spawn {}: {e}", cargo()))?;
    Ok(String::from_utf8_lossy(&o.stderr).into_owned())
}

const RUST_UNCAUGHT_PANIC_EXIT: i32 = 101;

pub fn compare_runs(c: &Run, r: &Run, compare_stderr: bool) -> Result<(), String> {
    let exit_matches =
        c.exit == r.exit || (c.exit.is_none() && r.exit == Some(RUST_UNCAUGHT_PANIC_EXIT));
    if !exit_matches {
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
    println!("cargo:rerun-if-changed=src/slate_long_double.c");
    cc::Build::new()
        .file("src/slate_long_double.c")
        .compile("slate_long_double");
}
"#
        .as_bytes(),
    )
    .map_err(|e| format!("write build.rs: {e}"))?;

    let mut source = String::from(
        r#"#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void __slate_strtold(char *nptr, char **endptr, double *out) {
    *out = (double)strtold(nptr, endptr);
}
"#,
    );
    for name in collect_long_double_shim_names(&project.join("src"))? {
        if let Some(trampoline) = render_long_double_shim_trampoline(&name) {
            source.push('\n');
            source.push_str(&trampoline);
        }
    }
    write_if_changed(project.join("src/slate_long_double.c"), source.as_bytes())
        .map(|_| ())
        .map_err(|e| format!("write slate_long_double.c: {e}"))
}

fn collect_long_double_shim_names(
    dir: &Path,
) -> Result<std::collections::BTreeSet<String>, String> {
    let mut names = std::collections::BTreeSet::new();
    if !dir.exists() {
        return Ok(names);
    }
    for entry in std::fs::read_dir(dir).map_err(|e| format!("read {}: {e}", dir.display()))? {
        let path = entry
            .map_err(|e| format!("read {} entry: {e}", dir.display()))?
            .path();
        if path.is_dir() {
            names.extend(collect_long_double_shim_names(&path)?);
        } else if path.extension().and_then(|e| e.to_str()) == Some("rs") {
            let text = std::fs::read_to_string(&path)
                .map_err(|e| format!("read {}: {e}", path.display()))?;
            extract_long_double_shim_names(&text, &mut names);
        }
    }
    Ok(names)
}

fn extract_long_double_shim_names(text: &str, names: &mut std::collections::BTreeSet<String>) {
    const PREFIX: &str = "__slate_";
    let mut rest = text;
    while let Some(start) = rest.find(PREFIX) {
        let token_start = &rest[start..];
        let end = token_start
            .find(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))
            .unwrap_or(token_start.len());
        let token = &token_start[..end];
        if token[PREFIX.len()..].contains("__") {
            names.insert(token.to_string());
        }
        rest = &token_start[end.max(1)..];
    }
}

fn shim_tag_c_type(tag: &str) -> String {
    if let Some(inner) = tag.strip_prefix('p') {
        return match inner {
            "i8" | "u8" => "char *".to_string(),
            "x" => "void *".to_string(),
            other => format!("{} *", shim_tag_c_type(other)),
        };
    }
    match tag {
        "i8" => "signed char",
        "u8" => "unsigned char",
        "i16" => "short",
        "u16" => "unsigned short",
        "i32" => "int",
        "u32" => "unsigned int",
        "i64" => "long long",
        "u64" => "unsigned long long",
        "isize" => "long",
        "usize" => "unsigned long",
        "f32" => "float",
        "f64" => "double",
        "bool" => "_Bool",
        "ld" => "double",
        _ => "void *",
    }
    .to_string()
}

fn render_long_double_shim_trampoline(name: &str) -> Option<String> {
    let rest = name.strip_prefix("__slate_")?;
    let sep = rest.find("__")?;
    let callee = &rest[..sep];
    let tags: Vec<&str> = rest[sep + 2..].split('_').collect();
    let params = tags
        .iter()
        .enumerate()
        .map(|(i, tag)| format!("{} _{i}", shim_tag_c_type(tag)))
        .collect::<Vec<_>>()
        .join(", ");
    let args = tags
        .iter()
        .enumerate()
        .map(|(i, tag)| {
            if *tag == "ld" {
                format!("(long double)_{i}")
            } else {
                format!("_{i}")
            }
        })
        .collect::<Vec<_>>()
        .join(", ");
    Some(format!(
        "int {name}({params}) {{\n    return {callee}({args});\n}}\n"
    ))
}

pub fn write_if_changed(path: impl AsRef<Path>, contents: &[u8]) -> std::io::Result<bool> {
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
    let mut cmd = if let Some(seconds) = config.timeout_seconds {
        let mut cmd = Command::new("timeout");
        cmd.args(["--kill-after=1", &format!("{seconds}s")])
            .arg(bin);
        cmd
    } else {
        Command::new(bin)
    };
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
    if let Some(seconds) = config.timeout_seconds
        && matches!(o.status.code(), Some(124 | 137))
    {
        return Err(format!(
            "run {} timed out after {} seconds",
            bin.display(),
            seconds
        ));
    }
    Ok(Run {
        stdout: o.stdout,
        stderr: o.stderr,
        exit: o.status.code(),
    })
}

pub fn translate(c_src: &Path, rs_out: &Path) -> Result<(), String> {
    translate_with_args(c_src, rs_out, &[])
}

pub fn translate_with_args(
    c_src: &Path,
    rs_out: &Path,
    extra_args: &[String],
) -> Result<(), String> {
    ensure_c23_clang_args();
    let rust =
        slate::api::translate_with_args(c_src, extra_args).map_err(|error| error.to_string())?;
    write_if_changed(rs_out, rust.as_bytes())
        .map(|_| ())
        .map_err(|e| format!("write {}: {e}", rs_out.display()))
}
