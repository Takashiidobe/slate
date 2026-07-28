use crate::fixups::Fixup;
use crate::fixups::facts::PathSegment;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, named_path_location, path_fact,
    stmt_snippet,
};
use crate::rust_ast::{Expr, IndentStmt, Path, Prim, RustValue, Stmt, Type};

pub(in crate::fixups) struct MainZeroExit<'a> {
    is_main: bool,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for MainZeroExit<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        if !self.is_main {
            return false;
        }
        if final_main_exit_is_zero(body) {
            if let Some(before) = self
                .logger
                .is_enabled()
                .then(|| body.last().map(|stmt| stmt.stmt.clone()))
                .flatten()
            {
                let path = vec![PathSegment::Stmt(body.len() - 1)];
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::MainZeroExit,
                    kind: "remove_trailing_zero_exit".into(),
                    location: named_path_location("main", &path),
                    before: vec![stmt_snippet("exit", &before)],
                    after: Vec::new(),
                    facts: vec![path_fact("stmt_path", &path), fact("exit_status", "0")],
                });
            }
            body.pop();
            return true;
        };
        false
    }
}

impl<'a> MainZeroExit<'a> {
    pub(in crate::fixups) fn new(is_main: bool, logger: &'a mut dyn TraceLogger) -> Self {
        Self { is_main, logger }
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
