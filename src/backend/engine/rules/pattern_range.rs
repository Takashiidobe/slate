use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::Pattern;

const MIN_RUN: usize = 3;

fn signed_value(pattern: &Pattern) -> Option<i128> {
    match pattern {
        Pattern::I64(n) => Some(i128::from(*n)),
        Pattern::I128(n) => Some(*n),
        Pattern::U128(n) => i128::try_from(*n).ok(),
        _ => None,
    }
}

fn unsigned_value(pattern: &Pattern) -> Option<u128> {
    match pattern {
        Pattern::U128(n) => Some(*n),
        _ => None,
    }
}

fn keyed<T>(alts: &[Pattern], key: impl Fn(&Pattern) -> Option<T>) -> Option<Vec<(T, Pattern)>> {
    alts.iter()
        .map(|alt| key(alt).map(|value| (value, alt.clone())))
        .collect()
}

fn fold_runs<T: Copy + Ord>(
    mut values: Vec<(T, Pattern)>,
    successor: impl Fn(T) -> Option<T>,
    range: impl Fn(T, T) -> Pattern,
) -> Vec<Pattern> {
    values.sort_by_key(|(value, _)| *value);
    values.dedup_by_key(|(value, _)| *value);

    let mut out: Vec<Pattern> = Vec::new();
    let mut run: Vec<(T, Pattern)> = Vec::new();
    for entry in values {
        match run.last() {
            Some((prev, _)) if successor(*prev) == Some(entry.0) => run.push(entry),
            _ => {
                flush_run(&mut run, &mut out, &range);
                run.push(entry);
            }
        }
    }
    flush_run(&mut run, &mut out, &range);
    out
}

fn flush_run<T: Copy>(
    run: &mut Vec<(T, Pattern)>,
    out: &mut Vec<Pattern>,
    range: impl Fn(T, T) -> Pattern,
) {
    if run.len() >= MIN_RUN {
        out.push(range(run[0].0, run[run.len() - 1].0));
        run.clear();
        return;
    }
    out.extend(run.drain(..).map(|(_, pattern)| pattern));
}

fn folded_alts(alts: &[Pattern]) -> Option<Vec<Pattern>> {
    if let Some(values) = keyed(alts, unsigned_value) {
        return Some(fold_runs(
            values,
            |value| value.checked_add(1),
            |start, end| Pattern::InclusiveRangeU128 { start, end },
        ));
    }
    let values = keyed(alts, signed_value)?;
    Some(fold_runs(
        values,
        |value| value.checked_add(1),
        |start, end| Pattern::InclusiveRange { start, end },
    ))
}

fn rewrite_of(pattern: &Pattern) -> Option<Pattern> {
    let Pattern::Or(alts) = pattern else {
        return None;
    };
    let folded = folded_alts(alts)?;
    let rewritten = match <[Pattern; 1]>::try_from(folded) {
        Ok([single]) => single,
        Err(alts) => Pattern::Or(alts),
    };
    (rewritten != *pattern).then_some(rewritten)
}

pub(in crate::backend::engine) struct MatchRangeFold;

impl NodeRule for MatchRangeFold {
    fn name(&self) -> &'static str {
        "pattern_range::fold"
    }

    fn priority(&self) -> u32 {
        47
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Match]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::Match { arms, .. }) = arena.get(id) else {
            return false;
        };
        arms.iter().any(|arm| rewrite_of(&arm.pattern).is_some())
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::Match { arms, .. }) = arena.get_mut(id) else {
            return false;
        };
        let mut changed = false;
        for arm in arms.iter_mut() {
            if let Some(rewritten) = rewrite_of(&arm.pattern) {
                arm.pattern = rewritten;
                changed = true;
            }
        }
        if !changed {
            return false;
        }
        arena.touch(id);
        true
    }
}
