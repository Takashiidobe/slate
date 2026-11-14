use super::builtins::{Expansion, MaskOp, Passthrough};
use super::ctype::QualType;
use super::expression::Lowerer;
use super::numeric::{ResolveError, conversion};
use super::operand::Operand;
use super::vector_builtins::LoweredCall;
use crate::ast::Expr;
use crate::ir::*;
use num_bigint::{BigInt, BigUint};

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
            Expansion::CompareMask { integer } => {
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
                let mask = match integer {
                    Some(unsigned) => {
                        self.integer_predicate_mask(e, predicate, &left, &right, unsigned)?
                    }
                    None => self.predicate_mask(e, predicate, &left, &right)?,
                };
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
            Expansion::ShuffleLanes => {
                let (Some(lanes), Some(bytes), Some(bits)) =
                    (lanes_of(&result), byte_count(&result), immediate(2))
                else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let [left, right, _]: [Value; 3] = arguments
                    .try_into()
                    .map_err(|_| ResolveError::Internal("vector shuffle arguments"))?;
                let group = lanes / (bytes / 16);
                let mut selector = bits * 0x0101_0101;
                let mut mask = Vec::new();
                for first in (0..lanes).step_by(group as usize) {
                    for lane in 0..group {
                        let mut index = selector % group;
                        selector /= group;
                        if lane >= group / 2 {
                            index += lanes;
                        }
                        mask.push(Some(first + index));
                    }
                }
                self.shuffle(e, result, left, Some(right), mask)
            }
            Expansion::AlignBytes => {
                let (Some(lanes), Some(shift)) = (lanes_of(&result), immediate(2)) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let mut captures = Vec::new();
                let (high, low) = self.bind_pair(signature, arguments, &mut captures)?;
                let value = if shift >= 32 {
                    self.zero(e, &result)?
                } else {
                    let (low, high, shift) = if shift > 16 {
                        (
                            self.read_place(e, &high),
                            self.zero(e, &result)?,
                            shift - 16,
                        )
                    } else {
                        (self.read_place(e, &low), self.read_place(e, &high), shift)
                    };
                    let mask = (0..lanes)
                        .map(|byte| {
                            let index = shift + byte % 16;
                            let index = if index >= 16 {
                                index + lanes - 16
                            } else {
                                index
                            };
                            Some(index + byte / 16 * 16)
                        })
                        .collect();
                    self.shuffle(e, result, low, Some(high), mask)
                };
                self.captured(e, captures, value)
            }
            Expansion::PermuteHalves => {
                let (Some(lanes), Some(bits)) = (lanes_of(&result), immediate(2)) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let mut captures = Vec::new();
                let (left, right) = self.bind_pair(signature, arguments, &mut captures)?;
                let half = lanes / 2;
                let mut sources = Vec::new();
                let mut mask = Vec::new();
                for part in 0..2 {
                    let control = bits >> (part * 4);
                    let source = if control & 8 != 0 {
                        self.zero(e, &result)?
                    } else if control & 2 != 0 {
                        self.read_place(e, &right)
                    } else {
                        self.read_place(e, &left)
                    };
                    sources.push(source);
                    let offset = if control & 1 != 0 { half } else { 0 };
                    mask.extend((0..half).map(|lane| Some(part * lanes + offset + lane)));
                }
                let [low, high]: [Value; 2] = sources
                    .try_into()
                    .map_err(|_| ResolveError::Internal("half permute sources"))?;
                let value = self.shuffle(e, result, low, Some(high), mask);
                self.captured(e, captures, value)
            }
            Expansion::MaskedFmaddSub(passthrough) => {
                let name = match &result {
                    Type::Vector {
                        element: NumericType::Float(FloatType::F64),
                        ..
                    } => "llvm.x86.avx512.vfmaddsub.pd.512",
                    Type::Vector {
                        element: NumericType::Float(FloatType::F32),
                        ..
                    } => "llvm.x86.avx512.vfmaddsub.ps.512",
                    _ => return Ok(Err((returned, ty, arguments))),
                };
                let [a, b, c, enabled, rounding]: [Value; 5] = arguments
                    .try_into()
                    .map_err(|_| ResolveError::Internal("masked fmaddsub arguments"))?;
                let mut captures = Vec::new();
                let places = self.bind_all(signature, vec![a, b, c], &mut captures)?;
                let mut operands: Vec<Value> = places
                    .iter()
                    .map(|place| self.read_place(e, place))
                    .collect();
                operands.push(rounding);
                let fused = self.value(e, result.clone(), intrinsic(name, operands));
                let kept = match passthrough {
                    Passthrough::First => self.read_place(e, &places[0]),
                    Passthrough::Zero => self.zero(e, &result)?,
                    Passthrough::Third => self.read_place(e, &places[2]),
                };
                let value = self.value(
                    e,
                    result,
                    intrinsic("slate.vector_select", vec![enabled, fused, kept]),
                );
                self.captured(e, captures, value)
            }
            Expansion::ShuffleWords { high } => {
                let (Some(lanes), Some(bits)) = (lanes_of(&result), immediate(1)) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let vector = arguments
                    .into_iter()
                    .next()
                    .ok_or(ResolveError::Internal("word shuffle operand"))?;
                let mask = (0..lanes)
                    .map(|lane| {
                        let (group, offset) = (lane / 8 * 8, lane % 8);
                        Some(if (offset >= 4) == high {
                            group + offset / 4 * 4 + (bits >> (2 * (offset % 4)) & 3)
                        } else {
                            lane
                        })
                    })
                    .collect();
                self.shuffle(e, result, vector, None, mask)
            }
            Expansion::PermutePairs => {
                let (Some(lanes), Some(bits)) = (lanes_of(&result), immediate(1)) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let vector = arguments
                    .into_iter()
                    .next()
                    .ok_or(ResolveError::Internal("pair permute operand"))?;
                let mask = (0..lanes)
                    .map(|lane| Some(lane / 2 * 2 + (bits >> (lane % 8) & 1)))
                    .collect();
                self.shuffle(e, result, vector, None, mask)
            }
            Expansion::MaskLogic(op) => {
                let Type::Numeric(NumericType::Integer { width, .. }) = result else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let shift = immediate(1);
                let mut operands = arguments.into_iter();
                let (Some(left), second) = (operands.next(), operands.next()) else {
                    return Err(ResolveError::Internal("mask operation operands"));
                };
                let require =
                    |value: Option<Value>| value.ok_or(ResolveError::Internal("mask operand"));
                match op {
                    MaskOp::And => self.bitwise(e, ArithOp::And, left, require(second)?),
                    MaskOp::AndNot => {
                        let inverted = self.not(e, left);
                        self.bitwise(e, ArithOp::And, inverted, require(second)?)
                    }
                    MaskOp::Or => self.bitwise(e, ArithOp::Or, left, require(second)?),
                    MaskOp::Xor => self.bitwise(e, ArithOp::Xor, left, require(second)?),
                    MaskOp::Xnor => {
                        let difference = self.bitwise(e, ArithOp::Xor, left, require(second)?);
                        self.not(e, difference)
                    }
                    MaskOp::Not => self.not(e, left),
                    MaskOp::Move => left,
                    MaskOp::Add => self.value(
                        e,
                        result,
                        ValueKind::Arith {
                            op: ArithOp::Add,
                            left: Box::new(left),
                            right: Box::new(require(second)?),
                            semantics: ArithSema::Integer {
                                overflow: Overflow::Wrap,
                            },
                        },
                    ),
                    MaskOp::Unpack => {
                        let half = width / 2;
                        let low =
                            self.mask_constant(e, &result, (BigUint::from(1u32) << half) - 1u32);
                        let low = self.bitwise(e, ArithOp::And, require(second)?, low);
                        let high = self.mask_shift(e, ArithOp::Shl, left, half);
                        self.bitwise(e, ArithOp::Or, high, low)
                    }
                    MaskOp::ShiftLeft | MaskOp::ShiftRight => {
                        let Some(shift) = shift else {
                            let mut arguments = vec![left];
                            arguments.extend(second);
                            return Ok(Err((returned, ty, arguments)));
                        };
                        if shift >= width {
                            let zero = self.zero(e, &result)?;
                            if super::effects::has_effects(&left) {
                                self.value(
                                    e,
                                    result,
                                    ValueKind::Sequence {
                                        left: Box::new(left),
                                        right: Box::new(zero),
                                    },
                                )
                            } else {
                                zero
                            }
                        } else {
                            let op = if op == MaskOp::ShiftLeft {
                                ArithOp::Shl
                            } else {
                                ArithOp::Shr
                            };
                            self.mask_shift(e, op, left, shift)
                        }
                    }
                }
            }
            Expansion::MaskedStore => {
                self.value(e, result, intrinsic("slate.masked_store", arguments))
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
        let mut places = self
            .bind_all(signature, arguments.into_iter().take(2).collect(), captures)?
            .into_iter();
        match (places.next(), places.next()) {
            (Some(left), Some(right)) => Ok((left, right)),
            _ => Err(ResolveError::Internal("vector operand pair")),
        }
    }

    fn bind_all(
        &mut self,
        signature: QualType,
        arguments: Vec<Value>,
        captures: &mut Captures,
    ) -> Result<Vec<Place>, ResolveError> {
        let parameters = self
            .types
            .ctypes
            .function_parts(signature)
            .map(|(_, params, _, _)| params.to_vec())
            .unwrap_or_default();
        if parameters.len() < arguments.len() {
            return Err(ResolveError::Internal("vector builtin operands"));
        }
        Ok(parameters
            .into_iter()
            .zip(arguments)
            .map(|(c, value)| self.bind_once(Operand { c, value }, captures))
            .collect())
    }

    fn integer_predicate_mask(
        &mut self,
        e: &Expr,
        predicate: u32,
        left: &Place,
        right: &Place,
        unsigned: bool,
    ) -> Result<Value, ResolveError> {
        let Type::Vector {
            element: NumericType::Integer { width, .. },
            lanes,
        } = left.ty
        else {
            return Err(ResolveError::Internal("integer vector compare operand"));
        };
        let operand = Type::Vector {
            element: NumericType::integer(width, !unsigned),
            lanes,
        };
        let mask = Type::Vector {
            element: NumericType::integer(width, true),
            lanes,
        };
        let op = match predicate & 7 {
            0 => CompareOp::Eq,
            1 => CompareOp::Lt,
            2 => CompareOp::Le,
            3 => return self.zero(e, &mask),
            4 => CompareOp::Ne,
            5 => CompareOp::Ge,
            6 => CompareOp::Gt,
            _ => {
                let zero = self.zero(e, &mask)?;
                return Ok(self.not(e, zero));
            }
        };
        let read = |this: &mut Self, place: &Place| {
            let value = this.read_place(e, place);
            if value.ty == operand {
                Ok(value)
            } else {
                this.context
                    .vector_convert(value, operand.clone(), ConversionReason::Explicit)
            }
        };
        let (x, y) = (read(self, left)?, read(self, right)?);
        Ok(self.value(
            e,
            mask,
            ValueKind::Compare {
                op,
                left: Box::new(x),
                right: Box::new(y),
                exceptions: None,
                reason: None,
            },
        ))
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

    fn mask_constant(&mut self, e: &Expr, ty: &Type, value: BigUint) -> Value {
        self.value(e, ty.clone(), ValueKind::Constant(Number::Integer(value)))
    }

    fn mask_shift(&mut self, e: &Expr, op: ArithOp, value: Value, amount: u32) -> Value {
        let ty = value.ty.clone();
        let amount = self.mask_constant(e, &ty, amount.into());
        let semantics = if op == ArithOp::Shl {
            ArithSema::ShiftLeft {
                overflow: Overflow::Wrap,
                amount_out_of_range: UbPolicy::Undefined,
                negative_left: None,
            }
        } else {
            ArithSema::ShiftRight {
                fill: ShiftFill::ZeroExtend,
                amount_out_of_range: UbPolicy::Undefined,
            }
        };
        self.value(
            e,
            ty,
            ValueKind::Arith {
                op,
                left: Box::new(value),
                right: Box::new(amount),
                semantics,
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
