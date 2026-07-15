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
fn pthread_opaque_types_use_libc_paths() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-pthread-types");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("mt-atomics.c");
    let generated = tmp.join("mt-atomics.generated.rs");

    support::translate(&c_src, &generated).expect("translate mt-atomics fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated mt-atomics rust");
    assert!(rust.contains("*mut libc::pthread_attr_t"));
    assert!(!rust.contains("*mut pthread_attr_t"));
}

#[test]
fn counted_varargs_loop_uses_range_for() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-varargs-loop");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("varargs.c");
    let generated = tmp.join("varargs.generated.rs");

    support::translate(&c_src, &generated).expect("translate varargs fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated varargs rust");
    assert!(rust.contains("mut ap: ..."));
    assert!(rust.contains("for _ in 0..n {"));
    assert!(rust.contains("let mut total: i32 = 0;"));
    assert!(rust.contains("total += unsafe { ap.next_arg::<i32>() };"));
    assert!(rust.contains("let first: i32 = unsafe { ap.next_arg::<i32>() };"));
    assert!(rust.contains("let second: i32 = unsafe { ap.next_arg::<i32>() };"));
    assert!(rust.contains("println!(\"{}\", unsafe { sum(4, 10, 20, 30, 40) });"));
    assert!(rust.contains("println!(\"{}\", unsafe { pick_second(5, 7, 9) });"));
    assert!(!rust.contains("total = 0;"));
    assert!(!rust.contains("let mut first: i32 = 0;"));
    assert!(!rust.contains("let mut second: i32 = 0;"));
    assert!(!rust.contains("first = unsafe { ap.next_arg::<i32>() };"));
    assert!(!rust.contains("second = unsafe { ap.next_arg::<i32>() };"));
    assert!(!rust.contains("let _v5: i32 = unsafe { ap.next_arg::<i32>() };"));
    assert!(!rust.contains("let _v0: i32 = unsafe { ap.next_arg::<i32>() };"));
    assert!(!rust.contains("let _v1: i32 = 4;"));
    assert!(!rust.contains("let _v6: i32 = unsafe { sum("));
    assert!(!rust.contains("__slate_va_args.clone()"));
    assert!(!rust.contains("let mut ap: core::ffi::VaList<'_>;"));
    assert!(!rust.contains("if !(i < n)"));
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
fn safe_struct_field_reads_do_not_use_unsafe() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-field-unsafe");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let structs_c = fixtures_dir().join("structs.c");
    let structs_generated = tmp.join("structs.generated.rs");
    support::translate(&structs_c, &structs_generated).expect("translate structs fixture");
    let structs_rust =
        std::fs::read_to_string(&structs_generated).expect("read generated structs rust");
    assert!(structs_rust.contains("let _v2: i32 = p.left;"));
    assert!(structs_rust.contains("let _v3: i32 = p.right;"));
    assert!(structs_rust.contains("return p.left;"));
    assert!(!structs_rust.contains("unsafe { p.left }"));
    assert!(!structs_rust.contains("unsafe { p.right }"));

    let mixed_c = fixtures_dir().join("non_int_fields.c");
    let mixed_generated = tmp.join("non_int_fields.generated.rs");
    support::translate(&mixed_c, &mixed_generated).expect("translate non_int_fields fixture");
    let mixed_rust =
        std::fs::read_to_string(&mixed_generated).expect("read generated non_int_fields rust");
    assert!(mixed_rust.contains("let _v4: i8 = m.tag;"));
    assert!(mixed_rust.contains("let _v6: u8 = m.code;"));
    assert!(mixed_rust.contains("let _v8: f32 = m.ratio;"));
    assert!(mixed_rust.contains("let _v10: f64 = m.total;"));
    assert!(mixed_rust.contains("return unsafe { s.total };"));
    assert!(mixed_rust.contains("let _v1: i8 = unsafe { s.tag };"));

    let unions_c = fixtures_dir().join("unions.c");
    let unions_generated = tmp.join("unions.generated.rs");
    support::translate(&unions_c, &unions_generated).expect("translate unions fixture");
    let unions_rust =
        std::fs::read_to_string(&unions_generated).expect("read generated unions rust");
    assert!(unions_rust.contains("return unsafe { p.left };"));

    let globals_c = fixtures_dir().join("global_vars.c");
    let globals_generated = tmp.join("global_vars.generated.rs");
    support::translate(&globals_c, &globals_generated).expect("translate global_vars fixture");
    let globals_rust =
        std::fs::read_to_string(&globals_generated).expect("read generated global_vars rust");
    assert!(globals_rust.contains("let _v13: i32 = unsafe { pair.right };"));
    assert!(globals_rust.contains("let _v15: i32 = unsafe { pair.left };"));
}

#[test]
fn final_return_temps_are_collapsed() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-final-return-temps");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    for fixture in ["arrays", "pointers", "volatile", "function_pointers"] {
        let c_src = fixtures_dir().join(format!("{fixture}.c"));
        let generated = tmp.join(format!("{fixture}.generated.rs"));
        support::translate(&c_src, &generated)
            .unwrap_or_else(|err| panic!("translate {fixture} fixture: {err}"));
        let rust = std::fs::read_to_string(&generated)
            .unwrap_or_else(|err| panic!("read generated {fixture} rust: {err}"));

        assert!(
            !rust.contains("return _v"),
            "{fixture} kept a synthetic return temp"
        );
    }

    let arrays = std::fs::read_to_string(tmp.join("arrays.generated.rs"))
        .expect("read generated arrays rust");
    assert!(arrays.contains("return values[2];"));
    assert!(arrays.contains("return values[((index as i64) as usize)];"));
    assert!(
        !arrays.contains("(0 as usize)")
            && !arrays.contains("(1 as usize)")
            && !arrays.contains("(2 as usize)"),
        "arrays kept literal index casts"
    );

    let array_types = fixtures_dir().join("array_types.c");
    let array_types_generated = tmp.join("array_types.generated.rs");
    support::translate(&array_types, &array_types_generated)
        .expect("translate array_types fixture");
    let array_types_rust =
        std::fs::read_to_string(&array_types_generated).expect("read generated array_types rust");
    assert!(
        !array_types_rust.contains("(0 as usize)")
            && !array_types_rust.contains("(1 as usize)")
            && !array_types_rust.contains("(2 as usize)"),
        "array_types kept literal index casts"
    );
    assert!(array_types_rust.contains("return values[((index as i64) as usize)];"));

    let pointers = std::fs::read_to_string(tmp.join("pointers.generated.rs"))
        .expect("read generated pointers rust");
    assert!(pointers.contains("return unsafe { *slot };"));

    let volatile = std::fs::read_to_string(tmp.join("volatile.generated.rs"))
        .expect("read generated volatile rust");
    assert!(volatile.contains("std::ptr::read_volatile"));
}

#[test]
fn pointer_arithmetic_uses_clearer_safe_offset_forms() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-pointer-arithmetic");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let pointers_c = fixtures_dir().join("pointers.c");
    let pointers_generated = tmp.join("pointers.generated.rs");
    support::translate(&pointers_c, &pointers_generated).expect("translate pointers fixture");
    let pointers =
        std::fs::read_to_string(&pointers_generated).expect("read generated pointers rust");
    assert!(pointers.contains("return unsafe { *_v8.offset(_v9 as isize) };"));
    assert!(!pointers.contains("unsafe { *unsafe {"));
    assert!(!pointers.contains(".add(_v9"));

    let bcopy_c = fixtures_dir().join("mem_bcopy_overlap.c");
    let bcopy_generated = tmp.join("mem_bcopy_overlap.generated.rs");
    support::translate(&bcopy_c, &bcopy_generated).expect("translate mem_bcopy_overlap fixture");
    let bcopy =
        std::fs::read_to_string(&bcopy_generated).expect("read generated mem_bcopy_overlap rust");
    assert!(bcopy.contains("buf.as_mut_ptr().add(2)"));
    assert!(!bcopy.contains("buf.as_mut_ptr().offset(2"));
    assert!(!bcopy.contains("unsafe { buf.as_mut_ptr().add(2) }"));
}

#[test]
fn address_of_array_elements_use_safe_indexes() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-array-element-pointer");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("address_of_array_element.c");
    let generated = tmp.join("address_of_array_element.generated.rs");
    support::translate(&c_src, &generated).expect("translate address_of_array_element fixture");
    let rust =
        std::fs::read_to_string(&generated).expect("read generated address_of_array_element rust");

    assert!(rust.contains("values[1] + values[3]"));
    assert!(rust.contains("3 - 1"));
    assert!(!rust.contains("__retval"));
    assert!(!rust.contains("std::process::exit"));
    assert!(!rust.contains("unsafe { *p }"));
    assert!(!rust.contains("unsafe { *q }"));
    assert!(!rust.contains(".offset_from("));
}

#[test]
fn redundant_singleton_scopes_are_unwrapped() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-singleton-scopes");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    for fixture in ["conditionals", "function_prototypes"] {
        let c_src = fixtures_dir().join(format!("{fixture}.c"));
        let generated = tmp.join(format!("{fixture}.generated.rs"));
        support::translate(&c_src, &generated)
            .unwrap_or_else(|err| panic!("translate {fixture} fixture: {err}"));
        let rust = std::fs::read_to_string(&generated)
            .unwrap_or_else(|err| panic!("read generated {fixture} rust: {err}"));

        assert!(
            !rust.contains("\n    {\n        if "),
            "{fixture} kept a redundant top-level if scope"
        );
        assert!(
            !rust.contains("\n        {\n            if "),
            "{fixture} kept a redundant nested if scope"
        );
    }

    let conditionals = std::fs::read_to_string(tmp.join("conditionals.generated.rs"))
        .expect("read generated conditionals rust");
    assert!(conditionals.contains("if n < 0 {\n        return -1;"));
    assert!(conditionals.contains("} else {\n        if n == 0 {"));
    assert!(conditionals.contains("if n < 10 {\n        r = 10;"));

    let prototypes = std::fs::read_to_string(tmp.join("function_prototypes.generated.rs"))
        .expect("read generated function_prototypes rust");
    assert!(prototypes.contains("if n == 0 {\n        return 1;"));
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
    assert!(rust.contains("println!(\"{}\", a);"));
    assert!(!rust.contains("println!(\"{}\", _v"));
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

    let alternate_c = fixtures_dir().join("printf_alternate_integer.c");
    let alternate_generated = tmp.join("printf_alternate_integer.generated.rs");
    support::translate(&alternate_c, &alternate_generated)
        .expect("translate printf alternate integer fixture");
    let alternate_rust = std::fs::read_to_string(&alternate_generated)
        .expect("read generated printf alternate integer rust");
    assert!(alternate_rust.contains("println!(\"{} {} {}\","));
    assert!(alternate_rust.contains("println!(\"{}|{}|{}\","));
    assert!(alternate_rust.contains("println!(\"{} {} {} {}\","));
    assert!(alternate_rust.contains("format!(\"{:#x}\", __slate_printf_arg)"));
    assert!(alternate_rust.contains("format!(\"0X{:X}\", __slate_printf_arg)"));
    assert!(alternate_rust.contains("format!(\"0{:o}\", __slate_printf_arg)"));
    assert!(alternate_rust.contains("format!(\"{:#08x}\", __slate_printf_arg)"));
    assert!(!alternate_rust.contains("fn printf("));
    assert!(!alternate_rust.contains("unsafe { printf("));

    let string_char_c = fixtures_dir().join("printf_string_char.c");
    let string_char_generated = tmp.join("printf_string_char.generated.rs");
    support::translate(&string_char_c, &string_char_generated)
        .expect("translate printf string/char fixture");
    let string_char_rust = std::fs::read_to_string(&string_char_generated)
        .expect("read generated printf string/char rust");
    assert!(string_char_rust.contains("println!(\"{} {} {} {}\", \"tag\", \"A\", \"\\n\", 7);"));
    assert!(!string_char_rust.contains("println!(\"{} {} {} {}\", \"tag\", \"A\", \"\\n\", _v3);"));
    assert!(string_char_rust.contains("print!(\"literal={}\", \"tail\");"));
    assert!(!string_char_rust.contains("fn printf("));
    assert!(!string_char_rust.contains("unsafe { printf("));

    let rejected_c = fixtures_dir().join("printf_string_char_rejected.c");
    let rejected_generated = tmp.join("printf_string_char_rejected.generated.rs");
    support::translate(&rejected_c, &rejected_generated)
        .expect("translate rejected printf string/char fixture");
    let rejected_rust = std::fs::read_to_string(&rejected_generated)
        .expect("read generated rejected printf string/char rust");
    assert!(rejected_rust.contains("let buf: &str = \"hey\";"));
    assert!(rejected_rust.contains("println!(\"{}\", \"hey\");"));
    assert!(!rejected_rust.contains("fn printf("));
    assert!(!rejected_rust.contains("unsafe { printf("));

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

    let pointer_c = fixtures_dir().join("printf_pointer.c");
    let pointer_generated = tmp.join("printf_pointer.generated.rs");
    support::translate(&pointer_c, &pointer_generated).expect("translate printf pointer fixture");
    let pointer_rust =
        std::fs::read_to_string(&pointer_generated).expect("read generated printf pointer rust");
    assert!(pointer_rust.contains("println!(\"{:p}\","));
    assert!(pointer_rust.contains("print!(\"addr={:p}\","));
    assert!(!pointer_rust.contains("fn printf("));
    assert!(!pointer_rust.contains("unsafe { printf("));

    let pointer_rejected_c = fixtures_dir().join("printf_pointer_rejected.c");
    let pointer_rejected_generated = tmp.join("printf_pointer_rejected.generated.rs");
    support::translate(&pointer_rejected_c, &pointer_rejected_generated)
        .expect("translate rejected printf pointer fixture");
    let pointer_rejected_rust = std::fs::read_to_string(&pointer_rejected_generated)
        .expect("read generated rejected printf pointer rust");
    assert!(pointer_rejected_rust.contains("fn printf("));
    assert!(pointer_rejected_rust.contains("unsafe { printf("));
    assert!(!pointer_rejected_rust.contains("println!(\"{:p}"));
}

#[test]
fn nul_terminated_char_pointer_literals_use_c_string_syntax() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-c-string-literal");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("c_string_literal.c");
    let generated = tmp.join("c_string_literal.generated.rs");

    support::translate(&c_src, &generated).expect("translate c string literal fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated c string literal rust");
    assert!(rust.contains("c\"write error\".as_ptr() as *mut i8"));
    assert!(!rust.contains("b\"write error\\0\".as_ptr()"));
}

#[test]
fn literal_fopen_fputs_fclose_owner_uses_open_options() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-stdio-file-write");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("stdio_file_write.c");
    let generated = tmp.join("stdio_file_write.generated.rs");

    support::translate(&c_src, &generated).expect("translate stdio file write fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated stdio file write rust");

    let remove_index = rust
        .find("unsafe { remove((c\"slate_stdio_file_write.tmp\"")
        .expect("generated rust should keep remove call");
    let open_index = rust
        .find("let mut f = std::fs::OpenOptions::new().write(true).create(true).truncate(true).open(\"slate_stdio_file_write.tmp\").unwrap_or_else")
        .expect("generated rust should open literal write-only owner with OpenOptions");
    assert!(
        remove_index < open_index,
        "remove call must remain before the rewritten open"
    );
    assert!(rust.contains("std::io::Write::write_all(&mut f, b\"owned\\n\").unwrap();"));
    assert!(!rust.contains("unsafe { fputs((c\"owned\\n\""));
    assert!(rust.contains("unsafe { fgets("));
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
    assert!(rust.contains("fn main() {"));
    assert!(!rust.contains("std::process::exit(0 as i32);"));
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
    assert!(compound_rust.contains("let mut a: i32 = 20;"));
    assert!(!compound_rust.contains("let mut a: i32 = 0;"));

    let pointers_c = fixtures_dir().join("pointers.c");
    let pointers_generated = tmp.join("pointers.generated.rs");
    support::translate(&pointers_c, &pointers_generated).expect("translate pointers fixture");
    let pointers_rust =
        std::fs::read_to_string(&pointers_generated).expect("read generated pointers rust");
    assert!(pointers_rust.contains("let mut local: i32 = value;"));
    assert!(!pointers_rust.contains("let mut local: i32 = 0;"));
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
    assert!(fp_rust.contains("return lhs + rhs;"));
    assert!(fp_rust.contains("return lhs * rhs;"));
    assert!(!fp_rust.contains("let mut __retval: i32 = lhs + rhs;"));
    assert!(!fp_rust.contains("let mut __retval: i32 = lhs * rhs;"));

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
fn function_pointer_presence_checks_use_option_methods() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-fnptr-presence");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("function_pointer_presence.c");
    let generated = tmp.join("function_pointer_presence.generated.rs");
    support::translate(&c_src, &generated).expect("translate function pointer presence fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated function pointer rust");

    assert!(rust.contains(".is_some()"));
    assert!(rust.contains("if op.is_some()"));
    assert!(rust.contains(".is_none()"));
    assert!(!rust.contains("!= None"));
    assert!(!rust.contains("== None"));
    assert!(!rust.contains("std::ptr::null_mut()"));
    assert!(rust.contains("return op.unwrap()(value);"));
    assert!(rust.contains("return value;"));
    assert!(!rust.contains("let _v0: Option<fn(i32) -> i32> = op;"));
    assert!(!rust.contains("let _v1: Option<fn(i32) -> i32> = op;"));
    assert!(!rust.contains("let _v2: i32 = value;"));
    assert!(!rust.contains("let _v3: i32 = _v1.unwrap()(_v2);"));
    assert!(!rust.contains("__retval = _v1.unwrap()(_v2);"));
    assert!(!rust.contains("__retval = value;"));
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
            &[
                "let mut local: i32 = value;",
                "*slot = _v",
                "values[2] = 12;",
            ][..],
        ),
        ("struct_with_array", &["b.data[0] = 10;", "b.len = 3;"][..]),
        (
            "global_vars",
            &["counter = _v", "numbers[2] =", "pair.right = _v"][..],
        ),
        (
            "bitfield_ops",
            &[
                "s.a = 5 << 29 >> 29;",
                "s.b = -3 << 27 >> 27;",
                "w.x = 1099511627775 << 24 >> 24;",
            ][..],
        ),
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

#[test]
fn preserves_documentation_comments_on_enums() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-comments");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("comments.c");
    let generated = tmp.join("comments.generated.rs");
    support::translate(&c_src, &generated).expect("translate comments fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated comments rust");

    assert!(rust.contains("/// selects an operating mode\n#[repr(C)]"));
    assert!(rust.contains("#[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]\nenum Mode"));
    assert!(rust.contains("    /// disable processing\n    MODE_OFF = 0,"));
    assert!(rust.contains("    /// enable processing\n    MODE_ON = 1,"));
    assert!(rust.contains("/// stores a selected mode\n#[repr(C)]"));
    assert!(rust.contains("    /// current mode value\n    mode: Mode,"));
}

#[test]
fn string_lift_recovers_safe_local_string_buffers() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-string-lift");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("string_lift.c");
    let generated = tmp.join("string_lift.generated.rs");
    support::translate(&c_src, &generated).expect("translate string_lift fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated string_lift rust");

    assert!(rust.contains("let greeting: &str = \"h\\u{e9}\";"));
    assert!(rust.contains("let mut mutate: [i8; 4] = [0; 4];"));
    assert!(rust.contains("mutate.as_mut_ptr()"));
}

#[test]
fn string_libc_calls_use_lifted_string_operations() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-string-libc");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("string_libc_fixup.c");
    let generated = tmp.join("string_libc_fixup.generated.rs");
    support::translate(&c_src, &generated).expect("translate string_libc_fixup fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated string_libc rust");

    assert!(rust.contains("let alpha: &str = \"abc\";"));
    assert!(rust.contains("let bytes_a: &[u8] = b\"\\xff\\x01\";"));
    assert!(rust.contains(": usize = alpha.len();"));
    assert!(!rust.contains("alpha.len() as u64"));
    assert!(rust.contains("alpha == alpha"));
    assert!(rust.contains("alpha.cmp(beta) == std::cmp::Ordering::Less"));
    assert!(rust.contains(".split_at(std::cmp::min("));
    assert!(rust.contains("hay.find(char::from("));
    assert!(rust.contains("hay.rfind(char::from("));
    assert!(rust.contains("hay.find(sub)"));
    assert!(rust.contains("hay.find(|__slate_ch| set.contains(__slate_ch))"));
    assert!(
        rust.contains("hay.find(|__slate_ch| !prefix.contains(__slate_ch)).unwrap_or(hay.len())")
    );
    assert!(
        rust.contains("hay.find(|__slate_ch| reject.contains(__slate_ch)).unwrap_or(hay.len())")
    );
    assert!(rust.contains("let _v43 = Some(hay.len());"));
    assert!(rust.contains("let _v45: bool = _v43.is_some();"));
    assert!(rust.contains("let utf8: &str = \"h\\u{e9}\";"));
    assert!(rust.contains("utf8.as_bytes().iter().position("));
    assert!(!rust.contains("fn strlen("));
    assert!(!rust.contains("fn strcmp("));
    assert!(!rust.contains("fn strncmp("));
    assert!(!rust.contains("fn memcmp("));
    assert!(!rust.contains("fn strchr("));
    assert!(!rust.contains("fn strrchr("));
    assert!(!rust.contains("fn strstr("));
    assert!(!rust.contains("fn strpbrk("));
    assert!(!rust.contains("fn strspn("));
    assert!(!rust.contains("fn strcspn("));
    assert!(!rust.contains("unsafe { strlen("));
    assert!(!rust.contains("unsafe { strcmp("));
    assert!(!rust.contains("unsafe { strncmp("));
    assert!(!rust.contains("unsafe { memcmp("));
    assert!(!rust.contains("unsafe { strchr("));
    assert!(!rust.contains("unsafe { strrchr("));
    assert!(!rust.contains("unsafe { strstr("));
    assert!(!rust.contains("unsafe { strpbrk("));
    assert!(!rust.contains("unsafe { strspn("));
    assert!(!rust.contains("unsafe { strcspn("));
}

#[test]
fn memchr_calls_use_iter_position_when_source_is_iterable() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-memchr-helper");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("mem_memchr.c");
    let generated = tmp.join("mem_memchr.generated.rs");
    support::translate(&c_src, &generated).expect("translate mem_memchr fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated mem_memchr rust");

    assert!(rust.contains("buf.as_slice().iter().position("));
    assert!(rust.contains("(*__slate_byte as u8) == ((_v1 as i32) as u8)"));
    assert!(rust.contains("let _v3 = buf.as_slice().iter().position("));
    assert!(rust.contains("let _v6 = buf.as_slice().iter().position("));
    assert!(rust.contains("let word: &core::ffi::CStr = c\"abc\";"));
    assert!(rust.contains("let _v9 = Some(3);"));
    assert!(rust.contains("let _v11: i64 = _v3.unwrap() as i64;"));
    assert!(rust.contains("let _v14: bool = _v6.is_none();"));
    assert!(rust.contains("println!(\"{} {} {}\", _v11, _v14 as i32, _v9.unwrap() as i64);"));
    assert!(!rust.contains("let _v16: i64 = _v9.unwrap() as i64;"));
    assert!(!rust.contains("let mut hit"));
    assert!(!rust.contains("let mut miss"));
    assert!(!rust.contains("let mut nul"));
    assert!(!rust.contains("let mut word: [i8; 4]"));
    assert!(!rust.contains("word = [97, 98, 99, 0];"));
    assert!(!rust.contains("map_or(std::ptr::null_mut()"));
    assert!(!rust.contains(".add(__slate_index)"));
    assert!(!rust.contains(".offset_from("));
    assert!(!rust.contains("hit = _v3 as *mut u8"));
    assert!(!rust.contains("miss = _v6 as *mut u8"));
    assert!(!rust.contains("nul = _v9 as *mut i8"));
    assert!(!rust.contains("miss == std::ptr::null_mut()"));
    assert!(!rust.contains("fn __slate_memchr("));
    assert!(!rust.contains("from_raw_parts"));
    assert!(!rust.contains("while i < n"));
}

#[test]
fn internal_char_pointer_params_lift_to_str() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-string-param-lift");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("string_param_lift.c");
    let generated = tmp.join("string_param_lift.generated.rs");
    support::translate(&c_src, &generated).expect("translate string_param_lift fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated string_param_lift rust");

    assert!(rust.contains("fn parse_num(s: &str) -> i32"));
    assert!(rust.contains("fn forward_num(s: &str) -> i32"));
    assert!(rust.contains("fn text_len(s: &str) -> i32"));
    assert!(rust.contains("return __slate_runtime::parse_i32(s);"));
    assert!(rust.contains("return parse_num(s);"));
    assert!(rust.contains("let _v1: usize = _v0.len();"));
    assert!(rust.contains("forward_num(digits)"));
    assert!(rust.contains("text_len(word)"));
    assert!(!rust.contains("fn atoi("));
    assert!(!rust.contains("fn strlen("));
}

#[test]
fn qsort_bsearch_calls_use_slice_sort_and_search() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-qsort-bsearch");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("qsort_bsearch_fixup.c");
    let generated = tmp.join("qsort_bsearch_fixup.generated.rs");
    support::translate(&c_src, &generated).expect("translate qsort_bsearch_fixup fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated qsort_bsearch rust");

    assert!(
        rust.contains(
            "nums.as_mut_slice().sort_by(|__slate_a, __slate_b| __slate_a.cmp(__slate_b));"
        )
    );
    assert!(
        rust.contains("nums.as_slice().binary_search_by(|__slate_probe| __slate_probe.cmp(&key))")
    );
    assert!(rust.contains(
        "items.as_mut_slice().sort_by(|__slate_a, __slate_b| __slate_a.key.cmp(&__slate_b.key));"
    ));
    assert!(rust.contains(
        "items.as_slice().binary_search_by(|__slate_probe| __slate_probe.key.cmp(&needle.key))"
    ));
    assert!(!rust.contains("fn qsort("));
    assert!(!rust.contains("fn bsearch("));
    assert!(!rust.contains("unsafe { qsort("));
    assert!(!rust.contains("unsafe { bsearch("));
}

#[test]
fn numeric_parse_calls_use_runtime_parse_support() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-numeric-parse");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("numeric_parse_fixup.c");
    let generated = tmp.join("numeric_parse_fixup.generated.rs");
    support::translate(&c_src, &generated).expect("translate numeric_parse_fixup fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated numeric parse rust");

    assert!(rust.contains("#![allow("));
    assert!(rust.contains("mod __slate_runtime"));
    assert!(rust.contains(".parse::<i32>().unwrap_or(0)"));
    assert!(rust.contains("whole_long.parse::<i64>().unwrap_or(0)"));
    assert!(rust.contains("whole_unsigned.parse::<u64>().unwrap_or(0)"));
    assert!(!rust.contains("__slate_runtime::parse_i32(whole)"));
    assert!(!rust.contains("__slate_runtime::parse_i64(whole_long)"));
    assert!(!rust.contains("__slate_runtime::parse_u64(whole_unsigned)"));
    assert!(rust.contains("__slate_runtime::parse_i64(leading)"));
    assert!(rust.contains("__slate_runtime::parse_i64(large)"));
    assert!(rust.contains("__slate_runtime::parse_u64(empty)"));
    assert!(rust.contains("__slate_runtime::parse_f64(flt)"));
    assert!(rust.contains("unsafe { strtol("));
    assert!(!rust.contains("fn atoi("));
    assert!(!rust.contains("fn atol("));
    assert!(!rust.contains("fn strtoul("));
    assert!(!rust.contains("fn strtod("));
}

#[test]
fn ptr_len_pairs_use_slice_params_for_full_array_calls() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-ptr-len-slice");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("ptr_len_slice.c");
    let generated = tmp.join("ptr_len_slice.generated.rs");
    support::translate(&c_src, &generated).expect("translate ptr_len_slice fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated ptr_len_slice rust");

    assert!(rust.contains("fn sum(items: &[i32]) -> i32"));
    assert!(rust.contains("fn bump(mut items: &mut [i32]) -> ()"));
    assert!(rust.contains("let len: i32 = items.len() as i32;"));
    assert!(rust.contains("total += items[(i as usize)];"));
    assert!(rust.contains("items[(i as usize)] = items[(i as usize)] + 1;"));
    assert!(!rust.contains("let _v7: i32 = items[(i as usize)];"));
    assert!(!rust.contains("items[(i as usize)] = _v8;"));
    assert!(rust.contains("sum(values.as_slice())"));
    assert!(rust.contains("bump(values.as_mut_slice())"));
    assert!(!rust.contains("for item in items.iter()"));
    assert!(!rust.contains("for item in items.iter_mut()"));
    assert!(!rust.contains("__slate_item"));
    assert!(!rust.contains(".offset("));
    assert!(!rust.contains("items.as_mut_ptr()"));
    assert!(!rust.contains("sum(values.as_mut_ptr(), 4)"));
    assert!(!rust.contains("bump(values.as_mut_ptr(), 4)"));
}

#[test]
fn ptr_len_slice_loop_uses_materialized_item_name() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-ptr-len-slice-item");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("ptr_len_slice_item.c");
    let generated = tmp.join("ptr_len_slice_item.generated.rs");
    support::translate(&c_src, &generated).expect("translate ptr_len_slice_item fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated ptr_len_slice_item rust");

    assert!(rust.contains("fn sum_items(items: &[i32]) -> i32"));
    assert!(rust.contains("for item in items.iter()"));
    assert!(rust.contains("sum_items(values.as_slice())"));
    assert!(rust.contains("total += *item;"));
    assert!(!rust.contains("let item: i32 = items[(i as usize)];"));
    assert!(!rust.contains("__slate_item"));
}

#[test]
fn scalar_heap_owner_uses_box_drop() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-heap-box");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("heap_box_fixup.c");
    let generated = tmp.join("heap_box_fixup.generated.rs");
    support::translate(&c_src, &generated).expect("translate heap_box_fixup fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated heap_box_fixup rust");

    assert!(rust.contains("let mut p: Box<i32> = Box::<i32>::new(0);"));
    assert!(rust.contains("*p = 41;"));
    assert!(rust.contains("*p = *p + 1;"));
    assert!(!rust.contains("*p = _v"));
    assert!(!rust.contains("fn malloc("));
    assert!(!rust.contains("fn free("));
    assert!(!rust.contains("unsafe { malloc("));
    assert!(!rust.contains("unsafe { free("));
}

#[test]
fn heap_malloc_buffer_uses_vec_drop() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-heap-vec-malloc");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("heap_vec_malloc.c");
    let generated = tmp.join("heap_vec_malloc.generated.rs");
    support::translate(&c_src, &generated).expect("translate heap_vec_malloc fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated heap_vec_malloc rust");

    assert!(rust.contains("let mut p: Vec<i32> = vec![0; 3usize];"));
    assert!(rust.contains("p[0] = 1;"));
    assert!(rust.contains("p[1] = 2;"));
    assert!(rust.contains("p[2] = 3;"));
    assert!(!rust.contains("fn malloc("));
    assert!(!rust.contains("fn free("));
    assert!(!rust.contains(".add("));
}

#[test]
fn heap_calloc_buffer_uses_zeroed_vec_drop() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-heap-vec-calloc");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("heap_vec_calloc.c");
    let generated = tmp.join("heap_vec_calloc.generated.rs");
    support::translate(&c_src, &generated).expect("translate heap_vec_calloc fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated heap_vec_calloc rust");

    assert!(rust.contains("let p: Vec<i32> = vec![0; 4usize];"));
    assert!(rust.contains("println!(\"{}\", p[0] + p[3]);"));
    assert!(!rust.contains("fn calloc("));
    assert!(!rust.contains("fn free("));
    assert!(!rust.contains(".add("));
}

#[test]
fn heap_realloc_growth_uses_vec_resize() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-heap-vec-realloc");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("heap_vec_realloc.c");
    let generated = tmp.join("heap_vec_realloc.generated.rs");
    support::translate(&c_src, &generated).expect("translate heap_vec_realloc fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated heap_vec_realloc rust");

    assert!(rust.contains("let mut p: Vec<i32> = vec![0; 2usize];"));
    assert!(rust.contains("p.resize(4usize, 0);"));
    assert!(rust.contains("p[2] = 3;"));
    assert!(rust.contains("p[3] = 4;"));
    assert!(!rust.contains("fn realloc("));
    assert!(!rust.contains("unsafe { realloc("));
    assert!(!rust.contains(".add("));
}

#[test]
fn string_copy_calls_use_lifted_string_operations() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-string-copy");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("string_copy_fixup.c");
    let generated = tmp.join("string_copy_fixup.generated.rs");
    support::translate(&c_src, &generated).expect("translate string_copy_fixup fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated string_copy rust");

    assert!(rust.contains("let mut copy: String = \"\".to_owned();"));
    assert!(rust.contains("let mut append: String = \"foo\".to_owned();"));
    assert!(rust.contains("copy = \"abc\".to_owned();"));
    assert!(rust.contains("append.push_str(\"bar\");"));
    assert!(rust.contains("trunc_copy = \"abc\".to_owned();"));
    assert!(rust.contains("trunc_append.push_str(\"suf\");"));
    assert!(!rust.contains("let _v7: usize = trunc_append.len();"));
    assert!(rust.contains(
        "println!(\"{} {} {} {} {}\", copy, append, trunc_copy, trunc_append, trunc_append.len());"
    ));
    for name in ["strcpy", "strncpy", "strcat", "strncat"] {
        assert!(!rust.contains(&format!("fn {name}(")));
        assert!(!rust.contains(&format!("unsafe {{ {name}(")));
    }
}
