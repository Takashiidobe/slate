mod support;

use std::path::{Path, PathBuf};

fn jobs() -> usize {
    std::env::var("SLATE_GCC_DG_JOBS")
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

fn root(group: &str) -> PathBuf {
    let suffix = (group != "supported").then_some(format!(".{group}"));
    Path::new(env!("CARGO_MANIFEST_DIR")).join(format!(
        "tests/fixtures.gcc-dg{}",
        suffix.as_deref().unwrap_or_default()
    ))
}

fn collect_cases(dir: &Path) -> Vec<(String, PathBuf)> {
    let selected = std::env::var("SLATE_GCC_DG_FIXTURE").ok();
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

fn run_cases(group: &str, dir: &Path) -> Vec<(String, Result<(), String>)> {
    let cases = collect_cases(dir);
    let jobs = jobs();
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/gcc-dg-suite")
        .join(group);
    std::fs::create_dir_all(&work).expect("create work dir");

    let translated = support::parallel_map_with_jobs(&cases, jobs, |(name, path)| {
        let generated = work.join(format!("{name}.generated.rs"));
        let options = support::dg_option_flags(path);
        support::translate_with_args(path, &generated, &options).map(|()| support::Case {
            name: name.clone(),
            c_src: path.clone(),
            rs_src: generated,
            config: support::RunConfig {
                timeout_seconds: Some(15),
                c_args: options
                    .into_iter()
                    .chain(["-latomic".to_string()])
                    .collect(),
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
fn gcc_dg_unsupported_tests_still_fail() {
    let results = run_cases("unsupported", &root("unsupported"));
    let unexpected_passes: Vec<String> = results
        .into_iter()
        .filter_map(|(name, result)| result.ok().map(|()| name))
        .collect();
    assert!(
        unexpected_passes.is_empty(),
        "gcc.dg test(s) now pass end-to-end -- promote them:\n{}",
        unexpected_passes
            .iter()
            .map(|name| format!(
                "  git mv tests/fixtures.gcc-dg.unsupported/{name}.c tests/fixtures.gcc-dg/{name}.c"
            ))
            .collect::<Vec<_>>()
            .join("\n")
    );
}

#[test]
fn gcc_dg_supported_tests_match_c() {
    let results = run_cases("supported", &root("supported"));
    let failures: Vec<String> = results
        .into_iter()
        .filter_map(|(name, result)| result.err().map(|e| format!("{name}: {e}")))
        .collect();
    assert!(
        failures.is_empty(),
        "gcc.dg supported tests failed:\n{}",
        failures.join("\n\n")
    );
}
