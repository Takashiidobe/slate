use crate::backend::facts::{AstPath, PathSegment, PlaceAccess, PlaceKind, Purity};
use crate::backend::idents::expr_ident;
use crate::backend::rust_ast::{Expr, IndentStmt, Prim, Stmt, Type};
use crate::backend::support::walk;
use crate::backend::trace::Pass;

use super::super::item::StatementRef;
use super::super::{
    BindingAccess, BindingUse, EditSet, Function, FunctionRef, ItemCaseContext, QueryRule,
    Rejection, UseSiteRef,
};

pub(in crate::backend) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(Pass::Retval, "collapse_retval", Function::default())
        .case("collapse", rewrite_case)
}

fn rewrite_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef<'db>,
) -> Result<EditSet, Rejection> {
    let mut replacement = case
        .fact(|query| query.function_snapshot(function))?
        .clone();

    let mut changed = collapse_return_slots(case, function, &mut replacement.body, &mut Vec::new());
    if function.name == "main" {
        changed |= collapse_main_exit_slots(case, function, &mut replacement.body, &mut Vec::new());
    }
    changed |= super::super::retval::remove_unused_retval_artifacts(&mut replacement.body);

    if !changed {
        return Err(case.reject());
    }
    Ok(EditSet::replace_function(function.clone(), replacement))
}

fn collapse_return_slots<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef<'db>,
    body: &mut Vec<IndentStmt>,
    path: &mut Vec<PathSegment>,
) -> bool {
    let mut changed = false;
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |nested, path| {
                changed |= collapse_return_slots(case, function, nested, path);
            });
        });
    }

    let mut collapses: Vec<(usize, usize, Expr)> = Vec::new();
    for ret_index in 1..body.len() {
        let Stmt::Return(Some(Expr::Var(name))) = &body[ret_index].stmt else {
            continue;
        };
        let name = name.clone();
        let return_ref = StatementRef {
            item_index: function.item_index,
            path: AstPath(stmt_path(path, ret_index)),
        };
        if !case
            .fact(|query| query.statement_reachable(&return_ref))
            .unwrap_or(false)
        {
            continue;
        }

        let decl_index = ret_index - 1;
        let decl_ref = StatementRef {
            item_index: function.item_index,
            path: AstPath(stmt_path(path, decl_index)),
        };
        let value = match &body[decl_index].stmt {
            Stmt::Let {
                name: decl_name,
                init: Some(_),
                ..
            } if decl_name.as_str() == name.as_str() => {
                decl_return_value(case, &decl_ref, &return_ref, name.as_str())
            }
            Stmt::Assign { target, value } if expr_ident(target) == Some(name.as_str()) => {
                assigns_local(case, &decl_ref, name.as_str()).then(|| value.clone())
            }
            _ => None,
        };
        if let Some(value) = value {
            collapses.push((ret_index, decl_index, value));
        }
    }

    collapses.sort_by_key(|(ret_index, ..)| *ret_index);
    for (ret_index, decl_index, value) in collapses.into_iter().rev() {
        body[ret_index].stmt = Stmt::Return(Some(value));
        body.remove(decl_index);
        changed = true;
    }
    changed
}

fn decl_return_value(
    case: &mut ItemCaseContext<'_, '_>,
    decl_ref: &StatementRef,
    return_ref: &StatementRef,
    name: &str,
) -> Option<Expr> {
    let binding = case.fact(|query| query.statement_binding(decl_ref)).ok()?;
    if binding.name != name {
        return None;
    }
    let uses = case.fact(|query| query.binding_uses(&binding)).ok()?;
    let [usage] = uses.uses.as_slice() else {
        return None;
    };
    if usage.access != BindingAccess::Read {
        return None;
    }
    let return_expr = case
        .fact(|query| query.statement_expression(return_ref, 0))
        .ok()?;
    if !usage
        .expression()
        .is_some_and(|expression| expression.site == return_expr.site)
    {
        return None;
    }
    let init = case
        .fact(|query| query.statement_expression(decl_ref, 0))
        .ok()?;
    case.expr(&init.site).cloned()
}

fn assigns_local(case: &mut ItemCaseContext<'_, '_>, statement: &StatementRef, name: &str) -> bool {
    let Ok(target) = case.fact(|query| query.statement_expression(statement, 0)) else {
        return false;
    };
    let Ok(place) = case.fact(|query| query.expression_place(&target)) else {
        return false;
    };
    place.access == PlaceAccess::Write
        && place.ordinary_slot
        && matches!(&place.kind, PlaceKind::Local { name: local } if local == name)
}

fn use_within(
    case: &mut ItemCaseContext<'_, '_>,
    use_: &BindingUse,
    statement: &StatementRef,
) -> bool {
    match &use_.site {
        UseSiteRef::Statement(candidate) => candidate == statement,
        UseSiteRef::Expression(expression) => case
            .fact(|query| query.enclosing_statement(expression))
            .is_ok_and(|enclosing| &enclosing == statement),
    }
}

fn collapse_main_exit_slots<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef<'db>,
    body: &mut Vec<IndentStmt>,
    path: &mut Vec<PathSegment>,
) -> bool {
    let mut changed = false;
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |nested, path| {
                changed |= collapse_main_exit_slots(case, function, nested, path);
            });
        });
    }
    changed |= collapse_main_exit_slot(case, function, body, path);
    changed
}

fn collapse_main_exit_slot<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef<'db>,
    body: &mut Vec<IndentStmt>,
    path: &[PathSegment],
) -> bool {
    let Some((exit_index, temp_name)) = body.iter().enumerate().find_map(|(index, stmt)| {
        let Stmt::Expr(expr) = &stmt.stmt else {
            return None;
        };
        let exit_ref = StatementRef {
            item_index: function.item_index,
            path: AstPath(stmt_path(path, index)),
        };
        if !case
            .fact(|query| query.statement_reachable(&exit_ref))
            .unwrap_or(false)
        {
            return None;
        }
        main_exit_arg_temp(expr).map(|name| (index, name.to_string()))
    }) else {
        return false;
    };
    if exit_index < 2 {
        return false;
    }
    let temp_index = exit_index - 1;
    let store_index = temp_index - 1;

    let Some(retval_name) = (match &body[temp_index].stmt {
        Stmt::Let {
            name,
            mutable: false,
            init: Some(init),
            ..
        } if name == &temp_name => expr_ident(init).map(str::to_string),
        _ => None,
    }) else {
        return false;
    };

    let exit_ref = StatementRef {
        item_index: function.item_index,
        path: AstPath(stmt_path(path, exit_index)),
    };
    let temp_ref = StatementRef {
        item_index: function.item_index,
        path: AstPath(stmt_path(path, temp_index)),
    };
    let store_ref = StatementRef {
        item_index: function.item_index,
        path: AstPath(stmt_path(path, store_index)),
    };

    let value = match &body[store_index].stmt {
        Stmt::Assign { target, value } if expr_ident(target) == Some(retval_name.as_str()) => {
            if !assigns_local(case, &store_ref, &retval_name) {
                return false;
            }
            value.clone()
        }
        _ => return false,
    };

    let Ok(temp_binding) = case.fact(|query| query.statement_binding(&temp_ref)) else {
        return false;
    };
    let Ok(temp_uses) = case.fact(|query| query.binding_uses(&temp_binding)) else {
        return false;
    };
    let [temp_usage] = temp_uses.uses.as_slice() else {
        return false;
    };
    if temp_usage.access != BindingAccess::Read || !use_within(case, temp_usage, &exit_ref) {
        return false;
    }

    let Ok(bindings) = case.fact(|query| query.function_bindings(function)) else {
        return false;
    };
    let Some(retval_binding) = bindings
        .iter()
        .find(|binding| binding.name == retval_name)
        .cloned()
    else {
        return false;
    };
    let Ok(retval_uses) = case.fact(|query| query.binding_uses(&retval_binding)) else {
        return false;
    };
    let reads: Vec<&BindingUse> = retval_uses
        .uses
        .iter()
        .filter(|usage| usage.access == BindingAccess::Read)
        .collect();
    let [read] = reads.as_slice() else {
        return false;
    };
    if !use_within(case, read, &temp_ref) {
        return false;
    }

    let Some(container) = store_ref.container() else {
        return false;
    };
    let Ok(siblings) = case.fact(|query| query.enclosing_statements(&store_ref)) else {
        return false;
    };
    let mut remove_extra = Vec::new();
    for (index, indent) in siblings.iter().enumerate() {
        if index == store_index {
            continue;
        }
        let Stmt::Assign { target, .. } = &indent.stmt else {
            continue;
        };
        if expr_ident(target) != Some(retval_name.as_str()) {
            continue;
        }
        let mut sibling_path = container.path.0.clone();
        sibling_path.push(PathSegment::Stmt(index));
        let sibling_ref = StatementRef {
            item_index: function.item_index,
            path: AstPath(sibling_path),
        };
        if !assigns_local(case, &sibling_ref, &retval_name) {
            continue;
        }
        if index >= store_index {
            return false;
        }
        let Ok(value_site) = case.fact(|query| query.statement_expression(&sibling_ref, 1)) else {
            return false;
        };
        let Ok(effects) = case.fact(|query| query.expression_effects(&value_site)) else {
            return false;
        };
        if effects.purity != Purity::MovablePure {
            return false;
        }
        remove_extra.push(index);
    }

    let Stmt::Expr(expr) = &mut body[exit_index].stmt else {
        unreachable!()
    };
    replace_main_exit_arg(expr, value);

    let mut remove = vec![temp_index, store_index];
    remove.extend(remove_extra);
    remove.sort_unstable();
    remove.dedup();
    for index in remove.into_iter().rev() {
        body.remove(index);
    }
    true
}

fn main_exit_arg_temp(expr: &Expr) -> Option<&str> {
    let Expr::Call { func, args, .. } = expr else {
        return None;
    };
    if !is_std_process_exit(func) || args.len() != 1 {
        return None;
    }
    match &args[0] {
        Expr::Cast {
            expr,
            ty: Type::Prim(Prim::I32),
        } => expr_ident(expr),
        arg => expr_ident(arg),
    }
}

fn replace_main_exit_arg(expr: &mut Expr, replacement: Expr) {
    let Expr::Call { args, .. } = expr else {
        return;
    };
    args[0] = replacement;
}

fn is_std_process_exit(expr: &Expr) -> bool {
    let Expr::Path(crate::backend::rust_ast::Path { segments }) = expr else {
        return false;
    };
    let expected = ["std", "process", "exit"];
    segments.len() == expected.len()
        && segments
            .iter()
            .zip(expected)
            .all(|(segment, expected)| segment.as_str() == expected)
}

fn stmt_path(parent_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = parent_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}
