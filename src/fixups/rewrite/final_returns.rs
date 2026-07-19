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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Block, Expr, Item, Program, UnaryOp};

    fn after(mut f: FnDef) -> String {
        fixup(&mut f);
        Program {
            items: vec![Item::Fn(f)],
        }
        .emit()
    }

    #[test]
    fn rewrites_final_value_return_to_tail_expr() {
        let out = after(func(
            vec![],
            Some("i32"),
            vec![temp("x", "i32", int(1)), Stmt::Return(Some(var("x")))],
        ));

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let x: i32 = 1;
    x
}
"
        );
    }

    #[test]
    fn leaves_early_return_unchanged() {
        let out = after(func(
            vec![],
            Some("i32"),
            vec![Stmt::Return(Some(int(0))), Stmt::Expr(int(1))],
        ));

        assert!(out.contains("return 0;"));
        assert!(out.ends_with("    1\n}\n"));
    }

    #[test]
    fn leaves_unit_return_unchanged() {
        let out = after(func(vec![], None, vec![Stmt::Return(None)]));

        assert_eq!(
            out,
            "\
fn f() {
    return;
}
"
        );
    }

    #[test]
    fn parenthesizes_unsafe_block_binary_tail() {
        let out = after(func(
            vec![],
            Some("i32"),
            vec![Stmt::Return(Some(bin(
                BinOp::Add,
                Expr::Unsafe(Box::new(Block {
                    stmts: vec![],
                    tail: Some(Box::new(Expr::Unary {
                        op: UnaryOp::Deref,
                        expr: Box::new(var("value")),
                    })),
                })),
                int(1),
            )))],
        ));

        assert!(out.contains("    (unsafe { *value }) + 1\n"));
    }
}
