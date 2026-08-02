use crate::fixups::facts::{AstPath, PathSegment, PlaceAccess, PlaceKind};
use crate::fixups::idents::expr_ident;
use crate::fixups::trace::Pass;
use crate::rust_ast::Stmt;

use super::super::item::StatementRef;
use super::super::{
    BindingAccess, BindingCategory, BindingRef, EditSet, Function, FunctionRef, ItemCaseContext,
    QueryRule, Rejection,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(Pass::ParamSpills, "fold_param_spills", Function::default())
        .case("spillable_params", rewrite_case)
}

fn rewrite_case(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef,
) -> Result<EditSet, Rejection> {
    let mut replacement = case
        .fact(|query| query.function_snapshot(function))?
        .clone();
    let bindings = case.fact(|query| query.function_bindings(function))?;

    let mut claimed: Vec<String> = Vec::new();
    let mut removed: Vec<usize> = Vec::new();
    for param_index in 0..replacement.params.len() {
        if replacement.params[param_index].mutable {
            continue;
        }
        let Some(spill) = spill_candidate(case, function, &bindings, param_index, &claimed) else {
            continue;
        };
        replacement.params[param_index].name = spill.local_name.clone();
        replacement.params[param_index].mutable = true;
        removed.push(spill.decl_index);
        removed.push(spill.store_index);
        claimed.push(spill.local_name);
    }

    if removed.is_empty() {
        return Err(case.reject());
    }
    removed.sort_unstable();
    removed.dedup();
    for index in removed.into_iter().rev() {
        replacement.body.remove(index);
    }
    Ok(EditSet::replace_function(function.clone(), replacement))
}

struct Spill {
    local_name: String,
    decl_index: usize,
    store_index: usize,
}

fn spill_candidate(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef,
    bindings: &[BindingRef],
    param_index: usize,
    claimed: &[String],
) -> Option<Spill> {
    let param_binding = bindings.iter().find(|binding| {
        matches!(binding.kind, BindingCategory::Parameter { index } if index == param_index)
    })?;
    let uses = case.fact(|query| query.binding_uses(param_binding)).ok()?;
    let [use_] = uses.uses.as_slice() else {
        return None;
    };
    if use_.access != BindingAccess::Read {
        return None;
    }
    let expr = use_.expression()?;
    let store = case.fact(|query| query.enclosing_statement(expr)).ok()?;
    if store.path.0.len() != 1 {
        return None;
    }
    let store_index = store.index()?;
    let store_stmt = case.fact(|query| query.statement(&store)).ok()?;
    let Stmt::Assign { target, value } = &store_stmt.stmt else {
        return None;
    };
    let local = expr_ident(target)?;
    if expr_ident(value) != Some(param_binding.name.as_str()) {
        return None;
    }
    if bindings
        .iter()
        .any(|other| matches!(other.kind, BindingCategory::Parameter { .. }) && other.name == local)
        || claimed.iter().any(|name| name == local)
    {
        return None;
    }

    let target_expr = case
        .fact(|query| query.statement_expression(&store, 0))
        .ok()?;
    let place = case
        .fact(|query| query.expression_place(&target_expr))
        .ok()?;
    if !(place.access == PlaceAccess::Write
        && place.ordinary_slot
        && matches!(&place.kind, PlaceKind::Local { name } if name == local))
    {
        return None;
    }

    let (decl_index, local_binding) = bindings
        .iter()
        .filter(|binding| matches!(binding.kind, BindingCategory::Local) && binding.name == local)
        .filter_map(|binding| match binding.definition.0.as_slice() {
            [PathSegment::Stmt(index)] if *index < store_index => Some((*index, binding)),
            _ => None,
        })
        .min_by_key(|(index, _)| *index)?;
    if local_binding.ty != param_binding.ty {
        return None;
    }
    let decl = StatementRef {
        item_index: function.item_index,
        path: AstPath(vec![PathSegment::Stmt(decl_index)]),
    };
    let decl_stmt = case.fact(|query| query.statement(&decl)).ok()?;
    let Stmt::Let { mutable: true, .. } = &decl_stmt.stmt else {
        return None;
    };

    let local_uses = case.fact(|query| query.binding_uses(local_binding)).ok()?;
    let read_between = local_uses.uses.iter().any(|usage| {
        matches!(usage.access, BindingAccess::Read | BindingAccess::ReadWrite)
            && matches!(
                usage.site.path().0.first(),
                Some(PathSegment::Stmt(index)) if *index > decl_index && *index < store_index
            )
    });
    if read_between {
        return None;
    }

    Some(Spill {
        local_name: local.to_string(),
        decl_index,
        store_index,
    })
}
