use std::path::{Path, PathBuf};
use std::process::Command;

pub struct Run {
    stdout: Vec<u8>,
    exit: Option<i32>,
}

fn cc() -> String {
    std::env::var("SLATE_CC").unwrap_or_else(|_| "clang".into())
}

fn cargo() -> String {
    std::env::var("SLATE_CARGO").unwrap_or_else(|_| "cargo".into())
}

pub fn compile_c(src: &Path, out: &Path) -> Result<(), String> {
    let o = Command::new(cc())
        .args(["-O0", "-std=c11", "-o"])
        .arg(out)
        .arg(src)
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
"#
        ),
    )
    .map_err(|e| format!("write Cargo.toml: {e}"))?;
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

pub fn run(bin: &Path) -> Result<Run, String> {
    let o = Command::new(bin)
        .output()
        .map_err(|e| format!("run {}: {e}", bin.display()))?;
    Ok(Run {
        stdout: o.stdout,
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
    std::fs::write(rs_out, o.stdout).map_err(|e| format!("write {}: {e}", rs_out.display()))
}

pub fn compare(name: &str, c_src: &Path, rs_src: &Path, tmp: &Path) -> Result<(), String> {
    let c_bin = tmp.join(format!("{name}_c"));
    compile_c(c_src, &c_bin)?;
    let rs_bin = compile_rs_cargo(rs_src, tmp, &format!("{name}_rs"))?;
    let c = run(&c_bin)?;
    let r = run(&rs_bin)?;

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
    Ok(())
}
