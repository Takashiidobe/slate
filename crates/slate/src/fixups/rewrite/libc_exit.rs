use crate::fixups::facts::PathSegment;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, named_path_location, path_fact, stmt_snippet,
};
use crate::rust_ast::{Expr, Ident, IndentStmt, Item, Path, Program, Stmt, Type};

pub(in crate::fixups) fn fixup(program: &mut Program) {
    let mut logger = crate::fixups::trace::NoopLogger;
    LibcExit::new(&mut logger).fixup(program);
}

pub(in crate::fixups) struct LibcExit<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> LibcExit<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program) -> bool {
        if !has_libc_exit_extern(program) {
            return false;
        }
        let mut changed = false;
        for item in &mut program.items {
            let Item::Fn(f) = item else {
                continue;
            };
            changed |= self.fixup_body(&mut f.body, &f.name, &mut Vec::new());
        }
        changed
    }

    fn fixup_body(
        &mut self,
        body: &mut [IndentStmt],
        function_name: &str,
        path: &mut Vec<PathSegment>,
    ) -> bool {
        let mut changed = false;
        for (index, indent) in body.iter_mut().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                    changed |= self.fixup_body(body, function_name, path);
                });
                let before = self.logger.is_enabled().then(|| indent.stmt.clone());
                if rewrite_stmt_exit(&mut indent.stmt) {
                    changed = true;
                    if let Some(before) = before {
                        self.logger.rewrite(RewriteEvent {
                            pass: TracePass::LibcExit,
                            kind: "rewrite_libc_exit".into(),
                            location: named_path_location(function_name, path),
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
}

fn has_libc_exit_extern(program: &Program) -> bool {
    program.items.iter().any(|item| {
        let Item::ExternBlock { decls, .. } = item else {
            return false;
        };
        decls.iter().any(|decl| match decl {
            crate::rust_ast::ExternDecl::Fn(f) => {
                f.name == "exit" && f.params.len() == 1 && matches!(f.ret, Some(Type::Never))
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
    let Expr::Call { func, args, .. } = expr else {
        return None;
    };
    matches!(&**func, Expr::Var(name) if name.as_str() == "exit").then(|| args.clone())
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, ExternDecl, ExternFnDecl, FnParam, Prim, Type};

    fn program_with_exit(stmts: Vec<Stmt>) -> Program {
        Program {
            items: vec![
                Item::ExternBlock {
                    abi: "C".into(),
                    decls: vec![ExternDecl::Fn(ExternFnDecl {
                        name: "exit".into(),
                        params: vec![FnParam {
                            name: "_0".into(),
                            mutable: false,
                            ty: Type::Prim(Prim::I32),
                            nonnull: false,
                        }],
                        variadic: false,
                        ret: Some(Type::Never),
                        returns_nonnull: false,
                    })],
                },
                Item::Fn(func(vec![param("code", "i32")], Some("!"), stmts)),
            ],
        }
    }

    #[test]
    fn rewrites_unsafe_libc_exit_statement_to_std_process_exit() {
        let mut program = program_with_exit(vec![Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: vec![],
            tail: Some(Box::new(call("exit", vec![var("code")]))),
        })))]);

        assert!(LibcExit::new(&mut crate::fixups::trace::NoopLogger).fixup(&mut program));

        assert_eq!(
            program.emit(),
            "\
unsafe extern \"C\" {
    fn exit(_0: i32) -> !;
}

fn f(code: i32) -> ! {
    std::process::exit(code)
}
"
        );
    }

    #[test]
    fn leaves_exit_named_call_without_extern() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![param("code", "i32")],
                None,
                vec![Stmt::Expr(call("exit", vec![var("code")]))],
            ))],
        };

        assert!(!LibcExit::new(&mut crate::fixups::trace::NoopLogger).fixup(&mut program));
        assert!(program.emit().contains("exit(code);"));
    }
}
