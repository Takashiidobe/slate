//! Cross-translation-unit differential tests: a directory of C files is
//! translated into separate Rust modules that import each other (rather than
//! one aggregated file), then C-linked-together and Rust-crate builds are run
//! and compared for identical stdout and exit code.

mod support;

use std::path::{Path, PathBuf};

fn fixture_dir(name: &str) -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.multi")
        .join(name)
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

/// Translate a multi-TU fixture into Rust modules, then diff the C (all units
/// linked) against the Rust (all modules built as one crate). Returns the
/// directory of generated `.rs` modules for per-test structural assertions.
fn build_and_diff(name: &str) -> PathBuf {
    let dir = fixture_dir(name);
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/cross-tu")
        .join(name);
    std::fs::create_dir_all(&work).expect("create work dir");

    let c_bin = work.join("c_bin");
    support::compile_c_multi(&c_sources(&dir), &c_bin).expect("compile C");

    let rs_dir = work.join("rs");
    let _ = std::fs::remove_dir_all(&rs_dir);
    support::translate_project(&dir, &rs_dir).expect("translate project");

    let rs_bin = support::compile_rs_project(&rs_dir, &work, name).expect("compile Rust");

    let run_dir = work.join("run");
    let _ = std::fs::remove_dir_all(&run_dir);
    std::fs::create_dir_all(&run_dir).expect("create run dir");
    let cfg = support::RunConfig::default();
    let c = support::run_with_config(&c_bin, &cfg, &run_dir).expect("run C");
    let r = support::run_with_config(&rs_bin, &cfg, &run_dir).expect("run Rust");
    support::compare_runs(&c, &r, false).expect("C and Rust outputs differ");

    rs_dir
}

#[test]
fn library_project_creates_cargo_crate_without_main() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.library")
        .join("simple");
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/cross-tu")
        .join("library-project");
    let crate_dir = work.join("crate");
    let _ = std::fs::remove_dir_all(&crate_dir);

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate-project", "--lib"])
        .arg(&dir)
        .arg(&crate_dir)
        .output()
        .expect("run slate translate-project --lib");
    assert!(
        output.status.success(),
        "translate-project --lib failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );

    assert!(crate_dir.join("Cargo.toml").is_file());
    let lib_rs = std::fs::read_to_string(crate_dir.join("src/lib.rs")).expect("read lib.rs");
    assert!(lib_rs.contains("pub mod math;"));
    assert!(lib_rs.contains("pub mod state;"));

    let math_rs = std::fs::read_to_string(crate_dir.join("src/math.rs")).expect("read math.rs");
    assert!(math_rs.contains("pub fn square"));
    assert!(crate_dir.join("tests/run_smoke.rs").is_file());

    let check = std::process::Command::new("cargo")
        .args(["check", "--quiet", "--lib", "--manifest-path"])
        .arg(crate_dir.join("Cargo.toml"))
        .output()
        .expect("cargo check generated lib crate");
    assert!(
        check.status.success(),
        "generated lib crate should type-check:\n{}",
        String::from_utf8_lossy(&check.stderr)
    );
}

#[test]
fn cross_tu_functions() {
    let rs_dir = build_and_diff("cross_tu");

    // definitions must be split into modules, not aggregated.
    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");
    assert!(
        main_rs.contains("mod math;"),
        "root should declare sibling module"
    );
    assert!(
        main_rs.contains("use crate::math::square;"),
        "cross-TU prototype should import from sibling, not extern C"
    );
    assert!(
        !main_rs.contains("fn square"),
        "definition must live in its own module, not the root"
    );
    let math_rs = std::fs::read_to_string(rs_dir.join("math.rs")).expect("read math.rs");
    assert!(
        math_rs.contains("pub fn square"),
        "sibling defs must be pub"
    );
}

#[test]
fn cross_tu_static_linkage() {
    let rs_dir = build_and_diff("static_linkage");

    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");
    assert!(
        main_rs.contains("use crate::other::compute;"),
        "external-linkage fn should import from its defining module"
    );
    assert!(
        !main_rs.contains("pub fn local") && main_rs.contains("fn local"),
        "internal-linkage fn must not be pub"
    );
    assert!(
        !main_rs.contains("pub static mut base") && main_rs.contains("static mut base"),
        "internal-linkage global must not be pub"
    );

    let other_rs = std::fs::read_to_string(rs_dir.join("other.rs")).expect("read other.rs");
    assert!(
        other_rs.contains("pub fn compute"),
        "external-linkage fn must be exported pub"
    );
    assert!(
        !other_rs.contains("pub fn local") && other_rs.contains("fn local"),
        "sibling's internal-linkage fn must not be pub"
    );
    assert!(
        !other_rs.contains("pub static mut base") && other_rs.contains("static mut base"),
        "sibling's internal-linkage global must not be pub"
    );
}

#[test]
fn cross_tu_globals() {
    let rs_dir = build_and_diff("globals");

    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");
    assert!(
        main_rs.contains("use crate::state::counter;"),
        "extern global should import from its defining module, not extern C"
    );
    assert!(
        !main_rs.contains("static mut counter"),
        "extern global must not be redeclared as an extern static in the root"
    );
    let state_rs = std::fs::read_to_string(rs_dir.join("state.rs")).expect("read state.rs");
    assert!(
        state_rs.contains("pub static mut counter"),
        "the defining module must export the global as pub"
    );
}
