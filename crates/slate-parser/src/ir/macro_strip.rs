use super::*;
use crate::ast::{MacroExpansionId, Span};

struct KeepOnly<F> {
    keep: F,
}

impl<F: Fn(MacroExpansionId) -> bool> Visitor for KeepOnly<F> {
    fn keep(&self, expansion: MacroExpansionId) -> bool {
        (self.keep)(expansion)
    }
}

impl Module {
    pub fn strip_macro_structure(&mut self, keep: impl Fn(MacroExpansionId) -> bool) {
        self.macro_written_casts
            .retain(|_, cast| keep(cast.expansion));
        self.macro_values.retain(|_, wraps| {
            wraps.retain(|wrap| keep(wrap.expansion()));
            !wraps.is_empty()
        });
        let mut visitor = KeepOnly { keep };
        for function in &mut self.functions {
            if let Some(body) = &mut function.value.body {
                walk_body(body, &mut visitor);
            }
        }
    }
}

pub fn strip_all_macro_structure(body: &mut Vec<Span<Statement>>) {
    walk_body(body, &mut KeepOnly { keep: |_| false });
}

pub fn strip_all_macro_structure_in_value(value: &mut Value) {
    walk_value(value, &mut KeepOnly { keep: |_| false });
}
