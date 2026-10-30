#[path = "support/filecheck.rs"]
mod filecheck;

use std::collections::BTreeSet;
use std::path::{Path, PathBuf};
use std::process::Command;

fn fixtures() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.lowered-names")
}

fn work() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated/lowered-names")
}

fn translate(
    matrix: &Path,
    target: &str,
    feature: Option<&str>,
    crate_dir: &Path,
) -> Result<String, String> {
    let _ = std::fs::remove_dir_all(crate_dir);
    let mut command = Command::new(env!("CARGO_BIN_EXE_slate"));
    command
        .arg("translate-project")
        .arg("--matrix")
        .arg(matrix)
        .args(["--matrix-target", target]);
    if let Some(feature) = feature {
        command.args(["--features", feature]);
    }
    let output = command
        .arg(fixtures())
        .arg(crate_dir)
        .env("SLATE_SYSROOTS", slate_parser::sysroot::root())
        .env_remove("SLATE_TARGET")
        .env_remove("SLATE_CLANG_ARGS")
        .output()
        .map_err(|error| format!("run slate: {error}"))?;
    if !output.status.success() {
        return Err(String::from_utf8_lossy(&output.stderr).into_owned());
    }
    let mut text = String::new();
    let mut paths: Vec<PathBuf> = std::fs::read_dir(crate_dir.join("src"))
        .map_err(|error| format!("read crate: {error}"))?
        .map(|entry| entry.expect("crate file").path())
        .collect();
    paths.sort();
    for path in paths {
        text.push_str(&std::fs::read_to_string(&path).map_err(|error| format!("read: {error}"))?);
    }
    Ok(text)
}

fn items(source: &str) -> BTreeSet<String> {
    let mut items = BTreeSet::new();
    let mut current = String::new();
    let mut in_extern_block = false;
    for line in source.lines() {
        if in_extern_block {
            if line == "}" {
                in_extern_block = false;
            } else {
                items.insert(line.trim().to_string());
            }
            continue;
        }
        if line.is_empty() && current.is_empty() {
            continue;
        }
        if line.contains("extern \"C") && line.trim_end().ends_with('{') && !line.contains("fn ") {
            in_extern_block = true;
            continue;
        }
        current.push_str(line);
        current.push('\n');
        let top_level = !line.starts_with([' ', '#', '/']);
        if top_level && (line.ends_with(';') || line.ends_with('}')) {
            items.insert(std::mem::take(&mut current));
        }
    }
    items
}

fn signature(item: &str) -> String {
    item.lines()
        .find(|line| !line.starts_with(['#', '/']))
        .unwrap_or_default()
        .trim_end_matches(" {")
        .to_string()
}

fn lowered_names_fixture(source: &Path) -> Result<(), String> {
    let stem = source.file_stem().unwrap().to_string_lossy().into_owned();
    let text = std::fs::read_to_string(source).map_err(|error| format!("read: {error}"))?;
    let base = fixtures().join("matrix.json");
    let base = std::fs::read(&base).map_err(|error| format!("read {}: {error}", base.display()))?;
    let mut matrix: serde_json::Value =
        serde_json::from_slice(&base).map_err(|error| format!("parse matrix: {error}"))?;
    matrix["sources"] = serde_json::json!([format!("{stem}.c")]);
    std::fs::create_dir_all(work()).map_err(|error| format!("create work: {error}"))?;
    let matrix_path = work().join(format!("{stem}.json"));
    std::fs::write(&matrix_path, matrix.to_string()).map_err(|error| format!("write: {error}"))?;
    let mut failures = Vec::new();
    for target in matrix["targets"].as_array().expect("matrix targets") {
        let name = target["name"].as_str().expect("target name");
        let triple = target["triple"].as_str().expect("target triple");
        let dir = work().join(triple);
        std::fs::create_dir_all(&dir).map_err(|error| format!("create: {error}"))?;
        let translated = dir.join("crates").join(&stem);
        let plain = translate(&matrix_path, name, None, &translated.join("plain"))?;
        let plain = items(&plain);
        let mut output = String::new();
        for feature in matrix["features"].as_array().expect("matrix features") {
            let feature = feature["name"].as_str().expect("feature name");
            let toggled = translate(&matrix_path, name, Some(feature), &translated.join(feature))?;
            let toggled = items(&toggled);
            output.push_str(&format!("feature {feature}\n"));
            for item in plain.difference(&toggled) {
                output.push_str(&format!("- {}\n", signature(item)));
            }
            for item in toggled.difference(&plain) {
                output.push_str(&format!("+ {}\n", signature(item)));
            }
        }
        let generated = dir.join(format!("{stem}.generated.rs"));
        std::fs::write(&generated, &output).map_err(|error| format!("write: {error}"))?;
        std::fs::write(dir.join(format!("{stem}.runtime-ok")), b"")
            .map_err(|error| format!("write: {error}"))?;
        if let Err(error) = filecheck::check_generated_rust(
            &text,
            &output,
            triple,
            &dir.join("filecheck").join(&stem),
        ) {
            failures.push(format!("{triple}: {error}"));
        }
    }
    if failures.is_empty() {
        Ok(())
    } else {
        Err(failures.join("\n"))
    }
}

#[test]
fn lowered_names_filecheck() {
    let filter = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut sources: Vec<PathBuf> = std::fs::read_dir(fixtures())
        .expect("read fixtures")
        .map(|entry| entry.expect("fixture").path())
        .filter(|path| path.extension().is_some_and(|extension| extension == "c"))
        .filter(|path| {
            filter
                .as_deref()
                .is_none_or(|filter| path.file_stem().is_some_and(|stem| stem == filter))
        })
        .collect();
    sources.sort();
    assert!(!sources.is_empty(), "no fixtures selected");
    let failures: Vec<String> = sources
        .iter()
        .filter_map(|source| {
            lowered_names_fixture(source)
                .err()
                .map(|error| format!("{}: {error}", source.display()))
        })
        .collect();
    assert!(failures.is_empty(), "{}", failures.join("\n"));
}
