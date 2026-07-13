//! Drop the binding of a synthetic temp that only holds an unused call result.
//!
//! Baseline lowering spills every call into its own `let` temp, even when the C
//! source discards the result: `let _v6 = unsafe { printf(...) };`. When such a
//! temp is never read, the binding is pure noise — the call still has to run for
//! its side effects, so the `let _v = <call>;` becomes a bare `<call>;`.
//!
//! Only temps whose initializer performs a call are touched (the side effect is
//! preserved), and only when the temp is unused in its entire lexical scope, so
//! a used call result is left materialized.

use crate::fixups::idents::{expr_ident_count, stmt_ident_count};
use crate::fixups::support::walk;
use crate::rust_ast::{Block, Expr, IndentStmt, Stmt};

pub(super) fn fixup(body: &mut Vec<IndentStmt>) {
    scope(body, None);
}

/// Process one lexical scope: `stmts` plus the optional trailing value `tail`
/// that the enclosing block evaluates to (so a temp read only by the tail counts
/// as used).
fn scope(stmts: &mut Vec<IndentStmt>, tail: Option<&Expr>) {
    for stmt in stmts.iter_mut() {
        recurse(&mut stmt.stmt);
    }
    for i in 0..stmts.len() {
        let name = match &stmts[i].stmt {
            Stmt::Let {
                name,
                mutable: false,
                init: Some(_),
                ..
            } if is_temp_name(name) => name.clone(),
            _ => continue,
        };
        if !init_has_call(&stmts[i].stmt) {
            continue;
        }
        let uses: usize = stmts
            .iter()
            .skip(i + 1)
            .map(|stmt| stmt_ident_count(&stmt.stmt, &name))
            .sum::<usize>()
            + tail.map_or(0, |tail| expr_ident_count(tail, &name));
        if uses != 0 {
            continue;
        }
        let init = match &mut stmts[i].stmt {
            Stmt::Let { init, .. } => init.take(),
            _ => None,
        };
        if let Some(init) = init {
            stmts[i].stmt = Stmt::Expr(init);
        }
    }
}

fn recurse(stmt: &mut Stmt) {
    match stmt {
        Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            scope(then_body, None);
            scope(else_body, None);
        }
        Stmt::LetIf {
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            scope(then_body, Some(then_value));
            scope(else_body, Some(else_value));
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            scope(body, None)
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => scope_block(body),
        Stmt::Match { arms, .. } => {
            for arm in arms {
                scope(&mut arm.body, None);
            }
        }
        _ => {}
    }
}

fn scope_block(block: &mut Block) {
    scope(&mut block.stmts, block.tail.as_deref());
}

fn init_has_call(stmt: &Stmt) -> bool {
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

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::Type;

    fn dropped(stmts: Vec<Stmt>) -> String {
        let mut f = func(vec![], None, stmts);
        fixup(&mut f.body);
        emit(f)
    }

    #[test]
    fn drops_unused_call_result_binding() {
        let out = dropped(vec![
            temp("_v0", "i32", var("a")),
            temp("_v1", "i32", call("printf", vec![var("fmt"), var("_v0")])),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = a;
    printf(fmt, _v0);
}
"
        );
    }

    #[test]
    fn preserves_unsafe_block_wrapper() {
        let out = dropped(vec![Stmt::Let {
            name: "_v1".into(),
            mutable: false,
            ty: Some(Type::parse("i32")),
            init: Some(Expr::Unsafe(Box::new(Block {
                stmts: vec![],
                tail: Some(Box::new(call("printf", vec![var("fmt")]))),
            }))),
        }]);

        assert_eq!(
            out,
            "\
fn f() {
    unsafe { printf(fmt) };
}
"
        );
    }

    #[test]
    fn keeps_used_call_result() {
        let out = dropped(vec![
            temp("_v0", "i32", call("getchar", vec![])),
            assign("b", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = getchar();
    b = _v0;
}
"
        );
    }

    #[test]
    fn leaves_pure_unused_temp_alone() {
        let out = dropped(vec![temp("_v0", "i32", int(5))]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = 5;
}
"
        );
    }
}
