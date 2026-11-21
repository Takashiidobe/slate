use crate::def_use::DefUse;
use foldhash::{HashMap, HashSet};
use slate_parser::ast::{MacroExpansionId, NodeId, Span};
use slate_parser::ir::{
    BindingId, Module, PlaceKind, Statement, Type, Visitor, walk_body_with_macros,
};

use super::inline_temporaries::record_captures;

pub(super) fn transform(module: &mut Module) {
    let mut captured = record_captures(module);
    captured.extend(module.macro_hidden_arguments.values().flatten().copied());
    let mut retired = Vec::new();
    for function in &mut module.functions {
        if let Some(body) = &mut function.value.body {
            let def_use = DefUse::of(body);
            let mut scan = Candidates::default();
            walk_body_with_macros(body, &module.macro_values, &mut scan);
            scan.temporaries.retain(|id| {
                !captured.contains(id)
                    && def_use.only_writes(*id, scan.writes.get(id).copied().unwrap_or(0))
            });
            if !scan.temporaries.is_empty() {
                walk_body_with_macros(
                    body,
                    &module.macro_values,
                    &mut Discard {
                        dead: &scan.temporaries,
                        retired: &mut retired,
                        depth: 0,
                    },
                );
            }
        }
    }
    for (node, binding) in retired {
        module.c_types.bindings.remove(&binding);
        module.c_types.nodes.remove(&node);
        module.metadata.remove(&node);
    }
}

fn direct_write(statement: &Statement) -> Option<BindingId> {
    let Statement::Write {
        place,
        value,
        ordering: None,
        unsequenced: false,
    } = statement
    else {
        return None;
    };
    match place.kind {
        PlaceKind::Binding(id) if place.access.is_plain() && place.ty == value.ty => Some(id),
        _ => None,
    }
}

#[derive(Default)]
struct Candidates {
    temporaries: HashSet<BindingId>,
    writes: HashMap<BindingId, usize>,
    depth: usize,
}

impl Visitor for Candidates {
    fn enter_expansion(&mut self, _: MacroExpansionId) {
        self.depth += 1;
    }

    fn leave_expansion(&mut self, _: MacroExpansionId) {
        self.depth -= 1;
    }

    fn enter_value_hole(&mut self, expansion: MacroExpansionId) {
        self.enter_expansion(expansion);
    }

    fn leave_value_hole(&mut self, expansion: MacroExpansionId) {
        self.leave_expansion(expansion);
    }

    fn statement(&mut self, statement: &Statement) {
        if self.depth != 0 {
            return;
        }
        if let Statement::Temporary {
            id,
            ty: Type::Bool | Type::Numeric(_) | Type::Pointer { .. },
            unsequenced: false,
            ..
        } = statement
        {
            self.temporaries.insert(*id);
        } else if let Some(id) = direct_write(statement) {
            *self.writes.entry(id).or_default() += 1;
        }
    }
}

struct Discard<'a> {
    dead: &'a HashSet<BindingId>,
    retired: &'a mut Vec<(NodeId, BindingId)>,
    depth: usize,
}

impl Visitor for Discard<'_> {
    fn enter_expansion(&mut self, _: MacroExpansionId) {
        self.depth += 1;
    }

    fn leave_expansion(&mut self, _: MacroExpansionId) {
        self.depth -= 1;
    }

    fn enter_value_hole(&mut self, expansion: MacroExpansionId) {
        self.enter_expansion(expansion);
    }

    fn leave_value_hole(&mut self, expansion: MacroExpansionId) {
        self.leave_expansion(expansion);
    }

    fn rewrite_statement_list(&mut self, body: &mut Vec<Span<Statement>>) {
        if self.depth != 0 {
            return;
        }
        body.retain_mut(|statement| {
            if let Statement::Temporary {
                id, initializer, ..
            } = &mut statement.value
                && self.dead.contains(id)
            {
                self.retired.push((statement.id, *id));
                if let Some(value) = initializer.take() {
                    statement.value = Statement::Expression(value);
                } else {
                    return false;
                }
            } else if direct_write(&statement.value).is_some_and(|id| self.dead.contains(&id))
                && let Statement::Write { value, .. } =
                    std::mem::replace(&mut statement.value, Statement::Null)
            {
                statement.value = Statement::Expression(value);
            }
            true
        });
    }
}
