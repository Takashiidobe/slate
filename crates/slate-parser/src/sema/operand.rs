use super::ctype::QualType;
use super::ctype::{CTypeKind, FloatKind, IntRank};
use super::numeric::{Context, ResolveError};
use super::types::TypeResolver;
use crate::ast::{Expr, ExprKind, IntegerRank};
use crate::const_expr::{BinaryOp, FloatSuffix, UnaryOp};
use crate::ir::{ConversionReason, ValueKind};
use crate::ir::{Place, Value};

#[derive(Clone, Debug)]
pub(super) struct Operand {
    pub value: Value,
    pub c: QualType,
}

#[derive(Clone, Debug)]
pub(super) struct Lvalue {
    pub place: Place,
    pub c: QualType,
}

impl std::ops::Deref for Operand {
    type Target = Value;

    fn deref(&self) -> &Value {
        &self.value
    }
}

impl std::ops::Deref for Lvalue {
    type Target = Place;

    fn deref(&self) -> &Place {
        &self.place
    }
}

impl TypeResolver {
    fn encoded_character_type(
        &mut self,
        encoding: crate::const_expr::Encoding,
        target: &crate::target_info::TargetInfo,
    ) -> QualType {
        use crate::const_expr::Encoding;
        let kind = match encoding {
            Encoding::Plain => CTypeKind::Char,
            Encoding::Utf8 if !self.features.u8_literals_are_unsigned => CTypeKind::Char,
            Encoding::Utf8 => CTypeKind::UChar,
            Encoding::Utf16 => CTypeKind::Int {
                rank: IntRank::Short,
                signed: false,
            },
            Encoding::Utf32 => CTypeKind::Int {
                rank: IntRank::Int,
                signed: false,
            },
            Encoding::Wide => CTypeKind::Int {
                rank: if target.wchar_width == target.short_width {
                    IntRank::Short
                } else {
                    IntRank::Int
                },
                signed: target.wchar_signed,
            },
        };
        self.ctypes.qual(kind)
    }

    pub(super) fn string_type(&mut self, literal: &crate::const_expr::StringLiteral) -> QualType {
        let target = self.target_info().clone();
        let element = self.encoded_character_type(literal.encoding, &target);
        self.ctypes.qual(CTypeKind::Array {
            element,
            extent: super::ctype::Extent::Fixed(
                literal.execution_units(target.wchar_width).len() as u64 + 1,
            ),
        })
    }

    pub(super) fn character_constant(
        &mut self,
        literal: &crate::const_expr::CharLiteral,
    ) -> Result<(QualType, crate::ir::Number), ResolveError> {
        let target = self.target_info().clone();
        let value = literal.value(&target)?;
        let number = match u64::try_from(value) {
            Ok(value) => crate::ir::Number::Integer(value.into()),
            Err(_) => crate::ir::Number::SignedInteger(value.into()),
        };
        let c = if literal.encoding == crate::const_expr::Encoding::Plain {
            self.ctypes.int()
        } else {
            self.encoded_character_type(literal.encoding, &target)
        };
        Ok((c, number))
    }

    pub(super) fn operand(&self, e: &Expr, c: QualType, kind: ValueKind) -> Operand {
        Operand {
            value: Value {
                ty: self.ir_type(c),
                node: e.derive(kind),
            },
            c,
        }
    }

    pub(super) fn literal(&mut self, context: &Context, e: &Expr) -> Result<Operand, ResolveError> {
        let kind = match &e.value {
            ExprKind::IntegerLiteral(literal) => {
                let component = if literal.suffix.size
                    == crate::const_expr::IntegerSizeSuffix::BitInt
                {
                    let width = super::validate::bit_int_literal_width(literal)
                        .ok_or_else(|| ResolveError::IntegerLiteral(literal.spelling.clone()))?;
                    CTypeKind::BitInt {
                        width,
                        signed: !literal.suffix.unsigned,
                    }
                } else {
                    let (rank, signed) = super::validate::select_integer_candidate(
                        literal,
                        &context.target,
                        context.features,
                    )
                    .ok_or_else(|| ResolveError::IntegerLiteral(literal.spelling.clone()))?;
                    let rank = match rank {
                        IntegerRank::Short => IntRank::Short,
                        IntegerRank::Int => IntRank::Int,
                        IntegerRank::Long => IntRank::Long,
                        IntegerRank::LongLong => IntRank::LongLong,
                        IntegerRank::Int128 => IntRank::Int128,
                    };
                    CTypeKind::Int { rank, signed }
                };
                if literal.imaginary {
                    CTypeKind::Complex(self.ctypes.intern(component))
                } else {
                    component
                }
            }
            ExprKind::FloatLiteral(literal) => {
                let kind = match literal.suffix {
                    FloatSuffix::F16 => FloatKind::Float16,
                    FloatSuffix::F | FloatSuffix::F32 => FloatKind::Float,
                    FloatSuffix::None | FloatSuffix::F64 | FloatSuffix::F32x => FloatKind::Double,
                    FloatSuffix::L => FloatKind::LongDouble,
                    FloatSuffix::F128 | FloatSuffix::Q => FloatKind::Float128,
                    FloatSuffix::DecimalF32 => FloatKind::Decimal32,
                    FloatSuffix::DecimalF64 => FloatKind::Decimal64,
                    FloatSuffix::DecimalF128 => FloatKind::Decimal128,
                    FloatSuffix::F64x => {
                        return Err(ResolveError::Unsupported("target-dependent f64x literals"));
                    }
                };
                let component = CTypeKind::Float(kind);
                if literal.imaginary {
                    CTypeKind::Complex(self.ctypes.intern(component))
                } else {
                    component
                }
            }
            ExprKind::BoolLiteral(_) => CTypeKind::Bool,
            _ => return Err(ResolveError::Unsupported("nonliteral numeric expression")),
        };
        let c = self.ctypes.qual(kind);
        Ok(Operand {
            value: context.resolve_literal(e)?,
            c,
        })
    }

    pub(super) fn arithmetic_conversion(
        &mut self,
        context: &Context,
        operand: Operand,
        c: QualType,
        reason: ConversionReason,
    ) -> Result<Operand, ResolveError> {
        let operand = self.enum_operand(operand);
        let c = self.ctypes.unqualified(c);
        if let Some(underlying) = self.ctypes.enum_underlying(c) {
            let value = context.emit_arithmetic_conversion(
                operand.value,
                self.ir_type(underlying),
                reason,
            )?;
            let node = value.node.clone();
            return Ok(Operand {
                c,
                value: Value {
                    ty: self.ir_type(c),
                    node: node.derive(ValueKind::Convert {
                        kind: crate::ir::ConversionKind::IntToEnum,
                        operand: Box::new(value),
                        reason,
                        semantics: crate::ir::ConversionSema::Exact,
                    }),
                },
            });
        }
        Ok(Operand {
            value: context.emit_arithmetic_conversion(operand.value, self.ir_type(c), reason)?,
            c,
        })
    }

    pub(super) fn promote_operand(
        &mut self,
        context: &Context,
        operand: Operand,
        bits: Option<u32>,
    ) -> Result<Operand, ResolveError> {
        let operand = self.enum_operand(operand);
        let c = self
            .ctypes
            .integer_promotion(operand.c, bits, &context.target);
        self.arithmetic_conversion(context, operand, c, ConversionReason::Promotion)
    }

    pub(super) fn arithmetic_operands(
        &mut self,
        context: &Context,
        left: Operand,
        right: Operand,
    ) -> Result<(Operand, Operand), ResolveError> {
        let left = self.promote_operand(context, left, None)?;
        let right = self.promote_operand(context, right, None)?;
        if self.ctypes.is_fixed_point(left.c) || self.ctypes.is_fixed_point(right.c) {
            let c = self.ctypes.usual_fixed_type(left.c, right.c)?;
            return Ok((
                self.arithmetic_conversion(context, left, c, ConversionReason::UsualArith)?,
                self.arithmetic_conversion(context, right, c, ConversionReason::UsualArith)?,
            ));
        }
        let component = self
            .ctypes
            .usual_real_type(left.c, right.c, &context.target)?;
        let domain = match (
            self.ctypes.canonical_kind(left.c),
            self.ctypes.canonical_kind(right.c),
        ) {
            (CTypeKind::Imaginary(_), CTypeKind::Imaginary(_)) => 1,
            (CTypeKind::Complex(_) | CTypeKind::Imaginary(_), _)
            | (_, CTypeKind::Complex(_) | CTypeKind::Imaginary(_)) => 2,
            _ => 0,
        };
        let c = self.arithmetic_domain(component, domain)?;
        Ok((
            self.arithmetic_conversion(context, left, c, ConversionReason::UsualArith)?,
            self.arithmetic_conversion(context, right, c, ConversionReason::UsualArith)?,
        ))
    }

    fn arithmetic_domain(
        &mut self,
        component: QualType,
        domain: u8,
    ) -> Result<QualType, ResolveError> {
        Ok(match domain {
            2 => self.ctypes.qual(CTypeKind::Complex(component.ty)),
            1 => {
                let CTypeKind::Float(kind) = self.ctypes.canonical_kind(component) else {
                    return Err(ResolveError::Unsupported("imaginary component"));
                };
                self.ctypes.qual(CTypeKind::Imaginary(*kind))
            }
            _ => component,
        })
    }

    pub(super) fn binary_operand(
        &mut self,
        context: &Context,
        e: &Expr,
        op: BinaryOp,
        left: Operand,
        right: Operand,
    ) -> Result<Operand, ResolveError> {
        if matches!(op, BinaryOp::And | BinaryOp::Or) {
            let (ty, kind) = context.emit_binary(op, left.value, right.value)?;
            return Ok(Operand {
                value: Value {
                    ty,
                    node: e.derive(kind),
                },
                c: self.ctypes.int(),
            });
        }
        super::numeric::reject_mixed_decimal(op.into(), &left.value, &right.value)?;
        let left = self.promote_operand(context, left, None)?;
        let right = self.promote_operand(context, right, None)?;
        let comparison = matches!(
            op,
            BinaryOp::Equal
                | BinaryOp::NotEqual
                | BinaryOp::Less
                | BinaryOp::LessEqual
                | BinaryOp::Greater
                | BinaryOp::GreaterEqual
                | BinaryOp::And
                | BinaryOp::Or
        );
        let shift = matches!(op, BinaryOp::ShiftLeft | BinaryOp::ShiftRight);
        let vector = matches!(self.ctypes.canonical_kind(left.c), CTypeKind::Vector { .. })
            || matches!(
                self.ctypes.canonical_kind(right.c),
                CTypeKind::Vector { .. }
            );
        let (left, right, result) = if vector {
            let c = if matches!(self.ctypes.canonical_kind(left.c), CTypeKind::Vector { .. }) {
                left.c
            } else {
                right.c
            };
            let c = if comparison {
                let CTypeKind::Vector {
                    element,
                    lanes,
                    bytes,
                } = self.ctypes.canonical_kind(c).clone()
                else {
                    return Err(ResolveError::Unsupported("comparison mask of nonvector"));
                };
                let element = match self.ctypes.canonical_kind(element) {
                    CTypeKind::Float(_) => {
                        let width =
                            context.target.storage_of(self.ir_type(element))?.size_bytes * 8;
                        let rank = if width == u64::from(context.target.short_width) {
                            IntRank::Short
                        } else if width == u64::from(context.target.int_width) {
                            IntRank::Int
                        } else if width == u64::from(context.target.long_width) {
                            IntRank::Long
                        } else if width == u64::from(context.target.long_long_width) {
                            IntRank::LongLong
                        } else {
                            IntRank::Int128
                        };
                        self.ctypes.qual(CTypeKind::Int { rank, signed: true })
                    }
                    _ => self.ctypes.integer_signedness(element, true)?,
                };
                self.ctypes.qual(CTypeKind::Vector {
                    element,
                    lanes,
                    bytes,
                })
            } else {
                c
            };
            (left, right, c)
        } else if shift || matches!(op, BinaryOp::And | BinaryOp::Or) {
            let c = left.c;
            (left, right, c)
        } else if self.ctypes.is_fixed_point(left.c) || self.ctypes.is_fixed_point(right.c) {
            let c = self.ctypes.usual_fixed_type(left.c, right.c)?;
            (
                self.arithmetic_conversion(context, left, c, ConversionReason::UsualArith)?,
                self.arithmetic_conversion(context, right, c, ConversionReason::UsualArith)?,
                c,
            )
        } else {
            let component = self
                .ctypes
                .usual_real_type(left.c, right.c, &context.target)?;
            let domain = |c| match self.ctypes.canonical_kind(c) {
                CTypeKind::Complex(_) => 2,
                CTypeKind::Imaginary(_) => 1,
                _ => 0,
            };
            let ld = domain(left.c);
            let rd = domain(right.c);
            let result_domain = if ld == 2 || rd == 2 {
                2
            } else if matches!(op, BinaryOp::Mul | BinaryOp::Div) {
                u8::from(ld != rd)
            } else if ld == rd {
                ld
            } else {
                2
            };
            let lc = self.arithmetic_domain(component, ld)?;
            let rc = self.arithmetic_domain(component, rd)?;
            let result = self.arithmetic_domain(component, result_domain)?;
            (
                self.arithmetic_conversion(context, left, lc, ConversionReason::UsualArith)?,
                self.arithmetic_conversion(context, right, rc, ConversionReason::UsualArith)?,
                result,
            )
        };
        let (ty, kind) = context.emit_binary(op, left.value, right.value)?;
        let c = if comparison && !vector {
            self.ctypes.int()
        } else {
            result
        };
        Ok(Operand {
            value: Value {
                ty,
                node: e.derive(kind),
            },
            c,
        })
    }

    pub(super) fn unary_operand(
        &mut self,
        context: &Context,
        e: &Expr,
        op: UnaryOp,
        operand: Operand,
    ) -> Result<Operand, ResolveError> {
        let operand = self.promote_operand(context, operand, None)?;
        if op == UnaryOp::Plus {
            return Ok(operand);
        }
        let c = operand.c;
        let (ty, kind) = context.emit_unary_arith(op, operand.value)?;
        Ok(Operand {
            value: Value {
                ty,
                node: e.derive(kind),
            },
            c,
        })
    }

    pub(super) fn enum_operand(&self, operand: Operand) -> Operand {
        let Some(c) = self.ctypes.enum_underlying(operand.c) else {
            return operand;
        };
        let node = operand.value.node.clone();
        Operand {
            c,
            value: Value {
                ty: self.ir_type(c),
                node: node.derive(ValueKind::Convert {
                    kind: crate::ir::ConversionKind::EnumToInt,
                    operand: Box::new(operand.value),
                    reason: ConversionReason::Promotion,
                    semantics: crate::ir::ConversionSema::Exact,
                }),
            },
        }
    }
}
