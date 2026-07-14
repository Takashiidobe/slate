use crate::rust_ast::{Expr, FnDef, Ident, IndentStmt, Path, Prim, Stmt, Type};

pub(in crate::fixups) fn fixup(f: &mut FnDef) {
    if f.name != "main" {
        return;
    }
    let Some(exit_arg) = final_main_exit_arg(&mut f.body) else {
        return;
    };

    f.ret = Some(Type::Custom("std::process::ExitCode".into()));
    *exit_arg = exit_code_expr(exit_arg.clone());
}

fn final_main_exit_arg(body: &mut [IndentStmt]) -> Option<&mut Expr> {
    let stmt = &mut body.last_mut()?.stmt;
    let Stmt::Expr(Expr::Call { func, args }) = stmt else {
        return None;
    };
    if !is_std_process_exit(func) || args.len() != 1 {
        return None;
    }
    *stmt = Stmt::Return(Some(args.pop()?));
    let Stmt::Return(Some(expr)) = stmt else {
        unreachable!();
    };
    Some(expr)
}

fn exit_code_expr(expr: Expr) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Path(path(["std", "process", "ExitCode", "from"]))),
        args: vec![Expr::Cast {
            expr: Box::new(expr),
            ty: Type::Prim(Prim::U8),
        }],
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

fn path<const N: usize>(segments: [&str; N]) -> Path {
    Path::new(segments.into_iter().map(Ident::from))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, Program, Visibility};

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

    #[test]
    fn rewrites_final_main_exit_to_exit_code_return() {
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
fn main() -> std::process::ExitCode {
    return std::process::ExitCode::from((0 as i32) as u8);
}
"
        );
    }

    #[test]
    fn leaves_non_main_exit_call_unchanged() {
        let f = FnDef {
            vis: Visibility::Private,
            unsafe_: false,
            extern_c: false,
            name: "f".into(),
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
}
