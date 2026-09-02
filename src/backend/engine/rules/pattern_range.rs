use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::Pattern;

const MIN_RUN: usize = 3;

fn pattern_value(pattern: &Pattern) -> Option<i128> {
    match pattern {
        Pattern::I64(n) => Some(i128::from(*n)),
        Pattern::I128(n) => Some(*n),
        Pattern::U128(n) => i128::try_from(*n).ok(),
        _ => None,
    }
}

fn folded_alts(alts: &[Pattern]) -> Option<Vec<Pattern>> {
    let mut values: Vec<(i128, Pattern)> = alts
        .iter()
        .map(|alt| pattern_value(alt).map(|value| (value, alt.clone())))
        .collect::<Option<_>>()?;
    values.sort_by_key(|(value, _)| *value);
    values.dedup_by_key(|(value, _)| *value);

    let mut out: Vec<Pattern> = Vec::new();
    let mut run: Vec<(i128, Pattern)> = Vec::new();
    for entry in values {
        match run.last() {
            Some((prev, _)) if entry.0 == prev.checked_add(1)? => run.push(entry),
            _ => {
                flush_run(&mut run, &mut out);
                run.push(entry);
            }
        }
    }
    flush_run(&mut run, &mut out);
    Some(out)
}

fn flush_run(run: &mut Vec<(i128, Pattern)>, out: &mut Vec<Pattern>) {
    if run.len() >= MIN_RUN {
        out.push(Pattern::InclusiveRange {
            start: run[0].0,
            end: run[run.len() - 1].0,
        });
        run.clear();
        return;
    }
    out.extend(run.drain(..).map(|(_, pattern)| pattern));
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

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        let Some(NodeKind::Match { arms, .. }) = arena.get(id) else {
            return false;
        };
        arms.iter().any(|arm| rewrite_of(&arm.pattern).is_some())
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
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
