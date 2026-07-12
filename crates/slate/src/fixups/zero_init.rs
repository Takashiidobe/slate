//! Fuse a zero-initialized declaration with the assignment that immediately
//! overwrites it, when the assignment does not read the placeholder.

use crate::fixups::idents::{expr_ident, expr_ident_count};
use crate::rust_ast::{Expr, IndentStmt, RustValue, Stmt};

pub(super) fn fixup(body: &mut Vec<IndentStmt>) {
    for stmt in body.iter_mut() {
        for_nested_body(&mut stmt.stmt, fixup);
    }

    loop {
        let mut changed = false;
        for i in 0..body.len().saturating_sub(1) {
            let Stmt::Let {
                name,
                mutable: true,
                ty: Some(_),
                init: Some(init),
            } = &body[i].stmt
            else {
                continue;
            };
            if !is_zero_expr(init) {
                continue;
            }
            let Stmt::Assign { target, value } = &body[i + 1].stmt else {
                continue;
            };
            if expr_ident(target) != Some(name.as_str()) || expr_ident_count(value, name) != 0 {
                continue;
            }
            let value = value.clone();
            if let Stmt::Let { init, .. } = &mut body[i].stmt {
                *init = Some(value);
            }
            body.remove(i + 1);
            changed = true;
            break;
        }
        if !changed {
            break;
        }
    }
}

fn for_nested_body(stmt: &mut Stmt, f: fn(&mut Vec<IndentStmt>)) {
    match stmt {
        Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            f(then_body);
            f(else_body);
        }
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        } => {
            f(then_body);
            f(else_body);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => f(body),
        Stmt::Unsafe { body } => f(&mut body.stmts),
        _ => {}
    }
}

fn is_zero_expr(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Value(RustValue::I64(0)) | Expr::Value(RustValue::I128(0))
    )
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::BinOp;

    #[test]
    fn fuses_zero_init_with_immediate_first_assignment() {
        let out = after_body(
            fixup,
            vec![],
            Some("i32"),
            vec![
                let_mut("c", "i32", int(0)),
                assign("c", bin(BinOp::Add, var("a"), var("b"))),
                Stmt::Return(Some(var("c"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let mut c: i32 = a + b;
    return c;
}
"
        );
    }

    #[test]
    fn does_not_fuse_when_first_assignment_reads_the_placeholder() {
        let stmts = vec![
            let_mut("c", "i32", int(0)),
            assign("c", bin(BinOp::Add, var("c"), int(1))),
            Stmt::Return(Some(var("c"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(after_body(fixup, vec![], Some("i32"), stmts), expected);
    }

    #[test]
    fn does_not_fuse_when_assignment_is_not_immediate() {
        let stmts = vec![
            let_mut("c", "i32", int(0)),
            Stmt::If {
                cond: var("cond"),
                then_body: vec![IndentStmt {
                    depth: 2,
                    stmt: assign("c", int(1)),
                }],
                else_body: vec![],
            },
            Stmt::Return(Some(var("c"))),
        ];
        let expected = emit(func(
            vec![param("cond", "bool")],
            Some("i32"),
            stmts.clone(),
        ));

        assert_eq!(
            after_body(fixup, vec![param("cond", "bool")], Some("i32"), stmts),
            expected
        );
    }

    #[test]
    fn does_not_fuse_non_placeholder_initializers() {
        let stmts = vec![
            let_mut("c", "i32", int(7)),
            assign("c", int(1)),
            Stmt::Return(Some(var("c"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(after_body(fixup, vec![], Some("i32"), stmts), expected);
    }
}
