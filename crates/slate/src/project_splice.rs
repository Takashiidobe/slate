use crate::backend::rust_ast::{
    Attr, Block, Cfg, FnDef, Ident, Item, RecordDef, RecordField, Stmt, Type,
};
use crate::backend::visit_names;
use std::collections::hash_map::DefaultHasher;
use std::collections::{BTreeSet, HashMap, HashSet};
use std::hash::{Hash, Hasher};

pub type Variants = BTreeSet<usize>;

struct Node {
    id: u32,
    on: Variants,
    shell: Stmt,
    erased: Stmt,
    hash: u64,
    names: Vec<Ident>,
    bodies: Vec<Vec<Node>>,
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

struct Builder<'a> {
    variant: usize,
    bound: &'a HashSet<Ident>,
    next_id: &'a mut u32,
}

impl Builder<'_> {
    fn node(&mut self, mut stmt: Stmt) -> Node {
        let bodies: Vec<Vec<Stmt>> = stmt
            .child_bodies_mut()
            .into_iter()
            .map(std::mem::take)
            .collect();
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
        Node {
            id: *self.next_id,
            on: Variants::from([self.variant]),
            shell: stmt,
            hash: hasher.finish(),
            erased,
            names,
            bodies: bodies.into_iter().map(|body| self.nodes(body)).collect(),
        }
    }

    fn nodes(&mut self, stmts: Vec<Stmt>) -> Vec<Node> {
        stmts.into_iter().map(|stmt| self.node(stmt)).collect()
    }
}

type Memo = HashMap<(u32, u32), u32>;

fn score(left: &Node, right: &Node, memo: &mut Memo) -> u32 {
    if left.hash != right.hash {
        return 0;
    }
    if left.bodies.is_empty() {
        return 1;
    }
    if let Some(&known) = memo.get(&(left.id, right.id)) {
        return known;
    }
    let total = 1 + left
        .bodies
        .iter()
        .zip(&right.bodies)
        .map(|(left, right)| table(left, right, memo)[0])
        .sum::<u32>();
    memo.insert((left.id, right.id), total);
    total
}

fn table(left: &[Node], right: &[Node], memo: &mut Memo) -> Vec<u32> {
    let width = right.len() + 1;
    let mut cells = vec![0; (left.len() + 1) * width];
    for i in (0..left.len()).rev() {
        for j in (0..right.len()).rev() {
            let matched = score(&left[i], &right[j], memo);
            let taken = (matched > 0).then(|| matched + cells[(i + 1) * width + j + 1]);
            cells[i * width + j] = cells[(i + 1) * width + j]
                .max(cells[i * width + j + 1])
                .max(taken.unwrap_or(0));
        }
    }
    cells
}

fn align(left: &[Node], right: &[Node], memo: &mut Memo) -> Vec<(usize, usize)> {
    let cells = table(left, right, memo);
    let width = right.len() + 1;
    let (mut i, mut j) = (0, 0);
    let mut pairs = Vec::new();
    while i < left.len() && j < right.len() {
        let matched = score(&left[i], &right[j], memo);
        if matched > 0 && cells[i * width + j] == matched + cells[(i + 1) * width + j + 1] {
            pairs.push((i, j));
            i += 1;
            j += 1;
        } else if cells[(i + 1) * width + j] >= cells[i * width + j + 1] {
            i += 1;
        } else {
            j += 1;
        }
    }
    pairs
}

#[derive(Default)]
struct Binding {
    to_merged: HashMap<Ident, Ident>,
    to_variant: HashMap<Ident, Ident>,
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
        for (merged, variant) in pending {
            self.to_merged.insert(variant, merged);
            self.to_variant.insert(merged, variant);
        }
        true
    }
}

struct Fold {
    variant: usize,
    memo: Memo,
    binding: Binding,
}

fn merge_into(merged: &mut Vec<Node>, incoming: Vec<Node>, fold: &mut Fold) {
    let pairs = align(merged, &incoming, &mut fold.memo);
    let mut left = std::mem::take(merged).into_iter().enumerate().peekable();
    let mut right = incoming.into_iter().enumerate().peekable();
    for (at_left, at_right) in pairs {
        while left.peek().is_some_and(|(index, _)| *index < at_left) {
            merged.push(left.next().expect("peeked").1);
        }
        while right.peek().is_some_and(|(index, _)| *index < at_right) {
            merged.push(right.next().expect("peeked").1);
        }
        let (_, mut node) = left.next().expect("aligned index");
        let (_, other) = right.next().expect("aligned index");
        if node.erased == other.erased && fold.binding.unify(&node.names, &other.names) {
            node.on.insert(fold.variant);
            for (body, other) in node.bodies.iter_mut().zip(other.bodies) {
                merge_into(body, other, fold);
            }
            merged.push(node);
        } else {
            merged.push(node);
            merged.push(other);
        }
    }
    merged.extend(left.map(|(_, node)| node));
    merged.extend(right.map(|(_, node)| node));
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
            if node.on.len() == 1 {
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
) -> Option<Spliced> {
    let shape = signature(variants[0]);
    if variants.iter().any(|variant| signature(variant) != shape) {
        return None;
    }
    let mut next_id = 0;
    let mut merged: Vec<Node> = Vec::new();
    for (variant, function) in variants.iter().enumerate() {
        let mut body = mark_tail(function);
        let bound = bound_names(&mut body);
        let incoming = Builder {
            variant,
            bound: &bound,
            next_id: &mut next_id,
        }
        .nodes(body);
        let mut taken = HashSet::new();
        collect_names(&merged, &mut taken);
        let mut fold = Fold {
            variant,
            memo: Memo::new(),
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
    for (variant, record) in variants.iter().enumerate() {
        let incoming = &record.fields;
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
        out.push(stmt);
    }
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
