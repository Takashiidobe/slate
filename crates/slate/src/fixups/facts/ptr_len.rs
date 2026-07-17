use std::collections::BTreeSet;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingId, BindingKind, CallCallee, CallsiteFact, FixupFacts, FunctionId,
    PtrLenSliceFact,
};
use crate::rust_ast::{
    Block, Expr, IndentStmt, Item, Prim, Program, RustValue, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.ptr_len_slices.clear();

    let candidates = collect_candidates(program, facts);
    let mut active = candidates
        .iter()
        .map(|candidate| candidate.key)
        .collect::<BTreeSet<_>>();

    loop {
        let before = active.clone();
        active.retain(|key| {
            candidates
                .iter()
                .find(|candidate| candidate.key == *key)
                .is_some_and(|candidate| candidate_is_sound(program, candidate, facts, &before))
        });
        if active == before {
            break;
        }
    }

    facts.ptr_len_slices = candidates
        .iter()
        .filter(|candidate| active.contains(&candidate.key))
        .flat_map(|candidate| proven_calls(facts, candidate))
        .collect();
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
struct Key {
    function: FunctionId,
    ptr: BindingId,
    len: BindingId,
}

#[derive(Clone)]
struct Candidate {
    key: Key,
    function_name: String,
    ptr_index: usize,
    len_index: usize,
    mutable: bool,
    elem: Type,
    len_ty: Type,
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum LengthSource {
    Const(u64),
    Bound(BindingId),
}

fn collect_candidates(program: &Program, facts: &FixupFacts) -> Vec<Candidate> {
    let mut candidates = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        for (i, pair) in f.params.windows(2).enumerate() {
            let Type::Ptr { mutable, inner } = &pair[0].ty else {
                continue;
            };
            if !is_integer_type(&pair[1].ty) {
                continue;
            }
            let Some(ptr) = facts.binding_by_param_index(function, i) else {
                continue;
            };
            let Some(len) = facts.binding_by_param_index(function, i + 1) else {
                continue;
            };
            candidates.push(Candidate {
                key: Key { function, ptr, len },
                function_name: f.name.clone(),
                ptr_index: i,
                len_index: i + 1,
                mutable: *mutable && pointer_param_mutated(&f.body, pair[0].name.as_str()),
                elem: (**inner).clone(),
                len_ty: pair[1].ty.clone(),
            });
        }
    }
    candidates
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

fn candidate_is_sound(
    program: &Program,
    candidate: &Candidate,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
) -> bool {
    facts
        .def_use(candidate.key.ptr)
        .is_some_and(|def_use| def_use.writes.is_empty())
        && facts
            .def_use(candidate.key.len)
            .is_some_and(|def_use| def_use.writes.is_empty())
        && all_callers_prove(program, candidate, facts, active)
}

fn all_callers_prove(
    program: &Program,
    candidate: &Candidate,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
) -> bool {
    let calls = matching_callsites(facts, &candidate.function_name).collect::<Vec<_>>();
    !calls.is_empty()
        && calls
            .iter()
            .all(|callsite| call_proves_pair(program, callsite, candidate, facts, active))
}

fn matching_callsites<'a>(
    facts: &'a FixupFacts,
    function_name: &'a str,
) -> impl Iterator<Item = &'a CallsiteFact> {
    facts
        .callsites
        .iter()
        .filter(move |callsite| match &callsite.callee {
            CallCallee::Direct { name, .. } => name == function_name,
            CallCallee::Indirect => false,
        })
}

fn call_proves_pair(
    program: &Program,
    callsite: &CallsiteFact,
    candidate: &Candidate,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
) -> bool {
    let Some(ptr_arg) = callsite
        .args
        .iter()
        .find(|arg| arg.slot == candidate.ptr_index)
    else {
        return false;
    };
    let Some(len_arg) = callsite
        .args
        .iter()
        .find(|arg| arg.slot == candidate.len_index)
    else {
        return false;
    };
    let Some(ptr_expr) = walk::expr_at_path(facts, program, callsite.function, &ptr_arg.path)
    else {
        return false;
    };
    let Some(len_expr) = walk::expr_at_path(facts, program, callsite.function, &len_arg.path)
    else {
        return false;
    };
    let Some(source) = pointer_length_source(
        program,
        ptr_expr,
        callsite.function,
        &ptr_arg.path,
        facts,
        active,
        0,
    ) else {
        return false;
    };
    match source {
        LengthSource::Const(n) => integer_value(len_expr) == Some(n),
        LengthSource::Bound(target) => {
            resolve_len_binding(len_expr, callsite.function, &len_arg.path, facts) == Some(target)
        }
    }
}

const MAX_ALIAS_DEPTH: u32 = 8;

/// Resolves a pointer-argument expression to a proof of how many elements it
/// points at: either a compile-time-constant array length, or the identity of
/// another binding proven (by an active candidate) to carry the same length.
fn pointer_length_source(
    program: &Program,
    expr: &Expr,
    function: FunctionId,
    path: &AstPath,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
    depth: u32,
) -> Option<LengthSource> {
    if depth > MAX_ALIAS_DEPTH {
        return None;
    }
    let Expr::Var(name) = peel_pointer_view(expr) else {
        return None;
    };
    let binding = facts
        .binding_read_under(function, name.as_str(), path)
        .or_else(|| facts.binding_named(function, name.as_str()))?;
    binding_length_source(program, binding, function, facts, active, depth)
}

fn binding_length_source(
    program: &Program,
    binding: BindingId,
    function: FunctionId,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
    depth: u32,
) -> Option<LengthSource> {
    if let Some(len) = facts.binding_type(binding).and_then(array_len_from_type) {
        return Some(LengthSource::Const(len));
    }
    if let Some(key) = active
        .iter()
        .find(|key| key.function == function && key.ptr == binding)
    {
        return Some(LengthSource::Bound(key.len));
    }
    let local = facts
        .bindings
        .iter()
        .find(|fact| fact.id == binding && fact.kind == BindingKind::Local)?;
    if !facts
        .def_use(binding)
        .is_some_and(|def_use| def_use.writes.is_empty())
    {
        return None;
    }
    let init = walk::expr_at_path(facts, program, function, &local.path)?;
    pointer_length_source(
        program,
        init,
        function,
        &local.path,
        facts,
        active,
        depth + 1,
    )
}

fn array_len_from_type(rendered: &str) -> Option<u64> {
    match Type::parse(rendered) {
        Type::Array { len, .. } => Some(len),
        _ => None,
    }
}

fn resolve_len_binding(
    expr: &Expr,
    function: FunctionId,
    path: &AstPath,
    facts: &FixupFacts,
) -> Option<BindingId> {
    let Expr::Var(name) = peel_cast(expr) else {
        return None;
    };
    facts
        .binding_read_under(function, name.as_str(), path)
        .or_else(|| facts.binding_named(function, name.as_str()))
}

fn peel_cast(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_cast(expr),
        _ => expr,
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

fn integer_value(expr: &Expr) -> Option<u64> {
    match expr {
        Expr::Value(RustValue::I64(n)) => u64::try_from(*n).ok(),
        Expr::Value(RustValue::I128(n)) => u64::try_from(*n).ok(),
        Expr::Cast { expr, .. } => integer_value(expr),
        _ => None,
    }
}

fn proven_calls(facts: &FixupFacts, candidate: &Candidate) -> Vec<PtrLenSliceFact> {
    matching_callsites(facts, &candidate.function_name)
        .map(|callsite| PtrLenSliceFact {
            caller: callsite.function,
            callee: candidate.key.function,
            ptr_param: candidate.key.ptr,
            len_param: candidate.key.len,
            mutable: candidate.mutable,
            elem_ty: candidate.elem.clone(),
            len_ty: candidate.len_ty.clone(),
        })
        .collect()
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
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => place
            .ptr_expr()
            .is_some_and(|ptr| expr_mutates_pointer_alias(ptr, aliases, AccessMode::Read)),
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            place
                .ptr_expr()
                .is_some_and(|ptr| expr_mutates_pointer_alias(ptr, aliases, AccessMode::Mutate))
                || expr_mutates_pointer_alias(value, aliases, AccessMode::Read)
        }
        Expr::AtomicNew { value, .. } => {
            expr_mutates_pointer_alias(value, aliases, AccessMode::Read)
        }
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            place
                .ptr_expr()
                .is_some_and(|ptr| expr_mutates_pointer_alias(ptr, aliases, AccessMode::Mutate))
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
