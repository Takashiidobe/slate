use clap::{Args, Parser, Subcommand};
use rayon::prelude::*;
use slate::backend::{self, codegen, rust_ast};
use slate::frontend::c_shim;
use slate::frontend::preprocess;
use slate::project_merge::{Unit as SlateUnit, collect_all_errors, rust_ident};
use slate::{api, compile_commands, project_matrix, project_merge, project_plan};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::ExitCode;

#[cfg(feature = "sysroot-install")]
mod sysroot;

#[cfg(not(target_family = "wasm"))]
#[global_allocator]
static GLOBAL: mimalloc::MiMalloc = mimalloc::MiMalloc;

#[derive(Parser)]
#[command(name = "slate", version, about = "Translate C to Rust")]
struct Cli {
    #[command(subcommand)]
    command: Command,
}

#[derive(Subcommand)]
enum Command {
    EmitSlateIr(RawArgs),
    Translate(RawArgs),
    LoweringBarriers(RawArgs),
    RecordCfg(RawArgs),
    TranslateProject(RawArgs),
    #[cfg(feature = "sysroot-install")]
    Sysroot(SysrootArgs),
}

#[derive(Args)]
#[command(disable_help_flag = true)]
struct RawArgs {
    #[arg(allow_hyphen_values = true, trailing_var_arg = true)]
    args: Vec<String>,
}

#[cfg(feature = "sysroot-install")]
#[derive(Args)]
struct SysrootArgs {
    #[command(subcommand)]
    action: SysrootAction,
}

#[cfg(feature = "sysroot-install")]
#[derive(Subcommand)]
enum SysrootAction {
    Install(SysrootRawArgs),
    Remove(SysrootRawArgs),
    Path(SysrootRawArgs),
    Doctor(SysrootRawArgs),
}

#[cfg(feature = "sysroot-install")]
#[derive(Args)]
struct SysrootRawArgs {
    #[arg(allow_hyphen_values = true, trailing_var_arg = true)]
    args: Vec<String>,
}

#[cfg(not(target_family = "wasm"))]
const MAIN_STACK_BYTES: usize = 256 << 20;

#[cfg(not(target_family = "wasm"))]
fn main() -> ExitCode {
    std::thread::Builder::new()
        .stack_size(MAIN_STACK_BYTES)
        .spawn(run_cli)
        .ok()
        .and_then(|handle| handle.join().ok())
        .unwrap_or(ExitCode::FAILURE)
}

#[cfg(target_family = "wasm")]
fn main() -> ExitCode {
    run_cli()
}

fn run_cli() -> ExitCode {
    #[cfg(not(target_family = "wasm"))]
    let _ = rayon::ThreadPoolBuilder::new()
        .stack_size(WORKER_STACK_BYTES)
        .build_global();
    let cli = Cli::parse();
    match cli.command {
        Command::EmitSlateIr(raw) => match raw.args.split_last() {
            Some((path, compiler_args)) => run(emit_slate_ir(Path::new(path), compiler_args)),
            None => ExitCode::from(2),
        },
        Command::Translate(raw) => match raw.args.split_last() {
            Some((path, compiler_args)) => {
                if compiler_args
                    .iter()
                    .any(|arg| arg == "--diagnostic-format=json")
                {
                    translate_json(Path::new(path), compiler_args)
                } else {
                    run(translate_with_compiler_args(Path::new(path), compiler_args))
                }
            }
            None => ExitCode::from(2),
        },
        Command::LoweringBarriers(raw) => match raw.args.split_last() {
            Some((path, compiler_args)) => lowering_barriers(Path::new(path), compiler_args),
            None => ExitCode::from(2),
        },
        Command::RecordCfg(raw) => match raw.args.first() {
            Some(path) => run(record_cfg(Path::new(path), &raw.args[1..])),
            None => ExitCode::from(2),
        },
        Command::TranslateProject(raw) => match raw.args.first() {
            Some(_) => run(translate_project_command(&raw.args)),
            None => ExitCode::from(2),
        },
        #[cfg(feature = "sysroot-install")]
        Command::Sysroot(args) => match args.action {
            SysrootAction::Install(raw) => {
                sysroot::main_result(std::iter::once("install".into()).chain(raw.args))
            }
            SysrootAction::Remove(raw) => {
                sysroot::main_result(std::iter::once("remove".into()).chain(raw.args))
            }
            SysrootAction::Path(raw) => {
                sysroot::main_result(std::iter::once("path".into()).chain(raw.args))
            }
            SysrootAction::Doctor(raw) => {
                sysroot::main_result(std::iter::once("doctor".into()).chain(raw.args))
            }
        },
    }
}

fn run(result: Result<String, String>) -> ExitCode {
    match result {
        Ok(text) => {
            print!("{text}");
            ExitCode::SUCCESS
        }
        Err(e) => {
            match e.strip_prefix("error ") {
                Some(_) => eprintln!("{e}"),
                None => eprintln!("error: {e}"),
            }
            ExitCode::FAILURE
        }
    }
}

fn cli_result<T, E: std::fmt::Display>(result: Result<T, E>) -> Result<T, String> {
    result.map_err(|error| error.to_string())
}

fn emit_slate_ir(path: &Path, compiler_args: &[String]) -> Result<String, String> {
    let (module, _) = cli_result(api::slate_ir_with_args(path, compiler_args))?;
    Ok(module.display(false).to_string())
}

fn lowering_barriers(path: &Path, compiler_args: &[String]) -> ExitCode {
    let (module, files) = match cli_result(api::slate_ir_with_args(path, compiler_args)) {
        Ok(parsed) => parsed,
        Err(error) => return run(Err(error)),
    };
    use slate::frontend::lowerer;
    let lowered = match lowerer::lower(&module, &files, &lowerer::LowerOptions::default()) {
        Ok(lowered) => lowered,
        Err(invalid) => {
            println!(
                "<invalid>\t{}\t{}",
                invalid.site.render(&files),
                lowerer::describe_types(&invalid.to_string(), &module)
            );
            return ExitCode::FAILURE;
        }
    };
    for barrier in &lowered.barriers {
        println!(
            "{}\t{}\t{}\t{}",
            barrier.function.as_deref().unwrap_or("<module>"),
            barrier.site.render(&files),
            barrier.construct.kind(),
            lowerer::describe_types(
                barrier
                    .construct
                    .to_string()
                    .lines()
                    .next()
                    .unwrap_or_default(),
                &module
            )
        );
    }
    for function in module
        .functions
        .iter()
        .filter(|function| function.body.is_some())
    {
        if !lowered
            .barriers
            .iter()
            .any(|barrier| barrier.function.as_deref() == Some(function.name.as_str()))
        {
            println!("{}\tok", function.name);
        }
    }
    if lowered.barriers.is_empty() {
        ExitCode::SUCCESS
    } else {
        ExitCode::FAILURE
    }
}

fn translate_with_compiler_args(path: &Path, compiler_args: &[String]) -> Result<String, String> {
    let mut targets = None;
    let mut remaining = Vec::with_capacity(compiler_args.len());
    for arg in compiler_args {
        match arg.strip_prefix("--targets=") {
            Some(value) => targets = Some(value.split(',').map(str::to_string).collect::<Vec<_>>()),
            None => remaining.push(arg.clone()),
        }
    }
    match targets {
        Some(targets) => cli_result(api::translate_targets_with_args(path, &remaining, &targets)),
        None => cli_result(api::translate_with_args(path, &remaining)),
    }
}

fn translate_json(path: &Path, compiler_args: &[String]) -> ExitCode {
    let mut targets = Vec::new();
    let mut remaining = Vec::new();
    for arg in compiler_args {
        if arg == "--diagnostic-format=json" {
            continue;
        }
        match arg.strip_prefix("--targets=") {
            Some(value) => targets = value.split(',').map(str::to_string).collect(),
            None => remaining.push(arg.clone()),
        }
    }
    let (result, diagnostics) = api::translate_with_diagnostics(path, &remaining, &targets);
    let has_errors = diagnostics
        .iter()
        .any(|diagnostic| diagnostic["severity"] == "error");
    eprintln!(
        "{}",
        serde_json::json!({"version": 1, "diagnostics": diagnostics})
    );
    match result {
        Ok(rust) => {
            print!("{rust}");
            if has_errors {
                ExitCode::FAILURE
            } else {
                ExitCode::SUCCESS
            }
        }
        Err(_) => ExitCode::FAILURE,
    }
}

fn package_name(crate_dir: &Path) -> String {
    let name = crate_dir
        .file_name()
        .and_then(|s| s.to_str())
        .map(rust_ident)
        .filter(|s| !s.is_empty())
        .unwrap_or_else(|| "slate_project".into());
    if codegen::is_rust_keyword(&name) {
        format!("slate_{name}")
    } else {
        name
    }
}

fn crate_manifest(
    package: &str,
    tests: &[String],
    slate_support: bool,
    c_shims: bool,
    cargo_features: &BTreeSet<String>,
    lib_crate_types: Option<&[String]>,
) -> String {
    let test_targets: String = tests
        .iter()
        .map(|test| {
            format!(
                r#"
[[test]]
name = "{test}"
path = "tests/{test}.rs"
harness = false
"#
            )
        })
        .collect();
    let support_dependency = if slate_support {
        "slate-support = { path = \"slate-support\" }\n"
    } else {
        ""
    };
    let build_section = if c_shims {
        "\n[build-dependencies]\ncc = \"1\"\n"
    } else {
        ""
    };
    let autobins_line = if lib_crate_types.is_none() {
        ""
    } else {
        "autobins = false\n"
    };
    let lib_section = lib_crate_types.map_or_else(String::new, |types| {
        let types: Vec<String> = types.iter().map(|ty| format!("\"{ty}\"")).collect();
        format!(
            "\n[lib]\npath = \"src/lib.rs\"\ndoctest = false\ncrate-type = [{}]\n",
            types.join(", ")
        )
    });
    let bitfields_path = Path::new(env!("CARGO_MANIFEST_DIR")).join("vendor/bitfields");
    let features_section: String = if cargo_features.is_empty() {
        String::new()
    } else {
        let mut section = "\n[features]\n".to_string();
        for feature in cargo_features {
            section.push_str(&format!("{feature} = []\n"));
        }
        section
    };
    format!(
        r#"[package]
name = "{package}"
version = "0.0.0"
edition = "2024"
{autobins_line}
{lib_section}
[workspace]

[dependencies]
libc = "0.2"
aligned = {{ path = "aligned" }}
bitint = {{ path = "bitint" }}
num-complex = {{ path = "num-complex", default-features = false }}
bitfields = {{ path = "{}" }}
{support_dependency}
{build_section}
[profile.dev]
overflow-checks = false
debug-assertions = false
codegen-units = 256
{test_targets}{features_section}"#,
        bitfields_path.display()
    )
}

fn write_crate_manifest(
    crate_dir: &Path,
    package: &str,
    tests: &[String],
    slate_support: bool,
    c_shims: bool,
    cargo_features: &BTreeSet<String>,
    lib_crate_types: Option<&[String]>,
) -> Result<(), String> {
    std::fs::write(
        crate_dir.join("Cargo.toml"),
        crate_manifest(
            package,
            tests,
            slate_support,
            c_shims,
            cargo_features,
            lib_crate_types,
        ),
    )
    .map_err(|e| format!("write {}: {e}", crate_dir.join("Cargo.toml").display()))
}

/// Scaffold a Cargo crate at `crate_dir` for translated output: `cargo init`,
/// a manifest with the shared dependencies, and the vendored `aligned` crate.
fn init_crate(crate_dir: &Path, lib_crate_types: Option<&[String]>) -> Result<(), String> {
    let src_dir = crate_dir.join("src");
    std::fs::create_dir_all(&src_dir).map_err(|e| format!("create {}: {e}", src_dir.display()))?;
    let package = package_name(crate_dir);
    write_crate_manifest(
        crate_dir,
        &package,
        &[],
        false,
        false,
        &BTreeSet::new(),
        lib_crate_types,
    )?;
    write_aligned_support(crate_dir)?;
    write_bitint_support(crate_dir)?;
    write_num_complex_support(crate_dir)?;
    let stale_root = src_dir.join(if lib_crate_types.is_some() {
        "main.rs"
    } else {
        "lib.rs"
    });
    if stale_root.exists() {
        std::fs::remove_file(&stale_root)
            .map_err(|e| format!("remove {}: {e}", stale_root.display()))?;
    }
    Ok(())
}

fn write_aligned_support(crate_dir: &Path) -> Result<(), String> {
    let aligned_dir = crate_dir.join("aligned");
    let src_dir = aligned_dir.join("src");
    std::fs::create_dir_all(&src_dir).map_err(|e| format!("create {}: {e}", src_dir.display()))?;
    for (path, contents) in [
        (
            aligned_dir.join("Cargo.toml"),
            include_str!("../vendor/aligned/Cargo.toml.template"),
        ),
        (
            aligned_dir.join("LICENSE-MIT"),
            include_str!("../vendor/aligned/LICENSE-MIT"),
        ),
        (
            aligned_dir.join("LICENSE-APACHE"),
            include_str!("../vendor/aligned/LICENSE-APACHE"),
        ),
        (
            src_dir.join("lib.rs"),
            include_str!("../vendor/aligned/src/lib.rs"),
        ),
    ] {
        write_file(&path, contents)?;
    }
    Ok(())
}

fn write_file(path: &Path, contents: &str) -> Result<(), String> {
    std::fs::write(path, contents).map_err(|e| format!("write {}: {e}", path.display()))
}

fn write_bitint_support(crate_dir: &Path) -> Result<(), String> {
    let bitint_dir = crate_dir.join("bitint");
    let src_dir = bitint_dir.join("src");
    std::fs::create_dir_all(&src_dir).map_err(|e| format!("create {}: {e}", src_dir.display()))?;
    let manifest = bitint_dir.join("Cargo.toml");
    std::fs::write(
        &manifest,
        include_str!("../vendor/bitint/Cargo.toml.template"),
    )
    .map_err(|e| format!("write {}: {e}", manifest.display()))?;
    write_file(
        &src_dir.join("lib.rs"),
        include_str!("../vendor/bitint/src/lib.rs"),
    )?;
    Ok(())
}

fn write_num_complex_support(crate_dir: &Path) -> Result<(), String> {
    let num_complex_dir = crate_dir.join("num-complex");
    let src_dir = num_complex_dir.join("src");
    std::fs::create_dir_all(&src_dir).map_err(|e| format!("create {}: {e}", src_dir.display()))?;
    for (path, contents) in [
        (
            num_complex_dir.join("Cargo.toml"),
            include_str!("../vendor/num-complex/Cargo.toml.template"),
        ),
        (
            num_complex_dir.join("LICENSE-MIT"),
            include_str!("../vendor/num-complex/LICENSE-MIT"),
        ),
        (
            num_complex_dir.join("LICENSE-APACHE"),
            include_str!("../vendor/num-complex/LICENSE-APACHE"),
        ),
        (
            num_complex_dir.join("build.rs"),
            include_str!("../vendor/num-complex/build.rs"),
        ),
    ] {
        std::fs::write(&path, contents).map_err(|e| format!("write {}: {e}", path.display()))?;
    }
    for (source, contents) in [
        ("lib.rs", include_str!("../vendor/num-complex/src/lib.rs")),
        (
            "c_ops.rs",
            include_str!("../vendor/num-complex/src/c_ops.rs"),
        ),
        ("cast.rs", include_str!("../vendor/num-complex/src/cast.rs")),
        (
            "complex_float.rs",
            include_str!("../vendor/num-complex/src/complex_float.rs"),
        ),
        (
            "crand.rs",
            include_str!("../vendor/num-complex/src/crand.rs"),
        ),
        ("pow.rs", include_str!("../vendor/num-complex/src/pow.rs")),
    ] {
        write_file(&src_dir.join(source), contents)?;
    }
    write_num_traits_support(crate_dir)
}

fn write_num_traits_support(crate_dir: &Path) -> Result<(), String> {
    let num_traits_dir = crate_dir.join("num-traits");
    let ops_dir = num_traits_dir.join("src/ops");
    std::fs::create_dir_all(&ops_dir).map_err(|e| format!("create {}: {e}", ops_dir.display()))?;
    for (path, contents) in [
        (
            "Cargo.toml",
            include_str!("../vendor/num-traits/Cargo.toml.template"),
        ),
        ("build.rs", include_str!("../vendor/num-traits/build.rs")),
        (
            "LICENSE-MIT",
            include_str!("../vendor/num-traits/LICENSE-MIT"),
        ),
        (
            "LICENSE-APACHE",
            include_str!("../vendor/num-traits/LICENSE-APACHE"),
        ),
        (
            "src/bounds.rs",
            include_str!("../vendor/num-traits/src/bounds.rs"),
        ),
        (
            "src/cast.rs",
            include_str!("../vendor/num-traits/src/cast.rs"),
        ),
        (
            "src/float.rs",
            include_str!("../vendor/num-traits/src/float.rs"),
        ),
        (
            "src/identities.rs",
            include_str!("../vendor/num-traits/src/identities.rs"),
        ),
        (
            "src/int.rs",
            include_str!("../vendor/num-traits/src/int.rs"),
        ),
        (
            "src/lib.rs",
            include_str!("../vendor/num-traits/src/lib.rs"),
        ),
        (
            "src/macros.rs",
            include_str!("../vendor/num-traits/src/macros.rs"),
        ),
        (
            "src/pow.rs",
            include_str!("../vendor/num-traits/src/pow.rs"),
        ),
        (
            "src/real.rs",
            include_str!("../vendor/num-traits/src/real.rs"),
        ),
        (
            "src/sign.rs",
            include_str!("../vendor/num-traits/src/sign.rs"),
        ),
        (
            "src/ops/bytes.rs",
            include_str!("../vendor/num-traits/src/ops/bytes.rs"),
        ),
        (
            "src/ops/checked.rs",
            include_str!("../vendor/num-traits/src/ops/checked.rs"),
        ),
        (
            "src/ops/euclid.rs",
            include_str!("../vendor/num-traits/src/ops/euclid.rs"),
        ),
        (
            "src/ops/inv.rs",
            include_str!("../vendor/num-traits/src/ops/inv.rs"),
        ),
        (
            "src/ops/mod.rs",
            include_str!("../vendor/num-traits/src/ops/mod.rs"),
        ),
        (
            "src/ops/mul_add.rs",
            include_str!("../vendor/num-traits/src/ops/mul_add.rs"),
        ),
        (
            "src/ops/overflowing.rs",
            include_str!("../vendor/num-traits/src/ops/overflowing.rs"),
        ),
        (
            "src/ops/saturating.rs",
            include_str!("../vendor/num-traits/src/ops/saturating.rs"),
        ),
        (
            "src/ops/wrapping.rs",
            include_str!("../vendor/num-traits/src/ops/wrapping.rs"),
        ),
    ] {
        write_file(&num_traits_dir.join(path), contents)?;
    }
    Ok(())
}

fn translate_project_command(args: &[String]) -> Result<String, String> {
    let mut paths = Vec::new();
    let mut compile_command_paths = Vec::new();
    let mut include_args = Vec::new();
    let mut crate_types = Vec::new();
    let mut flavor = None;
    let mut explicit_docs_only = false;
    let mut matrix_path = None;
    let mut matrix_target = None;
    let mut features: Vec<String> = Vec::new();
    let mut plan_mode = None;
    let mut merge = false;
    let mut projection = project_merge::Projection::Skip;
    let mut build_target = None;
    let current_dir = std::env::current_dir().map_err(|e| format!("current directory: {e}"))?;
    let mut index = 0;
    while index < args.len() {
        match args[index].as_str() {
            "--compile-commands" => {
                index += 1;
                let commands = args
                    .get(index)
                    .ok_or_else(|| "--compile-commands requires a file".to_string())?;
                compile_command_paths.push(PathBuf::from(commands));
            }
            "--build-target" => {
                index += 1;
                let target = args
                    .get(index)
                    .ok_or_else(|| "--build-target requires a name".to_string())?;
                build_target = Some(target.as_str());
            }
            "--explicit-docs-only" => explicit_docs_only = true,
            "--matrix" => {
                index += 1;
                let path = args
                    .get(index)
                    .ok_or_else(|| "--matrix requires a file".to_string())?;
                matrix_path = Some(PathBuf::from(path));
            }
            "--merge" => merge = true,
            "--check-projection" => {
                index += 1;
                let value = args
                    .get(index)
                    .ok_or_else(|| "--check-projection requires all or a count".to_string())?;
                projection = match value.as_str() {
                    "all" => project_merge::Projection::All,
                    count => project_merge::Projection::Sample(count.parse().map_err(|_| {
                        format!("--check-projection requires all or a count, got {count}")
                    })?),
                };
            }
            "--plan" => plan_mode = Some(project_plan::Mode::Plan),
            "--plan-exhaustive" => plan_mode = Some(project_plan::Mode::CheckExhaustive),
            "--matrix-target" => {
                index += 1;
                matrix_target = Some(
                    args.get(index)
                        .ok_or_else(|| "--matrix-target requires a target name".to_string())?
                        .clone(),
                );
            }
            "--features" => {
                index += 1;
                let names = args
                    .get(index)
                    .ok_or_else(|| "--features requires feature names".to_string())?;
                features.extend(names.split(',').map(str::to_string));
            }
            "--flavor" => {
                index += 1;
                let name = args
                    .get(index)
                    .ok_or_else(|| "--flavor requires gcc, clang, or msvc".to_string())?;
                flavor = Some(name.parse()?);
            }
            "--crate-type" => {
                index += 1;
                let types = args.get(index).ok_or_else(|| {
                    "--crate-type requires rlib, staticlib, or cdylib".to_string()
                })?;
                for ty in types.split(',') {
                    if !matches!(ty, "rlib" | "staticlib" | "cdylib") {
                        return Err(format!(
                            "unknown crate type {ty}: expected rlib, staticlib, or cdylib"
                        ));
                    }
                    crate_types.push(ty.to_string());
                }
            }
            flag if flag.starts_with('-') => {
                let (option, dir) = match compile_commands::path_option(flag) {
                    Some(compile_commands::PathOption::Separate(option)) => {
                        index += 1;
                        let dir = args
                            .get(index)
                            .ok_or_else(|| format!("{option} requires a directory"))?;
                        (option, dir.as_str())
                    }
                    Some(compile_commands::PathOption::Joined(option, dir)) => (option, dir),
                    None => return Err(format!("unknown translate-project option: {flag}")),
                };
                if !compile_commands::INCLUDE_DIR_OPTIONS.contains(&option) {
                    return Err(format!("unknown translate-project option: {flag}"));
                }
                include_args.push(option.to_string());
                include_args.push(
                    compile_commands::absolute_path(&current_dir, Path::new(dir))
                        .display()
                        .to_string(),
                );
            }
            path => paths.push(path),
        }
        index += 1;
    }
    if let Some(mode) = plan_mode {
        let matrix_path = matrix_path.ok_or("--plan requires --matrix")?;
        if !features.is_empty() {
            return Err("--plan chooses features itself; remove --features".into());
        }
        let [project] = paths.as_slice() else {
            return Err("--plan requires only a project directory".into());
        };
        let project = compile_commands::absolute_path(&current_dir, Path::new(project));
        let matrix = cli_result(project_matrix::Matrix::read(&matrix_path))?;
        let plan = slate_worker_pool()?
            .install(|| project_plan::plan(&matrix, &project, matrix_target.as_deref(), mode));
        let plan = cli_result(plan)?;
        print!("{}", plan.render(matrix.features.len()));
        return match plan.unplanned() {
            0 => Ok(String::new()),
            count => Err(format!("{count} translation units could not be planned")),
        };
    }
    if paths.len() != 2 {
        return Err("translate-project requires a project directory and crate directory".into());
    }
    if merge {
        let matrix_path = matrix_path.ok_or("--merge requires --matrix")?;
        if !features.is_empty() {
            return Err("--merge chooses features itself; remove --features".into());
        }
        if !compile_command_paths.is_empty() || flavor.is_some() || !include_args.is_empty() {
            return Err(
                "--merge cannot be combined with --compile-commands, --flavor or include options"
                    .into(),
            );
        }
        let project = compile_commands::absolute_path(&current_dir, Path::new(paths[0]));
        let matrix = cli_result(project_matrix::Matrix::read(&matrix_path))?;
        let targets: Vec<String> = matrix_target
            .iter()
            .flat_map(|names: &String| names.split(','))
            .map(str::to_string)
            .collect();
        let merged = slate_worker_pool()?
            .install(|| project_merge::merge(&matrix, &project, &targets, projection))?;
        print!("{}", merged.render());
        let mismatches = merged.mismatches;
        let signature_mismatches = merged.signature_mismatches;
        let written = write_project_crate(
            Path::new(paths[1]),
            merged.units,
            None,
            merged.root,
            &crate_types,
            &merged.features,
        )?;
        if mismatches > 0 {
            print!("{written}");
            return Err(format!("{mismatches} projection mismatches"));
        }
        if signature_mismatches > 0 {
            print!("{written}");
            return Err(format!("{signature_mismatches} signature mismatches"));
        }
        return Ok(written);
    }
    if projection != project_merge::Projection::Skip {
        return Err("--check-projection requires --merge".into());
    }
    if build_target.is_some() && compile_command_paths.is_empty() {
        return Err("--build-target requires --compile-commands".into());
    }
    if let Some(matrix_path) = matrix_path {
        if !compile_command_paths.is_empty() {
            return Err("--matrix cannot be combined with --compile-commands".into());
        }
        if flavor.is_some() || !include_args.is_empty() {
            return Err("--matrix cannot be combined with --flavor or include options".into());
        }
        let target = matrix_target.ok_or("--matrix requires --matrix-target")?;
        let project = compile_commands::absolute_path(&current_dir, Path::new(paths[0]));
        let matrix = cli_result(project_matrix::Matrix::read(&matrix_path))?;
        let commands = cli_result(matrix.commands(&project, &target, &features))?;
        return translate_slate_project(
            Path::new(paths[1]),
            commands,
            &crate_types,
            explicit_docs_only,
        );
    }
    if matrix_target.is_some() || !features.is_empty() {
        return Err("--matrix-target and --features require --matrix".into());
    }
    if compile_command_paths.is_empty() {
        return Err("translate-project requires at least one --compile-commands <file>".into());
    }
    let read = cli_result(compile_commands::read(
        &compile_command_paths,
        &compile_commands::ReadOptions {
            extra_args: &include_args,
            flavor,
            build_target,
        },
    ))?;
    if let Some(selected) = &read.selected {
        println!(
            "build target {}: {} files, other targets: {}",
            selected.name,
            read.commands.len(),
            selected.others.join(", ")
        );
    }
    translate_slate_project(
        Path::new(paths[1]),
        read.commands,
        &crate_types,
        explicit_docs_only,
    )
}

fn slate_job_count() -> usize {
    if let Some(value) = std::env::var_os("SLATE_JOBS")
        && let Some(parsed) = value.to_str().and_then(|value| value.parse::<usize>().ok())
    {
        return parsed.max(1);
    }
    let cores = std::thread::available_parallelism()
        .map(std::num::NonZeroUsize::get)
        .unwrap_or(1);
    (cores / 2).max(1)
}

fn parse_slate_units(
    commands: Vec<compile_commands::CompileCommand>,
) -> Result<Vec<SlateUnit>, String> {
    let mut by_stem: BTreeMap<String, compile_commands::CompileCommand> = BTreeMap::new();
    for command in commands {
        let stem = command
            .file
            .file_stem()
            .and_then(|stem| stem.to_str())
            .map(rust_ident)
            .ok_or_else(|| format!("bad file stem: {}", command.file.display()))?;
        if let Some(previous) = by_stem.get(&stem) {
            return Err(format!(
                "compile commands map both {} and {} to module {stem}",
                previous.file.display(),
                command.file.display()
            ));
        }
        by_stem.insert(stem, command);
    }
    if by_stem.is_empty() {
        return Err("translate-project: no C translation units in compile commands".into());
    }
    let results: Vec<Result<SlateUnit, String>> = slate_worker_pool()?.install(|| {
        by_stem
            .into_par_iter()
            .map(|(stem, command)| {
                slate::frontend::parse_module_with_compiler_args(&command.file, command.args)
                    .map(|(module, files)| SlateUnit {
                        stem,
                        path: command.file.clone(),
                        module,
                        files,
                    })
                    .map_err(|error| format!("{}: {error}", command.file.display()))
            })
            .collect()
    });
    collect_all_errors(results)
}

const WORKER_STACK_BYTES: usize = 64 << 20;

fn slate_worker_pool() -> Result<rayon::ThreadPool, String> {
    rayon::ThreadPoolBuilder::new()
        .num_threads(slate_job_count())
        .stack_size(WORKER_STACK_BYTES)
        .build()
        .map_err(|error| format!("build worker pool: {error}"))
}

fn unit_file_stem(stem: &str) -> String {
    if stem == "main" {
        "__slate_unit_main".to_string()
    } else {
        stem.to_string()
    }
}

fn translate_slate_project(
    crate_dir: &Path,
    commands: Vec<compile_commands::CompileCommand>,
    crate_types: &[String],
    explicit_docs_only: bool,
) -> Result<String, String> {
    let units = parse_slate_units(commands)?;
    let roots: Vec<&str> = units
        .iter()
        .filter(|unit| {
            unit.module
                .functions
                .iter()
                .any(|function| function.name == "main" && function.body.is_some())
        })
        .map(|unit| unit.stem.as_str())
        .collect();
    let root = match roots.as_slice() {
        [root] => Some(root.to_string()),
        [] => None,
        _ => return Err(format!("multiple units define main: {}", roots.join(", "))),
    };
    let translated = slate_worker_pool()?.install(|| {
        project_merge::translate_units(
            &units,
            project_merge::imported_commons(&units),
            explicit_docs_only,
            root.as_deref(),
            project_merge::Sections::ByProject,
        )
    })?;
    write_project_crate(
        crate_dir,
        translated.programs,
        translated.shared_types,
        root,
        crate_types,
        &BTreeSet::new(),
    )
}

fn write_project_crate(
    crate_dir: &Path,
    programs: Vec<(String, rust_ast::Program)>,
    shared_types: Option<rust_ast::Program>,
    root: Option<String>,
    crate_types: &[String],
    declared_features: &BTreeSet<String>,
) -> Result<String, String> {
    let lib_crate_types = match &root {
        Some(root) => {
            if !crate_types.is_empty() {
                return Err(format!(
                    "--crate-type applies only to library projects, but {root} defines main"
                ));
            }
            None
        }
        None if crate_types.is_empty() => Some(vec!["rlib".to_string()]),
        None => Some(crate_types.to_vec()),
    };
    init_crate(crate_dir, lib_crate_types.as_deref())?;
    let crate_src = crate_dir.join("src");
    std::fs::create_dir_all(&crate_src)
        .map_err(|e| format!("create {}: {e}", crate_src.display()))?;
    let mut cargo_features = declared_features.clone();
    let mut shim_names = BTreeSet::new();
    let shared_module = shared_types.as_ref().map(|_| rust_ast::Item::Mod {
        name: rust_ast::Ident::new(slate::backend::SHARED_TYPES_MODULE),
        path: None,
    });
    let children: Vec<rust_ast::Item> = shared_module
        .into_iter()
        .chain(
            programs
                .iter()
                .filter(|(stem, _)| root.as_ref() != Some(stem))
                .map(|(stem, _)| rust_ast::Item::Mod {
                    name: rust_ast::Ident::new(format!("__slate_unit_{stem}")),
                    path: Some(format!("{}.rs", unit_file_stem(stem))),
                }),
        )
        .collect();
    let mut outputs = Vec::new();
    if let Some(program) = shared_types {
        outputs.push((
            crate_src
                .join(slate::backend::SHARED_TYPES_MODULE)
                .with_extension("rs"),
            program,
        ));
    }
    let mut rust_features = BTreeSet::new();
    for (_, program) in &programs {
        for item in &program.items {
            if let rust_ast::Item::CrateAttrs(attrs) = item {
                for attr in attrs {
                    if let rust_ast::CrateAttr::Feature(feature) = attr {
                        rust_features.insert(*feature);
                    }
                }
            }
        }
    }
    for (stem, mut program) in programs {
        for item in &mut program.items {
            if let rust_ast::Item::CrateAttrs(attrs) = item {
                attrs.retain(|attr| {
                    !matches!(
                        attr,
                        rust_ast::CrateAttr::Feature(_) | rust_ast::CrateAttr::Allow(_)
                    )
                });
            }
        }
        program.cargo_features(&mut cargo_features);
        shim_names.extend(
            c_shim::collect_program_shim_names(&program)
                .into_iter()
                .filter(|name| name.starts_with("__slate_")),
        );
        let file = if root.as_ref() == Some(&stem) {
            let mut crate_attrs: Vec<_> = rust_features
                .iter()
                .copied()
                .map(rust_ast::CrateAttr::Feature)
                .collect();
            program.items.retain(|item| match item {
                rust_ast::Item::CrateAttrs(attrs) => {
                    crate_attrs.extend(attrs.iter().cloned());
                    false
                }
                _ => true,
            });
            let prologue = program
                .items
                .iter()
                .take_while(|item| {
                    matches!(
                        item,
                        rust_ast::Item::Comment(comment)
                            if comment.attach == rust_ast::CommentAttach::Prologue
                    )
                })
                .count();
            program
                .items
                .splice(prologue..prologue, children.iter().cloned());
            program
                .items
                .insert(0, rust_ast::Item::CrateAttrs(crate_attrs));
            "main".to_string()
        } else {
            unit_file_stem(&stem)
        };
        outputs.push((crate_src.join(file).with_extension("rs"), program));
    }
    if root.is_none() {
        let crate_attrs = [rust_ast::CrateAttr::Allow(vec![
            rust_ast::Lint::NonCamelCaseTypes,
            rust_ast::Lint::RustdocAll,
        ])]
        .into_iter()
        .chain(
            rust_features
                .iter()
                .copied()
                .map(rust_ast::CrateAttr::Feature),
        )
        .collect();
        let mut items = vec![rust_ast::Item::CrateAttrs(crate_attrs)];
        items.extend(children);
        outputs.push((crate_src.join("lib.rs"), rust_ast::Program { items }));
    }
    let results: Vec<Result<PathBuf, String>> = slate_worker_pool()?.install(|| {
        outputs
            .into_par_iter()
            .map(|(output, program)| {
                backend::write_rust(&output, &program.emit())?;
                Ok(output)
            })
            .collect()
    });
    let mut written = collect_all_errors(results)?;
    let has_shims = !shim_names.is_empty();
    if has_shims {
        write_file(
            &crate_dir.join("build.rs"),
            r#"fn main() {
    println!("cargo:rerun-if-changed=src/slate_long_double.c");
    cc::Build::new().file("src/slate_long_double.c").compile("slate_long_double");
}
"#,
        )?;
        let shim_path = crate_src.join("slate_long_double.c");
        std::fs::write(
            &shim_path,
            c_shim::render_shim_c_source_for_names(&shim_names),
        )
        .map_err(|e| format!("write {}: {e}", shim_path.display()))?;
        written.push(shim_path);
    }
    write_crate_manifest(
        crate_dir,
        &package_name(crate_dir),
        &[],
        false,
        has_shims,
        &cargo_features,
        lib_crate_types.as_deref(),
    )?;
    Ok(written
        .into_iter()
        .map(|path| format!("wrote {}\n", path.display()))
        .collect())
}

fn record_cfg(path: &Path, compiler_args: &[String]) -> Result<String, String> {
    let (source, _raw) =
        preprocess::read_source(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let pp = cli_result(preprocess::record_translation_unit(
        path,
        &source,
        compiler_args,
    ))?;
    let directives: Vec<serde_json::Value> = pp
        .directives
        .iter()
        .map(|directive| {
            serde_json::json!({
                "name": directive.name.as_str(),
                "disposition": directive.disposition().as_str(),
                "raw_payload": directive.raw_payload,
                "byte_start": directive.byte_start,
                "byte_end": directive.byte_end,
                "line_start": directive.line_start,
                "line_end": directive.line_end,
                "depth": directive.depth,
                "condition": directive.condition.as_ref().map(preprocess::predicate_text),
                "active": directive.active,
            })
        })
        .collect();
    let chains: Vec<serde_json::Value> = pp
        .chains
        .iter()
        .map(|chain| {
            let branches: Vec<serde_json::Value> = chain
                .branches
                .iter()
                .map(|branch| {
                    serde_json::json!({
                        "kind": branch.kind.as_str(),
                        "raw_predicate": branch.raw_predicate,
                        "directive_line": branch.directive_line,
                        "body_start": branch.body_start,
                        "body_end": branch.body_end,
                        "rust_cfg": branch.rust_cfg.as_ref().map(|c| c.render()),
                        "active": branch.active,
                    })
                })
                .collect();
            serde_json::json!({
                "depth": chain.depth,
                "open_line": chain.open_line,
                "endif_line": chain.endif_line,
                "branches": branches,
            })
        })
        .collect();
    let diagnostics: Vec<serde_json::Value> = pp
        .diagnostics
        .iter()
        .map(|d| {
            serde_json::json!({
                "kind": d.kind.as_str(),
                "line": d.line,
                "message": d.message,
            })
        })
        .collect();
    let doc = serde_json::json!({
        "file": path.to_string_lossy(),
        "directives": directives,
        "chains": chains,
        "diagnostics": diagnostics,
    });
    Ok(format!(
        "{}\n",
        serde_json::to_string_pretty(&doc).map_err(|e| format!("serialize cfg regions: {e}"))?
    ))
}
