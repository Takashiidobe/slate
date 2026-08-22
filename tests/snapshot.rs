//! Snapshot review harness: not a correctness gate. Records the generated
//! Rust for tests/fixtures/*.c so codegen changes show up as a normal git
//! diff in tests/snapshots/ instead of requiring a manual `translate` run.
//! Correctness stays owned by the differential/*_suite test binaries.

mod support;

use std::path::{Path, PathBuf};

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

#[test]
fn translated_fixtures_match_snapshots() {
    let mut settings = insta::Settings::clone_current();
    settings.set_snapshot_path("snapshots");
    settings.set_prepend_module_to_snapshot(false);
    let _guard = settings.bind_to_scope();

    let mut paths: Vec<PathBuf> = std::fs::read_dir(fixtures_dir())
        .expect("read tests/fixtures")
        .filter_map(|entry| entry.ok().map(|entry| entry.path()))
        .filter(|path| path.extension().and_then(|ext| ext.to_str()) == Some("c"))
        .collect();
    paths.sort();

    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/snapshot-generated");
    std::fs::create_dir_all(&tmp).expect("create snapshot work dir");

    let mut failures = Vec::new();
    for path in paths {
        let name = path.file_stem().unwrap().to_string_lossy().into_owned();
        let out = tmp.join(format!("{name}.rs"));
        match support::translate(&path, &out) {
            Ok(()) => {
                let rust = std::fs::read_to_string(&out).expect("read generated Rust");
                insta::assert_snapshot!(name, rust);
            }
            Err(e) => failures.push(format!("[{name}] {e}")),
        }
    }

    assert!(
        failures.is_empty(),
        "{} of the fixtures failed to translate (not a snapshot mismatch -- these never \
         produced Rust to snapshot):\n\n{}",
        failures.len(),
        failures.join("\n\n")
    );
}
