use std::collections::BTreeSet;

use crate::fixups::Fixup;
use crate::fixups::idents::expr_ident_count;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, stmts_snippet,
};
use crate::rust_ast::{Expr, IndentStmt, Stmt, UnaryOp};

pub(in crate::fixups) fn fixup(body: &mut Vec<IndentStmt>) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    StructFieldInit::new(&mut logger).fixup(body)
}

pub(in crate::fixups) struct StructFieldInit<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for StructFieldInit<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        let before = self.logger.is_enabled().then(|| body.clone());
        let changed = fixup_at(body);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::StructFieldInit,
                kind: "fold_struct_field_initializers".into(),
                location: TraceLocation::default(),
                before: vec![stmts_snippet("body", &before)],
                after: vec![stmts_snippet("body", body)],
                facts: Vec::new(),
            });
        }
        changed
    }
}

impl<'a> StructFieldInit<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }
}

fn fixup_at(body: &mut Vec<IndentStmt>) -> bool {
    for indent in body.iter_mut() {
        if nested_fixup(&mut indent.stmt) {
            return true;
        }
    }

    let mut index = 0;
    while index + 1 < body.len() {
        if fold_after_let(body, index) {
            return true;
        }
        index += 1;
    }
    false
}

fn nested_fixup(stmt: &mut Stmt) -> bool {
    let mut changed = false;
    walk::nested_body_vecs_mut_with_path(stmt, &mut Vec::new(), &mut |body, _| {
        if !changed && fixup_at(body) {
            changed = true;
        }
    });
    changed
}

fn fold_after_let(body: &mut Vec<IndentStmt>, let_index: usize) -> bool {
    let Some((binding, fields)) = struct_literal_binding(&body[let_index].stmt) else {
        return false;
    };
    let mut assigned = Vec::new();
    let mut seen = BTreeSet::new();
    let mut index = let_index + 1;
    while let Some((field, value)) = field_assignment(&body[index].stmt, &binding) {
        if !fields.iter().any(|existing| existing == field)
            || !seen.insert(field.to_string())
            || expr_ident_count(value, &binding) != 0
            || !is_pure_expr(value)
        {
            return false;
        }
        assigned.push((index, field.to_string(), value.clone()));
        index += 1;
        if index >= body.len() {
            break;
        }
    }
    if assigned.is_empty() {
        return false;
    }

    let Stmt::Let {
        init: Some(Expr::StructLit { fields, .. }),
        ..
    } = &mut body[let_index].stmt
    else {
        unreachable!();
    };
    for (_, assigned_field, assigned_value) in &assigned {
        let Some((_, value)) = fields.iter_mut().find(|(field, _)| field == assigned_field) else {
            unreachable!();
        };
        *value = assigned_value.clone();
    }
    for (removed, (assign_index, _, _)) in assigned.iter().enumerate() {
        body.remove(assign_index - removed);
    }
    true
}

fn struct_literal_binding(stmt: &Stmt) -> Option<(String, Vec<String>)> {
    let Stmt::Let {
        name,
        mutable: true,
        init: Some(Expr::StructLit { fields, .. }),
        ..
    } = stmt
    else {
        return None;
    };
    if fields.iter().any(|(_, value)| !is_pure_expr(value)) {
        return None;
    }
    Some((
        name.clone(),
        fields.iter().map(|(field, _)| field.clone()).collect(),
    ))
}

fn field_assignment<'a>(stmt: &'a Stmt, binding: &str) -> Option<(&'a str, &'a Expr)> {
    let Stmt::Assign {
        target: Expr::Field { base, field },
        value,
    } = stmt
    else {
        return None;
    };
    match base.as_ref() {
        Expr::Var(name) if name.as_str() == binding => Some((field.as_str(), value)),
        _ => None,
    }
}

fn is_pure_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) | Expr::Str(_) | Expr::ByteStr(_) | Expr::CStr(_) | Expr::HexFloat(_) => {
            true
        }
        Expr::Var(_) | Expr::Path(_) => true,
        Expr::Unary { op, expr } => matches!(op, UnaryOp::Neg | UnaryOp::Not) && is_pure_expr(expr),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        } => is_pure_expr(lhs) && is_pure_expr(rhs),
        Expr::Cast { expr, .. } => is_pure_expr(expr),
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => is_pure_expr(base),
        Expr::Index { base, index } => is_pure_expr(base) && is_pure_expr(index),
        Expr::StructLit { fields, .. } => fields.iter().all(|(_, value)| is_pure_expr(value)),
        Expr::TupleStructLit { fields, .. } => fields.iter().all(is_pure_expr),
        Expr::ArrayLit(values) | Expr::VecLit(values) => values.iter().all(is_pure_expr),
        Expr::ArrayRepeat { elem, .. } => is_pure_expr(elem),
        _ => false,
    }
}
