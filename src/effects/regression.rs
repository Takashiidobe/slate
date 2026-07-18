use std::panic::{AssertUnwindSafe, catch_unwind};
use std::path::{Path, PathBuf};
use std::sync::{Mutex, OnceLock};

use crate::rust_ast::Program;
use crate::{c_ast, ctx, fixups, lower};

fn lowered_program(path: &Path) -> Result<(crate::cir::ir::Module, Program), String> {
    let cir_text = crate::cir::emit_generic(path).expect("emit-cir");
    let module = crate::cir::parse_module(&cir_text).expect("parse-cir");
    let unit = c_ast::parse_file(path).expect("parse Clang AST");

    let mut ctx = ctx::Ctx::default();
    let program = lower::lower(&module, &unit, &mut ctx);
    if ctx.diagnostics.has_errors() {
        return Err(format!("lowering diagnostics: {:?}", ctx.diagnostics.items));
    }

    Ok((module, program))
}

fn idiomatized_program(path: &Path) -> Program {
    let (_, program) = lowered_program(path).expect("lower fixture");
    fixups::apply_with(program, &fixups::SkipSet::none())
}

fn compare_traces(
    left_name: &str,
    right_name: &str,
    left: &super::EffectTrace,
    right: &super::EffectTrace,
) -> Result<(), String> {
    super::interpreter::compare(left, right).map_err(|divergence| {
        format!("{divergence}\n{left_name} trace: {left:#?}\n{right_name} trace: {right:#?}")
    })
}

fn cir_and_idiomatized_rust_effects_match(path: &Path) -> Result<(), String> {
    let (module, program) = lowered_program(path)?;
    let cir_trace = super::cir::interpret_module_main(&module);
    let rust_trace = super::rust_ast::interpret_program_main(&fixups::apply_with(
        program,
        &fixups::SkipSet::none(),
    ));
    compare_traces("cir", "rust_ast", &cir_trace, &rust_trace)
}

fn raw_and_idiomatized_rust_effects_match(path: &Path) -> Result<(), String> {
    let (_, program) = lowered_program(path)?;
    let raw_trace = super::rust_ast::interpret_program_main(&program);
    let fixed_trace = super::rust_ast::interpret_program_main(&fixups::apply_with(
        program,
        &fixups::SkipSet::none(),
    ));
    compare_traces("raw rust_ast", "fixuped rust_ast", &raw_trace, &fixed_trace)
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

fn run_effect_check(path: &Path, check: fn(&Path) -> Result<(), String>) -> Result<(), String> {
    static PANIC_HOOK_LOCK: OnceLock<Mutex<()>> = OnceLock::new();
    let lock = PANIC_HOOK_LOCK.get_or_init(|| Mutex::new(()));
    let _guard = lock.lock().expect("panic hook lock");
    let hook = std::panic::take_hook();
    std::panic::set_hook(Box::new(|_| {}));
    match catch_unwind(AssertUnwindSafe(|| check(path))) {
        Ok(result) => {
            std::panic::set_hook(hook);
            result
        }
        Err(payload) => {
            std::panic::set_hook(hook);
            Err(panic_payload_message(payload))
        }
    }
}

fn fixture_paths() -> Vec<(String, PathBuf)> {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures");
    let mut fixtures = Vec::new();
    for entry in std::fs::read_dir(&dir).expect("read tests/fixtures") {
        let path = entry.expect("read fixture entry").path();
        if path.extension().and_then(|ext| ext.to_str()) != Some("c") {
            continue;
        }
        let name = path
            .file_name()
            .expect("fixture file name")
            .to_string_lossy()
            .into_owned();
        fixtures.push((name, path));
    }
    fixtures.sort_by(|a, b| a.0.cmp(&b.0));
    fixtures
}

fn assert_cir_and_rust_effects_match(fixture: &str) {
    let path = Path::new(fixture);
    cir_and_idiomatized_rust_effects_match(path).expect("CIR and Rust effects match");
}

fn selected_fixture_paths() -> Vec<(String, PathBuf)> {
    let Some(filter) = std::env::var("SLATE_EFFECT_FIXTURE")
        .ok()
        .filter(|filter| !filter.trim().is_empty())
    else {
        return fixture_paths();
    };
    let filter = filter.trim();
    let selected: Vec<_> = fixture_paths()
        .into_iter()
        .filter(|(name, path)| {
            name == filter || path.file_stem().and_then(|stem| stem.to_str()) == Some(filter)
        })
        .collect();
    assert!(
        !selected.is_empty(),
        "SLATE_EFFECT_FIXTURE={filter} did not match any tests/fixtures/*.c file"
    );
    selected
}

fn assert_all_selected_fixtures_match(check: fn(&Path) -> Result<(), String>) {
    let mut failures = Vec::new();
    let mut passed = 0usize;
    for (name, path) in selected_fixture_paths() {
        match run_effect_check(&path, check) {
            Ok(()) => passed += 1,
            Err(err) => failures.push(format!("[{name}] {err}")),
        }
    }

    if failures.is_empty() {
        return;
    }

    panic!(
        "{} fixture(s) passed; {} fixture(s) failed:\n\n{}",
        passed,
        failures.len(),
        failures.join("\n\n")
    );
}

#[test]
#[ignore = "diagnostic ratchet: run explicitly while expanding effects interpreter coverage"]
fn all_fixtures_match_cir_effects() {
    assert_all_selected_fixtures_match(cir_and_idiomatized_rust_effects_match);
}

#[test]
#[ignore = "diagnostic ratchet: run explicitly while expanding raw-to-fixuped Rust effects coverage"]
fn all_fixtures_preserve_rust_effects_through_fixups() {
    assert_all_selected_fixtures_match(raw_and_idiomatized_rust_effects_match);
}

#[test]
fn idiomatized_malloc_array_fixture_matches_cir_effects() {
    assert_cir_and_rust_effects_match("tests/fixtures/effects_malloc_array.c");
}

#[test]
fn idiomatized_printf_fixture_matches_cir_effects() {
    assert_cir_and_rust_effects_match("tests/fixtures/effects_printf.c");
}

#[test]
fn idiomatized_for_loop_fixture_matches_cir_effects() {
    assert_cir_and_rust_effects_match("tests/fixtures/effects_for_loop.c");
}

#[test]
fn idiomatized_struct_field_fixture_matches_cir_effects() {
    assert_cir_and_rust_effects_match("tests/fixtures/effects_struct_field.c");
}

#[test]
fn idiomatized_static_global_fixture_matches_cir_effects() {
    assert_cir_and_rust_effects_match("tests/fixtures/effects_static_globals.c");
}

#[test]
fn idiomatized_lazy_singleton_fixture_matches_cir_effects() {
    assert_cir_and_rust_effects_match("tests/fixtures/lazy_singleton.c");
}

#[test]
fn idiomatized_nullable_pointer_fixture_matches_cir_effects() {
    assert_cir_and_rust_effects_match("tests/fixtures/effects_nullable_pointer.c");
}
