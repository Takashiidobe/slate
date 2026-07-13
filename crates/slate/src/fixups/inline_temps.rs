//! Inline single-use pure temps directly on the statement list. The temp's init
//! is spliced as an `Expr` subtree into its use site and precedence-aware
//! rendering elides redundant parens.

use crate::fixups::idents::{expr_ident_count, stmt_ident_count};
use crate::fixups::support::walk;
use crate::rust_ast::{Block, Expr, IndentStmt, Stmt};

pub(super) fn fixup(body: &mut Vec<IndentStmt>) {
    fixup_with_tails(body, &[]);
}

// `tails` are the yielded value expressions of the enclosing block (a `LetIf`
// branch value, an `unsafe` block tail). They are not statements we can
// substitute into here, but a temp that feeds one is used past the statement
// list, so it must not be treated as single-use.
fn fixup_with_tails(body: &mut Vec<IndentStmt>, tails: &[&Expr]) {
    inline_nested_temps(body);
    loop {
        let mut applied = false;
        for i in 0..body.len() {
            let Stmt::Let {
                name,
                mutable: false,
                init: Some(init),
                ..
            } = &body[i].stmt
            else {
                continue;
            };
            if !is_temp_name(name) || !is_pure_expr(init) {
                continue;
            }
            let name = name.clone();
            let init = init.clone();
            let Some(use_index) = single_safe_use(body, i, &name, tails) else {
                continue;
            };
            if body[use_index].stmt.substitute_var(&name, &init) {
                body.remove(i);
                applied = true;
                break;
            }
        }
        if !applied {
            break;
        }
        inline_nested_temps(body);
    }
}

fn inline_nested_temps(body: &mut [IndentStmt]) {
    for stmt in body {
        match &mut stmt.stmt {
            Stmt::If {
                then_body,
                else_body,
                ..
            } => {
                fixup(then_body);
                fixup(else_body);
            }
            Stmt::LetIf {
                then_body,
                then_value,
                else_body,
                else_value,
                ..
            } => {
                fixup_with_tails(then_body, &[then_value]);
                fixup_with_tails(else_body, &[else_value]);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                fixup(body);
            }
            Stmt::Unsafe { body } => {
                let Block { stmts, tail } = body;
                match tail {
                    Some(tail) => fixup_with_tails(stmts, &[tail]),
                    None => fixup(stmts),
                }
            }
            _ => {}
        }
    }
}

fn single_safe_use(
    body: &[IndentStmt],
    def_index: usize,
    name: &str,
    tails: &[&Expr],
) -> Option<usize> {
    let mut found = None;
    for (index, stmt) in body.iter().enumerate().skip(def_index + 1) {
        let stmt = &stmt.stmt;
        let uses = stmt_ident_count(stmt, name);
        if uses > 0 {
            if uses == 1
                && found.is_none()
                && !stmt_contains_call(stmt)
                && !is_receiver_use(stmt, name)
            {
                found = Some(index);
                continue;
            }
            return None;
        }
        if found.is_some() {
            continue;
        }
        if !is_pure_temp_let(stmt) {
            return None;
        }
    }
    if tails.iter().any(|tail| expr_ident_count(tail, name) > 0) {
        return None;
    }
    found
}

fn is_pure_temp_let(stmt: &Stmt) -> bool {
    matches!(
        stmt,
        Stmt::Let { name, init: Some(init), .. } if is_temp_name(name) && is_pure_expr(init)
    )
}

// Conservative purity: only value/var arithmetic that has no side effects and no
// place dependence beyond its named operands. Matches (and never exceeds) what the
// prior text heuristic inlined, so inlining decisions are unchanged.
fn is_pure_expr(expr: &Expr) -> bool {
    super::effects::is_movable_pure_expr(expr)
}

fn stmt_contains_call(stmt: &Stmt) -> bool {
    walk::stmt_exprs_any(stmt, &mut |expr| {
        matches!(
            expr,
            Expr::Call { .. }
                | Expr::MethodCall { .. }
                | Expr::MethodCallGeneric { .. }
                | Expr::Macro { .. }
        )
    })
}

fn is_receiver_use(stmt: &Stmt, name: &str) -> bool {
    walk::stmt_exprs_any(stmt, &mut |expr| {
        let receiver = match expr {
            Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => Some(&**recv),
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => Some(&**base),
            _ => None,
        };
        matches!(receiver, Some(Expr::Var(v)) if v.as_str() == name)
    })
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Type};

    fn inlined(stmts: Vec<Stmt>) -> String {
        let mut f = func(vec![], None, stmts);
        fixup(&mut f.body);
        emit(f)
    }

    #[test]
    fn inlines_single_use_scalar_temps() {
        let out = inlined(vec![
            let_mut("a", "i32", int(0)),
            temp("_v0", "i32", int(20)),
            assign("a", var("_v0")),
            temp("_v1", "i32", int(5)),
            temp("_v2", "i32", var("a")),
            temp("_v3", "i32", bin(BinOp::Sub, var("_v2"), var("_v1"))),
            assign("a", var("_v3")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let mut a: i32 = 0;
    a = 20;
    a = a - 5;
}
"
        );
    }

    #[test]
    fn keeps_temp_that_feeds_a_branch_value() {
        // a temp used once in the branch body (the store) and once as the branch's
        // yielded value must not be inlined away, or the tail reference dangles.
        let out = inlined(vec![Stmt::LetIf {
            name: "_v0".into(),
            mutable: false,
            ty: Some(Type::parse("i32")),
            cond: var("c"),
            then_body: vec![
                IndentStmt {
                    depth: 2,
                    stmt: temp("_v1", "i32", bin(BinOp::Add, var("a"), int(1))),
                },
                IndentStmt {
                    depth: 2,
                    stmt: assign("a", var("_v1")),
                },
            ],
            then_value: var("_v1"),
            else_body: vec![],
            else_value: int(0),
        }]);

        assert!(
            out.contains("let _v1: i32 = a + 1;"),
            "binding must survive, got:\n{out}"
        );
        assert_eq!(
            out.matches("_v1").count(),
            3,
            "def + store use + branch value"
        );
    }

    #[test]
    fn does_not_inline_call_results() {
        let out = inlined(vec![
            temp("_v0", "i32", call("g", vec![])),
            temp("_v1", "i32", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = g();
    let _v1: i32 = _v0;
}
"
        );
    }

    #[test]
    fn does_not_cross_side_effecting_statement() {
        let out = inlined(vec![
            temp("_v0", "i32", var("a")),
            Stmt::Expr(call("printf", vec![var("_v1")])),
            assign("b", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = a;
    printf(_v1);
    b = _v0;
}
"
        );
    }

    #[test]
    fn does_not_inline_impure_intrinsics() {
        let out = inlined(vec![
            temp(
                "_v0",
                "i32",
                call("std::ptr::read_volatile", vec![var("p")]),
            ),
            assign("b", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = std::ptr::read_volatile(p);
    b = _v0;
}
"
        );
    }

    #[test]
    fn does_not_inline_method_receivers_that_need_type_annotations() {
        let out = inlined(vec![
            temp("_v0", "i32", int(2147483647)),
            temp("_v1", "i32", int(1)),
            Stmt::Let {
                name: "_v2".into(),
                mutable: false,
                ty: None,
                init: Some(Expr::MethodCall {
                    recv: Box::new(var("_v0")),
                    method: "overflowing_add".into(),
                    args: vec![var("_v1")],
                }),
            },
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = 2147483647;
    let _v1: i32 = 1;
    let _v2 = _v0.overflowing_add(_v1);
}
"
        );
    }

    #[test]
    fn does_not_inline_call_arguments_that_need_type_annotations() {
        let out = inlined(vec![
            temp("_v0", "i64", int(9223372036854775807)),
            Stmt::Let {
                name: "_v1".into(),
                mutable: false,
                ty: Some(Type::parse("i32")),
                init: Some(call("printf", vec![var("_v0")])),
            },
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i64 = 9223372036854775807;
    let _v1: i32 = printf(_v0);
}
"
        );
    }
}
