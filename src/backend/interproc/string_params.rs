use std::collections::{BTreeMap, BTreeSet};

use crate::backend::interproc::{self, CallGraph};
use crate::backend::rust_ast::{
    Block, CLibType, Expr, FnDef, Ident, Item, Pattern, Prim, Program, RustValue, Stmt, Type,
    Visibility,
};

struct CandidateFn {
    param_indices: Vec<usize>,
}

struct CallSite {
    caller: String,
    args: Vec<Expr>,
}

struct AliasInfo {
    tracked: BTreeSet<String>,
    established: BTreeSet<String>,
}

pub(in crate::backend) fn run(program: &mut Program) {
    let fn_defs = collect_fn_defs(&program.items);

    let candidates: BTreeMap<String, CandidateFn> = fn_defs
        .iter()
        .filter_map(|(name, f)| candidate_for(f).map(|c| (name.clone(), c)))
        .collect();
    if candidates.is_empty() {
        return;
    }

    let alias_sets: BTreeMap<(String, usize), AliasInfo> = candidates
        .iter()
        .flat_map(|(name, candidate)| {
            candidate.param_indices.iter().map(|&index| {
                let info = alias_fixpoint(fn_defs[name], &fn_defs[name].params[index].name);
                ((name.clone(), index), info)
            })
        })
        .collect();

    let literal_locals: BTreeMap<String, BTreeMap<String, Vec<u8>>> = fn_defs
        .iter()
        .map(|(name, f)| (name.clone(), literal_string_locals(&f.body)))
        .collect();

    let mut callsites_by_callee: BTreeMap<String, Vec<CallSite>> = BTreeMap::new();
    for (caller, f) in &fn_defs {
        let mut calls = Vec::new();
        collect_calls(&f.body, &mut calls);
        for (callee, args) in calls {
            if candidates.contains_key(&callee) {
                callsites_by_callee
                    .entry(callee)
                    .or_default()
                    .push(CallSite {
                        caller: caller.clone(),
                        args,
                    });
            }
        }
    }

    let mut graph = CallGraph::new(candidates.keys().cloned());
    for (callee, sites) in &callsites_by_callee {
        for site in sites {
            graph.add_edge(&site.caller, callee);
        }
    }

    let mut active: BTreeMap<String, BTreeSet<usize>> = candidates
        .iter()
        .map(|(name, c)| (name.clone(), c.param_indices.iter().copied().collect()))
        .collect();

    let order = interproc::scc_order(&graph);
    let empty_sites: Vec<CallSite> = Vec::new();

    interproc::run_worklist(&graph, &order, |name| {
        let fn_def = fn_defs[name];
        let candidate = &candidates[name];
        let sites = callsites_by_callee.get(name).unwrap_or(&empty_sites);

        let mut new_set = BTreeSet::new();
        for &index in &candidate.param_indices {
            if !active[name].contains(&index) {
                continue;
            }
            let info = &alias_sets[&(name.to_string(), index)];
            let eligible = !body_writes_tracked(&fn_def.body, &info.tracked, &info.established)
                && body_reads_ok(
                    &fn_def.body,
                    &info.tracked,
                    &info.established,
                    &candidates,
                    &active,
                )
                && callers_prove_arg_at(
                    sites,
                    index,
                    &candidates,
                    &active,
                    &alias_sets,
                    &literal_locals,
                );
            if eligible {
                new_set.insert(index);
            }
        }

        if new_set == active[name] {
            false
        } else {
            active.insert(name.to_string(), new_set);
            true
        }
    });

    let lifted: BTreeMap<String, BTreeSet<usize>> = active
        .into_iter()
        .filter(|(_, indices)| !indices.is_empty())
        .collect();
    if lifted.is_empty() {
        return;
    }

    apply_rewrite(&mut program.items, &lifted, &alias_sets, &literal_locals);
}

fn candidate_for(f: &FnDef) -> Option<CandidateFn> {
    if f.name == "main" || f.unsafe_ || f.abi.is_some() || !matches!(f.vis, Visibility::Private) {
        return None;
    }
    let param_indices: Vec<usize> = f
        .params
        .iter()
        .enumerate()
        .filter_map(|(index, param)| is_char_ptr(&param.ty).then_some(index))
        .collect();
    (!param_indices.is_empty()).then_some(CandidateFn { param_indices })
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

fn is_char_ptr(ty: &Type) -> bool {
    let Type::Ptr { inner, .. } = ty else {
        return false;
    };
    match &**inner {
        Type::Prim(Prim::I8 | Prim::U8) => true,
        Type::CLib(CLibType::CHAR) => true,
        Type::Custom(name) => name == "libc::c_char",
        _ => false,
    }
}

fn alias_fixpoint(f: &FnDef, param_name: &str) -> AliasInfo {
    let mut tracked = BTreeSet::from([param_name.to_string()]);
    let mut established = BTreeSet::new();
    loop {
        let before = tracked.clone();
        walk_stmts(&f.body, &mut |stmt| {
            if let Stmt::Let {
                name,
                ty: Some(ty),
                init: Some(Expr::Var(source)),
                ..
            } = stmt
                && is_char_ptr(ty)
                && before.contains(source.as_str())
            {
                tracked.insert(name.clone());
            }
        });
        let mut zero_decl_aliases = BTreeSet::new();
        find_zero_decl_then_assign_aliases(&f.body, &before, &mut zero_decl_aliases);
        established.extend(zero_decl_aliases.iter().cloned());
        tracked.extend(zero_decl_aliases);
        if tracked == before {
            return AliasInfo {
                tracked,
                established,
            };
        }
    }
}

fn find_zero_decl_then_assign_aliases(
    body: &[Stmt],
    tracked: &BTreeSet<String>,
    found: &mut BTreeSet<String>,
) {
    for (decl_pos, decl) in body.iter().enumerate() {
        let Stmt::Let {
            name,
            ty: Some(ty),
            init: Some(init),
            ..
        } = decl
        else {
            continue;
        };
        if !is_char_ptr(ty) || !is_zero_like(init) {
            continue;
        }
        let first_write = body[decl_pos + 1..].iter().find_map(|indent| match indent {
            Stmt::Assign {
                target: Expr::Var(v),
                value,
            } if v.as_str() == name.as_str() => Some(value),
            _ => None,
        });
        if let Some(Expr::Var(source)) = first_write
            && tracked.contains(source.as_str())
        {
            found.insert(name.clone());
        }
    }
    for indent in body {
        match indent {
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
                find_zero_decl_then_assign_aliases(then_body, tracked, found);
                find_zero_decl_then_assign_aliases(else_body, tracked, found);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => {
                find_zero_decl_then_assign_aliases(body, tracked, found)
            }
            Stmt::Unsafe { body } | Stmt::While { body, .. } => {
                find_zero_decl_then_assign_aliases(&body.stmts, tracked, found)
            }
            Stmt::Block(body) => find_zero_decl_then_assign_aliases(&body.stmts, tracked, found),
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    find_zero_decl_then_assign_aliases(&arm.body, tracked, found);
                }
            }
            _ => {}
        }
    }
}

fn is_zero_like(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Value(
            RustValue::I64(0)
                | RustValue::Usize(0)
                | RustValue::I128(0)
                | RustValue::U128(0)
                | RustValue::TypedInt(0, _)
                | RustValue::TypedUInt(0, _)
                | RustValue::NullPtr
                | RustValue::None
        )
    )
}

fn literal_string_locals(body: &[Stmt]) -> BTreeMap<String, Vec<u8>> {
    let mut arrays = BTreeSet::new();
    walk_stmts(body, &mut |stmt| {
        if let Stmt::Let {
            name,
            ty: Some(Type::Array { elem, .. }),
            ..
        } = stmt
            && matches!(**elem, Type::Prim(Prim::I8 | Prim::U8))
        {
            arrays.insert(name.clone());
        }
    });
    let mut temp_array_lits: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    walk_stmts(body, &mut |stmt| {
        let Stmt::Let {
            name,
            init: Some(init),
            ..
        } = stmt
        else {
            return;
        };
        let payload = match init {
            Expr::ArrayLit(items) => array_lit_payload(items),
            Expr::ArrayRepeat { elem, len } => array_repeat_payload(elem, *len),
            _ => None,
        };
        if let Some(payload) = payload {
            temp_array_lits.insert(name.clone(), payload);
        }
    });
    let mut confirmed: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    walk_stmts(body, &mut |stmt| {
        let Stmt::Assign {
            target: Expr::Var(name),
            value,
        } = stmt
        else {
            return;
        };
        if !arrays.contains(name.as_str()) {
            return;
        }
        let payload = match value {
            Expr::ArrayLit(items) => array_lit_payload(items),
            Expr::ArrayRepeat { elem, len } => array_repeat_payload(elem, *len),
            Expr::Var(temp) => temp_array_lits.get(temp.as_str()).cloned(),
            _ => None,
        };
        if let Some(payload) = payload {
            confirmed.insert(name.as_str().to_string(), payload);
        }
    });

    loop {
        let before = confirmed.clone();
        walk_stmts(body, &mut |stmt| {
            if let Stmt::Let {
                name,
                ty: Some(ty),
                init: Some(init),
                ..
            } = stmt
                && is_char_ptr(ty)
                && let Expr::Var(source) = peel_pointer_view(init)
                && let Some(payload) = before.get(source.as_str())
            {
                confirmed.insert(name.clone(), payload.clone());
            }
        });
        if confirmed == before {
            return confirmed;
        }
    }
}

fn array_repeat_payload(elem: &Expr, len: usize) -> Option<Vec<u8>> {
    (len >= 1 && is_zero_literal(elem)).then(Vec::new)
}

fn array_lit_payload(items: &[Expr]) -> Option<Vec<u8>> {
    if !items.last().is_some_and(is_zero_literal) {
        return None;
    }
    let bytes: Vec<u8> = items[..items.len() - 1]
        .iter()
        .map(|item| match item {
            Expr::Value(RustValue::I64(n)) => u8::try_from(*n & 0xff).ok(),
            Expr::Value(RustValue::TypedInt(n, _)) => u8::try_from(*n & 0xff).ok(),
            Expr::Value(RustValue::TypedUInt(n, _)) => u8::try_from(*n & 0xff).ok(),
            _ => None,
        })
        .collect::<Option<Vec<u8>>>()?;
    (!bytes.contains(&0) && std::str::from_utf8(&bytes).is_ok()).then_some(bytes)
}

fn is_zero_literal(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Value(
            RustValue::I64(0)
                | RustValue::Usize(0)
                | RustValue::I128(0)
                | RustValue::U128(0)
                | RustValue::TypedInt(0, _)
                | RustValue::TypedUInt(0, _)
        )
    )
}

fn collect_calls(body: &[Stmt], out: &mut Vec<(String, Vec<Expr>)>) {
    walk_exprs(body, &mut |expr| {
        if let Expr::Call { func, args, .. } = expr
            && let Expr::Var(name) = &**func
        {
            out.push((name.as_str().to_string(), args.clone()));
        }
    });
}

fn walk_stmts(body: &[Stmt], f: &mut impl FnMut(&Stmt)) {
    for indent in body {
        f(indent);
        match indent {
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
                walk_stmts(then_body, f);
                walk_stmts(else_body, f);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => walk_stmts(body, f),
            Stmt::Unsafe { body } => walk_stmts(&body.stmts, f),
            Stmt::While { body, .. } => walk_stmts(&body.stmts, f),
            Stmt::Block(body) => walk_stmts(&body.stmts, f),
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    walk_stmts(&arm.body, f);
                }
            }
            _ => {}
        }
    }
}

fn walk_exprs(body: &[Stmt], f: &mut impl FnMut(&Expr)) {
    walk_stmts(body, &mut |stmt| {
        stmt_visit_exprs(stmt, f);
    });
}

fn stmt_visit_exprs(stmt: &Stmt, f: &mut impl FnMut(&Expr)) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        } => visit_expr_deep(expr, f),
        Stmt::Let { init: None, .. } => {}
        Stmt::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => {
            visit_expr_deep(cond, f);
            visit_expr_deep(then_value, f);
            visit_expr_deep(else_value, f);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            visit_expr_deep(target, f);
            visit_expr_deep(value, f);
        }
        Stmt::InlineAsm(asm) => {
            for operand in &asm.operands {
                operand.visit_exprs(&mut |e| visit_expr_deep(e, f));
            }
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => visit_expr_deep(expr, f),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::For { iter, .. } => visit_expr_deep(iter, f),
        Stmt::Loop { .. } | Stmt::Scope { .. } | Stmt::LabeledBlock { .. } => {}
        Stmt::Unsafe { body } => {
            if let Some(tail) = &body.tail {
                visit_expr_deep(tail, f);
            }
        }
        Stmt::If { cond, .. } => visit_expr_deep(cond, f),
        Stmt::Match { expr, arms } => {
            visit_expr_deep(expr, f);
            for arm in arms {
                if let Pattern::Guarded { cond, .. } = &arm.pattern {
                    visit_expr_deep(cond, f);
                }
            }
        }
        Stmt::While { cond, body, .. } => {
            visit_expr_deep(cond, f);
            if let Some(tail) = &body.tail {
                visit_expr_deep(tail, f);
            }
        }
        Stmt::Block(body) => {
            if let Some(tail) = &body.tail {
                visit_expr_deep(tail, f);
            }
        }
    }
}

fn visit_expr_deep(expr: &Expr, f: &mut impl FnMut(&Expr)) {
    f(expr);
    match expr {
        Expr::Call { func, args, .. } => {
            visit_expr_deep(func, f);
            for arg in args {
                visit_expr_deep(arg, f);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            visit_expr_deep(recv, f);
            for arg in args {
                visit_expr_deep(arg, f);
            }
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => visit_expr_deep(expr, f),
        Expr::Block(block) | Expr::Unsafe(block) => visit_block_deep(block, f),
        Expr::Binary { lhs, rhs, .. } => {
            visit_expr_deep(lhs, f);
            visit_expr_deep(rhs, f);
        }
        Expr::Range { start, end } => {
            visit_expr_deep(start, f);
            visit_expr_deep(end, f);
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => visit_expr_deep(base, f),
        Expr::ArrayPtr { array, .. } => visit_expr_deep(array, f),
        Expr::Index { base, index } => {
            visit_expr_deep(base, f);
            visit_expr_deep(index, f);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                visit_expr_deep(value, f);
            }
        }
        Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) | Expr::VecLit(fields) => {
            for elem in fields {
                visit_expr_deep(elem, f);
            }
        }
        Expr::ArrayRepeat { elem, .. } => visit_expr_deep(elem, f),
        Expr::VecRepeat { elem, len } => {
            visit_expr_deep(elem, f);
            visit_expr_deep(len, f);
        }
        Expr::Closure { body, .. } => visit_expr_deep(body, f),
        Expr::Macro { args, .. } => {
            for arg in args {
                visit_expr_deep(arg, f);
            }
        }
        Expr::Match { expr, arms } => {
            visit_expr_deep(expr, f);
            for arm in arms {
                visit_expr_deep(&arm.value, f);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            visit_expr_deep(cond, f);
            visit_expr_deep(then_expr, f);
            visit_expr_deep(else_expr, f);
        }
        _ => {}
    }
}

fn visit_block_deep(block: &Block, f: &mut impl FnMut(&Expr)) {
    walk_exprs(&block.stmts, f);
    if let Some(tail) = &block.tail {
        visit_expr_deep(tail, f);
    }
}

fn body_writes_tracked(
    body: &[Stmt],
    tracked: &BTreeSet<String>,
    established: &BTreeSet<String>,
) -> bool {
    let mut found = false;
    walk_stmts(body, &mut |stmt| {
        if let Stmt::CompoundAssign {
            target: Expr::Var(v),
            ..
        } = stmt
            && tracked.contains(v.as_str())
        {
            found = true;
        }
        if let Stmt::Assign {
            target: Expr::Var(v),
            ..
        } = stmt
            && tracked.contains(v.as_str())
            && !established.contains(v.as_str())
        {
            found = true;
        }
    });
    found
}

struct Ctx<'a> {
    candidates: &'a BTreeMap<String, CandidateFn>,
    active: &'a BTreeMap<String, BTreeSet<usize>>,
    established: &'a BTreeSet<String>,
}

impl Ctx<'_> {
    fn arg_use_allowed(&self, callee_name: Option<&str>, index: usize) -> bool {
        let Some(name) = callee_name else {
            return false;
        };
        if is_supported_libc_str_fn(name) {
            return true;
        }
        if self.candidates.contains_key(name) {
            return self
                .active
                .get(name)
                .is_some_and(|set| set.contains(&index));
        }
        false
    }
}

fn body_reads_ok(
    body: &[Stmt],
    tracked: &BTreeSet<String>,
    established: &BTreeSet<String>,
    candidates: &BTreeMap<String, CandidateFn>,
    active: &BTreeMap<String, BTreeSet<usize>>,
) -> bool {
    let ctx = Ctx {
        candidates,
        active,
        established,
    };
    body.iter().all(|s| stmt_ok(s, tracked, &ctx))
}

fn stmt_ok(stmt: &Stmt, tracked: &BTreeSet<String>, ctx: &Ctx) -> bool {
    match stmt {
        Stmt::Let {
            ty: Some(ty),
            init: Some(Expr::Var(source)),
            ..
        } if is_char_ptr(ty) && tracked.contains(source.as_str()) => true,
        Stmt::Let {
            init: Some(init), ..
        } => expr_ok(init, tracked, ctx),
        Stmt::Let { init: None, .. } => true,
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_ok(cond, tracked, ctx)
                && then_body.iter().all(|s| stmt_ok(s, tracked, ctx))
                && expr_ok(then_value, tracked, ctx)
                && else_body.iter().all(|s| stmt_ok(s, tracked, ctx))
                && expr_ok(else_value, tracked, ctx)
        }
        Stmt::Assign {
            target: Expr::Var(v),
            value: Expr::Var(source),
        } if ctx.established.contains(v.as_str()) && tracked.contains(source.as_str()) => true,
        Stmt::Assign {
            target: Expr::Var(v),
            value,
        } if tracked.contains(v.as_str()) => expr_ok(value, tracked, ctx),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_ok(target, tracked, ctx) && expr_ok(value, tracked, ctx)
        }
        Stmt::InlineAsm(asm) => {
            let mut ok = true;
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| ok &= expr_ok(expr, tracked, ctx));
            }
            ok
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_ok(expr, tracked, ctx),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => true,
        Stmt::For { iter, body, .. } => {
            expr_ok(iter, tracked, ctx) && body.iter().all(|s| stmt_ok(s, tracked, ctx))
        }
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } | Stmt::Loop { body, .. } => {
            body.iter().all(|s| stmt_ok(s, tracked, ctx))
        }
        Stmt::Unsafe { body } => block_ok(body, tracked, ctx),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_ok(cond, tracked, ctx)
                && then_body
                    .iter()
                    .chain(else_body.iter())
                    .all(|s| stmt_ok(s, tracked, ctx))
        }
        Stmt::Match { expr, arms } => {
            expr_ok(expr, tracked, ctx)
                && arms.iter().all(|arm| {
                    let guard_ok = match &arm.pattern {
                        Pattern::Guarded { cond, .. } => expr_ok(cond, tracked, ctx),
                        _ => true,
                    };
                    guard_ok && arm.body.iter().all(|s| stmt_ok(s, tracked, ctx))
                })
        }
        Stmt::While { cond, body, .. } => {
            expr_ok(cond, tracked, ctx) && block_ok(body, tracked, ctx)
        }
        Stmt::Block(body) => block_ok(body, tracked, ctx),
    }
}

fn block_ok(block: &Block, tracked: &BTreeSet<String>, ctx: &Ctx) -> bool {
    block.stmts.iter().all(|s| stmt_ok(s, tracked, ctx))
        && block.tail.as_ref().is_none_or(|t| expr_ok(t, tracked, ctx))
}

fn expr_ok(expr: &Expr, tracked: &BTreeSet<String>, ctx: &Ctx) -> bool {
    match expr {
        Expr::Var(v) => !tracked.contains(v.as_str()),
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::ConstBlock(_)
        | Expr::CStr(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => true,
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_ok(expr, tracked, ctx),
        Expr::Block(block) | Expr::Unsafe(block) => block_ok(block, tracked, ctx),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            expr_ok(src, tracked, ctx) && expr_ok(dst, tracked, ctx)
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            expr_ok(src, tracked, ctx) && expr_ok(dst, tracked, ctx) && expr_ok(count, tracked, ctx)
        }
        Expr::WriteBytes { dst, val, count } => {
            expr_ok(dst, tracked, ctx) && expr_ok(val, tracked, ctx) && expr_ok(count, tracked, ctx)
        }
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            place.ptr_expr().is_none_or(|p| expr_ok(p, tracked, ctx))
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            place.ptr_expr().is_none_or(|p| expr_ok(p, tracked, ctx))
                && expr_ok(value, tracked, ctx)
        }
        Expr::AtomicNew { value, .. } => expr_ok(value, tracked, ctx),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            place.ptr_expr().is_none_or(|p| expr_ok(p, tracked, ctx))
                && expr_ok(expected, tracked, ctx)
                && expr_ok(desired, tracked, ctx)
        }
        Expr::Binary { lhs, rhs, .. } => expr_ok(lhs, tracked, ctx) && expr_ok(rhs, tracked, ctx),
        Expr::Range { start, end } => expr_ok(start, tracked, ctx) && expr_ok(end, tracked, ctx),
        Expr::Call { func, args, .. } => {
            let callee_name = match &**func {
                Expr::Var(v) => Some(v.as_str()),
                _ => None,
            };
            let mut ok = expr_ok(func, tracked, ctx);
            for (index, arg) in args.iter().enumerate() {
                let peeled = peel_pointer_view(arg);
                if let Expr::Var(v) = peeled
                    && tracked.contains(v.as_str())
                {
                    ok &= ctx.arg_use_allowed(callee_name, index);
                } else {
                    ok &= expr_ok(arg, tracked, ctx);
                }
            }
            ok
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_ok(recv, tracked, ctx) && args.iter().all(|a| expr_ok(a, tracked, ctx))
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => expr_ok(base, tracked, ctx),
        Expr::ArrayPtr { array, .. } => expr_ok(array, tracked, ctx),
        Expr::Index { base, index } => expr_ok(base, tracked, ctx) && expr_ok(index, tracked, ctx),
        Expr::StructLit { fields, .. } => fields.iter().all(|(_, v)| expr_ok(v, tracked, ctx)),
        Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) | Expr::VecLit(fields) => {
            fields.iter().all(|e| expr_ok(e, tracked, ctx))
        }
        Expr::ArrayRepeat { elem, .. } => expr_ok(elem, tracked, ctx),
        Expr::VecRepeat { elem, len } => expr_ok(elem, tracked, ctx) && expr_ok(len, tracked, ctx),
        Expr::Closure { params, body } => {
            params.iter().any(|p| tracked.contains(p.as_str())) || expr_ok(body, tracked, ctx)
        }
        Expr::Macro { args, .. } => args.iter().all(|a| expr_ok(a, tracked, ctx)),
        Expr::Match { expr, arms } => {
            expr_ok(expr, tracked, ctx) && arms.iter().all(|arm| expr_ok(&arm.value, tracked, ctx))
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_ok(cond, tracked, ctx)
                && expr_ok(then_expr, tracked, ctx)
                && expr_ok(else_expr, tracked, ctx)
        }
    }
}

fn is_supported_libc_str_fn(name: &str) -> bool {
    matches!(
        name,
        "strlen"
            | "strnlen"
            | "strcmp"
            | "strncmp"
            | "memcmp"
            | "strchr"
            | "strrchr"
            | "strstr"
            | "strpbrk"
            | "strspn"
            | "strcspn"
            | "atoi"
            | "atol"
            | "strtol"
            | "strtoul"
            | "strtod"
    )
}

fn callers_prove_arg_at(
    sites: &[CallSite],
    index: usize,
    candidates: &BTreeMap<String, CandidateFn>,
    active: &BTreeMap<String, BTreeSet<usize>>,
    alias_sets: &BTreeMap<(String, usize), AliasInfo>,
    literal_locals: &BTreeMap<String, BTreeMap<String, Vec<u8>>>,
) -> bool {
    if sites.is_empty() {
        return false;
    }
    sites.iter().all(|site| {
        site.args.get(index).is_some_and(|arg| {
            arg_is_liftable_source(
                arg,
                &site.caller,
                candidates,
                active,
                alias_sets,
                literal_locals,
            )
        })
    })
}

fn arg_is_liftable_source(
    expr: &Expr,
    caller: &str,
    candidates: &BTreeMap<String, CandidateFn>,
    active: &BTreeMap<String, BTreeSet<usize>>,
    alias_sets: &BTreeMap<(String, usize), AliasInfo>,
    literal_locals: &BTreeMap<String, BTreeMap<String, Vec<u8>>>,
) -> bool {
    match peel_pointer_view(expr) {
        Expr::Var(v) => {
            let passes_through_active_param = candidates
                .get(caller)
                .into_iter()
                .flat_map(|c| c.param_indices.iter())
                .any(|&p| {
                    active.get(caller).is_some_and(|set| set.contains(&p))
                        && alias_sets
                            .get(&(caller.to_string(), p))
                            .is_some_and(|info| info.tracked.contains(v.as_str()))
                });
            passes_through_active_param
                || literal_locals
                    .get(caller)
                    .is_some_and(|map| map.contains_key(v.as_str()))
        }
        Expr::CStr(bytes) => c_string_payload(bytes).is_some(),
        _ => false,
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

fn c_string_payload(bytes: &[u8]) -> Option<&[u8]> {
    let payload = bytes.strip_suffix(&[0])?;
    (!payload.contains(&0) && std::str::from_utf8(payload).is_ok()).then_some(payload)
}

fn apply_rewrite(
    items: &mut [Item],
    lifted: &BTreeMap<String, BTreeSet<usize>>,
    alias_sets: &BTreeMap<(String, usize), AliasInfo>,
    literal_locals: &BTreeMap<String, BTreeMap<String, Vec<u8>>>,
) {
    let empty_literals: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    for item in items {
        match item {
            Item::Fn(f) => {
                let mut own_tracked = BTreeSet::new();
                if let Some(indices) = lifted.get(&f.name) {
                    for &index in indices {
                        if let Some(param) = f.params.get_mut(index) {
                            param.ty = str_ref_type();
                        }
                        if let Some(info) = alias_sets.get(&(f.name.clone(), index)) {
                            own_tracked.extend(info.tracked.iter().cloned());
                        }
                    }
                    promote_tracked_lets(&mut f.body, &own_tracked);
                }
                let own_literals = literal_locals.get(&f.name).unwrap_or(&empty_literals);
                let ctx = RewriteCtx {
                    targets: lifted,
                    own_tracked: &own_tracked,
                    own_literals,
                };
                rewrite_calls_in_stmts(&mut f.body, &ctx);
            }
            Item::InlineMod { items, .. } => {
                apply_rewrite(items, lifted, alias_sets, literal_locals)
            }
            _ => {}
        }
    }
}

fn str_ref_type() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Str),
    }
}

fn is_str_ref(ty: &Type) -> bool {
    matches!(ty, Type::Ref { inner, .. } if matches!(**inner, Type::Str))
}

/// Promotes every `Let` declaring one of `tracked`'s names (the full alias
/// chain computed by `alias_fixpoint`, including the pre-`ZeroInit`
/// null-decl-then-assign shape) from a char-pointer type to `&str`. This
/// subsumes the old single-level "just the param's direct aliases" rewrite:
/// since `tracked` is already the transitive closure, one promotion pass
/// over it handles every hop.
fn promote_tracked_lets(body: &mut [Stmt], tracked: &BTreeSet<String>) -> bool {
    let mut changed = false;
    for indent in body.iter_mut() {
        if let Stmt::Let {
            name, ty: Some(ty), ..
        } = indent
            && is_char_ptr(ty)
            && tracked.contains(name.as_str())
        {
            *ty = str_ref_type();
            changed = true;
        }
        changed |= promote_nested_bodies(indent, tracked);
    }
    changed
}

fn promote_nested_bodies(stmt: &mut Stmt, tracked: &BTreeSet<String>) -> bool {
    match stmt {
        Stmt::If {
            then_body,
            else_body,
            ..
        }
        | Stmt::LetIf {
            then_body,
            else_body,
            ..
        } => promote_tracked_lets(then_body, tracked) | promote_tracked_lets(else_body, tracked),
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => promote_tracked_lets(body, tracked),
        Stmt::Unsafe { body } | Stmt::While { body, .. } => {
            promote_tracked_lets(&mut body.stmts, tracked)
        }
        Stmt::Block(body) => promote_tracked_lets(&mut body.stmts, tracked),
        Stmt::Match { arms, .. } => {
            let mut changed = false;
            for arm in arms {
                changed |= promote_tracked_lets(&mut arm.body, tracked);
            }
            changed
        }
        _ => false,
    }
}

struct RewriteCtx<'a> {
    targets: &'a BTreeMap<String, BTreeSet<usize>>,
    own_tracked: &'a BTreeSet<String>,
    own_literals: &'a BTreeMap<String, Vec<u8>>,
}

fn rewrite_calls_in_stmts(body: &mut [Stmt], ctx: &RewriteCtx) -> bool {
    let mut changed = false;
    for indent in body.iter_mut() {
        changed |= rewrite_calls_in_stmt(indent, ctx);
    }
    changed
}

fn rewrite_calls_in_stmt(stmt: &mut Stmt, ctx: &RewriteCtx) -> bool {
    match stmt {
        Stmt::Let { init: Some(e), .. } => rewrite_calls_in_expr(e, ctx),
        Stmt::Let { init: None, .. } => false,
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            let mut changed = rewrite_calls_in_expr(cond, ctx);
            changed |= rewrite_calls_in_stmts(then_body, ctx);
            changed |= rewrite_calls_in_expr(then_value, ctx);
            changed |= rewrite_calls_in_stmts(else_body, ctx);
            changed |= rewrite_calls_in_expr(else_value, ctx);
            changed
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            let mut changed = rewrite_calls_in_expr(target, ctx);
            changed |= rewrite_calls_in_expr(value, ctx);
            changed
        }
        Stmt::InlineAsm(asm) => {
            let mut changed = false;
            for operand in &mut asm.operands {
                operand.visit_exprs_mut(&mut |e| changed |= rewrite_calls_in_expr(e, ctx));
            }
            changed
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => rewrite_calls_in_expr(expr, ctx),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
        Stmt::For { iter, body, .. } => {
            let mut changed = rewrite_calls_in_expr(iter, ctx);
            changed |= rewrite_calls_in_stmts(body, ctx);
            changed
        }
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } | Stmt::Loop { body, .. } => {
            rewrite_calls_in_stmts(body, ctx)
        }
        Stmt::Unsafe { body } => rewrite_calls_in_block(body, ctx),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            let mut changed = rewrite_calls_in_expr(cond, ctx);
            changed |= rewrite_calls_in_stmts(then_body, ctx);
            changed |= rewrite_calls_in_stmts(else_body, ctx);
            changed
        }
        Stmt::Match { expr, arms } => {
            let mut changed = rewrite_calls_in_expr(expr, ctx);
            for arm in arms {
                if let Pattern::Guarded { cond, .. } = &mut arm.pattern {
                    changed |= rewrite_calls_in_expr(cond, ctx);
                }
                changed |= rewrite_calls_in_stmts(&mut arm.body, ctx);
            }
            changed
        }
        Stmt::While { cond, body, .. } => {
            let mut changed = rewrite_calls_in_expr(cond, ctx);
            changed |= rewrite_calls_in_block(body, ctx);
            changed
        }
        Stmt::Block(body) => rewrite_calls_in_block(body, ctx),
    }
}

fn rewrite_calls_in_block(block: &mut Block, ctx: &RewriteCtx) -> bool {
    let mut changed = rewrite_calls_in_stmts(&mut block.stmts, ctx);
    if let Some(tail) = &mut block.tail {
        changed |= rewrite_calls_in_expr(tail, ctx);
    }
    changed
}

fn rewrite_calls_in_expr(expr: &mut Expr, ctx: &RewriteCtx) -> bool {
    let mut changed = false;

    if let Expr::Cast { expr: inner, ty } = expr
        && !is_str_ref(ty)
        && let Expr::Var(name) = &**inner
        && ctx.own_tracked.contains(name.as_str())
    {
        let old = std::mem::replace(&mut **inner, Expr::Todo(String::new()));
        **inner = Expr::MethodCall {
            recv: Box::new(old),
            method: "as_ptr".to_string(),
            args: Vec::new(),
        };
        changed = true;
    }

    if let Expr::Call { func, args, .. } = expr {
        changed |= rewrite_calls_in_expr(func, ctx);
        if let Expr::Var(name) = &**func
            && let Some(indices) = ctx.targets.get(name.as_str())
        {
            for &index in indices {
                if let Some(arg) = args.get_mut(index)
                    && let Some(replacement) = lifted_arg(arg, ctx.own_literals)
                {
                    *arg = replacement;
                    changed = true;
                }
            }
        }
        for arg in args.iter_mut() {
            changed |= rewrite_calls_in_expr(arg, ctx);
        }
        return changed;
    }
    match expr {
        Expr::Var(_)
        | Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::ConstBlock(_)
        | Expr::CStr(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => {}
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => changed |= rewrite_calls_in_expr(expr, ctx),
        Expr::Block(block) | Expr::Unsafe(block) => changed |= rewrite_calls_in_block(block, ctx),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            changed |= rewrite_calls_in_expr(src, ctx);
            changed |= rewrite_calls_in_expr(dst, ctx);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            changed |= rewrite_calls_in_expr(src, ctx);
            changed |= rewrite_calls_in_expr(dst, ctx);
            changed |= rewrite_calls_in_expr(count, ctx);
        }
        Expr::WriteBytes { dst, val, count } => {
            changed |= rewrite_calls_in_expr(dst, ctx);
            changed |= rewrite_calls_in_expr(val, ctx);
            changed |= rewrite_calls_in_expr(count, ctx);
        }
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            if let Some(p) = place.ptr_expr_mut() {
                changed |= rewrite_calls_in_expr(p, ctx);
            }
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            if let Some(p) = place.ptr_expr_mut() {
                changed |= rewrite_calls_in_expr(p, ctx);
            }
            changed |= rewrite_calls_in_expr(value, ctx);
        }
        Expr::AtomicNew { value, .. } => changed |= rewrite_calls_in_expr(value, ctx),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            if let Some(p) = place.ptr_expr_mut() {
                changed |= rewrite_calls_in_expr(p, ctx);
            }
            changed |= rewrite_calls_in_expr(expected, ctx);
            changed |= rewrite_calls_in_expr(desired, ctx);
        }
        Expr::Binary { lhs, rhs, .. } => {
            changed |= rewrite_calls_in_expr(lhs, ctx);
            changed |= rewrite_calls_in_expr(rhs, ctx);
        }
        Expr::Range { start, end } => {
            changed |= rewrite_calls_in_expr(start, ctx);
            changed |= rewrite_calls_in_expr(end, ctx);
        }
        Expr::Call { .. } => unreachable!("handled above"),
        Expr::MethodCall { recv, args, .. } => {
            changed |= rewrite_calls_in_expr(recv, ctx);
            for arg in args {
                changed |= rewrite_calls_in_expr(arg, ctx);
            }
        }
        Expr::MethodCallGeneric { recv, args, .. } => {
            changed |= rewrite_calls_in_expr(recv, ctx);
            for arg in args {
                changed |= rewrite_calls_in_expr(arg, ctx);
            }
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
            changed |= rewrite_calls_in_expr(base, ctx)
        }
        Expr::ArrayPtr { array, .. } => changed |= rewrite_calls_in_expr(array, ctx),
        Expr::Index { base, index } => {
            changed |= rewrite_calls_in_expr(base, ctx);
            changed |= rewrite_calls_in_expr(index, ctx);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                changed |= rewrite_calls_in_expr(value, ctx);
            }
        }
        Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) | Expr::VecLit(fields) => {
            for elem in fields {
                changed |= rewrite_calls_in_expr(elem, ctx);
            }
        }
        Expr::ArrayRepeat { elem, .. } => changed |= rewrite_calls_in_expr(elem, ctx),
        Expr::VecRepeat { elem, len } => {
            changed |= rewrite_calls_in_expr(elem, ctx);
            changed |= rewrite_calls_in_expr(len, ctx);
        }
        Expr::Closure { body, .. } => changed |= rewrite_calls_in_expr(body, ctx),
        Expr::Macro { args, .. } => {
            for arg in args {
                changed |= rewrite_calls_in_expr(arg, ctx);
            }
        }
        Expr::Match { expr, arms } => {
            changed |= rewrite_calls_in_expr(expr, ctx);
            for arm in arms {
                changed |= rewrite_calls_in_expr(&mut arm.value, ctx);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            changed |= rewrite_calls_in_expr(cond, ctx);
            changed |= rewrite_calls_in_expr(then_expr, ctx);
            changed |= rewrite_calls_in_expr(else_expr, ctx);
        }
    }
    changed
}

fn lifted_arg(expr: &Expr, literals: &BTreeMap<String, Vec<u8>>) -> Option<Expr> {
    match expr {
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => lifted_arg(expr, literals),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            lifted_arg(recv, literals)
        }
        Expr::ArrayPtr { array, .. } => lifted_arg(array, literals),
        Expr::Var(name) => {
            if let Some(bytes) = literals.get(name.as_str()) {
                return std::str::from_utf8(bytes)
                    .ok()
                    .map(|s| Expr::Str(s.to_string()));
            }
            Some(Expr::Var(Ident::new(name.as_str())))
        }
        Expr::CStr(bytes) => {
            let payload = c_string_payload(bytes)?;
            Some(Expr::Str(std::str::from_utf8(payload).ok()?.to_string()))
        }
        _ => None,
    }
}
