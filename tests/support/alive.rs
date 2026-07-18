//! Emit before/after LLVM IR for one skippable fixup pass on a fixture, for
//! alive-tv translation-validation regression testing (slate-4us epic).
//!
//! Both `slate translate` invocations (pass skipped vs. default) and both
//! `rustc` invocations pin the same `--crate-name`/`-C metadata`, so a
//! touched function's mangled symbol is identical across both `.ll` files —
//! required for alive-tv's two-file mode to pair functions by name.

use std::path::{Path, PathBuf};
use std::process::Command;

/// Effect of one skippable fixup pass on one fixture.
pub enum PassEffect {
    /// The pass did not fire: the translated Rust was identical either way,
    /// so no `rustc` invocation was needed.
    Unchanged,
    /// The pass fired; before/after LLVM IR live at these paths.
    Changed {
        before_ll: PathBuf,
        after_ll: PathBuf,
    },
}

/// Run `slate translate` on `fixture` with `pass` skipped and with the
/// default pipeline, then emit LLVM IR for each variant that differs.
pub fn emit_pass_effect(fixture: &Path, pass: &str, work_dir: &Path) -> Result<PassEffect, String> {
    let stem = fixture
        .file_stem()
        .and_then(|s| s.to_str())
        .ok_or_else(|| format!("bad file stem: {}", fixture.display()))?;
    let crate_name = rust_crate_name(stem);

    let before_rs = translate_with_skip(fixture, Some(pass))?;
    let after_rs = translate_with_skip(fixture, None)?;
    if before_rs == after_rs {
        return Ok(PassEffect::Unchanged);
    }

    let dir = work_dir.join(format!("alive_{stem}"));
    std::fs::create_dir_all(&dir).map_err(|e| format!("create {}: {e}", dir.display()))?;
    let before_rs_path = dir.join("before.rs");
    let after_rs_path = dir.join("after.rs");
    std::fs::write(&before_rs_path, force_pub_fns(&before_rs))
        .map_err(|e| format!("write {}: {e}", before_rs_path.display()))?;
    std::fs::write(&after_rs_path, force_pub_fns(&after_rs))
        .map_err(|e| format!("write {}: {e}", after_rs_path.display()))?;

    let libc = if before_rs.contains("libc::") || after_rs.contains("libc::") {
        Some(ensure_libc_extern(work_dir)?)
    } else {
        None
    };
    let before_ll = dir.join("before.ll");
    let after_ll = dir.join("after.ll");
    emit_llvm_ir(&before_rs_path, &before_ll, &crate_name, libc.as_ref())?;
    emit_llvm_ir(&after_rs_path, &after_ll, &crate_name, libc.as_ref())?;

    Ok(PassEffect::Changed {
        before_ll,
        after_ll,
    })
}

/// Force every top-level fn item public so a `--crate-type=lib` compile
/// generates code for it even when nothing in the file calls it (rustc only
/// codegens non-generic private items that are actually reachable, and lib
/// crates root reachability at the crate's public surface, not at `main`).
fn force_pub_fns(src: &str) -> String {
    let mut out = String::with_capacity(src.len());
    for line in src.lines() {
        if !line.starts_with(char::is_whitespace) && is_fn_item_start(line) {
            out.push_str("pub ");
        }
        out.push_str(line);
        out.push('\n');
    }
    out
}

fn is_fn_item_start(line: &str) -> bool {
    let l = line.strip_prefix("pub ").unwrap_or(line);
    if l != line {
        return false; // already pub, nothing to add
    }
    let l = l.strip_prefix("unsafe ").unwrap_or(l);
    let l = match l.strip_prefix("extern \"") {
        Some(rest) => rest.split_once("\" ").map_or(l, |(_, r)| r),
        None => l,
    };
    l.starts_with("fn ")
}

/// A valid `rustc --crate-name` derived from a fixture's file stem (which may
/// contain hyphens or other characters rustc's crate names disallow).
fn rust_crate_name(stem: &str) -> String {
    let mut name: String = stem
        .chars()
        .map(|c| if c.is_ascii_alphanumeric() { c } else { '_' })
        .collect();
    if name.is_empty() || name.starts_with(|c: char| c.is_ascii_digit()) {
        name.insert(0, '_');
    }
    name
}

fn translate_with_skip(fixture: &Path, skip_pass: Option<&str>) -> Result<String, String> {
    let mut cmd = Command::new(env!("CARGO_BIN_EXE_slate"));
    cmd.arg("translate").arg(fixture);
    match skip_pass {
        Some(pass) => {
            cmd.env("SLATE_SKIP_PASS", pass);
        }
        None => {
            cmd.env_remove("SLATE_SKIP_PASS");
        }
    }
    let o = cmd
        .output()
        .map_err(|e| format!("spawn slate translate: {e}"))?;
    if !o.status.success() {
        return Err(format!(
            "translate failed:\n{}",
            String::from_utf8_lossy(&o.stderr)
        ));
    }
    Ok(String::from_utf8_lossy(&o.stdout).into_owned())
}

fn rustc() -> String {
    std::env::var("SLATE_RUSTC").unwrap_or_else(|_| "rustc".into())
}

/// Where to find the `libc` crate for a standalone `rustc` invocation
/// (fixtures may reference `libc::` directly in baseline lowering).
struct LibcExtern {
    rlib: PathBuf,
    deps_dir: PathBuf,
}

/// Build (once per `work_dir`) a scratch crate depending on `libc` and
/// locate its compiled rlib, so direct `rustc` calls can `--extern` it.
fn ensure_libc_extern(work_dir: &Path) -> Result<LibcExtern, String> {
    let project = work_dir.join("alive_libc_probe");
    if !project.join("Cargo.toml").exists() {
        std::fs::create_dir_all(project.join("src"))
            .map_err(|e| format!("create {}: {e}", project.display()))?;
        std::fs::write(
            project.join("Cargo.toml"),
            "[package]\nname = \"alive_libc_probe\"\nversion = \"0.0.0\"\nedition = \"2024\"\n\n[dependencies]\nlibc = \"0.2\"\n",
        )
        .map_err(|e| format!("write Cargo.toml: {e}"))?;
        std::fs::write(project.join("src/lib.rs"), "")
            .map_err(|e| format!("write src/lib.rs: {e}"))?;
    }
    let target_dir = project.join("target");
    let o = Command::new(std::env::var("SLATE_CARGO").unwrap_or_else(|_| "cargo".into()))
        .args(["build", "--quiet", "--manifest-path"])
        .arg(project.join("Cargo.toml"))
        .arg("--target-dir")
        .arg(&target_dir)
        .output()
        .map_err(|e| format!("spawn cargo build (libc probe): {e}"))?;
    if !o.status.success() {
        return Err(format!(
            "cargo build (libc probe) failed:\n{}",
            String::from_utf8_lossy(&o.stderr)
        ));
    }
    let deps_dir = target_dir.join("debug/deps");
    let rlib = std::fs::read_dir(&deps_dir)
        .map_err(|e| format!("read {}: {e}", deps_dir.display()))?
        .filter_map(|e| e.ok())
        .map(|e| e.path())
        .find(|p| {
            p.file_name()
                .and_then(|n| n.to_str())
                .is_some_and(|n| n.starts_with("liblibc-") && n.ends_with(".rlib"))
        })
        .ok_or_else(|| format!("no liblibc-*.rlib under {}", deps_dir.display()))?;
    Ok(LibcExtern { rlib, deps_dir })
}

fn emit_llvm_ir(
    rs_src: &Path,
    out_ll: &Path,
    crate_name: &str,
    libc: Option<&LibcExtern>,
) -> Result<(), String> {
    let mut cmd = Command::new(rustc());
    cmd.args(["--edition", "2024", "--crate-type", "lib"])
        .arg("--crate-name")
        .arg(crate_name)
        .arg("-C")
        .arg(format!("metadata={crate_name}"))
        .args(["-C", "opt-level=0", "--emit=llvm-ir", "-o"])
        .arg(out_ll)
        .arg(rs_src);
    if let Some(libc) = libc {
        cmd.arg("-L")
            .arg(format!("dependency={}", libc.deps_dir.display()))
            .arg("--extern")
            .arg(format!("libc={}", libc.rlib.display()));
    }
    let o = cmd
        .output()
        .map_err(|e| format!("spawn {}: {e}", rustc()))?;
    if !o.status.success() {
        return Err(format!(
            "rustc --emit=llvm-ir failed:\n{}",
            String::from_utf8_lossy(&o.stderr)
        ));
    }
    Ok(())
}
