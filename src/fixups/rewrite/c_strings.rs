use crate::fixups::Fixup;
use crate::fixups::facts::{AstPath, FixupFacts, FunctionId};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, TraceSnippet, function_path_location, path_fact,
};
use crate::rust_ast::{Expr, IndentStmt};

pub(in crate::fixups) struct CStrings<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for CStrings<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        let mut changed = false;
        walk::body_exprs_mut_with_path(body, &mut Vec::new(), &mut |expr, path| {
            if let Some(fact) = self
                .facts
                .c_string_literal(self.function, &AstPath(path.to_vec()))
                && matches!(expr, Expr::ByteStr(_))
            {
                let before = self.logger.is_enabled().then(|| expr.clone());
                *expr = Expr::CStr(fact.bytes.clone());
                changed = true;
                if let Some(before) = before {
                    self.logger.rewrite(RewriteEvent {
                        pass: TracePass::CStrings,
                        kind: "rewrite_c_string_literal".into(),
                        location: function_path_location(self.facts, self.function, path),
                        before: vec![TraceSnippet::new("expr", before.render().trim_end())],
                        after: vec![TraceSnippet::new("expr", expr.render().trim_end())],
                        facts: vec![path_fact("expr_path", path)],
                    });
                }
                return false;
            }
            true
        });
        changed
    }
}

impl<'a> CStrings<'a> {
    pub(in crate::fixups) fn new(
        function: FunctionId,
        facts: &'a FixupFacts,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            function,
            facts,
            logger,
        }
    }
}
