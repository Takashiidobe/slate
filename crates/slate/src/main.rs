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
    eprintln!(
        "  fixup-debug  <file.c> [--up-to-pass <pass>|--only-pass <pass>|--debug-only-pass <pass>]  print fixup pass trace"
    );
    eprintln!(
        "  translate   [--targets=<t1>,<t2>,...] [clang args...] <file.c>  C -> Rust (auto-expands target/arch #if regions into cfg items; --targets diffs and splices target/libc-only divergence with no #if required)"
    );
    eprintln!(
        "  translate-lowered  <file.c>  C -> Rust, raw lowered output with no fixup passes applied"
    );
    eprintln!("  record-cfg   <file.c> [clang args...]  print preprocessor cfg regions as JSON");
    eprintln!(
        "  translate-project --compile-commands <file>... <project_dir> <crate_dir>  cross-TU C project -> Cargo crate (bin if a unit defines main, else lib)"
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
        Some("fixup-debug") => run(fixup_debug(&args[2..])),
        Some("translate") => match args[2..].split_last() {
            Some((path, clang_args)) => run(translate_with_clang_args(Path::new(path), clang_args)),
            None => usage(),
        },
        Some("translate-lowered") => match args.get(2) {
            Some(path) => run(lowered_rust(Path::new(path))),
            None => usage(),
        },
        Some("record-cfg") => match args.get(2) {
            Some(path) => run(record_cfg(Path::new(path), &args[3..])),
            None => usage(),
        },
        Some("translate-project") => match args.get(2) {
            Some(_) => run(translate_project_command(&args[2..])),
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
            Self::Directive(source) => source.nyi_diagnostic(),
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

fn translate_with_clang_args(path: &Path, clang_args: &[String]) -> Result<String, String> {
    let mut targets = None;
    let mut remaining = Vec::with_capacity(clang_args.len());
    for arg in clang_args {
        match arg.strip_prefix("--targets=") {
            Some(value) => targets = Some(value.split(',').map(str::to_string).collect::<Vec<_>>()),
            None => remaining.push(arg.clone()),
        }
    }
    match targets {
        Some(targets) => cli_report(api::translate_targets_with_args(path, &remaining, &targets)),
        None => cli_report(api::translate_with_args(path, clang_args)),
    }
}

fn lowered_program(path: &Path) -> Result<(Module, rust_ast::Program), String> {
    cli_report(api::lowered_program(path))
}

fn reject_active_unsupported(pp: &preprocess::Preprocessing, context: &str) -> Result<(), String> {
    cli_report(api::reject_active_unsupported(pp, context))
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

    fn cargo_features(&self) -> BTreeSet<String> {
        let mut features = BTreeSet::new();
        for program in &self.programs {
            program.cargo_features(&mut features);
        }
        features
    }

    fn drain_module_features(&mut self) -> BTreeSet<rust_ast::Feature> {
        let mut features = BTreeSet::new();
        for program in &mut self.programs {
            if let Some(rust_ast::Item::CrateAttrs(attrs)) = program.items.first_mut() {
                attrs.retain(|attr| {
                    if let rust_ast::CrateAttr::Feature(feature) = attr {
                        features.insert(*feature);
                        false
                    } else {
                        true
                    }
                });
            }
        }
        features
    }

    fn install_crate_features(&mut self, path: &Path, features: &BTreeSet<rust_ast::Feature>) {
        let Some(program) = self
            .paths
            .iter()
            .position(|p| p == path)
            .map(|index| &mut self.programs[index])
        else {
            return;
        };
        if let Some(rust_ast::Item::CrateAttrs(attrs)) = program.items.first_mut() {
            for feature in features {
                if !attrs
                    .iter()
                    .any(|attr| matches!(attr, rust_ast::CrateAttr::Feature(existing) if existing == feature))
                {
                    attrs.insert(0, rust_ast::CrateAttr::Feature(*feature));
                }
            }
        }
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
    cargo_features: &BTreeSet<String>,
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
    autobins: bool,
    cargo_features: &BTreeSet<String>,
) -> Result<(), String> {
    std::fs::write(
        crate_dir.join("Cargo.toml"),
        crate_manifest(
            package,
            tests,
            slate_support,
            c_shims,
            autobins,
            cargo_features,
        ),
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

    write_crate_manifest(
        crate_dir,
        &package,
        &[],
        false,
        false,
        !wants_lib,
        &BTreeSet::new(),
    )?;
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

fn translate_project_command(args: &[String]) -> Result<String, String> {
    let mut paths = Vec::new();
    let mut compile_command_paths = Vec::new();
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
            flag if flag.starts_with('-') => {
                return Err(format!("unknown translate-project option: {flag}"));
            }
            path => paths.push(path),
        }
        index += 1;
    }
    if paths.len() != 2 {
        return Err("translate-project requires a project directory and crate directory".into());
    }
    if compile_command_paths.is_empty() {
        return Err("translate-project requires at least one --compile-commands <file>".into());
    }
    translate_project_with_compile_commands(
        Path::new(paths[0]),
        Path::new(paths[1]),
        &compile_command_paths,
    )
}

fn compile_command_args(command: &compile_commands::CompileCommand) -> Result<Vec<String>, String> {
    let mut args = cli_result(frontend::toolchain::target_override_args(&command.target))?;
    args.extend(command.args.iter().cloned());
    Ok(args)
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

struct LoadedVariant {
    cfg: rust_ast::Cfg,
    stem: String,
    path: PathBuf,
    module: Module,
    unit: c_ast::Unit,
    anon_records: Vec<c_ast::Record>,
    pp: preprocess::Preprocessing,
}

#[derive(Default)]
struct VariantFacts {
    defined: BTreeMap<String, String>,
    defined_globals: BTreeMap<String, String>,
    unsafe_functions: BTreeSet<String>,
    address_taken_functions: BTreeSet<String>,
    crate_features: BTreeSet<rust_ast::Feature>,
    has_setlocale: bool,
    cross_referenced_functions: BTreeSet<String>,
    cross_referenced_globals: BTreeSet<String>,
}

fn load_variant(
    path: &Path,
    project_dir: &Path,
    cfg: &rust_ast::Cfg,
    command: &compile_commands::CompileCommand,
    context: &str,
) -> Result<LoadedVariant, String> {
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
    let module = cli_report(frontend::cir_input::emit_module(path, &args))?;
    let unit = cli_result(c_ast::parse_file_with_project_records_and_args(
        path,
        project_dir,
        &args,
    ))?;
    let anon_records = frontend::anon_local_records(&module);
    Ok(LoadedVariant {
        cfg: cfg.clone(),
        stem,
        path: path.to_path_buf(),
        module,
        unit,
        anon_records,
        pp,
    })
}

fn load_variants_parallel(
    command_map: &BTreeMap<(PathBuf, rust_ast::Cfg), compile_commands::CompileCommand>,
    project_dir: &Path,
    context: &str,
) -> Result<Vec<LoadedVariant>, String> {
    let pool = rayon::ThreadPoolBuilder::new()
        .num_threads(slate_job_count())
        .build()
        .map_err(|error| format!("build worker pool: {error}"))?;
    let entries: Vec<_> = command_map.iter().collect();
    let results: Vec<Result<LoadedVariant, String>> = pool.install(|| {
        entries
            .par_iter()
            .map(|((path, cfg), command)| load_variant(path, project_dir, cfg, command, context))
            .collect()
    });
    results.into_iter().collect()
}

fn lower_macro_forked_program(
    variant: &LoadedVariant,
    command: &compile_commands::CompileCommand,
    project_dir: &Path,
    project: &frontend::ProjectInfo,
    fixup_skip: &backend::SkipSet,
) -> Result<Option<rust_ast::Program>, String> {
    let (source, _raw) = preprocess::read_source(&variant.path)
        .map_err(|error| format!("read {}: {error}", variant.path.display()))?;
    let branches = directive_translate::single_chain_macro_branches(&source);
    if branches.is_empty() {
        return Ok(None);
    }
    let base_args = compile_command_args(command)?;
    let mut branch_programs = Vec::with_capacity(branches.len());
    for branch in branches {
        let mut args = base_args.clone();
        args.extend(branch.clang_args);
        let module = cli_report(frontend::cir_input::emit_module(&variant.path, &args))?;
        let unit = cli_result(c_ast::parse_file_with_project_records_and_args(
            &variant.path,
            project_dir,
            &args,
        ))?;
        let mut ctx = ctx::Ctx::default();
        let program = frontend::lower_with_project(&module, &unit, &mut ctx, project);
        for d in &ctx.diagnostics.items {
            eprintln!("{:?}: {}", d.severity, d.message);
        }
        if ctx.diagnostics.has_errors() {
            return Err(format!(
                "lowering failed for {} (feature branch)",
                variant.path.display()
            ));
        }
        branch_programs.push((branch.cfg, backend::apply_with(program, fixup_skip)));
    }
    Ok(Some(merge_target_programs(&branch_programs)))
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
        return Err("translate-project: no C translation units in compile commands".into());
    }
    let cfgs: Vec<rust_ast::Cfg> = command_map
        .keys()
        .map(|(_, cfg)| cfg.clone())
        .collect::<BTreeSet<_>>()
        .into_iter()
        .collect();

    let context = "translate-project";
    let loaded_variants = load_variants_parallel(&command_map, project_dir, context)?;

    let mut facts: BTreeMap<rust_ast::Cfg, VariantFacts> = BTreeMap::new();
    let mut main_defining_stems: BTreeSet<String> = BTreeSet::new();
    let mut record_occurrences: BTreeMap<String, (c_ast::Record, usize)> = BTreeMap::new();
    let mut record_shape_conflicts: BTreeSet<String> = BTreeSet::new();
    let mut enum_occurrences: BTreeMap<String, (c_ast::Enum, usize)> = BTreeMap::new();
    let mut loaded_by_stem: BTreeMap<String, Vec<LoadedVariant>> = BTreeMap::new();
    for variant in loaded_variants {
        let variant_facts = facts.entry(variant.cfg.clone()).or_default();
        for symbol in frontend::defined_functions(&variant.module) {
            if symbol == "main" {
                main_defining_stems.insert(variant.stem.clone());
            } else {
                variant_facts.defined.insert(symbol, variant.stem.clone());
            }
        }
        for symbol in frontend::defined_globals(&variant.module) {
            variant_facts
                .defined_globals
                .insert(symbol, variant.stem.clone());
        }
        variant_facts
            .unsafe_functions
            .extend(frontend::unsafe_defined_functions(&variant.module));
        variant_facts
            .address_taken_functions
            .extend(frontend::address_taken_functions(&variant.module));
        variant_facts
            .crate_features
            .extend(frontend::required_features(&variant.module));
        variant_facts
            .cross_referenced_functions
            .extend(frontend::declared_functions(&variant.module));
        variant_facts
            .cross_referenced_globals
            .extend(frontend::declared_globals(&variant.module));
        variant_facts.has_setlocale |= frontend::declared_functions(&variant.module)
            .iter()
            .any(|name| name == "setlocale");

        let mut seen_records = BTreeSet::new();
        for record in variant
            .unit
            .records
            .iter()
            .chain(variant.anon_records.iter())
        {
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
        let mut seen_enums = BTreeSet::new();
        for enm in &variant.unit.enums {
            let name = rust_ident(&enm.name);
            if seen_enums.insert(name.clone()) {
                let entry = enum_occurrences
                    .entry(name)
                    .or_insert_with(|| (enm.clone(), 0));
                entry.1 += 1;
            }
        }
        for record in frontend::shim_records_for_module(&variant.module, &variant.unit) {
            record_occurrences.insert(rust_ident(&record.name), (record, usize::MAX));
        }

        loaded_by_stem
            .entry(variant.stem.clone())
            .or_default()
            .push(variant);
    }

    let root = main_defining_stems.into_iter().next();
    let is_lib = root.is_none();
    let min_occurrences = if is_lib { 1 } else { 2 };

    let all_records: BTreeMap<_, _> = record_occurrences
        .iter()
        .map(|(name, (record, _))| (name.clone(), record.clone()))
        .collect();
    let mut shared_records: BTreeMap<_, _> = record_occurrences
        .into_iter()
        .filter_map(|(name, (record, count))| {
            (count >= min_occurrences && !record_shape_conflicts.contains(&name))
                .then_some((name, record))
        })
        .collect();
    let shared_enums: BTreeMap<_, _> = enum_occurrences
        .into_iter()
        .filter_map(|(name, (enm, count))| (count >= min_occurrences).then_some((name, enm)))
        .collect();
    let mut referenced_record_types = BTreeSet::new();
    for variant in loaded_by_stem.values().flatten() {
        let local_candidates: Vec<c_ast::Record> = variant
            .unit
            .records
            .iter()
            .chain(&variant.unit.anonymous_header_records)
            .chain(&variant.unit.named_header_records)
            .cloned()
            .collect();
        for record in frontend::reconcile_anonymous_member_types(
            &variant.module,
            &mut shared_records,
            &local_candidates,
        ) {
            collect_record_field_type_names(&record, &mut referenced_record_types);
        }
    }
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
    let has_setlocale = facts.values().any(|facts| facts.has_setlocale);
    let fixup_skip = if has_setlocale {
        backend::SkipSet::skip(backend::Pass::CTypeLibc)
    } else {
        backend::SkipSet::none()
    };
    let crate_features: BTreeSet<_> = facts
        .values()
        .flat_map(|facts| facts.crate_features.iter().copied())
        .collect();

    if let Some(root_stem) = &root {
        for variant_facts in facts.values_mut() {
            for module in variant_facts.defined.values_mut() {
                if module == root_stem {
                    module.clear();
                }
            }
            for module in variant_facts.defined_globals.values_mut() {
                if module == root_stem {
                    module.clear();
                }
            }
        }
    }

    let mut siblings: Vec<String> = modules
        .iter()
        .map(|(stem, _)| stem.clone())
        .filter(|stem| Some(stem) != root.as_ref())
        .collect();
    let has_shared_types = !shared_records.is_empty() || !shared_enums.is_empty();
    if has_shared_types {
        siblings.push("types".into());
    }

    init_crate(crate_dir, is_lib)?;
    let crate_src = crate_dir.join("src");
    std::fs::create_dir_all(&crate_src)
        .map_err(|e| format!("create {}: {e}", crate_src.display()))?;

    let warning_backend = if is_lib {
        directive_translate::WarningBackend::SupportMacro
    } else {
        directive_translate::WarningBackend::Standalone
    };

    let mut uses_slate_support = false;
    let mut shims: BTreeMap<String, rust_ast::ExternFnDecl> = BTreeMap::new();
    let mut written = Vec::new();
    let mut project_modules = ProjectModules::default();
    for (stem, variants) in &loaded_by_stem {
        let is_root = Some(stem) == root.as_ref();
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
                shared_type_module: has_shared_types.then(|| "types".into()),
                shared_type_crate: None,
                shared_long_double,
                cross_module_crate: None,
                unsafe_functions: variant_facts.unsafe_functions.clone(),
                address_taken_functions: variant_facts.address_taken_functions.clone(),
                child_modules: if is_root {
                    siblings.clone()
                } else {
                    Vec::new()
                },
                crate_features: if is_root {
                    crate_features.clone()
                } else {
                    BTreeSet::new()
                },
                emit_pub: true,
                cross_referenced_functions: variant_facts.cross_referenced_functions.clone(),
                cross_referenced_globals: variant_facts.cross_referenced_globals.clone(),
                strong_symbols: variant_facts.defined.keys().cloned().collect(),
            };
            let mut ctx = ctx::Ctx::default();
            let program =
                frontend::lower_with_project(&variant.module, &variant.unit, &mut ctx, &project);
            for d in &ctx.diagnostics.items {
                eprintln!("{:?}: {}", d.severity, d.message);
            }
            if ctx.diagnostics.has_errors() {
                return Err(format!("lowering failed for {}", variant.path.display()));
            }
            let program = backend::apply_with(program, &fixup_skip);
            let command = command_map
                .get(&(variant.path.clone(), variant.cfg.clone()))
                .expect("loaded variant has a compile command");
            let program =
                lower_macro_forked_program(variant, command, project_dir, &project, &fixup_skip)?
                    .unwrap_or(program);
            programs.push((cfg.clone(), program));
        }
        let mut program = merge_target_programs(&programs);
        for shim in c_shim::collect_program_shims(&program) {
            shims
                .entry(shim.name.clone())
                .or_insert_with(|| shim.clone());
        }

        let primary = variants.first().expect("stem has at least one variant");
        let warning_items = project_warning_items(&primary.pp, context, warning_backend)?;
        uses_slate_support |= !warning_items.is_empty();
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
    let cargo_features = project_modules.cargo_features();

    if is_lib {
        let mut crate_features = crate_features;
        crate_features.extend(project_modules.drain_module_features());
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
            &cargo_features,
        )?;
    } else {
        let module_features = project_modules.drain_module_features();
        project_modules.install_crate_features(&crate_src.join("main.rs"), &module_features);
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
            &cargo_features,
        )?;
    }

    Ok(written
        .into_iter()
        .map(|path| format!("wrote {}\n", path.display()))
        .collect())
}

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

fn lowered_rust(path: &Path) -> Result<String, String> {
    let (_, program) = lowered_program(path)?;
    backend::format_rust(&program.emit())
}
