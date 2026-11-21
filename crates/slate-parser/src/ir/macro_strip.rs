use super::*;
use crate::ast::{MacroExpansionId, NodeId, Span};

struct KeepOnly<F> {
    keep: F,
    unwrapped: Vec<(NodeId, NodeId)>,
}

impl<F: Fn(MacroExpansionId) -> bool> Visitor for KeepOnly<F> {
    fn keep(&self, expansion: MacroExpansionId) -> bool {
        (self.keep)(expansion)
    }

    fn unwrapped_value(&mut self, wrapper: NodeId, inner: NodeId) {
        self.unwrapped.push((wrapper, inner));
    }
}

impl<F> KeepOnly<F> {
    fn new(keep: F) -> Self {
        Self {
            keep,
            unwrapped: Vec::new(),
        }
    }
}

impl Module {
    pub fn strip_macro_structure(&mut self, keep: impl Fn(MacroExpansionId) -> bool) {
        self.macro_written_casts
            .retain(|_, cast| keep(cast.expansion));
        let mut visitor = KeepOnly::new(keep);
        for function in &mut self.functions {
            if let Some(body) = &mut function.value.body {
                walk_body(body, &mut visitor);
            }
        }
        let nodes = &mut self.c_types.nodes;
        for (wrapper, inner) in visitor.unwrapped {
            if let Some(c) = nodes.get(&wrapper).copied() {
                nodes.insert(inner, c);
            }
        }
    }
}

pub fn strip_all_macro_structure(body: &mut Vec<Span<Statement>>) -> Vec<(NodeId, NodeId)> {
    let mut visitor = KeepOnly::new(|_| false);
    walk_body(body, &mut visitor);
    visitor.unwrapped
}

pub fn strip_all_macro_structure_in_value(value: &mut Value) -> Vec<(NodeId, NodeId)> {
    let mut visitor = KeepOnly::new(|_| false);
    walk_value(value, &mut visitor);
    visitor.unwrapped
}
