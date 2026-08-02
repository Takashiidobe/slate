use crate::fixups::trace::Pass;

use super::super::{EditSet, Parameter, QueryRule};

pub(in crate::fixups) fn rewrite() -> QueryRule<Parameter> {
    QueryRule::new(
        Pass::UnusedParams,
        "remove_unused_param",
        Parameter::default(),
    )
    .case("unreachable_param", |case, parameter| {
        let removal = case.fact(|query| {
            super::super::parameter::removable_parameter(query, &parameter.binding)
        })?;
        Ok(EditSet::remove_parameter(removal))
    })
}
