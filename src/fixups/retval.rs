//! Collapse a return-value slot store into the final return or main exit when
//! the slot is used only for that round trip.

use crate::fixups::idents::{expr_ident, stmt_ident_count};
use crate::rust_ast::{Expr, FnDef, IndentStmt, Path, Prim, Stmt, Type};

pub(super) fn fixup(f: &mut FnDef) {
    collapse_return_slot(&mut f.body);
    if f.name == "main" {
        collapse_main_exit_slot(&mut f.body);
    }
}

fn collapse_return_slot(body: &mut Vec<IndentStmt>) {
    let Some((ret_index, name)) =
        body.iter()
            .enumerate()
            .find_map(|(index, stmt)| match &stmt.stmt {
                Stmt::Return(Some(expr)) => expr_ident(expr).map(|name| (index, name.to_string())),
                _ => None,
            })
    else {
        return;
    };
    if ret_index == 0 {
        return;
    }

    let store_index = ret_index - 1;
    let value = match &body[store_index].stmt {
        Stmt::Assign { target, value } if expr_ident(target) == Some(name.as_str()) => {
            value.clone()
        }
        _ => return,
    };

    let mentions: usize = body
        .iter()
        .map(|stmt| stmt_ident_count(&stmt.stmt, &name))
        .sum();
    if mentions != 3 {
        return;
    }

    let Some(decl_index) = body
        .iter()
        .position(|stmt| matches!(&stmt.stmt, Stmt::Let { name: n, .. } if n == &name))
    else {
        return;
    };

    body[ret_index].stmt = Stmt::Return(Some(value));
    let mut remove = [store_index, decl_index];
    remove.sort_unstable();
    for index in remove.into_iter().rev() {
        body.remove(index);
    }
}

fn collapse_main_exit_slot(body: &mut Vec<IndentStmt>) {
    let Some((exit_index, temp_name, cast_ty)) =
        body.iter()
            .enumerate()
            .find_map(|(index, stmt)| match &stmt.stmt {
                Stmt::Expr(expr) => main_exit_arg_temp(expr)
                    .map(|(name, ty)| (index, name.to_string(), ty.cloned())),
                _ => None,
            })
    else {
        return;
    };
    if exit_index < 2 {
        return;
    }

    let temp_index = exit_index - 1;
    let retval_name = match &body[temp_index].stmt {
        Stmt::Let {
            name,
            mutable: false,
            init: Some(init),
            ..
        } if name == &temp_name => expr_ident(init).map(str::to_string),
        _ => None,
    };
    let Some(retval_name) = retval_name else {
        return;
    };

    let store_index = temp_index - 1;
    let value = match &body[store_index].stmt {
        Stmt::Assign { target, value } if expr_ident(target) == Some(retval_name.as_str()) => {
            value.clone()
        }
        _ => return,
    };

    let retval_mentions: usize = body
        .iter()
        .map(|stmt| stmt_ident_count(&stmt.stmt, &retval_name))
        .sum();
    let temp_mentions: usize = body
        .iter()
        .map(|stmt| stmt_ident_count(&stmt.stmt, &temp_name))
        .sum();
    if retval_mentions != 3 || temp_mentions != 2 {
        return;
    }

    let Some(decl_index) = body
        .iter()
        .position(|stmt| matches!(&stmt.stmt, Stmt::Let { name, .. } if name == &retval_name))
    else {
        return;
    };

    let replacement = if let Some(ty) = cast_ty {
        Expr::Cast {
            expr: Box::new(value),
            ty,
        }
    } else {
        value
    };

    let Stmt::Expr(expr) = &mut body[exit_index].stmt else {
        unreachable!();
    };
    replace_main_exit_arg(expr, replacement);
    let mut remove = [temp_index, store_index, decl_index];
    remove.sort_unstable();
    for index in remove.into_iter().rev() {
        body.remove(index);
    }
}

fn main_exit_arg_temp(expr: &Expr) -> Option<(&str, Option<&Type>)> {
    let Expr::Call { func, args } = expr else {
        return None;
    };
    if !is_std_process_exit(func) || args.len() != 1 {
        return None;
    }
    match &args[0] {
        Expr::Cast { expr, ty } if matches!(ty, Type::Prim(Prim::I32)) => {
            expr_ident(expr).map(|name| (name, Some(ty)))
        }
        arg => expr_ident(arg).map(|name| (name, None)),
    }
}

fn replace_main_exit_arg(expr: &mut Expr, replacement: Expr) {
    let Expr::Call { args, .. } = expr else {
        return;
    };
    args[0] = replacement;
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
    use crate::rust_ast::{BinOp, Ident, Visibility};

    fn retval_body(body: &mut Vec<IndentStmt>) {
        collapse_return_slot(body);
    }

    fn std_process_exit(expr: Expr) -> Expr {
        Expr::Call {
            func: Box::new(Expr::Path(Path::new(
                ["std", "process", "exit"].map(Ident::from),
            ))),
            args: vec![expr],
        }
    }

    #[test]
    fn collapses_retval_store_into_return() {
        let out = after_body(
            retval_body,
            vec![],
            Some("i32"),
            vec![
                let_mut("__retval", "i32", int(0)),
                let_mut("c", "i32", int(0)),
                assign("c", bin(BinOp::Add, var("a"), var("b"))),
                assign("__retval", var("c")),
                Stmt::Return(Some(var("__retval"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let mut c: i32 = 0;
    c = a + b;
    return c;
}
"
        );
    }

    #[test]
    fn does_not_collapse_when_retval_read_elsewhere() {
        let stmts = vec![
            let_mut("__retval", "i32", int(0)),
            assign("__retval", int(1)),
            let_mut("x", "i32", var("__retval")),
            assign("__retval", var("x")),
            Stmt::Return(Some(var("__retval"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(
            after_body(retval_body, vec![], Some("i32"), stmts),
            expected
        );
    }

    #[test]
    fn does_not_collapse_when_store_is_not_immediately_before_return() {
        let stmts = vec![
            let_mut("__retval", "i32", int(0)),
            assign("__retval", int(1)),
            let_mut("x", "i32", int(2)),
            Stmt::Return(Some(var("__retval"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(
            after_body(retval_body, vec![], Some("i32"), stmts),
            expected
        );
    }

    #[test]
    fn collapses_main_retval_store_into_exit() {
        let out = after_fn(
            fixup,
            FnDef {
                vis: Visibility::Private,
                unsafe_extern_c: false,
                name: "main".into(),
                params: vec![],
                ret: None,
                body: vec![
                    let_mut("__retval", "i32", int(0)),
                    assign("__retval", int(0)),
                    temp("_v1", "i32", var("__retval")),
                    Stmt::Expr(std_process_exit(Expr::Cast {
                        expr: Box::new(var("_v1")),
                        ty: Type::Prim(Prim::I32),
                    })),
                ]
                .into_iter()
                .map(|stmt| IndentStmt { depth: 1, stmt })
                .collect(),
            },
        );

        assert_eq!(
            out,
            "\
fn main() {
    std::process::exit(0 as i32);
}
"
        );
    }

    #[test]
    fn does_not_collapse_main_when_retval_read_elsewhere() {
        let f = FnDef {
            vis: Visibility::Private,
            unsafe_extern_c: false,
            name: "main".into(),
            params: vec![],
            ret: None,
            body: vec![
                let_mut("__retval", "i32", int(0)),
                assign("__retval", int(1)),
                let_mut("x", "i32", var("__retval")),
                assign("__retval", var("x")),
                temp("_v1", "i32", var("__retval")),
                Stmt::Expr(std_process_exit(Expr::Cast {
                    expr: Box::new(var("_v1")),
                    ty: Type::Prim(Prim::I32),
                })),
            ]
            .into_iter()
            .map(|stmt| IndentStmt { depth: 1, stmt })
            .collect(),
        };
        let expected = emit(f.clone());

        assert_eq!(after_fn(fixup, f), expected);
    }
}
