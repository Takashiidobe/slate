use crate::fixups::Fixup;
use crate::fixups::facts::PathSegment;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, named_path_location, path_fact, stmt_snippet,
};
use crate::function_identity::{Known, known_call, known_declaration};
use crate::rust_ast::{Expr, Ident, IndentStmt, Item, Path, Program, Stmt, Type};

pub(in crate::fixups) struct LibcExit<'a> {
    function_name: String,
    enabled: bool,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for LibcExit<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        if !self.enabled {
            return false;
        }
        self.fixup_body(body, &mut Vec::new())
    }
}

impl<'a> LibcExit<'a> {
    pub(in crate::fixups) fn new(
        function_name: impl Into<String>,
        enabled: bool,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            function_name: function_name.into(),
            enabled,
            logger,
        }
    }

    fn fixup_body(&mut self, body: &mut [IndentStmt], path: &mut Vec<PathSegment>) -> bool {
        let mut changed = false;
        for (index, indent) in body.iter_mut().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                    changed |= self.fixup_body(body, path);
                });
                let before = self.logger.is_enabled().then(|| indent.stmt.clone());
                if rewrite_stmt_exit(&mut indent.stmt) {
                    changed = true;
                    if let Some(before) = before {
                        self.logger.rewrite(RewriteEvent {
                            pass: TracePass::LibcExit,
                            kind: "rewrite_libc_exit".into(),
                            location: named_path_location(self.function_name.clone(), path),
                            before: vec![stmt_snippet("stmt", &before)],
                            after: vec![stmt_snippet("stmt", &indent.stmt)],
                            facts: vec![path_fact("stmt_path", path)],
                        });
                    }
                }
            });
        }
        changed
    }

    pub(in crate::fixups) fn is_enabled(program: &Program) -> bool {
        has_libc_exit_extern(program)
    }
}

fn has_libc_exit_extern(program: &Program) -> bool {
    program.items.iter().any(|item| {
        let Item::ExternBlock { decls, .. } = item else {
            return false;
        };
        decls.iter().any(|decl| match decl {
            crate::rust_ast::ExternDecl::Fn(f) => {
                known_declaration(f.identity, &f.name) == Some(Known::Exit)
                    && f.params.len() == 1
                    && matches!(f.ret, Some(Type::Never))
            }
            crate::rust_ast::ExternDecl::Static { .. } => false,
        })
    })
}

fn rewrite_stmt_exit(stmt: &mut Stmt) -> bool {
    match stmt {
        Stmt::Expr(expr) => rewrite_exit_expr_stmt(expr),
        Stmt::Return(Some(expr)) => rewrite_exit_expr(expr),
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Assign { value: expr, .. }
        | Stmt::CompoundAssign { value: expr, .. } => rewrite_exit_expr(expr),
        Stmt::If { cond, .. } | Stmt::While { cond, .. } => rewrite_exit_expr(cond),
        Stmt::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => {
            let c = rewrite_exit_expr(cond);
            let t = rewrite_exit_expr(then_value);
            let e = rewrite_exit_expr(else_value);
            c || t || e
        }
        _ => false,
    }
}

fn rewrite_exit_expr_stmt(expr: &mut Expr) -> bool {
    if let Expr::Unsafe(block) = expr
        && block.stmts.is_empty()
        && let Some(tail) = block.tail.as_deref()
        && let Some(args) = libc_exit_args(tail)
    {
        *expr = std_process_exit(args);
        return true;
    }
    rewrite_exit_expr(expr)
}

fn rewrite_exit_expr(expr: &mut Expr) -> bool {
    if let Some(args) = libc_exit_args(expr) {
        *expr = std_process_exit(args);
        return true;
    }
    false
}

fn libc_exit_args(expr: &Expr) -> Option<Vec<Expr>> {
    let Expr::Call { args, .. } = expr else {
        return None;
    };
    (known_call(expr) == Some(Known::Exit)).then(|| args.clone())
}

fn std_process_exit(args: Vec<Expr>) -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Path(Path::new(
            ["std", "process", "exit"].map(Ident::from),
        ))),
        args,
    }
}
