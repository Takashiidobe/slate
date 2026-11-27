use super::flow::{Flow, capability};
use super::ownership::OwnershipFacts;
use super::summary::{NodeKind, Summaries};
use super::{MACRO, OFFSET, Origin, Reason};
use foldhash::HashSet;

pub(super) fn unproven(
    index: &Summaries,
    flow: &Flow,
    ownership: &OwnershipFacts,
    covered: &HashSet<(usize, usize)>,
) -> Vec<Option<Origin>> {
    let Summaries {
        functions: summaries,
        base,
        candidates,
        parameters,
        ..
    } = index;
    let owned = &ownership.nodes;
    let total = flow.node_count();
    let owned_classes: HashSet<u32> = owned.keys().map(|&node| flow.class_of(node)).collect();
    let class_borrow = |flags: u8| capability(flags, false, false).borrow();
    let mut unproven = vec![None; total];
    let mut mark = |class: u32, reason: Reason| {
        unproven[class as usize].get_or_insert(Origin::local(reason));
    };
    for class in flow.classes() {
        if owned_classes.contains(&class) {
            continue;
        }
        if flow.flags(class) & MACRO != 0 {
            mark(class, Reason::MacroArgument);
        } else if class_borrow(flow.flags(class)).is_none() {
            mark(class, Reason::NoBorrow);
        }
    }
    for &parameter in flow.pinned() {
        mark(flow.class_of(parameter), Reason::LeakedAutomatic);
    }
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        let offset = base[function];
        if let Some(node) = summary.returned
            && flow.raw(offset + node)
        {
            mark(flow.class_of(offset + node), Reason::RawReturn);
        }
        for site in &summary.calls {
            let callee = site.callee.0 as usize;
            if !candidates[callee] || index.external[callee] {
                continue;
            }
            for (index, &parameter) in &parameters[callee] {
                let class = flow.class_of(parameter);
                if site.pinned {
                    mark(class, Reason::MacroCallSite);
                } else if site.unproven.contains(index) && !covered.contains(&(callee, *index)) {
                    mark(class, Reason::UnprovenArgument);
                } else if site.parts.contains(index) && flow.flags(class) & OFFSET != 0 {
                    mark(class, Reason::PartOffset);
                }
            }
            for &(index, node) in &site.arguments {
                if covered.contains(&(callee, index)) {
                    continue;
                }
                let global = offset + node;
                let item = &summary.nodes[node as usize];
                let class = flow.class_of(global);
                let lowered_raw = !owned.contains_key(&global)
                    && (flow.raw(global)
                        || item.opaque_pointee
                        || matches!(item.kind, NodeKind::Temporary | NodeKind::Return)
                        || (item.kind == NodeKind::Local
                            && class_borrow(flow.flags(class))
                                .is_none_or(|borrow| borrow.mutable)));
                if lowered_raw && let Some(&parameter) = parameters[callee].get(&index) {
                    mark(flow.class_of(parameter), Reason::RawArgument);
                }
            }
        }
    }

    flow.propagate_unproven(covered, &index.external, &mut unproven);
    unproven
}
