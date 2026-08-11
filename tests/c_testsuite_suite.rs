mod support;

use std::path::{Path, PathBuf};

fn fixtures_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.c-testsuite")
}

fn collect_cases(bucket: &str) -> Vec<(String, PathBuf)> {
    let dir = fixtures_root().join(bucket);
    let mut cases: Vec<(String, PathBuf)> = std::fs::read_dir(&dir)
        .unwrap_or_else(|e| panic!("read {}: {e}", dir.display()))
        .filter_map(|e| e.ok())
        .map(|e| e.path())
        .filter(|p| p.extension().and_then(|e| e.to_str()) == Some("c"))
        .map(|p| {
            let name = p.file_stem().unwrap().to_string_lossy().into_owned();
            (name, p)
        })
        .collect();
    cases.sort();
    cases
}

fn run_bucket(bucket: &str) -> Vec<(String, Result<(), String>)> {
    let cases = collect_cases(bucket);
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/c-testsuite-suite")
        .join(bucket);
    std::fs::create_dir_all(&work).expect("create work dir");

    let translated = support::parallel_map(&cases, |(name, path)| {
        let generated = work.join(format!("{name}.generated.rs"));
        support::translate(path, &generated).map(|()| support::Case {
            name: name.clone(),
            c_src: path.clone(),
            rs_src: generated,
            config: support::RunConfig::default(),
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

    results.extend(support::compare_batch(&compiled, &work));
    results.sort_by(|a, b| a.0.cmp(&b.0));
    results
}

#[test]
fn c_testsuite_supported_tests_match_c() {
    let results = run_bucket("supported");
    let failures: Vec<String> = results
        .into_iter()
        .filter_map(|(name, result)| result.err().map(|e| format!("{name}: {e}")))
        .collect();
    assert!(
        failures.is_empty(),
        "c-testsuite supported tests failed:\n{}",
        failures.join("\n\n")
    );
}

#[test]
fn c_testsuite_unsupported_tests_still_fail() {
    let results = run_bucket("unsupported");
    let unexpected_passes: Vec<String> = results
        .into_iter()
        .filter_map(|(name, result)| result.ok().map(|()| name))
        .collect();
    assert!(
        unexpected_passes.is_empty(),
        "c-testsuite tests unexpectedly passed, move to supported/:\n{}",
        unexpected_passes.join("\n")
    );
}

#[test]
#[ignore]
fn c_testsuite_unsupported_triage_report() {
    let results = run_bucket("unsupported");
    for (name, result) in results {
        if let Err(e) = result {
            println!("=== {name} ===\n{e}\n");
        }
    }
}
