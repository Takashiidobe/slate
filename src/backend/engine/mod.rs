mod arena;
mod rules;

use arena::{Arena, FunctionArena, NodeId};
use std::collections::BTreeSet;

use crate::backend::rust_ast::{IndentStmt, Item, Program};

pub(in crate::backend) trait NodeRule {
    fn name(&self) -> &'static str;
    fn priority(&self) -> u32;
    fn matches(&self, arena: &Arena, id: NodeId) -> bool;
    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool;
}

const EDIT_BUDGET: usize = 200_000;

pub(in crate::backend) fn apply(program: &mut Program) {
    let mut registry = rules::registry();
    registry.sort_by_key(|rule| rule.priority());
    for item in &mut program.items {
        apply_item(item, &registry);
    }
}

fn apply_item(item: &mut Item, registry: &[Box<dyn NodeRule>]) {
    match item {
        Item::Fn(func) => run_function(&mut func.body, registry),
        Item::InlineMod { items, .. } => {
            for item in items {
                apply_item(item, registry);
            }
        }
        Item::Impl(impl_block) => {
            for impl_item in &mut impl_block.items {
                if let crate::backend::rust_ast::ImplItem::Method(method) = impl_item {
                    let crate::backend::rust_ast::Expr::Block(block) = &mut method.body else {
                        continue;
                    };
                    let mut body = std::mem::take(&mut block.stmts);
                    run_function(&mut body, registry);
                    block.stmts = body;
                }
            }
        }
        _ => {}
    }
}

fn run_function(body: &mut Vec<IndentStmt>, registry: &[Box<dyn NodeRule>]) {
    let taken = std::mem::take(body);
    let FunctionArena { mut arena, root } = arena::build(taken);
    run_worklist(&mut arena, registry);
    *body = arena::reify(&arena, root);
}

fn run_worklist(arena: &mut Arena, registry: &[Box<dyn NodeRule>]) {
    let mut worklist: BTreeSet<u32> = arena.live_ids().iter().map(|id| id.index()).collect();
    let mut edits = 0usize;

    while let Some(&index) = worklist.iter().next() {
        worklist.remove(&index);
        let Some(id) = arena.resolve(index) else {
            continue;
        };
        let candidates: Vec<&Box<dyn NodeRule>> = registry
            .iter()
            .filter(|rule| rule.matches(arena, id))
            .collect();
        for rule in candidates {
            if rule.apply(arena, id) {
                edits += 1;
                assert!(
                    edits <= EDIT_BUDGET,
                    "rewrite worklist exceeded {EDIT_BUDGET} edits (rule={}); likely oscillation, not slow convergence",
                    rule.name()
                );
                if let Some(resolved) = arena.resolve(index) {
                    worklist.insert(resolved.index());
                }
                if let Some(parent) = arena.parent(id) {
                    worklist.insert(parent.index());
                }
                break;
            }
        }
    }
}
