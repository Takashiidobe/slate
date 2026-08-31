use slate::frontend::{c_ast::parse_file_with_args, toolchain::target_override_args};
use std::path::Path;

#[test]
fn retains_source_name_and_freebsd_symbol_version() {
    let source =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/freebsd/versioned_symbols.c");
    let mut args = target_override_args("x86_64-unknown-freebsd").expect("FreeBSD target args");
    args.push("-D__SLATE_FREEBSD_VERSION__=1501000".to_string());
    args.extend([
        "-target".to_string(),
        "x86_64-unknown-freebsd15.1".to_string(),
    ]);

    let unit = parse_file_with_args(&source, &args).expect("parse FreeBSD C AST");
    let facts = unit.call_symbol_facts();
    let fact = facts
        .values()
        .find(|fact| fact.symbol_version.is_some())
        .expect("versioned qsort_r fact");
    assert_eq!(fact.source_name, "qsort_r");
    assert_eq!(fact.foreign_name, "qsort_r");
    assert_eq!(fact.symbol_version.as_deref(), Some("FBSD_1.0"));
}
