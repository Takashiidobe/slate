use std::collections::BTreeMap;

use crate::fixups::facts::{BindingKind, FixupFacts, FunctionId, PtrLenSliceFact};
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, FnDef, Ident, IndentStmt, Item, Program, Stmt, Type};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
    let plans = plans_from_facts(facts);
    if plans.is_empty() {
        return;
    }

    for item in &mut program.items {
        if let Item::Fn(f) = item {
            if let Some(plan) = plans.get(&f.name) {
                rewrite_function(f, plan);
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

fn plans_from_facts(facts: &FixupFacts) -> BTreeMap<String, Plan> {
    let mut grouped = BTreeMap::<FunctionId, Vec<&PtrLenSliceFact>>::new();
    for fact in &facts.ptr_len_slices {
        grouped.entry(fact.callee).or_default().push(fact);
    }

    grouped
        .into_iter()
        .filter(|(function, calls)| {
            !calls.is_empty()
                && !facts
                    .ptr_len_unsupported_callsites
                    .iter()
                    .any(|unsupported| unsupported.callee == *function)
                && calls.iter().all(|call| call.backing_array_len > 0)
        })
        .filter_map(|(function, calls)| {
            let first = calls[0];
            if !calls
                .iter()
                .all(|call| call.ptr_param == first.ptr_param && call.len_param == first.len_param)
            {
                return None;
            }
            let function_fact = facts.functions.iter().find(|fact| fact.id == function)?;
            let ptr_binding = facts
                .bindings
                .iter()
                .find(|binding| binding.id == first.ptr_param)?;
            let len_binding = facts
                .bindings
                .iter()
                .find(|binding| binding.id == first.len_param)?;
            let BindingKind::Param { index: ptr_index } = ptr_binding.kind else {
                return None;
            };
            let BindingKind::Param { index: len_index } = len_binding.kind else {
                return None;
            };
            let mutable = calls.iter().any(|call| call.mutable);
            Some((
                function_fact.name.clone(),
                Plan {
                    ptr_index,
                    len_index,
                    ptr_name: ptr_binding.name.clone(),
                    len_name: len_binding.name.clone(),
                    mutable,
                    elem: first.elem_ty.clone(),
                    len_ty: first.len_ty.clone(),
                },
            ))
        })
        .collect()
}

fn rewrite_function(f: &mut FnDef, plan: &Plan) {
    if f.params.len() <= plan.len_index {
        return;
    }
    f.params[plan.ptr_index].ty = Type::Ref {
        mutable: plan.mutable,
        inner: Box::new(Type::Slice(Box::new(plan.elem.clone()))),
    };
    f.params.remove(plan.len_index);
    rewrite_body_pointer_param(&mut f.body, &plan.ptr_name, plan.mutable);
    f.body.insert(
        0,
        IndentStmt {
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
        },
    );
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

fn rewrite_calls_in_body(body: &mut [IndentStmt], plans: &BTreeMap<String, Plan>) {
    walk::body_exprs_mut_with(body, &mut |expr| {
        let Expr::Call { func, args } = expr else {
            return true;
        };
        let Expr::Var(name) = &**func else {
            return true;
        };
        let Some(plan) = plans.get(name.as_str()) else {
            return true;
        };
        let Some((array_name, _)) = args.get(plan.ptr_index).and_then(array_pointer_arg) else {
            return true;
        };
        let slice = Expr::MethodCall {
            recv: Box::new(Expr::Var(Ident::from(array_name))),
            method: if plan.mutable {
                "as_mut_slice"
            } else {
                "as_slice"
            }
            .into(),
            args: Vec::new(),
        };
        args[plan.ptr_index] = slice;
        args.remove(plan.len_index);
        false
    });
}

fn array_pointer_arg(expr: &Expr) -> Option<(String, bool)> {
    match expr {
        Expr::ArrayPtr { array, mutable } => {
            let Expr::Var(name) = &**array else {
                return None;
            };
            Some((name.as_str().into(), *mutable))
        }
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            let Expr::Var(name) = &**recv else {
                return None;
            };
            Some((name.as_str().into(), method == "as_mut_ptr"))
        }
        _ => None,
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

    #[test]
    fn rewrites_full_array_pointer_len_calls_to_slice_params() {
        let mut main = func(
            Vec::new(),
            None,
            vec![
                let_mut(
                    "values",
                    "[i32; 4]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 4,
                    },
                ),
                Stmt::Expr(call(
                    "f",
                    vec![
                        Expr::MethodCall {
                            recv: Box::new(var("values")),
                            method: "as_mut_ptr".into(),
                            args: Vec::new(),
                        },
                        int(4),
                    ],
                )),
            ],
        );
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![param("items", "*mut i32"), param("len", "i32")],
                    Some("i32"),
                    vec![
                        Stmt::Let {
                            name: "tmp".into(),
                            mutable: false,
                            ty: Some(Type::parse("*mut i32")),
                            init: Some(var("items")),
                        },
                        Stmt::Return(Some(var("len"))),
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
        assert_eq!(facts.ptr_len_slices[0].backing_array_len, 4);
        assert!(facts.ptr_len_slices[0].mutable);
        assert_eq!(
            program.emit(),
            "\
fn f(items: &mut [i32]) -> i32 {
    let len: i32 = items.len() as i32;
    let tmp: *mut i32 = items.as_mut_ptr();
    return len;
}

fn main() {
    let mut values: [i32; 4] = [0; 4];
    f(values.as_mut_slice());
}
"
        );
    }

    #[test]
    fn leaves_partial_lengths_raw() {
        let mut main = func(
            Vec::new(),
            None,
            vec![
                let_mut(
                    "values",
                    "[i32; 4]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 4,
                    },
                ),
                Stmt::Expr(call(
                    "f",
                    vec![
                        Expr::MethodCall {
                            recv: Box::new(var("values")),
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
                    vec![param("items", "*mut i32"), param("len", "i32")],
                    None,
                    vec![Stmt::Return(None)],
                )),
                Item::Fn(main),
            ],
        };

        let facts = analyze_collect_fixup(&mut program);

        assert!(facts.ptr_len_slices.is_empty());
        assert_eq!(facts.ptr_len_unsupported_callsites.len(), 1);
        assert!(program.emit().contains("fn f(items: *mut i32, len: i32)"));
    }

    #[test]
    fn leaves_function_raw_when_any_callsite_is_unproved() {
        let mut main = func(
            Vec::new(),
            None,
            vec![
                let_mut(
                    "values",
                    "[i32; 4]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 4,
                    },
                ),
                Stmt::Expr(call(
                    "f",
                    vec![
                        Expr::MethodCall {
                            recv: Box::new(var("values")),
                            method: "as_mut_ptr".into(),
                            args: Vec::new(),
                        },
                        int(4),
                    ],
                )),
                Stmt::Expr(call("f", vec![var("unknown"), int(4)])),
            ],
        );
        main.name = "main".into();
        let mut program = Program {
            items: vec![
                Item::Fn(func(
                    vec![param("items", "*mut i32"), param("len", "i32")],
                    None,
                    vec![Stmt::Return(None)],
                )),
                Item::Fn(main),
            ],
        };

        let facts = analyze_collect_fixup(&mut program);

        assert_eq!(facts.ptr_len_slices.len(), 1);
        assert_eq!(facts.ptr_len_unsupported_callsites.len(), 1);
        assert!(program.emit().contains("fn f(items: *mut i32, len: i32)"));
    }
}
