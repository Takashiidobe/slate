use slate_parser::ast::{NodeId, Span};
use slate_parser::ir::{Function, Statement};

use super::PassContext;
use super::inline_temporaries::{children_and_expansions, evaluations};

pub(super) fn transform(function: &mut Function, cx: &mut PassContext) {
    let Some(body) = &mut function.body else {
        return;
    };
    let mut retired = Vec::new();
    list(body, &mut retired);
    cx.retire(retired);
}

fn list(body: &mut Vec<Span<Statement>>, retired: &mut Vec<NodeId>) {
    let mut index = 0;
    while index < body.len() {
        for child in children_and_expansions(&mut body[index].value) {
            list(child, retired);
        }
        for evaluation in evaluations(&mut body[index].value) {
            list(&mut evaluation.statements, retired);
        }
        match &mut body[index].value {
            Statement::Block(inner) if !scopes(inner) => {
                let inner = std::mem::take(inner);
                let length = inner.len();
                retired.push(body[index].id);
                body.splice(index..=index, inner);
                index += length;
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only a block that binds nothing at its top level splices"
            )]
            _ => index += 1,
        }
    }
}

fn scopes(body: &[Span<Statement>]) -> bool {
    body.iter().any(|statement| {
        matches!(
            statement.value,
            Statement::Let(_) | Statement::Temporary { .. } | Statement::NestedFunction(_)
        )
    })
}
