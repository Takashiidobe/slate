use rustc_ast::Crate;
use rustc_errors::DiagDecorator;
use rustc_lint::{EarlyContext, EarlyLintPass, LintContext};
use rustc_span::sym;

rustc_lint_defs::declare_tool_lint! {
    pub slate::CRATE_WIDE_ALLOW,
    Deny,
    "crate-wide lint suppression with `#![allow(...)]`"
}

rustc_lint_defs::declare_lint_pass!(CrateWideAllow => [CRATE_WIDE_ALLOW]);

impl EarlyLintPass for CrateWideAllow {
    fn check_crate(&mut self, cx: &EarlyContext<'_>, krate: &Crate) {
        for attr in krate.attrs.iter().filter(|attr| attr.has_name(sym::allow)) {
            cx.emit_span_lint(
                CRATE_WIDE_ALLOW,
                attr.span,
                DiagDecorator(|diag| {
                    diag.primary_message("crate-wide `#![allow(...)]` suppresses lints");
                    diag.help("put an `#[expect(..., reason = \"...\")]` on the smallest item that needs it");
                }),
            );
        }
    }
}
