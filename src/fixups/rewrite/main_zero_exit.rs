use crate::fixups::facts::PathSegment;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, named_path_location, path_fact,
    stmt_snippet,
};
use crate::rust_ast::{Expr, FnDef, IndentStmt, Path, Prim, RustValue, Stmt, Type};

pub(in crate::fixups) fn fixup(f: &mut FnDef) {
    let mut logger = crate::fixups::trace::NoopLogger;
    MainZeroExit::new(&mut logger).fixup(f);
}

pub(in crate::fixups) struct MainZeroExit<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> MainZeroExit<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, f: &mut FnDef) {
        if f.name != "main" {
            return;
        }
        if final_main_exit_is_zero(&f.body) {
            if let Some(before) = self
                .logger
                .is_enabled()
                .then(|| f.body.last().map(|stmt| stmt.stmt.clone()))
                .flatten()
            {
                let path = vec![PathSegment::Stmt(f.body.len() - 1)];
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::MainZeroExit,
                    kind: "remove_trailing_zero_exit".into(),
                    location: named_path_location("main", &path),
                    before: vec![stmt_snippet("exit", &before)],
                    after: Vec::new(),
                    facts: vec![path_fact("stmt_path", &path), fact("exit_status", "0")],
                });
            }
            f.body.pop();
        };
    }
}

fn final_main_exit_is_zero(body: &[IndentStmt]) -> bool {
    let Some(stmt) = body.last().map(|indent| &indent.stmt) else {
        return false;
    };
    let Stmt::Expr(Expr::Call { func, args, .. }) = stmt else {
        return false;
    };
    if !is_std_process_exit(func) || args.len() != 1 {
        return false;
    }
    is_zero_exit_arg(&args[0])
}

fn is_zero_exit_arg(expr: &Expr) -> bool {
    match expr {
        Expr::Value(RustValue::I64(0) | RustValue::I128(0)) => true,
        Expr::Cast {
            expr,
            ty: Type::Prim(Prim::I32),
        } => is_zero_exit_arg(expr),
        _ => false,
    }
}

fn is_std_process_exit(expr: &Expr) -> bool {
    let Expr::Path(Path { segments }) = expr else {
        return false;
    };
    let expected = ["std", "process", "exit"];
    segments.len() == expected.len()
        && segments
            .iter()
            .zip(expected)
            .all(|(segment, expected)| segment.as_str() == expected)
}
