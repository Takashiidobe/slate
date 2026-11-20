use clap::{Args, Parser, Subcommand};
use rayon::prelude::*;
use slate::backend::build_script::{BuildScript, Step};
use slate::backend::rust_ast::Cfg;
use slate::backend::{self, codegen, rust_ast};
use slate::frontend::c_shim;
use slate::project::merge::{Unit as SlateUnit, collect_all_errors, rust_ident};
use slate::project::{build_target, matrix, merge, plan};
use slate::{api, compile_commands};
use slate_cargo_toml as cargo_toml;
use std::collections::{BTreeMap, BTreeSet, HashSet};
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
    TranslateProject(RawArgs),
    Project(ProjectArgs),
    #[cfg(feature = "sysroot-install")]
    Sysroot(SysrootArgs),
}

#[derive(Args)]
struct ProjectArgs {
    #[command(subcommand)]
    action: ProjectAction,
}

#[derive(Subcommand)]
enum ProjectAction {
    Target {
        #[command(subcommand)]
        action: TargetAction,
    },
    Feature {
        #[command(subcommand)]
        action: FeatureAction,
    },
}

#[derive(Subcommand)]
enum TargetAction {
    Add {
        manifest: PathBuf,
        name: String,
        compile_commands: PathBuf,
        #[arg(long, default_value = ".")]
        project: PathBuf,
        #[arg(long)]
        triple: Option<String>,
        #[arg(long)]
        flavor: Option<String>,
        #[arg(long)]
        sysroot: Option<String>,
        #[arg(long)]
        build_target: Option<String>,
        #[arg(long)]
        replace: bool,
    },
    Remove {
        manifest: PathBuf,
        #[arg(required = true)]
        names: Vec<String>,
    },
}

#[derive(Subcommand)]
enum FeatureAction {
    Add {
        manifest: PathBuf,
        defines: Vec<String>,
        #[arg(long, default_value = ".")]
        project: PathBuf,
        #[arg(long)]
        name: Option<String>,
        #[arg(long = "define")]
        extra_defines: Vec<String>,
    },
    Remove {
        manifest: PathBuf,
        #[arg(required = true)]
        names: Vec<String>,
    },
    Conflict {
        manifest: PathBuf,
        #[arg(required = true)]
        names: Vec<String>,
    },
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
        Command::TranslateProject(raw) => match raw.args.first() {
            Some(_) => run(translate_project_command(&raw.args)),
            None => ExitCode::from(2),
        },
        Command::Project(args) => run(project_command(args.action)),
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

fn project_command(action: ProjectAction) -> Result<String, String> {
    use slate::project::manifest;
    cli_result(match action {
        ProjectAction::Target {
            action:
                TargetAction::Add {
                    manifest: path,
                    name,
                    compile_commands,
                    project,
                    triple,
                    flavor,
                    sysroot,
                    build_target,
                    replace,
                },
        } => manifest::add_target(&manifest::TargetAdd {
            manifest: &path,
            project: &project,
            name: &name,
            compile_commands: &compile_commands,
            triple: triple.as_deref(),
            flavor: flavor.as_deref(),
            sysroot: sysroot.as_deref(),
            build_target: build_target.as_deref(),
            replace,
        }),
        ProjectAction::Target {
            action:
                TargetAction::Remove {
                    manifest: path,
                    names,
                },
        } => manifest::remove_targets(&path, &names),
        ProjectAction::Feature {
            action:
                FeatureAction::Add {
                    manifest: path,
                    mut defines,
                    project,
                    name,
                    extra_defines,
                },
        } => {
            defines.extend(extra_defines);
            manifest::add_features(&path, &project, &defines, name.as_deref())
        }
        ProjectAction::Feature {
            action:
                FeatureAction::Remove {
                    manifest: path,
                    names,
                },
        } => manifest::remove_features(&path, &names),
        ProjectAction::Feature {
            action:
                FeatureAction::Conflict {
                    manifest: path,
                    names,
                },
        } => manifest::add_conflict(&path, &names),
    })
}

fn cli_result<T, E: std::error::Error + 'static>(result: Result<T, E>) -> Result<T, String> {
    result.map_err(|error| slate::diagnostics::render_error(&error))
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
    let program = slate_analysis::Program::new([&module]);
    let facts = slate_analysis::analyze(&program);
    let lowered = match lowerer::lower(
        &module,
        &files,
        &lowerer::LowerOptions::default(),
        facts.unit(slate_analysis::UnitId(0)),
    ) {
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

const LINK_ORDER_CODEGEN_UNITS: u32 = 4096;
const LINK_ORDER_RUSTFLAGS: [&str; 3] = [
    "-Zcodegen-source-order",
    "-Zhuman-readable-cgu-names",
    "-Zcross-crate-inline-threshold=never",
];

fn crate_stack_protector(programs: &[(String, rust_ast::Program)]) -> Option<&'static str> {
    use slate_parser::compiler_options::StackProtector;
    let level = |program: &rust_ast::Program| match program.stack_protector {
        Some(StackProtector::On) => "basic",
        Some(StackProtector::Strong) => "strong",
        Some(StackProtector::All) => "all",
        Some(StackProtector::Off | StackProtector::Explicit) | None => "none",
    };
    let mut counts: BTreeMap<&str, usize> = BTreeMap::new();
    for (_, program) in programs {
        *counts.entry(level(program)).or_default() += 1;
    }
    let [(&level, _)] = counts.iter().collect::<Vec<_>>()[..] else {
        if counts.is_empty() {
            return None;
        }
        let units: Vec<String> = counts
            .iter()
            .map(|(level, count)| format!("{level} in {count}"))
            .collect();
        eprintln!(
            "warning: units disagree on the stack protector ({}); the crate builds without -Zstack-protector",
            units.join(", ")
        );
        return None;
    };
    if level == "none" {
        return None;
    }
    if programs
        .iter()
        .any(|(_, program)| !program.runtime.has_std())
    {
        eprintln!(
            "warning: -fstack-protector in freestanding or -nostdinc units is not carried over: rustc reads the userspace canary"
        );
        return None;
    }
    if programs
        .iter()
        .any(|(_, program)| program.stack_protector_guard)
    {
        eprintln!(
            "warning: -mstack-protector-guard* moves the canary, which rustc cannot express; the crate builds without -Zstack-protector"
        );
        return None;
    }
    Some(level)
}

fn crate_sanitizer(programs: &[(String, rust_ast::Program)]) -> Option<&'static str> {
    use slate_parser::sanitizers::Instrumentation;
    let mut counts: BTreeMap<&str, usize> = BTreeMap::new();
    for (_, program) in programs {
        let name = program.sanitize.map_or("none", Instrumentation::name);
        *counts.entry(name).or_default() += 1;
    }
    let [(&name, _)] = counts.iter().collect::<Vec<_>>()[..] else {
        if counts.is_empty() {
            return None;
        }
        let units: Vec<String> = counts
            .iter()
            .map(|(name, count)| format!("{name} in {count}"))
            .collect();
        eprintln!(
            "warning: units disagree on -fsanitize= ({}); the crate builds without -Zsanitizer",
            units.join(", ")
        );
        return None;
    };
    match name {
        "none" => None,
        "kernel-address" => {
            eprintln!(
                "warning: -fsanitize=kernel-address is not carried over: the kernel provides the KASAN runtime, so the crate builds without -Zsanitizer"
            );
            None
        }
        _ if programs
            .iter()
            .any(|(_, program)| !program.runtime.has_std()) =>
        {
            eprintln!(
                "warning: -fsanitize={name} in freestanding or -nostdinc units is not carried over; the crate builds without -Zsanitizer"
            );
            None
        }
        _ => Some(name),
    }
}

#[derive(Clone)]
struct TargetFlags {
    triple: String,
    cfg: Option<rust_ast::Cfg>,
    linux: bool,
    rustflags: Vec<String>,
}

struct TargetProfile {
    name: String,
    rustflags: Vec<String>,
}

struct ProfilePlan {
    build_script: bool,
    default_flags: Vec<String>,
    targets: Vec<TargetProfile>,
    guards: Vec<rust_ast::Item>,
}

impl ProfilePlan {
    fn new(targets: &[TargetFlags], links: bool) -> Self {
        let build_script = !targets
            .iter()
            .flat_map(|target| &target.rustflags)
            .any(|flag| flag.starts_with("-Zregparm=") || flag == "-Zreg-struct-return");
        let flags_of = |target: &TargetFlags| {
            let mut flags = target.rustflags.clone();
            if !build_script && links && target.linux {
                flags.push("-Clink-arg=-Wl,-z,nostart-stop-gc".to_string());
            }
            flags
        };
        let Some(default) = targets
            .iter()
            .find(|target| target.triple == env!("SLATE_BUILD_TARGET"))
            .or_else(|| targets.first())
        else {
            return Self {
                build_script,
                default_flags: Vec::new(),
                targets: Vec::new(),
                guards: Vec::new(),
            };
        };
        let mut default_flags = flags_of(default);
        let others: Vec<(&TargetFlags, Vec<String>)> = targets
            .iter()
            .filter(|target| target.triple != default.triple)
            .map(|target| (target, flags_of(target)))
            .filter(|(_, flags)| *flags != default_flags)
            .collect();
        if others.is_empty() {
            return Self {
                build_script,
                default_flags,
                targets: Vec::new(),
                guards: Vec::new(),
            };
        }
        let cfg_name = |profile: &str| {
            format!(
                "slate_profile_{}",
                profile.replace(|c: char| !c.is_ascii_alphanumeric(), "_")
            )
        };
        let names: Vec<String> = std::iter::once("default".to_string())
            .chain(
                others
                    .iter()
                    .map(|(target, _)| target.triple.replace('.', "_")),
            )
            .collect();
        let check_cfg = format!(
            "--check-cfg=cfg({})",
            names
                .iter()
                .map(|name| cfg_name(name))
                .collect::<Vec<_>>()
                .join(", ")
        );
        let guard = |target: &TargetFlags, profile: &str, message: String| {
            target.cfg.clone().map(|cfg| rust_ast::Item::Cfg {
                cfg: rust_ast::Cfg::All(vec![
                    cfg,
                    rust_ast::Cfg::Not(Box::new(rust_ast::Cfg::Flag(cfg_name(profile)))),
                ]),
                item: Box::new(rust_ast::Item::Macro {
                    name: "compile_error".into(),
                    args: vec![rust_ast::Expr::Str(message)],
                }),
            })
        };
        default_flags.extend(["--cfg".to_string(), cfg_name("default"), check_cfg.clone()]);
        let mut guards: Vec<rust_ast::Item> = guard(
            default,
            "default",
            format!("build {} with the dev or release profile", default.triple),
        )
        .into_iter()
        .collect();
        let mut profiles = Vec::new();
        for ((target, mut flags), name) in others.into_iter().zip(names.into_iter().skip(1)) {
            flags.extend(["--cfg".to_string(), cfg_name(&name), check_cfg.clone()]);
            guards.extend(guard(
                target,
                &name,
                format!("build {} with --profile {name}", target.triple),
            ));
            profiles.push(TargetProfile {
                name,
                rustflags: flags,
            });
        }
        Self {
            build_script,
            default_flags,
            targets: profiles,
            guards,
        }
    }
}

fn crate_codegen_rustflags(
    programs: &[(String, rust_ast::Program)],
) -> Result<Vec<String>, String> {
    Ok(crate_stack_protector(programs)
        .map(|level| format!("-Zstack-protector={level}"))
        .into_iter()
        .chain(crate_sanitizer(programs).into_iter().flat_map(|sanitizer| {
            [
                format!("-Zsanitizer={sanitizer}"),
                "-Cunsafe-allow-abi-mismatch=sanitizer".to_string(),
            ]
        }))
        .chain(crate_call_abi_rustflags(programs)?)
        .collect())
}

fn crate_call_abi_rustflags(
    programs: &[(String, rust_ast::Program)],
) -> Result<Vec<String>, String> {
    let settings: BTreeSet<(bool, u8)> = programs
        .iter()
        .map(|(_, program)| (program.reg_struct_return, program.regparm))
        .collect();
    let [(reg_struct_return, regparm)] = settings.iter().copied().collect::<Vec<_>>()[..] else {
        if settings.is_empty() {
            return Ok(Vec::new());
        }
        let units: Vec<String> = programs
            .iter()
            .map(|(stem, program)| {
                format!(
                    "{stem} ({}struct-return, regparm={})",
                    if program.reg_struct_return {
                        "reg-"
                    } else {
                        "pcc-"
                    },
                    program.regparm
                )
            })
            .collect();
        return Err(format!(
            "units disagree on -freg-struct-return or -mregparm, which rustc sets per crate: {}",
            units.join(", ")
        ));
    };
    let mut flags = Vec::new();
    let mut mismatches = Vec::new();
    if reg_struct_return {
        flags.push("-Zreg-struct-return".to_string());
        mismatches.push("reg-struct-return");
    }
    if regparm > 0 {
        flags.push(format!("-Zregparm={regparm}"));
        mismatches.push("regparm");
    }
    if !mismatches.is_empty() {
        flags.push(format!(
            "-Cunsafe-allow-abi-mismatch={}",
            mismatches.join(",")
        ));
    }
    Ok(flags)
}

fn link_order_sensitive(programs: &[(String, rust_ast::Program)]) -> bool {
    fn placement(item: &rust_ast::Item) -> Option<Option<&str>> {
        match item {
            rust_ast::Item::Cfg { item, .. } => placement(item),
            rust_ast::Item::GlobalAsm(_) => Some(None),
            rust_ast::Item::Static { attrs, .. }
                if !attrs.contains(&rust_ast::Attr::ThreadLocal) =>
            {
                attrs.iter().find_map(|attr| match attr {
                    rust_ast::Attr::LinkSection(section) => Some(Some(section.as_str())),
                    _ => None,
                })
            }
            _ => None,
        }
    }
    let mut asm = 0;
    let mut sections = HashSet::new();
    let mut statics = 0;
    for placed in programs
        .iter()
        .flat_map(|(_, program)| &program.items)
        .filter_map(placement)
    {
        match placed {
            None => asm += 1,
            Some(section) => {
                statics += 1;
                if !sections.insert(section) {
                    return true;
                }
            }
        }
    }
    asm >= 2 || (asm >= 1 && statics >= 1)
}

const LINK_ORDER_MAP: &str = "link-order.json";

fn write_link_order_map(
    crate_dir: &Path,
    link_order: bool,
    programs: &[(String, rust_ast::Program)],
    root: Option<&str>,
    unit_files: &BTreeMap<String, PathBuf>,
) -> Result<(), String> {
    let path = crate_dir.join(LINK_ORDER_MAP);
    if !link_order || unit_files.is_empty() {
        return match path.exists() {
            true => {
                std::fs::remove_file(&path).map_err(|e| format!("remove {}: {e}", path.display()))
            }
            false => Ok(()),
        };
    }
    let units: Vec<serde_json::Value> = programs
        .iter()
        .map(|(stem, _)| {
            let module = (root != Some(stem.as_str())).then(|| format!("__slate_unit_{stem}"));
            serde_json::json!({"file": unit_files.get(stem), "module": module})
        })
        .collect();
    let text = serde_json::to_string_pretty(&serde_json::json!({"version": 1, "units": units}))
        .map_err(|e| format!("serialize {}: {e}", path.display()))?;
    write_file(&path, &format!("{text}\n"))
}

fn crate_manifest(package: &str, lib_crate_types: Option<&[String]>) -> cargo_toml::Manifest {
    let mut manifest = cargo_toml::Manifest::new(package, "0.0.0", cargo_toml::Edition::E2024);
    manifest.workspace = Some(cargo_toml::Workspace {});
    if let Some(types) = lib_crate_types {
        manifest.package.autobins = Some(false);
        manifest.lib = Some(cargo_toml::Lib {
            path: Some("src/lib.rs".into()),
            doctest: Some(false),
            crate_type: types.to_vec(),
        });
    }
    manifest.dependencies.extend([
        (
            "slate-support".to_string(),
            cargo_toml::Dependency::path("slate-support"),
        ),
        (
            "aligned".to_string(),
            cargo_toml::Dependency::path("aligned"),
        ),
        ("bitint".to_string(), cargo_toml::Dependency::path("bitint")),
        (
            "num-complex".to_string(),
            cargo_toml::Dependency::path("num-complex").without_default_features(),
        ),
        (
            "bitfields".to_string(),
            cargo_toml::Dependency::path("bitfields"),
        ),
    ]);
    manifest
        .lints
        .entry(cargo_toml::LintTool::Cargo)
        .or_default()
        .insert(
            "unused_dependencies".to_string(),
            cargo_toml::LintLevel::Allow,
        );
    let dev = manifest.profile_mut("dev");
    dev.overflow_checks = Some(false);
    dev.debug_assertions = Some(false);
    dev.codegen_units = Some(256);
    manifest
}

fn make_manifest_freestanding(manifest: &mut cargo_toml::Manifest) {
    if let Some(support) = manifest.dependencies.remove("slate-support") {
        manifest.dependencies.insert(
            "slate-support".to_string(),
            support.without_default_features(),
        );
    }
    for profile in ["dev", "release"] {
        manifest.profile_mut(profile).panic = Some(cargo_toml::Panic::Abort);
    }
}

fn add_manifest_rustflags(
    manifest: &mut cargo_toml::Manifest,
    link_order: bool,
    default_rustflags: &[String],
    target_profiles: &[TargetProfile],
) {
    let package = manifest.package.name.clone();
    let package_profile = |codegen: &[String]| cargo_toml::Profile {
        codegen_units: link_order.then_some(LINK_ORDER_CODEGEN_UNITS),
        rustflags: LINK_ORDER_RUSTFLAGS
            .iter()
            .filter(|_| link_order)
            .map(|flag| flag.to_string())
            .chain(codegen.iter().cloned())
            .collect(),
        ..cargo_toml::Profile::default()
    };
    if link_order {
        manifest.profile_mut("release").incremental = Some(true);
    }
    let default = package_profile(default_rustflags);
    if !default.rustflags.is_empty() {
        for profile in ["dev", "release"] {
            manifest
                .profile_mut(profile)
                .package
                .insert(package.clone(), default.clone());
        }
    }
    for target in target_profiles {
        let profile = manifest.profile_mut(&target.name);
        profile.inherits = Some("release".to_string());
        profile
            .package
            .insert(package.clone(), package_profile(&target.rustflags));
    }
    if manifest
        .profile
        .values()
        .any(cargo_toml::Profile::uses_rustflags)
    {
        manifest
            .cargo_features
            .push(cargo_toml::CargoFeature::ProfileRustflags);
    }
}

fn write_crate_manifest(crate_dir: &Path, manifest: &cargo_toml::Manifest) -> Result<(), String> {
    let path = crate_dir.join("Cargo.toml");
    let text = manifest
        .to_toml()
        .map_err(|e| format!("serialize {}: {e}", path.display()))?;
    write_file(&path, &text)
}

/// Scaffold a Cargo crate at `crate_dir` for translated output: `cargo init`,
/// a manifest with the shared dependencies, and the vendored `aligned` crate.
fn init_crate(crate_dir: &Path, lib_crate_types: Option<&[String]>) -> Result<(), String> {
    let src_dir = crate_dir.join("src");
    std::fs::create_dir_all(&src_dir).map_err(|e| format!("create {}: {e}", src_dir.display()))?;
    let package = package_name(crate_dir);
    write_crate_manifest(crate_dir, &crate_manifest(&package, lib_crate_types))?;
    write_slate_support(crate_dir)?;
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
    backend::write_if_changed(path, contents)
}

include!(concat!(env!("OUT_DIR"), "/vendored_libc.rs"));
include!(concat!(env!("OUT_DIR"), "/vendored_mach2.rs"));
include!(concat!(env!("OUT_DIR"), "/vendored_bitfields.rs"));
include!(concat!(env!("OUT_DIR"), "/vendored_bitfields-impl.rs"));

fn write_vendored_crate(
    crate_dir: &Path,
    name: &str,
    files: &[(&str, &str)],
) -> Result<(), String> {
    let vendored_dir = crate_dir.join(name);
    for (relative, contents) in files {
        let path = vendored_dir.join(relative);
        if let Some(parent) = path.parent() {
            std::fs::create_dir_all(parent)
                .map_err(|e| format!("create {}: {e}", parent.display()))?;
        }
        write_file(&path, contents)?;
    }
    Ok(())
}

fn write_slate_support(crate_dir: &Path) -> Result<(), String> {
    write_vendored_crate(crate_dir, "libc", VENDORED_LIBC)?;
    write_vendored_crate(crate_dir, "mach2", VENDORED_MACH2)?;
    write_vendored_crate(crate_dir, "bitfields", VENDORED_BITFIELDS)?;
    write_vendored_crate(crate_dir, "bitfields-impl", VENDORED_BITFIELDS_IMPL)?;
    let support_dir = crate_dir.join("slate-support");
    let src_dir = support_dir.join("src");
    std::fs::create_dir_all(&src_dir).map_err(|e| format!("create {}: {e}", src_dir.display()))?;
    write_file(
        &support_dir.join("Cargo.toml"),
        include_str!("../vendor/slate-support/Cargo.toml.template"),
    )?;
    write_file(
        &src_dir.join("lib.rs"),
        include_str!("../vendor/slate-support/src/lib.rs"),
    )?;
    write_file(
        &src_dir.join("hosted.rs"),
        include_str!("../vendor/slate-support/src/hosted.rs"),
    )
}

fn write_blocks_support(crate_dir: &Path) -> Result<(), String> {
    let blocks_dir = crate_dir.join("slate-blocks");
    let c_dir = blocks_dir.join("c");
    let src_dir = blocks_dir.join("src");
    for dir in [&c_dir, &src_dir] {
        std::fs::create_dir_all(dir).map_err(|e| format!("create {}: {e}", dir.display()))?;
    }
    for (path, contents) in [
        (
            blocks_dir.join("Cargo.toml"),
            include_str!("../vendor/slate-blocks/Cargo.toml.template"),
        ),
        (
            blocks_dir.join("build.rs"),
            include_str!("../vendor/slate-blocks/build.rs"),
        ),
        (
            src_dir.join("lib.rs"),
            include_str!("../vendor/slate-blocks/src/lib.rs"),
        ),
        (
            c_dir.join("runtime.c"),
            include_str!("../vendor/slate-blocks/c/runtime.c"),
        ),
        (
            c_dir.join("data.c"),
            include_str!("../vendor/slate-blocks/c/data.c"),
        ),
        (
            c_dir.join("Block_private.h"),
            include_str!("../vendor/slate-blocks/c/Block_private.h"),
        ),
        (
            c_dir.join("Block.h"),
            include_str!("../vendor/slate-blocks/c/Block.h"),
        ),
        (
            c_dir.join("config.h"),
            include_str!("../vendor/slate-blocks/c/config.h"),
        ),
    ] {
        write_file(&path, contents)?;
    }
    Ok(())
}

fn write_bitint_support(crate_dir: &Path) -> Result<(), String> {
    let bitint_dir = crate_dir.join("bitint");
    let src_dir = bitint_dir.join("src");
    std::fs::create_dir_all(&src_dir).map_err(|e| format!("create {}: {e}", src_dir.display()))?;
    let manifest = bitint_dir.join("Cargo.toml");
    write_file(
        &manifest,
        include_str!("../vendor/bitint/Cargo.toml.template"),
    )?;
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
        write_file(&path, contents)?;
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
    let mut export_dynamic = false;
    let mut manifest: Option<PathBuf> = None;
    let mut manifest_target = None;
    let mut features: Vec<String> = Vec::new();
    let mut plan_mode = None;
    let mut merge = false;
    let mut projection = merge::Projection::Skip;
    let mut projection_triples: Vec<String> = Vec::new();
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
            "--export-dynamic" => export_dynamic = true,
            "--manifest" => {
                index += 1;
                let path = args
                    .get(index)
                    .ok_or_else(|| "--manifest requires a file".to_string())?;
                if manifest.is_some() {
                    return Err("pass one --manifest".into());
                }
                manifest = Some(PathBuf::from(path));
            }
            "--merge" => merge = true,
            "--check-projection" => {
                index += 1;
                let value = args
                    .get(index)
                    .ok_or_else(|| "--check-projection requires all or a count".to_string())?;
                projection = match value.as_str() {
                    "all" => merge::Projection::All,
                    count => merge::Projection::Sample(count.parse().map_err(|_| {
                        format!("--check-projection requires all or a count, got {count}")
                    })?),
                };
            }
            "--projection-triples" => {
                index += 1;
                let triples = args
                    .get(index)
                    .ok_or_else(|| "--projection-triples requires target triples".to_string())?;
                projection_triples.extend(triples.split(',').map(str::to_string));
            }
            "--plan" => plan_mode = Some(plan::Mode::Plan),
            "--plan-exhaustive" => plan_mode = Some(plan::Mode::CheckExhaustive),
            "--manifest-target" => {
                index += 1;
                manifest_target = Some(
                    args.get(index)
                        .ok_or_else(|| "--manifest-target requires a target name".to_string())?
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
        let manifest = manifest.ok_or("--plan requires --manifest")?;
        if !features.is_empty() {
            return Err("--plan chooses features itself; remove --features".into());
        }
        let [project] = paths.as_slice() else {
            return Err("--plan requires only a project directory".into());
        };
        let project = compile_commands::absolute_path(&current_dir, Path::new(project));
        let matrix = cli_result(matrix::Matrix::read_manifest(&manifest, &project))?;
        let plan =
            slate_worker_pool()?.install(|| plan::plan(&matrix, manifest_target.as_deref(), mode));
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
        let manifest = manifest.ok_or("--merge requires --manifest")?;
        if !features.is_empty() {
            return Err("--merge chooses features itself; remove --features".into());
        }
        if export_dynamic {
            return Err("--export-dynamic is not supported with --merge".into());
        }
        if !compile_command_paths.is_empty() || flavor.is_some() || !include_args.is_empty() {
            return Err(
                "--merge cannot be combined with --compile-commands, --flavor or include options"
                    .into(),
            );
        }
        let project = compile_commands::absolute_path(&current_dir, Path::new(paths[0]));
        let matrix = cli_result(matrix::Matrix::read_manifest(&manifest, &project))?;
        let targets: Vec<String> = manifest_target
            .iter()
            .flat_map(|names: &String| names.split(','))
            .map(str::to_string)
            .collect();
        let merged = slate_worker_pool()?
            .install(|| merge::merge(&matrix, &targets, projection, &projection_triples))?;
        let report = merged.render().to_string();
        print!("{report}");
        let mismatches = merged.mismatches;
        let signature_mismatches = merged.signature_mismatches;
        let target_rustflags = merged
            .target_codegen
            .iter()
            .map(|target| {
                Ok(TargetFlags {
                    triple: target.triple.clone(),
                    cfg: Some(target.cfg.clone()),
                    linux: target.programs.iter().any(|(_, program)| program.linux),
                    rustflags: crate_codegen_rustflags(&target.programs)?,
                })
            })
            .collect::<Result<Vec<_>, String>>()?;
        let written = write_project_crate(
            Path::new(paths[1]),
            merged.units,
            None,
            merged.root,
            &crate_types,
            &merged.features,
            Some(merged.target_guard),
            merged.shared_macros,
            &merged.unit_cfgs,
            &BTreeMap::new(),
            &target_rustflags,
        )?;
        let report_path = Path::new(paths[1]).join("merge-report.txt");
        std::fs::write(&report_path, report)
            .map_err(|error| format!("write {}: {error}", report_path.display()))?;
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
    if projection != merge::Projection::Skip {
        return Err("--check-projection requires --merge".into());
    }
    if build_target.is_some() && compile_command_paths.is_empty() {
        return Err("--build-target requires --compile-commands".into());
    }
    if let Some(manifest) = manifest {
        if !compile_command_paths.is_empty() {
            return Err("--manifest cannot be combined with --compile-commands".into());
        }
        if flavor.is_some() || !include_args.is_empty() {
            return Err("--manifest cannot be combined with --flavor or include options".into());
        }
        let target = manifest_target.ok_or("--manifest requires --manifest-target")?;
        let project = compile_commands::absolute_path(&current_dir, Path::new(paths[0]));
        let matrix = cli_result(matrix::Matrix::read_manifest(&manifest, &project))?;
        let commands = cli_result(matrix.commands(&target, &features))?;
        return translate_slate_project(
            Path::new(paths[1]),
            commands,
            &crate_types,
            explicit_docs_only,
            export_dynamic,
        );
    }
    if manifest_target.is_some() || !features.is_empty() {
        return Err("--manifest-target and --features require --manifest".into());
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
        export_dynamic,
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
    let names = unit_names(&commands)?;
    if names.is_empty() {
        return Err("translate-project: no C translation units in compile commands".into());
    }
    let results: Vec<Result<SlateUnit, String>> = slate_worker_pool()?.install(|| {
        names
            .into_par_iter()
            .zip(commands)
            .map(|(stem, command)| {
                slate::frontend::parse_module_with_compiler_args(&command.file, command.args)
                    .map(|(module, files)| SlateUnit {
                        stem,
                        path: command.file.clone(),
                        module,
                        files,
                    })
                    .map_err(|error| slate::diagnostics::render_error(&error))
            })
            .collect()
    });
    collect_all_errors(results)
}

fn unit_names(commands: &[compile_commands::CompileCommand]) -> Result<Vec<String>, String> {
    let components: Vec<Vec<&str>> = commands
        .iter()
        .map(|command| {
            let stem = command
                .file
                .file_stem()
                .and_then(|stem| stem.to_str())
                .ok_or_else(|| format!("bad file stem: {}", command.file.display()))?;
            let parents = command
                .file
                .parent()
                .into_iter()
                .flat_map(Path::components)
                .rev()
                .filter_map(|component| match component {
                    std::path::Component::Normal(name) => name.to_str(),
                    _ => None,
                });
            Ok(std::iter::once(stem).chain(parents).collect())
        })
        .collect::<Result<_, String>>()?;
    let mut depths = vec![1; commands.len()];
    loop {
        let names: Vec<String> = components
            .iter()
            .zip(&depths)
            .map(|(parts, &depth)| {
                let mut used = parts[..depth].to_vec();
                used.reverse();
                rust_ident(&used.join("_"))
            })
            .collect();
        let mut owners: BTreeMap<&str, Vec<usize>> = BTreeMap::new();
        for (index, name) in names.iter().enumerate() {
            owners.entry(name).or_default().push(index);
        }
        let mut grew = false;
        for indices in owners.values().filter(|indices| indices.len() > 1) {
            for &index in indices {
                if depths[index] < components[index].len() {
                    depths[index] += 1;
                    grew = true;
                }
            }
        }
        if !grew {
            if let Some((name, indices)) = owners.iter().find(|(_, indices)| indices.len() > 1) {
                return Err(format!(
                    "compile commands map both {} and {} to module {name}",
                    commands[indices[0]].file.display(),
                    commands[indices[1]].file.display()
                ));
            }
            return Ok(names);
        }
    }
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
    if matches!(stem, "main" | "lib") {
        format!("__slate_unit_{stem}")
    } else {
        stem.to_string()
    }
}

fn translate_slate_project(
    crate_dir: &Path,
    commands: Vec<compile_commands::CompileCommand>,
    crate_types: &[String],
    explicit_docs_only: bool,
    export_dynamic: bool,
) -> Result<String, String> {
    let unit_files: BTreeMap<PathBuf, PathBuf> = commands
        .iter()
        .map(|command| {
            (
                command.file.clone(),
                compile_commands::relative_path(&command.directory, &command.file),
            )
        })
        .collect();
    let units = parse_slate_units(commands)?;
    let target = build_target::project_build_target(
        units.iter().map(|unit| (unit.stem.as_str(), &unit.module)),
    )?;
    let roots: Vec<&str> = units
        .iter()
        .filter(|unit| {
            unit.module.functions.iter().any(|function| {
                slate::frontend::lowerer::program_entry(&unit.module, function)
                    && function.body.is_some()
            })
        })
        .map(|unit| unit.stem.as_str())
        .collect();
    let root = match roots.as_slice() {
        [root] => Some(root.to_string()),
        [] => None,
        _ => return Err(format!("multiple units define main: {}", roots.join(", "))),
    };
    let mut translated = slate_worker_pool()?.install(|| {
        merge::translate_units(
            &units,
            merge::imported_commons(&units),
            explicit_docs_only,
            root.as_deref().filter(|_| !export_dynamic),
            merge::Sections::ByProject,
            false,
            &merge::symbol_owners(&units),
        )
    })?;
    backend::assign_template_names(translated.programs.iter_mut().map(|(_, program)| program));
    let unit_files: BTreeMap<String, PathBuf> = units
        .iter()
        .map(|unit| (unit.stem.clone(), unit_files[&unit.path].clone()))
        .collect();
    let written = write_project_crate(
        crate_dir,
        translated.programs,
        translated.shared_types,
        root,
        crate_types,
        &BTreeSet::new(),
        None,
        None,
        &BTreeMap::new(),
        &unit_files,
        &[],
    )?;
    build_target::BuildTarget::write_config(target.as_ref(), crate_dir)?;
    Ok(written)
}

const SHARED_MACROS_MODULE: &str = "__slate_macros";

#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
enum CPanic {
    Returns,
    Noreturn,
}

fn c_panic(program: &rust_ast::Program) -> Option<CPanic> {
    let takes_message = |params: &[rust_ast::FnParam]| {
        matches!(
            params.first(),
            Some(rust_ast::FnParam {
                ty: rust_ast::Type::Ptr { .. },
                ..
            })
        )
    };
    let kind = |noreturn: bool| match noreturn {
        true => CPanic::Noreturn,
        false => CPanic::Returns,
    };
    program
        .items
        .iter()
        .filter_map(|item| match item {
            rust_ast::Item::Fn(function)
                if function.name == "panic" && takes_message(&function.params) =>
            {
                Some(kind(function.noreturn))
            }
            rust_ast::Item::ExternBlock { abi, decls } if abi != "llvm-intrinsic" => {
                decls.iter().find_map(|decl| match decl {
                    rust_ast::ExternDecl::Fn(function)
                        if function.name == "panic" && takes_message(&function.params) =>
                    {
                        Some(kind(function.noreturn))
                    }
                    _ => None,
                })
            }
            _ => None,
        })
        .max()
}

#[expect(
    clippy::too_many_arguments,
    reason = "each argument is a distinct project artifact"
)]
fn write_project_crate(
    crate_dir: &Path,
    programs: Vec<(String, rust_ast::Program)>,
    shared_types: Option<rust_ast::Program>,
    root: Option<String>,
    crate_types: &[String],
    declared_features: &BTreeSet<String>,
    target_guard: Option<rust_ast::Item>,
    shared_macros: Option<rust_ast::Program>,
    unit_cfgs: &BTreeMap<String, rust_ast::Cfg>,
    unit_files: &BTreeMap<String, PathBuf>,
    target_rustflags: &[TargetFlags],
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
    let mut shim_gates = Vec::new();
    let shared_module = shared_types.as_ref().map(|_| rust_ast::Item::Mod {
        name: rust_ast::Ident::new(slate::backend::SHARED_TYPES_MODULE),
        path: None,
    });
    let macro_module = shared_macros.as_ref().map(|_| rust_ast::Item::MacroUseMod {
        name: rust_ast::Ident::new(SHARED_MACROS_MODULE),
    });
    let link_arg_target = if root.is_some() {
        Some("bins")
    } else {
        lib_crate_types
            .as_deref()
            .is_some_and(|types| types.iter().any(|ty| ty == "cdylib"))
            .then_some("cdylib")
    };
    let targets = if target_rustflags.is_empty() {
        vec![TargetFlags {
            triple: String::new(),
            cfg: None,
            linux: programs.iter().any(|(_, program)| program.linux),
            rustflags: crate_codegen_rustflags(&programs)?,
        }]
    } else {
        target_rustflags.to_vec()
    };
    let profiles = ProfilePlan::new(&targets, link_arg_target.is_some());
    let children: Vec<rust_ast::Item> = target_guard
        .into_iter()
        .chain(profiles.guards.iter().cloned())
        .chain(macro_module)
        .chain(shared_module)
        .chain(
            programs
                .iter()
                .filter(|(stem, _)| root.as_ref() != Some(stem))
                .map(|(stem, _)| {
                    let declaration = rust_ast::Item::Mod {
                        name: rust_ast::Ident::new(format!("__slate_unit_{stem}")),
                        path: Some(format!("{}.rs", unit_file_stem(stem))),
                    };
                    match unit_cfgs.get(stem) {
                        Some(cfg) => rust_ast::Item::Cfg {
                            cfg: cfg.clone(),
                            item: Box::new(declaration),
                        },
                        None => declaration,
                    }
                }),
        )
        .collect();
    let mut outputs = Vec::new();
    if let Some(program) = shared_macros {
        outputs.push((
            crate_src.join(SHARED_MACROS_MODULE).with_extension("rs"),
            program,
        ));
    }
    if let Some(program) = shared_types {
        outputs.push((
            crate_src
                .join(slate::backend::SHARED_TYPES_MODULE)
                .with_extension("rs"),
            program,
        ));
    }
    let uses_blocks = programs.iter().any(|(_, program)| {
        program.items.iter().any(|item| {
            matches!(item, rust_ast::Item::SupportModule(module) if module.name.as_str() == "__slate_blocks")
        })
    });
    let mut rust_features = BTreeSet::new();
    let mut carried_lints = BTreeSet::new();
    for (_, program) in &programs {
        for item in &program.items {
            if let rust_ast::Item::CrateAttrs(attrs) = item {
                for attr in attrs {
                    match attr {
                        rust_ast::CrateAttr::Feature(feature) => {
                            rust_features.insert(*feature);
                        }
                        rust_ast::CrateAttr::Allow(lints) => {
                            carried_lints.extend(
                                lints
                                    .iter()
                                    .filter(|lint| {
                                        matches!(
                                            lint,
                                            rust_ast::Lint::SuspiciousRuntimeSymbolDefinitions
                                                | rust_ast::Lint::DerefNullptr
                                                | rust_ast::Lint::NamedAsmLabels
                                        )
                                    })
                                    .cloned(),
                            );
                        }
                        _ => {}
                    }
                }
            }
        }
    }
    let carried_allow = (!carried_lints.is_empty())
        .then(|| rust_ast::CrateAttr::Allow(carried_lints.into_iter().collect()));
    let freestanding = !programs.is_empty()
        && programs
            .iter()
            .all(|(_, program)| !program.runtime.has_std());
    let link_order = freestanding || link_order_sensitive(&programs);
    write_link_order_map(
        crate_dir,
        link_order,
        &programs,
        root.as_deref(),
        unit_files,
    )?;
    let no_builtins = programs.iter().any(|(_, program)| program.no_builtins);
    let no_std: Vec<_> = freestanding
        .then_some(rust_ast::CrateAttr::NoStd)
        .into_iter()
        .chain(no_builtins.then_some(rust_ast::CrateAttr::NoBuiltins))
        .collect();
    if freestanding {
        rust_features.insert(rust_ast::Feature::Linkage);
    }
    let panic_handler = freestanding.then(|| {
        let source = match programs
            .iter()
            .filter_map(|(_, program)| c_panic(program))
            .max()
        {
            Some(CPanic::Noreturn) => include_str!("frontend/support/panic_c_noreturn.rs"),
            Some(CPanic::Returns) => include_str!("frontend/support/panic_c.rs"),
            None => include_str!("frontend/support/panic_loop.rs"),
        };
        rust_ast::Item::SupportModule(rust_ast::SupportModule {
            name: rust_ast::Ident::new("__slate_panic"),
            source: source.into(),
            exports: Vec::new(),
        })
    });
    let children: Vec<rust_ast::Item> = children.into_iter().chain(panic_handler).collect();
    for (stem, mut program) in programs {
        program.no_unwind = freestanding;
        let is_root = root.as_ref() == Some(&stem);
        for item in &mut program.items {
            if let rust_ast::Item::CrateAttrs(attrs) = item {
                attrs.retain(|attr| match attr {
                    rust_ast::CrateAttr::Feature(_) | rust_ast::CrateAttr::Allow(_) => false,
                    rust_ast::CrateAttr::NoMain => is_root,
                    _ => true,
                });
            }
        }
        program.cargo_features(&mut cargo_features);
        for (name, gate) in c_shim::collect_program_shim_names(&program) {
            if name.starts_with("__slate_") {
                shim_names.insert(name);
                shim_gates.push(gate);
            }
        }
        let file = if root.as_ref() == Some(&stem) {
            let mut crate_attrs: Vec<_> = no_std
                .clone()
                .into_iter()
                .chain(
                    rust_features
                        .iter()
                        .copied()
                        .map(rust_ast::CrateAttr::Feature),
                )
                .chain(carried_allow.clone())
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
        let crate_attrs = no_std
            .into_iter()
            .chain([rust_ast::CrateAttr::Allow(vec![
                rust_ast::Lint::NonCamelCaseTypes,
                rust_ast::Lint::NonSnakeCase,
                rust_ast::Lint::NonUpperCaseGlobals,
                rust_ast::Lint::RustdocAll,
            ])])
            .chain(carried_allow)
            .chain(
                rust_features
                    .iter()
                    .copied()
                    .map(rust_ast::CrateAttr::Feature),
            )
            .collect();
        let mut items = vec![rust_ast::Item::CrateAttrs(crate_attrs)];
        items.extend(children);
        outputs.push((
            crate_src.join("lib.rs"),
            rust_ast::Program {
                items,
                no_unwind: freestanding,
                ..Default::default()
            },
        ));
    }
    let results: Vec<Result<PathBuf, String>> = slate_worker_pool()?.install(|| {
        outputs
            .into_par_iter()
            .map(|(output, program)| {
                backend::write_if_changed(&output, &program.emit())?;
                Ok(output)
            })
            .collect()
    });
    let mut written = collect_all_errors(results)?;
    let has_shims = !shim_names.is_empty();
    if has_shims && !profiles.build_script {
        return Err(
            "long double C shims need a build script, which rustc cannot compile for the host with -Zregparm or -Zreg-struct-return"
                .to_string(),
        );
    }
    let shim_path = crate_src.join("slate_long_double.c");
    if has_shims {
        write_file(
            &shim_path,
            &c_shim::render_shim_c_source_for_names(&shim_names),
        )?;
        written.push(shim_path);
    } else if shim_path.exists() {
        std::fs::remove_file(&shim_path)
            .map_err(|e| format!("remove {}: {e}", shim_path.display()))?;
    }
    let mut script = BuildScript::new();
    if has_shims {
        script.extend(c_shim::shim_build_steps(&shim_gates));
    }
    if let Some(target) = link_arg_target.filter(|_| profiles.build_script) {
        script.extend([
            Step::RerunIfChanged("build.rs".into()),
            Step::When {
                cfg: Cfg::Opt {
                    key: "target_os".into(),
                    value: "linux".into(),
                },
                steps: vec![Step::LinkArg {
                    target: target.to_string(),
                    arg: "-Wl,-z,nostart-stop-gc".into(),
                }],
            },
        ]);
    }
    let build_script = crate_dir.join("build.rs");
    if script.is_empty() {
        if build_script.exists() {
            std::fs::remove_file(&build_script)
                .map_err(|e| format!("remove {}: {e}", build_script.display()))?;
        }
    } else {
        write_file(&build_script, &script.render())?;
    }
    if uses_blocks {
        write_blocks_support(crate_dir)?;
    }
    let mut manifest = crate_manifest(&package_name(crate_dir), lib_crate_types.as_deref());
    if has_shims {
        manifest
            .build_dependencies
            .insert("cc".to_string(), cargo_toml::Dependency::version("1"));
    }
    if uses_blocks {
        manifest.dependencies.insert(
            "slate-blocks".to_string(),
            cargo_toml::Dependency::path("slate-blocks"),
        );
    }
    if freestanding {
        make_manifest_freestanding(&mut manifest);
    }
    add_manifest_rustflags(
        &mut manifest,
        link_order,
        &profiles.default_flags,
        &profiles.targets,
    );
    manifest.features = cargo_features
        .into_iter()
        .map(|feature| (feature, Vec::new()))
        .collect();
    write_crate_manifest(crate_dir, &manifest)?;
    Ok(written
        .into_iter()
        .map(|path| format!("wrote {}\n", path.display()))
        .collect())
}
