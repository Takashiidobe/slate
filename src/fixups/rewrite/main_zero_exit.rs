use crate::rust_ast::{Expr, FnDef, IndentStmt, Path, Prim, RustValue, Stmt, Type};

pub(in crate::fixups) fn fixup(f: &mut FnDef) {
    if f.name != "main" {
        return;
    }
    if final_main_exit_is_zero(&f.body) {
        f.body.pop();
    };
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
            vis: Visibility::Private,
            unsafe_: false,
            extern_c: false,
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
            vis: Visibility::Private,
            unsafe_: false,
            extern_c: false,
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
            vis: Visibility::Private,
            unsafe_: false,
            extern_c: false,
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
            vis: Visibility::Private,
            unsafe_: false,
            extern_c: false,
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
            vis: Visibility::Private,
            unsafe_: false,
            extern_c: false,
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
            vis: Visibility::Private,
            unsafe_: false,
            extern_c: false,
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
