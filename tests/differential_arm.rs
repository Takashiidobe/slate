mod support;

use std::path::{Path, PathBuf};

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

fn skip_reason(name: &str) -> Option<&'static str> {
    match name {
        "bitint_shift"
        | "float128"
        | "long_double"
        | "saturating_arith"
        | "switch_case_wide_bitint"
        | "switch_case_wide_bitint_range" => Some(
            "SLATE_CLANG's CIR frontend does not support _BitInt > 128 bits or __float128 \
             for armv7-unknown-linux-gnueabihf (slate-sdks.1)",
        ),
        "stat_struct" => Some(
            "known bug: struct stat field layout is wrong under raw lowering \
             (likely the same root cause as slate-jxmx's nlink_t widening bug)",
        ),
        "gnu_builtin_atomic_flag" => Some(
            "ClangIR limitation: 'cir.atomic.test_and_set' requires an 8-bit signed-integer \
             pointer operand unconditionally, but arm char is unsigned, so CIR itself \
             fails verification here regardless of slate's lowerer",
        ),
        "compound_literal_address"
        | "numeric_parse_fixup"
        | "local_record_same_tag"
        | "builtin_ctzll_shift"
        | "atoi_atof_const_fold"
        | "atoi_atol_prelude_dynamic" => Some(
            "known bug (slate-3f8g.4.16.12): slate lowers C 'long'/'unsigned long'/size_t-\
             returning builtins as 64-bit unconditionally, but armv7-unknown-linux-gnueabihf \
             is ILP32 (32-bit long, confirmed on-device)",
        ),
        "c23_stdlib_memory_management" => Some(
            "environment limitation, not a slate bug: the armv7 sysroot's glibc.so.6 (ARM's \
             official 15.2.rel1 toolchain, glibc 2.42) doesn't export free_sized/\
             free_aligned_sized despite advertising the GLIBC_2.42 version node",
        ),
        "gnu_libc_platform" => Some(
            "environment limitation, not a slate bug: the armv7 sysroot's <sys/syscall.h> \
             doesn't define SYS_gettid",
        ),
        "complex" | "stdlib_complex_cx_limited_range" => Some(
            "known bug (slate-3f8g.4.16.13, needs investigation): double _Complex arithmetic \
             crashes under qemu-arm-static on armv7 hard-float ABI; root cause not yet \
             identified",
        ),
        "int128_arith" | "int128_struct" | "f128_intrinsics" | "c99" | "local_vla"
        | "builtin_alloca" => Some(
            "known bug (slate-3f8g.4.16.14): ClangIR frontend limitation for \
             armv7-unknown-linux-gnueabihf -- rejects __int128/__float128 outright, and \
             requires 'cir.alloca' size operands to be 64-bit even though this is a 32-bit \
             target, failing CIR verification before slate's lowerer ever runs",
        ),
        "stat_mtime_member" => Some(
            "likely the same struct-stat field-layout bug tracked for stat_struct above \
             (st_mtime reads as 0 instead of the real mtime)",
        ),
        "c23_library" => Some(
            "environment limitation, not a slate bug (slate-iow4): the armv7 ARM GNU \
             toolchain 15.2.rel1 sysroot's libc doesn't export memset_explicit, so linking \
             the Rust batch binary fails with an undefined reference",
        ),
        "long_double_complex" => Some(
            "known bug (slate-3f8g.4.16.16): ARM32 f64 long-double complex arithmetic is not \
             differential-exact (last-bit rounding differences in div/div_assign)",
        ),
        _ => None,
    }
}

fn fixtures() -> Vec<(String, PathBuf)> {
    let dir = fixtures_dir();
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut fixtures = Vec::new();
    let paths = support::list_c_fixtures(&dir)
        .into_iter()
        .chain(support::list_c_fixtures(&dir.join("arm")));
    for path in paths {
        let name = path
            .file_stem()
            .and_then(|s| s.to_str())
            .unwrap_or_else(|| panic!("non-UTF8 fixture name: {}", path.display()))
            .to_string();
        if let Some(reason) = skip_reason(&name) {
            eprintln!("skip  {name}: {reason}");
            continue;
        }
        if let Some(reason) =
            support::fixture_target_restriction(&path, "armv7-unknown-linux-gnueabihf")
        {
            eprintln!("skip  {name}: {reason}");
            continue;
        }
        if let Some(selected) = &selected
            && &name != selected
        {
            continue;
        }
        fixtures.push((name, path));
    }
    fixtures.sort();
    fixtures
}

fn ensure_target_env() {
    static ONCE: std::sync::Once = std::sync::Once::new();
    ONCE.call_once(|| {
        unsafe { std::env::set_var("SLATE_TARGET", "armv7-unknown-linux-gnueabihf") };
        if std::env::var("NEXTEST_PROFILE").as_deref() == Ok("arm-lowering") {
            unsafe { std::env::set_var("SLATE_RAW_LOWER", "1") };
        }
    });
}

fn env_or(var: &str, default: &str) -> String {
    std::env::var(var).unwrap_or_else(|_| default.to_string())
}

fn gcc_libdir(linker: &str) -> Option<String> {
    let output = std::process::Command::new(linker)
        .arg("-print-libgcc-file-name")
        .output()
        .ok()?;
    let path = String::from_utf8_lossy(&output.stdout).trim().to_string();
    Path::new(&path)
        .parent()
        .map(|dir| dir.to_string_lossy().into_owned())
}

fn cross_ld(linker: &str) -> Option<String> {
    let prefix = linker.strip_suffix("-gcc")?;
    [format!("{prefix}-ld.bfd"), format!("{prefix}-ld")]
        .into_iter()
        .find(|candidate| Path::new(&candidate).exists())
}

fn arm_target() -> support::CrossTarget {
    let sysroot = env_or("SLATE_ARM_SYSROOT", "/usr/arm-linux-gnueabihf");
    let linker = env_or("SLATE_ARM_LINKER", "arm-linux-gnueabihf-gcc");
    let mut cc_extra_args = vec![
        "--target=armv7-linux-gnueabihf".to_string(),
        format!("--sysroot={sysroot}"),
    ];
    if let Some(libdir) = gcc_libdir(&linker) {
        cc_extra_args.push(format!("-B{libdir}"));
        cc_extra_args.push(format!("-L{libdir}"));
    }
    if let Some(ld) = cross_ld(&linker) {
        cc_extra_args.push(format!("-fuse-ld={ld}"));
    }
    support::CrossTarget {
        rust_triple: "armv7-unknown-linux-gnueabihf",
        cc: env_or("SLATE_ARM_CC", "clang"),
        cc_extra_args,
        cargo_linker_env: "CARGO_TARGET_ARMV7_UNKNOWN_LINUX_GNUEABIHF_LINKER".to_string(),
        linker,
        qemu: env_or("SLATE_ARM_QEMU", "qemu-arm-static"),
        qemu_args: vec!["-L".to_string(), sysroot],
    }
}

#[test]
fn generated_differential_arm() {
    ensure_target_env();

    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated-arm");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let fixtures = fixtures();
    assert!(
        !fixtures.is_empty(),
        "no fixtures found in {:?}",
        fixtures_dir()
    );

    let mut failures = Vec::new();

    let translated = support::parallel_map(&fixtures, |(name, path)| {
        let generated = tmp.join(format!("{name}.generated.rs"));
        let dg_options = support::fixture_dg_options(path);
        let mut extra_args = dg_options.clone();
        extra_args.extend(support::fixture_dg_additional_options(path));
        support::translate_with_args(path, &generated, &extra_args)
            .map(|()| (generated, dg_options))
    });

    let mut cases = Vec::new();
    for ((name, path), result) in fixtures.iter().zip(translated) {
        match result {
            Ok((generated, dg_options)) => {
                let mut config = support::RunConfig::default();
                config.c_args.extend(dg_options);
                cases.push(support::Case {
                    name: name.clone(),
                    c_src: path.clone(),
                    rs_src: generated,
                    config,
                });
            }
            Err(e) => {
                eprintln!("FAIL  {name} (translate)");
                failures.push(format!("[{name}] translate: {e}"));
            }
        }
    }

    let cross = arm_target();
    for (name, result) in support::compare_batch_for_target(&cases, &tmp, Some(&cross)) {
        match result {
            Ok(()) => eprintln!("ok    {name}"),
            Err(e) => {
                eprintln!("FAIL  {name}");
                failures.push(format!("[{name}] {e}"));
            }
        }
    }

    assert!(
        failures.is_empty(),
        "{} fixture(s) failed:\n{}",
        failures.len(),
        failures.join("\n")
    );
}
