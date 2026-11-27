use super::summary::{NodeKind, Summaries};
use super::{
    Borrow, ESCAPE, FREE, NULLABLE, OFFSET, Origin, PointerCapability, Reason, SHARED, STRING,
    WRITE,
};
use crate::program::{FunctionId, Program, ProgramBinding};
use foldhash::{HashSet, HashSetExt};
use slate_parser::ir::BindingId;

pub(super) struct Flow {
    class_of: Vec<u32>,
    class_flags: Vec<u8>,
    escape: Vec<Option<Origin>>,
    raw: Vec<Option<Origin>>,
    returned: Vec<bool>,
    liftable: Vec<bool>,
    pinned: Vec<u32>,
    pub(super) leaked_statics: HashSet<ProgramBinding>,
    edges: ValueEdges,
}

impl Flow {
    pub(super) fn node_count(&self) -> usize {
        self.class_of.len()
    }
    pub(super) fn class_of(&self, node: u32) -> u32 {
        self.class_of[node as usize]
    }
    pub(super) fn flags(&self, class: u32) -> u8 {
        self.class_flags[class as usize]
    }
    pub(super) fn raw(&self, node: u32) -> bool {
        self.raw[node as usize].is_some()
    }
    pub(super) fn raw_origin(&self, node: u32) -> Option<Origin> {
        self.raw[node as usize]
    }
    pub(super) fn escape_origin(&self, class: u32) -> Option<Origin> {
        self.escape[class as usize]
    }
    pub(super) fn returned(&self, class: u32) -> bool {
        self.returned[class as usize]
    }
    pub(super) fn lifts_return(&self, function: usize) -> bool {
        self.liftable[function]
    }
    pub(super) fn pinned(&self) -> &[u32] {
        &self.pinned
    }
    pub(super) fn classes(&self) -> impl Iterator<Item = u32> + '_ {
        self.class_of.iter().copied()
    }

    pub(super) fn borrow(&self, node: u32, byte_elements: bool) -> Option<Borrow> {
        capability(
            self.flags(self.class_of(node)),
            self.raw(node),
            byte_elements,
        )
        .borrow()
    }

    pub(super) fn propagate_unproven(
        &self,
        covered: &HashSet<(usize, usize)>,
        external: &[bool],
        unproven: &mut [Option<Origin>],
    ) {
        let mut edges: Vec<(u32, u32)> = self
            .edges
            .returns
            .iter()
            .copied()
            .chain(
                self.edges
                    .arguments
                    .iter()
                    .filter(|&&(_, _, callee, index)| {
                        !external[callee] && !covered.contains(&(callee, index))
                    })
                    .map(|&(from, to, _, _)| (from, to)),
            )
            .map(|(from, to)| (self.class_of(from), self.class_of(to)))
            .filter(|(from, to)| from != to)
            .collect();
        reach_labeled(self.node_count(), &mut edges, unproven);
    }
}

#[derive(Default)]
struct ValueEdges {
    copies: Vec<(u32, u32)>,
    returns: Vec<(u32, u32)>,
    arguments: Vec<(u32, u32, usize, usize)>,
}

impl ValueEdges {
    fn connect_return(&mut self, from: u32, to: u32) {
        self.returns.push((from, to));
        self.copies.push((from, to));
    }
}

type ReturnLifetime = Option<(usize, u32)>;

pub(super) fn solve(
    program: &Program<'_>,
    index: &Summaries,
    leaked_statics: HashSet<ProgramBinding>,
) -> Flow {
    let mut flow = Flow::from_local_summaries(index, leaked_statics);
    let lifetimes = flow.connect_returns(index);
    let aliases = flow.connect_arguments(index);
    let mut forward = flow.forward_edges();
    let mut backward = flow.propagate_uses(&forward, &aliases);
    flow.liftable = flow.liftable_returns(index, &lifetimes);
    flow.finish_returns(index, &lifetimes, &mut backward);
    flow.record_object_leaks(program, index);
    propagate(&mut flow.class_flags, NULLABLE, &mut forward);
    flow.propagate_raw(index);
    flow
}

impl Flow {
    fn from_local_summaries(index: &Summaries, leaked_statics: HashSet<ProgramBinding>) -> Self {
        let Summaries {
            functions: summaries,
            base,
            candidates,
            ..
        } = index;
        let total = index.count_nodes();
        let mut classes = UnionFind::new(total);
        let mut raw = vec![None; total];
        let mut seeds = vec![0u8; total];
        let mut escapes = vec![None; total];
        let mut edges = ValueEdges::default();
        for (function, summary) in summaries.iter().enumerate() {
            let Some(summary) = summary else { continue };
            let offset = base[function];
            for &(left, right) in &summary.unions {
                classes.union(offset + left, offset + right);
            }
            for &(source, destination) in &summary.copies {
                edges.copies.push((offset + source, offset + destination));
            }
            for (node, entry) in summary.nodes.iter().enumerate() {
                let global = (offset + node as u32) as usize;
                seeds[global] = entry.flags;
                escapes[global] = entry.escape;
                raw[global] = entry.raw.map(Origin::local).or_else(|| {
                    (matches!(entry.kind, NodeKind::Parameter(_)) && !candidates[function])
                        .then_some(Origin::local(Reason::NonCandidateParameter))
                });
                if entry.kind == NodeKind::Return && !candidates[function] {
                    seeds[global] |= ESCAPE;
                    escapes[global].get_or_insert(Reason::NonCandidateReturn);
                }
            }
        }
        let mut class_flags = vec![0u8; total];
        let mut escape = vec![None; total];
        for node in 0..total as u32 {
            let class = classes.find(node) as usize;
            class_flags[class] |= seeds[node as usize];
            if escape[class].is_none() {
                escape[class] = escapes[node as usize].map(Origin::local);
            }
        }

        Flow {
            class_of: (0..total as u32).map(|node| classes.find(node)).collect(),
            class_flags,
            escape,
            raw,
            edges,
            returned: vec![false; total],
            liftable: vec![false; index.functions.len()],
            pinned: Vec::new(),
            leaked_statics,
        }
    }

    fn connect_returns(&mut self, index: &Summaries) -> Vec<ReturnLifetime> {
        let Summaries {
            functions: summaries,
            base,
            candidates,
            parameters,
            ..
        } = index;
        let mut lifetimes: Vec<Option<(usize, u32)>> = vec![None; summaries.len()];
        for (function, summary) in summaries.iter().enumerate() {
            let Some(summary) = summary else { continue };
            let Some(node) = summary.returned else {
                continue;
            };
            let offset = base[function];
            let returned = offset + node;
            let item = &summary.nodes[node as usize];
            for &source in &summary.returned_from {
                self.edges.connect_return(offset + source, returned);
            }
            let mut tracked = parameters[function].iter();
            let (Some((&index, &parameter)), None) = (tracked.next(), tracked.next()) else {
                continue;
            };
            let class = self.class_of(parameter);
            if candidates[function]
                && item.raw.is_none()
                && !item.opaque_pointee
                && !summary.returned_from.is_empty()
                && summary
                    .returned_from
                    .iter()
                    .all(|&source| self.class_of(offset + source) == class)
            {
                lifetimes[function] = Some((index, parameter));
            }
        }
        for (function, summary) in summaries.iter().enumerate() {
            let Some(summary) = summary else { continue };
            for &(node, callee) in &summary.results {
                let result = base[function] + node;
                let returned = summaries
                    .get(callee.0 as usize)
                    .and_then(Option::as_ref)
                    .and_then(|callee| callee.returned);
                match returned {
                    Some(returned) => {
                        let returned = base[callee.0 as usize] + returned;
                        self.edges.connect_return(returned, result);
                    }
                    None => self.make_raw(result, Reason::CalleeNoReturn),
                }
            }
        }
        lifetimes
    }

    fn connect_arguments(&mut self, index: &Summaries) -> Vec<(u32, u32)> {
        let Summaries {
            functions: summaries,
            base,
            candidates,
            parameters,
            ..
        } = index;
        let mut aliased = Vec::new();
        for (function, summary) in summaries.iter().enumerate() {
            let Some(summary) = summary else { continue };
            let offset = base[function];
            for site in &summary.calls {
                let callee = site.callee.0 as usize;
                let mut objects = Vec::new();
                for &(index, node) in &site.arguments {
                    let caller = self.class_of(offset + node);
                    match parameters[callee].get(&index) {
                        Some(&parameter) if candidates[callee] => {
                            self.edges
                                .arguments
                                .push((caller, parameter, callee, index));
                            objects.push(((false, caller), index));
                        }
                        Some(_) | None => self.escape(caller, Reason::NonCandidateCallee),
                    }
                }
                if !candidates[callee] {
                    continue;
                }
                for index in &site.nulls {
                    if let Some(&parameter) = parameters[callee].get(index) {
                        let class = self.class_of(parameter);
                        self.class_flags[class as usize] |= NULLABLE;
                    }
                }
                for &(index, object) in &site.objects {
                    objects.push(((true, object.0), index));
                }
                objects.sort_unstable();
                objects.dedup();
                for pair in objects.windows(2) {
                    if pair[0].0 == pair[1].0
                        && let Some(&left) = parameters[callee].get(&pair[0].1)
                        && let Some(&right) = parameters[callee].get(&pair[1].1)
                    {
                        aliased.push((self.class_of(left), self.class_of(right)));
                    }
                }
            }
        }
        aliased
    }

    fn forward_edges(&self) -> Vec<(u32, u32)> {
        self.edges
            .returns
            .iter()
            .copied()
            .chain(
                self.edges
                    .arguments
                    .iter()
                    .map(|&(from, to, _, _)| (from, to)),
            )
            .map(|(from, to)| (self.class_of(from), self.class_of(to)))
            .filter(|(from, to)| from != to)
            .collect()
    }

    fn propagate_uses(
        &mut self,
        forward: &[(u32, u32)],
        aliased: &[(u32, u32)],
    ) -> Vec<(u32, u32)> {
        let mut backward: Vec<(u32, u32)> = forward.iter().map(|&(from, to)| (to, from)).collect();
        for &(left, right) in aliased {
            backward.push((left, right));
            backward.push((right, left));
        }
        for flag in [WRITE, OFFSET, FREE, STRING, SHARED] {
            propagate(&mut self.class_flags, flag, &mut backward);
        }
        for &(left, right) in aliased {
            if (self.class_flags[left as usize] | self.class_flags[right as usize]) & WRITE != 0 {
                self.escape(left, Reason::AliasedWrittenArguments);
                self.escape(right, Reason::AliasedWrittenArguments);
            }
        }
        for (flags, escape) in self.class_flags.iter().zip(&mut self.escape) {
            if flags & ESCAPE != 0 {
                escape.get_or_insert(Origin::local(Reason::Unlabeled));
            }
        }
        reach_labeled(self.class_of.len(), &mut backward, &mut self.escape);
        for (flags, escape) in self.class_flags.iter_mut().zip(&self.escape) {
            if escape.is_some() {
                *flags |= ESCAPE;
            }
        }
        backward
    }

    fn escape(&mut self, class: u32, reason: Reason) {
        self.class_flags[class as usize] |= ESCAPE;
        self.escape[class as usize].get_or_insert(Origin::local(reason));
    }

    fn make_raw(&mut self, node: u32, reason: Reason) {
        self.raw[node as usize].get_or_insert(Origin::local(reason));
    }

    fn liftable_returns(&self, index: &Summaries, lifetimes: &[ReturnLifetime]) -> Vec<bool> {
        let Summaries {
            functions: summaries,
            base,
            candidates,
            parameters,
            incoming,
            ..
        } = index;
        let immutable = |node: u32| {
            let flags = self.flags(self.class_of(node));
            flags & WRITE == 0 && capability(flags, false, false).borrow().is_some()
        };
        let mut liftable = vec![false; summaries.len()];
        for (function, lifetime) in lifetimes.iter().enumerate() {
            let (Some((index, parameter)), Some(summary)) = (*lifetime, &summaries[function])
            else {
                continue;
            };
            let Some(node) = summary.returned else {
                continue;
            };
            liftable[function] = immutable(base[function] + node)
                && immutable(parameter)
                && incoming[function].iter().all(|&(caller, site)| {
                    let Some(caller_summary) = &summaries[caller] else {
                        return false;
                    };
                    let site = &caller_summary.calls[site];
                    let frozen = |object: BindingId| {
                        !caller_summary.stored.contains(&object)
                            && caller_summary.calls.iter().all(|other| {
                                other
                                    .objects
                                    .iter()
                                    .filter(|&&(_, passed)| passed == object)
                                    .all(|&(position, _)| {
                                        let callee = other.callee.0 as usize;
                                        candidates[callee]
                                            && parameters[callee]
                                                .get(&position)
                                                .is_some_and(|&parameter| immutable(parameter))
                                    })
                            })
                    };
                    site.arguments
                        .iter()
                        .filter(|&&(position, _)| position == index)
                        .all(|&(_, node)| immutable(base[caller] + node))
                        && site
                            .objects
                            .iter()
                            .filter(|&&(position, _)| position == index)
                            .all(|&(_, object)| frozen(object))
                });
        }
        liftable
    }

    fn finish_returns(
        &mut self,
        index: &Summaries,
        lifetimes: &[ReturnLifetime],
        backward: &mut [(u32, u32)],
    ) {
        let Summaries {
            functions: summaries,
            base,
            ..
        } = index;
        for (function, summary) in summaries.iter().enumerate() {
            if let Some(node) = summary.as_ref().and_then(|summary| summary.returned)
                && !self.liftable[function]
            {
                let class = self.class_of(base[function] + node);
                self.returned[class as usize] = true;
            }
        }
        reach(self.node_count(), backward, &mut self.returned);
        for (function, summary) in summaries.iter().enumerate() {
            let Some(summary) = summary else { continue };
            let Some(node) = summary.returned else {
                continue;
            };
            let returned = base[function] + node;
            match lifetimes[function] {
                Some((_, parameter)) if self.liftable[function] => {
                    self.edges.connect_return(parameter, returned);
                }
                Some(_) => self.make_raw(returned, Reason::ReturnLifetimeRejected),
                None => self.make_raw(returned, Reason::ReturnNoLifetime),
            }
            for &(result, callee) in &summary.results {
                if !self.liftable[callee.0 as usize] {
                    self.make_raw(base[function] + result, Reason::CalleeReturnUnlifted);
                }
            }
        }
    }

    fn record_object_leaks(&mut self, program: &Program<'_>, index: &Summaries) {
        let Summaries {
            functions: summaries,
            base,
            candidates,
            parameters,
            ..
        } = index;
        let mut passed = HashSet::new();
        for (function, summary) in summaries.iter().enumerate() {
            let Some(summary) = summary else { continue };
            for site in &summary.calls {
                for &(_, node) in &site.arguments {
                    passed.insert(self.class_of(base[function] + node));
                }
            }
        }
        for (function, summary) in summaries.iter().enumerate() {
            let Some(summary) = summary else { continue };
            let Some(entry) = program.function(FunctionId(function as u32)) else {
                continue;
            };
            for &(node, object) in &summary.derived {
                let class = self.class_of(base[function] + node);
                if self.flags(class) & ESCAPE != 0
                    || self.returned(class)
                    || passed.contains(&class)
                {
                    self.leaked_statics.insert(ProgramBinding {
                        unit: entry.unit,
                        binding: object,
                    });
                }
            }
            let mut leaked = summary.leaked.clone();
            for site in &summary.calls {
                let callee = site.callee.0 as usize;
                for &(index, object) in &site.objects {
                    let retained = match parameters[callee].get(&index) {
                        Some(&parameter) => {
                            let class = self.class_of(parameter);
                            self.flags(class) & ESCAPE != 0 || self.returned(class)
                        }
                        None => true,
                    };
                    if retained {
                        leaked.insert(object);
                    }
                }
            }
            self.leaked_statics
                .extend(leaked.iter().map(|&binding| ProgramBinding {
                    unit: entry.unit,
                    binding,
                }));
            for site in &summary.calls {
                let callee = site.callee.0 as usize;
                for &(index, object) in &site.objects {
                    if leaked.contains(&object)
                        && summary.automatic.contains(&object)
                        && candidates[callee]
                        && let Some(&parameter) = parameters[callee].get(&index)
                    {
                        self.pinned.push(parameter);
                    }
                }
            }
        }
    }

    fn propagate_raw(&mut self, index: &Summaries) {
        let Summaries {
            functions: summaries,
            base,
            ..
        } = index;
        for (function, summary) in summaries.iter().enumerate() {
            let Some(summary) = summary else { continue };
            for &node in &summary.projected {
                let node = base[function] + node;
                if self.flags(self.class_of(node)) & NULLABLE != 0 {
                    self.make_raw(node, Reason::NullableProjection);
                }
            }
        }
        reach_labeled(self.node_count(), &mut self.edges.copies, &mut self.raw);
    }
}

struct UnionFind {
    parent: Vec<u32>,
    size: Vec<u32>,
}

impl UnionFind {
    fn new(count: usize) -> Self {
        Self {
            parent: (0..count as u32).collect(),
            size: vec![1; count],
        }
    }

    fn find(&mut self, mut node: u32) -> u32 {
        while self.parent[node as usize] != node {
            let grandparent = self.parent[self.parent[node as usize] as usize];
            self.parent[node as usize] = grandparent;
            node = grandparent;
        }
        node
    }

    fn union(&mut self, left: u32, right: u32) {
        let (mut left, mut right) = (self.find(left), self.find(right));
        if left == right {
            return;
        }
        if self.size[left as usize] < self.size[right as usize] {
            std::mem::swap(&mut left, &mut right);
        }
        self.parent[right as usize] = left;
        self.size[left as usize] += self.size[right as usize];
    }
}

fn reach(count: usize, edges: &mut [(u32, u32)], seeds: &mut [bool]) {
    let mut labels: Vec<Option<()>> = seeds.iter().map(|&seed| seed.then_some(())).collect();
    reach_from(count, edges, &mut labels, |()| ());
    for (seed, label) in seeds.iter_mut().zip(labels) {
        *seed = label.is_some();
    }
}

fn reach_labeled(count: usize, edges: &mut [(u32, u32)], origins: &mut [Option<Origin>]) {
    reach_from(count, edges, origins, |origin| Origin {
        propagated: true,
        ..origin
    });
}

fn reach_from<L: Copy>(
    count: usize,
    edges: &mut [(u32, u32)],
    labels: &mut [Option<L>],
    carry: impl Fn(L) -> L,
) {
    edges.sort_unstable();
    let mut start = vec![0usize; count + 1];
    for &(from, _) in edges.iter() {
        start[from as usize + 1] += 1;
    }
    for index in 0..count {
        start[index + 1] += start[index];
    }
    let mut stack: Vec<u32> = (0..count as u32)
        .filter(|&n| labels[n as usize].is_some())
        .collect();
    while let Some(node) = stack.pop() {
        let Some(label) = labels[node as usize] else {
            continue;
        };
        for &(_, to) in &edges[start[node as usize]..start[node as usize + 1]] {
            if labels[to as usize].is_none() {
                labels[to as usize] = Some(carry(label));
                stack.push(to);
            }
        }
    }
}

fn propagate(flags: &mut [u8], flag: u8, edges: &mut [(u32, u32)]) {
    let mut seeds: Vec<bool> = flags.iter().map(|flags| flags & flag != 0).collect();
    reach(flags.len(), edges, &mut seeds);
    for (flags, seeded) in flags.iter_mut().zip(seeds) {
        if seeded {
            *flags |= flag;
        }
    }
}

pub(super) fn capability(flags: u8, raw: bool, byte_elements: bool) -> PointerCapability {
    PointerCapability {
        write: flags & WRITE != 0,
        unique: flags & SHARED == 0,
        free: flags & FREE != 0,
        offset: flags & OFFSET != 0,
        escape: flags & ESCAPE != 0,
        nullable: flags & NULLABLE != 0,
        string: flags & STRING != 0 && byte_elements,
        raw,
        byte_elements,
    }
}
