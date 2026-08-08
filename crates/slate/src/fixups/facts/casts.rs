use std::collections::BTreeMap;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingFact, BindingKind, CallSignatureFact, CallsiteFact, CastFact, FunctionFact,
    FunctionId, PathSegment, Site,
};
use crate::rust_ast::{
    AtomicType, Block, Expr, FnDef, IndentStmt, Pattern, Prim, RustValue, Stmt, Type, UnaryOp,
};
pub(in crate::fixups) fn collect_for_function<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
    bindings: &[BindingFact<'db>],
    functions: &[FunctionFact<'db>],
    call_signatures: &[CallSignatureFact<'db>],
    callsites: &[CallsiteFact<'db>],
) -> Vec<CastFact<'db>> {
    let mut collector = Collector::new(function, bindings, functions, call_signatures, callsites);
    collector.enter_root_scope();
    collector.body(&f.body, &mut Vec::new(), false);
    collector.casts
}

struct Collector<'db, 'a> {
    function: FunctionId<'db>,
    bindings: &'a [BindingFact<'db>],
    functions: &'a [FunctionFact<'db>],
    call_signatures: &'a [CallSignatureFact<'db>],
    callsites: &'a [CallsiteFact<'db>],
    scopes: Vec<BTreeMap<String, Option<Type>>>,
    casts: Vec<CastFact<'db>>,
}

impl<'db, 'a> Collector<'db, 'a> {
    fn new(
        function: FunctionId<'db>,
        bindings: &'a [BindingFact<'db>],
        functions: &'a [FunctionFact<'db>],
        call_signatures: &'a [CallSignatureFact<'db>],
        callsites: &'a [CallsiteFact<'db>],
    ) -> Self {
        Self {
            function,
            bindings,
            functions,
            call_signatures,
            callsites,
            scopes: Vec::new(),
            casts: Vec::new(),
        }
    }

    fn enter_root_scope(&mut self) {
        self.scopes.push(BTreeMap::new());
        let params: Vec<_> = self
            .bindings
            .iter()
            .filter(|binding| binding.function == self.function)
            .filter_map(|binding| match binding.kind {
                BindingKind::Param { index } => self
                    .function_param_ty(index)
                    .map(|ty| (binding.name.clone(), Some(ty))),
                BindingKind::Local => None,
            })
            .collect();
        for (name, ty) in params {
            self.bind(name, ty);
        }
    }

    fn function_param_ty(&self, index: usize) -> Option<Type> {
        self.functions
            .iter()
            .find(|function| function.id == self.function)
            .and_then(|function| {
                self.call_signatures
                    .iter()
                    .find(|signature| signature.name == function.name)
                    .and_then(|signature| signature.params.get(index))
                    .map(|param| param.ty.clone())
            })
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

    fn block(&mut self, block: &Block, path: &mut Vec<PathSegment>) -> Option<Type> {
        self.scopes.push(BTreeMap::new());
        self.body(&block.stmts, path, false);
        let tail_ty = block.tail.as_ref().and_then(|tail| {
            walk::with_path_segment(path, PathSegment::BlockTail, |path| self.expr(tail, path))
        });
        self.scopes.pop();
        tail_ty
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let { name, ty, init, .. } => {
                if let Some(init) = init {
                    self.expr(init, path);
                }
                self.bind(name.to_string(), ty.clone());
            }
            Stmt::LetIf {
                name,
                ty,
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
                self.bind(name.to_string(), ty.clone());
            }
            Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
                self.expr(target, path);
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(value, path);
                });
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
                self.expr(expr, path);
            }
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
                walk::with_path_segment(path, PathSegment::ForBody, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.bind(pat.to_string(), None);
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
                    self.block(body, path);
                });
            }
            Stmt::While { cond, body } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                    self.block(body, path);
                });
            }
            Stmt::Block(body) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(body, path);
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

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) -> Option<Type> {
        match expr {
            Expr::Cast { expr: inner, ty } => {
                let from = walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(inner, path)
                });
                self.record_cast(from.clone(), ty.clone(), path);
                Some(ty.clone())
            }
            Expr::Value(value) => literal_type(value),
            Expr::Var(name) => self.ty_for_name(name.as_str()),
            Expr::Str(_) => Some(Type::Ref {
                mutable: false,
                inner: Box::new(Type::Str),
            }),
            Expr::ByteStr(bytes) => Some(Type::Ref {
                mutable: false,
                inner: Box::new(Type::Array {
                    elem: Box::new(Type::Prim(Prim::U8)),
                    len: bytes.len() as u64,
                }),
            }),
            Expr::CStr(_) => Some(Type::Ref {
                mutable: false,
                inner: Box::new(Type::Custom("core::ffi::CStr".to_string())),
            }),
            Expr::Unary { op, expr } => {
                let inner = walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(expr, path)
                });
                match op {
                    UnaryOp::Deref => match inner {
                        Some(Type::Ptr { inner, .. }) | Some(Type::Ref { inner, .. }) => {
                            Some(*inner)
                        }
                        _ => None,
                    },
                    UnaryOp::Neg => inner,
                    UnaryOp::Not => Some(Type::Prim(Prim::Bool)),
                }
            }
            Expr::Binary { lhs, rhs, .. } => {
                let lhs = walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(lhs, path)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(rhs, path));
                lhs
            }
            Expr::Range { start, end } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(start, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(end, path));
                None
            }
            Expr::Ref { mutable, expr } => {
                let inner = walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(expr, path)
                })?;
                Some(Type::Ref {
                    mutable: *mutable,
                    inner: Box::new(inner),
                })
            }
            Expr::AddrOf { mutable, expr } => {
                let inner = walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(expr, path)
                })?;
                Some(Type::Ptr {
                    mutable: *mutable,
                    inner: Box::new(inner),
                })
            }
            Expr::ArrayPtr { array, mutable } => {
                let inner = walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(array, path)
                });
                array_elem_ty(inner).map(|inner| Type::Ptr {
                    mutable: *mutable,
                    inner: Box::new(inner),
                })
            }
            Expr::Index { base, index } => {
                let base = walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(base, path)
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(index, path));
                array_elem_ty(base)
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(base, path));
                None
            }
            Expr::Call { func, args, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(func, path));
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path)
                    });
                }
                self.call_ret_ty(path)
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(recv, path));
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path)
                    });
                }
                None
            }
            Expr::ArrayLit(elems) => {
                let elem_ty = elems.iter().enumerate().find_map(|(index, elem)| {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(elem, path)
                    })
                });
                elem_ty.map(|elem| Type::Array {
                    elem: Box::new(elem),
                    len: elems.len() as u64,
                })
            }
            Expr::ArrayRepeat { elem, len } => {
                let elem = walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(elem, path)
                })?;
                Some(Type::Array {
                    elem: Box::new(elem),
                    len: *len as u64,
                })
            }
            Expr::VecLit(elems) => {
                let elem_ty = elems.iter().enumerate().find_map(|(index, elem)| {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(elem, path)
                    })
                });
                elem_ty.map(|elem| Type::Generic {
                    name: "Vec".into(),
                    args: vec![elem],
                })
            }
            Expr::VecRepeat { elem, len } => {
                let elem = walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(elem, path)
                })?;
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(len, path));
                Some(Type::Generic {
                    name: "Vec".into(),
                    args: vec![elem],
                })
            }
            Expr::Block(block) => walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                self.block(block, path)
            }),
            Expr::Unsafe(block) => walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                self.block(block, path)
            }),
            Expr::StructLit { fields, .. } => {
                for (index, (_, value)) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path)
                    });
                }
                None
            }
            Expr::TupleStructLit { fields, .. } => {
                for (index, value) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path)
                    });
                }
                None
            }
            Expr::Macro { args, .. } => {
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(arg, path)
                    });
                }
                None
            }
            Expr::Closure { body, params } => {
                self.scopes.push(BTreeMap::new());
                for param in params {
                    self.bind(param.to_string(), None);
                }
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(body, path));
                self.scopes.pop();
                None
            }
            Expr::Match { expr, arms } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path));

                arms.iter().enumerate().find_map(|(index, arm)| {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(&arm.value, path)
                    })
                })
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(cond, path));
                let then_ty = walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(then_expr, path)
                });
                walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                    self.expr(else_expr, path)
                });
                then_ty
            }
            Expr::Transmute { expr, to, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path));
                Some(to.clone())
            }
            Expr::AtomicRef { place, ty } | Expr::AtomicLoad { place, ty, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path)
                    });
                }
                Some(atomic_rust_type(*ty))
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
                None
            }
            Expr::AtomicNew { value, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(value, path));
                None
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
                None
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(src, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(dst, path));
                None
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(src, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(dst, path));
                walk::with_path_segment(path, PathSegment::Expr(2), |path| self.expr(count, path));
                None
            }
            Expr::WriteBytes { dst, val, count } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(dst, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(val, path));
                walk::with_path_segment(path, PathSegment::Expr(2), |path| self.expr(count, path));
                None
            }
            Expr::HexFloat(_) | Expr::AtomicFence { .. } | Expr::Todo(_) | Expr::Path(_) => None,
        }
    }

    fn record_cast(&mut self, from: Option<Type>, to: Type, path: &[PathSegment]) {
        self.casts.push(CastFact {
            site: Site {
                function: self.function,
                path: AstPath(path.to_vec()),
            },
            from,
            to,
        });
    }

    fn call_ret_ty(&self, path: &[PathSegment]) -> Option<Type> {
        let path = AstPath(path.to_vec());
        self.callsites
            .iter()
            .find(|callsite| callsite.site.function == self.function && callsite.site.path == path)
            .and_then(|callsite| callsite.ret.clone())
    }

    fn bind(&mut self, name: String, ty: Option<Type>) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.insert(name, ty);
        }
    }

    fn ty_for_name(&self, name: &str) -> Option<Type> {
        self.scopes
            .iter()
            .rev()
            .find_map(|scope| scope.get(name).cloned())
            .flatten()
    }

    fn shadow_pattern(&mut self, pattern: &Pattern) {
        match pattern {
            Pattern::Binding(name) => self.bind(name.to_string(), None),
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
}

fn literal_type(value: &RustValue) -> Option<Type> {
    match value {
        RustValue::I64(_) => Some(Type::Prim(Prim::I64)),
        RustValue::Usize(_) => Some(Type::Prim(Prim::Usize)),
        RustValue::I128(_) => Some(Type::Prim(Prim::I128)),
        RustValue::U128(_) => Some(Type::Prim(Prim::U128)),
        RustValue::Bool(_) => Some(Type::Prim(Prim::Bool)),
        RustValue::Float(_) => Some(Type::Prim(Prim::F64)),
        RustValue::NullPtr | RustValue::None => None,
    }
}

fn array_elem_ty(ty: Option<Type>) -> Option<Type> {
    match ty {
        Some(Type::Array { elem, .. }) | Some(Type::Slice(elem)) => Some(*elem),
        Some(Type::Ref { inner, .. }) => array_elem_ty(Some(*inner)),
        _ => None,
    }
}

fn atomic_rust_type(ty: AtomicType) -> Type {
    Type::Prim(match ty {
        AtomicType::I8 => Prim::I8,
        AtomicType::U8 => Prim::U8,
        AtomicType::I16 => Prim::I16,
        AtomicType::U16 => Prim::U16,
        AtomicType::I32 => Prim::I32,
        AtomicType::U32 => Prim::U32,
        AtomicType::I64 => Prim::I64,
        AtomicType::U64 => Prim::U64,
        AtomicType::Isize => Prim::Isize,
        AtomicType::Usize => Prim::Usize,
        AtomicType::Bool => Prim::Bool,
    })
}
