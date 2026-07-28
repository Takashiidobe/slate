use crate::fixups::Fixup;
use crate::fixups::facts::PathSegment;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, named_path_location, path_fact, stmt_snippet,
};
use crate::rust_ast::{IndentStmt, Stmt};

pub(in crate::fixups) struct FinalReturns<'a> {
    function_name: String,
    returns_value: bool,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for FinalReturns<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        if !self.returns_value {
            return false;
        }
        let Some(last_index) = body.len().checked_sub(1) else {
            return false;
        };
        let Stmt::Return(Some(expr)) = &body[last_index].stmt else {
            return false;
        };

        let before = self
            .logger
            .is_enabled()
            .then(|| body[last_index].stmt.clone());
        body[last_index].stmt = Stmt::Expr(expr.clone());

        if let Some(before) = before {
            let path = vec![PathSegment::Stmt(last_index)];
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::FinalReturns,
                kind: "rewrite_final_return".into(),
                location: named_path_location(&self.function_name, &path),
                before: vec![stmt_snippet("return", &before)],
                after: vec![stmt_snippet("expr", &body[last_index].stmt)],
                facts: vec![path_fact("stmt_path", &path)],
            });
        }

        true
    }
}

impl<'a> FinalReturns<'a> {
    pub(in crate::fixups) fn new(
        function_name: String,
        returns_value: bool,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            function_name,
            returns_value,
            logger,
        }
    }
}
