#[path = "support/filecheck.rs"]
mod filecheck;

use std::path::{Path, PathBuf};
use std::process::Command;

const TRIPLE: &str = "x86_64-unknown-linux-gnu";

fn fixtures() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.select")
}

fn work() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated/select")
}

fn crate_sources(crate_dir: &Path) -> String {
    let mut paths: Vec<PathBuf> = std::fs::read_dir(crate_dir.join("src"))
        .map(|entries| {
            entries
                .map(|entry| entry.expect("crate file").path())
                .collect()
        })
        .unwrap_or_default();
    paths.sort();
    let mut text = String::new();
    for path in paths {
        let name = path.file_name().unwrap().to_string_lossy().into_owned();
        text.push_str(&format!("file {name}\n"));
        text.push_str(&std::fs::read_to_string(&path).expect("read generated source"));
    }
    text
}

fn select_fixture(source: &Path) -> Result<(), String> {
    let stem = source.file_stem().unwrap().to_string_lossy().into_owned();
    let text = std::fs::read_to_string(source).map_err(|e| format!("read: {e}"))?;
    let crate_dir = work().join("crates").join(&stem);
    let _ = std::fs::remove_dir_all(&crate_dir);
    let args = text
        .lines()
        .find_map(|line| line.strip_prefix("// select-args:"))
        .map(|args| args.split_whitespace().collect::<Vec<_>>())
        .unwrap_or_default();
    let output = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-project")
        .arg("--compile-commands")
        .arg(source.with_extension("json"))
        .args(args)
        .arg(fixtures())
        .arg(&crate_dir)
        .env("SLATE_SYSROOTS", slate_parser::sysroot::root())
        .env_remove("SLATE_TARGET")
        .env_remove("SLATE_CLANG_ARGS")
        .output()
        .map_err(|e| format!("run slate: {e}"))?;
    let report: String = String::from_utf8_lossy(&output.stdout)
        .lines()
        .filter(|line| !line.starts_with("wrote "))
        .map(|line| format!("{line}\n"))
        .collect();
    let checked = format!(
        "{report}{}exit: {}\n{}",
        String::from_utf8_lossy(&output.stderr),
        output.status.code().unwrap_or(-1),
        crate_sources(&crate_dir)
    );
    let dir = work().join(TRIPLE);
    std::fs::create_dir_all(&dir).map_err(|e| format!("create {}: {e}", dir.display()))?;
    std::fs::write(dir.join(format!("{stem}.generated.rs")), &checked)
        .map_err(|e| format!("write output: {e}"))?;
    std::fs::write(dir.join(format!("{stem}.runtime-ok")), b"")
        .map_err(|e| format!("write marker: {e}"))?;
    filecheck::check_generated_rust(&text, &checked, TRIPLE, &dir.join("filecheck").join(&stem))
}

#[test]
fn select_filecheck() {
    let filter = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut sources: Vec<PathBuf> = std::fs::read_dir(fixtures())
        .expect("read select fixtures")
        .map(|entry| entry.expect("select fixture").path())
        .filter(|path| path.extension().is_some_and(|extension| extension == "c"))
        .filter(|path| {
            filter
                .as_deref()
                .is_none_or(|filter| path.file_stem().is_some_and(|stem| stem == filter))
        })
        .collect();
    sources.sort();
    assert!(!sources.is_empty(), "no select fixtures selected");
    let failures: Vec<String> = sources
        .iter()
        .filter_map(|source| {
            select_fixture(source)
                .err()
                .map(|error| format!("{}: {error}", source.display()))
        })
        .collect();
    assert!(failures.is_empty(), "{}", failures.join("\n"));
}
