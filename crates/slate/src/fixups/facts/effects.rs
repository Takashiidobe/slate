use std::collections::BTreeSet;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, EffectFact, EffectKind, EffectSubject, FunctionId, PathSegment, Purity, Site,
};
use crate::function_identity::{Known, known_call};
use crate::rust_ast::{Block, Expr, FnDef, IndentStmt, Stmt, UnaryOp};
/// Effects for one function's body, independent of any other function's
/// facts - the entry point `slate-04q.75.56.8` (incremental facts) needs to
/// re-derive one function's effects without a whole-program walk.
pub(in crate::fixups) fn collect_for_function<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
) -> Vec<EffectFact<'db>> {
    let mut collector = Collector {
        function,
        effects: Vec::new(),
    };
    collector.body(&f.body, &mut Vec::new());
    collector.effects
}

pub(in crate::fixups) fn is_movable_pure_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) | Expr::Var(_) => true,
        Expr::Cast { expr, .. } => is_movable_pure_expr(expr),
        Expr::Unary { op, expr } => !matches!(op, UnaryOp::Not) && is_movable_pure_expr(expr),
        Expr::Binary { lhs, rhs, .. } => is_movable_pure_expr(lhs) && is_movable_pure_expr(rhs),
        Expr::MethodCall { recv, method, args } if is_pure_primitive_bit_method(method) => {
            is_movable_pure_bit_operand(recv) && args.iter().all(is_movable_pure_expr)
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => is_movable_pure_expr(base),
        Expr::Index { base, index } => is_movable_pure_expr(base) && is_movable_pure_expr(index),
        Expr::StructLit { fields, .. } => {
            fields.iter().all(|(_, value)| is_movable_pure_expr(value))
        }
        Expr::TupleStructLit { fields, .. } => fields.iter().all(is_movable_pure_expr),
        Expr::ArrayLit(elems) => elems.iter().all(is_movable_pure_expr),
        Expr::ArrayRepeat { elem, .. } => is_movable_pure_expr(elem),
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            block.tail.as_deref().is_some_and(is_movable_pure_expr)
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            is_movable_pure_expr(cond)
                && is_movable_pure_expr(then_expr)
                && is_movable_pure_expr(else_expr)
        }
        _ => false,
    }
}

fn is_movable_pure_bit_operand(expr: &Expr) -> bool {
    match expr {
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => is_movable_pure_bit_operand(expr),
        Expr::Cast { expr, .. } => is_movable_pure_bit_operand(expr),
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            is_movable_pure_expr(cond)
                && is_movable_pure_bit_operand(then_expr)
                && is_movable_pure_bit_operand(else_expr)
        }
        _ => is_movable_pure_expr(expr),
    }
}

fn is_pure_primitive_bit_method(method: &str) -> bool {
    matches!(
        method,
        "reverse_bits"
            | "swap_bytes"
            | "leading_zeros"
            | "trailing_zeros"
            | "count_ones"
            | "rotate_left"
            | "rotate_right"
    )
}

struct Collector<'db> {
    function: FunctionId<'db>,
    effects: Vec<EffectFact<'db>>,
}

impl<'db> Collector<'db> {
    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>) -> BTreeSet<EffectKind> {
        let mut effects = BTreeSet::new();
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                effects.extend(self.stmt(&indent.stmt, path));
            });
        }
        effects
    }

    fn block(&mut self, block: &Block, path: &mut Vec<PathSegment>) -> BTreeSet<EffectKind> {
        let mut effects = self.body(&block.stmts, path);
        if let Some(tail) = &block.tail {
            walk::with_path_segment(path, PathSegment::BlockTail, |path| {
                effects.extend(self.expr(tail, path));
            });
        }
        effects
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>) -> BTreeSet<EffectKind> {
        let mut effects = BTreeSet::new();
        match stmt {
            Stmt::Let { init, .. } => {
                if let Some(init) = init {
                    effects.extend(self.root_expr(init, path, 0));
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
                effects.extend(self.root_expr(cond, path, 0));
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    effects.extend(self.body(then_body, path));
                    effects.extend(self.root_expr(then_value, path, 0));
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    effects.extend(self.body(else_body, path));
                    effects.extend(self.root_expr(else_value, path, 0));
                });
            }
            Stmt::Assign { target, value } => {
                effects.insert(EffectKind::MemoryWrite);
                effects.extend(self.root_expr(target, path, 0));
                effects.extend(self.root_expr(value, path, 1));
            }
            Stmt::CompoundAssign { target, value, .. } => {
                effects.insert(EffectKind::MemoryWrite);
                effects.extend(self.root_expr(target, path, 0));
                effects.extend(self.root_expr(value, path, 1));
            }
            Stmt::InlineAsm(asm) => {
                effects.insert(EffectKind::MacroExpansion);
                let mut index = 0;
                for operand in &asm.operands {
                    operand.visit_exprs(&mut |expr| {
                        effects.extend(self.root_expr(expr, path, index));
                        index += 1;
                    });
                }
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
                effects.extend(self.root_expr(expr, path, 0));
            }
            Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                effects.extend(self.root_expr(cond, path, 0));
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    effects.extend(self.body(then_body, path));
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    effects.extend(self.body(else_body, path));
                });
            }
            Stmt::Loop { body, .. } => {
                walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                    effects.extend(self.body(body, path));
                });
            }
            Stmt::For { iter, body, .. } => {
                effects.extend(self.root_expr(iter, path, 0));
                walk::with_path_segment(path, PathSegment::ForBody, |path| {
                    effects.extend(self.body(body, path));
                });
            }
            Stmt::Scope { body } => {
                walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                    effects.extend(self.body(body, path));
                });
            }
            Stmt::LabeledBlock { body, .. } => {
                walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                    effects.extend(self.body(body, path));
                });
            }
            Stmt::Unsafe { body } => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    effects.extend(self.block(body, path));
                });
            }
            Stmt::While { cond, body } => {
                effects.extend(self.root_expr(cond, path, 0));
                walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                    effects.extend(self.block(body, path));
                });
            }
            Stmt::Block(body) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    effects.extend(self.block(body, path));
                });
            }
            Stmt::Match { expr, arms } => {
                effects.extend(self.root_expr(expr, path, 0));
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                        effects.extend(self.body(&arm.body, path));
                    });
                }
            }
        }
        let purity = purity_for_stmt(&effects);
        let result = effects.clone();
        self.effects.push(EffectFact {
            site: Site {
                function: self.function,
                path: AstPath(path.clone()),
            },
            subject: EffectSubject::Stmt,
            purity,
            effects,
        });
        result
    }

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) -> BTreeSet<EffectKind> {
        let effects = self.expr_inner(expr, path);
        self.effects.push(EffectFact {
            site: Site {
                function: self.function,
                path: AstPath(path.clone()),
            },
            subject: EffectSubject::Expr,
            purity: purity_for_expr(expr, &effects),
            effects: effects.clone(),
        });
        effects
    }

    fn root_expr(
        &mut self,
        expr: &Expr,
        path: &mut Vec<PathSegment>,
        index: usize,
    ) -> BTreeSet<EffectKind> {
        let effects = self.child_expr(expr, path, index);
        self.effects.push(EffectFact {
            site: Site {
                function: self.function,
                path: AstPath(path.clone()),
            },
            subject: EffectSubject::Expr,
            purity: purity_for_expr(expr, &effects),
            effects: effects.clone(),
        });
        effects
    }

    fn child_expr(
        &mut self,
        expr: &Expr,
        path: &mut Vec<PathSegment>,
        index: usize,
    ) -> BTreeSet<EffectKind> {
        walk::with_path_segment(path, PathSegment::Expr(index), |path| self.expr(expr, path))
    }

    fn expr_inner(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) -> BTreeSet<EffectKind> {
        let mut effects = BTreeSet::new();
        match expr {
            Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::CStr(_)
            | Expr::Var(_)
            | Expr::Path(_)
            | Expr::Todo(_) => {}
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => {
                effects.extend(self.child_expr(expr, path, 0));
            }
            Expr::Binary { lhs, rhs, .. } => {
                effects.extend(self.child_expr(lhs, path, 0));
                effects.extend(self.child_expr(rhs, path, 1));
            }
            Expr::Range { start, end } => {
                effects.extend(self.child_expr(start, path, 0));
                effects.extend(self.child_expr(end, path, 1));
            }
            Expr::Call { func, args, .. } => {
                let call_effect = call_effect(expr);
                effects.insert(call_effect);
                if call_effect == EffectKind::UnknownCall {
                    effects.insert(EffectKind::UnknownSideEffect);
                }
                effects.extend(self.child_expr(func, path, 0));
                for (index, arg) in args.iter().enumerate() {
                    effects.extend(self.child_expr(arg, path, index + 1));
                }
            }
            Expr::MethodCall { recv, method, args } => {
                effects.extend(self.child_expr(recv, path, 0));
                for (index, arg) in args.iter().enumerate() {
                    effects.extend(self.child_expr(arg, path, index + 1));
                }
                if !is_pure_primitive_bit_method(method) {
                    effects.insert(EffectKind::MethodCall);
                    effects.insert(EffectKind::UnknownSideEffect);
                }
            }
            Expr::MethodCallGeneric { recv, args, .. } => {
                effects.insert(EffectKind::MethodCall);
                effects.insert(EffectKind::UnknownSideEffect);
                effects.extend(self.child_expr(recv, path, 0));
                for (index, arg) in args.iter().enumerate() {
                    effects.extend(self.child_expr(arg, path, index + 1));
                }
            }
            Expr::Field { base, .. }
            | Expr::TupleField { base, .. }
            | Expr::ArrayPtr { array: base, .. } => {
                effects.extend(self.child_expr(base, path, 0));
            }
            Expr::Index { base, index } => {
                effects.extend(self.child_expr(base, path, 0));
                effects.extend(self.child_expr(index, path, 1));
            }
            Expr::StructLit { fields, .. } => {
                for (index, (_, value)) in fields.iter().enumerate() {
                    effects.extend(self.child_expr(value, path, index));
                }
            }
            Expr::TupleStructLit { fields, .. } => {
                for (index, value) in fields.iter().enumerate() {
                    effects.extend(self.child_expr(value, path, index));
                }
            }
            Expr::ArrayLit(elems) => {
                for (index, elem) in elems.iter().enumerate() {
                    effects.extend(self.child_expr(elem, path, index));
                }
            }
            Expr::VecLit(elems) => {
                for (index, elem) in elems.iter().enumerate() {
                    effects.extend(self.child_expr(elem, path, index));
                }
            }
            Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
                effects.extend(self.child_expr(elem, path, 0));
            }
            Expr::VecRepeat { elem, len } => {
                effects.extend(self.child_expr(elem, path, 0));
                effects.extend(self.child_expr(len, path, 1));
            }
            Expr::Macro { args, .. } => {
                effects.insert(EffectKind::MacroExpansion);
                effects.insert(EffectKind::UnknownSideEffect);
                for (index, arg) in args.iter().enumerate() {
                    effects.extend(self.child_expr(arg, path, index));
                }
            }
            Expr::Match { expr, arms } => {
                effects.extend(self.child_expr(expr, path, 0));
                for (index, arm) in arms.iter().enumerate() {
                    effects.extend(self.child_expr(&arm.value, path, index + 1));
                }
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                effects.extend(self.child_expr(cond, path, 0));
                effects.extend(self.child_expr(then_expr, path, 1));
                effects.extend(self.child_expr(else_expr, path, 2));
            }
            Expr::Block(block) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    effects.extend(self.block(block, path));
                });
            }
            Expr::Unsafe(block) => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    effects.extend(self.block(block, path));
                });
            }
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
                effects.insert(EffectKind::AtomicRead);
                if let Some(ptr) = place.ptr_expr() {
                    effects.extend(self.child_expr(ptr, path, 0));
                }
            }
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                effects.insert(EffectKind::AtomicWrite);
                effects.insert(EffectKind::MemoryWrite);
                if let Some(ptr) = place.ptr_expr() {
                    effects.extend(self.child_expr(ptr, path, 0));
                }
                effects.extend(self.child_expr(value, path, 1));
            }
            Expr::AtomicNew { value, .. } => {
                effects.extend(self.child_expr(value, path, 0));
            }
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                effects.insert(EffectKind::AtomicWrite);
                effects.insert(EffectKind::MemoryWrite);
                if let Some(ptr) = place.ptr_expr() {
                    effects.extend(self.child_expr(ptr, path, 0));
                }
                effects.extend(self.child_expr(expected, path, 1));
                effects.extend(self.child_expr(desired, path, 2));
            }
            Expr::AtomicFence { .. } => {
                effects.insert(EffectKind::AtomicWrite);
                effects.insert(EffectKind::UnknownSideEffect);
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                effects.insert(EffectKind::MemoryWrite);
                effects.extend(self.child_expr(src, path, 0));
                effects.extend(self.child_expr(dst, path, 1));
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                effects.insert(EffectKind::MemoryWrite);
                effects.extend(self.child_expr(src, path, 0));
                effects.extend(self.child_expr(dst, path, 1));
                effects.extend(self.child_expr(count, path, 2));
            }
            Expr::WriteBytes { dst, val, count } => {
                effects.insert(EffectKind::MemoryWrite);
                effects.extend(self.child_expr(dst, path, 0));
                effects.extend(self.child_expr(val, path, 1));
                effects.extend(self.child_expr(count, path, 2));
            }
        }
        effects
    }
}

fn call_effect(expr: &Expr) -> EffectKind {
    if matches!(
        known_call(expr),
        Some(
            Known::StrLen
                | Known::StrCmp
                | Known::StrNCmp
                | Known::MemCmp
                | Known::ToUpper
                | Known::ToLower
                | Known::IsAlpha
                | Known::IsDigit
                | Known::IsUpper
                | Known::IsLower
                | Known::IsAlnum
                | Known::IsXDigit
                | Known::IsPunct
                | Known::IsCntrl
                | Known::IsGraph
                | Known::IsPrint
                | Known::IsSpace
        )
    ) {
        return EffectKind::ReadOnlyCall;
    }
    let Expr::Call { func, .. } = expr else {
        return EffectKind::UnknownCall;
    };
    let Expr::Var(name) = &**func else {
        return EffectKind::UnknownCall;
    };
    match name.as_str() {
        "std::ptr::read_volatile" | "core::ptr::read_volatile" => EffectKind::VolatileRead,
        "std::ptr::write_volatile" | "core::ptr::write_volatile" => EffectKind::VolatileWrite,
        _ => EffectKind::UnknownCall,
    }
}

fn purity_for_expr(expr: &Expr, effects: &BTreeSet<EffectKind>) -> Purity {
    if effects.iter().any(is_side_effect) {
        Purity::Effectful
    } else if is_movable_pure_expr(expr) {
        Purity::MovablePure
    } else {
        Purity::ReadOnly
    }
}

fn purity_for_stmt(effects: &BTreeSet<EffectKind>) -> Purity {
    if effects.iter().any(is_side_effect) {
        Purity::Effectful
    } else {
        Purity::ReadOnly
    }
}

fn is_side_effect(effect: &EffectKind) -> bool {
    matches!(
        effect,
        EffectKind::UnknownCall
            | EffectKind::MethodCall
            | EffectKind::MacroExpansion
            | EffectKind::VolatileWrite
            | EffectKind::AtomicWrite
            | EffectKind::MemoryWrite
            | EffectKind::UnknownSideEffect
    )
}
