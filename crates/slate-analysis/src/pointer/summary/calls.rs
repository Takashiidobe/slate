use super::super::{FREE, OFFSET, STRING, WRITE};
use super::heap::Release;
use super::sources::{part_address, peel_argument, pointee, same_pointee};
use super::{CallSite, ESCAPE, NodeKind, Shape, Walker, tracked_pointer};
use crate::heap_ir::{peel_conversions, read_binding, touches};
use crate::identity::{FunctionIdentity, Known};
use crate::program::ProgramBinding;
use slate_parser::ir::{
    self, BindingId, Callee, ConversionKind, Place, PlaceKind, Type, Value, ValueKind,
};

impl Walker<'_, '_> {
    fn shape(&self, value: &Value) -> Shape {
        let peeled = peel_argument(value);
        match &peeled.node.value {
            ValueKind::ArrayDecay {
                place:
                    Place {
                        kind: PlaceKind::Binding(array),
                        ..
                    },
                length: Some(length),
            } => {
                let object = ProgramBinding {
                    unit: self.unit,
                    binding: *array,
                };
                let object = if self.automatic.contains(array) || self.constants.contains(&object) {
                    None
                } else if self.mutable_statics.contains(&object) {
                    Some(*array)
                } else {
                    return Shape::Other;
                };
                match pointee(&peeled.ty) {
                    Some(element) => Shape::Array {
                        length: *length,
                        element: element.clone(),
                        object,
                    },
                    None => Shape::Other,
                }
            }
            ValueKind::Constant(ir::Number::Integer(number)) => {
                u64::try_from(number).map_or(Shape::Other, Shape::Constant)
            }
            ValueKind::Constant(ir::Number::SignedInteger(number)) => {
                u64::try_from(number).map_or(Shape::Other, Shape::Constant)
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "arrays, constants and binding reads are the only provable length sources"
            )]
            _ => match read_binding(peeled) {
                Some(binding) => match self.allocations.get(&binding) {
                    Some((count, element)) => Shape::Allocation {
                        pointer: binding,
                        count: *count,
                        element: element.clone(),
                    },
                    None => Shape::Binding(binding),
                },
                None => Shape::Other,
            },
        }
    }

    fn literal(&self, value: &Value) -> bool {
        matches!(
            &peel_argument(value).node.value,
            ValueKind::ArrayDecay {
                place: Place {
                    kind: PlaceKind::Binding(string),
                    ..
                },
                length: Some(_),
            } if self.strings.contains(&ProgramBinding {
                unit: self.unit,
                binding: *string,
            })
        )
    }

    pub(super) fn known(&self, callee: &Callee) -> Option<Known> {
        let Callee::Direct(binding) = callee else {
            return None;
        };
        match self.identities.get(&ProgramBinding {
            unit: self.unit,
            binding: *binding,
        }) {
            Some(FunctionIdentity::Known(known)) => Some(*known),
            Some(FunctionIdentity::HeaderInline | FunctionIdentity::Unknown) | None => None,
        }
    }

    pub(super) fn allocator<'v>(&self, value: &'v Value) -> Option<(Known, &'v [Value])> {
        let mut call = value;
        while let ValueKind::Copy { operand, .. }
        | ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } = &call.node.value
        {
            call = operand;
        }
        let ValueKind::Call {
            callee, arguments, ..
        } = &call.node.value
        else {
            return None;
        };
        let known = self.known(callee)?;
        matches!(known, Known::Malloc | Known::Calloc | Known::Realloc)
            .then_some((known, arguments.as_slice()))
    }

    pub(super) fn fresh(&self, value: &Value) -> bool {
        let module = &self.program.unit(self.unit).module;
        module.hosted
            && !module.nostdinc
            && matches!(
                self.allocator(value),
                Some((Known::Malloc, [_]) | (Known::Calloc, [_, _]))
            )
    }

    pub(super) fn reallocates(&self, binding: BindingId, value: &Value) -> bool {
        matches!(
            self.allocator(value),
            Some((Known::Realloc, [pointer, _])) if read_binding(peel_conversions(pointer)) == Some(binding)
        )
    }

    pub(super) fn allocation(&self, ty: &Type, initializer: &Value) -> Option<(BindingId, Type)> {
        let element = pointee(ty)?;
        let mut call = initializer;
        while let ValueKind::Copy { operand, .. }
        | ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } = &call.node.value
        {
            call = operand;
        }
        let ValueKind::Call {
            callee: Callee::Direct(function),
            arguments,
            ..
        } = &call.node.value
        else {
            return None;
        };
        if !matches!(
            self.identities.get(&ProgramBinding {
                unit: self.unit,
                binding: *function,
            }),
            Some(FunctionIdentity::Known(Known::Malloc))
        ) {
            return None;
        }
        let [size] = arguments.as_slice() else {
            return None;
        };
        let ValueKind::Arith {
            op: ir::ArithOp::Mul,
            left,
            right,
            ..
        } = &peel_conversions(size).node.value
        else {
            return None;
        };
        let size_of = |value: &Value| {
            matches!(
                &peel_conversions(value).node.value,
                ValueKind::LayoutQuery {
                    query: ir::LayoutQuery::SizeOf { ty, .. },
                    ..
                } if ty == element
            )
        };
        let count = if size_of(right) {
            left
        } else if size_of(left) {
            right
        } else {
            return None;
        };
        Some((read_binding(peel_conversions(count))?, element.clone()))
    }

    pub(super) fn proven(&self, value: &Value) -> bool {
        if null_like(value) {
            return true;
        }
        match &value.node.value {
            ValueKind::Read {
                place:
                    Place {
                        kind: PlaceKind::Binding(binding),
                        ..
                    },
                ordering: None,
            } => self.node(*binding).is_some(),
            ValueKind::Copy { operand, .. } => self.proven(operand),
            ValueKind::Convert {
                kind: ConversionKind::PointerCast,
                operand,
                ..
            } => same_pointee(&operand.ty, &value.ty) && self.proven(operand),
            ValueKind::Conditional {
                then_value,
                else_value,
                ..
            } => self.proven(then_value) && self.proven(else_value),
            ValueKind::AddressOf(
                place @ Place {
                    kind: PlaceKind::ComplexPart { .. } | PlaceKind::Field { .. },
                    ..
                },
            ) => self.aligned_local_part(place),
            ValueKind::AddressOf(Place {
                kind: PlaceKind::Binding(binding),
                ..
            })
            | ValueKind::ArrayDecay {
                place:
                    Place {
                        kind: PlaceKind::Binding(binding),
                        ..
                    },
                length: Some(_),
            } => self.automatic.contains(binding),
            #[expect(
                slate::sensitive_wildcard,
                reason = "only reads of tracked bindings and addresses of automatic objects lower to a borrow"
            )]
            _ => false,
        }
    }

    pub(super) fn call(&mut self, call: &Value, callee: &Callee, arguments: &[Value]) {
        if let Callee::Indirect(function) = callee {
            self.value(function);
        }
        for argument in arguments {
            self.value(argument);
        }
        if let Some(target) = self.program.callee(self.unit, callee) {
            if let Some(node) = self.anonymous(BindingId(u32::MAX), &call.ty, NodeKind::Temporary) {
                self.result_nodes.insert(call.node.id, node);
                self.results.push((node, target));
                self.call_results.insert(node);
            }
            let mut site = CallSite {
                callee: target,
                arguments: Vec::new(),
                objects: Vec::new(),
                unproven: Vec::new(),
                nulls: Vec::new(),
                parts: Vec::new(),
                pinned: self.macro_depth > 0,
                shapes: arguments
                    .iter()
                    .enumerate()
                    .map(|(index, argument)| match self.shape(argument) {
                        Shape::Array {
                            object: Some(object),
                            ..
                        } if arguments.iter().enumerate().any(|(other, argument)| {
                            other != index && touches(argument, object)
                        }) =>
                        {
                            Shape::Other
                        }
                        shape => shape,
                    })
                    .collect(),
                literals: arguments
                    .iter()
                    .map(|argument| self.literal(argument))
                    .collect(),
            };
            for (index, argument) in arguments.iter().enumerate() {
                if tracked_pointer(&argument.ty).is_some() {
                    if !self.proven(argument) {
                        site.unproven.push(index);
                    } else if part_address(argument) {
                        site.parts.push(index);
                    }
                }
                let sources = self.sources(argument);
                if sources.null && tracked_pointer(&argument.ty).is_some() {
                    site.nulls.push(index);
                }
                for (node, _) in sources.roots {
                    site.arguments.push((index, node));
                }
                for object in sources.objects {
                    site.objects.push((index, object));
                }
            }
            self.calls.push(site);
            return;
        }
        if arguments.iter().any(|argument| {
            matches!(&argument.ty, Type::Pointer { pointee, .. } if matches!(**pointee, Type::Function { .. }))
        }) {
            self.opaque = true;
        }
        if let Callee::Direct(binding) = callee
            && self.inert.contains(&ProgramBinding {
                unit: self.unit,
                binding: *binding,
            })
        {
            return;
        }
        let known = self.known(callee);
        if known.is_none() {
            self.opaque = true;
        }
        if known == Some(Known::Free)
            && let [pointer] = arguments
            && let Some(binding) = read_binding(peel_conversions(pointer))
            && let Some(&position) = self.ownership.last_read.get(&binding)
        {
            self.ownership.releases.push(Release {
                binding,
                call: call.node.id,
                position,
                depth: self.ownership.depth,
                scope: self.ownership.scope,
                ending: self.ownership.ending,
            });
        }
        for (index, argument) in arguments.iter().enumerate() {
            let flags = known.map_or(ESCAPE, |known| known_argument(known, index));
            if flags != 0 {
                self.mark(argument, flags);
            }
        }
    }
}
fn null_like(value: &Value) -> bool {
    match &value.node.value {
        ValueKind::Null => true,
        ValueKind::Constant(_) => matches!(value.ty, Type::Pointer { .. }),
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => null_like(operand),
        #[expect(
            slate::sensitive_wildcard,
            reason = "only null and pointer-typed constants, through copies and conversions, are null pointer constants"
        )]
        _ => false,
    }
}

fn known_argument(known: Known, index: usize) -> u8 {
    match known {
        Known::Free => FREE,
        Known::Realloc if index == 0 => FREE,
        Known::MemCpy | Known::MemMove | Known::MemSet => {
            if index == 0 {
                WRITE
            } else {
                0
            }
        }
        Known::StrCpy | Known::StrCat | Known::StrNCpy | Known::StrNCat => {
            if index == 0 {
                STRING | WRITE
            } else {
                STRING
            }
        }
        Known::StrLen
        | Known::StrCmp
        | Known::StrNCmp
        | Known::StrNLen
        | Known::StrChr
        | Known::StrRChr
        | Known::StrStr
        | Known::StrPBrk
        | Known::StrSpn
        | Known::StrCSpn => STRING | OFFSET,
        Known::MemChr | Known::MemCmp => 0,
        _ => ESCAPE,
    }
}
