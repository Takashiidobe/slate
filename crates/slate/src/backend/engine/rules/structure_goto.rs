pub(super) mod reducible;

use std::collections::BTreeSet;

use crate::backend::engine::arena::{Arena, NodeId, NodeKind};
use crate::backend::rust_ast::{Expr, Ident, Label, Pattern, RustValue};

pub(super) struct Dispatch {
    pub(super) state: Ident,
    pub(super) label: Label,
    pub(super) match_id: NodeId,
    pub(super) arms: Vec<(i64, Vec<NodeId>)>,
}

pub(super) struct Uses {
    pub(super) entry: NodeId,
}

pub(super) fn state_literal(expr: &Expr) -> Option<i64> {
    match expr {
        Expr::Value(RustValue::I64(value)) => Some(*value),
        Expr::Value(RustValue::Usize(value)) => i64::try_from(*value).ok(),
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
    if !label.as_str().starts_with("__dispatch") && !label.as_str().starts_with("__slate_dispatch")
    {
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
    if !state.as_str().starts_with("__state") && !state.as_str().starts_with("__slate_state") {
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
    let mut root = dispatch.match_id;
    while let Some(parent) = arena.parent(root) {
        root = parent;
    }
    scan(arena, root, dispatch, entry).then_some(Uses { entry })
}

fn scan(arena: &Arena, id: NodeId, dispatch: &Dispatch, entry: NodeId) -> bool {
    let Some(kind) = arena.get(id) else {
        return false;
    };
    if id != entry
        && id != dispatch.match_id
        && arena.reads(id).contains(&dispatch.state)
        && assigned_state(arena, id, dispatch.state).is_none()
    {
        return false;
    }
    kind.child_lists()
        .into_iter()
        .flatten()
        .all(|&child| arena.get(child).is_some() && scan(arena, child, dispatch, entry))
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
