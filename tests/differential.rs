//! Differential test harness: compile and run C plus Rust, then require
//! identical stdout and exit code.
//!
//! Compilers are overridable so the CIR-built clang can be swapped in:
//!   SLATE_CC=~/llvm-project/build-cir/bin/clang cargo test

mod support;

use std::path::{Path, PathBuf};

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

fn fixtures() -> Vec<(String, PathBuf)> {
    let dir = fixtures_dir();
    let mut fixtures = Vec::new();
    for entry in std::fs::read_dir(&dir).expect("read fixtures dir") {
        let path = entry.expect("dir entry").path();
        if path.extension().and_then(|e| e.to_str()) != Some("c") {
            continue;
        }
        let name = path.file_stem().unwrap().to_string_lossy().into_owned();
        fixtures.push((name, path));
    }
    fixtures.sort_by(|a, b| a.0.cmp(&b.0));
    fixtures
}

#[test]
fn generated_differential() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let fixtures = fixtures();
    assert!(
        !fixtures.is_empty(),
        "no fixtures found in {:?}",
        fixtures_dir()
    );

    let mut failures = Vec::new();
    for (name, c_src) in &fixtures {
        let generated = tmp.join(format!("{name}.generated.rs"));
        match support::translate(c_src, &generated)
            .and_then(|()| support::compare(name, c_src, &generated, &tmp))
        {
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
