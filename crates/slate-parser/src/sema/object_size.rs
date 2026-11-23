use super::fold;
use super::types::TypeResolver;
use crate::ir::{
    ConversionKind, Place, PlaceKind, RecordKind, Type, TypeDefinitionKind, Value, ValueKind,
};

#[derive(Clone, Copy)]
struct Subobject {
    start: i128,
    size: i128,
    kind: SubobjectKind,
    flexible: bool,
}

#[derive(Clone, Copy)]
enum SubobjectKind {
    Scalar,
    Array,
    Complex,
    ComplexComponent,
}

impl SubobjectKind {
    fn of(ty: &Type) -> Self {
        if matches!(ty, Type::Array { .. }) {
            Self::Array
        } else {
            Self::Scalar
        }
    }
}

#[derive(Clone, Copy)]
struct Designation {
    whole: Option<i128>,
    offset: i128,
    subobject: Option<Subobject>,
    index: i128,
    at_end: bool,
}

impl TypeResolver {
    pub(super) fn folded_object_size(&self, pointer: &Value, kind: u8) -> Option<u64> {
        let designation = self.pointer_designation(pointer)?;
        let end = if kind & 1 == 0 {
            designation.whole?
        } else {
            match designation.subobject {
                Some(sub) if !(kind == 1 && sub.flexible && designation.whole.is_none()) => {
                    sub.start + sub.size
                }
                _ if kind == 1 => designation.whole?,
                _ => return None,
            }
        };
        let remaining = end - designation.offset;
        Some(if designation.offset < 0 || remaining <= 0 {
            0
        } else {
            u64::try_from(remaining).ok()?
        })
    }

    fn pointer_designation(&self, value: &Value) -> Option<Designation> {
        match &value.node.value {
            ValueKind::Copy { operand, .. }
            | ValueKind::Convert {
                kind: ConversionKind::PointerCast,
                operand,
                ..
            } => self.pointer_designation(operand),
            ValueKind::ArrayDecay { place, .. } | ValueKind::AddressOf(place) => {
                self.place_designation(place)
            }
            ValueKind::PointerOffset {
                pointer,
                amount,
                subtract,
                element,
                ..
            } => {
                let mut designation = self.pointer_designation(pointer)?;
                let amount = fold::integer_constant(amount, self.fold_target())?;
                let mut amount = i128::try_from(amount).ok()?;
                if *subtract {
                    amount = -amount;
                }
                let element =
                    i128::from(self.sizeof_storage(element.clone(), false).ok()?.size_bytes);
                if amount != 0
                    && designation
                        .subobject
                        .is_some_and(|sub| matches!(sub.kind, SubobjectKind::ComplexComponent))
                {
                    return None;
                }
                designation.offset += amount * element;
                designation.index += amount;
                designation.subobject = designation.subobject.filter(|sub| match sub.kind {
                    SubobjectKind::Array => {
                        (sub.start..=sub.start + sub.size).contains(&designation.offset)
                    }
                    SubobjectKind::Complex => {
                        (amount == 0 || element == sub.size / 2)
                            && (0..=2).contains(&designation.index)
                    }
                    SubobjectKind::Scalar | SubobjectKind::ComplexComponent => {
                        (amount == 0 || element == sub.size) && (0..=1).contains(&designation.index)
                    }
                });
                Some(designation)
            }
            ValueKind::Read { .. } => Some(Designation {
                whole: None,
                offset: 0,
                subobject: None,
                index: 0,
                at_end: true,
            }),
            _ => None,
        }
    }

    fn place_designation(&self, place: &Place) -> Option<Designation> {
        match &place.kind {
            PlaceKind::Binding(_)
            | PlaceKind::CompoundLiteral { .. }
            | PlaceKind::Temporary { .. } => {
                let size = self.object_bytes(&place.ty)?;
                Some(Designation {
                    whole: Some(size),
                    offset: 0,
                    subobject: Some(Subobject {
                        start: 0,
                        size,
                        kind: SubobjectKind::of(&place.ty),
                        flexible: false,
                    }),
                    index: 0,
                    at_end: false,
                })
            }
            PlaceKind::Deref(pointer) => self.pointer_designation(pointer),
            PlaceKind::Field {
                base,
                index,
                bits: None,
            } => {
                let designation = self.place_designation(base)?;
                let Type::Defined(id) = &base.ty else {
                    return None;
                };
                let TypeDefinitionKind::Record {
                    kind,
                    fields: Some(fields),
                    ..
                } = &self.definitions.get(id.0 as usize)?.kind
                else {
                    return None;
                };
                let last = matches!(kind, RecordKind::Union) || *index + 1 == fields.len();
                let at_end = designation.at_end && last;
                let start = designation.offset + i128::from(self.field_offset(&base.ty, *index)?);
                Some(Designation {
                    whole: designation.whole,
                    offset: start,
                    subobject: Some(Subobject {
                        start,
                        size: self.object_bytes(&place.ty)?,
                        kind: SubobjectKind::of(&place.ty),
                        flexible: at_end && self.flexible_array(&place.ty),
                    }),
                    index: 0,
                    at_end,
                })
            }
            PlaceKind::ComplexPart { base, imaginary } => {
                let mut designation = self.place_designation(base)?;
                let size = self.object_bytes(&place.ty)?;
                let start = designation.offset;
                let index = i128::from(*imaginary);
                designation.offset = start.checked_add(index * size)?;
                if self.flavor().is_gcc() {
                    designation.subobject = Some(Subobject {
                        start: designation.offset,
                        size,
                        kind: SubobjectKind::ComplexComponent,
                        flexible: false,
                    });
                    designation.index = 0;
                } else {
                    let complex_size = 2 * size;
                    let end = start.checked_add(complex_size)?;
                    designation.subobject = designation.subobject.and_then(|base| {
                        let within_base =
                            start >= base.start && end <= base.start.checked_add(base.size)?;
                        (within_base || base.flexible && designation.whole.is_none()).then_some(
                            Subobject {
                                start,
                                size: complex_size,
                                kind: SubobjectKind::Complex,
                                flexible: base.flexible,
                            },
                        )
                    });
                    designation.index = index;
                }
                Some(designation)
            }
            _ => None,
        }
    }

    fn object_bytes(&self, ty: &Type) -> Option<i128> {
        match ty {
            Type::Array { length: None, .. } => Some(0),
            Type::Function { .. } | Type::Void | Type::VariableArray { .. } => None,
            #[expect(
                slate::sensitive_wildcard,
                reason = "types without a static size are listed; storage answers every other type, and an incomplete one yields None"
            )]
            ty => Some(i128::from(self.storage(ty.clone()).ok()?.size_bytes)),
        }
    }

    fn flexible_array(&self, ty: &Type) -> bool {
        let Type::Array { length, .. } = ty else {
            return false;
        };
        match (self.dialect().options().strict_flex_arrays, length) {
            (_, None) | (0, _) => true,
            (1, Some(length)) => *length <= 1,
            (2, Some(length)) => *length == 0,
            _ => false,
        }
    }
}
