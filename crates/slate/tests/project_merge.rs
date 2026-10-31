#[path = "support/filecheck.rs"]
mod filecheck;

use slate_parser::target_info::TargetInfo;
use std::collections::BTreeSet;
use std::path::{Path, PathBuf};
use std::process::Command;

const TRIPLE: &str = "x86_64-unknown-linux-gnu";

fn fixtures() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.merge")
}

fn work() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated/merge")
}

fn merge(matrix: &Path, crate_dir: &Path, jobs: Option<&str>) -> (String, String, i32) {
    let _ = std::fs::remove_dir_all(crate_dir);
    let mut command = Command::new(env!("CARGO_BIN_EXE_slate"));
    command
        .arg("translate-project")
        .arg("--matrix")
        .arg(matrix)
        .args(["--merge", "--check-projection", "all"])
        .arg(fixtures())
        .arg(crate_dir)
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

fn crate_sources(crate_dir: &Path) -> String {
    let mut paths: Vec<PathBuf> = std::fs::read_dir(crate_dir.join("src"))
        .map(|entries| {
            entries
                .map(|entry| entry.expect("crate file").path())
                .collect()
        })
        .unwrap_or_default();
    paths.retain(|path| path.extension().is_some_and(|extension| extension == "rs"));
    paths.sort();
    let mut text = String::new();
    for path in paths {
        let name = path.file_name().unwrap().to_string_lossy().into_owned();
        text.push_str(&format!("file {name}\n"));
        text.push_str(&std::fs::read_to_string(&path).expect("read generated source"));
    }
    text
}

fn feature_sets(matrix: &serde_json::Value) -> Vec<Vec<String>> {
    let names: Vec<String> = matrix["features"]
        .as_array()
        .expect("matrix features")
        .iter()
        .map(|feature| feature["name"].as_str().expect("feature name").to_string())
        .collect();
    let invalid = |set: &[String]| {
        matrix["invalid_features"]
            .as_array()
            .into_iter()
            .flatten()
            .any(|group| {
                group
                    .as_array()
                    .expect("invalid group")
                    .iter()
                    .all(|name| set.iter().any(|known| name == known.as_str()))
            })
    };
    (0u32..1 << names.len())
        .map(|bits| {
            names
                .iter()
                .enumerate()
                .filter(|(index, _)| bits >> index & 1 == 1)
                .map(|(_, name)| name.clone())
                .collect::<Vec<_>>()
        })
        .filter(|set| !invalid(set))
        .collect()
}

fn run_c(sources: &[PathBuf], defines: &[String], out: &Path) -> Option<String> {
    let built = Command::new("clang")
        .args(sources)
        .args(defines.iter().map(|define| format!("-D{define}")))
        .arg("-fcommon")
        .arg("-o")
        .arg(out)
        .output()
        .expect("run clang");
    if !built.status.success() {
        return None;
    }
    let output = Command::new(out).output().expect("run C binary");
    Some(format!(
        "{}exit: {:?}\n",
        String::from_utf8_lossy(&output.stdout),
        output.status.code()
    ))
}

fn run_rust(crate_dir: &Path, features: &[String]) -> Result<String, String> {
    let output = Command::new(std::env::var("SLATE_CARGO").unwrap_or_else(|_| "cargo".into()))
        .args([
            "run",
            "--quiet",
            "--release",
            "--features",
            &features.join(","),
        ])
        .current_dir(crate_dir)
        .env("CARGO_TARGET_DIR", work().join("cargo-target"))
        .output()
        .map_err(|error| format!("run cargo: {error}"))?;
    if !output.status.success() && output.stdout.is_empty() {
        return Err(String::from_utf8_lossy(&output.stderr).into_owned());
    }
    Ok(format!(
        "{}exit: {:?}\n",
        String::from_utf8_lossy(&output.stdout),
        output.status.code()
    ))
}

fn differential(
    matrix: &serde_json::Value,
    crate_dir: &Path,
    stem: &str,
    sources: &[PathBuf],
) -> Vec<String> {
    let defines = |set: &[String]| -> Vec<String> {
        matrix["features"]
            .as_array()
            .expect("matrix features")
            .iter()
            .filter(|feature| set.iter().any(|name| feature["name"] == name.as_str()))
            .flat_map(|feature| feature["defines"].as_array().expect("defines"))
            .map(|define| define.as_str().expect("define").to_string())
            .collect()
    };
    let mut failures = Vec::new();
    for set in feature_sets(matrix) {
        let binary = work().join(format!("{stem}-c"));
        let Some(expected) = run_c(sources, &defines(&set), &binary) else {
            continue;
        };
        match run_rust(crate_dir, &set) {
            Ok(actual) if actual == expected => {}
            Ok(actual) => failures.push(format!(
                "[{}]: C printed {expected:?}, Rust printed {actual:?}",
                set.join(",")
            )),
            Err(error) => failures.push(format!("[{}]: {error}", set.join(","))),
        }
    }
    failures
}

fn merge_fixture(source: &Path) -> Result<(), String> {
    let stem = source.file_stem().unwrap().to_string_lossy().into_owned();
    let text = std::fs::read_to_string(source).map_err(|e| format!("read: {e}"))?;
    let base = std::fs::read(fixtures().join("matrix.json")).map_err(|e| format!("read: {e}"))?;
    let mut matrix: serde_json::Value =
        serde_json::from_slice(&base).map_err(|e| format!("parse matrix: {e}"))?;
    let mut sources = vec![format!("{stem}.c")];
    sources.extend(
        text.lines()
            .find_map(|line| line.strip_prefix("// merge-sources:"))
            .into_iter()
            .flat_map(str::split_whitespace)
            .map(str::to_string),
    );
    matrix["sources"] = serde_json::json!(sources);
    if let Some(args) = text
        .lines()
        .find_map(|line| line.strip_prefix("// merge-linux-args:"))
    {
        matrix["targets"][0]["args"] =
            serde_json::json!(args.split_whitespace().collect::<Vec<_>>());
    }
    if let Some(other) = text
        .lines()
        .find_map(|line| line.strip_prefix("// merge-second-target:"))
    {
        let fields: Vec<&str> = other.split_whitespace().collect();
        matrix["targets"][1] =
            serde_json::json!({"name": fields[0], "triple": fields[1], "flavor": fields[2]});
    }
    if let Ok(extra) = std::fs::read(source.with_extension("targets.json")) {
        let extra: Vec<serde_json::Value> =
            serde_json::from_slice(&extra).map_err(|e| format!("parse targets: {e}"))?;
        matrix["targets"]
            .as_array_mut()
            .expect("matrix targets")
            .extend(extra);
    }
    if let Some(names) = text
        .lines()
        .find_map(|line| line.strip_prefix("// merge-features:"))
    {
        matrix["features"] = names
            .split_whitespace()
            .map(|name| serde_json::json!({"name": name, "defines": [name.to_uppercase()]}))
            .collect();
        matrix["invalid_features"] = serde_json::json!([]);
    }
    std::fs::create_dir_all(work()).map_err(|e| format!("create work: {e}"))?;
    let matrix_path = work().join(format!("{stem}.json"));
    std::fs::write(&matrix_path, matrix.to_string()).map_err(|e| format!("write matrix: {e}"))?;
    let crate_dir = work().join("crates").join(&stem);
    let (stdout, stderr, code) = merge(&matrix_path, &crate_dir, None);
    let generated = crate_sources(&crate_dir);
    let serial_dir = work().join("crates").join(format!("{stem}-serial"));
    let serial = merge(&matrix_path, &serial_dir, Some("1"));
    let report: String = stdout
        .lines()
        .filter(|line| !line.starts_with("wrote "))
        .map(|line| format!("{line}\n"))
        .collect();
    let output = format!("{report}{stderr}exit: {code}\n{generated}");
    let dir = work().join(TRIPLE);
    std::fs::create_dir_all(&dir).map_err(|e| format!("create {}: {e}", dir.display()))?;
    std::fs::write(dir.join(format!("{stem}.generated.rs")), &output)
        .map_err(|e| format!("write output: {e}"))?;
    std::fs::write(dir.join(format!("{stem}.runtime-ok")), b"")
        .map_err(|e| format!("write marker: {e}"))?;
    let mut failures = Vec::new();
    let serial_report: String = serial
        .0
        .lines()
        .filter(|line| !line.starts_with("wrote "))
        .map(|line| format!("{line}\n"))
        .collect();
    if serial_report != report || crate_sources(&serial_dir) != generated {
        failures.push("output depends on SLATE_JOBS".to_string());
    }
    let expected_code = text
        .lines()
        .find_map(|line| line.strip_prefix("// merge-expect-exit:"))
        .map_or(0, |code| code.trim().parse().expect("expected exit code"));
    if code != expected_code {
        failures.push(format!("merge exited {code}: {stderr}"));
    }
    if let Err(error) =
        filecheck::check_generated_rust(&text, &output, TRIPLE, &dir.join("filecheck").join(&stem))
    {
        failures.push(error);
    }
    if code == 0 {
        let paths: Vec<PathBuf> = sources.iter().map(|path| fixtures().join(path)).collect();
        failures.extend(differential(&matrix, &crate_dir, &stem, &paths));
    }
    if failures.is_empty() {
        Ok(())
    } else {
        Err(failures.join("\n"))
    }
}

#[test]
fn merge_filecheck() {
    let filter = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut sources: Vec<PathBuf> = std::fs::read_dir(fixtures())
        .expect("read merge fixtures")
        .map(|entry| entry.expect("merge fixture").path())
        .filter(|path| path.extension().is_some_and(|extension| extension == "c"))
        .filter(|path| {
            filter
                .as_deref()
                .is_none_or(|filter| path.file_stem().is_some_and(|stem| stem == filter))
        })
        .collect();
    sources.sort();
    assert!(!sources.is_empty(), "no merge fixtures selected");
    let failures: Vec<String> = sources
        .iter()
        .filter_map(|source| {
            merge_fixture(source)
                .err()
                .map(|error| format!("{}: {error}", source.display()))
        })
        .collect();
    assert!(failures.is_empty(), "{}", failures.join("\n"));
}

#[test]
fn target_outside_matrix_hits_guard() {
    let base = std::fs::read(fixtures().join("matrix.json")).expect("read matrix");
    let mut matrix: serde_json::Value = serde_json::from_slice(&base).expect("parse matrix");
    matrix["sources"] = serde_json::json!(["by_target.c"]);
    std::fs::create_dir_all(work()).expect("create work");
    let matrix_path = work().join("target_guard.json");
    std::fs::write(&matrix_path, matrix.to_string()).expect("write matrix");
    let crate_dir = work().join("crates/target_guard");
    let (stdout, stderr, code) = merge(&matrix_path, &crate_dir, None);
    assert_eq!(code, 0, "{stdout}{stderr}");
    let check = |triple: &str| {
        Command::new(std::env::var("SLATE_CARGO").unwrap_or_else(|_| "cargo".into()))
            .args(["check", "--quiet", "--target", triple])
            .current_dir(&crate_dir)
            .env("CARGO_TARGET_DIR", work().join("cargo-target"))
            .output()
            .expect("run cargo check")
    };
    let inside = check(TRIPLE);
    assert!(
        inside.status.success(),
        "{}",
        String::from_utf8_lossy(&inside.stderr)
    );
    let outside = check("i686-unknown-linux-gnu");
    let stderr = String::from_utf8_lossy(&outside.stderr);
    assert!(
        !outside.status.success()
            && stderr.contains("this crate was not translated for this target"),
        "{stderr}"
    );
}

#[test]
fn target_properties_match_rustc() {
    let workspace = Path::new(env!("CARGO_MANIFEST_DIR")).join("../..");
    let matrix = std::fs::read(workspace.join("tools/corpus/matrices/zlib.json")).expect("matrix");
    let matrix: serde_json::Value = serde_json::from_slice(&matrix).expect("parse matrix");
    let targets: Vec<(String, BTreeSet<String>, BTreeSet<String>)> = matrix["targets"]
        .as_array()
        .expect("matrix targets")
        .iter()
        .map(|target| {
            let triple = target["triple"].as_str().expect("triple");
            let info = TargetInfo::for_triple(triple).expect("known triple");
            let ours = slate::project_cfg::target_properties(&info)
                .iter()
                .map(|atom| atom.render().replace(" = ", "="))
                .collect();
            let printed = Command::new("rustc")
                .args(["--print", "cfg", "--target", triple])
                .output()
                .expect("run rustc");
            let theirs = String::from_utf8_lossy(&printed.stdout)
                .lines()
                .map(str::to_string)
                .collect();
            (triple.to_string(), ours, theirs)
        })
        .collect();
    let atoms: BTreeSet<&String> = targets.iter().flat_map(|(_, ours, _)| ours).collect();
    for (triple, ours, theirs) in &targets {
        for atom in &atoms {
            assert_eq!(
                ours.contains(*atom),
                theirs.contains(*atom),
                "{triple}: {atom}"
            );
        }
    }
}

#[test]
fn exhaustive_projection_is_bounded() {
    let base = std::fs::read(fixtures().join("matrix.json")).expect("read matrix");
    let mut matrix: serde_json::Value = serde_json::from_slice(&base).expect("parse matrix");
    matrix["sources"] = serde_json::json!(["by_target.c"]);
    matrix["invalid_features"] = serde_json::json!([]);
    matrix["features"] = (0..10)
        .map(|index| serde_json::json!({"name": format!("f{index}"), "defines": [format!("F{index}")]}))
        .collect();
    std::fs::create_dir_all(work()).expect("create work");
    let matrix_path = work().join("projection_bound.json");
    std::fs::write(&matrix_path, matrix.to_string()).expect("write matrix");
    let (_, stderr, code) = merge(&matrix_path, &work().join("crates/projection_bound"), None);
    assert!(
        code != 0 && stderr.contains("2048 whole-project translations"),
        "{code}: {stderr}"
    );
}

#[test]
fn zlib_projection_sample() {
    let Some(home) = std::env::var_os("HOME") else {
        return;
    };
    let project = Path::new(&home).join("c-corpus/zlib");
    if !project.join("build-clang").is_dir() {
        eprintln!("skipping: {} is not configured", project.display());
        return;
    }
    let workspace = Path::new(env!("CARGO_MANIFEST_DIR")).join("../..");
    let crate_dir = work().join("crates/zlib-sample");
    let _ = std::fs::remove_dir_all(&crate_dir);
    let output = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-project")
        .arg("--matrix")
        .arg(workspace.join("tools/corpus/matrices/zlib.json"))
        .args(["--merge", "--matrix-target", "linux-x86_64"])
        .args(["--check-projection", "8"])
        .arg(&project)
        .arg(&crate_dir)
        .env("SLATE_SYSROOTS", slate_parser::sysroot::root())
        .env_remove("SLATE_TARGET")
        .env_remove("SLATE_CLANG_ARGS")
        .output()
        .expect("run slate translate-project");
    let stdout = String::from_utf8_lossy(&output.stdout);
    assert!(
        output.status.success() && stdout.contains(" 0 mismatches"),
        "{stdout}{}",
        String::from_utf8_lossy(&output.stderr)
    );
}
