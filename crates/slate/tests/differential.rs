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
fn compound_assignment_temps_are_inlined() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-compound-fixup");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("compound_assignments.c");
    let generated = tmp.join("compound_assignments.generated.rs");

    support::translate(&c_src, &generated).expect("translate compound fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated compound rust");
    for expr in [
        "a -= 5;",
        "a *= 3;",
        "a /= 5;",
        "a %= 7;",
        "a <<= 3;",
        "a >>= 2;",
        "a &= 6;",
        "a ^= 3;",
        "a |= 8;",
    ] {
        assert!(rust.contains(expr), "missing compound expression: {expr}");
    }
    assert!(!rust.contains("let _v1: i32 = 20;"));
    assert!(!rust.contains("let _v4: i32 = (a - 5);"));
}

#[test]
fn call_lowering_preserves_function_pointer_and_extern_shapes() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-call-lowering");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let fp_c = fixtures_dir().join("function_pointers.c");
    let fp_generated = tmp.join("function_pointers.generated.rs");
    support::translate(&fp_c, &fp_generated).expect("translate function pointer fixture");
    let fp_rust =
        std::fs::read_to_string(&fp_generated).expect("read generated function pointer rust");
    assert!(fp_rust.contains("Some(add_pair)"));
    assert!(fp_rust.contains(".unwrap()("));

    let extern_c = fixtures_dir().join("extern_decl.c");
    let extern_generated = tmp.join("extern_decl.generated.rs");
    support::translate(&extern_c, &extern_generated).expect("translate extern fixture");
    let extern_rust =
        std::fs::read_to_string(&extern_generated).expect("read generated extern rust");
    assert!(extern_rust.contains("unsafe { toupper("));
    assert!(extern_rust.contains(" as i32) }"));
    assert!(extern_rust.contains("unsafe { printf("));
}

#[test]
fn switch_and_dispatch_use_block_match_arms() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-control-ast");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let switch_c = fixtures_dir().join("switch_fallthrough.c");
    let switch_generated = tmp.join("switch_fallthrough.generated.rs");
    support::translate(&switch_c, &switch_generated).expect("translate switch fixture");
    let switch_rust =
        std::fs::read_to_string(&switch_generated).expect("read generated switch rust");
    assert!(switch_rust.contains("match __switch_case0 {\n                    0 => {"));
    assert!(!switch_rust.contains("_ => break '__switch0,"));

    let goto_c = fixtures_dir().join("goto_if_scope.c");
    let goto_generated = tmp.join("goto_if_scope.generated.rs");
    support::translate(&goto_c, &goto_generated).expect("translate goto fixture");
    let goto_rust = std::fs::read_to_string(&goto_generated).expect("read generated goto rust");
    assert!(goto_rust.contains("match __state0 {\n            0 => {"));
    assert!(!goto_rust.contains("_ => break '__dispatch0,"));
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

#[test]
fn file_scope_globals_emit_static_mut_definitions() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-global-vars");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("global_vars.c");
    let generated = tmp.join("global_vars.generated.rs");

    support::translate(&c_src, &generated).expect("translate global vars fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated global vars rust");
    assert!(rust.contains("static mut counter: i32 = 4;"));
    assert!(rust.contains("static mut zeroed: i32 = 0;"));
    assert!(rust.contains("static mut numbers: [i32; 4] = [1, 2, 0, 0];"));
    assert!(rust.contains("static mut pair: Pair = Pair { left: 3, right: 5 };"));
}

#[test]
fn assignment_places_cover_slots_globals_members_elements_and_derefs() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-assignment-places");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    for (fixture, expected) in [
        (
            "pointers",
            &["local = value;", "*slot = _v", "values[(2 as usize)] = 12;"][..],
        ),
        (
            "struct_with_array",
            &["b.data[(0 as usize)] = 10;", "b.len = 3;"][..],
        ),
        (
            "global_vars",
            &[
                "counter = _v",
                "numbers[(2 as usize)] = _v",
                "pair.right = _v",
            ][..],
        ),
        ("bitfield_ops", &["s.a = _v", "s.b = _v", "w.x = _v"][..]),
    ] {
        let c_src = fixtures_dir().join(format!("{fixture}.c"));
        let generated = tmp.join(format!("{fixture}.generated.rs"));
        support::translate(&c_src, &generated).unwrap_or_else(|err| {
            panic!("translate {fixture} fixture: {err}");
        });
        let rust = std::fs::read_to_string(&generated)
            .unwrap_or_else(|err| panic!("read generated {fixture} rust: {err}"));
        for snippet in expected {
            assert!(
                rust.contains(snippet),
                "missing assignment snippet in {fixture}: {snippet}"
            );
        }
    }
}
