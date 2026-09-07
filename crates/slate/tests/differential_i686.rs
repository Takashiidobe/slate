mod support;

use std::path::{Path, PathBuf};

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

fn fixtures() -> Vec<(String, PathBuf)> {
    let dir = fixtures_dir();
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut fixtures = support::list_c_fixtures(&dir)
        .into_iter()
        .chain(support::list_c_fixtures(&dir.join("i686")))
        .filter_map(|path| {
            let name = path.file_stem()?.to_str()?.to_string();
            if selected.as_ref().is_some_and(|selected| selected != &name) {
                return None;
            }
            if let Some(reason) =
                support::fixture_target_restriction(&path, "i686-unknown-linux-gnu")
            {
                eprintln!("skip  {name}: {reason}");
                return None;
            }
            Some((name, path))
        })
        .collect::<Vec<_>>();
    fixtures.sort();
    fixtures
}

fn ensure_target_env() {
    static ONCE: std::sync::Once = std::sync::Once::new();
    ONCE.call_once(|| {
        unsafe { std::env::set_var("SLATE_TARGET", "i686-unknown-linux-gnu") };
        if std::env::var("NEXTEST_PROFILE").as_deref() == Ok("i686-lowering") {
            unsafe { std::env::set_var("SLATE_RAW_LOWER", "1") };
        }
    });
}

fn env_or(var: &str, default: &str) -> String {
    std::env::var(var).unwrap_or_else(|_| default.to_string())
}

fn i686_target() -> support::CrossTarget {
    let sysroot = env_or("SLATE_I686_SYSROOT", "/usr");
    let clang = env_or("SLATE_I686_CC", "clang");
    let target = "i686-linux-gnu";
    let rust_triple = "i686-unknown-linux-gnu";
    let target_key = rust_triple.replace('-', "_").to_uppercase();
    let cc_target_key = rust_triple.replace('-', "_");
    support::CrossTarget {
        rust_triple,
        cc: clang.clone(),
        cc_extra_args: vec![format!("--target={target}"), "-m32".to_string()],
        cargo_linker_env: format!("CARGO_TARGET_{target_key}_LINKER"),
        linker: clang,
        cargo_env: vec![
            (
                format!("CARGO_TARGET_{target_key}_RUSTFLAGS"),
                "-C link-arg=--target=i686-linux-gnu -C link-arg=-m32".to_string(),
            ),
            (
                format!("CFLAGS_{cc_target_key}"),
                format!("--target={target} -m32"),
            ),
        ],
        qemu: env_or("SLATE_I686_QEMU", "qemu-i386-static"),
        qemu_args: vec!["-L".to_string(), sysroot],
    }
}

#[test]
fn generated_differential_i686() {
    ensure_target_env();

    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated-i686");
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
            Err(error) => {
                eprintln!("FAIL  {name} (translate)");
                failures.push(format!("[{name}] translate: {error}"));
            }
        }
    }

    let cross = i686_target();
    for (name, result) in support::compare_batch_for_target(&cases, &tmp, Some(&cross)) {
        match result {
            Ok(()) => eprintln!("ok    {name}"),
            Err(error) => {
                eprintln!("FAIL  {name}");
                failures.push(format!("[{name}] {error}"));
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
