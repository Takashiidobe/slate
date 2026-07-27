use crate::fixups::facts::{AstPath, FixupFacts};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, TraceSnippet, function_path_location, path_fact,
};
use crate::rust_ast::{Expr, Item, Program};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
    let mut logger = crate::fixups::trace::NoopLogger;
    CStrings::new(&mut logger).fixup(program, facts);
}

pub(in crate::fixups) struct CStrings<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> CStrings<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) {
        fixup_impl(program, facts, self.logger);
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts, logger: &mut dyn TraceLogger) {
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        walk::body_exprs_mut_with_path(&mut f.body, &mut Vec::new(), &mut |expr, path| {
            if let Some(fact) = facts.c_string_literal(function, &AstPath(path.to_vec()))
                && matches!(expr, Expr::ByteStr(_))
            {
                let before = logger.is_enabled().then(|| expr.clone());
                *expr = Expr::CStr(fact.bytes.clone());
                if let Some(before) = before {
                    logger.rewrite(RewriteEvent {
                        pass: TracePass::CStrings,
                        kind: "rewrite_c_string_literal".into(),
                        location: function_path_location(facts, function, path),
                        before: vec![TraceSnippet::new("expr", before.render().trim_end())],
                        after: vec![TraceSnippet::new("expr", expr.render().trim_end())],
                        facts: vec![path_fact("expr_path", path)],
                    });
                }
                return false;
            }
            true
        });
    }
}
