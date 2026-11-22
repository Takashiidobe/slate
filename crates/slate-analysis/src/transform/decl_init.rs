use crate::def_use::{DefUse, captured};
use foldhash::{HashMap, HashSet};
use slate_parser::ast::{MacroExpansionId, NodeId, Span};
use slate_parser::ir::{
    BindingId, Function, PlaceKind, Statement, StorageDuration, Type, Variable, Visitor,
    walk_function_body_with_macros,
};

use super::PassContext;

const MAX_DISTANCE: usize = 32;

pub(super) fn transform(function: &mut Function, cx: &mut PassContext) {
    let Some(body) = &mut function.body else {
        return;
    };
    let mut pass = DeclInit {
        captured: cx.captured,
        depth: 0,
        retired: Vec::new(),
    };
    walk_function_body_with_macros(body, &cx.module.macro_values, &mut pass);
    cx.retire(pass.retired);
}

fn scalar_local(variable: &Variable) -> bool {
    variable.storage == StorageDuration::Automatic
        && matches!(
            variable.ty,
            Type::Bool | Type::Numeric(_) | Type::Pointer { .. }
        )
        && captured(&variable.ty).is_empty()
        && variable.cleanup.is_none()
        && !variable.byref
}

fn eligible(variable: &Variable) -> bool {
    scalar_local(variable)
        && variable.initializer.is_none()
        && variable.auto_init.is_none()
        && variable.access.is_plain()
        && variable.alignment.is_none()
        && variable.register.is_none()
        && !variable.is_const
        && !variable.constexpr
}

fn first_write(body: &[Span<Statement>], variable: &Variable) -> Option<usize> {
    for (index, statement) in body.iter().take(MAX_DISTANCE).enumerate() {
        match &statement.value {
            Statement::Write {
                place,
                value,
                ordering,
                unsequenced,
            } if matches!(place.kind, PlaceKind::Binding(id) if id == variable.id) => {
                let range = &body[..=index];
                if ordering.is_none()
                    && !unsequenced
                    && place.access.is_plain()
                    && place.ty == variable.ty
                    && value.ty == variable.ty
                    && DefUse::of(range).straight_line_write(variable.id, range.len())
                {
                    return Some(index);
                }
                return None;
            }
            Statement::Let(variable) if scalar_local(variable) => {}
            Statement::Expression(_)
            | Statement::Temporary { .. }
            | Statement::Write { .. }
            | Statement::Null => {}
            _ => return None,
        }
    }
    None
}

struct DeclInit<'a> {
    captured: &'a HashSet<BindingId>,
    depth: usize,
    retired: Vec<NodeId>,
}

impl Visitor for DeclInit<'_> {
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

    #[expect(
        clippy::unreachable,
        reason = "each plan pairs a declaration with its direct write"
    )]
    fn rewrite_statement_list(&mut self, body: &mut Vec<Span<Statement>>) {
        if self.depth != 0 {
            return;
        }
        let mut targets = HashMap::default();
        for (index, statement) in body.iter().enumerate() {
            let Statement::Let(variable) = &statement.value else {
                continue;
            };
            if !eligible(variable)
                || self.captured.contains(&variable.id)
                || index
                    .checked_sub(1)
                    .is_some_and(|previous| matches!(body[previous].value, Statement::Comment(_)))
            {
                continue;
            }
            if let Some(offset) = first_write(&body[index + 1..], variable) {
                targets.insert(index, index + 1 + offset);
            }
        }
        if targets.is_empty() {
            return;
        }
        let mut declarations = HashMap::default();
        for (index, mut statement) in std::mem::take(body).into_iter().enumerate() {
            if let Some(target) = targets.remove(&index) {
                declarations.insert(target, statement);
                continue;
            }
            if let Some(mut declaration) = declarations.remove(&index) {
                let (Statement::Let(variable), Statement::Write { value, .. }) =
                    (&mut declaration.value, statement.value)
                else {
                    unreachable!("decl-init plans only declarations and their direct writes");
                };
                variable.initializer = Some(value);
                self.retired.push(statement.id);
                statement = declaration;
            }
            body.push(statement);
        }
    }
}
