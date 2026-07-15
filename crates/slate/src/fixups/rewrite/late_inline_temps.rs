use crate::fixups::facts::{FixupFacts, FunctionId};
use crate::rust_ast::IndentStmt;

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) -> bool {
    super::inline_temps::fixup(body, function, facts, super::inline_temps::Phase::Late)
}
