//! Rebuild structured control flow from a naive `__state`/`__dispatch` loop.
//!
//! The lowerer emits every goto-bearing function as a state-machine dispatch
//! loop (correct but temp-heavy). This fixup recognizes that loop and, when the
//! state graph is reducible, rewrites it into ordinary `if`/`else`/sequence
//! Rust so the downstream cleanup passes can see the real shape. Disabling it
//! leaves the correct dispatch loop untouched.
//!
//! This pass structures **acyclic** reducible regions only; cyclic (loop) and
//! irreducible regions stay as dispatch loops for now.

use std::collections::BTreeSet;

use crate::fixups::facts::goto::{self, ArmFlow, DispatchLoop, Transfer};
use crate::rust_ast::{Expr, IndentStmt, Stmt, UnaryOp};

pub(in crate::fixups) fn fixup(body: &mut Vec<IndentStmt>) -> bool {
    for dispatch in goto::recognize_dispatch_loops(body) {
        if let Some(structured) = structure_acyclic(&dispatch) {
            replace_region(body, &dispatch, structured);
            return true;
        }
    }
    false
}

fn replace_region(
    body: &mut Vec<IndentStmt>,
    dispatch: &DispatchLoop,
    mut structured: Vec<IndentStmt>,
) {
    let base_depth = body[dispatch.loop_index].depth;
    for stmt in &mut structured {
        stmt.depth = base_depth;
    }
    body.splice(dispatch.loop_index..=dispatch.loop_index, structured);
    body.remove(dispatch.let_index);
}

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
    let mut structurer = Structurer::new(dispatch, n);
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

struct Structurer<'a> {
    dispatch: &'a DispatchLoop,
    n: usize,
    order_of: Vec<usize>,
    visited: BTreeSet<usize>,
}

impl<'a> Structurer<'a> {
    fn new(dispatch: &'a DispatchLoop, n: usize) -> Self {
        Structurer {
            dispatch,
            n,
            order_of: vec![usize::MAX; n],
            visited: BTreeSet::new(),
        }
    }

    fn succ(&self, state: usize) -> Vec<usize> {
        self.dispatch.states[state]
            .successors()
            .into_iter()
            .filter(|&t| t < self.n)
            .collect()
    }

    fn reachable(&self) -> BTreeSet<usize> {
        self.reach(&[self.dispatch.entry])
    }

    fn reach(&self, seeds: &[usize]) -> BTreeSet<usize> {
        let mut reached = BTreeSet::new();
        let mut stack: Vec<usize> = seeds.iter().copied().filter(|&t| t < self.n).collect();
        while let Some(state) = stack.pop() {
            if reached.insert(state) {
                stack.extend(self.succ(state));
            }
        }
        reached
    }

    fn has_cycle(&self) -> bool {
        let mut state = vec![0u8; self.n]; // 0 = unseen, 1 = on-stack, 2 = done
        let mut cycle = false;
        for start in 0..self.n {
            if state[start] == 0 {
                self.detect_cycle(start, &mut state, &mut cycle);
            }
        }
        cycle
    }

    fn detect_cycle(&self, node: usize, state: &mut [u8], cycle: &mut bool) {
        state[node] = 1;
        for next in self.succ(node) {
            match state[next] {
                1 => *cycle = true,
                0 => self.detect_cycle(next, state, cycle),
                _ => {}
            }
        }
        state[node] = 2;
    }

    fn compute_order(&mut self) {
        let mut order = Vec::new();
        let mut seen = BTreeSet::new();
        self.postorder(self.dispatch.entry, &mut seen, &mut order);
        order.reverse();
        for (rank, state) in order.iter().enumerate() {
            self.order_of[*state] = rank;
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
        if !self.visited.insert(state) {
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
                if stop.contains(target) {
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
            .min_by_key(|state| self.order_of[*state])
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
    matches!(
        stmts.last().map(|s| &s.stmt),
        Some(Stmt::Return(_) | Stmt::Break(_) | Stmt::Continue(_))
    )
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
