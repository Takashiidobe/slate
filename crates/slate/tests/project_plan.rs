#[path = "support/filecheck.rs"]
mod filecheck;

use std::path::{Path, PathBuf};
use std::process::Command;

fn fixtures() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.plan")
}

fn work() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated/plan")
}

fn slate(args: &[String], project: &Path, jobs: Option<&str>) -> (String, String, i32) {
    let mut command = Command::new(env!("CARGO_BIN_EXE_slate"));
    command
        .arg("translate-project")
        .args(args)
        .arg(project)
        .env("SLATE_SYSROOTS", slate_parser::sysroot::root())
        .env_remove("SLATE_TARGET")
        .env_remove("SLATE_CLANG_ARGS");
    if let Some(jobs) = jobs {
        command.env("SLATE_JOBS", jobs);
    }
    let output = command.output().expect("run slate translate-project");
    (
        String::from_utf8_lossy(&output.stdout).into_owned(),
        String::from_utf8_lossy(&output.stderr).into_owned(),
        output.status.code().unwrap_or(-1),
    )
}

fn captured((stdout, stderr, code): &(String, String, i32)) -> String {
    format!("{stdout}{stderr}exit: {code}\n")
}

fn exhaustive_line(args: &[String], project: &Path) -> String {
    let (stdout, stderr, _) = slate(args, project, None);
    let summary = stdout
        .lines()
        .find_map(|line| line.split_once(", exhaustive "))
        .map(|(_, summary)| summary);
    match summary {
        Some(summary) => format!("exhaustive: {summary}\n"),
        None => format!("exhaustive: {stderr}"),
    }
}

fn plan_fixture(source: &Path) -> Result<(), String> {
    let stem = source.file_stem().unwrap().to_string_lossy().into_owned();
    let text = std::fs::read_to_string(source).map_err(|e| format!("read: {e}"))?;
    let specific = source.with_extension("json");
    let base = if specific.is_file() {
        specific
    } else {
        fixtures().join("matrix.json")
    };
    let base = std::fs::read(&base).map_err(|e| format!("read {}: {e}", base.display()))?;
    let mut matrix: serde_json::Value =
        serde_json::from_slice(&base).map_err(|e| format!("parse matrix: {e}"))?;
    matrix["sources"] = serde_json::json!([format!("{stem}.c")]);
    let matrix_path = work().join(format!("{stem}.json"));
    std::fs::create_dir_all(work()).map_err(|e| format!("create work: {e}"))?;
    std::fs::write(&matrix_path, matrix.to_string()).map_err(|e| format!("write matrix: {e}"))?;
    let matrix_arg = matrix_path.display().to_string();
    let explicit = text
        .lines()
        .find_map(|line| line.strip_prefix("// plan-args:"))
        .map(|args| {
            args.split_whitespace()
                .map(|arg| arg.replace("{matrix}", &matrix_arg))
                .collect::<Vec<_>>()
        });
    let mut failures = Vec::new();
    for target in matrix["targets"].as_array().expect("matrix targets") {
        let name = target["name"].as_str().expect("target name").to_string();
        let triple = target["triple"].as_str().expect("target triple");
        let selected = |mode: &str| {
            [mode, "--matrix", &matrix_arg, "--matrix-target", &name].map(str::to_string)
        };
        let args = explicit
            .clone()
            .unwrap_or_else(|| selected("--plan").to_vec());
        let run = slate(&args, &fixtures(), None);
        let mut output = captured(&run);
        if explicit.is_none() {
            output.push_str(&exhaustive_line(
                &selected("--plan-exhaustive"),
                &fixtures(),
            ));
            let serial = slate(&args, &fixtures(), Some("1"));
            if serial != run {
                failures.push(format!("{triple}: output depends on SLATE_JOBS"));
            }
        }
        let dir = work().join(triple);
        std::fs::create_dir_all(&dir).map_err(|e| format!("create {}: {e}", dir.display()))?;
        let generated = dir.join(format!("{stem}.generated.rs"));
        std::fs::write(&generated, &output).map_err(|e| format!("write output: {e}"))?;
        std::fs::write(dir.join(format!("{stem}.runtime-ok")), b"")
            .map_err(|e| format!("write marker: {e}"))?;
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
fn plan_filecheck() {
    let filter = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut sources: Vec<PathBuf> = std::fs::read_dir(fixtures())
        .expect("read plan fixtures")
        .map(|entry| entry.expect("plan fixture").path())
        .filter(|path| path.extension().is_some_and(|extension| extension == "c"))
        .filter(|path| {
            filter
                .as_deref()
                .is_none_or(|filter| path.file_stem().is_some_and(|stem| stem == filter))
        })
        .collect();
    sources.sort();
    assert!(!sources.is_empty(), "no plan fixtures selected");
    let failures: Vec<String> = sources
        .iter()
        .filter_map(|source| {
            plan_fixture(source)
                .err()
                .map(|error| format!("{}: {error}", source.display()))
        })
        .collect();
    assert!(failures.is_empty(), "{}", failures.join("\n"));
}
