#![cfg(unix)]

use std::fs;
use std::io;
use std::os::unix::fs::symlink;
use std::path::Path;
use std::process::Command;

fn check_headers(root: &Path, target: &str, expected: u32) -> io::Result<()> {
    let source = root.join("selection.c");
    fs::write(
        &source,
        format!(
            "#include <selection.h>\n#if HEADER_VERSION != {expected}\n#error wrong compiler headers selected\n#endif\n"
        ),
    )?;
    let output = Command::new(env!("CARGO_BIN_EXE_slate-parser"))
        .arg("pp")
        .arg(&source)
        .arg(format!("-target={target}"))
        .arg("--flavor=clang")
        .env("SLATE_COMPILER_HEADERS", root)
        .env("SLATE_SYSROOTS", root.join("sysroots"))
        .output()?;
    if !output.status.success() {
        return Err(io::Error::other(format!(
            "expected header version {expected} for {target}: {}",
            String::from_utf8_lossy(&output.stderr)
        )));
    }
    Ok(())
}

#[test]
fn apple_clang_current_overrides_versions_and_falls_back() -> io::Result<()> {
    let temp = tempfile::tempdir()?;
    let root = temp.path();
    for (bundle, version) in [
        ("apple-clang-17.0.0", 17),
        ("apple-clang-18.0.0", 18),
        ("clang-22.1.8", 22),
    ] {
        let include = root.join(bundle).join("include");
        fs::create_dir_all(&include)?;
        fs::write(
            include.join("selection.h"),
            format!("#define HEADER_VERSION {version}\n"),
        )?;
    }
    let current = root.join("apple-clang-current");
    symlink("apple-clang-17.0.0", &current)?;
    for target in ["x86_64-apple-darwin", "aarch64-apple-darwin"] {
        check_headers(root, target, 17)?;
    }
    check_headers(root, "x86_64-unknown-linux-gnu", 22)?;
    fs::remove_file(&current)?;
    for target in ["x86_64-apple-darwin", "aarch64-apple-darwin"] {
        check_headers(root, target, 18)?;
    }
    symlink("missing-bundle", &current)?;
    for target in ["x86_64-apple-darwin", "aarch64-apple-darwin"] {
        check_headers(root, target, 18)?;
    }
    fs::remove_dir_all(root.join("apple-clang-17.0.0"))?;
    fs::remove_dir_all(root.join("apple-clang-18.0.0"))?;
    for target in ["x86_64-apple-darwin", "aarch64-apple-darwin"] {
        check_headers(root, target, 22)?;
    }
    Ok(())
}
