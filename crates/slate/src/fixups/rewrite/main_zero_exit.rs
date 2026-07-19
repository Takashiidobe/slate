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
                let mut path = Vec::new();
                path.push(PathSegment::Stmt(f.body.len() - 1));
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
    let Stmt::Expr(Expr::Call { func, args }) = stmt else {
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Ident, Item, MatchArm, Pattern, Program, Visibility};

    fn std_process_exit(expr: Expr) -> Expr {
        Expr::Call {
            func: Box::new(Expr::Path(path(["std", "process", "exit"]))),
            args: vec![expr],
        }
    }

    fn fixed_fn(mut f: FnDef) -> String {
        fixup(&mut f);
        Program {
            items: vec![Item::Fn(f)],
        }
        .emit()
    }

    fn path<const N: usize>(segments: [&str; N]) -> Path {
        Path::new(segments.into_iter().map(Ident::from))
    }

    #[test]
    fn elides_final_main_zero_exit() {
        let out = fixed_fn(FnDef {
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".into(),
            params: vec![],
            ret: None,
            body: vec![IndentStmt {
                depth: 1,
                stmt: Stmt::Expr(std_process_exit(Expr::Cast {
                    expr: Box::new(int(0)),
                    ty: Type::Prim(Prim::I32),
                })),
            }],
        });

        assert_eq!(
            out,
            "\
fn main() {
}
"
        );
    }

    #[test]
    fn leaves_final_main_nonzero_exit_unchanged() {
        let f = FnDef {
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".into(),
            params: vec![],
            ret: None,
            body: vec![IndentStmt {
                depth: 1,
                stmt: Stmt::Expr(std_process_exit(int(1))),
            }],
        };
        let expected = Program {
            items: vec![Item::Fn(f.clone())],
        }
        .emit();

        assert_eq!(fixed_fn(f), expected);
    }

    #[test]
    fn leaves_final_main_dynamic_exit_unchanged() {
        let f = FnDef {
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".into(),
            params: vec![],
            ret: None,
            body: vec![IndentStmt {
                depth: 1,
                stmt: Stmt::Expr(std_process_exit(var("status"))),
            }],
        };
        let expected = Program {
            items: vec![Item::Fn(f.clone())],
        }
        .emit();

        assert_eq!(fixed_fn(f), expected);
    }

    #[test]
    fn leaves_non_main_zero_exit_call_unchanged() {
        let f = FnDef {
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "f".into(),
            params: vec![],
            ret: None,
            body: vec![IndentStmt {
                depth: 1,
                stmt: Stmt::Expr(std_process_exit(int(0))),
            }],
        };
        let expected = Program {
            items: vec![Item::Fn(f.clone())],
        }
        .emit();

        assert_eq!(fixed_fn(f), expected);
    }

    #[test]
    fn leaves_non_final_main_exit_call_unchanged() {
        let f = FnDef {
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".into(),
            params: vec![],
            ret: None,
            body: vec![
                IndentStmt {
                    depth: 1,
                    stmt: Stmt::Expr(std_process_exit(int(1))),
                },
                IndentStmt {
                    depth: 1,
                    stmt: Stmt::Return(None),
                },
            ],
        };
        let expected = Program {
            items: vec![Item::Fn(f.clone())],
        }
        .emit();

        assert_eq!(fixed_fn(f), expected);
    }

    #[test]
    fn leaves_nested_match_arm_zero_exit_unchanged() {
        let f = FnDef {
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".into(),
            params: vec![],
            ret: None,
            body: vec![IndentStmt {
                depth: 1,
                stmt: Stmt::Match {
                    expr: var("state"),
                    arms: vec![MatchArm {
                        pattern: Pattern::I64(0),
                        body: vec![IndentStmt {
                            depth: 0,
                            stmt: Stmt::Expr(std_process_exit(int(0))),
                        }],
                    }],
                },
            }],
        };
        let expected = Program {
            items: vec![Item::Fn(f.clone())],
        }
        .emit();

        assert_eq!(fixed_fn(f), expected);
    }
}
