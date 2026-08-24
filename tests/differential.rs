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

    fn translation_target(self) -> Option<&'static str> {
        match self {
            FixtureFlavor::Default => None,
            FixtureFlavor::Bionic => Some("aarch64-linux-android"),
            FixtureFlavor::Macos => Some("aarch64-apple-darwin"),
            FixtureFlavor::Msvc => None,
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

    fn check_prefixes(self, profile: support::filecheck::Profile) -> &'static [&'static str] {
        use support::filecheck::Profile::{Lowering, Rewrites};
        match (self, profile) {
            (FixtureFlavor::Default, _) => &[],
            (FixtureFlavor::Bionic, Lowering) => &["LOWERING-BIONIC-AARCH64"],
            (FixtureFlavor::Bionic, Rewrites) => &["REWRITES-BIONIC-AARCH64"],
            (FixtureFlavor::Macos, Lowering) => &["LOWERING-MACOS"],
            (FixtureFlavor::Macos, Rewrites) => &["REWRITES-MACOS"],
            (FixtureFlavor::Msvc, Lowering) => &["LOWERING-MSVC"],
            (FixtureFlavor::Msvc, Rewrites) => &["REWRITES-MSVC"],
        }
    }

    fn selection_prefixes(self, profile: support::filecheck::Profile) -> &'static [&'static str] {
        match (self, profile) {
            (FixtureFlavor::Bionic, support::filecheck::Profile::Lowering) => {
                &["LOWERING-BIONIC-AARCH64", "LOWERING-BIONIC-X86_64"]
            }
            (FixtureFlavor::Bionic, support::filecheck::Profile::Rewrites) => {
                &["REWRITES-BIONIC-AARCH64", "REWRITES-BIONIC-X86_64"]
            }
            _ => self.check_prefixes(profile),
        }
    }
}

struct Fixture {
    name: String,
    path: PathBuf,
    flavor: FixtureFlavor,
}

struct TargetCheckFixture {
    name: &'static str,
    path: PathBuf,
    target: &'static str,
    prefix: &'static str,
}

fn target_check_fixtures(profile: support::filecheck::Profile) -> Vec<TargetCheckFixture> {
    let prefix = match profile {
        support::filecheck::Profile::Lowering => "LOWERING-X86_64-GNU",
        support::filecheck::Profile::Rewrites => "REWRITES-X86_64-GNU",
    };
    let fixture = TargetCheckFixture {
        name: "long_double_x86_64_gnu",
        path: fixtures_dir().join("../fixtures.link/long_double/main.c"),
        target: "x86_64-unknown-linux-gnu",
        prefix,
    };
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    if selected
        .as_ref()
        .is_some_and(|selected| selected != fixture.name)
    {
        return Vec::new();
    }
    let has_checks = std::fs::read_to_string(&fixture.path).is_ok_and(|source| {
        support::filecheck::has_checks_with_prefixes(&source, profile, &[fixture.prefix])
    });
    has_checks.then_some(fixture).into_iter().collect()
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
    if selected.is_none() {
        let profile = support::filecheck::Profile::active();
        fixtures.retain(|fixture| {
            if fixture.flavor == FixtureFlavor::Msvc {
                return true;
            }
            std::fs::read_to_string(&fixture.path).is_ok_and(|source| {
                support::filecheck::has_checks_with_prefixes(
                    &source,
                    profile,
                    fixture.flavor.selection_prefixes(profile),
                )
            })
        });
    }
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

fn check_generated_rust_for_target(
    name: &str,
    target: &str,
    rust: &str,
    work_dir: &Path,
) -> Result<(), String> {
    let rustc = std::env::var("RUSTC").unwrap_or_else(|_| "rustc".into());
    let libdir = Command::new(&rustc)
        .args(["--print", "target-libdir", "--target", target])
        .output()
        .map_err(|error| format!("spawn {rustc}: {error}"))?;
    if !libdir.status.success() {
        return Err(String::from_utf8_lossy(&libdir.stderr).into_owned());
    }
    let libdir = PathBuf::from(String::from_utf8_lossy(&libdir.stdout).trim());
    if !libdir.is_dir() {
        eprintln!("skip  {name} Rust target check: {target} is not installed");
        return Ok(());
    }
    std::fs::create_dir_all(work_dir)
        .map_err(|error| format!("create {}: {error}", work_dir.display()))?;
    let source = work_dir.join(format!("{name}.rs"));
    let metadata = work_dir.join(format!("lib{name}.rmeta"));
    std::fs::write(&source, rust)
        .map_err(|error| format!("write {}: {error}", source.display()))?;
    let output = Command::new(&rustc)
        .args(["--edition=2024", "--crate-type=lib", "--emit=metadata"])
        .arg("--target")
        .arg(target)
        .arg("-o")
        .arg(metadata)
        .arg(source)
        .output()
        .map_err(|error| format!("spawn {rustc}: {error}"))?;
    output.status.success().then_some(()).ok_or_else(|| {
        format!(
            "generated Rust target check failed:\n{}",
            String::from_utf8_lossy(&output.stderr)
        )
    })
}

fn run_cross_target_fixture(
    name: &str,
    flavor: FixtureFlavor,
    target: &str,
    path: &Path,
    work_dir: &Path,
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
    if flavor == FixtureFlavor::Msvc {
        return Ok(());
    }
    if flavor == FixtureFlavor::Macos
        && matches!(name, "fundamental_types" | "stdio_locale")
        && let Ok(sdk) = std::env::var("SLATE_MACOS_SDK")
        && !sdk.trim().is_empty()
    {
        let object_file = Path::new(env!("CARGO_MANIFEST_DIR")).join(format!(
            "target/test-cache/cross_target_fixture_{name}_macos_sdk.o"
        ));
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
    let fixture = std::fs::read_to_string(path)
        .map_err(|error| format!("read {}: {error}", path.display()))?;
    let profile = support::filecheck::Profile::active();
    let rust = translate_cross_target(
        path,
        flavor.translation_target().unwrap(),
        (flavor == FixtureFlavor::Bionic).then_some("21"),
    )?;
    if flavor == FixtureFlavor::Macos {
        check_generated_rust_for_target(
            name,
            flavor.translation_target().unwrap(),
            &rust,
            &work_dir.join("target-check"),
        )?;
    }
    support::filecheck::check_generated_rust_with_prefixes(
        &fixture,
        &rust,
        profile,
        flavor.check_prefixes(profile),
        &work_dir.join(flavor.name()),
    )?;
    if flavor == FixtureFlavor::Bionic {
        let rust = translate_cross_target(path, "x86_64-linux-android", Some("21"))?;
        support::filecheck::check_generated_rust_with_prefixes(
            &fixture,
            &rust,
            profile,
            match profile {
                support::filecheck::Profile::Lowering => &["LOWERING-BIONIC-X86_64"],
                support::filecheck::Profile::Rewrites => &["REWRITES-BIONIC-X86_64"],
            },
            &work_dir.join("bionic-x86_64"),
        )?;
    }
    Ok(())
}

fn translate_cross_target(
    path: &Path,
    target: &str,
    android_api: Option<&str>,
) -> Result<String, String> {
    let mut command = Command::new(env!("CARGO_BIN_EXE_slate"));
    command
        .args(["translate", path.to_str().unwrap()])
        .env("SLATE_TARGET", target);
    if let Some(api) = android_api {
        command.env("SLATE_ANDROID_API", api);
    }
    let output = command
        .output()
        .map_err(|error| format!("translate {} for {target}: {error}", path.display()))?;
    if !output.status.success() {
        return Err(format!(
            "translation failed for {target}:\n{}",
            String::from_utf8_lossy(&output.stderr)
        ));
    }
    String::from_utf8(output.stdout)
        .map_err(|error| format!("generated Rust is not UTF-8: {error}"))
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
    let profile = support::filecheck::Profile::active();
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let fixtures = fixtures();
    let target_check_fixtures = target_check_fixtures(profile);
    assert!(
        !fixtures.is_empty() || !target_check_fixtures.is_empty(),
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
        support::translate(&f.path, &generated).and_then(|()| {
            let fixture = std::fs::read_to_string(&f.path)
                .map_err(|e| format!("read {}: {e}", f.path.display()))?;
            let rust = std::fs::read_to_string(&generated)
                .map_err(|e| format!("read {}: {e}", generated.display()))?;
            support::filecheck::check_generated_rust(
                &fixture,
                &rust,
                support::filecheck::Profile::active(),
                &tmp.join("filecheck").join(&f.name),
            )?;
            Ok(support::Case {
                name: f.name.clone(),
                c_src: f.path.clone(),
                rs_src: generated,
                config: support::RunConfig::default(),
            })
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
        run_cross_target_fixture(
            &f.name,
            f.flavor,
            f.flavor.target().unwrap(),
            &f.path,
            &tmp.join("filecheck").join(&f.name),
        )
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

    let target_check_results = support::parallel_map(&target_check_fixtures, |fixture| {
        let source = std::fs::read_to_string(&fixture.path)
            .map_err(|error| format!("read {}: {error}", fixture.path.display()))?;
        let rust = translate_cross_target(&fixture.path, fixture.target, None)?;
        support::filecheck::check_generated_rust_with_prefixes(
            &source,
            &rust,
            profile,
            &[fixture.prefix],
            &tmp.join("filecheck").join(fixture.name),
        )
    });
    for (fixture, result) in target_check_fixtures.iter().zip(target_check_results) {
        match result {
            Ok(()) => eprintln!("ok    {} ({})", fixture.name, fixture.target),
            Err(error) => {
                eprintln!("FAIL  {} ({})", fixture.name, fixture.target);
                failures.push(format!("[{} ({})] {error}", fixture.name, fixture.target));
            }
        }
    }

    if !failures.is_empty() {
        panic!(
            "{} of {} generated fixtures failed:\n\n{}",
            failures.len(),
            fixtures.len() + target_check_fixtures.len(),
            failures.join("\n\n")
        );
    }
}

/*
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
fn anonymous_local_structs_report_complete_fixup_domain() {
    let c_src = fixtures_dir().join("anon_local_struct.c");

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
fn anonymous_struct_name_collisions_report_incomplete_fixup_domain() {
    let c_src = fixtures_dir().join("anon_struct_name_collision.c");

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
fn address_of_array_elements_report_known_origins() {
    let c_src = fixtures_dir().join("address_of_array_element.c");

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
fn unnecessary_promotion_casts_report_resolved_proofs() {
    let c_src = fixtures_dir().join("unnecessary_casts.c");

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
fn noreturn_c11_reports_never_returning_extern_proof() {
    let c_src = fixtures_dir().join("noreturn_c11_spelling.c");

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
fn memchr_zero_use_query_reports_unused_helper() {
    let c_src = fixtures_dir().join("memchr_nullable_pointer.c");

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

*/
