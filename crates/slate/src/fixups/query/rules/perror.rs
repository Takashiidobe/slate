use std::collections::BTreeSet;

use crate::fixups::facts::{AstPath, EffectKind, PathSegment};
use crate::fixups::trace::Pass;
use crate::function_identity::{CallBinding, Known};
use crate::rust_ast::{Expr, Stmt};

use super::super::{
    BindingAccess, BindingRef, CallRecord, CallTarget, EditSet, ExpressionRef, Field, FnCall,
    ItemCaseContext, QueryRule, Rejection, StatementRef,
};

pub(in crate::fixups) fn calls() -> QueryRule<FnCall> {
    QueryRule::new(
        Pass::Perror,
        "rewrite_perror_last_os_error",
        FnCall {
            target: Field::eq(CallTarget::Known(Known::Perror)),
            arity: Field::eq(1),
            ..Default::default()
        },
    )
    .case("closure_error_handler", closure_error_handler)
    .case("adjacent_call_guard", adjacent_call_guard)
}

fn closure_error_handler(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    let (effective_site, message) = perror_message(case, call)?;
    // `enclosing_statement`/`statement` only navigate through statement-shaped
    // containers (If/Loop/Scope/...); a closure body is reached through
    // expression segments, so find the statement index by hand instead.
    let is_first_statement = effective_site
        .path
        .0
        .iter()
        .rposition(|segment| matches!(segment, PathSegment::Stmt(_)))
        .is_some_and(|index| effective_site.path.0[index] == PathSegment::Stmt(0));
    case.require(is_first_statement)?;

    // Climb: the `unsafe { perror(..) }` root expr sits at the top of a
    // closure body block; the closure must be the error handler passed to
    // `.unwrap_or_else(..)`, which runs synchronously and only on failure, so
    // nothing can have touched errno since the fallible call returned.
    let mut current = ExpressionRef {
        site: effective_site.clone(),
    };
    let mut saw_closure = false;
    loop {
        let parent = case.fact(|query| query.parent_expression(&current))?;
        match case.expr(&parent.site) {
            Some(Expr::Block(_)) if !saw_closure => current = parent,
            Some(Expr::Closure { .. }) if !saw_closure => {
                saw_closure = true;
                current = parent;
            }
            Some(Expr::MethodCall { method, .. }) if saw_closure => {
                case.require(method == "unwrap_or_else")?;
                break;
            }
            _ => return Err(case.reject()),
        }
    }

    Ok(EditSet::replace_expression(
        effective_site,
        eprintln_last_os_error(message),
    ))
}

fn adjacent_call_guard(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    let (effective_site, message) = perror_message(case, call)?;
    let perror_stmt = case.fact(|query| {
        query.enclosing_statement(&ExpressionRef {
            site: effective_site.clone(),
        })
    })?;
    case.require(perror_stmt.index() == Some(0))?;

    let branch_container = perror_stmt.container().ok_or_else(|| case.reject())?;
    case.require(matches!(
        branch_container.path.0.last(),
        Some(PathSegment::Then | PathSegment::Else)
    ))?;
    let mut if_stmt_path = branch_container.path.0.clone();
    if_stmt_path.pop();
    let if_stmt = StatementRef {
        item_index: perror_stmt.item_index,
        path: AstPath(if_stmt_path),
    };
    case.fact(|query| query.statement(&if_stmt))?;

    let cond = case.fact(|query| query.statement_expression(&if_stmt, 0))?;
    let cond_effects = case.fact(|query| query.expression_effects(&cond))?;
    case.require(!has_call_effect(&cond_effects.effects))?;

    find_guarding_call(case, &cond, &if_stmt)?;

    Ok(EditSet::replace_expression(
        effective_site,
        eprintln_last_os_error(message),
    ))
}

/// Walks a chain of single-dependency pure aliases (e.g. a boolean condition
/// bound to a temp, itself bound from a comparison against a call result)
/// back to the call that can have set errno, proving at each hop that nothing
/// between that hop's declaration and `boundary` could have touched errno.
fn find_guarding_call<'db>(
    case: &mut ItemCaseContext<'_, 'db>,
    expr: &ExpressionRef,
    boundary: &StatementRef,
) -> Result<(), Rejection> {
    let dependencies = case.fact(|query| query.expression_dependencies(expr))?;
    let [guard] = dependencies.as_slice() else {
        return Err(case.reject());
    };
    let binding: BindingRef<'db> = case.fact(|query| query.expression_binding(guard))?;
    let (source, decl_stmt) = resolve_binding_source(case, &binding)?;
    let source_effects = case.fact(|query| query.expression_effects(&source))?;
    require_no_call_effects_between(case, &decl_stmt, boundary)?;
    if has_call_effect(&source_effects.effects) {
        return Ok(());
    }
    find_guarding_call(case, &source, &decl_stmt)
}

/// The baseline lowerer always declares a local with a placeholder value and
/// stores the real value in a separate `Stmt::Assign` (e.g. `let mut rc: i32
/// = 0; rc = remove(..);`), so a binding's meaningful source is its sole
/// reassignment when one exists, not its declaration's own initializer.
fn resolve_binding_source<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
) -> Result<(ExpressionRef, StatementRef), Rejection> {
    let uses = case.fact(|query| query.binding_uses(binding))?;
    let writes = uses
        .uses
        .iter()
        .filter(|use_| matches!(use_.access, BindingAccess::Write))
        .collect::<Vec<_>>();
    let [write] = writes.as_slice() else {
        let initializer = case.fact(|query| query.binding_initializer(binding))?;
        let decl_stmt = StatementRef {
            item_index: binding.item_index,
            path: binding.definition.clone(),
        };
        return Ok((initializer, decl_stmt));
    };
    let target = write.expression().cloned().ok_or_else(|| case.reject())?;
    let assign_stmt = case.fact(|query| query.enclosing_statement(&target))?;
    let value_index = match case.fact(|query| query.statement(&assign_stmt))?.stmt {
        Stmt::Let { .. } => 0,
        Stmt::Assign { .. } => 1,
        _ => return Err(case.reject()),
    };
    let value = case.fact(|query| query.statement_expression(&assign_stmt, value_index))?;
    Ok((value, assign_stmt))
}

/// Proves no statement reachable between `from` (exclusive) and `to`
/// (exclusive) can have a call-shaped effect, unwrapping the single
/// `Stmt::Scope` the lowerer always wraps a temp-bound `if` condition in.
fn require_no_call_effects_between(
    case: &mut ItemCaseContext<'_, '_>,
    from: &StatementRef,
    to: &StatementRef,
) -> Result<(), Rejection> {
    let from_container = from.container().ok_or_else(|| case.reject())?;
    let to_container = to.container().ok_or_else(|| case.reject())?;
    let boundary = if to_container == from_container {
        to.clone()
    } else {
        case.require(matches!(
            to_container.path.0.last(),
            Some(PathSegment::ScopeBody)
        ))?;
        if let Some(to_index) = to.index() {
            for index in 0..to_index {
                let mut path = to_container.path.0.clone();
                path.push(PathSegment::Stmt(index));
                let sibling = StatementRef {
                    item_index: to.item_index,
                    path: AstPath(path),
                };
                let effects = case.fact(|query| query.observed_statement_effects(&sibling))?;
                case.require(!effects.is_some_and(|effects| has_call_effect(&effects.effects)))?;
            }
        }
        let mut scope_path = to_container.path.0.clone();
        scope_path.pop();
        let scope_stmt = StatementRef {
            item_index: to.item_index,
            path: AstPath(scope_path),
        };
        case.require(scope_stmt.container() == Some(from_container))?;
        scope_stmt
    };
    let between = case.fact(|query| query.statements_between(from, &boundary))?;
    for statement in &between {
        let effects = case.fact(|query| query.observed_statement_effects(statement))?;
        case.require(!effects.is_some_and(|effects| has_call_effect(&effects.effects)))?;
    }
    Ok(())
}

fn perror_message(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<(super::super::ExprSite, String), Rejection> {
    let effective_site = call
        .trivial_unsafe_site
        .clone()
        .unwrap_or_else(|| call.site.clone());
    let [msg_site] = case.call_args::<1>(call);
    let message = case
        .expr(&msg_site)
        .and_then(literal_message)
        .ok_or_else(|| case.reject())?;
    Ok((effective_site, message))
}

fn literal_message(expr: &Expr) -> Option<String> {
    match expr {
        Expr::CStr(bytes) => String::from_utf8(bytes.clone()).ok(),
        Expr::Cast { expr, .. } => literal_message(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            literal_message(recv)
        }
        _ => None,
    }
}

fn has_call_effect(effects: &BTreeSet<EffectKind>) -> bool {
    effects.iter().any(|effect| {
        matches!(
            effect,
            EffectKind::ReadOnlyCall
                | EffectKind::UnknownCall
                | EffectKind::MethodCall
                | EffectKind::MacroExpansion
        )
    })
}

fn escape_format_braces(message: &str) -> String {
    message.replace('{', "{{").replace('}', "}}")
}

fn eprintln_last_os_error(message: String) -> Expr {
    Expr::Macro {
        name: "eprintln".into(),
        args: vec![
            Expr::Str(format!("{}: {{}}", escape_format_braces(&message))),
            Expr::Call {
                func: Box::new(Expr::Var("std::io::Error::last_os_error".into())),
                args: Vec::new(),
                binding: CallBinding::Generated,
            },
        ],
    }
}
