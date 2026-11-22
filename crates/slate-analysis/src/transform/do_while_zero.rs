use slate_parser::ast::{NodeId, Span};
use slate_parser::ir::{
    BindingId, Function, MacroValues, Number, Statement, Value, ValueKind, Visitor, walk_value,
};

use super::PassContext;
use super::const_fold::constant_comparison;
use super::inline_temporaries::{children, evaluations};

pub(super) fn transform(function: &mut Function, cx: &mut PassContext) {
    let Some(body) = &mut function.body else {
        return;
    };
    let mut retired = Vec::new();
    list(body, &cx.module.macro_values, &mut retired);
    cx.retire(retired);
}

fn list(body: &mut [Span<Statement>], macro_values: &MacroValues, retired: &mut Vec<NodeId>) {
    for statement in body {
        for child in children(&mut statement.value) {
            list(child, macro_values, retired);
        }
        for evaluation in evaluations(&mut statement.value) {
            list(&mut evaluation.statements, macro_values, retired);
        }
        let Statement::DoWhile {
            id,
            body,
            condition,
        } = &mut statement.value
        else {
            continue;
        };
        if !condition.statements.is_empty() || !is_false(&condition.value, macro_values) {
            continue;
        }
        let id = *id;
        let mut body = std::mem::take(body);
        let mut values = Values::default();
        walk_value(&mut condition.value, &mut values);
        retired.extend(values.0);
        statement.value = if exits(&mut body, id) {
            Statement::LabeledBlock { id, body }
        } else {
            Statement::Block(body)
        };
    }
}

fn is_false(value: &Value, macro_values: &MacroValues) -> bool {
    matches!(value.node.value, ValueKind::Constant(Number::Bool(false)))
        || constant_comparison(value, macro_values) == Some(false)
}

fn exits(body: &mut [Span<Statement>], id: BindingId) -> bool {
    let mut found = false;
    for statement in body {
        match statement.value {
            Statement::Break(target) if target == id => found = true,
            Statement::Continue(target) if target == id => {
                statement.value = Statement::Break(id);
                found = true;
            }
            _ => {}
        }
        for child in children(&mut statement.value) {
            found |= exits(child, id);
        }
        for evaluation in evaluations(&mut statement.value) {
            found |= exits(&mut evaluation.statements, id);
        }
    }
    found
}

#[derive(Default)]
struct Values(Vec<NodeId>);

impl Visitor for Values {
    fn rewrite_value(&mut self, value: &mut Value) {
        self.0.push(value.node.id);
    }
}
