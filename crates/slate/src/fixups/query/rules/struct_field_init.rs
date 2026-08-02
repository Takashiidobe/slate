use std::collections::BTreeSet;

use crate::fixups::facts::Purity;
use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, Stmt};

use super::super::item::StatementMatch;
use super::super::{
    EditSet, Field, ItemCaseContext, Local, QueryRule, Rejection, StatementSequence,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<StatementSequence<1>> {
    QueryRule::new(
        Pass::StructFieldInit,
        "fold_struct_field_initializers",
        StatementSequence::new().starting_with(Local {
            mutable: Field::eq(true),
            ..Default::default()
        }),
    )
    .case("contiguous_pure_assignments", rewrite_region)
    .ordered_non_overlapping()
}

fn rewrite_region(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<1>,
) -> Result<EditSet, Rejection> {
    let declaration_ref = matched.statement(0);
    let [mut declaration] = case.statements(matched)?;
    let Stmt::Let {
        name,
        init: Some(Expr::StructLit { fields, .. }),
        ..
    } = &declaration.stmt
    else {
        return Err(case.reject());
    };
    let binding_name = name.clone();
    let known_fields = fields
        .iter()
        .map(|(field, _)| field.clone())
        .collect::<BTreeSet<_>>();
    let initializer = case
        .fact(|query| query.statement_initializer(&declaration_ref))?
        .ok_or_else(|| case.reject())?;
    let initializer_effects = case.fact(|query| query.expression_effects(&initializer))?;
    case.require(initializer_effects.purity == Purity::MovablePure)?;
    let binding = case.fact(|query| query.statement_binding(&declaration_ref))?;
    let following = case.fact(|query| query.following_statements(&declaration_ref))?;

    let mut seen = BTreeSet::new();
    let mut assignments = Vec::new();
    let mut last = None;
    for statement_ref in following {
        let statement = case.fact(|query| query.statement(&statement_ref))?.clone();
        let Stmt::Assign {
            target: Expr::Field { base, field },
            value,
        } = statement.stmt
        else {
            break;
        };
        let Expr::Var(base) = &*base else {
            break;
        };
        if base.as_str() != binding_name {
            break;
        }
        case.require(known_fields.contains(&field) && seen.insert(field.clone()))?;
        let value_ref = case.fact(|query| query.statement_expression(&statement_ref, 1))?;
        let effects = case.fact(|query| query.expression_effects(&value_ref))?;
        case.require(effects.purity == Purity::MovablePure)?;
        let uses = case.fact(|query| query.binding_uses_in_expression(&binding, &value_ref))?;
        case.require(uses.uses.is_empty())?;
        assignments.push((field, value));
        last = Some(statement_ref);
    }
    let last = last.ok_or_else(|| case.reject())?;

    let Stmt::Let {
        init: Some(Expr::StructLit { fields, .. }),
        ..
    } = &mut declaration.stmt
    else {
        unreachable!()
    };
    for (assigned_field, assigned_value) in assignments {
        let Some((_, value)) = fields
            .iter_mut()
            .find(|(field, _)| *field == assigned_field)
        else {
            unreachable!()
        };
        *value = assigned_value;
    }
    let region = case.fact(|query| query.statement_range(&declaration_ref, &last))?;
    Ok(EditSet::replace_statements(region, vec![declaration]))
}
