use std::path::{Path, PathBuf};
use std::process::Command;

fn slate() -> &'static str {
    env!("CARGO_BIN_EXE_slate")
}

fn fixture_paths() -> Vec<(String, PathBuf)> {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures");
    let mut fixtures = Vec::new();
    for entry in std::fs::read_dir(&dir).expect("read tests/fixtures") {
        let path = entry.expect("read fixture entry").path();
        if path.extension().and_then(|ext| ext.to_str()) != Some("c") {
            continue;
        }
        let name = path
            .file_name()
            .expect("fixture file name")
            .to_string_lossy()
            .into_owned();
        fixtures.push((name, path));
    }
    fixtures.sort_by(|a, b| a.0.cmp(&b.0));
    fixtures
}

fn selected_fixture_paths() -> Vec<(String, PathBuf)> {
    let Some(filter) = std::env::var("SLATE_EFFECT_FIXTURE")
        .ok()
        .filter(|filter| !filter.trim().is_empty())
    else {
        return fixture_paths();
    };
    let filter = filter.trim();
    let selected: Vec<_> = fixture_paths()
        .into_iter()
        .filter(|(name, path)| {
            name == filter || path.file_stem().and_then(|stem| stem.to_str()) == Some(filter)
        })
        .collect();
    assert!(
        !selected.is_empty(),
        "SLATE_EFFECT_FIXTURE={filter} did not match any tests/fixtures/*.c file"
    );
    selected
}

fn run_fixture(command: &str, fixture: &Path) -> Result<(), String> {
    let output = Command::new(slate())
        .arg(command)
        .arg(fixture)
        .output()
        .map_err(|e| format!("spawn slate {command}: {e}"))?;
    if output.status.success() {
        return Ok(());
    }

    let stdout = String::from_utf8_lossy(&output.stdout);
    let stderr = String::from_utf8_lossy(&output.stderr);
    Err(format!(
        "slate {command} {} failed with status {}\nstdout:\n{stdout}\nstderr:\n{stderr}",
        fixture.display(),
        output.status
    ))
}

fn assert_fixture_matches(command: &str, fixture: &str) {
    run_fixture(command, Path::new(fixture)).expect("effects match");
}

fn assert_all_selected_fixtures_match(command: &str) {
    let mut failures = Vec::new();
    let mut passed = 0usize;
    for (name, path) in selected_fixture_paths() {
        match run_fixture(command, &path) {
            Ok(()) => passed += 1,
            Err(err) => failures.push(format!("[{name}] {err}")),
        }
    }

    if failures.is_empty() {
        return;
    }

    panic!(
        "{} fixture(s) passed; {} fixture(s) failed:\n\n{}",
        passed,
        failures.len(),
        failures.join("\n\n")
    );
}

#[test]
#[ignore = "diagnostic ratchet: run explicitly while expanding effects interpreter coverage"]
fn all_fixtures_match_cir_effects() {
    assert_all_selected_fixtures_match("compare-effects-cir-rust");
}

#[test]
#[ignore = "diagnostic ratchet: run explicitly while expanding raw-to-fixuped Rust effects coverage"]
fn all_fixtures_preserve_rust_effects_through_fixups() {
    assert_all_selected_fixtures_match("compare-effects-rust-rust");
}

#[test]
fn idiomatized_malloc_array_fixture_matches_cir_effects() {
    assert_fixture_matches(
        "compare-effects-cir-rust",
        "tests/fixtures/effects_malloc_array.c",
    );
}

#[test]
fn idiomatized_printf_fixture_matches_cir_effects() {
    assert_fixture_matches(
        "compare-effects-cir-rust",
        "tests/fixtures/effects_printf.c",
    );
}

#[test]
fn idiomatized_for_loop_fixture_matches_cir_effects() {
    assert_fixture_matches(
        "compare-effects-cir-rust",
        "tests/fixtures/effects_for_loop.c",
    );
}

#[test]
fn idiomatized_struct_field_fixture_matches_cir_effects() {
    assert_fixture_matches(
        "compare-effects-cir-rust",
        "tests/fixtures/effects_struct_field.c",
    );
}

#[test]
fn idiomatized_static_global_fixture_matches_cir_effects() {
    assert_fixture_matches(
        "compare-effects-cir-rust",
        "tests/fixtures/effects_static_globals.c",
    );
}

#[test]
fn idiomatized_lazy_singleton_fixture_matches_cir_effects() {
    assert_fixture_matches(
        "compare-effects-cir-rust",
        "tests/fixtures/lazy_singleton.c",
    );
}

#[test]
fn idiomatized_nullable_pointer_fixture_matches_cir_effects() {
    assert_fixture_matches(
        "compare-effects-cir-rust",
        "tests/fixtures/effects_nullable_pointer.c",
    );
}

#[test]
fn idiomatized_stdio_file_write_fixture_matches_cir_effects() {
    assert_fixture_matches(
        "compare-effects-cir-rust",
        "tests/fixtures/effects_stdio_file_write.c",
    );
}

#[test]
fn stdio_file_write_fixup_preserves_rust_effects() {
    assert_fixture_matches(
        "compare-effects-rust-rust",
        "tests/fixtures/effects_stdio_file_write.c",
    );
}

#[test]
fn idiomatized_atomics_fixture_matches_cir_effects() {
    assert_fixture_matches("compare-effects-cir-rust", "tests/fixtures/atomics.c");
}

#[test]
fn atomics_fixup_preserves_rust_effects() {
    assert_fixture_matches("compare-effects-rust-rust", "tests/fixtures/atomics.c");
}

#[test]
fn idiomatized_atomic_orderings_fixture_matches_cir_effects() {
    assert_fixture_matches(
        "compare-effects-cir-rust",
        "tests/fixtures/atomic_explicit_orderings.c",
    );
}

#[test]
fn atomic_orderings_fixup_preserves_rust_effects() {
    assert_fixture_matches(
        "compare-effects-rust-rust",
        "tests/fixtures/atomic_explicit_orderings.c",
    );
}

#[test]
fn idiomatized_atomic_compare_exchange_fixture_matches_cir_effects() {
    assert_fixture_matches(
        "compare-effects-cir-rust",
        "tests/fixtures/effects_atomic_compare_exchange.c",
    );
}

#[test]
fn atomic_compare_exchange_fixup_preserves_rust_effects() {
    assert_fixture_matches(
        "compare-effects-rust-rust",
        "tests/fixtures/effects_atomic_compare_exchange.c",
    );
}

#[test]
fn idiomatized_qsort_comparator_fixture_matches_cir_effects() {
    assert_fixture_matches(
        "compare-effects-cir-rust",
        "tests/fixtures/effects_qsort_comparator.c",
    );
}

#[test]
fn qsort_comparator_fixup_preserves_rust_effects() {
    assert_fixture_matches(
        "compare-effects-rust-rust",
        "tests/fixtures/effects_qsort_comparator.c",
    );
}
