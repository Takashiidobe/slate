use super::{NULLABLE, NodeKind, OFFSET, Reason, STACK_GROWTH, STACK_RED_ZONE, Sources, Walker};
use crate::counted_loops::integer_constant;
use crate::heap_ir::read_binding;
use crate::program::ProgramBinding;
use slate_parser::ir::{ConversionKind, Place, PlaceKind, Type, Value, ValueKind};

pub(super) fn part_address(value: &Value) -> bool {
    match &peel_argument(value).node.value {
        ValueKind::AddressOf(place) => matches!(
            place.kind,
            PlaceKind::Field { .. } | PlaceKind::ComplexPart { .. }
        ),
        ValueKind::Conditional {
            then_value,
            else_value,
            ..
        } => part_address(then_value) || part_address(else_value),
        #[expect(
            slate::sensitive_wildcard,
            reason = "only addresses of fields and complex parts are borrows of a part of their object"
        )]
        _ => false,
    }
}

pub(super) fn peel_argument(value: &Value) -> &Value {
    match &value.node.value {
        ValueKind::Copy { operand, .. } => peel_argument(operand),
        ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } if same_pointee(&operand.ty, &value.ty) => peel_argument(operand),
        #[expect(
            slate::sensitive_wildcard,
            reason = "only copies and same-pointee casts keep an argument's identity"
        )]
        _ => value,
    }
}

pub fn fixed_array_position(value: &Value) -> Option<(&Place, u64, u64)> {
    crate::def_use::deep(|| match &peel_argument(value).node.value {
        ValueKind::ArrayDecay {
            place,
            length: Some(extent),
        } if matches!(place.kind, PlaceKind::Binding(_)) => Some((place, *extent, 0)),
        ValueKind::PointerOffset {
            pointer,
            amount,
            subtract,
            ..
        } if same_pointee(&pointer.ty, &value.ty) => {
            let (place, extent, offset) = fixed_array_position(pointer)?;
            let amount = i128::try_from(integer_constant(amount)?).ok()?;
            let offset = if *subtract {
                i128::from(offset).checked_sub(amount)?
            } else {
                i128::from(offset).checked_add(amount)?
            };
            let offset = u64::try_from(offset).ok()?;
            (offset <= extent).then_some((place, extent, offset))
        }
        ValueKind::AddressOf(place) => {
            let PlaceKind::Deref(pointer) = &place.kind else {
                return None;
            };
            fixed_array_position(pointer)
        }
        #[expect(
            slate::sensitive_wildcard,
            reason = "only fixed array decay and constant element offsets define an array position"
        )]
        _ => None,
    })
}

pub(super) fn pointee(ty: &Type) -> Option<&Type> {
    match ty {
        Type::Pointer { pointee, .. } => Some(pointee),
        #[expect(slate::sensitive_wildcard, reason = "only pointers have a pointee")]
        _ => None,
    }
}

pub(super) fn same_pointee(from: &Type, to: &Type) -> bool {
    matches!((from, to), (Type::Pointer { pointee: left, .. }, Type::Pointer { pointee: right, .. }) if left == right)
}

impl Walker<'_, '_> {
    pub(super) fn sources(&self, value: &Value) -> Sources {
        let mut out = Sources::default();
        self.collect_sources(value, false, &mut out);
        out
    }

    fn collect_sources(&self, value: &Value, moved: bool, out: &mut Sources) {
        stacker::maybe_grow(STACK_RED_ZONE, STACK_GROWTH, || {
            self.collect_value_sources(value, moved, out)
        })
    }

    fn collect_value_sources(&self, value: &Value, moved: bool, out: &mut Sources) {
        match &value.node.value {
            ValueKind::Read { place, .. } => self.collect_read_sources(place, moved, out),
            ValueKind::Copy { operand, .. } => self.collect_sources(operand, moved, out),
            ValueKind::Convert { kind, operand, .. } => match kind {
                ConversionKind::PointerCast | ConversionKind::AddressSpaceCast => {
                    out.retyped |= !same_pointee(&operand.ty, &value.ty);
                    self.collect_sources(operand, moved, out)
                }
                _ => out.opaque = true,
            },
            ValueKind::PointerOffset { pointer, .. } => self.collect_sources(pointer, true, out),
            ValueKind::Conditional {
                then_value,
                else_value,
                ..
            } => {
                self.collect_sources(then_value, moved, out);
                self.collect_sources(else_value, moved, out);
            }
            ValueKind::Sequence { right, .. } => self.collect_sources(right, moved, out),
            ValueKind::Store { value, .. } => self.collect_sources(value, moved, out),
            ValueKind::StatementExpression(evaluation) => {
                self.collect_sources(&evaluation.value, moved, out)
            }
            ValueKind::AddressOf(place) | ValueKind::ArrayDecay { place, .. } => {
                self.collect_address_sources(place, moved, out)
            }
            ValueKind::Null | ValueKind::Constant(_) => out.null = true,
            ValueKind::Call { .. } => match self.result_nodes.get(&value.node.id) {
                Some(&node) => out.roots.push((node, moved)),
                None => out.opaque = true,
            },
            ValueKind::LayoutQuery { .. }
            | ValueKind::LabelAddress(_)
            | ValueKind::Seh(_)
            | ValueKind::Void
            | ValueKind::CodeUnits(_)
            | ValueKind::Aggregate { .. }
            | ValueKind::FunctionDecay { .. }
            | ValueKind::Update { .. }
            | ValueKind::CompareExchange { .. }
            | ValueKind::Fence { .. }
            | ValueKind::OldValue
            | ValueKind::PointerDifference { .. }
            | ValueKind::VaArg { .. }
            | ValueKind::Block(_)
            | ValueKind::Capture { .. }
            | ValueKind::VaStart { .. }
            | ValueKind::VaEnd { .. }
            | ValueKind::VaCopy { .. }
            | ValueKind::Arith { .. }
            | ValueKind::Overflow { .. }
            | ValueKind::Unary { .. }
            | ValueKind::Compare { .. }
            | ValueKind::Logical { .. }
            | ValueKind::FloatClass { .. }
            | ValueKind::Lane { .. }
            | ValueKind::Shuffle { .. }
            | ValueKind::Intrinsic { .. } => out.opaque = true,
        }
    }

    fn collect_read_sources(&self, place: &Place, moved: bool, out: &mut Sources) {
        match &place.kind {
            PlaceKind::Binding(binding) => match self.node(*binding) {
                Some(node) => out.roots.push((node, moved)),
                None => out.opaque = true,
            },
            PlaceKind::Deref(_)
            | PlaceKind::CompoundLiteral { .. }
            | PlaceKind::Temporary { .. }
            | PlaceKind::ComplexPart { .. }
            | PlaceKind::Field { .. }
            | PlaceKind::Index { .. }
            | PlaceKind::Lane { .. }
            | PlaceKind::Swizzle { .. } => out.opaque = true,
        }
    }

    pub(super) fn collect_address_sources(&self, place: &Place, moved: bool, out: &mut Sources) {
        match &place.kind {
            PlaceKind::Deref(pointer) => self.collect_sources(pointer, moved, out),
            PlaceKind::Index { base, .. } => self.collect_sources(base, true, out),
            PlaceKind::ComplexPart { base, .. }
            | PlaceKind::Field { base, .. }
            | PlaceKind::Lane { base, .. }
            | PlaceKind::Swizzle { base, .. } => self.collect_address_sources(base, true, out),
            PlaceKind::Binding(binding) => {
                out.objects.push(*binding);
                out.opaque = true;
            }
            PlaceKind::CompoundLiteral { .. } | PlaceKind::Temporary { .. } => out.opaque = true,
        }
    }

    pub(super) fn assign(&mut self, destination: u32, value: &Value) {
        let sources = self.sources(value);
        for (source, moved) in sources.roots {
            if source != destination {
                self.unions.push((destination, source));
                self.copies.push((source, destination));
            }
            if moved {
                self.make_raw(destination, Reason::Moved);
            } else if self.nodes[source as usize].kind == NodeKind::Return
                || (self.nodes[source as usize].kind == NodeKind::Temporary
                    && !self.call_results.contains(&source))
            {
                self.make_raw(destination, Reason::TemporarySource);
            }
        }
        if sources.opaque {
            self.make_raw(destination, Reason::OpaqueValue);
        } else if sources.retyped {
            self.make_raw(destination, Reason::Retyped);
        }
        if sources.null {
            self.flag(destination, NULLABLE);
        }
        for object in sources.objects {
            if self.mutable_statics.contains(&ProgramBinding {
                unit: self.unit,
                binding: object,
            }) {
                self.derived.push((destination, object));
            } else {
                self.leaked.insert(object);
            }
        }
    }

    pub(super) fn element_of(&self, value: &Value) -> Option<u32> {
        let mut value = value;
        while let ValueKind::Copy { operand, .. }
        | ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } = &value.node.value
            && same_pointee(&operand.ty, &value.ty)
        {
            value = operand;
        }
        let pointer = match &value.node.value {
            ValueKind::AddressOf(Place {
                kind: PlaceKind::Deref(pointer),
                ..
            }) => match &pointer.node.value {
                ValueKind::PointerOffset {
                    pointer,
                    subtract: false,
                    ..
                } => pointer,
                #[expect(
                    slate::sensitive_wildcard,
                    reason = "only an offset element of a binding is a derived borrow"
                )]
                _ => return None,
            },
            ValueKind::AddressOf(Place {
                kind: PlaceKind::Index { base, .. },
                ..
            }) => base,
            ValueKind::PointerOffset {
                pointer,
                subtract: false,
                ..
            } => pointer,
            #[expect(
                slate::sensitive_wildcard,
                reason = "only an offset element of a binding is a derived borrow"
            )]
            _ => return None,
        };
        self.node(read_binding(pointer)?)
    }

    pub(super) fn rebase(&mut self, place: &Place) {
        match &place.kind {
            PlaceKind::Binding(binding) => {
                if let Some(node) = self.node(*binding) {
                    self.make_raw(node, Reason::Rebased);
                    self.flag(node, OFFSET);
                }
            }
            PlaceKind::Deref(_)
            | PlaceKind::CompoundLiteral { .. }
            | PlaceKind::Temporary { .. }
            | PlaceKind::ComplexPart { .. }
            | PlaceKind::Field { .. }
            | PlaceKind::Index { .. }
            | PlaceKind::Lane { .. }
            | PlaceKind::Swizzle { .. } => {}
        }
    }
}
