pub(super) mod reducible;

use std::collections::{BTreeMap, BTreeSet};

use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{
    Arena, FunctionOptimizer, MatchArmNode, NodeId, NodeKind, NodeKindTag,
};
use crate::backend::rust_ast::{Expr, Ident, Label, Pattern, RustValue};

pub(in crate::backend::engine) struct StructureGoto;

pub(super) struct Dispatch {
    pub(super) state: Ident,
    pub(super) label: Label,
    pub(super) match_id: NodeId,
    pub(super) arms: Vec<(i64, Vec<NodeId>)>,
}

pub(super) struct Uses {
    pub(super) entry: NodeId,
    assigns: Vec<NodeId>,
}

pub(super) fn state_literal(expr: &Expr) -> Option<i64> {
    match expr {
        Expr::Value(RustValue::I64(value)) => Some(*value),
        _ => None,
    }
}

pub(super) fn assigned_state(arena: &Arena, id: NodeId, state: Ident) -> Option<i64> {
    let NodeKind::Assign {
        target: Expr::Var(target),
        value,
    } = arena.get(id)?
    else {
        return None;
    };
    (*target == state).then(|| state_literal(value))?
}

pub(super) fn parse(arena: &Arena, id: NodeId) -> Option<Dispatch> {
    let NodeKind::Loop {
        label: Some(label),
        body,
    } = arena.get(id)?
    else {
        return None;
    };
    if !label.as_str().starts_with("__dispatch") {
        return None;
    }
    let [match_id] = body[..] else {
        return None;
    };
    let NodeKind::Match {
        expr: Expr::Var(state),
        arms,
    } = arena.get(match_id)?
    else {
        return None;
    };
    if !state.as_str().starts_with("__state") {
        return None;
    }
    let (wildcard, cases) = arms.split_last()?;
    if wildcard.pattern != Pattern::Wildcard {
        return None;
    }
    let mut parsed = Vec::with_capacity(cases.len());
    for arm in cases {
        let Pattern::I64(value) = arm.pattern else {
            return None;
        };
        parsed.push((value, arm.body.clone()));
    }
    Some(Dispatch {
        state: *state,
        label: label.clone(),
        match_id,
        arms: parsed,
    })
}

pub(super) fn collect_uses(arena: &Arena, dispatch: &Dispatch) -> Option<Uses> {
    let entry = arena.definition(dispatch.state)?;
    let NodeKind::Let {
        init: Some(init), ..
    } = arena.get(entry)?
    else {
        return None;
    };
    state_literal(init)?;
    let mut assigns = Vec::new();
    let mut root = dispatch.match_id;
    while let Some(parent) = arena.parent(root) {
        root = parent;
    }
    scan(arena, root, dispatch, entry, &mut assigns).then_some(Uses { entry, assigns })
}

fn scan(
    arena: &Arena,
    id: NodeId,
    dispatch: &Dispatch,
    entry: NodeId,
    assigns: &mut Vec<NodeId>,
) -> bool {
    let Some(kind) = arena.get(id) else {
        return false;
    };
    if id != entry && id != dispatch.match_id && arena.reads(id).contains(&dispatch.state) {
        match assigned_state(arena, id, dispatch.state) {
            Some(_) => assigns.push(id),
            None => return false,
        }
    }
    kind.child_lists()
        .into_iter()
        .flatten()
        .all(|&child| arena.get(child).is_some() && scan(arena, child, dispatch, entry, assigns))
}

fn forwards_to(arena: &Arena, dispatch: &Dispatch, body: &[NodeId]) -> Option<i64> {
    let [assign, jump] = body[..] else {
        return None;
    };
    let Some(NodeKind::Continue(Some(label))) = arena.get(jump) else {
        return None;
    };
    (*label == dispatch.label).then(|| assigned_state(arena, assign, dispatch.state))?
}

pub(super) fn successors(
    arena: &Arena,
    dispatch: &Dispatch,
    body: &[NodeId],
    out: &mut BTreeSet<i64>,
) {
    for &id in body {
        if let Some(target) = assigned_state(arena, id, dispatch.state) {
            out.insert(target);
        }
        let Some(kind) = arena.get(id) else {
            continue;
        };
        for list in kind.child_lists() {
            successors(arena, dispatch, list, out);
        }
    }
}

fn thread(forwarding: &BTreeMap<i64, i64>, start: i64) -> i64 {
    let mut seen = BTreeSet::from([start]);
    let mut current = start;
    while let Some(&next) = forwarding.get(&current) {
        if !seen.insert(next) {
            return start;
        }
        current = next;
    }
    current
}

struct Plan {
    remap: BTreeMap<i64, i64>,
    keep: Vec<bool>,
}

fn plan(arena: &Arena, dispatch: &Dispatch, uses: &Uses) -> Option<Plan> {
    let NodeKind::Let {
        init: Some(init), ..
    } = arena.get(uses.entry)?
    else {
        return None;
    };
    let entry_state = state_literal(init)?;

    let mut forwarding = BTreeMap::new();
    for (state, body) in &dispatch.arms {
        if let Some(target) = forwards_to(arena, dispatch, body) {
            forwarding.insert(*state, target);
        }
    }

    let mut edges: BTreeMap<i64, BTreeSet<i64>> = BTreeMap::new();
    for (state, body) in &dispatch.arms {
        let mut out = BTreeSet::new();
        successors(arena, dispatch, body, &mut out);
        edges.insert(
            *state,
            out.into_iter().map(|s| thread(&forwarding, s)).collect(),
        );
    }

    let mut reachable = BTreeSet::new();
    let mut stack = vec![thread(&forwarding, entry_state)];
    while let Some(state) = stack.pop() {
        if !reachable.insert(state) {
            continue;
        }
        for &next in edges.get(&state).into_iter().flatten() {
            stack.push(next);
        }
    }

    let keep: Vec<bool> = dispatch
        .arms
        .iter()
        .map(|(state, _)| reachable.contains(state))
        .collect();
    let mut remap = BTreeMap::new();
    let mut next = 0;
    for (index, (state, _)) in dispatch.arms.iter().enumerate() {
        if keep[index] {
            remap.insert(*state, next);
            next += 1;
        }
    }
    for &state in forwarding.keys() {
        let target = thread(&forwarding, state);
        if let Some(&renumbered) = remap.get(&target) {
            remap.insert(state, renumbered);
        }
    }

    let unchanged = keep.iter().all(|kept| *kept)
        && remap.iter().all(|(old, new)| old == new)
        && thread(&forwarding, entry_state) == entry_state;
    (!unchanged).then_some(Plan { remap, keep })
}

fn retarget(arena: &mut FunctionOptimizer, id: NodeId, remap: &BTreeMap<i64, i64>) -> bool {
    let Some(kind) = arena.get_mut(id) else {
        return false;
    };
    let value = match kind {
        NodeKind::Assign { value, .. }
        | NodeKind::Let {
            init: Some(value), ..
        } => value,
        _ => return false,
    };
    let Some(target) = state_literal(value) else {
        return false;
    };
    let Some(&renumbered) = remap.get(&target) else {
        return false;
    };
    *value = Expr::Value(RustValue::I64(renumbered));
    arena.touch(id);
    true
}

impl NodeRule for StructureGoto {
    fn name(&self) -> &'static str {
        "structure_goto::normalize"
    }

    fn priority(&self) -> u32 {
        1
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Loop]
    }

    fn requeues_moved_nodes(&self) -> bool {
        true
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(dispatch) = parse(arena, id) else {
            return false;
        };
        let Some(uses) = collect_uses(arena, &dispatch) else {
            return false;
        };
        plan(arena, &dispatch, &uses).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(dispatch) = parse(arena, id) else {
            return false;
        };
        let Some(uses) = collect_uses(arena, &dispatch) else {
            return false;
        };
        let Some(Plan { remap, keep }) = plan(arena, &dispatch, &uses) else {
            return false;
        };

        retarget(arena, uses.entry, &remap);
        for &assign in &uses.assigns {
            retarget(arena, assign, &remap);
        }

        let Some(NodeKind::Match { expr, arms }) = arena.get(dispatch.match_id) else {
            return false;
        };
        let expr = expr.clone();
        let mut kept = Vec::with_capacity(arms.len());
        let mut dead = Vec::new();
        for (index, arm) in arms.iter().enumerate() {
            match keep.get(index) {
                Some(false) => dead.extend(arm.body.iter().copied()),
                Some(true) => kept.push(MatchArmNode {
                    pattern: Pattern::I64(remap[&dispatch.arms[index].0]),
                    body: arm.body.clone(),
                }),
                None => kept.push(MatchArmNode {
                    pattern: arm.pattern.clone(),
                    body: arm.body.clone(),
                }),
            }
        }
        for id in dead {
            arena.discard_subtree(id);
        }
        arena.set_kind(dispatch.match_id, NodeKind::Match { expr, arms: kept });
        arena.touch_subtree(id);
        true
    }
}
