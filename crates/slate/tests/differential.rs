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

fn fixture_clang_arg_overrides(name: &str) -> Vec<String> {
    match name {
        "goto_temp_cross_state" => vec!["-O2".to_string()],
        "ptr_param_field_addr_of_mut" => vec!["-O2".to_string()],
        "gnu_asm_register_variable" => vec!["-std=gnu23".to_string()],
        "c23_typeof_unqual" => vec!["-std=gnu23".to_string()],
        _ => Vec::new(),
    }
}

fn fixture_c_ref_std_override(name: &str) -> Option<String> {
    match name {
        "gnu_asm_register_variable" => Some("-std=gnu23".to_string()),
        "c23_typeof_unqual" => Some("-std=gnu23".to_string()),
        _ => None,
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
    let crate_dir = work_dir.join(format!("{name}_crate"));
    let src_dir = crate_dir.join("src");
    std::fs::create_dir_all(&src_dir)
        .map_err(|error| format!("create {}: {error}", src_dir.display()))?;
    std::fs::write(
        crate_dir.join("Cargo.toml"),
        format!(
            "[package]\nname = \"{name}\"\nversion = \"0.0.0\"\nedition = \"2024\"\n\n[dependencies]\nlibc = \"0.2\"\n\n[lib]\npath = \"src/lib.rs\"\n"
        ),
    )
    .map_err(|error| format!("write {}/Cargo.toml: {error}", crate_dir.display()))?;
    std::fs::write(src_dir.join("lib.rs"), rust)
        .map_err(|error| format!("write {}/lib.rs: {error}", src_dir.display()))?;
    let cargo = std::env::var("SLATE_CARGO").unwrap_or_else(|_| "cargo".into());
    let output = Command::new(&cargo)
        .args(["check", "--target", target])
        .arg("--target-dir")
        .arg(work_dir.join("target"))
        .current_dir(&crate_dir)
        .output()
        .map_err(|error| format!("spawn {cargo}: {error}"))?;
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
        && matches!(
            name,
            "fundamental_types"
                | "stdio_locale"
                | "filesystem"
                | "time_signal"
                | "pthread_sync"
                | "network"
        )
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
        support::translate_with_args(&f.path, &generated, &fixture_clang_arg_overrides(&f.name))
            .and_then(|()| {
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
                let mut config = support::RunConfig::default();
                config.c_args.extend(fixture_c_ref_std_override(&f.name));
                Ok(support::Case {
                    name: f.name.clone(),
                    c_src: f.path.clone(),
                    rs_src: generated,
                    config,
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
