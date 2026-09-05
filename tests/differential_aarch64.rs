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
             for aarch64-unknown-linux-gnu (slate-sdks.1)",
        ),
        "stat_struct" => Some(
            "known bug: aarch64 struct stat field layout is wrong under raw lowering \
             (likely the same root cause as slate-jxmx's nlink_t widening bug)",
        ),
        "gnu_builtin_atomic_flag" => Some(
            "ClangIR limitation: 'cir.atomic.test_and_set' requires an 8-bit signed-integer \
             pointer operand unconditionally, but aarch64 char is unsigned, so CIR itself \
             fails verification here regardless of slate's lowerer",
        ),
        _ => None,
    }
}

fn fixtures() -> Vec<(String, PathBuf)> {
    let dir = fixtures_dir();
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut fixtures = Vec::new();
    for entry in std::fs::read_dir(&dir).unwrap_or_else(|e| panic!("read {}: {e}", dir.display())) {
        let path = entry
            .unwrap_or_else(|e| panic!("read {} entry: {e}", dir.display()))
            .path();
        if path.extension().and_then(|e| e.to_str()) != Some("c") {
            continue;
        }
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
            support::fixture_target_restriction(&path, "aarch64-unknown-linux-gnu")
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
        unsafe { std::env::set_var("SLATE_TARGET", "aarch64-unknown-linux-gnu") };
        if std::env::var("NEXTEST_PROFILE").as_deref() == Ok("aarch64-lowering") {
            unsafe { std::env::set_var("SLATE_RAW_LOWER", "1") };
        }
    });
}

fn env_or(var: &str, default: &str) -> String {
    std::env::var(var).unwrap_or_else(|_| default.to_string())
}

fn aarch64_target() -> support::CrossTarget {
    let sysroot = env_or("SLATE_AARCH64_SYSROOT", "/usr/aarch64-linux-gnu");
    let linker = env_or("SLATE_AARCH64_LINKER", "aarch64-linux-gnu-gcc");
    support::CrossTarget {
        rust_triple: "aarch64-unknown-linux-gnu",
        cc: env_or("SLATE_AARCH64_CC", "clang"),
        cc_extra_args: vec![
            "--target=aarch64-linux-gnu".to_string(),
            format!("--sysroot={sysroot}"),
        ],
        cargo_linker_env: "CARGO_TARGET_AARCH64_UNKNOWN_LINUX_GNU_LINKER".to_string(),
        linker,
        qemu: env_or("SLATE_AARCH64_QEMU", "qemu-aarch64-static"),
        qemu_args: vec!["-L".to_string(), sysroot],
    }
}

#[test]
fn generated_differential_aarch64() {
    ensure_target_env();

    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated-aarch64");
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

    let cross = aarch64_target();
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
