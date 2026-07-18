//! Emit LLVM IR for a fixture's C source and for its baseline Rust lowering,
//! for alive-tv translation-validation of `src/lower.rs`'s C -> Rust
//! transformation itself (slate-4us epic).
//!
//! Baseline here means `SLATE_RAW_LOWER=1` output: `lower::lower`'s AST
//! printed before any `fixups::apply` rewrite runs. Fixups that change
//! representation (raw pointers -> Box/Vec/slice/String, ...) are not
//! candidates for alive-tv comparison since they change a function's memory
//! layout and ABI, not just its LLVM instructions; the pre-fixup baseline
//! stays a close transliteration of the C (repr(C), raw pointers, libc), so
//! its LLVM IR is directly comparable to clang's for the same function.

use std::io::Read;
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};
use std::time::{Duration, Instant};

/// LLVM IR for one fixture's C source and its baseline Rust lowering,
/// sharing a target triple so alive-tv can compare them.
pub struct LoweringIr {
    pub c_ll: PathBuf,
    pub rs_ll: PathBuf,
}

pub fn emit_lowering_ir(fixture: &Path, work_dir: &Path) -> Result<LoweringIr, String> {
    let stem = fixture
        .file_stem()
        .and_then(|s| s.to_str())
        .ok_or_else(|| format!("bad file stem: {}", fixture.display()))?;
    let dir = work_dir.join(format!("alive_{stem}"));
    std::fs::create_dir_all(&dir).map_err(|e| format!("create {}: {e}", dir.display()))?;

    let target = host_target()?;

    let raw_rs = translate_raw(fixture)?;
    let rs_src = dir.join("baseline.rs");
    std::fs::write(&rs_src, force_extern_c(&raw_rs))
        .map_err(|e| format!("write {}: {e}", rs_src.display()))?;

    let libc = if raw_rs.contains("libc::") {
        Some(ensure_libc_extern(work_dir)?)
    } else {
        None
    };

    let c_ll = dir.join("c.ll");
    emit_c_llvm_ir(fixture, &c_ll, &target)?;
    let rs_ll = dir.join("rs.ll");
    emit_rs_llvm_ir(&rs_src, &rs_ll, &target, libc.as_ref())?;

    Ok(LoweringIr { c_ll, rs_ll })
}

/// Result of running `alive-tv` on one pair of LLVM IR files.
pub struct AliveSummary {
    pub correct: u32,
    pub incorrect: u32,
    pub failed_to_prove: u32,
    pub output: String,
}

impl AliveSummary {
    pub fn ok(&self) -> bool {
        self.correct > 0 && self.incorrect == 0 && self.failed_to_prove == 0
    }

    /// True when alive-tv found zero functions in common between the two
    /// modules — e.g. a fixture whose only logic lives in `main`, which this
    /// harness deliberately excludes from pairing (its C and Rust signatures
    /// don't match: `int main(void)` vs. a `fn main()` that calls
    /// `std::process::exit`). Distinguishing this from a real
    /// failed-to-prove result matters: it means nothing was checked at all,
    /// not that alive-tv tried and came up short.
    pub fn vacuous(&self) -> bool {
        self.correct == 0 && self.incorrect == 0 && self.failed_to_prove == 0
    }
}

/// Outcome of a bounded `alive-tv` run: either it produced a `Summary:`
/// block, or it was killed for running past `timeout` (large-int fixtures in
/// particular can make alive-tv's solver take arbitrarily long even at a
/// modest unroll bound; that's a tool limitation, not evidence of a bug).
pub enum VerifyOutcome {
    Completed(AliveSummary),
    TimedOut,
}

/// Run `alive-tv` on `c_ll`/`rs_ll`, unrolling loops up to `unroll` times and
/// killing the process if it runs past `timeout`. alive-tv needs every loop
/// unrolled to a fixed bound to verify it at all, so a `Completed` result
/// only proves refinement for inputs whose concrete trip count is within
/// that bound (bounded translation validation, not an unbounded proof of the
/// lowering in general).
pub fn verify(
    c_ll: &Path,
    rs_ll: &Path,
    unroll: u32,
    timeout: Duration,
) -> Result<VerifyOutcome, String> {
    let mut child = Command::new(super::alive_tv())
        .arg(format!("--src-unroll={unroll}"))
        .arg(format!("--tgt-unroll={unroll}"))
        .arg(c_ll)
        .arg(rs_ll)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .map_err(|e| format!("spawn alive-tv: {e}"))?;

    let deadline = Instant::now() + timeout;
    let status = loop {
        if let Some(status) = child
            .try_wait()
            .map_err(|e| format!("wait alive-tv: {e}"))?
        {
            break status;
        }
        if Instant::now() >= deadline {
            let _ = child.kill();
            let _ = child.wait();
            return Ok(VerifyOutcome::TimedOut);
        }
        std::thread::sleep(Duration::from_millis(50));
    };

    let mut stdout = String::new();
    let mut stderr = String::new();
    let _ = child.stdout.take().unwrap().read_to_string(&mut stdout);
    let _ = child.stderr.take().unwrap().read_to_string(&mut stderr);
    if !status.success() {
        return Err(format!("alive-tv failed:\n{stdout}\n{stderr}"));
    }
    let summary = parse_summary(&stdout)
        .ok_or_else(|| format!("could not parse alive-tv summary:\n{stdout}"))?;
    Ok(VerifyOutcome::Completed(summary))
}

fn parse_summary(out: &str) -> Option<AliveSummary> {
    Some(AliveSummary {
        correct: extract_count(out, "correct transformations")?,
        incorrect: extract_count(out, "incorrect transformations")?,
        failed_to_prove: extract_count(out, "failed-to-prove transformations")?,
        output: out.to_string(),
    })
}

fn extract_count(out: &str, label: &str) -> Option<u32> {
    out.lines()
        .find_map(|line| line.trim().strip_suffix(label)?.trim().parse().ok())
}

/// Rewrites every top-level fn (including `main`) to a `#[no_mangle] pub
/// extern "C" fn` matching C's symbol/ABI for alive-tv's by-name pairing;
/// `main` also gets `-> i32` with `std::process::exit(EXPR)` turned into
/// `return (EXPR)` to match C's `main` signature. Top-level `static mut`s get
/// the same `#[no_mangle] pub` treatment, since alive-tv also matches globals
/// by name and otherwise sees Rust's mangled symbol as a new global the
/// target introduces, refusing to compare any function that touches it.
fn force_extern_c(src: &str) -> String {
    let mut out = String::with_capacity(src.len());
    for line in src.lines() {
        if line == "fn main() {" {
            out.push_str("#[unsafe(no_mangle)]\npub extern \"C\" fn main() -> i32 {");
        } else if is_toplevel_fn_start(line) {
            if let Some(rest) = line.strip_prefix("unsafe fn ") {
                out.push_str("#[unsafe(no_mangle)]\npub unsafe extern \"C\" fn ");
                out.push_str(rest);
            } else if let Some(rest) = line.strip_prefix("fn ") {
                out.push_str("#[unsafe(no_mangle)]\npub extern \"C\" fn ");
                out.push_str(rest);
            } else {
                out.push_str(line);
            }
        } else if let Some(rest) = line.strip_prefix("static mut ") {
            out.push_str("#[unsafe(no_mangle)]\npub static mut ");
            out.push_str(rest);
        } else {
            out.push_str(line);
        }
        out.push('\n');
    }
    let out = out
        .replace("Option<fn(", "Option<extern \"C\" fn(")
        .replace("std::process::exit(", "return (");
    inject_main_tail_zero(&out)
}

/// Once `main`'s body is used as an i32 tail expression, an unreachable
/// `break 'label;` (no value, from a goto fallback arm) no longer
/// typechecks against it. Appending a trailing `0` puts the body back in
/// statement position, matching its original `-> ()` typing.
fn inject_main_tail_zero(src: &str) -> String {
    let marker = "pub extern \"C\" fn main() -> i32 {";
    let Some(marker_pos) = src.find(marker) else {
        return src.to_string();
    };
    let open_brace = marker_pos + marker.len() - 1;
    let mut depth = 0i32;
    let close_brace = src
        .as_bytes()
        .iter()
        .enumerate()
        .skip(open_brace)
        .find_map(|(i, &b)| {
            match b {
                b'{' => depth += 1,
                b'}' => depth -= 1,
                _ => {}
            }
            (depth == 0).then_some(i)
        })
        .expect("unbalanced braces in main()");
    format!("{}    0\n{}", &src[..close_brace], &src[close_brace..])
}

fn is_toplevel_fn_start(line: &str) -> bool {
    !line.starts_with(char::is_whitespace)
        && (line.starts_with("fn ") || line.starts_with("unsafe fn "))
}

fn rustc() -> String {
    std::env::var("SLATE_RUSTC").unwrap_or_else(|_| "rustc".into())
}

/// Where to find the `libc` crate for a standalone `rustc` invocation
/// (baseline lowering may reference `libc::` types directly, e.g. `c_char`).
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

fn host_target() -> Result<String, String> {
    let o = Command::new(rustc())
        .arg("-vV")
        .output()
        .map_err(|e| format!("spawn rustc -vV: {e}"))?;
    let text = String::from_utf8_lossy(&o.stdout).into_owned();
    text.lines()
        .find_map(|l| l.strip_prefix("host: "))
        .map(str::to_string)
        .ok_or_else(|| format!("rustc -vV did not report a host triple:\n{text}"))
}

fn translate_raw(fixture: &Path) -> Result<String, String> {
    let o = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate")
        .arg(fixture)
        .env("SLATE_RAW_LOWER", "1")
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

fn emit_c_llvm_ir(src: &Path, out_ll: &Path, target: &str) -> Result<(), String> {
    let cc = super::cc();
    let o = Command::new(&cc)
        .args(["-S", "-emit-llvm", "-O0", "-std=c11", "-target"])
        .arg(target)
        .arg("-o")
        .arg(out_ll)
        .arg(src)
        .output()
        .map_err(|e| format!("spawn {cc}: {e}"))?;
    if !o.status.success() {
        return Err(format!(
            "clang --emit-llvm failed:\n{}",
            String::from_utf8_lossy(&o.stderr)
        ));
    }
    Ok(())
}

fn emit_rs_llvm_ir(
    rs_src: &Path,
    out_ll: &Path,
    target: &str,
    libc: Option<&LibcExtern>,
) -> Result<(), String> {
    let mut cmd = Command::new(rustc());
    cmd.args(["--edition", "2024", "--crate-type", "lib"])
        .args(["-C", "opt-level=0"])
        .args(["-C", "overflow-checks=off"])
        .args(["-C", "debug-assertions=off"])
        .args(["-C", "panic=abort"])
        .arg("--target")
        .arg(target)
        .args(["--emit=llvm-ir", "-o"])
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
