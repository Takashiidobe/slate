use crate::backend::facts::PathSegment;
use crate::backend::rust_ast::{Expr, Stmt};
use crate::backend::trace::Pass;

use super::super::item::StatementRef;
use super::super::{
    Binding, BindingCategory, BindingRef, BindingUse, EditSet, Field, ItemCaseContext, QueryRule,
    Rejection, StatementContainerRef,
};

pub(in crate::backend) fn rewrite() -> QueryRule<Binding> {
    QueryRule::new(
        Pass::VarAliases,
        "inline_var_alias",
        Binding {
            kind: Field::eq(BindingCategory::Local),
            name: Field::predicate(|name: &String, _| is_temp_name(name)),
            ..Default::default()
        },
    )
    .case("inline", inline_case)
    .ordered_non_overlapping()
}

fn inline_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
) -> Result<EditSet, Rejection> {
    let decl_ref = StatementRef {
        item_index: binding.item_index,
        path: binding.definition.clone(),
    };
    let decl_stmt = case.fact(|query| query.statement(&decl_ref))?;
    let Stmt::Let {
        mutable: false,
        init: Some(Expr::Var(source)),
        ..
    } = &decl_stmt.stmt
    else {
        return Err(case.reject());
    };
    let source = source.as_str().to_string();
    case.require(!is_temp_name(&source))?;

    let container = decl_ref.container().ok_or_else(|| case.reject())?;

    let uses = case.fact(|query| query.binding_uses(binding))?;
    case.require(!uses.uses.is_empty())?;
    let mut use_stmt: Option<StatementRef> = None;
    for usage in &uses.uses {
        let sibling = use_sibling(&container, usage).ok_or_else(|| case.reject())?;
        match &use_stmt {
            None => use_stmt = Some(sibling),
            Some(existing) => case.require(existing == &sibling)?,
        }
    }
    let use_ref = use_stmt.ok_or_else(|| case.reject())?;

    let between = case.fact(|query| query.statements_between(&decl_ref, &use_ref))?;
    for statement in &between {
        let indent = case.fact(|query| query.statement(statement))?;
        case.require(
            !super::super::var_aliases::stmt_declares_name(&indent.stmt, &source)
                && !super::super::var_aliases::stmt_changes_name(&indent.stmt, &source),
        )?;
    }

    let use_indent = case.fact(|query| query.statement(&use_ref))?;
    case.require(!super::super::var_aliases::stmt_declares_name(
        &use_indent.stmt,
        &source,
    ))?;

    let mut new_use_stmt = use_indent.clone();
    let replacement = Expr::Var(source.into());
    case.require(
        new_use_stmt
            .stmt
            .substitute_var(&binding.name, &replacement),
    )?;

    let mut edits = EditSet::new();
    edits.push_replace_statement(
        use_ref.item_index,
        use_ref.path.clone(),
        Some(new_use_stmt.stmt),
    );
    edits.push_replace_statement(binding.item_index, binding.definition.clone(), None);
    Ok(edits)
}

fn use_sibling(container: &StatementContainerRef, use_: &BindingUse) -> Option<StatementRef> {
    let path = use_.site.path();
    let rest = path.0.strip_prefix(container.path.0.as_slice())?;
    let PathSegment::Stmt(index) = rest.first()? else {
        return None;
    };
    let mut sibling_path = container.path.0.clone();
    sibling_path.push(PathSegment::Stmt(*index));
    Some(StatementRef {
        item_index: container.item_index,
        path: crate::backend::facts::AstPath(sibling_path),
    })
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|byte| byte.is_ascii_digit()))
}
