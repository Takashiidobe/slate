use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingId, BindingKind, CallArgFact, CallArgPinning, CallCallee, CallParamFact,
    CallSignatureFact, CallSignatureSource, CallsiteFact, FixupFacts, FunctionId, LibcCallSemantic,
    PathSegment, SignatureId, Site,
};
use crate::function_identity::{FunctionIdentity, Known};
use crate::rust_ast::{
    Block, Expr, ExternDecl, FnParam, IndentStmt, Item, Pattern, Program, Stmt, Type,
};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.call_signatures.clear();
    facts.callsites.clear();

    let signatures = collect_signatures(program, facts);
    let by_name = signatures
        .iter()
        .map(|signature| (signature.name.clone(), signature.id))
        .collect::<BTreeMap<_, _>>();
    let mut callsites = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut collector = Collector::new(function, facts, &signatures, &by_name);
        collector.enter_root_scope();
        collector.body(&f.body, &mut Vec::new(), false);
        callsites.extend(collector.callsites);
    }

    facts.call_signatures = signatures;
    facts.callsites = callsites;
}

fn collect_signatures(program: &Program, facts: &FixupFacts) -> Vec<CallSignatureFact> {
    let mut signatures = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        match item {
            Item::Fn(f) => {
                let Some(function) = facts.function_by_item_index(item_index) else {
                    continue;
                };
                push_signature(
                    &mut signatures,
                    f.name.clone(),
                    CallSignatureSource::Function(function),
                    params_from_fn_params(&f.params),
                    false,
                    f.ret.clone(),
                    f.returns_nonnull,
                );
            }
            Item::ExternBlock { decls, .. } => {
                for (decl_index, decl) in decls.iter().enumerate() {
                    let ExternDecl::Fn(f) = decl else {
                        continue;
                    };
                    push_signature(
                        &mut signatures,
                        f.name.clone(),
                        CallSignatureSource::Extern {
                            item_index,
                            decl_index,
                        },
                        params_from_fn_params(&f.params),
                        f.variadic,
                        f.ret.clone(),
                        f.returns_nonnull,
                    );
                }
            }
            Item::Cfg { item, .. } => {
                collect_cfg_signature(item, item_index, facts, &mut signatures)
            }
            _ => {}
        }
    }
    signatures
}

fn collect_cfg_signature(
    item: &Item,
    item_index: usize,
    facts: &FixupFacts,
    signatures: &mut Vec<CallSignatureFact>,
) {
    match item {
        Item::Fn(f) => {
            if let Some(function) = facts.function_by_item_index(item_index) {
                push_signature(
                    signatures,
                    f.name.clone(),
                    CallSignatureSource::Function(function),
                    params_from_fn_params(&f.params),
                    false,
                    f.ret.clone(),
                    f.returns_nonnull,
                );
            }
        }
        Item::ExternBlock { decls, .. } => {
            for (decl_index, decl) in decls.iter().enumerate() {
                let ExternDecl::Fn(f) = decl else {
                    continue;
                };
                push_signature(
                    signatures,
                    f.name.clone(),
                    CallSignatureSource::Extern {
                        item_index,
                        decl_index,
                    },
                    params_from_fn_params(&f.params),
                    f.variadic,
                    f.ret.clone(),
                    f.returns_nonnull,
                );
            }
        }
        Item::Cfg { item, .. } => collect_cfg_signature(item, item_index, facts, signatures),
        _ => {}
    }
}

fn push_signature(
    signatures: &mut Vec<CallSignatureFact>,
    name: String,
    source: CallSignatureSource,
    params: Vec<CallParamFact>,
    variadic: bool,
    ret: Option<Type>,
    returns_nonnull: bool,
) {
    let id = SignatureId(signatures.len());
    signatures.push(CallSignatureFact {
        id,
        semantics: BTreeSet::new(),
        name,
        source,
        params,
        variadic,
        ret,
        returns_nonnull,
    });
}

fn params_from_fn_params(params: &[FnParam]) -> Vec<CallParamFact> {
    params
        .iter()
        .enumerate()
        .map(|(index, param)| CallParamFact {
            index,
            name: param.name.clone(),
            ty: param.ty.clone(),
            nonnull: param.nonnull,
        })
        .collect()
}

struct Collector<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    signatures: &'a [CallSignatureFact],
    by_name: &'a BTreeMap<String, SignatureId>,
    scopes: Vec<BTreeMap<String, Option<BindingId>>>,
    callsites: Vec<CallsiteFact>,
}

impl<'a> Collector<'a> {
    fn new(
        function: FunctionId,
        facts: &'a FixupFacts,
        signatures: &'a [CallSignatureFact],
        by_name: &'a BTreeMap<String, SignatureId>,
    ) -> Self {
        Self {
            function,
            facts,
            signatures,
            by_name,
            scopes: Vec::new(),
            callsites: Vec::new(),
        }
    }

    fn enter_root_scope(&mut self) {
        self.scopes.push(BTreeMap::new());
        let params: Vec<_> = self
            .facts
            .bindings
            .iter()
            .filter(|binding| binding.function == self.function)
            .filter_map(|binding| match binding.kind {
                BindingKind::Param { .. } => Some((binding.name.clone(), binding.id)),
                BindingKind::Local => None,
            })
            .collect();
        for (name, id) in params {
            self.bind(name, Some(id));
        }
    }

    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>, scoped: bool) {
        if scoped {
            self.scopes.push(BTreeMap::new());
        }
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                self.stmt(&indent.stmt, path);
            });
        }
        if scoped {
            self.scopes.pop();
        }
    }

    fn block(&mut self, block: &Block, path: &mut Vec<PathSegment>) {
        self.scopes.push(BTreeMap::new());
        self.body(&block.stmts, path, false);
        if let Some(tail) = &block.tail {
            walk::with_path_segment(path, PathSegment::BlockTail, |path| {
                self.expr(tail, path, None);
            });
        }
        self.scopes.pop();
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let { name, init, .. } => {
                let result_binding = self.local_binding(name, path);
                if let Some(init) = init {
                    self.expr(init, path, result_binding);
                }
                self.define_local(name, path);
            }
            Stmt::LetIf {
                name,
                cond,
                then_body,
                then_value,
                else_body,
                else_value,
                ..
            } => {
                self.expr(cond, path, None);
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.body(then_body, path, false);
                    self.expr(then_value, path, None);
                    self.scopes.pop();
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.body(else_body, path, false);
                    self.expr(else_value, path, None);
                    self.scopes.pop();
                });
                self.define_local(name, path);
            }
            Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
                self.expr(target, path, None);
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(value, path, None);
                });
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => self.expr(expr, path, None),
            Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                self.expr(cond, path, None);
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.body(then_body, path, true)
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.body(else_body, path, true)
                });
            }
            Stmt::Loop { body, .. } => {
                walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                    self.body(body, path, true)
                });
            }
            Stmt::For { pat, iter, body } => {
                self.expr(iter, path, None);
                let binding = self.local_binding(pat, path);
                walk::with_path_segment(path, PathSegment::ForBody, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.bind(pat.to_string(), binding);
                    self.body(body, path, false);
                    self.scopes.pop();
                });
            }
            Stmt::Scope { body } => {
                walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                    self.body(body, path, true)
                });
            }
            Stmt::LabeledBlock { body, .. } => {
                walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                    self.body(body, path, true)
                });
            }
            Stmt::Unsafe { body } => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::While { cond, body } => {
                self.expr(cond, path, None);
                walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::Block(body) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::Match { expr, arms } => {
                self.expr(expr, path, None);
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                        self.scopes.push(BTreeMap::new());
                        self.shadow_pattern(&arm.pattern);
                        self.body(&arm.body, path, false);
                        self.scopes.pop();
                    });
                }
            }
            Stmt::InlineAsm(_) => {}
        }
    }

    fn expr(
        &mut self,
        expr: &Expr,
        path: &mut Vec<PathSegment>,
        result_binding: Option<BindingId>,
    ) {
        match expr {
            Expr::Call { func, args, .. } => {
                self.record_call(expr, args, path, result_binding);
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(func, path, None)
                });
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path, None)
                    });
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(recv, path, None)
                });
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path, None)
                    });
                }
            }
            Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::CStr(_)
            | Expr::Var(_)
            | Expr::Path(_)
            | Expr::Todo(_)
            | Expr::AtomicFence { .. } => {}
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(expr, path, None)
                });
            }
            Expr::Binary { lhs, rhs, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(lhs, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(rhs, path, None)
                });
            }
            Expr::Range { start, end } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(start, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(end, path, None)
                });
            }
            Expr::Field { base, .. }
            | Expr::TupleField { base, .. }
            | Expr::ArrayPtr { array: base, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(base, path, None)
                });
            }
            Expr::Index { base, index } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(base, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(index, path, None)
                });
            }
            Expr::StructLit { fields, .. } => {
                for (index, (_, value)) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path, None)
                    });
                }
            }
            Expr::TupleStructLit { fields, .. } => {
                for (index, value) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path, None)
                    });
                }
            }
            Expr::ArrayLit(elems) | Expr::VecLit(elems) | Expr::Macro { args: elems, .. } => {
                for (index, elem) in elems.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(elem, path, None)
                    });
                }
            }
            Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(elem, path, None)
                });
            }
            Expr::VecRepeat { elem, len } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(elem, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(len, path, None)
                });
            }
            Expr::Match { expr, arms } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(expr, path, None)
                });
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(&arm.value, path, None)
                    });
                }
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(cond, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(then_expr, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                    self.expr(else_expr, path, None)
                });
            }
            Expr::Block(block) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(block, path)
                });
            }
            Expr::Unsafe(block) => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.block(block, path)
                });
            }
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path, None)
                    });
                }
            }
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path, None)
                    });
                }
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(value, path, None)
                });
            }
            Expr::AtomicNew { value, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(value, path, None)
                });
            }
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path, None)
                    });
                }
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(expected, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                    self.expr(desired, path, None)
                });
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(src, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(dst, path, None)
                });
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(src, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(dst, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                    self.expr(count, path, None)
                });
            }
            Expr::WriteBytes { dst, val, count } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(dst, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(val, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                    self.expr(count, path, None)
                });
            }
        }
    }

    fn record_call(
        &mut self,
        expr: &Expr,
        args: &[Expr],
        path: &[PathSegment],
        result_binding: Option<BindingId>,
    ) {
        let (callee, signature) = match expr {
            Expr::Call { func, binding, .. } => match &**func {
                Expr::Var(name) => {
                    let signature = self.by_name.get(name.as_str()).copied();
                    let identity = match binding {
                        crate::function_identity::CallBinding::Direct { identity, .. } => *identity,
                        crate::function_identity::CallBinding::Indirect
                        | crate::function_identity::CallBinding::Generated => {
                            FunctionIdentity::Unknown
                        }
                    };
                    (
                        CallCallee::Direct {
                            name: name.as_str().to_string(),
                            signature,
                            identity,
                        },
                        signature.and_then(|id| self.signatures.get(id.0)),
                    )
                }
                _ => (CallCallee::Indirect, None),
            },
            _ => (CallCallee::Indirect, None),
        };
        let variadic_boundary = signature.and_then(|signature| {
            if signature.variadic {
                Some(signature.params.len())
            } else {
                None
            }
        });
        let arg_facts = args
            .iter()
            .enumerate()
            .map(|(slot, _)| {
                let declared_ty = signature
                    .and_then(|signature| signature.params.get(slot))
                    .map(|param| param.ty.clone());
                let variadic = variadic_boundary.is_some_and(|boundary| slot >= boundary);
                let pinning = if declared_ty.is_some() {
                    CallArgPinning::DeclaredParam
                } else if signature.is_some() && variadic {
                    CallArgPinning::VariadicUnpinned
                } else {
                    CallArgPinning::UnknownCallee
                };
                let mut arg_path = path.to_vec();
                arg_path.push(PathSegment::Expr(slot + 1));
                CallArgFact {
                    slot,
                    path: AstPath(arg_path),
                    declared_ty,
                    variadic,
                    pinning,
                }
            })
            .collect();
        let semantics = match &callee {
            CallCallee::Direct { identity, .. } => libc_semantics(*identity),
            CallCallee::Indirect => BTreeSet::new(),
        };
        self.callsites.push(CallsiteFact {
            site: Site {
                function: self.function,
                path: AstPath(path.to_vec()),
            },
            callee,
            args: arg_facts,
            variadic_boundary,
            ret: signature.and_then(|signature| signature.ret.clone()),
            result_binding,
            semantics,
        });
    }

    fn define_local(&mut self, name: &str, path: &[PathSegment]) {
        self.bind(name.to_string(), self.local_binding(name, path));
    }

    fn local_binding(&self, name: &str, path: &[PathSegment]) -> Option<BindingId> {
        self.facts
            .binding_by_local_path(self.function, name, &AstPath(path.to_vec()))
    }

    fn shadow_pattern(&mut self, pattern: &Pattern) {
        match pattern {
            Pattern::Binding(name) => self.bind(name.to_string(), None),
            Pattern::TupleStruct { fields, .. } => {
                for field in fields {
                    self.shadow_pattern(field);
                }
            }
            Pattern::Wildcard | Pattern::I64(_) | Pattern::I128(_) | Pattern::U128(_) => {}
        }
    }

    fn bind(&mut self, name: String, binding: Option<BindingId>) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.insert(name, binding);
        }
    }
}

fn libc_semantics(identity: FunctionIdentity) -> BTreeSet<LibcCallSemantic> {
    match identity {
        FunctionIdentity::Known(Known::Printf) => BTreeSet::from([LibcCallSemantic::Printf]),
        FunctionIdentity::Known(Known::StrLen) => BTreeSet::from([LibcCallSemantic::StrLen]),
        FunctionIdentity::Known(Known::StrCmp) => BTreeSet::from([LibcCallSemantic::StrCmp]),
        FunctionIdentity::Known(Known::StrNCmp) => BTreeSet::from([LibcCallSemantic::StrNCmp]),
        FunctionIdentity::Known(Known::MemCmp) => BTreeSet::from([LibcCallSemantic::MemCmp]),
        FunctionIdentity::Known(Known::StrCpy) => BTreeSet::from([LibcCallSemantic::StrCpy]),
        FunctionIdentity::Known(Known::StrNCpy) => BTreeSet::from([LibcCallSemantic::StrNCpy]),
        FunctionIdentity::Known(Known::StrCat) => BTreeSet::from([LibcCallSemantic::StrCat]),
        FunctionIdentity::Known(Known::StrNCat) => BTreeSet::from([LibcCallSemantic::StrNCat]),
        FunctionIdentity::Known(Known::MemCpy) => BTreeSet::from([LibcCallSemantic::MemCpy]),
        FunctionIdentity::Known(Known::MemSet) => BTreeSet::from([LibcCallSemantic::MemSet]),
        FunctionIdentity::Known(Known::FOpen) => BTreeSet::from([LibcCallSemantic::FOpen]),
        FunctionIdentity::Known(Known::FRead) => BTreeSet::from([LibcCallSemantic::FRead]),
        FunctionIdentity::Known(Known::FWrite) => BTreeSet::from([LibcCallSemantic::FWrite]),
        FunctionIdentity::Known(Known::FGets) => BTreeSet::from([LibcCallSemantic::FGets]),
        FunctionIdentity::Known(Known::FPuts) => BTreeSet::from([LibcCallSemantic::FPuts]),
        FunctionIdentity::Known(Known::FClose) => BTreeSet::from([LibcCallSemantic::FClose]),
        _ => BTreeSet::new(),
    }
}
