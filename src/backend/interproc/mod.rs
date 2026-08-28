pub(in crate::backend) mod string_params;

use std::collections::{BTreeMap, BTreeSet, VecDeque};

#[derive(Debug, Default)]
pub(in crate::backend) struct CallGraph {
    nodes: BTreeSet<String>,
    callees: BTreeMap<String, BTreeSet<String>>,
    callers: BTreeMap<String, BTreeSet<String>>,
}

impl CallGraph {
    pub(in crate::backend) fn new(nodes: impl IntoIterator<Item = String>) -> Self {
        Self {
            nodes: nodes.into_iter().collect(),
            callees: BTreeMap::new(),
            callers: BTreeMap::new(),
        }
    }

    pub(in crate::backend) fn add_edge(&mut self, caller: &str, callee: &str) {
        if !self.nodes.contains(caller) || !self.nodes.contains(callee) {
            return;
        }
        self.callees
            .entry(caller.to_string())
            .or_default()
            .insert(callee.to_string());
        self.callers
            .entry(callee.to_string())
            .or_default()
            .insert(caller.to_string());
    }

    fn callees_of(&self, name: &str) -> impl Iterator<Item = &str> {
        self.callees
            .get(name)
            .into_iter()
            .flat_map(|set| set.iter().map(String::as_str))
    }

    fn callers_of(&self, name: &str) -> impl Iterator<Item = &str> {
        self.callers
            .get(name)
            .into_iter()
            .flat_map(|set| set.iter().map(String::as_str))
    }
}

pub(in crate::backend) fn scc_order(graph: &CallGraph) -> Vec<String> {
    struct Tarjan<'g> {
        graph: &'g CallGraph,
        index: BTreeMap<String, usize>,
        lowlink: BTreeMap<String, usize>,
        on_stack: BTreeSet<String>,
        stack: Vec<String>,
        counter: usize,
        sccs: Vec<Vec<String>>,
    }

    impl<'g> Tarjan<'g> {
        fn visit(&mut self, node: &str) {
            self.index.insert(node.to_string(), self.counter);
            self.lowlink.insert(node.to_string(), self.counter);
            self.counter += 1;
            self.stack.push(node.to_string());
            self.on_stack.insert(node.to_string());

            let callees: Vec<String> = self.graph.callees_of(node).map(str::to_string).collect();
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
                    let done = popped == node;
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
    initial_order: &[String],
    mut recompute: impl FnMut(&str) -> bool,
) {
    let mut queued: BTreeSet<String> = initial_order.iter().cloned().collect();
    let mut queue: VecDeque<String> = initial_order.iter().cloned().collect();
    let mut iterations = 0usize;

    while let Some(name) = queue.pop_front() {
        queued.remove(&name);
        let changed = recompute(&name);
        iterations += 1;
        assert!(
            iterations <= WORKLIST_EDIT_BUDGET,
            "interproc worklist exceeded {WORKLIST_EDIT_BUDGET} iterations processing `{name}`; \
             likely oscillation (non-monotone recompute), not slow convergence"
        );
        if !changed {
            continue;
        }
        for neighbor in graph.callers_of(&name).chain(graph.callees_of(&name)) {
            if queued.insert(neighbor.to_string()) {
                queue.push_back(neighbor.to_string());
            }
        }
    }
}
