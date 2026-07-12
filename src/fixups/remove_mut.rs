//! Remove `mut` from bindings whose later use does not require mutation.

use std::collections::BTreeSet;

use crate::fixups::inline_temps::walk_stmt_exprs;
use crate::rust_ast::{Block, Expr, FnDef, IndentStmt, Stmt};

pub(super) fn fixup(f: &mut FnDef) {
    let mut required = BTreeSet::new();
    collect_required_mut(&f.body, &mut required);
    for param in &mut f.params {
        if !required.contains(&param.name) {
            param.mutable = false;
        }
    }
    remove_unneeded_mut(&mut f.body, &required);
}

fn collect_required_mut(body: &[IndentStmt], required: &mut BTreeSet<String>) {
    for stmt in body {
        collect_required_stmt(&stmt.stmt, required);
    }
}

fn collect_required_stmt(stmt: &Stmt, required: &mut BTreeSet<String>) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(init) = init {
                collect_expr_hazards(init, required);
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
            collect_expr_hazards(cond, required);
            collect_required_mut(then_body, required);
            collect_expr_hazards(then_value, required);
            collect_required_mut(else_body, required);
            collect_expr_hazards(else_value, required);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            collect_vars(target, required);
            collect_expr_hazards(value, required);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => collect_expr_hazards(expr, required),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            collect_expr_hazards(cond, required);
            collect_required_mut(then_body, required);
            collect_required_mut(else_body, required);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            collect_required_mut(body, required);
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            collect_block_required_mut(body, required);
        }
        Stmt::Match { expr, arms } => {
            collect_expr_hazards(expr, required);
            for arm in arms {
                collect_required_mut(&arm.body, required);
            }
        }
    }
}

fn collect_block_required_mut(block: &Block, required: &mut BTreeSet<String>) {
    collect_required_mut(&block.stmts, required);
    if let Some(tail) = &block.tail {
        collect_expr_hazards(tail, required);
    }
}

fn collect_expr_hazards(expr: &Expr, required: &mut BTreeSet<String>) {
    walk_stmt_exprs(&Stmt::Expr(expr.clone()), &mut |expr| match expr {
        Expr::AddrOf { expr, .. } => collect_vars(expr, required),
        Expr::Ref {
            mutable: true,
            expr,
        } => collect_vars(expr, required),
        Expr::ArrayPtr {
            mutable: true,
            array,
        } => collect_vars(array, required),
        Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => {
            collect_vars(recv, required)
        }
        Expr::AtomicRef { ptr, .. }
        | Expr::AtomicLoad { ptr, .. }
        | Expr::AtomicStore { ptr, .. }
        | Expr::AtomicFetch { ptr, .. }
        | Expr::AtomicSwap { ptr, .. }
        | Expr::AtomicCompareExchange { ptr, .. } => collect_vars(ptr, required),
        _ => {}
    });
}

fn collect_vars(expr: &Expr, vars: &mut BTreeSet<String>) {
    walk_stmt_exprs(&Stmt::Expr(expr.clone()), &mut |expr| {
        if let Expr::Var(name) = expr {
            vars.insert(name.as_str().to_string());
        }
    });
}

fn remove_unneeded_mut(body: &mut [IndentStmt], required: &BTreeSet<String>) {
    for indent in body {
        match &mut indent.stmt {
            Stmt::Let { name, mutable, .. } => {
                if !required.contains(name) {
                    *mutable = false;
                }
            }
            Stmt::LetIf {
                name,
                mutable,
                then_body,
                else_body,
                ..
            } => {
                if !required.contains(name) {
                    *mutable = false;
                }
                remove_unneeded_mut(then_body, required);
                remove_unneeded_mut(else_body, required);
            }
            Stmt::If {
                then_body,
                else_body,
                ..
            } => {
                remove_unneeded_mut(then_body, required);
                remove_unneeded_mut(else_body, required);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                remove_unneeded_mut(body, required);
            }
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                remove_block_unneeded_mut(body, required);
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    remove_unneeded_mut(&mut arm.body, required);
                }
            }
            _ => {}
        }
    }
}

fn remove_block_unneeded_mut(block: &mut Block, required: &BTreeSet<String>) {
    remove_unneeded_mut(&mut block.stmts, required);
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Expr, FnDef, IndentStmt, Stmt, Visibility};

    fn run(stmts: Vec<Stmt>) -> String {
        let mut f = func(vec![param("a", "i32")], Some("i32"), stmts);
        f.params[0].mutable = true;
        fixup(&mut f);
        emit(f)
    }

    #[test]
    fn removes_mut_from_params_and_locals_that_are_not_reassigned() {
        let out = run(vec![
            let_mut(
                "c",
                "i32",
                bin(crate::rust_ast::BinOp::Add, var("a"), int(1)),
            ),
            Stmt::Return(Some(var("c"))),
        ]);

        assert_eq!(
            out,
            "\
fn f(a: i32) -> i32 {
    let c: i32 = a + 1;
    return c;
}
"
        );
    }

    #[test]
    fn keeps_mut_when_binding_is_assigned() {
        let out = run(vec![
            let_mut("c", "i32", int(0)),
            assign("c", var("a")),
            Stmt::Return(Some(var("c"))),
        ]);

        assert_eq!(
            out,
            "\
fn f(a: i32) -> i32 {
    let mut c: i32 = 0;
    c = a;
    return c;
}
"
        );
    }

    #[test]
    fn keeps_mut_when_parameter_is_assigned() {
        let out = run(vec![assign("a", int(2)), Stmt::Return(Some(var("a")))]);

        assert_eq!(
            out,
            "\
fn f(mut a: i32) -> i32 {
    a = 2;
    return a;
}
"
        );
    }

    #[test]
    fn keeps_mut_when_address_is_taken() {
        let out = run(vec![
            let_mut("c", "i32", int(0)),
            temp(
                "p",
                "*mut i32",
                Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(var("c")),
                },
            ),
            Stmt::Return(Some(var("c"))),
        ]);

        assert_eq!(
            out,
            "\
fn f(a: i32) -> i32 {
    let mut c: i32 = 0;
    let p: *mut i32 = std::ptr::addr_of_mut!(c);
    return c;
}
"
        );
    }

    #[test]
    fn keeps_mut_when_mutably_borrowed() {
        let out = run(vec![
            let_mut("c", "i32", int(0)),
            temp(
                "p",
                "&mut i32",
                Expr::Ref {
                    mutable: true,
                    expr: Box::new(var("c")),
                },
            ),
            Stmt::Return(Some(var("c"))),
        ]);

        assert_eq!(
            out,
            "\
fn f(a: i32) -> i32 {
    let mut c: i32 = 0;
    let p: &mut i32 = &mut c;
    return c;
}
"
        );
    }

    #[test]
    fn keeps_mut_when_used_as_method_receiver() {
        let out = run(vec![
            let_mut(
                "items",
                "Vec<i32>",
                Expr::Call {
                    func: Box::new(Expr::Var("Vec::new".into())),
                    args: vec![],
                },
            ),
            Stmt::Expr(Expr::MethodCall {
                recv: Box::new(var("items")),
                method: "push".into(),
                args: vec![int(1)],
            }),
            Stmt::Return(Some(var("a"))),
        ]);

        assert_eq!(
            out,
            "\
fn f(a: i32) -> i32 {
    let mut items: Vec<i32> = Vec::new();
    items.push(1);
    return a;
}
"
        );
    }

    #[test]
    fn removes_mut_in_nested_bodies() {
        let mut f = FnDef {
            vis: Visibility::Private,
            unsafe_extern_c: false,
            name: "f".into(),
            params: vec![],
            ret: None,
            body: vec![IndentStmt {
                depth: 1,
                stmt: Stmt::If {
                    cond: var("cond"),
                    then_body: vec![IndentStmt {
                        depth: 0,
                        stmt: let_mut("x", "i32", int(1)),
                    }],
                    else_body: vec![],
                },
            }],
        };
        fixup(&mut f);

        assert_eq!(
            emit(f),
            "\
fn f() {
    if cond {
        let x: i32 = 1;
    }
}
"
        );
    }
}
