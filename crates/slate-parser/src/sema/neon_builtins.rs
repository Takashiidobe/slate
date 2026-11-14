use super::ctype::QualType;
use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::numeric::conversion;
use super::operand::Operand;
use super::vector_builtins::LoweredCall;
use crate::ast::Expr;
use crate::ir::*;
use num_bigint::BigInt;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(super) enum NeonOp {
    LaneRead,
    AddAcross { long: bool },
    MaxAcross,
    MinAcross,
    MaxNumAcross,
    MinNumAcross,
    BitSelect,
    Abs,
    Max,
    Min,
    MaxNum,
    MinNum,
    SaturatingAdd,
    SaturatingSub,
    Fma,
    Sqrt,
    AbsGreater,
    Load,
    LoadLane,
    Load2,
    Store,
    StoreLane,
    Store2,
    ShiftLeftImmediate,
    ShiftRightImmediate,
    ShiftLeft,
    Table(u8),
    Round(&'static str),
    ConvertRounding(&'static str),
    Widen,
    Narrow,
    MultiplyLong,
}

impl NeonOp {
    fn shape(self) -> NeonShape {
        match self {
            Self::AbsGreater | Self::ConvertRounding(_) => NeonShape::Float,
            Self::Widen | Self::MultiplyLong => NeonShape::Half,
            Self::Narrow => NeonShape::Double,
            _ => NeonShape::Same,
        }
    }
}

pub(super) fn neon_op(record: &str) -> Option<NeonOp> {
    let name = record.strip_prefix("__builtin_neon_")?;
    if name.starts_with("vgetq_lane_") || name.starts_with("vget_lane_") {
        return Some(NeonOp::LaneRead);
    }
    let across = |prefix: &str| {
        name.strip_prefix(prefix).is_some_and(|rest| {
            matches!(
                rest,
                "q_f32"
                    | "q_f64"
                    | "q_s8"
                    | "q_s16"
                    | "q_s32"
                    | "q_u8"
                    | "q_u16"
                    | "q_u32"
                    | "q_s64"
                    | "q_u64"
                    | "_f32"
                    | "_s8"
                    | "_s16"
                    | "_s32"
                    | "_u8"
                    | "_u16"
                    | "_u32"
            )
        })
    };
    Some(match name {
        _ if across("vaddv") => NeonOp::AddAcross { long: false },
        _ if across("vaddlv") => NeonOp::AddAcross { long: true },
        _ if across("vmaxnmv") => NeonOp::MaxNumAcross,
        _ if across("vminnmv") => NeonOp::MinNumAcross,
        _ if across("vmaxv") => NeonOp::MaxAcross,
        _ if across("vminv") => NeonOp::MinAcross,
        "vbsl_v" | "vbslq_v" => NeonOp::BitSelect,
        "vabs_v" | "vabsq_v" => NeonOp::Abs,
        "vmax_v" | "vmaxq_v" => NeonOp::Max,
        "vmin_v" | "vminq_v" => NeonOp::Min,
        "vmaxnm_v" | "vmaxnmq_v" => NeonOp::MaxNum,
        "vminnm_v" | "vminnmq_v" => NeonOp::MinNum,
        "vqadd_v" | "vqaddq_v" => NeonOp::SaturatingAdd,
        "vqsub_v" | "vqsubq_v" => NeonOp::SaturatingSub,
        "vfma_v" | "vfmaq_v" => NeonOp::Fma,
        "vsqrt_v" | "vsqrtq_v" => NeonOp::Sqrt,
        "vcagt_v" | "vcagtq_v" => NeonOp::AbsGreater,
        "vld1_v" | "vld1q_v" => NeonOp::Load,
        "vld1_lane_v" | "vld1q_lane_v" => NeonOp::LoadLane,
        "vld2_v" | "vld2q_v" => NeonOp::Load2,
        "vst1_v" | "vst1q_v" => NeonOp::Store,
        "vst1_lane_v" | "vst1q_lane_v" => NeonOp::StoreLane,
        "vst2_v" | "vst2q_v" => NeonOp::Store2,
        "vshl_n_v" | "vshlq_n_v" => NeonOp::ShiftLeftImmediate,
        "vshr_n_v" | "vshrq_n_v" => NeonOp::ShiftRightImmediate,
        "vshl_v" | "vshlq_v" => NeonOp::ShiftLeft,
        "vqtbl1_v" | "vqtbl1q_v" => NeonOp::Table(1),
        "vqtbl2_v" | "vqtbl2q_v" => NeonOp::Table(2),
        "vqtbl3_v" | "vqtbl3q_v" => NeonOp::Table(3),
        "vqtbl4_v" | "vqtbl4q_v" => NeonOp::Table(4),
        "vrnd_v" | "vrndq_v" => NeonOp::Round("llvm.trunc"),
        "vrndm_v" | "vrndmq_v" => NeonOp::Round("llvm.floor"),
        "vrndn_v" | "vrndnq_v" => NeonOp::Round("llvm.roundeven"),
        "vrndp_v" | "vrndpq_v" => NeonOp::Round("llvm.ceil"),
        "vrnda_v" | "vrndaq_v" => NeonOp::Round("llvm.round"),
        "vrndx_v" | "vrndxq_v" => NeonOp::Round("llvm.rint"),
        "vrndi_v" | "vrndiq_v" => NeonOp::Round("llvm.nearbyint"),
        "vmovl_v" => NeonOp::Widen,
        "vmovn_v" => NeonOp::Narrow,
        "vmull_v" => NeonOp::MultiplyLong,
        _ if convert_rounding(name).is_some() => NeonOp::ConvertRounding(convert_rounding(name)?),
        _ => return None,
    })
}

impl Lowerer {
    pub(super) fn expand_neon(
        &mut self,
        e: &Expr,
        op: NeonOp,
        signature: QualType,
        (returned, ty, arguments): LoweredCall,
    ) -> Result<Result<Operand, LoweredCall>, ResolveError> {
        let result = self.types.ir_type(returned);
        let typed = !matches!(
            op,
            NeonOp::LaneRead
                | NeonOp::AddAcross { .. }
                | NeonOp::MaxAcross
                | NeonOp::MinAcross
                | NeonOp::MaxNumAcross
                | NeonOp::MinNumAcross
        );
        let shape = arguments
            .last()
            .and_then(|code| self.neon_constant(code))
            .and_then(|code| neon_vector(code, op.shape()))
            .unwrap_or((Type::Void, Type::Void));
        if typed && shape.1 == Type::Void {
            return Ok(Err((returned, ty, arguments)));
        }
        let value = match op {
            NeonOp::LaneRead => {
                let Some((vector, lanes)) = arguments.first().and_then(|vector| match vector.ty {
                    Type::Vector { element, lanes } => Some((element, lanes)),
                    _ => None,
                }) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let Some(index) = arguments.get(1).and_then(|index| self.neon_constant(index))
                else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let index = self.neon_index(e, index % lanes);
                let source = arguments
                    .into_iter()
                    .next()
                    .ok_or(ResolveError::Internal("neon lane operand"))?;
                let lane = self.value(
                    e,
                    Type::Numeric(vector),
                    ValueKind::Lane {
                        vector: Box::new(source),
                        index: Box::new(index),
                    },
                );
                self.context
                    .emit_arithmetic_conversion(lane, result, ConversionReason::Explicit)?
            }
            NeonOp::AddAcross { .. }
            | NeonOp::MaxAcross
            | NeonOp::MinAcross
            | NeonOp::MaxNumAcross
            | NeonOp::MinNumAcross => {
                let [vector]: [Value; 1] = arguments
                    .try_into()
                    .map_err(|_| ResolveError::Internal("neon across-vector operand count"))?;
                let reduced = self.neon_across(e, op, vector)?;
                self.context.emit_arithmetic_conversion(
                    reduced,
                    result,
                    ConversionReason::Explicit,
                )?
            }
            NeonOp::Load
            | NeonOp::LoadLane
            | NeonOp::Load2
            | NeonOp::Store
            | NeonOp::StoreLane
            | NeonOp::Store2 => {
                let lane = match op {
                    NeonOp::LoadLane | NeonOp::StoreLane => {
                        let Some(lane) = arguments.get(2).and_then(|lane| self.neon_constant(lane))
                        else {
                            return Ok(Err((returned, ty, arguments)));
                        };
                        lane
                    }
                    _ => 0,
                };
                let mut operands = arguments;
                operands.pop();
                let value = self.neon_memory(e, op, signature, shape.1, lane, operands)?;
                match result {
                    Type::Void => value,
                    _ => self
                        .context
                        .vector_convert(value, result, ConversionReason::Explicit)?,
                }
            }
            NeonOp::ShiftLeftImmediate | NeonOp::ShiftRightImmediate => {
                let Some(amount) = arguments
                    .get(1)
                    .and_then(|amount| self.neon_constant(amount))
                else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let vector = arguments
                    .into_iter()
                    .next()
                    .ok_or(ResolveError::Internal("neon shift operand"))?;
                let shifted = self.neon_shift_immediate(e, op, shape.1, vector, amount)?;
                self.context
                    .vector_convert(shifted, result, ConversionReason::Explicit)?
            }
            NeonOp::Table(count) => {
                let mut operands = arguments;
                operands.pop();
                let index = operands
                    .pop()
                    .ok_or(ResolveError::Internal("neon table index"))?;
                if operands.len() != usize::from(count) {
                    return Err(ResolveError::Internal("neon table operand count"));
                }
                let (_, vector) = shape;
                operands.push(self.context.vector_convert(
                    index,
                    vector.clone(),
                    ConversionReason::Explicit,
                )?);
                let looked_up = self.value(
                    e,
                    vector,
                    ValueKind::Intrinsic {
                        name: format!("llvm.aarch64.neon.tbl{count}"),
                        arguments: operands,
                    },
                );
                self.context
                    .vector_convert(looked_up, result, ConversionReason::Explicit)?
            }
            _ => {
                if op == NeonOp::BitSelect && arguments.iter().any(super::effects::has_effects) {
                    return Ok(Err((returned, ty, arguments)));
                }
                let mut operands = arguments;
                operands.pop();
                let lanewise = self.neon_lanewise(e, op, shape, operands)?;
                self.context
                    .vector_convert(lanewise, result, ConversionReason::Explicit)?
            }
        };
        Ok(Ok(Operand { c: returned, value }))
    }

    fn neon_across(&mut self, e: &Expr, op: NeonOp, vector: Value) -> Result<Value, ResolveError> {
        let Type::Vector { element, lanes } = vector.ty else {
            return Err(ResolveError::Internal("neon across-vector operand"));
        };
        let float = matches!(element, NumericType::Float(_));
        let signed = matches!(element, NumericType::Integer { signed: true, .. });
        let (vector, element) = match (op, element) {
            (NeonOp::AddAcross { long: true }, NumericType::Integer { width, signed, .. }) => {
                let wide = NumericType::integer(width * 2, signed);
                let widened = self.context.elementwise_conversion(
                    vector,
                    Type::Numeric(element),
                    Type::Numeric(wide),
                    lanes,
                    ConversionReason::Explicit,
                )?;
                (widened, wide)
            }
            _ => (vector, element),
        };
        let name = match op {
            NeonOp::AddAcross { .. } if float => "llvm.aarch64.neon.faddv",
            NeonOp::AddAcross { .. } => "llvm.vector.reduce.add",
            NeonOp::MaxAcross if float => "llvm.aarch64.neon.fmaxv",
            NeonOp::MaxAcross if signed => "llvm.vector.reduce.smax",
            NeonOp::MaxAcross => "llvm.vector.reduce.umax",
            NeonOp::MinAcross if float => "llvm.aarch64.neon.fminv",
            NeonOp::MinAcross if signed => "llvm.vector.reduce.smin",
            NeonOp::MinAcross => "llvm.vector.reduce.umin",
            NeonOp::MaxNumAcross => "llvm.aarch64.neon.fmaxnmv",
            NeonOp::MinNumAcross => "llvm.aarch64.neon.fminnmv",
            _ => return Err(ResolveError::Internal("neon across-vector operation")),
        };
        Ok(self.value(
            e,
            Type::Numeric(element),
            ValueKind::Intrinsic {
                name: name.into(),
                arguments: vec![vector],
            },
        ))
    }

    fn neon_lanewise(
        &mut self,
        e: &Expr,
        op: NeonOp,
        (operand, result): (Type, Type),
        operands: Vec<Value>,
    ) -> Result<Value, ResolveError> {
        if op == NeonOp::BitSelect {
            let [mask, left, right]: [Value; 3] = operands
                .try_into()
                .map_err(|_| ResolveError::Internal("neon bit select operand count"))?;
            let ty = left.ty.clone();
            let bitwise = |this: &mut Self, op: ArithOp, left: Value, right: Value| {
                this.value(
                    e,
                    ty.clone(),
                    ValueKind::Arith {
                        op,
                        left: Box::new(left),
                        right: Box::new(right),
                        semantics: ArithSema::Exact,
                    },
                )
            };
            let difference = bitwise(self, ArithOp::Xor, left, right.clone());
            let chosen = bitwise(self, ArithOp::And, difference, mask);
            return Ok(bitwise(self, ArithOp::Xor, right, chosen));
        }
        let mut operands = operands
            .into_iter()
            .map(|value| {
                self.context
                    .vector_convert(value, operand.clone(), ConversionReason::Explicit)
            })
            .collect::<Result<Vec<_>, _>>()?;
        let Type::Vector { element, .. } = operand else {
            return Err(ResolveError::Internal("neon vector shape"));
        };
        let Type::Vector {
            element: result_element,
            lanes,
        } = result
        else {
            return Err(ResolveError::Internal("neon result shape"));
        };
        if matches!(op, NeonOp::Widen | NeonOp::Narrow | NeonOp::MultiplyLong) {
            let mut converted = operands.into_iter().map(|value| {
                self.context.elementwise_conversion(
                    value,
                    Type::Numeric(element),
                    Type::Numeric(result_element),
                    lanes,
                    ConversionReason::Explicit,
                )
            });
            let (Some(left), right) = (converted.next(), converted.next()) else {
                return Err(ResolveError::Internal("neon widen operand count"));
            };
            let (left, right) = (left?, right.transpose()?);
            return Ok(match right {
                Some(right) if op == NeonOp::MultiplyLong => self.value(
                    e,
                    result,
                    ValueKind::Arith {
                        op: ArithOp::Mul,
                        left: Box::new(left),
                        right: Box::new(right),
                        semantics: ArithSema::Integer {
                            overflow: Overflow::Wrap,
                        },
                    },
                ),
                _ => left,
            });
        }
        let float = matches!(element, NumericType::Float(_));
        let signed = matches!(element, NumericType::Integer { signed: true, .. });
        let result_signed = matches!(result_element, NumericType::Integer { signed: true, .. });
        let converted;
        let name = match op {
            NeonOp::Round(name) => name,
            NeonOp::ConvertRounding(mode) => {
                converted = format!(
                    "llvm.aarch64.neon.fcvt{mode}{}",
                    if result_signed { 's' } else { 'u' }
                );
                converted.as_str()
            }
            NeonOp::Abs if float => "llvm.fabs",
            NeonOp::Abs => "llvm.aarch64.neon.abs",
            NeonOp::Max if float => "llvm.aarch64.neon.fmax",
            NeonOp::Max if signed => "llvm.aarch64.neon.smax",
            NeonOp::Max => "llvm.aarch64.neon.umax",
            NeonOp::Min if float => "llvm.aarch64.neon.fmin",
            NeonOp::Min if signed => "llvm.aarch64.neon.smin",
            NeonOp::Min => "llvm.aarch64.neon.umin",
            NeonOp::MaxNum => "llvm.aarch64.neon.fmaxnm",
            NeonOp::MinNum => "llvm.aarch64.neon.fminnm",
            NeonOp::SaturatingAdd if signed => "llvm.aarch64.neon.sqadd",
            NeonOp::SaturatingAdd => "llvm.aarch64.neon.uqadd",
            NeonOp::SaturatingSub if signed => "llvm.aarch64.neon.sqsub",
            NeonOp::SaturatingSub => "llvm.aarch64.neon.uqsub",
            NeonOp::Fma => {
                operands.rotate_left(1);
                "llvm.fma"
            }
            NeonOp::Sqrt => "llvm.sqrt",
            NeonOp::AbsGreater => "llvm.aarch64.neon.facgt",
            NeonOp::ShiftLeft if signed => "llvm.aarch64.neon.sshl",
            NeonOp::ShiftLeft => "llvm.aarch64.neon.ushl",
            _ => return Err(ResolveError::Internal("neon lanewise operation")),
        };
        Ok(self.value(
            e,
            result,
            ValueKind::Intrinsic {
                name: name.into(),
                arguments: operands,
            },
        ))
    }

    fn neon_memory(
        &mut self,
        e: &Expr,
        op: NeonOp,
        signature: QualType,
        vector: Type,
        lane: u32,
        operands: Vec<Value>,
    ) -> Result<Value, ResolveError> {
        let Type::Vector { element, lanes } = vector else {
            return Err(ResolveError::Internal("neon memory vector"));
        };
        let scalar = Type::Numeric(element);
        fn count<T>(_: T) -> ResolveError {
            ResolveError::Internal("neon memory operand count")
        }
        match op {
            NeonOp::Load => {
                let [pointer]: [Value; 1] = operands.try_into().map_err(count)?;
                let place = self.neon_place(e, pointer, &vector, None);
                Ok(self.value(
                    e,
                    vector,
                    ValueKind::Read {
                        place,
                        ordering: None,
                    },
                ))
            }
            NeonOp::Store => {
                let [pointer, value]: [Value; 2] = operands.try_into().map_err(count)?;
                let value = self.context.vector_convert(
                    value,
                    vector.clone(),
                    ConversionReason::Explicit,
                )?;
                let place = self.neon_place(e, pointer, &vector, None);
                Ok(self.neon_store(e, place, value))
            }
            NeonOp::LoadLane => {
                let [pointer, source, _]: [Value; 3] = operands.try_into().map_err(count)?;
                let source = self.context.vector_convert(
                    source,
                    vector.clone(),
                    ConversionReason::Explicit,
                )?;
                let place = self.neon_place(e, pointer, &scalar, None);
                let loaded = self.value(
                    e,
                    scalar,
                    ValueKind::Read {
                        place,
                        ordering: None,
                    },
                );
                let splat = conversion(
                    loaded,
                    vector.clone(),
                    ConversionKind::VectorSplat,
                    ConversionReason::Explicit,
                    ConversionSema::Exact,
                );
                let index = lane % lanes;
                let mask = (0..lanes)
                    .map(|each| Some(if each == index { lanes + each } else { each }))
                    .collect();
                Ok(self.shuffle(e, vector, source, Some(splat), mask))
            }
            NeonOp::StoreLane => {
                let [pointer, source, _]: [Value; 3] = operands.try_into().map_err(count)?;
                let source = self.context.vector_convert(
                    source,
                    vector.clone(),
                    ConversionReason::Explicit,
                )?;
                let index = self.neon_index(e, lane % lanes);
                let value = self.value(
                    e,
                    scalar.clone(),
                    ValueKind::Lane {
                        vector: Box::new(source),
                        index: Box::new(index),
                    },
                );
                let place = self.neon_place(e, pointer, &scalar, None);
                Ok(self.neon_store(e, place, value))
            }
            NeonOp::Load2 => {
                let mut captures = Vec::new();
                let [target, source]: [Place; 2] = self
                    .bind_all(signature, operands, &mut captures)?
                    .try_into()
                    .map_err(count)?;
                let mut parts = Vec::new();
                for odd in 0..2 {
                    let mut halves = Vec::new();
                    for half in 0..2 {
                        let pointer = self.read_place(e, &source);
                        let place = self.neon_place(e, pointer, &vector, Some(half));
                        halves.push(self.value(
                            e,
                            vector.clone(),
                            ValueKind::Read {
                                place,
                                ordering: None,
                            },
                        ));
                    }
                    let [low, high]: [Value; 2] = halves.try_into().map_err(count)?;
                    let mask = (0..lanes).map(|each| Some(2 * each + odd)).collect();
                    let part = self.shuffle(e, vector.clone(), low, Some(high), mask);
                    let pointer = self.read_place(e, &target);
                    let place = self.neon_place(e, pointer, &vector, Some(odd));
                    parts.push(self.neon_store(e, place, part));
                }
                let stored = self.neon_sequence(e, parts);
                Ok(self.captured(e, captures, stored))
            }
            NeonOp::Store2 => {
                let mut captures = Vec::new();
                let [target, low, high]: [Place; 3] = self
                    .bind_all(signature, operands, &mut captures)?
                    .try_into()
                    .map_err(count)?;
                let mut parts = Vec::new();
                for half in 0..2 {
                    let mut sources = Vec::new();
                    for place in [&low, &high] {
                        let read = self.read_place(e, place);
                        sources.push(self.context.vector_convert(
                            read,
                            vector.clone(),
                            ConversionReason::Explicit,
                        )?);
                    }
                    let [first, second]: [Value; 2] = sources.try_into().map_err(count)?;
                    let mask = (0..lanes)
                        .map(|each| Some(half * lanes / 2 + each / 2 + each % 2 * lanes))
                        .collect();
                    let part = self.shuffle(e, vector.clone(), first, Some(second), mask);
                    let pointer = self.read_place(e, &target);
                    let place = self.neon_place(e, pointer, &vector, Some(half));
                    parts.push(self.neon_store(e, place, part));
                }
                let stored = self.neon_sequence(e, parts);
                Ok(self.captured(e, captures, stored))
            }
            _ => Err(ResolveError::Internal("neon memory operation")),
        }
    }

    fn neon_place(&mut self, e: &Expr, pointer: Value, ty: &Type, index: Option<u32>) -> Place {
        let (element, bytes) = match ty {
            Type::Vector { element, lanes } => {
                (*element, element_bytes(*element) * u64::from(*lanes))
            }
            Type::Numeric(element) => (*element, element_bytes(*element)),
            _ => (NumericType::integer(8, false), 1),
        };
        let align = element_bytes(element);
        let mut pointer = neon_pointer(pointer, ty);
        if let Some(index) = index.filter(|index| *index > 0) {
            let amount = self.neon_index(e, index);
            let pointer_type = pointer.ty.clone();
            pointer = self.value(
                e,
                pointer_type,
                ValueKind::PointerOffset {
                    pointer: Box::new(pointer),
                    amount: Box::new(amount),
                    subtract: false,
                    element: ty.clone(),
                    overflow: Overflow::Wrap,
                },
            );
        }
        Place {
            ty: ty.clone(),
            kind: PlaceKind::Deref(Box::new(pointer)),
            access: Access {
                align: (align < bytes).then_some(align),
                ..Access::default()
            },
        }
    }

    fn neon_store(&mut self, e: &Expr, place: Place, value: Value) -> Value {
        let ty = value.ty.clone();
        let store = self.value(
            e,
            ty,
            ValueKind::Store {
                place,
                value: Box::new(value),
                ordering: None,
            },
        );
        self.neon_sequence(e, vec![store])
    }

    fn neon_sequence(&mut self, e: &Expr, effects: Vec<Value>) -> Value {
        let void = self.value(e, Type::Void, ValueKind::Void);
        effects.into_iter().rev().fold(void, |rest, effect| {
            self.value(
                e,
                Type::Void,
                ValueKind::Sequence {
                    left: Box::new(effect),
                    right: Box::new(rest),
                },
            )
        })
    }

    fn neon_shift_immediate(
        &mut self,
        e: &Expr,
        op: NeonOp,
        vector: Type,
        value: Value,
        amount: u32,
    ) -> Result<Value, ResolveError> {
        let Type::Vector {
            element: element @ NumericType::Integer { width, signed, .. },
            ..
        } = vector
        else {
            return Err(ResolveError::Internal("neon shift lane type"));
        };
        let value =
            self.context
                .vector_convert(value, vector.clone(), ConversionReason::Explicit)?;
        let right = op == NeonOp::ShiftRightImmediate;
        let amount = match amount >= width {
            true if right && signed => width - 1,
            true if right => {
                let zero = self.zero(e, &vector)?;
                return Ok(if super::effects::has_effects(&value) {
                    self.value(
                        e,
                        vector,
                        ValueKind::Sequence {
                            left: Box::new(value),
                            right: Box::new(zero),
                        },
                    )
                } else {
                    zero
                });
            }
            _ => amount,
        };
        let amount = self.value(
            e,
            Type::Numeric(element),
            ValueKind::Constant(Number::Integer(amount.into())),
        );
        let amount = conversion(
            amount,
            vector.clone(),
            ConversionKind::VectorSplat,
            ConversionReason::Explicit,
            ConversionSema::Exact,
        );
        let (op, semantics) = if right {
            (
                ArithOp::Shr,
                ArithSema::ShiftRight {
                    fill: if signed {
                        ShiftFill::SignExtend
                    } else {
                        ShiftFill::ZeroExtend
                    },
                    amount_out_of_range: UbPolicy::Undefined,
                },
            )
        } else {
            (
                ArithOp::Shl,
                ArithSema::ShiftLeft {
                    overflow: Overflow::Wrap,
                    amount_out_of_range: UbPolicy::Undefined,
                    negative_left: None,
                },
            )
        };
        Ok(self.value(
            e,
            vector,
            ValueKind::Arith {
                op,
                left: Box::new(value),
                right: Box::new(amount),
                semantics,
            },
        ))
    }

    fn neon_constant(&self, value: &Value) -> Option<u32> {
        super::fold::integer_constant(value, self.types.fold_target())
            .and_then(|value| u32::try_from(value & BigInt::from(u32::MAX)).ok())
    }

    fn neon_index(&mut self, e: &Expr, value: u32) -> Value {
        let int = self.types.ctypes.int();
        self.operand(e, int, ValueKind::Constant(Number::Integer(value.into())))
            .value
    }
}

fn convert_rounding(name: &str) -> Option<&'static str> {
    let rest = name.strip_prefix("vcvt")?;
    let (mode, rest) = rest.split_at_checked(1)?;
    let rest = rest.strip_prefix('q').unwrap_or(rest);
    if !matches!(rest, "_s32_v" | "_s64_v" | "_u32_v" | "_u64_v") {
        return None;
    }
    Some(match mode {
        "n" => "n",
        "m" => "m",
        "p" => "p",
        "a" => "a",
        _ => return None,
    })
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum NeonShape {
    Same,
    Float,
    Half,
    Double,
}

fn neon_vector(code: u32, shape: NeonShape) -> Option<(Type, Type)> {
    let unsigned = code & 0x10 != 0;
    let bits = if code & 0x20 != 0 { 128 } else { 64 };
    let poly = matches!(code & 0xf, 4..=6);
    let element = match code & 0xf {
        0 => NumericType::integer(8, !unsigned),
        1 => NumericType::integer(16, !unsigned),
        2 => NumericType::integer(32, !unsigned),
        3 => NumericType::integer(64, !unsigned),
        4 => NumericType::integer(8, false),
        5 => NumericType::integer(16, false),
        6 => NumericType::integer(64, false),
        9 => NumericType::Float(FloatType::F32),
        10 => NumericType::Float(FloatType::F64),
        _ => return None,
    };
    let width = match element {
        NumericType::Integer { width, .. } => width,
        NumericType::Float(FloatType::F32) => 32,
        NumericType::Float(_) => 64,
    };
    let lanes = bits / width;
    let result = Type::Vector { element, lanes };
    let signed = matches!(element, NumericType::Integer { signed: true, .. });
    let operand = match (shape, element) {
        (NeonShape::Same, _) => element,
        (NeonShape::Float, _) if width == 32 => NumericType::Float(FloatType::F32),
        (NeonShape::Float, _) if width == 64 => NumericType::Float(FloatType::F64),
        (NeonShape::Half, NumericType::Integer { .. }) if !poly && width >= 16 => {
            NumericType::integer(width / 2, signed)
        }
        (NeonShape::Double, NumericType::Integer { .. }) if width <= 32 && bits == 64 => {
            NumericType::integer(width * 2, signed)
        }
        _ => return None,
    };
    Some((
        Type::Vector {
            element: operand,
            lanes,
        },
        result,
    ))
}

fn element_bytes(element: NumericType) -> u64 {
    match element {
        NumericType::Integer { width, .. } => u64::from(width / 8),
        NumericType::Float(FloatType::F32) => 4,
        NumericType::Float(_) => 8,
    }
}

fn neon_pointer(pointer: Value, pointee: &Type) -> Value {
    conversion(
        pointer,
        Type::Pointer {
            pointee: Box::new(pointee.clone()),
            is_const: false,
            access: Access::default(),
            space: PointerSpace::Default,
        },
        ConversionKind::PointerCast,
        ConversionReason::Explicit,
        ConversionSema::Exact,
    )
}
