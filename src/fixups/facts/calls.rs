use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingId, BindingKind, CallArgFact, CallArgPinning, CallCallee, CallParamFact,
    CallSignatureFact, CallSignatureSource, CallsiteFact, FixupFacts, FunctionId, LibcCallSemantic,
    PathSegment, SignatureId,
};
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
) {
    let id = SignatureId(signatures.len());
    signatures.push(CallSignatureFact {
        id,
        semantics: libc_semantics(&name),
        name,
        source,
        params,
        variadic,
        ret,
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
        }
    }

    fn expr(
        &mut self,
        expr: &Expr,
        path: &mut Vec<PathSegment>,
        result_binding: Option<BindingId>,
    ) {
        match expr {
            Expr::Call { func, args } => {
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
            Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(ptr, path, None)
                });
            }
            Expr::AtomicStore { ptr, value, .. }
            | Expr::AtomicFetch { ptr, value, .. }
            | Expr::AtomicSwap { ptr, value, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(ptr, path, None)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(value, path, None)
                });
            }
            Expr::AtomicCompareExchange {
                ptr,
                expected,
                desired,
                ..
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(ptr, path, None)
                });
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
            Expr::Call { func, .. } => match &**func {
                Expr::Var(name) => {
                    let signature = self.by_name.get(name.as_str()).copied();
                    (
                        CallCallee::Direct {
                            name: name.as_str().to_string(),
                            signature,
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
            CallCallee::Direct { name, .. } => libc_semantics(name),
            CallCallee::Indirect => BTreeSet::new(),
        };
        self.callsites.push(CallsiteFact {
            function: self.function,
            path: AstPath(path.to_vec()),
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
            Pattern::Wildcard | Pattern::I64(_) | Pattern::I128(_) => {}
        }
    }

    fn bind(&mut self, name: String, binding: Option<BindingId>) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.insert(name, binding);
        }
    }
}

fn libc_semantics(name: &str) -> BTreeSet<LibcCallSemantic> {
    match name {
        "printf" => BTreeSet::from([LibcCallSemantic::Printf]),
        "strlen" => BTreeSet::from([LibcCallSemantic::StrLen]),
        "strcmp" => BTreeSet::from([LibcCallSemantic::StrCmp]),
        "strncmp" => BTreeSet::from([LibcCallSemantic::StrNCmp]),
        "memcmp" => BTreeSet::from([LibcCallSemantic::MemCmp]),
        "strcpy" => BTreeSet::from([LibcCallSemantic::StrCpy]),
        "strncpy" => BTreeSet::from([LibcCallSemantic::StrNCpy]),
        "strcat" => BTreeSet::from([LibcCallSemantic::StrCat]),
        "strncat" => BTreeSet::from([LibcCallSemantic::StrNCat]),
        "memcpy" => BTreeSet::from([LibcCallSemantic::MemCpy]),
        "memset" => BTreeSet::from([LibcCallSemantic::MemSet]),
        _ => BTreeSet::new(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Expr, ExternDecl, ExternFnDecl, Item, Program, Stmt, Type};

    fn analyzed(items: Vec<Item>) -> facts::FixupFacts {
        facts::analyze(Program { items }).facts
    }

    fn extern_fn(name: &str, params: Vec<(&str, &str)>, variadic: bool, ret: Option<&str>) -> Item {
        Item::ExternBlock {
            abi: "C".into(),
            decls: vec![ExternDecl::Fn(ExternFnDecl {
                name: name.into(),
                params: params
                    .into_iter()
                    .map(|(name, ty)| crate::rust_ast::FnParam {
                        name: name.into(),
                        mutable: false,
                        ty: Type::parse(ty),
                    })
                    .collect(),
                variadic,
                ret: ret.map(Type::parse),
            })],
        }
    }

    fn signature<'a>(facts: &'a facts::FixupFacts, name: &str) -> &'a CallSignatureFact {
        facts
            .call_signatures
            .iter()
            .find(|signature| signature.name == name)
            .unwrap()
    }

    fn callsite_at(facts: &facts::FixupFacts, path: AstPath) -> &CallsiteFact {
        facts
            .callsites
            .iter()
            .find(|callsite| callsite.path == path)
            .unwrap()
    }

    #[test]
    fn records_function_and_extern_signatures() {
        let facts = analyzed(vec![
            Item::Fn(func(vec![param("x", "i32")], Some("i32"), vec![])),
            extern_fn("printf", vec![("fmt", "*mut i8")], true, Some("i32")),
        ]);

        let f = signature(&facts, "f");
        assert!(matches!(
            f.source,
            CallSignatureSource::Function(FunctionId(0))
        ));
        assert_eq!(f.params[0].name, "x");
        assert_eq!(f.params[0].ty.render(), "i32");
        assert_eq!(f.ret.as_ref().unwrap().render(), "i32");

        let printf = signature(&facts, "printf");
        assert!(matches!(
            printf.source,
            CallSignatureSource::Extern {
                item_index: 1,
                decl_index: 0
            }
        ));
        assert!(printf.variadic);
        assert!(printf.semantics.contains(&LibcCallSemantic::Printf));
    }

    #[test]
    fn records_normal_callsite_argument_slots_and_return_type() {
        let program = Program {
            items: vec![
                Item::Fn(crate::rust_ast::FnDef {
                    vis: crate::rust_ast::Visibility::Private,
                    unsafe_: false,
                    extern_c: false,
                    name: "add".into(),
                    params: vec![param("a", "i32"), param("b", "i32")],
                    ret: Some(Type::parse("i32")),
                    body: vec![],
                }),
                Item::Fn(func(
                    vec![],
                    None,
                    vec![temp("_v0", "i32", call("add", vec![int(1), int(2)]))],
                )),
            ],
        };
        let facts = facts::analyze(program).facts;
        let result = facts
            .bindings
            .iter()
            .find(|binding| binding.name == "_v0")
            .unwrap()
            .id;
        let callsite = callsite_at(&facts, AstPath(vec![PathSegment::Stmt(0)]));

        assert!(matches!(
            callsite.callee,
            CallCallee::Direct {
                ref name,
                signature: Some(_)
            } if name == "add"
        ));
        assert_eq!(callsite.ret.as_ref().unwrap().render(), "i32");
        assert_eq!(callsite.result_binding, Some(result));
        assert_eq!(callsite.args.len(), 2);
        assert_eq!(callsite.args[0].slot, 0);
        assert_eq!(
            callsite.args[0].path,
            AstPath(vec![PathSegment::Stmt(0), PathSegment::Expr(1)])
        );
        assert_eq!(
            callsite.args[0].declared_ty.as_ref().unwrap().render(),
            "i32"
        );
        assert_eq!(callsite.args[0].pinning, CallArgPinning::DeclaredParam);
    }

    #[test]
    fn records_variadic_boundary_and_unpinned_varargs() {
        let facts = analyzed(vec![
            extern_fn("printf", vec![("fmt", "*mut i8")], true, Some("i32")),
            Item::Fn(func(
                vec![],
                None,
                vec![Stmt::Expr(call("printf", vec![var("fmt"), int(7)]))],
            )),
        ]);
        let callsite = callsite_at(&facts, AstPath(vec![PathSegment::Stmt(0)]));

        assert_eq!(callsite.variadic_boundary, Some(1));
        assert_eq!(callsite.args[0].pinning, CallArgPinning::DeclaredParam);
        assert_eq!(callsite.args[1].pinning, CallArgPinning::VariadicUnpinned);
        assert!(callsite.args[1].variadic);
        assert!(callsite.semantics.contains(&LibcCallSemantic::Printf));
    }

    #[test]
    fn records_unknown_and_function_pointer_calls() {
        let facts = analyzed(vec![Item::Fn(func(
            vec![param("fp", "fn(i32) -> i32")],
            None,
            vec![
                Stmt::Expr(call("mystery", vec![int(1)])),
                Stmt::Expr(Expr::Call {
                    func: Box::new(var("fp")),
                    args: vec![int(2)],
                }),
            ],
        ))]);

        let mystery = callsite_at(&facts, AstPath(vec![PathSegment::Stmt(0)]));
        assert!(matches!(
            mystery.callee,
            CallCallee::Direct {
                ref name,
                signature: None
            } if name == "mystery"
        ));
        assert_eq!(mystery.args[0].pinning, CallArgPinning::UnknownCallee);

        let fp = callsite_at(&facts, AstPath(vec![PathSegment::Stmt(1)]));
        assert!(matches!(
            fp.callee,
            CallCallee::Direct {
                ref name,
                signature: None
            } if name == "fp"
        ));
    }

    #[test]
    fn records_calls_inside_unsafe_block_tails() {
        let facts = analyzed(vec![
            extern_fn("strlen", vec![("s", "*mut i8")], false, Some("usize")),
            Item::Fn(func(
                vec![],
                None,
                vec![temp(
                    "_v0",
                    "usize",
                    Expr::Unsafe(Box::new(Block {
                        stmts: vec![],
                        tail: Some(Box::new(call("strlen", vec![var("s")]))),
                    })),
                )],
            )),
        ]);

        let callsite = callsite_at(
            &facts,
            AstPath(vec![
                PathSegment::Stmt(0),
                PathSegment::UnsafeBody,
                PathSegment::BlockTail,
            ]),
        );
        assert_eq!(callsite.ret.as_ref().unwrap().render(), "usize");
        assert!(callsite.semantics.contains(&LibcCallSemantic::StrLen));
    }
}
