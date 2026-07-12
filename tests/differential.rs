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
        "a -= 5;", "a *= 3;", "a /= 5;", "a %= 7;", "a <<= 3;", "a >>= 2;", "a &= 6;", "a ^= 3;",
        "a |= 8;",
    ] {
        assert!(rust.contains(expr), "missing compound expression: {expr}");
    }
    assert!(!rust.contains("let _v1: i32 = 20;"));
    assert!(!rust.contains("let _v4: i32 = (a - 5);"));
}

#[test]
fn unused_call_result_temps_are_dropped() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-drop-call-result");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("compound_assignments.c");
    let generated = tmp.join("compound_assignments.generated.rs");

    support::translate(&c_src, &generated).expect("translate compound fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated compound rust");
    assert!(rust.contains("println!(\"{}\", _v"));
    assert!(!rust.contains(": i32 = unsafe { printf("));
    assert!(!rust.contains(": i32 = println!("));
}

#[test]
fn call_argument_temps_are_inlined() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-call-arg-fixup");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("add.c");
    let generated = tmp.join("add.generated.rs");

    support::translate(&c_src, &generated).expect("translate add fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated add rust");
    for call in ["add(2, 3)", "add(-10, 4)", "add(0, 0)"] {
        assert!(rust.contains(call), "missing inlined call: {call}");
    }
    assert!(!rust.contains("= add(_v"));
    assert!(!rust.contains("let _v1: i32 = 2;"));
}

#[test]
fn simple_printfs_are_recovered_as_format_macros() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-printf-format-fixup");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("add.c");
    let generated = tmp.join("add.generated.rs");
    support::translate(&c_src, &generated).expect("translate add fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated add rust");
    for call in [
        "println!(\"{}\", add(2, 3));",
        "println!(\"{}\", add(-10, 4));",
        "println!(\"{}\", add(0, 0));",
    ] {
        assert!(rust.contains(call), "missing recovered printf: {call}");
    }
    assert!(!rust.contains("fn printf("));
    assert!(!rust.contains("unsafe { printf("));

    let logical_c = fixtures_dir().join("logical_ops.c");
    let logical_generated = tmp.join("logical_ops.generated.rs");
    support::translate(&logical_c, &logical_generated).expect("translate logical_ops fixture");
    let logical_rust =
        std::fs::read_to_string(&logical_generated).expect("read generated logical_ops rust");
    assert!(logical_rust.contains("println!(\"{} {}\","));
    assert!(logical_rust.contains("println!(\"{} {} {}\","));
    assert!(!logical_rust.contains("fn printf("));
    assert!(!logical_rust.contains("unsafe { printf("));

    for fixture in ["unsigned", "longs", "longlong", "stdint_types"] {
        let c_src = fixtures_dir().join(format!("{fixture}.c"));
        let generated = tmp.join(format!("{fixture}.generated.rs"));
        support::translate(&c_src, &generated)
            .unwrap_or_else(|err| panic!("translate {fixture} fixture: {err}"));
        let rust = std::fs::read_to_string(&generated)
            .unwrap_or_else(|err| panic!("read generated {fixture} rust: {err}"));
        assert!(
            rust.contains("println!(\"{}"),
            "missing recovered integer printf in {fixture}"
        );
        assert!(
            !rust.contains("fn printf("),
            "printf extern should be removed in {fixture}"
        );
        assert!(
            !rust.contains("unsafe { printf("),
            "printf call should be removed in {fixture}"
        );
    }

    let widths_c = fixtures_dir().join("printf_integer_widths.c");
    let widths_generated = tmp.join("printf_integer_widths.generated.rs");
    support::translate(&widths_c, &widths_generated).expect("translate printf widths fixture");
    let widths_rust =
        std::fs::read_to_string(&widths_generated).expect("read generated printf widths rust");
    assert!(widths_rust.contains("println!(\"{:05}|{:<4}|{:+}|{:5}|{:+06}\","));
    assert!(!widths_rust.contains("fn printf("));
    assert!(!widths_rust.contains("unsafe { printf("));

    let hex_octal_c = fixtures_dir().join("printf_hex_octal.c");
    let hex_octal_generated = tmp.join("printf_hex_octal.generated.rs");
    support::translate(&hex_octal_c, &hex_octal_generated)
        .expect("translate printf hex/octal fixture");
    let hex_octal_rust = std::fs::read_to_string(&hex_octal_generated)
        .expect("read generated printf hex/octal rust");
    assert!(hex_octal_rust.contains("println!(\"{:x} {:X} {:o}\","));
    assert!(hex_octal_rust.contains("println!(\"{:08x}|{:<4X}|{:5o}\","));
    assert!(!hex_octal_rust.contains("fn printf("));
    assert!(!hex_octal_rust.contains("unsafe { printf("));

    let string_char_c = fixtures_dir().join("printf_string_char.c");
    let string_char_generated = tmp.join("printf_string_char.generated.rs");
    support::translate(&string_char_c, &string_char_generated)
        .expect("translate printf string/char fixture");
    let string_char_rust = std::fs::read_to_string(&string_char_generated)
        .expect("read generated printf string/char rust");
    assert!(string_char_rust.contains("println!(\"{} {} {} {}\", \"tag\", \"A\", \"\\n\", _v3);"));
    assert!(string_char_rust.contains("print!(\"literal={}\", \"tail\");"));
    assert!(!string_char_rust.contains("fn printf("));
    assert!(!string_char_rust.contains("unsafe { printf("));

    let rejected_c = fixtures_dir().join("printf_string_char_rejected.c");
    let rejected_generated = tmp.join("printf_string_char_rejected.generated.rs");
    support::translate(&rejected_c, &rejected_generated)
        .expect("translate rejected printf string/char fixture");
    let rejected_rust = std::fs::read_to_string(&rejected_generated)
        .expect("read generated rejected printf string/char rust");
    assert!(rejected_rust.contains("fn printf("));
    assert!(rejected_rust.contains("unsafe { printf("));
    assert!(!rejected_rust.contains("println!(\"{}\""));

    let float_c = fixtures_dir().join("printf_float.c");
    let float_generated = tmp.join("printf_float.generated.rs");
    support::translate(&float_c, &float_generated).expect("translate printf float fixture");
    let float_rust =
        std::fs::read_to_string(&float_generated).expect("read generated printf float rust");
    assert!(float_rust.contains("println!(\"{:.6} {:.2} {:.0}\","));
    assert!(float_rust.contains("print!(\"tail {:.3}\","));
    assert!(!float_rust.contains("fn printf("));
    assert!(!float_rust.contains("unsafe { printf("));

    let float_rejected_c = fixtures_dir().join("printf_float_rejected.c");
    let float_rejected_generated = tmp.join("printf_float_rejected.generated.rs");
    support::translate(&float_rejected_c, &float_rejected_generated)
        .expect("translate rejected printf float fixture");
    let float_rejected_rust = std::fs::read_to_string(&float_rejected_generated)
        .expect("read generated rejected printf float rust");
    assert!(float_rejected_rust.contains("fn printf("));
    assert!(float_rejected_rust.contains("unsafe { printf("));
    assert!(!float_rejected_rust.contains("println!(\"{:."));
}

#[test]
fn control_flow_conditions_drop_redundant_parens() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-redundant-parens");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("redundant_parens.c");
    let generated = tmp.join("redundant_parens.generated.rs");

    support::translate(&c_src, &generated).expect("translate redundant parens fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated redundant parens rust");

    for bare in ["if a == b {", "if a > b {", "if !(a < b) {"] {
        assert!(rust.contains(bare), "condition should render bare: {bare}");
    }
    for wrapped in ["if (a == b)", "if (a > b)", "if (!(a < b))"] {
        assert!(
            !rust.contains(wrapped),
            "condition should not be parenthesized: {wrapped}"
        );
    }
    // precedence-required parens across bitwise/shift must survive.
    assert!(rust.contains("(a & b) + (a << 1)"));
    assert!(rust.contains("return r + t + m;"));
}

#[test]
fn main_retval_boilerplate_is_collapsed() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-main-retval-fixup");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("add.c");
    let generated = tmp.join("add.generated.rs");

    support::translate(&c_src, &generated).expect("translate add fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated add rust");
    assert!(rust.contains("std::process::exit(0 as i32);"));
    assert!(!rust.contains("__retval"));
    assert!(!rust.contains("let _v14: i32 ="));
}

#[test]
fn unnecessary_mut_bindings_are_removed() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-remove-mut-fixup");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let add_c = fixtures_dir().join("add.c");
    let add_generated = tmp.join("add.generated.rs");
    support::translate(&add_c, &add_generated).expect("translate add fixture");
    let add_rust = std::fs::read_to_string(&add_generated).expect("read generated add rust");
    assert!(add_rust.contains("fn add(a: i32, b: i32) -> i32"));
    assert!(add_rust.contains("let c: i32 = a + b;"));
    assert!(!add_rust.contains("fn add(mut a: i32, mut b: i32)"));
    assert!(!add_rust.contains("let mut c: i32 ="));

    let compound_c = fixtures_dir().join("compound_assignments.c");
    let compound_generated = tmp.join("compound_assignments.generated.rs");
    support::translate(&compound_c, &compound_generated).expect("translate compound fixture");
    let compound_rust =
        std::fs::read_to_string(&compound_generated).expect("read generated compound rust");
    assert!(compound_rust.contains("let mut a: i32 = 0;"));

    let pointers_c = fixtures_dir().join("pointers.c");
    let pointers_generated = tmp.join("pointers.generated.rs");
    support::translate(&pointers_c, &pointers_generated).expect("translate pointers fixture");
    let pointers_rust =
        std::fs::read_to_string(&pointers_generated).expect("read generated pointers rust");
    assert!(pointers_rust.contains("let mut local: i32 = 0;"));
    assert!(pointers_rust.contains("std::ptr::addr_of_mut!(local)"));
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
