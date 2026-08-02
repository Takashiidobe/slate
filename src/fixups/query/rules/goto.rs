use crate::fixups::trace::Pass;

use super::super::{EditSet, Function, QueryRule};

pub(in crate::fixups) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(Pass::Goto, "structure_dispatch_loop", Function::default()).case(
        "structured_region",
        |case, function| {
            let regions = case.fact(|query| query.dispatch_regions(function))?;
            for region in regions {
                let Some(structured) = super::super::control_flow::structure_dispatch(&region)
                else {
                    continue;
                };
                let mut edits =
                    EditSet::replace_statements(region.dispatch_loop.range(), structured);
                edits.push_replace_statement(
                    region.state_declaration.item_index,
                    region.state_declaration.path,
                    None,
                );
                return Ok(edits);
            }
            Err(case.reject())
        },
    )
}
