//! Recover a slice reduction from the accumulator-loop shape `slice_loop`
//! leaves behind: `let mut acc = <init>;` immediately followed by
//! `for item in slice.iter() { acc OP= *item; }`. `+=`/`*=` fold to
//! `.sum()`/`.product()` when the init exactly matches the trait identity
//! (0/1); the bitwise ops have no such identity requirement and fold to
//! `.fold(init, |acc, x| acc OP *x)` instead, reusing the original init verbatim.

use crate::fixups::facts::PathSegment;
use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk;
use crate::rust_ast::{BinOp, Expr, IndentStmt, Item, Program, RustValue, Stmt, UnaryOp};

pub(in crate::fixups) fn fixup(program: &mut Program) -> bool {
    let mut changed = false;
    for item in &mut program.items {
        let Item::Fn(f) = item else {
            continue;
        };
        changed |= rewrite_body(&mut f.body, &mut Vec::new());
    }
    changed
}

fn rewrite_body(body: &mut Vec<IndentStmt>, path: &mut Vec<PathSegment>) -> bool {
    let mut changed = false;
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                changed |= rewrite_body(body, path);
            });
        });
    }

    for index in (0..body.len().saturating_sub(1)).rev() {
        let Some(replacement) = replacement_for_pair(&body[index..index + 2], &body[..index])
        else {
            continue;
        };
        body.splice(index..index + 2, [replacement]);
        changed = true;
    }
    changed
}

fn replacement_for_pair(pair: &[IndentStmt], preceding: &[IndentStmt]) -> Option<IndentStmt> {
    let Stmt::Let {
        name: accum_name,
        mutable: true,
        ty,
        init: Some(init),
    } = &pair[0].stmt
    else {
        return None;
    };
    let Stmt::For {
        pat: item_name,
        iter,
        body,
    } = &pair[1].stmt
    else {
        return None;
    };
    let Expr::MethodCall { recv, method, args } = iter else {
        return None;
    };
    if method != "iter" || !args.is_empty() {
        return None;
    }
    let Expr::Var(slice_name) = &**recv else {
        return None;
    };
    let [reduce_stmt] = body.as_slice() else {
        return None;
    };
    let Stmt::CompoundAssign { target, op, value } = &reduce_stmt.stmt else {
        return None;
    };
    if !matches!(target, Expr::Var(name) if name.as_str() == accum_name.as_str()) {
        return None;
    }
    if !is_bare_item_deref(value, item_name.as_str()) {
        return None;
    }
    if preceding
        .iter()
        .any(|indent| stmt_ident_count(&indent.stmt, accum_name.as_str()) > 0)
    {
        return None;
    }

    let init_call = reduction_call(slice_name.as_str(), *op, init)?;
    Some(IndentStmt {
        depth: pair[1].depth,
        stmt: Stmt::Let {
            name: accum_name.clone(),
            mutable: false,
            ty: ty.clone(),
            init: Some(init_call),
        },
    })
}

fn is_bare_item_deref(expr: &Expr, item_name: &str) -> bool {
    matches!(expr, Expr::Unary { op: UnaryOp::Deref, expr }
        if matches!(&**expr, Expr::Var(name) if name.as_str() == item_name))
}

fn reduction_call(slice_name: &str, op: BinOp, init: &Expr) -> Option<Expr> {
    let iter_expr = Expr::MethodCall {
        recv: Box::new(Expr::Var(slice_name.into())),
        method: "iter".into(),
        args: Vec::new(),
    };
    match op {
        BinOp::Add if integer_value(init) == Some(0) => Some(Expr::MethodCall {
            recv: Box::new(iter_expr),
            method: "sum".into(),
            args: Vec::new(),
        }),
        BinOp::Mul if integer_value(init) == Some(1) => Some(Expr::MethodCall {
            recv: Box::new(iter_expr),
            method: "product".into(),
            args: Vec::new(),
        }),
        BinOp::BitAnd | BinOp::BitOr | BinOp::BitXor => Some(Expr::MethodCall {
            recv: Box::new(iter_expr),
            method: "fold".into(),
            args: vec![
                init.clone(),
                Expr::Closure {
                    params: vec!["acc".into(), "x".into()],
                    body: Box::new(Expr::Binary {
                        op,
                        lhs: Box::new(Expr::Var("acc".into())),
                        rhs: Box::new(Expr::Unary {
                            op: UnaryOp::Deref,
                            expr: Box::new(Expr::Var("x".into())),
                        }),
                    }),
                },
            ],
        }),
        _ => None,
    }
}

fn integer_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some(i128::from(*n)),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Cast { expr, .. } => integer_value(expr),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{FnParam, Type};

    fn stmt(stmt: Stmt) -> IndentStmt {
        IndentStmt { depth: 1, stmt }
    }

    fn slice_param(name: &str) -> FnParam {
        FnParam {
            name: name.into(),
            mutable: false,
            ty: Type::parse("&[i32]"),
        }
    }

    fn item_deref() -> Expr {
        Expr::Unary {
            op: UnaryOp::Deref,
            expr: Box::new(var("item")),
        }
    }

    fn for_loop(reduce_body: Vec<IndentStmt>) -> Stmt {
        Stmt::For {
            pat: "item".into(),
            iter: Expr::MethodCall {
                recv: Box::new(var("items")),
                method: "iter".into(),
                args: vec![],
            },
            body: reduce_body,
        }
    }

    #[test]
    fn rewrites_sum_accumulator_to_iter_sum() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![slice_param("items")],
                Some("i32"),
                vec![
                    let_mut("total", "i32", int(0)),
                    for_loop(vec![stmt(Stmt::CompoundAssign {
                        target: var("total"),
                        op: BinOp::Add,
                        value: item_deref(),
                    })]),
                    Stmt::Return(Some(var("total"))),
                ],
            ))],
        };

        assert!(fixup(&mut program));
        let out = program.emit();

        assert!(out.contains("let total: i32 = items.iter().sum();"));
        assert!(!out.contains("for item in items.iter()"));
        assert!(!out.contains("total +="));
    }

    #[test]
    fn rewrites_xor_accumulator_to_iter_fold() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![slice_param("items")],
                Some("i32"),
                vec![
                    let_mut("acc", "i32", int(0)),
                    for_loop(vec![stmt(Stmt::CompoundAssign {
                        target: var("acc"),
                        op: BinOp::BitXor,
                        value: item_deref(),
                    })]),
                    Stmt::Return(Some(var("acc"))),
                ],
            ))],
        };

        assert!(fixup(&mut program));
        let out = program.emit();

        assert!(out.contains("let acc: i32 = items.iter().fold(0, |acc, x| acc ^ *x);"));
        assert!(!out.contains("for item in items.iter()"));
    }

    #[test]
    fn leaves_loop_intact_when_accumulator_is_mutated_twice() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![slice_param("items")],
                Some("i32"),
                vec![
                    let_mut("total", "i32", int(0)),
                    for_loop(vec![
                        stmt(Stmt::CompoundAssign {
                            target: var("total"),
                            op: BinOp::Add,
                            value: item_deref(),
                        }),
                        stmt(Stmt::CompoundAssign {
                            target: var("total"),
                            op: BinOp::Add,
                            value: int(1),
                        }),
                    ]),
                    Stmt::Return(Some(var("total"))),
                ],
            ))],
        };

        assert!(!fixup(&mut program));
        let out = program.emit();

        assert!(out.contains("for item in items.iter()"));
        assert!(!out.contains(".sum()"));
    }

    #[test]
    fn leaves_loop_intact_when_sum_init_is_not_zero() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![slice_param("items")],
                Some("i32"),
                vec![
                    let_mut("total", "i32", int(10)),
                    for_loop(vec![stmt(Stmt::CompoundAssign {
                        target: var("total"),
                        op: BinOp::Add,
                        value: item_deref(),
                    })]),
                    Stmt::Return(Some(var("total"))),
                ],
            ))],
        };

        assert!(!fixup(&mut program));
        let out = program.emit();

        assert!(out.contains("for item in items.iter()"));
        assert!(!out.contains(".sum()"));
    }
}
