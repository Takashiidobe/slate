use std::collections::BTreeMap;

use crate::fixups::facts::{BindingId, BindingKind, FixupFacts, FunctionId, PtrLenSliceFact};
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, FnDef, Ident, IndentStmt, Item, Program, Stmt, Type};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
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
    len_index: usize,
    ptr_name: String,
    len_name: String,
    mutable: bool,
    elem: Type,
    len_ty: Type,
}

/// A function's `(ptr, len)` pairs are always disjoint parameter slots: each
/// pair spans an adjacent `(Ptr, integer)` window, and a slot used as one
/// pair's `len` (an integer) can never simultaneously be another pair's `ptr`
/// (which must be a pointer). So plans for the same function never contend
/// for the same parameter index.
fn plans_from_facts(facts: &FixupFacts) -> BTreeMap<String, Vec<Plan>> {
    let mut grouped = BTreeMap::<(FunctionId, BindingId, BindingId), Vec<&PtrLenSliceFact>>::new();
    for fact in &facts.ptr_len_slices {
        grouped
            .entry((fact.callee, fact.ptr_param, fact.len_param))
            .or_default()
            .push(fact);
    }

    let mut plans = BTreeMap::<String, Vec<Plan>>::new();
    for ((function, ptr_param, len_param), calls) in grouped {
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
        let Some(len_binding) = facts
            .bindings
            .iter()
            .find(|binding| binding.id == len_param)
        else {
            continue;
        };
        let BindingKind::Param { index: ptr_index } = ptr_binding.kind else {
            continue;
        };
        let BindingKind::Param { index: len_index } = len_binding.kind else {
            continue;
        };
        let mutable = calls.iter().any(|call| call.mutable);
        let first = calls[0];
        plans
            .entry(function_fact.name.clone())
            .or_default()
            .push(Plan {
                ptr_index,
                len_index,
                ptr_name: ptr_binding.name.clone(),
                len_name: len_binding.name.clone(),
                mutable,
                elem: first.elem_ty.clone(),
                len_ty: first.len_ty.clone(),
            });
    }
    plans
}

fn rewrite_function(f: &mut FnDef, plans: &[Plan]) {
    if plans
        .iter()
        .any(|plan| f.params.len() <= plan.ptr_index || f.params.len() <= plan.len_index)
    {
        return;
    }

    for plan in plans {
        f.params[plan.ptr_index].ty = Type::Ref {
            mutable: plan.mutable,
            inner: Box::new(Type::Slice(Box::new(plan.elem.clone()))),
        };
        rewrite_body_pointer_param(&mut f.body, &plan.ptr_name, plan.mutable);
    }

    let mut len_indices: Vec<usize> = plans.iter().map(|plan| plan.len_index).collect();
    len_indices.sort_unstable_by(|a, b| b.cmp(a));
    for len_index in len_indices {
        f.params.remove(len_index);
    }

    let lets: Vec<IndentStmt> = plans
        .iter()
        .map(|plan| IndentStmt {
            depth: 1,
            stmt: Stmt::Let {
                name: plan.len_name.clone(),
                mutable: false,
                ty: Some(plan.len_ty.clone()),
                init: Some(Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::Var(plan.ptr_name.clone().into())),
                        method: "len".into(),
                        args: Vec::new(),
                    }),
                    ty: plan.len_ty.clone(),
                }),
            },
        })
        .collect();
    f.body.splice(0..0, lets);
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
        let Expr::Call { func, args } = expr else {
            return true;
        };
        let Expr::Var(name) = &**func else {
            return true;
        };
        let Some(fn_plans) = plans.get(name.as_str()) else {
            return true;
        };
        if fn_plans
            .iter()
            .any(|plan| args.len() <= plan.ptr_index || args.len() <= plan.len_index)
        {
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

        let mut len_indices: Vec<usize> = fn_plans.iter().map(|plan| plan.len_index).collect();
        len_indices.sort_unstable_by(|a, b| b.cmp(a));
        for len_index in len_indices {
            args.remove(len_index);
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
    use crate::rust_ast::{Item, Program};

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

        let facts = analyze_collect_fixup(&mut program);

        assert_eq!(facts.ptr_len_slices.len(), 1);
        let ptr_binding = facts
            .bindings
            .iter()
            .find(|binding| binding.id == facts.ptr_len_slices[0].ptr_param)
            .unwrap();
        let len_binding = facts
            .bindings
            .iter()
            .find(|binding| binding.id == facts.ptr_len_slices[0].len_param)
            .unwrap();
        assert_eq!(ptr_binding.name, "items");
        assert_eq!(len_binding.name, "len");
        assert!(!facts.ptr_len_slices[0].mutable);
        assert_eq!(
            program.emit(),
            "\
fn f(items: &[i32]) -> i32 {
    let len: i32 = items.len() as i32;
    for i in 0..len {
                unsafe { *items.as_ptr().offset(i) };
    }
    return unsafe { *items.as_ptr() };
}

fn main() {
    let mut values: [i32; 4] = [0; 4];
    f(values.as_slice());
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

        let facts = analyze_collect_fixup(&mut program);

        assert_eq!(facts.ptr_len_slices.len(), 1);
        assert!(facts.ptr_len_slices[0].mutable);
        assert!(program.emit().contains("fn f(items: &mut [i32])"));
        assert!(program.emit().contains("f(values.as_mut_slice());"));
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

        let facts = analyze_collect_fixup(&mut program);

        assert!(facts.ptr_len_slices.is_empty());
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

        let facts = analyze_collect_fixup(&mut program);

        assert!(facts.ptr_len_slices.is_empty());
        assert!(
            program
                .emit()
                .contains("fn f(items: *mut i32, printable: i32)")
        );
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

        let facts = analyze_collect_fixup(&mut program);

        assert_eq!(facts.ptr_len_slices.len(), 1);
        assert!(
            program
                .emit()
                .contains("fn f(items: &[i32], printable: i32)")
        );
        assert!(program.emit().contains("f(values.as_slice(), 5);"));
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

        let facts = analyze_collect_fixup(&mut program);

        assert!(facts.ptr_len_slices.is_empty());
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

        let facts = analyze_collect_fixup(&mut program);

        assert_eq!(facts.ptr_len_slices.len(), 2);
        // `q` is conservatively treated as mutated because `outer` forwards it
        // into an opaque call (`inner`) rather than proving `inner` read-only.
        assert!(program.emit().contains("fn inner(items: &[i32])"));
        assert!(program.emit().contains("fn outer(q: &mut [i32])"));
        assert!(program.emit().contains("inner(q.as_slice());"));
        assert!(program.emit().contains("outer(values.as_mut_slice());"));
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

        let facts = analyze_collect_fixup(&mut program);

        assert_eq!(facts.ptr_len_slices.len(), 2);
        assert!(program.emit().contains("fn f(items: &[i32])"));
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

        let facts = analyze_collect_fixup(&mut program);

        assert_eq!(facts.ptr_len_slices.len(), 2);
        assert!(program.emit().contains("fn f(a: &[i32], b: &[i32])"));
        assert!(program.emit().contains("f(a.as_slice(), b.as_slice());"));
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

        let facts = analyze_collect_fixup(&mut program);

        assert!(facts.ptr_len_slices.is_empty());
        assert!(program.emit().contains("fn f(items: *mut i32, len: i32)"));
    }
}
