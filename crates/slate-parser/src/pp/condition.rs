use crate::ast::Condition;

pub(super) fn is_statically_false(condition: &Condition) -> bool {
    match condition {
        Condition::Constant(0) => true,
        Condition::And(left, right) => is_statically_false(left) || is_statically_false(right),
        Condition::Or(left, right) => is_statically_false(left) && is_statically_false(right),
        Condition::Not(inner) => is_statically_true(inner),
        _ => false,
    }
}

pub(super) fn is_statically_true(condition: &Condition) -> bool {
    match condition {
        Condition::Constant(value) => *value != 0,
        Condition::And(left, right) => is_statically_true(left) && is_statically_true(right),
        Condition::Or(left, right) => is_statically_true(left) || is_statically_true(right),
        Condition::Not(inner) => is_statically_false(inner),
        _ => false,
    }
}

pub(super) fn implies(condition: &Condition, consequence: &Condition) -> bool {
    condition == consequence
        || is_statically_true(consequence)
        || matches!(condition, Condition::And(left, right)
            if implies(left, consequence) || implies(right, consequence))
}

fn negate(condition: &Condition) -> Condition {
    match condition {
        Condition::Not(inner) => inner.as_ref().clone(),
        _ => Condition::Not(Box::new(condition.clone())),
    }
}

pub(super) fn intersect(left: &Condition, right: &Condition) -> Condition {
    if !is_satisfiable(&conjunction(left, &negate(right))) {
        left.clone()
    } else if !is_satisfiable(&conjunction(right, &negate(left))) {
        right.clone()
    } else {
        conjunction(left, right)
    }
}

pub(super) fn difference(condition: &Condition, excluded: &Condition) -> Condition {
    intersect(condition, &negate(excluded))
}

const MAX_ENUMERATED_NAMES: usize = 16;

pub(super) fn is_satisfiable(condition: &Condition) -> bool {
    let condition = simplify_condition(condition);
    if let Condition::Constant(value) = condition {
        return value != 0;
    }
    let mut names = Vec::new();
    defined_names(&condition, &mut names);
    if names.len() > MAX_ENUMERATED_NAMES {
        return !is_statically_false(&condition);
    }
    (0..1u32 << names.len()).any(|assignment| holds(&condition, &names, assignment))
}

fn defined_names<'c>(condition: &'c Condition, names: &mut Vec<&'c str>) {
    match condition {
        Condition::Defined(name) => {
            if !names.contains(&name.as_str()) {
                names.push(name);
            }
        }
        Condition::Not(inner) => defined_names(inner, names),
        Condition::And(left, right) | Condition::Or(left, right) => {
            defined_names(left, names);
            defined_names(right, names);
        }
        Condition::Constant(_) => {}
    }
}

fn holds(condition: &Condition, names: &[&str], assignment: u32) -> bool {
    match condition {
        Condition::Defined(name) => names
            .iter()
            .position(|candidate| candidate == name)
            .is_some_and(|index| assignment >> index & 1 == 1),
        Condition::Constant(value) => *value != 0,
        Condition::Not(inner) => !holds(inner, names, assignment),
        Condition::And(left, right) => {
            holds(left, names, assignment) && holds(right, names, assignment)
        }
        Condition::Or(left, right) => {
            holds(left, names, assignment) || holds(right, names, assignment)
        }
    }
}

pub(super) fn conjunction(active: &Condition, branch: &Condition) -> Condition {
    simplify_condition(&Condition::And(
        Box::new(active.clone()),
        Box::new(branch.clone()),
    ))
}

pub(super) fn simplify_condition(condition: &Condition) -> Condition {
    match condition {
        Condition::And(left, right) => {
            let left = simplify_condition(left);
            let right = simplify_condition(right);
            match (&left, &right) {
                (Condition::Constant(0), _) | (_, Condition::Constant(0)) => Condition::Constant(0),
                (Condition::Constant(v), _) if *v != 0 => right,
                (_, Condition::Constant(v)) if *v != 0 => left,
                _ => Condition::And(Box::new(left), Box::new(right)),
            }
        }
        Condition::Or(left, right) => {
            let left = simplify_condition(left);
            let right = simplify_condition(right);
            match (&left, &right) {
                (Condition::Constant(v), _) if *v != 0 => Condition::Constant(1),
                (_, Condition::Constant(v)) if *v != 0 => Condition::Constant(1),
                (Condition::Constant(0), _) => right,
                (_, Condition::Constant(0)) => left,
                _ => Condition::Or(Box::new(left), Box::new(right)),
            }
        }
        Condition::Not(inner) => match simplify_condition(inner) {
            Condition::Constant(v) => Condition::Constant(if v != 0 { 0 } else { 1 }),
            other => Condition::Not(Box::new(other)),
        },
        Condition::Defined(_) | Condition::Constant(_) => condition.clone(),
    }
}

pub(super) fn replace_subterm(
    condition: &Condition,
    target: &Condition,
    replacement: &Condition,
) -> Condition {
    if condition == target {
        return replacement.clone();
    }
    match condition {
        Condition::Not(inner) => {
            Condition::Not(Box::new(replace_subterm(inner, target, replacement)))
        }
        Condition::And(left, right) => Condition::And(
            Box::new(replace_subterm(left, target, replacement)),
            Box::new(replace_subterm(right, target, replacement)),
        ),
        Condition::Or(left, right) => Condition::Or(
            Box::new(replace_subterm(left, target, replacement)),
            Box::new(replace_subterm(right, target, replacement)),
        ),
        Condition::Defined(_) | Condition::Constant(_) => condition.clone(),
    }
}
