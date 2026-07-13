//! Recover compound assignment (`a -= 5`) from the `store(binop(load a, rhs), a)`
//! shape that CIR lowers `a -= 5` into. CIR cannot distinguish `a -= 5` from
//! `a = a - 5`; when both spellings are equivalent this pass prefers the compound
//! form. Restricted to simple local slots (a plain variable target) with a pure
//! rhs, so it never reorders a side effect or touches a volatile/complex lvalue.

use crate::fixups::idents::expr_ident;
use crate::rust_ast::{BinOp, Expr, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(body: &mut Vec<IndentStmt>) {
    for indent in body.iter_mut() {
        match &mut indent.stmt {
            Stmt::If {
                then_body,
                else_body,
                ..
            }
            | Stmt::LetIf {
                then_body,
                else_body,
                ..
            } => {
                fixup(then_body);
                fixup(else_body);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                fixup(body)
            }
            Stmt::Unsafe { body } => fixup(&mut body.stmts),
            Stmt::Assign { target, value } => {
                if let Some((op, rhs)) = compound_parts(target, value) {
                    indent.stmt = Stmt::CompoundAssign {
                        target: target.clone(),
                        op,
                        value: rhs,
                    };
                }
            }
            _ => {}
        }
    }
}

fn compound_parts(target: &Expr, value: &Expr) -> Option<(BinOp, Expr)> {
    let name = expr_ident(target)?;
    let Expr::Binary { op, lhs, rhs } = value else {
        return None;
    };
    if !is_compound_op(*op) || expr_ident(lhs) != Some(name) || !is_pure_expr(rhs) {
        return None;
    }
    Some((*op, (**rhs).clone()))
}

fn is_compound_op(op: BinOp) -> bool {
    matches!(
        op,
        BinOp::Add
            | BinOp::Sub
            | BinOp::Mul
            | BinOp::Div
            | BinOp::Rem
            | BinOp::Shl
            | BinOp::Shr
            | BinOp::BitAnd
            | BinOp::BitOr
            | BinOp::BitXor
    )
}

// Same conservative purity as inline_temps: value/var arithmetic with no side
// effects, so `a op= rhs` cannot reorder or duplicate an effect.
fn is_pure_expr(expr: &Expr) -> bool {
    crate::fixups::facts::is_movable_pure_expr(expr)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::Type;

    #[test]
    fn recovers_every_arithmetic_and_bitwise_form() {
        let out = after_body(
            fixup,
            vec![],
            None,
            vec![
                let_mut("a", "i32", int(20)),
                assign("a", bin(BinOp::Sub, var("a"), int(5))),
                assign("a", bin(BinOp::Mul, var("a"), int(3))),
                assign("a", bin(BinOp::Div, var("a"), int(5))),
                assign("a", bin(BinOp::Rem, var("a"), int(7))),
                assign("a", bin(BinOp::Shl, var("a"), int(3))),
                assign("a", bin(BinOp::Shr, var("a"), int(2))),
                assign("a", bin(BinOp::BitAnd, var("a"), int(6))),
                assign("a", bin(BinOp::BitXor, var("a"), int(3))),
                assign("a", bin(BinOp::BitOr, var("a"), int(8))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let mut a: i32 = 20;
    a -= 5;
    a *= 3;
    a /= 5;
    a %= 7;
    a <<= 3;
    a >>= 2;
    a &= 6;
    a ^= 3;
    a |= 8;
}
"
        );
    }

    #[test]
    fn keeps_assignment_when_lhs_is_not_the_target() {
        let out = after_body(
            fixup,
            vec![],
            None,
            vec![
                let_mut("a", "i32", int(20)),
                assign("a", bin(BinOp::Sub, int(5), var("a"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let mut a: i32 = 20;
    a = 5 - a;
}
"
        );
    }

    #[test]
    fn keeps_assignment_when_rhs_is_impure() {
        let out = after_body(
            fixup,
            vec![],
            None,
            vec![
                let_mut("a", "i32", int(20)),
                assign("a", bin(BinOp::Add, var("a"), call("g", vec![]))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let mut a: i32 = 20;
    a = a + g();
}
"
        );
    }

    #[test]
    fn keeps_comparison_assignment() {
        let out = after_body(
            fixup,
            vec![],
            None,
            vec![
                Stmt::Let {
                    name: "b".into(),
                    mutable: true,
                    ty: Some(Type::parse("bool")),
                    init: Some(Expr::Value(crate::rust_ast::RustValue::Bool(false))),
                },
                assign("b", bin(BinOp::Lt, var("b"), int(1))),
            ],
        );

        assert!(out.contains("b = b < 1;"));
    }
}
