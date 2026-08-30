use std::collections::{BTreeMap, BTreeSet};

use crate::backend::interproc::{self, CallGraph};
use crate::backend::rust_ast::{
    BinOp, Block, Expr, FnDef, Ident, IndentStmt, Item, Path, Prim, Program, RustValue, Stmt, Type,
    UnaryOp, Visibility,
};
use crate::function_identity::{CallBinding, FunctionIdentity, Known};

fn source_var(expr: &Expr) -> Option<Ident> {
    match expr {
        Expr::Var(v) => Some(*v),
        Expr::Cast { expr, .. } => source_var(expr),
        _ => None,
    }
}

fn frame_field_key(expr: &Expr) -> Option<String> {
    if let Expr::TupleField { base, index } = expr
        && let Expr::Var(base) = &**base
        && base.as_str().starts_with("__slate_alloca_frame")
    {
        return Some(format!("{}.{index}", base.as_str()));
    }
    None
}

fn place_key(expr: &Expr) -> Option<String> {
    match expr {
        Expr::Var(v) => Some(v.to_string()),
        Expr::Cast { expr, .. } => place_key(expr),
        _ => frame_field_key(expr),
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub(in crate::backend) struct PointerBinding {
    pub function: String,
    pub name: String,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::backend) enum ResolvedPtrType {
    Ref,
    RefMut,
    RefCell,
    Owned,
    Slice,
    SliceMut,
    Vec,
    Str,
    StringOwned,
    RawConst,
    RawMut,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::backend) struct Representation {
    pub base: ResolvedPtrType,
    pub nullable: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct PointerFact {
    elem_is_byte: bool,
    write: bool,
    unique: bool,
    free: bool,
    offset: bool,
    escape: bool,
    nullable: bool,
    string: bool,
}

impl PointerFact {
    fn new(elem_is_byte: bool) -> Self {
        Self {
            elem_is_byte,
            write: false,
            unique: true,
            free: false,
            offset: false,
            escape: false,
            nullable: false,
            string: false,
        }
    }

    fn observe_write(&mut self) -> bool {
        flip_up(&mut self.write)
    }

    fn observe_offset(&mut self) -> bool {
        flip_up(&mut self.offset)
    }

    fn observe_free(&mut self) -> bool {
        flip_up(&mut self.free)
    }

    fn observe_escape(&mut self) -> bool {
        flip_up(&mut self.escape)
    }

    fn observe_nullable(&mut self) -> bool {
        flip_up(&mut self.nullable)
    }

    fn observe_string(&mut self) -> bool {
        if !self.elem_is_byte {
            return false;
        }
        flip_up(&mut self.string)
    }

    fn disprove_unique(&mut self) -> bool {
        flip_down(&mut self.unique)
    }

    fn merge_evidence_from(&mut self, other: &PointerFact) -> bool {
        let mut changed = false;
        if other.write {
            changed |= self.observe_write();
        }
        if other.offset {
            changed |= self.observe_offset();
        }
        if other.free {
            changed |= self.observe_free();
        }
        if other.escape {
            changed |= self.observe_escape();
        }
        if other.nullable {
            changed |= self.observe_nullable();
        }
        if other.string {
            changed |= self.observe_string();
        }
        if !other.unique {
            changed |= self.disprove_unique();
        }
        changed
    }

    pub(in crate::backend) fn resolved(&self) -> Representation {
        if self.escape && !(self.unique && self.free) {
            let base = if self.write {
                ResolvedPtrType::RawMut
            } else {
                ResolvedPtrType::RawConst
            };
            return Representation {
                base,
                nullable: self.nullable,
            };
        }
        let raw = || {
            if self.write {
                ResolvedPtrType::RawMut
            } else {
                ResolvedPtrType::RawConst
            }
        };
        let base = match (self.write, self.unique, self.free, self.offset) {
            (_, true, true, false) => ResolvedPtrType::Owned,
            (_, true, true, true) => ResolvedPtrType::Vec,
            (_, false, true, _) => raw(),
            (true, false, false, false) => ResolvedPtrType::RefCell,
            (true, false, false, true) => raw(),
            (true, true, false, false) => ResolvedPtrType::RefMut,
            (true, true, false, true) => ResolvedPtrType::SliceMut,
            (false, _, false, false) => ResolvedPtrType::Ref,
            (false, _, false, true) => ResolvedPtrType::Slice,
        };
        Representation {
            base: self.specialize_string(base),
            nullable: self.nullable,
        }
    }

    fn specialize_string(&self, base: ResolvedPtrType) -> ResolvedPtrType {
        if !self.string {
            return base;
        }
        match base {
            ResolvedPtrType::Slice => ResolvedPtrType::Str,
            ResolvedPtrType::Vec if !self.write => ResolvedPtrType::StringOwned,
            other => other,
        }
    }
}

fn flip_up(bit: &mut bool) -> bool {
    if *bit {
        false
    } else {
        *bit = true;
        true
    }
}

fn flip_down(bit: &mut bool) -> bool {
    if *bit {
        *bit = false;
        true
    } else {
        false
    }
}

struct Candidate {
    param_indices: Vec<usize>,
}

struct CallSite {
    caller: String,
    callee: String,
    arg_index: usize,
    caller_binding: String,
}

struct ReturnCallSite {
    caller: String,
    callee: String,
    param_index: usize,
    result_binding: String,
}

#[derive(Clone, Copy, PartialEq, Eq)]
struct ReturnAlias {
    param_index: usize,
    elem_is_byte: bool,
}

pub(in crate::backend) fn solve(program: &Program) -> BTreeMap<PointerBinding, PointerFact> {
    let fn_defs = collect_fn_defs(&program.items);
    let candidates: BTreeMap<String, Candidate> = fn_defs
        .iter()
        .filter_map(|(name, f)| candidate_for(f).map(|c| (name.clone(), c)))
        .collect();
    if candidates.is_empty() {
        return BTreeMap::new();
    }

    let return_aliases = collect_return_aliases(&fn_defs, &candidates);

    let mut facts: BTreeMap<PointerBinding, PointerFact> = BTreeMap::new();
    let mut call_sites: Vec<CallSite> = Vec::new();
    let mut return_sites: Vec<ReturnCallSite> = Vec::new();
    for (name, f) in &fn_defs {
        let (local, sites, local_return_sites) =
            classify_function(name, f, candidates.get(name), &candidates, &return_aliases);
        facts.extend(local);
        call_sites.extend(sites);
        return_sites.extend(local_return_sites);
    }

    let mut outbound: BTreeMap<String, Vec<&CallSite>> = BTreeMap::new();
    let mut inbound: BTreeMap<String, Vec<&CallSite>> = BTreeMap::new();
    for site in &call_sites {
        outbound.entry(site.caller.clone()).or_default().push(site);
        inbound.entry(site.callee.clone()).or_default().push(site);
    }

    let mut return_incident: BTreeMap<String, Vec<&ReturnCallSite>> = BTreeMap::new();
    for site in &return_sites {
        return_incident
            .entry(site.caller.clone())
            .or_default()
            .push(site);
        if site.callee != site.caller {
            return_incident
                .entry(site.callee.clone())
                .or_default()
                .push(site);
        }
    }

    let mut graph = CallGraph::new(fn_defs.keys().cloned());
    for site in &call_sites {
        graph.add_edge(&site.caller, &site.callee);
    }
    for site in &return_sites {
        graph.add_edge(&site.caller, &site.callee);
    }
    let order = interproc::scc_order(&graph);
    let empty_calls: Vec<&CallSite> = Vec::new();
    let empty_returns: Vec<&ReturnCallSite> = Vec::new();

    interproc::run_worklist(&graph, &order, |name| {
        let mut changed = false;
        for site in outbound.get(name).unwrap_or(&empty_calls) {
            let callee_binding = PointerBinding {
                function: site.callee.clone(),
                name: callee_param_name(&fn_defs, &site.callee, site.arg_index),
            };
            let Some(callee_fact) = facts.get(&callee_binding).cloned() else {
                continue;
            };
            let caller_binding = PointerBinding {
                function: site.caller.clone(),
                name: site.caller_binding.clone(),
            };
            if let Some(caller_fact) = facts.get_mut(&caller_binding) {
                changed |= caller_fact.merge_evidence_from(&callee_fact);
            }
        }
        for site in inbound.get(name).unwrap_or(&empty_calls) {
            let caller_binding = PointerBinding {
                function: site.caller.clone(),
                name: site.caller_binding.clone(),
            };
            let Some(caller_fact) = facts.get(&caller_binding).cloned() else {
                continue;
            };
            let callee_binding = PointerBinding {
                function: name.to_string(),
                name: callee_param_name(&fn_defs, name, site.arg_index),
            };
            if let Some(callee_fact) = facts.get_mut(&callee_binding) {
                changed |= callee_fact.merge_evidence_from(&caller_fact);
            }
        }
        for site in return_incident.get(name).unwrap_or(&empty_returns) {
            let result_binding = PointerBinding {
                function: site.caller.clone(),
                name: site.result_binding.clone(),
            };
            let callee_binding = PointerBinding {
                function: site.callee.clone(),
                name: callee_param_name(&fn_defs, &site.callee, site.param_index),
            };
            if let Some(result_fact) = facts.get(&result_binding).cloned()
                && let Some(callee_fact) = facts.get_mut(&callee_binding)
            {
                changed |= callee_fact.merge_evidence_from(&result_fact);
            }
            if let Some(callee_fact) = facts.get(&callee_binding).cloned()
                && let Some(result_fact) = facts.get_mut(&result_binding)
            {
                changed |= result_fact.merge_evidence_from(&callee_fact);
            }
        }
        changed
    });

    if std::env::var_os("SLATE_PTR_LATTICE_DEBUG").is_some() {
        for (binding, fact) in &facts {
            eprintln!(
                "{}::{} = {:?} -> {:?}",
                binding.function,
                binding.name,
                fact,
                fact.resolved()
            );
        }
    }

    facts
}

fn callee_param_name(fn_defs: &BTreeMap<String, &FnDef>, callee: &str, arg_index: usize) -> String {
    fn_defs
        .get(callee)
        .and_then(|f| f.params.get(arg_index))
        .map(|p| p.name.clone())
        .unwrap_or_default()
}

fn candidate_for(f: &FnDef) -> Option<Candidate> {
    if f.name == "main" || f.unsafe_ || f.abi.is_some() || !matches!(f.vis, Visibility::Private) {
        return None;
    }
    let param_indices: Vec<usize> = f
        .params
        .iter()
        .enumerate()
        .filter_map(|(index, param)| matches!(param.ty, Type::Ptr { .. }).then_some(index))
        .collect();
    (!param_indices.is_empty()).then_some(Candidate { param_indices })
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

fn elem_is_byte(ty: &Type) -> bool {
    let Type::Ptr { inner, .. } = ty else {
        return false;
    };
    match &**inner {
        Type::Prim(Prim::I8 | Prim::U8) => true,
        Type::Custom(name) => matches!(
            name.as_str(),
            "libc::c_char" | "std::ffi::c_char" | "core::ffi::c_char"
        ),
        _ => false,
    }
}

fn collect_return_aliases(
    fn_defs: &BTreeMap<String, &FnDef>,
    candidates: &BTreeMap<String, Candidate>,
) -> BTreeMap<String, ReturnAlias> {
    let mut aliases = BTreeMap::new();
    loop {
        let mut changed = false;
        for (name, candidate) in candidates {
            if aliases.contains_key(name) {
                continue;
            }
            if let Some(alias) = return_alias_for(fn_defs[name], candidate, &aliases) {
                aliases.insert(name.clone(), alias);
                changed = true;
            }
        }
        if !changed {
            return aliases;
        }
    }
}

fn return_alias_for(
    f: &FnDef,
    candidate: &Candidate,
    return_aliases: &BTreeMap<String, ReturnAlias>,
) -> Option<ReturnAlias> {
    let mut assignments: BTreeMap<String, Vec<Expr>> = BTreeMap::new();
    collect_assignments(&f.body, &mut assignments);
    let mut returns = Vec::new();
    collect_return_exprs(&f.body, &mut returns);
    let aliases: Vec<ReturnAlias> = candidate
        .param_indices
        .iter()
        .filter_map(|&param_index| {
            let param = &f.params[param_index];
            let mut tracked = BTreeSet::from([param.name.clone()]);
            loop {
                let before = tracked.len();
                for (name, values) in &assignments {
                    let has_alias = values.iter().any(|value| {
                        return_source_var(value, return_aliases)
                            .is_some_and(|source| tracked.contains(source.as_str()))
                    });
                    let compatible = values.iter().all(|value| {
                        is_null_like(value)
                            || return_source_var(value, return_aliases)
                                .is_some_and(|source| tracked.contains(source.as_str()))
                    });
                    if has_alias && compatible {
                        tracked.insert(name.clone());
                    }
                }
                if tracked.len() == before {
                    break;
                }
            }
            (!returns.is_empty()
                && returns.iter().all(|expr| {
                    return_source_var(expr, return_aliases)
                        .is_some_and(|name| tracked.contains(name.as_str()))
                }))
            .then_some(ReturnAlias {
                param_index,
                elem_is_byte: elem_is_byte(&param.ty),
            })
        })
        .collect();
    let [alias] = aliases.as_slice() else {
        return None;
    };
    Some(*alias)
}

fn return_source_var(expr: &Expr, return_aliases: &BTreeMap<String, ReturnAlias>) -> Option<Ident> {
    if let Some(source) = source_var(expr) {
        return Some(source);
    }
    let Expr::Call { func, args, .. } = peel_cast(peel_unsafe_tail(expr)) else {
        return None;
    };
    let Expr::Var(callee) = &**func else {
        return None;
    };
    let alias = return_aliases.get(callee.as_str())?;
    return_source_var(args.get(alias.param_index)?, return_aliases)
}

fn collect_assignments(body: &[IndentStmt], out: &mut BTreeMap<String, Vec<Expr>>) {
    for indent in body {
        match &indent.stmt {
            Stmt::Let {
                name,
                init: Some(init),
                ..
            } => out.entry(name.clone()).or_default().push(init.clone()),
            Stmt::Assign {
                target: Expr::Var(name),
                value,
            } => out
                .entry(name.as_str().to_string())
                .or_default()
                .push(value.clone()),
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
                collect_assignments(then_body, out);
                collect_assignments(else_body, out);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => collect_assignments(body, out),
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    collect_assignments(&arm.body, out);
                }
            }
            Stmt::While { body, .. } | Stmt::Unsafe { body } | Stmt::Block(body) => {
                collect_assignments(&body.stmts, out)
            }
            _ => {}
        }
    }
}

fn collect_return_exprs(body: &[IndentStmt], out: &mut Vec<Expr>) {
    for indent in body {
        match &indent.stmt {
            Stmt::Return(Some(expr)) => out.push(expr.clone()),
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
                collect_return_exprs(then_body, out);
                collect_return_exprs(else_body, out);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => collect_return_exprs(body, out),
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    collect_return_exprs(&arm.body, out);
                }
            }
            Stmt::While { body, .. } | Stmt::Unsafe { body } | Stmt::Block(body) => {
                collect_return_exprs(&body.stmts, out)
            }
            _ => {}
        }
    }
}

fn classify_function(
    name: &str,
    f: &FnDef,
    candidate: Option<&Candidate>,
    candidates: &BTreeMap<String, Candidate>,
    return_aliases: &BTreeMap<String, ReturnAlias>,
) -> (
    BTreeMap<PointerBinding, PointerFact>,
    Vec<CallSite>,
    Vec<ReturnCallSite>,
) {
    let mut facts = BTreeMap::new();
    let mut tracked: BTreeMap<String, String> = BTreeMap::new();
    if let Some(candidate) = candidate {
        for &index in &candidate.param_indices {
            let param = &f.params[index];
            let binding = PointerBinding {
                function: name.to_string(),
                name: param.name.clone(),
            };
            facts.insert(binding, PointerFact::new(elem_is_byte(&param.ty)));
            tracked.insert(param.name.clone(), param.name.clone());
        }
    }

    let mut var_uses = Vec::new();
    for indent in &f.body {
        indent.stmt.collect_vars(&mut var_uses);
    }
    let mut use_counts: BTreeMap<String, usize> = BTreeMap::new();
    for ident in &var_uses {
        *use_counts.entry(ident.as_str().to_string()).or_default() += 1;
    }

    let mut sites = Vec::new();
    let mut return_sites = Vec::new();
    let mut ctx = ClassifyCtx {
        function: name,
        candidates,
        return_aliases,
        has_return_alias: return_aliases.contains_key(name),
        facts: &mut facts,
        tracked: &mut tracked,
        sites: &mut sites,
        return_sites: &mut return_sites,
        use_counts: &use_counts,
    };
    ctx.body(&f.body);
    (facts, sites, return_sites)
}

struct ClassifyCtx<'a> {
    function: &'a str,
    candidates: &'a BTreeMap<String, Candidate>,
    return_aliases: &'a BTreeMap<String, ReturnAlias>,
    has_return_alias: bool,
    facts: &'a mut BTreeMap<PointerBinding, PointerFact>,
    tracked: &'a mut BTreeMap<String, String>,
    sites: &'a mut Vec<CallSite>,
    return_sites: &'a mut Vec<ReturnCallSite>,
    use_counts: &'a BTreeMap<String, usize>,
}

impl ClassifyCtx<'_> {
    fn observe(&mut self, canonical: &str, f: impl FnOnce(&mut PointerFact) -> bool) {
        let binding = PointerBinding {
            function: self.function.to_string(),
            name: canonical.to_string(),
        };
        if let Some(fact) = self.facts.get_mut(&binding) {
            f(fact);
        }
    }

    fn canonical_of(&self, name: &str) -> Option<String> {
        self.tracked.get(name).cloned()
    }

    fn try_alias(&mut self, name: &str, source_expr: &Expr) -> bool {
        let Some(source) = place_key(source_expr) else {
            return false;
        };
        let Some(canonical) = self.canonical_of(source.as_str()) else {
            return false;
        };
        self.tracked.insert(name.to_string(), canonical.clone());
        let is_temp = is_temp_name(name);
        let is_dead_rename = self.use_counts.get(source.as_str()).copied().unwrap_or(0) <= 1;
        if !is_temp && !is_dead_rename {
            self.observe(&canonical, PointerFact::disprove_unique);
        }
        true
    }

    fn try_offset_alias(&mut self, name: &str, source_expr: &Expr) -> bool {
        let Expr::MethodCall { recv, method, args } = peel_unsafe_tail(source_expr) else {
            return false;
        };
        if !matches!(
            method.as_str(),
            "add" | "offset" | "wrapping_add" | "sub" | "wrapping_sub"
        ) {
            return false;
        }
        let Expr::Var(recv_name) = &**recv else {
            return false;
        };
        let Some(canonical) = self.canonical_of(recv_name.as_str()) else {
            return false;
        };
        self.tracked.insert(name.to_string(), canonical.clone());
        self.observe(&canonical, PointerFact::observe_offset);
        for arg in args {
            self.expr(arg);
        }
        true
    }

    fn try_return_call_alias(&mut self, name: &str, source_expr: &Expr) -> bool {
        if self.canonical_of(name).is_some() {
            return false;
        }
        let Expr::Call {
            func,
            args,
            binding,
        } = peel_cast(peel_unsafe_tail(source_expr))
        else {
            return false;
        };
        let Expr::Var(callee) = &**func else {
            return false;
        };
        let Some(alias) = self.return_aliases.get(callee.as_str()).copied() else {
            return false;
        };
        self.call(func, args, binding);
        let binding = PointerBinding {
            function: self.function.to_string(),
            name: name.to_string(),
        };
        self.facts
            .entry(binding)
            .or_insert_with(|| PointerFact::new(alias.elem_is_byte));
        self.tracked.insert(name.to_string(), name.to_string());
        self.return_sites.push(ReturnCallSite {
            caller: self.function.to_string(),
            callee: callee.as_str().to_string(),
            param_index: alias.param_index,
            result_binding: name.to_string(),
        });
        true
    }

    fn observe_write_target(&mut self, expr: &Expr) {
        if let Some(name) = source_var(expr)
            && let Some(canonical) = self.canonical_of(name.as_str())
        {
            self.observe(&canonical, PointerFact::observe_write);
        } else {
            self.expr(expr);
        }
    }

    fn body(&mut self, body: &[crate::backend::rust_ast::IndentStmt]) {
        for indent in body {
            self.stmt(&indent.stmt);
        }
    }

    fn block(&mut self, block: &Block) {
        self.body(&block.stmts);
        if let Some(tail) = &block.tail {
            self.expr(tail);
        }
    }

    fn stmt(&mut self, stmt: &Stmt) {
        match stmt {
            Stmt::Let {
                name,
                init: Some(init),
                ..
            } => {
                if !self.try_alias(name, init)
                    && !self.try_offset_alias(name, init)
                    && !self.try_return_call_alias(name, init)
                {
                    self.expr(init);
                }
            }
            Stmt::Let { init: None, .. } => {}
            Stmt::LetIf {
                cond,
                then_body,
                then_value,
                else_body,
                else_value,
                ..
            } => {
                self.expr(cond);
                self.body(then_body);
                self.expr(then_value);
                self.body(else_body);
                self.expr(else_value);
            }
            Stmt::Assign { target, value } => {
                if let Expr::Var(name) = target {
                    if !self.try_alias(name.as_str(), value)
                        && !self.try_offset_alias(name.as_str(), value)
                        && !self.try_return_call_alias(name.as_str(), value)
                    {
                        self.expr(value);
                    }
                } else if let Some(key) = frame_field_key(target) {
                    match source_var(value).and_then(|s| self.canonical_of(s.as_str())) {
                        Some(canonical) => {
                            self.tracked.insert(key, canonical);
                        }
                        None => {
                            self.tracked.remove(&key);
                            self.expr(value);
                        }
                    }
                } else {
                    self.place(target, true);
                    self.expr(value);
                }
            }
            Stmt::CompoundAssign { target, value, .. } => {
                self.place(target, true);
                self.expr(value);
            }
            Stmt::InlineAsm(_) => {}
            Stmt::Expr(expr) => self.expr(expr),
            Stmt::Return(Some(expr)) => {
                if let Expr::Var(name) = expr
                    && let Some(canonical) = self.canonical_of(name.as_str())
                {
                    if !self.has_return_alias {
                        self.observe(&canonical, PointerFact::observe_escape);
                    }
                } else {
                    self.expr(expr);
                }
            }
            Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
            Stmt::Unsafe { body } => self.block(body),
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                self.expr(cond);
                self.body(then_body);
                self.body(else_body);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                self.body(body)
            }
            Stmt::For { iter, body, .. } => {
                self.expr(iter);
                self.body(body);
            }
            Stmt::Match { expr, arms } => {
                self.expr(expr);
                for arm in arms {
                    self.body(&arm.body);
                }
            }
            Stmt::While { cond, body } => {
                self.expr(cond);
                self.block(body);
            }
            Stmt::Block(body) => self.block(body),
        }
    }

    fn place(&mut self, expr: &Expr, is_write: bool) {
        match expr {
            Expr::Var(_) => {}
            Expr::Unary {
                op: crate::backend::rust_ast::UnaryOp::Deref,
                expr,
            } => {
                if let Some(key) = place_key(expr)
                    && let Some(canonical) = self.canonical_of(key.as_str())
                {
                    if is_write {
                        self.observe(&canonical, PointerFact::observe_write);
                    }
                } else {
                    self.expr(expr);
                }
            }
            Expr::Index { base, index } => {
                if let Expr::Var(name) = &**base
                    && let Some(canonical) = self.canonical_of(name.as_str())
                {
                    self.observe(&canonical, PointerFact::observe_offset);
                    if is_write {
                        self.observe(&canonical, PointerFact::observe_write);
                    }
                } else {
                    self.expr(base);
                }
                self.expr(index);
            }
            Expr::Cast { expr, .. } => self.place(expr, is_write),
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => self.place(base, is_write),
            other => self.expr(other),
        }
    }

    fn expr(&mut self, expr: &Expr) {
        match expr {
            Expr::Var(_) => {}
            Expr::Unary { expr, .. } => self.expr(expr),
            Expr::Ref { expr, .. } | Expr::AddrOf { expr, .. } => {
                if let Expr::Var(name) = &**expr
                    && let Some(canonical) = self.canonical_of(name.as_str())
                {
                    self.observe(&canonical, PointerFact::observe_escape);
                } else {
                    self.expr(expr);
                }
            }
            Expr::MethodCall { recv, method, args } => {
                if let Expr::Var(name) = &**recv
                    && let Some(canonical) = self.canonical_of(name.as_str())
                    && matches!(
                        method.as_str(),
                        "add" | "offset" | "wrapping_add" | "sub" | "wrapping_sub" | "offset_from"
                    )
                {
                    self.observe(&canonical, PointerFact::observe_offset);
                } else {
                    self.expr(recv);
                }
                for arg in args {
                    self.expr(arg);
                }
            }
            Expr::Binary { op, lhs, rhs } => {
                if matches!(op, BinOp::Eq | BinOp::Ne) {
                    self.observe_null_compare(lhs, rhs);
                    self.observe_null_compare(rhs, lhs);
                }
                self.expr(lhs);
                self.expr(rhs);
            }
            Expr::Call {
                func,
                args,
                binding,
            } => self.call(func, args, binding),
            Expr::Index { base, index } => {
                if let Expr::Var(name) = &**base
                    && let Some(canonical) = self.canonical_of(name.as_str())
                {
                    self.observe(&canonical, PointerFact::observe_offset);
                } else {
                    self.expr(base);
                }
                self.expr(index);
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => self.expr(base),
            Expr::ArrayPtr { array, .. } => self.expr(array),
            Expr::Cast { expr, .. } | Expr::Transmute { expr, .. } => self.expr(expr),
            Expr::Range { start, end } => {
                self.expr(start);
                self.expr(end);
            }
            Expr::StructLit { fields, .. } => {
                for (_, value) in fields {
                    self.expr(value);
                }
            }
            Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) | Expr::VecLit(fields) => {
                for elem in fields {
                    self.expr(elem);
                }
            }
            Expr::ArrayRepeat { elem, .. } => self.expr(elem),
            Expr::VecRepeat { elem, len } => {
                self.expr(elem);
                self.expr(len);
            }
            Expr::Closure { body, .. } => self.expr(body),
            Expr::MethodCallGeneric { recv, args, .. } => {
                self.expr(recv);
                for arg in args {
                    self.expr(arg);
                }
            }
            Expr::Macro { args, .. } => {
                for arg in args {
                    self.expr(arg);
                }
            }
            Expr::Match { expr, arms } => {
                self.expr(expr);
                for arm in arms {
                    self.expr(&arm.value);
                }
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                self.expr(cond);
                self.expr(then_expr);
                self.expr(else_expr);
            }
            Expr::Block(block) | Expr::Unsafe(block) => self.block(block),
            Expr::CopyNonoverlapping { src, dst, .. } => {
                self.expr(src);
                self.observe_write_target(dst);
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                self.expr(src);
                self.observe_write_target(dst);
                self.expr(count);
            }
            Expr::WriteBytes { dst, val, count } => {
                self.observe_write_target(dst);
                self.expr(val);
                self.expr(count);
            }
            Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::CStr(_)
            | Expr::AtomicFence { .. }
            | Expr::AtomicRef { .. }
            | Expr::AtomicLoad { .. }
            | Expr::AtomicStore { .. }
            | Expr::AtomicFetch { .. }
            | Expr::AtomicSwap { .. }
            | Expr::AtomicCompareExchange { .. }
            | Expr::AtomicNew { .. }
            | Expr::Todo(_)
            | Expr::Path(_) => {}
        }
    }

    fn observe_null_compare(&mut self, side: &Expr, other: &Expr) {
        if let Expr::Var(name) = side
            && let Some(canonical) = self.canonical_of(name.as_str())
            && is_null_like(other)
        {
            self.observe(&canonical, PointerFact::observe_nullable);
        }
    }

    fn call(&mut self, func: &Expr, args: &[Expr], binding: &CallBinding) {
        self.expr(func);
        let callee_name = match func {
            Expr::Var(name) => Some(name.as_str().to_string()),
            _ => None,
        };
        let known = match binding {
            CallBinding::Direct {
                identity: FunctionIdentity::Known(known),
                ..
            } => Some(*known),
            _ => None,
        };
        for (index, arg) in args.iter().enumerate() {
            let peeled = peel_cast(arg);
            let Expr::Var(name) = peeled else {
                self.expr(arg);
                continue;
            };
            let Some(canonical) = self.canonical_of(name.as_str()) else {
                continue;
            };
            self.classify_call_arg(&canonical, &callee_name, known, index);
        }
    }

    fn classify_call_arg(
        &mut self,
        canonical: &str,
        callee_name: &Option<String>,
        known: Option<Known>,
        index: usize,
    ) {
        if let Some(known) = known {
            match known {
                Known::Free => self.observe(canonical, PointerFact::observe_free),
                Known::MemCpy | Known::MemMove if index == 0 => {
                    self.observe(canonical, PointerFact::observe_write)
                }
                Known::MemCpy | Known::MemMove => {}
                Known::MemSet if index == 0 => self.observe(canonical, PointerFact::observe_write),
                Known::MemSet => {}
                Known::StrCpy | Known::StrCat | Known::StrNCpy | Known::StrNCat => {
                    self.observe(canonical, PointerFact::observe_string);
                    if index == 0 {
                        self.observe(canonical, PointerFact::observe_write);
                    }
                }
                Known::StrLen
                | Known::StrCmp
                | Known::StrNCmp
                | Known::StrChr
                | Known::StrRChr
                | Known::StrStr
                | Known::StrPBrk
                | Known::StrSpn
                | Known::StrCSpn => {
                    self.observe(canonical, PointerFact::observe_string);
                }
                Known::MemChr | Known::MemCmp => {}
                _ => self.observe(canonical, PointerFact::observe_escape),
            }
            return;
        }
        let Some(callee_name) = callee_name else {
            self.observe(canonical, PointerFact::observe_escape);
            return;
        };
        if self.candidates.contains_key(callee_name) {
            self.sites.push(CallSite {
                caller: self.function.to_string(),
                callee: callee_name.clone(),
                arg_index: index,
                caller_binding: canonical.to_string(),
            });
        } else {
            self.observe(canonical, PointerFact::observe_escape);
        }
    }
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|byte| byte.is_ascii_digit()))
}

fn peel_cast(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_cast(expr),
        other => other,
    }
}

fn peel_unsafe_tail(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => match &block.tail {
            Some(tail) => peel_unsafe_tail(tail),
            None => expr,
        },
        other => other,
    }
}

fn is_null_like(expr: &Expr) -> bool {
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
        )
    )
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum LiftKind {
    Scalar,
    Cell,
    Owned,
    #[expect(
        dead_code,
        reason = "lift_kind() excludes Buffer until call-site slice-length bridging lands; see slate-y0qs.4.8 follow-up"
    )]
    Buffer,
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct LiftPlan {
    ty: Type,
    kind: LiftKind,
    mutable: bool,
}

fn lift_kind(base: ResolvedPtrType) -> Option<LiftKind> {
    match base {
        ResolvedPtrType::Ref | ResolvedPtrType::RefMut => Some(LiftKind::Scalar),
        ResolvedPtrType::Owned => Some(LiftKind::Owned),
        ResolvedPtrType::RefCell
        | ResolvedPtrType::Slice
        | ResolvedPtrType::SliceMut
        | ResolvedPtrType::Str
        | ResolvedPtrType::Vec
        | ResolvedPtrType::StringOwned
        | ResolvedPtrType::RawConst
        | ResolvedPtrType::RawMut => None,
    }
}

fn type_for(base: ResolvedPtrType, inner: &Type) -> Type {
    match base {
        ResolvedPtrType::Ref => Type::Ref {
            mutable: false,
            inner: Box::new(inner.clone()),
        },
        ResolvedPtrType::RefMut => Type::Ref {
            mutable: true,
            inner: Box::new(inner.clone()),
        },
        ResolvedPtrType::RefCell => Type::Generic {
            name: "std::cell::Cell".to_string(),
            args: vec![inner.clone()],
        },
        ResolvedPtrType::Slice => Type::Ref {
            mutable: false,
            inner: Box::new(Type::Slice(Box::new(inner.clone()))),
        },
        ResolvedPtrType::SliceMut => Type::Ref {
            mutable: true,
            inner: Box::new(Type::Slice(Box::new(inner.clone()))),
        },
        ResolvedPtrType::Owned => Type::Generic {
            name: "Box".to_string(),
            args: vec![inner.clone()],
        },
        ResolvedPtrType::Str
        | ResolvedPtrType::Vec
        | ResolvedPtrType::StringOwned
        | ResolvedPtrType::RawConst
        | ResolvedPtrType::RawMut => unreachable!("filtered by lift_kind"),
    }
}

pub(in crate::backend) fn apply(program: &mut Program) {
    let facts = solve(program);

    crate::backend::interproc::length_lattice::apply(program, &facts);

    let mut plans: BTreeMap<String, BTreeMap<String, LiftPlan>> = BTreeMap::new();
    let mut param_names: BTreeMap<String, Vec<String>> = BTreeMap::new();
    {
        let fn_defs = collect_fn_defs(&program.items);
        for (name, f) in &fn_defs {
            param_names.insert(
                name.clone(),
                f.params.iter().map(|p| p.name.clone()).collect(),
            );
        }
        for (binding, fact) in &facts {
            if fact.nullable {
                continue;
            }
            let repr = fact.resolved();
            let Some(kind) = lift_kind(repr.base) else {
                continue;
            };
            let Some(f) = fn_defs.get(&binding.function) else {
                continue;
            };
            let Some(param) = f.params.iter().find(|p| p.name == binding.name) else {
                continue;
            };
            let Type::Ptr { inner, .. } = &param.ty else {
                continue;
            };
            let ty = type_for(repr.base, inner);
            plans.entry(binding.function.clone()).or_default().insert(
                binding.name.clone(),
                LiftPlan {
                    ty,
                    kind,
                    mutable: fact.write,
                },
            );
        }
    }
    if plans.is_empty() {
        return;
    }

    validate_plans(&program.items, &mut plans, &param_names);
    if plans.is_empty() {
        return;
    }

    apply_plans(&mut program.items, &plans, &param_names);
}

fn for_each_fn_body<'a>(items: &'a [Item], f: &mut impl FnMut(&'a str, &'a [IndentStmt])) {
    for item in items {
        match item {
            Item::Fn(func) => f(func.name.as_str(), &func.body),
            Item::InlineMod { items, .. } => for_each_fn_body(items, f),
            _ => {}
        }
    }
}

fn is_heap_alloc_call(expr: &Expr) -> bool {
    matches!(
        peel_cast(expr),
        Expr::Call {
            binding: CallBinding::Direct {
                identity: FunctionIdentity::Known(Known::Malloc | Known::Calloc | Known::Realloc),
                ..
            },
            ..
        }
    )
}

fn collect_heap_locals(body: &[IndentStmt], out: &mut std::collections::BTreeSet<String>) {
    for indent in body {
        collect_heap_locals_stmt(&indent.stmt, out);
    }
}

fn collect_heap_locals_stmt(stmt: &Stmt, out: &mut std::collections::BTreeSet<String>) {
    match stmt {
        Stmt::Let {
            name,
            init: Some(init),
            ..
        } if is_heap_alloc_call(init) => {
            out.insert(name.clone());
        }
        Stmt::Assign {
            target: Expr::Var(name),
            value,
        } if is_heap_alloc_call(value) => {
            out.insert(name.as_str().to_string());
        }
        _ => {}
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
            collect_heap_locals(then_body, out);
            collect_heap_locals(else_body, out);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            collect_heap_locals(body, out);
        }
        Stmt::For { body, .. } => collect_heap_locals(body, out),
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_heap_locals(&arm.body, out);
            }
        }
        Stmt::While { body, .. } => collect_heap_locals(&body.stmts, out),
        Stmt::Unsafe { body } => collect_heap_locals(&body.stmts, out),
        Stmt::Block(block) => collect_heap_locals(&block.stmts, out),
        _ => {}
    }
}

fn arg_is_provably_bridgeable(
    arg: &Expr,
    bridgeable_aliases: &BTreeSet<String>,
    heap_locals: &std::collections::BTreeSet<String>,
    plan: &LiftPlan,
) -> bool {
    if let Expr::Var(name) = arg
        && bridgeable_aliases.contains(name.as_str())
    {
        return true;
    }
    if plan.kind == LiftKind::Owned {
        return matches!(peel_cast(arg), Expr::Var(name) if heap_locals.contains(name.as_str()));
    }
    matches!(peel_cast(arg), Expr::AddrOf { .. } | Expr::Ref { .. })
}

fn collect_bridgeable_aliases(
    body: &[IndentStmt],
    own: Option<&BTreeMap<String, LiftPlan>>,
) -> BTreeSet<String> {
    let mut aliases: BTreeSet<String> = own
        .into_iter()
        .flat_map(|plans| plans.keys().cloned())
        .collect();
    let mut assignments = BTreeMap::new();
    collect_assignments(body, &mut assignments);
    loop {
        let before = aliases.len();
        for (name, values) in &assignments {
            let has_alias = values.iter().any(|value| {
                source_var(value).is_some_and(|source| aliases.contains(source.as_str()))
            });
            let compatible = values.iter().all(|value| {
                is_null_like(value)
                    || source_var(value).is_some_and(|source| aliases.contains(source.as_str()))
            });
            if has_alias && compatible {
                aliases.insert(name.clone());
            }
        }
        if aliases.len() == before {
            return aliases;
        }
    }
}

fn validate_plans(
    items: &[Item],
    plans: &mut BTreeMap<String, BTreeMap<String, LiftPlan>>,
    param_names: &BTreeMap<String, Vec<String>>,
) {
    let mut heap_locals_by_fn: BTreeMap<String, std::collections::BTreeSet<String>> =
        BTreeMap::new();
    for_each_fn_body(items, &mut |caller_name, body| {
        let mut locals = std::collections::BTreeSet::new();
        collect_heap_locals(body, &mut locals);
        heap_locals_by_fn.insert(caller_name.to_string(), locals);
    });

    loop {
        let mut invalid: Vec<(String, String)> = Vec::new();
        let empty_locals = std::collections::BTreeSet::new();
        for_each_fn_body(items, &mut |caller_name, body| {
            let own = plans.get(caller_name);
            let bridgeable_aliases = collect_bridgeable_aliases(body, own);
            let heap_locals = heap_locals_by_fn.get(caller_name).unwrap_or(&empty_locals);
            let mut calls = Vec::new();
            for indent in body {
                indent.stmt.collect_calls(&mut calls);
            }
            for (callee, args) in calls {
                let callee = callee.as_str();
                let Some(pnames) = param_names.get(callee) else {
                    continue;
                };
                let Some(callee_plans) = plans.get(callee) else {
                    continue;
                };
                for (index, arg) in args.iter().enumerate() {
                    let Some(pname) = pnames.get(index) else {
                        continue;
                    };
                    let Some(plan) = callee_plans.get(pname) else {
                        continue;
                    };
                    if !arg_is_provably_bridgeable(arg, &bridgeable_aliases, heap_locals, plan) {
                        invalid.push((callee.to_string(), pname.clone()));
                    }
                }
            }
        });
        if invalid.is_empty() {
            break;
        }
        for (fname, pname) in invalid {
            if let Some(m) = plans.get_mut(&fname) {
                m.remove(&pname);
                if m.is_empty() {
                    plans.remove(&fname);
                }
            }
        }
    }
}

fn apply_plans(
    items: &mut [Item],
    plans: &BTreeMap<String, BTreeMap<String, LiftPlan>>,
    param_names: &BTreeMap<String, Vec<String>>,
) {
    let empty = BTreeMap::new();
    for item in items {
        match item {
            Item::Fn(f) => {
                let own_plans = plans.get(&f.name).unwrap_or(&empty);
                for param in &mut f.params {
                    if let Some(plan) = own_plans.get(&param.name) {
                        param.ty = plan.ty.clone();
                        if plan.kind == LiftKind::Owned {
                            param.mutable = true;
                        }
                    }
                }
                let mut declared_mutability = BTreeMap::new();
                collect_declared_ptr_mutability(&f.body, &mut declared_mutability);
                let owned_aliases = collect_owned_aliases(&f.body, own_plans);
                let ctx = LiftCtx {
                    own: own_plans,
                    all: plans,
                    param_names,
                    declared_mutability: &declared_mutability,
                    owned_aliases: &owned_aliases,
                };
                rewrite_stmts(&mut f.body, &ctx);
            }
            Item::InlineMod { items, .. } => apply_plans(items, plans, param_names),
            _ => {}
        }
    }
}

fn accessor_method(mutable: bool) -> &'static str {
    if mutable { "as_mut_ptr" } else { "as_ptr" }
}

fn to_raw_pointer(name: &str, plan: &LiftPlan) -> Expr {
    to_raw_pointer_as(name, plan, plan.mutable)
}

fn to_raw_pointer_as(name: &str, plan: &LiftPlan, target_mutable: bool) -> Expr {
    let recv = Box::new(Expr::Var(Ident::new(name)));
    match plan.kind {
        LiftKind::Buffer => Expr::MethodCall {
            recv,
            method: accessor_method(target_mutable).to_string(),
            args: Vec::new(),
        },
        LiftKind::Scalar => {
            let inner = match &plan.ty {
                Type::Ref { inner, .. } => (**inner).clone(),
                other => other.clone(),
            };
            if target_mutable && !plan.mutable {
                let as_const = Expr::Cast {
                    expr: recv,
                    ty: Type::Ptr {
                        mutable: false,
                        inner: Box::new(inner.clone()),
                    },
                };
                Expr::Cast {
                    expr: Box::new(as_const),
                    ty: Type::Ptr {
                        mutable: true,
                        inner: Box::new(inner),
                    },
                }
            } else {
                Expr::Cast {
                    expr: recv,
                    ty: Type::Ptr {
                        mutable: target_mutable,
                        inner: Box::new(inner),
                    },
                }
            }
        }
        LiftKind::Cell => Expr::MethodCall {
            recv,
            method: "as_ptr".to_string(),
            args: Vec::new(),
        },
        LiftKind::Owned => {
            let inner = match &plan.ty {
                Type::Generic { args, .. } => args.first().cloned().unwrap_or(Type::Unit),
                other => other.clone(),
            };
            let borrow = Expr::Ref {
                mutable: target_mutable,
                expr: Box::new(Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: recv,
                }),
            };
            Expr::Cast {
                expr: Box::new(borrow),
                ty: Type::Ptr {
                    mutable: target_mutable,
                    inner: Box::new(inner),
                },
            }
        }
    }
}

fn bridge_arg_into_ref(arg: &mut Expr, plan: &LiftPlan) {
    let taken = std::mem::replace(arg, Expr::Value(RustValue::None));
    *arg = Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Ref {
            mutable: plan.mutable,
            expr: Box::new(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(taken),
            }),
        })),
    }));
}

fn bridge_arg_into_box(arg: &mut Expr) {
    let taken = std::mem::replace(arg, Expr::Value(RustValue::None));
    *arg = Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(["Box", "from_raw"].map(Ident::from)))),
            args: vec![taken],
        })),
    }));
}

fn callee_accepts_directly(
    callee: &str,
    arg_index: usize,
    own_plan: &LiftPlan,
    all_plans: &BTreeMap<String, BTreeMap<String, LiftPlan>>,
    param_names: &BTreeMap<String, Vec<String>>,
) -> bool {
    let Some(names) = param_names.get(callee) else {
        return false;
    };
    let Some(param_name) = names.get(arg_index) else {
        return false;
    };
    all_plans
        .get(callee)
        .and_then(|plans| plans.get(param_name))
        .is_some_and(|callee_plan| callee_plan.ty == own_plan.ty)
}

struct LiftCtx<'a> {
    own: &'a BTreeMap<String, LiftPlan>,
    all: &'a BTreeMap<String, BTreeMap<String, LiftPlan>>,
    param_names: &'a BTreeMap<String, Vec<String>>,
    declared_mutability: &'a BTreeMap<String, bool>,
    owned_aliases: &'a BTreeMap<String, String>,
}

fn resolve_alias_root(
    name: &str,
    own: &BTreeMap<String, LiftPlan>,
    aliases: &BTreeMap<String, String>,
) -> Option<String> {
    if own.contains_key(name) {
        return Some(name.to_string());
    }
    aliases.get(name).cloned()
}

fn collect_owned_aliases(
    body: &[IndentStmt],
    own: &BTreeMap<String, LiftPlan>,
) -> BTreeMap<String, String> {
    let mut aliases = BTreeMap::new();
    collect_owned_aliases_stmts(body, own, &mut aliases);
    aliases
}

fn record_owned_alias(
    name: &str,
    source_expr: &Expr,
    own: &BTreeMap<String, LiftPlan>,
    aliases: &mut BTreeMap<String, String>,
) {
    let Some(source) = source_var(source_expr) else {
        return;
    };
    let Some(root) = resolve_alias_root(source.as_str(), own, aliases) else {
        return;
    };
    if own
        .get(root.as_str())
        .is_some_and(|plan| plan.kind == LiftKind::Owned)
    {
        aliases.insert(name.to_string(), root);
    }
}

fn collect_owned_aliases_stmts(
    body: &[IndentStmt],
    own: &BTreeMap<String, LiftPlan>,
    aliases: &mut BTreeMap<String, String>,
) {
    for indent in body {
        collect_owned_aliases_stmt(&indent.stmt, own, aliases);
    }
}

fn collect_owned_aliases_stmt(
    stmt: &Stmt,
    own: &BTreeMap<String, LiftPlan>,
    aliases: &mut BTreeMap<String, String>,
) {
    match stmt {
        Stmt::Let {
            name,
            init: Some(init),
            ..
        } => record_owned_alias(name, init, own, aliases),
        Stmt::Assign {
            target: Expr::Var(name),
            value,
        } => record_owned_alias(name.as_str(), value, own, aliases),
        _ => {}
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
            collect_owned_aliases_stmts(then_body, own, aliases);
            collect_owned_aliases_stmts(else_body, own, aliases);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            collect_owned_aliases_stmts(body, own, aliases);
        }
        Stmt::For { body, .. } => collect_owned_aliases_stmts(body, own, aliases),
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_owned_aliases_stmts(&arm.body, own, aliases);
            }
        }
        Stmt::While { body, .. } => collect_owned_aliases_stmts(&body.stmts, own, aliases),
        Stmt::Unsafe { body } => collect_owned_aliases_stmts(&body.stmts, own, aliases),
        Stmt::Block(block) => collect_owned_aliases_stmts(&block.stmts, own, aliases),
        _ => {}
    }
}

fn free_call_arg(expr: &Expr) -> Option<&Expr> {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            block.tail.as_deref().and_then(free_call_arg)
        }
        Expr::Call { func, args, .. } if args.len() == 1 => {
            matches!(&**func, Expr::Var(name) if name.as_str() == "free").then(|| &args[0])
        }
        _ => None,
    }
}

fn is_owned_free_stmt(stmt: &Stmt, ctx: &LiftCtx) -> bool {
    let Stmt::Expr(expr) = stmt else {
        return false;
    };
    let Some(arg) = free_call_arg(expr) else {
        return false;
    };
    let Some(source) = source_var(arg) else {
        return false;
    };
    let Some(root) = resolve_alias_root(source.as_str(), ctx.own, ctx.owned_aliases) else {
        return false;
    };
    ctx.own
        .get(root.as_str())
        .is_some_and(|plan| plan.kind == LiftKind::Owned)
}

fn collect_declared_ptr_mutability(body: &[IndentStmt], out: &mut BTreeMap<String, bool>) {
    for indent in body {
        collect_declared_ptr_mutability_stmt(&indent.stmt, out);
    }
}

fn collect_declared_ptr_mutability_stmt(stmt: &Stmt, out: &mut BTreeMap<String, bool>) {
    if let Stmt::Let {
        name,
        ty: Some(Type::Ptr { mutable, .. }),
        ..
    } = stmt
    {
        out.insert(name.clone(), *mutable);
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
            collect_declared_ptr_mutability(then_body, out);
            collect_declared_ptr_mutability(else_body, out);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            collect_declared_ptr_mutability(body, out);
        }
        Stmt::For { body, .. } => collect_declared_ptr_mutability(body, out),
        Stmt::Match { arms, .. } => {
            for arm in arms {
                collect_declared_ptr_mutability(&arm.body, out);
            }
        }
        Stmt::While { body, .. } => collect_declared_ptr_mutability(&body.stmts, out),
        Stmt::Unsafe { body } => collect_declared_ptr_mutability(&body.stmts, out),
        Stmt::Block(block) => collect_declared_ptr_mutability(&block.stmts, out),
        _ => {}
    }
}

fn rewrite_stmts(body: &mut Vec<IndentStmt>, ctx: &LiftCtx) {
    body.retain_mut(|indent| {
        if is_owned_free_stmt(&indent.stmt, ctx) {
            return false;
        }
        rewrite_stmt(&mut indent.stmt, ctx);
        true
    });
}

fn rewrite_block(block: &mut Block, ctx: &LiftCtx) {
    rewrite_stmts(&mut block.stmts, ctx);
    if let Some(tail) = &mut block.tail {
        rewrite_expr(tail, ctx);
    }
}

fn rewrite_stmt(stmt: &mut Stmt, ctx: &LiftCtx) {
    match stmt {
        Stmt::Assign { target, value } => {
            if let Some((name, LiftKind::Cell)) = deref_of_tracked(target, ctx.own) {
                rewrite_expr(value, ctx);
                let taken = std::mem::replace(value, Expr::Todo(String::new()));
                *stmt = Stmt::Expr(Expr::MethodCall {
                    recv: Box::new(Expr::Var(Ident::new(name))),
                    method: "set".to_string(),
                    args: vec![taken],
                });
                return;
            }
            if let Expr::Var(target_name) = &*target
                && let Expr::Var(source_name) = &*value
                && let Some(plan) = ctx.own.get(source_name.as_str())
                && matches!(plan.kind, LiftKind::Scalar | LiftKind::Owned)
                && let Some(&mutable) = ctx.declared_mutability.get(target_name.as_str())
            {
                *value = to_raw_pointer_as(source_name.as_str(), plan, mutable);
                return;
            }
            rewrite_expr(target, ctx);
            rewrite_expr(value, ctx);
        }
        Stmt::CompoundAssign { target, op, value } => {
            if let Some((name, LiftKind::Cell)) = deref_of_tracked(target, ctx.own) {
                rewrite_expr(value, ctx);
                let taken = std::mem::replace(value, Expr::Todo(String::new()));
                let get = Expr::MethodCall {
                    recv: Box::new(Expr::Var(Ident::new(&name))),
                    method: "get".to_string(),
                    args: Vec::new(),
                };
                let combined = Expr::Binary {
                    op: *op,
                    lhs: Box::new(get),
                    rhs: Box::new(taken),
                };
                *stmt = Stmt::Expr(Expr::MethodCall {
                    recv: Box::new(Expr::Var(Ident::new(name))),
                    method: "set".to_string(),
                    args: vec![combined],
                });
                return;
            }
            rewrite_expr(target, ctx);
            rewrite_expr(value, ctx);
        }
        Stmt::Let {
            ty,
            init: Some(init),
            ..
        } => {
            if let Expr::Var(name) = &*init
                && let Some(plan) = ctx.own.get(name.as_str())
                && matches!(plan.kind, LiftKind::Scalar | LiftKind::Owned)
                && let Some(Type::Ptr { mutable, .. }) = ty
            {
                *init = to_raw_pointer_as(name.as_str(), plan, *mutable);
                return;
            }
            rewrite_expr(init, ctx)
        }
        Stmt::Let { init: None, .. } => {}
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            rewrite_expr(cond, ctx);
            rewrite_stmts(then_body, ctx);
            rewrite_expr(then_value, ctx);
            rewrite_stmts(else_body, ctx);
            rewrite_expr(else_value, ctx);
        }
        Stmt::InlineAsm(asm) => {
            for operand in &mut asm.operands {
                operand.visit_exprs_mut(&mut |e| rewrite_expr(e, ctx));
            }
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => rewrite_expr(expr, ctx),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::For { iter, body, .. } => {
            rewrite_expr(iter, ctx);
            rewrite_stmts(body, ctx);
        }
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } | Stmt::Loop { body, .. } => {
            rewrite_stmts(body, ctx)
        }
        Stmt::Unsafe { body } => rewrite_block(body, ctx),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            rewrite_expr(cond, ctx);
            rewrite_stmts(then_body, ctx);
            rewrite_stmts(else_body, ctx);
        }
        Stmt::Match { expr, arms } => {
            rewrite_expr(expr, ctx);
            for arm in arms {
                rewrite_stmts(&mut arm.body, ctx);
            }
        }
        Stmt::While { cond, body } => {
            rewrite_expr(cond, ctx);
            rewrite_block(body, ctx);
        }
        Stmt::Block(body) => rewrite_block(body, ctx),
    }
}

fn deref_of_tracked(expr: &Expr, own: &BTreeMap<String, LiftPlan>) -> Option<(String, LiftKind)> {
    let Expr::Unary {
        op: UnaryOp::Deref,
        expr,
    } = expr
    else {
        return None;
    };
    let Expr::Var(name) = &**expr else {
        return None;
    };
    own.get(name.as_str())
        .map(|plan| (name.as_str().to_string(), plan.kind))
}

fn rewrite_expr(expr: &mut Expr, ctx: &LiftCtx) {
    if let Some((name, kind)) = deref_of_tracked(expr, ctx.own)
        && kind == LiftKind::Cell
    {
        *expr = Expr::MethodCall {
            recv: Box::new(Expr::Var(Ident::new(name))),
            method: "get".to_string(),
            args: Vec::new(),
        };
        return;
    }

    if let Expr::Index { base, index } = expr {
        rewrite_expr(index, ctx);
        if !matches!(&**base, Expr::Var(name) if ctx.own.contains_key(name.as_str())) {
            rewrite_expr(base, ctx);
        }
        return;
    }

    if let Expr::MethodCall { recv, method, args } = expr
        && let Expr::Var(name) = &**recv
        && matches!(
            method.as_str(),
            "add" | "offset" | "wrapping_add" | "sub" | "wrapping_sub" | "offset_from"
        )
        && let Some(plan) = ctx.own.get(name.as_str())
    {
        **recv = to_raw_pointer(name.as_str(), plan);
        for arg in args {
            rewrite_expr(arg, ctx);
        }
        return;
    }

    if let Expr::Call { func, args, .. } = expr {
        rewrite_expr(func, ctx);
        let callee = match &**func {
            Expr::Var(name) => Some(name.as_str().to_string()),
            _ => None,
        };
        for (index, arg) in args.iter_mut().enumerate() {
            if let Expr::Var(name) = arg
                && let Some(plan) = ctx.own.get(name.as_str())
            {
                let direct = callee.as_deref().is_some_and(|callee| {
                    callee_accepts_directly(callee, index, plan, ctx.all, ctx.param_names)
                });
                if !direct {
                    *arg = to_raw_pointer(name.as_str(), plan);
                }
                continue;
            }
            let callee_plan = callee.as_deref().and_then(|callee| {
                let pname = ctx.param_names.get(callee)?.get(index)?;
                ctx.all.get(callee)?.get(pname.as_str())
            });
            if let Some(plan) = callee_plan {
                match plan.kind {
                    LiftKind::Scalar => {
                        bridge_arg_into_ref(arg, plan);
                        continue;
                    }
                    LiftKind::Owned => {
                        bridge_arg_into_box(arg);
                        continue;
                    }
                    LiftKind::Cell | LiftKind::Buffer => {}
                }
            }
            rewrite_expr(arg, ctx);
        }
        return;
    }

    if let Expr::Var(name) = expr
        && let Some(plan) = ctx.own.get(name.as_str())
    {
        *expr = to_raw_pointer(name.as_str(), plan);
        return;
    }

    match expr {
        Expr::Var(_)
        | Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => {}
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => rewrite_expr(expr, ctx),
        Expr::Block(block) | Expr::Unsafe(block) => rewrite_block(block, ctx),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            rewrite_expr(src, ctx);
            rewrite_expr(dst, ctx);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            rewrite_expr(src, ctx);
            rewrite_expr(dst, ctx);
            rewrite_expr(count, ctx);
        }
        Expr::WriteBytes { dst, val, count } => {
            rewrite_expr(dst, ctx);
            rewrite_expr(val, ctx);
            rewrite_expr(count, ctx);
        }
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            if let Some(p) = place.ptr_expr_mut() {
                rewrite_expr(p, ctx);
            }
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            if let Some(p) = place.ptr_expr_mut() {
                rewrite_expr(p, ctx);
            }
            rewrite_expr(value, ctx);
        }
        Expr::AtomicNew { value, .. } => rewrite_expr(value, ctx),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            if let Some(p) = place.ptr_expr_mut() {
                rewrite_expr(p, ctx);
            }
            rewrite_expr(expected, ctx);
            rewrite_expr(desired, ctx);
        }
        Expr::Binary { lhs, rhs, .. } => {
            rewrite_expr(lhs, ctx);
            rewrite_expr(rhs, ctx);
        }
        Expr::Range { start, end } => {
            rewrite_expr(start, ctx);
            rewrite_expr(end, ctx);
        }
        Expr::Call { .. } => unreachable!("handled above"),
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            rewrite_expr(recv, ctx);
            for arg in args {
                rewrite_expr(arg, ctx);
            }
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => rewrite_expr(base, ctx),
        Expr::ArrayPtr { array, .. } => rewrite_expr(array, ctx),
        Expr::Index { .. } => unreachable!("handled above"),
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                rewrite_expr(value, ctx);
            }
        }
        Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) | Expr::VecLit(fields) => {
            for elem in fields {
                rewrite_expr(elem, ctx);
            }
        }
        Expr::ArrayRepeat { elem, .. } => rewrite_expr(elem, ctx),
        Expr::VecRepeat { elem, len } => {
            rewrite_expr(elem, ctx);
            rewrite_expr(len, ctx);
        }
        Expr::Closure { body, .. } => rewrite_expr(body, ctx),
        Expr::Macro { args, .. } => {
            for arg in args {
                rewrite_expr(arg, ctx);
            }
        }
        Expr::Match { expr, arms } => {
            rewrite_expr(expr, ctx);
            for arm in arms {
                rewrite_expr(&mut arm.value, ctx);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            rewrite_expr(cond, ctx);
            rewrite_expr(then_expr, ctx);
            rewrite_expr(else_expr, ctx);
        }
    }
}
