//! Frontend: turn a C source file into ClangIR in MLIR *generic* form, the
//! regular `"op"(operands) <{attrs}> ({regions}) : type` syntax the parser reads.
//!
//! Tool paths default to the local CIR-enabled build and are overridable:
//!   SLATE_CLANG    (default ~/llvm-project/build-cir/bin/clang)
//!   SLATE_CIR_OPT  (default ~/llvm-project/build-cir/bin/cir-opt)

use std::path::Path;
use std::process::{Command, Stdio};

fn home() -> String {
    std::env::var("HOME").expect("HOME not set")
}

fn clang() -> String {
    std::env::var("SLATE_CLANG")
        .unwrap_or_else(|_| format!("{}/llvm-project/build-cir/bin/clang", home()))
}

fn cir_opt() -> String {
    std::env::var("SLATE_CIR_OPT")
        .unwrap_or_else(|_| format!("{}/llvm-project/build-cir/bin/cir-opt", home()))
}

pub fn target_args() -> Vec<String> {
    let mut args = Vec::new();
    if let Ok(target) = std::env::var("SLATE_TARGET") {
        if !target.trim().is_empty() {
            args.push("-target".into());
            args.push(target);
        }
    }
    if let Ok(extra) = std::env::var("SLATE_CLANG_ARGS") {
        args.extend(extra.split_whitespace().map(str::to_string));
    }
    args
}

/// Emit high-level ClangIR (pre-CFG-flattening, passes disabled) for `src` and
/// return it in MLIR generic form.
pub fn emit_generic(src: &Path) -> Result<String, String> {
    emit_generic_with_args(src, &[])
}

pub fn emit_generic_with_args(src: &Path, extra_args: &[String]) -> Result<String, String> {
    let mut cmd = Command::new(clang());
    cmd.args([
        "-fclangir",
        "-emit-cir",
        "-Xclang",
        "-disable-llvm-passes",
        "-S",
        "-o",
        "-",
    ])
    .args(target_args())
    .args(extra_args)
    .arg(src)
    .stderr(Stdio::piped());
    let clang_out = cmd
        .output()
        .map_err(|e| format!("spawn {}: {e}", clang()))?;
    if !clang_out.status.success() {
        return Err(format!(
            "clang -emit-cir failed:\n{}",
            String::from_utf8_lossy(&clang_out.stderr)
        ));
    }

    let mut child = Command::new(cir_opt())
        .arg("--mlir-print-op-generic")
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .map_err(|e| format!("spawn {}: {e}", cir_opt()))?;
    use std::io::Write;
    child
        .stdin
        .take()
        .unwrap()
        .write_all(&clang_out.stdout)
        .map_err(|e| format!("write to cir-opt: {e}"))?;
    let out = child
        .wait_with_output()
        .map_err(|e| format!("wait cir-opt: {e}"))?;
    if !out.status.success() {
        return Err(format!(
            "cir-opt failed:\n{}",
            String::from_utf8_lossy(&out.stderr)
        ));
    }
    Ok(String::from_utf8_lossy(&out.stdout).into_owned())
}
