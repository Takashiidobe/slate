mod support;

use std::path::{Path, PathBuf};
use std::process::Command;

fn cfg_fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.cfg")
}

fn translate_directives(name: &str) -> String {
    let src = cfg_fixtures_dir().join(name);
    let out = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-directives")
        .arg(&src)
        .output()
        .expect("run slate translate-directives");
    assert!(
        out.status.success(),
        "translate-directives failed for {name}:\n{}",
        String::from_utf8_lossy(&out.stderr)
    );
    String::from_utf8(out.stdout).expect("generated Rust is utf8")
}

fn translate_directives_err(name: &str) -> String {
    let src = cfg_fixtures_dir().join(name);
    let out = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-directives")
        .arg(&src)
        .output()
        .expect("run slate translate-directives");
    assert!(
        !out.status.success(),
        "translate-directives unexpectedly succeeded for {name}:\n{}",
        String::from_utf8_lossy(&out.stdout)
    );
    String::from_utf8(out.stderr).expect("diagnostics are utf8")
}

fn write_generated(name: &str, rust: &str) -> PathBuf {
    let out_dir =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("target/directive-translate-generated");
    std::fs::create_dir_all(&out_dir).expect("create cfg translate output dir");
    let out = out_dir.join(name).with_extension("rs");
    std::fs::write(&out, rust).expect("write generated cfg Rust");
    out
}

fn assert_tail_value(rust: &str, value: &str) {
    assert!(rust.contains(&format!("\n    {value}\n}}")));
}

#[test]
fn translates_os_macro_variants_to_cfg_items() {
    let rust = translate_directives("os_targets.c");

    assert!(rust.contains("#[cfg(windows)]\nfn os_code() -> i32"));
    assert!(rust.contains("#[cfg(target_os = \"android\")]\nfn os_code() -> i32"));
    assert!(rust.contains("#[cfg(target_os = \"linux\")]\nfn os_code() -> i32"));
    assert!(rust.contains("#[cfg(target_vendor = \"apple\")]\nfn os_code() -> i32"));
    assert!(rust.contains("#[cfg(target_os = \"freebsd\")]\nfn os_code() -> i32"));
    assert!(rust.contains(
        "#[cfg(not(any(windows, target_os = \"android\", target_os = \"linux\", target_vendor = \"apple\", target_os = \"freebsd\")))]\nfn os_code() -> i32"
    ));
    assert_tail_value(&rust, "10");
    assert_tail_value(&rust, "25");
    assert_tail_value(&rust, "20");
    assert_tail_value(&rust, "30");
    assert_tail_value(&rust, "35");
    assert_tail_value(&rust, "40");
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn translates_win64_macro_to_composed_cfg_item() {
    let rust = translate_directives("win64_target.c");

    assert!(
        rust.contains(
            "#[cfg(all(windows, target_pointer_width = \"64\"))]\nfn win64_code() -> i32"
        )
    );
    assert!(rust.contains(
        "#[cfg(not(all(windows, target_pointer_width = \"64\")))]\nfn win64_code() -> i32"
    ));
    assert_tail_value(&rust, "64");
    assert_tail_value(&rust, "0");
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn translates_unix_macro_to_cfg_item() {
    let rust = translate_directives("unix_target.c");

    assert!(rust.contains("#[cfg(unix)]\nfn unix_code() -> i32"));
    assert!(rust.contains("#[cfg(not(unix))]\nfn unix_code() -> i32"));
    assert_tail_value(&rust, "1");
    assert_tail_value(&rust, "0");
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn translates_arch_macro_variants_to_cfg_items() {
    let rust = translate_directives("arch_targets.c");

    assert!(rust.contains("#[cfg(target_arch = \"x86_64\")]\nfn arch_code() -> i32"));
    assert!(rust.contains("#[cfg(target_arch = \"x86\")]\nfn arch_code() -> i32"));
    assert!(rust.contains("#[cfg(target_arch = \"aarch64\")]\nfn arch_code() -> i32"));
    assert!(rust.contains("#[cfg(target_arch = \"arm\")]\nfn arch_code() -> i32"));
    assert!(rust.contains("#[cfg(target_arch = \"powerpc64\")]\nfn arch_code() -> i32"));
    assert!(rust.contains("#[cfg(target_arch = \"powerpc\")]\nfn arch_code() -> i32"));
    assert!(rust.contains("#[cfg(target_arch = \"wasm64\")]\nfn arch_code() -> i32"));
    assert!(rust.contains("#[cfg(target_arch = \"wasm32\")]\nfn arch_code() -> i32"));
    assert!(rust.contains("#[cfg(target_arch = \"riscv64\")]\nfn arch_code() -> i32"));
    assert!(rust.contains("#[cfg(target_arch = \"riscv32\")]\nfn arch_code() -> i32"));
    assert!(rust.contains(
        "#[cfg(not(any(target_arch = \"x86_64\", target_arch = \"x86\", target_arch = \"aarch64\", target_arch = \"arm\", target_arch = \"powerpc64\", target_arch = \"powerpc\", target_arch = \"wasm64\", target_arch = \"wasm32\", target_arch = \"riscv64\", target_arch = \"riscv32\")))]\nfn arch_code() -> i32"
    ));
    assert_tail_value(&rust, "64");
    assert_tail_value(&rust, "86");
    assert_tail_value(&rust, "128");
    assert_tail_value(&rust, "32");
    assert_tail_value(&rust, "640");
    assert_tail_value(&rust, "320");
    assert_tail_value(&rust, "6400");
    assert_tail_value(&rust, "3200");
    assert_tail_value(&rust, "645");
    assert_tail_value(&rust, "325");
    assert_tail_value(&rust, "0");
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn translates_pointer_width_macro_variants_to_cfg_items() {
    let rust = translate_directives("pointer_width_targets.c");

    assert!(rust.contains("#[cfg(target_pointer_width = \"64\")]\nfn pointer_width_code() -> i32"));
    assert!(rust.contains("#[cfg(target_pointer_width = \"32\")]\nfn pointer_width_code() -> i32"));
    assert!(rust.contains(
        "#[cfg(not(any(target_pointer_width = \"64\", target_pointer_width = \"32\")))]\nfn pointer_width_code() -> i32"
    ));
    assert_tail_value(&rust, "64");
    assert_tail_value(&rust, "32");
    assert_tail_value(&rust, "0");
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn translates_arm_endian_macro_variants_to_cfg_items() {
    let rust = translate_directives("arm_endian_targets.c");

    assert!(rust.contains(
        "#[cfg(any(all(target_arch = \"arm\", target_endian = \"big\"), all(target_arch = \"aarch64\", target_endian = \"big\")))]\nfn arm_endian_code() -> i32"
    ));
    assert!(rust.contains(
        "#[cfg(any(all(target_arch = \"arm\", target_endian = \"little\"), all(target_arch = \"aarch64\", target_endian = \"little\")))]\nfn arm_endian_code() -> i32"
    ));
    assert!(rust.contains(
        "#[cfg(not(any(any(all(target_arch = \"arm\", target_endian = \"big\"), all(target_arch = \"aarch64\", target_endian = \"big\")), any(all(target_arch = \"arm\", target_endian = \"little\"), all(target_arch = \"aarch64\", target_endian = \"little\")))))]\nfn arm_endian_code() -> i32"
    ));
    assert_tail_value(&rust, "100");
    assert_tail_value(&rust, "200");
    assert_tail_value(&rust, "0");
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn translates_ndebug_variants_to_debug_assertion_cfg_items() {
    let rust = translate_directives("ndebug.c");

    assert!(rust.contains("#[cfg(not(debug_assertions))]\nfn debug_code() -> i32"));
    assert!(rust.contains("#[cfg(debug_assertions)]\nfn debug_code() -> i32"));
    assert_tail_value(&rust, "0");
    assert_tail_value(&rust, "1");
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn translates_single_custom_macro_to_feature_cfg_items() {
    let rust = translate_directives("feature_single.c");

    assert!(rust.contains("#[cfg(feature = \"my_feature\")]\nfn feature_code() -> i32"));
    assert!(rust.contains("#[cfg(not(feature = \"my_feature\"))]\nfn feature_code() -> i32"));
    assert_tail_value(&rust, "10");
    assert_tail_value(&rust, "20");
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn translates_independent_custom_macro_chains_without_cross_product() {
    let rust = translate_directives("feature_multiple.c");

    assert!(rust.contains("#[cfg(feature = \"first_feature\")]\nfn first_code() -> i32"));
    assert!(rust.contains("#[cfg(not(feature = \"first_feature\"))]\nfn first_code() -> i32"));
    assert!(rust.contains("#[cfg(feature = \"second_feature\")]\nfn second_code() -> i32"));
    assert!(rust.contains("#[cfg(not(feature = \"second_feature\"))]\nfn second_code() -> i32"));
    assert!(!rust.contains("all(feature = \"first_feature\", feature = \"second_feature\")"));
    assert_eq!(rust.matches("fn first_code()").count(), 2);
    assert_eq!(rust.matches("fn second_code()").count(), 2);
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn translates_nested_custom_macro_chains_with_parent_cfg() {
    let rust = translate_directives("feature_nested.c");

    assert!(rust.contains(
        "#[cfg(all(feature = \"outer_feature\", feature = \"inner_feature\"))]\nfn nested_code() -> i32"
    ));
    assert!(rust.contains(
        "#[cfg(all(feature = \"outer_feature\", not(feature = \"inner_feature\")))]\nfn nested_code() -> i32"
    ));
    assert!(rust.contains("#[cfg(not(feature = \"outer_feature\"))]\nfn nested_code() -> i32"));
    assert_eq!(rust.matches("fn nested_code()").count(), 3);
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn directive_translated_fixtures_compile_for_current_host() {
    let work_dir = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/directive-translate-compile");
    for name in [
        "os_targets.c",
        "win64_target.c",
        "unix_target.c",
        "arch_targets.c",
        "pointer_width_targets.c",
        "arm_endian_targets.c",
        "ndebug.c",
        "feature_single.c",
        "feature_multiple.c",
        "feature_nested.c",
    ] {
        let rust = translate_directives(name);
        let rs = write_generated(name, &rust);
        let package = format!("cfg_{}", name.trim_end_matches(".c"));
        support::compile_rs_cargo(&rs, &work_dir, &package)
            .unwrap_or_else(|err| panic!("generated cfg Rust did not compile for {name}:\n{err}"));
    }
}

#[test]
fn refuses_conditional_inside_a_function_body() {
    let err = translate_directives_err("reject/fragment_stmt.c");
    assert!(
        err.contains("inside a function or record body"),
        "expected fragment-cut diagnostic, got:\n{err}"
    );
}

#[test]
fn refuses_predicate_without_a_known_cfg_mapping() {
    let err = translate_directives_err("reject/system_macro_feature.c");
    assert!(
        err.contains("does not map to a known Rust cfg"),
        "expected unmapped-predicate diagnostic, got:\n{err}"
    );
}

#[test]
fn refuses_cfg_plans_above_the_variant_cap() {
    let err = translate_directives_err("reject/too_many_feature_chains.c");
    assert!(
        err.contains("configuration variant cap"),
        "expected variant-cap diagnostic, got:\n{err}"
    );
}

#[test]
fn passes_through_sources_without_conditional_regions() {
    let src = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/add.c");
    let out = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-directives")
        .arg(&src)
        .output()
        .expect("run slate translate-directives");
    assert!(
        out.status.success(),
        "translate-directives failed on a plain source:\n{}",
        String::from_utf8_lossy(&out.stderr)
    );
    let rust = String::from_utf8(out.stdout).expect("generated Rust is utf8");
    assert!(rust.contains("fn add("));
    assert!(!rust.contains("#[cfg("));
}
