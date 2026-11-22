use std::collections::{BTreeMap, BTreeSet};

use crate::backend::rust_ast::{
    Block as RustBlock, Expr, Ident, Label, MatchArm, Pattern, RustValue, Stmt, UnaryOp,
};

pub(super) struct Structured {
    pub(super) body: Vec<Stmt>,
    pub(super) keeps_state: bool,
}

pub(super) fn structure(
    arms: &[(i64, Vec<Stmt>)],
    entry_state: i64,
    label: &Label,
    state: Ident,
) -> Option<Structured> {
    let dispatch = Dispatch { label, state };
    if !dispatch.state_only_steers(arms) {
        return None;
    }
    let graph = build(&dispatch, arms, entry_state)?;
    Some(Structured {
        keeps_state: graph.keeps_state,
        body: graph.emit(),
    })
}

struct Dispatch<'a> {
    label: &'a Label,
    state: Ident,
}

enum Term {
    Diverge,
    Escape(Vec<Stmt>),
    Exit,
    Jump(usize),
    Branch {
        cond: Expr,
        then_term: Box<Term>,
        else_term: Box<Term>,
    },
    Switch {
        selector: Expr,
        arms: Vec<(Pattern, Term)>,
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

fn bodies(stmt: &Stmt) -> Vec<&Vec<Stmt>> {
    match stmt {
        Stmt::Cfg { body, .. } => vec![body],
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        }
        | Stmt::If {
            then_body,
            else_body,
            ..
        } => vec![then_body, else_body],
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body, .. }
        | Stmt::LabeledBlock { body, .. } => vec![body],
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => vec![&body.stmts],
        Stmt::Match { arms, .. } => arms.iter().map(|arm| &arm.body).collect(),
        Stmt::Comment(_)
        | Stmt::Let { .. }
        | Stmt::Assign { .. }
        | Stmt::CompoundAssign { .. }
        | Stmt::InlineAsm(_)
        | Stmt::Expr(_)
        | Stmt::Return(_)
        | Stmt::Break(_)
        | Stmt::Continue(_) => Vec::new(),
    }
}

fn state_literal(expr: &Expr) -> Option<i64> {
    match expr {
        Expr::Value(RustValue::I64(value)) => Some(*value),
        Expr::Value(RustValue::Usize(value)) => i64::try_from(*value).ok(),
        _ => None,
    }
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
        ["std", "process", "exit"]
            | ["std", "process", "abort"]
            | ["core", "hint", "unreachable_unchecked"]
    )
}

fn diverges(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Return(_) => true,
        Stmt::Expr(Expr::Macro { name, .. }) => name == "unreachable" || name == "panic",
        Stmt::Expr(expr) => is_exit_path(expr),
        Stmt::If {
            then_body,
            else_body,
            ..
        } => tail_diverges(then_body) && tail_diverges(else_body),
        Stmt::Match { arms, .. } => arms.iter().all(|arm| tail_diverges(&arm.body)),
        Stmt::Unsafe {
            body: RustBlock { stmts, tail: None },
        }
        | Stmt::Block(RustBlock { stmts, tail: None })
        | Stmt::Scope { body: stmts, .. } => tail_diverges(stmts),
        _ => false,
    }
}

fn tail_diverges(body: &[Stmt]) -> bool {
    body.last().is_some_and(diverges)
}

impl Dispatch<'_> {
    fn assigned_state(&self, stmt: &Stmt) -> Option<i64> {
        let Stmt::Assign {
            target: Expr::Var(target),
            value,
        } = stmt
        else {
            return None;
        };
        (*target == self.state).then(|| state_literal(value))?
    }

    fn state_only_steers(&self, arms: &[(i64, Vec<Stmt>)]) -> bool {
        arms.iter().all(|(_, body)| self.steers(body))
    }

    fn steers(&self, body: &[Stmt]) -> bool {
        body.iter().all(|stmt| {
            if self.assigned_state(stmt).is_some() {
                return true;
            }
            let mut own = stmt.clone();
            for nested in own.child_bodies_mut() {
                nested.clear();
            }
            let mut reads = Vec::new();
            own.collect_vars(&mut reads);
            !reads.contains(&self.state) && bodies(stmt).into_iter().all(|list| self.steers(list))
        })
    }

    fn successors(&self, body: &[Stmt], out: &mut BTreeSet<i64>) {
        for stmt in body {
            if let Some(target) = self.assigned_state(stmt) {
                out.insert(target);
            }
            for list in bodies(stmt) {
                self.successors(list, out);
            }
        }
    }

    fn escapes(&self, body: &[Stmt]) -> bool {
        body.iter().any(|stmt| match stmt {
            Stmt::Break(label) | Stmt::Continue(label) => {
                label.as_ref().is_none_or(|label| label == self.label)
            }
            Stmt::Loop { .. } | Stmt::While { .. } | Stmt::For { .. } => bodies(stmt)
                .into_iter()
                .flatten()
                .any(|child| self.labelled_escape(child)),
            _ => bodies(stmt).into_iter().any(|list| self.escapes(list)),
        })
    }

    fn labelled_escape(&self, stmt: &Stmt) -> bool {
        match stmt {
            Stmt::Break(Some(label)) | Stmt::Continue(Some(label)) => label == self.label,
            _ => bodies(stmt)
                .into_iter()
                .flatten()
                .any(|child| self.labelled_escape(child)),
        }
    }

    fn is_continue(&self, stmt: &Stmt) -> bool {
        matches!(stmt, Stmt::Continue(Some(label)) if label == self.label)
    }
}

struct Extractor<'a> {
    dispatch: &'a Dispatch<'a>,
    index_of: &'a BTreeMap<i64, usize>,
}

impl Extractor<'_> {
    fn target(&self, body: &[Stmt]) -> Option<Term> {
        let assign = match body {
            [assign] => Some(assign),
            [assign, jump] if self.dispatch.is_continue(jump) => Some(assign),
            _ => None,
        };
        if let Some(assign) = assign
            && let Some(state) = self.dispatch.assigned_state(assign)
        {
            return self.index_of.get(&state).copied().map(Term::Jump);
        }
        if tail_diverges(body) {
            return self.prefix(body).map(Term::Escape);
        }
        let (prefix_len, term) = self.terminator(body)?;
        (prefix_len == 0).then_some(term)
    }

    fn prefix(&self, body: &[Stmt]) -> Option<Vec<Stmt>> {
        let mut writes = BTreeSet::new();
        self.dispatch.successors(body, &mut writes);
        if !writes.is_empty() || self.dispatch.escapes(body) {
            return None;
        }
        Some(body.to_vec())
    }

    fn terminator(&self, body: &[Stmt]) -> Option<(usize, Term)> {
        let (last, head) = body.split_last()?;
        if tail_diverges(body) {
            return Some((body.len(), Term::Diverge));
        }
        if matches!(last, Stmt::Break(Some(label)) if label == self.dispatch.label) {
            return Some((head.len(), Term::Exit));
        }
        let continues = self.dispatch.is_continue(last);
        let (exit, prefix) = if continues {
            let (exit, prefix) = head.split_last()?;
            (exit, prefix)
        } else {
            (last, head)
        };
        let term = match exit {
            Stmt::Assign { .. } if continues => {
                Term::Jump(*self.index_of.get(&self.dispatch.assigned_state(exit)?)?)
            }
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => Term::Branch {
                cond: cond.clone(),
                then_term: Box::new(self.target(then_body)?),
                else_term: Box::new(self.target(else_body)?),
            },
            Stmt::Match { expr, arms } => {
                let mut targets = Vec::with_capacity(arms.len());
                for arm in arms {
                    targets.push((arm.pattern.clone(), self.target(&arm.body)?));
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
}

fn edges(term: &Term) -> Vec<usize> {
    match term {
        Term::Diverge | Term::Escape(_) | Term::Exit => Vec::new(),
        Term::Jump(target) => vec![*target],
        Term::Branch {
            then_term,
            else_term,
            ..
        } => {
            let mut out = edges(then_term);
            out.extend(edges(else_term));
            out
        }
        Term::Switch { arms, .. } => arms.iter().flat_map(|(_, term)| edges(term)).collect(),
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

fn sccs(blocks: &[Block], members: &BTreeSet<usize>) -> Vec<Vec<usize>> {
    let count = blocks.len();
    let mut index = vec![usize::MAX; count];
    let mut low = vec![0usize; count];
    let mut on_stack = vec![false; count];
    let mut pending: Vec<usize> = Vec::new();
    let mut next = 0usize;
    let mut components = Vec::new();
    let mut calls: Vec<(usize, usize)> = Vec::new();

    for &start in members {
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
            let successors: Vec<usize> = edges(&blocks[node].term)
                .into_iter()
                .filter(|target| members.contains(target))
                .collect();
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
    match term {
        Term::Diverge | Term::Escape(_) | Term::Exit => {}
        Term::Jump(target) if *target == from => *target = to,
        Term::Jump(_) => {}
        Term::Branch {
            then_term,
            else_term,
            ..
        } => {
            retarget_term(then_term, from, to);
            retarget_term(else_term, from, to);
        }
        Term::Switch { arms, .. } => {
            for (_, term) in arms {
                retarget_term(term, from, to);
            }
        }
    }
}

fn split_irreducible(blocks: &mut Vec<Block>, entry: usize, state: Ident) -> Option<usize> {
    let original = blocks.len();
    let mut next_state = blocks.iter().map(|block| block.state).max()? + 1;
    let mut new_entry = entry;
    let mut regions: Vec<BTreeSet<usize>> = vec![(0..original).collect()];

    while let Some(region) = regions.pop() {
        for component in sccs(blocks, &region) {
            if component.len() < 2 {
                continue;
            }
            let members: BTreeSet<usize> = component.iter().copied().collect();
            let preds = predecessors(blocks, &(0..blocks.len()).collect::<Vec<_>>());
            let mut entries: Vec<usize> = component
                .iter()
                .copied()
                .filter(|&node| {
                    node == new_entry || preds[node].iter().any(|pred| !members.contains(pred))
                })
                .collect();
            entries.sort_unstable();
            if entries.len() < 2 {
                let mut inner = members;
                for header in &entries {
                    inner.remove(header);
                }
                regions.push(inner);
                continue;
            }

            let header = blocks.len();
            let arms = entries
                .iter()
                .enumerate()
                .map(|(position, &target)| {
                    let pattern = match position + 1 == entries.len() {
                        true => Pattern::Wildcard,
                        false => Pattern::I64(blocks[target].state),
                    };
                    (pattern, Term::Jump(target))
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

            if members.contains(&new_entry) {
                new_entry = header;
            }
            regions.push(members);
        }
    }
    (new_entry != entry || blocks.len() > original).then_some(new_entry)
}

fn build(dispatch: &Dispatch, arms: &[(i64, Vec<Stmt>)], entry_state: i64) -> Option<Graph> {
    let mut index_of = BTreeMap::new();
    for (index, (state, _)) in arms.iter().enumerate() {
        if index_of.insert(*state, index).is_some() {
            return None;
        }
    }
    let entry = index_of.get(&entry_state).copied()?;

    let extractor = Extractor {
        dispatch,
        index_of: &index_of,
    };
    let mut blocks = Vec::with_capacity(arms.len());
    for (state, body) in arms {
        let (prefix_len, term) = extractor.terminator(body)?;
        blocks.push(Block {
            state: *state,
            prefix: extractor.prefix(&body[..prefix_len])?,
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
    let loop_bodies: BTreeMap<usize, BTreeSet<usize>> = tails
        .iter()
        .map(|(&header, tails)| (header, natural_loop(&preds, header, tails)))
        .collect();

    let mut labeled = vec![false; blocks.len()];
    for &node in &order {
        labeled[node] =
            forward[node] > 1 || escapes_a_loop(&loop_bodies, &preds, &idom, entry, node);
    }
    labeled[entry] = false;

    let mut owner = vec![None; blocks.len()];
    for &node in &order {
        if labeled[node] {
            owner[node] = Some(owner_of(&loop_bodies, &idom, node));
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
    loop_bodies: &BTreeMap<usize, BTreeSet<usize>>,
    preds: &[Vec<usize>],
    idom: &[usize],
    entry: usize,
    node: usize,
) -> bool {
    loop_bodies.iter().any(|(_, body)| {
        !body.contains(&node)
            && preds[node]
                .iter()
                .any(|&pred| body.contains(&pred) && !dominates(idom, entry, node, pred))
    })
}

fn owner_of(loop_bodies: &BTreeMap<usize, BTreeSet<usize>>, idom: &[usize], node: usize) -> Owner {
    let home = idom[node];
    let mut enclosing: Vec<(usize, usize)> = loop_bodies
        .iter()
        .filter(|(_, body)| body.contains(&home))
        .map(|(&header, body)| (body.len(), header))
        .collect();
    enclosing.sort_unstable();
    enclosing.reverse();
    for (_, header) in enclosing {
        if !loop_bodies[&header].contains(&node) {
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
        Some(Stmt::LabeledBlock { body, .. }) | Some(Stmt::Scope { body, .. }) => {
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

fn header_test(body: &mut Vec<Stmt>) -> Option<Expr> {
    let only = body.len() == 1;
    let Some(Stmt::If {
        cond,
        then_body,
        else_body,
    }) = body.first_mut()
    else {
        return None;
    };
    if matches!(then_body[..], [Stmt::Break(None)]) && else_body.is_empty() {
        let cond = negate(cond);
        body.remove(0);
        return Some(cond);
    }
    if only && matches!(else_body[..], [Stmt::Break(None)]) {
        let cond = std::mem::replace(cond, Expr::Value(RustValue::Bool(true)));
        let stmts = std::mem::take(then_body);
        *body = stmts;
        return Some(cond);
    }
    None
}

pub(super) fn label_needed(body: &mut [Stmt], label: &Label) -> bool {
    if crate::backend::engine::stmts_have_stmt_exprs(body) || nested_jump(body, label, false) {
        return true;
    }
    elide_local_label(body, label, false);
    false
}

fn nested_jump(body: &[Stmt], label: &Label, nested: bool) -> bool {
    body.iter().any(|stmt| match stmt {
        Stmt::Break(Some(target)) | Stmt::Continue(Some(target)) => nested && target == label,
        _ => {
            let nested = nested
                || matches!(
                    stmt,
                    Stmt::LabeledBlock { .. }
                        | Stmt::Loop { .. }
                        | Stmt::While { .. }
                        | Stmt::For { .. }
                );
            bodies(stmt)
                .into_iter()
                .any(|list| nested_jump(list, label, nested))
        }
    })
}

pub(super) fn drop_tail_break(body: &mut Vec<Stmt>, label: &Label) {
    match body.last_mut() {
        Some(Stmt::Break(Some(target))) if target == label => {
            body.pop();
        }
        Some(Stmt::If {
            then_body,
            else_body,
            ..
        }) => {
            drop_tail_break(then_body, label);
            drop_tail_break(else_body, label);
        }
        Some(Stmt::Match { arms, .. }) => {
            for arm in arms {
                drop_tail_break(&mut arm.body, label);
            }
        }
        Some(Stmt::Scope { body }) => drop_tail_break(body, label),
        _ => {}
    }
}

pub(super) fn uses_label_or_exprs(body: &[Stmt], label: &Label) -> bool {
    crate::backend::engine::stmts_have_stmt_exprs(body) || uses_label(body, label)
}

fn elide_local_label(body: &mut [Stmt], label: &Label, blocked: bool) {
    for stmt in body {
        if !blocked {
            match stmt {
                Stmt::Break(target @ Some(_)) | Stmt::Continue(target @ Some(_))
                    if target.as_ref() == Some(label) =>
                {
                    *target = None
                }
                _ => {}
            }
        }
        let blocked = blocked
            || matches!(
                stmt,
                Stmt::LabeledBlock { .. }
                    | Stmt::Loop { .. }
                    | Stmt::While { .. }
                    | Stmt::For { .. }
            );
        for nested in stmt.child_bodies_mut() {
            elide_local_label(nested, label, blocked);
        }
    }
}

fn uses_label(body: &[Stmt], label: &Label) -> bool {
    body.iter().any(|stmt| {
        matches!(stmt, Stmt::Break(Some(target)) | Stmt::Continue(Some(target)) if target == label)
            || bodies(stmt)
                .into_iter()
                .any(|nested| uses_label(nested, label))
    })
}

impl Graph {
    fn suffix(&self) -> &str {
        self.label
            .as_str()
            .trim_start_matches("__slate_dispatch")
            .trim_start_matches("__dispatch")
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
        elide_local_label(&mut body, &label, false);
        drop_tail_continue(&mut body);
        fold_empty_branches(&mut body);
        let core = match header_test(&mut body) {
            Some(cond) => Stmt::While {
                label: uses_label(&body, &label).then(|| label.clone()),
                cond,
                body: RustBlock {
                    stmts: body,
                    tail: None,
                },
            },
            None => Stmt::Loop {
                label: uses_label(&body, &label).then(|| label.clone()),
                body,
            },
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
        out.extend(self.emit_term(&block.term, loops));
        out
    }

    fn emit_term(&self, term: &Term, loops: &[usize]) -> Vec<Stmt> {
        match term {
            Term::Diverge => Vec::new(),
            Term::Escape(stmts) => stmts.clone(),
            Term::Exit => vec![Stmt::Break(Some(self.label.clone()))],
            Term::Jump(target) => self.branch(*target, loops),
            Term::Branch {
                cond,
                then_term,
                else_term,
            } => vec![Stmt::If {
                cond: cond.clone(),
                then_body: self.emit_term(then_term, loops),
                else_body: self.emit_term(else_term, loops),
            }],
            Term::Switch { selector, arms } => vec![Stmt::Match {
                expr: selector.clone(),
                arms: arms
                    .iter()
                    .map(|(pattern, term)| MatchArm {
                        pattern: pattern.clone(),
                        body: self.emit_term(term, loops),
                    })
                    .collect(),
            }],
        }
    }

    fn branch(&self, target: usize, loops: &[usize]) -> Vec<Stmt> {
        if self.headers.contains(&target) && loops.contains(&target) {
            return vec![Stmt::Continue(Some(self.loop_label(target)))];
        }
        if !self.labeled[target] {
            return self.tree(target, loops);
        }
        match self.owner[target] {
            Some(Owner::AfterLoop(header)) if self.exits_directly(target, header) => {
                vec![Stmt::Break(Some(self.loop_label(header)))]
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
