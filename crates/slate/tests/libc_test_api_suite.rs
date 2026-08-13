mod support;

use std::path::{Path, PathBuf};

use support::libc_shim::{Architecture, LibcVariant, TestConfig, compile_test_program_with_args};

const API_EXTRA_ARGS: &[&str] = &[
    "-std=c99",
    "-pedantic-errors",
    "-Werror",
    "-Wno-unused",
    "-Wno-switch-bool",
    "-Wno-strict-prototypes",
    "-D_XOPEN_SOURCE=700",
];

fn libc_test_jobs() -> usize {
    std::env::var("SLATE_LIBC_TEST_JOBS")
        .ok()
        .and_then(|value| value.parse().ok())
        .filter(|jobs| *jobs > 0)
        .or_else(|| {
            std::env::var("SLATE_TEST_JOBS")
                .ok()
                .and_then(|value| value.parse().ok())
                .filter(|jobs| *jobs > 0)
        })
        .unwrap_or_else(support::test_jobs)
}

fn supported_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.libc-test/api/supported")
}

fn unsupported_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.libc-test/api/unsupported")
}

fn collect_cases(dir: &Path) -> Vec<(String, PathBuf)> {
    let selected = std::env::var("SLATE_LIBC_TEST_FIXTURE").ok();
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

const API_CONFIGS: &[(Architecture, LibcVariant)] = &[
    (Architecture::X86_64, LibcVariant::Glibc),
    (Architecture::X86_64, LibcVariant::Musl),
];

fn run_cases(dir: &Path) -> Vec<(String, Result<(), String>)> {
    let cases = collect_cases(dir);
    support::parallel_map_with_jobs(&cases, libc_test_jobs(), |(name, path)| {
        let source = std::fs::read_to_string(path)
            .unwrap_or_else(|e| panic!("read {}: {e}", path.display()));
        let result = API_CONFIGS.iter().try_for_each(|(arch, libc)| {
            let config = TestConfig::new(*arch, *libc);
            compile_test_program_with_args(&config, &source, API_EXTRA_ARGS)
        });
        (name.clone(), result)
    })
}

#[test]
fn libc_test_api_supported_tests_compile() {
    let results = run_cases(&supported_root());
    let failures: Vec<String> = results
        .into_iter()
        .filter_map(|(name, result)| result.err().map(|e| format!("{name}: {e}")))
        .collect();
    assert!(
        failures.is_empty(),
        "libc-test api supported tests failed:\n{}",
        failures.join("\n\n")
    );
}

#[test]
#[ignore = "run manually to find cases ready for promotion"]
fn libc_test_api_unsupported_tests_still_fail() {
    let results = run_cases(&unsupported_root());
    let unexpected_passes: Vec<String> = results
        .into_iter()
        .filter_map(|(name, result)| result.ok().map(|()| name))
        .collect();
    assert!(
        unexpected_passes.is_empty(),
        "libc-test api test(s) now compile cleanly -- promote them:\n{}",
        unexpected_passes
            .iter()
            .map(|name| format!(
                "  git mv tests/fixtures.libc-test/api/unsupported/{name}.c tests/fixtures.libc-test/api/supported/{name}.c"
            ))
            .collect::<Vec<_>>()
            .join("\n")
    );
}

#[test]
#[ignore]
fn libc_test_api_unsupported_triage_report() {
    let results = run_cases(&unsupported_root());
    for (name, result) in results {
        match result {
            Ok(()) => println!("PASS {name}"),
            Err(error) => println!("FAIL {name}\n{error}\n"),
        }
    }
}
