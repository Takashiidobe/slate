mod support;

use std::path::{Path, PathBuf};

fn fixtures_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.chibicc")
}

fn discover_cases(bucket: &str) -> Vec<(String, PathBuf)> {
    let dir = fixtures_root().join(bucket);
    let mut cases: Vec<(String, PathBuf)> = std::fs::read_dir(&dir)
        .unwrap_or_else(|e| panic!("read {}: {e}", dir.display()))
        .filter_map(|e| e.ok())
        .map(|e| e.path())
        .filter(|p| p.is_dir())
        .map(|p| {
            let name = p.file_name().unwrap().to_string_lossy().into_owned();
            (name, p)
        })
        .collect();
    cases.sort();
    cases
}

fn c_sources(dir: &Path) -> Vec<PathBuf> {
    let mut srcs: Vec<PathBuf> = std::fs::read_dir(dir)
        .expect("read fixture dir")
        .filter_map(|e| e.ok().map(|e| e.path()))
        .filter(|p| p.extension().and_then(|e| e.to_str()) == Some("c"))
        .collect();
    srcs.sort();
    srcs
}

struct Attempt {
    name: String,
    dir: PathBuf,
    multi_bin: Option<support::MultiBinCase>,
    translate_error: Option<String>,
}

fn fixture_std(dir: &Path) -> &'static str {
    if dir.join("gnu23.txt").is_file() {
        "gnu23"
    } else {
        "gnu11"
    }
}

fn attempt_translate(name: &str, dir: &Path, work: &Path) -> Attempt {
    let out_dir = work.join("translated").join(name);
    let _ = std::fs::remove_dir_all(&out_dir);
    match support::translate_project_with_std(dir, &out_dir, fixture_std(dir)) {
        Ok(()) => {
            let types_rs = out_dir.join("src/types.rs");
            Attempt {
                name: name.to_string(),
                dir: dir.to_path_buf(),
                multi_bin: Some(support::MultiBinCase {
                    name: name.to_string(),
                    main_rs: out_dir.join("src/main.rs"),
                    common_rs: out_dir.join("src/common.rs"),
                    types_rs: types_rs.is_file().then_some(types_rs),
                }),
                translate_error: None,
            }
        }
        Err(e) => Attempt {
            name: name.to_string(),
            dir: dir.to_path_buf(),
            multi_bin: None,
            translate_error: Some(e),
        },
    }
}

fn run_bucket(bucket: &str) -> Vec<(String, Result<(), String>)> {
    let cases = discover_cases(bucket);
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/chibicc-suite")
        .join(bucket);
    std::fs::create_dir_all(&work).expect("create work dir");

    let attempts = support::parallel_map(&cases, |(name, dir)| attempt_translate(name, dir, &work));

    let batch_cases: Vec<support::MultiBinCase> = attempts
        .iter()
        .filter_map(|a| a.multi_bin.as_ref())
        .map(|c| support::MultiBinCase {
            name: c.name.clone(),
            main_rs: c.main_rs.clone(),
            common_rs: c.common_rs.clone(),
            types_rs: c.types_rs.clone(),
        })
        .collect();

    let project = work.join("batch_cargo");
    let batch_build = if batch_cases.is_empty() {
        None
    } else {
        Some(
            support::build_multi_bin_batch(&batch_cases, &project)
                .expect("spawn batched cargo build"),
        )
    };

    support::parallel_map(&attempts, |attempt| {
        let result = (|| -> Result<(), String> {
            if attempt.multi_bin.is_none() {
                return Err(format!(
                    "translate-project failed: {}",
                    attempt
                        .translate_error
                        .as_deref()
                        .unwrap_or("unknown error")
                ));
            }
            let rs_bin = batch_build
                .as_ref()
                .ok_or_else(|| "Rust batch build produced no artifacts".to_string())?
                .executable(&attempt.name)
                .map_err(|error| format!("Rust build failed:\n{error}"))?;
            let c_bin = work.join(format!("{}_c", attempt.name));
            support::compile_c_multi_with_std(
                &c_sources(&attempt.dir),
                &c_bin,
                fixture_std(&attempt.dir),
            )?;
            let run_dir = work.join("runs").join(&attempt.name);
            let _ = std::fs::remove_dir_all(&run_dir);
            std::fs::create_dir_all(&run_dir)
                .map_err(|e| format!("create {}: {e}", run_dir.display()))?;
            let cfg = support::RunConfig::default();
            let c = support::run_with_config(&c_bin, &cfg, &run_dir)?;
            let r = support::run_with_config(&rs_bin, &cfg, &run_dir)?;
            support::compare_runs(&c, &r, false)
        })();
        (attempt.name.clone(), result)
    })
}

#[test]
fn chibicc_supported_tests_match_c() {
    let results = run_bucket("supported");
    let failures: Vec<String> = results
        .into_iter()
        .filter_map(|(name, result)| result.err().map(|e| format!("{name}: {e}")))
        .collect();
    assert!(
        failures.is_empty(),
        "chibicc supported tests failed:\n{}",
        failures.join("\n\n")
    );
}

#[test]
#[ignore]
fn chibicc_unsupported_triage_report() {
    let results = run_bucket("unsupported");
    for (name, result) in results {
        if let Err(e) = result {
            println!("=== {name} ===\n{e}\n");
        }
    }
}

#[test]
fn chibicc_unsupported_tests_still_fail() {
    let results = run_bucket("unsupported");
    let unexpected_passes: Vec<String> = results
        .into_iter()
        .filter_map(|(name, result)| result.is_ok().then_some(name))
        .collect();
    assert!(
        unexpected_passes.is_empty(),
        "chibicc test(s) now pass end-to-end -- promote them:\n{}",
        unexpected_passes
            .iter()
            .map(|name| format!(
                "  git mv tests/fixtures.chibicc/unsupported/{name} tests/fixtures.chibicc/supported/{name}"
            ))
            .collect::<Vec<_>>()
            .join("\n")
    );
}
