use crate::backend::trace::Pass;

use super::super::item::StatementMatch;
use super::super::{
    BindingAccess, BindingRef, BindingUses, EditSet, Field, ItemCaseContext, Local, QueryRule,
    Rejection, StatementRef, StatementSequence, collapse_atomic_compare_exchange,
};

pub(in crate::backend) fn rewrite() -> QueryRule<StatementSequence<6>> {
    QueryRule::new(
        Pass::AtomicCompareExchange,
        "collapse_compare_exchange_chain",
        StatementSequence::new().starting_with(Local {
            mutable: Field::eq(false),
            ..Default::default()
        }),
    )
    .case("dense_match", rewrite_chain)
    .ordered_non_overlapping()
}

fn rewrite_chain(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<6>,
) -> Result<EditSet, Rejection> {
    let statements: [StatementRef; 6] = std::array::from_fn(|offset| matched.statement(offset));
    let shape = case.fact(|query| query.atomic_compare_exchange_shape(&statements))?;

    for (definition, allowed) in [(0, [1, 2].as_slice()), (1, &[4]), (2, &[3, 5]), (3, &[4])] {
        only_read_within(case, &statements[definition], allowed, &statements)?;
    }

    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![collapse_atomic_compare_exchange(shape)],
    ))
}

fn only_read_within<'db>(
    case: &mut ItemCaseContext<'_, 'db>,
    definition: &StatementRef,
    allowed_offsets: &[usize],
    statements: &[StatementRef],
) -> Result<(), Rejection> {
    let binding: BindingRef<'db> = case.fact(|query| query.statement_binding(definition))?;
    let total: BindingUses = case.fact(|query| query.binding_uses(&binding))?;
    let mut allowed = 0usize;
    for offset in allowed_offsets {
        let uses =
            case.fact(|query| query.binding_uses_in_statement(&binding, &statements[*offset]))?;
        case.require(
            uses.uses
                .iter()
                .all(|usage| usage.access == BindingAccess::Read),
        )?;
        allowed += uses.uses.len();
    }
    case.require(allowed == total.uses.len())
}
