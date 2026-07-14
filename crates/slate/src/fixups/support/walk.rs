use crate::fixups::facts::PathSegment;
use crate::rust_ast::{Block, Expr, IndentStmt, Stmt};

pub(in crate::fixups) fn with_path_segment<R>(
    path: &mut Vec<PathSegment>,
    segment: PathSegment,
    f: impl FnOnce(&mut Vec<PathSegment>) -> R,
) -> R {
    path.push(segment);
    let out = f(path);
    path.pop();
    out
}

pub(in crate::fixups) fn nested_bodies_with_path(
    stmt: &Stmt,
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&[IndentStmt], &mut Vec<PathSegment>),
) {
    match stmt {
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
            with_path_segment(path, PathSegment::Then, |path| f(then_body, path));
            with_path_segment(path, PathSegment::Else, |path| f(else_body, path));
        }
        Stmt::Loop { body, .. } => {
            with_path_segment(path, PathSegment::LoopBody, |path| f(body, path));
        }
        Stmt::Scope { body } => {
            with_path_segment(path, PathSegment::ScopeBody, |path| f(body, path));
        }
        Stmt::LabeledBlock { body, .. } => {
            with_path_segment(path, PathSegment::LabeledBody, |path| f(body, path));
        }
        Stmt::Unsafe { body } => {
            with_path_segment(path, PathSegment::UnsafeBody, |path| f(&body.stmts, path));
        }
        Stmt::While { body, .. } => {
            with_path_segment(path, PathSegment::WhileBody, |path| f(&body.stmts, path));
        }
        Stmt::Block(body) => {
            with_path_segment(path, PathSegment::BlockBody, |path| f(&body.stmts, path));
        }
        Stmt::Match { arms, .. } => {
            for (index, arm) in arms.iter().enumerate() {
                with_path_segment(path, PathSegment::MatchArm(index), |path| {
                    f(&arm.body, path)
                });
            }
        }
        Stmt::Let { .. }
        | Stmt::Assign { .. }
        | Stmt::CompoundAssign { .. }
        | Stmt::Expr(_)
        | Stmt::Return(_)
        | Stmt::Break(_)
        | Stmt::Continue(_) => {}
    }
}

pub(in crate::fixups) fn nested_bodies_mut_with_path(
    stmt: &mut Stmt,
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&mut [IndentStmt], &mut Vec<PathSegment>),
) {
    match stmt {
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
            with_path_segment(path, PathSegment::Then, |path| f(then_body, path));
            with_path_segment(path, PathSegment::Else, |path| f(else_body, path));
        }
        Stmt::Loop { body, .. } => {
            with_path_segment(path, PathSegment::LoopBody, |path| f(body, path));
        }
        Stmt::Scope { body } => {
            with_path_segment(path, PathSegment::ScopeBody, |path| f(body, path));
        }
        Stmt::LabeledBlock { body, .. } => {
            with_path_segment(path, PathSegment::LabeledBody, |path| f(body, path));
        }
        Stmt::Unsafe { body } => {
            with_path_segment(path, PathSegment::UnsafeBody, |path| {
                f(&mut body.stmts, path)
            });
        }
        Stmt::While { body, .. } => {
            with_path_segment(path, PathSegment::WhileBody, |path| {
                f(&mut body.stmts, path)
            });
        }
        Stmt::Block(body) => {
            with_path_segment(path, PathSegment::BlockBody, |path| {
                f(&mut body.stmts, path)
            });
        }
        Stmt::Match { arms, .. } => {
            for (index, arm) in arms.iter_mut().enumerate() {
                with_path_segment(path, PathSegment::MatchArm(index), |path| {
                    f(&mut arm.body, path)
                });
            }
        }
        Stmt::Let { .. }
        | Stmt::Assign { .. }
        | Stmt::CompoundAssign { .. }
        | Stmt::Expr(_)
        | Stmt::Return(_)
        | Stmt::Break(_)
        | Stmt::Continue(_) => {}
    }
}

pub(in crate::fixups) fn body_exprs_mut_with(
    body: &mut [IndentStmt],
    f: &mut impl FnMut(&mut Expr) -> bool,
) {
    for stmt in body {
        stmt_exprs_mut_with(&mut stmt.stmt, f);
    }
}

pub(in crate::fixups) fn block_exprs_mut_with(
    block: &mut Block,
    f: &mut impl FnMut(&mut Expr) -> bool,
) {
    body_exprs_mut_with(&mut block.stmts, f);
    if let Some(tail) = &mut block.tail {
        exprs_mut_with(tail, f);
    }
}

pub(in crate::fixups) fn stmt_exprs_mut_with(
    stmt: &mut Stmt,
    f: &mut impl FnMut(&mut Expr) -> bool,
) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(expr) = init {
                exprs_mut_with(expr, f);
            }
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            exprs_mut_with(cond, f);
            body_exprs_mut_with(then_body, f);
            exprs_mut_with(then_value, f);
            body_exprs_mut_with(else_body, f);
            exprs_mut_with(else_value, f);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            exprs_mut_with(target, f);
            exprs_mut_with(value, f);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => exprs_mut_with(expr, f),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            exprs_mut_with(cond, f);
            body_exprs_mut_with(then_body, f);
            body_exprs_mut_with(else_body, f);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_exprs_mut_with(body, f);
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_exprs_mut_with(body, f);
        }
        Stmt::Match { expr, arms } => {
            exprs_mut_with(expr, f);
            for arm in arms {
                body_exprs_mut_with(&mut arm.body, f);
            }
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

pub(in crate::fixups) fn exprs_mut_with(expr: &mut Expr, f: &mut impl FnMut(&mut Expr) -> bool) {
    if !f(expr) {
        return;
    }
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => {}
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => exprs_mut_with(expr, f),
        Expr::Binary { lhs, rhs, .. } => {
            exprs_mut_with(lhs, f);
            exprs_mut_with(rhs, f);
        }
        Expr::Call { func, args } => {
            exprs_mut_with(func, f);
            for arg in args {
                exprs_mut_with(arg, f);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            exprs_mut_with(recv, f);
            for arg in args {
                exprs_mut_with(arg, f);
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => exprs_mut_with(base, f),
        Expr::Index { base, index } => {
            exprs_mut_with(base, f);
            exprs_mut_with(index, f);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                exprs_mut_with(value, f);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                exprs_mut_with(elem, f);
            }
        }
        Expr::ArrayRepeat { elem, .. } => exprs_mut_with(elem, f),
        Expr::Macro { args, .. } => {
            for arg in args {
                exprs_mut_with(arg, f);
            }
        }
        Expr::Closure { body, .. } => exprs_mut_with(body, f),
        Expr::Match { expr, arms } => {
            exprs_mut_with(expr, f);
            for arm in arms {
                exprs_mut_with(&mut arm.value, f);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            exprs_mut_with(cond, f);
            exprs_mut_with(then_expr, f);
            exprs_mut_with(else_expr, f);
        }
        Expr::Block(block) | Expr::Unsafe(block) => block_exprs_mut_with(block, f),
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => exprs_mut_with(ptr, f),
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            exprs_mut_with(ptr, f);
            exprs_mut_with(value, f);
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            exprs_mut_with(ptr, f);
            exprs_mut_with(expected, f);
            exprs_mut_with(desired, f);
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            exprs_mut_with(src, f);
            exprs_mut_with(dst, f);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            exprs_mut_with(src, f);
            exprs_mut_with(dst, f);
            exprs_mut_with(count, f);
        }
        Expr::WriteBytes { dst, val, count } => {
            exprs_mut_with(dst, f);
            exprs_mut_with(val, f);
            exprs_mut_with(count, f);
        }
    }
}

pub(in crate::fixups) fn body_exprs_mut_with_path(
    body: &mut [IndentStmt],
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&mut Expr, &mut Vec<PathSegment>) -> bool,
) {
    for (index, indent) in body.iter_mut().enumerate() {
        with_path_segment(path, PathSegment::Stmt(index), |path| {
            stmt_exprs_mut_with_path(&mut indent.stmt, path, f);
        });
    }
}

pub(in crate::fixups) fn block_exprs_mut_with_path(
    block: &mut Block,
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&mut Expr, &mut Vec<PathSegment>) -> bool,
) {
    body_exprs_mut_with_path(&mut block.stmts, path, f);
    if let Some(tail) = &mut block.tail {
        with_path_segment(path, PathSegment::BlockTail, |path| {
            exprs_mut_with_path(tail, path, f);
        });
    }
}

pub(in crate::fixups) fn stmt_exprs_mut_with_path(
    stmt: &mut Stmt,
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&mut Expr, &mut Vec<PathSegment>) -> bool,
) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(expr) = init {
                stmt_root_expr_mut_with_path(expr, 0, path, f);
            }
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            stmt_root_expr_mut_with_path(cond, 0, path, f);
            with_path_segment(path, PathSegment::Then, |path| {
                body_exprs_mut_with_path(then_body, path, f);
                stmt_root_expr_mut_with_path(then_value, 0, path, f);
            });
            with_path_segment(path, PathSegment::Else, |path| {
                body_exprs_mut_with_path(else_body, path, f);
                stmt_root_expr_mut_with_path(else_value, 0, path, f);
            });
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            stmt_root_expr_mut_with_path(target, 0, path, f);
            stmt_root_expr_mut_with_path(value, 1, path, f);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            stmt_root_expr_mut_with_path(expr, 0, path, f);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            stmt_root_expr_mut_with_path(cond, 0, path, f);
            with_path_segment(path, PathSegment::Then, |path| {
                body_exprs_mut_with_path(then_body, path, f);
            });
            with_path_segment(path, PathSegment::Else, |path| {
                body_exprs_mut_with_path(else_body, path, f);
            });
        }
        Stmt::Loop { body, .. } => {
            with_path_segment(path, PathSegment::LoopBody, |path| {
                body_exprs_mut_with_path(body, path, f);
            });
        }
        Stmt::Scope { body } => {
            with_path_segment(path, PathSegment::ScopeBody, |path| {
                body_exprs_mut_with_path(body, path, f);
            });
        }
        Stmt::LabeledBlock { body, .. } => {
            with_path_segment(path, PathSegment::LabeledBody, |path| {
                body_exprs_mut_with_path(body, path, f);
            });
        }
        Stmt::Unsafe { body } => {
            with_path_segment(path, PathSegment::UnsafeBody, |path| {
                block_exprs_mut_with_path(body, path, f);
            });
        }
        Stmt::While { cond, body } => {
            stmt_root_expr_mut_with_path(cond, 0, path, f);
            with_path_segment(path, PathSegment::WhileBody, |path| {
                block_exprs_mut_with_path(body, path, f);
            });
        }
        Stmt::Block(body) => {
            with_path_segment(path, PathSegment::BlockBody, |path| {
                block_exprs_mut_with_path(body, path, f);
            });
        }
        Stmt::Match { expr, arms } => {
            stmt_root_expr_mut_with_path(expr, 0, path, f);
            for (index, arm) in arms.iter_mut().enumerate() {
                with_path_segment(path, PathSegment::MatchArm(index), |path| {
                    body_exprs_mut_with_path(&mut arm.body, path, f);
                });
            }
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

fn stmt_root_expr_mut_with_path(
    expr: &mut Expr,
    index: usize,
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&mut Expr, &mut Vec<PathSegment>) -> bool,
) {
    with_path_segment(path, PathSegment::Expr(index), |path| {
        exprs_mut_with_path(expr, path, f);
    });
}

pub(in crate::fixups) fn exprs_mut_with_path(
    expr: &mut Expr,
    path: &mut Vec<PathSegment>,
    f: &mut impl FnMut(&mut Expr, &mut Vec<PathSegment>) -> bool,
) {
    if !f(expr, path) {
        return;
    }
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => {}
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(expr, path, f);
            });
        }
        Expr::Binary { lhs, rhs, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(lhs, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_mut_with_path(rhs, path, f);
            });
        }
        Expr::Call { func, args } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(func, path, f);
            });
            for (index, arg) in args.iter_mut().enumerate() {
                with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    exprs_mut_with_path(arg, path, f);
                });
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(recv, path, f);
            });
            for (index, arg) in args.iter_mut().enumerate() {
                with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    exprs_mut_with_path(arg, path, f);
                });
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(base, path, f);
            });
        }
        Expr::Index { base, index } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(base, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_mut_with_path(index, path, f);
            });
        }
        Expr::StructLit { fields, .. } => {
            for (index, (_, value)) in fields.iter_mut().enumerate() {
                with_path_segment(path, PathSegment::Expr(index), |path| {
                    exprs_mut_with_path(value, path, f);
                });
            }
        }
        Expr::ArrayLit(elems) => {
            for (index, elem) in elems.iter_mut().enumerate() {
                with_path_segment(path, PathSegment::Expr(index), |path| {
                    exprs_mut_with_path(elem, path, f);
                });
            }
        }
        Expr::ArrayRepeat { elem, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(elem, path, f);
            });
        }
        Expr::Macro { args, .. } => {
            for (index, arg) in args.iter_mut().enumerate() {
                with_path_segment(path, PathSegment::Expr(index), |path| {
                    exprs_mut_with_path(arg, path, f);
                });
            }
        }
        Expr::Closure { body, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(body, path, f);
            });
        }
        Expr::Match { expr, arms } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(expr, path, f);
            });
            for (index, arm) in arms.iter_mut().enumerate() {
                with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    exprs_mut_with_path(&mut arm.value, path, f);
                });
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(cond, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_mut_with_path(then_expr, path, f);
            });
            with_path_segment(path, PathSegment::Expr(2), |path| {
                exprs_mut_with_path(else_expr, path, f);
            });
        }
        Expr::Block(block) | Expr::Unsafe(block) => {
            with_path_segment(path, PathSegment::BlockBody, |path| {
                block_exprs_mut_with_path(block, path, f);
            });
        }
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(ptr, path, f);
            });
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(ptr, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_mut_with_path(value, path, f);
            });
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(ptr, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_mut_with_path(expected, path, f);
            });
            with_path_segment(path, PathSegment::Expr(2), |path| {
                exprs_mut_with_path(desired, path, f);
            });
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(src, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_mut_with_path(dst, path, f);
            });
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(src, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_mut_with_path(dst, path, f);
            });
            with_path_segment(path, PathSegment::Expr(2), |path| {
                exprs_mut_with_path(count, path, f);
            });
        }
        Expr::WriteBytes { dst, val, count } => {
            with_path_segment(path, PathSegment::Expr(0), |path| {
                exprs_mut_with_path(dst, path, f);
            });
            with_path_segment(path, PathSegment::Expr(1), |path| {
                exprs_mut_with_path(val, path, f);
            });
            with_path_segment(path, PathSegment::Expr(2), |path| {
                exprs_mut_with_path(count, path, f);
            });
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, Expr, IndentStmt, MatchArm, Pattern, Stmt};

    fn marker(name: &str) -> IndentStmt {
        IndentStmt {
            depth: 0,
            stmt: Stmt::Expr(var(name)),
        }
    }

    fn child_paths(stmt: &Stmt) -> Vec<Vec<PathSegment>> {
        let mut out = Vec::new();
        nested_bodies_with_path(stmt, &mut Vec::new(), &mut |body, path| {
            if !body.is_empty() {
                out.push(path.clone());
            }
        });
        out
    }

    #[test]
    fn immutable_walker_visits_branch_body_paths() {
        assert_eq!(
            child_paths(&Stmt::If {
                cond: var("c"),
                then_body: vec![marker("t")],
                else_body: vec![marker("e")],
            }),
            vec![vec![PathSegment::Then], vec![PathSegment::Else]]
        );
        assert_eq!(
            child_paths(&Stmt::LetIf {
                name: "x".into(),
                mutable: false,
                ty: None,
                cond: var("c"),
                then_body: vec![marker("t")],
                then_value: int(1),
                else_body: vec![marker("e")],
                else_value: int(2),
            }),
            vec![vec![PathSegment::Then], vec![PathSegment::Else]]
        );
    }

    #[test]
    fn immutable_walker_visits_structural_body_paths() {
        let cases = vec![
            (
                Stmt::Loop {
                    label: None,
                    body: vec![marker("x")],
                },
                PathSegment::LoopBody,
            ),
            (
                Stmt::Scope {
                    body: vec![marker("x")],
                },
                PathSegment::ScopeBody,
            ),
            (
                Stmt::LabeledBlock {
                    label: "label".into(),
                    body: vec![marker("x")],
                },
                PathSegment::LabeledBody,
            ),
            (
                Stmt::Unsafe {
                    body: Block {
                        stmts: vec![marker("x")],
                        tail: None,
                    },
                },
                PathSegment::UnsafeBody,
            ),
            (
                Stmt::While {
                    cond: var("c"),
                    body: Block {
                        stmts: vec![marker("x")],
                        tail: None,
                    },
                },
                PathSegment::WhileBody,
            ),
            (
                Stmt::Block(Block {
                    stmts: vec![marker("x")],
                    tail: None,
                }),
                PathSegment::BlockBody,
            ),
        ];

        for (stmt, segment) in cases {
            assert_eq!(child_paths(&stmt), vec![vec![segment]]);
        }
    }

    #[test]
    fn immutable_walker_visits_match_arm_paths() {
        assert_eq!(
            child_paths(&Stmt::Match {
                expr: var("x"),
                arms: vec![
                    MatchArm {
                        pattern: Pattern::I64(0),
                        body: vec![marker("a")],
                    },
                    MatchArm {
                        pattern: Pattern::Wildcard,
                        body: vec![marker("b")],
                    },
                ],
            }),
            vec![
                vec![PathSegment::MatchArm(0)],
                vec![PathSegment::MatchArm(1)]
            ]
        );
    }

    #[test]
    fn mutable_walker_can_rewrite_nested_bodies() {
        let mut stmt = Stmt::If {
            cond: var("c"),
            then_body: vec![marker("t")],
            else_body: vec![marker("e")],
        };
        nested_bodies_mut_with_path(&mut stmt, &mut Vec::new(), &mut |body, path| {
            body[0].stmt = Stmt::Expr(Expr::Var(format!("p{}", path.len()).into()));
        });

        let Stmt::If {
            then_body,
            else_body,
            ..
        } = stmt
        else {
            panic!("expected if");
        };
        assert!(matches!(
            &then_body[0].stmt,
            Stmt::Expr(Expr::Var(name)) if name.as_str() == "p1"
        ));
        assert!(matches!(
            &else_body[0].stmt,
            Stmt::Expr(Expr::Var(name)) if name.as_str() == "p1"
        ));
    }
}
