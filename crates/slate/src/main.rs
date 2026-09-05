use clang_ir::ast::Type as CirType;
use clang_ir::model::Module;
use rayon::prelude::*;
use slate::backend::{self, codegen, rust_ast};
use slate::frontend::{self, c_ast, c_shim, directive_translate, preprocess};
use slate::{api, compile_commands, ctx};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};

#[global_allocator]
static GLOBAL: mimalloc::MiMalloc = mimalloc::MiMalloc;

fn usage() -> ExitCode {
    eprintln!("usage: slate <command> [file.c]");
    eprintln!("  emit-cir    print ClangIR (generic form)");
    eprintln!("  emit-fixtures  write every supported test fixture to sibling .generated dirs");
    eprintln!(
        "  emit-lowered-fixtures  write raw lowered test fixtures to tests/fixtures.lowered.generated/"
    );
    eprintln!(
        "  fixup-debug  <file.c> [--up-to-pass <pass>|--only-pass <pass>|--debug-only-pass <pass>]  print fixup pass trace"
    );
    eprintln!("  translate   [clang args...] <file.c>  C -> Rust");
    eprintln!(
        "  translate-lowered  <file.c>  C -> Rust, raw lowered output with no fixup passes applied"
    );
    eprintln!("  translate-directives   experimental multi-config C -> Rust");
    eprintln!("  record-cfg   <file.c> [clang args...]  print preprocessor cfg regions as JSON");
    eprintln!(
        "  translate-project [--target <triple>]... <dir> <crate_dir>  cross-TU C dir -> Cargo binary crate"
    );
    eprintln!(
        "  translate-project --compile-commands <file>... <dir> <crate_dir>  cross-TU C dir -> Cargo binary crate, driven by a compile commands database"
    );
    eprintln!(
        "  translate-project --lib [--source-manifest <file>|--compile-commands <file>...] <project_dir> <crate_dir>  cross-TU C library -> Cargo crate"
    );
    ExitCode::from(2)
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().collect();
    match args.get(1).map(String::as_str) {
        Some("emit-cir") => match args.get(2) {
            Some(path) => run(emit_cir(Path::new(path))),
            None => usage(),
        },
        Some("emit-fixtures") => run(emit_fixtures()),
        Some("emit-lowered-fixtures") => run(emit_lowered_fixtures()),
        Some("fixup-debug") => run(fixup_debug(&args[2..])),
        Some("translate") => match args[2..].split_last() {
            Some((path, clang_args)) => run(translate_with_clang_args(Path::new(path), clang_args)),
            None => usage(),
        },
        Some("translate-lowered") => match args.get(2) {
            Some(path) => run(lowered_rust(Path::new(path))),
            None => usage(),
        },
        Some("translate-directives") => match args.get(2) {
            Some(path) => run(cli_report(directive_translate::translate_directives(
                Path::new(path),
            ))),
            None => usage(),
        },
        Some("record-cfg") => match args.get(2) {
            Some(path) => run(record_cfg(Path::new(path), &args[3..])),
            None => usage(),
        },
        Some("translate-project") => match args.get(2).map(String::as_str) {
            Some("--lib") => run(translate_project_lib_command(&args[3..])),
            Some(_) => match args.get(3) {
                Some(_) => run(translate_project_command(&args[2..])),
                None => usage(),
            },
            None => usage(),
        },
        _ => usage(),
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

const SLATE_ISSUES_URL: &str = "https://github.com/takashiidobe/slate/issues";

trait Diagnosable {
    fn nyi_diagnostic(&self) -> Option<Vec<&str>>;
}

impl Diagnosable for frontend::toolchain::EmitError {
    fn nyi_diagnostic(&self) -> Option<Vec<&str>> {
        match self {
            Self::ToolFailed { stderr, .. } => {
                let diagnostics: Vec<&str> = stderr
                    .lines()
                    .filter(|line| {
                        line.contains("error:")
                            && line.to_lowercase().contains("not yet implemented")
                    })
                    .collect();
                (!diagnostics.is_empty()).then_some(diagnostics)
            }
            _ => None,
        }
    }
}

impl Diagnosable for frontend::cir_input::ModuleError {
    fn nyi_diagnostic(&self) -> Option<Vec<&str>> {
        match self {
            Self::Emit(e) => e.nyi_diagnostic(),
            _ => None,
        }
    }
}

impl Diagnosable for api::Error {
    fn nyi_diagnostic(&self) -> Option<Vec<&str>> {
        match self {
            Self::Cir { source, .. } => source.nyi_diagnostic(),
            _ => None,
        }
    }
}

impl Diagnosable for directive_translate::DirectiveError {
    fn nyi_diagnostic(&self) -> Option<Vec<&str>> {
        match self {
            Self::Cir { source, .. } => source.nyi_diagnostic(),
            _ => None,
        }
    }
}

fn cli_report<T, E: std::fmt::Display + Diagnosable>(result: Result<T, E>) -> Result<T, String> {
    result.map_err(|error| match error.nyi_diagnostic() {
        Some(diagnostics) => format!(
            "error emitting Clang IR:\n{}\n\nReport this at {SLATE_ISSUES_URL}",
            diagnostics.join("\n")
        ),
        None => error.to_string(),
    })
}

fn emit_cir(path: &Path) -> Result<String, String> {
    cli_report(frontend::toolchain::emit_generic(path))
}

fn translate(path: &Path) -> Result<String, String> {
    cli_report(api::translate(path))
}

fn translate_with_clang_args(path: &Path, clang_args: &[String]) -> Result<String, String> {
    cli_report(api::translate_with_args(path, clang_args))
}

fn lowered_program(path: &Path) -> Result<(Module, rust_ast::Program), String> {
    cli_report(api::lowered_program(path))
}

fn reject_active_unsupported(pp: &preprocess::Preprocessing, context: &str) -> Result<(), String> {
    cli_report(api::reject_active_unsupported(pp, context))
}

fn reject_active_unsupported_file(path: &Path, context: &str) -> Result<(), String> {
    cli_report(api::reject_active_unsupported_file(path, context))
}

fn fixup_debug(args: &[String]) -> Result<String, String> {
    let (path, options) = parse_fixup_debug_args(args)?;
    let (_, program) = lowered_program(path)?;
    backend::format_rust(&backend::debug_with(program, options))
}

fn parse_fixup_debug_args(args: &[String]) -> Result<(&Path, backend::DebugOptions), String> {
    let mut path = None;
    let mut options = backend::DebugOptions::default();
    let mut i = 0;
    while i < args.len() {
        match args[i].as_str() {
            "--up-to-pass" => {
                i += 1;
                let Some(name) = args.get(i) else {
                    return Err("--up-to-pass requires a pass name".into());
                };
                options.up_to_pass = Some(parse_debug_pass("--up-to-pass", name)?);
            }
            "--only-pass" => {
                i += 1;
                let Some(name) = args.get(i) else {
                    return Err("--only-pass requires a pass name".into());
                };
                options.only_pass = Some(parse_debug_pass("--only-pass", name)?);
            }
            "--debug-only-pass" => {
                i += 1;
                let Some(name) = args.get(i) else {
                    return Err("--debug-only-pass requires a pass name".into());
                };
                options.debug_only_pass = Some(parse_debug_pass("--debug-only-pass", name)?);
            }
            flag if flag.starts_with('-') => {
                return Err(format!("unknown fixup-debug option: {flag}"));
            }
            file => {
                if path.replace(Path::new(file)).is_some() {
                    return Err("fixup-debug accepts exactly one input file".into());
                }
            }
        }
        i += 1;
    }
    let selected_mode_count = [
        options.up_to_pass.is_some(),
        options.only_pass.is_some(),
        options.debug_only_pass.is_some(),
    ]
    .into_iter()
    .filter(|selected| *selected)
    .count();
    if selected_mode_count > 1 {
        return Err("--up-to-pass, --only-pass, and --debug-only-pass cannot be combined".into());
    }
    path.map(|path| (path, options))
        .ok_or_else(|| "fixup-debug requires an input file".into())
}

fn parse_debug_pass(flag: &str, name: &str) -> Result<backend::Pass, String> {
    backend::Pass::parse(name).ok_or_else(|| {
        format!(
            "unknown pass for {flag}: {name}\nvalid passes: {}",
            backend::valid_pass_names()
        )
    })
}

fn collect_c_modules(dir: &Path) -> Result<Vec<(String, PathBuf)>, String> {
    let mut modules = Vec::new();
    for entry in std::fs::read_dir(dir).map_err(|e| format!("read {}: {e}", dir.display()))? {
        let path = entry
            .map_err(|e| format!("read {} entry: {e}", dir.display()))?
            .path();
        if path.extension().and_then(|e| e.to_str()) != Some("c") {
            continue;
        }
        let stem = path
            .file_stem()
            .and_then(|s| s.to_str())
            .ok_or_else(|| format!("bad file stem: {}", path.display()))?;
        modules.push((rust_ident(stem), path));
    }
    modules.sort();
    Ok(modules)
}

fn collect_c_modules_from_manifest(
    project_dir: &Path,
    manifest: &Path,
) -> Result<Vec<(String, PathBuf)>, String> {
    let source = std::fs::read_to_string(manifest)
        .map_err(|e| format!("read source manifest {}: {e}", manifest.display()))?;
    let mut modules = Vec::new();
    let mut paths = BTreeSet::new();
    let mut stems = BTreeMap::new();
    for (index, entry) in source.lines().enumerate() {
        let entry = entry.trim();
        if entry.is_empty() || entry.starts_with('#') {
            continue;
        }
        let relative = Path::new(entry);
        let path = if relative.is_absolute() {
            relative.to_path_buf()
        } else {
            project_dir.join(relative)
        };
        if path.extension().and_then(|extension| extension.to_str()) != Some("c") {
            return Err(format!(
                "source manifest {}:{} does not name a .c file: {}",
                manifest.display(),
                index + 1,
                entry
            ));
        }
        if !path.is_file() {
            return Err(format!(
                "source manifest {}:{} file does not exist: {}",
                manifest.display(),
                index + 1,
                path.display()
            ));
        }
        if !paths.insert(path.clone()) {
            return Err(format!(
                "source manifest {}:{} repeats {}",
                manifest.display(),
                index + 1,
                entry
            ));
        }
        let stem = path
            .file_stem()
            .and_then(|stem| stem.to_str())
            .map(rust_ident)
            .ok_or_else(|| format!("bad file stem: {}", path.display()))?;
        if let Some(previous) = stems.insert(stem.clone(), path.clone()) {
            return Err(format!(
                "source manifest {} maps both {} and {} to module {}",
                manifest.display(),
                previous.display(),
                path.display(),
                stem
            ));
        }
        modules.push((stem, path));
    }
    if modules.is_empty() {
        return Err(format!(
            "source manifest {} contains no C translation units",
            manifest.display()
        ));
    }
    modules.sort();
    Ok(modules)
}

fn rust_ident(name: &str) -> String {
    let mut out: String = name
        .chars()
        .map(|c| {
            if c.is_ascii_alphanumeric() || c == '_' {
                c
            } else {
                '_'
            }
        })
        .collect();
    if out.is_empty() || out.starts_with(|c: char| c.is_ascii_digit()) {
        out.insert(0, '_');
    }
    out
}

fn collect_record_type_names(ty: &c_ast::CType, out: &mut BTreeSet<String>) {
    match ty {
        c_ast::CType::Ptr(inner) | c_ast::CType::Array(inner, _) => {
            collect_record_type_names(inner, out);
        }
        c_ast::CType::FuncPtr { ret, params } => {
            collect_record_type_names(ret, out);
            for param in params {
                collect_record_type_names(param, out);
            }
        }
        c_ast::CType::Record(name) if !frontend::is_clib_record_type(name) => {
            out.insert(rust_ident(name));
        }
        _ => {}
    }
}

fn collect_record_field_type_names(record: &c_ast::Record, out: &mut BTreeSet<String>) {
    for field in &record.fields {
        collect_record_type_names(&field.ty, out);
    }
}

fn records_have_same_shape(a: &c_ast::Record, b: &c_ast::Record) -> bool {
    a.kind == b.kind
        && a.fields.len() == b.fields.len()
        && a.fields
            .iter()
            .zip(&b.fields)
            .all(|(x, y)| x.name == y.name && x.ty == y.ty && x.bit_width == y.bit_width)
}

fn merge_record_into_shared(
    record: c_ast::Record,
    shared_records: &mut BTreeMap<String, c_ast::Record>,
    record_shape_conflicts: &mut BTreeSet<String>,
    referenced_record_types: &mut BTreeSet<String>,
) {
    collect_record_field_type_names(&record, referenced_record_types);
    let key = rust_ident(&record.name);
    match shared_records.get(&key) {
        Some(existing) if !records_have_same_shape(existing, &record) => {
            record_shape_conflicts.insert(key);
        }
        Some(_) => {}
        None => {
            shared_records.insert(key, record);
        }
    }
}

fn merge_shared_types_for_module(
    module: &Module,
    unit: &c_ast::Unit,
    shared_enums: &mut BTreeMap<String, c_ast::Enum>,
    shared_records: &mut BTreeMap<String, c_ast::Record>,
    record_shape_conflicts: &mut BTreeSet<String>,
    referenced_record_types: &mut BTreeSet<String>,
) {
    for enm in &unit.enums {
        shared_enums
            .entry(rust_ident(&enm.name))
            .or_insert_with(|| enm.clone());
    }
    for record in &unit.records {
        merge_record_into_shared(
            record.clone(),
            shared_records,
            record_shape_conflicts,
            referenced_record_types,
        );
    }
    for record in frontend::anon_local_records(module) {
        merge_record_into_shared(
            record,
            shared_records,
            record_shape_conflicts,
            referenced_record_types,
        );
    }
    for record in frontend::shim_records_for_module(module, unit) {
        collect_record_field_type_names(&record, referenced_record_types);
        shared_records
            .entry(rust_ident(&record.name))
            .or_insert(record);
    }
}

struct TargetVariant {
    cfg: rust_ast::Cfg,
    clang_args: Vec<String>,
}

fn target_cfg(target: &str) -> Result<rust_ast::Cfg, String> {
    let target = cli_result(frontend::toolchain::target_config(target))?;
    Ok(rust_ast::Cfg::All(vec![
        rust_ast::Cfg::Opt {
            key: "target_arch".into(),
            value: target.arch.into(),
        },
        rust_ast::Cfg::Opt {
            key: "target_endian".into(),
            value: target.endian.into(),
        },
        rust_ast::Cfg::Opt {
            key: "target_env".into(),
            value: target.env.into(),
        },
        rust_ast::Cfg::Opt {
            key: "target_os".into(),
            value: target.os.into(),
        },
        rust_ast::Cfg::Opt {
            key: "target_pointer_width".into(),
            value: target.pointer_width,
        },
        rust_ast::Cfg::Opt {
            key: "target_vendor".into(),
            value: target.vendor.into(),
        },
    ]))
}

fn target_variants(extra_targets: &[String]) -> Result<Vec<TargetVariant>, String> {
    let active = frontend::toolchain::active_target();
    let mut variants = vec![TargetVariant {
        cfg: target_cfg(&active)?,
        clang_args: Vec::new(),
    }];
    let mut seen = BTreeSet::from([target_cfg(&active)?]);
    for target in extra_targets {
        let cfg = target_cfg(target)?;
        if seen.insert(cfg.clone()) {
            variants.push(TargetVariant {
                cfg,
                clang_args: cli_result(frontend::toolchain::target_override_args(target))?,
            });
        }
    }
    Ok(variants)
}

fn write_project_modules(
    paths: Vec<PathBuf>,
    mut programs: Vec<rust_ast::Program>,
) -> Result<Vec<PathBuf>, String> {
    backend::propagate_unwind_abi_across_project(&mut programs);
    let mut written = Vec::new();
    for (output, program) in paths.iter().zip(&programs) {
        backend::write_rust(output, &program.emit())?;
        written.push(output.clone());
    }
    Ok(written)
}

#[derive(Default)]
struct ProjectModules {
    paths: Vec<PathBuf>,
    programs: Vec<rust_ast::Program>,
    aliases: BTreeMap<String, CirType>,
    bitfield_storages: frontend::BitfieldStorages,
}

impl ProjectModules {
    fn collect_shared_type_inputs(&mut self, module: &Module) {
        self.aliases.extend(module.type_aliases.clone());
        self.bitfield_storages
            .extend(frontend::collect_bitfield_storages(module));
    }

    fn push(&mut self, path: PathBuf, program: rust_ast::Program) {
        self.paths.push(path);
        self.programs.push(program);
    }

    fn push_module(&mut self, path: PathBuf, module: &Module, program: rust_ast::Program) {
        self.collect_shared_type_inputs(module);
        self.push(path, program);
    }

    fn push_shared_types(
        &mut self,
        crate_src: &Path,
        shared_records: BTreeMap<String, c_ast::Record>,
        shared_enums: BTreeMap<String, c_ast::Enum>,
        shims: &mut BTreeMap<String, rust_ast::ExternFnDecl>,
    ) -> bool {
        if shared_records.is_empty() && shared_enums.is_empty() {
            return false;
        }
        let records: Vec<_> = shared_records.into_values().collect();
        let enums: Vec<_> = shared_enums.into_values().collect();
        let program =
            frontend::lower_shared_types(&records, &enums, &self.aliases, &self.bitfield_storages);
        for shim in c_shim::collect_program_shims(&program) {
            shims.entry(shim.name.clone()).or_insert(shim);
        }
        self.push(crate_src.join("types.rs"), program);
        true
    }

    fn write(self) -> Result<Vec<PathBuf>, String> {
        write_project_modules(self.paths, self.programs)
    }
}

fn merge_target_programs(variants: &[(rust_ast::Cfg, rust_ast::Program)]) -> rust_ast::Program {
    if variants.len() <= 1 {
        return variants.first().map(|(_, p)| p.clone()).unwrap_or_default();
    }
    let baseline = &variants[0].1;

    let mut items = Vec::new();
    for item in &baseline.items {
        if let rust_ast::Item::CrateAttrs(_) = item {
            items.push(item.clone());
        }
    }

    let skip_kinds = |item: &rust_ast::Item| matches!(item, rust_ast::Item::CrateAttrs(_));

    let item_id = |item: &rust_ast::Item| {
        (
            directive_translate::item_key(item),
            codegen::item_to_string(item),
        )
    };

    let mut emitted: BTreeSet<(String, String)> = BTreeSet::new();
    for item in &baseline.items {
        if skip_kinds(item) {
            continue;
        }
        let key = directive_translate::item_key(item);
        let id = item_id(item);
        let all_same = variants.iter().skip(1).all(|(_, prog)| {
            prog.items
                .iter()
                .filter(|i| !skip_kinds(i))
                .any(|i| item_id(i) == id)
        });
        if all_same {
            if emitted.insert(id.clone()) {
                items.push(item.clone());
            }
        } else {
            // Collect all items with this key from every variant, grouped by
            // rendered text so identical content across targets can be
            // coalesced into a single cfg(any(...)) instead of duplicated.
            // A target may produce multiple items with the same key (e.g.
            // multiple extern blocks); preserve their relative order.
            let mut groups: BTreeMap<String, (Vec<rust_ast::Cfg>, rust_ast::Item)> =
                BTreeMap::new();
            for (cfg, prog) in variants {
                for variant_item in prog.items.iter().filter(|i| !skip_kinds(i)) {
                    if directive_translate::item_key(variant_item) != key {
                        continue;
                    }
                    let variant_id = item_id(variant_item);
                    if emitted.contains(&variant_id) {
                        continue;
                    }
                    let text = variant_id.1.clone();
                    let entry = groups
                        .entry(text)
                        .or_insert_with(|| (Vec::new(), variant_item.clone()));
                    entry.0.push(cfg.clone());
                }
            }
            for (cfgs, item) in groups.into_values() {
                let id = item_id(&item);
                emitted.insert(id);
                if cfgs.len() == variants.len() {
                    items.push(item);
                } else if cfgs.len() == 1 {
                    items.push(rust_ast::Item::Cfg {
                        cfg: cfgs.into_iter().next().unwrap(),
                        item: Box::new(item),
                    });
                } else {
                    items.push(rust_ast::Item::Cfg {
                        cfg: rust_ast::Cfg::Any(cfgs),
                        item: Box::new(item),
                    });
                }
            }
        }
    }

    for (cfg, prog) in variants.iter().skip(1) {
        for item in prog.items.iter().filter(|i| !skip_kinds(i)) {
            let id = item_id(item);
            if emitted.insert(id) {
                items.push(rust_ast::Item::Cfg {
                    cfg: cfg.clone(),
                    item: Box::new(item.clone()),
                });
            }
        }
    }

    rust_ast::Program { items }
}

fn cargo() -> String {
    std::env::var("SLATE_CARGO").unwrap_or_else(|_| "cargo".into())
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
    autobins: bool,
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
    let autobins_line = if autobins { "" } else { "autobins = false\n" };
    let bitfields_path = Path::new(env!("CARGO_MANIFEST_DIR")).join("vendor/bitfields");
    format!(
        r#"[package]
name = "{package}"
version = "0.0.0"
edition = "2024"
{autobins_line}
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
codegen-units = 256
{test_targets}"#,
        bitfields_path.display()
    )
}

fn write_crate_manifest(
    crate_dir: &Path,
    package: &str,
    tests: &[String],
    slate_support: bool,
    c_shims: bool,
    autobins: bool,
) -> Result<(), String> {
    std::fs::write(
        crate_dir.join("Cargo.toml"),
        crate_manifest(package, tests, slate_support, c_shims, autobins),
    )
    .map_err(|e| format!("write {}: {e}", crate_dir.join("Cargo.toml").display()))
}

/// Scaffold a Cargo crate at `crate_dir` for translated output: `cargo init`,
/// a manifest with the shared dependencies, and the vendored `aligned` crate.
/// A library crate (`wants_lib`) exposes every module as `pub mod` from
/// `lib.rs` and disables bin autodiscovery, since an incidental module named
/// `main` must not collide with Cargo's `src/main.rs` binary detection. An
/// executable crate keeps `src/main.rs` as its one binary entry point.
fn init_crate(crate_dir: &Path, wants_lib: bool) -> Result<(), String> {
    std::fs::create_dir_all(crate_dir)
        .map_err(|e| format!("create {}: {e}", crate_dir.display()))?;
    let package = package_name(crate_dir);
    if !crate_dir.join("Cargo.toml").exists() {
        let mut init = Command::new(cargo());
        init.arg("init");
        if wants_lib {
            init.arg("--lib");
        }
        let out = init
            .args(["--vcs", "none", "--name"])
            .arg(&package)
            .arg(crate_dir)
            .output()
            .map_err(|e| format!("spawn cargo init: {e}"))?;
        if !out.status.success() {
            return Err(format!(
                "cargo init failed:\n{}",
                String::from_utf8_lossy(&out.stderr)
            ));
        }
    }

    write_crate_manifest(crate_dir, &package, &[], false, false, !wants_lib)?;
    write_aligned_support(crate_dir)?;
    write_bitint_support(crate_dir)?;
    write_num_complex_support(crate_dir)?;
    if wants_lib {
        let main_rs = crate_dir.join("src/main.rs");
        if main_rs.exists() {
            std::fs::remove_file(&main_rs)
                .map_err(|e| format!("remove {}: {e}", main_rs.display()))?;
        }
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
            include_str!("../vendor/aligned/Cargo.toml"),
        ),
        (
            aligned_dir.join("LICENSE-MIT"),
            include_str!("../vendor/aligned/LICENSE-MIT"),
        ),
        (
            aligned_dir.join("LICENSE-APACHE"),
            include_str!("../vendor/aligned/LICENSE-APACHE"),
        ),
    ] {
        std::fs::write(&path, contents).map_err(|e| format!("write {}: {e}", path.display()))?;
    }
    backend::write_rust(
        &src_dir.join("lib.rs"),
        include_str!("../vendor/aligned/src/lib.rs"),
    )?;
    Ok(())
}

fn write_bitint_support(crate_dir: &Path) -> Result<(), String> {
    let bitint_dir = crate_dir.join("bitint");
    let src_dir = bitint_dir.join("src");
    std::fs::create_dir_all(&src_dir).map_err(|e| format!("create {}: {e}", src_dir.display()))?;
    let manifest = bitint_dir.join("Cargo.toml");
    std::fs::write(&manifest, include_str!("../vendor/bitint/Cargo.toml"))
        .map_err(|e| format!("write {}: {e}", manifest.display()))?;
    backend::write_rust(
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
            include_str!("../vendor/num-complex/Cargo.toml"),
        ),
        (
            num_complex_dir.join("LICENSE-MIT"),
            include_str!("../vendor/num-complex/LICENSE-MIT"),
        ),
        (
            num_complex_dir.join("LICENSE-APACHE"),
            include_str!("../vendor/num-complex/LICENSE-APACHE"),
        ),
    ] {
        std::fs::write(&path, contents).map_err(|e| format!("write {}: {e}", path.display()))?;
    }
    for (source, contents) in [
        ("lib.rs", include_str!("../vendor/num-complex/src/lib.rs")),
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
        backend::write_rust(&src_dir.join(source), contents)?;
    }
    Ok(())
}

fn write_slate_support(crate_dir: &Path) -> Result<(), String> {
    let support_dir = crate_dir.join("slate-support");
    let src_dir = support_dir.join("src");
    std::fs::create_dir_all(&src_dir).map_err(|e| format!("create {}: {e}", src_dir.display()))?;
    std::fs::write(
        support_dir.join("Cargo.toml"),
        r#"[package]
name = "slate-support"
version = "0.0.0"
edition = "2024"

[lib]
proc-macro = true
"#,
    )
    .map_err(|e| format!("write {}: {e}", support_dir.join("Cargo.toml").display()))?;
    backend::write_rust(
        &src_dir.join("lib.rs"),
        r#"#![feature(proc_macro_diagnostic, proc_macro_value)]

extern crate proc_macro;

use proc_macro::{Diagnostic, Level, TokenStream, TokenTree};

#[proc_macro]
pub fn warning(input: TokenStream) -> TokenStream {
    let mut tokens = input.into_iter();
    let Some(TokenTree::Literal(message)) = tokens.next() else {
        panic!("slate_support::warning! expects one string literal");
    };
    if tokens.next().is_some() {
        panic!("slate_support::warning! expects one string literal");
    }
    let message = message
        .str_value()
        .expect("slate_support::warning! expects one string literal");
    Diagnostic::new(Level::Warning, message).emit();
    TokenStream::new()
}
"#,
    )
}

fn write_c_shims(crate_dir: &Path, shims: &[rust_ast::ExternFnDecl]) -> Result<(), String> {
    backend::write_rust(
        &crate_dir.join("build.rs"),
        r#"fn main() {
    println!("cargo:rerun-if-changed=src/slate_shims.c");
    cc::Build::new().file("src/slate_shims.c").compile("slate_shims");
}
"#,
    )?;
    let shim_path = crate_dir.join("src/slate_shims.c");
    std::fs::write(&shim_path, c_shim::render_shim_c_source(shims))
        .map_err(|e| format!("write {}: {e}", shim_path.display()))
}

fn project_warning_items(
    pp: &preprocess::Preprocessing,
    context: &str,
    backend: directive_translate::WarningBackend,
) -> Result<Vec<rust_ast::Item>, String> {
    let mut items = Vec::new();
    for (index, directive) in pp
        .directives
        .iter()
        .filter(|directive| directive.name == preprocess::DirectiveName::Warning)
        .enumerate()
    {
        let cfg = directive
            .condition
            .as_ref()
            .map(|condition| {
                preprocess::pred_to_cfg(condition).ok_or_else(|| {
    format!(
                        "{context}: #warning at line {} is guarded by predicate `{}` which does not map to a known Rust cfg",
                        directive.line_start,
                        preprocess::predicate_text(condition)
                    )
                })
            })
            .transpose()?;
        items.extend(directive_translate::warning_items(
            &directive.raw_payload,
            index,
            cfg,
            backend,
        ));
    }
    Ok(items)
}

fn translate_project_lib_command(args: &[String]) -> Result<String, String> {
    let mut paths = Vec::new();
    let mut source_manifest = None;
    let mut compile_command_paths = Vec::new();
    let mut index = 0;
    while index < args.len() {
        match args[index].as_str() {
            "--source-manifest" => {
                index += 1;
                let manifest = args
                    .get(index)
                    .ok_or_else(|| "--source-manifest requires a file".to_string())?;
                if source_manifest.replace(PathBuf::from(manifest)).is_some() {
                    return Err("--source-manifest may only be specified once".into());
                }
            }
            "--compile-commands" => {
                index += 1;
                let commands = args
                    .get(index)
                    .ok_or_else(|| "--compile-commands requires a file".to_string())?;
                compile_command_paths.push(PathBuf::from(commands));
            }
            flag if flag.starts_with('-') => {
                return Err(format!("unknown translate-project --lib option: {flag}"));
            }
            path => paths.push(path),
        }
        index += 1;
    }
    if paths.len() != 2 {
        return Err(
            "translate-project --lib requires a project directory and crate directory".into(),
        );
    }
    if source_manifest.is_some() && !compile_command_paths.is_empty() {
        return Err("--source-manifest and --compile-commands cannot be used together".into());
    }
    if !compile_command_paths.is_empty() {
        return translate_project_lib_crate_with_compile_commands(
            Path::new(paths[0]),
            Path::new(paths[1]),
            &compile_command_paths,
        );
    }
    translate_project_lib_crate_with_manifest(
        Path::new(paths[0]),
        Path::new(paths[1]),
        source_manifest.as_deref(),
    )
}

fn translate_project_lib_crate(project_dir: &Path, crate_dir: &Path) -> Result<String, String> {
    translate_project_lib_crate_with_manifest(project_dir, crate_dir, None)
}

struct ProjectModule {
    stem: String,
    path: PathBuf,
    module: Module,
    unit: c_ast::Unit,
    warning_items: Vec<rust_ast::Item>,
}

fn load_project_module(
    path: &Path,
    project_dir: &Path,
    stem: &str,
    context: &str,
) -> Result<ProjectModule, String> {
    let (source, _raw) =
        preprocess::read_source(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let pp = cli_result(preprocess::record_file(&source, &[]))?;
    reject_active_unsupported(&pp, context)?;
    let warning_items = project_warning_items(
        &pp,
        context,
        directive_translate::WarningBackend::SupportMacro,
    )?;
    let module = cli_report(frontend::cir_input::emit_module(path, &[]))?;
    let unit = cli_result(c_ast::parse_file_with_project_records(path, project_dir))?;
    Ok(ProjectModule {
        stem: stem.to_string(),
        path: path.to_path_buf(),
        module,
        unit,
        warning_items,
    })
}

fn load_project_modules_parallel(
    modules: &[(String, PathBuf)],
    project_dir: &Path,
    context: &str,
) -> Result<Vec<ProjectModule>, String> {
    let pool = rayon::ThreadPoolBuilder::new()
        .num_threads(slate_job_count())
        .build()
        .map_err(|error| format!("build worker pool: {error}"))?;
    let results: Vec<Result<ProjectModule, String>> = pool.install(|| {
        modules
            .par_iter()
            .map(|(stem, path)| load_project_module(path, project_dir, stem, context))
            .collect()
    });
    results.into_iter().collect()
}

fn translate_project_lib_crate_with_manifest(
    project_dir: &Path,
    crate_dir: &Path,
    source_manifest: Option<&Path>,
) -> Result<String, String> {
    let nested_src = project_dir.join("src");
    let src_dir = if nested_src.is_dir() {
        nested_src.as_path()
    } else {
        project_dir
    };
    let modules = match source_manifest {
        Some(manifest) => collect_c_modules_from_manifest(project_dir, manifest)?,
        None => collect_c_modules(src_dir)?,
    };
    if modules.is_empty() {
        return Err(format!(
            "translate-project --lib: no C files in {}",
            src_dir.display()
        ));
    }

    init_crate(crate_dir, true)?;
    let crate_src = crate_dir.join("src");
    std::fs::create_dir_all(&crate_src)
        .map_err(|e| format!("create {}: {e}", crate_src.display()))?;

    let mut loaded_modules = Vec::new();
    let mut defined: BTreeMap<String, String> = BTreeMap::new();
    let mut defined_globals: BTreeMap<String, String> = BTreeMap::new();
    let mut unsafe_functions = BTreeSet::new();
    let mut crate_features = BTreeSet::new();
    let mut shared_records = BTreeMap::new();
    let mut record_shape_conflicts: BTreeSet<String> = BTreeSet::new();
    let mut shared_enums = BTreeMap::new();
    let mut referenced_record_types = BTreeSet::new();
    let mut uses_slate_support = false;
    let mut has_setlocale = false;
    let mut cross_referenced_functions: BTreeSet<String> = BTreeSet::new();
    let mut cross_referenced_globals: BTreeSet<String> = BTreeSet::new();
    let mut address_taken_functions: BTreeSet<String> = BTreeSet::new();
    for loaded in load_project_modules_parallel(&modules, project_dir, "translate-project --lib")? {
        let ProjectModule {
            stem,
            path,
            module,
            unit,
            warning_items,
        } = loaded;
        uses_slate_support |= !warning_items.is_empty();
        for sym in frontend::defined_functions(&module) {
            defined.insert(sym, stem.clone());
        }
        for sym in frontend::defined_globals(&module) {
            defined_globals.insert(sym, stem.clone());
        }
        unsafe_functions.extend(frontend::unsafe_defined_functions(&module));
        address_taken_functions.extend(frontend::address_taken_functions(&module));
        crate_features.extend(frontend::required_features(&module));
        cross_referenced_functions.extend(frontend::declared_functions(&module));
        cross_referenced_globals.extend(frontend::declared_globals(&module));
        has_setlocale |= frontend::declared_functions(&module)
            .iter()
            .any(|name| name == "setlocale");
        merge_shared_types_for_module(
            &module,
            &unit,
            &mut shared_enums,
            &mut shared_records,
            &mut record_shape_conflicts,
            &mut referenced_record_types,
        );
        loaded_modules.push((stem, path, module, unit, warning_items));
    }
    for name in referenced_record_types {
        shared_records.entry(name.clone()).or_insert(c_ast::Record {
            name,
            comments: Vec::new(),
            kind: c_ast::RecordKind::Struct,
            fields: Vec::new(),
            packed: None,
            align: None,
        });
    }
    shared_records.retain(|name, _| !record_shape_conflicts.contains(name));
    let shared_record_names: BTreeSet<String> = shared_records.keys().cloned().collect();
    let shared_enum_names: BTreeSet<String> = shared_enums.keys().cloned().collect();
    let shared_long_double = frontend::shared_types_use_long_double(
        &shared_records.values().cloned().collect::<Vec<_>>(),
    );

    let tests_dir = project_dir.join("tests");
    let translate_tests = source_manifest.is_none() && tests_dir.is_dir();
    if translate_tests {
        for (_, path) in collect_c_modules(&tests_dir)? {
            let module = cli_report(frontend::cir_input::emit_module(&path, &[]))?;
            has_setlocale |= frontend::declared_functions(&module)
                .iter()
                .any(|name| name == "setlocale");
            address_taken_functions.extend(frontend::address_taken_functions(&module));
        }
    }

    let project = frontend::ProjectInfo {
        cross_module: defined,
        cross_module_globals: defined_globals,
        shared_records: shared_record_names,
        shared_enums: shared_enum_names,
        shared_type_module: Some("types".into()),
        shared_type_crate: None,
        shared_long_double,
        cross_module_crate: None,
        unsafe_functions,
        crate_features,
        child_modules: Vec::new(),
        emit_pub: true,
        cross_referenced_functions,
        cross_referenced_globals,
        address_taken_functions,
    };
    let fixup_skip = if has_setlocale {
        backend::SkipSet::skip(backend::Pass::CTypeLibc)
    } else {
        backend::SkipSet::none()
    };

    let mut shims: BTreeMap<String, rust_ast::ExternFnDecl> = BTreeMap::new();
    let mut written = Vec::new();
    let mut project_modules = ProjectModules::default();
    for (stem, path, module, unit, warning_items) in loaded_modules {
        let mut ctx = ctx::Ctx::default();
        let mut program = frontend::lower_with_project(&module, &unit, &mut ctx, &project);
        for d in &ctx.diagnostics.items {
            eprintln!("{:?}: {}", d.severity, d.message);
        }
        if ctx.diagnostics.has_errors() {
            return Err(format!("lowering failed for {}", path.display()));
        }
        for shim in c_shim::collect_program_shims(&program) {
            shims
                .entry(shim.name.clone())
                .or_insert_with(|| shim.clone());
        }
        directive_translate::insert_directive_items(&mut program, warning_items);
        let output = crate_src.join(stem).with_extension("rs");
        project_modules.push_module(output, &module, backend::apply_with(program, &fixup_skip));
    }

    let has_shared_types =
        project_modules.push_shared_types(&crate_src, shared_records, shared_enums, &mut shims);

    let mut lib_rs = String::new();
    for feature in &project.crate_features {
        lib_rs.push_str(&format!("#![feature({})]\n", feature.spelling()));
    }
    if has_shared_types {
        lib_rs.push_str("pub mod types;\n");
    }
    lib_rs.push_str(
        &modules
            .iter()
            .map(|(stem, _)| format!("pub mod {};\n", codegen::escape_ident(stem)))
            .collect::<String>(),
    );
    let lib_rs_path = crate_src.join("lib.rs");
    backend::write_rust(&lib_rs_path, &lib_rs)?;
    written.push(lib_rs_path);

    let mut test_modules = Vec::new();
    if translate_tests {
        let crate_tests = crate_dir.join("tests");
        std::fs::create_dir_all(&crate_tests)
            .map_err(|e| format!("create {}: {e}", crate_tests.display()))?;
        let package = package_name(crate_dir);
        for (stem, path) in collect_c_modules(&tests_dir)? {
            let source = std::fs::read_to_string(&path)
                .map_err(|e| format!("read {}: {e}", path.display()))?;
            let pp = cli_result(preprocess::record_file(&source, &[]))?;
            reject_active_unsupported(&pp, "translate-project --lib")?;
            let warning_items = project_warning_items(
                &pp,
                "translate-project --lib",
                directive_translate::WarningBackend::SupportMacro,
            )?;
            uses_slate_support |= !warning_items.is_empty();
            let module = cli_report(frontend::cir_input::emit_module(&path, &[]))?;
            let unit = cli_result(c_ast::parse_file_with_project_records(&path, project_dir))?;
            let test_project = frontend::ProjectInfo {
                cross_module: project.cross_module.clone(),
                cross_module_globals: project.cross_module_globals.clone(),
                unsafe_functions: project.unsafe_functions.clone(),
                address_taken_functions: project.address_taken_functions.clone(),
                shared_records: project.shared_records.clone(),
                shared_enums: project.shared_enums.clone(),
                shared_type_module: Some("types".into()),
                shared_type_crate: Some(package.clone()),
                shared_long_double: project.shared_long_double,
                cross_module_crate: Some(package.clone()),
                crate_features: project.crate_features.clone(),
                emit_pub: true,
                ..frontend::ProjectInfo::default()
            };
            let mut ctx = ctx::Ctx::default();
            let mut program = frontend::lower_with_project(&module, &unit, &mut ctx, &test_project);
            for d in &ctx.diagnostics.items {
                eprintln!("{:?}: {}", d.severity, d.message);
            }
            if ctx.diagnostics.has_errors() {
                return Err(format!("lowering failed for {}", path.display()));
            }
            for shim in c_shim::collect_program_shims(&program) {
                shims
                    .entry(shim.name.clone())
                    .or_insert_with(|| shim.clone());
            }
            directive_translate::insert_directive_items(&mut program, warning_items);
            let output = crate_tests.join(&stem).with_extension("rs");
            project_modules.push(output, backend::apply_with(program, &fixup_skip));
            test_modules.push(stem);
        }
    }
    written.extend(project_modules.write()?);
    if uses_slate_support {
        write_slate_support(crate_dir)?;
    }
    let shims: Vec<rust_ast::ExternFnDecl> = shims.into_values().collect();
    if !shims.is_empty() {
        write_c_shims(crate_dir, &shims)?;
    }
    write_crate_manifest(
        crate_dir,
        &package_name(crate_dir),
        &test_modules,
        uses_slate_support,
        !shims.is_empty(),
        false,
    )?;

    Ok(written
        .into_iter()
        .map(|path| format!("wrote {}\n", path.display()))
        .collect())
}

#[derive(Default)]
struct LibraryVariantFacts {
    defined: BTreeMap<String, String>,
    defined_globals: BTreeMap<String, String>,
    unsafe_functions: BTreeSet<String>,
    address_taken_functions: BTreeSet<String>,
    crate_features: BTreeSet<rust_ast::Feature>,
    has_setlocale: bool,
    cross_referenced_functions: BTreeSet<String>,
    cross_referenced_globals: BTreeSet<String>,
}

struct LoadedLibraryVariant {
    cfg: rust_ast::Cfg,
    stem: String,
    path: PathBuf,
    module: Module,
    unit: c_ast::Unit,
    warning_items: Vec<rust_ast::Item>,
}

fn compile_command_args(command: &compile_commands::CompileCommand) -> Result<Vec<String>, String> {
    let mut args = cli_result(frontend::toolchain::target_override_args(&command.target))?;
    args.extend(command.args.iter().cloned());
    Ok(args)
}

fn load_library_variant(
    path: &Path,
    project_dir: &Path,
    cfg: &rust_ast::Cfg,
    command: &compile_commands::CompileCommand,
    context: &str,
) -> Result<LoadedLibraryVariant, String> {
    let stem = path
        .file_stem()
        .and_then(|stem| stem.to_str())
        .map(rust_ident)
        .ok_or_else(|| format!("bad file stem: {}", path.display()))?;
    let args = compile_command_args(command)?;
    let (source, _raw) = preprocess::read_source(path)
        .map_err(|error| format!("read {}: {error}", path.display()))?;
    let pp = cli_result(preprocess::record_translation_unit(path, &source, &args))?;
    reject_active_unsupported(&pp, context)?;
    let warning_items = project_warning_items(
        &pp,
        context,
        directive_translate::WarningBackend::SupportMacro,
    )?;
    let module = cli_report(frontend::cir_input::emit_module(path, &args))?;
    let unit = cli_result(c_ast::parse_file_with_project_records_and_args(
        path,
        project_dir,
        &args,
    ))?;
    Ok(LoadedLibraryVariant {
        cfg: cfg.clone(),
        stem,
        path: path.to_path_buf(),
        module,
        unit,
        warning_items,
    })
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

fn load_library_variants_parallel(
    command_map: &BTreeMap<(PathBuf, rust_ast::Cfg), compile_commands::CompileCommand>,
    project_dir: &Path,
    context: &str,
) -> Result<Vec<LoadedLibraryVariant>, String> {
    let pool = rayon::ThreadPoolBuilder::new()
        .num_threads(slate_job_count())
        .build()
        .map_err(|error| format!("build worker pool: {error}"))?;
    let entries: Vec<_> = command_map.iter().collect();
    let results: Vec<Result<LoadedLibraryVariant, String>> = pool.install(|| {
        entries
            .par_iter()
            .map(|((path, cfg), command)| {
                load_library_variant(path, project_dir, cfg, command, context)
            })
            .collect()
    });
    results.into_iter().collect()
}

fn translate_project_lib_crate_with_compile_commands(
    project_dir: &Path,
    crate_dir: &Path,
    database_paths: &[PathBuf],
) -> Result<String, String> {
    let commands = cli_result(compile_commands::read(database_paths))?;
    let mut command_map = BTreeMap::new();
    let mut paths_by_stem = BTreeMap::new();
    let mut variant_targets = BTreeMap::new();
    for command in commands {
        let stem = command
            .file
            .file_stem()
            .and_then(|stem| stem.to_str())
            .map(rust_ident)
            .ok_or_else(|| format!("bad file stem: {}", command.file.display()))?;
        if let Some(previous) = paths_by_stem.insert(stem.clone(), command.file.clone())
            && previous != command.file
        {
            return Err(format!(
                "compile commands map both {} and {} to module {}",
                previous.display(),
                command.file.display(),
                stem
            ));
        }
        let cfg = target_cfg(&command.target)?;
        if let Some(previous) =
            command_map.insert((command.file.clone(), cfg.clone()), command.clone())
            && previous.args != command.args
        {
            return Err(format!(
                "compile commands provide conflicting variants for {} and target {}",
                command.file.display(),
                command.target
            ));
        }
        variant_targets.insert(cfg, command.target);
    }

    init_crate(crate_dir, true)?;
    let crate_src = crate_dir.join("src");
    std::fs::create_dir_all(&crate_src)
        .map_err(|error| format!("create {}: {error}", crate_src.display()))?;

    let mut loaded = Vec::new();
    let mut facts: BTreeMap<rust_ast::Cfg, LibraryVariantFacts> = BTreeMap::new();
    let mut shared_records = BTreeMap::new();
    let mut record_shape_conflicts: BTreeSet<String> = BTreeSet::new();
    let mut shared_enums = BTreeMap::new();
    let mut referenced_record_types = BTreeSet::new();
    let mut uses_slate_support = false;
    let loaded_variants = load_library_variants_parallel(
        &command_map,
        project_dir,
        "translate-project --lib --compile-commands",
    )?;
    for loaded_variant in loaded_variants {
        uses_slate_support |= !loaded_variant.warning_items.is_empty();
        let variant_facts = facts.entry(loaded_variant.cfg.clone()).or_default();
        for symbol in frontend::defined_functions(&loaded_variant.module) {
            variant_facts
                .defined
                .insert(symbol, loaded_variant.stem.clone());
        }
        for symbol in frontend::defined_globals(&loaded_variant.module) {
            variant_facts
                .defined_globals
                .insert(symbol, loaded_variant.stem.clone());
        }
        variant_facts
            .unsafe_functions
            .extend(frontend::unsafe_defined_functions(&loaded_variant.module));
        variant_facts
            .address_taken_functions
            .extend(frontend::address_taken_functions(&loaded_variant.module));
        variant_facts
            .crate_features
            .extend(frontend::required_features(&loaded_variant.module));
        variant_facts
            .cross_referenced_functions
            .extend(frontend::declared_functions(&loaded_variant.module));
        variant_facts
            .cross_referenced_globals
            .extend(frontend::declared_globals(&loaded_variant.module));
        variant_facts.has_setlocale |= frontend::declared_functions(&loaded_variant.module)
            .iter()
            .any(|name| name == "setlocale");
        merge_shared_types_for_module(
            &loaded_variant.module,
            &loaded_variant.unit,
            &mut shared_enums,
            &mut shared_records,
            &mut record_shape_conflicts,
            &mut referenced_record_types,
        );
        loaded.push(loaded_variant);
    }
    for name in referenced_record_types {
        shared_records.entry(name.clone()).or_insert(c_ast::Record {
            name,
            comments: Vec::new(),
            kind: c_ast::RecordKind::Struct,
            fields: Vec::new(),
            packed: None,
            align: None,
        });
    }
    shared_records.retain(|name, _| !record_shape_conflicts.contains(name));

    let shared_record_names: BTreeSet<_> = shared_records.keys().cloned().collect();
    let shared_enum_names: BTreeSet<_> = shared_enums.keys().cloned().collect();
    let shared_long_double = frontend::shared_types_use_long_double(
        &shared_records.values().cloned().collect::<Vec<_>>(),
    );
    let has_setlocale = facts.values().any(|facts| facts.has_setlocale);
    let fixup_skip = if has_setlocale {
        backend::SkipSet::skip(backend::Pass::CTypeLibc)
    } else {
        backend::SkipSet::none()
    };
    let cfgs: Vec<_> = variant_targets.keys().cloned().collect();
    let crate_features: BTreeSet<_> = facts
        .values()
        .flat_map(|facts| facts.crate_features.iter().copied())
        .collect();

    let mut loaded_by_stem: BTreeMap<String, Vec<LoadedLibraryVariant>> = BTreeMap::new();
    for variant in loaded {
        loaded_by_stem
            .entry(variant.stem.clone())
            .or_default()
            .push(variant);
    }
    let mut shims = BTreeMap::new();
    let mut written = Vec::new();
    let mut project_modules = ProjectModules::default();
    for (stem, variants) in &loaded_by_stem {
        let mut programs = Vec::new();
        for cfg in &cfgs {
            let Some(variant) = variants.iter().find(|variant| &variant.cfg == cfg) else {
                programs.push((cfg.clone(), rust_ast::Program::default()));
                continue;
            };
            project_modules.collect_shared_type_inputs(&variant.module);
            let variant_facts = facts.get(cfg).expect("variant facts");
            let project = frontend::ProjectInfo {
                cross_module: variant_facts.defined.clone(),
                cross_module_globals: variant_facts.defined_globals.clone(),
                shared_records: shared_record_names.clone(),
                shared_enums: shared_enum_names.clone(),
                shared_type_module: Some("types".into()),
                shared_type_crate: None,
                shared_long_double,
                cross_module_crate: None,
                unsafe_functions: variant_facts.unsafe_functions.clone(),
                address_taken_functions: variant_facts.address_taken_functions.clone(),
                crate_features: crate_features.clone(),
                child_modules: Vec::new(),
                emit_pub: true,
                cross_referenced_functions: variant_facts.cross_referenced_functions.clone(),
                cross_referenced_globals: variant_facts.cross_referenced_globals.clone(),
            };
            let mut context = ctx::Ctx::default();
            let mut program = frontend::lower_with_project(
                &variant.module,
                &variant.unit,
                &mut context,
                &project,
            );
            for diagnostic in &context.diagnostics.items {
                eprintln!("{:?}: {}", diagnostic.severity, diagnostic.message);
            }
            if context.diagnostics.has_errors() {
                return Err(format!("lowering failed for {}", variant.path.display()));
            }
            directive_translate::insert_directive_items(
                &mut program,
                variant.warning_items.clone(),
            );
            programs.push((cfg.clone(), backend::apply_with(program, &fixup_skip)));
        }
        let program = merge_target_programs(&programs);
        for shim in c_shim::collect_program_shims(&program) {
            shims
                .entry(shim.name.clone())
                .or_insert_with(|| shim.clone());
        }
        let output = crate_src.join(stem).with_extension("rs");
        project_modules.push(output, program);
    }
    let has_shared_types =
        project_modules.push_shared_types(&crate_src, shared_records, shared_enums, &mut shims);
    written.extend(project_modules.write()?);

    let mut lib_rs = String::new();
    for feature in &crate_features {
        lib_rs.push_str(&format!("#![feature({})]\n", feature.spelling()));
    }
    if has_shared_types {
        lib_rs.push_str("pub mod types;\n");
    }
    for (stem, variants) in &loaded_by_stem {
        let present: Vec<_> = cfgs
            .iter()
            .filter(|cfg| variants.iter().any(|variant| &variant.cfg == *cfg))
            .cloned()
            .collect();
        if present.len() != cfgs.len() {
            let cfg = if present.len() == 1 {
                present.into_iter().next().unwrap()
            } else {
                rust_ast::Cfg::Any(present)
            };
            lib_rs.push_str(&format!("#[cfg({})]\n", cfg.render()));
        }
        lib_rs.push_str(&format!("pub mod {};\n", codegen::escape_ident(stem)));
    }
    let lib_rs_path = crate_src.join("lib.rs");
    backend::write_rust(&lib_rs_path, &lib_rs)?;
    written.push(lib_rs_path);

    if uses_slate_support {
        write_slate_support(crate_dir)?;
    }
    let shims: Vec<_> = shims.into_values().collect();
    if !shims.is_empty() {
        write_c_shims(crate_dir, &shims)?;
    }
    write_crate_manifest(
        crate_dir,
        &package_name(crate_dir),
        &[],
        uses_slate_support,
        !shims.is_empty(),
        false,
    )?;

    Ok(written
        .into_iter()
        .map(|path| format!("wrote {}\n", path.display()))
        .collect())
}

/// Translate a directory of `.c` files (one project spanning several translation
/// units) into a Cargo crate at `crate_dir`, scaffolded the same way as
/// `--lib` (Cargo.toml, vendored `aligned`, C shims). The unit defining
/// `main` becomes `src/main.rs`, the crate's one binary entry point, and
/// declares the other units with `mod`; a prototype resolved to a sibling
/// unit becomes a module import.
fn translate_project_command(args: &[String]) -> Result<String, String> {
    let mut paths = Vec::new();
    let mut targets = Vec::new();
    let mut compile_command_paths = Vec::new();
    let mut index = 0;
    while index < args.len() {
        match args[index].as_str() {
            "--target" => {
                index += 1;
                let target = args
                    .get(index)
                    .ok_or_else(|| "--target requires a target triple".to_string())?;
                targets.push(target.clone());
            }
            "--compile-commands" => {
                index += 1;
                let commands = args
                    .get(index)
                    .ok_or_else(|| "--compile-commands requires a file".to_string())?;
                compile_command_paths.push(PathBuf::from(commands));
            }
            flag if flag.starts_with('-') => {
                return Err(format!("unknown translate-project option: {flag}"));
            }
            path => paths.push(path),
        }
        index += 1;
    }
    if paths.len() != 2 {
        return Err("translate-project requires an input directory and output directory".into());
    }
    if !compile_command_paths.is_empty() {
        if !targets.is_empty() {
            return Err("--target and --compile-commands cannot be used together".into());
        }
        return translate_project_with_compile_commands(
            Path::new(paths[0]),
            Path::new(paths[1]),
            &compile_command_paths,
        );
    }
    translate_project_with_targets(Path::new(paths[0]), Path::new(paths[1]), &targets)
}

fn translate_project(dir: &Path, out_dir: &Path) -> Result<String, String> {
    translate_project_with_targets(dir, out_dir, &[])
}

struct ProjectFactModule {
    stem: String,
    module: Module,
    unit: c_ast::Unit,
    anon_records: Vec<c_ast::Record>,
}

fn load_project_fact_module(
    path: &Path,
    dir: &Path,
    stem: &str,
) -> Result<ProjectFactModule, String> {
    reject_active_unsupported_file(path, "translate-project")?;
    let module = cli_report(frontend::cir_input::emit_module(path, &[]))?;
    let unit = cli_result(c_ast::parse_file_with_project_records(path, dir))?;
    let anon_records = frontend::anon_local_records(&module);
    Ok(ProjectFactModule {
        stem: stem.to_string(),
        module,
        unit,
        anon_records,
    })
}

fn load_project_fact_modules_parallel(
    modules: &[(String, PathBuf)],
    dir: &Path,
) -> Result<Vec<ProjectFactModule>, String> {
    let pool = rayon::ThreadPoolBuilder::new()
        .num_threads(slate_job_count())
        .build()
        .map_err(|error| format!("build worker pool: {error}"))?;
    let results: Vec<Result<ProjectFactModule, String>> = pool.install(|| {
        modules
            .par_iter()
            .map(|(stem, path)| load_project_fact_module(path, dir, stem))
            .collect()
    });
    results.into_iter().collect()
}

fn translate_project_with_targets(
    dir: &Path,
    crate_dir: &Path,
    extra_targets: &[String],
) -> Result<String, String> {
    let modules = collect_c_modules(dir)?;

    // pass 1: which unit defines which function/global, and which owns `main`.
    let mut defined: BTreeMap<String, String> = BTreeMap::new();
    let mut defined_globals: BTreeMap<String, String> = BTreeMap::new();
    let mut unsafe_functions = BTreeSet::new();
    let mut crate_features = BTreeSet::new();
    let mut root: Option<String> = None;
    let mut has_setlocale = false;
    let mut cross_referenced_functions: BTreeSet<String> = BTreeSet::new();
    let mut cross_referenced_globals: BTreeSet<String> = BTreeSet::new();
    let mut address_taken_functions: BTreeSet<String> = BTreeSet::new();
    let mut record_occurrences: BTreeMap<String, (c_ast::Record, usize)> = BTreeMap::new();
    let mut record_shape_conflicts: BTreeSet<String> = BTreeSet::new();
    let mut enum_occurrences: BTreeMap<String, (c_ast::Enum, usize)> = BTreeMap::new();
    for fact_module in load_project_fact_modules_parallel(&modules, dir)? {
        let ProjectFactModule {
            stem,
            module,
            unit,
            anon_records,
        } = fact_module;
        for sym in frontend::defined_functions(&module) {
            if sym == "main" {
                root = Some(stem.clone());
            } else {
                defined.insert(sym, stem.clone());
            }
        }
        for sym in frontend::defined_globals(&module) {
            defined_globals.insert(sym, stem.clone());
        }
        unsafe_functions.extend(frontend::unsafe_defined_functions(&module));
        address_taken_functions.extend(frontend::address_taken_functions(&module));
        crate_features.extend(frontend::required_features(&module));
        cross_referenced_functions.extend(frontend::declared_functions(&module));
        cross_referenced_globals.extend(frontend::declared_globals(&module));
        has_setlocale |= frontend::declared_functions(&module)
            .iter()
            .any(|name| name == "setlocale");
        let mut seen_enums = BTreeSet::new();
        for enm in &unit.enums {
            let name = rust_ident(&enm.name);
            if seen_enums.insert(name.clone()) {
                let entry = enum_occurrences
                    .entry(name)
                    .or_insert_with(|| (enm.clone(), 0));
                entry.1 += 1;
            }
        }
        let mut seen_records = BTreeSet::new();
        for record in unit.records.iter().chain(anon_records.iter()) {
            let name = rust_ident(&record.name);
            if seen_records.insert(name.clone()) {
                match record_occurrences.get_mut(&name) {
                    Some(entry) => {
                        if !records_have_same_shape(&entry.0, record) {
                            record_shape_conflicts.insert(name);
                        }
                        entry.1 += 1;
                    }
                    None => {
                        record_occurrences.insert(name, (record.clone(), 1));
                    }
                }
            }
        }
        for record in frontend::shim_records_for_module(&module, &unit) {
            record_occurrences.insert(rust_ident(&record.name), (record, usize::MAX));
        }
    }
    let all_records: BTreeMap<_, _> = record_occurrences
        .iter()
        .map(|(name, (record, _))| (name.clone(), record.clone()))
        .collect();
    let mut shared_records: BTreeMap<_, _> = record_occurrences
        .into_iter()
        .filter_map(|(name, (record, count))| {
            (count > 1 && !record_shape_conflicts.contains(&name)).then_some((name, record))
        })
        .collect();
    let shared_enums: BTreeMap<_, _> = enum_occurrences
        .into_iter()
        .filter_map(|(name, (enm, count))| (count > 1).then_some((name, enm)))
        .collect();
    let mut referenced_record_types = BTreeSet::new();
    for record in shared_records.values() {
        collect_record_field_type_names(record, &mut referenced_record_types);
    }
    while let Some(name) = referenced_record_types.pop_first() {
        if shared_records.contains_key(&name) || record_shape_conflicts.contains(&name) {
            continue;
        }
        let record = all_records.get(&name).cloned().unwrap_or(c_ast::Record {
            name: name.clone(),
            comments: Vec::new(),
            kind: c_ast::RecordKind::Struct,
            fields: Vec::new(),
            packed: None,
            align: None,
        });
        collect_record_field_type_names(&record, &mut referenced_record_types);
        shared_records.insert(name, record);
    }
    let shared_record_names: BTreeSet<String> = shared_records.keys().cloned().collect();
    let shared_enum_names: BTreeSet<String> = shared_enums.keys().cloned().collect();
    let shared_long_double = frontend::shared_types_use_long_double(
        &shared_records.values().cloned().collect::<Vec<_>>(),
    );
    let fixup_skip = if has_setlocale {
        backend::SkipSet::skip(backend::Pass::CTypeLibc)
    } else {
        backend::SkipSet::none()
    };
    let root = root.ok_or("translate-project: no unit defines main")?;
    for module in defined.values_mut() {
        if *module == root {
            module.clear();
        }
    }
    for module in defined_globals.values_mut() {
        if *module == root {
            module.clear();
        }
    }
    let mut siblings: Vec<String> = modules
        .iter()
        .map(|(stem, _)| stem.clone())
        .filter(|stem| *stem != root)
        .collect();
    let has_shared_types = !shared_records.is_empty() || !shared_enums.is_empty();
    if has_shared_types {
        siblings.push("types".into());
    }

    init_crate(crate_dir, false)?;
    let crate_src = crate_dir.join("src");
    std::fs::create_dir_all(&crate_src)
        .map_err(|e| format!("create {}: {e}", crate_src.display()))?;

    // pass 2: lower each unit with project-wide knowledge and write its module.
    let targets = target_variants(extra_targets)?;
    let mut written = Vec::new();
    let mut shims: BTreeMap<String, rust_ast::ExternFnDecl> = BTreeMap::new();
    let mut project_modules = ProjectModules::default();
    for (stem, path) in &modules {
        let is_root = *stem == root;
        let project = frontend::ProjectInfo {
            cross_module: defined.clone(),
            cross_module_globals: defined_globals.clone(),
            unsafe_functions: unsafe_functions.clone(),
            address_taken_functions: address_taken_functions.clone(),
            child_modules: if is_root {
                siblings.clone()
            } else {
                Vec::new()
            },
            shared_records: shared_record_names.clone(),
            shared_enums: shared_enum_names.clone(),
            shared_type_module: has_shared_types.then(|| "types".into()),
            shared_type_crate: None,
            shared_long_double,
            cross_module_crate: None,
            emit_pub: true,
            crate_features: if is_root {
                crate_features.clone()
            } else {
                BTreeSet::new()
            },
            cross_referenced_functions: cross_referenced_functions.clone(),
            cross_referenced_globals: cross_referenced_globals.clone(),
        };
        let mut variant_programs = Vec::new();
        for target in &targets {
            let module = match frontend::cir_input::emit_module(path, &target.clang_args) {
                Ok(m) => m,
                Err(e) => {
                    eprintln!(
                        "translate-project: skipping target {:?} for {}: {e}",
                        target.cfg,
                        path.display()
                    );
                    continue;
                }
            };
            project_modules.collect_shared_type_inputs(&module);
            let unit = match c_ast::parse_file_with_args(path, &target.clang_args) {
                Ok(u) => u,
                Err(e) => {
                    eprintln!(
                        "translate-project: skipping target {:?} for {}: {e}",
                        target.cfg,
                        path.display()
                    );
                    continue;
                }
            };
            let mut ctx = ctx::Ctx::default();
            let program = frontend::lower_with_project(&module, &unit, &mut ctx, &project);
            for d in &ctx.diagnostics.items {
                eprintln!("{:?}: {}", d.severity, d.message);
            }
            if ctx.diagnostics.has_errors() {
                return Err(format!("lowering failed for {}", path.display()));
            }
            variant_programs.push((
                target.cfg.clone(),
                backend::apply_with(program, &fixup_skip),
            ));
        }
        let mut program = merge_target_programs(&variant_programs);
        for shim in c_shim::collect_program_shims(&program) {
            shims
                .entry(shim.name.clone())
                .or_insert_with(|| shim.clone());
        }
        let (source, _raw) =
            preprocess::read_source(path).map_err(|e| format!("read {}: {e}", path.display()))?;
        let pp = cli_result(preprocess::record_file(&source, &[]))?;
        let warning_items = project_warning_items(
            &pp,
            "translate-project",
            directive_translate::WarningBackend::Standalone,
        )?;
        directive_translate::insert_directive_items(&mut program, warning_items);
        let file = if is_root {
            "main".to_string()
        } else {
            stem.clone()
        };
        let output = crate_src.join(file).with_extension("rs");
        project_modules.push(output, program);
    }
    project_modules.push_shared_types(&crate_src, shared_records, shared_enums, &mut shims);
    written.extend(project_modules.write()?);

    let shim_output = crate_src.join("slate_shims.c");
    let has_shims = !shims.is_empty();
    if has_shims {
        let shims: Vec<_> = shims.into_values().collect();
        write_c_shims(crate_dir, &shims)?;
        written.push(shim_output);
    } else if shim_output.exists() {
        std::fs::remove_file(&shim_output)
            .map_err(|e| format!("remove {}: {e}", shim_output.display()))?;
    }
    write_crate_manifest(
        crate_dir,
        &package_name(crate_dir),
        &[],
        false,
        has_shims,
        true,
    )?;

    Ok(written
        .into_iter()
        .map(|path| format!("wrote {}\n", path.display()))
        .collect())
}

/// Translate a directory of `.c` files into a Cargo crate under `crate_dir`,
/// using per-file compiler arguments from a compile commands database
/// instead of directory scanning and a shared clang invocation.
struct CommandFactModule {
    stem: String,
    module: Module,
    unit: c_ast::Unit,
}

fn load_command_fact_module(
    stem: &str,
    path: &Path,
    project_dir: &Path,
    primary: &compile_commands::CompileCommand,
    context: &str,
) -> Result<CommandFactModule, String> {
    let args = compile_command_args(primary)?;
    let (source, _raw) =
        preprocess::read_source(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let pp = cli_result(preprocess::record_translation_unit(path, &source, &args))?;
    reject_active_unsupported(&pp, context)?;
    let module = cli_report(frontend::cir_input::emit_module(path, &args))?;
    let unit = cli_result(c_ast::parse_file_with_project_records_and_args(
        path,
        project_dir,
        &args,
    ))?;
    Ok(CommandFactModule {
        stem: stem.to_string(),
        module,
        unit,
    })
}

fn load_command_fact_modules_parallel(
    modules: &[(String, PathBuf)],
    variants_by_path: &BTreeMap<PathBuf, Vec<(rust_ast::Cfg, compile_commands::CompileCommand)>>,
    project_dir: &Path,
    context: &str,
) -> Result<Vec<CommandFactModule>, String> {
    let pool = rayon::ThreadPoolBuilder::new()
        .num_threads(slate_job_count())
        .build()
        .map_err(|error| format!("build worker pool: {error}"))?;
    let results: Vec<Result<CommandFactModule, String>> = pool.install(|| {
        modules
            .par_iter()
            .map(|(stem, path)| {
                let variants = variants_by_path
                    .get(path)
                    .expect("module has a compile command variant");
                let (_, primary) = &variants[0];
                load_command_fact_module(stem, path, project_dir, primary, context)
            })
            .collect()
    });
    results.into_iter().collect()
}

fn translate_project_with_compile_commands(
    project_dir: &Path,
    crate_dir: &Path,
    database_paths: &[PathBuf],
) -> Result<String, String> {
    let commands = cli_result(compile_commands::read(database_paths))?;
    let mut command_map: BTreeMap<(PathBuf, rust_ast::Cfg), compile_commands::CompileCommand> =
        BTreeMap::new();
    let mut paths_by_stem: BTreeMap<String, PathBuf> = BTreeMap::new();
    for command in commands {
        let stem = command
            .file
            .file_stem()
            .and_then(|stem| stem.to_str())
            .map(rust_ident)
            .ok_or_else(|| format!("bad file stem: {}", command.file.display()))?;
        if let Some(previous) = paths_by_stem.insert(stem.clone(), command.file.clone())
            && previous != command.file
        {
            return Err(format!(
                "compile commands map both {} and {} to module {}",
                previous.display(),
                command.file.display(),
                stem
            ));
        }
        let cfg = target_cfg(&command.target)?;
        if let Some(previous) =
            command_map.insert((command.file.clone(), cfg.clone()), command.clone())
            && previous.args != command.args
        {
            return Err(format!(
                "compile commands provide conflicting variants for {} and target {}",
                command.file.display(),
                command.target
            ));
        }
    }
    let modules: Vec<(String, PathBuf)> = paths_by_stem.into_iter().collect();
    if modules.is_empty() {
        return Err("translate-project --compile-commands: no C translation units".into());
    }
    let mut variants_by_path: BTreeMap<
        PathBuf,
        Vec<(rust_ast::Cfg, compile_commands::CompileCommand)>,
    > = BTreeMap::new();
    for ((path, cfg), command) in command_map {
        variants_by_path
            .entry(path)
            .or_default()
            .push((cfg, command));
    }

    // pass 1: which unit defines which function/global, and which owns `main`.
    let mut defined: BTreeMap<String, String> = BTreeMap::new();
    let mut defined_globals: BTreeMap<String, String> = BTreeMap::new();
    let mut unsafe_functions = BTreeSet::new();
    let mut crate_features = BTreeSet::new();
    let mut root: Option<String> = None;
    let mut has_setlocale = false;
    let mut cross_referenced_functions: BTreeSet<String> = BTreeSet::new();
    let mut cross_referenced_globals: BTreeSet<String> = BTreeSet::new();
    let mut address_taken_functions: BTreeSet<String> = BTreeSet::new();
    let mut record_occurrences: BTreeMap<String, (c_ast::Record, usize)> = BTreeMap::new();
    let mut record_shape_conflicts: BTreeSet<String> = BTreeSet::new();
    let mut enum_occurrences: BTreeMap<String, (c_ast::Enum, usize)> = BTreeMap::new();
    for fact_module in load_command_fact_modules_parallel(
        &modules,
        &variants_by_path,
        project_dir,
        "translate-project --compile-commands",
    )? {
        let CommandFactModule { stem, module, unit } = fact_module;
        for sym in frontend::defined_functions(&module) {
            if sym == "main" {
                root = Some(stem.clone());
            } else {
                defined.insert(sym, stem.clone());
            }
        }
        for sym in frontend::defined_globals(&module) {
            defined_globals.insert(sym, stem.clone());
        }
        unsafe_functions.extend(frontend::unsafe_defined_functions(&module));
        address_taken_functions.extend(frontend::address_taken_functions(&module));
        crate_features.extend(frontend::required_features(&module));
        cross_referenced_functions.extend(frontend::declared_functions(&module));
        cross_referenced_globals.extend(frontend::declared_globals(&module));
        has_setlocale |= frontend::declared_functions(&module)
            .iter()
            .any(|name| name == "setlocale");
        let mut seen_enums = BTreeSet::new();
        for enm in &unit.enums {
            let name = rust_ident(&enm.name);
            if seen_enums.insert(name.clone()) {
                let entry = enum_occurrences
                    .entry(name)
                    .or_insert_with(|| (enm.clone(), 0));
                entry.1 += 1;
            }
        }
        let mut seen_records = BTreeSet::new();
        for record in &unit.records {
            let name = rust_ident(&record.name);
            if seen_records.insert(name.clone()) {
                match record_occurrences.get_mut(&name) {
                    Some(entry) => {
                        if !records_have_same_shape(&entry.0, record) {
                            record_shape_conflicts.insert(name);
                        }
                        entry.1 += 1;
                    }
                    None => {
                        record_occurrences.insert(name, (record.clone(), 1));
                    }
                }
            }
        }
        for record in frontend::shim_records_for_module(&module, &unit) {
            record_occurrences.insert(rust_ident(&record.name), (record, usize::MAX));
        }
    }
    let all_records: BTreeMap<_, _> = record_occurrences
        .iter()
        .map(|(name, (record, _))| (name.clone(), record.clone()))
        .collect();
    let mut shared_records: BTreeMap<_, _> = record_occurrences
        .into_iter()
        .filter_map(|(name, (record, count))| {
            (count > 1 && !record_shape_conflicts.contains(&name)).then_some((name, record))
        })
        .collect();
    let shared_enums: BTreeMap<_, _> = enum_occurrences
        .into_iter()
        .filter_map(|(name, (enm, count))| (count > 1).then_some((name, enm)))
        .collect();
    let mut referenced_record_types = BTreeSet::new();
    for record in shared_records.values() {
        collect_record_field_type_names(record, &mut referenced_record_types);
    }
    while let Some(name) = referenced_record_types.pop_first() {
        if shared_records.contains_key(&name) || record_shape_conflicts.contains(&name) {
            continue;
        }
        let record = all_records.get(&name).cloned().unwrap_or(c_ast::Record {
            name: name.clone(),
            comments: Vec::new(),
            kind: c_ast::RecordKind::Struct,
            fields: Vec::new(),
            packed: None,
            align: None,
        });
        collect_record_field_type_names(&record, &mut referenced_record_types);
        shared_records.insert(name, record);
    }
    let shared_record_names: BTreeSet<String> = shared_records.keys().cloned().collect();
    let shared_enum_names: BTreeSet<String> = shared_enums.keys().cloned().collect();
    let shared_long_double = frontend::shared_types_use_long_double(
        &shared_records.values().cloned().collect::<Vec<_>>(),
    );
    let fixup_skip = if has_setlocale {
        backend::SkipSet::skip(backend::Pass::CTypeLibc)
    } else {
        backend::SkipSet::none()
    };
    let root = root.ok_or("translate-project --compile-commands: no unit defines main")?;
    for module in defined.values_mut() {
        if *module == root {
            module.clear();
        }
    }
    for module in defined_globals.values_mut() {
        if *module == root {
            module.clear();
        }
    }
    let mut siblings: Vec<String> = modules
        .iter()
        .map(|(stem, _)| stem.clone())
        .filter(|stem| *stem != root)
        .collect();
    let has_shared_types = !shared_records.is_empty() || !shared_enums.is_empty();
    if has_shared_types {
        siblings.push("types".into());
    }

    init_crate(crate_dir, false)?;
    let crate_src = crate_dir.join("src");
    std::fs::create_dir_all(&crate_src)
        .map_err(|e| format!("create {}: {e}", crate_src.display()))?;

    // pass 2: lower each unit with project-wide knowledge and write its module.
    let mut written = Vec::new();
    let mut shims: BTreeMap<String, rust_ast::ExternFnDecl> = BTreeMap::new();
    let mut project_modules = ProjectModules::default();
    for (stem, path) in &modules {
        let is_root = *stem == root;
        let project = frontend::ProjectInfo {
            cross_module: defined.clone(),
            cross_module_globals: defined_globals.clone(),
            unsafe_functions: unsafe_functions.clone(),
            address_taken_functions: address_taken_functions.clone(),
            child_modules: if is_root {
                siblings.clone()
            } else {
                Vec::new()
            },
            shared_records: shared_record_names.clone(),
            shared_enums: shared_enum_names.clone(),
            shared_type_module: has_shared_types.then(|| "types".into()),
            shared_type_crate: None,
            shared_long_double,
            cross_module_crate: None,
            emit_pub: true,
            crate_features: if is_root {
                crate_features.clone()
            } else {
                BTreeSet::new()
            },
            cross_referenced_functions: cross_referenced_functions.clone(),
            cross_referenced_globals: cross_referenced_globals.clone(),
        };
        let variants = variants_by_path
            .get(path)
            .expect("module has a compile command variant");
        let mut variant_programs = Vec::new();
        for (cfg, command) in variants {
            let args = compile_command_args(command)?;
            let module = match frontend::cir_input::emit_module(path, &args) {
                Ok(m) => m,
                Err(e) => {
                    eprintln!(
                        "translate-project --compile-commands: skipping target {:?} for {}: {e}",
                        cfg,
                        path.display()
                    );
                    continue;
                }
            };
            project_modules.collect_shared_type_inputs(&module);
            let unit = match c_ast::parse_file_with_project_records_and_args(
                path,
                project_dir,
                &args,
            ) {
                Ok(u) => u,
                Err(e) => {
                    eprintln!(
                        "translate-project --compile-commands: skipping target {:?} for {}: {e}",
                        cfg,
                        path.display()
                    );
                    continue;
                }
            };
            let mut ctx = ctx::Ctx::default();
            let program = frontend::lower_with_project(&module, &unit, &mut ctx, &project);
            for d in &ctx.diagnostics.items {
                eprintln!("{:?}: {}", d.severity, d.message);
            }
            if ctx.diagnostics.has_errors() {
                return Err(format!("lowering failed for {}", path.display()));
            }
            variant_programs.push((cfg.clone(), backend::apply_with(program, &fixup_skip)));
        }
        let mut program = merge_target_programs(&variant_programs);
        for shim in c_shim::collect_program_shims(&program) {
            shims
                .entry(shim.name.clone())
                .or_insert_with(|| shim.clone());
        }
        let (_, primary) = &variants[0];
        let primary_args = compile_command_args(primary)?;
        let (source, _raw) =
            preprocess::read_source(path).map_err(|e| format!("read {}: {e}", path.display()))?;
        let pp = cli_result(preprocess::record_translation_unit(
            path,
            &source,
            &primary_args,
        ))?;
        let warning_items = project_warning_items(
            &pp,
            "translate-project --compile-commands",
            directive_translate::WarningBackend::Standalone,
        )?;
        directive_translate::insert_directive_items(&mut program, warning_items);
        let file = if is_root {
            "main".to_string()
        } else {
            stem.clone()
        };
        let output = crate_src.join(file).with_extension("rs");
        project_modules.push(output, program);
    }
    project_modules.push_shared_types(&crate_src, shared_records, shared_enums, &mut shims);
    written.extend(project_modules.write()?);

    let shim_output = crate_src.join("slate_shims.c");
    let has_shims = !shims.is_empty();
    if has_shims {
        let shims: Vec<_> = shims.into_values().collect();
        write_c_shims(crate_dir, &shims)?;
        written.push(shim_output);
    } else if shim_output.exists() {
        std::fs::remove_file(&shim_output)
            .map_err(|e| format!("remove {}: {e}", shim_output.display()))?;
    }
    write_crate_manifest(
        crate_dir,
        &package_name(crate_dir),
        &[],
        false,
        has_shims,
        true,
    )?;

    Ok(written
        .into_iter()
        .map(|path| format!("wrote {}\n", path.display()))
        .collect())
}

/// Record the preprocessor conditional regions of `path` (resolving active
/// branches for the given clang args) and print them as JSON for later stages.
fn record_cfg(path: &Path, clang_args: &[String]) -> Result<String, String> {
    let (source, _raw) =
        preprocess::read_source(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let pp = cli_result(preprocess::record_file(&source, clang_args))?;
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

fn emit_fixtures() -> Result<String, String> {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR"));
    let generated_roots = [
        manifest.join("tests/fixtures.generated"),
        manifest.join("tests/fixtures.cfg.generated"),
        manifest.join("tests/fixtures.multi.generated"),
        manifest.join("tests/fixtures.chibicc.generated"),
        manifest.join("tests/fixtures.c-testsuite.generated"),
        manifest.join("tests/fixtures.gcc-torture.generated"),
        manifest.join("tests/fixtures.library.generated"),
        manifest.join("tests/stdlib.generated"),
    ];
    for root in &generated_roots {
        clear_generated_dir(root)?;
    }
    let mut report = String::new();

    report.push_str(&emit_c_fixture_tree(
        &manifest.join("tests/fixtures"),
        &manifest.join("tests/fixtures.generated"),
        false,
        |_| true,
        translate,
    )?);
    report.push_str(&emit_c_fixture_tree(
        &manifest.join("tests/fixtures.cfg"),
        &manifest.join("tests/fixtures.cfg.generated"),
        false,
        |_| true,
        |path| cli_report(directive_translate::translate_directives(path)),
    )?);
    report.push_str(&emit_project_fixture_tree(
        &manifest.join("tests/fixtures.multi"),
        &manifest.join("tests/fixtures.multi.generated"),
        translate_project,
    )?);
    report.push_str(&emit_project_fixture_tree(
        &manifest.join("tests/fixtures.chibicc/supported"),
        &manifest.join("tests/fixtures.chibicc.generated/supported"),
        translate_project,
    )?);
    report.push_str(&emit_c_fixture_tree(
        &manifest.join("tests/fixtures.c-testsuite"),
        &manifest.join("tests/fixtures.c-testsuite.generated"),
        false,
        |_| true,
        translate,
    )?);
    report.push_str(&emit_c_fixture_tree(
        &manifest.join("tests/fixtures.gcc-torture"),
        &manifest.join("tests/fixtures.gcc-torture.generated"),
        false,
        |_| true,
        translate,
    )?);
    report.push_str(&emit_project_fixture_tree(
        &manifest.join("tests/fixtures.library"),
        &manifest.join("tests/fixtures.library.generated"),
        translate_project_lib_crate,
    )?);
    report.push_str(&emit_c_fixture_tree(
        &manifest.join("tests/stdlib"),
        &manifest.join("tests/stdlib.generated"),
        true,
        |relative| relative != Path::new("setjmp/setjmp.c"),
        translate,
    )?);

    Ok(report)
}

fn clear_generated_dir(path: &Path) -> Result<(), String> {
    match std::fs::symlink_metadata(path) {
        Ok(metadata) if metadata.is_dir() && !metadata.file_type().is_symlink() => {
            std::fs::remove_dir_all(path).map_err(|e| format!("remove {}: {e}", path.display()))
        }
        Ok(_) => Err(format!(
            "refusing to replace non-directory generated path {}",
            path.display()
        )),
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => Ok(()),
        Err(error) => Err(format!("inspect {}: {error}", path.display())),
    }
}

fn emit_c_fixture_tree(
    source_root: &Path,
    output_root: &Path,
    recursive: bool,
    include: impl Fn(&Path) -> bool,
    translator: impl Fn(&Path) -> Result<String, String>,
) -> Result<String, String> {
    if !source_root.is_dir() {
        return Ok(String::new());
    }
    let mut report = String::new();
    for input in collect_c_files(source_root, recursive)? {
        let relative = input
            .strip_prefix(source_root)
            .map_err(|e| format!("relativize {}: {e}", input.display()))?;
        if !include(relative) {
            continue;
        }
        let output = output_root.join(relative).with_extension("rs");
        let parent = output
            .parent()
            .ok_or_else(|| format!("missing parent: {}", output.display()))?;
        std::fs::create_dir_all(parent).map_err(|e| format!("create {}: {e}", parent.display()))?;
        let rust = translator(&input)?;
        std::fs::write(&output, rust)
            .map_err(|error| format!("write {}: {error}", output.display()))?;
        report.push_str(&format!("wrote {}\n", output.display()));
    }
    Ok(report)
}

fn collect_c_files(root: &Path, recursive: bool) -> Result<Vec<PathBuf>, String> {
    let mut files = Vec::new();
    let mut dirs = vec![root.to_path_buf()];
    while let Some(dir) = dirs.pop() {
        for entry in std::fs::read_dir(&dir).map_err(|e| format!("read {}: {e}", dir.display()))? {
            let path = entry
                .map_err(|e| format!("read {} entry: {e}", dir.display()))?
                .path();
            if recursive && path.is_dir() {
                dirs.push(path);
            } else if path.extension().and_then(|ext| ext.to_str()) == Some("c") {
                files.push(path);
            }
        }
    }
    files.sort();
    Ok(files)
}

fn emit_project_fixture_tree(
    source_root: &Path,
    output_root: &Path,
    translator: impl Fn(&Path, &Path) -> Result<String, String>,
) -> Result<String, String> {
    if !source_root.is_dir() {
        return Ok(String::new());
    }
    let mut projects = Vec::new();
    for entry in std::fs::read_dir(source_root)
        .map_err(|e| format!("read {}: {e}", source_root.display()))?
    {
        let path = entry
            .map_err(|e| format!("read {} entry: {e}", source_root.display()))?
            .path();
        if path.is_dir() {
            projects.push(path);
        }
    }
    projects.sort();

    let mut report = String::new();
    for project in projects {
        let name = project
            .file_name()
            .ok_or_else(|| format!("missing dir name: {}", project.display()))?;
        report.push_str(&translator(&project, &output_root.join(name))?);
    }
    Ok(report)
}

fn emit_lowered_fixtures() -> Result<String, String> {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR"));
    let src_dir = manifest.join("tests/fixtures");
    let out_dir = manifest.join("tests/fixtures.lowered.generated");
    std::fs::create_dir_all(&out_dir).map_err(|e| format!("create {}: {e}", out_dir.display()))?;

    let mut inputs = Vec::new();
    for entry in
        std::fs::read_dir(&src_dir).map_err(|e| format!("read {}: {e}", src_dir.display()))?
    {
        let path = entry
            .map_err(|e| format!("read {} entry: {e}", src_dir.display()))?
            .path();
        if path.extension().and_then(|e| e.to_str()) == Some("c") {
            inputs.push(path);
        }
    }
    inputs.sort();

    let mut report = String::new();
    for input in inputs {
        let name = input
            .file_stem()
            .ok_or_else(|| format!("missing file stem: {}", input.display()))?;
        let output = out_dir.join(name).with_extension("rs");
        let rust = lowered_rust(&input)?;
        std::fs::write(&output, rust)
            .map_err(|error| format!("write {}: {error}", output.display()))?;
        report.push_str(&format!("wrote {}\n", output.display()));
    }

    Ok(report)
}

fn lowered_rust(path: &Path) -> Result<String, String> {
    let (_, program) = lowered_program(path)?;
    backend::format_rust(&program.emit())
}
