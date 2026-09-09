mod support;

use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::atomic::{AtomicU64, Ordering};

fn cfg_fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.cfg")
}

fn check_directive_output(name: &str, rust: &str) {
    static SEQ: AtomicU64 = AtomicU64::new(0);
    let fixture_path = cfg_fixtures_dir().join(name);
    let fixture = std::fs::read_to_string(&fixture_path).expect("read directive fixture");
    let slot = format!(
        "{name}.{}.{}",
        std::process::id(),
        SEQ.fetch_add(1, Ordering::Relaxed)
    );
    support::filecheck::check_generated_rust_with_prefixes(
        &fixture,
        rust,
        support::filecheck::Profile::Lowering,
        &["DIRECTIVES"],
        &Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target/directive-filecheck")
            .join(slot),
    )
    .unwrap_or_else(|error| panic!("{}: {error}", fixture_path.display()));
}

fn check_rust_shape(label: &str, checks: &str, rust: &str, profile: support::filecheck::Profile) {
    static SEQ: AtomicU64 = AtomicU64::new(0);
    let slot = format!(
        "{label}.{}.{}",
        std::process::id(),
        SEQ.fetch_add(1, Ordering::Relaxed)
    );
    support::filecheck::check_generated_rust(
        checks,
        rust,
        profile,
        &Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target/directive-filecheck")
            .join(slot),
    )
    .unwrap_or_else(|error| panic!("{label}: {error}"));
}

fn directive_filecheck_fixtures() -> Vec<String> {
    let mut fixtures = std::fs::read_dir(cfg_fixtures_dir())
        .expect("read directive fixtures")
        .filter_map(|entry| entry.ok())
        .filter(|entry| entry.path().extension().and_then(|ext| ext.to_str()) == Some("c"))
        .filter(|entry| {
            std::fs::read_to_string(entry.path()).is_ok_and(|fixture| {
                support::filecheck::has_checks_with_prefixes(
                    &fixture,
                    support::filecheck::Profile::Lowering,
                    &["DIRECTIVES"],
                )
            })
        })
        .map(|entry| entry.file_name().to_string_lossy().into_owned())
        .collect::<Vec<_>>();
    fixtures.sort();
    fixtures
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

#[test]
fn generated_directive_filecheck() {
    for fixture in directive_filecheck_fixtures() {
        let rust = translate(&fixture);
        check_directive_output(&fixture, &rust);
    }
}

#[test]
fn target_only_divergence_needs_no_source_ifdef_and_splices_by_target_arch() {
    let src = cfg_fixtures_dir().join("target_only_divergence.c");
    let out = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate")
        .arg("--targets=x86_64-linux-gnu,aarch64-linux-gnu")
        .arg(&src)
        .output()
        .expect("run slate translate --targets");
    assert!(
        out.status.success(),
        "translate --targets failed:\n{}",
        String::from_utf8_lossy(&out.stderr)
    );
    let rust = String::from_utf8(out.stdout).expect("generated Rust is utf8");

    assert_eq!(rust.matches("fn printf(").count(), 1);

    check_rust_shape(
        "target_only_divergence",
        concat!(
            "// COMMON: #[cfg(target_arch = \"x86_64\")]\n",
            "// COMMON-NEXT: fn main()\n",
            "// COMMON: #[cfg(target_arch = \"aarch64\")]\n",
            "// COMMON-NEXT: fn main()\n",
            "// COMMON: println!(\"{}\", 295 as i32);\n",
            "// COMMON: println!(\"{}\", 69 as i32);\n",
        ),
        &rust,
        support::filecheck::Profile::Rewrites,
    );

    let output = compile_and_run("target_only_divergence", &rust);
    assert!(output.status.success());
    assert_eq!(
        String::from_utf8_lossy(&output.stdout),
        format!(
            "{}\n",
            if cfg!(target_arch = "x86_64") {
                295
            } else {
                69
            }
        )
    );
}

#[test]
fn target_and_macro_divergence_compose_without_cross_product() {
    let src = cfg_fixtures_dir().join("target_and_macro_divergence.c");
    let out = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate")
        .arg("--targets=x86_64-linux-gnu,aarch64-linux-gnu")
        .arg(&src)
        .output()
        .expect("run slate translate --targets");
    assert!(
        out.status.success(),
        "translate --targets failed:\n{}",
        String::from_utf8_lossy(&out.stderr)
    );
    let rust = String::from_utf8(out.stdout).expect("generated Rust is utf8");

    assert_eq!(rust.matches("fn feature_code()").count(), 2);
    assert_eq!(rust.matches("fn main()").count(), 2);

    check_rust_shape(
        "target_and_macro_divergence",
        concat!(
            "// COMMON: #[cfg(feature = \"my_feature\")]\n",
            "// COMMON-NEXT: fn feature_code()\n",
            "// COMMON: #[cfg(not(feature = \"my_feature\"))]\n",
            "// COMMON-NEXT: fn feature_code()\n",
            "// COMMON-NOT: #[cfg(any(target_arch\n",
            "// COMMON: #[cfg(target_arch = \"x86_64\")]\n",
            "// COMMON-NEXT: fn main()\n",
            "// COMMON: #[cfg(target_arch = \"aarch64\")]\n",
            "// COMMON-NEXT: fn main()\n",
        ),
        &rust,
        support::filecheck::Profile::Rewrites,
    );

    let output = compile_and_run("target_and_macro_divergence", &rust);
    assert!(output.status.success());
    assert_eq!(
        String::from_utf8_lossy(&output.stdout),
        format!(
            "20 {}\n",
            if cfg!(target_arch = "x86_64") {
                295
            } else {
                69
            }
        )
    );
}

#[test]
fn target_macro_conditionals_auto_expand_and_run_correctly_on_host() {
    let rust = translate("os_targets.c");
    let output = compile_and_run("os_targets", &rust);
    assert!(output.status.success());
    assert_eq!(String::from_utf8_lossy(&output.stdout), "20\n");
}

#[test]
fn unconditional_error_is_typed_preserved_and_fails_rust_compilation() {
    let rust = translate("error_unconditional.c");

    let output = compile_with_cfgs("error_unconditional", &rust, &[]);
    assert!(!output.status.success());
    assert!(String::from_utf8_lossy(&output.stderr).contains("unexpanded ERROR_TOKEN"));
}

#[test]
fn conditional_error_only_triggers_when_its_macro_is_defined() {
    let active_single = translate_with_clang_args("error_conditional.c", Some("-DFAIL_BUILD"));
    check_rust_shape(
        "error_conditional_active",
        concat!(
            "// COMMON: compile_error!(\"selected failure\");\n",
            "// COMMON-NOT: #[cfg(\n",
        ),
        &active_single,
        support::filecheck::Profile::Rewrites,
    );
}

#[test]
fn warning_uses_a_self_contained_compile_time_fallback() {
    let rust = translate("warning_directives.c");

    let output = compile_with_cfgs("warning_standalone", &rust, &[]);
    assert!(output.status.success());
    let stderr = String::from_utf8_lossy(&output.stderr);
    assert!(stderr.contains("WARNING_TOKEN \"quoted\" C:\\tmp"));
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
    assert!(error.contains("unsupported semantic directive #pragma at line 6"));
    assert!(error.contains("optimize(\"O2\")"));
}

#[test]
fn pack_pragma_is_consumed_by_clang_and_does_not_block_translation() {
    let pragma = translate("reject/unsupported_pragma.c");
    assert!(
        compile_and_run("single_config_pack_pragma", &pragma)
            .status
            .success()
    );

    let unknown = translate_err_with_clang_args("reject/unsupported_unknown.c", None);
    assert!(unknown.contains("unsupported semantic directive #slate_unknown at line 1"));
    assert!(unknown.contains("preserve this payload"));
}

#[test]
fn conditional_pack_pragma_is_a_no_op_when_its_macro_is_undefined() {
    let active_single =
        translate_with_clang_args("unsupported_conditional.c", Some("-DPACKED_LAYOUT"));
    check_rust_shape(
        "unsupported_conditional_active",
        "// COMMON-NOT: compile_error!\n",
        &active_single,
        support::filecheck::Profile::Rewrites,
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
fn clang_consumed_directives_preserve_generated_behavior() {
    let rust = translate("common_directives.c");

    assert!(compile_and_run("common_directives", &rust).status.success());
}

#[test]
fn include_next_uses_the_clang_header_search_order() {
    let rust = translate("include_next.c");

    assert!(compile_and_run("include_next", &rust).status.success());
}

#[test]
fn line_directive_presumed_location_is_correct_by_default() {
    let rust = translate("line_directive.c");

    assert!(
        compile_and_run("line_directive_default", &rust)
            .status
            .success()
    );
}

#[test]
fn embed_bytes_are_consumed_by_clang_and_lowered() {
    let rust = translate("embed_basic.c");

    assert!(compile_and_run("embed_basic", &rust).status.success());
}

#[test]
fn unsupported_embed_input_fails_explicitly() {
    let err = translate_err_with_clang_args("reject/embed_missing.c", None);

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
        let rust = translate(name);
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
fn passes_through_sources_without_conditional_regions() {
    let src = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/add.c");
    let out = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate")
        .arg(&src)
        .output()
        .expect("run slate translate");
    assert!(
        out.status.success(),
        "translate failed on a plain source:\n{}",
        String::from_utf8_lossy(&out.stderr)
    );
    let rust = String::from_utf8(out.stdout).expect("generated Rust is utf8");
    check_rust_shape(
        "add_plain_source",
        concat!("// COMMON: fn add(\n", "// COMMON-NOT: #[cfg(\n"),
        &rust,
        support::filecheck::Profile::Rewrites,
    );
}

#[test]
fn raw_lower_skips_fixups_for_translation() {
    let src = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/add.c");
    let out = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate")
        .arg(&src)
        .env("SLATE_RAW_LOWER", "1")
        .output()
        .expect("run raw slate translate");
    assert!(
        out.status.success(),
        "raw translate failed:\n{}",
        String::from_utf8_lossy(&out.stderr)
    );
    let rust = String::from_utf8(out.stdout).expect("generated Rust is utf8");
    check_rust_shape(
        "add_raw_lower",
        concat!(
            "// COMMON: let __v0: i32 = arg0 + arg1;\n",
            "// COMMON-NOT: println!\n",
        ),
        &rust,
        support::filecheck::Profile::Lowering,
    );
}
