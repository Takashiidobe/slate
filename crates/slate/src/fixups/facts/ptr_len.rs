use std::collections::BTreeMap;

use crate::fixups::facts::{
    BindingId, FixupFacts, FunctionId, PtrLenSliceFact, PtrLenUnsupportedCallsiteFact,
};
use crate::fixups::support::walk;
use crate::rust_ast::{
    Block, Expr, FnParam, IndentStmt, Item, Prim, Program, RustValue, Stmt, Type,
};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.ptr_len_slices.clear();
    facts.ptr_len_unsupported_callsites.clear();
    let candidates = collect_candidates(program, facts);
    collect_callsites(program, facts, &candidates);
}

#[derive(Clone)]
struct Candidate {
    function: FunctionId,
    ptr_param: BindingId,
    len_param: BindingId,
    ptr_index: usize,
    len_index: usize,
    ptr_mutable: bool,
    elem: Type,
    len_ty: Type,
}

#[derive(Clone)]
struct Callsite {
    ptr_mutable: bool,
    array_len: u64,
}

fn collect_candidates(program: &Program, facts: &FixupFacts) -> BTreeMap<String, Candidate> {
    program
        .items
        .iter()
        .enumerate()
        .filter_map(|(item_index, item)| {
            let Item::Fn(f) = item else {
                return None;
            };
            let function = facts.function_by_item_index(item_index)?;
            let candidate = adjacent_ptr_len_pair(function, &f.params, facts)?;
            Some((f.name.clone(), candidate))
        })
        .collect()
}

fn adjacent_ptr_len_pair(
    function: FunctionId,
    params: &[FnParam],
    facts: &FixupFacts,
) -> Option<Candidate> {
    for (i, pair) in params.windows(2).enumerate() {
        let Type::Ptr { mutable, inner } = &pair[0].ty else {
            continue;
        };
        if !is_integer_type(&pair[1].ty) {
            continue;
        }
        let ptr_param = facts.binding_by_param_index(function, i)?;
        let len_param = facts.binding_by_param_index(function, i + 1)?;
        return Some(Candidate {
            function,
            ptr_param,
            len_param,
            ptr_index: i,
            len_index: i + 1,
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
    facts: &mut FixupFacts,
    candidates: &BTreeMap<String, Candidate>,
) {
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(caller) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut arrays = BTreeMap::new();
        collect_body_arrays(&f.body, &mut arrays);
        collect_body_calls(caller, &f.body, facts, candidates, &arrays);
    }
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
    caller: FunctionId,
    body: &[IndentStmt],
    facts: &mut FixupFacts,
    candidates: &BTreeMap<String, Candidate>,
    arrays: &BTreeMap<String, u64>,
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
            match callsite(expr, candidates, arrays) {
                Some((_, callsite)) => {
                    let candidate = &candidates[name.as_str()];
                    facts.ptr_len_slices.push(PtrLenSliceFact {
                        caller,
                        callee: candidate.function,
                        ptr_param: candidate.ptr_param,
                        len_param: candidate.len_param,
                        backing_array_len: callsite.array_len,
                        mutable: candidate.ptr_mutable || callsite.ptr_mutable,
                        elem_ty: candidate.elem.clone(),
                        len_ty: candidate.len_ty.clone(),
                    });
                }
                None => {
                    let candidate = &candidates[name.as_str()];
                    facts
                        .ptr_len_unsupported_callsites
                        .push(PtrLenUnsupportedCallsiteFact {
                            caller,
                            callee: candidate.function,
                        });
                }
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
