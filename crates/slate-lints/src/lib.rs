#![feature(rustc_private, default_field_values)]
#![warn(unused_extern_crates)]

extern crate rustc_ast;
extern crate rustc_attr_ir;
extern crate rustc_errors;
extern crate rustc_hir;
extern crate rustc_lint;
extern crate rustc_lint_defs;
extern crate rustc_middle;
extern crate rustc_session;
extern crate rustc_span;

mod sensitive;
mod sensitive_field_rest;
mod sensitive_wildcard;

dylint_linting::dylint_library!();

#[unsafe(no_mangle)]
pub fn register_lints(sess: &rustc_session::Session, lint_store: &mut rustc_lint::LintStore) {
    dylint_linting::init_config(sess);
    lint_store.register_lints(&[
        sensitive_wildcard::SENSITIVE_WILDCARD,
        sensitive_field_rest::SENSITIVE_FIELD_REST,
    ]);
    lint_store.register_late_lint_pass(Box::new(|_| {
        Box::new(sensitive_wildcard::SensitiveWildcard)
    }));
    lint_store.register_late_lint_pass(Box::new(|_| {
        Box::new(sensitive_field_rest::SensitiveFieldRest)
    }));
}

#[test]
fn ui() {
    dylint_testing::ui_test(env!("CARGO_PKG_NAME"), "ui");
}
