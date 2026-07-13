//! Remove `mut` from bindings whose analyzed facts do not require mutation.

use crate::fixups::facts::{AstPath, FixupFacts, FunctionId, PathSegment};
use crate::fixups::support::walk;
use crate::rust_ast::{FnDef, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(f: &mut FnDef, function: FunctionId, facts: &FixupFacts) {
    for (index, param) in f.params.iter_mut().enumerate() {
        if facts
            .binding_by_param_index(function, index)
            .is_some_and(|binding| !facts.binding_requires_mut(binding))
        {
            param.mutable = false;
        }
    }
    remove_unneeded_mut(&mut f.body, function, facts, &mut Vec::new());
}

fn remove_unneeded_mut(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) {
    for (index, indent) in body.iter_mut().enumerate() {
        path.push(PathSegment::Stmt(index));
        remove_stmt_unneeded_mut(&mut indent.stmt, function, facts, path);
        path.pop();
    }
}

fn remove_stmt_unneeded_mut(
    stmt: &mut Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) {
    match stmt {
        Stmt::Let { name, mutable, .. } | Stmt::LetIf { name, mutable, .. }
            if local_can_drop_mut(function, facts, name, path) =>
        {
            *mutable = false;
        }
        _ => {}
    }
    walk::nested_bodies_mut_with_path(stmt, path, &mut |body, path| {
        remove_unneeded_mut(body, function, facts, path);
    });
}

fn local_can_drop_mut(
    function: FunctionId,
    facts: &FixupFacts,
    name: &str,
    path: &[PathSegment],
) -> bool {
    facts
        .binding_by_local_path(function, name, &AstPath(path.to_vec()))
        .is_some_and(|binding| !facts.binding_requires_mut(binding))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Expr, FnDef, IndentStmt, Item, Program, Stmt, Visibility};

    fn run(stmts: Vec<Stmt>) -> String {
        let mut f = func(vec![param("a", "i32")], Some("i32"), stmts);
        f.params[0].mutable = true;
        run_fn(f)
    }

    fn run_fn(f: FnDef) -> String {
        let analyzed = facts::analyze(Program {
            items: vec![Item::Fn(f)],
        });
        let mut f = match analyzed.program.items.into_iter().next().unwrap() {
            Item::Fn(f) => f,
            _ => unreachable!(),
        };
        fixup(&mut f, facts::FunctionId(0), &analyzed.facts);
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
    fn keeps_mut_when_raw_pointer_is_derived() {
        let out = run(vec![
            let_mut("items", "[i32; 1]", Expr::ArrayLit(vec![int(0)])),
            temp(
                "p",
                "*mut i32",
                Expr::ArrayPtr {
                    array: Box::new(var("items")),
                    mutable: true,
                },
            ),
            Stmt::Return(Some(var("a"))),
        ]);

        assert_eq!(
            out,
            "\
fn f(a: i32) -> i32 {
    let mut items: [i32; 1] = [0];
    let p: *mut i32 = items.as_mut_ptr();
    return a;
}
"
        );
    }

    #[test]
    fn removes_mut_in_nested_bodies() {
        let f = FnDef {
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

        assert_eq!(
            run_fn(f),
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
