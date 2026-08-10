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

/// Translate a multi-TU fixture into a Cargo crate, then diff the C (all
/// units linked) against the Rust (the crate's binary). Returns the crate's
/// `src/` directory of generated `.rs` modules for per-test structural
/// assertions.
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

    let rs_bin = support::compile_rs_project(&rs_dir).expect("compile Rust");

    let run_dir = work.join("run");
    let _ = std::fs::remove_dir_all(&run_dir);
    std::fs::create_dir_all(&run_dir).expect("create run dir");
    let cfg = support::RunConfig::default();
    let c = support::run_with_config(&c_bin, &cfg, &run_dir).expect("run C");
    let r = support::run_with_config(&rs_bin, &cfg, &run_dir).expect("run Rust");
    support::compare_runs(&c, &r, false).expect("C and Rust outputs differ");

    rs_dir.join("src")
}

fn cross_tu_work_dir(name: &str) -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/cross-tu")
        .join(name)
}

#[test]
fn project_translation_rejects_active_unsupported_directives() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.multi.reject")
        .join("unsupported_directive");
    let out_dir = cross_tu_work_dir("unsupported-directive-policy").join("rs");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-project")
        .arg(&dir)
        .arg(&out_dir)
        .output()
        .expect("run translate-project");

    assert!(!output.status.success());
    let stderr = String::from_utf8_lossy(&output.stderr);
    assert!(stderr.contains("unsupported semantic directive #pragma at line 1"));
    assert!(stderr.contains("STDC FENV_ACCESS ON"));
}

#[test]
fn project_translation_defaults_to_the_active_target() {
    let dir = fixture_dir("project_strtold");
    let out_dir = cross_tu_work_dir("host-only-project").join("rs");
    let _ = std::fs::remove_dir_all(&out_dir);
    support::translate_project(&dir, &out_dir).expect("translate host project");

    let parse = std::fs::read_to_string(out_dir.join("src/parse.rs")).expect("read parse.rs");
    assert!(!parse.contains("target_arch ="));
}

#[test]
fn project_translation_adds_explicit_target_variants() {
    let dir = fixture_dir("project_strtold");
    let out_dir = cross_tu_work_dir("extra-target-project").join("rs");
    let _ = std::fs::remove_dir_all(&out_dir);
    let (target, added_arch) = if std::env::consts::ARCH == "aarch64" {
        ("x86_64-linux-gnu", "x86_64")
    } else {
        ("aarch64-linux-gnu", "aarch64")
    };
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate-project", "--target", target])
        .arg(&dir)
        .arg(&out_dir)
        .env("SLATE_CLANG_ARGS", "-std=c23")
        .output()
        .expect("translate multi-target project");
    assert!(
        output.status.success(),
        "translate-project failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );

    let parse = std::fs::read_to_string(out_dir.join("src/parse.rs")).expect("read parse.rs");
    assert!(parse.contains(&format!("target_arch = \"{}\"", std::env::consts::ARCH)));
    assert!(parse.contains(&format!("target_arch = \"{added_arch}\"")));
}

fn assert_binary_sections(binary: &Path, sections: &[&str]) {
    let out = std::process::Command::new("readelf")
        .args(["-S", "--wide"])
        .arg(binary)
        .output()
        .expect("run readelf");
    assert!(
        out.status.success(),
        "readelf failed for {}:\n{}",
        binary.display(),
        String::from_utf8_lossy(&out.stderr)
    );
    let stdout = String::from_utf8_lossy(&out.stdout);
    for section in sections {
        assert!(
            stdout.contains(section),
            "missing section {section} in {}:\n{stdout}",
            binary.display()
        );
    }
}

fn binary_symbol_table(binary: &Path) -> String {
    let out = std::process::Command::new("readelf")
        .args(["-s", "--wide"])
        .arg(binary)
        .output()
        .expect("run readelf");
    assert!(
        out.status.success(),
        "readelf failed for {}:\n{}",
        binary.display(),
        String::from_utf8_lossy(&out.stderr)
    );
    String::from_utf8_lossy(&out.stdout).into_owned()
}

fn assert_binary_symbols(binary: &Path, symbols: &[&str]) {
    let table = binary_symbol_table(binary);
    for symbol in symbols {
        assert!(
            table.contains(symbol),
            "missing symbol {symbol} in {}:\n{table}",
            binary.display()
        );
    }
}

fn assert_binary_lacks_symbols(binary: &Path, symbols: &[&str]) {
    let table = binary_symbol_table(binary);
    for symbol in symbols {
        assert!(
            !table.contains(symbol),
            "unexpected symbol {symbol} in {}:\n{table}",
            binary.display()
        );
    }
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
    assert!(
        math_rs.contains("slate_support::warning!(\"PROJECT_WARNING_TOKEN remains unexpanded\");")
    );
    assert!(math_rs.contains("#[unsafe(no_mangle)]\npub extern \"C\" fn square"));
    assert!(math_rs.contains("use crate::types::shared_mode_t;"));
    assert!(math_rs.contains("value.mode = shared_mode_t::SHARED_READY;"));
    let types_rs = std::fs::read_to_string(crate_dir.join("src/types.rs")).expect("read types.rs");
    assert!(types_rs.contains("pub enum shared_mode_t"));
    assert!(types_rs.contains("SHARED_READY = 1"));
    assert!(crate_dir.join("tests/run_smoke.rs").is_file());
    let smoke_rs =
        std::fs::read_to_string(crate_dir.join("tests/run_smoke.rs")).expect("read run_smoke.rs");
    assert!(smoke_rs.contains("use slate_crate::types::shared_mode_t;"));
    assert!(smoke_rs.contains("use slate_crate::math::square;"));
    assert!(smoke_rs.contains("use slate_crate::state::bump;"));
    assert!(!smoke_rs.contains("fn square"));
    assert!(!smoke_rs.contains("fn bump"));
    assert!(!smoke_rs.contains("enum shared_mode_t"));
    let manifest = std::fs::read_to_string(crate_dir.join("Cargo.toml")).expect("read manifest");
    assert!(manifest.contains("slate-support = { path = \"slate-support\" }"));
    assert!(manifest.contains("[[test]]"));
    assert!(manifest.contains("name = \"run_smoke\""));
    assert!(manifest.contains("harness = false"));

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
    let check_stderr = String::from_utf8_lossy(&check.stderr);
    assert!(check_stderr.contains("PROJECT_WARNING_TOKEN remains unexpanded"));
    assert!(!check_stderr.contains("--> src/math.rs"));
    let check_tests = std::process::Command::new("cargo")
        .args(["check", "--quiet", "--tests", "--manifest-path"])
        .arg(crate_dir.join("Cargo.toml"))
        .output()
        .expect("cargo check generated tests");
    assert!(
        check_tests.status.success(),
        "generated integration tests should type-check:\n{}",
        String::from_utf8_lossy(&check_tests.stderr)
    );
    let run_tests = std::process::Command::new("cargo")
        .args(["test", "--quiet", "--tests", "--manifest-path"])
        .arg(crate_dir.join("Cargo.toml"))
        .output()
        .expect("cargo run generated tests");
    assert!(
        run_tests.status.success(),
        "generated integration tests should run:\n{}",
        String::from_utf8_lossy(&run_tests.stderr)
    );
}

#[test]
fn library_project_source_manifest_selects_translation_units() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.library")
        .join("source_manifest");
    let work = support::test_cache_root().join("cross-tu/library-source-manifest");
    let crate_dir = work.join("crate");
    let _ = std::fs::remove_dir_all(&work);

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate-project", "--lib", "--source-manifest"])
        .arg(dir.join("sources.txt"))
        .arg(&dir)
        .arg(&crate_dir)
        .output()
        .expect("run slate translate-project --lib --source-manifest");
    assert!(
        output.status.success(),
        "translate-project --lib --source-manifest failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );

    let lib_rs = std::fs::read_to_string(crate_dir.join("src/lib.rs")).expect("read lib.rs");
    assert!(lib_rs.contains("pub mod xmlparse;"));
    assert!(lib_rs.contains("pub mod random_getrandom;"));
    assert!(!lib_rs.contains("xmltok_impl"));
    assert!(!lib_rs.contains("random_rand_s"));
    assert!(!crate_dir.join("src/xmltok_impl.rs").exists());
    assert!(!crate_dir.join("src/random_rand_s.rs").exists());
    assert!(!crate_dir.join("tests/upstream_suite.rs").exists());

    let check = std::process::Command::new("cargo")
        .args(["check", "--quiet", "--lib", "--manifest-path"])
        .arg(crate_dir.join("Cargo.toml"))
        .output()
        .expect("cargo check generated lib crate");
    assert!(
        check.status.success(),
        "generated manifest-selected lib crate should type-check:\n{}",
        String::from_utf8_lossy(&check.stderr)
    );
}

#[test]
fn library_project_merges_normalized_compile_command_variants() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.library")
        .join("compile_commands");
    let work = support::test_cache_root().join("cross-tu/library-compile-commands");
    let host_build = work.join("host-build");
    let other_build = work.join("other-build");
    let crate_dir = work.join("crate");
    let _ = std::fs::remove_dir_all(&work);
    std::fs::create_dir_all(&host_build).expect("create host build directory");
    std::fs::create_dir_all(&other_build).expect("create other build directory");
    std::fs::write(host_build.join("config.h"), "#define CONFIGURED_VALUE 32\n")
        .expect("write host config");
    std::fs::write(
        other_build.join("config.h"),
        "#define CONFIGURED_VALUE 64\n",
    )
    .expect("write other config");

    let configured = dir.join("configured.c");
    let host_commands = serde_json::json!([
        {
            "directory": dir,
            "file": "configured.c",
            "arguments": [
                "clang", "-std=c23", "-I", host_build, "-MD", "-MF", "configured.d",
                "-c", "configured.c", "-o", "configured.o"
            ]
        },
        {
            "directory": dir,
            "file": "host_only.c",
            "arguments": [
                "clang", "-std=c23", "-I", host_build, "-c", "host_only.c", "-o", "host_only.o"
            ]
        }
    ]);
    let host_database = host_build.join("compile_commands.json");
    std::fs::write(
        &host_database,
        serde_json::to_vec(&host_commands).expect("encode host commands"),
    )
    .expect("write host commands");

    let (other_target, other_arch) = if std::env::consts::ARCH == "aarch64" {
        ("x86_64-linux-gnu", "x86_64")
    } else {
        ("aarch64-linux-gnu", "aarch64")
    };
    let other_command = format!(
        "clang -std=c23 -target {other_target} -I{} -c {} -o configured.o",
        other_build.display(),
        configured.display()
    );
    let other_commands = serde_json::json!([{
        "directory": other_build,
        "file": configured,
        "command": other_command
    }]);
    let other_database = other_build.join("compile_commands.json");
    std::fs::write(
        &other_database,
        serde_json::to_vec(&other_commands).expect("encode other commands"),
    )
    .expect("write other commands");

    let direct = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate")
        .arg("-I")
        .arg(&host_build)
        .arg(&configured)
        .env("SLATE_RAW_LOWER", "1")
        .output()
        .expect("translate configured source directly");
    assert!(
        direct.status.success(),
        "configured source translation failed:\n{}",
        String::from_utf8_lossy(&direct.stderr)
    );

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate-project", "--lib", "--compile-commands"])
        .arg(&host_database)
        .arg("--compile-commands")
        .arg(&other_database)
        .arg(&dir)
        .arg(&crate_dir)
        .output()
        .expect("run slate translate-project --lib --compile-commands");
    assert!(
        output.status.success(),
        "translate-project --lib --compile-commands failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );

    let configured_rs =
        std::fs::read_to_string(crate_dir.join("src/configured.rs")).expect("read configured.rs");
    assert!(configured_rs.contains("32"));
    assert!(configured_rs.contains("64"));
    assert!(configured_rs.contains(&format!("target_arch = \"{}\"", std::env::consts::ARCH)));
    assert!(configured_rs.contains(&format!("target_arch = \"{other_arch}\"")));

    let host_only_rs =
        std::fs::read_to_string(crate_dir.join("src/host_only.rs")).expect("read host_only.rs");
    assert!(host_only_rs.contains(&format!("target_arch = \"{}\"", std::env::consts::ARCH)));
    let lib_rs = std::fs::read_to_string(crate_dir.join("src/lib.rs")).expect("read lib.rs");
    assert!(lib_rs.contains(&format!("target_arch = \"{}\"", std::env::consts::ARCH)));
    assert!(lib_rs.contains("pub mod host_only;"));

    let reversed_crate = work.join("crate-reversed");
    let reversed = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate-project", "--lib", "--compile-commands"])
        .arg(&other_database)
        .arg("--compile-commands")
        .arg(&host_database)
        .arg(&dir)
        .arg(&reversed_crate)
        .output()
        .expect("run slate with reversed compile commands");
    assert!(
        reversed.status.success(),
        "reversed compile commands failed:\n{}",
        String::from_utf8_lossy(&reversed.stderr)
    );
    for file in ["configured.rs", "host_only.rs", "lib.rs"] {
        let forward = std::fs::read(crate_dir.join("src").join(file)).expect("read forward file");
        let reversed =
            std::fs::read(reversed_crate.join("src").join(file)).expect("read reversed file");
        assert_eq!(forward, reversed, "compile command order changed {file}");
    }

    let check = std::process::Command::new("cargo")
        .args(["check", "--quiet", "--lib", "--manifest-path"])
        .arg(crate_dir.join("Cargo.toml"))
        .output()
        .expect("cargo check generated library");
    assert!(
        check.status.success(),
        "generated library should compile:\n{}",
        String::from_utf8_lossy(&check.stderr)
    );
}

#[test]
fn library_compile_commands_preserve_active_fatal_directives() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.library")
        .join("compile_commands");
    let work = support::test_cache_root().join("cross-tu/library-compile-commands-error");
    let build = work.join("build");
    let crate_dir = work.join("crate");
    let _ = std::fs::remove_dir_all(&work);
    std::fs::create_dir_all(&build).expect("create build directory");
    std::fs::write(build.join("config.h"), "").expect("write empty config");
    let configured = dir.join("configured.c");
    let commands = serde_json::json!([{
        "directory": build,
        "file": configured,
        "arguments": ["clang", "-I", build, "-c", configured, "-o", "configured.o"]
    }]);
    let database = build.join("compile_commands.json");
    std::fs::write(
        &database,
        serde_json::to_vec(&commands).expect("encode commands"),
    )
    .expect("write commands");

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate-project", "--lib", "--compile-commands"])
        .arg(&database)
        .arg(&dir)
        .arg(&crate_dir)
        .output()
        .expect("run slate with active fatal directive");
    assert!(!output.status.success());
    let stderr = String::from_utf8_lossy(&output.stderr);
    assert!(stderr.contains("CONFIGURED_VALUE must be provided by the configured build"));
    assert!(!stderr.contains("cannot determine whether #error"));
}

#[test]
fn library_compile_commands_reject_invalid_shell_commands() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.library")
        .join("compile_commands");
    let work = support::test_cache_root().join("cross-tu/library-compile-commands-invalid");
    let crate_dir = work.join("crate");
    let _ = std::fs::remove_dir_all(&work);
    std::fs::create_dir_all(&work).expect("create work directory");
    let commands = serde_json::json!([{
        "directory": dir,
        "file": "configured.c",
        "command": "clang 'unterminated"
    }]);
    let database = work.join("compile_commands.json");
    std::fs::write(
        &database,
        serde_json::to_vec(&commands).expect("encode commands"),
    )
    .expect("write commands");

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate-project", "--lib", "--compile-commands"])
        .arg(&database)
        .arg(&dir)
        .arg(&crate_dir)
        .output()
        .expect("run slate with invalid compile command");
    assert!(!output.status.success());
    assert!(String::from_utf8_lossy(&output.stderr).contains("invalid shell quoting"));
}

#[test]
fn uart_library_preserves_exported_volatile_io() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.library")
        .join("uart");
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/cross-tu")
        .join("uart-library");
    let crate_dir = work.join("uart");
    let _ = std::fs::remove_dir_all(&work);
    std::fs::create_dir_all(&work).expect("create uart library work dir");

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

    let rust = std::fs::read_to_string(crate_dir.join("src/uart.rs")).expect("read uart.rs");
    assert!(rust.contains("#[unsafe(no_mangle)]\npub extern \"C\" fn send_byte"));
    assert!(rust.contains("std::ptr::read_volatile"));
    assert!(rust.contains("std::ptr::write_volatile"));
}

#[test]
fn library_crate_links_generated_c_abi_shim_for_long_double_libc_call() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.library")
        .join("c23_strfrom");
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/cross-tu")
        .join("c23-strfrom-library");
    let crate_dir = work.join("crate");
    let _ = std::fs::remove_dir_all(&work);

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate-project", "--lib"])
        .arg(&dir)
        .arg(&crate_dir)
        .env("SLATE_CLANG_ARGS", "-std=c23")
        .output()
        .expect("run slate translate-project --lib");
    assert!(
        output.status.success(),
        "translate-project --lib failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );

    assert!(crate_dir.join("build.rs").is_file());
    let shim_c =
        std::fs::read_to_string(crate_dir.join("src/slate_shims.c")).expect("read slate_shims.c");
    assert!(shim_c.contains("strfroml"));
    assert!(shim_c.contains("(long double)"));
    let manifest = std::fs::read_to_string(crate_dir.join("Cargo.toml")).expect("read manifest");
    assert!(manifest.contains("[build-dependencies]"));
    assert!(manifest.contains("cc = \"1\""));

    let strfrom_rs =
        std::fs::read_to_string(crate_dir.join("src/strfrom.rs")).expect("read strfrom.rs");
    assert!(strfrom_rs.contains("fn __slate_strfroml__pi8_u64_pi8_ld("));
    assert!(strfrom_rs.contains("unsafe { __slate_strfroml__pi8_u64_pi8_ld("));

    let run_tests = std::process::Command::new("cargo")
        .args(["test", "--quiet", "--tests", "--manifest-path"])
        .arg(crate_dir.join("Cargo.toml"))
        .output()
        .expect("cargo run generated tests");
    assert!(
        run_tests.status.success(),
        "generated integration test should run and pass (verifies the C shim actually links \
         and strfroml formats correctly):\n{}",
        String::from_utf8_lossy(&run_tests.stderr)
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
        main_rs.contains("use crate::math::square;\nuse crate::math::cube;\n\n"),
        "consecutive imports should form one block"
    );
    assert!(
        !main_rs.contains("fn square"),
        "definition must live in its own module, not the root"
    );
    let math_rs = std::fs::read_to_string(rs_dir.join("math.rs")).expect("read math.rs");
    assert!(
        math_rs.contains("#[unsafe(no_mangle)]\npub extern \"C\" fn square"),
        "external-linkage sibling defs must be pub C ABI symbols"
    );
}

#[test]
fn project_translation_escapes_rust_keywords() {
    build_and_diff("keyword_project_idents");
}

#[test]
fn sibling_can_call_function_defined_in_root_translation_unit() {
    build_and_diff("root_tu_symbol");
}

#[test]
fn project_translation_shares_record_types() {
    build_and_diff("shared_record");
}

#[test]
fn project_translation_shares_enum_types() {
    build_and_diff("shared_enum");
}

#[test]
fn project_translation_shares_long_double_types() {
    build_and_diff("shared_long_double");
}

#[test]
fn project_translation_emits_required_long_double_shims() {
    let rs_dir = build_and_diff("project_strtold");
    let shim = std::fs::read_to_string(rs_dir.join("slate_shims.c")).expect("read slate_shims.c");
    assert!(shim.contains("void __slate_strtold"));
}

#[test]
fn project_translation_shares_shim_record_types() {
    let rs_dir = build_and_diff("shared_stat");
    let types = std::fs::read_to_string(rs_dir.join("types.rs")).expect("read types.rs");
    assert!(types.contains("pub struct __slate_stat"));
}

#[test]
fn project_translation_shares_anonymous_record_types() {
    build_and_diff("shared_anonymous_record");
}

#[test]
fn project_translation_shares_function_pointer_types() {
    build_and_diff("shared_function_pointer");
}

#[test]
fn ctype_libc_fixup_stays_off_when_a_sibling_tu_changes_locale() {
    let rs_dir = build_and_diff("ctype_locale");

    // locale_setup.c's non-"C" setlocale call is invisible to main.c's own
    // Program, so the ctype fixup must stay disabled project-wide rather than
    // idiomizing toupper/tolower as if the C locale were guaranteed.
    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");
    assert!(main_rs.contains("unsafe { toupper("));
    assert!(main_rs.contains("unsafe { tolower("));
}

#[test]
fn public_pointer_deref_functions_are_unsafe() {
    let rs_dir = build_and_diff("unsafe_public");

    let pointers_rs =
        std::fs::read_to_string(rs_dir.join("pointers.rs")).expect("read pointers.rs");
    assert!(pointers_rs.contains("pub unsafe extern \"C\" fn read_ptr"));

    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");
    assert!(main_rs.contains("unsafe { read_ptr("));
}

#[test]
fn cross_tu_variadic_calls_are_unsafe() {
    let rs_dir = build_and_diff("unsafe_variadic");

    let helper_rs = std::fs::read_to_string(rs_dir.join("helper.rs")).expect("read helper.rs");
    assert!(helper_rs.contains("pub unsafe extern \"C\" fn bump"));

    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");
    assert!(main_rs.contains("unsafe { bump("));
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
        other_rs.contains("#[unsafe(no_mangle)]\npub extern \"C\" fn compute"),
        "external-linkage fn must be exported as a pub C ABI symbol"
    );
    assert!(
        !other_rs.contains("pub extern \"C\" fn local")
            && !other_rs.contains("no_mangle)]\nfn local")
            && other_rs.contains("fn local"),
        "sibling's internal-linkage fn must not be externally exported"
    );
    assert!(
        !other_rs.contains("no_mangle)]\nstatic mut base")
            && !other_rs.contains("pub static mut base")
            && other_rs.contains("static mut base"),
        "sibling's internal-linkage global must not be externally exported"
    );
}

#[test]
fn cross_tu_globals() {
    let rs_dir = build_and_diff("globals");
    let work = cross_tu_work_dir("globals");

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
        state_rs.contains(
            "#[unsafe(no_mangle)]\n#[unsafe(link_section = \".slate_data\")]\npub static mut counter"
        ),
        "the defining module must export the global as a stable C symbol"
    );
    assert!(
        state_rs.contains(
            "#[unsafe(no_mangle)]\n#[unsafe(link_section = \".slate_fn\")]\npub extern \"C\" fn bump"
        ),
        "the defining module must export external-linkage functions as stable C ABI symbols"
    );
    assert!(
        state_rs.contains("pub static mut unreferenced_global"),
        "an externally-linked global must survive unused_items pruning even with no in-project references"
    );
    assert_binary_sections(&work.join("c_bin"), &[".slate_data", ".slate_fn"]);
    let rs_bin = support::compile_rs_project(&work.join("rs")).expect("compile Rust");
    assert_binary_sections(&rs_bin, &[".slate_data", ".slate_fn"]);
}

#[test]
fn cross_tu_thread_local_globals() {
    let rs_dir = build_and_diff("thread_local");
    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");
    let state_rs = std::fs::read_to_string(rs_dir.join("state.rs")).expect("read state.rs");

    assert!(main_rs.contains("use crate::state::shared_value;"));
    assert!(state_rs.contains("#[thread_local]\npub static mut shared_value: i32 = 5;"));
}

#[test]
fn used_and_retain_attrs_preserve_dead_statics() {
    let rs_dir = build_and_diff("used_retain");
    let work = cross_tu_work_dir("used_retain");
    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");

    assert!(main_rs.contains("#![feature(used_with_arg)]"));
    assert!(main_rs.contains("#[used]\nstatic mut used_only"));
    assert!(main_rs.contains("#[used]\n#[used(linker)]\nstatic mut used_and_retained"));
    assert!(
        !main_rs.contains("retain_only"),
        "retain-only dead static should match C and not be kept alive"
    );

    let symbols = &["used_only", "used_and_retained"];
    assert_binary_symbols(&work.join("c_bin"), symbols);
    assert_binary_lacks_symbols(&work.join("c_bin"), &["retain_only"]);
    let rs_bin = support::compile_rs_project(&work.join("rs")).expect("compile Rust");
    assert_binary_symbols(&rs_bin, symbols);
    assert_binary_lacks_symbols(&rs_bin, &["retain_only"]);
}

#[test]
fn weak_linkage_attrs_emit_for_globals_and_functions() {
    let rs_dir = build_and_diff("weak_linkage");

    let weak_rs = std::fs::read_to_string(rs_dir.join("weak.rs")).expect("read weak.rs");
    assert!(weak_rs.contains("#![feature(linkage)]"));
    assert!(
        weak_rs.contains("#[unsafe(no_mangle)]\n#[linkage = \"weak\"]\npub static mut weak_global"),
        "weak global should be exported as a weak C symbol:\n{weak_rs}"
    );
    assert!(
        weak_rs.contains(
            "#[unsafe(no_mangle)]\n#[linkage = \"weak\"]\npub extern \"C\" fn fallback_value"
        ),
        "weak function should be exported as a weak C ABI symbol:\n{weak_rs}"
    );
}

#[test]
fn generated_weak_symbols_lose_to_strong_external_definitions() {
    let dir = fixture_dir("weak_override");
    let work = cross_tu_work_dir("weak_override");
    std::fs::create_dir_all(&work).expect("create weak override work dir");

    let weak_rs = work.join("weak.rs");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate")
        .arg(dir.join("weak.c"))
        .output()
        .expect("translate weak.c");
    assert!(
        output.status.success(),
        "translate weak.c failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    std::fs::write(&weak_rs, output.stdout).expect("write weak.rs");

    let libweak = work.join("libweak.a");
    let rustc = std::env::var("RUSTC").unwrap_or_else(|_| "rustc".into());
    let output = std::process::Command::new(rustc)
        .args(["--edition=2024", "--crate-type", "staticlib"])
        .arg(&weak_rs)
        .arg("-o")
        .arg(&libweak)
        .output()
        .expect("compile generated weak staticlib");
    assert!(
        output.status.success(),
        "rustc staticlib failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );

    let bin = work.join("strong_wins");
    let cc = std::env::var("SLATE_CC").unwrap_or_else(|_| "clang".into());
    let output = std::process::Command::new(cc)
        .args(["-O0", "-std=c23"])
        .arg(dir.join("strong_main.c"))
        .args(["-Wl,--whole-archive"])
        .arg(&libweak)
        .args(["-Wl,--no-whole-archive", "-o"])
        .arg(&bin)
        .output()
        .expect("link strong C definitions with generated weak Rust staticlib");
    assert!(
        output.status.success(),
        "C/Rust link failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );

    let output = std::process::Command::new(&bin)
        .output()
        .expect("run strong wins binary");
    assert!(
        output.status.success(),
        "strong wins binary failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    assert_eq!(String::from_utf8_lossy(&output.stdout), "42 91\n");
}

#[test]
fn gnu_symbol_pragmas_preserve_cross_tu_linkage() {
    let rs_dir = build_and_diff("gnu_symbol_pragmas");
    let symbols_rs = std::fs::read_to_string(rs_dir.join("symbols.rs")).expect("read symbols.rs");
    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");

    assert!(
        symbols_rs.contains(".weak pragma_weak_alias\\n.set pragma_weak_alias, pragma_weak_target")
    );
    assert!(symbols_rs.contains("#[unsafe(no_mangle)]\npub extern \"C\" fn pragma_weak_target"));
    assert!(symbols_rs.contains("#[unsafe(no_mangle)]\npub extern \"C\" fn pragma_actual"));
    assert!(main_rs.contains(
        "mod strong;\nmod symbols;\n\nuse crate::strong::pragma_weak_alias;\nuse crate::symbols::pragma_actual;\n\n"
    ));
    assert!(main_rs.contains("use crate::strong::pragma_weak_alias;"));
    assert!(main_rs.contains("use crate::symbols::pragma_actual;"));
    assert!(!main_rs.contains("pragma_renamed"));
}

#[test]
fn function_alias_exports_forwarding_wrapper() {
    let rs_dir = build_and_diff("alias_function");
    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");

    assert!(main_rs.contains("#[unsafe(no_mangle)]\npub extern \"C\" fn alias_impl"));
    assert!(
        main_rs.contains("real_impl(_0)\n}"),
        "alias wrapper should forward to real_impl:\n{main_rs}"
    );
}

#[test]
fn visibility_attrs_lower_best_effort() {
    let rs_dir = build_and_diff("visibility");
    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");

    assert!(main_rs.contains("#[unsafe(no_mangle)]\npub static mut default_global"));
    assert!(main_rs.contains("#[unsafe(no_mangle)]\npub extern \"C\" fn default_fn"));
    assert!(main_rs.contains("#[unsafe(no_mangle)]\npub static mut protected_global"));
    assert!(main_rs.contains("#[unsafe(no_mangle)]\npub extern \"C\" fn protected_fn"));
    assert!(
        main_rs.contains("static mut hidden_global")
            && !main_rs.contains("#[unsafe(no_mangle)]\npub static mut hidden_global")
            && !main_rs.contains("pub static mut hidden_global"),
        "hidden global should remain private:\n{main_rs}"
    );
    assert!(
        main_rs.contains("fn hidden_fn")
            && !main_rs.contains("#[unsafe(no_mangle)]\npub extern \"C\" fn hidden_fn")
            && !main_rs.contains("pub extern \"C\" fn hidden_fn"),
        "hidden function should remain private:\n{main_rs}"
    );

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-project")
        .arg(fixture_dir("visibility"))
        .arg(cross_tu_work_dir("visibility-diagnostics").join("rs"))
        .output()
        .expect("run translate-project visibility fixture");
    assert!(
        output.status.success(),
        "visibility fixture should translate"
    );
    let stderr = String::from_utf8_lossy(&output.stderr);
    assert!(
        stderr.contains("protected")
            && stderr.contains("protected_fn")
            && stderr.contains("protected_global"),
        "expected protected visibility warnings, got:\n{stderr}"
    );
}

#[test]
fn visibility_pragma_stack_controls_rust_exports() {
    let rs_dir = build_and_diff("visibility_pragma");
    let main_rs = std::fs::read_to_string(rs_dir.join("main.rs")).expect("read main.rs");

    for name in ["visible_before", "visible_inner", "visible_after"] {
        assert!(
            main_rs.contains(&format!("#[unsafe(no_mangle)]\npub extern \"C\" fn {name}")),
            "visible symbol `{name}` should remain exported:\n{main_rs}"
        );
    }
    for name in [
        "visible_before_global",
        "visible_inner_global",
        "visible_after_global",
    ] {
        assert!(
            main_rs.contains(&format!("#[unsafe(no_mangle)]\npub static mut {name}")),
            "visible symbol `{name}` should remain exported:\n{main_rs}"
        );
    }
    for name in ["hidden_outer", "hidden_again"] {
        assert!(
            main_rs.contains(&format!("fn {name}"))
                && !main_rs.contains(&format!("pub extern \"C\" fn {name}")),
            "hidden symbol `{name}` should remain private:\n{main_rs}"
        );
    }
    for name in ["hidden_outer_global", "hidden_again_global"] {
        assert!(
            main_rs.contains(&format!("static mut {name}"))
                && !main_rs.contains(&format!("pub static mut {name}")),
            "hidden symbol `{name}` should remain private:\n{main_rs}"
        );
    }
}
