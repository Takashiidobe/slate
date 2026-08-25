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

fn check_project_modules(
    fixture_dir: &Path,
    rust_src_dir: &Path,
    profile: support::filecheck::Profile,
    work_dir: &Path,
) -> Result<(), String> {
    for c_source in c_sources(fixture_dir) {
        let fixture = std::fs::read_to_string(&c_source)
            .map_err(|error| format!("read {}: {error}", c_source.display()))?;
        if !support::filecheck::has_checks(&fixture, profile) {
            continue;
        }
        let stem = c_source
            .file_stem()
            .and_then(|stem| stem.to_str())
            .ok_or_else(|| format!("invalid C fixture name: {}", c_source.display()))?;
        let rust_source = rust_src_dir.join(format!("{stem}.rs"));
        let generated = std::fs::read_to_string(&rust_source)
            .map_err(|error| format!("read {}: {error}", rust_source.display()))?;
        support::filecheck::check_generated_rust(
            &fixture,
            &generated,
            profile,
            &work_dir.join(stem),
        )
        .map_err(|error| {
            format!(
                "{} -> {}: {error}",
                c_source.display(),
                rust_source.display()
            )
        })?;
    }
    Ok(())
}

fn project_filecheck_fixtures(profile: support::filecheck::Profile) -> Vec<String> {
    let root = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.multi");
    let mut fixtures = std::fs::read_dir(&root)
        .expect("read multi-TU fixtures")
        .filter_map(|entry| entry.ok())
        .filter(|entry| entry.path().is_dir())
        .filter(|entry| {
            c_sources(&entry.path()).into_iter().any(|source| {
                std::fs::read_to_string(source)
                    .is_ok_and(|fixture| support::filecheck::has_checks(&fixture, profile))
            })
        })
        .map(|entry| entry.file_name().to_string_lossy().into_owned())
        .collect::<Vec<_>>();
    fixtures.sort();
    fixtures
}

fn library_filecheck_fixtures(profile: support::filecheck::Profile) -> Vec<String> {
    let root = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.library");
    let mut fixtures = std::fs::read_dir(&root)
        .expect("read library fixtures")
        .filter_map(|entry| entry.ok())
        .filter(|entry| entry.path().join("src").is_dir())
        .filter(|entry| {
            c_sources(&entry.path().join("src"))
                .into_iter()
                .any(|source| {
                    std::fs::read_to_string(source)
                        .is_ok_and(|fixture| support::filecheck::has_checks(&fixture, profile))
                })
        })
        .map(|entry| entry.file_name().to_string_lossy().into_owned())
        .collect::<Vec<_>>();
    fixtures.sort();
    fixtures
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
    check_project_modules(
        &dir,
        &rs_dir.join("src"),
        support::filecheck::Profile::active(),
        &work.join("filecheck"),
    )
    .expect("check generated project modules");

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
fn generated_cross_tu_filecheck() {
    for fixture in project_filecheck_fixtures(support::filecheck::Profile::active()) {
        build_and_diff(&fixture);
    }
}

#[test]
fn generated_library_filecheck() {
    let root = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.library");
    let profile = support::filecheck::Profile::active();
    for fixture in library_filecheck_fixtures(profile) {
        let fixture_dir = root.join(&fixture);
        let work = cross_tu_work_dir("generated-library-filecheck").join(&fixture);
        let crate_dir = work.join("crate");
        let _ = std::fs::remove_dir_all(&work);
        let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
            .args(["translate-project", "--lib"])
            .arg(&fixture_dir)
            .arg(&crate_dir)
            .env("SLATE_CLANG_ARGS", "-std=c23")
            .output()
            .expect("translate library FileCheck fixture");
        assert!(
            output.status.success(),
            "library fixture {fixture} failed:\n{}",
            String::from_utf8_lossy(&output.stderr)
        );
        check_project_modules(
            &fixture_dir.join("src"),
            &crate_dir.join("src"),
            profile,
            &work.join("filecheck"),
        )
        .expect("check generated library modules");
    }
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
    assert!(stderr.contains("GCC optimize"));
}

#[test]
fn project_translation_defaults_to_the_active_target() {
    let dir = fixture_dir("project_strtold");
    let out_dir = cross_tu_work_dir("host-only-project").join("rs");
    let _ = std::fs::remove_dir_all(&out_dir);
    support::translate_project(&dir, &out_dir).expect("translate host project");
    check_project_modules(
        &dir,
        &out_dir.join("src"),
        support::filecheck::Profile::Lowering,
        &cross_tu_work_dir("host-only-project").join("filecheck"),
    )
    .expect("check host project modules");
}

#[test]
fn raw_lower_skips_fixups_for_project_translation() {
    let dir = fixture_dir("cross_tu");
    let out_dir = cross_tu_work_dir("raw-lower-project").join("rs");
    let _ = std::fs::remove_dir_all(&out_dir);
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-project")
        .arg(&dir)
        .arg(&out_dir)
        .env("SLATE_RAW_LOWER", "1")
        .env("SLATE_CLANG_ARGS", "-std=c23")
        .output()
        .expect("run raw slate translate-project");
    assert!(
        output.status.success(),
        "raw translate-project failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    check_project_modules(
        &dir,
        &out_dir.join("src"),
        support::filecheck::Profile::Lowering,
        &cross_tu_work_dir("raw-lower-project").join("filecheck"),
    )
    .expect("check raw project modules");
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
fn library_project_shares_anonymous_union_member_across_types_module() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.library")
        .join("anon_union_member");
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/cross-tu")
        .join("library-anon-union-member");
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
}

/// visibility only, not in-crate callability -- slate must not treat a hidden
/// function as an opaque external and strip its "unused" parameter.
#[test]
#[ignore = "rewrite passes disabled while lowering is the focus"]
fn library_project_resolves_hidden_visibility_cross_tu_callback() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.library")
        .join("hidden_visibility_callback");
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/cross-tu")
        .join("hidden-visibility-callback");
    let crate_dir = work.join("crate");
    let _ = std::fs::remove_dir_all(&crate_dir);

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate-project", "--lib"])
        .arg(&dir)
        .arg(&crate_dir)
        .env("SLATE_CLANG_ARGS", "-fvisibility=hidden")
        .output()
        .expect("run slate translate-project --lib");
    assert!(
        output.status.success(),
        "translate-project --lib failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );

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
    assert!(shim_c.contains("int strfroml(char *, size_t, const char *, long double);"));

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
    build_and_diff("cross_tu");
}

#[test]
fn project_translation_escapes_rust_keywords() {
    build_and_diff("keyword_project_idents");
}

#[test]
fn project_translation_exports_nonascii_function_names() {
    build_and_diff("nonascii_function_name");
}

#[test]
fn sibling_can_call_function_defined_in_root_translation_unit() {
    build_and_diff("root_tu_symbol");
}

#[test]
#[ignore = "rewrite passes disabled while lowering is the focus"]
fn setjmp_unwind_abi_propagates_to_cross_module_callers() {
    build_and_diff("setjmp_cross_module_callback");
}

#[test]
fn project_translation_shares_record_types() {
    build_and_diff("shared_record");
}

#[test]
fn same_named_local_structs_in_different_tus_stay_distinct() {
    let rs_dir = build_and_diff("local_struct_cross_file_collision");
    let types_rs = std::fs::read_to_string(rs_dir.join("types.rs")).unwrap_or_default();
    assert!(
        !types_rs.contains("struct Item"),
        "differently-shaped same-named local structs must not be hoisted into a shared \
         types.rs definition: {types_rs}"
    );
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
fn long_double_callback_uses_c_abi_trampoline() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.link")
        .join("long_double_callback");
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/cross-tu")
        .join("long_double_callback");
    let _ = std::fs::remove_dir_all(&work);
    std::fs::create_dir_all(&work).expect("create work dir");

    let main_c = dir.join("main.c");
    let native_c = dir.join("native.c");
    let object = work.join("native.o");
    support::compile_c_object(&native_c, &object).expect("compile callback object");

    let c_bin = work.join("c_bin");
    support::compile_c_with_args(&main_c, &c_bin, &[object.display().to_string()])
        .expect("compile C reference");

    let rs_src = work.join("main.rs");
    let (_, program) = slate::api::lowered_program(&main_c).expect("lower callback main");
    std::fs::write(&rs_src, program.emit()).expect("write callback main");
    let shim_source = slate::frontend::c_shim::render_shim_c_source_for_program(&program);

    let link_dir = work.join("linksrc");
    std::fs::create_dir_all(&link_dir).expect("create link dir");
    std::fs::copy(&object, link_dir.join("native.o")).expect("copy callback object");
    let rs_bin = support::compile_rs_cargo_with_link_and_shims(
        &rs_src,
        &work,
        "long_double_callback",
        &link_dir,
        Some(&shim_source),
    )
    .expect("compile Rust with callback trampoline");

    let run_dir = work.join("run");
    std::fs::create_dir_all(&run_dir).expect("create run dir");
    let c_run = support::run_with_config(&c_bin, &support::RunConfig::default(), &run_dir)
        .expect("run C reference");
    let r_run = support::run_with_config(&rs_bin, &support::RunConfig::default(), &run_dir)
        .expect("run translated callback");
    support::compare_runs(&c_run, &r_run, false).expect("callback outputs differ");
}

#[test]
#[ignore = "rewrite passes disabled while lowering is the focus"]
fn long_double_aggregate_uses_c_abi_shim() {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.link")
        .join("long_double_aggregate");
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/cross-tu")
        .join("long_double_aggregate");
    let _ = std::fs::remove_dir_all(&work);
    std::fs::create_dir_all(&work).expect("create work dir");

    let main_c = dir.join("main.c");
    let native_c = dir.join("native.c");
    let object = work.join("native.o");
    support::compile_c_object(&native_c, &object).expect("compile aggregate object");

    let c_bin = work.join("c_bin");
    support::compile_c_with_args(&main_c, &c_bin, &[object.display().to_string()])
        .expect("compile C reference");

    let rs_src = work.join("main.rs");
    let (_, program) = slate::api::lowered_program(&main_c).expect("lower aggregate main");
    std::fs::write(&rs_src, program.emit()).expect("write aggregate main");
    let shim_source = slate::frontend::c_shim::render_shim_c_source_for_program(&program);

    let link_dir = work.join("linksrc");
    std::fs::create_dir_all(&link_dir).expect("create link dir");
    std::fs::copy(&object, link_dir.join("native.o")).expect("copy aggregate object");
    let rs_bin = support::compile_rs_cargo_with_link_and_shims(
        &rs_src,
        &work,
        "long_double_aggregate",
        &link_dir,
        Some(&shim_source),
    )
    .expect("compile Rust with aggregate shim");

    let run_dir = work.join("run");
    std::fs::create_dir_all(&run_dir).expect("create run dir");
    let c_run = support::run_with_config(&c_bin, &support::RunConfig::default(), &run_dir)
        .expect("run C reference");
    let r_run = support::run_with_config(&rs_bin, &support::RunConfig::default(), &run_dir)
        .expect("run translated aggregate");
    support::compare_runs(&c_run, &r_run, false).expect("aggregate outputs differ");
}

#[test]
fn project_translation_emits_required_long_double_shims() {
    let rs_dir = build_and_diff("project_strtold");
    let shim = std::fs::read_to_string(rs_dir.join("slate_shims.c")).expect("read slate_shims.c");
    assert!(shim.contains("__slate_f80 __slate_strtold"));
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
    build_and_diff("ctype_locale");
}

#[test]
fn public_pointer_deref_functions_are_unsafe() {
    build_and_diff("unsafe_public");
}

#[test]
fn address_taken_pointer_deref_function_stays_unsafe_across_tus() {
    build_and_diff("unsafe_deref_callback_cross_tu");
}

#[test]
fn cross_tu_variadic_calls_are_unsafe() {
    build_and_diff("unsafe_variadic");
}

#[test]
fn cross_tu_static_linkage() {
    build_and_diff("static_linkage");
}

#[test]
fn cross_tu_globals() {
    build_and_diff("globals");
    let work = cross_tu_work_dir("globals");
    assert_binary_sections(&work.join("c_bin"), &[".slate_data", ".slate_fn"]);
    let rs_bin = support::compile_rs_project(&work.join("rs")).expect("compile Rust");
    assert_binary_sections(&rs_bin, &[".slate_data", ".slate_fn"]);
}

#[test]
fn cross_tu_thread_local_globals() {
    build_and_diff("thread_local");
}

#[test]
fn used_and_retain_attrs_preserve_dead_statics() {
    build_and_diff("used_retain");
    let work = cross_tu_work_dir("used_retain");

    let symbols = &["used_only", "used_and_retained"];
    assert_binary_symbols(&work.join("c_bin"), symbols);
    assert_binary_lacks_symbols(&work.join("c_bin"), &["retain_only"]);
    let rs_bin = support::compile_rs_project(&work.join("rs")).expect("compile Rust");
    assert_binary_symbols(&rs_bin, symbols);
    assert_binary_lacks_symbols(&rs_bin, &["retain_only"]);
}

#[test]
fn weak_linkage_attrs_emit_for_globals_and_functions() {
    build_and_diff("weak_linkage");
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
    build_and_diff("gnu_symbol_pragmas");
}

#[test]
#[ignore = "rewrite passes disabled while lowering is the focus"]
fn function_alias_exports_forwarding_wrapper() {
    build_and_diff("alias_function");
}

#[test]
fn visibility_attrs_lower_best_effort() {
    build_and_diff("visibility");

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
    build_and_diff("visibility_pragma");
}
