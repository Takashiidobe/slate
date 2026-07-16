//! Front-end-agnostic control-flow-graph analysis shared by goto lowering and
//! the goto-structuring fixup. The node/edge model is abstract: build a
//! `Vec<CfgNode>` from CIR blocks or from a Rust dispatch `match` loop, then run
//! the same dominator / natural-loop / SCC algorithms over it.

use std::collections::BTreeSet;

use crate::rust_ast::{Expr, IndentStmt, RustValue, Stmt};

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

// ---------------------------------------------------------------------------
// Dispatch-loop recognizer
//
// `lower_dispatch` emits, for a goto-bearing function:
//
//     let mut __stateN: i32 = 0;
//     '__dispatchN: loop {
//         match __stateN {
//             0 => { ..body..; __stateN = k; continue '__dispatchN; }
//             ..
//             _ => { unreachable!() }   // or break '__dispatchN
//         }
//     }
//
// The recognizer parses that shape back out of the Rust AST so the structuring
// fixup can rebuild `if`/`loop` from the state machine. Each arm's tail is a
// recursive guard tree (chained `if cond { goto } ..`), captured as `ArmFlow`.

/// A recognized dispatch region found at `stmt_index` within a statement list:
/// the `let mut __stateN` binding immediately followed by the labeled loop.
#[derive(Debug, Clone)]
pub(crate) struct DispatchLoop {
    pub(crate) state_var: String,
    pub(crate) loop_label: String,
    pub(crate) let_index: usize,
    pub(crate) loop_index: usize,
    pub(crate) entry: usize,
    pub(crate) states: Vec<DispatchState>,
    pub(crate) fallback: Vec<IndentStmt>,
    /// A state is assigned a non-constant value (e.g. computed goto jump table);
    /// the region must stay a dispatch loop.
    pub(crate) dynamic: bool,
}

#[derive(Debug, Clone)]
pub(crate) struct DispatchState {
    pub(crate) index: usize,
    pub(crate) flow: ArmFlow,
}

/// Straight-line `prefix`, then a control transfer.
#[derive(Debug, Clone)]
pub(crate) struct ArmFlow {
    pub(crate) prefix: Vec<IndentStmt>,
    pub(crate) transfer: Transfer,
}

#[derive(Debug, Clone)]
pub(crate) enum Transfer {
    /// `__stateN = target; continue 'L` — an edge to another state (or, when
    /// `target == states.len()`, out through the wildcard fallback).
    Goto(usize),
    Branch {
        cond: Expr,
        then_: Box<ArmFlow>,
        else_: Box<ArmFlow>,
    },
    Return(Option<Expr>),
    /// `unreachable!()`, `break 'L`, `std::process::exit(..)` and other
    /// divergence; carries the original terminal statement for re-emission.
    Diverge(Box<Stmt>),
}

impl DispatchLoop {
    /// `Goto` target `t` lands on the wildcard fallback rather than a real state.
    pub(crate) fn is_exit(&self, target: usize) -> bool {
        target >= self.states.len()
    }

    pub(crate) fn cfg_nodes(&self) -> Vec<CfgNode> {
        self.states
            .iter()
            .map(|state| {
                let successors = state
                    .successors()
                    .into_iter()
                    .filter(|&to| to < self.states.len())
                    .map(|to| CfgEdge {
                        from: state.index,
                        to,
                        kind: CfgEdgeKind::Goto,
                    })
                    .collect();
                CfgNode {
                    labels: Vec::new(),
                    successors,
                }
            })
            .collect()
    }

    /// A reducible state graph can be rebuilt into structured control flow;
    /// dynamic assignment or an irreducible (multi-entry) cycle forces fallback.
    pub(crate) fn is_reducible(&self) -> bool {
        if self.dynamic {
            return false;
        }
        let nodes = self.cfg_nodes();
        cyclic_sccs(&nodes)
            .iter()
            .all(|cycle| cycle_entry_targets(cycle, &nodes).len() <= 1)
    }
}

impl DispatchState {
    pub(crate) fn successors(&self) -> Vec<usize> {
        let mut out = Vec::new();
        collect_gotos(&self.flow, &mut out);
        out
    }
}

impl ArmFlow {
    /// Distinct states this flow's transfer tree jumps to.
    pub(crate) fn gotos(&self) -> Vec<usize> {
        let mut out = Vec::new();
        collect_gotos(self, &mut out);
        out
    }
}

fn collect_gotos(flow: &ArmFlow, out: &mut Vec<usize>) {
    match &flow.transfer {
        Transfer::Goto(target) => {
            if !out.contains(target) {
                out.push(*target);
            }
        }
        Transfer::Branch { then_, else_, .. } => {
            collect_gotos(then_, out);
            collect_gotos(else_, out);
        }
        Transfer::Return(_) | Transfer::Diverge(_) => {}
    }
}

/// Find every top-level dispatch region in a statement list.
pub(crate) fn recognize_dispatch_loops(stmts: &[IndentStmt]) -> Vec<DispatchLoop> {
    let mut found = Vec::new();
    for loop_index in 0..stmts.len() {
        let Stmt::Loop {
            label: Some(label),
            body,
        } = &stmts[loop_index].stmt
        else {
            continue;
        };
        let loop_label = label.as_str().to_string();
        if !loop_label.starts_with("__dispatch") {
            continue;
        }
        let [inner] = &body[..] else { continue };
        let Stmt::Match { expr, arms } = &inner.stmt else {
            continue;
        };
        let Some(state_var) = var_name(expr) else {
            continue;
        };
        if !state_var.starts_with("__state") {
            continue;
        }
        let Some(let_index) = state_let_index(stmts, loop_index, &state_var) else {
            continue;
        };
        let Some(dispatch) = parse_arms(arms, &state_var, &loop_label) else {
            continue;
        };
        found.push(DispatchLoop {
            state_var,
            loop_label,
            let_index,
            loop_index,
            entry: 0,
            states: dispatch.0,
            fallback: dispatch.1,
            dynamic: dispatch.2,
        });
    }
    found
}

fn state_let_index(stmts: &[IndentStmt], loop_index: usize, state_var: &str) -> Option<usize> {
    (0..loop_index)
        .rev()
        .find(|&i| matches!(&stmts[i].stmt, Stmt::Let { name, .. } if name == state_var))
}

type ParsedArms = (Vec<DispatchState>, Vec<IndentStmt>, bool);

fn parse_arms(
    arms: &[crate::rust_ast::MatchArm],
    state_var: &str,
    loop_label: &str,
) -> Option<ParsedArms> {
    use crate::rust_ast::Pattern;
    let mut states = Vec::new();
    let mut fallback = None;
    let mut dynamic = false;
    for arm in arms {
        match &arm.pattern {
            Pattern::Wildcard => {
                if fallback.is_some() {
                    return None;
                }
                fallback = Some(arm.body.clone());
            }
            Pattern::I64(_) | Pattern::I128(_) => {
                let index = match &arm.pattern {
                    Pattern::I64(v) => *v as usize,
                    Pattern::I128(v) => *v as usize,
                    _ => unreachable!(),
                };
                if index != states.len() {
                    return None;
                }
                let flow = parse_flow(&arm.body, state_var, loop_label, &mut dynamic)?;
                states.push(DispatchState { index, flow });
            }
            _ => return None,
        }
    }
    Some((states, fallback.unwrap_or_default(), dynamic))
}

/// Parse an arm (or a nested branch body) into `prefix` + `transfer`.
fn parse_flow(
    stmts: &[IndentStmt],
    state_var: &str,
    loop_label: &str,
    dynamic: &mut bool,
) -> Option<ArmFlow> {
    let stmts = flatten_scopes(stmts);
    let stmts = &stmts[..];
    let mut prefix = Vec::new();
    let mut i = 0;
    while i < stmts.len() {
        if let Some((target, consumed)) = goto_transfer(stmts, i, state_var, loop_label, dynamic) {
            let _ = consumed;
            return Some(ArmFlow {
                prefix,
                transfer: Transfer::Goto(target),
            });
        }
        match &stmts[i].stmt {
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                let then_ = parse_flow(then_body, state_var, loop_label, dynamic)?;
                let else_ = if else_body.is_empty() {
                    parse_flow(&stmts[i + 1..], state_var, loop_label, dynamic)?
                } else {
                    parse_flow(else_body, state_var, loop_label, dynamic)?
                };
                return Some(ArmFlow {
                    prefix,
                    transfer: Transfer::Branch {
                        cond: cond.clone(),
                        then_: Box::new(then_),
                        else_: Box::new(else_),
                    },
                });
            }
            Stmt::Return(value) => {
                return Some(ArmFlow {
                    prefix,
                    transfer: Transfer::Return(value.clone()),
                });
            }
            Stmt::Break(_) | Stmt::Continue(_) => {
                return Some(ArmFlow {
                    prefix,
                    transfer: Transfer::Diverge(Box::new(stmts[i].stmt.clone())),
                });
            }
            Stmt::Expr(expr) if expr_diverges(expr) => {
                return Some(ArmFlow {
                    prefix,
                    transfer: Transfer::Diverge(Box::new(stmts[i].stmt.clone())),
                });
            }
            _ => {
                prefix.push(stmts[i].clone());
                i += 1;
            }
        }
    }
    None
}

/// Inline plain lexical scopes so a branch spilled into `{ let _v = ...; if ... }`
/// reads as a flat statement stream. Compiler temps are uniquely named, so
/// dropping the block boundary is safe here. Labeled blocks stay — they are
/// break targets.
fn flatten_scopes(stmts: &[IndentStmt]) -> Vec<IndentStmt> {
    let mut out = Vec::new();
    for s in stmts {
        if let Stmt::Scope { body } = &s.stmt {
            out.extend(flatten_scopes(body));
        } else {
            out.push(s.clone());
        }
    }
    out
}

/// `__stateN = <value>; continue 'L` at position `i`; returns the target state
/// and how many statements were consumed. Non-constant values set `dynamic`.
fn goto_transfer(
    stmts: &[IndentStmt],
    i: usize,
    state_var: &str,
    loop_label: &str,
    dynamic: &mut bool,
) -> Option<(usize, usize)> {
    let Stmt::Assign { target, value } = &stmts[i].stmt else {
        return None;
    };
    if var_name(target).as_deref() != Some(state_var) {
        return None;
    }
    let continues = stmts
        .get(i + 1)
        .is_some_and(|s| matches!(&s.stmt, Stmt::Continue(Some(l)) if l.as_str() == loop_label));
    if !continues {
        return None;
    }
    match const_usize(value) {
        Some(target) => Some((target, 2)),
        None => {
            *dynamic = true;
            Some((usize::MAX, 2))
        }
    }
}

fn expr_diverges(expr: &Expr) -> bool {
    match expr {
        Expr::Macro { name, .. } => {
            matches!(
                name.as_str(),
                "unreachable" | "panic" | "todo" | "unimplemented"
            )
        }
        Expr::Call { func, .. } => callee_name(func)
            .as_deref()
            .is_some_and(|name| name.ends_with("process::exit") || name == "abort"),
        _ => false,
    }
}

fn callee_name(expr: &Expr) -> Option<String> {
    match expr {
        Expr::Var(ident) => Some(ident.as_str().to_string()),
        Expr::Path(path) => Some(
            path.segments
                .iter()
                .map(|s| s.as_str())
                .collect::<Vec<_>>()
                .join("::"),
        ),
        _ => None,
    }
}

fn var_name(expr: &Expr) -> Option<String> {
    match expr {
        Expr::Var(ident) => Some(ident.as_str().to_string()),
        _ => None,
    }
}

fn const_usize(expr: &Expr) -> Option<usize> {
    match expr {
        Expr::Value(RustValue::I64(v)) if *v >= 0 => Some(*v as usize),
        Expr::Value(RustValue::I128(v)) if *v >= 0 => Some(*v as usize),
        Expr::Value(RustValue::Usize(v)) => Some(*v),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::rust_ast::{BinOp, Label, MatchArm, Pattern};

    fn ind(stmt: Stmt) -> IndentStmt {
        IndentStmt { depth: 0, stmt }
    }

    fn var(name: &str) -> Expr {
        Expr::Var(name.into())
    }

    fn int(v: i64) -> Expr {
        Expr::Value(RustValue::I64(v))
    }

    fn goto(state: i64) -> Vec<IndentStmt> {
        vec![
            ind(Stmt::Assign {
                target: var("__state0"),
                value: int(state),
            }),
            ind(Stmt::Continue(Some(Label::new("__dispatch0")))),
        ]
    }

    fn lt(lhs: &str, rhs: i64) -> Expr {
        Expr::Binary {
            op: BinOp::Lt,
            lhs: Box::new(var(lhs)),
            rhs: Box::new(int(rhs)),
        }
    }

    fn arm(index: i64, body: Vec<IndentStmt>) -> MatchArm {
        MatchArm {
            pattern: Pattern::I64(index),
            body,
        }
    }

    fn wildcard(body: Vec<IndentStmt>) -> MatchArm {
        MatchArm {
            pattern: Pattern::Wildcard,
            body,
        }
    }

    fn dispatch(arms: Vec<MatchArm>) -> DispatchLoop {
        let stmts = vec![
            ind(Stmt::Let {
                name: "__state0".into(),
                mutable: true,
                ty: None,
                init: Some(int(0)),
            }),
            ind(Stmt::Loop {
                label: Some(Label::new("__dispatch0")),
                body: vec![ind(Stmt::Match {
                    expr: var("__state0"),
                    arms,
                })],
            }),
        ];
        let mut found = recognize_dispatch_loops(&stmts);
        assert_eq!(found.len(), 1, "expected exactly one dispatch region");
        found.pop().unwrap()
    }

    #[test]
    fn recognizes_state_var_let_and_loop_indices() {
        let d = dispatch(vec![
            arm(0, goto(1)),
            arm(1, vec![ind(Stmt::Return(Some(int(0))))]),
            wildcard(vec![ind(Stmt::Expr(Expr::Macro {
                name: "unreachable".into(),
                args: Vec::new(),
            }))]),
        ]);
        assert_eq!(d.state_var, "__state0");
        assert_eq!(d.loop_label, "__dispatch0");
        assert_eq!(d.let_index, 0);
        assert_eq!(d.loop_index, 1);
        assert_eq!(d.states.len(), 2);
    }

    #[test]
    fn fallthrough_goto_is_single_successor() {
        let d = dispatch(vec![
            arm(0, goto(1)),
            arm(1, vec![ind(Stmt::Return(None))]),
            wildcard(vec![ind(Stmt::Return(None))]),
        ]);
        assert!(matches!(d.states[0].flow.transfer, Transfer::Goto(1)));
        assert_eq!(d.states[0].successors(), vec![1]);
        assert!(matches!(d.states[1].flow.transfer, Transfer::Return(None)));
        assert!(d.states[1].successors().is_empty());
        assert!(d.is_reducible());
    }

    #[test]
    fn if_else_arm_is_branch_with_two_gotos() {
        let mut then_body = goto(1);
        let mut else_body = goto(2);
        let d = dispatch(vec![
            arm(
                0,
                vec![ind(Stmt::If {
                    cond: lt("n", 0),
                    then_body: std::mem::take(&mut then_body),
                    else_body: std::mem::take(&mut else_body),
                })],
            ),
            arm(1, vec![ind(Stmt::Return(Some(int(-1))))]),
            arm(2, vec![ind(Stmt::Return(Some(int(1))))]),
            wildcard(vec![ind(Stmt::Return(None))]),
        ]);
        match &d.states[0].flow.transfer {
            Transfer::Branch { then_, else_, .. } => {
                assert!(matches!(then_.transfer, Transfer::Goto(1)));
                assert!(matches!(else_.transfer, Transfer::Goto(2)));
            }
            other => panic!("expected branch, got {other:?}"),
        }
        let mut succ = d.states[0].successors();
        succ.sort();
        assert_eq!(succ, vec![1, 2]);
        assert!(d.is_reducible());
    }

    #[test]
    fn chained_guards_end_in_return() {
        // n=arg0; if n<0 { goto 1 } if n==0 { goto 2 } return 1
        let mut body = vec![ind(Stmt::If {
            cond: lt("n", 0),
            then_body: goto(1),
            else_body: Vec::new(),
        })];
        body.push(ind(Stmt::If {
            cond: lt("n", 1),
            then_body: goto(2),
            else_body: Vec::new(),
        }));
        body.push(ind(Stmt::Return(Some(int(1)))));
        let d = dispatch(vec![
            arm(0, body),
            arm(1, vec![ind(Stmt::Return(Some(int(-1))))]),
            arm(2, vec![ind(Stmt::Return(Some(int(0))))]),
            wildcard(vec![ind(Stmt::Return(None))]),
        ]);
        match &d.states[0].flow.transfer {
            Transfer::Branch { then_, else_, .. } => {
                assert!(matches!(then_.transfer, Transfer::Goto(1)));
                match &else_.transfer {
                    Transfer::Branch { then_, else_, .. } => {
                        assert!(matches!(then_.transfer, Transfer::Goto(2)));
                        assert!(matches!(else_.transfer, Transfer::Return(_)));
                    }
                    other => panic!("expected nested branch, got {other:?}"),
                }
            }
            other => panic!("expected branch, got {other:?}"),
        }
        assert!(d.is_reducible());
    }

    #[test]
    fn non_constant_state_assignment_is_dynamic() {
        // __state0 = [2, 3][x]; continue  -> unstructurable
        let index_assign = vec![
            ind(Stmt::Assign {
                target: var("__state0"),
                value: Expr::Index {
                    base: Box::new(Expr::ArrayLit(vec![int(2), int(3)])),
                    index: Box::new(var("x")),
                },
            }),
            ind(Stmt::Continue(Some(Label::new("__dispatch0")))),
        ];
        let d = dispatch(vec![
            arm(0, index_assign),
            arm(1, vec![ind(Stmt::Return(Some(int(10))))]),
            arm(2, vec![ind(Stmt::Return(Some(int(20))))]),
            wildcard(vec![ind(Stmt::Return(None))]),
        ]);
        assert!(d.dynamic);
        assert!(!d.is_reducible());
    }

    #[test]
    fn multi_entry_cycle_is_irreducible() {
        // entry branches into both members of a 2-node cycle {1,2}.
        let d = dispatch(vec![
            arm(
                0,
                vec![ind(Stmt::If {
                    cond: lt("c", 1),
                    then_body: goto(1),
                    else_body: goto(2),
                })],
            ),
            arm(
                1,
                vec![ind(Stmt::If {
                    cond: lt("x", 3),
                    then_body: goto(2),
                    else_body: vec![ind(Stmt::Return(None))],
                })],
            ),
            arm(
                2,
                vec![ind(Stmt::If {
                    cond: lt("x", 4),
                    then_body: goto(1),
                    else_body: vec![ind(Stmt::Return(None))],
                })],
            ),
            wildcard(vec![ind(Stmt::Return(None))]),
        ]);
        assert!(!d.dynamic);
        assert!(!d.is_reducible());
    }
}
