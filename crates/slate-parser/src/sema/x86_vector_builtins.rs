use super::builtins::Expansion;
use super::ctype::QualType;
use super::expression::Lowerer;
use super::numeric::{ResolveError, conversion};
use super::operand::Operand;
use super::vector_builtins::LoweredCall;
use crate::ast::Expr;
use crate::ir::*;
use num_bigint::BigInt;

type Captures = Vec<(BindingId, Value)>;

impl Lowerer {
    pub(super) fn expand_x86_vector(
        &mut self,
        e: &Expr,
        expansion: Expansion,
        signature: QualType,
        (returned, ty, arguments): LoweredCall,
    ) -> Result<Result<Operand, LoweredCall>, ResolveError> {
        let result = self.types.ir_type(returned);
        let immediate = |index: usize| {
            arguments
                .get(index)
                .and_then(|value| super::fold::integer_constant(value, self.types.fold_target()))
                .and_then(|value| u32::try_from(value & BigInt::from(0xff)).ok())
        };
        let value = match expansion {
            Expansion::Zero => {
                let zero = self.zero(e, &result)?;
                match arguments.into_iter().next() {
                    Some(argument) if super::effects::has_effects(&argument) => self.value(
                        e,
                        result,
                        ValueKind::Sequence {
                            left: Box::new(argument),
                            right: Box::new(zero),
                        },
                    ),
                    _ => zero,
                }
            }
            Expansion::LaneInsert => {
                let (Some(lanes), Some(index)) = (lanes_of(&result), immediate(2)) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let index = index & (lanes - 1);
                let [vector, element, _]: [Value; 3] = arguments
                    .try_into()
                    .map_err(|_| ResolveError::Internal("vector lane insert arguments"))?;
                let splat = conversion(
                    element,
                    result.clone(),
                    ConversionKind::VectorSplat,
                    ConversionReason::Explicit,
                    ConversionSema::Exact,
                );
                let mask = (0..lanes)
                    .map(|lane| Some(if lane == index { lanes + lane } else { lane }))
                    .collect();
                self.shuffle(e, result, vector, Some(splat), mask)
            }
            Expansion::LaneExtract => {
                let (Some(lanes), Some(index)) = (
                    arguments.first().and_then(|vector| lanes_of(&vector.ty)),
                    immediate(1),
                ) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let vector = arguments
                    .into_iter()
                    .next()
                    .ok_or(ResolveError::Internal("vector lane extract operand"))?;
                let index = self.int_constant(e, index & (lanes - 1));
                self.value(
                    e,
                    result,
                    ValueKind::Lane {
                        vector: Box::new(vector),
                        index: Box::new(index),
                    },
                )
            }
            Expansion::Blend => {
                let (Some(lanes), Some(bits)) = (lanes_of(&result), immediate(2)) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let [left, right, _]: [Value; 3] = arguments
                    .try_into()
                    .map_err(|_| ResolveError::Internal("vector blend arguments"))?;
                let mask = (0..lanes)
                    .map(|lane| {
                        Some(if bits >> (lane % 8) & 1 == 1 {
                            lanes + lane
                        } else {
                            lane
                        })
                    })
                    .collect();
                self.shuffle(e, result, left, Some(right), mask)
            }
            Expansion::PermuteQuads => {
                let (Some(lanes), Some(bits)) = (lanes_of(&result), immediate(1)) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let vector = arguments
                    .into_iter()
                    .next()
                    .ok_or(ResolveError::Internal("vector permute operand"))?;
                let mask = (0..lanes)
                    .map(|lane| Some(lane / 4 * 4 + (bits >> (2 * (lane % 4)) & 3)))
                    .collect();
                self.shuffle(e, result, vector, None, mask)
            }
            Expansion::ByteShift { left } => {
                let (Some(bytes), Some(amount)) = (byte_count(&result), immediate(1)) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let vector = arguments
                    .into_iter()
                    .next()
                    .ok_or(ResolveError::Internal("byte shift operand"))?;
                let byte_vector = Type::Vector {
                    element: NumericType::integer(8, false),
                    lanes: bytes,
                };
                let source = self.context.vector_convert(
                    vector,
                    byte_vector.clone(),
                    ConversionReason::Explicit,
                )?;
                let zero = self.zero(e, &byte_vector)?;
                let mask = (0..bytes)
                    .map(|byte| {
                        let (group, offset) = (byte / 16 * 16, byte % 16);
                        let from = if left {
                            offset.checked_sub(amount)
                        } else {
                            Some(offset + amount).filter(|from| *from < 16)
                        };
                        Some(match from {
                            Some(from) if amount < 16 => group + from,
                            _ => bytes + byte,
                        })
                    })
                    .collect();
                let shifted = self.shuffle(e, byte_vector, source, Some(zero), mask);
                self.context
                    .vector_convert(shifted, result, ConversionReason::Explicit)?
            }
            Expansion::Compare { predicate, scalar } => {
                let (Some(lanes), Some(predicate)) = (
                    lanes_of(&result),
                    predicate.map(u32::from).or_else(|| immediate(2)),
                ) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let mut captures = Vec::new();
                let (left, right) = self.bind_pair(signature, arguments, &mut captures)?;
                let mask = self.predicate_mask(e, predicate, &left, &right)?;
                let mask = self.context.vector_convert(
                    mask,
                    result.clone(),
                    ConversionReason::Explicit,
                )?;
                let value = if scalar {
                    let read = self.read_place(e, &left);
                    let lanes = std::iter::once(Some(lanes))
                        .chain((1..lanes).map(Some))
                        .collect();
                    self.shuffle(e, result, read, Some(mask), lanes)
                } else {
                    mask
                };
                self.captured(e, captures, value)
            }
            Expansion::CompareMask => {
                let (Some(lanes), Some(predicate)) = (
                    arguments.first().and_then(|vector| lanes_of(&vector.ty)),
                    immediate(2),
                ) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let mut arguments = arguments.into_iter();
                let (Some(left), Some(right), Some(_), Some(enabled)) = (
                    arguments.next(),
                    arguments.next(),
                    arguments.next(),
                    arguments.next(),
                ) else {
                    return Err(ResolveError::Internal("vector compare mask arguments"));
                };
                let full = super::fold::integer_constant(&enabled, self.types.fold_target())
                    .is_some_and(|mask| {
                        let all = (BigInt::from(1) << lanes) - 1;
                        mask & &all == all
                    });
                let mut captures = Vec::new();
                let (left, right) = self.bind_pair(signature, vec![left, right], &mut captures)?;
                let mask = self.predicate_mask(e, predicate, &left, &right)?;
                let bits = self.value(
                    e,
                    result.clone(),
                    intrinsic("slate.vector_sign_bitmask", vec![mask]),
                );
                let value = if full {
                    bits
                } else {
                    self.bitwise(e, ArithOp::And, bits, enabled)
                };
                self.captured(e, captures, value)
            }
            Expansion::MaskToVector => {
                let Type::Vector { element, .. } = result else {
                    return Err(ResolveError::Internal("mask to vector result"));
                };
                let ones = self.value(
                    e,
                    Type::Numeric(element),
                    ValueKind::Constant(Number::SignedInteger(BigInt::from(-1))),
                );
                let ones = conversion(
                    ones,
                    result.clone(),
                    ConversionKind::VectorSplat,
                    ConversionReason::Explicit,
                    ConversionSema::Exact,
                );
                let zero = self.zero(e, &result)?;
                let mut arguments = arguments;
                arguments.extend([ones, zero]);
                self.value(e, result, intrinsic("slate.vector_select", arguments))
            }
            Expansion::VectorToMask => {
                self.value(e, result, intrinsic("slate.vector_sign_bitmask", arguments))
            }
            Expansion::MaskedLoad => {
                self.value(e, result, intrinsic("slate.masked_load", arguments))
            }
            Expansion::InsertSubvector => {
                let (Some(lanes), Some(sublanes), Some(index)) = (
                    lanes_of(&result),
                    arguments.get(1).and_then(|vector| lanes_of(&vector.ty)),
                    immediate(2),
                ) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let [vector, part, _]: [Value; 3] = arguments
                    .try_into()
                    .map_err(|_| ResolveError::Internal("subvector insert arguments"))?;
                let first = index % (lanes / sublanes) * sublanes;
                let widened = (0..lanes)
                    .map(|lane| (lane < sublanes).then_some(lane))
                    .collect();
                let widened = self.shuffle(e, result.clone(), part, None, widened);
                let mask = (0..lanes)
                    .map(|lane| {
                        Some(if (first..first + sublanes).contains(&lane) {
                            lanes + lane - first
                        } else {
                            lane
                        })
                    })
                    .collect();
                self.shuffle(e, result, vector, Some(widened), mask)
            }
            _ => return Err(ResolveError::Internal("x86 vector expansion")),
        };
        Ok(Ok(Operand { c: returned, value }))
    }

    fn bind_pair(
        &mut self,
        signature: QualType,
        arguments: Vec<Value>,
        captures: &mut Captures,
    ) -> Result<(Place, Place), ResolveError> {
        let parameters = self
            .types
            .ctypes
            .function_parts(signature)
            .map(|(_, params, _, _)| params.to_vec())
            .unwrap_or_default();
        let mut arguments = arguments.into_iter();
        let (Some(&left_c), Some(&right_c), Some(left), Some(right)) = (
            parameters.first(),
            parameters.get(1),
            arguments.next(),
            arguments.next(),
        ) else {
            return Err(ResolveError::Internal("vector compare operands"));
        };
        let left = self.bind_once(
            Operand {
                c: left_c,
                value: left,
            },
            captures,
        );
        let right = self.bind_once(
            Operand {
                c: right_c,
                value: right,
            },
            captures,
        );
        Ok((left, right))
    }

    fn predicate_mask(
        &mut self,
        e: &Expr,
        predicate: u32,
        left: &Place,
        right: &Place,
    ) -> Result<Value, ResolveError> {
        let Type::Vector {
            element: NumericType::Float(float),
            lanes,
        } = left.ty
        else {
            return Err(ResolveError::Internal("vector compare operand"));
        };
        let width = match float {
            FloatType::F32 => 32,
            FloatType::F64 => 64,
            _ => return Err(ResolveError::Internal("vector compare lane")),
        };
        let mask = Type::Vector {
            element: NumericType::integer(width, true),
            lanes,
        };
        let mut compare = |this: &mut Self, op, x: &Place, y: &Place| {
            let (x, y) = (this.read_place(e, x), this.read_place(e, y));
            this.value(
                e,
                mask.clone(),
                ValueKind::Compare {
                    op,
                    left: Box::new(x),
                    right: Box::new(y),
                    exceptions: Some(Exceptions::Ignore),
                    reason: None,
                },
            )
        };
        let ordered =
            |this: &mut Self,
             compare: &mut dyn FnMut(&mut Self, CompareOp, &Place, &Place) -> Value| {
                let left = compare(this, CompareOp::Eq, left, left);
                let right = compare(this, CompareOp::Eq, right, right);
                this.bitwise(e, ArithOp::And, left, right)
            };
        let unordered =
            |this: &mut Self,
             compare: &mut dyn FnMut(&mut Self, CompareOp, &Place, &Place) -> Value| {
                let left = compare(this, CompareOp::Ne, left, left);
                let right = compare(this, CompareOp::Ne, right, right);
                this.bitwise(e, ArithOp::Or, left, right)
            };
        Ok(match predicate & 15 {
            0 => compare(self, CompareOp::Eq, left, right),
            1 => compare(self, CompareOp::Lt, left, right),
            2 => compare(self, CompareOp::Le, left, right),
            3 => unordered(self, &mut compare),
            4 => compare(self, CompareOp::Ne, left, right),
            5 => {
                let value = compare(self, CompareOp::Lt, left, right);
                self.not(e, value)
            }
            6 => {
                let value = compare(self, CompareOp::Le, left, right);
                self.not(e, value)
            }
            7 => ordered(self, &mut compare),
            8 => {
                let equal = compare(self, CompareOp::Eq, left, right);
                let unordered = unordered(self, &mut compare);
                self.bitwise(e, ArithOp::Or, equal, unordered)
            }
            9 => {
                let value = compare(self, CompareOp::Ge, left, right);
                self.not(e, value)
            }
            10 => {
                let value = compare(self, CompareOp::Gt, left, right);
                self.not(e, value)
            }
            11 => self.zero(e, &mask)?,
            12 => {
                let unequal = compare(self, CompareOp::Ne, left, right);
                let ordered = ordered(self, &mut compare);
                self.bitwise(e, ArithOp::And, unequal, ordered)
            }
            13 => compare(self, CompareOp::Ge, left, right),
            14 => compare(self, CompareOp::Gt, left, right),
            _ => {
                let zero = self.zero(e, &mask)?;
                self.not(e, zero)
            }
        })
    }

    fn read_place(&mut self, e: &Expr, place: &Place) -> Value {
        self.value(
            e,
            place.ty.clone(),
            ValueKind::Read {
                place: place.clone(),
                ordering: None,
            },
        )
    }

    fn bitwise(&mut self, e: &Expr, op: ArithOp, left: Value, right: Value) -> Value {
        let ty = left.ty.clone();
        self.value(
            e,
            ty,
            ValueKind::Arith {
                op,
                left: Box::new(left),
                right: Box::new(right),
                semantics: ArithSema::Exact,
            },
        )
    }

    fn not(&mut self, e: &Expr, operand: Value) -> Value {
        let ty = operand.ty.clone();
        self.value(
            e,
            ty,
            ValueKind::Unary {
                op: UnaryArithOp::Not,
                operand: Box::new(operand),
                semantics: ArithSema::Exact,
            },
        )
    }

    fn shuffle(
        &mut self,
        e: &Expr,
        ty: Type,
        left: Value,
        right: Option<Value>,
        lanes: Vec<Option<u32>>,
    ) -> Value {
        self.value(
            e,
            ty,
            ValueKind::Shuffle {
                left: Box::new(left),
                right: right.map(Box::new),
                mask: ShuffleMask::Lanes(lanes),
            },
        )
    }

    fn int_constant(&mut self, e: &Expr, value: u32) -> Value {
        let int = self.types.ctypes.int();
        self.operand(e, int, ValueKind::Constant(Number::Integer(value.into())))
            .value
    }

    fn zero(&mut self, e: &Expr, ty: &Type) -> Result<Value, ResolveError> {
        let scalar = |element: NumericType| match element {
            NumericType::Float(_) => Number::FloatBits(0),
            NumericType::Integer { .. } => Number::Integer(0u32.into()),
        };
        Ok(match ty {
            Type::Vector { element, .. } => {
                let zero = self.value(
                    e,
                    Type::Numeric(*element),
                    ValueKind::Constant(scalar(*element)),
                );
                conversion(
                    zero,
                    ty.clone(),
                    ConversionKind::VectorSplat,
                    ConversionReason::Explicit,
                    ConversionSema::Exact,
                )
            }
            Type::Numeric(element) => {
                self.value(e, ty.clone(), ValueKind::Constant(scalar(*element)))
            }
            _ => return Err(ResolveError::Internal("zero value type")),
        })
    }
}

fn intrinsic(name: &str, arguments: Vec<Value>) -> ValueKind {
    ValueKind::Intrinsic {
        name: name.into(),
        arguments,
    }
}

fn lanes_of(ty: &Type) -> Option<u32> {
    match ty {
        Type::Vector { lanes, .. } => Some(*lanes),
        _ => None,
    }
}

fn byte_count(ty: &Type) -> Option<u32> {
    let Type::Vector { element, lanes } = ty else {
        return None;
    };
    let bits = match element {
        NumericType::Integer { width, .. } => *width,
        NumericType::Float(FloatType::F32) => 32,
        NumericType::Float(FloatType::F64) => 64,
        NumericType::Float(_) => return None,
    };
    Some(bits / 8 * lanes)
}
