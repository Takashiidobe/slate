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

    let mut cases = Vec::new();
    let mut failures = Vec::new();
    for (name, c_src) in &fixtures {
        let generated = tmp.join(format!("{name}.generated.rs"));
        match support::translate(c_src, &generated) {
            Ok(()) => cases.push(support::Case {
                name: name.clone(),
                c_src: c_src.clone(),
                rs_src: generated,
                config: support::RunConfig::default(),
            }),
            Err(e) => {
                eprintln!("FAIL  {name}");
                failures.push(format!("[{name}] {e}"));
            }
        }
    }

    for (name, result) in support::compare_batch(&cases, &tmp) {
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
fn volatile_uses_rust_volatile_intrinsics() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-volatile");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("volatile.c");
    let generated = tmp.join("volatile.generated.rs");

    support::translate(&c_src, &generated).expect("translate volatile fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated volatile rust");
    assert!(rust.contains("std::ptr::read_volatile"));
    assert!(rust.contains("std::ptr::write_volatile"));
}

#[test]
fn file_scope_static_emits_rust_static_mut() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-static-globals");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("static_globals.c");
    let generated = tmp.join("static_globals.generated.rs");

    support::translate(&c_src, &generated).expect("translate static globals fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated static globals rust");
    assert!(rust.contains("static mut counter: i32 = 2;"));
    assert!(!rust.contains("*counter"));
}
