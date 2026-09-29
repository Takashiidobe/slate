//! Which objects an expression reads and writes, for the unsequenced check in
//! `effects`. See `wiki/concepts/ir/control-flow.md`.

use crate::ir::*;

/// The syntactic lvalue an access names: a binding, or the object one pointer
/// binding points at, plus the member path reached inside it. Distinct members
/// are distinct objects, so `x.a++` and `x.b++` do not conflict even when they
/// share a storage unit; a subscript contributes no step, so any two accesses
/// to one array are treated as the same location.
#[derive(PartialEq)]
enum Root {
    Binding(BindingId),
    Pointee(BindingId),
}

/// One step into an object. A subscript keeps its constant index so that
/// `b[0]` and `b[1]` stay distinct objects; a computed index compares equal
/// only to another computed one, which is what both Clang and GCC report.
#[derive(PartialEq)]
enum Step {
    Field(usize),
    Element(Option<i128>),
}

type Location = (Root, Vec<Step>);

#[derive(Default)]
pub(super) struct Touches {
    reads: Vec<Location>,
    writes: Vec<Location>,
}

fn overlaps(left: &Location, right: &Location) -> bool {
    left.0 == right.0 && left.1.iter().zip(right.1.iter()).all(|(a, b)| a == b)
}

/// What a pointer value points at, seen through the conversions a subscript or
/// `->` inserts: a decayed array is the named object itself, while reading a
/// pointer variable names whatever it points at. Anything computed has no
/// lvalue to compare and is never reported.
fn pointee(value: &Value) -> Option<Root> {
    match &value.node.value {
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => pointee(operand),

        ValueKind::ArrayDecay {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ..
        } => Some(Root::Binding(*id)),
        ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ..
        } => Some(Root::Pointee(*id)),
        _ => None,
    }
}

impl Touches {
    fn modifies_any(&self, other: &[Location]) -> bool {
        self.writes
            .iter()
            .any(|write| other.iter().any(|touch| overlaps(write, touch)))
    }

    fn conflicts_with(&self, other: &Self) -> bool {
        self.modifies_any(&other.writes)
            || self.modifies_any(&other.reads)
            || other.modifies_any(&self.reads)
    }
}

/// Whether two operands of one unsequenced group both touch an object that at
/// least one of them modifies (C11 6.5p2). A write with no named lvalue behind
/// it, such as one inside a call, has nothing to compare and is not reported.
pub(super) fn unsequenced(operands: &[&Value]) -> bool {
    let touches: Vec<Touches> = operands.iter().map(|value| touches(value)).collect();
    touches.iter().enumerate().any(|(i, left)| {
        touches[i + 1..]
            .iter()
            .any(|right| left.conflicts_with(right))
    })
}

/// C11 6.5.16p3 sequences an assignment's update after both operands' value
/// computations, so `i = i + 1` is fine; what stays unsequenced is a second
/// side effect on the target, and the left operand's own subexpressions
/// against the right operand.
pub(super) fn assignment(place: &Place, value: &Value) -> bool {
    let mut target = Touches::default();
    walk_place(place, true, &mut target);
    let operand = touches(value);
    target.modifies_any(&operand.writes) || operand.modifies_any(&target.reads)
}

fn touches(value: &Value) -> Touches {
    let mut found = Touches::default();
    walk(value, &mut found);
    found
}

fn walk(value: &Value, found: &mut Touches) {
    match &value.node.value {
        ValueKind::Store { place, value, .. } => {
            walk_place(place, true, found);
            walk(value, found);
        }
        ValueKind::Update {
            place, computation, ..
        } => {
            walk_place(place, true, found);
            walk(computation, found);
        }
        ValueKind::CompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            walk_place(place, true, found);
            walk(expected, found);
            walk(desired, found);
        }
        ValueKind::Overflow {
            left,
            right,
            result,
            ..
        } => {
            walk(left, found);
            walk(right, found);
            walk_place(result, true, found);
        }
        ValueKind::Call {
            callee, arguments, ..
        } => {
            if let Callee::Indirect(value) = callee {
                walk(value, found);
            }
            arguments.iter().for_each(|argument| walk(argument, found));
        }
        ValueKind::Arith { left, right, .. }
        | ValueKind::Compare { left, right, .. }
        | ValueKind::Logical { left, right, .. }
        | ValueKind::PointerDifference { left, right, .. }
        | ValueKind::Sequence { left, right } => {
            walk(left, found);
            walk(right, found);
        }
        ValueKind::PointerOffset {
            pointer, amount, ..
        } => {
            walk(pointer, found);
            walk(amount, found);
        }
        ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            walk(condition, found);
            walk(then_value, found);
            walk(else_value, found);
        }
        ValueKind::Aggregate { members, .. } => {
            members.iter().for_each(|member| walk(&member.value, found));
        }
        ValueKind::Lane { vector, index } => {
            walk(vector, found);
            walk(index, found);
        }
        ValueKind::Shuffle { left, right, mask } => {
            walk(left, found);
            if let Some(right) = right {
                walk(right, found);
            }
            if let ShuffleMask::Dynamic(mask) = mask {
                walk(mask, found);
            }
        }
        ValueKind::Capture { extent, value, .. } => {
            walk(extent, found);
            walk(value, found);
        }
        ValueKind::Copy { operand, .. }
        | ValueKind::Unary { operand, .. }
        | ValueKind::FloatClass { operand, .. }
        | ValueKind::Convert { operand, .. } => walk(operand, found),
        ValueKind::Read { place, .. }
        | ValueKind::AddressOf(place)
        | ValueKind::ArrayDecay { place, .. }
        | ValueKind::FunctionDecay { place }
        | ValueKind::VaArg { list: place }
        | ValueKind::VaStart { list: place }
        | ValueKind::VaEnd { list: place } => walk_place(place, false, found),
        ValueKind::VaCopy {
            destination,
            source,
        } => {
            walk_place(destination, true, found);
            walk_place(source, false, found);
        }
        ValueKind::StatementExpression(_)
        | ValueKind::Fence { .. }
        | ValueKind::OldValue
        | ValueKind::Constant(_)
        | ValueKind::Null
        | ValueKind::LabelAddress(_)
        | ValueKind::Void
        | ValueKind::CodeUnits(_) => {}
    }
}

fn record(root: Root, write: bool, path: &mut Vec<Step>, found: &mut Touches) {
    path.reverse();
    let location = (root, std::mem::take(path));
    if write {
        found.writes.push(location);
    } else {
        found.reads.push(location);
    }
}

fn walk_place(place: &Place, write: bool, found: &mut Touches) {
    let mut path = Vec::new();
    walk_path(place, write, &mut path, found);
}

/// The constant a subscript adds, if it is one.
fn constant_index(value: &Value) -> Option<i128> {
    match &value.node.value {
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
            constant_index(operand)
        }
        ValueKind::Constant(Number::Integer(value)) => i128::try_from(value.clone()).ok(),
        ValueKind::Constant(Number::SignedInteger(value)) => i128::try_from(value.clone()).ok(),
        _ => None,
    }
}

/// Peels the subscripts a `base[index]` chain lowered to, pushing one step per
/// level and collecting each level's index, and returns the object the
/// innermost pointer names.
fn offset_root<'a>(
    value: &'a Value,
    path: &mut Vec<Step>,
    amounts: &mut Vec<&'a Value>,
) -> Option<Root> {
    match &value.node.value {
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
            offset_root(operand, path, amounts)
        }
        ValueKind::PointerOffset {
            pointer, amount, ..
        } => {
            amounts.push(amount);
            path.push(Step::Element(constant_index(amount)));
            offset_root(pointer, path, amounts)
        }
        _ => pointee(value),
    }
}

fn walk_path(place: &Place, write: bool, path: &mut Vec<Step>, found: &mut Touches) {
    match &place.kind {
        PlaceKind::Binding(id) => record(Root::Binding(*id), write, path, found),
        PlaceKind::Deref(value) => {
            walk(value, found);
            if let Some(root) = offset_root(value, path, &mut Vec::new()) {
                record(root, write, path, found);
            }
        }
        PlaceKind::CompoundLiteral { initializer, .. }
        | PlaceKind::Temporary { initializer, .. } => walk(initializer, found),
        PlaceKind::Field { base, index, .. } => {
            path.push(Step::Field(*index));
            walk_path(base, write, path, found);
        }
        PlaceKind::ComplexPart { base, imaginary } => {
            path.push(Step::Field(usize::from(*imaginary)));
            walk_path(base, write, path, found);
        }
        PlaceKind::Index { base, index } => {
            walk(index, found);
            path.push(Step::Element(constant_index(index)));
            let mut amounts = Vec::new();
            if let Some(root) = offset_root(base, path, &mut amounts) {
                amounts.into_iter().for_each(|amount| walk(amount, found));
                record(root, write, path, found);
            } else {
                walk(base, found);
            }
        }
        PlaceKind::Lane { base, index } => {
            walk_path(base, write, path, found);
            walk(index, found);
        }
        PlaceKind::Swizzle { base, .. } => walk_path(base, write, path, found),
    }
}
