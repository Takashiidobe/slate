//! Give proven non-escaping `_Atomic` locals native atomic storage: the
//! declaration becomes `let <name> = std::sync::atomic::AtomicN::new(<init>);`
//! and every `unsafe { AtomicN::from_ptr(addr_of_mut!(<name>)).op(..) }` view
//! becomes a safe direct method call on the local.

use std::collections::BTreeMap;

use crate::fixups::facts::{FixupFacts, atomic_locals};
use crate::fixups::support::walk;
use crate::rust_ast::{AtomicPlace, AtomicType, Expr, IndentStmt, Item, Program, Stmt};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut changed = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let promoted: BTreeMap<String, AtomicType> = facts
            .atomic_locals
            .iter()
            .filter(|fact| fact.function == function)
            .map(|fact| (fact.name.clone(), fact.ty))
            .collect();
        if promoted.is_empty() {
            continue;
        }
        rewrite_decls(&mut f.body, &promoted, &mut changed);
        walk::body_exprs_mut_with(&mut f.body, &mut |expr| {
            rewrite_atomic_expr(expr, &promoted, &mut changed);
            true
        });
    }
    changed
}

fn rewrite_decls(
    body: &mut [IndentStmt],
    promoted: &BTreeMap<String, AtomicType>,
    changed: &mut bool,
) {
    for indent in body.iter_mut() {
        match &mut indent.stmt {
            Stmt::Let {
                name,
                mutable: mutable @ true,
                ty,
                init: Some(init),
            } => {
                if let Some(atomic_ty) = promoted.get(name) {
                    *mutable = false;
                    *ty = None;
                    *init = Expr::AtomicNew {
                        ty: *atomic_ty,
                        value: Box::new(init.clone()),
                    };
                    *changed = true;
                }
            }
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
                rewrite_decls(then_body, promoted, changed);
                rewrite_decls(else_body, promoted, changed);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => rewrite_decls(body, promoted, changed),
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                rewrite_decls(&mut body.stmts, promoted, changed)
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    rewrite_decls(&mut arm.body, promoted, changed);
                }
            }
            _ => {}
        }
    }
}

/// `unsafe { <atomic op on promoted local> }` unwraps to the bare op with a
/// `Local` place; a bare op (already unwrapped) just switches its place.
fn rewrite_atomic_expr(
    expr: &mut Expr,
    promoted: &BTreeMap<String, AtomicType>,
    changed: &mut bool,
) {
    if let Expr::Unsafe(block) = expr
        && block.stmts.is_empty()
        && let Some(tail) = &mut block.tail
        && promote_place(tail, promoted)
    {
        *expr = std::mem::replace(tail.as_mut(), Expr::Todo(String::new()));
        *changed = true;
        return;
    }
    if promote_place(expr, promoted) {
        *changed = true;
    }
}

fn promote_place(expr: &mut Expr, promoted: &BTreeMap<String, AtomicType>) -> bool {
    let (place, ty) = match expr {
        Expr::AtomicLoad { place, ty, .. }
        | Expr::AtomicStore { place, ty, .. }
        | Expr::AtomicFetch { place, ty, .. }
        | Expr::AtomicSwap { place, ty, .. }
        | Expr::AtomicCompareExchange { place, ty, .. } => (place, *ty),
        _ => return false,
    };
    let Some((local, local_ty)) = atomic_locals::place_local_target(place, promoted) else {
        return false;
    };
    if local_ty != ty {
        return false;
    }
    *place = AtomicPlace::Local(local);
    true
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{AtomicOrdering, Block, Item, Stmt};

    fn apply(mut program: Program) -> String {
        let analyzed = facts::analyze(program.clone());
        fixup(&mut program, &analyzed.facts);
        program.emit()
    }

    fn program(stmts: Vec<Stmt>) -> Program {
        Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        }
    }

    fn addr_of_mut(name: &str) -> Expr {
        Expr::AddrOf {
            mutable: true,
            expr: Box::new(var(name)),
        }
    }

    fn unsafe_expr(expr: Expr) -> Expr {
        Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(expr)),
        }))
    }

    fn atomic_store(name: &str, value: Expr) -> Expr {
        Expr::AtomicStore {
            ty: AtomicType::I32,
            place: AtomicPlace::Ptr(Box::new(addr_of_mut(name))),
            value: Box::new(value),
            ordering: AtomicOrdering::SeqCst,
        }
    }

    fn atomic_load(name: &str) -> Expr {
        Expr::AtomicLoad {
            ty: AtomicType::I32,
            place: AtomicPlace::Ptr(Box::new(addr_of_mut(name))),
            ordering: AtomicOrdering::SeqCst,
        }
    }

    #[test]
    fn promotes_local_used_only_through_atomic_ops() {
        let out = apply(program(vec![
            let_mut("a", "i32", int(0)),
            Stmt::Expr(unsafe_expr(atomic_store("a", int(100)))),
            temp("loaded", "i32", unsafe_expr(atomic_load("a"))),
        ]));
        assert_eq!(
            out,
            "\
fn f() {
    let a = std::sync::atomic::AtomicI32::new(0);
    a.store(100, std::sync::atomic::Ordering::SeqCst);
    let loaded: i32 = a.load(std::sync::atomic::Ordering::SeqCst);
}
"
        );
    }

    #[test]
    fn plain_read_keeps_baseline_from_ptr_path() {
        let out = apply(program(vec![
            let_mut("a", "i32", int(0)),
            Stmt::Expr(unsafe_expr(atomic_store("a", int(100)))),
            temp("copy", "i32", var("a")),
        ]));
        assert!(out.contains("let mut a: i32 = 0;"));
        assert!(out.contains("unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).store(100, std::sync::atomic::Ordering::SeqCst) }"));
    }

    #[test]
    fn escaping_address_keeps_baseline_from_ptr_path() {
        let out = apply(program(vec![
            let_mut("a", "i32", int(0)),
            Stmt::Expr(unsafe_expr(atomic_store("a", int(1)))),
            Stmt::Expr(call("observe", vec![addr_of_mut("a")])),
        ]));
        assert!(out.contains("let mut a: i32 = 0;"));
        assert!(out.contains("from_ptr"));
    }

    #[test]
    fn width_mismatch_keeps_baseline_from_ptr_path() {
        let out = apply(program(vec![
            let_mut("a", "i64", int(0)),
            Stmt::Expr(unsafe_expr(atomic_store("a", int(1)))),
        ]));
        assert!(out.contains("let mut a: i64 = 0;"));
        assert!(out.contains("from_ptr"));
    }
}
