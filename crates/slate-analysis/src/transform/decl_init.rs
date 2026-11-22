use crate::def_use::{DefUse, captured};
use foldhash::{HashMap, HashSet};
use slate_parser::ast::{MacroExpansionId, NodeId, Span};
use slate_parser::ir::{
    BindingId, Evaluation, Function, PlaceKind, Statement, StorageDuration, Type, Value, ValueKind,
    Variable, Visitor, walk_function_body_with_macros,
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

fn branch_writes(statement: &Span<Statement>, variable: &Variable) -> bool {
    if !matches!(variable.ty, Type::Bool | Type::Numeric(_)) {
        return false;
    }
    let Statement::If {
        then_body,
        else_body: Some(else_body),
        folded: None,
        ..
    } = &statement.value
    else {
        return false;
    };
    let arm = |body: &[Span<Statement>]| {
        let (body, blocks) = match body {
            [
                Span {
                    value: Statement::Block(body),
                    ..
                },
            ] => (body.as_slice(), 1),
            _ => (body, 0),
        };
        let (last, prefix) = body.split_last()?;
        (matches!(&last.value, Statement::Write { place, value, ordering: None, unsequenced: false }
            if matches!(place.kind, PlaceKind::Binding(id) if id == variable.id)
                && place.access.is_plain()
                && place.ty == variable.ty
                && value.ty == variable.ty)
            && prefix.iter().all(|statement| {
                matches!(
                    &statement.value,
                    Statement::Let(variable) if scalar_local(variable)
                ) || matches!(
                    statement.value,
                    Statement::Expression(_) | Statement::Temporary { .. } | Statement::Null
                )
            }))
        .then_some(body.len() + blocks)
    };
    let (Some(then_len), Some(else_len)) = (arm(then_body), arm(else_body)) else {
        return false;
    };
    DefUse::of(std::slice::from_ref(statement)).straight_line_writes(
        variable.id,
        then_len + else_len + 1,
        2,
    )
}

fn arm_value(mut body: Vec<Span<Statement>>, retired: &mut Vec<NodeId>) -> Option<Value> {
    if matches!(
        body.as_slice(),
        [Span {
            value: Statement::Block(_),
            ..
        }]
    ) {
        let block = body.pop()?;
        if let Statement::Block(statements) = block.value {
            retired.push(block.id);
            body = statements;
        }
    }
    let write = body.pop()?;
    let site = write.erased();
    let Statement::Write { value, .. } = write.value else {
        return None;
    };
    retired.push(write.id);
    let ty = value.ty.clone();
    Some(Value {
        ty,
        node: site.derive(ValueKind::StatementExpression(Box::new(Evaluation {
            statements: body,
            value,
        }))),
    })
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
            if body
                .get(index + 1)
                .is_some_and(|next| branch_writes(next, variable))
            {
                targets.insert(index, index + 1);
            } else if let Some(offset) = first_write(&body[index + 1..], variable) {
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
                let Statement::Let(variable) = &mut declaration.value else {
                    unreachable!("decl-init plans only local declarations");
                };
                let site = statement.erased();
                variable.initializer = Some(match statement.value {
                    Statement::Write { value, .. } => {
                        self.retired.push(statement.id);
                        value
                    }
                    Statement::If {
                        condition,
                        then_body,
                        else_body: Some(else_body),
                        ..
                    } => {
                        let (Some(then_value), Some(else_value)) = (
                            arm_value(then_body, &mut self.retired),
                            arm_value(else_body, &mut self.retired),
                        ) else {
                            unreachable!("decl-init plans only branches ending in writes");
                        };
                        self.retired.push(statement.id);
                        Value {
                            ty: variable.ty.clone(),
                            node: site.derive(ValueKind::Conditional {
                                condition: Box::new(condition),
                                then_value: Box::new(then_value),
                                else_value: Box::new(else_value),
                            }),
                        }
                    }
                    _ => unreachable!("decl-init plans only direct and conditional writes"),
                });
                statement = declaration;
            }
            body.push(statement);
        }
    }
}
