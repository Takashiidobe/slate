use std::collections::BTreeSet;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, EffectFact, EffectKind, EffectSubject, FixupFacts, FunctionId, PathSegment, Purity,
};
use crate::rust_ast::{Block, Expr, IndentStmt, Item, Program, Stmt, UnaryOp};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.effects.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut collector = Collector {
            function,
            effects: Vec::new(),
        };
        collector.body(&f.body, &mut Vec::new());
        all.extend(collector.effects);
    }
    facts.effects = all;
}

pub(in crate::fixups) fn is_movable_pure_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) | Expr::Var(_) => true,
        Expr::Unary { op, expr } => !matches!(op, UnaryOp::Not) && is_movable_pure_expr(expr),
        Expr::Binary { lhs, rhs, .. } => is_movable_pure_expr(lhs) && is_movable_pure_expr(rhs),
        _ => false,
    }
}

struct Collector {
    function: FunctionId,
    effects: Vec<EffectFact>,
}

impl Collector {
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
                    effects.extend(self.expr(init, path));
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
                effects.extend(self.expr(cond, path));
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    effects.extend(self.body(then_body, path));
                    effects.extend(self.expr(then_value, path));
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    effects.extend(self.body(else_body, path));
                    effects.extend(self.expr(else_value, path));
                });
            }
            Stmt::Assign { target, value } => {
                effects.insert(EffectKind::MemoryWrite);
                effects.extend(self.expr(target, path));
                effects.extend(self.expr(value, path));
            }
            Stmt::CompoundAssign { target, value, .. } => {
                effects.insert(EffectKind::MemoryWrite);
                effects.extend(self.expr(target, path));
                effects.extend(self.expr(value, path));
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
                effects.extend(self.expr(expr, path));
            }
            Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                effects.extend(self.expr(cond, path));
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
                effects.extend(self.expr(cond, path));
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
                effects.extend(self.expr(expr, path));
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
            function: self.function,
            subject: EffectSubject::Stmt,
            path: AstPath(path.clone()),
            purity,
            effects,
        });
        result
    }

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) -> BTreeSet<EffectKind> {
        let effects = self.expr_inner(expr, path);
        self.effects.push(EffectFact {
            function: self.function,
            subject: EffectSubject::Expr,
            path: AstPath(path.clone()),
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
            Expr::Call { func, args } => {
                let call_effect = call_effect(func);
                effects.insert(call_effect);
                if call_effect == EffectKind::UnknownCall {
                    effects.insert(EffectKind::UnknownSideEffect);
                }
                effects.extend(self.child_expr(func, path, 0));
                for (index, arg) in args.iter().enumerate() {
                    effects.extend(self.child_expr(arg, path, index + 1));
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
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
            Expr::ArrayLit(elems) => {
                for (index, elem) in elems.iter().enumerate() {
                    effects.extend(self.child_expr(elem, path, index));
                }
            }
            Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
                effects.extend(self.child_expr(elem, path, 0));
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
            Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
                effects.insert(EffectKind::AtomicRead);
                effects.extend(self.child_expr(ptr, path, 0));
            }
            Expr::AtomicStore { ptr, value, .. }
            | Expr::AtomicFetch { ptr, value, .. }
            | Expr::AtomicSwap { ptr, value, .. } => {
                effects.insert(EffectKind::AtomicWrite);
                effects.insert(EffectKind::MemoryWrite);
                effects.extend(self.child_expr(ptr, path, 0));
                effects.extend(self.child_expr(value, path, 1));
            }
            Expr::AtomicCompareExchange {
                ptr,
                expected,
                desired,
                ..
            } => {
                effects.insert(EffectKind::AtomicWrite);
                effects.insert(EffectKind::MemoryWrite);
                effects.extend(self.child_expr(ptr, path, 0));
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

fn call_effect(func: &Expr) -> EffectKind {
    let Expr::Var(name) = func else {
        return EffectKind::UnknownCall;
    };
    match name.as_str() {
        "strlen" | "strcmp" | "strncmp" | "memcmp" => EffectKind::ReadOnlyCall,
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{AtomicOrdering, AtomicType, BinOp, Expr, Item, Program, Stmt};

    fn analyzed(stmts: Vec<Stmt>) -> facts::FixupFacts {
        facts::analyze(Program {
            items: vec![Item::Fn(func(vec![param("p", "*mut i32")], None, stmts))],
        })
        .facts
    }

    fn effect_for(facts: &facts::FixupFacts, subject: EffectSubject, path: AstPath) -> &EffectFact {
        facts
            .effects
            .iter()
            .find(|fact| fact.subject == subject && fact.path == path)
            .unwrap()
    }

    #[test]
    fn classifies_pure_arithmetic_as_movable_pure() {
        let facts = analyzed(vec![temp(
            "x",
            "i32",
            bin(BinOp::Add, var("p"), bin(BinOp::Mul, int(2), int(3))),
        )]);

        let expr = effect_for(
            &facts,
            EffectSubject::Expr,
            AstPath(vec![PathSegment::Stmt(0)]),
        );
        assert_eq!(expr.purity, Purity::MovablePure);
        assert!(expr.effects.is_empty());
        assert!(is_movable_pure_expr(&bin(BinOp::Add, var("a"), int(1))));
    }

    #[test]
    fn classifies_read_only_and_unknown_calls() {
        let facts = analyzed(vec![
            temp("len", "usize", call("strlen", vec![var("p")])),
            temp("x", "i32", call("mystery", vec![var("p")])),
        ]);

        let strlen = effect_for(
            &facts,
            EffectSubject::Expr,
            AstPath(vec![PathSegment::Stmt(0)]),
        );
        assert_eq!(strlen.purity, Purity::ReadOnly);
        assert_eq!(strlen.effects, BTreeSet::from([EffectKind::ReadOnlyCall]));

        let mystery = effect_for(
            &facts,
            EffectSubject::Expr,
            AstPath(vec![PathSegment::Stmt(1)]),
        );
        assert_eq!(mystery.purity, Purity::Effectful);
        assert!(mystery.effects.contains(&EffectKind::UnknownCall));
        assert!(mystery.effects.contains(&EffectKind::UnknownSideEffect));
    }

    #[test]
    fn classifies_volatile_atomic_and_memory_writes() {
        let facts = analyzed(vec![
            temp("x", "i32", call("std::ptr::read_volatile", vec![var("p")])),
            Stmt::Expr(call("std::ptr::write_volatile", vec![var("p"), int(1)])),
            Stmt::Expr(Expr::AtomicLoad {
                ty: AtomicType::I32,
                ptr: Box::new(var("p")),
                ordering: AtomicOrdering::SeqCst,
            }),
            Stmt::Expr(Expr::AtomicStore {
                ty: AtomicType::I32,
                ptr: Box::new(var("p")),
                value: Box::new(int(2)),
                ordering: AtomicOrdering::SeqCst,
            }),
            assign("p", var("p")),
        ]);

        assert!(
            effect_for(
                &facts,
                EffectSubject::Expr,
                AstPath(vec![PathSegment::Stmt(0)])
            )
            .effects
            .contains(&EffectKind::VolatileRead)
        );
        assert!(
            effect_for(
                &facts,
                EffectSubject::Expr,
                AstPath(vec![PathSegment::Stmt(1)])
            )
            .effects
            .contains(&EffectKind::VolatileWrite)
        );
        assert!(
            effect_for(
                &facts,
                EffectSubject::Expr,
                AstPath(vec![PathSegment::Stmt(2)])
            )
            .effects
            .contains(&EffectKind::AtomicRead)
        );
        let atomic_store = effect_for(
            &facts,
            EffectSubject::Expr,
            AstPath(vec![PathSegment::Stmt(3)]),
        );
        assert!(atomic_store.effects.contains(&EffectKind::AtomicWrite));
        assert!(atomic_store.effects.contains(&EffectKind::MemoryWrite));

        let assign = effect_for(
            &facts,
            EffectSubject::Stmt,
            AstPath(vec![PathSegment::Stmt(4)]),
        );
        assert_eq!(assign.purity, Purity::Effectful);
        assert!(assign.effects.contains(&EffectKind::MemoryWrite));
    }

    #[test]
    fn classifies_macro_and_method_calls_conservatively() {
        let facts = analyzed(vec![
            Stmt::Expr(Expr::Macro {
                name: "println".into(),
                args: vec![var("p")],
            }),
            Stmt::Expr(Expr::MethodCall {
                recv: Box::new(var("p")),
                method: "len".into(),
                args: vec![],
            }),
        ]);

        assert!(
            effect_for(
                &facts,
                EffectSubject::Expr,
                AstPath(vec![PathSegment::Stmt(0)])
            )
            .effects
            .contains(&EffectKind::MacroExpansion)
        );
        assert!(
            effect_for(
                &facts,
                EffectSubject::Expr,
                AstPath(vec![PathSegment::Stmt(1)])
            )
            .effects
            .contains(&EffectKind::MethodCall)
        );
    }

    #[test]
    fn records_nested_expression_paths() {
        let facts = analyzed(vec![temp(
            "x",
            "i32",
            bin(BinOp::Add, call("mystery", vec![var("p")]), int(1)),
        )]);

        let nested = effect_for(
            &facts,
            EffectSubject::Expr,
            AstPath(vec![PathSegment::Stmt(0), PathSegment::Expr(0)]),
        );
        assert!(nested.effects.contains(&EffectKind::UnknownCall));

        let leaf = effect_for(
            &facts,
            EffectSubject::Expr,
            AstPath(vec![PathSegment::Stmt(0), PathSegment::Expr(1)]),
        );
        assert_eq!(leaf.purity, Purity::MovablePure);
    }

    #[test]
    fn propagates_block_expression_effects_to_parent() {
        let facts = analyzed(vec![Stmt::Expr(Expr::Block(Box::new(
            crate::rust_ast::Block {
                stmts: vec![crate::rust_ast::IndentStmt {
                    depth: 2,
                    stmt: Stmt::Expr(call("mystery", vec![])),
                }],
                tail: Some(Box::new(int(0))),
            },
        )))]);

        let parent = effect_for(
            &facts,
            EffectSubject::Expr,
            AstPath(vec![PathSegment::Stmt(0)]),
        );
        assert_eq!(parent.purity, Purity::Effectful);
        assert!(parent.effects.contains(&EffectKind::UnknownCall));

        let stmt = effect_for(
            &facts,
            EffectSubject::Stmt,
            AstPath(vec![PathSegment::Stmt(0)]),
        );
        assert_eq!(stmt.purity, Purity::Effectful);
        assert!(stmt.effects.contains(&EffectKind::UnknownCall));
    }
}
