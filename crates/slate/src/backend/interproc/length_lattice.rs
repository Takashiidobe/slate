use std::collections::{BTreeMap, BTreeSet, VecDeque};

use crate::backend::interproc::pointer_lattice::{PointerBinding, PointerFact, ResolvedPtrType};
use crate::backend::rust_ast::{
    BinOp, Block, Expr, FnDef, Ident, IndentStmt, Item, Path, Prim, Program, Stmt, Type, UnaryOp,
    Visibility,
};
use crate::function_identity::{CallBinding, Known};

#[derive(Clone)]
pub(in crate::backend) struct Pairing {
    len_param: String,
    idx_name: String,
    kind: BufferKind,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum BufferKind {
    Shared,
    Mutable,
    Owned,
    Str,
    StringOwned,
}

impl BufferKind {
    fn mutable(self) -> bool {
        matches!(self, Self::Mutable | Self::Owned | Self::StringOwned)
    }

    fn owned(self) -> bool {
        matches!(self, Self::Owned | Self::StringOwned)
    }
}

pub(in crate::backend) fn compute(
    program: &Program,
    facts: &BTreeMap<PointerBinding, PointerFact>,
) -> BTreeMap<PointerBinding, Pairing> {
    let mut out = BTreeMap::new();
    collect_items(&program.items, facts, &mut out);
    out
}

pub(in crate::backend) fn apply(
    program: &mut Program,
    facts: &BTreeMap<PointerBinding, PointerFact>,
) {
    let pairings = compute(program, facts);
    if !pairings.is_empty() {
        let lifted = apply_signature_lifts(program, &pairings, facts);
        let remaining = pairings
            .into_iter()
            .filter(|(binding, pairing)| !lifted.contains(binding) && !pairing.kind.owned())
            .collect();
        apply_items(&mut program.items, &remaining);
    }
    apply_const_length(program, facts);
    apply_return_buffers(program);
}

#[derive(Clone)]
struct SignatureCandidate {
    binding: PointerBinding,
    function: String,
    ptr_param: String,
    len_param: String,
    ptr_index: usize,
    len_index: usize,
    kind: BufferKind,
    elem_ty: Type,
    raw_mutable: bool,
    seeded: bool,
    len_reads: usize,
    stable: bool,
}

#[derive(Debug, Clone, Copy)]
enum CallProof {
    Exact,
    Forwarded(usize),
}

struct CallSiteProof {
    caller: String,
    callee: String,
    target: usize,
    proof: Option<CallProof>,
}

#[derive(Clone, PartialEq, Eq)]
struct ArraySource {
    elem_ty: Type,
    len: u64,
}

#[derive(Clone, Default)]
struct ArrayCatalog {
    sources: BTreeMap<String, ArraySource>,
    ambiguous: BTreeSet<String>,
}

impl ArrayCatalog {
    fn insert(&mut self, name: String, source: ArraySource) {
        if self.ambiguous.contains(&name) {
            return;
        }
        if self.sources.remove(&name).is_some() {
            self.ambiguous.insert(name);
        } else {
            self.sources.insert(name, source);
        }
    }

    fn contains_key(&self, name: &str) -> bool {
        self.sources.contains_key(name)
    }

    fn get(&self, name: &str) -> Option<&ArraySource> {
        self.sources.get(name)
    }
}

fn apply_signature_lifts(
    program: &mut Program,
    pairings: &BTreeMap<PointerBinding, Pairing>,
    facts: &BTreeMap<PointerBinding, PointerFact>,
) -> BTreeSet<PointerBinding> {
    let (candidates, sites) = {
        let fn_defs = collect_fn_defs(&program.items);
        let global_arrays = collect_global_arrays(&program.items);
        let candidates = signature_candidates(&fn_defs, pairings, facts);
        if candidates.is_empty() {
            return BTreeSet::new();
        }
        let sites = signature_call_sites(&fn_defs, &candidates, &global_arrays);
        (candidates, sites)
    };

    let mut active = vec![true; candidates.len()];
    let mut call_counts = vec![0usize; candidates.len()];
    let mut forwarded_counts = vec![0usize; candidates.len()];
    let mut dependents = vec![Vec::new(); candidates.len()];
    for site in &sites {
        call_counts[site.target] += 1;
        match site.proof {
            Some(CallProof::Exact) => {}
            Some(CallProof::Forwarded(source)) => {
                forwarded_counts[source] += 1;
                dependents[source].push(site.target);
            }
            None => active[site.target] = false,
        }
    }
    for (id, candidate) in candidates.iter().enumerate() {
        let forwarding_only = forwarded_counts[id] > 0
            && candidate.len_reads == forwarded_counts[id]
            && candidate.stable;
        active[id] &= candidate.seeded || forwarding_only;
    }
    for (id, count) in call_counts.into_iter().enumerate() {
        if count == 0 {
            active[id] = false;
        }
    }

    if std::env::var_os("SLATE_PTR_LEN_DEBUG").is_some() {
        for (id, candidate) in candidates.iter().enumerate() {
            eprintln!(
                "ptr_len[{id}] {}::({}, {}) kind={:?} elem={:?} seed={} reads={} forwarded={} active={}",
                candidate.function,
                candidate.ptr_param,
                candidate.len_param,
                candidate.kind,
                candidate.elem_ty,
                candidate.seeded,
                candidate.len_reads,
                forwarded_counts[id],
                active[id]
            );
        }
        for site in &sites {
            eprintln!(
                "ptr_len call {} -> {} target={} proof={:?}",
                site.caller, site.callee, site.target, site.proof
            );
        }
    }

    let mut queue: VecDeque<usize> = active
        .iter()
        .enumerate()
        .filter_map(|(id, active)| (!active).then_some(id))
        .collect();
    while let Some(source) = queue.pop_front() {
        for &target in &dependents[source] {
            if active[target] {
                active[target] = false;
                queue.push_back(target);
            }
        }
    }

    let lifted: BTreeSet<PointerBinding> = candidates
        .iter()
        .enumerate()
        .filter(|(id, _)| active[*id])
        .map(|(_, candidate)| candidate.binding.clone())
        .collect();
    if lifted.is_empty() {
        return lifted;
    }

    let accepted: BTreeMap<String, Vec<usize>> = candidates
        .iter()
        .enumerate()
        .filter(|(id, _)| active[*id])
        .fold(BTreeMap::new(), |mut map, (id, candidate)| {
            map.entry(candidate.function.clone()).or_default().push(id);
            map
        });
    let mut proofs: BTreeMap<(String, String), VecDeque<CallProof>> = BTreeMap::new();
    for site in sites {
        if !active[site.target] {
            continue;
        }
        let Some(proof) = site.proof else {
            continue;
        };
        proofs
            .entry((site.caller, site.callee))
            .or_default()
            .push_back(proof);
    }
    apply_signature_items(&mut program.items, &accepted, &candidates, &mut proofs);
    lifted
}

fn collect_fn_defs(items: &[Item]) -> BTreeMap<String, &FnDef> {
    let mut out = BTreeMap::new();
    collect_fn_defs_into(items, &mut out);
    out
}

fn collect_fn_defs_into<'a>(items: &'a [Item], out: &mut BTreeMap<String, &'a FnDef>) {
    for item in items {
        match item {
            Item::Fn(f) => {
                out.insert(f.name.clone(), f);
            }
            Item::InlineMod { items, .. } => collect_fn_defs_into(items, out),
            _ => {}
        }
    }
}

fn collect_global_arrays(items: &[Item]) -> ArrayCatalog {
    let mut out = ArrayCatalog::default();
    collect_global_arrays_into(items, &mut out);
    out
}

fn collect_global_arrays_into(items: &[Item], out: &mut ArrayCatalog) {
    for item in items {
        match item {
            Item::Static { name, ty, .. } | Item::Const { name, ty, .. } => {
                if let Some(source) = array_source(ty) {
                    out.insert(name.clone(), source);
                }
            }
            Item::InlineMod { items, .. } => collect_global_arrays_into(items, out),
            _ => {}
        }
    }
}

fn array_source(ty: &Type) -> Option<ArraySource> {
    match ty {
        Type::Array { elem, len } => Some(ArraySource {
            elem_ty: (**elem).clone(),
            len: *len,
        }),
        Type::Generic { name, args }
            if name == "aligned::Aligned"
                && let [_, inner] = args.as_slice() =>
        {
            array_source(inner)
        }
        _ => None,
    }
}

fn signature_candidates(
    fn_defs: &BTreeMap<String, &FnDef>,
    pairings: &BTreeMap<PointerBinding, Pairing>,
    facts: &BTreeMap<PointerBinding, PointerFact>,
) -> Vec<SignatureCandidate> {
    let mut out = Vec::new();
    for (function, f) in fn_defs {
        for (ptr_index, ptr_param) in f.params.iter().enumerate() {
            let Type::Ptr {
                mutable: raw_mutable,
                inner,
            } = &ptr_param.ty
            else {
                continue;
            };
            let binding = PointerBinding {
                function: function.clone(),
                name: ptr_param.name.clone(),
            };
            let Some(fact) = facts.get(&binding) else {
                continue;
            };
            let kind = match fact.resolved().base {
                ResolvedPtrType::Slice => BufferKind::Shared,
                ResolvedPtrType::SliceMut => BufferKind::Mutable,
                ResolvedPtrType::Vec => BufferKind::Owned,
                ResolvedPtrType::Str => BufferKind::Str,
                ResolvedPtrType::StringOwned => BufferKind::StringOwned,
                _ => continue,
            };
            for (len_index, len_param) in f.params.iter().enumerate() {
                if ptr_index == len_index || !is_integer_type(&len_param.ty) {
                    continue;
                }
                let ptr_local = shadow_local_name(&f.body, &ptr_param.name)
                    .unwrap_or_else(|| ptr_param.name.clone());
                let len_local = shadow_local_name(&f.body, &len_param.name)
                    .unwrap_or_else(|| len_param.name.clone());
                let seeded = pairings
                    .get(&binding)
                    .is_some_and(|pairing| pairing.len_param == len_param.name);
                out.push(SignatureCandidate {
                    binding: binding.clone(),
                    function: function.clone(),
                    ptr_param: ptr_param.name.clone(),
                    len_param: len_param.name.clone(),
                    ptr_index,
                    len_index,
                    kind,
                    elem_ty: (**inner).clone(),
                    raw_mutable: *raw_mutable,
                    seeded,
                    len_reads: count_var_reads(&f.body, &len_local),
                    stable: !assigned_more_than_once(&f.body, &ptr_local)
                        && !assigned_more_than_once(&f.body, &len_local),
                });
            }
        }
    }
    out
}

fn signature_call_sites(
    fn_defs: &BTreeMap<String, &FnDef>,
    candidates: &[SignatureCandidate],
    global_arrays: &ArrayCatalog,
) -> Vec<CallSiteProof> {
    let by_callee: BTreeMap<&str, Vec<usize>> =
        candidates
            .iter()
            .enumerate()
            .fold(BTreeMap::new(), |mut map, (id, candidate)| {
                map.entry(candidate.function.as_str()).or_default().push(id);
                map
            });
    let by_caller: BTreeMap<&str, Vec<usize>> =
        candidates
            .iter()
            .enumerate()
            .fold(BTreeMap::new(), |mut map, (id, candidate)| {
                map.entry(candidate.function.as_str()).or_default().push(id);
                map
            });
    let mut out = Vec::new();
    for (caller, f) in fn_defs {
        let defs = collect_value_defs(&f.body);
        let mut arrays = global_arrays.clone();
        collect_local_arrays(&f.body, &mut arrays);
        let mut calls = Vec::new();
        for indent in &f.body {
            indent.stmt.collect_calls(&mut calls);
        }
        for (callee, args) in calls {
            let Some(targets) = by_callee.get(callee.as_str()) else {
                continue;
            };
            for &target in targets {
                let candidate = &candidates[target];
                let proof = call_args(args, candidate).and_then(|(ptr_arg, len_arg)| {
                    let exact = match candidate.kind {
                        BufferKind::Shared | BufferKind::Mutable => {
                            exact_allocation_pair(ptr_arg, len_arg, &candidate.elem_ty, &defs)
                                || exact_array_pair(
                                    ptr_arg,
                                    len_arg,
                                    &candidate.elem_ty,
                                    &defs,
                                    &arrays,
                                )
                        }
                        BufferKind::Owned => {
                            exact_allocation_pair(ptr_arg, len_arg, &candidate.elem_ty, &defs)
                        }
                        BufferKind::Str => exact_utf8_array_pair(ptr_arg, len_arg, &defs),
                        BufferKind::StringOwned => {
                            exact_allocation_pair(ptr_arg, len_arg, &candidate.elem_ty, &defs)
                                && utf8_owned_fill_proof(
                                    &f.body, ptr_arg, len_arg, &defs, &by_callee,
                                )
                        }
                    };
                    if exact {
                        return Some(CallProof::Exact);
                    }
                    by_caller
                        .get(caller.as_str())
                        .into_iter()
                        .flatten()
                        .find(|&&source| {
                            candidates[source].kind == candidate.kind
                                && forwarded_pair(ptr_arg, len_arg, &candidates[source], &defs)
                        })
                        .copied()
                        .map(CallProof::Forwarded)
                });
                out.push(CallSiteProof {
                    caller: caller.clone(),
                    callee: callee.to_string(),
                    target,
                    proof,
                });
            }
        }
    }
    out
}

fn call_args<'a>(args: &'a [Expr], candidate: &SignatureCandidate) -> Option<(&'a Expr, &'a Expr)> {
    Some((
        args.get(candidate.ptr_index)?,
        args.get(candidate.len_index)?,
    ))
}

fn forwarded_pair(
    ptr_arg: &Expr,
    len_arg: &Expr,
    source: &SignatureCandidate,
    defs: &BTreeMap<String, Expr>,
) -> bool {
    canonical_var(ptr_arg, defs).as_deref() == Some(source.ptr_param.as_str())
        && canonical_var(len_arg, defs).as_deref() == Some(source.len_param.as_str())
}

fn exact_allocation_pair(
    ptr_arg: &Expr,
    len_arg: &Expr,
    elem_ty: &Type,
    defs: &BTreeMap<String, Expr>,
) -> bool {
    let Some(len_name) = canonical_var(len_arg, defs) else {
        return false;
    };
    let Some(ptr_name) = canonical_var(ptr_arg, defs) else {
        return false;
    };
    let Some(source) = defs.get(&ptr_name) else {
        return false;
    };
    if std::env::var_os("SLATE_PTR_LEN_DEBUG").is_some() {
        eprintln!(
            "ptr_len exact ptr={ptr_name} len={len_name} len_source={:?} source={source:?}",
            defs.get(&len_name)
        );
    }
    let call @ Expr::Call { args, .. } = peel_value(source) else {
        return false;
    };
    if !is_malloc_call(call) {
        return false;
    }
    let [size] = args.as_slice() else {
        return false;
    };
    let Some(elem_size) = primitive_size(elem_ty) else {
        return false;
    };
    let resolved_size = resolved_expr(size, defs, 0);
    let Expr::Binary {
        op: BinOp::Mul,
        lhs,
        rhs,
    } = peel_value(&resolved_size)
    else {
        return false;
    };
    if std::env::var_os("SLATE_PTR_LEN_DEBUG").is_some() {
        eprintln!(
            "ptr_len size lhs={lhs:?} canon={:?} rhs={rhs:?} canon={:?} lhs_int={:?} rhs_int={:?}",
            canonical_var(lhs, defs),
            canonical_var(rhs, defs),
            resolved_integer(lhs, defs, 0),
            resolved_integer(rhs, defs, 0)
        );
    }
    (same_value(lhs, len_arg, defs) && resolved_integer(rhs, defs, 0) == Some(elem_size))
        || (same_value(rhs, len_arg, defs) && resolved_integer(lhs, defs, 0) == Some(elem_size))
}

fn exact_array_pair(
    ptr_arg: &Expr,
    len_arg: &Expr,
    elem_ty: &Type,
    defs: &BTreeMap<String, Expr>,
    arrays: &ArrayCatalog,
) -> bool {
    let Some(len) = resolved_integer(len_arg, defs, 0) else {
        return false;
    };
    let Some(root) = array_root_name(ptr_arg, defs, arrays, 0) else {
        return false;
    };
    arrays
        .get(&root)
        .is_some_and(|source| source.len == len && source.elem_ty == *elem_ty)
}

fn array_root_name(
    expr: &Expr,
    defs: &BTreeMap<String, Expr>,
    arrays: &ArrayCatalog,
    depth: usize,
) -> Option<String> {
    if depth > 32 {
        return None;
    }
    match peel_value(expr) {
        Expr::Var(name) if arrays.contains_key(name.as_str()) => Some(name.to_string()),
        Expr::Var(name) => array_root_name(defs.get(name.as_str())?, defs, arrays, depth + 1),
        Expr::ArrayPtr { array, .. } => array_root_name(array, defs, arrays, depth + 1),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            array_root_name(recv, defs, arrays, depth + 1)
        }
        Expr::MethodCallGeneric {
            recv, method, args, ..
        } if args.is_empty() && method == "cast" => array_root_name(recv, defs, arrays, depth + 1),
        Expr::AddrOf { expr, .. } | Expr::Ref { expr, .. } => {
            array_root_name(expr, defs, arrays, depth + 1)
        }
        _ => None,
    }
}

fn exact_utf8_array_pair(ptr_arg: &Expr, len_arg: &Expr, defs: &BTreeMap<String, Expr>) -> bool {
    if std::env::var_os("SLATE_PTR_LEN_DEBUG").is_some() {
        eprintln!(
            "ptr_len utf8 ptr={ptr_arg:?} len={len_arg:?} ptr_root={:?} ptr_source={:?} len_value={:?}",
            canonical_var(ptr_arg, defs),
            canonical_var(ptr_arg, defs).and_then(|name| defs.get(&name)),
            resolved_integer(len_arg, defs, 0)
        );
    }
    let Some(len) = resolved_integer(len_arg, defs, 0).and_then(|len| usize::try_from(len).ok())
    else {
        return false;
    };
    let Some(bytes) = resolved_byte_array(ptr_arg, defs, 0) else {
        return false;
    };
    let payload_len = bytes
        .iter()
        .position(|byte| *byte == 0)
        .unwrap_or(bytes.len());
    len <= payload_len && std::str::from_utf8(&bytes[..len]).is_ok()
}

fn utf8_owned_fill_proof(
    body: &[IndentStmt],
    ptr_arg: &Expr,
    len_arg: &Expr,
    defs: &BTreeMap<String, Expr>,
    candidate_fns: &BTreeMap<&str, Vec<usize>>,
) -> bool {
    let Some(ptr_canon) = canonical_var(ptr_arg, defs) else {
        return false;
    };
    let Some(len) = resolved_integer(len_arg, defs, 0).and_then(|len| usize::try_from(len).ok())
    else {
        return false;
    };

    let mut calls = Vec::new();
    for indent in body {
        indent.stmt.collect_calls(&mut calls);
    }
    let mut valid_fills = 0usize;
    let mut transfers = 0usize;
    for &(callee, args) in &calls {
        let touches = args
            .iter()
            .any(|arg| canonical_var(arg, defs).as_deref() == Some(ptr_canon.as_str()));
        if !touches {
            continue;
        }
        match callee.as_str() {
            "memcpy" | "memmove" => {
                let [dst, src, count] = args else {
                    return false;
                };
                if canonical_var(dst, defs).as_deref() != Some(ptr_canon.as_str()) {
                    return false;
                }
                let Some(fill_len) =
                    resolved_integer(count, defs, 0).and_then(|len| usize::try_from(len).ok())
                else {
                    return false;
                };
                let Some(bytes) = resolved_byte_array(src, defs, 0) else {
                    return false;
                };
                if fill_len != len
                    || bytes.len() < len
                    || std::str::from_utf8(&bytes[..len]).is_err()
                {
                    return false;
                }
                valid_fills += 1;
            }
            other if candidate_fns.contains_key(other) => transfers += 1,
            _ => return false,
        }
    }

    valid_fills == 1 && transfers == 1 && !body_writes_ptr(body, &ptr_canon, defs)
}

fn body_writes_ptr(body: &[IndentStmt], ptr_canon: &str, defs: &BTreeMap<String, Expr>) -> bool {
    let mut offset_lets = Vec::new();
    collect_offset_lets(body, &mut offset_lets);
    let offset_recv: BTreeMap<&str, &str> = offset_lets
        .iter()
        .map(|(temp, recv, _)| (temp.as_str(), recv.as_str()))
        .collect();
    let mut targets = Vec::new();
    collect_write_bases(body, &mut targets);
    targets
        .into_iter()
        .any(|base| write_root(base, &offset_recv, defs) == ptr_canon)
}

fn write_root(
    base: &str,
    offset_recv: &BTreeMap<&str, &str>,
    defs: &BTreeMap<String, Expr>,
) -> String {
    let mut cur = base.to_string();
    for _ in 0..64 {
        if let Some(recv) = offset_recv.get(cur.as_str()) {
            cur = (*recv).to_string();
            continue;
        }
        match canonical_var(&Expr::Var(Ident::new(cur.as_str())), defs) {
            Some(canon) if canon != cur => cur = canon,
            _ => break,
        }
    }
    cur
}

fn collect_write_bases<'a>(body: &'a [IndentStmt], out: &mut Vec<&'a str>) {
    for indent in body {
        collect_write_bases_stmt(&indent.stmt, out);
    }
}

fn collect_write_bases_stmt<'a>(stmt: &'a Stmt, out: &mut Vec<&'a str>) {
    let base_of = |target: &'a Expr| -> Option<&'a str> {
        match peel_cast(target) {
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => peeled_var(expr),
            Expr::Index { base, .. } => peeled_var(base),
            _ => None,
        }
    };
    match stmt {
        Stmt::Assign { target, .. } | Stmt::CompoundAssign { target, .. } => {
            out.extend(base_of(target));
        }
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            collect_write_bases(then_body, out);
            collect_write_bases(else_body, out);
        }
        Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. }
        | Stmt::For { body, .. } => collect_write_bases(body, out),
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_write_bases(&arm.body, out);
            }
        }
        Stmt::While { body, .. } | Stmt::Unsafe { body } | Stmt::Block(body) => {
            collect_write_bases(&body.stmts, out)
        }
        _ => {}
    }
}

fn resolved_byte_array(
    expr: &Expr,
    defs: &BTreeMap<String, Expr>,
    depth: usize,
) -> Option<Vec<u8>> {
    if depth > 32 {
        return None;
    }
    match peel_value(expr) {
        Expr::Var(name) => resolved_byte_array(defs.get(name.as_str())?, defs, depth + 1),
        Expr::ArrayPtr { array, .. } => resolved_byte_array(array, defs, depth + 1),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            resolved_byte_array(recv, defs, depth + 1)
        }
        Expr::ArrayLit(values) => values
            .iter()
            .map(|value| {
                resolved_integer(value, defs, depth + 1).and_then(|value| u8::try_from(value).ok())
            })
            .collect(),
        Expr::ByteStr(bytes) | Expr::CStr(bytes) => Some(bytes.clone()),
        _ => None,
    }
}

fn same_value(lhs: &Expr, rhs: &Expr, defs: &BTreeMap<String, Expr>) -> bool {
    resolved_expr(lhs, defs, 0) == resolved_expr(rhs, defs, 0)
}

fn resolved_expr(expr: &Expr, defs: &BTreeMap<String, Expr>, depth: usize) -> Expr {
    if depth > 32 {
        return expr.clone();
    }
    match peel_value(expr) {
        Expr::Var(name) => defs
            .get(name.as_str())
            .map(|value| resolved_expr(value, defs, depth + 1))
            .unwrap_or_else(|| expr.clone()),
        Expr::Binary { op, lhs, rhs } => Expr::Binary {
            op: *op,
            lhs: Box::new(resolved_expr(lhs, defs, depth + 1)),
            rhs: Box::new(resolved_expr(rhs, defs, depth + 1)),
        },
        other => other.clone(),
    }
}

fn is_malloc_call(expr: &Expr) -> bool {
    matches!(
        peel_value(expr),
        Expr::Call { binding, .. } if binding.known() == Some(Known::Malloc)
    )
}

fn primitive_size(ty: &Type) -> Option<u64> {
    match ty {
        Type::Prim(Prim::I8 | Prim::U8) => Some(1),
        Type::Prim(Prim::I16 | Prim::U16 | Prim::F16) => Some(2),
        Type::Prim(Prim::I32 | Prim::U32 | Prim::F32) => Some(4),
        Type::Prim(Prim::I64 | Prim::U64 | Prim::F64) => Some(8),
        Type::Prim(Prim::I128 | Prim::U128 | Prim::F128) => Some(16),
        _ => None,
    }
}

fn canonical_var(expr: &Expr, defs: &BTreeMap<String, Expr>) -> Option<String> {
    let Expr::Var(name) = peel_value(expr) else {
        return None;
    };
    let mut current = name.as_str();
    for _ in 0..32 {
        let Some(next) = defs.get(current) else {
            break;
        };
        let Expr::Var(next) = peel_value(next) else {
            break;
        };
        if next.as_str() == current {
            break;
        }
        current = next.as_str();
    }
    Some(current.to_string())
}

fn resolved_integer(expr: &Expr, defs: &BTreeMap<String, Expr>, depth: usize) -> Option<u64> {
    if depth > 32 {
        return None;
    }
    if let Some(value) = integer_value(peel_value(expr)) {
        return Some(value);
    }
    let Expr::Var(name) = peel_value(expr) else {
        return None;
    };
    resolved_integer(defs.get(name.as_str())?, defs, depth + 1)
}

fn integer_value(expr: &Expr) -> Option<u64> {
    match expr {
        Expr::Value(crate::backend::rust_ast::RustValue::I64(value)) => u64::try_from(*value).ok(),
        Expr::Value(crate::backend::rust_ast::RustValue::I128(value)) => u64::try_from(*value).ok(),
        Expr::Value(crate::backend::rust_ast::RustValue::Usize(value)) => {
            u64::try_from(*value).ok()
        }
        Expr::Value(crate::backend::rust_ast::RustValue::U128(value)) => u64::try_from(*value).ok(),
        Expr::Value(crate::backend::rust_ast::RustValue::TypedInt(value, _)) => {
            u64::try_from(*value).ok()
        }
        Expr::Value(crate::backend::rust_ast::RustValue::TypedUInt(value, _)) => {
            u64::try_from(*value).ok()
        }
        _ => None,
    }
}

fn peel_value(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } | Expr::Transmute { expr, .. } => peel_value(expr),
        Expr::Block(block) | Expr::Unsafe(block)
            if block.stmts.is_empty() && block.tail.is_some() =>
        {
            peel_value(block.tail.as_deref().expect("checked above"))
        }
        _ => expr,
    }
}

fn collect_value_defs(body: &[IndentStmt]) -> BTreeMap<String, Expr> {
    let mut out = BTreeMap::new();
    collect_value_defs_into(body, &mut out);
    out
}

fn collect_local_arrays(body: &[IndentStmt], out: &mut ArrayCatalog) {
    for indent in body {
        match &indent.stmt {
            Stmt::Let {
                name, ty: Some(ty), ..
            }
            | Stmt::LetIf {
                name, ty: Some(ty), ..
            } => {
                if let Some(source) = array_source(ty) {
                    out.insert(name.clone(), source);
                }
            }
            _ => {}
        }
        match &indent.stmt {
            Stmt::LetIf {
                then_body,
                else_body,
                ..
            }
            | Stmt::If {
                then_body,
                else_body,
                ..
            } => {
                collect_local_arrays(then_body, out);
                collect_local_arrays(else_body, out);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => collect_local_arrays(body, out),
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                collect_local_arrays(&body.stmts, out)
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    collect_local_arrays(&arm.body, out);
                }
            }
            _ => {}
        }
    }
}

fn collect_value_defs_into(body: &[IndentStmt], out: &mut BTreeMap<String, Expr>) {
    for indent in body {
        match &indent.stmt {
            Stmt::Let {
                name,
                init: Some(init),
                ..
            } => {
                out.insert(name.clone(), init.clone());
            }
            Stmt::Assign {
                target: Expr::Var(name),
                value,
            } => {
                out.insert(name.to_string(), value.clone());
            }
            _ => {}
        }
        match &indent.stmt {
            Stmt::LetIf {
                then_body,
                else_body,
                ..
            }
            | Stmt::If {
                then_body,
                else_body,
                ..
            } => {
                collect_value_defs_into(then_body, out);
                collect_value_defs_into(else_body, out);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => collect_value_defs_into(body, out),
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                collect_value_defs_into(&body.stmts, out)
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    collect_value_defs_into(&arm.body, out);
                }
            }
            _ => {}
        }
    }
}

fn remove_owned_frees(
    body: &mut Vec<IndentStmt>,
    candidate: &SignatureCandidate,
    defs: &BTreeMap<String, Expr>,
) {
    body.retain_mut(|indent| {
        if owned_free_arg(&indent.stmt)
            .and_then(|arg| canonical_var(arg, defs))
            .as_deref()
            == Some(candidate.ptr_param.as_str())
        {
            return false;
        }
        remove_owned_frees_stmt(&mut indent.stmt, candidate, defs);
        true
    });
}

fn owned_free_arg(stmt: &Stmt) -> Option<&Expr> {
    let Stmt::Expr(expr) = stmt else {
        return None;
    };
    free_expr_arg(expr)
}

fn free_expr_arg(expr: &Expr) -> Option<&Expr> {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            block.tail.as_deref().and_then(free_expr_arg)
        }
        Expr::Call { args, binding, .. } if binding.known() == Some(Known::Free) => {
            matches!(args.as_slice(), [_]).then(|| &args[0])
        }
        _ => None,
    }
}

fn remove_owned_frees_stmt(
    stmt: &mut Stmt,
    candidate: &SignatureCandidate,
    defs: &BTreeMap<String, Expr>,
) {
    match stmt {
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            remove_owned_frees(then_body, candidate, defs);
            remove_owned_frees(else_body, candidate, defs);
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => remove_owned_frees(body, candidate, defs),
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            remove_owned_frees(&mut body.stmts, candidate, defs)
        }
        Stmt::Match { arms, .. } => {
            for arm in arms {
                remove_owned_frees(&mut arm.body, candidate, defs);
            }
        }
        _ => {}
    }
}

fn apply_signature_items(
    items: &mut [Item],
    accepted: &BTreeMap<String, Vec<usize>>,
    candidates: &[SignatureCandidate],
    proofs: &mut BTreeMap<(String, String), VecDeque<CallProof>>,
) {
    for item in items {
        match item {
            Item::Fn(f) => apply_signature_fn(f, accepted, candidates, proofs),
            Item::InlineMod { items, .. } => {
                apply_signature_items(items, accepted, candidates, proofs)
            }
            _ => {}
        }
    }
}

fn apply_signature_fn(
    f: &mut FnDef,
    accepted: &BTreeMap<String, Vec<usize>>,
    candidates: &[SignatureCandidate],
    proofs: &mut BTreeMap<(String, String), VecDeque<CallProof>>,
) {
    if let Some(ids) = accepted.get(&f.name) {
        let mut len_indices = Vec::new();
        for &id in ids {
            let candidate = &candidates[id];
            if candidate.kind.owned() {
                let defs = collect_value_defs(&f.body);
                remove_owned_frees(&mut f.body, candidate, &defs);
            }
            let raw = Expr::MethodCall {
                recv: Box::new(Expr::Var(Ident::new(candidate.ptr_param.as_str()))),
                method: if candidate.kind.mutable() {
                    "as_mut_ptr"
                } else {
                    "as_ptr"
                }
                .to_string(),
                args: Vec::new(),
            };
            let raw = Expr::Cast {
                expr: Box::new(raw),
                ty: Type::Ptr {
                    mutable: candidate.raw_mutable,
                    inner: Box::new(candidate.elem_ty.clone()),
                },
            };
            let len = Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Var(Ident::new(candidate.ptr_param.as_str()))),
                    method: "len".to_string(),
                    args: Vec::new(),
                }),
                ty: f.params[candidate.len_index].ty.clone(),
            };
            for indent in &mut f.body {
                indent.stmt.substitute_var(&candidate.ptr_param, &raw);
                indent.stmt.substitute_var(&candidate.len_param, &len);
            }
            f.params[candidate.ptr_index].ty = match candidate.kind {
                BufferKind::Shared | BufferKind::Mutable => Type::Ref {
                    mutable: candidate.kind == BufferKind::Mutable,
                    inner: Box::new(Type::Slice(Box::new(candidate.elem_ty.clone()))),
                },
                BufferKind::Owned => Type::Generic {
                    name: "Vec".to_string(),
                    args: vec![candidate.elem_ty.clone()],
                },
                BufferKind::Str => Type::Ref {
                    mutable: false,
                    inner: Box::new(Type::Str),
                },
                BufferKind::StringOwned => Type::Custom("String".to_string()),
            };
            f.params[candidate.ptr_index].mutable = candidate.kind.owned();
            len_indices.push(candidate.len_index);
        }
        len_indices.sort_unstable();
        for len_index in len_indices.into_iter().rev() {
            f.params.remove(len_index);
        }
    }
    for indent in &mut f.body {
        rewrite_signature_stmt(&mut indent.stmt, &f.name, accepted, candidates, proofs);
    }
}

fn rewrite_signature_stmt(
    stmt: &mut Stmt,
    caller: &str,
    accepted: &BTreeMap<String, Vec<usize>>,
    candidates: &[SignatureCandidate],
    proofs: &mut BTreeMap<(String, String), VecDeque<CallProof>>,
) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        } => rewrite_signature_expr(expr, caller, accepted, candidates, proofs),
        Stmt::Let { init: None, .. } => {}
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            rewrite_signature_expr(cond, caller, accepted, candidates, proofs);
            for indent in then_body.iter_mut().chain(else_body.iter_mut()) {
                rewrite_signature_stmt(&mut indent.stmt, caller, accepted, candidates, proofs);
            }
            rewrite_signature_expr(then_value, caller, accepted, candidates, proofs);
            rewrite_signature_expr(else_value, caller, accepted, candidates, proofs);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            rewrite_signature_expr(target, caller, accepted, candidates, proofs);
            rewrite_signature_expr(value, caller, accepted, candidates, proofs);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            rewrite_signature_expr(expr, caller, accepted, candidates, proofs)
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) | Stmt::InlineAsm(_) => {}
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            rewrite_signature_expr(cond, caller, accepted, candidates, proofs);
            for indent in then_body.iter_mut().chain(else_body.iter_mut()) {
                rewrite_signature_stmt(&mut indent.stmt, caller, accepted, candidates, proofs);
            }
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => {
            for indent in body {
                rewrite_signature_stmt(&mut indent.stmt, caller, accepted, candidates, proofs);
            }
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            for indent in &mut body.stmts {
                rewrite_signature_stmt(&mut indent.stmt, caller, accepted, candidates, proofs);
            }
            if let Some(tail) = &mut body.tail {
                rewrite_signature_expr(tail, caller, accepted, candidates, proofs);
            }
        }
        Stmt::Match { expr, arms } => {
            rewrite_signature_expr(expr, caller, accepted, candidates, proofs);
            for arm in arms {
                for indent in &mut arm.body {
                    rewrite_signature_stmt(&mut indent.stmt, caller, accepted, candidates, proofs);
                }
            }
        }
    }
}

fn rewrite_signature_expr(
    expr: &mut Expr,
    caller: &str,
    accepted: &BTreeMap<String, Vec<usize>>,
    candidates: &[SignatureCandidate],
    proofs: &mut BTreeMap<(String, String), VecDeque<CallProof>>,
) {
    match expr {
        Expr::Call { func, args, .. } => {
            rewrite_signature_expr(func, caller, accepted, candidates, proofs);
            for arg in args.iter_mut() {
                rewrite_signature_expr(arg, caller, accepted, candidates, proofs);
            }
            let Expr::Var(callee) = &**func else {
                return;
            };
            let Some(ids) = accepted.get(callee.as_str()) else {
                return;
            };
            let Some(queue) = proofs.get_mut(&(caller.to_string(), callee.to_string())) else {
                return;
            };
            let mut bridges = Vec::new();
            let mut len_indices = Vec::new();
            for &id in ids {
                let candidate = &candidates[id];
                let Some(proof) = queue.pop_front() else {
                    return;
                };
                let Some(ptr_arg) = args.get(candidate.ptr_index).cloned() else {
                    return;
                };
                let Some(len_arg) = args.get(candidate.len_index).cloned() else {
                    return;
                };
                let bridge = match proof {
                    CallProof::Exact => buffer_bridge(ptr_arg, len_arg, candidate),
                    CallProof::Forwarded(source) => {
                        Expr::Var(Ident::new(candidates[source].ptr_param.as_str()))
                    }
                };
                bridges.push((candidate.ptr_index, bridge));
                len_indices.push(candidate.len_index);
            }
            for (ptr_index, bridge) in bridges {
                args[ptr_index] = bridge;
            }
            len_indices.sort_unstable();
            for len_index in len_indices.into_iter().rev() {
                args.remove(len_index);
            }
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => {
            rewrite_signature_expr(expr, caller, accepted, candidates, proofs)
        }
        Expr::Block(block) | Expr::Unsafe(block) => {
            for indent in &mut block.stmts {
                rewrite_signature_stmt(&mut indent.stmt, caller, accepted, candidates, proofs);
            }
            if let Some(tail) = &mut block.tail {
                rewrite_signature_expr(tail, caller, accepted, candidates, proofs);
            }
        }
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        } => {
            rewrite_signature_expr(lhs, caller, accepted, candidates, proofs);
            rewrite_signature_expr(rhs, caller, accepted, candidates, proofs);
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            rewrite_signature_expr(recv, caller, accepted, candidates, proofs);
            for arg in args {
                rewrite_signature_expr(arg, caller, accepted, candidates, proofs);
            }
        }
        Expr::Index { base, index } => {
            rewrite_signature_expr(base, caller, accepted, candidates, proofs);
            rewrite_signature_expr(index, caller, accepted, candidates, proofs);
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => {
            rewrite_signature_expr(base, caller, accepted, candidates, proofs)
        }
        _ => {}
    }
}

fn buffer_bridge(ptr: Expr, len: Expr, candidate: &SignatureCandidate) -> Expr {
    if candidate.kind == BufferKind::Str {
        let bytes = Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                ["std", "slice", "from_raw_parts"].map(Ident::from),
            ))),
            args: vec![
                Expr::Cast {
                    expr: Box::new(ptr),
                    ty: Type::Ptr {
                        mutable: false,
                        inner: Box::new(Type::Prim(Prim::U8)),
                    },
                },
                Expr::Cast {
                    expr: Box::new(len),
                    ty: Type::Prim(Prim::Usize),
                },
            ],
        };
        return Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Call {
                binding: CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "str", "from_utf8_unchecked"].map(Ident::from),
                ))),
                args: vec![bytes],
            })),
        }));
    }
    let elem_ty = if candidate.kind == BufferKind::StringOwned {
        Type::Prim(Prim::U8)
    } else {
        candidate.elem_ty.clone()
    };
    let ptr = Expr::Cast {
        expr: Box::new(ptr),
        ty: Type::Ptr {
            mutable: candidate.kind.mutable(),
            inner: Box::new(elem_ty),
        },
    };
    let len = Expr::Cast {
        expr: Box::new(len),
        ty: Type::Prim(Prim::Usize),
    };
    let (path, args) = match candidate.kind {
        BufferKind::Shared => (vec!["std", "slice", "from_raw_parts"], vec![ptr, len]),
        BufferKind::Mutable => (vec!["std", "slice", "from_raw_parts_mut"], vec![ptr, len]),
        BufferKind::Owned => (vec!["Vec", "from_raw_parts"], vec![ptr, len.clone(), len]),
        BufferKind::StringOwned => (
            vec!["String", "from_raw_parts"],
            vec![ptr, len.clone(), len],
        ),
        BufferKind::Str => unreachable!("handled above"),
    };
    Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(path.into_iter().map(Ident::from)))),
            args,
        })),
    }))
}

fn collect_items(
    items: &[Item],
    facts: &BTreeMap<PointerBinding, PointerFact>,
    out: &mut BTreeMap<PointerBinding, Pairing>,
) {
    for item in items {
        match item {
            Item::Fn(f) => collect_fn(f, facts, out),
            Item::InlineMod { items, .. } => collect_items(items, facts, out),
            _ => {}
        }
    }
}

fn apply_items(items: &mut [Item], pairings: &BTreeMap<PointerBinding, Pairing>) {
    for item in items {
        match item {
            Item::Fn(f) => apply_fn(f, pairings),
            Item::InlineMod { items, .. } => apply_items(items, pairings),
            _ => {}
        }
    }
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

fn collect_fn(
    f: &FnDef,
    facts: &BTreeMap<PointerBinding, PointerFact>,
    out: &mut BTreeMap<PointerBinding, Pairing>,
) {
    let local_name_of = |param: &str| -> String {
        shadow_local_name(&f.body, param).unwrap_or_else(|| param.to_string())
    };
    let has_owned = f.params.iter().any(|param| {
        facts
            .get(&PointerBinding {
                function: f.name.clone(),
                name: param.name.clone(),
            })
            .is_some_and(|fact| {
                matches!(
                    fact.resolved().base,
                    ResolvedPtrType::Vec | ResolvedPtrType::StringOwned
                )
            })
    });
    let calls_free = !has_owned || body_calls_free(&f.body);

    let buffer_ptrs: Vec<(String, String, BufferKind)> = f
        .params
        .iter()
        .filter(|p| matches!(p.ty, Type::Ptr { .. }))
        .filter(|p| {
            let binding = PointerBinding {
                function: f.name.clone(),
                name: p.name.clone(),
            };
            facts.get(&binding).is_some_and(|fact| {
                matches!(
                    fact.resolved().base,
                    ResolvedPtrType::Slice
                        | ResolvedPtrType::SliceMut
                        | ResolvedPtrType::Vec
                        | ResolvedPtrType::Str
                        | ResolvedPtrType::StringOwned
                )
            })
        })
        .filter_map(|p| {
            let binding = PointerBinding {
                function: f.name.clone(),
                name: p.name.clone(),
            };
            let kind = match facts[&binding].resolved().base {
                ResolvedPtrType::Slice => BufferKind::Shared,
                ResolvedPtrType::SliceMut => BufferKind::Mutable,
                ResolvedPtrType::Vec => BufferKind::Owned,
                ResolvedPtrType::Str => BufferKind::Str,
                ResolvedPtrType::StringOwned => BufferKind::StringOwned,
                _ => unreachable!("filtered above"),
            };
            if kind.owned() && !calls_free {
                return None;
            }
            Some((p.name.clone(), local_name_of(&p.name), kind))
        })
        .collect();
    if buffer_ptrs.is_empty() {
        return;
    }
    let int_params: Vec<(String, String)> = f
        .params
        .iter()
        .filter(|p| is_integer_type(&p.ty))
        .map(|p| (p.name.clone(), local_name_of(&p.name)))
        .collect();
    if int_params.is_empty() {
        return;
    }

    let let_exprs = collect_let_exprs(&f.body);
    let mut bound_pairs: Vec<(String, String)> = Vec::new();
    collect_loop_bounds(&f.body, &let_exprs, &mut bound_pairs);

    let mut offsets = Vec::new();
    for indent in &f.body {
        indent.stmt.collect_offset_calls(&mut offsets);
    }

    if bound_pairs.is_empty() {
        return;
    }

    for (ptr_param, ptr_local, kind) in &buffer_ptrs {
        for (len_param, len_local) in &int_params {
            if ptr_param == len_param {
                continue;
            }
            let matched_idx = bound_pairs.iter().find_map(|(idx, len)| {
                let matches_len = len == len_local;
                let matches_offset = offsets.iter().any(|(recv, _, index)| {
                    resolve_var(&let_exprs, recv.as_str()) == ptr_local.as_str()
                        && peeled_var(index).map(|v| resolve_var(&let_exprs, v))
                            == Some(idx.as_str())
                });
                (matches_len && matches_offset).then(|| idx.clone())
            });
            let Some(idx_name) = matched_idx else {
                continue;
            };
            let guard_reads = bound_pairs
                .iter()
                .filter(|(_, len)| len == len_local)
                .count();
            if count_var_reads(&f.body, len_local) != guard_reads {
                continue;
            }
            if assigned_more_than_once(&f.body, len_local)
                || assigned_more_than_once(&f.body, ptr_local)
            {
                continue;
            }
            let temp_names = matched_offset_temps(&f.body, &let_exprs, ptr_local, &idx_name);
            let writes = is_written_through(&f.body, &temp_names);
            if temp_names.is_empty() || (writes && !kind.mutable()) {
                continue;
            }
            out.insert(
                PointerBinding {
                    function: f.name.clone(),
                    name: ptr_param.clone(),
                },
                Pairing {
                    len_param: len_param.clone(),
                    idx_name,
                    kind: *kind,
                },
            );
        }
    }
}

fn body_calls_free(body: &[IndentStmt]) -> bool {
    let mut calls = Vec::new();
    for indent in body {
        indent.stmt.collect_calls(&mut calls);
    }
    calls.iter().any(|(callee, _)| callee.as_str() == "free")
}

fn shadow_local_name(body: &[IndentStmt], param: &str) -> Option<String> {
    body.iter().find_map(|indent| match &indent.stmt {
        Stmt::Assign {
            target: Expr::Var(t),
            value: Expr::Var(v),
        } if v.as_str() == param => Some(t.as_str().to_string()),
        _ => None,
    })
}

fn apply_fn(f: &mut FnDef, pairings: &BTreeMap<PointerBinding, Pairing>) {
    for (ptr_name, elem_ty) in f
        .params
        .iter()
        .filter_map(|p| match &p.ty {
            Type::Ptr { inner, .. } => Some((p.name.clone(), (**inner).clone())),
            _ => None,
        })
        .collect::<Vec<_>>()
    {
        let binding = PointerBinding {
            function: f.name.clone(),
            name: ptr_name.clone(),
        };
        let Some(pairing) = pairings.get(&binding) else {
            continue;
        };
        apply_view(f, &ptr_name, &elem_ty, pairing);
    }
}

fn matched_offset_temps(
    body: &[IndentStmt],
    let_exprs: &BTreeMap<String, Expr>,
    ptr_local: &str,
    idx_name: &str,
) -> Vec<String> {
    let mut offset_lets = Vec::new();
    collect_offset_lets(body, &mut offset_lets);
    offset_lets
        .iter()
        .filter(|(_, recv, idx)| {
            resolve_var(let_exprs, recv.as_str()) == ptr_local
                && peeled_var(idx).map(|v| resolve_var(let_exprs, v)) == Some(idx_name)
        })
        .map(|(temp, ..)| temp.clone())
        .collect()
}

fn is_written_through(body: &[IndentStmt], temp_names: &[String]) -> bool {
    body.iter()
        .any(|indent| stmt_writes_temp(&indent.stmt, temp_names))
}

fn stmt_writes_temp(stmt: &Stmt, temp_names: &[String]) -> bool {
    let is_temp_deref = |target: &Expr| matches!(peel_cast(target), Expr::Unary{op: UnaryOp::Deref, expr} if matches!(&**expr, Expr::Var(v) if temp_names.iter().any(|t| t == v.as_str())));
    match stmt {
        Stmt::Assign { target, .. } | Stmt::CompoundAssign { target, .. } => is_temp_deref(target),
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => is_written_through(then_body, temp_names) || is_written_through(else_body, temp_names),
        Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. }
        | Stmt::For { body, .. } => is_written_through(body, temp_names),
        Stmt::Match { arms, .. } => arms
            .iter()
            .any(|arm| is_written_through(&arm.body, temp_names)),
        Stmt::While { body, .. } | Stmt::Unsafe { body } => {
            is_written_through(&body.stmts, temp_names)
        }
        Stmt::Block(body) => is_written_through(&body.stmts, temp_names),
        _ => false,
    }
}

fn peel_cast(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_cast(expr),
        other => other,
    }
}

fn apply_view(f: &mut FnDef, ptr_name: &str, elem_ty: &Type, pairing: &Pairing) {
    let let_exprs = collect_let_exprs(&f.body);
    let ptr_local = shadow_local_name(&f.body, ptr_name).unwrap_or_else(|| ptr_name.to_string());
    let temp_names = matched_offset_temps(&f.body, &let_exprs, &ptr_local, &pairing.idx_name);
    if temp_names.is_empty() {
        return;
    }

    let view_name = format!("__{ptr_name}_view");
    let insert_at = 0;
    let depth = f.body.first().map(|indent| indent.depth).unwrap_or(0);

    let view_let = IndentStmt {
        depth,
        stmt: Stmt::Let {
            name: view_name.clone(),
            mutable: false,
            ty: None,
            init: Some(Expr::Unsafe(Box::new(Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::Call {
                    binding: CallBinding::Generated,
                    func: Box::new(Expr::Path(Path::new(
                        [
                            "std",
                            "slice",
                            if pairing.kind.mutable() {
                                "from_raw_parts_mut"
                            } else {
                                "from_raw_parts"
                            },
                        ]
                        .map(Ident::from),
                    ))),
                    args: vec![
                        Expr::Cast {
                            expr: Box::new(Expr::Var(Ident::new(ptr_name))),
                            ty: Type::Ptr {
                                mutable: pairing.kind.mutable(),
                                inner: Box::new(elem_ty.clone()),
                            },
                        },
                        Expr::Cast {
                            expr: Box::new(Expr::Var(Ident::new(pairing.len_param.as_str()))),
                            ty: Type::Prim(Prim::Usize),
                        },
                    ],
                })),
            }))),
        },
    };
    f.body.insert(insert_at, view_let);

    for indent in &mut f.body {
        rewrite_stmt(&mut indent.stmt, &temp_names, &view_name, &pairing.idx_name);
    }
}

fn rewrite_stmt(stmt: &mut Stmt, temp_names: &[String], view_name: &str, idx_name: &str) {
    match stmt {
        Stmt::Let {
            init: Some(init), ..
        } => rewrite_expr(init, temp_names, view_name, idx_name),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            rewrite_expr(target, temp_names, view_name, idx_name);
            rewrite_expr(value, temp_names, view_name, idx_name);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            rewrite_expr(expr, temp_names, view_name, idx_name);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            rewrite_expr(cond, temp_names, view_name, idx_name);
            for indent in then_body.iter_mut().chain(else_body.iter_mut()) {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
            }
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            rewrite_expr(cond, temp_names, view_name, idx_name);
            for indent in then_body.iter_mut() {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
            }
            rewrite_expr(then_value, temp_names, view_name, idx_name);
            for indent in else_body.iter_mut() {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
            }
            rewrite_expr(else_value, temp_names, view_name, idx_name);
        }
        Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. }
        | Stmt::For { body, .. } => {
            for indent in body {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
            }
        }
        Stmt::While { cond, body } => {
            rewrite_expr(cond, temp_names, view_name, idx_name);
            for indent in &mut body.stmts {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
            }
            if let Some(tail) = &mut body.tail {
                rewrite_expr(tail, temp_names, view_name, idx_name);
            }
        }
        Stmt::Unsafe { body } | Stmt::Block(body) => {
            let body: &mut Block = body;
            for indent in &mut body.stmts {
                rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
            }
            if let Some(tail) = &mut body.tail {
                rewrite_expr(tail, temp_names, view_name, idx_name);
            }
        }
        Stmt::Match { expr, arms } => {
            rewrite_expr(expr, temp_names, view_name, idx_name);
            for arm in arms {
                for indent in &mut arm.body {
                    rewrite_stmt(&mut indent.stmt, temp_names, view_name, idx_name);
                }
            }
        }
        _ => {}
    }
}

fn rewrite_expr(expr: &mut Expr, temp_names: &[String], view_name: &str, idx_name: &str) {
    if let Expr::Unary {
        op: UnaryOp::Deref,
        expr: inner,
    } = expr
        && let Some(t) = peeled_var(inner)
        && temp_names.iter().any(|name| name == t)
    {
        *expr = Expr::Index {
            base: Box::new(Expr::Var(Ident::new(view_name))),
            index: Box::new(Expr::Cast {
                expr: Box::new(Expr::Var(Ident::new(idx_name))),
                ty: Type::Prim(Prim::Usize),
            }),
        };
        return;
    }
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. } => rewrite_expr(expr, temp_names, view_name, idx_name),
        Expr::Block(block) | Expr::Unsafe(block) => {
            for stmt in &mut block.stmts {
                rewrite_stmt(&mut stmt.stmt, temp_names, view_name, idx_name);
            }
            if let Some(tail) = &mut block.tail {
                rewrite_expr(tail, temp_names, view_name, idx_name);
            }
        }
        Expr::Binary { lhs, rhs, .. } => {
            rewrite_expr(lhs, temp_names, view_name, idx_name);
            rewrite_expr(rhs, temp_names, view_name, idx_name);
        }
        Expr::Call { func, args, .. } => {
            rewrite_expr(func, temp_names, view_name, idx_name);
            for arg in args {
                rewrite_expr(arg, temp_names, view_name, idx_name);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            rewrite_expr(recv, temp_names, view_name, idx_name);
            for arg in args {
                rewrite_expr(arg, temp_names, view_name, idx_name);
            }
        }
        Expr::Index { base, index } => {
            rewrite_expr(base, temp_names, view_name, idx_name);
            rewrite_expr(index, temp_names, view_name, idx_name);
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
            rewrite_expr(base, temp_names, view_name, idx_name);
        }
        _ => {}
    }
}

fn collect_loop_bounds(
    body: &[IndentStmt],
    let_exprs: &BTreeMap<String, Expr>,
    out: &mut Vec<(String, String)>,
) {
    for indent in body {
        collect_loop_bounds_stmt(&indent.stmt, let_exprs, out);
    }
}

fn collect_loop_bounds_stmt(
    stmt: &Stmt,
    let_exprs: &BTreeMap<String, Expr>,
    out: &mut Vec<(String, String)>,
) {
    match stmt {
        Stmt::Loop { body, .. } => {
            if let Some(pair) = loop_break_guard(body, let_exprs) {
                out.push(pair);
            }
            collect_loop_bounds(body, let_exprs, out);
        }
        Stmt::While { cond, body } => {
            if let Some(pair) = lt_pair(cond, let_exprs) {
                out.push(pair);
            }
            collect_loop_bounds(&body.stmts, let_exprs, out);
        }
        Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            collect_loop_bounds(then_body, let_exprs, out);
            collect_loop_bounds(else_body, let_exprs, out);
        }
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        } => {
            collect_loop_bounds(then_body, let_exprs, out);
            collect_loop_bounds(else_body, let_exprs, out);
        }
        Stmt::For { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            collect_loop_bounds(body, let_exprs, out);
        }
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_loop_bounds(&arm.body, let_exprs, out);
            }
        }
        Stmt::Unsafe { body } => collect_loop_bounds(&body.stmts, let_exprs, out),
        Stmt::Block(body) => collect_loop_bounds(&body.stmts, let_exprs, out),
        _ => {}
    }
}

fn loop_break_guard(
    body: &[IndentStmt],
    let_exprs: &BTreeMap<String, Expr>,
) -> Option<(String, String)> {
    body.iter().find_map(|indent| {
        let Stmt::If {
            cond,
            then_body,
            else_body,
        } = &indent.stmt
        else {
            return None;
        };
        if !else_body.is_empty() {
            return None;
        }
        let [only] = then_body.as_slice() else {
            return None;
        };
        if !matches!(only.stmt, Stmt::Break(None)) {
            return None;
        }
        let Expr::Unary {
            op: UnaryOp::Not,
            expr: inner,
        } = cond
        else {
            return None;
        };
        lt_pair(inner, let_exprs)
    })
}

fn lt_pair(expr: &Expr, let_exprs: &BTreeMap<String, Expr>) -> Option<(String, String)> {
    let resolved = resolve_full(expr, let_exprs, 0);
    let Expr::Binary {
        op: BinOp::Lt,
        lhs,
        rhs,
    } = &resolved
    else {
        return None;
    };
    let idx = peeled_var(lhs)?;
    let len = peeled_var(rhs)?;
    Some((
        resolve_var(let_exprs, idx).to_string(),
        resolve_var(let_exprs, len).to_string(),
    ))
}

fn peeled_var(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(v) => Some(v.as_str()),
        Expr::Cast { expr, .. } => peeled_var(expr),
        _ => None,
    }
}

fn count_var_reads(body: &[IndentStmt], name: &str) -> usize {
    body.iter()
        .map(|indent| stmt_read_count(&indent.stmt, name))
        .sum()
}

fn expr_read_count(expr: &Expr, name: &str) -> usize {
    let mut vars = Vec::new();
    expr.collect_vars(&mut vars);
    vars.iter().filter(|v| v.as_str() == name).count()
}

fn stmt_read_count(stmt: &Stmt, name: &str) -> usize {
    match stmt {
        Stmt::Let {
            init: Some(init), ..
        } => expr_read_count(init, name),
        Stmt::Let { init: None, .. } => 0,
        Stmt::Assign { value, .. } => expr_read_count(value, name),
        Stmt::CompoundAssign { target, value, .. } => {
            expr_read_count(target, name) + expr_read_count(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_read_count(expr, name),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) | Stmt::InlineAsm(_) => 0,
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_read_count(cond, name)
                + count_var_reads(then_body, name)
                + count_var_reads(else_body, name)
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_read_count(cond, name)
                + count_var_reads(then_body, name)
                + expr_read_count(then_value, name)
                + count_var_reads(else_body, name)
                + expr_read_count(else_value, name)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            count_var_reads(body, name)
        }
        Stmt::For { iter, body, .. } => expr_read_count(iter, name) + count_var_reads(body, name),
        Stmt::Match { expr, arms } => {
            expr_read_count(expr, name)
                + arms
                    .iter()
                    .map(|arm| count_var_reads(&arm.body, name))
                    .sum::<usize>()
        }
        Stmt::While { cond, body } => {
            expr_read_count(cond, name)
                + count_var_reads(&body.stmts, name)
                + body
                    .tail
                    .as_deref()
                    .map(|t| expr_read_count(t, name))
                    .unwrap_or(0)
        }
        Stmt::Unsafe { body } => {
            count_var_reads(&body.stmts, name)
                + body
                    .tail
                    .as_deref()
                    .map(|t| expr_read_count(t, name))
                    .unwrap_or(0)
        }
        Stmt::Block(body) => {
            count_var_reads(&body.stmts, name)
                + body
                    .tail
                    .as_deref()
                    .map(|t| expr_read_count(t, name))
                    .unwrap_or(0)
        }
    }
}

fn assigned_more_than_once(body: &[IndentStmt], name: &str) -> bool {
    count_assigns(body, name) > 1
}

fn count_assigns(body: &[IndentStmt], name: &str) -> usize {
    body.iter()
        .map(|indent| stmt_assign_count(&indent.stmt, name))
        .sum()
}

fn stmt_assign_count(stmt: &Stmt, name: &str) -> usize {
    match stmt {
        Stmt::Assign { target, .. } | Stmt::CompoundAssign { target, .. } => {
            usize::from(matches!(target, Expr::Var(v) if v.as_str() == name))
        }
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => count_assigns(then_body, name) + count_assigns(else_body, name),
        Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. }
        | Stmt::For { body, .. } => count_assigns(body, name),
        Stmt::Match { arms, .. } => arms.iter().map(|arm| count_assigns(&arm.body, name)).sum(),
        Stmt::While { body, .. } | Stmt::Unsafe { body } => count_assigns(&body.stmts, name),
        Stmt::Block(body) => count_assigns(&body.stmts, name),
        _ => 0,
    }
}

fn collect_let_exprs(body: &[IndentStmt]) -> BTreeMap<String, Expr> {
    let mut out = BTreeMap::new();
    collect_let_exprs_stmts(body, &mut out);
    out
}

fn collect_let_exprs_stmts(body: &[IndentStmt], out: &mut BTreeMap<String, Expr>) {
    for indent in body {
        collect_let_exprs_stmt(&indent.stmt, out);
    }
}

fn collect_let_exprs_stmt(stmt: &Stmt, out: &mut BTreeMap<String, Expr>) {
    if let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
    {
        out.insert(name.clone(), init.clone());
    }
    match stmt {
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            collect_let_exprs_stmts(then_body, out);
            collect_let_exprs_stmts(else_body, out);
        }
        Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. }
        | Stmt::For { body, .. } => collect_let_exprs_stmts(body, out),
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_let_exprs_stmts(&arm.body, out);
            }
        }
        Stmt::While { body, .. } | Stmt::Unsafe { body } => {
            collect_let_exprs_stmts(&body.stmts, out)
        }
        Stmt::Block(body) => collect_let_exprs_stmts(&body.stmts, out),
        _ => {}
    }
}

fn resolve_var<'a>(let_exprs: &'a BTreeMap<String, Expr>, name: &'a str) -> &'a str {
    let mut cur = name;
    for _ in 0..32 {
        match let_exprs.get(cur).and_then(peeled_var) {
            Some(next) if next != cur => cur = next,
            _ => break,
        }
    }
    cur
}

fn resolve_full(expr: &Expr, let_exprs: &BTreeMap<String, Expr>, depth: u32) -> Expr {
    if depth > 32 {
        return expr.clone();
    }
    if let Expr::Var(v) = expr
        && let Some(next) = let_exprs.get(v.as_str())
    {
        return resolve_full(next, let_exprs, depth + 1);
    }
    expr.clone()
}

fn collect_offset_lets(body: &[IndentStmt], out: &mut Vec<(String, String, Expr)>) {
    for indent in body {
        collect_offset_lets_stmt(&indent.stmt, out);
    }
}

fn collect_offset_lets_stmt(stmt: &Stmt, out: &mut Vec<(String, String, Expr)>) {
    if let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
        && let Some((recv, idx)) = peeled_offset_call(init)
    {
        out.push((name.clone(), recv.to_string(), idx.clone()));
    }
    match stmt {
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            collect_offset_lets(then_body, out);
            collect_offset_lets(else_body, out);
        }
        Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. }
        | Stmt::For { body, .. } => collect_offset_lets(body, out),
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_offset_lets(&arm.body, out);
            }
        }
        Stmt::While { body, .. } | Stmt::Unsafe { body } => collect_offset_lets(&body.stmts, out),
        Stmt::Block(body) => collect_offset_lets(&body.stmts, out),
        _ => {}
    }
}

fn peeled_offset_call(expr: &Expr) -> Option<(&str, &Expr)> {
    match expr {
        Expr::MethodCall { recv, method, args }
            if matches!(method.as_str(), "offset" | "add" | "wrapping_add") =>
        {
            let Expr::Var(r) = &**recv else {
                return None;
            };
            let [idx] = args.as_slice() else {
                return None;
            };
            Some((r.as_str(), idx))
        }
        Expr::Unsafe(block) | Expr::Block(block) => {
            block.tail.as_deref().and_then(peeled_offset_call)
        }
        Expr::Cast { expr, .. } => peeled_offset_call(expr),
        _ => None,
    }
}

struct ConstCandidate {
    function: String,
    ptr_param: String,
    ptr_index: usize,
    elem_ty: Type,
    kind: BufferKind,
    raw_mutable: bool,
    bound: u64,
}

fn apply_const_length(program: &mut Program, facts: &BTreeMap<PointerBinding, PointerFact>) {
    let candidates = {
        let fn_defs = collect_fn_defs(&program.items);
        const_candidates(&fn_defs, facts)
    };
    if candidates.is_empty() {
        return;
    }
    let global_arrays = collect_global_arrays(&program.items);
    let accepted = const_accepted(&program.items, &candidates, &global_arrays);
    if accepted.is_empty() {
        return;
    }
    retype_const_items(&mut program.items, &accepted);
    rewrite_const_call_items(&mut program.items, &accepted);
}

fn const_candidates(
    fn_defs: &BTreeMap<String, &FnDef>,
    facts: &BTreeMap<PointerBinding, PointerFact>,
) -> Vec<ConstCandidate> {
    let mut out = Vec::new();
    for (function, f) in fn_defs {
        let let_exprs = collect_let_exprs(&f.body);
        let value_defs = collect_value_defs(&f.body);
        let mut bound_pairs = Vec::new();
        collect_loop_bounds(&f.body, &let_exprs, &mut bound_pairs);
        let mut idx_bounds: BTreeMap<String, u64> = BTreeMap::new();
        for (idx, len_name) in &bound_pairs {
            let Some(n) =
                resolved_integer(&Expr::Var(Ident::new(len_name.as_str())), &value_defs, 0)
            else {
                continue;
            };
            idx_bounds
                .entry(idx.clone())
                .and_modify(|cur| *cur = (*cur).min(n))
                .or_insert(n);
        }
        if idx_bounds.is_empty() {
            continue;
        }
        let mut offsets = Vec::new();
        for indent in &f.body {
            indent.stmt.collect_offset_calls(&mut offsets);
        }
        for (ptr_index, param) in f.params.iter().enumerate() {
            let Type::Ptr {
                inner,
                mutable: raw_mutable,
            } = &param.ty
            else {
                continue;
            };
            let binding = PointerBinding {
                function: function.clone(),
                name: param.name.clone(),
            };
            let Some(fact) = facts.get(&binding) else {
                continue;
            };
            let kind = match fact.resolved().base {
                ResolvedPtrType::Slice => BufferKind::Shared,
                ResolvedPtrType::SliceMut => BufferKind::Mutable,
                _ => continue,
            };
            let ptr_local =
                shadow_local_name(&f.body, &param.name).unwrap_or_else(|| param.name.clone());
            if assigned_more_than_once(&f.body, &ptr_local) {
                continue;
            }
            let ptr_offsets: Vec<&Expr> = offsets
                .iter()
                .filter(|(recv, _, _)| resolve_var(&let_exprs, recv.as_str()) == ptr_local.as_str())
                .map(|(_, _, index)| *index)
                .collect();
            if ptr_offsets.is_empty() {
                continue;
            }
            let mut bound = 0u64;
            let mut ok = true;
            for index in ptr_offsets {
                let Some(var) = peeled_var(index) else {
                    ok = false;
                    break;
                };
                let idx = resolve_var(&let_exprs, var);
                let Some(n) = idx_bounds.get(idx) else {
                    ok = false;
                    break;
                };
                bound = bound.max(*n);
            }
            if !ok || bound == 0 {
                continue;
            }
            out.push(ConstCandidate {
                function: function.clone(),
                ptr_param: param.name.clone(),
                ptr_index,
                elem_ty: (**inner).clone(),
                kind,
                raw_mutable: *raw_mutable,
                bound,
            });
        }
    }
    out
}

fn const_accepted(
    items: &[Item],
    candidates: &[ConstCandidate],
    global_arrays: &ArrayCatalog,
) -> BTreeMap<String, Vec<ConstCandidate>> {
    let by_callee: BTreeMap<&str, Vec<usize>> =
        candidates
            .iter()
            .enumerate()
            .fold(BTreeMap::new(), |mut map, (id, candidate)| {
                map.entry(candidate.function.as_str()).or_default().push(id);
                map
            });
    let mut call_counts = vec![0usize; candidates.len()];
    let mut proven = vec![true; candidates.len()];
    let mut callers = Vec::new();
    collect_fn_bodies(items, &mut callers);
    for f in &callers {
        let defs = collect_value_defs(&f.body);
        let mut arrays = global_arrays.clone();
        collect_local_arrays(&f.body, &mut arrays);
        let mut calls = Vec::new();
        for indent in &f.body {
            indent.stmt.collect_calls(&mut calls);
        }
        for (callee, args) in calls {
            let Some(ids) = by_callee.get(callee.as_str()) else {
                continue;
            };
            for &id in ids {
                let candidate = &candidates[id];
                call_counts[id] += 1;
                let ok = args.get(candidate.ptr_index).is_some_and(|ptr_arg| {
                    source_len_at_least(
                        ptr_arg,
                        &defs,
                        &arrays,
                        &candidate.elem_ty,
                        candidate.bound,
                    )
                });
                proven[id] &= ok;
            }
        }
    }
    let mut out: BTreeMap<String, Vec<ConstCandidate>> = BTreeMap::new();
    for (id, candidate) in candidates.iter().enumerate() {
        if call_counts[id] == 0 || !proven[id] {
            continue;
        }
        out.entry(candidate.function.clone())
            .or_default()
            .push(ConstCandidate {
                function: candidate.function.clone(),
                ptr_param: candidate.ptr_param.clone(),
                ptr_index: candidate.ptr_index,
                elem_ty: candidate.elem_ty.clone(),
                kind: candidate.kind,
                raw_mutable: candidate.raw_mutable,
                bound: candidate.bound,
            });
    }
    out
}

fn collect_fn_bodies<'a>(items: &'a [Item], out: &mut Vec<&'a FnDef>) {
    for item in items {
        match item {
            Item::Fn(f) => out.push(f),
            Item::InlineMod { items, .. } => collect_fn_bodies(items, out),
            _ => {}
        }
    }
}

fn source_len_at_least(
    ptr_arg: &Expr,
    defs: &BTreeMap<String, Expr>,
    arrays: &ArrayCatalog,
    elem_ty: &Type,
    need: u64,
) -> bool {
    if let Some(root) = array_root_name(ptr_arg, defs, arrays, 0)
        && let Some(source) = arrays.get(&root)
    {
        return source.elem_ty == *elem_ty && source.len >= need;
    }
    let Some(name) = canonical_var(ptr_arg, defs) else {
        return false;
    };
    let Some(source) = defs.get(&name) else {
        return false;
    };
    let call @ Expr::Call { args, .. } = peel_value(source) else {
        return false;
    };
    if !is_malloc_call(call) {
        return false;
    }
    let [size] = args.as_slice() else {
        return false;
    };
    let Some(elem_size) = primitive_size(elem_ty) else {
        return false;
    };
    if let Some(total) = resolved_integer(size, defs, 0) {
        return total >= need.saturating_mul(elem_size);
    }
    let resolved_size = resolved_expr(size, defs, 0);
    let Expr::Binary {
        op: BinOp::Mul,
        lhs,
        rhs,
    } = peel_value(&resolved_size)
    else {
        return false;
    };
    (resolved_integer(rhs, defs, 0) == Some(elem_size)
        && resolved_integer(lhs, defs, 0).is_some_and(|k| k >= need))
        || (resolved_integer(lhs, defs, 0) == Some(elem_size)
            && resolved_integer(rhs, defs, 0).is_some_and(|k| k >= need))
}

fn retype_const_items(items: &mut [Item], accepted: &BTreeMap<String, Vec<ConstCandidate>>) {
    for item in items {
        match item {
            Item::Fn(f) => {
                if let Some(cands) = accepted.get(&f.name) {
                    retype_const_fn(f, cands);
                }
            }
            Item::InlineMod { items, .. } => retype_const_items(items, accepted),
            _ => {}
        }
    }
}

fn retype_const_fn(f: &mut FnDef, cands: &[ConstCandidate]) {
    for candidate in cands {
        let raw = Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(Expr::Var(Ident::new(candidate.ptr_param.as_str()))),
                method: if candidate.kind.mutable() {
                    "as_mut_ptr"
                } else {
                    "as_ptr"
                }
                .to_string(),
                args: Vec::new(),
            }),
            ty: Type::Ptr {
                mutable: candidate.raw_mutable,
                inner: Box::new(candidate.elem_ty.clone()),
            },
        };
        for indent in &mut f.body {
            indent.stmt.substitute_var(&candidate.ptr_param, &raw);
        }
        f.params[candidate.ptr_index].ty = Type::Ref {
            mutable: candidate.kind.mutable(),
            inner: Box::new(Type::Slice(Box::new(candidate.elem_ty.clone()))),
        };
        f.params[candidate.ptr_index].mutable = false;
    }
}

fn rewrite_const_call_items(items: &mut [Item], accepted: &BTreeMap<String, Vec<ConstCandidate>>) {
    for item in items {
        match item {
            Item::Fn(f) => {
                for indent in &mut f.body {
                    rewrite_const_call_stmt(&mut indent.stmt, accepted);
                }
            }
            Item::InlineMod { items, .. } => rewrite_const_call_items(items, accepted),
            _ => {}
        }
    }
}

fn rewrite_const_call_stmt(stmt: &mut Stmt, accepted: &BTreeMap<String, Vec<ConstCandidate>>) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        } => rewrite_const_call_expr(expr, accepted),
        Stmt::Let { init: None, .. }
        | Stmt::Return(None)
        | Stmt::Break(_)
        | Stmt::Continue(_)
        | Stmt::InlineAsm(_) => {}
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            rewrite_const_call_expr(cond, accepted);
            for indent in then_body.iter_mut().chain(else_body.iter_mut()) {
                rewrite_const_call_stmt(&mut indent.stmt, accepted);
            }
            rewrite_const_call_expr(then_value, accepted);
            rewrite_const_call_expr(else_value, accepted);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            rewrite_const_call_expr(target, accepted);
            rewrite_const_call_expr(value, accepted);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => rewrite_const_call_expr(expr, accepted),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            rewrite_const_call_expr(cond, accepted);
            for indent in then_body.iter_mut().chain(else_body.iter_mut()) {
                rewrite_const_call_stmt(&mut indent.stmt, accepted);
            }
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => {
            for indent in body {
                rewrite_const_call_stmt(&mut indent.stmt, accepted);
            }
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            for indent in &mut body.stmts {
                rewrite_const_call_stmt(&mut indent.stmt, accepted);
            }
            if let Some(tail) = &mut body.tail {
                rewrite_const_call_expr(tail, accepted);
            }
        }
        Stmt::Match { expr, arms } => {
            rewrite_const_call_expr(expr, accepted);
            for arm in arms {
                for indent in &mut arm.body {
                    rewrite_const_call_stmt(&mut indent.stmt, accepted);
                }
            }
        }
    }
}

fn rewrite_const_call_expr(expr: &mut Expr, accepted: &BTreeMap<String, Vec<ConstCandidate>>) {
    match expr {
        Expr::Call { func, args, .. } => {
            rewrite_const_call_expr(func, accepted);
            for arg in args.iter_mut() {
                rewrite_const_call_expr(arg, accepted);
            }
            let Expr::Var(callee) = &**func else {
                return;
            };
            let Some(cands) = accepted.get(callee.as_str()) else {
                return;
            };
            for candidate in cands {
                if let Some(arg) = args.get_mut(candidate.ptr_index) {
                    *arg = const_bridge(arg.clone(), candidate);
                }
            }
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => rewrite_const_call_expr(expr, accepted),
        Expr::Block(block) | Expr::Unsafe(block) => {
            for indent in &mut block.stmts {
                rewrite_const_call_stmt(&mut indent.stmt, accepted);
            }
            if let Some(tail) = &mut block.tail {
                rewrite_const_call_expr(tail, accepted);
            }
        }
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        } => {
            rewrite_const_call_expr(lhs, accepted);
            rewrite_const_call_expr(rhs, accepted);
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            rewrite_const_call_expr(recv, accepted);
            for arg in args {
                rewrite_const_call_expr(arg, accepted);
            }
        }
        Expr::Index { base, index } => {
            rewrite_const_call_expr(base, accepted);
            rewrite_const_call_expr(index, accepted);
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => rewrite_const_call_expr(base, accepted),
        _ => {}
    }
}

fn const_bridge(ptr: Expr, candidate: &ConstCandidate) -> Expr {
    let ptr = Expr::Cast {
        expr: Box::new(ptr),
        ty: Type::Ptr {
            mutable: candidate.kind.mutable(),
            inner: Box::new(candidate.elem_ty.clone()),
        },
    };
    let len = Expr::Value(crate::backend::rust_ast::RustValue::Usize(
        candidate.bound as usize,
    ));
    let method = if candidate.kind.mutable() {
        "from_raw_parts_mut"
    } else {
        "from_raw_parts"
    };
    Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                ["std", "slice", method].map(Ident::from),
            ))),
            args: vec![ptr, len],
        })),
    }))
}

struct ReturnBufferLift {
    function: String,
    elem_ty: Type,
    count: Expr,
}

#[derive(Clone)]
struct ReturnRetentionPlan {
    caller: String,
    callee: String,
    binding: String,
    elem_ty: Type,
    call: Expr,
    call_temps: BTreeSet<String>,
    base_aliases: BTreeSet<String>,
}

fn apply_return_buffers(program: &mut Program) {
    let lifts = {
        let fn_defs = collect_fn_defs(&program.items);
        return_buffer_candidates(&fn_defs)
    };
    if lifts.is_empty() {
        return;
    }
    let by_fn: BTreeMap<String, &ReturnBufferLift> = lifts
        .iter()
        .map(|lift| (lift.function.clone(), lift))
        .collect();
    let elems: BTreeMap<String, Type> = lifts
        .iter()
        .map(|lift| (lift.function.clone(), lift.elem_ty.clone()))
        .collect();
    let (retained, retained_callees) = return_retention_plans(program, &elems);
    retype_return_items(&mut program.items, &by_fn);
    apply_return_retention(&mut program.items, &retained);
    let raw_elems: BTreeMap<String, Type> = elems
        .into_iter()
        .filter(|(name, _)| !retained_callees.contains(name))
        .collect();
    for item in &mut program.items {
        rewrite_return_call_item(item, &raw_elems);
    }
}

fn return_retention_plans(
    program: &Program,
    elems: &BTreeMap<String, Type>,
) -> (Vec<ReturnRetentionPlan>, BTreeSet<String>) {
    let mut call_counts = BTreeMap::<String, usize>::new();
    let mut plans = Vec::new();
    collect_return_retention_items(&program.items, elems, &mut call_counts, &mut plans);
    let mut plan_counts = BTreeMap::<String, usize>::new();
    for plan in &plans {
        *plan_counts.entry(plan.callee.clone()).or_default() += 1;
    }
    if std::env::var_os("SLATE_PTR_LEN_DEBUG").is_some() {
        eprintln!("return retention calls={call_counts:?} plans={plan_counts:?}");
    }
    let accepted: BTreeSet<String> = call_counts
        .into_iter()
        .filter_map(|(callee, count)| {
            (count > 0 && plan_counts.get(&callee) == Some(&count)).then_some(callee)
        })
        .collect();
    plans.retain(|plan| accepted.contains(&plan.callee));
    (plans, accepted)
}

fn collect_return_retention_items(
    items: &[Item],
    elems: &BTreeMap<String, Type>,
    call_counts: &mut BTreeMap<String, usize>,
    plans: &mut Vec<ReturnRetentionPlan>,
) {
    for item in items {
        match item {
            Item::Fn(f) => {
                let mut calls = Vec::new();
                for indent in &f.body {
                    indent.stmt.collect_calls(&mut calls);
                }
                for (callee, _) in calls {
                    if elems.contains_key(callee.as_str()) {
                        *call_counts.entry(callee.to_string()).or_default() += 1;
                    }
                }
                collect_return_retention_body(&f.body, &f.name, &f.body, elems, plans);
            }
            Item::InlineMod { items, .. } => {
                collect_return_retention_items(items, elems, call_counts, plans)
            }
            _ => {}
        }
    }
}

fn collect_return_retention_body(
    body: &[IndentStmt],
    caller: &str,
    function_body: &[IndentStmt],
    elems: &BTreeMap<String, Type>,
    plans: &mut Vec<ReturnRetentionPlan>,
) {
    for (decl_pos, indent) in body.iter().enumerate() {
        let Stmt::Let {
            name,
            ty: Some(Type::Ptr { inner, .. }),
            init: Some(init),
            ..
        } = &indent.stmt
        else {
            continue;
        };
        if !is_zero_pointer(init) || count_assigns(function_body, name) != 1 {
            continue;
        }
        let Some((assign_pos, value)) =
            body[decl_pos + 1..]
                .iter()
                .enumerate()
                .find_map(|(offset, indent)| match &indent.stmt {
                    Stmt::Assign {
                        target: Expr::Var(target),
                        value,
                    } if target.as_str() == name => Some((decl_pos + 1 + offset, value)),
                    _ => None,
                })
        else {
            continue;
        };
        let mut call_temps = BTreeSet::new();
        let Some((callee, call)) = resolve_retained_call(
            value,
            &body[..assign_pos],
            function_body,
            elems,
            &mut call_temps,
            0,
        ) else {
            continue;
        };
        let Some(elem_ty) = elems.get(&callee) else {
            continue;
        };
        if **inner != *elem_ty {
            continue;
        }
        let Some(base_aliases) = retained_use_domain(function_body, name, &callee, &call_temps)
        else {
            continue;
        };
        plans.push(ReturnRetentionPlan {
            caller: caller.to_string(),
            callee,
            binding: name.clone(),
            elem_ty: elem_ty.clone(),
            call,
            call_temps,
            base_aliases,
        });
    }
    for indent in body {
        match &indent.stmt {
            Stmt::LetIf {
                then_body,
                else_body,
                ..
            }
            | Stmt::If {
                then_body,
                else_body,
                ..
            } => {
                collect_return_retention_body(then_body, caller, function_body, elems, plans);
                collect_return_retention_body(else_body, caller, function_body, elems, plans);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => {
                collect_return_retention_body(body, caller, function_body, elems, plans)
            }
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                collect_return_retention_body(&body.stmts, caller, function_body, elems, plans)
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    collect_return_retention_body(&arm.body, caller, function_body, elems, plans);
                }
            }
            _ => {}
        }
    }
}

fn is_zero_pointer(expr: &Expr) -> bool {
    matches!(
        peel_value(expr),
        Expr::Value(crate::backend::rust_ast::RustValue::NullPtr)
    )
}

fn resolve_retained_call(
    expr: &Expr,
    prior: &[IndentStmt],
    function_body: &[IndentStmt],
    elems: &BTreeMap<String, Type>,
    temps: &mut BTreeSet<String>,
    depth: usize,
) -> Option<(String, Expr)> {
    if depth > 32 {
        return None;
    }
    match peel_value(expr) {
        call @ Expr::Call { func, .. } => {
            let Expr::Var(callee) = &**func else {
                return None;
            };
            elems
                .contains_key(callee.as_str())
                .then(|| (callee.to_string(), call.clone()))
        }
        Expr::Var(name)
            if count_var_reads(function_body, name.as_str()) == 1
                && count_assigns(function_body, name.as_str()) == 0 =>
        {
            let init = prior.iter().rev().find_map(|indent| match &indent.stmt {
                Stmt::Let {
                    name: candidate,
                    init: Some(init),
                    ..
                } if candidate == name.as_str() => Some(init),
                _ => None,
            })?;
            temps.insert(name.to_string());
            resolve_retained_call(init, prior, function_body, elems, temps, depth + 1)
        }
        _ => None,
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum RetainedAliasKind {
    Base,
    Offset,
}

fn retained_use_domain(
    body: &[IndentStmt],
    binding: &str,
    callee: &str,
    call_temps: &BTreeSet<String>,
) -> Option<BTreeSet<String>> {
    let mut aliases = BTreeMap::from([(binding.to_string(), RetainedAliasKind::Base)]);
    loop {
        let before = aliases.len();
        collect_retained_aliases(body, &mut aliases);
        if aliases.len() == before {
            break;
        }
    }
    let base_aliases: BTreeSet<String> = aliases
        .iter()
        .filter_map(|(name, kind)| (*kind == RetainedAliasKind::Base).then_some(name.clone()))
        .collect();
    let mut frees = 0usize;
    let safe = retained_stmts_safe(
        body,
        binding,
        callee,
        call_temps,
        &aliases,
        &base_aliases,
        &mut frees,
    );
    if std::env::var_os("SLATE_PTR_LEN_DEBUG").is_some() {
        eprintln!(
            "return retention {callee}:{binding} aliases={aliases:?} safe={safe} frees={frees}"
        );
    }
    if !safe || frees != 1 {
        return None;
    }
    Some(base_aliases)
}

fn collect_retained_aliases(
    body: &[IndentStmt],
    aliases: &mut BTreeMap<String, RetainedAliasKind>,
) {
    for indent in body {
        if let Stmt::Let {
            name,
            ty: Some(Type::Ptr { .. }),
            init: Some(init),
            ..
        } = &indent.stmt
            && let Some(kind) = retained_pointer_origin(init, aliases)
        {
            aliases.insert(name.clone(), kind);
        }
        match &indent.stmt {
            Stmt::LetIf {
                then_body,
                else_body,
                ..
            }
            | Stmt::If {
                then_body,
                else_body,
                ..
            } => {
                collect_retained_aliases(then_body, aliases);
                collect_retained_aliases(else_body, aliases);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => collect_retained_aliases(body, aliases),
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                collect_retained_aliases(&body.stmts, aliases)
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    collect_retained_aliases(&arm.body, aliases);
                }
            }
            _ => {}
        }
    }
}

fn retained_pointer_origin(
    expr: &Expr,
    aliases: &BTreeMap<String, RetainedAliasKind>,
) -> Option<RetainedAliasKind> {
    match expr {
        Expr::Var(name) => aliases.get(name.as_str()).copied(),
        Expr::Cast { expr, .. } | Expr::Transmute { expr, .. } => {
            retained_pointer_origin(expr, aliases)
        }
        Expr::Block(block) | Expr::Unsafe(block)
            if block.stmts.is_empty() && block.tail.is_some() =>
        {
            retained_pointer_origin(block.tail.as_deref().expect("checked above"), aliases)
        }
        Expr::MethodCall { recv, method, .. } | Expr::MethodCallGeneric { recv, method, .. }
            if matches!(method.as_str(), "add" | "offset" | "wrapping_add") =>
        {
            retained_pointer_origin(recv, aliases).map(|_| RetainedAliasKind::Offset)
        }
        _ => None,
    }
}

fn retained_stmts_safe(
    body: &[IndentStmt],
    binding: &str,
    callee: &str,
    call_temps: &BTreeSet<String>,
    aliases: &BTreeMap<String, RetainedAliasKind>,
    base_aliases: &BTreeSet<String>,
    frees: &mut usize,
) -> bool {
    for indent in body {
        if !retained_stmt_safe(
            &indent.stmt,
            binding,
            callee,
            call_temps,
            aliases,
            base_aliases,
            frees,
        ) {
            if std::env::var_os("SLATE_PTR_LEN_DEBUG").is_some() {
                eprintln!("return retention rejected: {:#?}", indent.stmt);
            }
            return false;
        }
    }
    true
}

fn retained_stmt_safe(
    stmt: &Stmt,
    binding: &str,
    callee: &str,
    call_temps: &BTreeSet<String>,
    aliases: &BTreeMap<String, RetainedAliasKind>,
    base_aliases: &BTreeSet<String>,
    frees: &mut usize,
) -> bool {
    if let Some(arg) = retained_free_arg(stmt) {
        let is_match = peeled_var(arg).is_some_and(|root| base_aliases.contains(root));
        if is_match {
            *frees += 1;
        }
        return is_match || !expr_reads_alias(arg, aliases);
    }
    match stmt {
        Stmt::Let { name, init, .. } if name == binding => init
            .as_ref()
            .is_none_or(|expr| !expr_reads_alias(expr, aliases)),
        Stmt::Let { name, init, .. } if call_temps.contains(name) => init
            .as_ref()
            .is_some_and(|expr| {
                direct_call_name(expr) == Some(callee)
                    || matches!(peel_value(expr), Expr::Var(temp) if call_temps.contains(temp.as_str()))
            }),
        Stmt::Let {
            name,
            ty: Some(Type::Ptr { .. }),
            init: Some(init),
            ..
        } if aliases.contains_key(name) => retained_pointer_origin(init, aliases).is_some(),
        Stmt::Let {
            init: Some(init), ..
        } => retained_value_safe(init, aliases),
        Stmt::Let { init: None, .. } => true,
        Stmt::Assign {
            target: Expr::Var(name),
            value,
        } if name.as_str() == binding => {
            direct_call_name(value) == Some(callee)
                || matches!(peel_value(value), Expr::Var(temp) if call_temps.contains(temp.as_str()))
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            retained_value_safe(target, aliases) && retained_value_safe(value, aliases)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => retained_value_safe(expr, aliases),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => true,
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            retained_value_safe(cond, aliases)
                && retained_stmts_safe(
                    then_body,
                    binding,
                    callee,
                    call_temps,
                    aliases,
                    base_aliases,
                    frees,
                )
                && retained_value_safe(then_value, aliases)
                && retained_stmts_safe(
                    else_body,
                    binding,
                    callee,
                    call_temps,
                    aliases,
                    base_aliases,
                    frees,
                )
                && retained_value_safe(else_value, aliases)
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            retained_value_safe(cond, aliases)
                && retained_stmts_safe(
                    then_body,
                    binding,
                    callee,
                    call_temps,
                    aliases,
                    base_aliases,
                    frees,
                )
                && retained_stmts_safe(
                    else_body,
                    binding,
                    callee,
                    call_temps,
                    aliases,
                    base_aliases,
                    frees,
                )
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            retained_stmts_safe(
                body,
                binding,
                callee,
                call_temps,
                aliases,
                base_aliases,
                frees,
            )
        }
        Stmt::For { iter, body, .. } => {
            retained_value_safe(iter, aliases)
                && retained_stmts_safe(
                    body,
                    binding,
                    callee,
                    call_temps,
                    aliases,
                    base_aliases,
                    frees,
                )
        }
        Stmt::Unsafe { body } | Stmt::Block(body) => {
            retained_stmts_safe(
                &body.stmts,
                binding,
                callee,
                call_temps,
                aliases,
                base_aliases,
                frees,
            ) && body
                .tail
                .as_deref()
                .is_none_or(|expr| retained_value_safe(expr, aliases))
        }
        Stmt::While { cond, body } => {
            retained_value_safe(cond, aliases)
                && retained_stmts_safe(
                    &body.stmts,
                    binding,
                    callee,
                    call_temps,
                    aliases,
                    base_aliases,
                    frees,
                )
                && body
                    .tail
                    .as_deref()
                    .is_none_or(|expr| retained_value_safe(expr, aliases))
        }
        Stmt::Match { expr, arms } => {
            retained_value_safe(expr, aliases)
                && arms.iter().all(|arm| {
                    retained_stmts_safe(
                        &arm.body,
                        binding,
                        callee,
                        call_temps,
                        aliases,
                        base_aliases,
                        frees,
                    )
                })
        }
        Stmt::InlineAsm(asm) => {
            let mut safe = true;
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| safe &= !expr_reads_alias(expr, aliases));
            }
            safe
        }
    }
}

fn retained_free_arg(stmt: &Stmt) -> Option<&Expr> {
    match stmt {
        Stmt::Expr(expr) => free_expr_arg(expr),
        Stmt::Unsafe { body } => {
            body.tail
                .as_deref()
                .and_then(free_expr_arg)
                .or_else(|| match body.stmts.as_slice() {
                    [indent] => retained_free_arg(&indent.stmt),
                    _ => None,
                })
        }
        _ => None,
    }
}

fn direct_call_name(expr: &Expr) -> Option<&str> {
    let Expr::Call { func, .. } = peel_value(expr) else {
        return None;
    };
    let Expr::Var(name) = &**func else {
        return None;
    };
    Some(name.as_str())
}

fn retained_value_safe(expr: &Expr, aliases: &BTreeMap<String, RetainedAliasKind>) -> bool {
    !expr_reads_alias(expr, aliases) || retained_deref_root(expr, aliases).is_some()
}

fn expr_reads_alias(expr: &Expr, aliases: &BTreeMap<String, RetainedAliasKind>) -> bool {
    aliases.keys().any(|name| expr.reads_var(name))
}

fn retained_deref_root(
    expr: &Expr,
    aliases: &BTreeMap<String, RetainedAliasKind>,
) -> Option<RetainedAliasKind> {
    match expr {
        Expr::Cast { expr, .. } | Expr::Transmute { expr, .. } => {
            retained_deref_root(expr, aliases)
        }
        Expr::Block(block) | Expr::Unsafe(block)
            if block.stmts.is_empty() && block.tail.is_some() =>
        {
            retained_deref_root(block.tail.as_deref().expect("checked above"), aliases)
        }
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => retained_pointer_origin(expr, aliases),
        _ => None,
    }
}

fn return_buffer_candidates(fn_defs: &BTreeMap<String, &FnDef>) -> Vec<ReturnBufferLift> {
    let mut out = Vec::new();
    for (name, f) in fn_defs {
        if f.unsafe_ || f.abi.is_some() || !matches!(f.vis, Visibility::Private) {
            continue;
        }
        let Some(Type::Ptr { inner, .. }) = &f.ret else {
            continue;
        };
        let elem_ty = (**inner).clone();
        if primitive_size(&elem_ty).is_none() {
            continue;
        }
        if body_calls_free(&f.body) {
            continue;
        }
        let defs = collect_value_defs(&f.body);
        let params: BTreeSet<&str> = f.params.iter().map(|p| p.name.as_str()).collect();
        let mut returns = Vec::new();
        collect_returns(&f.body, &mut returns);
        if returns.is_empty() {
            continue;
        }
        let mut count: Option<Expr> = None;
        let mut ok = true;
        for ret in returns {
            let Some(c) = returned_malloc_count(ret, &f.body, &defs, &elem_ty, &params) else {
                ok = false;
                break;
            };
            match &count {
                None => count = Some(c),
                Some(prev) if *prev != c => {
                    ok = false;
                    break;
                }
                _ => {}
            }
        }
        if !ok {
            continue;
        }
        out.push(ReturnBufferLift {
            function: name.clone(),
            elem_ty,
            count: count.expect("returns non-empty"),
        });
    }
    out
}

fn collect_returns<'a>(body: &'a [IndentStmt], out: &mut Vec<&'a Expr>) {
    for indent in body {
        collect_returns_stmt(&indent.stmt, out);
    }
}

fn collect_returns_stmt<'a>(stmt: &'a Stmt, out: &mut Vec<&'a Expr>) {
    match stmt {
        Stmt::Return(Some(expr)) => out.push(expr),
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            collect_returns(then_body, out);
            collect_returns(else_body, out);
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => collect_returns(body, out),
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_returns(&arm.body, out);
            }
        }
        Stmt::While { body, .. } | Stmt::Unsafe { body } | Stmt::Block(body) => {
            collect_returns(&body.stmts, out)
        }
        _ => {}
    }
}

fn returned_malloc_count(
    ret: &Expr,
    body: &[IndentStmt],
    defs: &BTreeMap<String, Expr>,
    elem_ty: &Type,
    params: &BTreeSet<&str>,
) -> Option<Expr> {
    let call = resolve_to_malloc(ret, body, defs, 0)?;
    let Expr::Call { args, .. } = call else {
        return None;
    };
    let [size] = args.as_slice() else {
        return None;
    };
    let elem_size = primitive_size(elem_ty)?;
    let resolved = resolved_expr(size, defs, 0);
    let Expr::Binary {
        op: BinOp::Mul,
        lhs,
        rhs,
    } = peel_value(&resolved)
    else {
        return None;
    };
    let count = if resolved_integer(rhs, defs, 0) == Some(elem_size) {
        (**lhs).clone()
    } else if resolved_integer(lhs, defs, 0) == Some(elem_size) {
        (**rhs).clone()
    } else {
        return None;
    };
    let mut vars = Vec::new();
    count.collect_vars(&mut vars);
    if !vars.iter().all(|v| params.contains(v.as_str())) {
        return None;
    }
    Some(count)
}

fn resolve_to_malloc<'a>(
    expr: &'a Expr,
    body: &[IndentStmt],
    defs: &'a BTreeMap<String, Expr>,
    depth: usize,
) -> Option<&'a Expr> {
    if depth > 32 {
        return None;
    }
    match peel_value(expr) {
        Expr::Var(name) => {
            if assigned_more_than_once(body, name.as_str()) {
                return None;
            }
            resolve_to_malloc(defs.get(name.as_str())?, body, defs, depth + 1)
        }
        call @ Expr::Call { .. } if is_malloc_call(call) => Some(call),
        _ => None,
    }
}

fn box_slice_type(elem_ty: &Type) -> Type {
    Type::Generic {
        name: "Box".to_string(),
        args: vec![Type::Slice(Box::new(elem_ty.clone()))],
    }
}

fn apply_return_retention(items: &mut [Item], plans: &[ReturnRetentionPlan]) {
    for item in items {
        match item {
            Item::Fn(f) => {
                for plan in plans.iter().filter(|plan| plan.caller == f.name) {
                    remove_retained_frees(&mut f.body, &plan.base_aliases);
                    retain_call_binding(&mut f.body, plan);
                    let raw = Expr::MethodCall {
                        recv: Box::new(Expr::Var(Ident::new(plan.binding.as_str()))),
                        method: "as_mut_ptr".to_string(),
                        args: Vec::new(),
                    };
                    for indent in &mut f.body {
                        indent.stmt.substitute_var(&plan.binding, &raw);
                    }
                    let aliases = plan
                        .base_aliases
                        .iter()
                        .filter(|name| *name != &plan.binding)
                        .cloned()
                        .collect();
                    remove_unused_retained_aliases(&mut f.body, &aliases);
                }
            }
            Item::InlineMod { items, .. } => apply_return_retention(items, plans),
            _ => {}
        }
    }
}

fn remove_unused_retained_aliases(body: &mut Vec<IndentStmt>, base_aliases: &BTreeSet<String>) {
    loop {
        let unused: BTreeSet<String> = base_aliases
            .iter()
            .filter(|name| count_var_reads(body, name) == 0)
            .cloned()
            .collect();
        if unused.is_empty() {
            return;
        }
        let before = body.len();
        body.retain(
            |indent| !matches!(&indent.stmt, Stmt::Let { name, .. } if unused.contains(name)),
        );
        for indent in body.iter_mut() {
            remove_unused_retained_aliases_stmt(&mut indent.stmt, &unused);
        }
        if body.len() == before
            && !body.iter().any(|indent| {
                unused
                    .iter()
                    .any(|name| indent.stmt.reads_var(name.as_str()))
            })
        {
            return;
        }
    }
}

fn remove_unused_retained_aliases_stmt(stmt: &mut Stmt, unused: &BTreeSet<String>) {
    match stmt {
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            remove_unused_retained_aliases(then_body, unused);
            remove_unused_retained_aliases(else_body, unused);
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => remove_unused_retained_aliases(body, unused),
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            remove_unused_retained_aliases(&mut body.stmts, unused)
        }
        Stmt::Match { arms, .. } => {
            for arm in arms {
                remove_unused_retained_aliases(&mut arm.body, unused);
            }
        }
        _ => {}
    }
}

fn remove_retained_frees(body: &mut Vec<IndentStmt>, base_aliases: &BTreeSet<String>) {
    body.retain_mut(|indent| {
        if retained_free_arg(&indent.stmt)
            .and_then(peeled_var)
            .is_some_and(|root| base_aliases.contains(root))
        {
            return false;
        }
        remove_retained_frees_stmt(&mut indent.stmt, base_aliases);
        true
    });
}

fn remove_retained_frees_stmt(stmt: &mut Stmt, base_aliases: &BTreeSet<String>) {
    match stmt {
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            remove_retained_frees(then_body, base_aliases);
            remove_retained_frees(else_body, base_aliases);
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => remove_retained_frees(body, base_aliases),
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            remove_retained_frees(&mut body.stmts, base_aliases)
        }
        Stmt::Match { arms, .. } => {
            for arm in arms {
                remove_retained_frees(&mut arm.body, base_aliases);
            }
        }
        _ => {}
    }
}

fn retain_call_binding(body: &mut Vec<IndentStmt>, plan: &ReturnRetentionPlan) -> bool {
    let has_decl = body
        .iter()
        .any(|indent| matches!(&indent.stmt, Stmt::Let { name, .. } if name == &plan.binding));
    let has_assign = body.iter().any(|indent| {
        matches!(&indent.stmt, Stmt::Assign { target: Expr::Var(name), .. } if name.as_str() == plan.binding)
    });
    if has_decl && has_assign {
        body.retain(|indent| {
            !matches!(&indent.stmt, Stmt::Let { name, .. } if name == &plan.binding || plan.call_temps.contains(name))
        });
        for indent in body.iter_mut() {
            if matches!(&indent.stmt, Stmt::Assign { target: Expr::Var(name), .. } if name.as_str() == plan.binding)
            {
                indent.stmt = Stmt::Let {
                    name: plan.binding.clone(),
                    mutable: true,
                    ty: Some(box_slice_type(&plan.elem_ty)),
                    init: Some(plan.call.clone()),
                };
                return true;
            }
        }
    }
    for indent in body {
        let changed = match &mut indent.stmt {
            Stmt::LetIf {
                then_body,
                else_body,
                ..
            }
            | Stmt::If {
                then_body,
                else_body,
                ..
            } => retain_call_binding(then_body, plan) || retain_call_binding(else_body, plan),
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => retain_call_binding(body, plan),
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                retain_call_binding(&mut body.stmts, plan)
            }
            Stmt::Match { arms, .. } => arms
                .iter_mut()
                .any(|arm| retain_call_binding(&mut arm.body, plan)),
            _ => false,
        };
        if changed {
            return true;
        }
    }
    false
}

fn retype_return_items(items: &mut [Item], by_fn: &BTreeMap<String, &ReturnBufferLift>) {
    for item in items {
        match item {
            Item::Fn(f) => {
                if let Some(lift) = by_fn.get(&f.name) {
                    f.ret = Some(box_slice_type(&lift.elem_ty));
                    for indent in &mut f.body {
                        wrap_returns_stmt(&mut indent.stmt, &lift.elem_ty, &lift.count);
                    }
                }
            }
            Item::InlineMod { items, .. } => retype_return_items(items, by_fn),
            _ => {}
        }
    }
}

fn wrap_returns_stmt(stmt: &mut Stmt, elem_ty: &Type, count: &Expr) {
    match stmt {
        Stmt::Return(Some(expr)) => {
            let inner = std::mem::replace(expr, Expr::Var(Ident::new("__slate_placeholder")));
            *expr = box_from_raw(inner, elem_ty, count);
        }
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            for indent in then_body.iter_mut().chain(else_body.iter_mut()) {
                wrap_returns_stmt(&mut indent.stmt, elem_ty, count);
            }
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => {
            for indent in body {
                wrap_returns_stmt(&mut indent.stmt, elem_ty, count);
            }
        }
        Stmt::Match { arms, .. } => {
            for arm in arms {
                for indent in &mut arm.body {
                    wrap_returns_stmt(&mut indent.stmt, elem_ty, count);
                }
            }
        }
        Stmt::While { body, .. } | Stmt::Unsafe { body } | Stmt::Block(body) => {
            for indent in &mut body.stmts {
                wrap_returns_stmt(&mut indent.stmt, elem_ty, count);
            }
        }
        _ => {}
    }
}

fn box_from_raw(ptr: Expr, elem_ty: &Type, count: &Expr) -> Expr {
    let raw = Expr::Cast {
        expr: Box::new(ptr),
        ty: Type::Ptr {
            mutable: true,
            inner: Box::new(elem_ty.clone()),
        },
    };
    let len = Expr::Cast {
        expr: Box::new(count.clone()),
        ty: Type::Prim(Prim::Usize),
    };
    let slice = Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Path(Path::new(
            ["std", "slice", "from_raw_parts_mut"].map(Ident::from),
        ))),
        args: vec![raw, len],
    };
    Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(["Box", "from_raw"].map(Ident::from)))),
            args: vec![slice],
        })),
    }))
}

fn rewrite_return_call_item(item: &mut Item, elems: &BTreeMap<String, Type>) {
    match item {
        Item::Fn(f) => {
            for indent in &mut f.body {
                rewrite_return_call_stmt(&mut indent.stmt, elems);
            }
        }
        Item::InlineMod { items, .. } => {
            for item in items {
                rewrite_return_call_item(item, elems);
            }
        }
        _ => {}
    }
}

fn rewrite_return_call_stmt(stmt: &mut Stmt, elems: &BTreeMap<String, Type>) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        } => rewrite_return_call_expr(expr, elems),
        Stmt::Let { init: None, .. }
        | Stmt::Return(None)
        | Stmt::Break(_)
        | Stmt::Continue(_)
        | Stmt::InlineAsm(_) => {}
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            rewrite_return_call_expr(cond, elems);
            for indent in then_body.iter_mut().chain(else_body.iter_mut()) {
                rewrite_return_call_stmt(&mut indent.stmt, elems);
            }
            rewrite_return_call_expr(then_value, elems);
            rewrite_return_call_expr(else_value, elems);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            rewrite_return_call_expr(target, elems);
            rewrite_return_call_expr(value, elems);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => rewrite_return_call_expr(expr, elems),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            rewrite_return_call_expr(cond, elems);
            for indent in then_body.iter_mut().chain(else_body.iter_mut()) {
                rewrite_return_call_stmt(&mut indent.stmt, elems);
            }
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => {
            for indent in body {
                rewrite_return_call_stmt(&mut indent.stmt, elems);
            }
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            for indent in &mut body.stmts {
                rewrite_return_call_stmt(&mut indent.stmt, elems);
            }
            if let Some(tail) = &mut body.tail {
                rewrite_return_call_expr(tail, elems);
            }
        }
        Stmt::Match { expr, arms } => {
            rewrite_return_call_expr(expr, elems);
            for arm in arms {
                for indent in &mut arm.body {
                    rewrite_return_call_stmt(&mut indent.stmt, elems);
                }
            }
        }
    }
}

fn rewrite_return_call_expr(expr: &mut Expr, elems: &BTreeMap<String, Type>) {
    match expr {
        Expr::Call { func, args, .. } => {
            rewrite_return_call_expr(func, elems);
            for arg in args.iter_mut() {
                rewrite_return_call_expr(arg, elems);
            }
            if let Expr::Var(callee) = &**func
                && let Some(elem_ty) = elems.get(callee.as_str())
            {
                let call = std::mem::replace(expr, Expr::Var(Ident::new("__slate_placeholder")));
                *expr = box_into_raw(call, elem_ty);
            }
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => rewrite_return_call_expr(expr, elems),
        Expr::Block(block) | Expr::Unsafe(block) => {
            for indent in &mut block.stmts {
                rewrite_return_call_stmt(&mut indent.stmt, elems);
            }
            if let Some(tail) = &mut block.tail {
                rewrite_return_call_expr(tail, elems);
            }
        }
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        } => {
            rewrite_return_call_expr(lhs, elems);
            rewrite_return_call_expr(rhs, elems);
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            rewrite_return_call_expr(recv, elems);
            for arg in args {
                rewrite_return_call_expr(arg, elems);
            }
        }
        Expr::Index { base, index } => {
            rewrite_return_call_expr(base, elems);
            rewrite_return_call_expr(index, elems);
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => rewrite_return_call_expr(base, elems),
        _ => {}
    }
}

fn box_into_raw(call: Expr, elem_ty: &Type) -> Expr {
    let into_raw = Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Path(Path::new(["Box", "into_raw"].map(Ident::from)))),
        args: vec![call],
    };
    Expr::MethodCallGeneric {
        recv: Box::new(into_raw),
        method: "cast".to_string(),
        type_args: vec![elem_ty.clone()],
        args: Vec::new(),
    }
}
