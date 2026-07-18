use std::path::Path;

use crate::cir::ir::{Attr, Op};
use crate::rust_ast::{FnDef, Item};
use crate::{c_ast, ctx, fixups, lower};

fn main_cir_ops(module: &crate::cir::ir::Module) -> Vec<Op> {
    let builtin_module = &module.ops[0];
    let top_level = &builtin_module.regions[0].blocks[0].ops;
    let main_fn = top_level
        .iter()
        .find(|op| op.attrs.get("sym_name").and_then(Attr::as_str) == Some("main"))
        .expect("fixture must define `main`");
    main_fn.regions[0].blocks[0].ops.clone()
}

fn idiomatized_main(path: &Path) -> FnDef {
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

    let program = fixups::apply_with(program, &fixups::SkipSet::none());
    program
        .items
        .into_iter()
        .find_map(|item| match item {
            Item::Fn(f) if f.name == "main" => Some(f),
            _ => None,
        })
        .expect("fixture must lower to a `main` fn")
}

fn assert_cir_and_rust_effects_match(fixture: &str) {
    let path = Path::new(fixture);

    let cir_text = crate::cir::emit_generic(path).expect("emit-cir");
    let module = crate::cir::parse_module(&cir_text).expect("parse-cir");
    let cir_trace = super::cir::interpret(&main_cir_ops(&module));

    let main_fn = idiomatized_main(path);
    let rust_trace = super::rust_ast::interpret(&main_fn);

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
