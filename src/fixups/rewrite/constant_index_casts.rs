use crate::fixups::support::walk;
use crate::rust_ast::{Expr, IndentStmt, Prim, RustValue, Type};

pub(in crate::fixups) fn fixup(body: &mut Vec<IndentStmt>) {
    walk::body_exprs_mut_with(body, &mut |expr| {
        if let Expr::Index { index, .. } = expr
            && let Some(replacement) = simplified_index(index)
        {
            **index = replacement;
        }
        true
    });
}

fn simplified_index(index: &Expr) -> Option<Expr> {
    let Expr::Cast { expr, ty } = index else {
        return None;
    };
    if !matches!(ty, Type::Prim(Prim::Usize)) {
        return None;
    }
    match &**expr {
        Expr::Value(RustValue::I64(value)) if *value >= 0 => Some((**expr).clone()),
        Expr::Value(RustValue::I128(value)) if *value >= 0 => Some((**expr).clone()),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{RustValue, Stmt};

    fn cast(expr: Expr, ty: &str) -> Expr {
        Expr::Cast {
            expr: Box::new(expr),
            ty: Type::parse(ty),
        }
    }

    fn index(base: Expr, index: Expr) -> Expr {
        Expr::Index {
            base: Box::new(base),
            index: Box::new(index),
        }
    }

    #[test]
    fn strips_nonnegative_literal_usize_index_casts() {
        let out = after_body(
            fixup,
            vec![],
            None,
            vec![
                let_mut("values", "[i32; 3]", int(0)),
                Stmt::Expr(index(var("values"), cast(int(0), "usize"))),
                Stmt::Expr(index(
                    var("values"),
                    cast(Expr::Value(RustValue::I128(2)), "usize"),
                )),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let mut values: [i32; 3] = 0;
    values[0];
    values[2];
}
"
        );
    }

    #[test]
    fn keeps_dynamic_and_potentially_signed_index_casts() {
        let out = after_body(
            fixup,
            vec![param("index", "i32")],
            None,
            vec![
                Stmt::Expr(index(
                    var("values"),
                    cast(cast(var("index"), "i64"), "usize"),
                )),
                Stmt::Expr(index(
                    var("values"),
                    cast(
                        Expr::Unary {
                            op: crate::rust_ast::UnaryOp::Neg,
                            expr: Box::new(int(1)),
                        },
                        "usize",
                    ),
                )),
                Stmt::Expr(cast(int(0), "usize")),
            ],
        );

        assert_eq!(
            out,
            "\
fn f(index: i32) {
    values[((index as i64) as usize)];
    values[(-1 as usize)];
    0 as usize;
}
"
        );
    }
}
