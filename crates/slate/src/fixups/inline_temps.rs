//! Inline single-use pure temps directly on the statement list. The temp's init
//! is spliced as an `Expr` subtree into its use site and precedence-aware
//! rendering elides redundant parens.

use crate::fixups::idents::stmt_ident_count;
use crate::rust_ast::{Block, Expr, IndentStmt, Stmt, UnaryOp};

pub(super) fn fixup(body: &mut Vec<IndentStmt>) {
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
            let Some(use_index) = single_safe_use(body, i, &name) else {
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
                else_body,
                ..
            } => {
                fixup(then_body);
                fixup(else_body);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                fixup(body);
            }
            Stmt::Unsafe { body } => fixup(&mut body.stmts),
            _ => {}
        }
    }
}

fn single_safe_use(body: &[IndentStmt], def_index: usize, name: &str) -> Option<usize> {
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
    match expr {
        Expr::Value(_) => true,
        Expr::Var(_) => true,
        Expr::Unary { op, expr } => !matches!(op, UnaryOp::Not) && is_pure_expr(expr),
        Expr::Binary { lhs, rhs, .. } => is_pure_expr(lhs) && is_pure_expr(rhs),
        _ => false,
    }
}

fn stmt_contains_call(stmt: &Stmt) -> bool {
    let mut found = false;
    walk_stmt_exprs(stmt, &mut |expr| {
        found |= matches!(
            expr,
            Expr::Call { .. }
                | Expr::MethodCall { .. }
                | Expr::MethodCallGeneric { .. }
                | Expr::Macro { .. }
        );
    });
    found
}

fn is_receiver_use(stmt: &Stmt, name: &str) -> bool {
    let mut found = false;
    walk_stmt_exprs(stmt, &mut |expr| {
        let receiver = match expr {
            Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => Some(&**recv),
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => Some(&**base),
            _ => None,
        };
        if let Some(Expr::Var(v)) = receiver {
            found |= v.as_str() == name;
        }
    });
    found
}

fn walk_stmt_exprs(stmt: &Stmt, f: &mut impl FnMut(&Expr)) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(expr) = init {
                walk_expr(expr, f);
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
            walk_expr(cond, f);
            for stmt in then_body.iter().chain(else_body) {
                walk_stmt_exprs(&stmt.stmt, f);
            }
            walk_expr(then_value, f);
            walk_expr(else_value, f);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            walk_expr(target, f);
            walk_expr(value, f);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => walk_expr(expr, f),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            walk_expr(cond, f);
            for stmt in then_body.iter().chain(else_body) {
                walk_stmt_exprs(&stmt.stmt, f);
            }
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            for stmt in body {
                walk_stmt_exprs(&stmt.stmt, f);
            }
        }
        Stmt::Unsafe { body } => walk_block(body, f),
        Stmt::Match { expr, arms } => {
            walk_expr(expr, f);
            for arm in arms {
                for stmt in &arm.body {
                    walk_stmt_exprs(&stmt.stmt, f);
                }
            }
        }
        Stmt::While { cond, body } => {
            walk_expr(cond, f);
            walk_block(body, f);
        }
        Stmt::Block(body) => walk_block(body, f),
    }
}

fn walk_block(block: &Block, f: &mut impl FnMut(&Expr)) {
    for stmt in &block.stmts {
        walk_stmt_exprs(&stmt.stmt, f);
    }
    if let Some(tail) = &block.tail {
        walk_expr(tail, f);
    }
}

fn walk_expr(expr: &Expr, f: &mut impl FnMut(&Expr)) {
    f(expr);
    match expr {
        Expr::Value(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => {}
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => walk_expr(expr, f),
        Expr::Binary { lhs, rhs, .. } => {
            walk_expr(lhs, f);
            walk_expr(rhs, f);
        }
        Expr::Call { func, args } => {
            walk_expr(func, f);
            for arg in args {
                walk_expr(arg, f);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            walk_expr(recv, f);
            for arg in args {
                walk_expr(arg, f);
            }
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => walk_expr(base, f),
        Expr::ArrayPtr { array, .. } => walk_expr(array, f),
        Expr::Index { base, index } => {
            walk_expr(base, f);
            walk_expr(index, f);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                walk_expr(value, f);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                walk_expr(elem, f);
            }
        }
        Expr::ArrayRepeat { elem, .. } => walk_expr(elem, f),
        Expr::Macro { args, .. } => {
            for arg in args {
                walk_expr(arg, f);
            }
        }
        Expr::Closure { body, .. } => walk_expr(body, f),
        Expr::Match { expr, arms } => {
            walk_expr(expr, f);
            for arm in arms {
                walk_expr(&arm.value, f);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            walk_expr(cond, f);
            walk_expr(then_expr, f);
            walk_expr(else_expr, f);
        }
        Expr::Block(block) | Expr::Unsafe(block) => walk_block(block, f),
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => walk_expr(ptr, f),
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            walk_expr(ptr, f);
            walk_expr(value, f);
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            walk_expr(ptr, f);
            walk_expr(expected, f);
            walk_expr(desired, f);
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            walk_expr(src, f);
            walk_expr(dst, f);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            walk_expr(src, f);
            walk_expr(dst, f);
            walk_expr(count, f);
        }
        Expr::WriteBytes { dst, val, count } => {
            walk_expr(dst, f);
            walk_expr(val, f);
            walk_expr(count, f);
        }
    }
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
