use super::*;
use crate::ast::{MacroExpansionId, Span};

struct KeepOnly<F>(F);

impl<F: Fn(MacroExpansionId) -> bool> Visitor for KeepOnly<F> {
    fn keep(&self, expansion: MacroExpansionId) -> bool {
        (self.0)(expansion)
    }
}

impl Module {
    pub fn strip_macro_structure(&mut self, keep: impl Fn(MacroExpansionId) -> bool) {
        let mut visitor = KeepOnly(keep);
        for function in &mut self.functions {
            if let Some(body) = &mut function.value.body {
                walk_body(body, &mut visitor);
            }
        }
    }
}

pub fn strip_all_macro_structure(body: &mut Vec<Span<Statement>>) {
    walk_body(body, &mut KeepOnly(|_| false));
}

pub fn strip_all_macro_structure_in_value(value: &mut Value) {
    walk_value(value, &mut KeepOnly(|_| false));
}
