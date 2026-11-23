use super::flow::Flow;
use super::summary::{Shape, Summaries};
use super::{ESCAPE, FREE, MACRO, NULLABLE, STRING, WRITE};
use foldhash::{HashSet, HashSetExt};

pub(super) fn parameters(index: &Summaries, flow: &Flow) -> HashSet<(usize, usize)> {
    let Summaries {
        functions: summaries,
        base,
        candidates,
        parameters,
        incoming,
    } = index;
    let mut active = HashSet::new();
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        if !candidates[function] || incoming[function].is_empty() {
            continue;
        }
        for (&index, &node) in &parameters[function] {
            let item = &summary.nodes[(node - base[function]) as usize];
            let class = flow.class_of(node);
            let flags = flow.flags(class);
            if item.byte_elements
                && !item.opaque_pointee
                && !flow.raw(node)
                && !flow.returned(class)
                && !flow.pinned().contains(&node)
                && flags & STRING != 0
                && flags & (WRITE | FREE | ESCAPE | NULLABLE | MACRO) == 0
                && !summary.written.contains(&summary.parameters[index])
            {
                active.insert((function, index));
            }
        }
    }
    loop {
        let next: HashSet<(usize, usize)> = active
            .iter()
            .copied()
            .filter(|&(function, index)| {
                incoming[function].iter().all(|&(caller, site)| {
                    let Some(summary) = &summaries[caller] else {
                        return false;
                    };
                    let site = &summary.calls[site];
                    let forwarded = match site.shapes.get(index) {
                        Some(Shape::Binding(binding)) => summary
                            .unwritten_parameter(*binding)
                            .is_some_and(|parameter| active.contains(&(caller, parameter))),
                        Some(_) | None => false,
                    };
                    !site.pinned
                        && (forwarded || site.literals.get(index).copied().unwrap_or(false))
                })
            })
            .collect();
        if next == active {
            return active;
        }
        active = next;
    }
}
