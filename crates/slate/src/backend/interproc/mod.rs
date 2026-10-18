pub(in crate::backend) mod length_lattice;
pub(in crate::backend) mod pointer_lattice;
pub(in crate::backend) mod string_params;

use std::collections::{BTreeMap, BTreeSet, VecDeque};
use std::fmt;

use crate::backend::rust_ast::{FnDef, Item, Program};

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) struct FnKey {
    pub(in crate::backend) unit: usize,
    pub(in crate::backend) name: String,
}

impl FnKey {
    pub(in crate::backend) fn new(unit: usize, name: impl Into<String>) -> Self {
        Self {
            unit,
            name: name.into(),
        }
    }
}

impl fmt::Display for FnKey {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(&self.name)
    }
}

pub(in crate::backend) struct Symbols {
    defined: BTreeSet<FnKey>,
}

impl Symbols {
    pub(in crate::backend) fn new(programs: &[Program]) -> Self {
        Self {
            defined: fn_defs(programs).into_keys().collect(),
        }
    }

    pub(in crate::backend) fn resolve(&self, unit: usize, name: &str) -> Option<FnKey> {
        let key = FnKey::new(unit, name);
        self.defined.contains(&key).then_some(key)
    }
}

pub(in crate::backend) fn fn_defs(programs: &[Program]) -> BTreeMap<FnKey, &FnDef> {
    let mut out = BTreeMap::new();
    for (unit, program) in programs.iter().enumerate() {
        collect_fn_defs_into(unit, &program.items, &mut out);
    }
    out
}

fn collect_fn_defs_into<'a>(unit: usize, items: &'a [Item], out: &mut BTreeMap<FnKey, &'a FnDef>) {
    for item in items {
        match item {
            Item::Fn(f) => {
                out.insert(FnKey::new(unit, f.name.clone()), f);
            }
            Item::InlineMod { items, .. } => collect_fn_defs_into(unit, items, out),
            _ => {}
        }
    }
}

#[derive(Debug, Default)]
pub(in crate::backend) struct CallGraph {
    nodes: BTreeSet<FnKey>,
    callees: BTreeMap<FnKey, BTreeSet<FnKey>>,
    callers: BTreeMap<FnKey, BTreeSet<FnKey>>,
}

impl CallGraph {
    pub(in crate::backend) fn new(nodes: impl IntoIterator<Item = FnKey>) -> Self {
        Self {
            nodes: nodes.into_iter().collect(),
            callees: BTreeMap::new(),
            callers: BTreeMap::new(),
        }
    }

    pub(in crate::backend) fn add_edge(&mut self, caller: &FnKey, callee: &FnKey) {
        if !self.nodes.contains(caller) || !self.nodes.contains(callee) {
            return;
        }
        self.callees
            .entry(caller.clone())
            .or_default()
            .insert(callee.clone());
        self.callers
            .entry(callee.clone())
            .or_default()
            .insert(caller.clone());
    }

    fn callees_of(&self, key: &FnKey) -> impl Iterator<Item = &FnKey> {
        self.callees.get(key).into_iter().flatten()
    }

    fn callers_of(&self, key: &FnKey) -> impl Iterator<Item = &FnKey> {
        self.callers.get(key).into_iter().flatten()
    }
}

pub(in crate::backend) fn scc_order(graph: &CallGraph) -> Vec<FnKey> {
    struct Tarjan<'g> {
        graph: &'g CallGraph,
        index: BTreeMap<FnKey, usize>,
        lowlink: BTreeMap<FnKey, usize>,
        on_stack: BTreeSet<FnKey>,
        stack: Vec<FnKey>,
        counter: usize,
        sccs: Vec<Vec<FnKey>>,
    }

    impl<'g> Tarjan<'g> {
        fn visit(&mut self, node: &FnKey) {
            self.index.insert(node.clone(), self.counter);
            self.lowlink.insert(node.clone(), self.counter);
            self.counter += 1;
            self.stack.push(node.clone());
            self.on_stack.insert(node.clone());

            let callees: Vec<FnKey> = self.graph.callees_of(node).cloned().collect();
            for callee in callees {
                if !self.index.contains_key(&callee) {
                    self.visit(&callee);
                    let callee_low = self.lowlink[&callee];
                    let entry = self.lowlink.get_mut(node).expect("node just inserted");
                    *entry = (*entry).min(callee_low);
                } else if self.on_stack.contains(&callee) {
                    let callee_index = self.index[&callee];
                    let entry = self.lowlink.get_mut(node).expect("node just inserted");
                    *entry = (*entry).min(callee_index);
                }
            }

            if self.lowlink[node] == self.index[node] {
                let mut scc = Vec::new();
                loop {
                    let popped = self.stack.pop().expect("node's own SCC root is on stack");
                    self.on_stack.remove(&popped);
                    let done = &popped == node;
                    scc.push(popped);
                    if done {
                        break;
                    }
                }
                self.sccs.push(scc);
            }
        }
    }

    let mut tarjan = Tarjan {
        graph,
        index: BTreeMap::new(),
        lowlink: BTreeMap::new(),
        on_stack: BTreeSet::new(),
        stack: Vec::new(),
        counter: 0,
        sccs: Vec::new(),
    };
    for node in &graph.nodes {
        if !tarjan.index.contains_key(node) {
            tarjan.visit(node);
        }
    }
    tarjan.sccs.into_iter().flatten().collect()
}

const WORKLIST_EDIT_BUDGET: usize = 200_000;

pub(in crate::backend) fn run_worklist(
    graph: &CallGraph,
    initial_order: &[FnKey],
    mut recompute: impl FnMut(&FnKey) -> bool,
) {
    let mut queued: BTreeSet<FnKey> = initial_order.iter().cloned().collect();
    let mut queue: VecDeque<FnKey> = initial_order.iter().cloned().collect();
    let mut iterations = 0usize;

    while let Some(key) = queue.pop_front() {
        queued.remove(&key);
        let changed = recompute(&key);
        iterations += 1;
        assert!(
            iterations <= WORKLIST_EDIT_BUDGET,
            "interproc worklist exceeded {WORKLIST_EDIT_BUDGET} iterations processing `{key}`; \
             likely oscillation (non-monotone recompute), not slow convergence"
        );
        if !changed {
            continue;
        }
        for neighbor in graph.callers_of(&key).chain(graph.callees_of(&key)) {
            if queued.insert(neighbor.clone()) {
                queue.push_back(neighbor.clone());
            }
        }
    }
}
