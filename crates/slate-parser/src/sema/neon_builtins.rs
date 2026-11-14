use super::expression::Lowerer;
use super::numeric::ResolveError;
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
        _ => return None,
    })
}

impl Lowerer {
    pub(super) fn expand_neon(
        &mut self,
        e: &Expr,
        op: NeonOp,
        (returned, ty, arguments): LoweredCall,
    ) -> Result<Result<Operand, LoweredCall>, ResolveError> {
        let result = self.types.ir_type(returned);
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
            _ => {
                let Some((code, operands)) = arguments.split_last() else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let Some(shape) = self
                    .neon_constant(code)
                    .and_then(|code| neon_vector(code, op == NeonOp::AbsGreater))
                else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let operands = operands.to_vec();
                if op == NeonOp::BitSelect && operands.iter().any(super::effects::has_effects) {
                    return Ok(Err((returned, ty, arguments)));
                }
                let typed = self.neon_lanewise(e, op, shape, operands)?;
                self.context
                    .vector_convert(typed, result, ConversionReason::Explicit)?
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
        let float = matches!(element, NumericType::Float(_));
        let signed = matches!(element, NumericType::Integer { signed: true, .. });
        let name = match op {
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

fn neon_vector(code: u32, float_operands: bool) -> Option<(Type, Type)> {
    let unsigned = code & 0x10 != 0;
    let bits = if code & 0x20 != 0 { 128 } else { 64 };
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
    let operand = match (float_operands, width) {
        (false, _) => result.clone(),
        (true, 32) => Type::Vector {
            element: NumericType::Float(FloatType::F32),
            lanes,
        },
        (true, 64) => Type::Vector {
            element: NumericType::Float(FloatType::F64),
            lanes,
        },
        (true, _) => return None,
    };
    Some((operand, result))
}
