use std::collections::BTreeSet;

use crate::fixups::Fixup;
use crate::fixups::facts::{AstPath, FixupFacts, FunctionId, PathSegment, PointerOffsetUnit};
use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, function_path_location, stmts_snippet,
};
use crate::rust_ast::{Expr, Ident, IndentStmt, Prim, Stmt, Type, UnaryOp};

pub(in crate::fixups) struct SliceIndex<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for SliceIndex<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        let plans = plans_for_function(self.facts, self.function);
        if plans.is_empty() {
            return false;
        }
        let before = self.logger.is_enabled().then(|| body.clone());
        let mut changed = rewrite_body(body, &plans, &mut Vec::new());
        changed |= prune_dead_fact_temps(body, &plans);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::SliceIndex,
                kind: "rewrite_slice_pointer_indexes".into(),
                location: function_path_location(self.facts, self.function, &[]),
                before: vec![stmts_snippet("body", &before)],
                after: vec![stmts_snippet("body", body)],
                facts: vec![fact("planned_indexes", plans.len().to_string())],
            });
        }
        changed
    }
}

impl<'a> SliceIndex<'a> {
    pub(in crate::fixups) fn new(
        function: FunctionId,
        facts: &'a FixupFacts,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            function,
            facts,
            logger,
        }
    }
}

#[derive(Clone)]
struct Plan {
    path: AstPath,
    pointer_name: String,
    offset_name: String,
    slice_name: String,
    index_name: String,
    mutable: bool,
}

fn plans_for_function(facts: &FixupFacts, function: FunctionId) -> Vec<Plan> {
    let mut plans = Vec::new();
    for fact in &facts.slice_pointer_indexes {
        if fact.site.function != function || fact.unit != PointerOffsetUnit::Elements {
            continue;
        }
        let Some(view) = facts
            .slice_pointer_views
            .iter()
            .find(|view| view.site.function == fact.site.function && view.pointer == fact.pointer)
        else {
            continue;
        };
        let (Some(pointer_name), Some(offset_name), Some(slice_name), Some(index_name)) = (
            facts.binding_name(fact.pointer),
            facts.binding_name(fact.offset_index),
            facts.binding_name(fact.slice),
            facts.binding_name(fact.ranged_index),
        ) else {
            continue;
        };
        plans.push(Plan {
            path: fact.site.path.clone(),
            pointer_name: pointer_name.to_string(),
            offset_name: offset_name.to_string(),
            slice_name: slice_name.to_string(),
            index_name: index_name.to_string(),
            mutable: view.mutable,
        });
    }
    plans
}

fn rewrite_body(body: &mut [IndentStmt], plans: &[Plan], path: &mut Vec<PathSegment>) -> bool {
    let mut changed = false;
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            changed |= rewrite_stmt(&mut indent.stmt, plans, path);
            walk::nested_bodies_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                changed |= rewrite_body(body, plans, path);
            });
            changed |= unwrap_safe_singleton_unsafe(&mut indent.stmt);
        });
    }
    changed
}

fn rewrite_stmt(stmt: &mut Stmt, plans: &[Plan], path: &[PathSegment]) -> bool {
    let mut changed = false;
    let stmt_plans = plans_for_path(plans, path);
    if stmt_plans.is_empty() {
        return false;
    }
    match stmt {
        Stmt::Let {
            init: Some(init), ..
        } => changed |= rewrite_expr(init, &stmt_plans, false),
        Stmt::Assign { target, value } => {
            changed |= rewrite_expr(target, &stmt_plans, true);
            changed |= rewrite_expr(value, &stmt_plans, false);
        }
        Stmt::CompoundAssign { target, value, .. } => {
            changed |= rewrite_expr(target, &stmt_plans, true);
            changed |= rewrite_expr(value, &stmt_plans, false);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            changed |= rewrite_expr(expr, &stmt_plans, false);
        }
        Stmt::If { cond, .. } | Stmt::While { cond, .. } => {
            changed |= rewrite_expr(cond, &stmt_plans, false);
        }
        Stmt::For { iter, .. } => {
            changed |= rewrite_expr(iter, &stmt_plans, false);
        }
        Stmt::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => {
            changed |= rewrite_expr(cond, &stmt_plans, false);
            changed |= rewrite_expr(then_value, &stmt_plans, false);
            changed |= rewrite_expr(else_value, &stmt_plans, false);
        }
        Stmt::Match { expr, .. } => changed |= rewrite_expr(expr, &stmt_plans, false),
        Stmt::Let { init: None, .. }
        | Stmt::InlineAsm(_)
        | Stmt::Loop { .. }
        | Stmt::Scope { .. }
        | Stmt::LabeledBlock { .. }
        | Stmt::Unsafe { .. }
        | Stmt::Block(_)
        | Stmt::Return(None)
        | Stmt::Break(_)
        | Stmt::Continue(_) => {}
    }
    changed
}

fn plans_for_path<'a>(plans: &'a [Plan], path: &[PathSegment]) -> Vec<&'a Plan> {
    plans
        .iter()
        .filter(|plan| plan.path == AstPath(path.to_vec()))
        .collect()
}

fn rewrite_expr(expr: &mut Expr, plans: &[&Plan], write_target: bool) -> bool {
    let mut changed = false;
    walk::exprs_mut_with(expr, &mut |expr| {
        if let Some(replacement) = indexed_expr(expr, plans, write_target) {
            *expr = replacement;
            changed = true;
            return false;
        }
        true
    });
    changed
}

fn indexed_expr(expr: &Expr, plans: &[&Plan], write_target: bool) -> Option<Expr> {
    let offset = deref_offset(expr)?;
    let pointer_name = receiver_var(offset.recv)?;
    let offset_name = offset_arg_var(offset.arg)?;
    let plan = plans.iter().find(|plan| {
        plan.pointer_name == pointer_name
            && plan.offset_name == offset_name
            && (!write_target || plan.mutable)
    })?;
    Some(Expr::Index {
        base: Box::new(Expr::Var(Ident::from(plan.slice_name.as_str()))),
        index: Box::new(Expr::Cast {
            expr: Box::new(Expr::Var(Ident::from(plan.index_name.as_str()))),
            ty: Type::Prim(Prim::Usize),
        }),
    })
}

struct OffsetParts<'a> {
    recv: &'a Expr,
    arg: &'a Expr,
}

fn deref_offset(expr: &Expr) -> Option<OffsetParts<'_>> {
    match expr {
        Expr::Unsafe(block) => deref_offset(block.tail.as_deref()?),
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => offset_parts(peel_unsafe(expr)),
        _ => None,
    }
}

fn peel_unsafe(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) => block.tail.as_deref().map_or(expr, peel_unsafe),
        _ => expr,
    }
}

fn offset_parts(expr: &Expr) -> Option<OffsetParts<'_>> {
    let Expr::MethodCall { recv, method, args } = expr else {
        return None;
    };
    if method != "offset" || args.len() != 1 {
        return None;
    }
    Some(OffsetParts {
        recv,
        arg: &args[0],
    })
}

fn receiver_var(expr: &Expr) -> Option<&str> {
    let Expr::Var(name) = expr else {
        return None;
    };
    Some(name.as_str())
}

fn offset_arg_var(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Cast { expr, .. } => offset_arg_var(expr),
        Expr::Var(name) => Some(name.as_str()),
        _ => None,
    }
}

fn unwrap_safe_singleton_unsafe(stmt: &mut Stmt) -> bool {
    let Stmt::Unsafe { body } = stmt else {
        return false;
    };
    if body.tail.is_some() || body.stmts.len() != 1 {
        return false;
    }
    let inner = body.stmts[0].stmt.clone();
    if !is_safe_slice_index_stmt(&inner) {
        return false;
    }
    *stmt = inner;
    true
}

fn is_safe_slice_index_stmt(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Assign { target, value } => is_slice_index(target) && !contains_unsafe(value),
        Stmt::CompoundAssign { target, value, .. } => {
            is_slice_index(target) && !contains_unsafe(value)
        }
        _ => false,
    }
}

fn is_slice_index(expr: &Expr) -> bool {
    matches!(expr, Expr::Index { .. })
}

fn contains_unsafe(expr: &Expr) -> bool {
    let mut found = false;
    crate::fixups::support::walk::expr_any(expr, &mut |expr| {
        if matches!(
            expr,
            Expr::Unsafe(_)
                | Expr::Unary {
                    op: UnaryOp::Deref,
                    ..
                }
        ) {
            found = true;
        }
        found
    });
    found
}

fn prune_dead_fact_temps(body: &mut Vec<IndentStmt>, plans: &[Plan]) -> bool {
    let removable = removable_temp_names(plans);
    prune_dead_fact_temps_in_body(body, &removable)
}

fn prune_dead_fact_temps_in_body(body: &mut Vec<IndentStmt>, removable: &BTreeSet<String>) -> bool {
    let mut changed = false;
    for indent in body.iter_mut() {
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut Vec::new(), &mut |body, _| {
            changed |= prune_dead_fact_temps_in_body(body, removable);
        });
    }
    let mut index = 0;
    while index < body.len() {
        let remove = match &body[index].stmt {
            Stmt::Let {
                name,
                init: Some(_),
                ..
            } if removable.contains(name) => body[index + 1..]
                .iter()
                .all(|indent| stmt_ident_count(&indent.stmt, name) == 0),
            _ => false,
        };
        if remove {
            body.remove(index);
            changed = true;
        } else {
            index += 1;
        }
    }
    changed
}

fn removable_temp_names(plans: &[Plan]) -> BTreeSet<String> {
    plans
        .iter()
        .flat_map(|plan| [&plan.pointer_name, &plan.offset_name])
        .filter(|name| is_temp_name(name))
        .cloned()
        .collect()
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}
