use crate::fixups::Fixup;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet,
};
use crate::rust_ast::{Expr, IndentStmt, Prim, RustValue, Type};

pub(in crate::fixups) struct ConstantIndexCasts<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for ConstantIndexCasts<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        let mut changed = false;
        walk::body_exprs_mut_with(body, &mut |expr| {
            if let Expr::Index { index, .. } = expr
                && let Some(replacement) = simplified_index(index)
            {
                changed = true;
                let before = self.logger.is_enabled().then(|| (**index).clone());
                **index = replacement;
                if let Some(before) = before {
                    self.logger.rewrite(RewriteEvent {
                        pass: TracePass::ConstantIndexCasts,
                        kind: "strip_constant_index_cast".into(),
                        location: TraceLocation::default(),
                        before: vec![TraceSnippet::new("index", before.render().trim_end())],
                        after: vec![TraceSnippet::new("index", index.render().trim_end())],
                        facts: Vec::new(),
                    });
                }
            }
            true
        });
        changed
    }
}

impl<'a> ConstantIndexCasts<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }
}

fn simplified_index(index: &Expr) -> Option<Expr> {
    let Expr::Cast { expr, ty } = index else {
        return None;
    };
    if !matches!(ty, Type::Prim(Prim::Usize)) {
        return None;
    }
    match &**expr {
        Expr::Value(RustValue::I64(value)) if *value >= 0 => Some((**expr).clone()),
        Expr::Value(RustValue::I128(value)) if *value >= 0 => Some((**expr).clone()),
        _ => None,
    }
}
