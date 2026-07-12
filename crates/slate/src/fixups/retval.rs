//! Collapse a return-value slot store into the `return` that immediately reads
//! it, when the slot is used only for that round trip.

use crate::fixups::idents::{expr_ident, stmt_ident_count};
use crate::rust_ast::{IndentStmt, Stmt};

pub(super) fn fixup(body: &mut Vec<IndentStmt>) {
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::BinOp;

    #[test]
    fn collapses_retval_store_into_return() {
        let out = after_body(
            fixup,
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

        assert_eq!(after_body(fixup, vec![], Some("i32"), stmts), expected);
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

        assert_eq!(after_body(fixup, vec![], Some("i32"), stmts), expected);
    }
}
