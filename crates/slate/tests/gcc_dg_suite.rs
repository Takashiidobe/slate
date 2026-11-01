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

struct Corpus {
    dir: &'static str,
    gcc: bool,
}

const CLANG: Corpus = Corpus {
    dir: "fixtures.gcc-dg",
    gcc: false,
};

const GCC_ONLY: Corpus = Corpus {
    dir: "fixtures.gcc-dg.gcc-only",
    gcc: true,
};

impl Corpus {
    fn root(&self, group: &str) -> PathBuf {
        if let Ok(source) = std::env::var("SLATE_GCC_DG_SOURCE") {
            return PathBuf::from(source);
        }
        let suffix = (group != "supported").then_some(format!(".{group}"));
        Path::new(env!("CARGO_MANIFEST_DIR")).join(format!(
            "tests/{}{}",
            self.dir,
            suffix.as_deref().unwrap_or_default()
        ))
    }
}

fn collect_cases(dir: &Path) -> Vec<(String, PathBuf)> {
    if !dir.exists() {
        return Vec::new();
    }
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

fn run_cases(corpus: &Corpus, group: &str, dir: &Path) -> Vec<(String, Result<(), String>)> {
    let cases = collect_cases(dir);
    let jobs = jobs();
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/gcc-dg-suite")
        .join(corpus.dir)
        .join(group);
    std::fs::create_dir_all(&work).expect("create work dir");

    let translated = support::parallel_map_with_jobs(&cases, jobs, |(name, path)| {
        let generated = work.join(format!("{name}.generated.rs"));
        let options = support::gnu_dg_option_flags(path);
        let mut translate_args = options.clone();
        if corpus.gcc {
            translate_args.push("--flavor=gcc".into());
        }
        support::translate_slate(path, &generated, &translate_args).map(|()| support::Case {
            name: name.clone(),
            c_src: path.clone(),
            rs_src: generated,
            config: support::RunConfig {
                timeout_seconds: Some(15),
                c_args: options
                    .into_iter()
                    .chain(["-latomic".to_string()])
                    .collect(),
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

fn assert_unsupported_still_fail(corpus: &Corpus) {
    let results = run_cases(corpus, "unsupported", &corpus.root("unsupported"));
    if let Ok(report) = std::env::var("SLATE_GCC_DG_FAILURES") {
        let text: String = results
            .iter()
            .map(|(name, result)| match result {
                Ok(()) => format!("{name}: PASS\n"),
                Err(e) => format!("{name}: {e}\n"),
            })
            .collect();
        std::fs::write(report, text).expect("write gcc.dg failure report");
    }
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
fn gcc_dg_unsupported_tests_still_fail() {
    assert_unsupported_still_fail(&CLANG);
}

#[test]
fn gcc_dg_gcc_only_unsupported_tests_still_fail() {
    assert_unsupported_still_fail(&GCC_ONLY);
}

#[test]
#[ignore]
fn gcc_dg_unsupported_triage_report() {
    support::print_triage(
        "",
        &run_cases(&CLANG, "unsupported", &CLANG.root("unsupported")),
    );
    support::print_triage(
        "gcc-only ",
        &run_cases(&GCC_ONLY, "unsupported", &GCC_ONLY.root("unsupported")),
    );
}

#[test]
fn gcc_dg_supported_tests_match_c() {
    assert_supported(&CLANG);
}

#[test]
fn gcc_dg_gcc_only_supported_tests_match_c() {
    assert_supported(&GCC_ONLY);
}

fn assert_supported(corpus: &Corpus) {
    let results = run_cases(corpus, "supported", &corpus.root("supported"));
    let failures: Vec<_> = results
        .into_iter()
        .filter_map(|(name, result)| result.err().map(|e| (name, e)))
        .collect();
    if let Ok(report) = std::env::var("SLATE_GCC_DG_FAILURES") {
        let text: Vec<String> = failures
            .iter()
            .map(|(name, e)| format!("{name}: {e}"))
            .collect();
        std::fs::write(report, text.join("\n")).expect("write gcc.dg failure report");
    }
    assert!(
        failures.is_empty(),
        "{} supported tests failed:\n{}",
        corpus.dir,
        support::failure_report(&failures)
    );
}
