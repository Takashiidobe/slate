use std::collections::BTreeMap;

use crate::fixups::facts::{BindingId, BindingKind, FixupFacts, FunctionId, PtrLenSliceFact};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{Expr, FnDef, Ident, IndentStmt, Item, Program, Type};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
    let mut logger = crate::fixups::trace::NoopLogger;
    PtrLen::new(&mut logger).fixup(program, facts);
}

pub(in crate::fixups) struct PtrLen<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> PtrLen<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) {
        let before = self.logger.is_enabled().then(|| program.emit());
        fixup_impl(program, facts);
        if let Some(before) = before {
            let after = program.emit();
            if before != after {
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::PtrLen,
                    kind: "rewrite_ptr_len_slice_params".into(),
                    location: TraceLocation::default(),
                    before: vec![TraceSnippet::new("program", before.trim_end())],
                    after: vec![TraceSnippet::new("program", after.trim_end())],
                    facts: vec![fact(
                        "planned_pairs",
                        facts.ptr_len_slices.len().to_string(),
                    )],
                });
            }
        }
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) {
    let plans = plans_from_facts(facts);
    if plans.is_empty() {
        return;
    }

    for item in &mut program.items {
        if let Item::Fn(f) = item {
            if let Some(fn_plans) = plans.get(&f.name) {
                rewrite_function(f, fn_plans);
            }
            rewrite_calls_in_body(&mut f.body, &plans);
        }
    }
}

#[derive(Clone)]
struct Plan {
    ptr_index: usize,
    ptr_name: String,
    mutable: bool,
    elem: Type,
}

fn plans_from_facts(facts: &FixupFacts) -> BTreeMap<String, Vec<Plan>> {
    let mut grouped = BTreeMap::<(FunctionId, BindingId), Vec<&PtrLenSliceFact>>::new();
    for fact in &facts.ptr_len_slices {
        grouped
            .entry((fact.callee, fact.ptr_param))
            .or_default()
            .push(fact);
    }

    let mut plans = BTreeMap::<String, Vec<Plan>>::new();
    for ((function, ptr_param), calls) in grouped {
        let Some(function_fact) = facts.functions.iter().find(|fact| fact.id == function) else {
            continue;
        };
        let Some(ptr_binding) = facts
            .bindings
            .iter()
            .find(|binding| binding.id == ptr_param)
        else {
            continue;
        };
        let BindingKind::Param { index: ptr_index } = ptr_binding.kind else {
            continue;
        };
        let mutable = calls.iter().any(|call| call.mutable);
        let first = calls[0];
        plans
            .entry(function_fact.name.clone())
            .or_default()
            .push(Plan {
                ptr_index,
                ptr_name: ptr_binding.name.clone(),
                mutable,
                elem: first.elem_ty.clone(),
            });
    }
    plans
}

fn rewrite_function(f: &mut FnDef, plans: &[Plan]) {
    if plans.iter().any(|plan| f.params.len() <= plan.ptr_index) {
        return;
    }

    for plan in plans {
        f.params[plan.ptr_index].ty = Type::Ref {
            mutable: plan.mutable,
            inner: Box::new(Type::Slice(Box::new(plan.elem.clone()))),
        };
        rewrite_body_pointer_param(&mut f.body, &plan.ptr_name, plan.mutable);
    }
}

fn rewrite_body_pointer_param(body: &mut [IndentStmt], name: &str, mutable: bool) {
    walk::body_exprs_mut_with(body, &mut |expr| {
        if matches!(expr, Expr::Var(var) if var.as_str() == name) {
            *expr = Expr::MethodCall {
                recv: Box::new(Expr::Var(name.into())),
                method: if mutable { "as_mut_ptr" } else { "as_ptr" }.into(),
                args: Vec::new(),
            };
            return false;
        }
        true
    });
}

fn rewrite_calls_in_body(body: &mut [IndentStmt], plans: &BTreeMap<String, Vec<Plan>>) {
    walk::body_exprs_mut_with(body, &mut |expr| {
        let Expr::Call { func, args, .. } = expr else {
            return true;
        };
        let Expr::Var(name) = &**func else {
            return true;
        };
        let Some(fn_plans) = plans.get(name.as_str()) else {
            return true;
        };
        if fn_plans.iter().any(|plan| args.len() <= plan.ptr_index) {
            return true;
        }
        let Some(array_names) = fn_plans
            .iter()
            .map(|plan| args.get(plan.ptr_index).and_then(array_pointer_arg))
            .collect::<Option<Vec<_>>>()
        else {
            return true;
        };

        for (plan, array_name) in fn_plans.iter().zip(array_names) {
            args[plan.ptr_index] = Expr::MethodCall {
                recv: Box::new(Expr::Var(Ident::from(array_name))),
                method: if plan.mutable {
                    "as_mut_slice"
                } else {
                    "as_slice"
                }
                .into(),
                args: Vec::new(),
            };
        }

        false
    });
}

fn array_pointer_arg(expr: &Expr) -> Option<String> {
    match peel_pointer_view(expr) {
        Expr::Var(name) => Some(name.as_str().into()),
        _ => None,
    }
}

fn peel_pointer_view(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => peel_pointer_view(expr),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            peel_pointer_view(recv)
        }
        Expr::ArrayPtr { array, .. } => peel_pointer_view(array),
        _ => expr,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, Program, Stmt};

    fn analyze_collect_fixup(program: &mut Program) -> FixupFacts {
        let analyzed = facts::analyze(program.clone());
        let mut facts = analyzed.facts;
        facts::ptr_len::collect_facts(program, &mut facts);
        fixup(program, &facts);
        facts
    }

    fn array_call(name: &str, _elems: i64, len_arg: i64) -> Stmt {
        Stmt::Expr(call(
            name,
            vec![
                Expr::MethodCall {
                    recv: Box::new(var("values")),
                    method: "as_mut_ptr".into(),
                    args: Vec::new(),
                },
                int(len_arg),
            ],
        ))
    }

    fn array_decl(elems: i64) -> Stmt {
        let_mut(
            "values",
            &format!("[i32; {elems}]"),
            Expr::ArrayRepeat {
                elem: Box::new(int(0)),
                len: elems as usize,
            },
        )
    }

    fn offset_deref(ptr: &str, index: &str) -> Expr {
        Expr::Unsafe(Box::new(crate::rust_ast::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Unary {
                op: crate::rust_ast::UnaryOp::Deref,
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(var(ptr)),
                    method: "offset".into(),
                    args: vec![var(index)],
                }),
            })),
        }))
    }

    fn pointer_len_loop(ptr: &str, len: Expr) -> Stmt {
        Stmt::For {
            pat: "i".into(),
            iter: Expr::Range {
                start: Box::new(int(0)),
                end: Box::new(len),
            },
            body: vec![IndentStmt {
                depth: 2,
                stmt: Stmt::Expr(offset_deref(ptr, "i")),
            }],
        }
    }

    #[test]
    fn rewrites_full_array_pointer_len_calls_to_slice_params() {
        let mut main = func(Vec::new(), None, vec![array_decl(4), array_call("f", 4, 4)]);
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![param("items", "*mut i32"), param("len", "i32")],
                    Some("i32"),
                    vec![
                        pointer_len_loop("items", var("len")),
                        Stmt::Return(Some(Expr::Unsafe(Box::new(crate::rust_ast::Block {
                            stmts: Vec::new(),
                            tail: Some(Box::new(Expr::Unary {
                                op: crate::rust_ast::UnaryOp::Deref,
                                expr: Box::new(var("items")),
                            })),
                        })))),
                    ],
                )),
                Item::Fn(main),
            ],
        };

        analyze_collect_fixup(&mut program);

        assert_eq!(
            program.emit(),
            "\
fn f(items: &[i32], len: i32) -> i32 {
    for i in 0..len {
                unsafe { *items.as_ptr().offset(i) };
    }
    return unsafe { *items.as_ptr() };
}

fn main() {
    let mut values: [i32; 4] = [0; 4];
    f(values.as_slice(), 4);
}
"
        );
    }

    #[test]
    fn keeps_slice_param_mutable_when_pointer_is_written() {
        let mut main = func(Vec::new(), None, vec![array_decl(4), array_call("f", 4, 4)]);
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![param("items", "*mut i32"), param("len", "i32")],
                    None,
                    vec![
                        pointer_len_loop("items", var("len")),
                        Stmt::Assign {
                            target: Expr::Unary {
                                op: crate::rust_ast::UnaryOp::Deref,
                                expr: Box::new(var("items")),
                            },
                            value: int(1),
                        },
                    ],
                )),
                Item::Fn(main),
            ],
        };

        analyze_collect_fixup(&mut program);

        assert!(program.emit().contains("fn f(items: &mut [i32], len: i32)"));
        assert!(program.emit().contains("f(values.as_mut_slice(), 4);"));
    }

    #[test]
    fn leaves_partial_lengths_raw() {
        let mut main = func(Vec::new(), None, vec![array_decl(4), array_call("f", 4, 3)]);
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![param("items", "*mut i32"), param("len", "i32")],
                    None,
                    vec![pointer_len_loop("items", var("len")), Stmt::Return(None)],
                )),
                Item::Fn(main),
            ],
        };

        analyze_collect_fixup(&mut program);

        assert!(program.emit().contains("fn f(items: *mut i32, len: i32)"));
    }

    #[test]
    fn leaves_unrelated_equal_printable_param_in_place() {
        let mut main = func(Vec::new(), None, vec![array_decl(5), array_call("f", 5, 5)]);
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![param("items", "*mut i32"), param("printable", "i32")],
                    None,
                    vec![
                        Stmt::Expr(var("printable")),
                        pointer_len_loop("items", int(5)),
                        Stmt::Return(None),
                    ],
                )),
                Item::Fn(main),
            ],
        };

        analyze_collect_fixup(&mut program);

        assert!(
            program
                .emit()
                .contains("fn f(items: &[i32], printable: i32)")
        );
        assert!(program.emit().contains("f(values.as_slice(), 5);"));
    }

    #[test]
    fn rewrites_non_adjacent_real_length_param() {
        let mut main = func(
            Vec::new(),
            None,
            vec![
                array_decl(4),
                Stmt::Expr(call(
                    "f",
                    vec![
                        Expr::MethodCall {
                            recv: Box::new(var("values")),
                            method: "as_mut_ptr".into(),
                            args: Vec::new(),
                        },
                        int(5),
                        int(4),
                    ],
                )),
            ],
        );
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![
                        param("items", "*mut i32"),
                        param("printable", "i32"),
                        param("length", "i32"),
                    ],
                    None,
                    vec![
                        Stmt::Expr(var("printable")),
                        pointer_len_loop("items", var("length")),
                        Stmt::Return(None),
                    ],
                )),
                Item::Fn(main),
            ],
        };

        analyze_collect_fixup(&mut program);

        assert!(
            program
                .emit()
                .contains("fn f(items: &[i32], printable: i32, length: i32)")
        );
        assert!(program.emit().contains("f(values.as_slice(), 5, 4);"));
    }

    #[test]
    fn leaves_function_raw_when_any_callsite_is_unproved() {
        let mut main = func(
            Vec::new(),
            None,
            vec![
                array_decl(4),
                array_call("f", 4, 4),
                Stmt::Expr(call("f", vec![var("unknown"), int(4)])),
            ],
        );
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![param("items", "*mut i32"), param("len", "i32")],
                    None,
                    vec![pointer_len_loop("items", var("len")), Stmt::Return(None)],
                )),
                Item::Fn(main),
            ],
        };

        analyze_collect_fixup(&mut program);

        assert!(program.emit().contains("fn f(items: *mut i32, len: i32)"));
    }

    #[test]
    fn proves_ptr_len_forwarded_unchanged_through_a_call_chain() {
        let mut outer = func(
            vec![param("q", "*mut i32"), param("qlen", "i32")],
            None,
            vec![Stmt::Expr(call("inner", vec![var("q"), var("qlen")]))],
        );
        outer.name = "outer".into();

        let mut main = func(
            Vec::new(),
            None,
            vec![array_decl(4), array_call("outer", 4, 4)],
        );
        main.name = "main".into();

        let mut inner = func(
            vec![param("items", "*mut i32"), param("len", "i32")],
            None,
            vec![pointer_len_loop("items", var("len")), Stmt::Return(None)],
        );
        inner.name = "inner".into();

        let mut program = Program {
            items: vec![Item::Fn(inner), Item::Fn(outer), Item::Fn(main)],
        };

        analyze_collect_fixup(&mut program);

        // `q` is conservatively treated as mutated because `outer` forwards it
        // into an opaque call (`inner`) rather than proving `inner` read-only.
        assert!(program.emit().contains("fn inner(items: &[i32], len: i32)"));
        assert!(
            program
                .emit()
                .contains("fn outer(q: &mut [i32], qlen: i32)")
        );
        assert!(program.emit().contains("inner(q.as_slice(), qlen);"));
        assert!(program.emit().contains("outer(values.as_mut_slice(), 4);"));
    }

    #[test]
    fn shadowed_same_named_arrays_in_sibling_branches_resolve_independently() {
        let mut main = func(
            Vec::new(),
            None,
            vec![Stmt::If {
                cond: var("cond"),
                then_body: vec![IndentStmt {
                    depth: 2,
                    stmt: array_decl(4),
                }]
                .into_iter()
                .chain(std::iter::once(IndentStmt {
                    depth: 2,
                    stmt: array_call("f", 4, 4),
                }))
                .collect(),
                else_body: vec![IndentStmt {
                    depth: 2,
                    stmt: array_decl(2),
                }]
                .into_iter()
                .chain(std::iter::once(IndentStmt {
                    depth: 2,
                    stmt: array_call("f", 2, 2),
                }))
                .collect(),
            }],
        );
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![param("items", "*mut i32"), param("count", "i32")],
                    None,
                    vec![pointer_len_loop("items", var("count")), Stmt::Return(None)],
                )),
                Item::Fn(main),
            ],
        };

        analyze_collect_fixup(&mut program);

        assert!(program.emit().contains("fn f(items: &[i32], count: i32)"));
    }

    #[test]
    fn two_independent_ptr_len_pairs_on_one_function_both_convert() {
        let mut main = func(
            Vec::new(),
            None,
            vec![
                let_mut(
                    "a",
                    "[i32; 4]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 4,
                    },
                ),
                let_mut(
                    "b",
                    "[i32; 3]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 3,
                    },
                ),
                Stmt::Expr(call(
                    "f",
                    vec![
                        Expr::MethodCall {
                            recv: Box::new(var("a")),
                            method: "as_mut_ptr".into(),
                            args: Vec::new(),
                        },
                        int(4),
                        Expr::MethodCall {
                            recv: Box::new(var("b")),
                            method: "as_mut_ptr".into(),
                            args: Vec::new(),
                        },
                        int(3),
                    ],
                )),
            ],
        );
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![
                        param("a", "*mut i32"),
                        param("alen", "i32"),
                        param("b", "*mut i32"),
                        param("blen", "i32"),
                    ],
                    None,
                    vec![
                        pointer_len_loop("a", var("alen")),
                        pointer_len_loop("b", var("blen")),
                        Stmt::Return(None),
                    ],
                )),
                Item::Fn(main),
            ],
        };

        analyze_collect_fixup(&mut program);

        assert!(
            program
                .emit()
                .contains("fn f(a: &[i32], alen: i32, b: &[i32], blen: i32)")
        );
        assert!(
            program
                .emit()
                .contains("f(a.as_slice(), 4, b.as_slice(), 3);")
        );
    }

    #[test]
    fn reassigned_len_param_is_not_converted() {
        let mut main = func(Vec::new(), None, vec![array_decl(4), array_call("f", 4, 4)]);
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![param("items", "*mut i32"), param("len", "i32")],
                    None,
                    vec![assign("len", int(0)), Stmt::Return(None)],
                )),
                Item::Fn(main),
            ],
        };

        analyze_collect_fixup(&mut program);

        assert!(program.emit().contains("fn f(items: *mut i32, len: i32)"));
    }
}
