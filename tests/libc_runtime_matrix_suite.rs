mod support;

use std::path::{Path, PathBuf};
use std::process::Command;

use support::libc_probe;
use support::libc_shim::{Architecture, LibcVariant};
use support::{CrossTarget, RunConfig, RustCase};

const FIXTURES: [&str; 3] = ["cstr_literal_arg", "alias_global", "extern_char_ptr_ffi"];

fn env_or(var: &str, default: &str) -> String {
    std::env::var(var).unwrap_or_else(|_| default.to_string())
}

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

fn rustup_target_installed(triple: &str) -> bool {
    let Ok(output) = Command::new("rustc").args(["--print", "sysroot"]).output() else {
        return false;
    };
    if !output.status.success() {
        return false;
    }
    let sysroot = String::from_utf8_lossy(&output.stdout).trim().to_string();
    Path::new(&sysroot)
        .join("lib/rustlib")
        .join(triple)
        .join("lib")
        .is_dir()
}

fn selected_libcs() -> Vec<LibcVariant> {
    match std::env::var("SLATE_LIBC_RUNTIME_LIBC").ok().as_deref() {
        Some("musl") => vec![LibcVariant::Musl],
        Some("glibc") => vec![LibcVariant::Glibc],
        Some(other) => panic!("unknown SLATE_LIBC_RUNTIME_LIBC value: {other}"),
        None => vec![LibcVariant::Musl, LibcVariant::Glibc],
    }
}

fn selected_arches() -> Vec<Architecture> {
    match std::env::var("SLATE_LIBC_RUNTIME_ARCH") {
        Ok(value) => value
            .split(',')
            .filter(|value| !value.is_empty())
            .map(|value| {
                libc_probe::arch_from_key(value)
                    .unwrap_or_else(|| panic!("unknown SLATE_LIBC_RUNTIME_ARCH value: {value}"))
            })
            .collect(),
        Err(_) => libc_probe::ARCHES.to_vec(),
    }
}

fn selected_fixtures() -> Vec<&'static str> {
    let selected = std::env::var("SLATE_LIBC_RUNTIME_FIXTURE").ok();
    FIXTURES
        .iter()
        .copied()
        .filter(|name| selected.as_deref().is_none_or(|f| f == *name))
        .collect()
}

fn resolve_rust_target(
    arch: Architecture,
    libc: LibcVariant,
    config: &libc_probe::ProbeConfig,
) -> Result<(String, Option<CrossTarget>), String> {
    match libc {
        LibcVariant::Glibc if arch == Architecture::X86_64 => {
            Ok(("x86_64-unknown-linux-gnu".to_string(), None))
        }
        LibcVariant::Glibc => {
            let triple = match arch {
                Architecture::X86 => "i686-unknown-linux-gnu",
                Architecture::Arm => "armv7-unknown-linux-gnueabihf",
                Architecture::Aarch64 => "aarch64-unknown-linux-gnu",
                _ => {
                    return Err(format!(
                        "no glibc Rust target for {}",
                        libc_probe::arch_key(arch)
                    ));
                }
            };
            if !rustup_target_installed(triple) {
                return Err(format!("rust target {triple} is not installed"));
            }
            let target_key = triple.replace('-', "_").to_uppercase();
            let rustflags: Vec<String> = config
                .linker_args
                .iter()
                .map(|arg| format!("-C link-arg={arg}"))
                .collect();
            let cargo_env = if rustflags.is_empty() {
                Vec::new()
            } else {
                vec![(
                    format!("CARGO_TARGET_{target_key}_RUSTFLAGS"),
                    rustflags.join(" "),
                )]
            };
            Ok((
                triple.to_string(),
                Some(CrossTarget {
                    rust_triple: triple,
                    cc: String::new(),
                    cc_extra_args: Vec::new(),
                    cargo_linker_env: format!("CARGO_TARGET_{target_key}_LINKER"),
                    linker: config.linker.to_string_lossy().into_owned(),
                    cargo_env,
                    qemu: String::new(),
                    qemu_args: Vec::new(),
                }),
            ))
        }
        LibcVariant::Musl => {
            let (triple, extra_rustflags): (&str, Option<&str>) = match arch {
                Architecture::X86_64 => ("x86_64-unknown-linux-musl", None),
                Architecture::X86 => ("i686-unknown-linux-musl", None),
                Architecture::Arm => (
                    "armv7-unknown-linux-musleabihf",
                    Some("-C link-arg=--target=armv7-linux-musleabihf -C link-arg=-fuse-ld=lld"),
                ),
                Architecture::Aarch64 => (
                    "aarch64-unknown-linux-musl",
                    Some("-C link-arg=--target=aarch64-linux-musl -C link-arg=-fuse-ld=lld"),
                ),
                _ => {
                    return Err(format!(
                        "no musl Rust target for {}",
                        libc_probe::arch_key(arch)
                    ));
                }
            };
            if !rustup_target_installed(triple) {
                return Err(format!("rust target {triple} is not installed"));
            }
            let target_key = triple.replace('-', "_").to_uppercase();
            let cargo_env = extra_rustflags
                .map(|flags| {
                    vec![(
                        format!("CARGO_TARGET_{target_key}_RUSTFLAGS"),
                        flags.to_string(),
                    )]
                })
                .unwrap_or_default();
            Ok((
                triple.to_string(),
                Some(CrossTarget {
                    rust_triple: triple,
                    cc: String::new(),
                    cc_extra_args: Vec::new(),
                    cargo_linker_env: format!("CARGO_TARGET_{target_key}_LINKER"),
                    linker: env_or("SLATE_MUSL_RUST_CC", "clang"),
                    cargo_env,
                    qemu: String::new(),
                    qemu_args: Vec::new(),
                }),
            ))
        }
        _ => unreachable!("libc runtime matrix only covers musl and glibc"),
    }
}

fn run_target(config: &libc_probe::ProbeConfig) -> Option<CrossTarget> {
    let runner = config.runner.as_ref()?;
    Some(CrossTarget {
        rust_triple: "",
        cc: String::new(),
        cc_extra_args: Vec::new(),
        cargo_linker_env: String::new(),
        linker: String::new(),
        cargo_env: Vec::new(),
        qemu: runner.to_string_lossy().into_owned(),
        qemu_args: config.runner_args.clone(),
    })
}

fn run_checked(mut command: Command, label: &str) -> Result<(), String> {
    let output = command
        .output()
        .map_err(|error| format!("{label}: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "{label} failed ({}):\n{}",
            output.status,
            String::from_utf8_lossy(&output.stderr),
        ));
    }
    Ok(())
}

fn compile_oracle(
    config: &libc_probe::ProbeConfig,
    source: &Path,
    work_dir: &Path,
) -> Result<PathBuf, String> {
    std::fs::create_dir_all(work_dir)
        .map_err(|error| format!("create {}: {error}", work_dir.display()))?;
    let object = work_dir.join("oracle.o");
    let executable = work_dir.join("oracle");

    let mut compile = Command::new(&config.compiler);
    compile.args(&config.compiler_args);
    compile.arg(format!("--target={}", config.target));
    compile.arg(format!("--sysroot={}", config.sysroot.display()));
    compile.args(["-std=c23", "-O0"]);
    compile.arg("-c").arg(source).arg("-o").arg(&object);
    run_checked(compile, "compile C oracle")?;

    let mut link = Command::new(&config.linker);
    link.args(&config.linker_args);
    link.arg(&object);
    link.args(&config.linker_post_args);
    link.arg("-o").arg(&executable);
    run_checked(link, "link C oracle")?;

    Ok(executable)
}

#[test]
fn libc_runtime_matrix() {
    let fixtures = selected_fixtures();
    assert!(!fixtures.is_empty(), "no fixtures selected");

    let root = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/libc-runtime-matrix");

    let mut skipped = Vec::new();
    let mut failures = Vec::new();
    let mut targets_passed = 0usize;

    for libc in selected_libcs() {
        for arch in selected_arches() {
            let label = format!("{}/{}", libc.name(), libc_probe::arch_key(arch));

            let config = match libc_probe::resolve(arch, libc) {
                Ok(config) => config,
                Err(reason) => {
                    eprintln!("SKIP {label}: {reason}");
                    skipped.push(label);
                    continue;
                }
            };

            let (rust_triple, rust_cross) = match resolve_rust_target(arch, libc, &config) {
                Ok(resolved) => resolved,
                Err(reason) => {
                    eprintln!("SKIP {label}: {reason}");
                    skipped.push(label);
                    continue;
                }
            };

            let work_dir = root.join(label.replace('/', "-"));
            let run_cross = run_target(&config);

            let mut target_failed = false;
            for &name in &fixtures {
                let case_dir = work_dir.join(name);
                let source = fixtures_dir().join(format!("{name}.c"));

                let c_result = (|| -> Result<(), String> {
                    let generated = case_dir.join(format!("{name}.rs"));
                    unsafe { std::env::set_var("SLATE_TARGET", &rust_triple) };
                    support::translate(&source, &generated)?;

                    let oracle = compile_oracle(&config, &source, &case_dir.join("oracle"))?;

                    let rust_case = RustCase {
                        name: name.to_string(),
                        rs_src: generated,
                    };
                    let batch = support::build_batch(
                        &[rust_case],
                        &case_dir.join("batch_cargo"),
                        &case_dir.join("batch_cargo/src/bin"),
                        1,
                        rust_cross.as_ref(),
                    )?;
                    let rust_bin = batch.executable(name)?;

                    let run_dir = case_dir.join("run");
                    std::fs::create_dir_all(&run_dir)
                        .map_err(|error| format!("create {}: {error}", run_dir.display()))?;

                    let config_run = RunConfig {
                        timeout_seconds: Some(5),
                        ..RunConfig::default()
                    };
                    let c_run = support::run_with_config_for_target(
                        &oracle,
                        &config_run,
                        &run_dir,
                        run_cross.as_ref(),
                    )?;
                    let rust_run = support::run_with_config_for_target(
                        &rust_bin,
                        &config_run,
                        &run_dir,
                        run_cross.as_ref(),
                    )?;
                    support::compare_runs(&c_run, &rust_run, false)
                })();

                match c_result {
                    Ok(()) => println!("  ok    {label}/{name}"),
                    Err(error) => {
                        println!("  FAIL  {label}/{name}: {error}");
                        failures.push(format!("{label}/{name}: {error}"));
                        target_failed = true;
                    }
                }
            }

            if target_failed {
                println!("FAIL {label}");
            } else {
                println!("PASS {label}");
                targets_passed += 1;
            }
        }
    }

    println!(
        "SUMMARY targets_passed={targets_passed} targets_skipped={} fixture_failures={}",
        skipped.len(),
        failures.len()
    );

    assert!(
        failures.is_empty(),
        "libc runtime matrix failed:\n\n{}",
        failures.join("\n\n")
    );
}
