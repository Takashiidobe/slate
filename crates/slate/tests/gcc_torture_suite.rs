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

struct Corpus {
    dir: &'static str,
    gcc: bool,
}

const CLANG: Corpus = Corpus {
    dir: "fixtures.gcc-torture",
    gcc: false,
};

const GCC_ONLY: Corpus = Corpus {
    dir: "fixtures.gcc-torture.gcc-only",
    gcc: true,
};

impl Corpus {
    fn supported_root(&self) -> PathBuf {
        Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("tests")
            .join(self.dir)
    }

    fn unsupported_root(&self) -> PathBuf {
        Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("tests")
            .join(format!("{}.unsupported", self.dir))
    }
}

fn collect_cases(dir: &Path) -> Vec<(String, PathBuf)> {
    if !dir.exists() {
        return Vec::new();
    }
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

fn run_cases(corpus: &Corpus, group: &str, dir: &Path) -> Vec<(String, Result<(), String>)> {
    let cases = collect_cases(dir);
    let jobs = gcc_torture_jobs();
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/gcc-torture-suite")
        .join(corpus.dir)
        .join(group);
    std::fs::create_dir_all(&work).expect("create work dir");

    let translated = support::parallel_map_with_jobs(&cases, jobs, |(name, path)| {
        let generated = work.join(format!("{name}.generated.rs"));
        let extra_args = support::gnu_dg_option_flags(path);
        let mut translate_args = extra_args.clone();
        if corpus.gcc {
            translate_args.push("--flavor=gcc".into());
        }
        support::translate_slate(path, &generated, &translate_args).map(|()| support::Case {
            name: name.clone(),
            c_src: path.clone(),
            rs_src: generated,
            config: support::RunConfig {
                timeout_seconds: Some(5),
                c_args: extra_args,
                cc: corpus.gcc.then(|| "gcc".to_string()),
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

fn assert_supported(corpus: &Corpus) {
    let results = run_cases(corpus, "supported", &corpus.supported_root());
    let failures: Vec<_> = results
        .into_iter()
        .filter_map(|(name, result)| result.err().map(|e| (name, e)))
        .collect();
    assert!(
        failures.is_empty(),
        "{} supported tests failed:\n{}",
        corpus.dir,
        support::failure_report(&failures)
    );
}

fn assert_unsupported_still_fail(corpus: &Corpus) {
    let results = run_cases(corpus, "unsupported", &corpus.unsupported_root());
    let unexpected_passes: Vec<String> = results
        .into_iter()
        .filter_map(|(name, result)| result.ok().map(|()| name))
        .collect();
    assert!(
        unexpected_passes.is_empty(),
        "{} test(s) now pass end-to-end -- promote them:\n{}",
        corpus.dir,
        unexpected_passes
            .iter()
            .map(|name| format!(
                "  git mv crates/slate/tests/{dir}.unsupported/{name}.c crates/slate/tests/{dir}/{name}.c",
                dir = corpus.dir
            ))
            .collect::<Vec<_>>()
            .join("\n")
    );
}

#[test]
fn gcc_torture_supported_tests_match_c() {
    assert_supported(&CLANG);
}

#[test]
fn gcc_torture_unsupported_tests_still_fail() {
    assert_unsupported_still_fail(&CLANG);
}

#[test]
fn gcc_torture_gcc_only_supported_tests_match_c() {
    assert_supported(&GCC_ONLY);
}

#[test]
fn gcc_torture_gcc_only_unsupported_tests_still_fail() {
    assert_unsupported_still_fail(&GCC_ONLY);
}

#[test]
#[ignore]
fn gcc_torture_unsupported_triage_report() {
    support::print_triage(
        "",
        &run_cases(&CLANG, "unsupported", &CLANG.unsupported_root()),
    );
    support::print_triage(
        "gcc-only ",
        &run_cases(&GCC_ONLY, "unsupported", &GCC_ONLY.unsupported_root()),
    );
}
