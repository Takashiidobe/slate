#![allow(dead_code)]

mod c_ast;
mod cfg_translate;
mod cir;
mod codegen;
mod ctx;
mod fixups;
mod lower;
mod preprocess;
mod rust_ast;

use std::collections::BTreeMap;
use std::path::Path;
use std::process::ExitCode;

fn usage() -> ExitCode {
    eprintln!("usage: slate <command> [file.c]");
    eprintln!("  emit-cir    print ClangIR (generic form)");
    eprintln!("  emit-fixtures  write translated test fixtures to tests/fixtures.generated/");
    eprintln!("  translate   C -> Rust");
    eprintln!("  translate-cfg   experimental multi-config C -> Rust");
    eprintln!("  record-cfg   <file.c> [clang args...]  print preprocessor cfg regions as JSON");
    eprintln!("  translate-project  <dir> <out_dir>  cross-TU C dir -> Rust modules");
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
        Some("translate-project") => match (args.get(2), args.get(3)) {
            (Some(dir), Some(out)) => run(translate_project(Path::new(dir), Path::new(out))),
            _ => usage(),
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

    Ok(fixups::apply(program).emit())
}

/// Translate a directory of `.c` files (one project spanning several translation
/// units) into separate Rust module files under `out_dir`. The unit defining
/// `main` becomes the crate root `main.rs` and declares the other units with
/// `mod`; a prototype resolved to a sibling unit becomes a module import.
fn translate_project(dir: &Path, out_dir: &Path) -> Result<String, String> {
    let mut modules: Vec<(String, std::path::PathBuf)> = Vec::new();
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
            .ok_or_else(|| format!("bad file stem: {}", path.display()))?
            .to_string();
        modules.push((stem, path));
    }
    modules.sort();

    // pass 1: which unit defines which function/global, and which owns `main`.
    let mut defined: BTreeMap<String, String> = BTreeMap::new();
    let mut defined_globals: BTreeMap<String, String> = BTreeMap::new();
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
            child_modules: if is_root {
                siblings.clone()
            } else {
                Vec::new()
            },
            emit_pub: true,
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
    let doc = serde_json::json!({
        "file": path.to_string_lossy(),
        "chains": chains,
        "diagnostics": pp.diagnostics,
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
