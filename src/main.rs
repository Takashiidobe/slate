#![allow(dead_code)]

mod c_ast;
mod cir;
mod ctx;
mod lower;
mod rust_ast;

use std::path::Path;
use std::process::ExitCode;

fn usage() -> ExitCode {
    eprintln!("usage: slate <command> [file.c]");
    eprintln!("  emit-cir    print ClangIR (generic form)");
    eprintln!("  emit-fixtures  write translated test fixtures to tests/fixtures.generated/");
    eprintln!("  translate   C -> Rust");
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

    Ok(program.emit())
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

    let mut written = Vec::new();
    for input in inputs {
        let name = input
            .file_stem()
            .ok_or_else(|| format!("missing file stem: {}", input.display()))?;
        let output = out_dir.join(name).with_extension("rs");
        std::fs::write(&output, translate(&input)?)
            .map_err(|e| format!("write {}: {e}", output.display()))?;
        written.push(output);
    }

    Ok(written
        .into_iter()
        .map(|path| format!("wrote {}\n", path.display()))
        .collect())
}
