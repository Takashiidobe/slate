//! Remove `mut` from bindings whose analyzed facts do not require mutation.

use crate::fixups::facts::{
    AstPath, BindingId, BorrowAliasReason, CallArgPinning, FixupFacts, FunctionId, PathSegment,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, TraceSnippet, binding_facts, fact,
    function_path_location, path_fact, stmt_snippet,
};
use crate::rust_ast::{FnDef, IndentStmt, Stmt, Type};

pub(in crate::fixups) fn fixup(f: &mut FnDef, function: FunctionId, facts: &FixupFacts) {
    let mut logger = crate::fixups::trace::NoopLogger;
    RemoveMut::new(&mut logger).fixup(f, function, facts);
}

pub(in crate::fixups) struct RemoveMut<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> RemoveMut<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(
        &mut self,
        f: &mut FnDef,
        function: FunctionId,
        facts: &FixupFacts,
    ) {
        for (index, param) in f.params.iter_mut().enumerate() {
            if matches!(param.ty, Type::Variadic | Type::VaList) {
                continue;
            }
            let Some(binding) = facts.binding_by_param_index(function, index) else {
                continue;
            };
            if param_can_drop_mut(binding, facts) {
                let before = self
                    .logger
                    .is_enabled()
                    .then(|| format!("mut {}: {}", param.name, param.ty.render()));
                let param_name = param.name.clone();
                let param_ty = param.ty.render();
                param.mutable = false;
                if let Some(before) = before {
                    let mut event_facts = binding_facts(facts, binding);
                    event_facts.extend([
                        fact("param", param_name.clone()),
                        fact("param_index", index.to_string()),
                    ]);
                    self.logger.rewrite(RewriteEvent {
                        pass: TracePass::RemoveMut,
                        kind: "remove_param_mut".into(),
                        location: function_path_location(facts, function, &[]),
                        before: vec![TraceSnippet::new("param", before)],
                        after: vec![TraceSnippet::new(
                            "param",
                            format!("{param_name}: {param_ty}"),
                        )],
                        facts: event_facts,
                    });
                }
            }
        }
        self.remove_unneeded_mut(&mut f.body, function, facts, &mut Vec::new());
    }
}

fn param_can_drop_mut(binding: BindingId, facts: &FixupFacts) -> bool {
    if !facts.binding_requires_mut(binding) {
        return true;
    }
    let Some(def_use) = facts.def_use(binding) else {
        return false;
    };
    if !def_use.writes.is_empty() {
        return false;
    }
    let Some(alias) = facts
        .borrow_alias
        .iter()
        .find(|fact| fact.binding == binding)
    else {
        return true;
    };
    if !alias.reasons.iter().all(|reason| {
        matches!(
            reason,
            BorrowAliasReason::Read | BorrowAliasReason::UnknownCallEscape
        )
    }) {
        return false;
    }
    def_use.reads.iter().all(|read| {
        facts
            .call_arg_at(alias.function, read)
            .is_none_or(|(_, arg)| {
                arg.pinning == CallArgPinning::DeclaredParam
                    && matches!(
                        arg.declared_ty.as_ref(),
                        Some(Type::Ref { mutable: false, .. })
                    )
            })
    })
}

impl<'a> RemoveMut<'a> {
    fn remove_unneeded_mut(
        &mut self,
        body: &mut [IndentStmt],
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) {
        for (index, indent) in body.iter_mut().enumerate() {
            path.push(PathSegment::Stmt(index));
            self.remove_stmt_unneeded_mut(&mut indent.stmt, function, facts, path);
            path.pop();
        }
    }

    fn remove_stmt_unneeded_mut(
        &mut self,
        stmt: &mut Stmt,
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) {
        let before = self.logger.is_enabled().then(|| stmt.clone());
        let mut removed_name = None;
        match stmt {
            Stmt::Let { name, mutable, .. } | Stmt::LetIf { name, mutable, .. }
                if local_can_drop_mut(function, facts, name, path) =>
            {
                removed_name = Some(name.clone());
                *mutable = false;
            }
            _ => {}
        }
        if let (Some(before), Some(name)) = (before, removed_name) {
            let mut event_facts = if let Some(binding) =
                facts.binding_by_local_path(function, &name, &AstPath(path.to_vec()))
            {
                binding_facts(facts, binding)
            } else {
                vec![fact("binding_name", name)]
            };
            event_facts.push(path_fact("stmt_path", path));
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::RemoveMut,
                kind: "remove_local_mut".into(),
                location: function_path_location(facts, function, path),
                before: vec![stmt_snippet("binding", &before)],
                after: vec![stmt_snippet("binding", stmt)],
                facts: event_facts,
            });
        }
        walk::nested_bodies_mut_with_path(stmt, path, &mut |body, path| {
            self.remove_unneeded_mut(body, function, facts, path);
        });
    }
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

    fn run_program(program: Program, item_index: usize) -> String {
        let analyzed = facts::analyze(program);
        let mut program = analyzed.program;
        let Item::Fn(f) = &mut program.items[item_index] else {
            unreachable!();
        };
        fixup(f, facts::FunctionId(item_index), &analyzed.facts);
        emit(f.clone())
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
    fn removes_mut_from_param_passed_to_declared_param_call() {
        let mut caller = func(
            vec![param("s", "&str")],
            Some("i32"),
            vec![Stmt::Return(Some(call("parse_num", vec![var("s")])))],
        );
        caller.name = "forward_num".into();
        caller.params[0].mutable = true;
        let mut callee = func(
            vec![param("s", "&str")],
            Some("i32"),
            vec![Stmt::Return(Some(int(0)))],
        );
        callee.name = "parse_num".into();

        let out = run_program(
            Program {
                items: vec![Item::Fn(callee), Item::Fn(caller)],
            },
            1,
        );

        assert_eq!(
            out,
            "\
fn forward_num(s: &str) -> i32 {
    return parse_num(s);
}
"
        );
    }

    #[test]
    fn keeps_mut_on_variadic_param() {
        let mut f = func(vec![], Some("i32"), vec![Stmt::Return(Some(int(0)))]);
        f.unsafe_ = true;
        f.abi = Some(crate::rust_ast::Abi::C);
        f.params.push(crate::rust_ast::FnParam {
            name: "ap".into(),
            mutable: true,
            ty: Type::Variadic,
            nonnull: false,
        });

        let out = run_fn(f);

        assert_eq!(
            out,
            "\
unsafe extern \"C\" fn f(mut ap: ...) -> i32 {
    return 0;
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
    fn keeps_mut_when_deref_assigned() {
        let out = run(vec![
            let_mut(
                "p",
                "Box<i32>",
                Expr::Call {
                    func: Box::new(Expr::Var("Box::<i32>::new".into())),
                    args: vec![int(0)],
                },
            ),
            Stmt::Assign {
                target: Expr::Unary {
                    op: crate::rust_ast::UnaryOp::Deref,
                    expr: Box::new(var("p")),
                },
                value: int(1),
            },
            Stmt::Return(Some(var("a"))),
        ]);

        assert_eq!(
            out,
            "\
fn f(a: i32) -> i32 {
    let mut p: Box<i32> = Box::<i32>::new(0);
    *p = 1;
    return a;
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
    fn removes_mut_for_read_only_slice_method_receivers() {
        let mut f = func(
            vec![param("items", "&[i32]")],
            Some("i32"),
            vec![
                temp(
                    "len",
                    "i32",
                    Expr::Cast {
                        expr: Box::new(Expr::MethodCall {
                            recv: Box::new(var("items")),
                            method: "len".into(),
                            args: Vec::new(),
                        }),
                        ty: crate::rust_ast::Type::parse("i32"),
                    },
                ),
                Stmt::Expr(Expr::MethodCall {
                    recv: Box::new(var("items")),
                    method: "iter".into(),
                    args: Vec::new(),
                }),
                Stmt::Return(Some(var("len"))),
            ],
        );
        f.params[0].mutable = true;

        let out = run_fn(f);

        assert_eq!(
            out,
            "\
fn f(items: &[i32]) -> i32 {
    let len: i32 = items.len() as i32;
    items.iter();
    return len;
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
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
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
            returns_nonnull: false,
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
