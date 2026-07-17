use std::collections::BTreeSet;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingId, BindingKind, CallCallee, CallsiteFact, FixupFacts, FunctionId, PathSegment,
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
    facts
        .ptr_len_slices
        .extend(proven_constant_extent_calls(program, facts, &active));
    facts.ptr_len_slices.sort_by_key(|fact| {
        (
            fact.caller,
            fact.callee,
            fact.ptr_param,
            fact.len_param.unwrap_or(BindingId(usize::MAX)),
        )
    });
    facts.ptr_len_slices.dedup_by_key(|fact| {
        (
            fact.caller,
            fact.callee,
            fact.ptr_param,
            fact.len_param.unwrap_or(BindingId(usize::MAX)),
        )
    });
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
    ptr_name: String,
    len_name: String,
    mutable: bool,
    elem: Type,
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
        for (ptr_index, ptr_param) in f.params.iter().enumerate() {
            let Type::Ptr { mutable, inner } = &ptr_param.ty else {
                continue;
            };
            let Some(ptr) = facts.binding_by_param_index(function, ptr_index) else {
                continue;
            };
            for (len_index, len_param) in f.params.iter().enumerate() {
                if ptr_index == len_index || !is_integer_type(&len_param.ty) {
                    continue;
                }
                let Some(len) = facts.binding_by_param_index(function, len_index) else {
                    continue;
                };
                candidates.push(Candidate {
                    key: Key { function, ptr, len },
                    function_name: f.name.clone(),
                    ptr_index,
                    len_index,
                    ptr_name: ptr_param.name.to_string(),
                    len_name: len_param.name.to_string(),
                    mutable: *mutable && pointer_param_mutated(&f.body, ptr_param.name.as_str()),
                    elem: (**inner).clone(),
                });
            }
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
        && len_reads_are_length_uses(program, candidate, facts, active)
        && all_callers_prove(program, candidate, facts, active)
}

fn len_reads_are_length_uses(
    program: &Program,
    candidate: &Candidate,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
) -> bool {
    let Some(def_use) = facts.def_use(candidate.key.len) else {
        return false;
    };
    if def_use.reads.is_empty() {
        return false;
    }
    let mut allowed = Vec::new();
    collect_length_use_paths(program, candidate, facts, active, &mut allowed);
    def_use.reads.iter().all(|read| {
        allowed
            .iter()
            .any(|path: &AstPath| walk::paths_overlap(&read.0, &path.0))
    })
}

fn collect_length_use_paths(
    program: &Program,
    candidate: &Candidate,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
    allowed: &mut Vec<AstPath>,
) {
    if let Some(item_index) = facts.function_item_index(candidate.key.function)
        && let Some(Item::Fn(f)) = program.items.get(item_index)
    {
        collect_bounded_loop_paths(
            &f.body,
            candidate.key.function,
            candidate,
            facts,
            &mut Vec::new(),
            allowed,
        );
    }
    for callsite in facts
        .callsites
        .iter()
        .filter(|callsite| callsite.site.function == candidate.key.function)
    {
        if call_forwards_pair(callsite, candidate, facts, active) {
            allowed.push(callsite.site.path.clone());
        }
    }
}

fn collect_bounded_loop_paths(
    body: &[IndentStmt],
    function: FunctionId,
    candidate: &Candidate,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    allowed: &mut Vec<AstPath>,
) {
    for (index, indent) in body.iter().enumerate() {
        path.push(PathSegment::Stmt(index));
        collect_bounded_loop_paths_in_stmt(&indent.stmt, function, candidate, facts, path, allowed);
        path.pop();
    }
}

fn collect_bounded_loop_paths_in_stmt(
    stmt: &Stmt,
    function: FunctionId,
    candidate: &Candidate,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    allowed: &mut Vec<AstPath>,
) {
    match stmt {
        Stmt::For { pat, iter, body }
            if range_ends_at_binding(
                iter,
                function,
                &AstPath(path.clone()),
                candidate.key.len,
                facts,
            ) && body_accesses_pointer_index(body, &candidate.ptr_name, pat) =>
        {
            allowed.push(AstPath(path.clone()));
            path.push(PathSegment::ForBody);
            collect_bounded_loop_paths(body, function, candidate, facts, path, allowed);
            path.pop();
        }
        Stmt::For { body, .. } => {
            path.push(PathSegment::ForBody);
            collect_bounded_loop_paths(body, function, candidate, facts, path, allowed);
            path.pop();
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
            path.push(PathSegment::Then);
            collect_bounded_loop_paths(then_body, function, candidate, facts, path, allowed);
            path.pop();
            path.push(PathSegment::Else);
            collect_bounded_loop_paths(else_body, function, candidate, facts, path, allowed);
            path.pop();
        }
        Stmt::Loop { body, .. }
            if lowered_loop_index(body, function, candidate, facts, path).is_some_and(
                |index| body_accesses_pointer_index(body, &candidate.ptr_name, &index),
            ) =>
        {
            allowed.push(AstPath(path.clone()));
            path.push(PathSegment::LoopBody);
            collect_bounded_loop_paths(body, function, candidate, facts, path, allowed);
            path.pop();
        }
        Stmt::Loop { body, .. } => {
            path.push(PathSegment::LoopBody);
            collect_bounded_loop_paths(body, function, candidate, facts, path, allowed);
            path.pop();
        }
        Stmt::Scope { body } => {
            path.push(PathSegment::ScopeBody);
            collect_bounded_loop_paths(body, function, candidate, facts, path, allowed);
            path.pop();
        }
        Stmt::LabeledBlock { body, .. } => {
            path.push(PathSegment::LabeledBody);
            collect_bounded_loop_paths(body, function, candidate, facts, path, allowed);
            path.pop();
        }
        Stmt::Unsafe { body } => {
            path.push(PathSegment::UnsafeBody);
            collect_bounded_loop_paths(&body.stmts, function, candidate, facts, path, allowed);
            path.pop();
        }
        Stmt::While { body, .. } | Stmt::Block(body) => {
            path.push(PathSegment::WhileBody);
            collect_bounded_loop_paths(&body.stmts, function, candidate, facts, path, allowed);
            path.pop();
        }
        Stmt::Match { arms, .. } => {
            for (index, arm) in arms.iter().enumerate() {
                path.push(PathSegment::MatchArm(index));
                collect_bounded_loop_paths(&arm.body, function, candidate, facts, path, allowed);
                path.pop();
            }
        }
        _ => {}
    }
}

fn range_ends_at_binding(
    expr: &Expr,
    function: FunctionId,
    path: &AstPath,
    binding: BindingId,
    facts: &FixupFacts,
) -> bool {
    let Expr::Range { end, .. } = expr else {
        return false;
    };
    resolve_len_binding(end, function, path, facts) == Some(binding)
}

fn body_accesses_pointer_index(body: &[IndentStmt], ptr_name: &str, index_name: &str) -> bool {
    let mut aliases = PointerIndexAliases::new(ptr_name, index_name);
    body.iter()
        .any(|indent| stmt_accesses_pointer_index(&indent.stmt, &mut aliases))
}

fn lowered_loop_index(
    body: &[IndentStmt],
    function: FunctionId,
    candidate: &Candidate,
    facts: &FixupFacts,
    loop_path: &[PathSegment],
) -> Option<String> {
    for (index, indent) in body.iter().enumerate() {
        let Stmt::If {
            cond, then_body, ..
        } = &indent.stmt
        else {
            continue;
        };
        if !matches!(
            then_body.as_slice(),
            [IndentStmt {
                stmt: Stmt::Break(None),
                ..
            }]
        ) {
            continue;
        }
        let mut path = loop_path.to_vec();
        path.push(PathSegment::LoopBody);
        path.push(PathSegment::Stmt(index));
        if let Some(index) =
            loop_bound_index(cond, function, &AstPath(path), candidate.key.len, facts)
        {
            return Some(index);
        }
    }
    None
}

fn loop_bound_index(
    cond: &Expr,
    function: FunctionId,
    path: &AstPath,
    len: BindingId,
    facts: &FixupFacts,
) -> Option<String> {
    let Expr::Unary {
        op: UnaryOp::Not,
        expr,
    } = cond
    else {
        return None;
    };
    let Expr::Binary { op, lhs, rhs } = peel_cast(expr) else {
        return None;
    };
    if !matches!(op, crate::rust_ast::BinOp::Lt)
        || resolve_len_binding(rhs, function, path, facts) != Some(len)
    {
        return None;
    }
    let Expr::Var(index) = peel_cast(lhs) else {
        return None;
    };
    Some(index.to_string())
}

#[derive(Clone)]
struct PointerIndexAliases {
    ptrs: BTreeSet<String>,
    indexes: BTreeSet<String>,
}

impl PointerIndexAliases {
    fn new(ptr_name: &str, index_name: &str) -> Self {
        Self {
            ptrs: BTreeSet::from([ptr_name.to_string()]),
            indexes: BTreeSet::from([index_name.to_string()]),
        }
    }

    fn is_ptr(&self, expr: &Expr) -> bool {
        matches!(peel_cast(expr), Expr::Var(name) if self.ptrs.contains(name.as_str()))
    }

    fn is_index(&self, expr: &Expr) -> bool {
        matches!(peel_cast(expr), Expr::Var(name) if self.indexes.contains(name.as_str()))
    }
}

fn stmt_accesses_pointer_index(stmt: &Stmt, aliases: &mut PointerIndexAliases) -> bool {
    match stmt {
        Stmt::Let { name, init, .. } => {
            if let Some(init) = init {
                if expr_accesses_pointer_index(init, aliases) {
                    return true;
                }
                if aliases.is_ptr(peel_pointer_view(init)) {
                    aliases.ptrs.insert(name.clone());
                }
                if aliases.is_index(init) {
                    aliases.indexes.insert(name.clone());
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
            expr_accesses_pointer_index(cond, aliases)
                || body_accesses_pointer_index_with_aliases(then_body, &mut aliases.clone())
                || expr_accesses_pointer_index(then_value, &mut aliases.clone())
                || body_accesses_pointer_index_with_aliases(else_body, &mut aliases.clone())
                || expr_accesses_pointer_index(else_value, &mut aliases.clone())
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_accesses_pointer_index(target, aliases)
                || expr_accesses_pointer_index(value, aliases)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_accesses_pointer_index(expr, aliases),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_accesses_pointer_index(cond, aliases)
                || body_accesses_pointer_index_with_aliases(then_body, &mut aliases.clone())
                || body_accesses_pointer_index_with_aliases(else_body, &mut aliases.clone())
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => {
            body_accesses_pointer_index_with_aliases(body, &mut aliases.clone())
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            body_accesses_pointer_index_with_aliases(&body.stmts, &mut aliases.clone())
                || body
                    .tail
                    .as_deref()
                    .is_some_and(|tail| expr_accesses_pointer_index(tail, &mut aliases.clone()))
        }
        Stmt::Match { expr, arms } => {
            expr_accesses_pointer_index(expr, aliases)
                || arms.iter().any(|arm| {
                    body_accesses_pointer_index_with_aliases(&arm.body, &mut aliases.clone())
                })
        }
    }
}

fn body_accesses_pointer_index_with_aliases(
    body: &[IndentStmt],
    aliases: &mut PointerIndexAliases,
) -> bool {
    body.iter()
        .any(|indent| stmt_accesses_pointer_index(&indent.stmt, aliases))
}

fn expr_accesses_pointer_index(expr: &Expr, aliases: &mut PointerIndexAliases) -> bool {
    if pointer_index_expr(expr, aliases) {
        return true;
    }
    walk::exprs_any(expr, &mut |expr| pointer_index_expr(expr, aliases))
}

fn pointer_index_expr(expr: &Expr, aliases: &PointerIndexAliases) -> bool {
    match expr {
        Expr::Index { base, index } => aliases.is_ptr(base) && aliases.is_index(index),
        Expr::MethodCall { .. } => pointer_offset_expr(expr, aliases),
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => pointer_offset_expr(expr, aliases),
        Expr::Unsafe(block) | Expr::Block(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| pointer_index_expr(tail, aliases)),
        _ => false,
    }
}

fn pointer_offset_expr(expr: &Expr, aliases: &PointerIndexAliases) -> bool {
    let Expr::MethodCall { recv, method, args } = peel_cast(expr) else {
        return false;
    };
    matches!(
        method.as_str(),
        "offset" | "add" | "wrapping_offset" | "wrapping_add"
    ) && aliases.is_ptr(recv)
        && matches!(args.as_slice(), [arg] if aliases.is_index(arg))
}

fn call_forwards_pair(
    callsite: &CallsiteFact,
    candidate: &Candidate,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
) -> bool {
    let CallCallee::Direct { name, .. } = &callsite.callee else {
        return false;
    };
    let Some(callee) = facts
        .functions
        .iter()
        .find(|function| function.name == *name)
        .map(|function| function.id)
    else {
        return false;
    };
    active
        .iter()
        .filter(|key| key.function == callee)
        .any(|key| call_forwards_to_key(callsite, candidate, *key, facts))
}

fn call_forwards_to_key(
    callsite: &CallsiteFact,
    candidate: &Candidate,
    target: Key,
    facts: &FixupFacts,
) -> bool {
    let Some(ptr_index) = param_index(facts, target.ptr) else {
        return false;
    };
    let Some(len_index) = param_index(facts, target.len) else {
        return false;
    };
    let Some(ptr_arg) = callsite.args.iter().find(|arg| arg.slot == ptr_index) else {
        return false;
    };
    let Some(len_arg) = callsite.args.iter().find(|arg| arg.slot == len_index) else {
        return false;
    };
    let ptr_ok = facts
        .binding_read_under(
            callsite.site.function,
            candidate.ptr_name.as_str(),
            &ptr_arg.path,
        )
        .or_else(|| facts.binding_named(callsite.site.function, candidate.ptr_name.as_str()))
        == Some(candidate.key.ptr);
    let len_ok = facts
        .binding_read_under(
            callsite.site.function,
            candidate.len_name.as_str(),
            &len_arg.path,
        )
        .or_else(|| facts.binding_named(callsite.site.function, candidate.len_name.as_str()))
        == Some(candidate.key.len);
    ptr_ok && len_ok
}

fn param_index(facts: &FixupFacts, binding: BindingId) -> Option<usize> {
    facts
        .bindings
        .iter()
        .find(|fact| fact.id == binding)
        .and_then(|fact| match fact.kind {
            BindingKind::Param { index } => Some(index),
            BindingKind::Local => None,
        })
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

fn proven_constant_extent_calls(
    program: &Program,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
) -> Vec<PtrLenSliceFact> {
    let mut out = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        for (ptr_index, param) in f.params.iter().enumerate() {
            let Type::Ptr { mutable, inner } = &param.ty else {
                continue;
            };
            let Some(ptr) = facts.binding_by_param_index(function, ptr_index) else {
                continue;
            };
            if !facts
                .def_use(ptr)
                .is_some_and(|def_use| def_use.writes.is_empty())
            {
                continue;
            }
            let bounds = constant_pointer_extents(&f.body, param.name.as_str());
            let [bound] = bounds.as_slice() else {
                continue;
            };
            let candidate = PointerCandidate {
                function,
                function_name: f.name.clone(),
                ptr,
                ptr_index,
                mutable: *mutable && pointer_param_mutated(&f.body, param.name.as_str()),
                elem: (**inner).clone(),
                bound: *bound,
            };
            if all_callers_prove_pointer_extent(program, &candidate, facts, active) {
                out.extend(proven_pointer_calls(facts, &candidate));
            }
        }
    }
    out
}

struct PointerCandidate {
    function: FunctionId,
    function_name: String,
    ptr: BindingId,
    ptr_index: usize,
    mutable: bool,
    elem: Type,
    bound: u64,
}

fn constant_pointer_extents(body: &[IndentStmt], ptr_name: &str) -> Vec<u64> {
    let mut bounds = BTreeSet::new();
    collect_constant_pointer_extents(body, ptr_name, &mut bounds);
    bounds.into_iter().collect()
}

fn collect_constant_pointer_extents(
    body: &[IndentStmt],
    ptr_name: &str,
    bounds: &mut BTreeSet<u64>,
) {
    for pair in body.windows(2) {
        let Stmt::Let {
            name: index_name,
            init: Some(init),
            ..
        } = &pair[0].stmt
        else {
            continue;
        };
        if integer_value(init) != Some(0) {
            continue;
        }
        let Stmt::Loop {
            body: loop_body, ..
        } = &pair[1].stmt
        else {
            continue;
        };
        if let Some(bound) = constant_lowered_loop_bound(loop_body, index_name.as_str())
            && body_accesses_pointer_index(loop_body, ptr_name, index_name)
        {
            bounds.insert(bound);
        }
    }
    for indent in body {
        collect_constant_pointer_extents_stmt(&indent.stmt, ptr_name, bounds);
    }
}

fn constant_lowered_loop_bound(body: &[IndentStmt], index_name: &str) -> Option<u64> {
    let first = body.first()?;
    let Stmt::If {
        cond, then_body, ..
    } = &first.stmt
    else {
        return None;
    };
    if !matches!(
        then_body.as_slice(),
        [IndentStmt {
            stmt: Stmt::Break(None),
            ..
        }]
    ) {
        return None;
    }
    let Expr::Unary {
        op: UnaryOp::Not,
        expr,
    } = cond
    else {
        return None;
    };
    let Expr::Binary { op, lhs, rhs } = peel_cast(expr) else {
        return None;
    };
    if !matches!(op, crate::rust_ast::BinOp::Lt) {
        return None;
    }
    let Expr::Var(index) = peel_cast(lhs) else {
        return None;
    };
    (index.as_str() == index_name)
        .then(|| integer_value(rhs))
        .flatten()
}

fn collect_constant_pointer_extents_stmt(stmt: &Stmt, ptr_name: &str, bounds: &mut BTreeSet<u64>) {
    match stmt {
        Stmt::For { pat, iter, body } => {
            if let Expr::Range { end, .. } = iter
                && let Some(bound) = integer_value(end)
                && body_accesses_pointer_index(body, ptr_name, pat)
            {
                bounds.insert(bound);
            }
            collect_constant_pointer_extents(body, ptr_name, bounds);
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
            collect_constant_pointer_extents(then_body, ptr_name, bounds);
            collect_constant_pointer_extents(else_body, ptr_name, bounds);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            collect_constant_pointer_extents(body, ptr_name, bounds)
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            collect_constant_pointer_extents(&body.stmts, ptr_name, bounds);
        }
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_constant_pointer_extents(&arm.body, ptr_name, bounds);
            }
        }
        _ => {}
    }
}

fn all_callers_prove_pointer_extent(
    program: &Program,
    candidate: &PointerCandidate,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
) -> bool {
    let calls = matching_callsites(facts, &candidate.function_name).collect::<Vec<_>>();
    !calls.is_empty()
        && calls
            .iter()
            .all(|callsite| call_proves_pointer_extent(program, callsite, candidate, facts, active))
}

fn call_proves_pointer_extent(
    program: &Program,
    callsite: &CallsiteFact,
    candidate: &PointerCandidate,
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
    let Some(ptr_expr) = walk::expr_at_path(facts, program, callsite.site.function, &ptr_arg.path)
    else {
        return false;
    };
    matches!(
        pointer_length_source(
            program,
            ptr_expr,
            callsite.site.function,
            &ptr_arg.path,
            facts,
            active,
            0,
        ),
        Some(LengthSource::Const(n)) if n == candidate.bound
    )
}

fn proven_pointer_calls(facts: &FixupFacts, candidate: &PointerCandidate) -> Vec<PtrLenSliceFact> {
    matching_callsites(facts, &candidate.function_name)
        .map(|callsite| PtrLenSliceFact {
            caller: callsite.site.function,
            callee: candidate.function,
            ptr_param: candidate.ptr,
            len_param: None,
            mutable: candidate.mutable,
            elem_ty: candidate.elem.clone(),
        })
        .collect()
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
    let Some(ptr_expr) = walk::expr_at_path(facts, program, callsite.site.function, &ptr_arg.path)
    else {
        return false;
    };
    let Some(len_expr) = walk::expr_at_path(facts, program, callsite.site.function, &len_arg.path)
    else {
        return false;
    };
    let Some(source) = pointer_length_source(
        program,
        ptr_expr,
        callsite.site.function,
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
            resolve_len_binding(len_expr, callsite.site.function, &len_arg.path, facts)
                == Some(target)
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
            caller: callsite.site.function,
            callee: candidate.key.function,
            ptr_param: candidate.key.ptr,
            len_param: Some(candidate.key.len),
            mutable: candidate.mutable,
            elem_ty: candidate.elem.clone(),
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
