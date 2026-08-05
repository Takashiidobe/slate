//! Frontend: turn a C source file into ClangIR in MLIR *generic* form, the
//! regular `"op"(operands) <{attrs}> ({regions}) : type` syntax the parser reads.
//!
//! Tool paths default to the local CIR-enabled build and are overridable:
//!   SLATE_CLANG    (default ~/llvm-project/build-cir/bin/clang)
//!   SLATE_CIR_OPT  (default ~/llvm-project/build-cir/bin/cir-opt)
//!   SLATE_LIBC_SHIM (defaults to the repo's libc-shim/include; a directory
//!                    overrides it; an empty value disables the shim and
//!                    falls back to system libc headers. SLATE_CLANG parses
//!                    with -nostdlibinc -isystem <dir> against whichever
//!                    directory is active, while keeping clang's own builtin
//!                    freestanding headers such as stddef.h/stdint.h/stdatomic.h)

use std::collections::BTreeMap;
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

pub fn libc_shim_dir() -> Option<String> {
    match std::env::var("SLATE_LIBC_SHIM") {
        Ok(dir) if dir.trim().is_empty() => None,
        Ok(dir) => Some(dir),
        Err(_) => Some(format!("{}/libc-shim/include", env!("CARGO_MANIFEST_DIR"))),
    }
}

fn libc_shim_args() -> Vec<String> {
    match libc_shim_dir() {
        Some(dir) => {
            let mut args = vec!["-nostdlibinc".into(), "-isystem".into(), dir];
            for fallback in system_fallback_include_dirs() {
                args.push("-idirafter".into());
                args.push(fallback);
            }
            args
        }
        None => Vec::new(),
    }
}

pub fn clang_resource_dir_include() -> Option<String> {
    static RESOURCE_DIR: std::sync::OnceLock<Option<String>> = std::sync::OnceLock::new();
    RESOURCE_DIR
        .get_or_init(|| {
            let out = Command::new(clang())
                .arg("-print-resource-dir")
                .output()
                .ok()?;
            if !out.status.success() {
                return None;
            }
            let dir = String::from_utf8_lossy(&out.stdout).trim().to_string();
            (!dir.is_empty()).then(|| format!("{dir}/include"))
        })
        .clone()
}

fn system_fallback_include_dirs() -> Vec<String> {
    static DIRS: std::sync::OnceLock<Vec<String>> = std::sync::OnceLock::new();
    DIRS.get_or_init(|| {
        let Ok(out) = Command::new(clang())
            .args(["-E", "-Wp,-v", "-x", "c", "/dev/null"])
            .output()
        else {
            return Vec::new();
        };
        let resource_dir_include = clang_resource_dir_include();
        let stderr = String::from_utf8_lossy(&out.stderr);
        let mut dirs = Vec::new();
        let mut in_list = false;
        for line in stderr.lines() {
            if line.starts_with("#include <...> search starts here") {
                in_list = true;
                continue;
            }
            if !in_list {
                continue;
            }
            if line.starts_with("End of search list") {
                break;
            }
            let dir = line.trim();
            if dir.is_empty() || Some(dir) == resource_dir_include.as_deref() {
                continue;
            }
            dirs.push(dir.to_string());
        }
        dirs
    })
    .clone()
}

pub fn target_args() -> Vec<String> {
    let mut args = libc_shim_args();
    if let Ok(target) = std::env::var("SLATE_TARGET")
        && !target.trim().is_empty()
    {
        if target.contains("musl") {
            args.push("-D__SLATE_LIBC_MUSL=1".into());
        }
        args.push("-target".into());
        args.push(target);
    }
    if let Ok(extra) = std::env::var("SLATE_CLANG_ARGS") {
        args.extend(extra.split_whitespace().map(str::to_string));
    }
    args
}

/// Query the macro environment Clang predefines for `extra_args` (target,
/// `-D`/`-U`, etc.). Used by the preprocessing oracle to decide which
/// conditional branch is active for a given invocation.
pub fn predefined_macros(extra_args: &[String]) -> Result<BTreeMap<String, String>, String> {
    let out = Command::new(clang())
        .args(["-dM", "-E", "-x", "c"])
        .args(target_args())
        .args(extra_args)
        .arg("/dev/null")
        .output()
        .map_err(|e| format!("spawn {}: {e}", clang()))?;
    if !out.status.success() {
        return Err(format!(
            "clang -dM -E failed:\n{}",
            String::from_utf8_lossy(&out.stderr)
        ));
    }
    let mut macros = BTreeMap::new();
    for line in String::from_utf8_lossy(&out.stdout).lines() {
        let mut parts = line.splitn(3, ' ');
        if parts.next() != Some("#define") {
            continue;
        }
        let Some(name) = parts.next() else { continue };
        // function-like macros carry a `(` in the name field; keep the bare name.
        let name = name.split('(').next().unwrap_or(name);
        macros.insert(name.to_string(), parts.next().unwrap_or("").to_string());
    }
    Ok(macros)
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
        "-std=c23",
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

    // TODO: cir-opt could run `--cir-idiom-recognizer` here to raise raw
    // `@memcpy`/`@memset`/`@memmove` calls into `cir.libc.*` ops (easier to
    // pattern-match downstream), but the pass's recognizeStandardLibraryCall is
    // a no-op stub in the current CIR build, so it would raise nothing. Revisit
    // if it lands.
    let mut child = Command::new(cir_opt())
        .arg("--mlir-print-op-generic")
        .arg("--mlir-print-debuginfo")
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
