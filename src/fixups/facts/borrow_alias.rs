use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingFact, BindingId, BorrowAliasFact, BorrowAliasReason, BorrowAliasState,
    BorrowAliasUseFact, BorrowAliasUseKind, FunctionId, PathSegment,
};
use crate::rust_ast::{AsmOperand, Block, Expr, FnDef, Ident, IndentStmt, Stmt, UnaryOp};
pub(in crate::fixups) fn collect_for_function<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
    bindings: &[BindingFact<'db>],
) -> Vec<BorrowAliasFact<'db>> {
    let mut collector = Collector::new(function, bindings);
    collector.body(&f.body, &mut Vec::new());
    collector.finish()
}

struct Collector<'db, 'a> {
    function: FunctionId<'db>,
    bindings: &'a [BindingFact<'db>],
    by_binding: BTreeMap<BindingId<'db>, BindingSummary>,
}

#[derive(Default)]
struct BindingSummary {
    reasons: BTreeSet<BorrowAliasReason>,
    uses: Vec<BorrowAliasUseFact>,
}

impl<'db, 'a> Collector<'db, 'a> {
    fn new(function: FunctionId<'db>, bindings: &'a [BindingFact<'db>]) -> Self {
        Self {
            function,
            bindings,
            by_binding: BTreeMap::new(),
        }
    }

    fn finish(self) -> Vec<BorrowAliasFact<'db>> {
        self.bindings
            .iter()
            .filter(|binding| binding.function == self.function)
            .filter_map(|binding| {
                let summary = self.by_binding.get(&binding.id)?;
                Some(BorrowAliasFact {
                    function: self.function,
                    binding: binding.id,
                    state: state_for(&summary.reasons),
                    reasons: summary.reasons.clone(),
                    uses: summary.uses.clone(),
                })
            })
            .collect()
    }

    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>) {
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                self.stmt(&indent.stmt, path);
            });
        }
    }

    fn block(&mut self, block: &Block, path: &mut Vec<PathSegment>) {
        self.body(&block.stmts, path);
        if let Some(tail) = &block.tail {
            self.expr(tail, path);
        }
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let { init, .. } => {
                if let Some(init) = init {
                    self.expr(init, path);
                }
            }
            Stmt::LetIf {
                cond,
                then_body,
                then_value,
                else_body,
                else_value,
                ..
            } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.body(then_body, path);
                    self.expr(then_value, path);
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.body(else_body, path);
                    self.expr(else_value, path);
                });
            }
            Stmt::Assign { target, value } => {
                self.place(target, PlaceAccess::Assign, path);
                self.expr(value, path);
            }
            Stmt::CompoundAssign { target, value, .. } => {
                self.place(target, PlaceAccess::MutateProjection, path);
                self.expr(value, path);
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => self.expr(expr, path),
            Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::Then, |path| self.body(then_body, path));
                walk::with_path_segment(path, PathSegment::Else, |path| self.body(else_body, path));
            }
            Stmt::Loop { body, .. } => {
                walk::with_path_segment(path, PathSegment::LoopBody, |path| self.body(body, path));
            }
            Stmt::For { iter, body, .. } => {
                self.expr(iter, path);
                walk::with_path_segment(path, PathSegment::ForBody, |path| self.body(body, path));
            }
            Stmt::Scope { body } => {
                walk::with_path_segment(path, PathSegment::ScopeBody, |path| self.body(body, path));
            }
            Stmt::LabeledBlock { body, .. } => {
                walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                    self.body(body, path)
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
                        self.body(&arm.body, path)
                    });
                }
            }
            Stmt::InlineAsm(asm) => {
                for operand in &asm.operands {
                    match operand {
                        AsmOperand::In { value, .. } | AsmOperand::Const(value) => {
                            self.expr(value, path);
                        }
                        AsmOperand::Out { value, .. } => {
                            self.place(value, PlaceAccess::Assign, path);
                        }
                        AsmOperand::InOut { input, output, .. } => {
                            self.expr(input, path);
                            self.place(output, PlaceAccess::Assign, path);
                        }
                        AsmOperand::Label { state, value, .. } => {
                            self.place(state, PlaceAccess::Assign, path);
                            self.expr(value, path);
                        }
                    }
                }
            }
        }
    }

    fn expr(&mut self, expr: &Expr, path: &mut [PathSegment]) {
        match expr {
            Expr::Var(name) => self.record_name(
                name,
                BorrowAliasReason::Read,
                BorrowAliasUseKind::Read,
                path,
            ),
            Expr::Ref { mutable, expr } => {
                let (reason, kind) = if *mutable {
                    (
                        BorrowAliasReason::MutableBorrow,
                        BorrowAliasUseKind::MutableBorrow,
                    )
                } else {
                    (
                        BorrowAliasReason::SharedBorrow,
                        BorrowAliasUseKind::SharedBorrow,
                    )
                };
                self.record_expr_vars(expr, reason, kind, path);
                self.expr(expr, path);
            }
            Expr::AddrOf { expr, .. } => {
                self.record_expr_vars(
                    expr,
                    BorrowAliasReason::AddressTaken,
                    BorrowAliasUseKind::AddressTaken,
                    path,
                );
                self.expr(expr, path);
            }
            Expr::ArrayPtr { array, .. } => {
                self.record_expr_vars(
                    array,
                    BorrowAliasReason::RawPtrDerived,
                    BorrowAliasUseKind::RawPtrDerived,
                    path,
                );
                self.expr(array, path);
            }
            Expr::Call { func, args, .. } => {
                self.expr(func, path);
                for arg in args {
                    self.record_expr_vars(
                        arg,
                        BorrowAliasReason::UnknownCallEscape,
                        BorrowAliasUseKind::UnknownCallEscape,
                        path,
                    );
                    self.expr(arg, path);
                }
            }
            Expr::MethodCall { recv, method, args } if is_read_only_receiver_method(method) => {
                self.expr(recv, path);
                for arg in args {
                    self.expr(arg, path);
                }
            }
            Expr::MethodCallGeneric {
                recv, method, args, ..
            } if is_read_only_receiver_method(method) => {
                self.expr(recv, path);
                for arg in args {
                    self.expr(arg, path);
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                self.record_expr_vars(
                    recv,
                    BorrowAliasReason::UnknownCallEscape,
                    BorrowAliasUseKind::UnknownCallEscape,
                    path,
                );
                self.expr(recv, path);
                for arg in args {
                    self.record_expr_vars(
                        arg,
                        BorrowAliasReason::UnknownCallEscape,
                        BorrowAliasUseKind::UnknownCallEscape,
                        path,
                    );
                    self.expr(arg, path);
                }
            }
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    self.record_expr_vars(
                        ptr,
                        BorrowAliasReason::AtomicAccess,
                        BorrowAliasUseKind::AtomicAccess,
                        path,
                    );
                    self.expr(ptr, path);
                }
            }
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    self.record_expr_vars(
                        ptr,
                        BorrowAliasReason::AtomicAccess,
                        BorrowAliasUseKind::AtomicAccess,
                        path,
                    );
                    self.expr(ptr, path);
                }
                self.expr(value, path);
            }
            Expr::AtomicNew { value, .. } => self.expr(value, path),
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                if let Some(ptr) = place.ptr_expr() {
                    self.record_expr_vars(
                        ptr,
                        BorrowAliasReason::AtomicAccess,
                        BorrowAliasUseKind::AtomicAccess,
                        path,
                    );
                    self.expr(ptr, path);
                }
                self.expr(expected, path);
                self.expr(desired, path);
            }
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
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => self.expr(base, path),
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
            Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
                self.expr(elem, path)
            }
            Expr::VecRepeat { elem, len } => {
                self.expr(elem, path);
                self.expr(len, path);
            }
            Expr::Match { expr, arms } => {
                self.expr(expr, path);
                for arm in arms {
                    self.expr(&arm.value, path);
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
                let mut nested = path.to_vec();
                walk::with_path_segment(&mut nested, PathSegment::BlockBody, |path| {
                    self.block(block, path)
                });
            }
            Expr::Unsafe(block) => {
                let mut nested = path.to_vec();
                walk::with_path_segment(&mut nested, PathSegment::UnsafeBody, |path| {
                    self.block(block, path)
                });
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                self.record_expr_vars(
                    dst,
                    BorrowAliasReason::UnknownCallEscape,
                    BorrowAliasUseKind::UnknownCallEscape,
                    path,
                );
                self.expr(src, path);
                self.expr(dst, path);
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                self.record_expr_vars(
                    dst,
                    BorrowAliasReason::UnknownCallEscape,
                    BorrowAliasUseKind::UnknownCallEscape,
                    path,
                );
                self.expr(src, path);
                self.expr(dst, path);
                self.expr(count, path);
            }
            Expr::WriteBytes { dst, val, count } => {
                self.record_expr_vars(
                    dst,
                    BorrowAliasReason::UnknownCallEscape,
                    BorrowAliasUseKind::UnknownCallEscape,
                    path,
                );
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

    fn place(&mut self, expr: &Expr, access: PlaceAccess, path: &mut [PathSegment]) {
        match expr {
            Expr::Var(name) => {
                let (reason, kind) = match access {
                    PlaceAccess::Assign => {
                        (BorrowAliasReason::Assigned, BorrowAliasUseKind::Assigned)
                    }
                    PlaceAccess::MutateProjection => (
                        BorrowAliasReason::MutatedProjection,
                        BorrowAliasUseKind::MutatedProjection,
                    ),
                };
                self.record_name(name, reason, kind, path);
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                self.place(base, PlaceAccess::MutateProjection, path);
            }
            Expr::Index { base, index } => {
                self.place(base, PlaceAccess::MutateProjection, path);
                self.expr(index, path);
            }
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => self.place(expr, PlaceAccess::MutateProjection, path),
            Expr::Ref { expr, .. } | Expr::AddrOf { expr, .. } | Expr::Cast { expr, .. } => {
                self.place(expr, access, path);
            }
            _ => self.expr(expr, path),
        }
    }

    fn record_expr_vars(
        &mut self,
        expr: &Expr,
        reason: BorrowAliasReason,
        kind: BorrowAliasUseKind,
        path: &[PathSegment],
    ) {
        walk::exprs(expr, &mut |expr| {
            if let Expr::Var(name) = expr {
                self.record_name(name, reason, kind, path);
            }
        });
    }

    fn record_name(
        &mut self,
        name: &Ident,
        reason: BorrowAliasReason,
        kind: BorrowAliasUseKind,
        path: &[PathSegment],
    ) {
        let Some(binding) = self.binding_for_name(name.as_str()) else {
            return;
        };
        let summary = self.by_binding.entry(binding).or_default();
        summary.reasons.insert(reason);
        summary.uses.push(BorrowAliasUseFact {
            kind,
            path: AstPath(path.to_vec()),
        });
    }

    fn binding_for_name(&self, name: &str) -> Option<BindingId<'db>> {
        self.bindings
            .iter()
            .find(|binding| binding.function == self.function && binding.name == name)
            .map(|binding| binding.id)
    }
}

fn is_read_only_receiver_method(method: &str) -> bool {
    matches!(
        method,
        "len" | "is_empty" | "iter" | "as_slice" | "as_ptr" | "first" | "last" | "get"
    )
}

#[derive(Clone, Copy)]
enum PlaceAccess {
    Assign,
    MutateProjection,
}

fn state_for(reasons: &BTreeSet<BorrowAliasReason>) -> BorrowAliasState {
    if reasons.iter().any(|reason| {
        matches!(
            reason,
            BorrowAliasReason::MutableBorrow
                | BorrowAliasReason::AddressTaken
                | BorrowAliasReason::RawPtrDerived
                | BorrowAliasReason::UnknownCallEscape
                | BorrowAliasReason::AtomicAccess
        )
    }) {
        BorrowAliasState::Escaped
    } else if reasons.iter().any(|reason| {
        matches!(
            reason,
            BorrowAliasReason::Assigned | BorrowAliasReason::MutatedProjection
        )
    }) {
        BorrowAliasState::UniqueMutation
    } else {
        BorrowAliasState::ReadOnly
    }
}
