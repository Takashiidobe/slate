mod support;

use std::path::{Path, PathBuf};

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

fn unsupported_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.unsupported")
}

struct Fixture {
    name: String,
    relative: PathBuf,
    path: PathBuf,
}

fn collect_fixtures(root: &Path, sub: &Path, selected: &Option<String>, out: &mut Vec<Fixture>) {
    let Ok(entries) = std::fs::read_dir(root.join(sub)) else {
        return;
    };
    for entry in entries {
        let path = entry.expect("dir entry").path();
        if path.extension().and_then(|e| e.to_str()) != Some("c") {
            continue;
        }
        let name = path.file_stem().unwrap().to_string_lossy().into_owned();
        if selected.as_ref().is_some_and(|selected| selected != &name) {
            continue;
        }
        let relative = sub.join(path.file_name().unwrap());
        out.push(Fixture {
            name,
            relative,
            path,
        });
    }
}

fn fixtures(root: &Path) -> Vec<Fixture> {
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut fixtures = Vec::new();
    collect_fixtures(root, Path::new(""), &selected, &mut fixtures);
    if cfg!(target_arch = "x86_64") {
        collect_fixtures(root, Path::new("x86_64"), &selected, &mut fixtures);
    }
    fixtures.sort_by(|a, b| a.name.cmp(&b.name));
    fixtures
}

fn run_cases(group: &str, fixtures: &[Fixture]) -> Vec<(String, Result<(), String>)> {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/difftest-generated")
        .join(group);
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let translated = support::parallel_map(fixtures, |f| {
        let generated = tmp.join(format!("{}.generated.rs", f.name));
        let dg_options = support::fixture_dg_options(&f.path);
        let mut extra_args = dg_options.clone();
        extra_args.extend(support::fixture_dg_additional_options(&f.path));
        support::translate_slate(&f.path, &generated, &extra_args).map(|()| {
            let mut config = support::RunConfig::default();
            config.c_args.extend(dg_options);
            support::Case {
                name: f.name.clone(),
                c_src: f.path.clone(),
                rs_src: generated,
                config,
            }
        })
    });
    let mut cases = Vec::new();
    let mut results = Vec::new();
    for (f, result) in fixtures.iter().zip(translated) {
        match result {
            Ok(case) => cases.push(case),
            Err(e) => results.push((f.name.clone(), Err(e))),
        }
    }
    results.extend(support::compare_batch(&cases, &tmp));
    results.sort_by(|a, b| a.0.cmp(&b.0));
    results
}

fn first_barrier(error: &str) -> (&'static str, &str) {
    let detail = error
        .lines()
        .map(str::trim)
        .find(|line| line.starts_with("error"))
        .unwrap_or_else(|| error.lines().next().unwrap_or_default());
    let class = if error.starts_with("slate translate-lowered failed") {
        if detail.contains("unsupported slate-parser IR") {
            "unsupported lowering"
        } else {
            "parse/sema"
        }
    } else if error.starts_with("Rust batch build failed") {
        "rustc"
    } else if error.starts_with("exit code differs")
        || error.starts_with("stdout differs")
        || error.starts_with("stderr differs")
    {
        "runtime mismatch"
    } else {
        "harness"
    };
    (class, detail)
}

#[test]
fn generated_differential() {
    let fixtures = fixtures(&fixtures_dir());
    assert!(
        !fixtures.is_empty() || std::env::var("SLATE_DIFF_FIXTURE").is_ok(),
        "no fixtures found in {:?}",
        fixtures_dir()
    );

    let mut failures = Vec::new();
    for (name, result) in run_cases("supported", &fixtures) {
        match result {
            Ok(()) => eprintln!("ok    {name}"),
            Err(e) => {
                eprintln!("FAIL  {name}");
                failures.push(format!("[{name}] {e}"));
            }
        }
    }

    if !failures.is_empty() {
        panic!(
            "{} of {} generated fixtures failed:\n\n{}",
            failures.len(),
            fixtures.len(),
            failures.join("\n\n")
        );
    }
}

#[test]
fn fixtures_unsupported_tests_still_fail() {
    let fixtures = fixtures(&unsupported_dir());
    let results = run_cases("unsupported", &fixtures);
    let unexpected_passes: Vec<String> = results
        .into_iter()
        .filter(|(_, result)| result.is_ok())
        .filter_map(|(name, _)| fixtures.iter().find(|f| f.name == name))
        .map(|f| {
            format!(
                "  git mv tests/fixtures.unsupported/{0} tests/fixtures/{0}",
                f.relative.display()
            )
        })
        .collect();
    assert!(
        unexpected_passes.is_empty(),
        "fixture(s) now pass end-to-end -- promote them:\n{}",
        unexpected_passes.join("\n")
    );
}

#[test]
#[ignore]
fn fixtures_unsupported_triage_report() {
    let fixtures = fixtures(&unsupported_dir());
    for (name, result) in run_cases("unsupported", &fixtures) {
        match result {
            Ok(()) => println!("PASS {name}"),
            Err(error) => {
                let (class, detail) = first_barrier(&error);
                println!("FAIL {name} [{class}] {detail}");
            }
        }
    }
}

#[test]
fn translation_is_self_hosted() {
    let source = fixtures_dir().join("atoi_atol_prelude_dynamic.c");
    let work = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated/self-hosted");
    std::fs::create_dir_all(&work).expect("create self-hosted test directory");
    let commands = work.join("compile_commands.json");
    std::fs::write(
        &commands,
        serde_json::to_vec(&serde_json::json!([{
            "directory": env!("CARGO_MANIFEST_DIR"),
            "file": source,
            "arguments": ["clang", "--target=x86_64-unknown-linux-gnu", "-c", source],
        }]))
        .expect("encode compile commands"),
    )
    .expect("write compile commands");
    let target = "--target=x86_64-unknown-linux-gnu";
    let inputs = [
        vec![
            "translate",
            "--frontend=slate",
            target,
            source.to_str().unwrap(),
        ],
        vec![
            "translate-lowered",
            "--frontend=slate",
            target,
            source.to_str().unwrap(),
        ],
        vec!["record-cfg", source.to_str().unwrap(), target],
        vec![
            "translate-project",
            "--frontend=slate",
            "--compile-commands",
            commands.to_str().unwrap(),
            env!("CARGO_MANIFEST_DIR"),
            work.to_str().unwrap(),
        ],
    ];
    for args in inputs {
        let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
            .args(&args)
            .env("PATH", "")
            .env("SLATE_TARGET", "invalid-default-target")
            .env_remove("SLATE_CLANG_ARGS")
            .output()
            .expect("run Slate without external tools");
        assert!(
            output.status.success(),
            "{} failed without external tools:\n{}",
            args[0],
            String::from_utf8_lossy(&output.stderr)
        );
    }
}
