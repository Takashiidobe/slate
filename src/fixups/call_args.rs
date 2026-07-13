//! Inline single-use argument temps into type-safe call positions.
//!
//! Baseline lowering materializes every call argument as its own `let` temp:
//! `let _v1: i32 = 2; let _v3 = add(_v1, _v2); printf(fmt, _v3)`. The pure-temp
//! inliner ([`super::inline_temps`]) deliberately refuses call-argument slots,
//! because dropping a literal's type annotation into a vararg slot would change
//! its inferred type — `printf(_v0)` with `_v0: i64 = 9223372036854775807` must
//! not become `printf(9223372036854775807)`, where the literal defaults to `i32`.
//!
//! This pass inlines exactly the two positions where the argument's Rust type is
//! pinned by something other than the temp's annotation:
//!
//! - **(a)** a pure temp whose single use is a *declared* parameter slot of a
//!   non-variadic callee — the parameter type pins any literal.
//! - **(b)** a temp initialized by a call to a known function — its Rust type is
//!   fixed by the callee's return type, so inlining is safe even in a vararg
//!   slot; the between-statements guard keeps its side effect from being
//!   reordered.

use std::collections::HashMap;

use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, ExternDecl, IndentStmt, Item, Program, Stmt, Type};

pub(super) struct Signature {
    params: Vec<Type>,
    variadic: bool,
}

pub(super) type Signatures = HashMap<String, Signature>;

pub(super) fn collect_signatures(program: &Program) -> Signatures {
    let mut sigs = Signatures::new();
    let mut record = |name: &str, params: Vec<Type>, variadic: bool| {
        sigs.insert(name.to_string(), Signature { params, variadic });
    };
    for item in &program.items {
        match item {
            Item::Fn(f) => record(
                &f.name,
                f.params.iter().map(|p| p.ty.clone()).collect(),
                false,
            ),
            Item::Func(f) => record(
                &f.name,
                f.params.iter().map(|p| p.ty.clone()).collect(),
                false,
            ),
            Item::ExternBlock { decls, .. } => {
                for decl in decls {
                    if let ExternDecl::Fn(d) = decl {
                        record(
                            &d.name,
                            d.params.iter().map(|p| p.ty.clone()).collect(),
                            d.variadic,
                        );
                    }
                }
            }
            _ => {}
        }
    }
    sigs
}

pub(super) fn fixup(body: &mut Vec<IndentStmt>, sigs: &Signatures) {
    fixup_nested(body, sigs);
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
            if !is_temp_name(name) {
                continue;
            }
            let name = name.clone();
            let init = init.clone();
            let Some((use_index, callee, slot)) = single_arg_use(body, i, &name) else {
                continue;
            };
            if !inlinable(&init, sigs, sigs.get(&callee), slot) {
                continue;
            }
            if body[use_index].stmt.substitute_var(&name, &init) {
                body.remove(i);
                applied = true;
                break;
            }
        }
        if !applied {
            break;
        }
        fixup_nested(body, sigs);
    }
}

fn fixup_nested(body: &mut [IndentStmt], sigs: &Signatures) {
    for stmt in body {
        match &mut stmt.stmt {
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
                fixup(then_body, sigs);
                fixup(else_body, sigs);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                fixup(body, sigs);
            }
            Stmt::Unsafe { body } => fixup(&mut body.stmts, sigs),
            _ => {}
        }
    }
}

/// The single use of `name` after `def_index`, as `(use_index, callee, slot)`,
/// when that use is a top-level argument of a `name`-free call to a plain-ident
/// callee and every statement in between is a pure temp-let (so the definition
/// can move to the use site without crossing a side effect).
fn single_arg_use(
    body: &[IndentStmt],
    def_index: usize,
    name: &str,
) -> Option<(usize, String, usize)> {
    let mut target: Option<(usize, String, usize)> = None;
    for (index, stmt) in body.iter().enumerate().skip(def_index + 1) {
        let uses = stmt_ident_count(&stmt.stmt, name);
        if uses == 0 {
            if target.is_none() && !is_pure_temp_let(&stmt.stmt) {
                return None;
            }
            continue;
        }
        if uses != 1 || target.is_some() {
            return None;
        }
        let (callee, slot) = find_arg_slot(&stmt.stmt, name)?;
        target = Some((index, callee, slot));
    }
    target
}

fn find_arg_slot(stmt: &Stmt, name: &str) -> Option<(String, usize)> {
    let mut result = None;
    walk::stmt_exprs(stmt, &mut |expr| {
        if result.is_some() {
            return;
        }
        if let Expr::Call { func, args } = expr
            && let Expr::Var(callee) = &**func
            && let Some(slot) = args
                .iter()
                .position(|arg| matches!(arg, Expr::Var(v) if v.as_str() == name))
        {
            result = Some((callee.as_str().to_string(), slot));
        }
    });
    result
}

fn inlinable(init: &Expr, sigs: &Signatures, callee: Option<&Signature>, slot: usize) -> bool {
    // (b) result of a known call: its Rust type is fixed by the callee's return
    // type, so it needs no annotation even in a vararg slot.
    if let Expr::Call { func, .. } = init
        && let Expr::Var(inner) = &**func
        && sigs.contains_key(inner.as_str())
    {
        return true;
    }
    // (a) pure temp into a declared, non-variadic parameter slot: the parameter
    // type pins any literal, so dropping the annotation cannot change inference.
    match callee {
        Some(sig) if !sig.variadic && slot < sig.params.len() => is_pure_expr(init),
        _ => false,
    }
}

fn is_pure_temp_let(stmt: &Stmt) -> bool {
    matches!(
        stmt,
        Stmt::Let { name, init: Some(init), .. } if is_temp_name(name) && is_pure_expr(init)
    )
}

fn is_pure_expr(expr: &Expr) -> bool {
    super::effects::is_movable_pure_expr(expr)
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::Stmt;

    fn sig(params: &[&str], variadic: bool) -> Signature {
        Signature {
            params: params.iter().map(|t| Type::parse(t)).collect(),
            variadic,
        }
    }

    fn run(sigs: Vec<(&str, Signature)>, stmts: Vec<Stmt>) -> String {
        let sigmap: Signatures = sigs.into_iter().map(|(n, s)| (n.to_string(), s)).collect();
        let mut f = func(vec![], None, stmts);
        fixup(&mut f.body, &sigmap);
        emit(f)
    }

    #[test]
    fn inlines_literals_into_params_and_call_result_into_vararg() {
        let out = run(
            vec![
                ("add", sig(&["i32", "i32"], false)),
                ("printf", sig(&["*mut i8"], true)),
            ],
            vec![
                temp("_v1", "i32", int(2)),
                temp("_v2", "i32", int(3)),
                temp("_v3", "i32", call("add", vec![var("_v1"), var("_v2")])),
                Stmt::Expr(call("printf", vec![var("fmt"), var("_v3")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    printf(fmt, add(2, 3));
}
"
        );
    }

    #[test]
    fn does_not_inline_bare_literal_into_vararg_slot() {
        let out = run(
            vec![("printf", sig(&["*mut i8"], true))],
            vec![
                temp("_v0", "i64", int(9223372036854775807)),
                Stmt::Expr(call("printf", vec![var("fmt"), var("_v0")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i64 = 9223372036854775807;
    printf(fmt, _v0);
}
"
        );
    }

    #[test]
    fn does_not_inline_into_unknown_callee() {
        let out = run(
            vec![],
            vec![
                temp("_v0", "i32", int(5)),
                Stmt::Expr(call("mystery", vec![var("_v0")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = 5;
    mystery(_v0);
}
"
        );
    }

    #[test]
    fn does_not_reorder_call_result_across_side_effect() {
        let out = run(
            vec![("g", sig(&[], false)), ("printf", sig(&["*mut i8"], true))],
            vec![
                temp("_v0", "i32", call("g", vec![])),
                Stmt::Expr(call("side_effect", vec![])),
                Stmt::Expr(call("printf", vec![var("fmt"), var("_v0")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = g();
    side_effect();
    printf(fmt, _v0);
}
"
        );
    }
}
