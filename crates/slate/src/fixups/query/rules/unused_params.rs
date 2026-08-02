use crate::fixups::trace::Pass;

use super::super::{Binding, BindingCategory, EditSet, Field, QueryRule};

pub(in crate::fixups) fn rewrite() -> QueryRule<Binding> {
    QueryRule::new(
        Pass::UnusedParams,
        "remove_unused_param",
        Binding {
            kind: Field::predicate(|kind: &BindingCategory, _| {
                matches!(kind, BindingCategory::Parameter { .. })
            }),
            ..Default::default()
        },
    )
    .case("unreachable_param", |case, binding| {
        let removal = case.removable_parameter(binding)?;
        Ok(EditSet::remove_parameter(removal))
    })
}
