use super::{CTypeKind, CTypes, Extent, FloatKind, IntRank, QualType};
use crate::ir::{Access, FloatType, NumericType, Type, VariableExtent};
use crate::target_info::{LongDoubleFormat, TargetInfo};

impl CTypes {
    pub fn ir_type(&self, q: QualType, target: &TargetInfo) -> Type {
        let q = self.desugar(q);
        match self.kind(q.ty) {
            CTypeKind::Void => Type::Void,
            CTypeKind::Bool => Type::Bool,
            CTypeKind::Char
            | CTypeKind::SChar
            | CTypeKind::UChar
            | CTypeKind::Int { .. }
            | CTypeKind::BitInt { .. }
            | CTypeKind::Float(_) => Type::Numeric(self.numeric(q, target)),
            CTypeKind::Complex(component) => {
                Type::Complex(self.numeric(QualType::new(*component), target))
            }
            CTypeKind::Imaginary(kind) => Type::Imaginary(float_type(*kind, target)),
            CTypeKind::Vector { element, lanes, .. } => Type::Vector {
                element: self.numeric(*element, target),
                lanes: *lanes,
            },
            CTypeKind::VaList => Type::VaList,
            CTypeKind::Record { id, .. } | CTypeKind::Enum(id) => Type::Defined(*id),
            CTypeKind::Pointer(pointee) => {
                let quals = self.quals(*pointee);
                Type::Pointer {
                    pointee: Box::new(self.ir_type(*pointee, target)),
                    is_const: quals.is_const,
                    access: self.access(*pointee),
                }
            }
            CTypeKind::Array { element, extent } => {
                let element = Box::new(self.ir_type(*element, target));
                match extent {
                    Extent::Incomplete => Type::Array {
                        element,
                        length: None,
                    },
                    Extent::Fixed(length) => Type::Array {
                        element,
                        length: Some(*length),
                    },
                    Extent::Variable(binding) => Type::VariableArray {
                        element,
                        extent: binding
                            .map_or(VariableExtent::Unspecified, VariableExtent::Captured),
                    },
                }
            }
            CTypeKind::Function {
                ret,
                params,
                variadic,
                prototyped,
            } => Type::Function {
                return_type: (!self.is_void(*ret)).then(|| Box::new(self.ir_type(*ret, target))),
                parameters: params
                    .iter()
                    .map(|param| self.parameter_ir_type(*param, target))
                    .collect(),
                variadic: *variadic,
                prototyped: *prototyped,
            },
            CTypeKind::Typedef { .. }
            | CTypeKind::TypeOf { .. }
            | CTypeKind::AtomicSpecifier(_) => Type::Void,
        }
    }

    pub fn parameter_ir_type(&self, q: QualType, target: &TargetInfo) -> Type {
        let pointee = if let Some((element, _)) = self.element(q) {
            element
        } else if self.is_function(q) {
            q.local_unqualified()
        } else {
            return self.ir_type(q, target);
        };
        Type::Pointer {
            pointee: Box::new(self.ir_type(pointee, target)),
            is_const: self.quals(pointee).is_const,
            access: self.access(pointee),
        }
    }

    pub fn access(&self, q: QualType) -> Access {
        let quals = self.quals(q);
        Access {
            volatile: quals.is_volatile,
            atomic: quals.is_atomic,
        }
    }

    fn numeric(&self, q: QualType, target: &TargetInfo) -> NumericType {
        match self.canonical_kind(q) {
            CTypeKind::Char => NumericType::integer(8, target.char_signed),
            CTypeKind::SChar => NumericType::integer(8, true),
            CTypeKind::UChar => NumericType::integer(8, false),
            CTypeKind::Int { rank, signed } => {
                NumericType::integer(rank_width(*rank, target), *signed)
            }
            CTypeKind::BitInt { width, signed } => NumericType::Integer {
                width: *width,
                signed: *signed,
                bit_precise: true,
            },
            CTypeKind::Float(kind) => NumericType::Float(float_type(*kind, target)),
            _ => NumericType::integer(target.int_width, true),
        }
    }
}

pub fn rank_width(rank: IntRank, target: &TargetInfo) -> u32 {
    match rank {
        IntRank::Short => target.short_width,
        IntRank::Int => target.int_width,
        IntRank::Long => target.long_width,
        IntRank::LongLong => target.long_long_width,
        IntRank::Int128 => 128,
    }
}

pub fn float_type(kind: FloatKind, target: &TargetInfo) -> FloatType {
    match kind {
        FloatKind::Float16 | FloatKind::Fp16 => FloatType::F16,
        FloatKind::Float => FloatType::F32,
        FloatKind::Double => FloatType::F64,
        FloatKind::LongDouble => match target.long_double {
            LongDoubleFormat::Binary64 => FloatType::F64,
            LongDoubleFormat::X87 => FloatType::F80,
            LongDoubleFormat::Binary128 => FloatType::F128,
        },
        FloatKind::Float128 => FloatType::F128,
        FloatKind::Decimal32 => FloatType::D32,
        FloatKind::Decimal64 => FloatType::D64,
        FloatKind::Decimal128 => FloatType::D128,
    }
}
