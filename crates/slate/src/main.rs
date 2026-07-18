#![allow(dead_code)]

mod c_ast;
mod cfg_translate;
mod cir;
mod codegen;
mod ctx;
mod effects;
mod fixups;
mod lower;
mod preprocess;
mod rust_ast;

use std::collections::{BTreeMap, BTreeSet};
use std::panic::{AssertUnwindSafe, catch_unwind};
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};

fn usage() -> ExitCode {
    eprintln!("usage: slate <command> [file.c]");
    eprintln!("  emit-cir    print ClangIR (generic form)");
    eprintln!("  emit-fixtures  write translated test fixtures to tests/fixtures.generated/");
    eprintln!(
        "  emit-lowered-fixtures  write raw lowered test fixtures to tests/fixtures.lowered.generated/"
    );
    eprintln!(
        "  compare-effects-cir-rust  <file.c>  compare C/CIR effects to fixuped Rust effects"
    );
    eprintln!(
        "  compare-effects-rust-rust  <file.c>  compare raw lowered Rust effects to fixuped Rust effects"
    );
    eprintln!("  translate   C -> Rust");
    eprintln!("  translate-cfg   experimental multi-config C -> Rust");
    eprintln!("  record-cfg   <file.c> [clang args...]  print preprocessor cfg regions as JSON");
    eprintln!("  translate-project  <dir> <out_dir>  cross-TU C dir -> Rust modules");
    eprintln!(
        "  translate-project --lib  <project_dir> <crate_dir>  cross-TU C library -> Cargo crate"
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
        Some("compare-effects-cir-rust") => match args.get(2) {
            Some(path) => run(compare_effects_cir_rust(Path::new(path))),
            None => usage(),
        },
        Some("compare-effects-rust-rust") => match args.get(2) {
            Some(path) => run(compare_effects_rust_rust(Path::new(path))),
            None => usage(),
        },
        Some("translate") => match args.get(2) {
            Some(path) => run(translate(Path::new(path))),
            None => usage(),
        },
        Some("translate-cfg") => match args.get(2) {
            Some(path) => run(cfg_translate::translate_cfg(Path::new(path))),
            None => usage(),
        },
        Some("record-cfg") => match args.get(2) {
            Some(path) => run(record_cfg(Path::new(path), &args[3..])),
            None => usage(),
        },
        Some("translate-project") => match args.get(2).map(String::as_str) {
            Some("--lib") => match (args.get(3), args.get(4)) {
                (Some(dir), Some(out)) => {
                    run(translate_project_lib_crate(Path::new(dir), Path::new(out)))
                }
                _ => usage(),
            },
            Some(dir) => match args.get(3) {
                Some(out) => run(translate_project(Path::new(dir), Path::new(out))),
                _ => usage(),
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
            eprintln!("error: {e}");
            ExitCode::FAILURE
        }
    }
}

fn emit_cir(path: &Path) -> Result<String, String> {
    cir::emit_generic(path)
}

/// The V0 spine: emit-cir -> parse-cir + load Clang AST -> lower -> print.
fn translate(path: &Path) -> Result<String, String> {
    let (_, program) = lowered_program(path)?;
    if std::env::var("SLATE_RAW_LOWER").is_ok() {
        return Ok(program.emit());
    }
    Ok(fixups::apply_with(program, &skip_set_from_env()?).emit())
}

fn lowered_program(path: &Path) -> Result<(cir::ir::Module, rust_ast::Program), String> {
    let cir_text = cir::emit_generic(path)?;
    let module = cir::parse_module(&cir_text)?;
    let unit = c_ast::parse_file(path)?;

    let mut ctx = ctx::Ctx::default();
    let program = lower::lower(&module, &unit, &mut ctx);
    for d in &ctx.diagnostics.items {
        eprintln!("{:?}: {}", d.severity, d.message);
    }
    if ctx.diagnostics.has_errors() {
        return Err("lowering failed".into());
    }

    Ok((module, program))
}

fn compare_traces(
    left_name: &str,
    right_name: &str,
    left: &effects::EffectTrace,
    right: &effects::EffectTrace,
) -> Result<(), String> {
    effects::interpreter::compare(left, right).map_err(|divergence| {
        format!("{divergence}\n{left_name} trace: {left:#?}\n{right_name} trace: {right:#?}")
    })
}

fn panic_payload_message(payload: Box<dyn std::any::Any + Send>) -> String {
    if let Some(message) = payload.downcast_ref::<String>() {
        return message.clone();
    }
    if let Some(message) = payload.downcast_ref::<&'static str>() {
        return message.to_string();
    }
    "panic without string payload".to_string()
}

fn run_effect_compare(check: impl FnOnce() -> Result<(), String>) -> Result<String, String> {
    let hook = std::panic::take_hook();
    std::panic::set_hook(Box::new(|_| {}));
    let result = match catch_unwind(AssertUnwindSafe(check)) {
        Ok(Ok(())) => Ok("ok\n".to_string()),
        Ok(Err(err)) => Err(err),
        Err(payload) => Err(panic_payload_message(payload)),
    };
    std::panic::set_hook(hook);
    result
}

fn extract_effects<T>(
    mode: &str,
    path: &Path,
    side: &str,
    extract: impl FnOnce() -> T,
) -> Result<T, String> {
    match catch_unwind(AssertUnwindSafe(extract)) {
        Ok(value) => Ok(value),
        Err(payload) => Err(format!(
            "effect extraction failed\nmode: {mode}\nfixture: {}\nside: {side}\nreason: {}",
            path.display(),
            panic_payload_message(payload)
        )),
    }
}

fn compare_effects_cir_rust(path: &Path) -> Result<String, String> {
    run_effect_compare(|| {
        let mode = "compare-effects-cir-rust";
        let (module, program) = lowered_program(path)?;
        let cir_trace = extract_effects(mode, path, "cir", || {
            effects::cir::interpret_module_main(&module)
        })?;
        let fixed_program = fixups::apply_with(program, &fixups::SkipSet::none());
        let rust_trace = extract_effects(mode, path, "fixuped rust_ast", || {
            effects::rust_ast::interpret_program_main(&fixed_program)
        })?;
        compare_traces("cir", "rust_ast", &cir_trace, &rust_trace)
    })
}

fn compare_effects_rust_rust(path: &Path) -> Result<String, String> {
    run_effect_compare(|| {
        let mode = "compare-effects-rust-rust";
        let (_, program) = lowered_program(path)?;
        let raw_trace = extract_effects(mode, path, "raw rust_ast", || {
            effects::rust_ast::interpret_program_main(&program)
        })?;
        let fixed_program = fixups::apply_with(program, &fixups::SkipSet::none());
        let fixed_trace = extract_effects(mode, path, "fixuped rust_ast", || {
            effects::rust_ast::interpret_program_main(&fixed_program)
        })?;
        compare_traces("raw rust_ast", "fixuped rust_ast", &raw_trace, &fixed_trace)
    })
}

/// `SLATE_SKIP_PASS=<name>` disables one named fixup pass, for
/// translation-validation regression testing (slate-4us epic): comparing a
/// fixture's translated output with and without a given pass active.
fn skip_set_from_env() -> Result<fixups::SkipSet, String> {
    match std::env::var("SLATE_SKIP_PASS") {
        Ok(name) if !name.trim().is_empty() => fixups::Pass::parse(name.trim())
            .map(fixups::SkipSet::skip)
            .ok_or_else(|| format!("unknown SLATE_SKIP_PASS: {name}")),
        _ => Ok(fixups::SkipSet::none()),
    }
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
        c_ast::CType::Record(name) => {
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
    match name.as_str() {
        "as" | "break" | "const" | "continue" | "crate" | "else" | "enum" | "extern" | "false"
        | "fn" | "for" | "if" | "impl" | "in" | "let" | "loop" | "match" | "mod" | "move"
        | "mut" | "pub" | "ref" | "return" | "self" | "Self" | "static" | "struct" | "super"
        | "trait" | "true" | "type" | "unsafe" | "use" | "where" | "while" | "async" | "await"
        | "dyn" => format!("slate_{name}"),
        _ => name,
    }
}

fn lib_crate_manifest(package: &str, tests: &[String]) -> String {
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
    format!(
        r#"[package]
name = "{package}"
version = "0.0.0"
edition = "2024"

[dependencies]
libc = "0.2"

[profile.dev]
overflow-checks = false
{test_targets}"#
    )
}

fn write_lib_crate_manifest(
    crate_dir: &Path,
    package: &str,
    tests: &[String],
) -> Result<(), String> {
    std::fs::write(
        crate_dir.join("Cargo.toml"),
        lib_crate_manifest(package, tests),
    )
    .map_err(|e| format!("write {}: {e}", crate_dir.join("Cargo.toml").display()))
}

fn init_lib_crate(crate_dir: &Path) -> Result<(), String> {
    std::fs::create_dir_all(crate_dir)
        .map_err(|e| format!("create {}: {e}", crate_dir.display()))?;
    let package = package_name(crate_dir);
    if !crate_dir.join("Cargo.toml").exists() {
        let out = Command::new(cargo())
            .args(["init", "--lib", "--vcs", "none", "--name"])
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

    write_lib_crate_manifest(crate_dir, &package, &[])?;
    let main_rs = crate_dir.join("src/main.rs");
    if main_rs.exists() {
        std::fs::remove_file(&main_rs).map_err(|e| format!("remove {}: {e}", main_rs.display()))?;
    }
    Ok(())
}

fn translate_project_lib_crate(project_dir: &Path, crate_dir: &Path) -> Result<String, String> {
    let nested_src = project_dir.join("src");
    let src_dir = if nested_src.is_dir() {
        nested_src.as_path()
    } else {
        project_dir
    };
    let modules = collect_c_modules(src_dir)?;
    if modules.is_empty() {
        return Err(format!(
            "translate-project --lib: no C files in {}",
            src_dir.display()
        ));
    }

    init_lib_crate(crate_dir)?;
    let crate_src = crate_dir.join("src");
    std::fs::create_dir_all(&crate_src)
        .map_err(|e| format!("create {}: {e}", crate_src.display()))?;

    let mut loaded_modules = Vec::new();
    let mut defined: BTreeMap<String, String> = BTreeMap::new();
    let mut defined_globals: BTreeMap<String, String> = BTreeMap::new();
    let mut unsafe_functions = BTreeSet::new();
    let mut crate_features = BTreeSet::new();
    let mut shared_records = BTreeMap::new();
    let mut shared_enums = BTreeMap::new();
    let mut referenced_record_types = BTreeSet::new();
    for (stem, path) in &modules {
        let module = cir::parse_module(&cir::emit_generic(path)?)?;
        for sym in lower::defined_functions(&module) {
            defined.insert(sym, stem.clone());
        }
        for sym in lower::defined_globals(&module) {
            defined_globals.insert(sym, stem.clone());
        }
        unsafe_functions.extend(lower::unsafe_defined_functions(&module));
        crate_features.extend(lower::required_features(&module));
        let unit = c_ast::parse_file_with_project_records(path, project_dir)?;
        for enm in &unit.enums {
            shared_enums
                .entry(rust_ident(&enm.name))
                .or_insert_with(|| enm.clone());
        }
        for record in &unit.records {
            collect_record_field_type_names(record, &mut referenced_record_types);
            shared_records
                .entry(rust_ident(&record.name))
                .or_insert_with(|| record.clone());
        }
        loaded_modules.push((stem.clone(), path.clone(), module, unit));
    }
    for name in referenced_record_types {
        shared_records.entry(name.clone()).or_insert(c_ast::Record {
            name,
            comments: Vec::new(),
            kind: c_ast::RecordKind::Struct,
            fields: Vec::new(),
            packed: false,
            align: None,
        });
    }
    let shared_record_names: BTreeSet<String> = shared_records.keys().cloned().collect();
    let shared_enum_names: BTreeSet<String> = shared_enums.keys().cloned().collect();

    let project = lower::ProjectInfo {
        cross_module: defined,
        cross_module_globals: defined_globals,
        shared_records: shared_record_names,
        shared_enums: shared_enum_names,
        shared_type_module: Some("types".into()),
        shared_type_crate: None,
        cross_module_crate: None,
        unsafe_functions,
        crate_features,
        child_modules: Vec::new(),
        emit_pub: true,
    };

    let mut written = Vec::new();
    for (stem, path, module, unit) in loaded_modules {
        let mut ctx = ctx::Ctx::default();
        let program = lower::lower_with_project(&module, &unit, &mut ctx, &project);
        for d in &ctx.diagnostics.items {
            eprintln!("{:?}: {}", d.severity, d.message);
        }
        if ctx.diagnostics.has_errors() {
            return Err(format!("lowering failed for {}", path.display()));
        }
        let output = crate_src.join(stem).with_extension("rs");
        std::fs::write(&output, fixups::apply(program).emit())
            .map_err(|e| format!("write {}: {e}", output.display()))?;
        written.push(output);
    }

    if !shared_records.is_empty() || !shared_enums.is_empty() {
        let records: Vec<_> = shared_records.into_values().collect();
        let enums: Vec<_> = shared_enums.into_values().collect();
        let output = crate_src.join("types.rs");
        std::fs::write(&output, lower::lower_shared_types(&records, &enums).emit())
            .map_err(|e| format!("write {}: {e}", output.display()))?;
        written.push(output);
    }

    let mut lib_rs = String::new();
    for feature in &project.crate_features {
        lib_rs.push_str(&format!("#![feature({})]\n", feature.spelling()));
    }
    if project.shared_type_module.is_some() {
        lib_rs.push_str("pub mod types;\n");
    }
    lib_rs.push_str(
        &modules
            .iter()
            .map(|(stem, _)| format!("pub mod {stem};\n"))
            .collect::<String>(),
    );
    let lib_rs_path = crate_src.join("lib.rs");
    std::fs::write(&lib_rs_path, lib_rs)
        .map_err(|e| format!("write {}: {e}", lib_rs_path.display()))?;
    written.push(lib_rs_path);

    let tests_dir = project_dir.join("tests");
    let mut test_modules = Vec::new();
    if tests_dir.is_dir() {
        let crate_tests = crate_dir.join("tests");
        std::fs::create_dir_all(&crate_tests)
            .map_err(|e| format!("create {}: {e}", crate_tests.display()))?;
        let package = package_name(crate_dir);
        for (stem, path) in collect_c_modules(&tests_dir)? {
            let module = cir::parse_module(&cir::emit_generic(&path)?)?;
            let unit = c_ast::parse_file_with_project_records(&path, project_dir)?;
            let test_project = lower::ProjectInfo {
                cross_module: project.cross_module.clone(),
                cross_module_globals: project.cross_module_globals.clone(),
                unsafe_functions: project.unsafe_functions.clone(),
                shared_records: project.shared_records.clone(),
                shared_enums: project.shared_enums.clone(),
                shared_type_module: Some("types".into()),
                shared_type_crate: Some(package.clone()),
                cross_module_crate: Some(package.clone()),
                crate_features: project.crate_features.clone(),
                emit_pub: true,
                ..lower::ProjectInfo::default()
            };
            let mut ctx = ctx::Ctx::default();
            let program = lower::lower_with_project(&module, &unit, &mut ctx, &test_project);
            for d in &ctx.diagnostics.items {
                eprintln!("{:?}: {}", d.severity, d.message);
            }
            if ctx.diagnostics.has_errors() {
                return Err(format!("lowering failed for {}", path.display()));
            }
            let output = crate_tests.join(&stem).with_extension("rs");
            std::fs::write(&output, fixups::apply(program).emit())
                .map_err(|e| format!("write {}: {e}", output.display()))?;
            written.push(output);
            test_modules.push(stem);
        }
    }
    write_lib_crate_manifest(crate_dir, &package_name(crate_dir), &test_modules)?;

    Ok(written
        .into_iter()
        .map(|path| format!("wrote {}\n", path.display()))
        .collect())
}

/// Translate a directory of `.c` files (one project spanning several translation
/// units) into separate Rust module files under `out_dir`. The unit defining
/// `main` becomes the crate root `main.rs` and declares the other units with
/// `mod`; a prototype resolved to a sibling unit becomes a module import.
fn translate_project(dir: &Path, out_dir: &Path) -> Result<String, String> {
    let modules = collect_c_modules(dir)?;

    // pass 1: which unit defines which function/global, and which owns `main`.
    let mut defined: BTreeMap<String, String> = BTreeMap::new();
    let mut defined_globals: BTreeMap<String, String> = BTreeMap::new();
    let mut unsafe_functions = BTreeSet::new();
    let mut crate_features = BTreeSet::new();
    let mut root: Option<String> = None;
    for (stem, path) in &modules {
        let module = cir::parse_module(&cir::emit_generic(path)?)?;
        for sym in lower::defined_functions(&module) {
            if sym == "main" {
                root = Some(stem.clone());
            } else {
                defined.insert(sym, stem.clone());
            }
        }
        for sym in lower::defined_globals(&module) {
            defined_globals.insert(sym, stem.clone());
        }
        unsafe_functions.extend(lower::unsafe_defined_functions(&module));
        crate_features.extend(lower::required_features(&module));
    }
    let root = root.ok_or("translate-project: no unit defines main")?;
    let siblings: Vec<String> = modules
        .iter()
        .map(|(stem, _)| stem.clone())
        .filter(|stem| *stem != root)
        .collect();

    std::fs::create_dir_all(out_dir).map_err(|e| format!("create {}: {e}", out_dir.display()))?;

    // pass 2: lower each unit with project-wide knowledge and write its module.
    let mut written = Vec::new();
    for (stem, path) in &modules {
        let is_root = *stem == root;
        let project = lower::ProjectInfo {
            cross_module: defined.clone(),
            cross_module_globals: defined_globals.clone(),
            unsafe_functions: unsafe_functions.clone(),
            child_modules: if is_root {
                siblings.clone()
            } else {
                Vec::new()
            },
            shared_records: BTreeSet::new(),
            shared_enums: BTreeSet::new(),
            shared_type_module: None,
            shared_type_crate: None,
            cross_module_crate: None,
            emit_pub: true,
            crate_features: if is_root {
                crate_features.clone()
            } else {
                BTreeSet::new()
            },
        };
        let module = cir::parse_module(&cir::emit_generic(path)?)?;
        let unit = c_ast::parse_file(path)?;
        let mut ctx = ctx::Ctx::default();
        let program = lower::lower_with_project(&module, &unit, &mut ctx, &project);
        for d in &ctx.diagnostics.items {
            eprintln!("{:?}: {}", d.severity, d.message);
        }
        if ctx.diagnostics.has_errors() {
            return Err(format!("lowering failed for {}", path.display()));
        }
        let file = if is_root {
            "main".to_string()
        } else {
            stem.clone()
        };
        let output = out_dir.join(file).with_extension("rs");
        std::fs::write(&output, fixups::apply(program).emit())
            .map_err(|e| format!("write {}: {e}", output.display()))?;
        written.push(output);
    }

    Ok(written
        .into_iter()
        .map(|path| format!("wrote {}\n", path.display()))
        .collect())
}

/// Record the preprocessor conditional regions of `path` (resolving active
/// branches for the given clang args) and print them as JSON for later stages.
fn record_cfg(path: &Path, clang_args: &[String]) -> Result<String, String> {
    let source =
        std::fs::read_to_string(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let pp = preprocess::record_file(&source, clang_args)?;
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
    let src_dir = manifest.join("tests/fixtures");
    let out_dir = manifest.join("tests/fixtures.generated");
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
        std::fs::write(&output, translate(&input)?)
            .map_err(|e| format!("write {}: {e}", output.display()))?;
        report.push_str(&format!("wrote {}\n", output.display()));
    }

    // multi-TU projects: each subdirectory of tests/fixtures.multi becomes a
    // directory of translated Rust modules, mirroring the single-file flow.
    let multi_src = manifest.join("tests/fixtures.multi");
    let multi_out = manifest.join("tests/fixtures.multi.generated");
    if multi_src.is_dir() {
        let mut projects = Vec::new();
        for entry in std::fs::read_dir(&multi_src)
            .map_err(|e| format!("read {}: {e}", multi_src.display()))?
        {
            let path = entry
                .map_err(|e| format!("read {} entry: {e}", multi_src.display()))?
                .path();
            if path.is_dir() {
                projects.push(path);
            }
        }
        projects.sort();
        for project in projects {
            let name = project
                .file_name()
                .ok_or_else(|| format!("missing dir name: {}", project.display()))?;
            report.push_str(&translate_project(&project, &multi_out.join(name))?);
        }
    }

    // multi-config cfg fixtures: render the portable #[cfg(...)] merge so the C
    // and generated Rust can be compared side by side. The `reject/` subdir holds
    // sources that translate-cfg is meant to refuse, so it is skipped.
    let cfg_src = manifest.join("tests/fixtures.cfg");
    let cfg_out = manifest.join("tests/fixtures.cfg.generated");
    if cfg_src.is_dir() {
        std::fs::create_dir_all(&cfg_out)
            .map_err(|e| format!("create {}: {e}", cfg_out.display()))?;
        let mut cfg_inputs = Vec::new();
        for entry in
            std::fs::read_dir(&cfg_src).map_err(|e| format!("read {}: {e}", cfg_src.display()))?
        {
            let path = entry
                .map_err(|e| format!("read {} entry: {e}", cfg_src.display()))?
                .path();
            if path.extension().and_then(|e| e.to_str()) == Some("c") {
                cfg_inputs.push(path);
            }
        }
        cfg_inputs.sort();
        for input in cfg_inputs {
            let name = input
                .file_stem()
                .ok_or_else(|| format!("missing file stem: {}", input.display()))?;
            let output = cfg_out.join(name).with_extension("rs");
            std::fs::write(&output, cfg_translate::translate_cfg(&input)?)
                .map_err(|e| format!("write {}: {e}", output.display()))?;
            report.push_str(&format!("wrote {}\n", output.display()));
        }
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
        std::fs::write(&output, lowered_rust(&input)?)
            .map_err(|e| format!("write {}: {e}", output.display()))?;
        report.push_str(&format!("wrote {}\n", output.display()));
    }

    Ok(report)
}

fn lowered_rust(path: &Path) -> Result<String, String> {
    let (_, program) = lowered_program(path)?;
    Ok(program.emit())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn cir_effect_extraction_failure_reports_mode_fixture_side_and_reason() {
        let err = extract_effects(
            "compare-effects-cir-rust",
            Path::new("tests/fixtures/switch.c"),
            "cir",
            || panic!("effects::cir: unsupported op `cir.switch`"),
        )
        .expect_err("unsupported CIR effect extraction should fail");

        assert!(err.contains("effect extraction failed"));
        assert!(err.contains("mode: compare-effects-cir-rust"));
        assert!(err.contains("fixture: tests/fixtures/switch.c"));
        assert!(err.contains("side: cir"));
        assert!(err.contains("reason: effects::cir: unsupported op `cir.switch`"));
    }

    #[test]
    fn rust_effect_extraction_failure_reports_mode_fixture_side_and_reason() {
        let err = extract_effects(
            "compare-effects-rust-rust",
            Path::new("tests/fixtures/memcpy.c"),
            "raw rust_ast",
            || panic!("effects::rust_ast: unsupported call target `memcpy`"),
        )
        .expect_err("unsupported Rust effect extraction should fail");

        assert!(err.contains("effect extraction failed"));
        assert!(err.contains("mode: compare-effects-rust-rust"));
        assert!(err.contains("fixture: tests/fixtures/memcpy.c"));
        assert!(err.contains("side: raw rust_ast"));
        assert!(err.contains("reason: effects::rust_ast: unsupported call target `memcpy`"));
    }
}
