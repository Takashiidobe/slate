use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::{
    AstPath, BindingId, BindingKind, FixupFacts, FunctionId, NulTermination, PathSegment,
    StringBufferFact, StringBufferKind, StringBufferProvenance, StringBufferRejection,
    StringLibcFunction, StringLibcUseFact, StringPointerViewFact, StringPointerViewKind,
    StringRecoveryCandidate,
};
use crate::fixups::support::walk;
use crate::rust_ast::{
    Block, Expr, IndentStmt, Item, Pattern, Prim, Program, RustValue, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.string_buffers.clear();
    facts.string_pointer_views.clear();
    facts.string_libc_uses.clear();

    let mut buffers = Vec::new();
    let mut pointer_views = Vec::new();
    let mut libc_uses = Vec::new();
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
        let collected = collector.finish();
        buffers.extend(collected.buffers);
        pointer_views.extend(collected.pointer_views);
        libc_uses.extend(collected.libc_uses);
    }

    facts.string_buffers = buffers;
    facts.string_pointer_views = pointer_views;
    facts.string_libc_uses = libc_uses;
}

struct Collected {
    buffers: Vec<StringBufferFact>,
    pointer_views: Vec<StringPointerViewFact>,
    libc_uses: Vec<StringLibcUseFact>,
}

struct Collector<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    scopes: Vec<BTreeMap<String, Option<BindingId>>>,
    summaries: BTreeMap<BindingId, BufferSummary>,
    pointer_views: Vec<StringPointerViewFact>,
    libc_uses: Vec<StringLibcUseFact>,
}

#[derive(Clone)]
struct BufferSummary {
    binding: BindingId,
    path: AstPath,
    kind: StringBufferKind,
    provenance: StringBufferProvenance,
    bytes: Option<Vec<u8>>,
    nul_termination: NulTermination,
    interior_nul: bool,
    rejections: BTreeSet<StringBufferRejection>,
}

struct LiteralBytes {
    bytes: Vec<u8>,
    nul_termination: NulTermination,
    interior_nul: bool,
}

impl<'a> Collector<'a> {
    fn new(function: FunctionId, facts: &'a FixupFacts) -> Self {
        Self {
            function,
            facts,
            scopes: Vec::new(),
            summaries: BTreeMap::new(),
            pointer_views: Vec::new(),
            libc_uses: Vec::new(),
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

    fn finish(self) -> Collected {
        Collected {
            buffers: self
                .summaries
                .into_values()
                .map(|summary| summary.into_fact(self.function))
                .collect(),
            pointer_views: self.pointer_views,
            libc_uses: self.libc_uses,
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
            Stmt::Let { name, ty, init, .. } => {
                if let Some(init) = init {
                    self.expr(init, path);
                }
                self.define_local(name, ty.as_ref(), init.as_ref(), path);
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
                self.define_local(name, ty.as_ref(), None, path);
            }
            Stmt::Assign { target, value } => {
                self.expr(value, path);
                self.assign(target, value, path);
            }
            Stmt::CompoundAssign { target, value, .. } => {
                self.expr(value, path);
                self.mutate_target(target, path);
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
        }
    }

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) {
        if let Some((source, kind, mutable)) = self.pointer_view(expr)
            && let Some(binding) = self.binding_for_name(source)
        {
            self.pointer_views.push(StringPointerViewFact {
                function: self.function,
                source: binding,
                path: AstPath(path.to_vec()),
                mutable,
                kind,
            });
        }
        if let Some(callee) = libc_function(expr) {
            let pointer_args = libc_pointer_args(expr)
                .into_iter()
                .filter_map(|source| self.binding_for_name(source))
                .collect();
            self.libc_uses.push(StringLibcUseFact {
                function: self.function,
                callee,
                path: AstPath(path.to_vec()),
                pointer_args,
            });
        }

        match expr {
            Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::Var(_)
            | Expr::Path(_)
            | Expr::Todo(_)
            | Expr::AtomicFence { .. } => {}
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path)),
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
            Expr::Call { func, args } => {
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
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(base, path));
            }
            Expr::ArrayPtr { array, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(array, path));
            }
            Expr::Index { base, index } => {
                if let Expr::Var(name) = &**base
                    && let Some(binding) = self.binding_for_name(name.as_str())
                    && let Some(summary) = self.summaries.get_mut(&binding)
                {
                    summary.rejections.insert(StringBufferRejection::Indexed);
                }
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
            Expr::ArrayLit(elems) | Expr::Macro { args: elems, .. } => {
                for (index, elem) in elems.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(elem, path)
                    });
                }
            }
            Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(elem, path));
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
            Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(ptr, path));
            }
            Expr::AtomicStore { ptr, value, .. }
            | Expr::AtomicFetch { ptr, value, .. }
            | Expr::AtomicSwap { ptr, value, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(ptr, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(value, path));
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

    fn define_local(
        &mut self,
        name: &str,
        ty: Option<&Type>,
        init: Option<&Expr>,
        path: &[PathSegment],
    ) {
        let binding =
            self.facts
                .binding_by_local_path(self.function, name, &AstPath(path.to_vec()));
        self.bind(name.to_string(), binding);
        let Some(binding) = binding else {
            return;
        };
        let Some(ty) = ty else {
            return;
        };
        let Some(summary) = self.summary_for_binding(binding, ty, init, AstPath(path.to_vec()))
        else {
            return;
        };
        self.summaries.insert(binding, summary);
    }

    fn summary_for_binding(
        &self,
        binding: BindingId,
        ty: &Type,
        init: Option<&Expr>,
        path: AstPath,
    ) -> Option<BufferSummary> {
        if is_char_array(ty) {
            let mut summary = BufferSummary::new(binding, path, StringBufferKind::CharArray);
            match init {
                Some(expr) if is_zero_array(expr) => {
                    summary.provenance = StringBufferProvenance::ZeroInitialized;
                    summary.nul_termination = NulTermination::AllZero;
                    summary.bytes = Some(Vec::new());
                }
                Some(expr) => match literal_bytes(expr) {
                    Some(literal) => {
                        summary.apply_literal(literal, StringBufferProvenance::Literal);
                    }
                    None => {
                        summary
                            .rejections
                            .insert(StringBufferRejection::UnsupportedInitializer);
                    }
                },
                None => {
                    summary.provenance = StringBufferProvenance::Unknown;
                }
            }
            return Some(summary);
        }

        let kind = lifted_kind(ty)?;
        let mut summary = BufferSummary::new(binding, path, kind);
        summary.provenance = StringBufferProvenance::Lifted;
        summary.nul_termination = NulTermination::NotApplicable;
        if let Some(init) = init {
            match init {
                Expr::Str(s) => {
                    summary.bytes = Some(s.as_bytes().to_vec());
                    summary.interior_nul = s.as_bytes().contains(&0);
                }
                Expr::ByteStr(bytes) => {
                    summary.bytes = Some(bytes.clone());
                    summary.interior_nul = bytes.contains(&0);
                }
                _ => {}
            }
        }
        Some(summary)
    }

    fn assign(&mut self, target: &Expr, value: &Expr, path: &mut Vec<PathSegment>) {
        match target {
            Expr::Var(name) => {
                let Some(binding) = self.binding_for_name(name.as_str()) else {
                    return;
                };
                let Some(summary) = self.summaries.get_mut(&binding) else {
                    return;
                };
                if summary.kind != StringBufferKind::CharArray {
                    summary.rejections.insert(StringBufferRejection::Mutated);
                    return;
                }
                if !matches!(
                    summary.provenance,
                    StringBufferProvenance::ZeroInitialized | StringBufferProvenance::Unknown
                ) {
                    summary.rejections.insert(StringBufferRejection::Mutated);
                    return;
                }
                match literal_bytes(value) {
                    Some(literal) => {
                        summary.apply_literal(
                            literal,
                            StringBufferProvenance::AssignedLiteral {
                                assignment: AstPath(path.to_vec()),
                            },
                        );
                    }
                    None => {
                        summary.rejections.insert(StringBufferRejection::Mutated);
                    }
                }
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                self.assign(base, value, path)
            }
            Expr::Index { base, index } => {
                self.mutate_target(base, path);
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(index, path));
            }
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path)),
            Expr::Cast { expr, .. } => self.assign(expr, value, path),
            _ => self.expr(target, path),
        }
    }

    fn mutate_target(&mut self, target: &Expr, path: &mut Vec<PathSegment>) {
        match target {
            Expr::Var(name) => {
                if let Some(binding) = self.binding_for_name(name.as_str())
                    && let Some(summary) = self.summaries.get_mut(&binding)
                {
                    summary.rejections.insert(StringBufferRejection::Mutated);
                }
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                self.mutate_target(base, path)
            }
            Expr::Index { base, index } => {
                self.mutate_target(base, path);
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(index, path));
            }
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path)),
            Expr::Cast { expr, .. } => self.mutate_target(expr, path),
            _ => self.expr(target, path),
        }
    }

    fn pointer_view<'b>(&self, expr: &'b Expr) -> Option<(&'b str, StringPointerViewKind, bool)> {
        match expr {
            Expr::MethodCall { recv, method, args } if args.is_empty() => {
                let source = var_name(recv)?;
                match method.as_str() {
                    "as_ptr" => Some((source, StringPointerViewKind::AsPtr, false)),
                    "as_mut_ptr" => Some((source, StringPointerViewKind::AsMutPtr, true)),
                    _ => None,
                }
            }
            Expr::ArrayPtr { array, mutable } => {
                Some((var_name(array)?, StringPointerViewKind::ArrayPtr, *mutable))
            }
            Expr::Cast { expr, .. }
            | Expr::Unary { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => self.pointer_view(expr),
            _ => None,
        }
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

    fn binding_for_name(&self, name: &str) -> Option<BindingId> {
        self.scopes
            .iter()
            .rev()
            .find_map(|scope| scope.get(name).copied())
            .flatten()
    }
}

impl BufferSummary {
    fn new(binding: BindingId, path: AstPath, kind: StringBufferKind) -> Self {
        Self {
            binding,
            path,
            kind,
            provenance: StringBufferProvenance::Unknown,
            bytes: None,
            nul_termination: NulTermination::NotApplicable,
            interior_nul: false,
            rejections: BTreeSet::new(),
        }
    }

    fn apply_literal(&mut self, literal: LiteralBytes, provenance: StringBufferProvenance) {
        self.provenance = provenance;
        self.bytes = Some(literal.bytes);
        self.nul_termination = literal.nul_termination;
        self.interior_nul = literal.interior_nul;
        if self.nul_termination == NulTermination::Unterminated {
            self.rejections.insert(StringBufferRejection::Unterminated);
        }
    }

    fn into_fact(self, function: FunctionId) -> StringBufferFact {
        let candidates = self.candidates();
        StringBufferFact {
            function,
            binding: self.binding,
            path: self.path,
            kind: self.kind,
            provenance: self.provenance,
            bytes: self.bytes,
            nul_termination: self.nul_termination,
            interior_nul: self.interior_nul,
            candidates,
            rejections: self.rejections,
        }
    }

    fn candidates(&self) -> BTreeSet<StringRecoveryCandidate> {
        if !self.rejections.is_empty() {
            return BTreeSet::new();
        }
        match self.kind {
            StringBufferKind::BorrowedStr => BTreeSet::from([StringRecoveryCandidate::BorrowedStr]),
            StringBufferKind::BorrowedBytes => {
                BTreeSet::from([StringRecoveryCandidate::BorrowedBytes])
            }
            StringBufferKind::OwnedString => BTreeSet::from([StringRecoveryCandidate::OwnedString]),
            StringBufferKind::CharArray => match self.provenance {
                StringBufferProvenance::ZeroInitialized => {
                    BTreeSet::from([StringRecoveryCandidate::OwnedString])
                }
                StringBufferProvenance::Literal
                | StringBufferProvenance::AssignedLiteral { .. }
                    if matches!(
                        self.nul_termination,
                        NulTermination::Terminated | NulTermination::AllZero
                    ) =>
                {
                    match &self.bytes {
                        Some(bytes) if !self.interior_nul && std::str::from_utf8(bytes).is_ok() => {
                            BTreeSet::from([
                                StringRecoveryCandidate::BorrowedStr,
                                StringRecoveryCandidate::OwnedString,
                            ])
                        }
                        Some(_) => BTreeSet::from([StringRecoveryCandidate::BorrowedBytes]),
                        None => BTreeSet::new(),
                    }
                }
                _ => BTreeSet::new(),
            },
        }
    }
}

fn is_char_array(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Array { elem, .. } if matches!(&**elem, Type::Prim(Prim::I8 | Prim::U8))
    )
}

fn lifted_kind(ty: &Type) -> Option<StringBufferKind> {
    match ty {
        Type::Custom(name) if name == "String" => Some(StringBufferKind::OwnedString),
        Type::Ref {
            mutable: false,
            inner,
        } => match &**inner {
            Type::Str => Some(StringBufferKind::BorrowedStr),
            Type::Slice(elem) if matches!(&**elem, Type::Prim(Prim::U8)) => {
                Some(StringBufferKind::BorrowedBytes)
            }
            _ => None,
        },
        _ => None,
    }
}

fn is_zero_array(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::ArrayRepeat { elem, .. }
            if matches!(&**elem, Expr::Value(RustValue::I64(0) | RustValue::I128(0)))
    )
}

fn literal_bytes(expr: &Expr) -> Option<LiteralBytes> {
    match expr {
        Expr::ArrayLit(elems) => {
            let bytes = elems.iter().map(byte_literal).collect::<Option<Vec<_>>>()?;
            Some(classify_bytes(bytes))
        }
        Expr::ByteStr(bytes) => Some(classify_bytes(bytes.clone())),
        Expr::Str(s) => Some(LiteralBytes {
            bytes: s.as_bytes().to_vec(),
            nul_termination: NulTermination::NotApplicable,
            interior_nul: s.as_bytes().contains(&0),
        }),
        Expr::Cast { expr, .. } => literal_bytes(expr),
        _ => None,
    }
}

fn classify_bytes(mut bytes: Vec<u8>) -> LiteralBytes {
    let Some(nul) = bytes.iter().position(|byte| *byte == 0) else {
        return LiteralBytes {
            bytes,
            nul_termination: NulTermination::Unterminated,
            interior_nul: false,
        };
    };
    let all_after_nul = bytes[nul..].iter().all(|byte| *byte == 0);
    let all_zero = bytes.iter().all(|byte| *byte == 0);
    let interior_nul = !all_after_nul;
    let payload = if all_after_nul {
        bytes.truncate(nul);
        bytes
    } else {
        bytes
    };
    LiteralBytes {
        bytes: payload,
        nul_termination: if all_zero {
            NulTermination::AllZero
        } else {
            NulTermination::Terminated
        },
        interior_nul,
    }
}

fn byte_literal(expr: &Expr) -> Option<u8> {
    let n = match expr {
        Expr::Value(RustValue::I64(n)) => *n,
        Expr::Value(RustValue::I128(n)) => i64::try_from(*n).ok()?,
        Expr::Cast { expr, .. } => return byte_literal(expr),
        _ => return None,
    };
    u8::try_from(n).ok()
}

fn var_name(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => Some(name.as_str()),
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => var_name(expr),
        _ => None,
    }
}

fn libc_function(expr: &Expr) -> Option<StringLibcFunction> {
    let Expr::Call { func, .. } = expr else {
        return None;
    };
    let Expr::Var(name) = &**func else {
        return None;
    };
    Some(match name.as_str() {
        "strlen" => StringLibcFunction::StrLen,
        "strcmp" => StringLibcFunction::StrCmp,
        "strncmp" => StringLibcFunction::StrNCmp,
        "memcmp" => StringLibcFunction::MemCmp,
        "strcpy" => StringLibcFunction::StrCpy,
        "strncpy" => StringLibcFunction::StrNCpy,
        "strcat" => StringLibcFunction::StrCat,
        "strncat" => StringLibcFunction::StrNCat,
        "printf" => StringLibcFunction::Printf,
        _ => return None,
    })
}

fn libc_pointer_args(expr: &Expr) -> Vec<&str> {
    let Expr::Call { args, .. } = expr else {
        return Vec::new();
    };
    args.iter().filter_map(pointer_source).collect()
}

fn pointer_source(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => Some(name.as_str()),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            pointer_source(recv)
        }
        Expr::ArrayPtr { array, .. } => pointer_source(array),
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => pointer_source(expr),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, Expr, Item, Program, Stmt};

    fn analyzed(stmts: Vec<Stmt>) -> facts::FixupFacts {
        facts::analyze(Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        })
        .facts
    }

    fn bytes(values: &[i64]) -> Expr {
        Expr::ArrayLit(values.iter().copied().map(int).collect())
    }

    fn binding_for(facts: &facts::FixupFacts, name: &str, path: AstPath) -> BindingId {
        facts
            .bindings
            .iter()
            .find(|binding| binding.name == name && binding.path == path)
            .unwrap()
            .id
    }

    fn buffer_for(facts: &facts::FixupFacts, binding: BindingId) -> &StringBufferFact {
        facts
            .string_buffers
            .iter()
            .find(|fact| fact.binding == binding)
            .unwrap()
    }

    #[test]
    fn records_literal_and_zero_initialized_char_buffers() {
        let facts = analyzed(vec![
            let_mut("literal", "[i8; 3]", bytes(&[104, 105, 0])),
            let_mut(
                "zero",
                "[i8; 4]",
                Expr::ArrayRepeat {
                    elem: Box::new(int(0)),
                    len: 4,
                },
            ),
        ]);
        let literal = binding_for(&facts, "literal", AstPath(vec![PathSegment::Stmt(0)]));
        let zero = binding_for(&facts, "zero", AstPath(vec![PathSegment::Stmt(1)]));

        let literal = buffer_for(&facts, literal);
        assert_eq!(literal.provenance, StringBufferProvenance::Literal);
        assert_eq!(literal.bytes, Some(b"hi".to_vec()));
        assert_eq!(literal.nul_termination, NulTermination::Terminated);
        assert!(
            literal
                .candidates
                .contains(&StringRecoveryCandidate::BorrowedStr)
        );
        assert!(
            literal
                .candidates
                .contains(&StringRecoveryCandidate::OwnedString)
        );

        let zero = buffer_for(&facts, zero);
        assert_eq!(zero.provenance, StringBufferProvenance::ZeroInitialized);
        assert_eq!(zero.nul_termination, NulTermination::AllZero);
        assert!(
            zero.candidates
                .contains(&StringRecoveryCandidate::OwnedString)
        );
    }

    #[test]
    fn records_assigned_c_strings_and_interior_nul_bytes() {
        let facts = analyzed(vec![
            let_mut(
                "s",
                "[i8; 5]",
                Expr::ArrayRepeat {
                    elem: Box::new(int(0)),
                    len: 5,
                },
            ),
            assign("s", bytes(&[97, 0, 98, 0, 0])),
        ]);
        let s = binding_for(&facts, "s", AstPath(vec![PathSegment::Stmt(0)]));
        let fact = buffer_for(&facts, s);

        assert_eq!(
            fact.provenance,
            StringBufferProvenance::AssignedLiteral {
                assignment: AstPath(vec![PathSegment::Stmt(1)])
            }
        );
        assert!(fact.interior_nul);
        assert!(
            fact.candidates
                .contains(&StringRecoveryCandidate::BorrowedBytes)
        );
        assert!(
            !fact
                .candidates
                .contains(&StringRecoveryCandidate::BorrowedStr)
        );
    }

    #[test]
    fn records_lifted_borrowed_and_owned_string_values() {
        let facts = analyzed(vec![
            temp("s", "&str", Expr::Str("hi".into())),
            temp("b", "&[u8]", Expr::ByteStr(b"hi".to_vec())),
            temp("owned", "String", Expr::Str("hi".into())),
        ]);

        let s = buffer_for(
            &facts,
            binding_for(&facts, "s", AstPath(vec![PathSegment::Stmt(0)])),
        );
        assert_eq!(s.kind, StringBufferKind::BorrowedStr);
        assert!(s.candidates.contains(&StringRecoveryCandidate::BorrowedStr));

        let b = buffer_for(
            &facts,
            binding_for(&facts, "b", AstPath(vec![PathSegment::Stmt(1)])),
        );
        assert_eq!(b.kind, StringBufferKind::BorrowedBytes);
        assert!(
            b.candidates
                .contains(&StringRecoveryCandidate::BorrowedBytes)
        );

        let owned = buffer_for(
            &facts,
            binding_for(&facts, "owned", AstPath(vec![PathSegment::Stmt(2)])),
        );
        assert_eq!(owned.kind, StringBufferKind::OwnedString);
        assert!(
            owned
                .candidates
                .contains(&StringRecoveryCandidate::OwnedString)
        );
    }

    #[test]
    fn records_pointer_views_and_libc_use_paths() {
        let facts = analyzed(vec![
            let_mut("s", "[i8; 3]", bytes(&[104, 105, 0])),
            Stmt::Expr(Expr::Unsafe(Box::new(Block {
                stmts: vec![],
                tail: Some(Box::new(call(
                    "printf",
                    vec![
                        Expr::ByteStr(b"%s\n\0".to_vec()),
                        Expr::ArrayPtr {
                            array: Box::new(var("s")),
                            mutable: true,
                        },
                    ],
                ))),
            }))),
        ]);
        let s = binding_for(&facts, "s", AstPath(vec![PathSegment::Stmt(0)]));

        let pointer = facts
            .string_pointer_views
            .iter()
            .find(|fact| fact.source == s)
            .unwrap();
        assert_eq!(pointer.kind, StringPointerViewKind::ArrayPtr);
        assert!(pointer.mutable);
        assert_eq!(
            pointer.path,
            AstPath(vec![
                PathSegment::Stmt(1),
                PathSegment::UnsafeBody,
                PathSegment::BlockTail,
                PathSegment::Expr(2)
            ])
        );

        let libc = facts
            .string_libc_uses
            .iter()
            .find(|fact| fact.callee == StringLibcFunction::Printf)
            .unwrap();
        assert_eq!(
            libc.path,
            AstPath(vec![
                PathSegment::Stmt(1),
                PathSegment::UnsafeBody,
                PathSegment::BlockTail
            ])
        );
        assert_eq!(libc.pointer_args, vec![s]);
    }

    #[test]
    fn rejects_indexed_mutated_and_unterminated_buffers() {
        let facts = analyzed(vec![
            let_mut("indexed", "[i8; 3]", bytes(&[104, 105, 0])),
            Stmt::Expr(Expr::Index {
                base: Box::new(var("indexed")),
                index: Box::new(int(0)),
            }),
            let_mut("mutated", "[i8; 3]", bytes(&[104, 105, 0])),
            assign("mutated", bytes(&[98, 121, 0])),
            let_mut("unterminated", "[i8; 2]", bytes(&[104, 105])),
        ]);

        let indexed = buffer_for(
            &facts,
            binding_for(&facts, "indexed", AstPath(vec![PathSegment::Stmt(0)])),
        );
        assert!(indexed.rejections.contains(&StringBufferRejection::Indexed));
        assert!(indexed.candidates.is_empty());

        let mutated = buffer_for(
            &facts,
            binding_for(&facts, "mutated", AstPath(vec![PathSegment::Stmt(2)])),
        );
        assert!(mutated.rejections.contains(&StringBufferRejection::Mutated));
        assert!(mutated.candidates.is_empty());

        let unterminated = buffer_for(
            &facts,
            binding_for(&facts, "unterminated", AstPath(vec![PathSegment::Stmt(4)])),
        );
        assert_eq!(unterminated.nul_termination, NulTermination::Unterminated);
        assert!(
            unterminated
                .rejections
                .contains(&StringBufferRejection::Unterminated)
        );
    }
}
