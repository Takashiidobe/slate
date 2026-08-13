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

fn translate(name: &str) -> String {
    translate_with_clang_args(name, None)
}

fn translate_with_clang_args(name: &str, clang_args: Option<&str>) -> String {
    let src = cfg_fixtures_dir().join(name);
    let mut command = Command::new(env!("CARGO_BIN_EXE_slate"));
    command.arg("translate").arg(&src);
    match clang_args {
        Some(args) => {
            command.env("SLATE_CLANG_ARGS", args);
        }
        None => {
            command.env_remove("SLATE_CLANG_ARGS");
        }
    }
    let out = command.output().expect("run slate translate");
    assert!(
        out.status.success(),
        "translate failed for {name}:\n{}",
        String::from_utf8_lossy(&out.stderr)
    );
    String::from_utf8(out.stdout).expect("generated Rust is utf8")
}

fn translate_err_with_clang_args(name: &str, clang_args: Option<&str>) -> String {
    let src = cfg_fixtures_dir().join(name);
    let mut command = Command::new(env!("CARGO_BIN_EXE_slate"));
    command.arg("translate").arg(&src);
    match clang_args {
        Some(args) => {
            command.env("SLATE_CLANG_ARGS", args);
        }
        None => {
            command.env_remove("SLATE_CLANG_ARGS");
        }
    }
    let out = command.output().expect("run slate translate");
    assert!(
        !out.status.success(),
        "translate unexpectedly succeeded for {name}:\n{}",
        String::from_utf8_lossy(&out.stdout)
    );
    String::from_utf8(out.stderr).expect("diagnostics are utf8")
}

fn compile_with_cfgs(name: &str, rust: &str, cfgs: &[&str]) -> std::process::Output {
    let source = write_generated(name, rust);
    let binary = source.with_extension("bin");
    let mut command = Command::new(std::env::var("RUSTC").unwrap_or_else(|_| "rustc".into()));
    command.args(["--edition=2024"]);
    for cfg in cfgs {
        command.arg("--cfg").arg(format!("feature=\"{cfg}\""));
    }
    command
        .arg(&source)
        .arg("-o")
        .arg(binary)
        .output()
        .expect("compile generated directive Rust")
}

fn compile_and_run(name: &str, rust: &str) -> std::process::Output {
    compile_and_run_with_cfgs(name, rust, &[])
}

fn compile_and_run_with_cfgs(name: &str, rust: &str, cfgs: &[&str]) -> std::process::Output {
    let compile = compile_with_cfgs(name, rust, cfgs);
    assert!(
        compile.status.success(),
        "generated Rust failed to compile:\n{}",
        String::from_utf8_lossy(&compile.stderr)
    );
    Command::new(
        Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target/directive-translate-generated")
            .join(name)
            .with_extension("bin"),
    )
    .output()
    .expect("run generated directive binary")
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
fn sanitizes_fatal_directives_without_changing_frontend_source_identity() {
    let rust = translate_directives("sanitized_input.c");

    assert!(rust.contains("#[cfg(feature = \"sanitized_left\")]\nfn selected() -> i32"));
    assert!(rust.contains("#[cfg(not(feature = \"sanitized_left\"))]\nfn selected() -> i32"));
    assert!(rust.contains("i32::MAX"));
    assert_tail_value(&rust, "37");
    assert!(rust.contains("tests/fixtures.cfg/sanitized_input.c"));
}

#[test]
fn unconditional_error_is_typed_preserved_and_fails_rust_compilation() {
    let rust = translate("error_unconditional.c");

    assert!(rust.contains("compile_error!(\"unexpanded ERROR_TOKEN \\\"quoted\\\" C:\\\\tmp\");"));
    let output = compile_with_cfgs("error_unconditional", &rust, &[]);
    assert!(!output.status.success());
    assert!(String::from_utf8_lossy(&output.stderr).contains("unexpanded ERROR_TOKEN"));
}

#[test]
fn conditional_error_fails_only_when_its_cfg_is_selected() {
    let single = translate("error_conditional.c");
    assert!(!single.contains("compile_error!"));
    let active_single = translate_with_clang_args("error_conditional.c", Some("-DFAIL_BUILD"));
    assert!(active_single.contains("compile_error!(\"selected failure\");"));
    assert!(!active_single.contains("#[cfg("));

    let rust = translate_directives("error_conditional.c");
    assert!(
        rust.contains("#[cfg(feature = \"fail_build\")]\ncompile_error!(\"selected failure\");")
    );
    assert!(
        compile_with_cfgs("error_conditional_inactive", &rust, &[])
            .status
            .success()
    );
    let active = compile_with_cfgs("error_conditional_active", &rust, &["fail_build"]);
    assert!(!active.status.success());
    assert!(String::from_utf8_lossy(&active.stderr).contains("selected failure"));
}

#[test]
fn nested_error_uses_the_effective_cfg_condition() {
    let rust = translate_directives("error_nested.c");

    assert!(rust.contains(
        "#[cfg(all(feature = \"outer_failure\", feature = \"inner_failure\"))]\ncompile_error!(\"nested failure\");"
    ));
    assert!(
        compile_with_cfgs("error_nested_outer_only", &rust, &["outer_failure"])
            .status
            .success()
    );
    let active = compile_with_cfgs(
        "error_nested_active",
        &rust,
        &["outer_failure", "inner_failure"],
    );
    assert!(!active.status.success());
    assert!(String::from_utf8_lossy(&active.stderr).contains("nested failure"));
}

#[test]
fn conditional_error_with_unmappable_predicate_is_refused() {
    let err = translate_directives_err("reject/error_unmapped.c");

    assert!(err.contains("does not map to a known Rust cfg"));
    assert!(err.contains("FAILURE_LEVEL == 2"));
}

#[test]
fn warning_uses_a_self_contained_compile_time_fallback() {
    let rust = translate("warning_directives.c");

    assert!(rust.contains(
        "#[deprecated(note = \"WARNING_TOKEN \\\"quoted\\\" C:\\\\tmp\")]\nconst __SLATE_WARNING_0: () = {  };\n\nconst _: () = __SLATE_WARNING_0;"
    ));
    assert!(!rust.contains("selected warning"));
    let output = compile_with_cfgs("warning_standalone", &rust, &[]);
    assert!(output.status.success());
    let stderr = String::from_utf8_lossy(&output.stderr);
    assert!(stderr.contains("WARNING_TOKEN \"quoted\" C:\\tmp"));
}

#[test]
fn warning_follows_its_recovered_cfg_without_becoming_an_error() {
    let rust = translate_directives("warning_directives.c");

    assert!(rust.contains("WARNING_TOKEN \\\"quoted\\\" C:\\\\tmp"));
    assert!(rust.contains(
        "#[cfg(feature = \"slate_warning_feature\")]\n#[deprecated(note = \"selected warning\")]"
    ));
    let inactive = compile_with_cfgs("warning_cfg_inactive", &rust, &[]);
    assert!(inactive.status.success());
    assert!(!String::from_utf8_lossy(&inactive.stderr).contains("selected warning"));
    let active = compile_with_cfgs("warning_cfg_active", &rust, &["slate_warning_feature"]);
    assert!(active.status.success());
    assert!(String::from_utf8_lossy(&active.stderr).contains("selected warning"));
}

#[test]
fn common_diagnostic_pragmas_are_diagnostic_only() {
    let src = cfg_fixtures_dir().join("diagnostic_pragmas.c");
    let output = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("record-cfg")
        .arg(&src)
        .output()
        .expect("record diagnostic pragmas");
    assert!(output.status.success());
    let json: serde_json::Value = serde_json::from_slice(&output.stdout).expect("parse record-cfg");
    let directives = json["directives"].as_array().expect("directive array");
    let pragmas: Vec<_> = directives
        .iter()
        .filter(|directive| directive["name"] == "pragma")
        .collect();
    assert_eq!(pragmas.len(), 6);
    assert!(
        pragmas
            .iter()
            .all(|directive| directive["disposition"] == "diagnostic-only")
    );

    let rust = translate("diagnostic_pragmas.c");
    assert!(
        compile_and_run("diagnostic_pragmas", &rust)
            .status
            .success()
    );
}

#[test]
fn pragma_once_is_no_output_and_does_not_block_translation() {
    let src = cfg_fixtures_dir().join("pragma_once.c");
    let output = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("record-cfg")
        .arg(&src)
        .output()
        .expect("record pragma once");
    assert!(output.status.success());
    let json: serde_json::Value = serde_json::from_slice(&output.stdout).expect("parse record-cfg");
    assert_eq!(json["directives"][0]["name"], "pragma");
    assert_eq!(json["directives"][0]["raw_payload"], "once");
    assert_eq!(json["directives"][0]["disposition"], "no-output");

    let rust = translate("pragma_once.c");
    assert!(compile_and_run("pragma_once", &rust).status.success());
}

#[test]
fn semantic_and_unknown_pragmas_remain_explicitly_unsupported() {
    let src = cfg_fixtures_dir().join("reject/pragma_inventory.c");
    let output = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("record-cfg")
        .arg(&src)
        .output()
        .expect("record pragma inventory");
    assert!(output.status.success());
    let json: serde_json::Value = serde_json::from_slice(&output.stdout).expect("parse record-cfg");
    let pragmas = json["directives"].as_array().expect("directive array");
    assert_eq!(pragmas.len(), 7);
    assert!(
        pragmas
            .iter()
            .all(|directive| directive["disposition"] == "unsupported-semantic")
    );

    let error = translate_err_with_clang_args("reject/pragma_inventory.c", None);
    assert!(error.contains("unsupported semantic directive #pragma at line 1"));
    assert!(error.contains("STDC FENV_ACCESS ON"));
}

#[test]
fn pack_pragmas_are_consumed_in_single_config_translation() {
    let pragma = translate("reject/unsupported_pragma.c");
    assert!(
        compile_and_run("single_config_pack_pragma", &pragma)
            .status
            .success()
    );
    let pragma = translate_directives("reject/unsupported_pragma.c");
    assert!(
        compile_and_run("unconditional_directive_pack_pragma", &pragma)
            .status
            .success()
    );

    let unknown = translate_err_with_clang_args("reject/unsupported_unknown.c", None);
    assert!(unknown.contains("unsupported semantic directive #slate_unknown at line 1"));
    assert!(unknown.contains("preserve this payload"));
}

#[test]
fn conditional_pack_remains_unsupported_only_in_multi_config_translation() {
    let single = translate("unsupported_conditional.c");
    assert!(!single.contains("compile_error!"));

    let active_single =
        translate_with_clang_args("unsupported_conditional.c", Some("-DPACKED_LAYOUT"));
    assert!(!active_single.contains("compile_error!"));

    let rust = translate_directives("unsupported_conditional.c");
    assert!(rust.contains(
        "#[cfg(feature = \"packed_layout\")]\ncompile_error!(\"unsupported semantic directive #pragma at line 2: pack(push, 1)\");"
    ));
    assert!(
        compile_with_cfgs("unsupported_conditional_inactive", &rust, &[])
            .status
            .success()
    );
    let active = compile_with_cfgs("unsupported_conditional_active", &rust, &["packed_layout"]);
    assert!(!active.status.success());
    assert!(
        String::from_utf8_lossy(&active.stderr)
            .contains("unsupported semantic directive #pragma at line 2")
    );
}

#[test]
fn conditional_visibility_remains_unsupported_only_in_multi_config_translation() {
    let single = translate("unsupported_conditional_visibility.c");
    assert!(!single.contains("compile_error!"));

    let active_single =
        translate_with_clang_args("unsupported_conditional_visibility.c", Some("-DHIDDEN_API"));
    assert!(!active_single.contains("compile_error!"));

    let rust = translate_directives("unsupported_conditional_visibility.c");
    assert!(rust.contains(
        "#[cfg(feature = \"hidden_api\")]\ncompile_error!(\"unsupported semantic directive #pragma at line 2: GCC visibility push(hidden)\");"
    ));
    assert!(
        compile_with_cfgs("unsupported_conditional_visibility_inactive", &rust, &[])
            .status
            .success()
    );
    let active = compile_with_cfgs(
        "unsupported_conditional_visibility_active",
        &rust,
        &["hidden_api"],
    );
    assert!(!active.status.success());
    assert!(
        String::from_utf8_lossy(&active.stderr)
            .contains("unsupported semantic directive #pragma at line 2")
    );
}

#[test]
fn conditional_symbol_pragmas_remain_unsupported_only_in_multi_config_translation() {
    let single = translate("unsupported_conditional_symbol_pragmas.c");
    assert!(!single.contains("compile_error!"));

    let active_single = translate_with_clang_args(
        "unsupported_conditional_symbol_pragmas.c",
        Some("-DSYMBOL_PRAGMAS"),
    );
    assert!(!active_single.contains("compile_error!"));

    let rust = translate_directives("unsupported_conditional_symbol_pragmas.c");
    assert!(rust.contains(
        "#[cfg(feature = \"symbol_pragmas\")]\ncompile_error!(\"unsupported semantic directive #pragma at line 2: weak conditional_alias = conditional_target\");"
    ));
    assert!(rust.contains(
        "#[cfg(feature = \"symbol_pragmas\")]\ncompile_error!(\"unsupported semantic directive #pragma at line 3: redefine_extname conditional_name conditional_actual\");"
    ));
    assert!(
        compile_with_cfgs(
            "unsupported_conditional_symbol_pragmas_inactive",
            &rust,
            &[]
        )
        .status
        .success()
    );
    let active = compile_with_cfgs(
        "unsupported_conditional_symbol_pragmas_active",
        &rust,
        &["symbol_pragmas"],
    );
    assert!(!active.status.success());
    assert!(
        String::from_utf8_lossy(&active.stderr)
            .contains("unsupported semantic directive #pragma at line 2")
    );
}

#[test]
fn conditional_macro_state_remains_unsupported_only_in_multi_config_translation() {
    let single = translate("unsupported_conditional_macro_state.c");
    assert!(!single.contains("compile_error!"));

    let active_single = translate_with_clang_args(
        "unsupported_conditional_macro_state.c",
        Some("-DNESTED_MACRO_STATE"),
    );
    assert!(!active_single.contains("compile_error!"));

    let rust = translate_directives("unsupported_conditional_macro_state.c");
    assert!(rust.contains(
        "#[cfg(feature = \"nested_macro_state\")]\ncompile_error!(\"unsupported semantic directive #pragma at line 4: push_macro(\\\"MACRO_STATE_VALUE\\\")\");"
    ));
    assert!(
        compile_with_cfgs("unsupported_conditional_macro_state_inactive", &rust, &[])
            .status
            .success()
    );
    let active = compile_with_cfgs(
        "unsupported_conditional_macro_state_active",
        &rust,
        &["nested_macro_state"],
    );
    assert!(!active.status.success());
}

#[test]
fn unused_conditional_poison_needs_no_generated_error() {
    let rust = translate_directives("conditional_poison_unused.c");
    assert!(!rust.contains("compile_error!"));
    assert!(
        compile_with_cfgs("conditional_poison_unused_inactive", &rust, &[])
            .status
            .success()
    );
    assert!(
        compile_with_cfgs("conditional_poison_unused_active", &rust, &["strict_names"],)
            .status
            .success()
    );
}

#[test]
fn poison_use_surfaces_the_clang_frontend_error() {
    let error = translate_err_with_clang_args("reject/poison_used.c", None);

    assert!(error.contains("clang -emit-cir failed"));
    assert!(error.contains("attempt to use a poisoned identifier"));
    assert!(error.contains("forbidden_identifier"));
    assert!(!error.contains("unsupported semantic directive"));
}

#[test]
fn unsupported_directive_with_unmappable_condition_stops_translation() {
    let err = translate_directives_err("reject/unsupported_unmapped.c");

    assert!(err.contains("unsupported semantic directive #pragma at line 2"));
    assert!(err.contains("PACK_LEVEL == 1"));
    assert!(err.contains("does not map to a known Rust cfg"));
}

#[test]
fn clang_consumed_directives_preserve_generated_behavior() {
    let rust = translate("common_directives.c");

    assert!(!rust.contains("DIRECTIVE_VALUE"));
    assert!(compile_and_run("common_directives", &rust).status.success());
}

#[test]
fn include_next_uses_the_clang_header_search_order() {
    let rust = translate("include_next.c");

    assert!(compile_and_run("include_next", &rust).status.success());
}

#[test]
fn line_directive_preserves_cfg_item_joins_and_presumed_values() {
    let rust = translate_directives("line_directive.c");

    assert!(rust.contains("#[cfg(feature = \"line_feature\")]\nfn line_value() -> i32"));
    assert!(rust.contains("#[cfg(not(feature = \"line_feature\"))]\nfn line_value() -> i32"));
    assert!(rust.contains("701"));
    assert!(rust.contains("704"));
    assert!(
        compile_and_run("line_directive_default", &rust)
            .status
            .success()
    );
    assert!(
        compile_and_run_with_cfgs("line_directive_feature", &rust, &["line_feature"])
            .status
            .success()
    );
}

#[test]
fn embed_bytes_are_consumed_by_clang_and_lowered() {
    let rust = translate_directives("embed_basic.c");

    assert!(compile_and_run("embed_basic", &rust).status.success());
}

#[test]
fn unsupported_embed_input_fails_explicitly() {
    let err = translate_directives_err("reject/embed_missing.c");

    assert!(err.contains("missing-embed-data.bin"));
    assert!(err.contains("file not found"));
}

#[test]
fn directive_translated_fixtures_compile_for_current_host() {
    let work_dir = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/directive-translate-compile");
    let names = [
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
        "common_directives.c",
        "embed_basic.c",
        "include_next.c",
        "line_directive.c",
        "pragma_once.c",
        "warning_directives.c",
        "diagnostic_pragmas.c",
        "unsupported_conditional.c",
    ];
    let cases = support::parallel_map(&names, |name| {
        let rust = translate_directives(name);
        let rs = write_generated(name, &rust);
        support::RustCase {
            name: format!("cfg_{}", name.trim_end_matches(".c")),
            rs_src: rs,
        }
    });
    let failures: Vec<String> = support::compile_rs_batch(&cases, &work_dir)
        .into_iter()
        .filter_map(|(name, result)| result.err().map(|err| format!("{name}:\n{err}")))
        .collect();
    if !failures.is_empty() {
        panic!(
            "generated cfg Rust did not compile:\n{}",
            failures.join("\n\n")
        );
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

#[test]
fn raw_lower_skips_fixups_for_directive_translation() {
    let src = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/add.c");
    let out = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-directives")
        .arg(&src)
        .env("SLATE_RAW_LOWER", "1")
        .output()
        .expect("run raw slate translate-directives");
    assert!(
        out.status.success(),
        "raw translate-directives failed:\n{}",
        String::from_utf8_lossy(&out.stderr)
    );
    let rust = String::from_utf8(out.stdout).expect("generated Rust is utf8");
    assert!(rust.contains("let mut __retval"));
    assert!(!rust.contains("println!"));
}
