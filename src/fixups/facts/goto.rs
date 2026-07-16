//! Front-end-agnostic control-flow-graph analysis shared by goto lowering and
//! the goto-structuring fixup. The node/edge model is abstract: build a
//! `Vec<CfgNode>` from CIR blocks or from a Rust dispatch `match` loop, then run
//! the same dominator / natural-loop / SCC algorithms over it.

use std::collections::BTreeSet;

#[derive(Debug, Clone)]
pub(crate) struct CfgNode {
    pub(crate) labels: Vec<String>,
    pub(crate) successors: Vec<CfgEdge>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub(crate) struct CfgEdge {
    pub(crate) from: usize,
    pub(crate) to: usize,
    pub(crate) kind: CfgEdgeKind,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(crate) enum CfgEdgeKind {
    Goto,
    Branch,
    Fallthrough,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub(crate) enum CfgUnsupported {
    UnknownLabel(String),
    UnknownBlock(String),
    DynamicBranch,
    IndirectBranch,
    IrreducibleCycle(Vec<usize>),
}

#[derive(Debug, Clone)]
pub(crate) struct NaturalLoop {
    pub(crate) header: usize,
    pub(crate) latch: usize,
    pub(crate) nodes: BTreeSet<usize>,
    pub(crate) entries: Vec<CfgEdge>,
    pub(crate) exits: Vec<CfgEdge>,
}

pub(crate) fn dominators(entry: usize, nodes: &[CfgNode]) -> Vec<BTreeSet<usize>> {
    let all_nodes = (0..nodes.len()).collect::<BTreeSet<_>>();
    let mut doms = vec![all_nodes.clone(); nodes.len()];
    if let Some(entry_doms) = doms.get_mut(entry) {
        *entry_doms = BTreeSet::from([entry]);
    }
    let predecessors = predecessors(nodes);
    let mut changed = true;
    while changed {
        changed = false;
        for node in 0..nodes.len() {
            if node == entry {
                continue;
            }
            let preds = &predecessors[node];
            let mut next = if preds.is_empty() {
                BTreeSet::new()
            } else {
                preds
                    .iter()
                    .map(|&pred| doms[pred].clone())
                    .reduce(|acc, dom| acc.intersection(&dom).cloned().collect())
                    .unwrap_or_default()
            };
            next.insert(node);
            if next != doms[node] {
                doms[node] = next;
                changed = true;
            }
        }
    }
    doms
}

pub(crate) fn predecessors(nodes: &[CfgNode]) -> Vec<Vec<usize>> {
    let mut predecessors = vec![Vec::new(); nodes.len()];
    for node in nodes {
        for edge in &node.successors {
            if let Some(preds) = predecessors.get_mut(edge.to)
                && !preds.contains(&edge.from)
            {
                preds.push(edge.from);
            }
        }
    }
    predecessors
}

pub(crate) fn natural_loop(backedge: &CfgEdge, nodes: &[CfgNode]) -> NaturalLoop {
    let predecessors = predecessors(nodes);
    let mut loop_nodes = BTreeSet::from([backedge.to]);
    let mut stack = if backedge.from == backedge.to {
        Vec::new()
    } else {
        loop_nodes.insert(backedge.from);
        vec![backedge.from]
    };
    while let Some(node) = stack.pop() {
        for &pred in &predecessors[node] {
            if loop_nodes.insert(pred) {
                stack.push(pred);
            }
        }
    }
    let mut entries = Vec::new();
    let mut exits = Vec::new();
    for node in nodes {
        for edge in &node.successors {
            let from_in_loop = loop_nodes.contains(&edge.from);
            let to_in_loop = loop_nodes.contains(&edge.to);
            if !from_in_loop && to_in_loop {
                entries.push(edge.clone());
            } else if from_in_loop && !to_in_loop {
                exits.push(edge.clone());
            }
        }
    }
    NaturalLoop {
        header: backedge.to,
        latch: backedge.from,
        nodes: loop_nodes,
        entries,
        exits,
    }
}

pub(crate) fn cyclic_sccs(nodes: &[CfgNode]) -> Vec<Vec<usize>> {
    let mut result = Vec::new();
    let mut seen = BTreeSet::new();
    for node in 0..nodes.len() {
        if seen.contains(&node) {
            continue;
        }
        let forward = reachable_from(node, nodes);
        let reverse = reverse_reachable_from(node, nodes);
        let scc = forward
            .intersection(&reverse)
            .copied()
            .collect::<BTreeSet<_>>();
        for member in &scc {
            seen.insert(*member);
        }
        let self_loop = nodes[node]
            .successors
            .iter()
            .any(|edge| edge.from == node && edge.to == node);
        if scc.len() > 1 || self_loop {
            result.push(scc.into_iter().collect());
        }
    }
    result
}

fn reachable_from(start: usize, nodes: &[CfgNode]) -> BTreeSet<usize> {
    let mut reached = BTreeSet::new();
    let mut stack = vec![start];
    while let Some(node) = stack.pop() {
        if !reached.insert(node) {
            continue;
        }
        for edge in &nodes[node].successors {
            stack.push(edge.to);
        }
    }
    reached
}

fn reverse_reachable_from(start: usize, nodes: &[CfgNode]) -> BTreeSet<usize> {
    let predecessors = predecessors(nodes);
    let mut reached = BTreeSet::new();
    let mut stack = vec![start];
    while let Some(node) = stack.pop() {
        if !reached.insert(node) {
            continue;
        }
        for &pred in &predecessors[node] {
            stack.push(pred);
        }
    }
    reached
}

pub(crate) fn cycle_entry_targets(cycle: &[usize], nodes: &[CfgNode]) -> BTreeSet<usize> {
    let cycle = cycle.iter().copied().collect::<BTreeSet<_>>();
    let mut targets = BTreeSet::new();
    for node in nodes {
        for edge in &node.successors {
            if !cycle.contains(&edge.from) && cycle.contains(&edge.to) {
                targets.insert(edge.to);
            }
        }
    }
    targets
}
