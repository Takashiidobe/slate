use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{
    self, Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag,
};
use crate::backend::rust_ast::{BinOp, Expr, Ident, Label, MatchArm, Pattern, RustValue, Stmt};

const MAX_DUPLICATED_STMTS: usize = 24;

pub(in crate::backend::engine) struct StructureDispatch;

#[derive(Clone, Copy, PartialEq, Eq)]
enum Term {
    Exit,
    FallsThrough,
    Diverges,
}

struct Arm {
    patterns: Vec<Pattern>,
    body: Vec<Stmt>,
    term: Term,
}

struct Switch {
    label: Label,
    selector: Expr,
    fallback: i64,
    needs_label: bool,
    arms: Vec<Arm>,
}

fn scope_body(arena: &Arena, id: NodeId) -> Option<[NodeId; 3]> {
    let NodeKind::Scope { body } = arena.get(id)? else {
        return None;
    };
    let [value_id, case_id, loop_id] = body[..] else {
        return None;
    };
    Some([value_id, case_id, loop_id])
}

fn named_let(arena: &Arena, id: NodeId, prefix: &str) -> Option<(Ident, Expr)> {
    let NodeKind::Let {
        name,
        init: Some(init),
        ..
    } = arena.get(id)?
    else {
        return None;
    };
    name.as_str()
        .starts_with(prefix)
        .then(|| (*name, init.clone()))
}

fn parse(arena: &Arena, id: NodeId) -> Option<Switch> {
    let [value_id, case_id, loop_id] = scope_body(arena, id)?;
    let (value_name, selector) = named_let(arena, value_id, "__switch_value")?;
    let (case_name, dispatch) = named_let(arena, case_id, "__switch_case")?;
    let Expr::Match {
        expr,
        arms: sel_arms,
    } = &dispatch
    else {
        return None;
    };
    if **expr != Expr::Var(value_name) {
        return None;
    }

    let NodeKind::Loop {
        label: Some(label),
        body: loop_body,
    } = arena.get(loop_id)?
    else {
        return None;
    };
    let [match_id] = loop_body[..] else {
        return None;
    };
    let NodeKind::Match {
        expr,
        arms: case_arms,
    } = arena.get(match_id)?
    else {
        return None;
    };
    if *expr != Expr::Var(case_name) {
        return None;
    }

    let (default_arm, case_arms) = case_arms.split_last()?;
    if default_arm.pattern != Pattern::Wildcard || !exits(arena, &default_arm.body, label) {
        return None;
    }

    let (wildcard, value_arms) = sel_arms.split_last()?;
    let Expr::Value(RustValue::I64(fallback)) = wildcard.value else {
        return None;
    };
    if wildcard.pattern != Pattern::Wildcard || fallback >= case_arms.len() as i64 {
        return None;
    }
    let mut patterns: Vec<Vec<Pattern>> = vec![Vec::new(); case_arms.len()];
    for arm in value_arms {
        let Expr::Value(RustValue::I64(index)) = arm.value else {
            return None;
        };
        patterns
            .get_mut(usize::try_from(index).ok()?)?
            .push(arm.pattern.clone());
    }

    let mut needs_label = false;
    let mut arms = Vec::new();
    for (index, arm) in case_arms.iter().enumerate() {
        if arm.pattern != Pattern::I64(index as i64) {
            return None;
        }
        let (term, kept) = terminator(arena, &arm.body, label, case_name, index)?;
        if !scan(arena, kept, label, case_name, &mut needs_label) {
            return None;
        }
        arms.push(Arm {
            patterns: std::mem::take(&mut patterns[index]),
            body: arena::reify_bodies(arena, kept),
            term,
        });
    }
    if matches!(arms.last()?.term, Term::FallsThrough) {
        return None;
    }

    Some(Switch {
        label: label.clone(),
        selector,
        fallback,
        needs_label,
        arms,
    })
}

fn exits(arena: &Arena, body: &[NodeId], label: &Label) -> bool {
    matches!(body[..], [only] if matches!(arena.get(only), Some(NodeKind::Break(Some(l))) if l == label))
}

fn terminator<'a>(
    arena: &Arena,
    body: &'a [NodeId],
    label: &Label,
    case_name: Ident,
    index: usize,
) -> Option<(Term, &'a [NodeId])> {
    if let Some((&last, kept)) = body.split_last()
        && let Some(NodeKind::Break(Some(l))) = arena.get(last)
        && l == label
    {
        return Some((Term::Exit, kept));
    }
    if let [kept @ .., assign, jump] = body
        && let Some(NodeKind::Continue(Some(l))) = arena.get(*jump)
        && l == label
        && let Some(NodeKind::Assign {
            target: Expr::Var(target),
            value: Expr::Value(RustValue::I64(next)),
        }) = arena.get(*assign)
        && *target == case_name
    {
        return (*next == index as i64 + 1).then_some((Term::FallsThrough, kept));
    }
    Some((Term::Diverges, body))
}

fn scan(
    arena: &Arena,
    ids: &[NodeId],
    label: &Label,
    case_name: Ident,
    needs_label: &mut bool,
) -> bool {
    for &id in ids {
        let Some(kind) = arena.get(id) else {
            return false;
        };
        match kind {
            NodeKind::Continue(Some(l)) if l == label => return false,
            NodeKind::Break(Some(l)) if l == label => *needs_label = true,
            NodeKind::Assign {
                target: Expr::Var(target),
                ..
            }
            | NodeKind::CompoundAssign {
                target: Expr::Var(target),
                ..
            } if *target == case_name => return false,
            _ => {}
        }
        if arena.reads(id).contains(&case_name) {
            return false;
        }
        if !kind
            .child_lists()
            .into_iter()
            .all(|list| scan(arena, list, label, case_name, needs_label))
        {
            return false;
        }
    }
    true
}

fn temp_index(name: &str) -> Option<u32> {
    name.strip_prefix("_v")?.parse().ok()
}

fn is_compiler_temp_let(stmt: &Stmt) -> bool {
    matches!(stmt, Stmt::Let { name, .. } if temp_index(name.as_str()).is_some())
}

fn max_temp(arena: &Arena, id: NodeId) -> u32 {
    let Some(kind) = arena.get(id) else {
        return 0;
    };
    let mut max = kind
        .declared_name()
        .and_then(|name| temp_index(name.as_str()))
        .unwrap_or(0);
    for name in arena.reads(id) {
        max = max.max(temp_index(name.as_str()).unwrap_or(0));
    }
    for list in kind.child_lists() {
        for &child in list {
            max = max.max(max_temp(arena, child));
        }
    }
    max
}

fn rename_decls(body: &mut [Stmt], next: &mut u32, renames: &mut Vec<(String, String)>) {
    for stmt in body.iter_mut() {
        if let Some(name) = stmt.declared_name_mut()
            && temp_index(name).is_some()
        {
            let fresh = format!("_v{}", *next);
            *next += 1;
            renames.push((std::mem::replace(name, fresh.clone()), fresh));
        }
        for nested in stmt.child_bodies_mut() {
            rename_decls(nested, next, renames);
        }
    }
}

fn freshen(body: &mut [Stmt], next: &mut u32) {
    let mut renames = Vec::new();
    rename_decls(body, next, &mut renames);
    for (old, new) in renames {
        let replacement = Expr::Var(new.as_str().into());
        for stmt in body.iter_mut() {
            stmt.substitute_var(&old, &replacement);
        }
    }
}

fn guard_test(pattern: &Pattern, bind: Ident) -> Option<Expr> {
    match pattern {
        Pattern::Guarded { bind: other, cond } if *other == bind => Some((**cond).clone()),
        _ => None,
    }
}

fn merged_guard(patterns: &[Pattern]) -> Option<Pattern> {
    let bind = patterns.iter().find_map(|pattern| match pattern {
        Pattern::Guarded { bind, .. } => Some(*bind),
        _ => None,
    })?;
    let mut cond: Option<Expr> = None;
    for pattern in patterns {
        let test = guard_test(pattern, bind)?;
        cond = Some(match cond {
            None => test,
            Some(prev) => Expr::Binary {
                op: BinOp::Or,
                lhs: Box::new(prev),
                rhs: Box::new(test),
            },
        });
    }
    Some(Pattern::Guarded {
        bind,
        cond: Box::new(cond?),
    })
}

fn pattern_of(switch: &Switch, index: usize) -> Option<Pattern> {
    if switch.fallback == index as i64 {
        return Some(Pattern::Wildcard);
    }
    match switch.arms[index].patterns.as_slice() {
        [] => None,
        [only] => Some(only.clone()),
        many if many
            .iter()
            .any(|pattern| matches!(pattern, Pattern::Guarded { .. })) =>
        {
            merged_guard(many)
        }
        many => Some(Pattern::Or(many.to_vec())),
    }
}

fn rebuild(mut switch: Switch, next_temp: &mut u32) -> Option<Stmt> {
    for index in 0..switch.arms.len() {
        if switch.arms[index].body.is_empty()
            && switch.arms[index].term == Term::FallsThrough
            && switch.fallback != index as i64
        {
            let merged = std::mem::take(&mut switch.arms[index].patterns);
            switch.arms[index + 1].patterns.splice(0..0, merged);
        }
    }

    let mut duplicated = 0;
    let mut arms: Vec<MatchArm> = Vec::new();
    let mut wildcard = None;
    for index in 0..switch.arms.len() {
        let Some(pattern) = pattern_of(&switch, index) else {
            continue;
        };
        let mut body: Vec<Stmt> = switch.arms[index].body.clone();
        let mut cursor = index;
        while switch.arms[cursor].term == Term::FallsThrough {
            cursor += 1;
            duplicated += switch.arms[cursor]
                .body
                .iter()
                .filter(|stmt| !is_compiler_temp_let(stmt))
                .count();
            let mut copy: Vec<Stmt> = switch.arms[cursor].body.clone();
            freshen(&mut copy, next_temp);
            body.extend(copy);
        }
        let arm = MatchArm { pattern, body };
        match arm.pattern {
            Pattern::Wildcard => wildcard = Some(arm),
            _ => arms.push(arm),
        }
    }
    if duplicated > MAX_DUPLICATED_STMTS {
        return None;
    }
    arms.push(wildcard.unwrap_or(MatchArm {
        pattern: Pattern::Wildcard,
        body: Vec::new(),
    }));

    let dispatch = Stmt::Match {
        expr: switch.selector,
        arms,
    };
    Some(if switch.needs_label {
        Stmt::LabeledBlock {
            label: switch.label,
            body: vec![dispatch],
        }
    } else {
        dispatch
    })
}

impl NodeRule for StructureDispatch {
    fn name(&self) -> &'static str {
        "structure_dispatch::structure"
    }

    fn priority(&self) -> u32 {
        1
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Scope]
    }

    fn requeues_moved_nodes(&self) -> bool {
        true
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        scope_body(arena, id)
            .is_some_and(|[value_id, ..]| named_let(arena, value_id, "__switch_value").is_some())
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(switch) = parse(arena, id) else {
            return false;
        };
        let mut root = id;
        while let Some(parent) = arena.parent(root) {
            root = parent;
        }
        let mut next_temp = max_temp(arena, root) + 1;
        let Some(stmt) = rebuild(switch, &mut next_temp) else {
            return false;
        };
        let Some(NodeKind::Scope { body }) = arena.get(id) else {
            return false;
        };
        for child in body.clone() {
            arena.discard_subtree(child);
        }
        let body = arena::insert_stmts(arena, Some(id), vec![stmt]);
        arena.set_kind(id, NodeKind::Scope { body });
        arena.touch_subtree(id);
        true
    }
}
