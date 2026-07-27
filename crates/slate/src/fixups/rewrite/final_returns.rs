use crate::fixups::facts::PathSegment;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, named_path_location, path_fact, stmt_snippet,
};
use crate::rust_ast::{FnDef, Stmt};

pub(in crate::fixups) fn fixup(f: &mut FnDef) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    FinalReturns::new(&mut logger).fixup(f)
}

pub(in crate::fixups) struct FinalReturns<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> FinalReturns<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, f: &mut FnDef) -> bool {
        if f.ret.is_none() {
            return false;
        }
        let Some(last_index) = f.body.len().checked_sub(1) else {
            return false;
        };
        let Stmt::Return(Some(expr)) = &f.body[last_index].stmt else {
            return false;
        };

        let before = self
            .logger
            .is_enabled()
            .then(|| f.body[last_index].stmt.clone());
        f.body[last_index].stmt = Stmt::Expr(expr.clone());

        if let Some(before) = before {
            let path = vec![PathSegment::Stmt(last_index)];
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::FinalReturns,
                kind: "rewrite_final_return".into(),
                location: named_path_location(&f.name, &path),
                before: vec![stmt_snippet("return", &before)],
                after: vec![stmt_snippet("expr", &f.body[last_index].stmt)],
                facts: vec![path_fact("stmt_path", &path)],
            });
        }

        true
    }
}
