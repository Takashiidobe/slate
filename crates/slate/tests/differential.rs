//! Differential test harness: compile and run C plus Rust, then require
//! identical stdout and exit code.
//!
//! Compilers are overridable so the CIR-built clang can be swapped in:
//!   SLATE_CC=~/llvm-project/build-cir/bin/clang cargo nextest r --release

mod support;

use std::path::{Path, PathBuf};
use std::process::Command;

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum FixtureFlavor {
    Default,
    Bionic,
    Macos,
    Msvc,
}

impl FixtureFlavor {
    fn name(self) -> &'static str {
        match self {
            FixtureFlavor::Default => "default",
            FixtureFlavor::Bionic => "bionic",
            FixtureFlavor::Macos => "macos",
            FixtureFlavor::Msvc => "msvc",
        }
    }

    fn target(self) -> Option<&'static str> {
        match self {
            FixtureFlavor::Default => None,
            FixtureFlavor::Bionic => Some("aarch64-linux-android21"),
            FixtureFlavor::Macos => Some("arm64-apple-macos11.0"),
            FixtureFlavor::Msvc => Some("x86_64-pc-windows-msvc"),
        }
    }

    fn shim_defines(self) -> &'static [&'static str] {
        match self {
            FixtureFlavor::Default => &[],
            FixtureFlavor::Bionic => &[
                "-D_SLATE_LIBC",
                "-D__SLATE_ARCH_AARCH64",
                "-D__SLATE_VENDOR_UNKNOWN",
                "-D__SLATE_KERNEL_LINUX",
                "-D__SLATE_PLATFORM_ANDROID",
                "-D__SLATE_LIBC_BIONIC",
                "-D__SLATE_OBJ_ELF",
                "-D__SLATE_WORDSIZE_64",
                "-D__SLATE_ENDIAN_LITTLE",
                "-D__SLATE_ANDROID_API__=21",
                "-DEXPECT_AARCH64",
            ],
            FixtureFlavor::Macos => &[
                "-D_SLATE_LIBC",
                "-D__SLATE_ARCH_AARCH64",
                "-D__SLATE_VENDOR_APPLE",
                "-D__SLATE_KERNEL_DARWIN",
                "-D__SLATE_PLATFORM_MACOS",
                "-D__SLATE_LIBC_DARWIN",
                "-D__SLATE_OBJ_MACHO",
                "-D__SLATE_WORDSIZE_64",
                "-D__SLATE_ENDIAN_LITTLE",
            ],
            FixtureFlavor::Msvc => &[
                "-D_SLATE_LIBC",
                "-D__SLATE_ARCH_X86_64",
                "-D__SLATE_VENDOR_PC",
                "-D__SLATE_KERNEL_WINDOWS",
                "-D__SLATE_LIBC_MSVC",
                "-D__SLATE_OBJ_COFF",
                "-D__SLATE_WORDSIZE_64",
                "-D__SLATE_ENDIAN_LITTLE",
            ],
        }
    }
}

struct Fixture {
    name: String,
    path: PathBuf,
    flavor: FixtureFlavor,
}

fn collect_fixtures(
    dir: &Path,
    flavor: FixtureFlavor,
    selected: &Option<String>,
    out: &mut Vec<Fixture>,
) {
    let Ok(entries) = std::fs::read_dir(dir) else {
        return;
    };
    for entry in entries {
        let path = entry.expect("dir entry").path();
        if path.extension().and_then(|e| e.to_str()) != Some("c") {
            continue;
        }
        let name = path.file_stem().unwrap().to_string_lossy().into_owned();
        if selected.as_ref().is_some_and(|selected| selected != &name) {
            continue;
        }
        out.push(Fixture { name, path, flavor });
    }
}

fn fixtures() -> Vec<Fixture> {
    let dir = fixtures_dir();
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut fixtures = Vec::new();
    collect_fixtures(&dir, FixtureFlavor::Default, &selected, &mut fixtures);
    collect_fixtures(
        &dir.join("bionic"),
        FixtureFlavor::Bionic,
        &selected,
        &mut fixtures,
    );
    collect_fixtures(
        &dir.join("macos"),
        FixtureFlavor::Macos,
        &selected,
        &mut fixtures,
    );
    collect_fixtures(
        &dir.join("msvc"),
        FixtureFlavor::Msvc,
        &selected,
        &mut fixtures,
    );
    fixtures.sort_by(|a, b| a.name.cmp(&b.name));
    fixtures
}

fn clang() -> String {
    std::env::var("SLATE_CLANG").unwrap_or_else(|_| {
        std::path::PathBuf::from(std::env::var("HOME").unwrap_or_default())
            .join("llvm-project/build-cir/bin/clang")
            .to_string_lossy()
            .into_owned()
    })
}

fn libc_shim_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/include")
}

fn xwin_sysroot_dirs() -> Option<(PathBuf, PathBuf)> {
    let sysroot = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/msvc-sysroot");
    let crt = sysroot.join("crt/include");
    let ucrt = sysroot.join("sdk/include/ucrt");
    if !crt.is_dir() || !ucrt.is_dir() {
        return None;
    }
    Some((crt, ucrt))
}

fn compile_for_target(
    name: &str,
    target: &str,
    isystem_dirs: &[PathBuf],
    defines: &[&str],
    source: &Path,
) -> Result<(), String> {
    let cache_root = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/test-cache");
    std::fs::create_dir_all(&cache_root).unwrap();
    let object_file = cache_root.join(format!("cross_target_fixture_{name}.o"));
    let mut cmd = Command::new(clang());
    cmd.args(["-xc", "-c", "-nostdlibinc"])
        .arg(format!("--target={target}"))
        .arg("-o")
        .arg(&object_file);
    for dir in isystem_dirs {
        cmd.arg("-isystem").arg(dir);
    }
    cmd.args(defines);
    cmd.arg(source);
    let output = cmd
        .output()
        .map_err(|e| format!("spawn {}: {e}", clang()))?;
    if !output.status.success() {
        return Err(String::from_utf8_lossy(&output.stderr).into_owned());
    }
    Ok(())
}

fn run_cross_target_fixture(
    name: &str,
    flavor: FixtureFlavor,
    target: &str,
    path: &Path,
) -> Result<(), String> {
    let artifact_name = format!("{name}_{}", flavor.name());
    compile_for_target(
        &artifact_name,
        target,
        &[libc_shim_dir()],
        flavor.shim_defines(),
        path,
    )
    .map_err(|e| format!("libc-shim compile failed:\n{e}"))?;
    if flavor == FixtureFlavor::Bionic {
        let defines = [
            "-D_SLATE_LIBC",
            "-D__SLATE_ARCH_X86_64",
            "-D__SLATE_VENDOR_UNKNOWN",
            "-D__SLATE_KERNEL_LINUX",
            "-D__SLATE_PLATFORM_ANDROID",
            "-D__SLATE_LIBC_BIONIC",
            "-D__SLATE_OBJ_ELF",
            "-D__SLATE_WORDSIZE_64",
            "-D__SLATE_ENDIAN_LITTLE",
            "-D__SLATE_ANDROID_API__=21",
            "-DEXPECT_X86_64",
        ];
        compile_for_target(
            &format!("{name}_x86_64"),
            "x86_64-linux-android21",
            &[libc_shim_dir()],
            &defines,
            path,
        )
        .map_err(|e| format!("x86-64 libc-shim compile failed:\n{e}"))?;
        for (arch, target, defines) in [
            ("aarch64", "aarch64-linux-android21", flavor.shim_defines()),
            ("x86_64", "x86_64-linux-android21", defines.as_slice()),
        ] {
            if let Some(ndk) = android_ndk_clang() {
                compile_with_android_ndk(
                    &ndk,
                    &format!("{name}_{arch}_ndk"),
                    target,
                    defines,
                    path,
                )
                .map_err(|e| format!("{arch} NDK oracle compile failed:\n{e}"))?;
            }
        }
    }
    if flavor == FixtureFlavor::Msvc
        && let Some((crt, ucrt)) = xwin_sysroot_dirs()
    {
        compile_for_target(&format!("{name}_xwin"), target, &[crt, ucrt], &[], path)
            .map_err(|e| format!("xwin oracle compile failed:\n{e}"))?;
    }
    if flavor == FixtureFlavor::Macos
        && name == "fundamental_types"
        && let Ok(sdk) = std::env::var("SLATE_MACOS_SDK")
        && !sdk.trim().is_empty()
    {
        let object_file = Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target/test-cache/cross_target_fixture_fundamental_types_macos_sdk.o");
        let output = Command::new(clang())
            .args(["-xc", "-c", "--target=arm64-apple-macos11.0", "-isysroot"])
            .arg(sdk)
            .arg("-o")
            .arg(object_file)
            .arg(path)
            .output()
            .map_err(|e| format!("spawn {}: {e}", clang()))?;
        if !output.status.success() {
            return Err(format!(
                "macOS SDK oracle compile failed:\n{}",
                String::from_utf8_lossy(&output.stderr)
            ));
        }
    }
    Ok(())
}

fn cargo_check_generated_for_target(name: &str, rust: &str, target: &str) -> Result<(), String> {
    let libdir = Command::new("rustc")
        .args(["--print", "target-libdir", "--target", target])
        .output()
        .map_err(|e| format!("spawn rustc: {e}"))?;
    let libdir = PathBuf::from(String::from_utf8_lossy(&libdir.stdout).trim());
    if !libdir.is_dir() {
        eprintln!("skipping {target} cargo check: Rust target is not installed");
        return Ok(());
    }
    let project = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/difftest-cross-check")
        .join(name);
    support::write_if_changed(
        project.join("Cargo.toml"),
        format!(
            "[package]\nname = \"{name}\"\nversion = \"0.0.0\"\nedition = \"2024\"\n\n[dependencies]\nlibc = \"0.2\"\n"
        )
        .as_bytes(),
    )
    .map_err(|e| format!("write cross-check manifest: {e}"))?;
    support::write_if_changed(project.join("src/main.rs"), rust.as_bytes())
        .map_err(|e| format!("write cross-check source: {e}"))?;
    let output = Command::new("cargo")
        .args(["check", "--quiet", "--manifest-path"])
        .arg(project.join("Cargo.toml"))
        .args(["--target", target, "--target-dir"])
        .arg(project.join("target"))
        .output()
        .map_err(|e| format!("spawn cargo check: {e}"))?;
    if output.status.success() {
        Ok(())
    } else {
        Err(format!(
            "generated Rust cargo check failed for {target}:\n{}",
            String::from_utf8_lossy(&output.stderr)
        ))
    }
}

fn android_ndk_clang() -> Option<PathBuf> {
    let path = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/android-ndk-oracle/ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/clang");
    path.is_file().then_some(path)
}

fn compile_with_android_ndk(
    clang: &Path,
    name: &str,
    target: &str,
    defines: &[&str],
    source: &Path,
) -> Result<(), String> {
    let cache_root = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/test-cache");
    std::fs::create_dir_all(&cache_root).unwrap();
    let object_file = cache_root.join(format!("cross_target_fixture_{name}.o"));
    let output = Command::new(clang)
        .args(["-xc", "-c"])
        .arg(format!("--target={target}"))
        .arg("-o")
        .arg(object_file)
        .args(defines)
        .arg(source)
        .output()
        .map_err(|e| format!("spawn {}: {e}", clang.display()))?;
    if !output.status.success() {
        return Err(String::from_utf8_lossy(&output.stderr).into_owned());
    }
    Ok(())
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

    let mut failures = Vec::new();

    let default_fixtures: Vec<&Fixture> = fixtures
        .iter()
        .filter(|f| f.flavor == FixtureFlavor::Default)
        .collect();
    let translated = support::parallel_map(&default_fixtures, |f| {
        let generated = tmp.join(format!("{}.generated.rs", f.name));
        support::translate(&f.path, &generated).map(|()| support::Case {
            name: f.name.clone(),
            c_src: f.path.clone(),
            rs_src: generated,
            config: support::RunConfig::default(),
        })
    });
    let mut cases = Vec::new();
    for (f, result) in default_fixtures.iter().zip(translated) {
        match result {
            Ok(case) => cases.push(case),
            Err(e) => {
                eprintln!("FAIL  {}", f.name);
                failures.push(format!("[{}] {e}", f.name));
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

    let cross_target_fixtures: Vec<&Fixture> = fixtures
        .iter()
        .filter(|f| f.flavor != FixtureFlavor::Default)
        .collect();
    let cross_target_results = support::parallel_map(&cross_target_fixtures, |f| {
        run_cross_target_fixture(&f.name, f.flavor, f.flavor.target().unwrap(), &f.path)
    });
    for (f, result) in cross_target_fixtures.iter().zip(cross_target_results) {
        match result {
            Ok(()) => eprintln!("ok    {} ({:?})", f.name, f.flavor),
            Err(e) => {
                eprintln!("FAIL  {} ({:?})", f.name, f.flavor);
                failures.push(format!("[{} ({:?})] {e}", f.name, f.flavor));
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
fn failed_batch_rebuild_does_not_accept_stale_binary() {
    let work = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-stale-bin-regression");
    let _ = std::fs::remove_dir_all(&work);
    std::fs::create_dir_all(&work).expect("create regression work dir");
    let generated = work.join("stale_bin.generated.rs");
    std::fs::write(&generated, "fn main() {}\n").expect("write valid generated Rust");
    let cases = [support::RustCase {
        name: "stale_bin".into(),
        rs_src: generated.clone(),
    }];
    let first = support::compile_rs_batch(&cases, &work);
    assert!(first[0].1.is_ok(), "initial build failed: {:?}", first[0].1);
    let fresh = support::compile_rs_batch(&cases, &work);
    assert!(fresh[0].1.is_ok(), "fresh build failed: {:?}", fresh[0].1);

    std::fs::write(&generated, "fn main( {}\n").expect("write invalid generated Rust");
    let second = support::compile_rs_batch(&cases, &work);
    assert!(
        second[0].1.is_err(),
        "failed rebuild accepted the stale executable"
    );
}

#[test]
fn failed_multi_bin_rebuild_does_not_accept_stale_binary() {
    let work =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-stale-multi-bin-regression");
    let _ = std::fs::remove_dir_all(&work);
    std::fs::create_dir_all(&work).expect("create regression work dir");
    let main_rs = work.join("main.generated.rs");
    let common_rs = work.join("common.generated.rs");
    std::fs::write(&main_rs, "mod common; fn main() { common::run(); }\n")
        .expect("write valid generated main");
    std::fs::write(&common_rs, "pub fn run() {}\n").expect("write valid generated module");
    let cases = [support::MultiBinCase {
        name: "stale_multi_bin".into(),
        main_rs,
        common_rs: common_rs.clone(),
        types_rs: None,
    }];
    let project = work.join("batch_cargo");
    let first = support::build_multi_bin_batch(&cases, &project).expect("initial multi-bin build");
    first
        .executable("stale_multi_bin")
        .expect("initial multi-bin artifact");

    std::fs::write(&common_rs, "pub fn run( {}\n").expect("write invalid generated module");
    let second =
        support::build_multi_bin_batch(&cases, &project).expect("failed multi-bin Cargo build");
    assert!(
        second.executable("stale_multi_bin").is_err(),
        "failed multi-bin rebuild accepted the stale executable"
    );
}

#[test]
fn c_oracle_binary_is_reused_until_its_inputs_change() {
    let work = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-c-cache-regression");
    let _ = std::fs::remove_dir_all(&work);
    std::fs::create_dir_all(&work).expect("create regression work dir");
    let source = work.join("oracle.c");
    let binary = work.join("oracle");
    std::fs::write(&source, "int main(void) { return VALUE; }\n").expect("write C oracle source");
    let zero = vec!["-DVALUE=0".to_string()];
    support::compile_c_with_args(&source, &binary, &zero).expect("initial C build");
    let initial = std::fs::metadata(&binary)
        .expect("stat initial C binary")
        .modified()
        .expect("initial C binary mtime");

    std::thread::sleep(std::time::Duration::from_millis(20));
    support::compile_c_with_args(&source, &binary, &zero).expect("cached C build");
    let cached = std::fs::metadata(&binary)
        .expect("stat cached C binary")
        .modified()
        .expect("cached C binary mtime");
    assert_eq!(initial, cached, "unchanged C oracle was recompiled");

    std::thread::sleep(std::time::Duration::from_millis(20));
    let one = vec!["-DVALUE=1".to_string()];
    support::compile_c_with_args(&source, &binary, &one).expect("C rebuild after argument change");
    let rebuilt = std::fs::metadata(&binary)
        .expect("stat rebuilt C binary")
        .modified()
        .expect("rebuilt C binary mtime");
    assert!(rebuilt > cached, "changed C arguments did not rebuild");

    std::thread::sleep(std::time::Duration::from_millis(20));
    std::fs::write(&source, "int main(void) {\n").expect("write invalid C oracle source");
    assert!(
        support::compile_c_with_args(&source, &binary, &one).is_err(),
        "failed C rebuild accepted the stale executable"
    );
}

#[test]
fn explicit_targets_define_slate_feature_macros() {
    let fixture = fixtures_dir().join("target_feature_macros.c");
    for (target, expected) in [
        ("x86_64-unknown-linux-gnu", "EXPECT_LINUX_GLIBC_X86_64"),
        ("aarch64-unknown-linux-musl", "EXPECT_LINUX_MUSL_AARCH64"),
        ("riscv64-unknown-linux-gnu", "EXPECT_LINUX_GLIBC_RISCV64"),
        ("aarch64-apple-darwin", "EXPECT_MACOS_DARWIN_AARCH64"),
    ] {
        let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
            .args(["translate", &format!("-D{expected}=1")])
            .arg(&fixture)
            .env("SLATE_TARGET", target)
            .output()
            .expect("run Slate for explicit target");
        assert!(
            output.status.success(),
            "target {target} failed:\n{}",
            String::from_utf8_lossy(&output.stderr)
        );
    }
}

#[test]
fn macos_fundamental_fixture_translates_with_darwin_abi_types() {
    let fixture = fixtures_dir().join("macos/fundamental_types.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate", fixture.to_str().unwrap()])
        .env("SLATE_TARGET", "aarch64-apple-darwin")
        .output()
        .expect("translate macOS fundamental fixture");
    assert!(
        output.status.success(),
        "macOS translation failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let rust = String::from_utf8(output.stdout).expect("generated Rust is UTF-8");
    assert!(rust.contains("fn darwin_import("));
    assert!(rust.contains("_0: usize"));
    assert!(rust.contains("_1: isize"));
    assert!(rust.contains("_2: isize"));
    assert!(rust.contains("_3: usize"));
    assert!(rust.contains("_4: i32"));
    assert!(rust.contains("_5: i64"));
    assert!(rust.contains("_6: u64"));
    assert!(rust.contains("_7: f64"));
    assert!(!rust.contains("f128"));
    assert!(rust.contains("next_arg::<i32>()"));
    cargo_check_generated_for_target("slate_macos_fundamental", &rust, "aarch64-apple-darwin")
        .unwrap();
}

#[test]
fn macos_long_double_layout_fixture_translates_with_f64_abi_layout() {
    let fixture = fixtures_dir().join("macos/long_double_layout.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate", fixture.to_str().unwrap()])
        .env("SLATE_TARGET", "aarch64-apple-darwin")
        .output()
        .expect("translate macOS long double layout fixture");
    assert!(
        output.status.success(),
        "macOS long double layout translation failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let rust = String::from_utf8(output.stdout).expect("generated Rust is UTF-8");
    assert!(!rust.contains("LongDouble"));
    assert!(rust.contains("value: f64"));
    assert!(rust.contains("ld: f64"));
    assert!(rust.contains("let _v1: u64 = 8;"));
    assert!(rust.contains("let _v2: u64 = 8;"));
    assert!(rust.contains("std::mem::offset_of!(ld_box, value) as u64"));
    cargo_check_generated_for_target(
        "slate_macos_long_double_layout",
        &rust,
        "aarch64-apple-darwin",
    )
    .unwrap();
}

#[test]
fn macos_long_double_macro_constant_lowers_to_f64_bits() {
    let fixture = fixtures_dir().join("macos/long_double_macro_constant.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate", fixture.to_str().unwrap()])
        .env("SLATE_TARGET", "aarch64-apple-darwin")
        .output()
        .expect("translate macOS long double macro constant fixture");
    assert!(
        output.status.success(),
        "macOS long double macro constant translation failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let rust = String::from_utf8(output.stdout).expect("generated Rust is UTF-8");
    assert!(!rust.contains("LongDouble"));
    assert!(rust.contains("f64::from_bits"));
    cargo_check_generated_for_target(
        "slate_macos_long_double_macro_constant",
        &rust,
        "aarch64-apple-darwin",
    )
    .unwrap();
}

#[test]
fn android_targets_require_an_api_level() {
    let fixture = fixtures_dir().join("bionic/target_features.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate", fixture.to_str().unwrap()])
        .env("SLATE_TARGET", "aarch64-linux-android")
        .env_remove("SLATE_ANDROID_API")
        .output()
        .expect("translate Android fixture without API level");
    assert!(!output.status.success());
    assert!(String::from_utf8_lossy(&output.stderr).contains("SLATE_ANDROID_API"));
}

#[test]
fn bionic_long_double_macro_constant_lowers_to_f128_bits() {
    let fixture = fixtures_dir().join("bionic/long_double_macro_constant.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate", fixture.to_str().unwrap()])
        .env("SLATE_TARGET", "x86_64-linux-android")
        .env("SLATE_ANDROID_API", "21")
        .output()
        .expect("translate Bionic long double macro constant fixture");
    assert!(
        output.status.success(),
        "Bionic long double macro constant translation failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let rust = String::from_utf8(output.stdout).expect("generated Rust is UTF-8");
    assert!(!rust.contains("struct LongDouble"));
    assert!(rust.contains("#![feature(f128)]"));
    assert!(rust.contains("f128::from_bits"));
}

#[test]
fn android_targets_reject_invalid_api_levels() {
    let fixture = fixtures_dir().join("bionic/target_features.c");
    for api in ["twenty-one", "20"] {
        let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
            .args(["translate", fixture.to_str().unwrap()])
            .env("SLATE_TARGET", "aarch64-linux-android")
            .env("SLATE_ANDROID_API", api)
            .output()
            .expect("translate Android fixture with invalid API level");
        assert!(!output.status.success());
        assert!(String::from_utf8_lossy(&output.stderr).contains("SLATE_ANDROID_API"));
    }
}

#[test]
fn android_targets_translate_for_both_64_bit_architectures() {
    let fixture = fixtures_dir().join("bionic/target_features.c");
    for (target, expected) in [
        ("aarch64-linux-android", "EXPECT_AARCH64"),
        ("x86_64-linux-android", "EXPECT_X86_64"),
    ] {
        let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
            .args([
                "translate",
                &format!("-D{expected}"),
                fixture.to_str().unwrap(),
            ])
            .env("SLATE_TARGET", target)
            .env("SLATE_ANDROID_API", "21")
            .output()
            .expect("translate Android target fixture");
        assert!(
            output.status.success(),
            "target {target} failed:\n{}",
            String::from_utf8_lossy(&output.stderr)
        );
    }
}

#[test]
fn android_fundamental_fixture_translates_with_architecture_abi_types() {
    let fixture = fixtures_dir().join("bionic/fundamental_types.c");
    for (target, wchar_type) in [
        ("aarch64-linux-android", "_4: u32"),
        ("x86_64-linux-android", "_4: i32"),
    ] {
        let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
            .args(["translate", fixture.to_str().unwrap()])
            .env("SLATE_TARGET", target)
            .env("SLATE_ANDROID_API", "21")
            .output()
            .expect("translate Android fundamental fixture");
        assert!(
            output.status.success(),
            "target {target} failed:\n{}",
            String::from_utf8_lossy(&output.stderr)
        );
        let rust = String::from_utf8(output.stdout).expect("generated Rust is UTF-8");
        assert!(rust.contains("fn bionic_import("));
        assert!(rust.contains("_0: usize"));
        assert!(rust.contains("_1: isize"));
        assert!(rust.contains("_2: isize"));
        assert!(rust.contains("_3: usize"));
        assert!(rust.contains(wchar_type));
        assert!(rust.contains("_5: u32"));
        assert!(rust.contains("#![feature(f128)]"));
        assert!(rust.contains("_6: f128"));
        assert!(!rust.contains("struct LongDouble"));
        assert!(rust.contains("next_arg::<i32>()"));
    }
}

#[test]
fn msvc_llp64_fixture_translates_with_target_abi_types() {
    let fixture = fixtures_dir().join("msvc/llp64.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate", fixture.to_str().unwrap()])
        .env("SLATE_TARGET", "x86_64-pc-windows-msvc")
        .output()
        .expect("translate MSVC LLP64 fixture");
    assert!(
        output.status.success(),
        "MSVC translation failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let rust = String::from_utf8(output.stdout).expect("generated Rust is UTF-8");
    assert!(rust.contains("fn imported_msvc("));
    assert!(rust.contains("_0: usize"));
    assert!(rust.contains("_1: isize"));
    assert!(rust.contains("_2: isize"));
    assert!(rust.contains("_3: usize"));
    assert!(rust.contains("_4: u16"));
    assert!(rust.contains("_5: i32"));
    assert!(rust.contains("_6: i64"));
    assert!(rust.contains("_7: f64"));
    assert!(rust.contains(") -> i64;"));
    assert!(!rust.contains("pthread"));
    assert!(!rust.contains("ioctl"));
    assert!(!rust.contains("socket"));
    assert!(!rust.contains("fork"));
}

#[test]
fn msvc_long_double_macro_constant_lowers_to_f64_bits() {
    let fixture = fixtures_dir().join("msvc/long_double_macro_constant.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate", fixture.to_str().unwrap()])
        .env("SLATE_TARGET", "x86_64-pc-windows-msvc")
        .output()
        .expect("translate MSVC long double macro constant fixture");
    assert!(
        output.status.success(),
        "MSVC long double macro constant translation failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let rust = String::from_utf8(output.stdout).expect("generated Rust is UTF-8");
    assert!(!rust.contains("LongDouble"));
    assert!(rust.contains("f64::from_bits"));
    cargo_check_generated_for_target(
        "slate_msvc_long_double_macro_constant",
        &rust,
        "x86_64-pc-windows-msvc",
    )
    .unwrap();
}

#[test]
fn function_alias_lowers_to_forwarding_wrapper() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-alias-function");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("alias_function.c");
    let generated = tmp.join("alias_function.generated.rs");

    support::translate(&c_src, &generated).expect("translate alias function fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated alias function rust");

    assert!(rust.contains("fn alias_impl(_0: i32) -> i32"));
    assert!(
        rust.contains("real_impl(_0)\n}"),
        "alias wrapper should forward to real_impl:\n{rust}"
    );
}

#[test]
fn weakref_function_calls_preserve_local_and_external_targets() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-weakref-function");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("weakref_function.c");
    let generated = tmp.join("weakref_function.generated.rs");

    support::translate(&c_src, &generated).expect("translate weakref function fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated weakref function rust");

    assert!(!rust.contains("fn weakref_alias"));
    assert!(rust.contains("weakref_target(35)"));
    assert!(rust.contains("#[linkage = \"extern_weak\"]"));
    assert!(rust.contains("static abs: Option<unsafe extern \"C\" fn(i32) -> i32>;"));
    assert!(rust.contains("abs.unwrap()(-53 as i32)"));
}

#[test]
fn gnu_symbol_pragmas_preserve_alias_and_external_names() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-gnu-symbol-pragmas");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("gnu_symbol_pragmas.c");
    let generated = tmp.join("gnu_symbol_pragmas.generated.rs");

    support::translate(&c_src, &generated).expect("translate GNU symbol pragmas fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated GNU symbol pragma Rust");

    assert!(rust.contains(".weak pragma_weak_alias\\n.set pragma_weak_alias, pragma_weak_target"));
    assert!(rust.contains("fn pragma_weak_alias("));
    assert!(rust.contains("fn pragma_actual("));
    assert!(!rust.contains("fn pragma_renamed("));
}

#[test]
fn gnu_macro_pragmas_are_consumed_by_clang() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-gnu-macro-pragmas");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("gnu_macro_pragmas.c");
    let generated = tmp.join("gnu_macro_pragmas.generated.rs");

    support::translate(&c_src, &generated).expect("translate GNU macro pragmas fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated GNU macro pragma Rust");

    assert!(rust.contains("static mut macro_inner: i32 = 11;"));
    assert!(rust.contains("static mut macro_outer: i32 = 7;"));
    assert!(!rust.contains("poisoned_but_unused"));
}

#[test]
fn global_alias_emits_unsupported_diagnostic() {
    let c_src = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.unsupported")
        .join("alias_global.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate")
        .arg(c_src)
        .output()
        .expect("run slate translate on global alias");

    assert!(
        !output.status.success(),
        "global alias translation should fail until Slate has a faithful representation"
    );
    let stderr = String::from_utf8_lossy(&output.stderr);
    assert!(
        stderr.contains("global alias") && stderr.contains("alias_global"),
        "expected global alias diagnostic, got:\n{stderr}"
    );
}

#[test]
fn gnu_basic_asm_preserves_target_scope_and_symbols() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-gnu-basic-asm");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("gnu_basic_asm.c");
    let generated = tmp.join("gnu_basic_asm.generated.rs");

    support::translate(&c_src, &generated).expect("translate gnu_basic_asm fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated gnu_basic_asm rust");

    assert!(rust.contains("#[cfg(target_arch = \"x86_64\")]\ncore::arch::global_asm!("));
    assert!(rust.contains("#[unsafe(no_mangle)]\nstatic mut gnu_basic_asm_value: i32"));
    assert!(rust.contains(
        "core::arch::asm!(\"movl $23, gnu_basic_asm_value(%rip)\", options(att_syntax, raw))"
    ));
}

#[test]
fn anonymous_local_structs_use_generated_tuple_structs() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-anon-local-struct");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("anon_local_struct.c");
    let generated = tmp.join("anon_local_struct.generated.rs");

    support::translate(&c_src, &generated).expect("translate anon_local_struct fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated anon_local_struct rust");

    assert!(rust.contains("struct __slate_anonymous_struct_0(i32, i32);"));
    assert!(
        rust.contains("let point: __slate_anonymous_struct_0 = __slate_anonymous_struct_0(3, 4);")
    );
    assert!(rust.contains("point.0"));
    assert!(rust.contains("point.1"));
    assert!(!rust.contains("struct anon_0"));
    assert!(!rust.contains("anon_0 { x:"));
    assert!(!rust.contains("point.x"));
    assert!(!rust.contains("point.y"));

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("fixup-debug")
        .arg("--debug-only-pass")
        .arg("anonymous_structs")
        .arg(c_src)
        .output()
        .expect("run anonymous_structs query trace");
    assert!(
        output.status.success(),
        "fixup-debug failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let stdout = String::from_utf8(output.stdout).expect("debug output is utf8");
    assert!(stdout.contains("rewrite_anonymous_structs"));
    assert!(stdout.contains("query_case=complete_domain"));
    assert!(
        stdout.contains(
            "evidence.anonymous_struct_domain=records=1;facts=1;conflicts=0;complete=true"
        )
    );
}

#[test]
fn anonymous_struct_arrays_use_generated_tuple_structs() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-anon-struct-array");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("anon_struct_array.c");
    let generated = tmp.join("anon_struct_array.generated.rs");

    support::translate(&c_src, &generated).expect("translate anonymous struct array fixture");
    let rust = std::fs::read_to_string(&generated).expect("read anonymous struct array rust");

    assert!(rust.contains("struct __slate_anonymous_struct_0(i32, *mut i8, f64);"));
    assert!(
        rust.contains("error_log: aligned::Aligned<aligned::A16, [__slate_anonymous_struct_0; 3]>")
    );
    assert!(rust.contains("__slate_anonymous_struct_0(404,"));
    assert!(rust.contains("error_log[0].0"));
    assert!(!rust.contains("anon_0"));
    assert!(!rust.contains("error_log[0].code"));
}

#[test]
fn anonymous_struct_name_collisions_preserve_the_baseline_ast() {
    let tmp =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-anon-struct-name-collision");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("anon_struct_name_collision.c");
    let generated = tmp.join("anon_struct_name_collision.generated.rs");

    support::translate(&c_src, &generated).expect("translate anonymous struct collision fixture");
    let rust = std::fs::read_to_string(&generated).expect("read anonymous struct collision Rust");
    assert!(rust.contains("struct __slate_anonymous_struct_0 {"));
    assert!(rust.contains("struct anon_0 {"));
    assert!(rust.contains("let point: anon_0 = anon_0 { x: 3, y: 4 };"));

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("fixup-debug")
        .arg("--debug-only-pass")
        .arg("anonymous_structs")
        .arg(c_src)
        .output()
        .expect("run rejected anonymous_structs query trace");
    assert!(
        output.status.success(),
        "fixup-debug failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let stdout = String::from_utf8(output.stdout).expect("debug output is utf8");
    assert!(
        stdout.contains("rejected_case.complete_domain=anonymous_struct_domain:incomplete_domain")
    );
    assert!(
        stdout.contains(
            "evidence.anonymous_struct_domain=records=1;facts=1;conflicts=1;complete=false"
        )
    );
}

#[test]
fn anonymous_members_emit_nested_repr_c_storage() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-anonymous-members");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("anonymous_members.c");
    let generated = tmp.join("anonymous_members.generated.rs");

    support::translate(&c_src, &generated).expect("translate anonymous members fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated anonymous members rust");
    assert!(rust.contains("union anon_0 {\n    integer: i32,\n    real: f32,"));
    assert!(rust.contains(
        "struct container {\n    prefix: i32,\n    __slate_anon_1: anon_0,\n    __slate_anon_2: __slate_anonymous_struct_0,"
    ));
    assert!(rust.contains("value.__slate_anon_1.integer"));
    assert!(rust.contains("value.__slate_anon_2.0"));
    assert!(rust.contains("value.__slate_anon_2.1"));
}

#[test]
fn buffer_pointer_cursor_uses_safe_array_indexes() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-buffer-cursor");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("anon_local_struct.c");
    let generated = tmp.join("anon_local_struct.generated.rs");

    support::translate(&c_src, &generated).expect("translate anon_local_struct fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated anon_local_struct rust");

    assert!(rust.contains("storage[0] ="));
    assert!(rust.contains("storage[1] ="));
    assert!(rust.contains("println!(\"{}\", 2 as i64);"));
    assert!(rust.contains("println!(\"{}\", 4 as i64);"));
    assert!(!rust.contains("let buf:"));
    assert!(!rust.contains("buf.pointer"));
    assert!(!rust.contains("buf.start"));
    assert!(!rust.contains("buf.end"));
    assert!(!rust.contains("offset_from"));
    assert!(!rust.contains("unsafe {\n        *"));
}

#[test]
fn atomic_temp_allocas_forward_instead_of_shadowed_locals() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-atomic-temps");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("atomics.c");
    let generated = tmp.join("atomics.generated.rs");

    support::translate(&c_src, &generated).expect("translate atomics fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated atomics rust");

    assert!(!rust.contains("_atomictmp"));
    assert!(!rust.contains("atomic_temp"));
    assert!(!rust.contains("cmpxchg_bool"));
    assert!(rust.contains(".store(100, std::sync::atomic::Ordering::SeqCst)"));
    assert!(
        rust.contains("let loaded: i32 = a.load(std::sync::atomic::Ordering::SeqCst);"),
        "{rust}"
    );
    assert!(
        rust.contains("let fa: i32 = a.fetch_add(5, std::sync::atomic::Ordering::SeqCst);"),
        "{rust}"
    );
    assert!(
        rust.contains("let xchg_old: i32 = a.swap(7, std::sync::atomic::Ordering::SeqCst);"),
        "{rust}"
    );
    assert!(rust.contains(".fetch_add(5, std::sync::atomic::Ordering::SeqCst)"));
    assert!(rust.contains(".fetch_sub(10, std::sync::atomic::Ordering::SeqCst)"));
    assert!(rust.contains(".swap(7, std::sync::atomic::Ordering::SeqCst)"));
    assert!(rust.contains(
        "let ok: i32 = (match a.compare_exchange(expected, 42, std::sync::atomic::Ordering::SeqCst, std::sync::atomic::Ordering::SeqCst)"
    ));
    assert!(rust.contains(
        "let bad: i32 = (match a.compare_exchange(expected2, 0, std::sync::atomic::Ordering::SeqCst, std::sync::atomic::Ordering::SeqCst)"
    ));
    assert!(rust.contains("Ok(_) => true"));
    assert!(rust.contains("expected = v;"));
    assert!(rust.contains("expected2 = v;"));
    for alias in [
        "let _v30: i32 = loaded;",
        "let _v31: i32 = fa;",
        "let _v32: i32 = fs;",
        "let _v33: i32 = fand;",
        "let _v34: i32 = forr;",
        "let _v35: i32 = fxor;",
        "let _v36: i32 = xchg_old;",
        "let _v37: i32 = ok;",
        "let _v38: i32 = expected;",
        "let _v39: i32 = bad;",
        "let _v40: i32 = expected2;",
    ] {
        assert!(!rust.contains(alias), "{alias} survived in:\n{rust}");
    }
    assert!(
        rust.contains(
            "println!(\"{} {} {} {} {} {} {} {} {} {} {} {}\", loaded, fa, fs, fand, forr, fxor, xchg_old, ok, expected, bad, expected2, _v41);"
        ),
        "{rust}"
    );
    assert!(!rust.contains(".is_ok()"));
    assert!(!rust.contains("enum memory_order"));
    assert!(!rust.contains("struct atomic_flag"));
}

#[test]
fn non_escaping_atomic_local_gets_native_atomic_storage() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-atomic-locals");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("atomics.c");
    let generated = tmp.join("atomics.generated.rs");

    support::translate(&c_src, &generated).expect("translate atomics fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated atomics rust");

    assert!(rust.contains("let a = std::sync::atomic::AtomicI32::new(0);"));
    assert!(rust.contains("a.store(100, std::sync::atomic::Ordering::SeqCst);"));
    assert!(rust.contains("a.fetch_add(5, std::sync::atomic::Ordering::SeqCst)"));
    assert!(rust.contains("a.compare_exchange(expected, 42, "));
    assert!(!rust.contains("from_ptr"));
    assert!(!rust.contains("unsafe"));
    assert!(!rust.contains("let mut a"));
}

#[test]
fn expanded_atomic_fixtures_cover_orderings_flag_fallbacks_and_widths() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-atomic-expanded");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let explicit = translate_fixture(&tmp, "atomic_explicit_orderings");
    assert!(explicit.contains(".store(10, std::sync::atomic::Ordering::Relaxed)"));
    assert!(explicit.contains(".store(20, std::sync::atomic::Ordering::Release)"));
    assert!(explicit.contains(".load(std::sync::atomic::Ordering::Acquire)"));
    assert!(explicit.contains(".fetch_add(2, std::sync::atomic::Ordering::AcqRel)"));
    assert!(explicit.contains(".swap(5, std::sync::atomic::Ordering::Acquire)"));
    assert!(explicit.contains("std::sync::atomic::fence(std::sync::atomic::Ordering::Release);"));
    assert!(explicit.contains("std::sync::atomic::fence(std::sync::atomic::Ordering::Acquire);"));
    assert!(!explicit.contains("std::sync::atomic::fence(std::sync::atomic::Ordering::Relaxed);"));

    let flag = translate_fixture(&tmp, "atomic_flag_ops");
    assert!(flag.contains("struct atomic_flag"));
    assert!(flag.contains("std::sync::atomic::AtomicU8::from_ptr"));
    assert!(flag.contains(".swap(_v8, std::sync::atomic::Ordering::Acquire)"));
    assert!(flag.contains(".store(_v6, std::sync::atomic::Ordering::Release)"));

    let fallbacks = translate_fixture(&tmp, "atomic_fallbacks");
    assert!(!fallbacks.contains("AtomicF32"));
    assert!(fallbacks.contains("*std::ptr::addr_of_mut!(f) = _v3 + _v2;"));
    assert!(fallbacks.contains("let _v14: *mut i32 = unsafe { *std::ptr::addr_of_mut!(p) };"));
    assert!(fallbacks.contains("*std::ptr::addr_of_mut!(p) = _v13;"));

    let widths = translate_fixture(&tmp, "atomic_widths");
    assert!(widths.contains("std::sync::atomic::AtomicU8::new(250)"));
    assert!(widths.contains("std::sync::atomic::AtomicI8::new(-5)"));
    assert!(widths.contains("std::sync::atomic::AtomicU32::new(1000)"));
    assert!(widths.contains("std::sync::atomic::AtomicI64::new(-10000000000i64)"));
}

fn translate_fixture(tmp: &Path, fixture: &str) -> String {
    let c_src = fixtures_dir().join(format!("{fixture}.c"));
    let generated = tmp.join(format!("{fixture}.generated.rs"));
    support::translate(&c_src, &generated)
        .unwrap_or_else(|err| panic!("translate {fixture}: {err}"));
    std::fs::read_to_string(&generated).unwrap_or_else(|err| panic!("read {fixture}: {err}"))
}

#[test]
fn dense_temp_chain_translates_within_a_bounded_time() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-dense-temp-chain");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let start = std::time::Instant::now();
    translate_fixture(&tmp, "dense_temp_chain_completes_in_bounded_time");
    let elapsed = start.elapsed();
    assert!(
        elapsed < std::time::Duration::from_secs(20),
        "translation took {elapsed:?}, expected a couple of seconds; \
         fixups/query fact lookups likely regressed to O(n^2) or worse \
         (see slate-xzn6.4)"
    );
}

#[test]
fn serial_temp_chain_translates_within_a_bounded_time() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-serial-temp-chain");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let start = std::time::Instant::now();
    translate_fixture(&tmp, "serial_temp_chain_completes_in_bounded_time");
    let elapsed = start.elapsed();
    assert!(
        elapsed < std::time::Duration::from_secs(5),
        "translation took {elapsed:?}, expected under five seconds; \
         the fixups query engine's overlap checks and per-function fact \
         lookups likely regressed to O(n^2) or worse (see slate-xzn6.6)"
    );
}

#[test]
fn gnu_empty_struct_emits_zero_sized_rust_type() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-gnu-empty-struct");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let rust = translate_fixture(&tmp, "gnu_empty_struct");
    assert!(rust.contains("#[repr(C)]\n#[derive(Clone, Copy)]\nstruct GNUEmpty {\n}"));
    assert!(rust.contains("fn empty_size(value: GNUEmpty) -> u64"));
    assert!(rust.contains("let mut value: GNUEmpty = GNUEmpty {  };"));
}

#[test]
fn pointer_comparisons_preserve_address_operands() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-pointer-compare-address");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let rust = translate_fixture(&tmp, "pointer_compare_address");
    assert!(rust.contains("_v2 == std::ptr::addr_of_mut!(utc)"));
    assert!(rust.contains("_v4 == std::ptr::addr_of_mut!(local)"));
    assert!(!rust.contains("_v2 == utc"));
    assert!(!rust.contains("_v4 == local"));
}

#[test]
fn timespec_get_uses_libc_timespec_abi() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-timespec-get");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let rust = translate_fixture(&tmp, "timespec_get");
    assert!(rust.contains("fn timespec_get(_0: *mut libc::timespec, _1: i32) -> i32;"));
    assert!(
        rust.contains("let mut value: libc::timespec = libc::timespec { tv_sec: 0, tv_nsec: 0 };")
    );
    assert!(rust.contains("value.tv_nsec >= 0"));
    assert!(rust.contains("value.tv_nsec < 1000000000"));
    assert!(!rust.contains("*mut timespec"));
}

#[test]
fn uchar_conversions_use_libc_mbstate_abi() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-uchar-conversions");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let rust = translate_fixture(&tmp, "uchar_conversions");
    assert!(rust.contains(
        "fn mbrtoc16(_0: *mut u16, _1: *const i8, _2: usize, _3: *mut libc::mbstate_t) -> usize;"
    ));
    assert!(rust.contains("fn c16rtomb(_0: *mut i8, _1: u16, _2: *mut libc::mbstate_t) -> usize;"));
    assert!(rust.contains(
        "fn mbrtoc32(_0: *mut u32, _1: *const i8, _2: usize, _3: *mut libc::mbstate_t) -> usize;"
    ));
    assert!(rust.contains("fn c32rtomb(_0: *mut i8, _1: u32, _2: *mut libc::mbstate_t) -> usize;"));
    assert!(rust.contains(
        "let mut state16: libc::mbstate_t = unsafe { std::mem::zeroed::<libc::mbstate_t>() };"
    ));
    assert!(rust.contains(
        "let mut state32: libc::mbstate_t = unsafe { std::mem::zeroed::<libc::mbstate_t>() };"
    ));
    assert!(!rust.contains("*mut __mbstate_t"));
}

#[test]
fn c23_nullptr_uses_concrete_void_pointer_type() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-c23-nullptr");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let rust = translate_fixture(&tmp, "c23_nullptr");
    assert!(rust.contains("let mut null_value: *mut core::ffi::c_void"));
    assert!(rust.contains("std::ptr::null_mut() as *mut core::ffi::c_void"));
    assert!(!rust.contains("std::ptr::null_mut() == std::ptr::null_mut()"));
}

#[test]
fn lazy_singleton_recovers_once_lock() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-lazy-singleton");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let rust = translate_fixture(&tmp, "lazy_singleton");
    assert!(
        rust.contains(
            "static cached_value: std::sync::OnceLock<i32> = std::sync::OnceLock::new();"
        )
    );
    assert!(rust.contains("*cached_value.get_or_init(|| compute())"));
    assert!(!rust.contains("static mut cached_value"));
    assert!(!rust.contains("computed"));

    let rust = translate_fixture(&tmp, "lazy_singleton_unsafe_alias");
    assert!(rust.contains("static mut cached_value"));
    assert!(rust.contains("static mut computed"));
    assert!(!rust.contains("OnceLock"));
}

#[test]
fn counted_varargs_loop_uses_range_for() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-varargs-loop");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("varargs.c");
    let generated = tmp.join("varargs.generated.rs");

    support::translate(&c_src, &generated).expect("translate varargs fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated varargs rust");
    assert!(rust.contains("let mut ap: __SlateVaArgs;"));
    assert!(rust.contains("for _ in 0..n {"));
    assert!(rust.contains("let mut total: i32 = 0;"));
    assert!(rust.contains("total += unsafe { ap.next_arg::<i32>() };"));
    assert!(rust.contains("let first: i32 = unsafe { ap.next_arg::<i32>() };"));
    assert!(rust.contains("let second: i32 = unsafe { ap.next_arg::<i32>() };"));
    assert!(rust.contains("sum(4, __SlateVaArgs::new(vec![__SlateVaArg::new(_v2)"));
    assert!(rust.contains("pick_second(5, __SlateVaArgs::new(vec![__SlateVaArg::new(_v9)"));
    assert!(rust.contains("ap = __slate_va_args.clone();"));
    assert!(!rust.contains("total = 0;"));
    assert!(!rust.contains("let mut first: i32 = 0;"));
    assert!(!rust.contains("let mut second: i32 = 0;"));
    assert!(!rust.contains("first = unsafe { ap.next_arg::<i32>() };"));
    assert!(!rust.contains("second = unsafe { ap.next_arg::<i32>() };"));
    assert!(!rust.contains("let _v5: i32 = unsafe { ap.next_arg::<i32>() };"));
    assert!(!rust.contains("let _v0: i32 = unsafe { ap.next_arg::<i32>() };"));
    assert!(!rust.contains("let _v1: i32 = 4;"));
    assert!(!rust.contains("let mut ap: core::ffi::VaList<'_>;"));
    assert!(!rust.contains("if !(i < n)"));
}

#[test]
fn array_init_literal_counted_loop_uses_range_for() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-array-init-loop");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("array_init.c");
    let generated = tmp.join("array_init.generated.rs");

    support::translate(&c_src, &generated).expect("translate array_init fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated array_init rust");

    assert!(rust.contains("for i in 0..5 {"));
    assert!(rust.contains("sum += a[((i as i64) as usize)];"));
    assert!(!rust.contains("if !(i < 5)"));
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
    assert!(structs_rust.contains("p.left + p.right\n}"));
    assert!(structs_rust.contains("p.left = p.right + 2;"));
    assert!(structs_rust.contains("p.left\n}"));
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
    assert!(mixed_rust.contains("unsafe { s.total }\n}"));
    assert!(mixed_rust.contains("(unsafe { s.tag }) as i32\n}"));

    let unions_c = fixtures_dir().join("unions.c");
    let unions_generated = tmp.join("unions.generated.rs");
    support::translate(&unions_c, &unions_generated).expect("translate unions fixture");
    let unions_rust =
        std::fs::read_to_string(&unions_generated).expect("read generated unions rust");
    assert!(unions_rust.contains("unsafe { p.left }\n}"));

    let globals_c = fixtures_dir().join("global_vars.c");
    let globals_generated = tmp.join("global_vars.generated.rs");
    support::translate(&globals_c, &globals_generated).expect("translate global_vars fixture");
    let globals_rust =
        std::fs::read_to_string(&globals_generated).expect("read generated global_vars rust");
    assert!(globals_rust.contains("(unsafe { pair.left }) + unsafe { pair.right }\n}"));
}

#[test]
fn struct_field_initialization_is_folded_into_literal() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-struct-field-init");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("aligned_struct.c");
    let generated = tmp.join("aligned_struct.generated.rs");

    support::translate(&c_src, &generated).expect("translate aligned_struct fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated aligned_struct rust");

    assert!(
        rust.contains("let s: Aligned = Aligned { a: 5, b: 4660 };"),
        "expected folded struct initializer:\n{rust}"
    );
    assert!(rust.contains("std::mem::size_of::<Aligned>() as u64"));
    assert!(rust.contains("std::mem::align_of::<Aligned>() as u64"));
    assert!(rust.contains("std::mem::offset_of!(Aligned, a) as u64"));
    assert!(rust.contains("std::mem::offset_of!(Aligned, b) as u64"));
    assert!(!rust.contains("let mut s: Aligned = Aligned { a: 0, b: 0 };"));
    assert!(!rust.contains("s.a = 5;"));
    assert!(!rust.contains("s.b = 4660;"));
    assert!(rust.contains("let mut effectful: Aligned = Aligned { a: 0, b: 0 };"));
    assert!(rust.contains("effectful.a ="));
    assert!(rust.contains("effectful.b = 7;"));
    assert!(rust.contains("let mut repeated: Aligned = Aligned { a: 0, b: 0 };"));
    assert!(rust.contains("repeated.a = 1;"));
    assert!(rust.contains("repeated.a = 2;"));
    assert!(rust.contains("repeated.b = 3;"));
    assert!(rust.contains("let mut dependent: Aligned = Aligned { a: 0, b: 0 };"));
    assert!(rust.contains("dependent.b = 8;"));
    assert!(rust.contains("dependent.a = dependent.b as i8;"));
    assert!(rust.contains("let mut interrupted: Aligned = Aligned { a: 4, b: 0 };"));
    assert!(rust.contains("interrupted.b = 6;"));
    assert!(!rust.contains("let _v3: u64 = 16;"));
    assert!(!rust.contains("let _v4: u64 = 16;"));
    assert!(!rust.contains("let _v6: u64 = 0;"));
    assert!(!rust.contains("let _v7: u64 = 4;"));
}

#[test]
fn over_aligned_types_and_locals_preserve_alignment_and_addresses() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-over-aligned-local");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let rust = translate_fixture(&tmp, "over_aligned_local");
    assert!(rust.contains("#[repr(C, align(32))]"));
    assert!(rust.contains("std::mem::align_of::<OverAligned>() as u64"));
    assert!(rust.contains("std::ptr::addr_of_mut!(object) as u64"));
    assert!(rust.contains("aligned::Aligned<aligned::A64, i32>"));
    assert!(rust.contains("std::ptr::addr_of_mut!(*local) as u64"));
    assert!(!rust.contains("(object as u64)"));
    assert!(!rust.contains("(*local as u64)"));
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
    assert!(arrays.contains("values[2]\n}"));
    assert!(arrays.contains("values[((index as i64) as usize)]\n}"));
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
    assert!(array_types_rust.contains("values[2] = values[0] + values[1];"));
    assert!(!array_types_rust.contains("((values[0] as i32) + (values[1] as i32)) as i8"));
    assert!(array_types_rust.contains("values[((index as i64) as usize)]\n}"));

    let pointers = std::fs::read_to_string(tmp.join("pointers.generated.rs"))
        .expect("read generated pointers rust");
    assert!(pointers.contains("unsafe { *slot }\n}"));

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
    assert!(pointers.contains("unsafe { *ptr.offset(index as isize) }\n}"));
    assert!(!pointers.contains("unsafe { *unsafe {"));
    assert!(!pointers.contains(".add(_v9"));

    let bcopy_c = fixtures_dir().join("mem_bcopy_overlap.c");
    let bcopy_generated = tmp.join("mem_bcopy_overlap.generated.rs");
    support::translate(&bcopy_c, &bcopy_generated).expect("translate mem_bcopy_overlap fixture");
    let bcopy =
        std::fs::read_to_string(&bcopy_generated).expect("read generated mem_bcopy_overlap rust");
    assert!(bcopy.contains("buf.copy_within(0..4, 2);"));
    assert!(!bcopy.contains("std::ptr::copy"));
    assert!(!bcopy.contains("buf.as_mut_ptr().add(2)"));

    let bzero_c = fixtures_dir().join("mem_bzero.c");
    let bzero_generated = tmp.join("mem_bzero.generated.rs");
    support::translate(&bzero_c, &bzero_generated).expect("translate mem_bzero fixture");
    let bzero = std::fs::read_to_string(&bzero_generated).expect("read generated mem_bzero rust");
    assert!(bzero.contains("a.fill(0);"));
    assert!(!bzero.contains("std::ptr::write_bytes"));
}

#[test]
fn memset_recovers_fill_when_provable_and_falls_back_otherwise() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-memset");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let memset_c = fixtures_dir().join("memset.c");
    let memset_generated = tmp.join("memset.generated.rs");
    support::translate(&memset_c, &memset_generated).expect("translate memset fixture");
    let memset = std::fs::read_to_string(&memset_generated).expect("read generated memset rust");

    assert!(memset.contains("zero_buf.fill(0);"));
    assert!(memset.contains("value_buf.fill(65);"));
    assert!(memset.contains("partial_buf[(0..4)].fill(9);"));
    assert!(memset.contains("unsafe { memset("));
    assert!(memset.contains("dynamic_buf.as_mut_ptr()"));
    assert!(memset.contains(
        "fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;"
    ));
    assert!(!memset.contains("safe fn memset("));
    for lint in [
        "non_camel_case_types",
        "suspicious_runtime_symbol_definitions",
        "unpredictable_function_pointer_comparisons",
        "unused_comparisons",
    ] {
        assert!(memset.contains(lint), "missing generated allow for {lint}");
    }
}

#[test]
fn memcpy_recovers_slice_ops_when_provable_and_falls_back_otherwise() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-memcpy");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let memcpy_c = fixtures_dir().join("memcpy.c");
    let memcpy_generated = tmp.join("memcpy.generated.rs");
    support::translate(&memcpy_c, &memcpy_generated).expect("translate memcpy fixture");
    let memcpy = std::fs::read_to_string(&memcpy_generated).expect("read generated memcpy rust");

    assert!(memcpy.contains("full_dst = full_src;"));
    assert!(memcpy.contains("partial_dst[(0..4)].copy_from_slice(&partial_src[(0..4)]);"));
    assert!(memcpy.contains("unsafe { memcpy("));
    assert!(memcpy.contains("alias_buf.as_mut_ptr()"));
    assert!(memcpy.contains("dyn_dst.as_mut_ptr()"));
    assert!(memcpy.contains(
        "fn memcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;"
    ));
    assert!(!memcpy.contains("safe fn memcpy("));
}

#[test]
fn memmove_recovers_copy_within_when_provable_and_falls_back_otherwise() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-memmove");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let memmove_c = fixtures_dir().join("memmove.c");
    let memmove_generated = tmp.join("memmove.generated.rs");
    support::translate(&memmove_c, &memmove_generated).expect("translate memmove fixture");
    let memmove = std::fs::read_to_string(&memmove_generated).expect("read generated memmove rust");

    assert!(memmove.contains("forward_buf.copy_within(0..5, 1);"));
    assert!(memmove.contains("backward_buf.copy_within(1..6, 0);"));
    assert!(memmove.contains("unsafe { memmove("));
    assert!(memmove.contains("dyn_buf.as_mut_ptr()"));
    assert!(memmove.contains(
        "fn memmove(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;"
    ));
    assert!(!memmove.contains("safe fn memmove("));
}

#[test]
fn memcmp_recovers_slice_equality_when_provable_and_falls_back_otherwise() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-memcmp");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let memcmp_c = fixtures_dir().join("memcmp.c");
    let memcmp_generated = tmp.join("memcmp.generated.rs");
    support::translate(&memcmp_c, &memcmp_generated).expect("translate memcmp fixture");
    let memcmp = std::fs::read_to_string(&memcmp_generated).expect("read generated memcmp rust");

    assert!(memcmp.contains("(equal_a == equal_b) as i32"));
    assert!(memcmp.contains("(unequal_a == unequal_b) as i32"));
    assert!(memcmp.contains("(partial_a[(0..4)] == partial_b[(0..4)]) as i32"));
    assert!(memcmp.contains("unsafe { memcmp("));
    assert!(memcmp.contains("dyn_a.as_mut_ptr()"));
    assert!(memcmp.contains(
        "fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;"
    ));
    assert!(!memcmp.contains("safe fn memcmp("));
}

#[test]
fn trusted_strlen_uses_the_rust_runtime_signature() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-runtime-strlen");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("stdio_gets_loop_unsupported.c");
    let generated = tmp.join("stdio_gets_loop_unsupported.generated.rs");
    support::translate(&c_src, &generated).expect("translate stdio_gets_loop_unsupported fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated strlen Rust");

    assert!(rust.contains("fn strlen(_0: *const i8) -> usize;"));
    assert!(!rust.contains("safe fn strlen("));
}

#[test]
fn long_double_variadic_calls_retain_their_shim_declarations() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-long-double-variadic");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let rust = translate_fixture(&tmp, "long_double_variadic");
    assert!(rust.contains("fn __slate_printf__ri32_pi8_f80("));
    assert!(rust.contains("fn __slate_sscanf__ri32_pi8_pi8_pf80("));
}

#[test]
fn long_double_pointer_types_follow_target_abi() {
    let link_fixture = fixtures_dir().join("../fixtures.link/long_double/main.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate", link_fixture.to_str().unwrap()])
        .env("SLATE_TARGET", "x86_64-unknown-linux-gnu")
        .output()
        .expect("translate x87 long double pointer fixture");
    assert!(
        output.status.success(),
        "x87 long double pointer translation failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let rust = String::from_utf8(output.stdout).expect("generated Rust is UTF-8");
    assert!(rust.contains("fn ext_store(_0: *mut LongDouble, _1: LongDouble);"));
    assert!(rust.contains("fn ext_load(_0: *const LongDouble) -> LongDouble;"));
    assert!(rust.contains("fn __slate_ext_store__rv_pf80_f80(_0: *mut LongDouble"));
    assert!(rust.contains("fn __slate_ext_load__rf80_pf80(_0: *const LongDouble"));

    for (flavor, target, rust_type, android_api) in [
        ("macos", "aarch64-apple-darwin", "f64", None),
        ("msvc", "x86_64-pc-windows-msvc", "f64", None),
        ("bionic", "x86_64-linux-android", "f128", Some("21")),
    ] {
        let fixture = fixtures_dir().join(flavor).join("long_double_pointer.c");
        let mut command = std::process::Command::new(env!("CARGO_BIN_EXE_slate"));
        command
            .args(["translate", fixture.to_str().unwrap()])
            .env("SLATE_TARGET", target);
        if let Some(api) = android_api {
            command.env("SLATE_ANDROID_API", api);
        }
        let output = command.output().expect("translate pointer fixture");
        assert!(
            output.status.success(),
            "{flavor} long double pointer translation failed:\n{}",
            String::from_utf8_lossy(&output.stderr)
        );
        let rust = String::from_utf8(output.stdout).expect("generated Rust is UTF-8");
        assert!(rust.contains(&format!("fn store_long_double(_0: *mut {rust_type});")));
        assert!(rust.contains(&format!(
            "fn load_long_double(_0: *const {rust_type}) -> *const {rust_type};"
        )));
    }
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
    assert!(rust.contains("fn read_pointer(pointer: &mut i32) -> i32"));
    assert!(!rust.contains("unsafe { *pointer }"));
    assert!(rust.contains("read_pointer(&mut values[0])"));

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("fixup-debug")
        .arg("--debug-only-pass")
        .arg("array_element_pointer_origin")
        .arg(c_src)
        .output()
        .expect("run array_element_pointer_origin query trace");
    assert!(
        output.status.success(),
        "fixup-debug failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let stdout = String::from_utf8(output.stdout).expect("debug output is utf8");
    assert!(stdout.contains("query_rule=rewrite_array_element_pointer_origins"));
    assert_eq!(
        stdout.matches("query_case=known_origin").count(),
        3,
        "expected one known_origin match per pointer (p, q, _v8):\n{stdout}"
    );
}

#[test]
fn buffer_cursor_writes_collapse_to_array_indexing() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-buffer-cursor");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("buffer_cursor.c");
    let generated = tmp.join("buffer_cursor.generated.rs");
    support::translate(&c_src, &generated).expect("translate buffer_cursor fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated buffer_cursor rust");

    assert!(rust.contains("values[1] + values[3]"));
    assert!(rust.contains("3 - 1"));
    assert!(!rust.contains("let mut c: cursor"));
    assert!(!rust.contains("let mut d: cursor"));
    assert!(!rust.contains("c.ptr"));
    assert!(!rust.contains("d.ptr"));
    assert!(!rust.contains(".offset_from("));
}

#[test]
fn unnecessary_promotion_casts_are_stripped() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-unnecessary-casts");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("unnecessary_casts.c");
    let generated = tmp.join("unnecessary_casts.generated.rs");
    support::translate(&c_src, &generated).expect("translate unnecessary_casts fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated unnecessary_casts rust");

    assert!(rust.contains("c = a + b;"));
    assert!(!rust.contains("a as i32"));
    assert!(!rust.contains("b as i32"));

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("fixup-debug")
        .arg("--debug-only-pass")
        .arg("unnecessary_casts")
        .arg(&c_src)
        .output()
        .expect("run unnecessary_casts query trace");
    assert!(
        output.status.success(),
        "fixup-debug failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let stdout = String::from_utf8(output.stdout).expect("debug output is utf8");
    assert!(stdout.contains("query_rule=strip_unnecessary_assignment_cast"));
    assert!(stdout.contains("query_case=resolved"));
    assert_eq!(
        stdout.matches("evidence.cast=").count(),
        3,
        "expected one cast proof for the outer cast plus each operand:\n{stdout}"
    );
}

#[test]
fn zero_init_declarations_fold_into_their_overwriting_assignment() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-zero-init");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("zero_init.c");
    let generated = tmp.join("zero_init.generated.rs");
    support::translate(&c_src, &generated).expect("translate zero_init fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated zero_init rust");

    assert!(rust.contains("let x: i32 = 42;"));
    assert!(rust.contains("let z: i32 = unsafe { getchar() };\n    let y: i32 = z + 1;"));
    assert!(rust.contains("let y: i32 = z + 1;"));
    assert!(!rust.contains("let mut x"));
    assert!(!rust.contains("let mut y"));
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
fn do_while_loop_body_scope_is_unwrapped() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-do-while-scope");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("do_while.c");
    let generated = tmp.join("do_while.generated.rs");
    support::translate(&c_src, &generated).expect("translate do_while fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated do_while rust");

    assert!(rust.contains("loop {\n        total += i;\n        i += 1;\n        if !(i <= n)"));
    assert!(rust.contains("loop {\n        i += 1;\n        if i % 2 != 0"));
    assert!(!rust.contains("loop {\n        {\n            total += i;"));
    assert!(!rust.contains("loop {\n        {\n            i += 1;"));
}

#[test]
fn while_loop_body_scope_is_unwrapped() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-while-scope");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("while_loop.c");
    let generated = tmp.join("while_loop.generated.rs");
    support::translate(&c_src, &generated).expect("translate while_loop fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated while_loop rust");

    assert!(rust.contains(
        "loop {\n        if !(i <= n) {\n            break;\n        }\n        total += i;\n        i += 1;"
    ));
    assert!(!rust.contains(
        "loop {\n        if !(i <= n) {\n            break;\n        }\n        {\n            total += i;"
    ));
}

#[test]
fn decrement_temp_updates_use_compound_assignment() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-decrement");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("decrement.c");
    let generated = tmp.join("decrement.generated.rs");
    support::translate(&c_src, &generated).expect("translate decrement fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated decrement rust");

    assert!(rust.contains("let _v2: i32 = a;\n    a -= 1;\n    let post: i32 = _v2;"));
    assert!(rust.contains("a -= 1;\n    let mut pre: i32 = a;"));
    assert!(rust.contains("let _v6: i32 = a;\n    a -= 1;"));
    assert!(rust.contains("pre -= 1;\n    let sum: i32 = _v6 + pre;"));
    assert!(!rust.contains("a = _v2 - 1;"));
    assert!(!rust.contains("let _v5: i32 = a - 1;"));
    assert!(!rust.contains("a = _v5;"));
    assert!(!rust.contains("let _v9: i32 = pre - 1;"));
    assert!(!rust.contains("pre = _v9;"));
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

    let precision_c = fixtures_dir().join("printf_integer_precision.c");
    let precision_generated = tmp.join("printf_integer_precision.generated.rs");
    support::translate(&precision_c, &precision_generated)
        .expect("translate printf integer precision fixture");
    let precision_rust = std::fs::read_to_string(&precision_generated)
        .expect("read generated printf integer precision rust");
    assert!(precision_rust.contains("println!(\"{} {}\","));
    assert!(precision_rust.contains("println!(\"{}|{}|{}\","));
    assert!(precision_rust.contains("println!(\"{}\","));
    assert!(precision_rust.contains("println!(\"{} {} {} {}\","));
    assert!(precision_rust.contains("format!(\"-{:03}\", __slate_printf_arg.unsigned_abs())"));
    assert!(precision_rust.contains("format!(\"{:03}\", __slate_printf_arg)"));
    assert!(precision_rust.contains("format!(\"{:>8}\","));
    assert!(precision_rust.contains("format!(\"{:<8}\","));
    assert!(precision_rust.contains("format!(\"+{:03}\", __slate_printf_arg)"));
    assert!(precision_rust.contains("format!(\"{:04x}\", hex)"));
    assert!(precision_rust.contains("format!(\"{:04X}\", hex)"));
    assert!(precision_rust.contains("format!(\"{:04o}\", hex)"));
    assert!(!precision_rust.contains("fn printf("));
    assert!(!precision_rust.contains("unsafe { printf("));

    let string_width_c = fixtures_dir().join("printf_string_width_precision.c");
    let string_width_generated = tmp.join("printf_string_width_precision.generated.rs");
    support::translate(&string_width_c, &string_width_generated)
        .expect("translate printf string width/precision fixture");
    let string_width_rust = std::fs::read_to_string(&string_width_generated)
        .expect("read generated printf string width/precision rust");
    assert!(
        string_width_rust
            .contains("println!(\"{}|{}|{}|{}\", \"   hi\", \"hi   \", \"h\", \"     h\");")
    );
    assert!(!string_width_rust.contains("fn printf("));
    assert!(!string_width_rust.contains("unsafe { printf("));

    let char_width_c = fixtures_dir().join("printf_char_width.c");
    let char_width_generated = tmp.join("printf_char_width.generated.rs");
    support::translate(&char_width_c, &char_width_generated)
        .expect("translate printf char width fixture");
    let char_width_rust = std::fs::read_to_string(&char_width_generated)
        .expect("read generated printf char width rust");
    assert!(char_width_rust.contains("println!(\"{}|{}|{}\", \"  a\", \"b  \", \"c\");"));
    assert!(!char_width_rust.contains("fn printf("));
    assert!(!char_width_rust.contains("unsafe { printf("));

    let char_width_rejected_c = fixtures_dir().join("printf_char_width_rejected.c");
    let char_width_rejected_generated = tmp.join("printf_char_width_rejected.generated.rs");
    support::translate(&char_width_rejected_c, &char_width_rejected_generated)
        .expect("translate rejected printf char width fixture");
    let char_width_rejected_rust = std::fs::read_to_string(&char_width_rejected_generated)
        .expect("read generated rejected printf char width rust");
    assert!(char_width_rejected_rust.contains("fn printf("));
    assert!(char_width_rejected_rust.contains("unsafe { printf("));
    assert!(!char_width_rejected_rust.contains("println!("));

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

    let array_init_c = fixtures_dir().join("array_init.c");
    let array_init_generated = tmp.join("array_init.generated.rs");
    support::translate(&array_init_c, &array_init_generated).expect("translate array_init fixture");
    let array_init_rust =
        std::fs::read_to_string(&array_init_generated).expect("read generated array_init rust");
    assert!(array_init_rust.contains("println!(\"{}\", sum);"));
    assert!(array_init_rust.contains("println!(\"{} {}\", partial[1], partial[3]);"));
    assert!(array_init_rust.contains("println!(\"{}\", s);"));
    assert!(array_init_rust.contains("println!(\"{} {}\", \"hi\", padded[4] as i32);"));
    assert!(array_init_rust.contains(
        "let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);"
    ));
    assert!(array_init_rust.contains(
        "let mut partial: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);"
    ));
    assert!(array_init_rust.contains("*a = [1, 2, 3, 4, 5];"));
    assert!(array_init_rust.contains("*partial = [7, 8, 0, 0];"));
    assert!(array_init_rust.contains("let padded: [i8; 8] = [104, 105, 0, 0, 0, 0, 0, 0];"));
    assert!(!array_init_rust.contains("let mut a: [i32; 5] = [0; 5];"));
    assert!(!array_init_rust.contains("let mut partial: [i32; 4] = [0; 4];"));
    assert!(!array_init_rust.contains("let mut padded: [i8; 8] = [0; 8];"));
    assert!(!array_init_rust.contains("padded = [104, 105, 0, 0, 0, 0, 0, 0];"));
    assert!(!array_init_rust.contains("fn printf("));
    assert!(!array_init_rust.contains("unsafe { printf("));

    let rejected_c = fixtures_dir().join("printf_string_char_rejected.c");
    let rejected_generated = tmp.join("printf_string_char_rejected.generated.rs");
    support::translate(&rejected_c, &rejected_generated)
        .expect("translate rejected printf string/char fixture");
    let rejected_rust = std::fs::read_to_string(&rejected_generated)
        .expect("read generated rejected printf string/char rust");
    assert!(rejected_rust.contains("let buf: &str = \"hey\";"));
    assert!(rejected_rust.contains("println!(\"{}\", buf);"));
    assert!(!rejected_rust.contains("fn printf("));
    assert!(!rejected_rust.contains("unsafe { printf("));

    let partial_c = fixtures_dir().join("printf_partial_conversion.c");
    let partial_generated = tmp.join("printf_partial_conversion.generated.rs");
    support::translate(&partial_c, &partial_generated)
        .expect("translate printf partial conversion fixture");
    let partial_rust = std::fs::read_to_string(&partial_generated)
        .expect("read generated printf partial conversion rust");
    assert!(partial_rust.contains("println!(\"first {}\", 1);"));
    assert!(partial_rust.contains("println!(\"last {}\", 3);"));
    assert!(partial_rust.contains("std::io::Write::flush(&mut std::io::stdout()).unwrap();"));
    assert!(partial_rust.contains("unsafe { printf("));
    assert!(partial_rust.contains("unsafe { fflush((unsafe { stdout }) as *mut libc::FILE) };"));
    let flush_before = partial_rust.find("std::io::Write::flush").unwrap();
    let raw_printf = partial_rust.find("unsafe { printf(").unwrap();
    let fflush_after = partial_rust.find("unsafe { fflush(").unwrap();
    let last_println = partial_rust.rfind("println!(\"last {}\", 3);").unwrap();
    assert!(flush_before < raw_printf && raw_printf < fflush_after && fflush_after < last_println);

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

    let float_width_c = fixtures_dir().join("printf_float_width.c");
    let float_width_generated = tmp.join("printf_float_width.generated.rs");
    support::translate(&float_width_c, &float_width_generated)
        .expect("translate printf float width fixture");
    let float_width_rust = std::fs::read_to_string(&float_width_generated)
        .expect("read generated printf float width rust");
    assert!(
        float_width_rust
            .contains("println!(\"{:8.2}|{:+8.2}|{:08.2}|{:<8.2}|{:+.2}\", a, a, b, a, b);")
    );
    assert!(!float_width_rust.contains("fn printf("));
    assert!(!float_width_rust.contains("unsafe { printf("));

    let float_width_rejected_c = fixtures_dir().join("printf_float_width_rejected.c");
    let float_width_rejected_generated = tmp.join("printf_float_width_rejected.generated.rs");
    support::translate(&float_width_rejected_c, &float_width_rejected_generated)
        .expect("translate rejected printf float width fixture");
    let float_width_rejected_rust = std::fs::read_to_string(&float_width_rejected_generated)
        .expect("read generated rejected printf float width rust");
    assert!(float_width_rejected_rust.contains("fn printf("));
    assert!(float_width_rejected_rust.contains("unsafe { printf("));
    assert!(!float_width_rejected_rust.contains("println!("));

    let general_rejected_c = fixtures_dir().join("printf_general_rejected.c");
    let general_rejected_generated = tmp.join("printf_general_rejected.generated.rs");
    support::translate(&general_rejected_c, &general_rejected_generated)
        .expect("translate rejected printf general float fixture");
    let general_rejected_rust = std::fs::read_to_string(&general_rejected_generated)
        .expect("read generated rejected printf general float rust");
    assert!(general_rejected_rust.contains("fn printf("));
    assert!(general_rejected_rust.contains("unsafe { printf("));
    assert!(!general_rejected_rust.contains("println!("));

    let general_c = fixtures_dir().join("printf_general.c");
    let general_generated = tmp.join("printf_general.generated.rs");
    support::translate(&general_c, &general_generated)
        .expect("translate printf general float fixture");
    let general_rust = std::fs::read_to_string(&general_generated)
        .expect("read generated printf general float rust");
    assert!(general_rust.contains("println!(\"{} {} {}|\", \"1234.57\", \"1.23e+03\","));
    assert!(!general_rust.contains("fn printf("));
    assert!(!general_rust.contains("unsafe { printf("));

    let literal_escaping_c = fixtures_dir().join("printf_literal_escaping.c");
    let literal_escaping_generated = tmp.join("printf_literal_escaping.generated.rs");
    support::translate(&literal_escaping_c, &literal_escaping_generated)
        .expect("translate printf literal escaping fixture");
    let literal_escaping_rust = std::fs::read_to_string(&literal_escaping_generated)
        .expect("read generated printf literal escaping rust");
    assert!(
        literal_escaping_rust.contains(
            r#"println!("{{{}}} % \"quoted\" back\\slash {}|{}|{:x}", d, "hi", "X", h);"#
        )
    );
    assert!(literal_escaping_rust.contains(r#"println!("}}}}%{{{{{}}}}}", d);"#));
    assert!(literal_escaping_rust.contains(r#"println!("%%{}%%", d);"#));
    assert!(literal_escaping_rust.contains(r#"println!("{{{{}}}}{}{{{{}}}}", "mid");"#));
    assert!(!literal_escaping_rust.contains("fn printf("));
    assert!(!literal_escaping_rust.contains("unsafe { printf("));

    let narrow_c = fixtures_dir().join("printf_narrow_length_modifiers.c");
    let narrow_generated = tmp.join("printf_narrow_length_modifiers.generated.rs");
    support::translate(&narrow_c, &narrow_generated)
        .expect("translate printf narrow length modifiers fixture");
    let narrow_rust = std::fs::read_to_string(&narrow_generated)
        .expect("read generated printf narrow length modifiers rust");
    assert!(narrow_rust.contains(
        "println!(\"{} {} {} {} {} {} {}\", ((s as i32) as u16) as i16, (us as i32) as u16, ((c as i32) as u8) as i8, (uc as i32) as u8, j, ju, t);"
    ));
    assert!(narrow_rust.contains(
        "println!(\"{} {} {} {} {}\", (full_int as u8) as i8, ((uc as i32) as u8) as i8, negative as u8, (wide as u16) as i16, wide as u16);"
    ));
    assert!(!narrow_rust.contains("fn printf("));
    assert!(!narrow_rust.contains("unsafe { printf("));

    let narrow_precision_alternate_c =
        fixtures_dir().join("printf_narrow_length_modifiers_precision_alternate.c");
    let narrow_precision_alternate_generated =
        tmp.join("printf_narrow_length_modifiers_precision_alternate.generated.rs");
    support::translate(
        &narrow_precision_alternate_c,
        &narrow_precision_alternate_generated,
    )
    .expect("translate printf narrow length modifiers precision/alternate fixture");
    let narrow_precision_alternate_rust =
        std::fs::read_to_string(&narrow_precision_alternate_generated)
            .expect("read generated printf narrow length modifiers precision/alternate rust");
    assert!(narrow_precision_alternate_rust.contains("let __slate_printf_arg = (a as u8) as i8;"));
    assert!(
        narrow_precision_alternate_rust.contains("let __slate_printf_arg = (b as u16) as i16;")
    );
    assert!(narrow_precision_alternate_rust.contains("let __slate_printf_arg = a as u8;"));
    assert!(narrow_precision_alternate_rust.contains("format!(\"{:04o}\", a as u8)"));
    assert!(narrow_precision_alternate_rust.contains("format!(\"{:02x}\", a as u8)"));
    assert!(narrow_precision_alternate_rust.contains("let __slate_printf_arg = c as u16;"));
    assert!(!narrow_precision_alternate_rust.contains("fn printf("));
    assert!(!narrow_precision_alternate_rust.contains("unsafe { printf("));

    let narrow_rejected_c = fixtures_dir().join("printf_narrow_length_modifiers_rejected.c");
    let narrow_rejected_generated =
        tmp.join("printf_narrow_length_modifiers_rejected.generated.rs");
    support::translate(&narrow_rejected_c, &narrow_rejected_generated)
        .expect("translate rejected printf narrow length modifiers fixture");
    let narrow_rejected_rust = std::fs::read_to_string(&narrow_rejected_generated)
        .expect("read generated rejected printf narrow length modifiers rust");
    assert!(narrow_rejected_rust.contains("fn printf("));
    assert!(narrow_rejected_rust.contains("unsafe { printf("));
    assert!(!narrow_rejected_rust.contains("println!("));

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
fn u8_string_literal_preserves_storage_and_index_reads() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-u8-string-literal");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("u8_string_literal.c");
    let generated = tmp.join("u8_string_literal.generated.rs");

    support::translate(&c_src, &generated).expect("translate u8 string literal fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated u8 string literal rust");
    assert!(rust.contains("static mut main_text: [i8; 3] = [-50, -87, 0];"));
    assert_eq!(rust.matches("main_text[1]").count(), 3);
    assert!(rust.contains("main_text[2]"));
    assert!(rust.contains("let alignment: i32 = 4;"));
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
    assert!(!rust.contains("unsafe { fgets("));
    assert!(!rust.contains("unsafe { fclose("));
    assert!(rust.contains("std::io::BufRead::read_until("));
    assert!(rust.contains("unsafe { fputs(buf.as_mut_ptr()"));

    let drop_index = rust
        .find("drop(f);")
        .expect("close before the later reopen must become an explicit drop");
    let reopen_index = rust
        .find("std::io::BufReader::new(std::fs::OpenOptions::new().read(true).open(\"slate_stdio_file_write.tmp\")")
        .expect("generated rust should reopen the same path with OpenOptions");
    assert!(
        drop_index < reopen_index,
        "explicit drop must precede the reopen of the same path"
    );
}

#[test]
fn close_before_remove_owner_preserves_close_timing_as_explicit_drop() {
    let tmp =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-stdio-close-before-remove");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("stdio_file_close_before_remove.c");
    let generated = tmp.join("stdio_file_close_before_remove.generated.rs");

    support::translate(&c_src, &generated).expect("translate stdio close-before-remove fixture");
    let rust =
        std::fs::read_to_string(&generated).expect("read generated stdio close-before-remove rust");

    let drop_index = rust
        .find("drop(f);")
        .expect("close before a later remove of the same path must become an explicit drop");
    let remove_index = rust
        .find("unsafe { remove(")
        .expect("generated rust should keep the remove call");
    assert!(
        drop_index < remove_index,
        "explicit drop must precede the later remove of the same path"
    );
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
    assert!(rust.contains("r + t + m\n}"));
}

#[test]
fn integer_bit_builtins_inline_pure_temps() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-integer-bit-builtins");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("integer_bit_builtin_ops.c");
    let generated = tmp.join("integer_bit_builtin_ops.generated.rs");

    support::translate(&c_src, &generated).expect("translate integer bit builtin fixture");
    let rust =
        std::fs::read_to_string(&generated).expect("read generated integer bit builtin rust");

    assert!(!rust.contains("let _v"), "{rust}");
    assert!(rust.contains("let leading: i32 = u.leading_zeros() as i32;"));
    assert!(rust.contains("let trailing: i32 = u.trailing_zeros() as i32;"));
    assert!(rust.contains(
        "let first_set: i32 = if (u as i32) == 0 { 0 } else { ((u as i32).trailing_zeros() as i32) + 1 };"
    ));
    assert!(rust.contains("let ones: i32 = u.count_ones() as i32;"));
    assert!(rust.contains("let odd: i32 = (u.count_ones() & 1) as i32;"));
    assert!(rust.contains(
        "let redundant_sign: i32 = (if s < 0 { !s } else { s }.leading_zeros() as i32) - 1;"
    ));
}

#[test]
fn control_flow_assume_preserves_ub_contract() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-control-flow-builtins");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("control_flow_builtin_ops.c");
    let generated = tmp.join("control_flow_builtin_ops.generated.rs");

    support::translate(&c_src, &generated).expect("translate control-flow builtin fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated control-flow rust");

    assert!(rust.contains("unsafe { core::hint::assert_unchecked("));
}

#[test]
fn for_continue_uses_structured_guards_without_synthetic_labels() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-for-continue");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let continue_c = fixtures_dir().join("continue_for.c");
    let continue_generated = tmp.join("continue_for.generated.rs");
    support::translate(&continue_c, &continue_generated).expect("translate continue_for fixture");
    let continue_rust =
        std::fs::read_to_string(&continue_generated).expect("read generated continue_for rust");
    assert!(!continue_rust.contains("__continue"));
    assert!(!continue_rust.contains("__loop"));
    assert!(continue_rust.contains("if i % 2 != 0 {"));
    assert!(continue_rust.contains("total += i;"));

    let combined_c = fixtures_dir().join("loop_break_continue.c");
    let combined_generated = tmp.join("loop_break_continue.generated.rs");
    support::translate(&combined_c, &combined_generated)
        .expect("translate loop_break_continue fixture");
    let combined_rust = std::fs::read_to_string(&combined_generated)
        .expect("read generated loop_break_continue rust");
    assert!(!combined_rust.contains("__continue"));
    assert!(!combined_rust.contains("__loop"));
    assert!(combined_rust.contains("if c != r {"));
    assert!(combined_rust.contains("if c > 4 {"));
    assert!(combined_rust.contains("break;"));
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
fn noreturn_c11_uses_process_exit_and_removes_dead_false_branch() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-noreturn-c11-cleanup");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("noreturn_c11_spelling.c");
    let generated = tmp.join("noreturn_c11_spelling.generated.rs");

    support::translate(&c_src, &generated).expect("translate noreturn_c11_spelling fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated noreturn C11 rust");
    assert!(rust.contains("std::process::exit(code as i32)\n}"));
    assert!(!rust.contains("unsafe extern \"C\""));
    assert!(!rust.contains("fn exit("));
    assert!(!rust.contains("let _v1: i32 = 0;"));
    assert!(!rust.contains("die(1);"));

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("fixup-debug")
        .arg(c_src)
        .arg("--debug-only-pass")
        .arg("libc_exit")
        .output()
        .expect("run libc_exit query trace");
    assert!(
        output.status.success(),
        "fixup-debug failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let stdout = String::from_utf8(output.stdout).expect("debug output is utf8");
    assert!(stdout.contains("rewrite_libc_exit"));
    assert!(stdout.contains("query_case=never_returning_extern"));
    assert!(stdout.contains("evidence.call=known:exit;arity=1"));
    assert!(stdout.contains("evidence.extern_fn=name=exit;arity=1;returns_never=true"));
}

#[test]
fn project_exit_definition_is_not_rewritten_as_libc_exit() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-local-exit");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("function_provenance.c");
    let generated = tmp.join("function_provenance.generated.rs");

    support::translate(&c_src, &generated).expect("translate function_provenance fixture");
    let rust = std::fs::read_to_string(&generated).expect("read function_provenance Rust");
    assert!(rust.contains("fn exit(status: i32) {"));
    assert!(rust.contains("    exit(0);"));
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
    assert!(fp_rust.contains("add_pair as *const ()"));
    assert!(
        fp_rust.contains("Option<unsafe extern \"C\" fn(i32, i32) -> i32>>(add_pair as *const ())")
    );
    assert!(fp_rust.contains(".unwrap()("));
    assert!(fp_rust.contains("lhs + rhs\n}"));
    assert!(fp_rust.contains("lhs * rhs\n}"));
    assert!(!fp_rust.contains("let mut __retval: i32 = lhs + rhs;"));
    assert!(!fp_rust.contains("let mut __retval: i32 = lhs * rhs;"));

    let extern_c = fixtures_dir().join("extern_decl.c");
    let extern_generated = tmp.join("extern_decl.generated.rs");
    support::translate(&extern_c, &extern_generated).expect("translate extern fixture");
    let extern_rust =
        std::fs::read_to_string(&extern_generated).expect("read generated extern rust");
    assert!(extern_rust.contains("unsafe { toupper("));
    assert!(extern_rust.contains(" as i32) }"));
    assert!(!extern_rust.contains("unsafe { printf("));
    assert!(extern_rust.contains("as u8) as char"));
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
    assert!(rust.contains("return unsafe { op.unwrap()(value) };"));
    assert!(rust.contains("value\n}"));
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
    assert!(!goto_rust.contains("__state0"));
    assert!(!goto_rust.contains("__dispatch0"));
    assert!(
        goto_rust.contains("if n < 0 {\n        cls = -1;\n    } else {\n        cls = 1;\n    }")
    );
    assert!(goto_rust.contains("println!(\"{}\", cls);"));

    let goto_return_c = fixtures_dir().join("goto_return_value.c");
    let goto_return_generated = tmp.join("goto_return_value.generated.rs");
    support::translate(&goto_return_c, &goto_return_generated)
        .expect("translate goto return fixture");
    let goto_return_rust =
        std::fs::read_to_string(&goto_return_generated).expect("read generated goto return rust");
    assert!(!goto_return_rust.contains("__state0"));
    assert!(!goto_return_rust.contains("__dispatch0"));
    assert!(goto_return_rust.contains("if n < 0 {\n        return -1;\n    }"));
    assert!(goto_return_rust.contains("if n == 0 {\n        return 0;\n    }"));
    assert!(goto_return_rust.contains("1\n}"));

    let goto_forward_c = fixtures_dir().join("goto_forward.c");
    let goto_forward_generated = tmp.join("goto_forward.generated.rs");
    support::translate(&goto_forward_c, &goto_forward_generated)
        .expect("translate forward goto fixture");
    let goto_forward_rust =
        std::fs::read_to_string(&goto_forward_generated).expect("read generated forward goto rust");
    assert!(!goto_forward_rust.contains("__state0"));
    assert!(!goto_forward_rust.contains("__dispatch0"));
    assert!(goto_forward_rust.contains("let x: i32 = 1;"));
    assert!(goto_forward_rust.contains("println!(\"{}\", x);"));
    assert!(!goto_forward_rust.contains("x = 99;"));

    let goto_nested_c = fixtures_dir().join("goto_nested_if.c");
    let goto_nested_generated = tmp.join("goto_nested_if.generated.rs");
    support::translate(&goto_nested_c, &goto_nested_generated)
        .expect("translate nested goto fixture");
    let goto_nested_rust =
        std::fs::read_to_string(&goto_nested_generated).expect("read generated nested goto rust");
    assert!(!goto_nested_rust.contains("__state0"));
    assert!(!goto_nested_rust.contains("__dispatch0"));
    assert!(goto_nested_rust.contains(
        "if a > 0 {\n        if b > 0 {\n            r = 1;\n        } else {\n            r = 2;\n        }\n    } else {\n        r = 3;\n    }"
    ));

    let goto_switch_c = fixtures_dir().join("goto_switch_forward.c");
    let goto_switch_generated = tmp.join("goto_switch_forward.generated.rs");
    support::translate(&goto_switch_c, &goto_switch_generated)
        .expect("translate switch goto fixture");
    let goto_switch_rust =
        std::fs::read_to_string(&goto_switch_generated).expect("read generated switch goto rust");
    assert!(!goto_switch_rust.contains("__state0"));
    assert!(!goto_switch_rust.contains("__dispatch0"));
    assert!(goto_switch_rust.contains("match x {\n        1 => {"));
    assert!(goto_switch_rust.contains("r = 20;"));
    assert!(goto_switch_rust.contains("_ => {\n            r = 30;"));

    let goto_loop_c = fixtures_dir().join("goto_backward_loop.c");
    let goto_loop_generated = tmp.join("goto_backward_loop.generated.rs");
    support::translate(&goto_loop_c, &goto_loop_generated).expect("translate goto loop fixture");
    let goto_loop_rust =
        std::fs::read_to_string(&goto_loop_generated).expect("read generated goto loop rust");
    assert!(!goto_loop_rust.contains("__state0"));
    assert!(!goto_loop_rust.contains("__dispatch0"));
    assert!(goto_loop_rust.contains("loop {"));
    assert!(goto_loop_rust.contains("if !(i < 5) {\n            break;\n        }"));
    assert!(goto_loop_rust.contains("sum += i;"));
    assert!(goto_loop_rust.contains("i += 1;"));
    assert!(goto_loop_rust.contains("println!(\"{}\", sum);"));
    assert!(!goto_loop_rust.contains("__retval"));
    assert!(!goto_loop_rust.contains("let _v3: i32 = sum;"));
    assert!(!goto_loop_rust.contains("let _v15: i32 = __retval;"));

    let goto_multi_exit_c = fixtures_dir().join("goto_multi_exit_loop.c");
    let goto_multi_exit_generated = tmp.join("goto_multi_exit_loop.generated.rs");
    support::translate(&goto_multi_exit_c, &goto_multi_exit_generated)
        .expect("translate multi-exit goto loop fixture");
    let goto_multi_exit_rust = std::fs::read_to_string(&goto_multi_exit_generated)
        .expect("read generated multi-exit goto loop rust");
    assert!(!goto_multi_exit_rust.contains("__state0"));
    assert!(!goto_multi_exit_rust.contains("__dispatch0"));
    assert!(goto_multi_exit_rust.contains("loop {\n        sum += i;"));
    assert!(goto_multi_exit_rust.contains("println!(\"overflow\");"));
    assert!(goto_multi_exit_rust.contains("if i < 5 {\n            continue;\n        }"));

    let goto_irreducible_c = fixtures_dir().join("goto_irreducible.c");
    let goto_irreducible_generated = tmp.join("goto_irreducible.generated.rs");
    support::translate(&goto_irreducible_c, &goto_irreducible_generated)
        .expect("translate irreducible goto fixture");
    let goto_irreducible_rust = std::fs::read_to_string(&goto_irreducible_generated)
        .expect("read generated irreducible goto rust");
    assert!(!goto_irreducible_rust.contains("__state0"));
    assert!(!goto_irreducible_rust.contains("__dispatch0"));
    assert!(goto_irreducible_rust.contains("let mut __block0: i32;"));
    assert!(goto_irreducible_rust.contains("loop {\n        match __block0 {"));
    assert!(goto_irreducible_rust.contains("__block0 = 2;"));
    assert!(goto_irreducible_rust.contains("__block0 = 1;"));
    assert!(goto_irreducible_rust.contains("println!(\"{}\", x);"));
    // the zero-init decl absorbs the redundant `x = 0;` store past the volatile write
    assert!(!goto_irreducible_rust.contains("\n    x = 0;"));

    let computed_goto_c = fixtures_dir().join("computed_goto_ops.c");
    let computed_goto_generated = tmp.join("computed_goto_ops.generated.rs");
    support::translate(&computed_goto_c, &computed_goto_generated)
        .expect("translate computed goto fixture");
    let computed_goto_rust = std::fs::read_to_string(&computed_goto_generated)
        .expect("read generated computed goto rust");
    assert!(computed_goto_rust.contains("__state0 = [1, 2, 3]"));
    assert!(computed_goto_rust.contains("continue '__dispatch0;"));
    assert!(computed_goto_rust.contains("match __state0 {"));
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
    assert!(rust.contains(
        "static mut numbers: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([1, 2, 0, 0]);"
    ));
    assert!(rust.contains("static mut pair: Pair = Pair { left: 3, right: 5 };"));
}

#[test]
fn thread_local_globals_emit_thread_local_statics() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-thread-local");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("thread_local.c");
    let generated = tmp.join("thread_local.generated.rs");

    support::translate(&c_src, &generated).expect("translate thread local fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated thread local rust");
    assert!(rust.contains("#![feature(thread_local)]"));
    assert!(rust.contains("#[thread_local]\nstatic mut file_value: i32 = 5;"));
    assert!(rust.contains("#[thread_local]\nstatic mut update_values_block_value: i32 = 7;"));
}

#[test]
fn c11_library_callbacks_preserve_c_abi() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-c11-callbacks");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let rust = translate_fixture(&tmp, "c11_callbacks");
    assert_eq!(rust.matches("Option<unsafe extern \"C\" fn").count(), 4);
    assert_eq!(rust.matches("extern \"C\" fn ").count(), 4);
    assert!(!rust.contains("Option<fn("));
    assert!(rust.contains("let mut control: i32 = 0;"));
    assert!(rust.contains("call_once(std::ptr::addr_of_mut!(control) as *mut i32"));
}

#[test]
fn libc_function_pointers_preserve_unsafe_c_abi() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-libc-function-pointers");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let rust = translate_fixture(&tmp, "libc_address_taken_safe_callback");
    assert!(rust.contains(
        "realloc_fn: Option<unsafe extern \"C\" fn(*mut core::ffi::c_void, u64) -> *mut core::ffi::c_void>"
    ));
    assert!(rust.contains("free_fn: Option<unsafe extern \"C\" fn(*mut core::ffi::c_void)>"));
    assert!(rust.contains("unsafe { unsafe { alloc.realloc_fn }.unwrap()("));
    assert!(rust.contains("unsafe { unsafe { alloc.free_fn }.unwrap()(p) };"));
}

#[test]
fn aggregate_value_member_ops_inline_member_access_temps() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-aggregate-members");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("aggregate_value_member_ops.c");
    let generated = tmp.join("aggregate_value_member_ops.generated.rs");

    support::translate(&c_src, &generated).expect("translate aggregate member fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated aggregate member rust");
    assert!(rust.contains(
        "\
fn take_pair(p: Pair) -> i32 {
    p.left * 10 + p.right
}
"
    ));
    assert!(rust.contains(
        "\
fn nested_total(n: Nested) -> i32 {
    n.inner.left + n.inner.right + n.tag
}
"
    ));
    assert!(rust.contains(
        "\
fn array_value(w: WithArray) -> i32 {
    w.data[1] + w.marker
}
"
    ));
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
                "*slot = (unsafe { *slot }) + amount;",
                "values[2] = 12;",
            ][..],
        ),
        ("struct_with_array", &["b.data[0] = 10;", "b.len = 3;"][..]),
        (
            "global_vars",
            &[
                "counter = (unsafe { counter }) + by;",
                "(*numbers)[2] =",
                "pair.right = (unsafe { pair.right }) + unsafe { (*numbers)[1] };",
            ][..],
        ),
        (
            "bitfield_ops",
            &[
                "__bitfield_0: __slate_bitfields::__SlateBitfield_Bits_0,",
                "s.__bitfield_0.set_a(_v2);",
                "s.__bitfield_0.set_b(_v4);",
                "w.__bitfield_0.set_x(_v49);",
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
fn preserves_full_comments_on_declarations() {
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
    assert!(rust.contains("/// stores a selected mode\n/// names holder records\n#[repr(C)]"));
    assert!(rust.contains("    /// current mode value\n    mode: Mode,"));
    assert!(rust.contains("/// counts completed operations\nstatic mut completed_count"));
    assert!(rust.contains(
        "/// increments a value and records the operation\n/// stores the intermediate result\nfn increment"
    ));
    for comment in [
        "selects an operating mode",
        "disable processing",
        "enable processing",
        "stores a selected mode",
        "names holder records",
        "current mode value",
        "counts completed operations",
        "increments a value and records the operation",
        "stores the intermediate result",
    ] {
        assert_eq!(
            rust.matches(comment).count(),
            1,
            "duplicate comment: {comment}"
        );
    }
}

#[test]
fn preserves_enum_variant_names_instead_of_folded_constants() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-enums");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("enums.c");
    let generated = tmp.join("enums.generated.rs");

    support::translate(&c_src, &generated).expect("translate enums fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated enums rust");

    assert!(rust.contains("enum Basic"));
    assert!(rust.contains("BasicFive = 5"));
    assert!(rust.contains("BasicNegative = -2"));
    assert!(rust.contains("Basic::BasicZero as i32"));
    assert!(rust.contains("Basic::BasicOne as i32"));
    assert!(rust.contains("Basic::BasicFive as i32"));
    assert!(rust.contains("Basic::BasicSix as i32"));
    assert!(rust.contains("Basic::BasicNegative as i32"));
    assert!(rust.contains("Basic::BasicNegativeNext as i32"));
    assert!(!rust.contains("println!(\"{}\", 5);"));
}

#[test]
fn unused_type_definitions_are_pruned_but_reachable_ones_survive() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-unused-items");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("unused_items.c");
    let generated = tmp.join("unused_items.generated.rs");

    support::translate(&c_src, &generated).expect("translate unused_items fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated unused_items rust");

    assert!(!rust.contains("totally_unused"));
    assert!(!rust.contains("linked_a"));
    assert!(!rust.contains("linked_b"));
    assert!(!rust.contains("truly_dead"));
    assert!(rust.contains("enum color"));
    assert!(rust.contains("mut c: color = color::RED"));
    assert!(rust.contains("c = color::GREEN"));
    assert!(rust.contains("c as u32"));

    assert!(!rust.contains("hidden_static"));
    assert!(rust.contains("live_static"));
}

#[test]
fn unused_params_are_removed_but_used_ones_survive() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-unused-params");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("unused_params.c");
    let generated = tmp.join("unused_params.generated.rs");

    support::translate(&c_src, &generated).expect("translate unused_params fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated unused_params rust");

    assert!(rust.contains("fn add(a: i32) -> i32"));
    assert!(rust.contains("fn get_used(a: i32, b: i32) -> i32"));
    assert!(rust.contains("fn remove_two(a: i32) -> i32"));
    assert!(rust.contains("fn address_taken(a: i32, unused: i32) -> i32"));
    assert!(rust.contains("add(5)"));
    assert!(rust.contains("get_used(1, 2)"));
    assert!(rust.contains("remove_two(3)"));
    assert!(rust.contains("indirect.unwrap()(8, 9)"));
    assert!(!rust.contains("add(5, 10)"));
}

#[test]
fn string_lift_recovers_safe_local_string_buffers() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-string-lift");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("string_lift.c");
    let generated = tmp.join("string_lift.generated.rs");
    support::translate(&c_src, &generated).expect("translate string_lift fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated string_lift rust");

    // greeting's printf converts on its own now, embedding the known constant, so the unused declaration is eliminated
    assert!(!rust.contains("greeting"));
    assert!(rust.contains("println!(\"{}\", \"h\\u{e9}\");"));
    assert!(rust.contains("let mut mutate: [i8; 4] = [97, 98, 99, 0];"));
    assert!(rust.contains("mutate.as_mut_ptr()"));
    assert!(!rust.contains("mutate: &str"));
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
fn ctype_libc_calls_use_ascii_range_classification() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-ctype-libc");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("ctype_libc_fixup.c");
    let generated = tmp.join("ctype_libc_fixup.generated.rs");
    support::translate(&c_src, &generated).expect("translate ctype_libc_fixup fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated ctype_libc rust");

    assert!(rust.contains("(lower as i32) >= 97 && (lower as i32) <= 122"));
    assert!(rust.contains("(upper as i32) >= 65 && (upper as i32) <= 90"));
    assert!(!rust.contains("fn toupper("));
    assert!(!rust.contains("fn tolower("));
    assert!(!rust.contains("unsafe { toupper("));
    assert!(!rust.contains("unsafe { tolower("));
}

#[test]
fn ctype_libc_calls_stay_raw_when_locale_is_not_provably_c() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-ctype-libc-locale");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_only = fixtures_dir().join("ctype_libc_setlocale_c.c");
    let c_only_generated = tmp.join("ctype_libc_setlocale_c.generated.rs");
    support::translate(&c_only, &c_only_generated)
        .expect("translate ctype_libc_setlocale_c fixture");
    let rust = std::fs::read_to_string(&c_only_generated).expect("read generated rust");
    assert!(!rust.contains("fn toupper("));
    assert!(!rust.contains("fn tolower("));

    let non_c = fixtures_dir().join("ctype_libc_setlocale_non_c.c");
    let non_c_generated = tmp.join("ctype_libc_setlocale_non_c.generated.rs");
    support::translate(&non_c, &non_c_generated)
        .expect("translate ctype_libc_setlocale_non_c fixture");
    let rust = std::fs::read_to_string(&non_c_generated).expect("read generated rust");
    assert!(rust.contains("unsafe { toupper("));
    assert!(rust.contains("unsafe { tolower("));
}

#[test]
fn ctype_classify_calls_in_boolean_context_use_ascii_methods() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-ctype-classify");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("ctype_classify_fixup.c");
    let generated = tmp.join("ctype_classify_fixup.generated.rs");
    support::translate(&c_src, &generated).expect("translate ctype_classify_fixup fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated ctype_classify rust");

    assert!(rust.contains(".is_ascii_alphabetic()"));
    assert!(rust.contains(".is_ascii_digit()"));
    assert!(rust.contains(".is_ascii_uppercase()"));
    assert!(rust.contains(".is_ascii_lowercase()"));
    assert!(rust.contains(".is_ascii_alphanumeric()"));
    assert!(rust.contains(".is_ascii_hexdigit()"));
    assert!(rust.contains(".is_ascii_punctuation()"));
    assert!(rust.contains(".is_ascii_control()"));
    assert!(rust.contains(".is_ascii_graphic()"));
    assert!(rust.contains(".is_ascii_whitespace()"));
    assert!(rust.contains("!(") && rust.contains(").is_ascii_alphabetic()"));
    assert!(rust.contains("is_ascii_whitespace() || "));
    assert!(rust.contains("is_ascii_graphic() || "));
    assert!(rust.contains("unsafe { isalpha("));
    assert_eq!(rust.matches("unsafe { isalpha(").count(), 1);
    for function in [
        "isdigit", "isupper", "islower", "isalnum", "isxdigit", "ispunct", "iscntrl", "isgraph",
        "isprint", "isspace",
    ] {
        assert!(!rust.contains(&format!("unsafe {{ {function}(")));
        assert!(!rust.contains(&format!("fn {function}(")));
    }
}

#[test]
fn ctype_classify_calls_stay_raw_when_locale_is_not_provably_c() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-ctype-classify-locale");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_only = fixtures_dir().join("ctype_classify_setlocale_c.c");
    let c_only_generated = tmp.join("ctype_classify_setlocale_c.generated.rs");
    support::translate(&c_only, &c_only_generated)
        .expect("translate ctype_classify_setlocale_c fixture");
    let rust = std::fs::read_to_string(&c_only_generated).expect("read generated rust");
    assert!(rust.contains(".is_ascii_alphabetic()"));

    let non_c = fixtures_dir().join("ctype_classify_setlocale_non_c.c");
    let non_c_generated = tmp.join("ctype_classify_setlocale_non_c.generated.rs");
    support::translate(&non_c, &non_c_generated)
        .expect("translate ctype_classify_setlocale_non_c fixture");
    let rust = std::fs::read_to_string(&non_c_generated).expect("read generated rust");
    assert!(!rust.contains("is_ascii_alphabetic"));
    assert!(rust.contains("isalpha("));
    assert!(rust.contains(".is_ascii_digit()"));
}

#[test]
fn ctype_classify_stays_raw_for_a_parameter_without_a_provable_byte_type() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-ctype-classify-param");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("ctype_classify_param_unsupported.c");
    let generated = tmp.join("ctype_classify_param_unsupported.generated.rs");
    support::translate(&c_src, &generated)
        .expect("translate ctype_classify_param_unsupported fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated rust");
    assert!(!rust.contains("is_ascii_alphabetic"));
    assert!(rust.contains("isalpha("));
}

#[test]
fn value_swaps_use_std_mem_swap_only_when_tmp_is_fully_dead_and_a_ne_b() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-swap");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("swap_fixup.c");
    let generated = tmp.join("swap_fixup.generated.rs");
    support::translate(&c_src, &generated).expect("translate swap_fixup fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated swap rust");

    assert!(rust.contains("std::mem::swap(&mut a, &mut b);"));

    let tmp_reused = &rust[rust.find("fn tmp_reused").expect("find tmp_reused")..];
    assert!(!tmp_reused.contains("std::mem::swap"));
    assert!(tmp_reused.contains("let tmp: i32 = a;"));
    assert!(tmp_reused.contains("a = b;"));
    assert!(tmp_reused.contains("b = tmp;"));

    let self_swap = &rust[rust.find("fn self_swap").expect("find self_swap")..];
    assert!(!self_swap.contains("std::mem::swap"));
    assert!(self_swap.contains("a = a;"));
}

#[test]
fn slice_swaps_use_slice_swap_only_when_tmp_is_fully_dead_and_i_ne_j() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-slice-swap");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("slice_swap_fixup.c");
    let generated = tmp.join("slice_swap_fixup.generated.rs");
    support::translate(&c_src, &generated).expect("translate slice_swap_fixup fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated slice_swap rust");

    let pairwise = &rust[rust.find("fn pairwise_swap(").expect("find pairwise_swap")
        ..rust.find("fn pairwise_swap_tmp_reused").unwrap()];
    assert!(pairwise.contains("items.swap(i as usize, j as usize);"));

    let tmp_reused = &rust[rust
        .find("fn pairwise_swap_tmp_reused")
        .expect("find tmp_reused")
        ..rust.find("fn nested_self_swap").unwrap()];
    assert!(!tmp_reused.contains(".swap("));
    assert!(tmp_reused.contains("let tmp: i32 = items[(i as usize)];"));
    assert!(tmp_reused.contains("items[(i as usize)] = items[(j as usize)];"));
    assert!(tmp_reused.contains("items[(j as usize)] = tmp;"));

    let self_swap = &rust[rust
        .find("fn nested_self_swap")
        .expect("find nested_self_swap")..];
    assert!(!self_swap.contains(".swap("));
    assert!(self_swap.contains("items[(j as usize)] = items[(j as usize)];"));
}

#[test]
fn memchr_calls_rewrite_only_with_proven_extent_and_nul_range() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-memchr-helper");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("mem_memchr.c");
    let generated = tmp.join("mem_memchr.generated.rs");
    support::translate(&c_src, &generated).expect("translate mem_memchr fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated mem_memchr rust");

    assert!(rust.contains("let hit = buf.as_slice().iter().position("));
    assert!(rust.contains("(*__slate_byte as u8) == ((needle as i32) as u8)"));
    assert!(rust.contains("let miss = buf.as_slice().iter().position("));
    assert!(rust.contains("let const_hit = cbuf.as_slice().iter().position("));
    assert!(rust.contains("let nul_after = Some(3);"));
    assert!(rust.contains("fn __slate_memchr("));
    assert!(rust.contains("std::slice::from_raw_parts(bytes, n)"));
    assert_eq!(rust.matches("__slate_memchr(").count(), 6);
    for fallback in [
        "let zero: *mut u8",
        "let nul_equal: *mut i8",
        "let nul_before: *mut i8",
        "let partial: *mut u8",
        "let offset: *mut u8",
    ] {
        assert!(rust.contains(fallback), "{fallback} missing from:\n{rust}");
    }
    assert!(!rust.contains("let mut hit"));
    assert!(!rust.contains("let mut miss"));
    assert!(!rust.contains("let mut nul_after"));
    assert!(!rust.contains("let mut word: [i8; 4]"));
    assert!(!rust.contains("word = [97, 98, 99, 0];"));
    assert!(!rust.contains("map_or(std::ptr::null_mut()"));
    assert!(rust.contains("partial.offset_from("));
    assert!(rust.contains("offset.offset_from("));
    assert!(!rust.contains("miss == std::ptr::null_mut()"));
}

#[test]
fn memchr_zero_use_query_prunes_the_generated_helper() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-memchr-zero-users");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("memchr_nullable_pointer.c");
    let generated = tmp.join("memchr_nullable_pointer.generated.rs");
    support::translate(&c_src, &generated).expect("translate nullable pointer fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated nullable pointer rust");

    assert!(!rust.contains("fn __slate_memchr("));

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("fixup-debug")
        .arg("--debug-only-pass")
        .arg("memchr_prelude")
        .arg(c_src)
        .output()
        .expect("run zero-user memchr cleanup trace");
    assert!(
        output.status.success(),
        "fixup-debug failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let stdout = String::from_utf8(output.stdout).expect("debug output is utf8");
    assert!(stdout.contains("manage_memchr_helper"));
    assert!(stdout.contains("query_case=unused"));
    assert!(stdout.contains("evidence.zero_users=name=__slate_memchr;users=0;complete=true"));
}

#[test]
fn fixup_debug_reports_passes_and_change_summary() {
    let c_src = fixtures_dir().join("mem_memchr.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("fixup-debug")
        .arg(c_src)
        .output()
        .expect("run slate fixup-debug on memchr fixture");

    assert!(
        output.status.success(),
        "fixup-debug failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let stdout = String::from_utf8(output.stdout).expect("debug output is utf8");
    assert!(stdout.contains("early_inline_temps"));
    assert!(stdout.contains("memchr_prelude::fixup_calls"));
    assert!(stdout.contains("late_inline_temps"));
    assert!(stdout.contains("dead_locals"));
    assert!(stdout.contains("rewrite_memchr_call"));
    assert!(stdout.contains("query_case=known_nul"));
    assert!(stdout.contains("query_case=byte_position"));
    assert!(stdout.contains("evidence.prefix_contains=count=4;nul=3"));
    assert!(stdout.contains("rejected_case.known_nul=constant_u8:contradicted"));
    assert!(stdout.contains("manage_memchr_helper"));
    assert!(stdout.contains("query_case=retained"));
    assert!(stdout.contains("rejected_case.unused=zero_users:contradicted"));
    assert!(stdout.contains("changed; stmts"));
    assert!(stdout.contains("temp_lets"));
    assert!(stdout.contains("final: items="));
}

#[test]
fn fixup_debug_can_stop_after_selected_pass() {
    let c_src = fixtures_dir().join("mem_memchr.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("fixup-debug")
        .arg("--up-to-pass")
        .arg("memchr_prelude")
        .arg(c_src)
        .output()
        .expect("run slate fixup-debug up to memchr_prelude");

    assert!(
        output.status.success(),
        "fixup-debug failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let stdout = String::from_utf8(output.stdout).expect("debug output is utf8");
    assert!(stdout.contains("memchr_prelude"));
    assert!(!stdout.contains("array_element_pointer_origin"));
    assert!(!stdout.contains("late_inline_temps"));
}

#[test]
fn fixup_debug_can_run_only_selected_pass() {
    let c_src = fixtures_dir().join("mem_memchr.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("fixup-debug")
        .arg(c_src)
        .arg("--only-pass")
        .arg("late_inline_temps")
        .output()
        .expect("run slate fixup-debug only late_inline_temps");

    assert!(
        output.status.success(),
        "fixup-debug failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let stdout = String::from_utf8(output.stdout).expect("debug output is utf8");
    assert!(stdout.contains("late_inline_temps"));
    assert!(!stdout.contains("early_inline_temps"));
    assert!(!stdout.contains("memchr_prelude"));
}

#[test]
fn fixup_debug_reports_valid_passes_for_unknown_pass() {
    let c_src = fixtures_dir().join("mem_memchr.c");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("fixup-debug")
        .arg(c_src)
        .arg("--only-pass")
        .arg("not_a_pass")
        .output()
        .expect("run slate fixup-debug with bad pass");

    assert!(!output.status.success());
    let stderr = String::from_utf8(output.stderr).expect("debug error is utf8");
    assert!(stderr.contains("unknown pass for --only-pass: not_a_pass"));
    assert!(stderr.contains("valid passes:"));
    assert!(stderr.contains("late_inline_temps"));
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
    assert!(rust.contains("__slate_runtime::parse_i32(s)\n}"));
    assert!(rust.contains("parse_num(s)\n}"));
    assert!(rust.contains("let _v1: usize = s.len();"));
    assert!(rust.contains("forward_num(digits)"));
    assert!(rust.contains("text_len(word)"));
    assert!(!rust.contains("fn atoi("));
    assert!(!rust.contains("fn strlen("));
}

#[test]
fn generated_support_tracks_qualified_ast_users() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-support-module-cleanup");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let c_src = fixtures_dir().join("support_module_cleanup.c");
    let generated = tmp.join("support_module_cleanup.generated.rs");
    support::translate(&c_src, &generated).expect("translate support_module_cleanup fixture");
    let rust =
        std::fs::read_to_string(&generated).expect("read generated support_module_cleanup rust");

    assert!(rust.contains("digits.parse::<i32>().unwrap_or(0)"));
    assert!(!rust.contains("mod __slate_runtime"));
    assert!(!rust.contains("fn atoi("));
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

    assert!(rust.contains("fn sum(items: &[i32], len: i32) -> i32"));
    assert!(rust.contains("fn bump(mut items: &mut [i32], len: i32) {"));
    assert!(!rust.contains("let len: i32 = items.len() as i32;"));
    assert!(rust.contains("total += items[(i as usize)];"));
    assert!(rust.contains("items[(i as usize)] = items[(i as usize)] + 1;"));
    assert!(!rust.contains("let _v7: i32 = items[(i as usize)];"));
    assert!(!rust.contains("items[(i as usize)] = _v8;"));
    assert!(rust.contains("sum(values.as_slice(), 4)"));
    assert!(rust.contains("bump(values.as_mut_slice(), 4)"));
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

    // slice_loop first recovers `for item in items.iter() { total += *item; }`;
    // slice_reduce then collapses that accumulator loop into `.sum()`; unused_params
    // then drops the now-dead `len` parameter from the signature and its call site.
    assert!(rust.contains("fn sum_items(items: &[i32]) -> i32"));
    assert!(rust.contains("let total: i32 = items.iter().sum();"));
    assert!(rust.contains("sum_items(values.as_slice())"));
    assert!(!rust.contains("let len: i32 = items.len() as i32;"));
    assert!(!rust.contains("let item: i32 = items[(i as usize)];"));
    assert!(!rust.contains("for item in items.iter()"));
    assert!(!rust.contains("__slate_item"));
}

#[test]
fn ptr_len_lift_uses_extent_param_not_adjacent_printable() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-ptr-len-nonadjacent");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("ptr_len_nonadjacent.c");
    let generated = tmp.join("ptr_len_nonadjacent.generated.rs");
    support::translate(&c_src, &generated).expect("translate ptr_len_nonadjacent fixture");
    let rust =
        std::fs::read_to_string(&generated).expect("read generated ptr_len_nonadjacent rust");

    assert!(rust.contains("fn sum_items(items: &[i32], printable: i32, length: i32) -> i32"));
    assert!(!rust.contains("let length: i32 = items.len() as i32;"));
    assert!(rust.contains("println!(\"this is another number: {}\", printable);"));
    assert!(rust.contains("sum_items(values.as_slice(), 5, 4)"));
    assert!(!rust.contains("let printable: i32 = items.len() as i32;"));
    assert!(!rust.contains("fn sum_items(items: &[i32])"));
    assert!(!rust.contains("sum_items(values.as_slice())"));
}

#[test]
fn ptr_len_lift_uses_constant_extent_without_consuming_scalar_params() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-ptr-len-const-bound");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("ptr_len_const_bound.c");
    let generated = tmp.join("ptr_len_const_bound.generated.rs");
    support::translate(&c_src, &generated).expect("translate ptr_len_const_bound fixture");
    let rust =
        std::fs::read_to_string(&generated).expect("read generated ptr_len_const_bound rust");

    assert!(rust.contains("fn sum_items(items: &[i32], printable: i32, length: i32) -> i32"));
    assert!(rust.contains("println!(\"this is another number: {} {}\", printable, length);"));
    assert!(rust.contains("for i in 0..4"));
    assert!(rust.contains("total += items[(i as usize)];"));
    assert!(rust.contains("sum_items(values.as_slice(), 5, 4)"));
    assert!(!rust.contains(".offset("));
    assert!(!rust.contains("sum_items(values.as_mut_ptr(), 5, 4)"));
}

#[test]
fn scalar_heap_owner_uses_box_drop() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-heap-box");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("heap_box_fixup.c");
    let generated = tmp.join("heap_box_fixup.generated.rs");
    support::translate(&c_src, &generated).expect("translate heap_box_fixup fixture");
    let rust = std::fs::read_to_string(&generated).expect("read generated heap_box_fixup rust");

    assert!(rust.contains("let mut p: Box<i32> = Box::<i32>::new(41);"));
    assert!(!rust.contains("*p = 41;"));
    assert!(rust.contains("*p = *p + 1;"));
    assert!(rust.contains("println!(\"{}\", *p);"));
    assert!(!rust.contains("let _v11: i32 = *p;"));
    assert!(!rust.contains("*p = _v"));
    assert!(!rust.contains("fn malloc("));
    assert!(!rust.contains("fn free("));
    assert!(!rust.contains("unsafe { malloc("));
    assert!(!rust.contains("unsafe { free("));
}

#[test]
fn scalar_heap_owner_folds_first_store_after_non_reading_statements() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-heap-box-delayed");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("heap_box_delayed_store.c");
    let generated = tmp.join("heap_box_delayed_store.generated.rs");
    support::translate(&c_src, &generated).expect("translate heap_box_delayed_store fixture");
    let rust =
        std::fs::read_to_string(&generated).expect("read generated heap_box_delayed_store rust");

    assert!(rust.contains("let p: Box<i32> = Box::<i32>::new(41);"));
    assert!(rust.contains("let mut marker: i32 = 7;"));
    assert!(rust.contains("marker += 1;"));
    assert!(!rust.contains("*p = 41;"));
}

#[test]
fn heap_owner_bails_out_when_a_use_follows_the_free_call() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-heap-box-uaf");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("heap_box_use_after_free_bailout.c");
    let generated = tmp.join("heap_box_use_after_free_bailout.generated.rs");
    support::translate(&c_src, &generated)
        .expect("translate heap_box_use_after_free_bailout fixture");
    let rust = std::fs::read_to_string(&generated)
        .expect("read generated heap_box_use_after_free_bailout rust");

    assert!(!rust.contains("Box<i32>"));
    assert!(rust.contains("let mut x: *mut i32"));
    assert!(rust.contains("fn free("));
    assert!(rust.contains("unsafe { free("));
}

#[test]
fn interprocedural_alloc_promotes_callee_return_and_caller_binding() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-interproc-alloc");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("interprocedural_alloc_promotion.c");
    let generated = tmp.join("interprocedural_alloc_promotion.generated.rs");
    support::translate(&c_src, &generated)
        .expect("translate interprocedural_alloc_promotion fixture");
    let rust = std::fs::read_to_string(&generated)
        .expect("read generated interprocedural_alloc_promotion rust");

    assert!(rust.contains("fn alloc() -> Vec<i32>"));
    assert!(rust.contains("vec![0; 10usize]"));
    assert!(rust.contains("let mut x: Vec<i32>"));
    assert!(rust.contains("x[0] = 10;"));
    assert!(rust.contains("println!(\"{}\", x[0]);"));
    assert!(!rust.contains("*mut i32"));
    assert!(!rust.contains("unsafe"));
    assert!(!rust.contains("fn malloc("));
}

#[test]
fn interprocedural_alloc_promotion_flows_through_a_pass_through_chain() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-interproc-alloc-chain");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("interprocedural_alloc_promotion_chain.c");
    let generated = tmp.join("interprocedural_alloc_promotion_chain.generated.rs");
    support::translate(&c_src, &generated)
        .expect("translate interprocedural_alloc_promotion_chain fixture");
    let rust = std::fs::read_to_string(&generated)
        .expect("read generated interprocedural_alloc_promotion_chain rust");

    assert!(rust.contains("fn alloc() -> Vec<i32>"));
    assert!(rust.contains("fn indirect() -> Vec<i32>"));
    assert!(rust.contains("let mut x: Vec<i32>"));
    assert!(rust.contains("x[0] = 10;"));
    assert!(!rust.contains("*mut i32"));
    assert!(!rust.contains("unsafe"));
    assert!(!rust.contains("fn malloc("));
}

#[test]
fn interprocedural_alloc_promotion_bails_out_on_pointer_arithmetic() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-interproc-alloc-arith");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("interprocedural_alloc_promotion_arith_bailout.c");
    let generated = tmp.join("interprocedural_alloc_promotion_arith_bailout.generated.rs");
    support::translate(&c_src, &generated)
        .expect("translate interprocedural_alloc_promotion_arith_bailout fixture");
    let rust = std::fs::read_to_string(&generated)
        .expect("read generated interprocedural_alloc_promotion_arith_bailout rust");

    assert!(rust.contains("fn alloc() -> *mut i32"));
    assert!(!rust.contains("Vec<i32>"));
    assert!(rust.contains("fn free("));
    assert!(rust.contains("unsafe { free("));
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
    assert!(!rust.contains("let _v"));
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

#[test]
fn guarded_nullable_local_recovers_option_box() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-option-box-guarded");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("option_box_local_guarded.c");
    let generated = tmp.join("option_box_local_guarded.generated.rs");
    support::translate(&c_src, &generated).expect("translate option_box_local_guarded fixture");
    let rust =
        std::fs::read_to_string(&generated).expect("read generated option_box_local_guarded rust");

    assert!(rust.contains("let mut p: Option<Box<i32>> = None;"));
    assert!(rust.contains("p = Some(Box::<i32>::new(0));"));
    assert!(rust.contains("match p.take() {"));
    assert!(!rust.contains("fn free("));
    assert!(!rust.contains("unsafe { free("));
    assert!(!rust.contains("std::ptr::null_mut()"));
    assert!(!rust.contains("fn malloc("));
    assert!(!rust.contains("unsafe { malloc("));
    assert!(rust.contains("*p = 41;"));
    assert!(rust.contains("let v: i32 = *p;"));
    assert!(!rust.contains("unsafe"));
}

#[test]
fn identity_comparison_between_option_box_locals_uses_ptr_eq() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-option-box-identity");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("option_box_local_identity.c");
    let generated = tmp.join("option_box_local_identity.generated.rs");
    support::translate(&c_src, &generated).expect("translate option_box_local_identity fixture");
    let rust =
        std::fs::read_to_string(&generated).expect("read generated option_box_local_identity rust");

    assert!(rust.contains("let mut p: Option<Box<i32>> = None;"));
    assert!(rust.contains("let mut q: Option<Box<i32>> = None;"));
    assert!(rust.contains("std::ptr::eq("));
    assert!(rust.contains(".as_deref().map_or(std::ptr::null(),"));
    assert!(!rust.contains("p == q"));
    assert!(!rust.contains("fn malloc("));
    assert!(!rust.contains("unsafe"));
}

#[test]
fn pointer_arithmetic_bails_out_of_option_box_recovery() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-option-box-arith");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");
    let c_src = fixtures_dir().join("option_box_local_arith_bailout.c");
    let generated = tmp.join("option_box_local_arith_bailout.generated.rs");
    support::translate(&c_src, &generated)
        .expect("translate option_box_local_arith_bailout fixture");
    let rust = std::fs::read_to_string(&generated)
        .expect("read generated option_box_local_arith_bailout rust");

    assert!(rust.contains("let mut p: *mut i32 = std::ptr::null_mut();"));
    assert!(rust.contains("if p != std::ptr::null_mut()"));
    assert!(rust.contains(".add(1)"));
    assert!(rust.contains("fn free("));
    assert!(!rust.contains("Option<Box"));
}
