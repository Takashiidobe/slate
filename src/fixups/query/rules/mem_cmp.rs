use crate::fixups::facts::{AstPath, PathSegment};
use crate::fixups::trace::Pass;
use crate::function_identity::Known;
use crate::rust_ast::{BinOp, Expr, RustValue, Stmt};

use super::super::{
    CallRecord, CallTarget, EditSet, ExprSite, ExpressionRef, FnCall, ItemCaseContext, QueryRule,
    Rejection, StatementRef, ValueSite,
};

pub(in crate::fixups) fn calls() -> QueryRule<FnCall> {
    QueryRule::new(Pass::MemCmp, "rewrite_mem_cmp", FnCall::default()).case("mem_cmp", call_case)
}

fn edit_target(call: &CallRecord) -> ExprSite {
    call.trivial_unsafe_site
        .clone()
        .unwrap_or_else(|| call.site.clone())
}

fn is_direct_statement_root(statement: &AstPath, site: &AstPath) -> bool {
    site.0.len() == statement.0.len() + 1 && site.0.last() == Some(&PathSegment::Expr(0))
}

fn ancestor_statement_refs(statement: &StatementRef) -> Vec<StatementRef> {
    let mut refs = Vec::new();
    let mut path = statement.path.0.clone();
    loop {
        refs.push(StatementRef {
            item_index: statement.item_index,
            path: AstPath(path.clone()),
        });
        path.pop();
        let mut found = false;
        while let Some(segment) = path.last() {
            if matches!(segment, PathSegment::Stmt(_)) {
                found = true;
                break;
            }
            path.pop();
        }
        if !found {
            break;
        }
    }
    refs
}

fn call_case(case: &mut ItemCaseContext<'_, '_>, call: &CallRecord) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::MemCmp)))?;
    case.require(call.args.len() == 3)?;
    let a = case
        .expr(&call.args[0])
        .cloned()
        .ok_or_else(|| case.reject())?;
    let b = case
        .expr(&call.args[1])
        .cloned()
        .ok_or_else(|| case.reject())?;
    let count = case
        .expr(&call.args[2])
        .cloned()
        .ok_or_else(|| case.reject())?;

    let statement = case.fact(|query| {
        query.enclosing_statement(&ExpressionRef {
            site: edit_target(call),
        })
    })?;
    let mut env = super::super::array_env::CopyEnv::new();
    for ancestor in ancestor_statement_refs(&statement) {
        let body = case.fact(|query| query.enclosing_statements(&ancestor))?;
        env.extend(super::super::array_env::CopyEnv::from_body(body));
    }
    let (lhs, rhs) = super::super::mem_cmp::compare_operands(&a, &b, &count, &env)
        .ok_or_else(|| case.reject())?;

    if let Some(edits) = direct_zero_comparison_edit(case, call, &lhs, &rhs) {
        return Ok(edits);
    }
    if let Some(edits) = temp_zero_comparison_edit(case, call, &lhs, &rhs) {
        return Ok(edits);
    }
    Err(case.reject())
}

fn direct_zero_comparison_edit(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
    lhs: &Expr,
    rhs: &Expr,
) -> Option<EditSet> {
    let parent = case
        .fact(|query| {
            query.parent_expression(&ExpressionRef {
                site: edit_target(call),
            })
        })
        .ok()?;
    let op = case.expr(&parent.site).and_then(zero_comparison_op)?;
    Some(EditSet::replace_expression(
        parent.site.clone(),
        compare_to_bool(lhs, rhs, op),
    ))
}

fn temp_zero_comparison_edit(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
    lhs: &Expr,
    rhs: &Expr,
) -> Option<EditSet> {
    let target = edit_target(call);
    let statement_ref = case
        .fact(|query| {
            query.enclosing_statement(&ExpressionRef {
                site: target.clone(),
            })
        })
        .ok()?;
    if !is_direct_statement_root(&statement_ref.path, &target.path) {
        return None;
    }
    let stmt = case.fact(|query| query.statement(&statement_ref)).ok()?;
    let Stmt::Let {
        name,
        init: Some(_),
        ..
    } = &stmt.stmt
    else {
        return None;
    };
    let name = name.clone();
    let value_site = ValueSite {
        item_index: statement_ref.item_index,
        path: statement_ref.path.clone(),
    };
    let uses = case
        .fact(|query| query.value_uses(&value_site, &name))
        .ok()?;
    if uses.is_empty() {
        return None;
    }
    let all_exprs = case
        .fact(|query| query.all_exprs(value_site.item_index))
        .ok()?;
    let mut replacements = Vec::new();
    for use_stmt in &uses {
        let var_sites = all_exprs
            .iter()
            .filter(|site| site.path.0.starts_with(&use_stmt.path.0))
            .filter(|site| matches!(case.expr(site), Some(Expr::Var(v)) if v.as_str() == name))
            .cloned()
            .collect::<Vec<_>>();
        if var_sites.is_empty() {
            return None;
        }
        for var_site in var_sites {
            let parent = case
                .fact(|query| query.parent_expression(&ExpressionRef { site: var_site }))
                .ok()?;
            let op = case
                .expr(&parent.site)
                .and_then(|expr| zero_comparison_op_for_var(expr, &name))?;
            replacements.push((parent.site.clone(), op));
        }
    }
    let mut edits = EditSet::new();
    edits.push_replace_statement(statement_ref.item_index, statement_ref.path.clone(), None);
    for (site, op) in replacements {
        edits.push_replace_expression(site, compare_to_bool(lhs, rhs, op));
    }
    Some(edits)
}

fn zero_comparison_op(expr: &Expr) -> Option<BinOp> {
    let Expr::Binary { op, lhs, rhs } = expr else {
        return None;
    };
    if !matches!(op, BinOp::Eq | BinOp::Ne) {
        return None;
    }
    if is_zero(rhs) || is_zero(lhs) {
        return Some(*op);
    }
    None
}

fn zero_comparison_op_for_var(expr: &Expr, name: &str) -> Option<BinOp> {
    let Expr::Binary { op, lhs, rhs } = expr else {
        return None;
    };
    if !matches!(op, BinOp::Eq | BinOp::Ne) {
        return None;
    }
    match (&**lhs, &**rhs) {
        (Expr::Var(v), rhs) if v.as_str() == name && is_zero(rhs) => Some(*op),
        (lhs, Expr::Var(v)) if v.as_str() == name && is_zero(lhs) => Some(*op),
        _ => None,
    }
}

fn is_zero(expr: &Expr) -> bool {
    match expr {
        Expr::Value(RustValue::I64(0) | RustValue::I128(0)) => true,
        Expr::Cast { expr, .. } => is_zero(expr),
        _ => false,
    }
}

fn compare_to_bool(lhs: &Expr, rhs: &Expr, op: BinOp) -> Expr {
    Expr::Binary {
        op,
        lhs: Box::new(lhs.clone()),
        rhs: Box::new(rhs.clone()),
    }
}
