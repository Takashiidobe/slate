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
