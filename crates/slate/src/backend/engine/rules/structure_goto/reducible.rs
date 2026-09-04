use std::collections::{BTreeMap, BTreeSet};

use super::{Dispatch, assigned_state, collect_uses, parse, state_literal, successors};
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{
    self, Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag,
};
use crate::backend::rust_ast::{Expr, Ident, Label, MatchArm, Pattern, RustValue, Stmt, UnaryOp};

pub(in crate::backend::engine) struct StructureReducible;

enum Term {
    Diverge,
    Jump(usize),
    Branch {
        cond: Expr,
        then_target: usize,
        else_target: usize,
    },
    Switch {
        selector: Expr,
        arms: Vec<(Pattern, usize)>,
    },
}

struct Block {
    state: i64,
    prefix: Vec<Stmt>,
    term: Term,
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum Owner {
    Inside(usize),
    AfterLoop(usize),
}

struct Graph {
    label: Label,
    blocks: Vec<Block>,
    entry: usize,
    order: Vec<usize>,
    rank: Vec<usize>,
    headers: BTreeSet<usize>,
    labeled: Vec<bool>,
    owner: Vec<Option<Owner>>,
    preamble: Vec<Stmt>,
    keeps_state: bool,
}

fn negate(cond: &Expr) -> Expr {
    match cond {
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => (**expr).clone(),
        _ => Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(cond.clone()),
        },
    }
}

fn is_exit_path(expr: &Expr) -> bool {
    let Expr::Call { func, .. } = expr else {
        return false;
    };
    let Expr::Path(path) = func.as_ref() else {
        return false;
    };
    let segments: Vec<&str> = path.segments.iter().map(|s| s.as_str()).collect();
    matches!(
        segments.as_slice(),
        ["std", "process", "exit"] | ["std", "process", "abort"]
    )
}

fn diverges(arena: &Arena, id: NodeId) -> bool {
    let Some(kind) = arena.get(id) else {
        return false;
    };
    match kind {
        NodeKind::Return(_) => true,
        NodeKind::Expr(Expr::Macro { name, .. }) => name == "unreachable" || name == "panic",
        NodeKind::Expr(expr) => is_exit_path(expr),
        NodeKind::If {
            then_body,
            else_body,
            ..
        } => tail_diverges(arena, then_body) && tail_diverges(arena, else_body),
        NodeKind::Match { arms, .. } => arms.iter().all(|arm| tail_diverges(arena, &arm.body)),
        NodeKind::Unsafe { stmts, tail: None } => tail_diverges(arena, stmts),
        NodeKind::Block { stmts, tail: None } | NodeKind::Scope { body: stmts } => {
            tail_diverges(arena, stmts)
        }
        _ => false,
    }
}

fn tail_diverges(arena: &Arena, body: &[NodeId]) -> bool {
    body.last().is_some_and(|&id| diverges(arena, id))
}

fn escapes_dispatch(arena: &Arena, dispatch: &Dispatch, body: &[NodeId]) -> bool {
    body.iter().any(|&id| {
        let Some(kind) = arena.get(id) else {
            return true;
        };
        match kind {
            NodeKind::Break(label) | NodeKind::Continue(label) => {
                label.as_ref().is_none_or(|label| *label == dispatch.label)
            }
            NodeKind::Loop { .. } | NodeKind::While { .. } | NodeKind::For { .. } => kind
                .child_lists()
                .into_iter()
                .flatten()
                .any(|&child| labelled_escape(arena, dispatch, child)),
            _ => kind
                .child_lists()
                .into_iter()
                .any(|list| escapes_dispatch(arena, dispatch, list)),
        }
    })
}

fn labelled_escape(arena: &Arena, dispatch: &Dispatch, id: NodeId) -> bool {
    let Some(kind) = arena.get(id) else {
        return true;
    };
    match kind {
        NodeKind::Break(Some(label)) | NodeKind::Continue(Some(label)) => *label == dispatch.label,
        _ => kind
            .child_lists()
            .into_iter()
            .flatten()
            .any(|&child| labelled_escape(arena, dispatch, child)),
    }
}

fn jump_target(
    arena: &Arena,
    dispatch: &Dispatch,
    body: &[NodeId],
    index_of: &BTreeMap<i64, usize>,
) -> Option<usize> {
    let [only] = body[..] else {
        return None;
    };
    index_of
        .get(&assigned_state(arena, only, dispatch.state)?)
        .copied()
}

fn terminator(
    arena: &Arena,
    dispatch: &Dispatch,
    body: &[NodeId],
    index_of: &BTreeMap<i64, usize>,
) -> Option<(usize, Term)> {
    let (&last, head) = body.split_last()?;
    let continues = matches!(arena.get(last), Some(NodeKind::Continue(Some(label))) if *label == dispatch.label);
    if !continues {
        return tail_diverges(arena, body).then_some((body.len(), Term::Diverge));
    }
    let (&exit, prefix) = head.split_last()?;
    let term = match arena.get(exit)? {
        NodeKind::Assign { .. } => Term::Jump(
            index_of
                .get(&assigned_state(arena, exit, dispatch.state)?)
                .copied()?,
        ),
        NodeKind::If {
            cond,
            then_body,
            else_body,
        } => Term::Branch {
            cond: cond.clone(),
            then_target: jump_target(arena, dispatch, then_body, index_of)?,
            else_target: jump_target(arena, dispatch, else_body, index_of)?,
        },
        NodeKind::Match { expr, arms } => {
            let mut targets = Vec::with_capacity(arms.len());
            for arm in arms {
                targets.push((
                    arm.pattern.clone(),
                    jump_target(arena, dispatch, &arm.body, index_of)?,
                ));
            }
            Term::Switch {
                selector: expr.clone(),
                arms: targets,
            }
        }
        _ => return None,
    };
    Some((prefix.len(), term))
}

fn edges(term: &Term) -> Vec<usize> {
    match term {
        Term::Diverge => Vec::new(),
        Term::Jump(target) => vec![*target],
        Term::Branch {
            then_target,
            else_target,
            ..
        } => vec![*then_target, *else_target],
        Term::Switch { arms, .. } => arms.iter().map(|(_, target)| *target).collect(),
    }
}

fn reverse_postorder(blocks: &[Block], entry: usize) -> Vec<usize> {
    let mut seen = vec![false; blocks.len()];
    let mut order = Vec::new();
    let mut stack = vec![(entry, 0usize)];
    seen[entry] = true;
    while let Some(&mut (node, ref mut next)) = stack.last_mut() {
        let successors = edges(&blocks[node].term);
        if *next < successors.len() {
            let child = successors[*next];
            *next += 1;
            if !seen[child] {
                seen[child] = true;
                stack.push((child, 0));
            }
            continue;
        }
        order.push(node);
        stack.pop();
    }
    order.reverse();
    order
}

fn predecessors(blocks: &[Block], order: &[usize]) -> Vec<Vec<usize>> {
    let mut preds: Vec<Vec<usize>> = vec![Vec::new(); blocks.len()];
    for &node in order {
        for target in edges(&blocks[node].term) {
            preds[target].push(node);
        }
    }
    preds
}

fn dominators(
    blocks: &[Block],
    entry: usize,
    order: &[usize],
    rank: &[usize],
    preds: &[Vec<usize>],
) -> Vec<usize> {
    let undefined = blocks.len();
    let mut idom = vec![undefined; blocks.len()];
    idom[entry] = entry;
    let mut changed = true;
    while changed {
        changed = false;
        for &node in order.iter().skip(1) {
            let mut candidate = undefined;
            for &pred in &preds[node] {
                if idom[pred] == undefined {
                    continue;
                }
                candidate = match candidate {
                    n if n == undefined => pred,
                    n => intersect(&idom, rank, pred, n),
                };
            }
            if candidate != undefined && idom[node] != candidate {
                idom[node] = candidate;
                changed = true;
            }
        }
    }
    idom
}

fn intersect(idom: &[usize], rank: &[usize], mut a: usize, mut b: usize) -> usize {
    while a != b {
        while rank[a] > rank[b] {
            a = idom[a];
        }
        while rank[b] > rank[a] {
            b = idom[b];
        }
    }
    a
}

fn dominates(idom: &[usize], entry: usize, ancestor: usize, mut node: usize) -> bool {
    loop {
        if node == ancestor {
            return true;
        }
        if node == entry {
            return false;
        }
        node = idom[node];
    }
}

fn natural_loop(preds: &[Vec<usize>], header: usize, tails: &[usize]) -> BTreeSet<usize> {
    let mut body = BTreeSet::from([header]);
    let mut stack: Vec<usize> = tails.to_vec();
    while let Some(node) = stack.pop() {
        if !body.insert(node) {
            continue;
        }
        stack.extend(preds[node].iter().copied());
    }
    body
}

fn is_reducible(blocks: &[Block], entry: usize) -> bool {
    let order = reverse_postorder(blocks, entry);
    let mut rank = vec![usize::MAX; blocks.len()];
    for (position, &node) in order.iter().enumerate() {
        rank[node] = position;
    }
    let preds = predecessors(blocks, &order);
    let idom = dominators(blocks, entry, &order, &rank, &preds);
    reducible(blocks, &idom, entry)
}

fn sccs(blocks: &[Block]) -> Vec<Vec<usize>> {
    let count = blocks.len();
    let mut index = vec![usize::MAX; count];
    let mut low = vec![0usize; count];
    let mut on_stack = vec![false; count];
    let mut pending: Vec<usize> = Vec::new();
    let mut next = 0usize;
    let mut components = Vec::new();
    let mut calls: Vec<(usize, usize)> = Vec::new();

    for start in 0..count {
        if index[start] != usize::MAX {
            continue;
        }
        index[start] = next;
        low[start] = next;
        next += 1;
        pending.push(start);
        on_stack[start] = true;
        calls.push((start, 0));
        while let Some(&mut (node, ref mut cursor)) = calls.last_mut() {
            let successors = edges(&blocks[node].term);
            if *cursor < successors.len() {
                let child = successors[*cursor];
                *cursor += 1;
                if index[child] == usize::MAX {
                    index[child] = next;
                    low[child] = next;
                    next += 1;
                    pending.push(child);
                    on_stack[child] = true;
                    calls.push((child, 0));
                } else if on_stack[child] {
                    low[node] = low[node].min(index[child]);
                }
                continue;
            }
            calls.pop();
            if let Some(&(parent, _)) = calls.last() {
                low[parent] = low[parent].min(low[node]);
            }
            if low[node] == index[node] {
                let mut component = Vec::new();
                while let Some(top) = pending.pop() {
                    on_stack[top] = false;
                    component.push(top);
                    if top == node {
                        break;
                    }
                }
                components.push(component);
            }
        }
    }
    components
}

fn retarget_term(term: &mut Term, from: usize, to: usize) {
    let targets: Vec<&mut usize> = match term {
        Term::Diverge => Vec::new(),
        Term::Jump(target) => vec![target],
        Term::Branch {
            then_target,
            else_target,
            ..
        } => vec![then_target, else_target],
        Term::Switch { arms, .. } => arms.iter_mut().map(|(_, target)| target).collect(),
    };
    for target in targets {
        if *target == from {
            *target = to;
        }
    }
}

fn split_irreducible(blocks: &mut Vec<Block>, entry: usize, state: Ident) -> Option<usize> {
    let components = sccs(blocks);
    let preds = predecessors(blocks, &(0..blocks.len()).collect::<Vec<_>>());
    let mut next_state = blocks.iter().map(|block| block.state).max()? + 1;
    let mut new_entry = entry;

    for component in components {
        if component.len() < 2 {
            continue;
        }
        let members: BTreeSet<usize> = component.iter().copied().collect();
        let mut entries: Vec<usize> = component
            .iter()
            .copied()
            .filter(|&node| node == entry || preds[node].iter().any(|pred| !members.contains(pred)))
            .collect();
        if entries.len() < 2 {
            continue;
        }
        entries.sort_unstable();

        let header = blocks.len();
        let arms = entries
            .iter()
            .enumerate()
            .map(|(position, &target)| {
                let pattern = match position + 1 == entries.len() {
                    true => Pattern::Wildcard,
                    false => Pattern::I64(blocks[target].state),
                };
                (pattern, target)
            })
            .collect();
        blocks.push(Block {
            state: next_state,
            prefix: Vec::new(),
            term: Term::Switch {
                selector: Expr::Var(state),
                arms,
            },
        });
        next_state += 1;

        for source in 0..header {
            let reached: BTreeSet<usize> = edges(&blocks[source].term)
                .into_iter()
                .filter(|target| entries.contains(target))
                .collect();
            for target in reached {
                let trampoline = blocks.len();
                blocks.push(Block {
                    state: next_state,
                    prefix: vec![Stmt::Assign {
                        target: Expr::Var(state),
                        value: Expr::Value(RustValue::I64(blocks[target].state)),
                    }],
                    term: Term::Jump(header),
                });
                next_state += 1;
                retarget_term(&mut blocks[source].term, target, trampoline);
            }
        }

        if members.contains(&entry) {
            new_entry = header;
        }
    }
    (new_entry != entry || blocks.len() > preds.len()).then_some(new_entry)
}

fn build(arena: &Arena, dispatch: &Dispatch, entry_state: i64) -> Option<Graph> {
    let mut index_of = BTreeMap::new();
    for (index, (state, _)) in dispatch.arms.iter().enumerate() {
        if index_of.insert(*state, index).is_some() {
            return None;
        }
    }
    let entry = index_of.get(&entry_state).copied()?;

    let mut blocks = Vec::with_capacity(dispatch.arms.len());
    for (state, body) in &dispatch.arms {
        let (prefix_len, term) = terminator(arena, dispatch, body, &index_of)?;
        let prefix = &body[..prefix_len];
        let mut writes = BTreeSet::new();
        successors(arena, dispatch, prefix, &mut writes);
        if !writes.is_empty() || escapes_dispatch(arena, dispatch, prefix) {
            return None;
        }
        blocks.push(Block {
            state: *state,
            prefix: arena::reify_bodies(arena, prefix),
            term,
        });
    }

    let mut entry = entry;
    let mut preamble = Vec::new();
    let mut keeps_state = false;
    if !is_reducible(&blocks, entry) {
        let split = split_irreducible(&mut blocks, entry, dispatch.state)?;
        if split != entry {
            preamble.push(Stmt::Assign {
                target: Expr::Var(dispatch.state),
                value: Expr::Value(RustValue::I64(entry_state)),
            });
            entry = split;
        }
        keeps_state = true;
        if !is_reducible(&blocks, entry) {
            return None;
        }
    }

    let order = reverse_postorder(&blocks, entry);
    let mut rank = vec![usize::MAX; blocks.len()];
    for (position, &node) in order.iter().enumerate() {
        rank[node] = position;
    }
    let preds = predecessors(&blocks, &order);
    let idom = dominators(&blocks, entry, &order, &rank, &preds);

    let mut tails: BTreeMap<usize, Vec<usize>> = BTreeMap::new();
    let mut forward: Vec<usize> = vec![0; blocks.len()];
    for &node in &order {
        for target in edges(&blocks[node].term) {
            match dominates(&idom, entry, target, node) {
                true => tails.entry(target).or_default().push(node),
                false => forward[target] += 1,
            }
        }
    }

    let headers: BTreeSet<usize> = tails.keys().copied().collect();
    let bodies: BTreeMap<usize, BTreeSet<usize>> = tails
        .iter()
        .map(|(&header, tails)| (header, natural_loop(&preds, header, tails)))
        .collect();

    let mut labeled = vec![false; blocks.len()];
    for &node in &order {
        labeled[node] = forward[node] > 1 || escapes_a_loop(&bodies, &preds, &idom, entry, node);
    }
    labeled[entry] = false;

    let mut owner = vec![None; blocks.len()];
    for &node in &order {
        if labeled[node] {
            owner[node] = Some(owner_of(&bodies, &idom, node));
        }
    }
    inline_secondary_exits(&blocks, &headers, &rank, &forward, &mut labeled, &mut owner);

    Some(Graph {
        label: dispatch.label.clone(),
        blocks,
        entry,
        order,
        rank,
        headers,
        labeled,
        owner,
        preamble,
        keeps_state,
    })
}

fn inline_secondary_exits(
    blocks: &[Block],
    headers: &BTreeSet<usize>,
    rank: &[usize],
    forward: &[usize],
    labeled: &mut [bool],
    owner: &mut [Option<Owner>],
) {
    for &header in headers {
        let mut follows: Vec<usize> = (0..labeled.len())
            .filter(|&node| owner[node] == Some(Owner::AfterLoop(header)))
            .collect();
        let Some(&innermost) = follows.iter().min_by_key(|&&node| rank[node]) else {
            continue;
        };
        follows.retain(|&node| {
            node != innermost && forward[node] == 1 && matches!(blocks[node].term, Term::Diverge)
        });
        for node in follows {
            labeled[node] = false;
            owner[node] = None;
        }
    }
}

fn reducible(blocks: &[Block], idom: &[usize], entry: usize) -> bool {
    let mut state = vec![0u8; blocks.len()];
    let mut stack = vec![(entry, 0usize)];
    state[entry] = 1;
    while let Some(&mut (node, ref mut next)) = stack.last_mut() {
        let successors: Vec<usize> = edges(&blocks[node].term)
            .into_iter()
            .filter(|&target| !dominates(idom, entry, target, node))
            .collect();
        if *next < successors.len() {
            let child = successors[*next];
            *next += 1;
            match state[child] {
                0 => {
                    state[child] = 1;
                    stack.push((child, 0));
                }
                1 => return false,
                _ => {}
            }
            continue;
        }
        state[node] = 2;
        stack.pop();
    }
    true
}

fn escapes_a_loop(
    bodies: &BTreeMap<usize, BTreeSet<usize>>,
    preds: &[Vec<usize>],
    idom: &[usize],
    entry: usize,
    node: usize,
) -> bool {
    bodies.iter().any(|(_, body)| {
        !body.contains(&node)
            && preds[node]
                .iter()
                .any(|&pred| body.contains(&pred) && !dominates(idom, entry, node, pred))
    })
}

fn owner_of(bodies: &BTreeMap<usize, BTreeSet<usize>>, idom: &[usize], node: usize) -> Owner {
    let home = idom[node];
    let mut enclosing: Vec<(usize, usize)> = bodies
        .iter()
        .filter(|(_, body)| body.contains(&home))
        .map(|(&header, body)| (body.len(), header))
        .collect();
    enclosing.sort_unstable();
    enclosing.reverse();
    for (_, header) in enclosing {
        if !bodies[&header].contains(&node) {
            return Owner::AfterLoop(header);
        }
    }
    Owner::Inside(home)
}

fn drop_tail_continue(body: &mut Vec<Stmt>) {
    match body.last_mut() {
        Some(Stmt::Continue(None)) => {
            body.pop();
        }
        Some(Stmt::If {
            then_body,
            else_body,
            ..
        }) => {
            drop_tail_continue(then_body);
            drop_tail_continue(else_body);
        }
        Some(Stmt::Match { arms, .. }) => {
            for arm in arms {
                drop_tail_continue(&mut arm.body);
            }
        }
        Some(Stmt::LabeledBlock { body, .. }) | Some(Stmt::Scope { body }) => {
            drop_tail_continue(body)
        }
        _ => {}
    }
}

fn fold_empty_branches(body: &mut Vec<Stmt>) {
    body.retain_mut(|stmt| {
        for nested in stmt.child_bodies_mut() {
            fold_empty_branches(nested);
        }
        let Stmt::If {
            cond,
            then_body,
            else_body,
        } = stmt
        else {
            return true;
        };
        if !then_body.is_empty() {
            return true;
        }
        if else_body.is_empty() {
            return false;
        }
        *stmt = Stmt::If {
            cond: negate(cond),
            then_body: std::mem::take(else_body),
            else_body: Vec::new(),
        };
        true
    });
}

fn uses_label(body: &mut [Stmt], label: &Label) -> bool {
    body.iter_mut().any(|stmt| {
        matches!(&*stmt, Stmt::Break(Some(target)) | Stmt::Continue(Some(target)) if target == label)
            || stmt
                .child_bodies_mut()
                .into_iter()
                .any(|nested| uses_label(nested, label))
    })
}

impl Graph {
    fn suffix(&self) -> &str {
        self.label.as_str().trim_start_matches("__dispatch")
    }

    fn join_label(&self, node: usize) -> Label {
        Label::new(format!(
            "__join{}_{}",
            self.suffix(),
            self.blocks[node].state
        ))
    }

    fn loop_label(&self, node: usize) -> Label {
        Label::new(format!(
            "__loop{}_{}",
            self.suffix(),
            self.blocks[node].state
        ))
    }

    fn owned(&self, node: usize, kind: Owner) -> Vec<usize> {
        let mut owned: Vec<usize> = self
            .order
            .iter()
            .copied()
            .filter(|&other| self.labeled[other] && self.owner[other] == Some(kind))
            .collect();
        owned.retain(|&other| other != node);
        owned.sort_by_key(|&other| std::cmp::Reverse(self.rank[other]));
        owned
    }

    fn tree(&self, node: usize, loops: &[usize]) -> Vec<Stmt> {
        if !self.headers.contains(&node) {
            return self.within(node, &self.owned(node, Owner::Inside(node)), loops);
        }
        let mut nested = loops.to_vec();
        nested.push(node);
        let label = self.loop_label(node);
        let mut body = self.within(node, &self.owned(node, Owner::Inside(node)), &nested);
        drop_tail_continue(&mut body);
        fold_empty_branches(&mut body);
        let core = Stmt::Loop {
            label: uses_label(&mut body, &label).then(|| label.clone()),
            body,
        };
        self.wrap(vec![core], &self.owned(node, Owner::AfterLoop(node)), loops)
    }

    fn wrap(&self, core: Vec<Stmt>, follows: &[usize], loops: &[usize]) -> Vec<Stmt> {
        let Some((&innermost, outer)) = follows.split_last() else {
            return core;
        };
        let mut inner = core;
        inner.extend(self.tree(innermost, loops));
        self.wrap_labeled(inner, outer, loops)
    }

    fn wrap_labeled(&self, inner: Vec<Stmt>, follows: &[usize], loops: &[usize]) -> Vec<Stmt> {
        let Some((&next, outer)) = follows.split_last() else {
            return inner;
        };
        let mut out = vec![Stmt::LabeledBlock {
            label: self.join_label(next),
            body: inner,
        }];
        out.extend(self.tree(next, loops));
        self.wrap_labeled(out, outer, loops)
    }

    fn within(&self, node: usize, joins: &[usize], loops: &[usize]) -> Vec<Stmt> {
        let Some((&outermost, inner_joins)) = joins.split_first() else {
            return self.block(node, loops);
        };
        let inner = self.within(node, inner_joins, loops);
        let mut out = vec![Stmt::LabeledBlock {
            label: self.join_label(outermost),
            body: inner,
        }];
        out.extend(self.tree(outermost, loops));
        out
    }

    fn block(&self, node: usize, loops: &[usize]) -> Vec<Stmt> {
        let block = &self.blocks[node];
        let mut out = block.prefix.clone();
        match &block.term {
            Term::Diverge => {}
            Term::Jump(target) => out.extend(self.branch(*target, loops)),
            Term::Branch {
                cond,
                then_target,
                else_target,
            } => out.push(Stmt::If {
                cond: cond.clone(),
                then_body: self.branch(*then_target, loops),
                else_body: self.branch(*else_target, loops),
            }),
            Term::Switch { selector, arms } => out.push(Stmt::Match {
                expr: selector.clone(),
                arms: arms
                    .iter()
                    .map(|(pattern, target)| MatchArm {
                        pattern: pattern.clone(),
                        body: self.branch(*target, loops),
                    })
                    .collect(),
            }),
        }
        out
    }

    fn branch(&self, target: usize, loops: &[usize]) -> Vec<Stmt> {
        if self.headers.contains(&target) && loops.contains(&target) {
            let innermost = loops.last() == Some(&target);
            return vec![Stmt::Continue(
                (!innermost).then(|| self.loop_label(target)),
            )];
        }
        if !self.labeled[target] {
            return self.tree(target, loops);
        }
        match self.owner[target] {
            Some(Owner::AfterLoop(header)) if self.exits_directly(target, header) => {
                let innermost = loops.last() == Some(&header);
                vec![Stmt::Break((!innermost).then(|| self.loop_label(header)))]
            }
            _ => vec![Stmt::Break(Some(self.join_label(target)))],
        }
    }

    fn exits_directly(&self, target: usize, header: usize) -> bool {
        self.owned(header, Owner::AfterLoop(header)).last() == Some(&target)
    }

    fn emit(&self) -> Vec<Stmt> {
        let mut out = self.preamble.clone();
        out.extend(self.tree(self.entry, &[]));
        out
    }
}

fn remove_stmt(arena: &mut FunctionOptimizer, id: NodeId) {
    if let Some(parent) = arena.parent(id)
        && let Some(kind) = arena.get_mut(parent)
    {
        for list in kind.child_lists_mut() {
            if let Some(position) = list.iter().position(|&child| child == id) {
                list.remove(position);
                break;
            }
        }
    }
    arena.discard_subtree(id);
}

fn graph_of(arena: &Arena, id: NodeId) -> Option<(Dispatch, NodeId, Graph)> {
    let dispatch = parse(arena, id)?;
    let uses = collect_uses(arena, &dispatch)?;
    let NodeKind::Let {
        init: Some(init), ..
    } = arena.get(uses.entry)?
    else {
        return None;
    };
    let graph = build(arena, &dispatch, state_literal(init)?)?;
    Some((dispatch, uses.entry, graph))
}

impl NodeRule for StructureReducible {
    fn name(&self) -> &'static str {
        "structure_goto::reducible"
    }

    fn priority(&self) -> u32 {
        2
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Loop]
    }

    fn requeues_moved_nodes(&self) -> bool {
        true
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        graph_of(arena, id).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some((dispatch, entry, graph)) = graph_of(arena, id) else {
            return false;
        };
        let keeps_state = graph.keeps_state;
        let stmts = graph.emit();
        arena.discard_subtree(dispatch.match_id);
        let body = arena::insert_stmts(arena, Some(id), stmts);
        arena.set_kind(id, NodeKind::Scope { body });
        if !keeps_state {
            remove_stmt(arena, entry);
        }
        arena.touch_subtree(id);
        true
    }
}
