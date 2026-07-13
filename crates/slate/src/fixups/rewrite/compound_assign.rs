//! Recover compound assignment (`a -= 5`) from the `store(binop(load a, rhs), a)`
//! shape that CIR lowers `a -= 5` into. CIR cannot distinguish `a -= 5` from
//! `a = a - 5`; when both spellings are equivalent this pass prefers the compound
//! form. Restricted to simple local slots (a plain variable target) with a pure
//! rhs, so it never reorders a side effect or touches a volatile/complex lvalue.

use crate::fixups::facts::{AstPath, EffectSubject, FixupFacts, FunctionId, PathSegment, Purity};
use crate::fixups::idents::expr_ident;
use crate::fixups::support::walk;
use crate::rust_ast::{BinOp, Expr, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) {
    fixup_at(body, function, facts, &mut Vec::new());
}

fn fixup_at(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) {
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
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
                    walk::with_path_segment(path, PathSegment::Then, |path| {
                        fixup_at(then_body, function, facts, path);
                    });
                    walk::with_path_segment(path, PathSegment::Else, |path| {
                        fixup_at(else_body, function, facts, path);
                    });
                }
                Stmt::Loop { body, .. } => {
                    walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                        fixup_at(body, function, facts, path);
                    });
                }
                Stmt::Scope { body } => {
                    walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                        fixup_at(body, function, facts, path);
                    });
                }
                Stmt::LabeledBlock { body, .. } => {
                    walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                        fixup_at(body, function, facts, path);
                    });
                }
                Stmt::Unsafe { body } => {
                    walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                        fixup_at(&mut body.stmts, function, facts, path);
                    });
                }
                Stmt::Assign { target, value } => {
                    if let Some((op, rhs)) = compound_parts(target, value, function, facts, path) {
                        indent.stmt = Stmt::CompoundAssign {
                            target: target.clone(),
                            op,
                            value: rhs,
                        };
                    }
                }
                _ => {}
            }
        });
    }
}

fn compound_parts(
    target: &Expr,
    value: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<(BinOp, Expr)> {
    let name = expr_ident(target)?;
    let Expr::Binary { op, lhs, rhs } = value else {
        return None;
    };
    let mut rhs_path = path.to_vec();
    rhs_path.push(PathSegment::Expr(1));
    if !is_compound_op(*op)
        || expr_ident(lhs) != Some(name)
        || !is_pure_expr(function, facts, &rhs_path)
    {
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

fn is_pure_expr(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| fact.purity == Purity::MovablePure)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, Program, Type};

    fn after_facts(
        params: Vec<crate::rust_ast::FnParam>,
        ret: Option<&str>,
        stmts: Vec<Stmt>,
    ) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(params, ret, stmts))],
        };
        let analyzed = crate::fixups::facts::analyze(program.clone());
        let facts = analyzed.facts;
        let Item::Fn(f) = &mut program.items[0] else {
            unreachable!();
        };
        fixup(&mut f.body, FunctionId(0), &facts);
        program.emit()
    }

    #[test]
    fn recovers_every_arithmetic_and_bitwise_form() {
        let out = after_facts(
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
        let out = after_facts(
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
        let out = after_facts(
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
        let out = after_facts(
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
