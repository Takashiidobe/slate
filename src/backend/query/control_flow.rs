//! Rebuild structured control flow from a naive `__state`/`__dispatch` loop.
//!
//! The lowerer emits every goto-bearing function as a state-machine dispatch
//! loop (correct but temp-heavy). This fixup recognizes that loop and, when the
//! state graph is reducible enough, rewrites it into ordinary
//! `if`/`else`/`loop`/sequence Rust so the downstream cleanup passes can see the
//! real shape. Disabling it leaves the correct dispatch loop untouched.
//!
//! Three shapes are recovered, tried in order:
//!   * acyclic reducible regions → `if`/`else`/sequence;
//!   * a single self-loop (`while`-style guard) → `loop { ..; if !cond break }`;
//!   * a single irreducible cycle → residual `loop { match __blockN }` peeled
//!     from its acyclic preheader/exit.
//!
//! Anything else (dynamic jump tables, multi-loop nests) stays a dispatch loop.

use std::collections::BTreeSet;

use crate::backend::facts::goto::{
    ArmFlow, CfgEdge, CfgNode, DispatchLoop, NaturalLoop, Transfer, cycle_entry_targets,
    cyclic_sccs, dominators, natural_loop,
};
use crate::backend::rust_ast::{Expr, IndentStmt, MatchArm, Prim, RustValue, Stmt, Type, UnaryOp};

use super::DispatchRegion;

pub(super) fn structure_dispatch(region: &DispatchRegion) -> Option<Vec<IndentStmt>> {
    let dispatch = &region.dispatch;
    if dispatch.dynamic {
        return None;
    }
    let mut structured = structure_acyclic(dispatch)
        .or_else(|| structure_multi_exit_self_loop(dispatch))
        .or_else(|| structure_self_loop(dispatch))
        .or_else(|| structure_localized_cycle(dispatch))?;
    for stmt in &mut structured {
        stmt.depth = region.depth;
    }
    Some(structured)
}

// --- shared CFG analysis --------------------------------------------------

struct Analysis {
    nodes: Vec<CfgNode>,
    natural_loops: Vec<NaturalLoop>,
    irreducible: Vec<Vec<usize>>,
}

impl Analysis {
    fn new(dispatch: &DispatchLoop) -> Self {
        let nodes = dispatch.cfg_nodes();
        let doms = dominators(dispatch.entry, &nodes);
        let backedges: Vec<CfgEdge> = nodes
            .iter()
            .flat_map(|node| node.successors.iter())
            .filter(|edge| doms.get(edge.from).is_some_and(|d| d.contains(&edge.to)))
            .cloned()
            .collect();
        let natural_loops = backedges
            .iter()
            .map(|edge| natural_loop(edge, &nodes))
            .collect();
        let irreducible = cyclic_sccs(&nodes)
            .into_iter()
            .filter(|cycle| cycle_entry_targets(cycle, &nodes).len() > 1)
            .collect();
        Analysis {
            nodes,
            natural_loops,
            irreducible,
        }
    }
}

// --- acyclic --------------------------------------------------------------

fn structure_acyclic(dispatch: &DispatchLoop) -> Option<Vec<IndentStmt>> {
    if !dispatch.is_reducible() {
        return None;
    }
    let n = dispatch.states.len();
    for state in &dispatch.states {
        // any cycle, or any jump out through the wildcard fallback, is out of
        // scope for the acyclic pass.
        if state.successors().iter().any(|&t| t >= n) {
            return None;
        }
    }
    let region: BTreeSet<usize> = (0..n).collect();
    let mut structurer = Structurer::new(dispatch, region);
    if structurer.has_cycle() {
        return None;
    }
    structurer.compute_order();
    let stmts = structurer.emit_state(dispatch.entry, &BTreeSet::new());
    if structurer.visited.len() != structurer.reachable().len() {
        return None;
    }
    Some(stmts)
}

// --- single self-loop -----------------------------------------------------

fn structure_multi_exit_self_loop(dispatch: &DispatchLoop) -> Option<Vec<IndentStmt>> {
    let n = dispatch.states.len();
    let analysis = Analysis::new(dispatch);
    if !analysis.irreducible.is_empty() || analysis.natural_loops.len() != 1 {
        return None;
    }
    let nl = &analysis.natural_loops[0];
    if nl.header != nl.latch || nl.nodes != BTreeSet::from([nl.header]) {
        return None;
    }
    let header = nl.header;
    let successors = dispatch.states[header].successors();
    if !successors.contains(&header)
        || successors.iter().any(|&t| t >= n)
        || successors.iter().all(|&t| t == header)
    {
        return None;
    }

    let all: BTreeSet<usize> = (0..n).collect();
    let exit_seeds: Vec<usize> = successors
        .iter()
        .copied()
        .filter(|&t| t != header)
        .collect();
    let mut exit_region = BTreeSet::new();
    for seed in exit_seeds {
        exit_region.extend(reach_within(dispatch, seed, &sub(&all, &nl.nodes)));
    }
    let prefix_region: BTreeSet<usize> = reach_within(dispatch, dispatch.entry, &all)
        .difference(&union(&nl.nodes, &exit_region))
        .copied()
        .collect();

    let mut out = Vec::new();
    let mut prefix = Structurer::new(dispatch, prefix_region);
    prefix.compute_order();
    out.extend(prefix.emit_state(dispatch.entry, &BTreeSet::new()));

    let mut exits = Structurer::new(dispatch, exit_region);
    exits.compute_order();
    let loop_body = emit_self_loop_flow(&dispatch.states[header].flow, header, &mut exits)?;
    out.push(ind(Stmt::Loop {
        label: None,
        body: loop_body,
    }));
    Some(out)
}

fn structure_self_loop(dispatch: &DispatchLoop) -> Option<Vec<IndentStmt>> {
    let n = dispatch.states.len();
    let analysis = Analysis::new(dispatch);
    if !analysis.irreducible.is_empty() || analysis.natural_loops.len() != 1 {
        return None;
    }
    let nl = &analysis.natural_loops[0];
    if nl.header != nl.latch || nl.nodes != BTreeSet::from([nl.header]) {
        return None;
    }
    let header = nl.header;
    if dispatch.states[header].successors().iter().any(|&t| t > n) {
        return None;
    }
    let Transfer::Branch { cond, then_, else_ } = &dispatch.states[header].flow.transfer else {
        return None;
    };
    let then_back = is_goto_to(then_, header);
    let else_back = is_goto_to(else_, header);
    if then_back == else_back {
        return None;
    }
    // the back-edge arm re-enters the loop; the other arm leaves it. break when
    // the loop condition fails.
    let (guard_cond, back_arm, exit_arm) = if then_back {
        (not_expr(cond.clone()), then_, else_)
    } else {
        (cond.clone(), else_, then_)
    };
    if !back_arm.prefix.is_empty() {
        return None;
    }

    let all: BTreeSet<usize> = (0..n).collect();
    let exit_states: BTreeSet<usize> = match goto_target(exit_arm) {
        Some(x) if x < n => reach_within(dispatch, x, &sub(&all, &nl.nodes)),
        Some(_) => return None,
        None => BTreeSet::new(),
    };
    let reach_entry = reach_within(dispatch, dispatch.entry, &all);
    let prefix_region: BTreeSet<usize> = reach_entry
        .difference(&union(&nl.nodes, &exit_states))
        .copied()
        .collect();

    let mut out = Vec::new();
    let mut prefix = Structurer::new(dispatch, prefix_region);
    prefix.compute_order();
    out.extend(prefix.emit_state(dispatch.entry, &BTreeSet::new()));

    let mut loop_body = dispatch.states[header].flow.prefix.clone();
    loop_body.push(ind(Stmt::If {
        cond: guard_cond,
        then_body: vec![ind(Stmt::Break(None))],
        else_body: Vec::new(),
    }));
    out.push(ind(Stmt::Loop {
        label: None,
        body: loop_body,
    }));

    let mut exit = Structurer::new(dispatch, exit_states);
    exit.compute_order();
    out.extend(exit.emit_flow(exit_arm, &BTreeSet::new()));
    Some(out)
}

// --- single irreducible cycle ---------------------------------------------

fn structure_localized_cycle(dispatch: &DispatchLoop) -> Option<Vec<IndentStmt>> {
    let n = dispatch.states.len();
    let analysis = Analysis::new(dispatch);
    if analysis.irreducible.len() != 1 {
        return None;
    }
    let cycle_nodes = analysis.irreducible[0].clone();
    let cycle: BTreeSet<usize> = cycle_nodes.iter().copied().collect();
    if cycle.contains(&dispatch.entry) {
        return None;
    }
    // the preheader must be the region entry: everything before the cycle is the
    // entry state's own branch into it.
    if cycle_nodes
        .iter()
        .any(|&node| dispatch.states[node].successors().iter().any(|&t| t >= n))
    {
        return None;
    }

    let mut exits = BTreeSet::new();
    let mut entry_edges = Vec::new();
    for node in &analysis.nodes {
        for edge in &node.successors {
            match (cycle.contains(&edge.from), cycle.contains(&edge.to)) {
                (true, false) => {
                    exits.insert(edge.to);
                }
                (false, true) => entry_edges.push(edge.clone()),
                _ => {}
            }
        }
    }
    if exits.len() != 1 || entry_edges.is_empty() {
        return None;
    }
    let exit_state = *exits.iter().next().unwrap();
    let preheader = entry_edges[0].from;
    if preheader != dispatch.entry || entry_edges.iter().any(|e| e.from != preheader) {
        return None;
    }

    let block_var = block_var_name(&dispatch.state_var);
    let mut out = vec![ind(Stmt::Let {
        name: block_var.clone(),
        mutable: true,
        ty: Some(Type::Prim(Prim::I32)),
        init: None,
    })];
    out.extend(emit_preheader(
        &dispatch.states[preheader].flow,
        &cycle,
        &block_var,
    )?);

    let mut arms = Vec::new();
    for &node in &cycle_nodes {
        let body = emit_cycle_arm(&dispatch.states[node].flow, &cycle, exit_state, &block_var)?;
        arms.push(crate::backend::rust_ast::MatchArm {
            pattern: crate::backend::rust_ast::Pattern::I64(node as i64),
            body,
        });
    }
    if arms.len() == 2 {
        let last = arms.pop().unwrap();
        arms.push(crate::backend::rust_ast::MatchArm {
            pattern: crate::backend::rust_ast::Pattern::Wildcard,
            body: last.body,
        });
    }
    out.push(ind(Stmt::Loop {
        label: None,
        body: vec![ind(Stmt::Match {
            expr: Expr::Var(block_var.clone().into()),
            arms,
        })],
    }));

    let all: BTreeSet<usize> = (0..n).collect();
    let exit_region = reach_within(dispatch, exit_state, &sub(&all, &cycle));
    let mut exit = Structurer::new(dispatch, exit_region);
    exit.compute_order();
    out.extend(exit.emit_state(exit_state, &BTreeSet::new()));
    Some(out)
}

/// Structure the acyclic preheader, rewriting each edge into the cycle as a
/// `__blockN = target` assignment.
fn emit_preheader(
    flow: &ArmFlow,
    cycle: &BTreeSet<usize>,
    block_var: &str,
) -> Option<Vec<IndentStmt>> {
    let mut out = flow.prefix.clone();
    match &flow.transfer {
        Transfer::Goto(target) if cycle.contains(target) => {
            out.push(assign_block(block_var, *target));
        }
        Transfer::Branch { cond, then_, else_ } => {
            out.push(ind(Stmt::If {
                cond: cond.clone(),
                then_body: emit_preheader(then_, cycle, block_var)?,
                else_body: emit_preheader(else_, cycle, block_var)?,
            }));
        }
        _ => return None,
    }
    Some(out)
}

/// Structure one cycle node's arm: internal edges set `__blockN` and `continue`;
/// the single exit edge becomes `break`.
fn emit_cycle_arm(
    flow: &ArmFlow,
    cycle: &BTreeSet<usize>,
    exit_state: usize,
    block_var: &str,
) -> Option<Vec<IndentStmt>> {
    let mut out = flow.prefix.clone();
    match &flow.transfer {
        Transfer::Goto(target) if cycle.contains(target) => {
            out.push(assign_block(block_var, *target));
            out.push(ind(Stmt::Continue(None)));
        }
        Transfer::Goto(target) if *target == exit_state => {
            out.push(ind(Stmt::Break(None)));
        }
        Transfer::Branch { cond, then_, else_ } => {
            out.push(ind(Stmt::If {
                cond: cond.clone(),
                then_body: emit_cycle_arm(then_, cycle, exit_state, block_var)?,
                else_body: emit_cycle_arm(else_, cycle, exit_state, block_var)?,
            }));
        }
        Transfer::Return(value) => out.push(ind(Stmt::Return(value.clone()))),
        Transfer::Diverge(stmt) => out.push(ind((**stmt).clone())),
        _ => return None,
    }
    Some(out)
}

// --- region emitter (acyclic within a fixed set of states) ----------------

struct Structurer<'a> {
    dispatch: &'a DispatchLoop,
    region: BTreeSet<usize>,
    order_of: std::collections::BTreeMap<usize, usize>,
    visited: BTreeSet<usize>,
}

impl<'a> Structurer<'a> {
    fn new(dispatch: &'a DispatchLoop, region: BTreeSet<usize>) -> Self {
        Structurer {
            dispatch,
            region,
            order_of: std::collections::BTreeMap::new(),
            visited: BTreeSet::new(),
        }
    }

    fn succ(&self, state: usize) -> Vec<usize> {
        self.dispatch.states[state]
            .successors()
            .into_iter()
            .filter(|t| self.region.contains(t))
            .collect()
    }

    fn reachable(&self) -> BTreeSet<usize> {
        self.reach(&[self.dispatch.entry])
    }

    fn reach(&self, seeds: &[usize]) -> BTreeSet<usize> {
        let mut reached = BTreeSet::new();
        let mut stack: Vec<usize> = seeds
            .iter()
            .copied()
            .filter(|t| self.region.contains(t))
            .collect();
        while let Some(state) = stack.pop() {
            if reached.insert(state) {
                stack.extend(self.succ(state));
            }
        }
        reached
    }

    fn has_cycle(&self) -> bool {
        let mut color = std::collections::BTreeMap::new();
        let mut cycle = false;
        for &start in &self.region {
            if !color.contains_key(&start) {
                self.detect_cycle(start, &mut color, &mut cycle);
            }
        }
        cycle
    }

    fn detect_cycle(
        &self,
        node: usize,
        color: &mut std::collections::BTreeMap<usize, u8>,
        cycle: &mut bool,
    ) {
        color.insert(node, 1);
        for next in self.succ(node) {
            match color.get(&next) {
                Some(1) => *cycle = true,
                None => self.detect_cycle(next, color, cycle),
                _ => {}
            }
        }
        color.insert(node, 2);
    }

    fn compute_order(&mut self) {
        let mut order = Vec::new();
        let mut seen = BTreeSet::new();
        if self.region.contains(&self.dispatch.entry) {
            self.postorder(self.dispatch.entry, &mut seen, &mut order);
        }
        for &state in &self.region {
            self.postorder(state, &mut seen, &mut order);
        }
        order.reverse();
        for (rank, state) in order.iter().enumerate() {
            self.order_of.insert(*state, rank);
        }
    }

    fn postorder(&self, node: usize, seen: &mut BTreeSet<usize>, order: &mut Vec<usize>) {
        if !seen.insert(node) {
            return;
        }
        for next in self.succ(node) {
            self.postorder(next, seen, order);
        }
        order.push(node);
    }

    fn emit_state(&mut self, state: usize, stop: &BTreeSet<usize>) -> Vec<IndentStmt> {
        if !self.region.contains(&state) || !self.visited.insert(state) {
            return Vec::new();
        }
        let flow = self.dispatch.states[state].flow.clone();
        self.emit_flow(&flow, stop)
    }

    fn emit_flow(&mut self, flow: &ArmFlow, stop: &BTreeSet<usize>) -> Vec<IndentStmt> {
        let mut out = flow.prefix.clone();
        out.extend(self.emit_transfer(&flow.transfer, stop));
        out
    }

    fn emit_transfer(&mut self, transfer: &Transfer, stop: &BTreeSet<usize>) -> Vec<IndentStmt> {
        match transfer {
            Transfer::Goto(target) => {
                if stop.contains(target) || !self.region.contains(target) {
                    Vec::new()
                } else {
                    self.emit_state(*target, stop)
                }
            }
            Transfer::Return(value) => vec![ind(Stmt::Return(value.clone()))],
            Transfer::Diverge(stmt) => vec![ind((**stmt).clone())],
            Transfer::Branch { cond, then_, else_ } => {
                let join = self.find_join(then_, else_);
                let inner_stop = match join {
                    Some(j) => {
                        let mut s = stop.clone();
                        s.insert(j);
                        s
                    }
                    None => stop.clone(),
                };
                let then_stmts = self.emit_flow(then_, &inner_stop);
                let else_stmts = self.emit_flow(else_, &inner_stop);
                let mut out = render_branch(cond.clone(), then_stmts, else_stmts);
                if let Some(j) = join
                    && !stop.contains(&j)
                {
                    out.extend(self.emit_state(j, stop));
                }
                out
            }
            Transfer::Switch { expr, arms } => {
                let join = self.find_switch_join(arms);
                let inner_stop = match join {
                    Some(j) => {
                        let mut s = stop.clone();
                        s.insert(j);
                        s
                    }
                    None => stop.clone(),
                };
                let match_arms = arms
                    .iter()
                    .map(|arm| MatchArm {
                        pattern: arm.pattern.clone(),
                        body: self.emit_flow(&arm.flow, &inner_stop),
                    })
                    .collect();
                let mut out = vec![ind(Stmt::Match {
                    expr: expr.clone(),
                    arms: match_arms,
                })];
                if let Some(j) = join
                    && !stop.contains(&j)
                {
                    out.extend(self.emit_state(j, stop));
                }
                out
            }
        }
    }

    /// The nearest state both branch arms inevitably reach — the post-dominator
    /// where an `if`/`else` reconverges. `None` when the arms never rejoin.
    fn find_join(&self, then_: &ArmFlow, else_: &ArmFlow) -> Option<usize> {
        let then_reach = self.reach(&then_.gotos());
        let else_reach = self.reach(&else_.gotos());
        then_reach
            .intersection(&else_reach)
            .copied()
            .min_by_key(|state| self.order_of.get(state).copied().unwrap_or(usize::MAX))
    }

    fn find_switch_join(
        &self,
        arms: &[crate::backend::facts::goto::SwitchFlowArm],
    ) -> Option<usize> {
        let mut reaches = arms.iter().map(|arm| self.reach(&arm.flow.gotos()));
        let first = reaches.next()?;
        reaches
            .fold(first, |acc, reach| {
                acc.intersection(&reach).copied().collect()
            })
            .into_iter()
            .min_by_key(|state| self.order_of.get(state).copied().unwrap_or(usize::MAX))
    }
}

fn render_branch(
    cond: Expr,
    then_stmts: Vec<IndentStmt>,
    else_stmts: Vec<IndentStmt>,
) -> Vec<IndentStmt> {
    if then_stmts.is_empty() && else_stmts.is_empty() {
        return Vec::new();
    }
    if else_stmts.is_empty() {
        return vec![ind(Stmt::If {
            cond,
            then_body: then_stmts,
            else_body: Vec::new(),
        })];
    }
    if then_stmts.is_empty() {
        return vec![ind(Stmt::If {
            cond: not_expr(cond),
            then_body: else_stmts,
            else_body: Vec::new(),
        })];
    }
    if diverges(&then_stmts) {
        let mut out = vec![ind(Stmt::If {
            cond,
            then_body: then_stmts,
            else_body: Vec::new(),
        })];
        out.extend(else_stmts);
        return out;
    }
    vec![ind(Stmt::If {
        cond,
        then_body: then_stmts,
        else_body: else_stmts,
    })]
}

fn diverges(stmts: &[IndentStmt]) -> bool {
    match stmts.last().map(|s| &s.stmt) {
        Some(Stmt::Return(_) | Stmt::Break(_) | Stmt::Continue(_)) => true,
        Some(Stmt::Expr(expr)) => expr_diverges(expr),
        _ => false,
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

// --- small helpers --------------------------------------------------------

fn emit_self_loop_flow(
    flow: &ArmFlow,
    header: usize,
    exits: &mut Structurer<'_>,
) -> Option<Vec<IndentStmt>> {
    let mut out = flow.prefix.clone();
    out.extend(emit_self_loop_transfer(&flow.transfer, header, exits)?);
    Some(out)
}

fn emit_self_loop_transfer(
    transfer: &Transfer,
    header: usize,
    exits: &mut Structurer<'_>,
) -> Option<Vec<IndentStmt>> {
    match transfer {
        Transfer::Goto(target) if *target == header => Some(vec![ind(Stmt::Continue(None))]),
        Transfer::Goto(target) => {
            let out = exits.emit_state(*target, &BTreeSet::new());
            diverges(&out).then_some(out)
        }
        Transfer::Return(value) => Some(vec![ind(Stmt::Return(value.clone()))]),
        Transfer::Diverge(stmt) => Some(vec![ind((**stmt).clone())]),
        Transfer::Branch { cond, then_, else_ } => {
            let then_stmts = emit_self_loop_flow(then_, header, exits)?;
            let else_stmts = emit_self_loop_flow(else_, header, exits)?;
            Some(render_branch(cond.clone(), then_stmts, else_stmts))
        }
        Transfer::Switch { expr, arms } => {
            let mut match_arms = Vec::new();
            for arm in arms {
                match_arms.push(MatchArm {
                    pattern: arm.pattern.clone(),
                    body: emit_self_loop_flow(&arm.flow, header, exits)?,
                });
            }
            Some(vec![ind(Stmt::Match {
                expr: expr.clone(),
                arms: match_arms,
            })])
        }
    }
}

fn is_goto_to(flow: &ArmFlow, target: usize) -> bool {
    goto_target(flow) == Some(target)
}

fn goto_target(flow: &ArmFlow) -> Option<usize> {
    if flow.prefix.is_empty()
        && let Transfer::Goto(t) = &flow.transfer
    {
        Some(*t)
    } else {
        None
    }
}

fn reach_within(
    dispatch: &DispatchLoop,
    entry: usize,
    region: &BTreeSet<usize>,
) -> BTreeSet<usize> {
    let mut reached = BTreeSet::new();
    let mut stack = Vec::new();
    if region.contains(&entry) {
        stack.push(entry);
    }
    while let Some(state) = stack.pop() {
        if reached.insert(state) {
            for t in dispatch.states[state].successors() {
                if region.contains(&t) {
                    stack.push(t);
                }
            }
        }
    }
    reached
}

fn block_var_name(state_var: &str) -> String {
    let suffix = state_var.strip_prefix("__state").unwrap_or("0");
    format!("__block{suffix}")
}

fn assign_block(block_var: &str, target: usize) -> IndentStmt {
    ind(Stmt::Assign {
        target: Expr::Var(block_var.to_string().into()),
        value: Expr::Value(RustValue::I64(target as i64)),
    })
}

fn union(a: &BTreeSet<usize>, b: &BTreeSet<usize>) -> BTreeSet<usize> {
    a.union(b).copied().collect()
}

fn sub(a: &BTreeSet<usize>, b: &BTreeSet<usize>) -> BTreeSet<usize> {
    a.difference(b).copied().collect()
}

fn not_expr(cond: Expr) -> Expr {
    Expr::Unary {
        op: UnaryOp::Not,
        expr: Box::new(cond),
    }
}

fn ind(stmt: Stmt) -> IndentStmt {
    IndentStmt { depth: 0, stmt }
}
