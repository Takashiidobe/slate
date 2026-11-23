use crate::counted_loops::plain_access;
use slate_parser::ast::Span;
use slate_parser::ir::{
    self, BindingId, Callee, ConversionKind, Place, PlaceKind, PointerSpace, Statement, Type,
    Value, ValueKind, Visitor,
};

#[derive(Clone, Copy, Default, PartialEq, Eq)]
pub(crate) enum Ending {
    #[default]
    Continues,
    Returns,
    ClosesScope,
}

impl Ending {
    pub(crate) fn elides_release(self, in_declaring_scope: bool) -> bool {
        match self {
            Self::Returns => true,
            Self::ClosesScope => in_declaring_scope,
            Self::Continues => false,
        }
    }
}

pub(crate) fn peel_conversions(value: &Value) -> &Value {
    match &value.node.value {
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
            peel_conversions(operand)
        }
        #[expect(
            slate::sensitive_wildcard,
            reason = "every other value ends the conversion chain"
        )]
        _ => value,
    }
}

pub(crate) fn read_binding(value: &Value) -> Option<BindingId> {
    match &value.node.value {
        ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(binding),
                    ..
                },
            ordering: None,
        } => Some(*binding),
        #[expect(
            slate::sensitive_wildcard,
            reason = "only a plain read names a binding's current value"
        )]
        _ => None,
    }
}

pub(crate) fn code_statements(
    body: &[Span<Statement>],
) -> impl DoubleEndedIterator<Item = &Span<Statement>> + Clone {
    body.iter()
        .filter(|statement| !matches!(statement.value, Statement::Comment(_) | Statement::Null))
}

pub(crate) fn binding_write(statement: &Statement) -> Option<(BindingId, &Value)> {
    let Statement::Write {
        place,
        value,
        ordering: None,
        ..
    } = statement
    else {
        return None;
    };
    match place.kind {
        PlaceKind::Binding(binding) if plain_access(&place.access) => Some((binding, value)),
        _ => None,
    }
}

pub(crate) fn plain_read(value: &Value) -> Option<BindingId> {
    match &value.node.value {
        ValueKind::Read {
            place,
            ordering: None,
        } if plain_access(&place.access) => read_binding(value),
        _ => None,
    }
}

pub(crate) fn is_null(value: &Value) -> bool {
    matches!(peel_pointer(value).node.value, ValueKind::Null)
}

pub(crate) fn is_null_pointer(value: &Value) -> bool {
    matches!(value.ty, Type::Pointer { .. }) && is_null(value)
}

pub(crate) fn pointer_null_test(value: &Value) -> Option<(BindingId, bool)> {
    let ValueKind::Compare {
        op: op @ (ir::CompareOp::Eq | ir::CompareOp::Ne),
        left,
        right,
        ..
    } = &value.node.value
    else {
        return None;
    };
    let pointer = if is_null_pointer(right) {
        left
    } else if is_null_pointer(left) {
        right
    } else {
        return None;
    };
    Some((plain_read(pointer)?, *op == ir::CompareOp::Eq))
}

pub(crate) fn released_binding(
    statement: &Statement,
    is_free: impl FnOnce(&Callee) -> bool,
) -> Option<BindingId> {
    let Statement::Expression(value) = statement else {
        return None;
    };
    let ValueKind::Call {
        callee, arguments, ..
    } = &value.node.value
    else {
        return None;
    };
    let [pointer] = arguments.as_slice() else {
        return None;
    };
    is_free(callee)
        .then(|| read_binding(peel_conversions(pointer)))
        .flatten()
}

pub(crate) fn release_endings(
    body: &[Span<Statement>],
    released: impl Fn(&Span<Statement>) -> Option<BindingId>,
) -> Vec<Ending> {
    let mut endings = vec![Ending::Continues; body.len()];
    let mut rest: Option<Option<&Value>> = Some(None);
    let mut returns = false;
    for (index, statement) in body.iter().enumerate().rev() {
        match &statement.value {
            Statement::Comment(_) | Statement::Null => continue,
            Statement::Return(value) => {
                rest = Some(value.as_ref());
                returns = true;
                continue;
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only frees, comments and a return keep a following free at the end of its run"
            )]
            _ => {}
        }
        match (released(statement), rest) {
            (Some(binding), Some(value)) => {
                endings[index] = if !returns {
                    Ending::ClosesScope
                } else if value.is_none_or(|value| !touches(value, binding)) {
                    Ending::Returns
                } else {
                    Ending::Continues
                };
            }
            (Some(_) | None, _) => rest = None,
        }
    }
    endings
}

struct Touches {
    object: BindingId,
    hit: bool,
}

impl Visitor for Touches {
    fn binding(&mut self, binding: BindingId) {
        self.hit |= binding == self.object;
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        self.hit |= matches!(kind, ValueKind::Call { .. });
    }
}

pub(crate) fn touches(value: &Value, object: BindingId) -> bool {
    let mut touches = Touches { object, hit: false };
    ir::walk_value(&mut value.clone(), &mut touches);
    touches.hit
}

pub(crate) fn peel_pointer(mut value: &Value) -> &Value {
    while let ValueKind::Copy { operand, .. }
    | ValueKind::Convert {
        kind: ConversionKind::PointerCast,
        operand,
        ..
    } = &value.node.value
        && matches!(
            (&value.ty, &operand.ty),
            (
                Type::Pointer {
                    space: PointerSpace::Default,
                    ..
                },
                Type::Pointer {
                    space: PointerSpace::Default,
                    ..
                }
            )
        )
    {
        value = operand;
    }
    value
}
