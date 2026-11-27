use super::flow::Flow;
use super::length::accesses_within_count;
use super::summary::{Node, NodeKind, Summaries, Summary};
use super::{Borrow, ESCAPE, FREE, Origin, PointerFacts, Reason, SHARED, STRING, WRITE};
use crate::counted_loops::CountedLoop;
use crate::program::{FunctionEntry, FunctionId, Program, ProgramBinding};
use foldhash::{HashMap, HashMapExt};
use slate_parser::ir::{BindingId, Parameters};

#[derive(Clone, Copy, PartialEq, Eq, Hash, PartialOrd, Ord)]
enum Outcome {
    Lifted(&'static str),
    NotCandidate(&'static str),
    Raw(&'static str, Via),
}

#[derive(Clone, Copy, PartialEq, Eq, Hash, PartialOrd, Ord)]
enum Via {
    Local,
    Propagated,
}

pub(super) fn report(
    program: &Program<'_>,
    index: &Summaries,
    flow: &Flow,
    unproven: &[Option<Origin>],
    facts: &PointerFacts,
    sites: bool,
) {
    let mut totals: HashMap<(&'static str, Outcome), u32> = HashMap::new();
    for (function, summary) in index.functions.iter().enumerate() {
        let Some(summary) = summary else { continue };
        let Some(entry) = program.function(FunctionId(function as u32)) else {
            continue;
        };
        let base = index.base[function];
        for (node, item) in summary.nodes.iter().enumerate() {
            if matches!(item.kind, NodeKind::Local | NodeKind::Temporary) {
                continue;
            }
            let role = role(item.kind);
            let global = base + node as u32;
            let lifted = lift(facts, &entry, item).map(Lift::kind);
            let outcome = match (index.blockers[function], lifted) {
                (Some(blocker), _) => Outcome::NotCandidate(blocker),
                (None, Some(kind)) => Outcome::Lifted(kind),
                (None, None) => raw_reason(
                    flow,
                    unproven,
                    global,
                    item.opaque_pointee,
                    item.byte_elements,
                ),
            };
            *totals.entry((role, outcome)).or_default() += 1;
            if sites {
                let (kind, reason, via) = fields(outcome);
                eprintln!(
                    "lift-site function={} slot={} outcome={kind} reason={reason} via={via}",
                    entry.function.value.name,
                    slot_name(&entry, item.kind)
                );
            }
        }
    }
    let mut totals: Vec<_> = totals.into_iter().collect();
    totals.sort_by(|(left, left_count), (right, right_count)| {
        right_count.cmp(left_count).then(left.cmp(right))
    });
    for ((role, outcome), count) in totals {
        let (kind, reason, via) = fields(outcome);
        eprintln!("lift-reason role={role} outcome={kind} reason={reason} via={via} count={count}");
    }
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum Lift {
    Vec,
    CStr,
    Borrow(Borrow, bool),
}

impl Lift {
    fn kind(self) -> &'static str {
        match self {
            Lift::Vec => "vec",
            Lift::CStr => "c_str",
            Lift::Borrow(borrow, length) => borrow_kind(borrow, length),
        }
    }
}

fn lift(facts: &PointerFacts, entry: &FunctionEntry<'_>, item: &Node) -> Option<Lift> {
    match item.kind {
        NodeKind::Parameter(_) => facts
            .bindings
            .get(&ProgramBinding {
                unit: entry.unit,
                binding: item.binding,
            })
            .and_then(|fact| {
                if fact.owner.is_some() {
                    Some(Lift::Vec)
                } else if fact.c_string {
                    Some(Lift::CStr)
                } else {
                    fact.borrow
                        .map(|borrow| Lift::Borrow(borrow, fact.length.is_some()))
                }
            }),
        NodeKind::Return => facts
            .returns
            .get(&ProgramBinding {
                unit: entry.unit,
                binding: entry.function.value.id,
            })
            .map(|&borrow| Lift::Borrow(borrow, false)),
        NodeKind::Local | NodeKind::Temporary => None,
    }
}

#[expect(
    clippy::too_many_arguments,
    reason = "the shim measurement compares the real facts with a second solve"
)]
pub(super) fn report_shims(
    program: &Program<'_>,
    index: &Summaries,
    flow: &Flow,
    unproven: &[Option<Origin>],
    before: &PointerFacts,
    after: &PointerFacts,
    counted_loops: &[HashMap<BindingId, CountedLoop>],
    sites: bool,
) {
    let mut shims: HashMap<(&'static str, &'static str, Outcome), u32> = HashMap::new();
    let mut callers: HashMap<(&'static str, &'static str, &'static str), u32> = HashMap::new();
    for (function, summary) in index.functions.iter().enumerate() {
        let Some(summary) = summary else { continue };
        let Some(entry) = program.function(FunctionId(function as u32)) else {
            continue;
        };
        let base = index.base[function];
        let slots = summary
            .nodes
            .iter()
            .enumerate()
            .filter(|(_, item)| matches!(item.kind, NodeKind::Parameter(_) | NodeKind::Return));
        if !index.external[function] {
            for (_, item) in slots {
                let (old, new) = (
                    lift(before, &entry, item).map(Lift::kind),
                    lift(after, &entry, item).map(Lift::kind),
                );
                let change = match (old, new) {
                    (None, Some(_)) => "gained",
                    (Some(_), None) => "lost",
                    (Some(old), Some(new)) if old != new => "changed",
                    _ => continue,
                };
                *callers
                    .entry((role(item.kind), change, new.or(old).unwrap_or("-")))
                    .or_default() += 1;
            }
            continue;
        }
        let pin = index.blockers[function].unwrap_or("-");
        let mut outcomes: Vec<(&Node, Outcome)> = slots
            .map(|(node, item)| {
                let outcome = match lift(after, &entry, item) {
                    Some(Lift::Borrow(borrow, false)) if borrow.many => match item.kind {
                        NodeKind::Parameter(position)
                            if body_length(summary, &entry, position, counted_loops) =>
                        {
                            Outcome::Lifted(Lift::Borrow(borrow, true).kind())
                        }
                        NodeKind::Parameter(_)
                        | NodeKind::Return
                        | NodeKind::Local
                        | NodeKind::Temporary => Outcome::Raw("shim.unbounded_many", Via::Local),
                    },
                    Some(lift) => Outcome::Lifted(lift.kind()),
                    None => raw_reason(
                        flow,
                        unproven,
                        base + node as u32,
                        item.opaque_pointee,
                        item.byte_elements,
                    ),
                };
                (item, outcome)
            })
            .collect();
        let lifted_parameters = outcomes
            .iter()
            .filter(|(item, outcome)| {
                matches!(item.kind, NodeKind::Parameter(_)) && matches!(outcome, Outcome::Lifted(_))
            })
            .count();
        if lifted_parameters > 1 {
            for (item, outcome) in &mut outcomes {
                if matches!(item.kind, NodeKind::Parameter(_))
                    && matches!(lift(after, &entry, item), Some(Lift::Borrow(borrow, _)) if borrow.mutable)
                    && matches!(outcome, Outcome::Lifted(_))
                {
                    *outcome = Outcome::Raw("shim.aliased_mut", Via::Local);
                }
            }
        }
        for (item, outcome) in outcomes {
            *shims.entry((role(item.kind), pin, outcome)).or_default() += 1;
            if sites {
                let (kind, reason, via) = shim_fields(outcome);
                eprintln!(
                    "lift-shim-site function={} pin={pin} slot={} outcome={kind} reason={reason} via={via}",
                    entry.function.value.name,
                    slot_name(&entry, item.kind)
                );
            }
        }
    }
    let mut shims: Vec<_> = shims.into_iter().collect();
    shims.sort_by(|(left, left_count), (right, right_count)| {
        right_count.cmp(left_count).then(left.cmp(right))
    });
    for ((role, pin, outcome), count) in shims {
        let (kind, reason, via) = shim_fields(outcome);
        eprintln!(
            "lift-shim role={role} pin={pin} outcome={kind} reason={reason} via={via} count={count}"
        );
    }
    let mut callers: Vec<_> = callers.into_iter().collect();
    callers.sort_by(|(left, left_count), (right, right_count)| {
        right_count.cmp(left_count).then(left.cmp(right))
    });
    for ((role, change, kind), count) in callers {
        eprintln!("lift-shim-caller role={role} change={change} reason={kind} count={count}");
    }
}

fn body_length(
    summary: &Summary,
    entry: &FunctionEntry<'_>,
    pointer: usize,
    counted_loops: &[HashMap<BindingId, CountedLoop>],
) -> bool {
    let unwritten = |index: usize| !summary.written.contains(&summary.parameters[index]);
    unwritten(pointer)
        && summary
            .lengths
            .iter()
            .enumerate()
            .any(|(length, &integer)| {
                integer
                    && length != pointer
                    && unwritten(length)
                    && accesses_within_count(
                        &entry.function.value,
                        summary.parameters[pointer],
                        summary.parameters[length],
                        &counted_loops[entry.unit.0 as usize],
                    )
            })
}

fn role(kind: NodeKind) -> &'static str {
    match kind {
        NodeKind::Parameter(_) => "parameter",
        NodeKind::Return | NodeKind::Local | NodeKind::Temporary => "return",
    }
}

fn slot_name(entry: &FunctionEntry<'_>, kind: NodeKind) -> String {
    match kind {
        NodeKind::Parameter(position) => parameter_name(&entry.function.value.parameters, position),
        NodeKind::Return | NodeKind::Local | NodeKind::Temporary => "return".to_owned(),
    }
}

fn shim_fields(outcome: Outcome) -> (&'static str, &'static str, &'static str) {
    match outcome {
        Outcome::Lifted(kind) => ("would_lift_with_shim", kind, "-"),
        Outcome::NotCandidate(_) | Outcome::Raw(..) => fields(outcome),
    }
}

fn raw_reason(
    flow: &Flow,
    unproven: &[Option<Origin>],
    node: u32,
    opaque_pointee: bool,
    byte_elements: bool,
) -> Outcome {
    let class = flow.class_of(node);
    let flags = flow.flags(class);
    let origin = |origin: Origin| {
        Outcome::Raw(
            origin.reason.name(),
            if origin.propagated {
                Via::Propagated
            } else {
                Via::Local
            },
        )
    };
    if opaque_pointee {
        return Outcome::Raw("void_pointee", Via::Local);
    }
    if let Some(raw) = flow.raw_origin(node) {
        return origin(raw);
    }
    if flags & ESCAPE != 0 {
        return origin(
            flow.escape_origin(class)
                .unwrap_or(Origin::local(Reason::Unlabeled)),
        );
    }
    if flags & FREE != 0 {
        return Outcome::Raw("freed", Via::Local);
    }
    if flags & STRING != 0 && byte_elements {
        return Outcome::Raw("string_not_c_str", Via::Local);
    }
    if flags & WRITE != 0 && flags & SHARED != 0 {
        return Outcome::Raw("shared_write", Via::Local);
    }
    match unproven[class as usize] {
        Some(unproven) => origin(unproven),
        None => Outcome::Raw("unknown", Via::Local),
    }
}

fn borrow_kind(borrow: Borrow, length: bool) -> &'static str {
    match (
        borrow.nullable,
        borrow.mutable,
        borrow.many || length,
        length,
    ) {
        (false, false, false, _) => "ref",
        (false, true, false, _) => "mut",
        (false, false, true, false) => "slice",
        (false, true, true, false) => "mut_slice",
        (false, false, true, true) => "slice_length",
        (false, true, true, true) => "mut_slice_length",
        (true, false, false, _) => "option_ref",
        (true, true, false, _) => "option_mut",
        (true, false, true, false) => "option_slice",
        (true, true, true, false) => "option_mut_slice",
        (true, false, true, true) => "option_slice_length",
        (true, true, true, true) => "option_mut_slice_length",
    }
}

fn fields(outcome: Outcome) -> (&'static str, &'static str, &'static str) {
    match outcome {
        Outcome::Lifted(kind) => ("lifted", kind, "-"),
        Outcome::NotCandidate(pin) => ("not_candidate", pin, "-"),
        Outcome::Raw(reason, Via::Local) => ("raw", reason, "local"),
        Outcome::Raw(reason, Via::Propagated) => ("raw", reason, "propagated"),
    }
}

fn parameter_name(parameters: &Parameters, position: usize) -> String {
    let Parameters::Prototype { fixed, .. } = parameters else {
        return position.to_string();
    };
    fixed
        .get(position)
        .and_then(|parameter| parameter.value.name.clone())
        .unwrap_or_else(|| position.to_string())
}
