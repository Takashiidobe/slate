mod arena;
mod rules;

use arena::{Arena, FunctionArena, NodeId, NodeKindTag};
use std::collections::{BTreeSet, HashMap};

use crate::backend::rust_ast::{Ident, IndentStmt, Item, Program};

pub(in crate::backend) trait NodeRule {
    fn name(&self) -> &'static str;
    fn priority(&self) -> u32;
    fn kinds(&self) -> &'static [NodeKindTag];
    fn call_anchor(&self) -> Option<Ident> {
        None
    }
    fn matches(&self, arena: &Arena, id: NodeId) -> bool;
    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool;
}

const EDIT_BUDGET: usize = 200_000;

struct RuleRegistry {
    rules: Vec<Box<dyn NodeRule>>,
    by_kind: [Vec<u32>; NodeKindTag::COUNT],
    by_anchor: HashMap<(NodeKindTag, Ident), Vec<u32>>,
}

impl RuleRegistry {
    fn build(mut rules: Vec<Box<dyn NodeRule>>) -> Self {
        rules.sort_by_key(|rule| rule.priority());
        let mut by_kind: [Vec<u32>; NodeKindTag::COUNT] = std::array::from_fn(|_| Vec::new());
        let mut by_anchor: HashMap<(NodeKindTag, Ident), Vec<u32>> = HashMap::new();
        for (index, rule) in rules.iter().enumerate() {
            for &kind in rule.kinds() {
                match rule.call_anchor() {
                    Some(anchor) => by_anchor
                        .entry((kind, anchor))
                        .or_default()
                        .push(index as u32),
                    None => by_kind[kind as usize].push(index as u32),
                }
            }
        }
        Self {
            rules,
            by_kind,
            by_anchor,
        }
    }

    fn candidates(&self, kind: NodeKindTag, call_anchor: Option<Ident>) -> Vec<&dyn NodeRule> {
        let mut indices: Vec<u32> = self.by_kind[kind as usize].clone();
        if let Some(anchor) = call_anchor
            && let Some(anchored) = self.by_anchor.get(&(kind, anchor))
        {
            indices.extend_from_slice(anchored);
            indices.sort_unstable();
        }
        indices
            .into_iter()
            .map(|index| self.rules[index as usize].as_ref())
            .collect()
    }
}

pub(in crate::backend) fn apply(program: &mut Program) {
    crate::backend::interproc::string_params::run(program);

    let registry = RuleRegistry::build(rules::registry());
    for item in &mut program.items {
        apply_item(item, &registry);
    }
}

fn apply_item(item: &mut Item, registry: &RuleRegistry) {
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

fn run_function(body: &mut Vec<IndentStmt>, registry: &RuleRegistry) {
    let taken = std::mem::take(body);
    let FunctionArena { mut arena, root } = arena::build(taken);
    run_worklist(&mut arena, registry);
    *body = arena::reify(&arena, root);
}

fn run_worklist(arena: &mut Arena, registry: &RuleRegistry) {
    let mut worklist: BTreeSet<u32> = arena.live_ids().iter().map(|id| id.index()).collect();
    let mut edits = 0usize;

    while let Some(&index) = worklist.iter().next() {
        worklist.remove(&index);
        let Some(id) = arena.resolve(index) else {
            continue;
        };
        let Some(kind) = arena.get(id) else {
            continue;
        };
        let def_use_targets: Vec<NodeId> = kind
            .declared_name()
            .map(|name| arena.def_use_neighbors(name).to_vec())
            .unwrap_or_default();
        let candidates: Vec<&dyn NodeRule> = registry
            .candidates(kind.tag(), kind.call_anchor())
            .into_iter()
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
                arena.touch(id);
                if let Some(resolved) = arena.resolve(index) {
                    worklist.insert(resolved.index());
                }
                if let Some(parent) = arena.parent(id) {
                    worklist.insert(parent.index());
                }
                for &neighbor in &def_use_targets {
                    if arena.get(neighbor).is_some() {
                        worklist.insert(neighbor.index());
                    }
                }
                break;
            }
        }
    }
}
