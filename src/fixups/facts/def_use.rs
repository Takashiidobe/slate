use std::collections::BTreeMap;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    self, AstPath, BindingFact, BindingId, BindingKind, DefUseFact, FunctionId, PathSegment,
};
use crate::rust_ast::{
    AsmOperand, AtomicPlace, Block, Expr, FnDef, Ident, IndentStmt, Pattern, Stmt, UnaryOp,
};
pub(in crate::fixups) fn collect_for_function<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
    bindings: &[BindingFact<'db>],
) -> Vec<DefUseFact<'db>> {
    let mut collector = Collector::new(function, bindings);
    collector.enter_root_scope();
    collector.body(&f.body, &mut Vec::new(), false);
    collector.finish()
}

struct Collector<'db, 'a> {
    function: FunctionId<'db>,
    bindings: &'a [BindingFact<'db>],
    scopes: Vec<BTreeMap<String, Option<BindingId<'db>>>>,
    by_binding: BTreeMap<BindingId<'db>, BindingSummary>,
}

#[derive(Default)]
struct BindingSummary {
    reads: Vec<AstPath>,
    writes: Vec<AstPath>,
}

impl<'db, 'a> Collector<'db, 'a> {
    fn new(function: FunctionId<'db>, bindings: &'a [BindingFact<'db>]) -> Self {
        Self {
            function,
            bindings,
            scopes: Vec::new(),
            by_binding: BTreeMap::new(),
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

    fn finish(self) -> Vec<DefUseFact<'db>> {
        self.bindings
            .iter()
            .filter(|binding| binding.function == self.function)
            .map(|binding| {
                let summary = self.by_binding.get(&binding.id);
                let reads = summary
                    .map(|summary| summary.reads.clone())
                    .unwrap_or_default();
                let writes = summary
                    .map(|summary| summary.writes.clone())
                    .unwrap_or_default();
                DefUseFact {
                    function: self.function,
                    binding: binding.id,
                    definition: binding.path.clone(),
                    last_use: reads.last().cloned(),
                    reads,
                    writes,
                }
            })
            .collect()
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
            Stmt::Assign { target, value } => {
                self.place(target, PlaceAccess::Write, path);
                self.expr(value, path);
            }
            Stmt::CompoundAssign { target, value, .. } => {
                self.place(target, PlaceAccess::ReadWrite, path);
                self.expr(value, path);
            }
            Stmt::InlineAsm(asm) => {
                for operand in &asm.operands {
                    match operand {
                        AsmOperand::In { value, .. } | AsmOperand::Const(value) => {
                            self.expr(value, path);
                        }
                        AsmOperand::Out { value, .. } => {
                            self.place(value, PlaceAccess::Write, path);
                        }
                        AsmOperand::InOut { input, output, .. } => {
                            self.expr(input, path);
                            self.place(output, PlaceAccess::Write, path);
                        }
                        AsmOperand::Label { state, value, .. } => {
                            self.place(state, PlaceAccess::Write, path);
                            self.expr(value, path);
                        }
                    }
                }
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
        }
    }

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) {
        match expr {
            Expr::Var(name) => self.record_read(name, path),
            Expr::Unary { expr, .. } | Expr::Cast { expr, .. } | Expr::Transmute { expr, .. } => {
                self.expr(expr, path)
            }
            Expr::Binary { lhs, rhs, .. } => {
                self.expr(lhs, path);
                self.expr(rhs, path);
            }
            Expr::Range { start, end } => {
                self.expr(start, path);
                self.expr(end, path);
            }
            Expr::Call { func, args, .. } => {
                self.expr(func, path);
                for arg in args {
                    self.expr(arg, path);
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                self.expr(recv, path);
                for arg in args {
                    self.expr(arg, path);
                }
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => self.expr(base, path),
            Expr::ArrayPtr { array, .. } => self.expr(array, path),
            Expr::Index { base, index } => {
                self.expr(base, path);
                self.expr(index, path);
            }
            Expr::StructLit { fields, .. } => {
                for (_, value) in fields {
                    self.expr(value, path);
                }
            }
            Expr::TupleStructLit { fields, .. } => {
                for value in fields {
                    self.expr(value, path);
                }
            }
            Expr::ArrayLit(elems) | Expr::VecLit(elems) | Expr::Macro { args: elems, .. } => {
                for elem in elems {
                    self.expr(elem, path);
                }
            }
            Expr::ArrayRepeat { elem, .. } => self.expr(elem, path),
            Expr::VecRepeat { elem, len } => {
                self.expr(elem, path);
                self.expr(len, path);
            }
            Expr::Closure { params, body } => {
                self.scopes.push(BTreeMap::new());
                for param in params {
                    self.bind(param.to_string(), None);
                }
                self.expr(body, path);
                self.scopes.pop();
            }
            Expr::Match { expr, arms } => {
                self.expr(expr, path);
                for arm in arms {
                    self.scopes.push(BTreeMap::new());
                    self.shadow_pattern(&arm.pattern);
                    self.expr(&arm.value, path);
                    self.scopes.pop();
                }
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                self.expr(cond, path);
                self.expr(then_expr, path);
                self.expr(else_expr, path);
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
            Expr::Ref { expr, .. } | Expr::AddrOf { expr, .. } => self.expr(expr, path),
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
                self.atomic_place(place, path)
            }
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                self.atomic_place(place, path);
                self.expr(value, path);
            }
            Expr::AtomicNew { value, .. } => self.expr(value, path),
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                self.atomic_place(place, path);
                self.expr(expected, path);
                self.expr(desired, path);
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                self.expr(src, path);
                self.expr(dst, path);
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                self.expr(src, path);
                self.expr(dst, path);
                self.expr(count, path);
            }
            Expr::WriteBytes { dst, val, count } => {
                self.expr(dst, path);
                self.expr(val, path);
                self.expr(count, path);
            }
            Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::CStr(_)
            | Expr::AtomicFence { .. }
            | Expr::Todo(_)
            | Expr::Path(_) => {}
        }
    }

    fn place(&mut self, expr: &Expr, access: PlaceAccess, path: &mut Vec<PathSegment>) {
        match expr {
            Expr::Var(name) => match access {
                PlaceAccess::Write => self.record_write(name, path),
                PlaceAccess::ReadWrite => {
                    self.record_read(name, path);
                    self.record_write(name, path);
                }
            },
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                self.place(base, access, path)
            }
            Expr::Index { base, index } => {
                self.place(base, access, path);
                self.expr(index, path);
            }
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => self.expr(expr, path),
            Expr::Ref { expr, .. } | Expr::AddrOf { expr, .. } | Expr::Cast { expr, .. } => {
                self.place(expr, access, path);
            }
            _ => self.expr(expr, path),
        }
    }

    fn define_local(&mut self, name: &str, path: &[PathSegment]) {
        if let Some(id) = self.local_binding(name, path) {
            self.bind(name.to_string(), Some(id));
        }
    }

    fn local_binding(&self, name: &str, path: &[PathSegment]) -> Option<BindingId<'db>> {
        facts::binding_by_local_path(self.bindings, self.function, name, &AstPath(path.to_vec()))
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

    fn bind(&mut self, name: String, binding: Option<BindingId<'db>>) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.insert(name, binding);
        }
    }

    // a promoted atomic local reads the binding; its contents change only
    // through the atomic op itself, never by reassigning the binding.
    fn atomic_place(&mut self, place: &AtomicPlace, path: &mut Vec<PathSegment>) {
        match place {
            AtomicPlace::Ptr(ptr) => self.expr(ptr, path),
            AtomicPlace::Local(name) => self.record_read(name, path),
        }
    }

    fn record_read(&mut self, name: &Ident, path: &[PathSegment]) {
        let Some(binding) = self.binding_for_name(name.as_str()) else {
            return;
        };
        self.by_binding
            .entry(binding)
            .or_default()
            .reads
            .push(AstPath(path.to_vec()));
    }

    fn record_write(&mut self, name: &Ident, path: &[PathSegment]) {
        let Some(binding) = self.binding_for_name(name.as_str()) else {
            return;
        };
        self.by_binding
            .entry(binding)
            .or_default()
            .writes
            .push(AstPath(path.to_vec()));
    }

    fn binding_for_name(&self, name: &str) -> Option<BindingId<'db>> {
        self.scopes
            .iter()
            .rev()
            .find_map(|scope| scope.get(name).copied())
            .flatten()
    }
}

#[derive(Clone, Copy)]
enum PlaceAccess {
    Write,
    ReadWrite,
}
