use std::collections::BTreeMap;

use crate::backend::interproc::{self, CallGraph};
use crate::backend::rust_ast::{
    Block, Expr, FnDef, Item, Prim, Program, RustValue, Stmt, Type, Visibility,
};
use crate::function_identity::{CallBinding, FunctionIdentity, Known};

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
        if self.escape {
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
        let base = match (self.write, self.unique, self.free, self.offset) {
            (false, false, false, false) => ResolvedPtrType::Ref,
            (true, true, false, false) => ResolvedPtrType::RefMut,
            (true, false, false, false) => ResolvedPtrType::RefCell,
            (false, true, true, false) => ResolvedPtrType::Owned,
            (false, false, false, true) => ResolvedPtrType::Slice,
            (true, true, false, true) => ResolvedPtrType::SliceMut,
            (false, true, true, true) => ResolvedPtrType::Vec,
            _ => {
                if self.write {
                    ResolvedPtrType::RawMut
                } else {
                    ResolvedPtrType::RawConst
                }
            }
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
            ResolvedPtrType::Vec => ResolvedPtrType::StringOwned,
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

pub(in crate::backend) fn solve(program: &Program) -> BTreeMap<PointerBinding, PointerFact> {
    let fn_defs = collect_fn_defs(&program.items);
    let candidates: BTreeMap<String, Candidate> = fn_defs
        .iter()
        .filter_map(|(name, f)| candidate_for(f).map(|c| (name.clone(), c)))
        .collect();
    if candidates.is_empty() {
        return BTreeMap::new();
    }

    let mut facts: BTreeMap<PointerBinding, PointerFact> = BTreeMap::new();
    let mut call_sites: Vec<CallSite> = Vec::new();
    for (name, candidate) in &candidates {
        let f = fn_defs[name];
        let (local, sites) = classify_function(name, f, candidate, &candidates);
        facts.extend(local);
        call_sites.extend(sites);
    }

    let mut outbound: BTreeMap<String, Vec<&CallSite>> = BTreeMap::new();
    for site in &call_sites {
        outbound.entry(site.caller.clone()).or_default().push(site);
    }

    let mut graph = CallGraph::new(candidates.keys().cloned());
    for site in &call_sites {
        graph.add_edge(&site.caller, &site.callee);
    }
    let order = interproc::scc_order(&graph);
    let empty: Vec<&CallSite> = Vec::new();

    interproc::run_worklist(&graph, &order, |name| {
        let mut changed = false;
        for site in outbound.get(name).unwrap_or(&empty) {
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
        for site in &call_sites {
            if site.callee != name {
                continue;
            }
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
        changed
    });

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

fn classify_function(
    name: &str,
    f: &FnDef,
    candidate: &Candidate,
    candidates: &BTreeMap<String, Candidate>,
) -> (BTreeMap<PointerBinding, PointerFact>, Vec<CallSite>) {
    let mut facts = BTreeMap::new();
    let mut tracked: BTreeMap<String, String> = BTreeMap::new();
    for &index in &candidate.param_indices {
        let param = &f.params[index];
        let binding = PointerBinding {
            function: name.to_string(),
            name: param.name.clone(),
        };
        facts.insert(binding, PointerFact::new(elem_is_byte(&param.ty)));
        tracked.insert(param.name.clone(), param.name.clone());
    }

    let mut sites = Vec::new();
    let mut ctx = ClassifyCtx {
        function: name,
        candidates,
        facts: &mut facts,
        tracked: &mut tracked,
        sites: &mut sites,
    };
    ctx.body(&f.body);
    (facts, sites)
}

struct ClassifyCtx<'a> {
    function: &'a str,
    candidates: &'a BTreeMap<String, Candidate>,
    facts: &'a mut BTreeMap<PointerBinding, PointerFact>,
    tracked: &'a mut BTreeMap<String, String>,
    sites: &'a mut Vec<CallSite>,
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
                if let Expr::Var(source) = init
                    && let Some(canonical) = self.canonical_of(source.as_str())
                {
                    self.tracked.insert(name.clone(), canonical.clone());
                    self.observe(&canonical, PointerFact::disprove_unique);
                } else {
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
                self.place(target, true);
                self.expr(value);
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
                    self.observe(&canonical, PointerFact::observe_escape);
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
                if let Expr::Var(name) = &**expr
                    && let Some(canonical) = self.canonical_of(name.as_str())
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
                if matches!(
                    op,
                    crate::backend::rust_ast::BinOp::Eq | crate::backend::rust_ast::BinOp::Ne
                ) {
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
                self.expr(dst);
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                self.expr(src);
                self.expr(dst);
                self.expr(count);
            }
            Expr::WriteBytes { dst, val, count } => {
                self.expr(dst);
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

fn peel_cast(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_cast(expr),
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
