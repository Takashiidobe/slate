use std::collections::BTreeMap;

use crate::backend::facts::walk;
use crate::backend::facts::{
    self, AstPath, BindingFact, BindingId, BindingKind, CallArgFact, CallArgPinning, CallCallee,
    CallParamFact, CallSignatureFact, CallSignatureSource, CallsiteFact, FunctionId, PathSegment,
    SignatureId, Site,
};
use crate::backend::rust_ast::{
    Block, Expr, ExternDecl, ExternFnDecl, FnDef, FnParam, IndentStmt, Item, Pattern, Program, Stmt,
};
use crate::function_identity::FunctionIdentity;
pub(in crate::backend) fn collect_callsites_for_function<'db>(
    function: FunctionId<'db>,
    f_body: &[IndentStmt],
    bindings: &[BindingFact<'db>],
    signatures: &[CallSignatureFact<'db>],
    by_name: &BTreeMap<String, SignatureId>,
) -> Vec<CallsiteFact<'db>> {
    let mut collector = Collector::new(function, bindings, signatures, by_name);
    collector.enter_root_scope();
    collector.body(f_body, &mut Vec::new(), false);
    collector.callsites
}

fn push_extern_signatures<'db>(
    item_index: usize,
    decls: &[ExternDecl],
    signatures: &mut Vec<CallSignatureFact<'db>>,
) {
    for (decl_index, decl) in decls.iter().enumerate() {
        let ExternDecl::Fn(f) = decl else {
            continue;
        };
        push_signature(signatures, extern_call_signature(item_index, decl_index, f));
    }
}

pub(in crate::backend) fn collect_extern_signatures<'db>(
    program: &Program,
) -> Vec<CallSignatureFact<'db>> {
    let mut signatures = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        collect_extern_signatures_from_item(item, item_index, &mut signatures);
    }
    signatures
}

fn collect_extern_signatures_from_item<'db>(
    item: &Item,
    item_index: usize,
    signatures: &mut Vec<CallSignatureFact<'db>>,
) {
    match item {
        Item::ExternBlock { decls, .. } => push_extern_signatures(item_index, decls, signatures),
        Item::Cfg { item, .. } => collect_extern_signatures_from_item(item, item_index, signatures),
        _ => {}
    }
}

pub(in crate::backend) fn local_call_signature<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
) -> CallSignatureFact<'db> {
    CallSignatureFact {
        id: SignatureId(0),
        name: f.name.clone(),
        source: CallSignatureSource::Function(function),
        params: params_from_fn_params(&f.params),
        variadic: false,
        ret: f.ret.clone(),
    }
}

fn extern_call_signature<'db>(
    item_index: usize,
    decl_index: usize,
    f: &ExternFnDecl,
) -> CallSignatureFact<'db> {
    CallSignatureFact {
        id: SignatureId(0),
        name: f.name.clone(),
        source: CallSignatureSource::Extern {
            item_index,
            decl_index,
        },
        params: params_from_fn_params(&f.params),
        variadic: f.variadic,
        ret: f.ret.clone(),
    }
}

fn push_signature<'db>(
    signatures: &mut Vec<CallSignatureFact<'db>>,
    mut fact: CallSignatureFact<'db>,
) {
    fact.id = SignatureId(signatures.len());
    signatures.push(fact);
}

fn params_from_fn_params(params: &[FnParam]) -> Vec<CallParamFact> {
    params
        .iter()
        .map(|param| CallParamFact {
            ty: param.ty.clone(),
        })
        .collect()
}

struct Collector<'db, 'a> {
    function: FunctionId<'db>,
    bindings: &'a [BindingFact<'db>],
    signatures: &'a [CallSignatureFact<'db>],
    by_name: &'a BTreeMap<String, SignatureId>,
    scopes: Vec<BTreeMap<String, Option<BindingId<'db>>>>,
    callsites: Vec<CallsiteFact<'db>>,
}

impl<'db, 'a> Collector<'db, 'a> {
    fn new(
        function: FunctionId<'db>,
        bindings: &'a [BindingFact<'db>],
        signatures: &'a [CallSignatureFact<'db>],
        by_name: &'a BTreeMap<String, SignatureId>,
    ) -> Self {
        Self {
            function,
            bindings,
            signatures,
            by_name,
            scopes: Vec::new(),
            callsites: Vec::new(),
        }
    }

    fn enter_root_scope(&mut self) {
        self.scopes.push(BTreeMap::new());
        let params: Vec<_> = self
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
                self.expr(tail, path);
            });
        }
        self.scopes.pop();
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let { name, init, .. } => {
                if let Some(init) = init {
                    self.expr(init, path);
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
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.body(then_body, path, false);
                    self.expr(then_value, path);
                    self.scopes.pop();
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.body(else_body, path, false);
                    self.expr(else_value, path);
                    self.scopes.pop();
                });
                self.define_local(name, path);
            }
            Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
                self.expr(target, path);
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(value, path);
                });
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => self.expr(expr, path),
            Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                self.expr(cond, path);
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
                self.expr(iter, path);
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
                self.expr(cond, path);
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
                self.expr(expr, path);
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

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) {
        match expr {
            Expr::Call { func, args, .. } => {
                self.record_call(expr, args, path);
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(func, path));
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path)
                    });
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(recv, path));
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path)
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
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path));
            }
            Expr::Binary { lhs, rhs, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(lhs, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(rhs, path));
            }
            Expr::Range { start, end } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(start, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(end, path));
            }
            Expr::Field { base, .. }
            | Expr::TupleField { base, .. }
            | Expr::ArrayPtr { array: base, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(base, path));
            }
            Expr::Index { base, index } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(base, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(index, path));
            }
            Expr::StructLit { fields, .. } => {
                for (index, (_, value)) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path)
                    });
                }
            }
            Expr::TupleStructLit { fields, .. } => {
                for (index, value) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path)
                    });
                }
            }
            Expr::ArrayLit(elems) | Expr::VecLit(elems) | Expr::Macro { args: elems, .. } => {
                for (index, elem) in elems.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(elem, path)
                    });
                }
            }
            Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(elem, path));
            }
            Expr::VecRepeat { elem, len } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(elem, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(len, path));
            }
            Expr::Match { expr, arms } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path));
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(&arm.value, path)
                    });
                }
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(cond, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(then_expr, path)
                });
                walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                    self.expr(else_expr, path)
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
                        self.expr(ptr, path)
                    });
                }
            }
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path)
                    });
                }
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(value, path));
            }
            Expr::AtomicNew { value, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(value, path));
            }
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path)
                    });
                }
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(expected, path)
                });
                walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                    self.expr(desired, path)
                });
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(src, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(dst, path));
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(src, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(dst, path));
                walk::with_path_segment(path, PathSegment::Expr(2), |path| self.expr(count, path));
            }
            Expr::WriteBytes { dst, val, count } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(dst, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(val, path));
                walk::with_path_segment(path, PathSegment::Expr(2), |path| self.expr(count, path));
            }
        }
    }

    fn record_call(&mut self, expr: &Expr, args: &[Expr], path: &[PathSegment]) {
        let (callee, signature) = match expr {
            Expr::Call { func, binding, .. } => match &**func {
                Expr::Var(name) => {
                    let signature = self
                        .by_name
                        .get(name.as_str())
                        .and_then(|id| self.signatures.get(id.0));
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
                            signature: signature.cloned(),
                            identity,
                        },
                        signature,
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
        self.callsites.push(CallsiteFact {
            site: Site {
                function: self.function,
                path: AstPath(path.to_vec()),
            },
            callee,
            args: arg_facts,
            ret: signature.and_then(|signature| signature.ret.clone()),
        });
    }

    fn define_local(&mut self, name: &str, path: &[PathSegment]) {
        self.bind(name.to_string(), self.local_binding(name, path));
    }

    fn local_binding(&self, name: &str, path: &[PathSegment]) -> Option<BindingId<'db>> {
        facts::binding_by_local_path(self.bindings, self.function, name, &AstPath(path.to_vec()))
    }

    fn shadow_pattern(&mut self, pattern: &Pattern) {
        match pattern {
            Pattern::Binding(name) => self.bind(name.to_string(), None),
            Pattern::Guarded { bind, .. } => self.bind(bind.to_string(), None),
            Pattern::TupleStruct { fields, .. } => {
                for field in fields {
                    self.shadow_pattern(field);
                }
            }
            Pattern::Wildcard
            | Pattern::I64(_)
            | Pattern::I128(_)
            | Pattern::U128(_)
            | Pattern::InclusiveRange { .. } => {}
        }
    }

    fn bind(&mut self, name: String, binding: Option<BindingId<'db>>) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.insert(name, binding);
        }
    }
}
