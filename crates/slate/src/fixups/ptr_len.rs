use std::collections::BTreeMap;

use crate::fixups::support::walk;
use crate::rust_ast::{
    Block, Expr, FnDef, FnParam, Ident, IndentStmt, Item, Prim, Program, RustValue, Stmt, Type,
};

pub(super) fn fixup(program: &mut Program) {
    let candidates = collect_candidates(program);
    if candidates.is_empty() {
        return;
    }

    let observed = collect_callsites(program, &candidates);
    let plans = candidates
        .into_iter()
        .filter_map(|(name, candidate)| {
            let calls = observed.get(&name)?;
            if calls.has_unsupported {
                return None;
            }
            plan_for_candidate(candidate, calls).map(|plan| (name, plan))
        })
        .collect::<BTreeMap<_, _>>();
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
struct Candidate {
    ptr_index: usize,
    len_index: usize,
    ptr_name: String,
    len_name: String,
    ptr_mutable: bool,
    elem: Type,
    len_ty: Type,
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

#[derive(Clone)]
struct Callsite {
    ptr_index: usize,
    len_index: usize,
    ptr_mutable: bool,
    array_len: u64,
}

#[derive(Default)]
struct ObservedCalls {
    supported: Vec<Callsite>,
    has_unsupported: bool,
}

impl std::ops::Deref for ObservedCalls {
    type Target = [Callsite];

    fn deref(&self) -> &Self::Target {
        &self.supported
    }
}

fn collect_candidates(program: &Program) -> BTreeMap<String, Candidate> {
    program
        .items
        .iter()
        .filter_map(|item| {
            let Item::Fn(f) = item else {
                return None;
            };
            let candidate = adjacent_ptr_len_pair(&f.params)?;
            Some((f.name.clone(), candidate))
        })
        .collect()
}

fn adjacent_ptr_len_pair(params: &[FnParam]) -> Option<Candidate> {
    for (i, pair) in params.windows(2).enumerate() {
        let Type::Ptr { mutable, inner } = &pair[0].ty else {
            continue;
        };
        if !is_integer_type(&pair[1].ty) {
            continue;
        }
        return Some(Candidate {
            ptr_index: i,
            len_index: i + 1,
            ptr_name: pair[0].name.clone(),
            len_name: pair[1].name.clone(),
            ptr_mutable: *mutable,
            elem: (**inner).clone(),
            len_ty: pair[1].ty.clone(),
        });
    }
    None
}

fn is_integer_type(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Prim(
            Prim::I8
                | Prim::I16
                | Prim::I32
                | Prim::I64
                | Prim::I128
                | Prim::Isize
                | Prim::U8
                | Prim::U16
                | Prim::U32
                | Prim::U64
                | Prim::U128
                | Prim::Usize
        )
    )
}

fn collect_callsites(
    program: &Program,
    candidates: &BTreeMap<String, Candidate>,
) -> BTreeMap<String, ObservedCalls> {
    let mut calls = BTreeMap::new();
    for item in &program.items {
        let Item::Fn(f) = item else {
            continue;
        };
        let mut arrays = BTreeMap::new();
        collect_body_arrays(&f.body, &mut arrays);
        collect_body_calls(&f.body, candidates, &arrays, &mut calls);
    }
    calls
}

fn collect_body_arrays(body: &[IndentStmt], arrays: &mut BTreeMap<String, u64>) {
    for indent in body {
        match &indent.stmt {
            Stmt::Let {
                name,
                ty: Some(Type::Array { len, .. }),
                ..
            } => {
                arrays.insert(name.clone(), *len);
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
                collect_body_arrays(then_body, arrays);
                collect_body_arrays(else_body, arrays);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                collect_body_arrays(body, arrays);
            }
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                collect_block_arrays(body, arrays);
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    collect_body_arrays(&arm.body, arrays);
                }
            }
            _ => {}
        }
    }
}

fn collect_block_arrays(block: &Block, arrays: &mut BTreeMap<String, u64>) {
    collect_body_arrays(&block.stmts, arrays);
}

fn collect_body_calls(
    body: &[IndentStmt],
    candidates: &BTreeMap<String, Candidate>,
    arrays: &BTreeMap<String, u64>,
    calls: &mut BTreeMap<String, ObservedCalls>,
) {
    for indent in body {
        walk::stmt_exprs(&indent.stmt, &mut |expr| {
            let Expr::Call { func, .. } = expr else {
                return;
            };
            let Expr::Var(name) = &**func else {
                return;
            };
            if !candidates.contains_key(name.as_str()) {
                return;
            }
            let entry = calls.entry(name.as_str().into()).or_default();
            match callsite(expr, candidates, arrays) {
                Some((_, callsite)) => entry.supported.push(callsite),
                None => entry.has_unsupported = true,
            }
        });
    }
}

fn callsite(
    expr: &Expr,
    candidates: &BTreeMap<String, Candidate>,
    arrays: &BTreeMap<String, u64>,
) -> Option<(String, Callsite)> {
    let Expr::Call { func, args } = expr else {
        return None;
    };
    let Expr::Var(name) = &**func else {
        return None;
    };
    let candidate = candidates.get(name.as_str())?;
    let ptr_arg = args.get(candidate.ptr_index)?;
    let len_arg = args.get(candidate.len_index)?;
    let (array_name, ptr_mutable) = array_pointer_arg(ptr_arg)?;
    let array_len = *arrays.get(array_name.as_str())?;
    if integer_value(len_arg)? != array_len {
        return None;
    }
    Some((
        name.as_str().into(),
        Callsite {
            ptr_index: candidate.ptr_index,
            len_index: candidate.len_index,
            ptr_mutable,
            array_len,
        },
    ))
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

fn integer_value(expr: &Expr) -> Option<u64> {
    match expr {
        Expr::Value(RustValue::I64(n)) => u64::try_from(*n).ok(),
        Expr::Value(RustValue::I128(n)) => u64::try_from(*n).ok(),
        Expr::Cast { expr, .. } => integer_value(expr),
        _ => None,
    }
}

fn plan_for_candidate(candidate: Candidate, calls: &[Callsite]) -> Option<Plan> {
    if calls.is_empty() {
        return None;
    }
    if !calls.iter().all(|call| {
        call.ptr_index == candidate.ptr_index
            && call.len_index == candidate.len_index
            && call.array_len > 0
    }) {
        return None;
    }
    let mutable = candidate.ptr_mutable || calls.iter().any(|call| call.ptr_mutable);
    Some(Plan {
        ptr_index: candidate.ptr_index,
        len_index: candidate.len_index,
        ptr_name: candidate.ptr_name,
        len_name: candidate.len_name,
        mutable,
        elem: candidate.elem,
        len_ty: candidate.len_ty,
    })
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, Program};

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

        fixup(&mut program);

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

        fixup(&mut program);

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

        fixup(&mut program);

        assert!(program.emit().contains("fn f(items: *mut i32, len: i32)"));
    }
}
