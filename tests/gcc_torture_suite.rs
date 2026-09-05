mod support;

use std::path::{Path, PathBuf};

fn gcc_torture_jobs() -> usize {
    std::env::var("SLATE_GCC_TORTURE_JOBS")
        .ok()
        .and_then(|value| value.parse().ok())
        .filter(|jobs| *jobs > 0)
        .or_else(|| {
            std::env::var("SLATE_TEST_JOBS")
                .ok()
                .and_then(|value| value.parse().ok())
                .filter(|jobs| *jobs > 0)
        })
        .unwrap_or_else(|| {
            std::thread::available_parallelism()
                .map(usize::from)
                .unwrap_or(1)
        })
}

fn supported_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.gcc-torture")
}

fn unsupported_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.gcc-torture.unsupported")
}

fn collect_cases(dir: &Path) -> Vec<(String, PathBuf)> {
    let selected = std::env::var("SLATE_GCC_TORTURE_FIXTURE").ok();
    let mut cases: Vec<(String, PathBuf)> = std::fs::read_dir(dir)
        .unwrap_or_else(|e| panic!("read {}: {e}", dir.display()))
        .filter_map(|e| e.ok())
        .map(|e| e.path())
        .filter(|p| p.extension().and_then(|e| e.to_str()) == Some("c"))
        .filter_map(|path| {
            let name = path.file_stem().unwrap().to_string_lossy().into_owned();
            selected
                .as_ref()
                .is_none_or(|selected| selected == &name)
                .then_some((name, path))
        })
        .collect();
    cases.sort();
    cases
}

fn dg_option_flags(path: &Path) -> Vec<String> {
    let Ok(text) = std::fs::read_to_string(path) else {
        return Vec::new();
    };
    let mut flags = Vec::new();
    for line in text.lines() {
        if !line.contains("dg-options") && !line.contains("dg-additional-options") {
            continue;
        }
        let Some(quote_start) = line.find('"') else {
            continue;
        };
        let rest = &line[quote_start + 1..];
        let Some(quote_len) = rest.find('"') else {
            continue;
        };
        let quoted = &rest[..quote_len];
        let after = &rest[quote_len + 1..];
        if after.contains("target") && !dg_target_clause_applies(after) {
            continue;
        }
        flags.extend(
            quoted
                .split_whitespace()
                .filter(|flag| is_semantic_dg_flag(flag))
                .map(str::to_string),
        );
    }
    flags
}

fn dg_target_clause_applies(clause: &str) -> bool {
    if clause.contains("ia32") {
        return false;
    }
    clause.contains("x86_64") || clause.contains("i?86") || clause.contains("i386")
}

fn is_semantic_dg_flag(flag: &str) -> bool {
    matches!(
        flag,
        "-fwrapv"
            | "-fno-strict-overflow"
            | "-fno-strict-aliasing"
            | "-fno-trapping-math"
            | "-ffast-math"
            | "-fno-common"
            | "-mno-mmx"
    ) || flag.starts_with("-std=")
        || flag.starts_with("-finput-charset=")
}

fn run_cases(group: &str, dir: &Path) -> Vec<(String, Result<(), String>)> {
    let cases = collect_cases(dir);
    let jobs = gcc_torture_jobs();
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/gcc-torture-suite")
        .join(group);
    std::fs::create_dir_all(&work).expect("create work dir");

    let translated = support::parallel_map_with_jobs(&cases, jobs, |(name, path)| {
        let generated = work.join(format!("{name}.generated.rs"));
        let extra_args = dg_option_flags(path);
        support::translate_with_args(path, &generated, &extra_args).map(|()| support::Case {
            name: name.clone(),
            c_src: path.clone(),
            rs_src: generated,
            config: support::RunConfig {
                timeout_seconds: Some(5),
                c_args: extra_args,
                ..support::RunConfig::default()
            },
        })
    });

    let mut compiled = Vec::new();
    let mut results = Vec::new();
    for ((name, _), result) in cases.iter().zip(translated) {
        match result {
            Ok(case) => compiled.push(case),
            Err(e) => results.push((name.clone(), Err(format!("translate failed: {e}")))),
        }
    }

    results.extend(support::compare_batch_with_jobs(&compiled, &work, jobs));
    results.sort_by(|a, b| a.0.cmp(&b.0));
    results
}

#[test]
fn gcc_torture_supported_tests_match_c() {
    let results = run_cases("supported", &supported_root());
    let failures: Vec<String> = results
        .into_iter()
        .filter_map(|(name, result)| result.err().map(|e| format!("{name}: {e}")))
        .collect();
    assert!(
        failures.is_empty(),
        "gcc-torture supported tests failed:\n{}",
        failures.join("\n\n")
    );
}

#[test]
fn gcc_torture_unsupported_tests_still_fail() {
    let results = run_cases("unsupported", &unsupported_root());
    let unexpected_passes: Vec<String> = results
        .into_iter()
        .filter_map(|(name, result)| result.ok().map(|()| name))
        .collect();
    assert!(
        unexpected_passes.is_empty(),
        "gcc-torture test(s) now pass end-to-end -- promote them:\n{}",
        unexpected_passes
            .iter()
            .map(|name| format!(
                "  git mv tests/fixtures.gcc-torture.unsupported/{name}.c tests/fixtures.gcc-torture/{name}.c"
            ))
            .collect::<Vec<_>>()
            .join("\n")
    );
}

#[test]
#[ignore]
fn gcc_torture_unsupported_triage_report() {
    let results = run_cases("unsupported", &unsupported_root());
    for (name, result) in results {
        match result {
            Ok(()) => println!("PASS {name}"),
            Err(error) => println!("FAIL {name}\n{error}\n"),
        }
    }
}
