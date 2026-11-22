use crate::def_use::{DefUse, deep};
use foldhash::{HashMap, HashMapExt, HashSet, HashSetExt};
use num_bigint::BigInt;
use slate_parser::ast::Span;
use slate_parser::ir::*;

#[derive(Debug, Clone)]
pub struct CountedLoop {
    pub index: BindingId,
    pub start: Value,
    pub end: Value,
    pub inclusive: bool,
}

struct Scope {
    plain: HashSet<BindingId>,
    def_use: DefUse,
}

pub(crate) fn analyze(function: &Function, loops: &mut HashMap<BindingId, CountedLoop>) {
    let Some(body) = &function.body else {
        return;
    };
    let def_use = DefUse::of(body);
    let mut plain = HashSet::new();
    if let Parameters::Prototype { fixed, .. } = &function.parameters {
        plain.extend(
            fixed
                .iter()
                .filter(|parameter| plain_access(&parameter.value.access))
                .map(|parameter| parameter.value.id),
        );
    }
    collect_locals(body, &mut plain);
    plain.retain(|id| !def_use.escapes(*id));
    mark(body, &Scope { plain, def_use }, loops);
}

fn plain_access(access: &Access) -> bool {
    !access.volatile && !access.atomic
}

fn collect_locals(body: &[Span<Statement>], plain: &mut HashSet<BindingId>) {
    for_each_let(body, &mut |variable| {
        if plain_local(variable) {
            plain.insert(variable.id);
        }
    });
}

pub(crate) fn plain_local(variable: &Variable) -> bool {
    variable.storage == StorageDuration::Automatic
        && plain_access(&variable.access)
        && variable.register.is_none()
        && variable.cleanup.is_none()
}

pub(crate) fn for_each_let<'a>(body: &'a [Span<Statement>], visit: &mut impl FnMut(&'a Variable)) {
    for statement in body {
        deep(|| match &statement.value {
            Statement::Let(variable) => visit(variable),
            Statement::MacroExpansion { body, .. }
            | Statement::Block(body)
            | Statement::LabeledBlock { body, .. }
            | Statement::While { body, .. }
            | Statement::DoWhile { body, .. }
            | Statement::Switch { body, .. }
            | Statement::Case { body, .. }
            | Statement::Default { body, .. }
            | Statement::Label { body, .. } => for_each_let(body, visit),
            Statement::If {
                then_body,
                else_body,
                ..
            } => {
                for_each_let(then_body, visit);
                if let Some(body) = else_body {
                    for_each_let(body, visit);
                }
            }
            Statement::For { init, body, .. } => {
                for_each_let(init, visit);
                for_each_let(body, visit);
            }
            _ => {}
        });
    }
}

fn mark(body: &[Span<Statement>], scope: &Scope, loops: &mut HashMap<BindingId, CountedLoop>) {
    for statement in body {
        deep(|| match &statement.value {
            Statement::MacroExpansion { body, .. }
            | Statement::Block(body)
            | Statement::LabeledBlock { body, .. }
            | Statement::While { body, .. }
            | Statement::DoWhile { body, .. }
            | Statement::Switch { body, .. }
            | Statement::Case { body, .. }
            | Statement::Default { body, .. }
            | Statement::Label { body, .. } => mark(body, scope, loops),
            Statement::If {
                then_body,
                else_body,
                ..
            } => {
                mark(then_body, scope, loops);
                if let Some(body) = else_body {
                    mark(body, scope, loops);
                }
            }
            Statement::For {
                id,
                init,
                condition,
                increment,
                body,
            } => {
                mark(init, scope, loops);
                mark(body, scope, loops);
                if let Some(counted) =
                    recognize(*id, init, condition.as_ref(), increment.as_ref(), scope)
                {
                    loops.insert(*id, counted);
                }
            }
            _ => {}
        });
    }
}

fn recognize(
    id: BindingId,
    init: &[Span<Statement>],
    condition: Option<&Evaluation>,
    increment: Option<&Evaluation>,
    scope: &Scope,
) -> Option<CountedLoop> {
    let Statement::Let(index) = &init.last()?.value else {
        return None;
    };
    let Type::Numeric(NumericType::Integer { .. }) = index.ty else {
        return None;
    };
    if !scope.plain.contains(&index.id) {
        return None;
    }
    let start = index.initializer.clone()?;
    let condition = condition?;
    if !condition.statements.is_empty() {
        return None;
    }
    let ValueKind::Compare {
        op, left, right, ..
    } = &condition.value.node.value
    else {
        return None;
    };
    let (end, inclusive) = match op {
        CompareOp::Lt if reads(left, index.id) => (right, false),
        CompareOp::Gt if reads(right, index.id) => (left, false),
        CompareOp::Le if reads(left, index.id) => (right, true),
        CompareOp::Ge if reads(right, index.id) => (left, true),
        _ => return None,
    };
    if end.ty != index.ty {
        return None;
    }
    if inclusive && !below_max(end, &index.ty) {
        return None;
    }
    let mut end_reads = HashSet::new();
    if !invariant(end, &mut end_reads)
        || end_reads.contains(&index.id)
        || !end_reads.iter().all(|id| scope.plain.contains(id))
    {
        return None;
    }
    if !unit_increment(increment?, index.id) {
        return None;
    }
    let body = scope.def_use.body(id)?;
    if !scope.def_use.structured(&body)
        || scope.def_use.touched_in(index.id, &body)
        || end_reads
            .iter()
            .any(|id| scope.def_use.touched_in(*id, &body))
    {
        return None;
    }
    Some(CountedLoop {
        index: index.id,
        start,
        end: (**end).clone(),
        inclusive,
    })
}

fn reads(value: &Value, id: BindingId) -> bool {
    matches!(
        &value.node.value,
        ValueKind::Read { place, ordering: None }
            if matches!(place.kind, PlaceKind::Binding(read) if read == id)
    )
}

fn invariant(value: &Value, reads: &mut HashSet<BindingId>) -> bool {
    match &value.node.value {
        ValueKind::Constant(_) | ValueKind::LayoutQuery { .. } => true,
        ValueKind::Copy { operand, .. }
        | ValueKind::Convert { operand, .. }
        | ValueKind::Unary { operand, .. } => invariant(operand, reads),
        ValueKind::Arith { left, right, .. } => invariant(left, reads) && invariant(right, reads),
        ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    access,
                    ..
                },
            ordering: None,
        } if plain_access(access) => {
            reads.insert(*id);
            true
        }
        #[expect(
            slate::sensitive_wildcard,
            reason = "invariant only accepts pure arithmetic over constants and plain local reads; any other kind may change across iterations"
        )]
        _ => false,
    }
}

fn unit_increment(increment: &Evaluation, index: BindingId) -> bool {
    if !matches!(increment.value.node.value, ValueKind::Void) {
        return false;
    }
    let mut temporaries: HashMap<BindingId, &Value> = HashMap::new();
    let mut written = None;
    for statement in &increment.statements {
        match &statement.value {
            Statement::Temporary {
                id,
                initializer: Some(value),
                ..
            } => {
                temporaries.insert(*id, resolve(value, &temporaries));
            }
            Statement::Write {
                place:
                    Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                value,
                ordering: None,
                ..
            } if *id == index && written.is_none() => {
                written = Some(resolve(value, &temporaries));
            }
            _ => return false,
        }
    }
    let Some(ValueKind::Arith {
        op: ArithOp::Add,
        left,
        right,
        ..
    }) = written.map(|value| &value.node.value)
    else {
        return false;
    };
    reads(resolve(left, &temporaries), index)
        && integer_constant(right).is_some_and(|one| one == BigInt::from(1))
}

fn resolve<'a>(value: &'a Value, temporaries: &HashMap<BindingId, &'a Value>) -> &'a Value {
    match &value.node.value {
        ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ..
        } => temporaries.get(id).copied().unwrap_or(value),
        _ => value,
    }
}

fn integer_range(ty: &Type) -> Option<(BigInt, BigInt)> {
    let Type::Numeric(NumericType::Integer { width, signed, .. }) = ty else {
        return None;
    };
    Some(if *signed {
        let half = BigInt::from(1) << (width - 1);
        (-half.clone(), half - 1)
    } else {
        (BigInt::from(0), (BigInt::from(1) << width) - 1)
    })
}

fn integer_constant(value: &Value) -> Option<BigInt> {
    let constant = match &value.node.value {
        ValueKind::Constant(Number::Integer(value)) => BigInt::from(value.clone()),
        ValueKind::Constant(Number::SignedInteger(value)) => value.clone(),
        ValueKind::LayoutQuery { amount, .. } => BigInt::from(*amount),
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
            integer_constant(operand)?
        }
        #[expect(
            slate::sensitive_wildcard,
            reason = "integer constants and their conversions are listed; anything else is not a literal bound"
        )]
        _ => return None,
    };
    let (min, max) = integer_range(&value.ty)?;
    (min <= constant && constant <= max).then_some(constant)
}

fn below_max(end: &Value, ty: &Type) -> bool {
    match (slate_parser::sema::fold_integer(end), integer_range(ty)) {
        (Some(end), Some((_, max))) => end < max,
        _ => false,
    }
}
