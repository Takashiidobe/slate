#![allow(
    dead_code,
    reason = "test helper toolbox; helpers may sit unused between runs"
)]
pub mod filecheck;
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

pub fn fixture_clang_arg_overrides(name: &str) -> Vec<String> {
    match name {
        "goto_temp_cross_state" => vec!["-O2".to_string()],
        "ptr_param_field_addr_of_mut" => vec!["-O2".to_string()],
        "branch_hint_builtins" => vec!["-O1".to_string()],
        "gnu_asm_register_variable" => vec!["-std=gnu23".to_string()],
        "c23_typeof_unqual" => vec!["-std=gnu23".to_string()],
        _ => Vec::new(),
    }
}

pub fn fixture_c_ref_std_override(name: &str) -> Option<String> {
    match name {
        "gnu_asm_register_variable" => Some("-std=gnu23".to_string()),
        "c23_typeof_unqual" => Some("-std=gnu23".to_string()),
        _ => None,
    }
}

pub struct CrossTarget {
    pub rust_triple: &'static str,
    pub cc: String,
    pub cc_extra_args: Vec<String>,
    pub cargo_linker_env: String,
    pub linker: String,
    pub qemu: String,
    pub qemu_args: Vec<String>,
}

fn cc() -> String {
    std::env::var("SLATE_CC").unwrap_or_else(|_| "clang".into())
}

fn cargo() -> String {
    std::env::var("SLATE_CARGO").unwrap_or_else(|_| "cargo".into())
}

fn std_clang_args(std: &str) -> String {
    let existing = std::env::var("SLATE_CLANG_ARGS").unwrap_or_default();
    format!("{existing} -std={std}").trim().to_string()
}

fn c23_clang_args() -> String {
    std_clang_args("c23")
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

fn bitint_path() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("vendor/bitint")
}

fn num_complex_path() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("vendor/num-complex")
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
bitint = {{ path = "{}" }}
num-complex = {{ path = "{}", default-features = false }}
bitfields = "3.0.0"

[build-dependencies]
cc = "1"

[profile.dev]
debug = 0
overflow-checks = false
panic = "unwind"
codegen-units = 256
"#,
        aligned_path().display(),
        bitint_path().display(),
        num_complex_path().display()
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
    compile_c_with_args_for_target(src, out, extra_args, None)
}

pub fn compile_c_with_args_for_target(
    src: &Path,
    out: &Path,
    extra_args: &[String],
    cross: Option<&CrossTarget>,
) -> Result<(), String> {
    let cc_bin = cross.map_or_else(cc, |cross| cross.cc.clone());
    let cc_extra_args = cross
        .map(|cross| cross.cc_extra_args.clone())
        .unwrap_or_default();
    let cache_key = serde_json::to_string(&(1, src, &cc_bin, &cc_extra_args, extra_args))
        .map_err(|e| format!("encode C cache key: {e}"))?;
    let mut inputs = vec![src];
    inputs.extend(
        extra_args
            .iter()
            .map(Path::new)
            .filter(|argument| argument.is_file()),
    );
    compile_c_cached(&inputs, out, &cache_key, "C compile failed", |temporary| {
        Command::new(&cc_bin)
            .args(["-O0", "-std=c23"])
            .args(&cc_extra_args)
            .args(extra_args)
            .arg("-o")
            .arg(temporary)
            .arg(src)
            .arg("-lm")
            .output()
            .map_err(|e| format!("spawn {cc_bin}: {e}"))
    })
}

fn c_cache_sidecar(binary: &Path, suffix: &str) -> PathBuf {
    let mut name = binary.file_name().unwrap_or_default().to_os_string();
    name.push(suffix);
    binary.with_file_name(name)
}

/// Compile a single C translation unit into an object file for later linking.
pub fn compile_c_object(src: &Path, out: &Path) -> Result<(), String> {
    let cache_key = serde_json::to_string(&(2, src, cc()))
        .map_err(|e| format!("encode C object cache key: {e}"))?;
    compile_c_cached(
        &[src],
        out,
        &cache_key,
        "C object compile failed",
        |temporary| {
            Command::new(cc())
                .args(["-O0", "-std=c23", "-c", "-o"])
                .arg(temporary)
                .arg(src)
                .output()
                .map_err(|e| format!("spawn {}: {e}", cc()))
        },
    )
}

/// Compile several C translation units together into one binary (cross-TU link).
pub fn compile_c_multi(srcs: &[PathBuf], out: &Path) -> Result<(), String> {
    compile_c_multi_with_std(srcs, out, "c23")
}

pub fn compile_c_multi_with_std(srcs: &[PathBuf], out: &Path, std: &str) -> Result<(), String> {
    compile_c_multi_with_std_and_include(srcs, out, std, None)
}

pub fn compile_c_multi_with_std_and_include(
    srcs: &[PathBuf],
    out: &Path,
    std: &str,
    include_dir: Option<&Path>,
) -> Result<(), String> {
    compile_c_multi_with_std_include_and_args(srcs, out, std, include_dir, &[])
}

pub fn compile_c_multi_with_std_include_and_args(
    srcs: &[PathBuf],
    out: &Path,
    std: &str,
    include_dir: Option<&Path>,
    extra_args: &[String],
) -> Result<(), String> {
    let cache_key = serde_json::to_string(&(3, srcs, cc(), std, include_dir, extra_args))
        .map_err(|e| format!("encode multi-file C cache key: {e}"))?;
    let inputs: Vec<&Path> = srcs.iter().map(PathBuf::as_path).collect();
    compile_c_cached(&inputs, out, &cache_key, "C compile failed", |temporary| {
        Command::new(cc())
            .args(["-O0", &format!("-std={std}"), "-fcommon", "-o"])
            .arg(temporary)
            .args(srcs)
            .args(include_dir.map(|dir| format!("-I{}", dir.display())))
            .args(extra_args)
            .arg("-lm")
            .output()
            .map_err(|e| format!("spawn {}: {e}", cc()))
    })
}

fn compile_c_cached(
    inputs: &[&Path],
    out: &Path,
    cache_key: &str,
    failure: &str,
    compile: impl FnOnce(&Path) -> Result<std::process::Output, String>,
) -> Result<(), String> {
    let stamp = c_cache_sidecar(out, ".slate-c-cache");
    let newest_input = inputs
        .iter()
        .map(|input| {
            std::fs::metadata(input)
                .and_then(|metadata| metadata.modified())
                .map_err(|e| format!("stat {}: {e}", input.display()))
        })
        .collect::<Result<Vec<_>, _>>()?
        .into_iter()
        .max()
        .unwrap_or(std::time::SystemTime::UNIX_EPOCH);
    let cache_is_fresh = out.is_file()
        && std::fs::read_to_string(&stamp).is_ok_and(|stored| stored == cache_key)
        && std::fs::metadata(out)
            .and_then(|metadata| metadata.modified())
            .is_ok_and(|output_mtime| output_mtime >= newest_input);
    if cache_is_fresh {
        return Ok(());
    }

    let _ = std::fs::remove_file(&stamp);
    let temporary = c_cache_sidecar(out, &format!(".slate-c-tmp-{}", std::process::id()));
    let _ = std::fs::remove_file(&temporary);
    let output = compile(&temporary)?;
    if !output.status.success() {
        let _ = std::fs::remove_file(&temporary);
        return Err(format!(
            "{failure}:\n{}",
            String::from_utf8_lossy(&output.stderr)
        ));
    }
    if out.exists() {
        std::fs::remove_file(out).map_err(|e| format!("remove {}: {e}", out.display()))?;
    }
    std::fs::rename(&temporary, out)
        .map_err(|e| format!("rename {} to {}: {e}", temporary.display(), out.display()))?;
    write_if_changed(&stamp, cache_key.as_bytes())
        .map_err(|e| format!("write {}: {e}", stamp.display()))?;
    Ok(())
}

/// Invoke `slate translate-project <dir> <crate_dir>`, writing a Cargo crate
/// (Cargo.toml, vendored `aligned`, one Rust module per C translation unit
/// under `src/`; the unit with `main` becomes `src/main.rs`) at `crate_dir`.
pub fn translate_project(dir: &Path, crate_dir: &Path) -> Result<(), String> {
    translate_project_with_clang_args(dir, crate_dir, c23_clang_args())
}

pub fn translate_project_with_std(dir: &Path, crate_dir: &Path, std: &str) -> Result<(), String> {
    translate_project_with_std_and_args(dir, crate_dir, std, &[])
}

pub fn translate_project_with_std_and_args(
    dir: &Path,
    crate_dir: &Path,
    std: &str,
    extra_args: &[String],
) -> Result<(), String> {
    let mut clang_args = format!("{} -I{} -fcommon", std_clang_args(std), dir.display());
    for arg in extra_args {
        clang_args.push(' ');
        clang_args.push_str(arg);
    }
    translate_project_with_clang_args(dir, crate_dir, clang_args)
}

fn translate_project_with_clang_args(
    dir: &Path,
    crate_dir: &Path,
    clang_args: String,
) -> Result<(), String> {
    let o = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-project")
        .arg(dir)
        .arg(crate_dir)
        .env("SLATE_CLANG_ARGS", clang_args)
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

pub fn compile_rs_cargo_with_link(
    src: &Path,
    work_dir: &Path,
    package: &str,
    link_dir: &Path,
) -> Result<PathBuf, String> {
    compile_rs_cargo_with_link_and_shims(src, work_dir, package, link_dir, None)
}

pub fn compile_rs_cargo_with_link_and_shims(
    src: &Path,
    work_dir: &Path,
    package: &str,
    link_dir: &Path,
    shim_source: Option<&str>,
) -> Result<PathBuf, String> {
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

    if link_dir.exists() {
        let dest_dir = project.join("linkfiles");
        std::fs::create_dir_all(&dest_dir)
            .map_err(|e| format!("create {}: {e}", dest_dir.display()))?;
        for entry in
            std::fs::read_dir(link_dir).map_err(|e| format!("read {}: {e}", link_dir.display()))?
        {
            let path = entry
                .map_err(|e| format!("read {} entry: {e}", link_dir.display()))?
                .path();
            if path.is_file() {
                let fname = path.file_name().unwrap();
                std::fs::copy(&path, dest_dir.join(fname))
                    .map_err(|e| format!("copy link file {}: {e}", path.display()))?;
            }
        }
    }

    if let Some(shim_source) = shim_source {
        std::fs::write(
            project.join("build.rs"),
            r#"fn main() {
    if let Ok(entries) = std::fs::read_dir("linkfiles") {
        for e in entries.flatten() {
            let p = e.path();
            if p.is_file() {
                println!("cargo:rerun-if-changed={}", p.display());
                println!("cargo:rustc-link-arg={}", p.display());
            }
        }
    }
    cc::Build::new()
        .file("src/slate_long_double.c")
        .compile("slate_long_double");
}
"#,
        )
        .map_err(|e| format!("write build.rs: {e}"))?;
        std::fs::write(project.join("src/slate_long_double.c"), shim_source)
            .map_err(|e| format!("write slate_long_double.c: {e}"))?;
    } else {
        write_long_double_shim(&project)?;
        let build_rs = r#"fn main() {
    if let Ok(entries) = std::fs::read_dir("linkfiles") {
        for e in entries.flatten() {
            let p = e.path();
            if p.is_file() {
                println!("cargo:rerun-if-changed={}", p.display());
                println!("cargo:rustc-link-arg={}", p.display());
            }
        }
    }
    cc::Build::new()
        .file("src/slate_long_double.c")
        .compile("slate_long_double");
}
"#;
        std::fs::write(project.join("build.rs"), build_rs)
            .map_err(|e| format!("write build.rs: {e}"))?;
    }

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

pub fn compile_rs_cargo_with_syslibs(
    src: &Path,
    work_dir: &Path,
    package: &str,
    libs: &[String],
    shim_source: Option<&str>,
) -> Result<PathBuf, String> {
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

    let link_lib_lines: String = libs
        .iter()
        .map(|lib| format!("    println!(\"cargo:rustc-link-lib={lib}\");\n"))
        .collect();
    let shim_source = shim_source.unwrap_or("");
    std::fs::write(
        project.join("build.rs"),
        format!(
            r#"fn main() {{
{link_lib_lines}    cc::Build::new()
        .file("src/slate_long_double.c")
        .compile("slate_long_double");
}}
"#
        ),
    )
    .map_err(|e| format!("write build.rs: {e}"))?;
    std::fs::write(project.join("src/slate_long_double.c"), shim_source)
        .map_err(|e| format!("write slate_long_double.c: {e}"))?;

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

pub struct BatchBuild {
    artifacts: BTreeMap<String, PathBuf>,
    errors: BTreeMap<String, String>,
    stderr: String,
}

impl BatchBuild {
    fn error_for(&self, name: &str) -> String {
        if let Some(error) = self.errors.get(name) {
            return error.clone();
        }
        if !self.stderr.trim().is_empty() {
            return self.stderr.clone();
        }
        format!("Cargo did not report a successful bin artifact for {name}")
    }

    pub fn executable(&self, name: &str) -> Result<PathBuf, String> {
        let name = bin_name(name);
        self.artifacts
            .get(&name)
            .cloned()
            .ok_or_else(|| self.error_for(&name))
    }
}

#[derive(Default, Clone)]
pub struct RunConfig {
    pub args: Vec<String>,
    pub stdin: Vec<u8>,
    pub env: BTreeMap<String, String>,
    pub compare_stderr: bool,
    pub timeout_seconds: Option<u64>,
    pub c_args: Vec<String>,
    pub extra_files: Vec<PathBuf>,
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
    compare_batch_with_jobs_for_target(cases, work_dir, jobs, None)
}

pub fn compare_batch_for_target(
    cases: &[Case],
    work_dir: &Path,
    cross: Option<&CrossTarget>,
) -> Vec<(String, Result<(), String>)> {
    compare_batch_with_jobs_for_target(cases, work_dir, test_jobs(), cross)
}

pub fn compare_batch_with_jobs_for_target(
    cases: &[Case],
    work_dir: &Path,
    jobs: usize,
    cross: Option<&CrossTarget>,
) -> Vec<(String, Result<(), String>)> {
    if cases.is_empty() {
        return Vec::new();
    }

    let project = work_dir.join("batch_cargo");
    let bin_dir = project.join("src/bin");
    let rust_cases: Vec<RustCase> = cases
        .iter()
        .map(|case| RustCase {
            name: case.name.clone(),
            rs_src: case.rs_src.clone(),
        })
        .collect();

    let batch_bins = build_batch(&rust_cases, &project, &bin_dir, jobs, cross);

    parallel_map_with_jobs(cases, jobs, |case| {
        let result = (|| {
            let bn = bin_name(&case.name);
            let rs_bin = match &batch_bins {
                Ok(build) => build
                    .executable(&bn)
                    .map_err(|error| format!("Rust batch build failed:\n{error}"))?,
                Err(error) => return Err(format!("Rust batch build failed:\n{error}")),
            };
            let c_bin = work_dir.join(format!("{}_c", bn));
            compile_c_with_args_for_target(&case.c_src, &c_bin, &case.config.c_args, cross)?;
            let run_dir = work_dir.join("runs").join(&bn);
            if run_dir.exists() {
                std::fs::remove_dir_all(&run_dir)
                    .map_err(|e| format!("remove {}: {e}", run_dir.display()))?;
            }
            std::fs::create_dir_all(&run_dir)
                .map_err(|e| format!("create {}: {e}", run_dir.display()))?;
            for extra in &case.config.extra_files {
                let name = extra
                    .file_name()
                    .ok_or_else(|| format!("extra file has no name: {}", extra.display()))?;
                let dest_dir = run_dir.join("src/functional");
                std::fs::create_dir_all(&dest_dir)
                    .map_err(|e| format!("create {}: {e}", dest_dir.display()))?;
                std::fs::copy(extra, dest_dir.join(name))
                    .map_err(|e| format!("stage {}: {e}", extra.display()))?;
            }
            compare_runs(
                &run_with_config_for_target(&c_bin, &case.config, &run_dir, cross)?,
                &run_with_config_for_target(&rs_bin, &case.config, &run_dir, cross)?,
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
    let batch = build_batch(cases, &project, &bin_dir, test_jobs(), None);
    parallel_map(cases, |case| {
        let bn = bin_name(&case.name);
        let result = match &batch {
            Ok(build) => build
                .executable(&bn)
                .map(|_| ())
                .map_err(|error| format!("Rust batch build failed:\n{error}")),
            Err(error) => Err(format!("Rust batch build failed:\n{error}")),
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
    cross: Option<&CrossTarget>,
) -> Result<BatchBuild, String> {
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
    let mut command = Command::new(cargo());
    command
        .args([
            "build",
            "--quiet",
            "--keep-going",
            "--message-format=json",
            "--manifest-path",
        ])
        .arg(project.join("Cargo.toml"))
        .arg("--jobs")
        .arg(jobs.max(1).to_string())
        .arg("--target-dir")
        .arg(&target_dir);
    if let Some(cross) = cross {
        command
            .arg("--target")
            .arg(cross.rust_triple)
            .env(&cross.cargo_linker_env, &cross.linker);
    }
    let o = command
        .output()
        .map_err(|e| format!("spawn {}: {e}", cargo()))?;

    Ok(parse_batch_build(&o))
}

fn parse_batch_build(output: &std::process::Output) -> BatchBuild {
    let mut build = BatchBuild {
        artifacts: BTreeMap::new(),
        errors: BTreeMap::new(),
        stderr: String::from_utf8_lossy(&output.stderr).into_owned(),
    };
    for line in String::from_utf8_lossy(&output.stdout).lines() {
        let Ok(message) = serde_json::from_str::<serde_json::Value>(line) else {
            continue;
        };
        let target = message["target"]["name"].as_str();
        match message["reason"].as_str() {
            Some("compiler-artifact")
                if message["target"]["kind"]
                    .as_array()
                    .is_some_and(|kinds| kinds.iter().any(|kind| kind.as_str() == Some("bin"))) =>
            {
                if let (Some(name), Some(executable)) = (target, message["executable"].as_str()) {
                    build
                        .artifacts
                        .insert(name.to_string(), PathBuf::from(executable));
                }
            }
            Some("compiler-message") if message["message"]["level"].as_str() == Some("error") => {
                if let Some(rendered) = message["message"]["rendered"].as_str() {
                    build.stderr.push_str(rendered);
                    if let Some(name) = target {
                        build
                            .errors
                            .entry(name.to_string())
                            .and_modify(|errors| errors.push_str(rendered))
                            .or_insert_with(|| rendered.to_string());
                    }
                }
            }
            _ => {}
        }
    }
    build
}

pub struct MultiBinCase {
    pub name: String,
    pub main_rs: PathBuf,
    pub common_rs: PathBuf,
    pub types_rs: Option<PathBuf>,
}

pub fn multi_bin_batch_path(project: &Path, name: &str) -> PathBuf {
    test_target_dir_for_project(project)
        .join("debug")
        .join(bin_name(name))
}

pub fn build_multi_bin_batch(cases: &[MultiBinCase], project: &Path) -> Result<BatchBuild, String> {
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
        let mut files = vec![(&case.main_rs, "main.rs"), (&case.common_rs, "common.rs")];
        if let Some(types_rs) = &case.types_rs {
            files.push((types_rs, "types.rs"));
        }
        for (src, file) in files {
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

    let target_dir = test_target_dir_for_project(project);
    std::fs::create_dir_all(&target_dir)
        .map_err(|e| format!("create {}: {e}", target_dir.display()))?;
    let o = Command::new(cargo())
        .args([
            "build",
            "--quiet",
            "--keep-going",
            "--message-format=json",
            "--manifest-path",
        ])
        .arg(project.join("Cargo.toml"))
        .arg("--jobs")
        .arg(test_jobs().to_string())
        .arg("--target-dir")
        .arg(&target_dir)
        .output()
        .map_err(|e| format!("spawn {}: {e}", cargo()))?;
    Ok(parse_batch_build(&o))
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

    let names = collect_long_double_shim_names(&project.join("src"))?;
    let source = slate::frontend::c_shim::render_shim_c_source_for_names(&names);
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
        names.insert(token.to_string());
        rest = &token_start[end.max(1)..];
    }
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
    run_with_config_for_target(bin, config, cwd, None)
}

pub fn run_with_config_for_target(
    bin: &Path,
    config: &RunConfig,
    cwd: &Path,
    cross: Option<&CrossTarget>,
) -> Result<Run, String> {
    let mut cmd = if let Some(seconds) = config.timeout_seconds {
        let mut cmd = Command::new("timeout");
        cmd.args(["--kill-after=1", &format!("{seconds}s")]);
        if let Some(cross) = cross {
            cmd.arg(&cross.qemu).args(&cross.qemu_args);
        }
        cmd.arg(bin);
        cmd
    } else if let Some(cross) = cross {
        let mut cmd = Command::new(&cross.qemu);
        cmd.args(&cross.qemu_args).arg(bin);
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

pub fn dg_option_flags(path: &Path) -> Vec<String> {
    let Ok(text) = std::fs::read_to_string(path) else {
        return Vec::new();
    };
    let mut flags = Vec::new();
    for line in text.lines() {
        if !line.contains("dg-options") && !line.contains("dg-additional-options") {
            continue;
        }
        let Some(quote_start) = line.find('"') else {
            continue;
        };
        let rest = &line[quote_start + 1..];
        let Some(quote_len) = rest.find('"') else {
            continue;
        };
        let quoted = &rest[..quote_len];
        let after = &rest[quote_len + 1..];
        if after.contains("target") && !dg_target_clause_applies(after) {
            continue;
        }
        flags.extend(
            quoted
                .split_whitespace()
                .filter(|flag| is_semantic_dg_flag(flag))
                .map(str::to_string),
        );
    }
    flags
}

fn dg_target_clause_applies(clause: &str) -> bool {
    if clause.contains("ia32") {
        return false;
    }
    clause.contains("x86_64") || clause.contains("i?86") || clause.contains("i386")
}

fn is_semantic_dg_flag(flag: &str) -> bool {
    matches!(
        flag,
        "-fwrapv"
            | "-fno-strict-overflow"
            | "-fno-strict-aliasing"
            | "-fno-trapping-math"
            | "-ffast-math"
            | "-fno-common"
            | "-mno-mmx"
            | "-fsignaling-nans"
            | "-pthread"
    ) || flag.starts_with("-std=")
        || flag.starts_with("-finput-charset=")
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
