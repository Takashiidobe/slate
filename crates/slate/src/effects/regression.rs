use std::path::Path;

use crate::rust_ast::Program;
use crate::{c_ast, ctx, fixups, lower};

fn idiomatized_program(path: &Path) -> Program {
    let cir_text = crate::cir::emit_generic(path).expect("emit-cir");
    let module = crate::cir::parse_module(&cir_text).expect("parse-cir");
    let unit = c_ast::parse_file(path).expect("parse Clang AST");

    let mut ctx = ctx::Ctx::default();
    let program = lower::lower(&module, &unit, &mut ctx);
    assert!(
        !ctx.diagnostics.has_errors(),
        "lowering diagnostics: {:?}",
        ctx.diagnostics.items
    );

    fixups::apply_with(program, &fixups::SkipSet::none())
}

fn assert_cir_and_rust_effects_match(fixture: &str) {
    let path = Path::new(fixture);

    let cir_text = crate::cir::emit_generic(path).expect("emit-cir");
    let module = crate::cir::parse_module(&cir_text).expect("parse-cir");
    let cir_trace = super::cir::interpret_module_main(&module);

    let program = idiomatized_program(path);
    let rust_trace = super::rust_ast::interpret_program_main(&program);

    if let Err(divergence) = super::interpreter::compare(&cir_trace, &rust_trace) {
        panic!(
            "{divergence}\ncir trace: {:#?}\nrust_ast trace: {:#?}",
            cir_trace, rust_trace
        );
    }
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
