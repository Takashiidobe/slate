use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingKind, CastFact, CastKind, CastRequirement, FixupFacts, FunctionId, PathSegment,
};
use crate::rust_ast::{
    AtomicType, Block, Expr, IndentStmt, Item, Pattern, Prim, Program, RustValue, Stmt, Type,
    UnaryOp,
};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.casts.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut collector = Collector::new(function, facts);
        collector.enter_root_scope();
        collector.body(&f.body, &mut Vec::new(), false);
        all.extend(collector.casts);
    }
    facts.casts = all;
}

struct Collector<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    scopes: Vec<BTreeMap<String, Option<Type>>>,
    casts: Vec<CastFact>,
}

impl<'a> Collector<'a> {
    fn new(function: FunctionId, facts: &'a FixupFacts) -> Self {
        Self {
            function,
            facts,
            scopes: Vec::new(),
            casts: Vec::new(),
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
        self.facts
            .functions
            .iter()
            .find(|function| function.id == self.function)
            .and_then(|function| {
                self.facts
                    .call_signatures
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
        }
    }

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) -> Option<Type> {
        match expr {
            Expr::Cast { expr: inner, ty } => {
                let from = walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(inner, path)
                });
                self.record_cast(inner, from.clone(), ty.clone(), path);
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
            Expr::Call { func, args } => {
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
            Expr::AtomicRef { ptr, ty } | Expr::AtomicLoad { ptr, ty, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(ptr, path));
                Some(atomic_rust_type(*ty))
            }
            Expr::AtomicStore { ptr, value, .. }
            | Expr::AtomicFetch { ptr, value, .. }
            | Expr::AtomicSwap { ptr, value, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(ptr, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(value, path));
                None
            }
            Expr::AtomicCompareExchange {
                ptr,
                expected,
                desired,
                ..
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(ptr, path));
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

    fn record_cast(
        &mut self,
        cast_expr: &Expr,
        from: Option<Type>,
        to: Type,
        path: &[PathSegment],
    ) {
        let mut reasons = BTreeSet::new();
        let abi_required = self.cast_is_abi_pinned(path, &to);
        if abi_required {
            reasons.insert(CastRequirement::Abi);
        }
        let kind = classify_cast(cast_expr, from.as_ref(), &to, &mut reasons);
        let required = !reasons.is_empty();
        let removable_candidate = matches!(
            kind,
            CastKind::NoOp | CastKind::ReferenceCoercion | CastKind::SliceCoercion
        ) && !abi_required;
        self.casts.push(CastFact {
            function: self.function,
            path: AstPath(path.to_vec()),
            from,
            to,
            kind,
            required,
            reasons,
            removable_candidate,
        });
    }

    fn cast_is_abi_pinned(&self, path: &[PathSegment], to: &Type) -> bool {
        let path = AstPath(path.to_vec());
        self.facts
            .callsites
            .iter()
            .filter(|callsite| callsite.function == self.function)
            .flat_map(|callsite| callsite.args.iter())
            .any(|arg| {
                arg.path == path
                    && (arg
                        .declared_ty
                        .as_ref()
                        .is_some_and(|declared| same_type(declared, to))
                        || arg.variadic)
            })
    }

    fn call_ret_ty(&self, path: &[PathSegment]) -> Option<Type> {
        let path = AstPath(path.to_vec());
        self.facts
            .callsites
            .iter()
            .find(|callsite| callsite.function == self.function && callsite.path == path)
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
            Pattern::Wildcard | Pattern::I64(_) | Pattern::I128(_) => {}
        }
    }
}

fn classify_cast(
    expr: &Expr,
    from: Option<&Type>,
    to: &Type,
    reasons: &mut BTreeSet<CastRequirement>,
) -> CastKind {
    if is_numeric_literal(expr) && is_numeric_type(to) {
        reasons.insert(CastRequirement::Inference);
        return CastKind::LiteralInferenceGuard;
    }
    let Some(from) = from else {
        reasons.insert(CastRequirement::UnknownSource);
        return CastKind::Unknown;
    };
    if same_type(from, to) {
        return CastKind::NoOp;
    }
    match (from, to) {
        (Type::Prim(from), Type::Prim(to)) => classify_prim_cast(*from, *to, reasons),
        (Type::Ptr { .. }, Type::Ptr { .. })
        | (Type::Ptr { .. }, Type::Prim(_))
        | (Type::Prim(_), Type::Ptr { .. }) => {
            reasons.insert(CastRequirement::Semantics);
            CastKind::PointerCast
        }
        (Type::Ref { .. }, Type::Ref { .. }) => {
            reasons.insert(CastRequirement::RustCoercion);
            CastKind::ReferenceCoercion
        }
        (Type::Ref { inner, .. }, Type::Slice(_)) if matches!(&**inner, Type::Array { .. }) => {
            reasons.insert(CastRequirement::RustCoercion);
            CastKind::SliceCoercion
        }
        (Type::Array { .. }, Type::Slice(_)) => {
            reasons.insert(CastRequirement::RustCoercion);
            CastKind::SliceCoercion
        }
        _ => {
            reasons.insert(CastRequirement::Semantics);
            CastKind::Semantic
        }
    }
}

fn classify_prim_cast(from: Prim, to: Prim, reasons: &mut BTreeSet<CastRequirement>) -> CastKind {
    if from == to {
        return CastKind::NoOp;
    }
    if is_integer_prim(from) && is_integer_prim(to) {
        reasons.insert(CastRequirement::Semantics);
        return match (
            prim_bits(from) == prim_bits(to),
            prim_signed(from) == prim_signed(to),
        ) {
            (true, false) => CastKind::IntegerSignChange,
            (false, true) => CastKind::IntegerWidthChange,
            (false, false) => CastKind::IntegerSignAndWidthChange,
            (true, true) => CastKind::IntegerSameShape,
        };
    }
    if is_float_prim(from) && is_float_prim(to) {
        reasons.insert(CastRequirement::Semantics);
        return CastKind::FloatWidthChange;
    }
    if (is_integer_prim(from) && is_float_prim(to)) || (is_float_prim(from) && is_integer_prim(to))
    {
        reasons.insert(CastRequirement::Semantics);
        return CastKind::FloatInteger;
    }
    reasons.insert(CastRequirement::Semantics);
    CastKind::Semantic
}

fn literal_type(value: &RustValue) -> Option<Type> {
    match value {
        RustValue::I64(_) => Some(Type::Prim(Prim::I64)),
        RustValue::Usize(_) => Some(Type::Prim(Prim::Usize)),
        RustValue::I128(_) => Some(Type::Prim(Prim::I128)),
        RustValue::Bool(_) => Some(Type::Prim(Prim::Bool)),
        RustValue::Float(_) => Some(Type::Prim(Prim::F64)),
        RustValue::NullPtr | RustValue::None => None,
    }
}

fn is_numeric_literal(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Value(
            RustValue::I64(_) | RustValue::Usize(_) | RustValue::I128(_) | RustValue::Float(_),
        )
    )
}

fn same_type(lhs: &Type, rhs: &Type) -> bool {
    lhs.render() == rhs.render()
}

fn array_elem_ty(ty: Option<Type>) -> Option<Type> {
    match ty {
        Some(Type::Array { elem, .. }) | Some(Type::Slice(elem)) => Some(*elem),
        Some(Type::Ref { inner, .. }) => array_elem_ty(Some(*inner)),
        _ => None,
    }
}

fn is_numeric_type(ty: &Type) -> bool {
    matches!(ty, Type::Prim(prim) if is_integer_prim(*prim) || is_float_prim(*prim))
}

fn is_integer_prim(prim: Prim) -> bool {
    matches!(
        prim,
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
}

fn is_float_prim(prim: Prim) -> bool {
    matches!(prim, Prim::F32 | Prim::F64)
}

fn prim_signed(prim: Prim) -> Option<bool> {
    Some(match prim {
        Prim::I8 | Prim::I16 | Prim::I32 | Prim::I64 | Prim::I128 | Prim::Isize => true,
        Prim::U8 | Prim::U16 | Prim::U32 | Prim::U64 | Prim::U128 | Prim::Usize => false,
        _ => return None,
    })
}

fn prim_bits(prim: Prim) -> Option<u16> {
    Some(match prim {
        Prim::I8 | Prim::U8 => 8,
        Prim::I16 | Prim::U16 => 16,
        Prim::I32 | Prim::U32 | Prim::F32 => 32,
        Prim::I64 | Prim::U64 | Prim::F64 => 64,
        Prim::I128 | Prim::U128 => 128,
        Prim::Isize | Prim::Usize => usize::BITS as u16,
        _ => return None,
    })
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts::{CastKind, CastRequirement};
    use crate::fixups::test_support::*;
    use crate::rust_ast::{ExternDecl, ExternFnDecl, FnParam, RustValue};

    fn cast(expr: Expr, ty: &str) -> Expr {
        Expr::Cast {
            expr: Box::new(expr),
            ty: Type::parse(ty),
        }
    }

    fn facts_for(stmts: Vec<Stmt>) -> FixupFacts {
        crate::fixups::facts::analyze(Program {
            items: vec![Item::Fn(func(vec![param("x", "i32")], Some("i32"), stmts))],
        })
        .facts
    }

    #[test]
    fn records_noop_and_removable_candidate() {
        let facts = facts_for(vec![Stmt::Return(Some(cast(var("x"), "i32")))]);

        assert_eq!(facts.casts.len(), 1);
        assert_eq!(facts.casts[0].kind, CastKind::NoOp);
        assert_eq!(
            facts.casts[0].from.as_ref().map(Type::render),
            Some("i32".into())
        );
        assert!(!facts.casts[0].required);
        assert!(facts.casts[0].removable_candidate);
    }

    #[test]
    fn classifies_integer_sign_and_width_changes() {
        let facts = facts_for(vec![
            temp("u", "u32", cast(var("x"), "u32")),
            Stmt::Return(Some(cast(var("u"), "i64"))),
        ]);

        assert_eq!(facts.casts[0].kind, CastKind::IntegerSignChange);
        assert_eq!(facts.casts[1].kind, CastKind::IntegerSignAndWidthChange);
        assert!(facts.casts[0].reasons.contains(&CastRequirement::Semantics));
        assert!(facts.casts[1].reasons.contains(&CastRequirement::Semantics));
    }

    #[test]
    fn classifies_pointer_casts_as_semantic() {
        let ptr = Type::parse("*mut i32");
        let facts = crate::fixups::facts::analyze(Program {
            items: vec![Item::Fn(func(
                vec![FnParam {
                    name: "p".into(),
                    mutable: false,
                    ty: ptr,
                }],
                Some("i32"),
                vec![Stmt::Return(Some(cast(var("p"), "*const u8")))],
            ))],
        })
        .facts;

        assert_eq!(facts.casts[0].kind, CastKind::PointerCast);
        assert!(facts.casts[0].required);
        assert!(!facts.casts[0].removable_candidate);
    }

    #[test]
    fn records_literal_inference_guards() {
        let facts = facts_for(vec![Stmt::Return(Some(cast(
            Expr::Value(RustValue::I64(7)),
            "i32",
        )))]);

        assert_eq!(facts.casts[0].kind, CastKind::LiteralInferenceGuard);
        assert!(facts.casts[0].reasons.contains(&CastRequirement::Inference));
        assert!(facts.casts[0].required);
        assert!(!facts.casts[0].removable_candidate);
    }

    #[test]
    fn marks_declared_call_argument_casts_as_abi_required() {
        let program = Program {
            items: vec![
                Item::ExternBlock {
                    abi: "C".into(),
                    decls: vec![ExternDecl::Fn(ExternFnDecl {
                        name: "takes_i32".into(),
                        params: vec![FnParam {
                            name: "v".into(),
                            mutable: false,
                            ty: Type::Prim(Prim::I32),
                        }],
                        ret: None,
                        variadic: false,
                    })],
                },
                Item::Fn(func(
                    vec![param("x", "i64")],
                    Some("i32"),
                    vec![
                        Stmt::Expr(call("takes_i32", vec![cast(var("x"), "i32")])),
                        Stmt::Return(Some(int(0))),
                    ],
                )),
            ],
        };

        let facts = crate::fixups::facts::analyze(program).facts;

        assert_eq!(facts.casts[0].kind, CastKind::IntegerWidthChange);
        assert!(facts.casts[0].reasons.contains(&CastRequirement::Abi));
        assert!(facts.casts[0].required);
    }

    #[test]
    fn records_reference_and_slice_coercions_as_removable_candidates() {
        let facts = crate::fixups::facts::analyze(Program {
            items: vec![Item::Fn(func(
                vec![],
                Some("i32"),
                vec![
                    temp(
                        "a",
                        "[i32; 3]",
                        Expr::ArrayLit(vec![int(1), int(2), int(3)]),
                    ),
                    temp(
                        "s",
                        "[i32]",
                        cast(
                            Expr::Ref {
                                mutable: false,
                                expr: Box::new(var("a")),
                            },
                            "[i32]",
                        ),
                    ),
                    Stmt::Return(Some(int(0))),
                ],
            ))],
        })
        .facts;

        assert_eq!(facts.casts[0].kind, CastKind::SliceCoercion);
        assert!(
            facts.casts[0]
                .reasons
                .contains(&CastRequirement::RustCoercion)
        );
        assert!(facts.casts[0].removable_candidate);
    }

    #[test]
    fn rejects_bool_casts_as_semantic() {
        let facts = facts_for(vec![Stmt::Return(Some(cast(var("x"), "bool")))]);

        assert_eq!(facts.casts[0].kind, CastKind::Semantic);
        assert!(facts.casts[0].required);
        assert!(!facts.casts[0].removable_candidate);
    }

    #[test]
    fn classifies_variadic_casts_as_abi_pinned() {
        let program = Program {
            items: vec![
                Item::ExternBlock {
                    abi: "C".into(),
                    decls: vec![ExternDecl::Fn(ExternFnDecl {
                        name: "sink".into(),
                        params: vec![FnParam {
                            name: "fmt".into(),
                            mutable: false,
                            ty: Type::parse("*const i8"),
                        }],
                        ret: None,
                        variadic: true,
                    })],
                },
                Item::Fn(func(
                    vec![param("x", "i32")],
                    Some("i32"),
                    vec![
                        Stmt::Expr(call("sink", vec![var("fmt"), cast(var("x"), "i32")])),
                        Stmt::Return(Some(int(0))),
                    ],
                )),
            ],
        };

        let facts = crate::fixups::facts::analyze(program).facts;

        assert_eq!(facts.casts[0].kind, CastKind::NoOp);
        assert!(facts.casts[0].reasons.contains(&CastRequirement::Abi));
        assert!(facts.casts[0].required);
        assert!(!facts.casts[0].removable_candidate);
    }
}
