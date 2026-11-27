use super::flow::Flow;
use super::summary::{NodeKind, Summaries};
use super::{Borrow, ESCAPE, FREE, Origin, PointerFacts, Reason, SHARED, STRING, WRITE};
use crate::program::{FunctionId, Program, ProgramBinding};
use foldhash::{HashMap, HashMapExt};
use slate_parser::ir::Parameters;

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
            let role = match item.kind {
                NodeKind::Parameter(_) => "parameter",
                NodeKind::Return => "return",
                NodeKind::Local | NodeKind::Temporary => continue,
            };
            let global = base + node as u32;
            let lifted = match item.kind {
                NodeKind::Parameter(_) => facts
                    .bindings
                    .get(&ProgramBinding {
                        unit: entry.unit,
                        binding: item.binding,
                    })
                    .and_then(|fact| {
                        if fact.owner.is_some() {
                            Some("vec")
                        } else if fact.c_string {
                            Some("c_str")
                        } else {
                            fact.borrow
                                .map(|borrow| borrow_kind(borrow, fact.length.is_some()))
                        }
                    }),
                NodeKind::Return => facts
                    .returns
                    .get(&ProgramBinding {
                        unit: entry.unit,
                        binding: entry.function.value.id,
                    })
                    .map(|&borrow| borrow_kind(borrow, false)),
                NodeKind::Local | NodeKind::Temporary => None,
            };
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
                let slot = match item.kind {
                    NodeKind::Parameter(position) => {
                        parameter_name(&entry.function.value.parameters, position)
                    }
                    NodeKind::Return | NodeKind::Local | NodeKind::Temporary => "return".to_owned(),
                };
                let (kind, reason, via) = fields(outcome);
                eprintln!(
                    "lift-site function={} slot={slot} outcome={kind} reason={reason} via={via}",
                    entry.function.value.name
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
