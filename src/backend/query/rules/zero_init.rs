use crate::backend::facts::{ConstValue, PlaceAccess, PlaceKind};
use crate::backend::rust_ast::Stmt;
use crate::backend::trace::Pass;

use super::super::item::StatementRef;
use super::super::{
    Binding, BindingAccess, BindingCategory, BindingRef, EditSet, Field, ItemCaseContext,
    Predicate, QueryRule, Rejection, initialize_local,
};

fn matcher() -> Binding {
    Binding {
        kind: Field::eq(BindingCategory::Local),
        ..Default::default()
    }
}

pub(in crate::backend) fn direct() -> QueryRule<Binding> {
    QueryRule::new(Pass::ZeroInit, "fold_zero_init_assignment", matcher())
        .case("direct", direct_case)
        .ordered_non_overlapping()
}

pub(in crate::backend) fn deferred() -> QueryRule<Binding> {
    QueryRule::new(Pass::ZeroInit, "fold_zero_init_assignment", matcher())
        .case("deferred", deferred_case)
        .ordered_non_overlapping()
}

fn direct_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
) -> Result<EditSet, Rejection> {
    apply(case, binding, false)
}

fn deferred_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
) -> Result<EditSet, Rejection> {
    apply(case, binding, true)
}

fn apply<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
    cross_effects: bool,
) -> Result<EditSet, Rejection> {
    let declaration_ref = StatementRef {
        item_index: binding.item_index,
        path: binding.definition.clone(),
    };
    let declaration = case
        .fact(|query| query.statement(&declaration_ref))?
        .stmt
        .clone();
    case.require(matches!(
        declaration,
        Stmt::Let {
            mutable: true,
            ty: Some(_),
            init: Some(_),
            ..
        }
    ))?;
    let initializer = case.fact(|query| query.binding_initializer(binding))?;
    let values = case.fact(|query| query.expression_values(&initializer))?;
    case.require_at(
        values.values.contains(&ConstValue::Zero),
        Predicate::ExpressionValues,
        &initializer.site,
    )?;
    let _ = case.fact(|query| query.binding_type(binding))?;
    let uses = case.fact(|query| query.binding_uses(binding))?;
    let declaration_index = declaration_ref.index().ok_or_else(|| case.reject())?;
    let declaration_container = declaration_ref.container().ok_or_else(|| case.reject())?;

    let mut writes = Vec::new();
    for usage in uses.uses.iter().filter(|usage| {
        matches!(
            usage.access,
            BindingAccess::Write | BindingAccess::ReadWrite
        )
    }) {
        let Some(expression) = usage.expression() else {
            continue;
        };
        let statement =
            case.fact(|query| query.statement_in_container(&declaration_container, expression))?;
        let index = statement.index().ok_or_else(|| case.reject())?;
        if index > declaration_index {
            writes.push((usage, statement, index));
        }
    }
    writes.sort_by_key(|(_, _, index)| *index);
    let (write, assignment_ref, _) = writes.into_iter().next().ok_or_else(|| case.reject())?;

    let assignment = case
        .fact(|query| query.statement(&assignment_ref))?
        .stmt
        .clone();
    let Stmt::Assign { .. } = assignment else {
        return Err(case.reject());
    };
    let target = case.fact(|query| query.statement_expression(&assignment_ref, 0))?;
    case.require(
        write
            .expression()
            .is_some_and(|expression| expression.site == target.site),
    )?;
    let place = case.fact(|query| query.expression_place(&target))?;
    case.require_at(
        place.access == PlaceAccess::Write
            && place.ordinary_slot
            && matches!(&place.kind, PlaceKind::Local { name } if name == &binding.name),
        Predicate::ExpressionPlace,
        &target.site,
    )?;
    let value = case.fact(|query| query.statement_expression(&assignment_ref, 1))?;
    let value_expr = case
        .expr(&value.site)
        .cloned()
        .ok_or_else(|| case.reject())?;

    let assignment_uses =
        case.fact(|query| query.binding_uses_in_statement(binding, &assignment_ref))?;
    let reads_assignment = assignment_uses
        .uses
        .iter()
        .any(|usage| matches!(usage.access, BindingAccess::Read | BindingAccess::ReadWrite));
    case.require(!reads_assignment)?;

    let intervening =
        case.fact(|query| query.statements_between(&declaration_ref, &assignment_ref))?;
    let value_dependencies = case.fact(|query| query.expression_dependencies(&value))?;
    let value_effects = case.fact(|query| query.expression_effects(&value))?;
    let value_reads_nothing = value_dependencies.is_empty() && value_effects.effects.is_empty();
    let mut direct = true;
    let mut moved = true;
    for statement in intervening {
        let indent = case.fact(|query| query.statement(&statement))?;
        let is_declaration = matches!(indent.stmt, Stmt::Let { .. });
        let statement_uses =
            case.fact(|query| query.binding_uses_in_statement(binding, &statement))?;
        let touched = !statement_uses.uses.is_empty();
        let movable =
            !touched && case.fact(|query| query.statement_is_movable_declaration(&statement))?;
        let effect_free = !touched && value_reads_nothing;
        if !(movable || (cross_effects && effect_free)) {
            direct = false;
        }
        if touched {
            moved = false;
        }
        if is_declaration {
            let assignment_dependencies = case
                .fact(|query| query.declaration_uses_in_statement(&statement, &assignment_ref))?;
            if !assignment_dependencies.uses.is_empty() {
                direct = false;
            }
        }
    }

    let moved_decl = if direct {
        false
    } else {
        case.require(cross_effects && moved)?;
        true
    };
    let replacement = initialize_local(&declaration, value_expr).ok_or_else(|| case.reject())?;
    let mut edits = EditSet::new();
    if moved_decl {
        edits.push_replace_statement(binding.item_index, assignment_ref.path, Some(replacement));
        edits.push_replace_statement(binding.item_index, binding.definition.clone(), None);
    } else {
        edits.push_replace_statement(
            binding.item_index,
            binding.definition.clone(),
            Some(replacement),
        );
        edits.push_replace_statement(binding.item_index, assignment_ref.path, None);
    }
    Ok(edits)
}
