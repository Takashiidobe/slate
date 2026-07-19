use crate::fixups::facts::{FixupFacts, FunctionId};
use crate::fixups::trace::TraceLogger;
use crate::rust_ast::IndentStmt;

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) -> bool {
    super::inline_temps::fixup(body, function, facts, super::inline_temps::Phase::Late)
}

pub(in crate::fixups) fn fixup_with_logger(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    logger: &mut impl TraceLogger,
) -> bool {
    super::inline_temps::InlineTemps::new(super::inline_temps::Phase::Late, logger)
        .fixup(body, function, facts)
}
