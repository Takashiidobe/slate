use crate::backend::engine::unlabeled_jump;
use crate::backend::rust_ast::{
    Attr, Block, Cfg, FnDef, Ident, Item, Label, RecordDef, RecordField, Stmt, Type,
};
use crate::backend::visit_names;
use std::collections::hash_map::DefaultHasher;
use std::collections::{BTreeSet, HashMap, HashSet};
use std::hash::{Hash, Hasher};
use std::rc::Rc;

pub type Variants = BTreeSet<usize>;

struct Node {
    id: u32,
    origin: usize,
    on: Variants,
    shell: Stmt,
    erased: Stmt,
    hash: u64,
    names: Vec<Ident>,
    bodies: Vec<Vec<Node>>,
    binds: bool,
    jumps: bool,
}

impl Node {
    fn wrapper(&self) -> bool {
        matches!(self.shell, Stmt::LabeledBlock { .. })
    }
}

fn wrappable(node: &Node) -> bool {
    !node.binds && !node.jumps
}

fn is_loop(stmt: &Stmt) -> bool {
    matches!(
        stmt,
        Stmt::Loop { .. } | Stmt::While { .. } | Stmt::For { .. }
    )
}

fn generated_prefix(name: &str) -> Option<&str> {
    let prefix = name.trim_end_matches(|c: char| c.is_ascii_digit());
    (name.starts_with("__") && prefix.len() < name.len()).then_some(prefix)
}

fn bound_names(stmts: &mut [Stmt]) -> HashSet<Ident> {
    let mut bound = HashSet::new();
    for stmt in stmts {
        visit_names(stmt, |name, binds| {
            if binds && generated_prefix(name.as_str()).is_some() {
                bound.insert(*name);
            }
        });
    }
    bound
}

fn loop_label(stmt: &mut Stmt) -> Option<&mut Option<Label>> {
    match stmt {
        Stmt::Loop { label, .. } | Stmt::While { label, .. } | Stmt::For { label, .. } => {
            Some(label)
        }
        _ => None,
    }
}

fn point_direct_jumps(body: &mut [Stmt], label: &Label) {
    for stmt in body {
        match stmt {
            Stmt::Break(target @ None) | Stmt::Continue(target @ None) => {
                *target = Some(label.clone())
            }
            stmt if !is_loop(stmt) => {
                for body in stmt.child_bodies_mut() {
                    point_direct_jumps(body, label);
                }
            }
            _ => {}
        }
    }
}

fn label_loops(
    stmts: &mut [Stmt],
    fresh: &mut impl FnMut() -> Label,
    unscoped_macro: &dyn Fn(&str) -> bool,
) {
    for stmt in stmts {
        if loop_label(stmt).is_some_and(|label| label.is_none()) {
            let label = fresh();
            let mut labeled = stmt.clone();
            *loop_label(&mut labeled).expect("a loop") = Some(label.clone());
            let mut escapes = false;
            for body in labeled.child_bodies_mut() {
                point_direct_jumps(body, &label);
                escapes |= body.iter().any(|stmt| unlabeled_jump(stmt, unscoped_macro));
            }
            if !escapes {
                *stmt = labeled;
            }
        }
        for body in stmt.child_bodies_mut() {
            label_loops(body, fresh, unscoped_macro);
        }
    }
}

fn fresh_loop_labels(taken: HashSet<Ident>) -> impl FnMut() -> Label {
    let mut next = 0;
    move || loop {
        let candidate = Ident::new(format!("__loop_{next}"));
        next += 1;
        if !taken.contains(&candidate) {
            break Label::new(candidate);
        }
    }
}

fn names_in(stmts: &mut [Stmt], out: &mut HashSet<Ident>) {
    for stmt in stmts {
        visit_names(stmt, |name, _| {
            out.insert(*name);
        });
    }
}

fn direct_jumps(body: &mut [Stmt], label: &Label, retarget: bool) -> usize {
    let mut count = 0;
    for stmt in body {
        match stmt {
            Stmt::Break(target) | Stmt::Continue(target) if target.as_ref() == Some(label) => {
                count += 1;
                if retarget {
                    *target = None;
                }
            }
            Stmt::LabeledBlock { .. } => {}
            stmt if !is_loop(stmt) => {
                for body in stmt.child_bodies_mut() {
                    count += direct_jumps(body, label, retarget);
                }
            }
            _ => {}
        }
    }
    count
}

fn elide_introduced_labels(stmts: &mut [Stmt], original: &HashSet<Ident>) {
    for stmt in stmts {
        for body in stmt.child_bodies_mut() {
            elide_introduced_labels(body, original);
        }
        let Some(Some(label)) = loop_label(stmt).map(|label| label.clone()) else {
            continue;
        };
        if original.contains(&Ident::new(label.as_str())) {
            continue;
        }
        let mut uses = 0;
        visit_names(stmt, |name, binds| {
            uses += usize::from(!binds && name.as_str() == label.as_str())
        });
        let mut bodies = stmt.child_bodies_mut();
        let direct: usize = bodies
            .iter_mut()
            .map(|body| direct_jumps(body, &label, false))
            .sum();
        if direct == uses {
            for body in bodies {
                direct_jumps(body, &label, true);
            }
            *loop_label(stmt).expect("a loop") = None;
        }
    }
}

struct Builder<'a> {
    variant: usize,
    bound: &'a HashSet<Ident>,
    next_id: &'a mut u32,
    unscoped_macro: &'a dyn Fn(&str) -> bool,
}

impl Builder<'_> {
    fn node(&mut self, mut stmt: Stmt) -> Node {
        let binds = is_binding(&stmt) || matches!(&stmt, Stmt::Block(Block { tail: Some(_), .. }));
        let bodies: Vec<Vec<Stmt>> = stmt
            .child_bodies_mut()
            .into_iter()
            .map(std::mem::take)
            .collect();
        let jumps = unlabeled_jump(&stmt, self.unscoped_macro);
        let mut erased = stmt.clone();
        let mut names = Vec::new();
        visit_names(&mut erased, |name, _| {
            if self.bound.contains(name) {
                names.push(*name);
                let prefix = generated_prefix(name.as_str())
                    .expect("bound names are generated")
                    .to_string();
                *name = Ident::new(prefix);
            }
        });
        let mut hasher = DefaultHasher::new();
        erased.hash(&mut hasher);
        *self.next_id += 1;
        let id = *self.next_id;
        let bodies: Vec<Vec<Node>> = bodies.into_iter().map(|body| self.nodes(body)).collect();
        let jumps = jumps || (!is_loop(&stmt) && bodies.iter().flatten().any(|node| node.jumps));
        Node {
            id,
            origin: self.variant,
            on: Variants::from([self.variant]),
            shell: stmt,
            hash: hasher.finish(),
            erased,
            names,
            bodies,
            binds,
            jumps,
        }
    }

    fn nodes(&mut self, stmts: Vec<Stmt>) -> Vec<Node> {
        stmts.into_iter().map(|stmt| self.node(stmt)).collect()
    }
}

#[derive(Default)]
struct Memo {
    scores: HashMap<(u32, u32), u32>,
    opened: HashMap<(u32, u32, u32), Rc<Vec<u32>>>,
}

#[derive(Clone, Copy)]
enum Step {
    Match(usize, usize),
    OpenMerged { at: usize, run: (usize, usize) },
    OpenIncoming { at: usize, run: (usize, usize) },
}

fn score(left: &Node, right: &Node, memo: &mut Memo) -> u32 {
    if left.hash != right.hash {
        return 0;
    }
    if left.bodies.is_empty() {
        return 1;
    }
    if let Some(&known) = memo.scores.get(&(left.id, right.id)) {
        return known;
    }
    let total = 1 + left
        .bodies
        .iter()
        .zip(&right.bodies)
        .map(|(left, right)| table(left, right, memo)[0])
        .sum::<u32>();
    memo.scores.insert((left.id, right.id), total);
    total
}

fn opened_merged(wrapper: &Node, right: &[Node], end: usize, memo: &mut Memo) -> Rc<Vec<u32>> {
    let key = (wrapper.id, right[0].id, right[end - 1].id);
    if let Some(known) = memo.opened.get(&key) {
        return known.clone();
    }
    let cells = table(&wrapper.bodies[0], &right[..end], memo);
    let row = Rc::new(cells[..=end].to_vec());
    memo.opened.insert(key, row.clone());
    row
}

fn opened_incoming(wrapper: &Node, left: &[Node], end: usize, memo: &mut Memo) -> Rc<Vec<u32>> {
    let key = (wrapper.id, left[0].id, left[end - 1].id);
    if let Some(known) = memo.opened.get(&key) {
        return known.clone();
    }
    let body = &wrapper.bodies[0];
    let cells = table(&left[..end], body, memo);
    let width = body.len() + 1;
    let column: Rc<Vec<u32>> = Rc::new((0..=end).map(|i| cells[i * width]).collect());
    memo.opened.insert(key, column.clone());
    column
}

fn openings(
    left: &[Node],
    right: &[Node],
    (i, j): (usize, usize),
    cells: &[u32],
    memo: &mut Memo,
) -> Vec<(u32, Step)> {
    let width = right.len() + 1;
    let mut out = Vec::new();
    if left[i].wrapper() {
        for end in j + 1..=right.len() {
            if !wrappable(&right[end - 1]) {
                break;
            }
            let inside = opened_merged(&left[i], right, end, memo)[j];
            if inside > 0 {
                out.push((
                    inside + cells[(i + 1) * width + end],
                    Step::OpenMerged {
                        at: i,
                        run: (j, end),
                    },
                ));
            }
        }
    }
    if right[j].wrapper() {
        for end in i + 1..=left.len() {
            if !wrappable(&left[end - 1]) {
                break;
            }
            let inside = opened_incoming(&right[j], left, end, memo)[i];
            if inside > 0 {
                out.push((
                    inside + cells[end * width + j + 1],
                    Step::OpenIncoming {
                        at: j,
                        run: (i, end),
                    },
                ));
            }
        }
    }
    out
}

fn table(left: &[Node], right: &[Node], memo: &mut Memo) -> Vec<u32> {
    let width = right.len() + 1;
    let mut cells = vec![0; (left.len() + 1) * width];
    for i in (0..left.len()).rev() {
        for j in (0..right.len()).rev() {
            let matched = score(&left[i], &right[j], memo);
            let taken = (matched > 0).then(|| matched + cells[(i + 1) * width + j + 1]);
            let opened = openings(left, right, (i, j), &cells, memo)
                .into_iter()
                .map(|(value, _)| value)
                .max();
            cells[i * width + j] = cells[(i + 1) * width + j]
                .max(cells[i * width + j + 1])
                .max(taken.unwrap_or(0))
                .max(opened.unwrap_or(0));
        }
    }
    cells
}

fn align(left: &[Node], right: &[Node], memo: &mut Memo) -> Vec<Step> {
    let cells = table(left, right, memo);
    let width = right.len() + 1;
    let (mut i, mut j) = (0, 0);
    let mut steps = Vec::new();
    while i < left.len() && j < right.len() {
        let here = cells[i * width + j];
        let matched = score(&left[i], &right[j], memo);
        if matched > 0 && here == matched + cells[(i + 1) * width + j + 1] {
            steps.push(Step::Match(i, j));
            i += 1;
            j += 1;
        } else if here == cells[(i + 1) * width + j] {
            i += 1;
        } else if here == cells[i * width + j + 1] {
            j += 1;
        } else {
            let step = openings(left, right, (i, j), &cells, memo)
                .into_iter()
                .find(|(value, _)| *value == here)
                .expect("the best cell comes from an opening")
                .1;
            match step {
                Step::OpenMerged { run: (_, end), .. } => {
                    i += 1;
                    j = end;
                }
                Step::OpenIncoming { run: (_, end), .. } => {
                    i = end;
                    j += 1;
                }
                Step::Match(..) => unreachable!("openings only open wrappers"),
            }
            steps.push(step);
        }
    }
    steps
}

#[derive(Default)]
struct Binding {
    to_merged: HashMap<Ident, Ident>,
    to_variant: HashMap<Ident, Ident>,
    reserved_merged: HashSet<Ident>,
    reserved_variant: HashSet<Ident>,
}

impl Binding {
    fn unify(&mut self, merged: &[Ident], variant: &[Ident]) -> bool {
        let mut pending: Vec<(Ident, Ident)> = Vec::new();
        for (&merged, &variant) in merged.iter().zip(variant) {
            let bound_merged = self.to_merged.get(&variant).copied().or_else(|| {
                pending
                    .iter()
                    .find(|(_, known)| *known == variant)
                    .map(|(known, _)| *known)
            });
            let bound_variant = self.to_variant.get(&merged).copied().or_else(|| {
                pending
                    .iter()
                    .find(|(known, _)| *known == merged)
                    .map(|(_, known)| *known)
            });
            match (bound_merged, bound_variant) {
                (None, None) => pending.push((merged, variant)),
                (Some(known), Some(other)) if known == merged && other == variant => {}
                _ => return false,
            }
        }
        for (merged, variant) in &pending {
            if self.reserved_merged.contains(merged) || self.reserved_variant.contains(variant) {
                return false;
            }
        }
        for (merged, variant) in pending {
            self.to_merged.insert(variant, merged);
            self.to_variant.insert(merged, variant);
        }
        true
    }

    fn reserve_merged(&mut self, names: &[Ident]) -> bool {
        if names.iter().any(|name| self.to_variant.contains_key(name)) {
            return false;
        }
        self.reserved_merged.extend(names.iter().copied());
        true
    }

    fn reserve_variant(&mut self, names: &[Ident]) -> bool {
        if names.iter().any(|name| self.to_merged.contains_key(name)) {
            return false;
        }
        self.reserved_variant.extend(names.iter().copied());
        true
    }
}

struct Fold {
    variant: usize,
    memo: Memo,
    binding: Binding,
}

fn merge_into(merged: &mut Vec<Node>, incoming: Vec<Node>, fold: &mut Fold) {
    let steps = align(merged, &incoming, &mut fold.memo);
    let mut left = std::mem::take(merged).into_iter().enumerate().peekable();
    let mut right = incoming.into_iter().enumerate().peekable();
    for step in steps {
        let (at_left, at_right) = match step {
            Step::Match(i, j) => (i, j),
            Step::OpenMerged { at, run } => (at, run.0),
            Step::OpenIncoming { at, run } => (run.0, at),
        };
        while left.peek().is_some_and(|(index, _)| *index < at_left) {
            merged.push(left.next().expect("peeked").1);
        }
        while right.peek().is_some_and(|(index, _)| *index < at_right) {
            merged.push(right.next().expect("peeked").1);
        }
        match step {
            Step::Match(..) => {
                let (_, mut node) = left.next().expect("aligned index");
                let (_, other) = right.next().expect("aligned index");
                if node.erased == other.erased && fold.binding.unify(&node.names, &other.names) {
                    node.on.insert(fold.variant);
                    node.jumps |= other.jumps;
                    for (body, other) in node.bodies.iter_mut().zip(other.bodies) {
                        merge_into(body, other, fold);
                    }
                    merged.push(node);
                } else {
                    merged.push(node);
                    merged.push(other);
                }
            }
            Step::OpenMerged { run: (_, end), .. } => {
                let (_, mut wrapper) = left.next().expect("aligned index");
                let run: Vec<Node> = std::iter::from_fn(|| {
                    right
                        .next_if(|(index, _)| *index < end)
                        .map(|(_, node)| node)
                })
                .collect();
                if fold.binding.reserve_merged(&wrapper.names) {
                    wrapper.on.insert(fold.variant);
                    merge_into(&mut wrapper.bodies[0], run, fold);
                    merged.push(wrapper);
                } else {
                    merged.push(wrapper);
                    merged.extend(run);
                }
            }
            Step::OpenIncoming { run: (_, end), .. } => {
                let (_, mut wrapper) = right.next().expect("aligned index");
                let run: Vec<Node> = std::iter::from_fn(|| {
                    left.next_if(|(index, _)| *index < end)
                        .map(|(_, node)| node)
                })
                .collect();
                if fold.binding.reserve_variant(&wrapper.names) {
                    for node in &run {
                        wrapper.on.extend(node.on.iter().copied());
                    }
                    let body = std::mem::replace(&mut wrapper.bodies[0], run);
                    merge_into(&mut wrapper.bodies[0], body, fold);
                    merged.push(wrapper);
                } else {
                    merged.extend(run);
                    merged.push(wrapper);
                }
            }
        }
    }
    merged.extend(left.map(|(_, node)| node));
    merged.extend(right.map(|(_, node)| node));
}

fn node_count(nodes: &[Node]) -> usize {
    nodes
        .iter()
        .map(|node| {
            1 + node
                .bodies
                .iter()
                .map(|body| node_count(body))
                .sum::<usize>()
        })
        .sum()
}

fn collect_names(nodes: &[Node], out: &mut HashSet<Ident>) {
    for node in nodes {
        out.extend(node.names.iter().copied());
        for body in &node.bodies {
            collect_names(body, out);
        }
    }
}

struct Renamer<'a> {
    bound: &'a HashSet<Ident>,
    binding: &'a Binding,
    fresh: HashMap<Ident, Ident>,
    taken: HashSet<Ident>,
    counter: u32,
}

impl Renamer<'_> {
    fn target(&mut self, name: Ident) -> Ident {
        if let Some(&merged) = self.binding.to_merged.get(&name) {
            return merged;
        }
        if let Some(&fresh) = self.fresh.get(&name) {
            return fresh;
        }
        let chosen = if self.taken.contains(&name) {
            let prefix = generated_prefix(name.as_str()).expect("bound names are generated");
            loop {
                self.counter += 1;
                let candidate = Ident::new(format!("{prefix}{}", self.counter));
                if !self.taken.contains(&candidate) {
                    break candidate;
                }
            }
        } else {
            name
        };
        self.taken.insert(chosen);
        self.fresh.insert(name, chosen);
        chosen
    }

    fn apply(&mut self, nodes: &mut [Node], variant: usize) {
        for node in nodes {
            if !node.on.contains(&variant) {
                continue;
            }
            if node.origin == variant {
                let bound = self.bound;
                visit_names(&mut node.shell, |name, _| {
                    if bound.contains(name) {
                        *name = self.target(*name);
                    }
                });
                for name in &mut node.names {
                    *name = self.target(*name);
                }
            }
            for body in &mut node.bodies {
                self.apply(body, variant);
            }
        }
    }
}

fn shares_statement(nodes: &[Node]) -> bool {
    nodes.iter().any(|node| {
        let carries_code = node.bodies.is_empty()
            || matches!(&node.shell, Stmt::Block(Block { tail: Some(_), .. }));
        (carries_code && node.on.len() > 1 && !matches!(node.shell, Stmt::Comment(_)))
            || node.bodies.iter().any(|body| shares_statement(body))
    })
}

fn is_binding(stmt: &Stmt) -> bool {
    matches!(stmt, Stmt::Let { .. } | Stmt::LetIf { .. })
}

fn emit(
    nodes: Vec<Node>,
    parent: &Variants,
    gate: &mut impl FnMut(&Variants, &Variants) -> Option<Cfg>,
) -> Vec<Stmt> {
    let mut out: Vec<Stmt> = Vec::new();
    for node in nodes {
        let cfg = if node.on == *parent {
            None
        } else {
            gate(&node.on, parent)
        };
        let mut stmt = node.shell;
        for (slot, body) in stmt.child_bodies_mut().into_iter().zip(node.bodies) {
            *slot = emit(body, &node.on, gate);
        }
        let Some(cfg) = cfg else {
            out.push(stmt);
            continue;
        };
        if matches!(stmt, Stmt::Comment(_)) {
            continue;
        }
        match out.last_mut() {
            Some(Stmt::Cfg { cfg: last, body })
                if *last == cfg && !is_binding(&stmt) && !body.iter().any(is_binding) =>
            {
                body.push(stmt);
            }
            _ => out.push(Stmt::Cfg {
                cfg,
                body: vec![stmt],
            }),
        }
    }
    out
}

fn mark_tail(function: &FnDef) -> Vec<Stmt> {
    let mut body = function.body.clone();
    if function.ret.is_some()
        && let Some(last) = body.last_mut()
        && let Stmt::Expr(tail) = last
    {
        *last = Stmt::Block(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(tail.clone())),
        });
    }
    body
}

fn unmark_tail(function: &mut FnDef) {
    if function.ret.is_some()
        && let Some(last) = function.body.last_mut()
        && let Stmt::Block(Block { stmts, tail }) = last
        && stmts.is_empty()
        && let Some(tail) = tail
    {
        *last = Stmt::Expr((**tail).clone());
    }
}

fn signature(function: &FnDef) -> FnDef {
    let mut shape = FnDef {
        body: Vec::new(),
        ..function.clone()
    };
    shape.ret = shape.ret.map(|_| Type::Unit);
    shape
        .attrs
        .retain(|attr| !matches!(attr, Attr::LinkSection(_)));
    for param in &mut shape.params {
        param.ty = Type::Unit;
        param.mutable = false;
    }
    shape
}

pub struct Spliced {
    pub function: FnDef,
    pub bindings: Vec<(Variants, Item)>,
}

fn bind_position<'a>(
    name: String,
    types: impl Iterator<Item = &'a Type>,
    slot: &mut Type,
    bindings: &mut Vec<(Variants, Item)>,
) {
    let mut groups: Vec<(&Type, Variants)> = Vec::new();
    for (variant, ty) in types.enumerate() {
        match groups.iter_mut().find(|(known, _)| *known == ty) {
            Some((_, on)) => {
                on.insert(variant);
            }
            None => groups.push((ty, Variants::from([variant]))),
        }
    }
    if groups.len() == 1 {
        return;
    }
    *slot = Type::Custom(name.clone());
    for (ty, on) in groups {
        bindings.push((
            on,
            Item::TypeAlias {
                name: name.clone(),
                ty: ty.clone(),
            },
        ));
    }
}

fn gate_sections(
    function: &mut FnDef,
    variants: &[&FnDef],
    all: &Variants,
    gate: &mut impl FnMut(&Variants, &Variants) -> Option<Cfg>,
) {
    let mut sections: Vec<(Attr, Variants)> = Vec::new();
    for (variant, source) in variants.iter().enumerate() {
        for attr in source
            .attrs
            .iter()
            .filter(|attr| matches!(attr, Attr::LinkSection(_)))
        {
            match sections.iter_mut().find(|(known, _)| known == attr) {
                Some((_, on)) => {
                    on.insert(variant);
                }
                None => sections.push((attr.clone(), Variants::from([variant]))),
            }
        }
    }
    sections.retain(|(_, on)| on != all);
    function
        .attrs
        .retain(|attr| !sections.iter().any(|(known, _)| known == attr));
    for (attr, on) in sections {
        function.attrs.push(match gate(&on, all) {
            Some(cfg) => Attr::CfgAttr(cfg, Box::new(attr)),
            None => attr,
        });
    }
}

pub fn splice_function(
    variants: &[&FnDef],
    gate: &mut impl FnMut(&Variants, &Variants) -> Option<Cfg>,
    unscoped_macro: &dyn Fn(&str) -> bool,
) -> Option<Spliced> {
    let shape = signature(variants[0]);
    if variants.iter().any(|variant| signature(variant) != shape) {
        return None;
    }
    let mut next_id = 0;
    let mut bodies: Vec<Vec<Stmt>> = variants
        .iter()
        .map(|function| mark_tail(function))
        .collect();
    let mut original = HashSet::new();
    for body in &mut bodies {
        names_in(body, &mut original);
    }
    let mut fresh = fresh_loop_labels(original.clone());
    let mut prepared: Vec<(usize, Vec<Node>, HashSet<Ident>)> = bodies
        .into_iter()
        .enumerate()
        .map(|(variant, mut body)| {
            label_loops(&mut body, &mut fresh, unscoped_macro);
            let bound = bound_names(&mut body);
            let incoming = Builder {
                variant,
                bound: &bound,
                next_id: &mut next_id,
                unscoped_macro,
            }
            .nodes(body);
            (variant, incoming, bound)
        })
        .collect();
    prepared.sort_by_key(|(variant, nodes, _)| (std::cmp::Reverse(node_count(nodes)), *variant));
    let mut merged: Vec<Node> = Vec::new();
    for (variant, incoming, bound) in prepared {
        let mut taken = HashSet::new();
        collect_names(&merged, &mut taken);
        let mut fold = Fold {
            variant,
            memo: Memo::default(),
            binding: Binding::default(),
        };
        merge_into(&mut merged, incoming, &mut fold);
        Renamer {
            bound: &bound,
            binding: &fold.binding,
            fresh: HashMap::new(),
            taken,
            counter: 0,
        }
        .apply(&mut merged, variant);
    }
    if !shares_statement(&merged) {
        return None;
    }
    let all: Variants = (0..variants.len()).collect();
    let mut function = variants[0].clone();
    function.body = emit(merged, &all, gate);
    elide_introduced_labels(&mut function.body, &original);
    unmark_tail(&mut function);
    for variant in variants {
        for (param, other) in function.params.iter_mut().zip(&variant.params) {
            param.mutable |= other.mutable;
        }
    }
    gate_sections(&mut function, variants, &all, gate);
    let mut bindings = Vec::new();
    if let Some(ret) = &mut function.ret {
        bind_position(
            format!("__SlateRet_{}", variants[0].name),
            variants.iter().filter_map(|variant| variant.ret.as_ref()),
            ret,
            &mut bindings,
        );
    }
    for (position, param) in function.params.iter_mut().enumerate() {
        bind_position(
            format!("__SlateParam_{}_{position}", variants[0].name),
            variants.iter().map(|variant| &variant.params[position].ty),
            &mut param.ty,
            &mut bindings,
        );
    }
    Some(Spliced { function, bindings })
}

fn record_shape(record: &RecordDef) -> RecordDef {
    RecordDef {
        fields: Vec::new(),
        ..record.clone()
    }
}

pub fn splice_record(
    variants: &[&RecordDef],
    gate: &mut impl FnMut(&Variants, &Variants) -> Option<Cfg>,
) -> Option<RecordDef> {
    let shape = record_shape(variants[0]);
    if variants
        .iter()
        .any(|variant| record_shape(variant) != shape)
    {
        return None;
    }
    let mut merged: Vec<(RecordField, Variants)> = Vec::new();
    let mut fold_order: Vec<usize> = (0..variants.len()).collect();
    fold_order
        .sort_by_key(|variant| (std::cmp::Reverse(variants[*variant].fields.len()), *variant));
    for variant in fold_order {
        let incoming = &variants[variant].fields;
        let width = incoming.len() + 1;
        let mut cells = vec![0u32; (merged.len() + 1) * width];
        for i in (0..merged.len()).rev() {
            for j in (0..incoming.len()).rev() {
                let matched = merged[i].0 == incoming[j];
                cells[i * width + j] = cells[(i + 1) * width + j]
                    .max(cells[i * width + j + 1])
                    .max(if matched {
                        1 + cells[(i + 1) * width + j + 1]
                    } else {
                        0
                    });
            }
        }
        let mut next = Vec::with_capacity(merged.len() + incoming.len());
        let (mut i, mut j) = (0, 0);
        while i < merged.len() || j < incoming.len() {
            if i < merged.len()
                && j < incoming.len()
                && merged[i].0 == incoming[j]
                && cells[i * width + j] == 1 + cells[(i + 1) * width + j + 1]
            {
                let (field, mut on) = merged[i].clone();
                on.insert(variant);
                next.push((field, on));
                i += 1;
                j += 1;
            } else if i < merged.len() && cells[(i + 1) * width + j] >= cells[i * width + j + 1] {
                next.push(merged[i].clone());
                i += 1;
            } else if j < incoming.len() {
                next.push((incoming[j].clone(), Variants::from([variant])));
                j += 1;
            } else {
                next.push(merged[i].clone());
                i += 1;
            }
        }
        merged = next;
    }
    if !merged.iter().any(|(_, on)| on.len() > 1) {
        return None;
    }
    let all: Variants = (0..variants.len()).collect();
    let mut record = shape;
    record.fields = merged
        .into_iter()
        .map(|(mut field, on)| {
            field.cfg = if on == all { None } else { gate(&on, &all) };
            field
        })
        .collect();
    Some(record)
}

fn resolve_stmts(stmts: Vec<Stmt>, holds: &impl Fn(&Cfg) -> bool, out: &mut Vec<Stmt>) {
    for mut stmt in stmts {
        if matches!(stmt, Stmt::Comment(_)) {
            continue;
        }
        if let Stmt::Cfg { cfg, body } = stmt {
            if holds(&cfg) {
                resolve_stmts(body, holds, out);
            }
            continue;
        }
        for slot in stmt.child_bodies_mut() {
            let body = std::mem::take(slot);
            resolve_stmts(body, holds, slot);
        }
        if let Stmt::LabeledBlock { label, body } = &mut stmt
            && !body.iter().any(is_binding)
            && !targets(body, label.as_str())
        {
            out.append(body);
            continue;
        }
        out.push(stmt);
    }
}

fn targets(body: &mut [Stmt], label: &str) -> bool {
    let mut found = false;
    for stmt in body {
        visit_names(stmt, |name, binds| {
            found |= !binds && name.as_str() == label
        });
    }
    found
}

pub fn resolve_function(function: &FnDef, holds: &impl Fn(&Cfg) -> bool) -> FnDef {
    let mut resolved = function.clone();
    let body = std::mem::take(&mut resolved.body);
    resolve_stmts(body, holds, &mut resolved.body);
    unmark_tail(&mut resolved);
    resolved.attrs = std::mem::take(&mut resolved.attrs)
        .into_iter()
        .filter_map(|attr| match attr {
            Attr::CfgAttr(cfg, attr) => holds(&cfg).then_some(*attr),
            attr => Some(attr),
        })
        .collect();
    for param in &mut resolved.params {
        param.mutable = false;
    }
    let mut taken = HashSet::new();
    names_in(&mut resolved.body, &mut taken);
    label_loops(&mut resolved.body, &mut fresh_loop_labels(taken), &|_| {
        false
    });
    let bound = bound_names(&mut resolved.body);
    let mut canonical: HashMap<Ident, Ident> = HashMap::new();
    for stmt in &mut resolved.body {
        visit_names(stmt, |name, _| {
            if bound.contains(name) {
                let next = canonical.len();
                *name = *canonical.entry(*name).or_insert_with(|| {
                    let prefix =
                        generated_prefix(name.as_str()).expect("bound names are generated");
                    Ident::new(format!("{prefix}{next}"))
                });
            }
        });
    }
    resolved
}

pub fn resolve_record(record: &RecordDef, holds: &impl Fn(&Cfg) -> bool) -> RecordDef {
    let mut resolved = record.clone();
    resolved
        .fields
        .retain(|field| field.cfg.as_ref().is_none_or(holds));
    for field in &mut resolved.fields {
        field.cfg = None;
    }
    resolved
}
