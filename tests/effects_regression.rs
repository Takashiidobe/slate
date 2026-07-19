use std::path::{Path, PathBuf};
use std::process::Command;

const RUST_EFFECTS_COMMAND: &str = "compare-effects-rust-rust";

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

fn run_fixture(fixture: &Path) -> Result<(), String> {
    let output = Command::new(slate())
        .arg(RUST_EFFECTS_COMMAND)
        .arg(fixture)
        .output()
        .map_err(|e| format!("spawn slate {RUST_EFFECTS_COMMAND}: {e}"))?;
    if output.status.success() {
        return Ok(());
    }

    let stdout = String::from_utf8_lossy(&output.stdout);
    let stderr = String::from_utf8_lossy(&output.stderr);
    Err(format!(
        "slate {RUST_EFFECTS_COMMAND} {} failed with status {}\nstdout:\n{stdout}\nstderr:\n{stderr}",
        fixture.display(),
        output.status
    ))
}

fn assert_fixture_matches(fixture: &str) {
    run_fixture(Path::new(fixture)).expect("effects match");
}

fn assert_all_selected_fixtures_match() {
    let mut failures = Vec::new();
    let mut passed = 0usize;
    for (name, path) in selected_fixture_paths() {
        match run_fixture(&path) {
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
#[ignore = "diagnostic ratchet: run explicitly while expanding raw-to-fixuped Rust effects coverage"]
fn all_fixtures_preserve_rust_effects_through_fixups() {
    assert_all_selected_fixtures_match();
}

#[test]
fn switch_fixups_preserve_rust_effects() {
    assert_fixture_matches("tests/fixtures/switch_fallthrough.c");
}

#[test]
fn goto_fixups_preserve_rust_effects() {
    assert_fixture_matches("tests/fixtures/goto_forward.c");
}

#[test]
fn stdio_file_write_fixup_preserves_rust_effects() {
    assert_fixture_matches("tests/fixtures/effects_stdio_file_write.c");
}

#[test]
fn atomics_fixup_preserve_rust_effects() {
    assert_fixture_matches("tests/fixtures/atomics.c");
}

#[test]
fn atomic_orderings_fixup_preserves_rust_effects() {
    assert_fixture_matches("tests/fixtures/atomic_explicit_orderings.c");
}

#[test]
fn atomic_compare_exchange_fixup_preserves_rust_effects() {
    assert_fixture_matches("tests/fixtures/effects_atomic_compare_exchange.c");
}

#[test]
fn qsort_comparator_fixup_preserves_rust_effects() {
    assert_fixture_matches("tests/fixtures/effects_qsort_comparator.c");
}

#[test]
fn memcpy_fixups_preserve_rust_effects() {
    assert_fixture_matches("tests/stdlib/string/memcpy.c");
}

#[test]
fn memmove_fixups_preserve_rust_effects() {
    assert_fixture_matches("tests/stdlib/string/memmove.c");
}

#[test]
fn memset_fixups_preserve_rust_effects() {
    assert_fixture_matches("tests/stdlib/string/memset.c");
}

#[test]
fn memchr_fixups_preserve_rust_effects() {
    assert_fixture_matches("tests/stdlib/string/memchr.c");
}

#[test]
fn pointer_and_deref_fixups_preserve_rust_effects() {
    assert_fixture_matches("tests/fixtures/pointers.c");
    assert_fixture_matches("tests/fixtures/address_of_array_element.c");
    assert_fixture_matches("tests/fixtures/effects_malloc_array.c");
    assert_fixture_matches("tests/fixtures/ptr_member_store_unsafe.c");
    assert_fixture_matches("tests/fixtures/struct_with_array.c");
}

#[test]
fn ptr_len_slice_fixups_preserve_rust_effects() {
    assert_fixture_matches("tests/fixtures/ptr_len_slice.c");
    assert_fixture_matches("tests/fixtures/ptr_len_slice_item.c");
    assert_fixture_matches("tests/fixtures/ptr_len_slice_enumerate.c");
}

#[test]
fn libc_resource_shims_preserve_rust_effects() {
    assert_fixture_matches("tests/fixtures/heap_vec_malloc.c");
    assert_fixture_matches("tests/fixtures/heap_vec_calloc.c");
    assert_fixture_matches("tests/fixtures/string_copy_fixup.c");
    assert_fixture_matches("tests/fixtures/numeric_parse_fixup.c");
    assert_fixture_matches("tests/fixtures/string_param_lift.c");
    assert_fixture_matches("tests/fixtures/stdio_file_close_before_remove.c");
}

#[test]
fn scalar_constants_and_intrinsics_preserve_rust_effects() {
    assert_fixture_matches("tests/fixtures/extern_decl.c");
    assert_fixture_matches("tests/fixtures/integer_bit_builtin_ops.c");
    assert_fixture_matches("tests/fixtures/overflow_scalar_ops.c");
    assert_fixture_matches("tests/fixtures/sizeof.c");
    assert_fixture_matches("tests/fixtures/aligned_struct.c");
    assert_fixture_matches("tests/fixtures/packed_struct.c");
    assert_fixture_matches("tests/fixtures/control_flow_builtin_ops.c");
    assert_fixture_matches("tests/fixtures/printf_float.c");
    assert_fixture_matches("tests/fixtures/unary_negation.c");
    assert_fixture_matches("tests/fixtures/constant_object_query_ops.c");
}
