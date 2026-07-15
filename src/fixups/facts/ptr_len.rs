use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    BindingId, FixupFacts, FunctionId, PtrLenSliceFact, PtrLenUnsupportedCallsiteFact,
};
use crate::rust_ast::{
    Block, Expr, FnParam, IndentStmt, Item, Prim, Program, RustValue, Stmt, Type, UnaryOp,
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
    body_mutable: bool,
    elem: Type,
    len_ty: Type,
}

#[derive(Clone)]
struct Callsite {
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
            let candidate = adjacent_ptr_len_pair(function, &f.params, &f.body, facts)?;
            Some((f.name.clone(), candidate))
        })
        .collect()
}

fn adjacent_ptr_len_pair(
    function: FunctionId,
    params: &[FnParam],
    body: &[IndentStmt],
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
            body_mutable: *mutable && pointer_param_mutated(body, pair[0].name.as_str()),
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
                        mutable: candidate.body_mutable,
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

fn pointer_param_mutated(body: &[IndentStmt], ptr_name: &str) -> bool {
    let mut aliases = BTreeSet::from([ptr_name.to_string()]);
    body_mutates_pointer_alias(body, &mut aliases)
}

fn body_mutates_pointer_alias(body: &[IndentStmt], aliases: &mut BTreeSet<String>) -> bool {
    body.iter()
        .any(|indent| stmt_mutates_pointer_alias(&indent.stmt, aliases))
}

fn block_mutates_pointer_alias(block: &Block, aliases: &mut BTreeSet<String>) -> bool {
    body_mutates_pointer_alias(&block.stmts, aliases)
        || block
            .tail
            .as_deref()
            .is_some_and(|expr| expr_mutates_pointer_alias(expr, aliases, AccessMode::Read))
}

fn stmt_mutates_pointer_alias(stmt: &Stmt, aliases: &mut BTreeSet<String>) -> bool {
    match stmt {
        Stmt::Let { name, init, .. } => {
            if let Some(init) = init {
                if expr_mutates_pointer_alias(init, aliases, AccessMode::Read) {
                    return true;
                }
                if expr_aliases_pointer(init, aliases) {
                    aliases.insert(name.clone());
                }
            }
            false
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_mutates_pointer_alias(cond, aliases, AccessMode::Read)
                || body_mutates_pointer_alias(then_body, &mut aliases.clone())
                || expr_mutates_pointer_alias(then_value, &mut aliases.clone(), AccessMode::Read)
                || body_mutates_pointer_alias(else_body, &mut aliases.clone())
                || expr_mutates_pointer_alias(else_value, &mut aliases.clone(), AccessMode::Read)
        }
        Stmt::Assign { target, value } => {
            expr_mutates_pointer_alias(target, aliases, AccessMode::Mutate)
                || expr_mutates_pointer_alias(value, aliases, AccessMode::Read)
        }
        Stmt::CompoundAssign { target, value, .. } => {
            expr_mutates_pointer_alias(target, aliases, AccessMode::Mutate)
                || expr_mutates_pointer_alias(value, aliases, AccessMode::Read)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            expr_mutates_pointer_alias(expr, aliases, AccessMode::Read)
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_mutates_pointer_alias(body, &mut aliases.clone())
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_mutates_pointer_alias(cond, aliases, AccessMode::Read)
                || body_mutates_pointer_alias(then_body, &mut aliases.clone())
                || body_mutates_pointer_alias(else_body, &mut aliases.clone())
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => body_mutates_pointer_alias(body, &mut aliases.clone()),
        Stmt::Match { expr, arms } => {
            expr_mutates_pointer_alias(expr, aliases, AccessMode::Read)
                || arms
                    .iter()
                    .any(|arm| body_mutates_pointer_alias(&arm.body, &mut aliases.clone()))
        }
    }
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum AccessMode {
    Read,
    Mutate,
}

fn expr_mutates_pointer_alias(
    expr: &Expr,
    aliases: &mut BTreeSet<String>,
    mode: AccessMode,
) -> bool {
    match expr {
        Expr::Var(name) => mode == AccessMode::Mutate && aliases.contains(name.as_str()),
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => {
            if expr_aliases_pointer(expr, aliases) {
                return mode == AccessMode::Mutate;
            }
            expr_mutates_pointer_alias(expr, aliases, AccessMode::Read)
        }
        Expr::Unary { expr, .. } => expr_mutates_pointer_alias(expr, aliases, AccessMode::Read),
        Expr::Binary { lhs, rhs, .. } => {
            expr_mutates_pointer_alias(lhs, aliases, AccessMode::Read)
                || expr_mutates_pointer_alias(rhs, aliases, AccessMode::Read)
        }
        Expr::Range { start, end } => {
            expr_mutates_pointer_alias(start, aliases, AccessMode::Read)
                || expr_mutates_pointer_alias(end, aliases, AccessMode::Read)
        }
        Expr::Call { func, args } => {
            if expr_mutates_pointer_alias(func, aliases, AccessMode::Read) {
                return true;
            }
            args.iter().any(|arg| {
                arg_contains_pointer_alias(arg, aliases)
                    || expr_mutates_pointer_alias(arg, aliases, AccessMode::Read)
            })
        }
        Expr::MethodCall { recv, method, args } => {
            if method_is_pointer_arithmetic(method) {
                expr_mutates_pointer_alias(recv, aliases, AccessMode::Read)
                    || args
                        .iter()
                        .any(|arg| expr_mutates_pointer_alias(arg, aliases, AccessMode::Read))
            } else {
                (arg_contains_pointer_alias(recv, aliases)
                    || expr_mutates_pointer_alias(recv, aliases, AccessMode::Read))
                    || args.iter().any(|arg| {
                        arg_contains_pointer_alias(arg, aliases)
                            || expr_mutates_pointer_alias(arg, aliases, AccessMode::Read)
                    })
            }
        }
        Expr::MethodCallGeneric { recv, args, .. } => {
            (arg_contains_pointer_alias(recv, aliases)
                || expr_mutates_pointer_alias(recv, aliases, AccessMode::Read))
                || args.iter().any(|arg| {
                    arg_contains_pointer_alias(arg, aliases)
                        || expr_mutates_pointer_alias(arg, aliases, AccessMode::Read)
                })
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
            expr_mutates_pointer_alias(base, aliases, mode)
        }
        Expr::ArrayPtr { array, mutable } => expr_mutates_pointer_alias(
            array,
            aliases,
            if *mutable {
                AccessMode::Mutate
            } else {
                AccessMode::Read
            },
        ),
        Expr::Index { base, index } => {
            expr_mutates_pointer_alias(base, aliases, mode)
                || expr_mutates_pointer_alias(index, aliases, AccessMode::Read)
        }
        Expr::StructLit { fields, .. } => fields
            .iter()
            .any(|(_, expr)| expr_mutates_pointer_alias(expr, aliases, AccessMode::Read)),
        Expr::TupleStructLit { fields, .. } => fields
            .iter()
            .any(|expr| expr_mutates_pointer_alias(expr, aliases, AccessMode::Read)),
        Expr::ArrayLit(elems) | Expr::VecLit(elems) | Expr::Macro { args: elems, .. } => elems
            .iter()
            .any(|expr| expr_mutates_pointer_alias(expr, aliases, AccessMode::Read)),
        Expr::ArrayRepeat { elem, .. } => {
            expr_mutates_pointer_alias(elem, aliases, AccessMode::Read)
        }
        Expr::VecRepeat { elem, len } => {
            expr_mutates_pointer_alias(elem, aliases, AccessMode::Read)
                || expr_mutates_pointer_alias(len, aliases, AccessMode::Read)
        }
        Expr::Closure { body, .. } => expr_mutates_pointer_alias(body, aliases, AccessMode::Read),
        Expr::Match { expr, arms } => {
            expr_mutates_pointer_alias(expr, aliases, AccessMode::Read)
                || arms
                    .iter()
                    .any(|arm| expr_mutates_pointer_alias(&arm.value, aliases, AccessMode::Read))
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_mutates_pointer_alias(cond, aliases, AccessMode::Read)
                || expr_mutates_pointer_alias(then_expr, &mut aliases.clone(), AccessMode::Read)
                || expr_mutates_pointer_alias(else_expr, &mut aliases.clone(), AccessMode::Read)
        }
        Expr::Block(block) | Expr::Unsafe(block) => {
            block_mutates_pointer_alias(block, &mut aliases.clone())
        }
        Expr::Cast { expr, .. } | Expr::Transmute { expr, .. } => {
            expr_mutates_pointer_alias(expr, aliases, mode)
        }
        Expr::Ref { mutable, expr } | Expr::AddrOf { mutable, expr } => expr_mutates_pointer_alias(
            expr,
            aliases,
            if *mutable { AccessMode::Mutate } else { mode },
        ),
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
            expr_mutates_pointer_alias(ptr, aliases, AccessMode::Read)
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            expr_mutates_pointer_alias(ptr, aliases, AccessMode::Mutate)
                || expr_mutates_pointer_alias(value, aliases, AccessMode::Read)
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            expr_mutates_pointer_alias(ptr, aliases, AccessMode::Mutate)
                || expr_mutates_pointer_alias(expected, aliases, AccessMode::Read)
                || expr_mutates_pointer_alias(desired, aliases, AccessMode::Read)
        }
        Expr::CopyNonoverlapping { src, dst, .. } | Expr::PtrCopy { src, dst, .. } => {
            expr_mutates_pointer_alias(src, aliases, AccessMode::Read)
                || expr_mutates_pointer_alias(dst, aliases, AccessMode::Mutate)
        }
        Expr::WriteBytes { dst, val, count } => {
            expr_mutates_pointer_alias(dst, aliases, AccessMode::Mutate)
                || expr_mutates_pointer_alias(val, aliases, AccessMode::Read)
                || expr_mutates_pointer_alias(count, aliases, AccessMode::Read)
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_)
        | Expr::Path(_) => false,
    }
}

fn arg_contains_pointer_alias(expr: &Expr, aliases: &BTreeSet<String>) -> bool {
    match expr {
        Expr::Var(name) => aliases.contains(name.as_str()),
        _ => {
            let mut aliases = aliases.clone();
            expr_aliases_pointer(expr, &mut aliases)
        }
    }
}

fn expr_aliases_pointer(expr: &Expr, aliases: &mut BTreeSet<String>) -> bool {
    match expr {
        Expr::Var(name) => aliases.contains(name.as_str()),
        Expr::Cast { expr, .. } | Expr::Transmute { expr, .. } => {
            expr_aliases_pointer(expr, aliases)
        }
        Expr::Unsafe(block) | Expr::Block(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| expr_aliases_pointer(tail, aliases)),
        Expr::MethodCall { recv, method, args } if method_is_pointer_arithmetic(method) => {
            expr_aliases_pointer(recv, aliases)
                && args
                    .iter()
                    .all(|arg| !expr_mutates_pointer_alias(arg, aliases, AccessMode::Read))
        }
        _ => false,
    }
}

fn method_is_pointer_arithmetic(method: &str) -> bool {
    matches!(
        method,
        "add" | "sub" | "offset" | "wrapping_add" | "wrapping_sub" | "wrapping_offset"
    )
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
    let (array_name, _) = array_pointer_arg(ptr_arg)?;
    let array_len = *arrays.get(array_name.as_str())?;
    if integer_value(len_arg)? != array_len {
        return None;
    }
    Some((name.as_str().into(), Callsite { array_len }))
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
