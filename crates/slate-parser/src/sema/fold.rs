use crate::compiler_args::CompilerFlavor;
use crate::ir::{
    ArithOp, ArithSema, CompareOp, ConversionKind, FloatType, LogicalOp, Number, NumericType,
    Overflow, ShiftFill, Type, UnaryArithOp, Value, ValueKind,
};
use num_bigint::{BigInt, BigUint, Sign};
use rustc_apfloat::{
    Float, FloatConvert, Status,
    ieee::{BFloat, Double, Half, Quad, Single, X87DoubleExtended},
};
use std::cmp::Ordering;

const MAX_WIDTH: u32 = 65_536;
const MAX_DEPTH: usize = 256;

/// Evaluate the executed, side-effect-free integer portion of typed IR.
/// This is not a check of C's syntactic integer-constant-expression rules.
pub(super) fn integer(value: &Value) -> Option<BigInt> {
    evaluate(value, MAX_DEPTH, None)
}

pub(super) fn integer_constant(value: &Value, flavor: CompilerFlavor) -> Option<BigInt> {
    evaluate(value, MAX_DEPTH, Some(flavor))
}

pub(super) fn fold_msvc_static_divisions(value: &mut Value) {
    let zero_division = match &mut value.node.value {
        ValueKind::Arith {
            op, left, right, ..
        } => {
            fold_msvc_static_divisions(left);
            fold_msvc_static_divisions(right);
            matches!(op, ArithOp::Div | ArithOp::Rem)
                && integer_constant(left, CompilerFlavor::Msvc).is_some()
                && integer_constant(right, CompilerFlavor::Msvc)
                    .is_some_and(|value| value == 0.into())
        }
        ValueKind::Convert { operand, .. }
        | ValueKind::Unary { operand, .. }
        | ValueKind::Copy { operand, .. } => {
            fold_msvc_static_divisions(operand);
            false
        }
        ValueKind::Compare { left, right, .. }
        | ValueKind::Logical { left, right, .. }
        | ValueKind::Sequence { left, right } => {
            fold_msvc_static_divisions(left);
            fold_msvc_static_divisions(right);
            false
        }
        ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            fold_msvc_static_divisions(condition);
            fold_msvc_static_divisions(then_value);
            fold_msvc_static_divisions(else_value);
            false
        }
        ValueKind::Aggregate { members, .. } => {
            for member in members {
                fold_msvc_static_divisions(&mut member.value);
            }
            false
        }
        _ => false,
    };
    if zero_division && let Type::Numeric(NumericType::Integer { signed, .. }) = value.ty {
        value.node.value = ValueKind::Constant(if signed {
            Number::SignedInteger(BigInt::from(0))
        } else {
            Number::Integer(BigUint::from(0u8))
        });
    }
}

fn evaluate(value: &Value, depth: usize, flavor: Option<CompilerFlavor>) -> Option<BigInt> {
    let depth = depth.checked_sub(1)?;
    let (width, signed) = integer_type(&value.ty)?;
    let result = match &value.node.value {
        ValueKind::Constant(Number::Bool(value)) => BigInt::from(u8::from(*value)),
        ValueKind::Constant(Number::Integer(value)) => {
            if value.bits() > u64::from(MAX_WIDTH) {
                return None;
            }
            BigInt::from(value.clone())
        }
        ValueKind::Constant(Number::SignedInteger(value)) => {
            if value.bits() > u64::from(MAX_WIDTH) {
                return None;
            }
            value.clone()
        }
        ValueKind::Convert {
            kind: ConversionKind::FloatToInt,
            operand,
            ..
        } => float_to_integer(floating(operand, depth, flavor)?, width, signed, flavor)?,
        ValueKind::Convert { kind, operand, .. } => {
            match kind {
                ConversionKind::Widen | ConversionKind::Truncate | ConversionKind::Reinterpret
                    if matches!(operand.ty, Type::Numeric(NumericType::Integer { .. })) => {}
                ConversionKind::FromBool if operand.ty == Type::Bool => {}
                _ => return None,
            }
            if !matches!(value.ty, Type::Numeric(NumericType::Integer { .. })) {
                return None;
            }
            evaluate(operand, depth, flavor)?
        }
        ValueKind::Unary {
            op,
            operand,
            semantics,
        } => {
            let operand = evaluate(operand, depth, flavor)?;
            match (op, semantics) {
                (UnaryArithOp::Not, ArithSema::Exact) if value.ty == Type::Bool => {
                    BigInt::from(u8::from(operand.sign() == Sign::NoSign))
                }
                (UnaryArithOp::Not, ArithSema::Exact) => !operand,
                (UnaryArithOp::Neg, ArithSema::Integer { overflow }) => {
                    arithmetic_result(-operand, width, signed, *overflow, flavor.is_some())?
                }
                _ => return None,
            }
        }
        ValueKind::Arith {
            op,
            left,
            right,
            semantics,
        } => {
            if !matches!(value.ty, Type::Numeric(NumericType::Integer { .. })) {
                return None;
            }
            let left = evaluate(left, depth, flavor)?;
            let right = evaluate(right, depth, flavor)?;
            arithmetic(*op, *semantics, left, right, width, signed, flavor)?
        }
        ValueKind::Compare {
            op, left, right, ..
        } if matches!(left.ty, Type::Numeric(NumericType::Float(_))) => {
            let order =
                floating(left, depth, flavor)?.partial_cmp(&floating(right, depth, flavor)?);
            BigInt::from(u8::from(match op {
                CompareOp::Eq => order == Some(Ordering::Equal),
                CompareOp::Ne => order != Some(Ordering::Equal),
                CompareOp::Lt => order == Some(Ordering::Less),
                CompareOp::Le => matches!(order, Some(Ordering::Less | Ordering::Equal)),
                CompareOp::Gt => order == Some(Ordering::Greater),
                CompareOp::Ge => matches!(order, Some(Ordering::Greater | Ordering::Equal)),
            }))
        }
        ValueKind::Compare {
            op, left, right, ..
        } => {
            let left = evaluate(left, depth, flavor)?;
            let right = evaluate(right, depth, flavor)?;
            BigInt::from(u8::from(match op {
                CompareOp::Eq => left == right,
                CompareOp::Ne => left != right,
                CompareOp::Lt => left < right,
                CompareOp::Le => left <= right,
                CompareOp::Gt => left > right,
                CompareOp::Ge => left >= right,
            }))
        }
        ValueKind::Logical { op, left, right } => {
            let left = evaluate(left, depth, flavor)?.sign() != Sign::NoSign;
            let result = match op {
                LogicalOp::And => left && evaluate(right, depth, flavor)?.sign() != Sign::NoSign,
                LogicalOp::Or => left || evaluate(right, depth, flavor)?.sign() != Sign::NoSign,
            };
            BigInt::from(u8::from(result))
        }
        ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            let condition = evaluate(condition, depth, flavor)?;
            evaluate(
                if condition.sign() != Sign::NoSign {
                    then_value
                } else {
                    else_value
                },
                depth,
                flavor,
            )?
        }
        ValueKind::Sequence { left, right } => {
            evaluate(left, depth, flavor)?;
            evaluate(right, depth, flavor)?
        }
        _ => return None,
    };
    Some(normalize(result, width, signed))
}

// quad holds every binary format exactly, so values travel as quad and round per operation
macro_rules! in_format {
    ($format:expr, $F:ident => $body:expr) => {
        match $format {
            FloatType::BF16 => {
                type $F = BFloat;
                $body
            }
            FloatType::F16 => {
                type $F = Half;
                $body
            }
            FloatType::F32 => {
                type $F = Single;
                $body
            }
            FloatType::F64 => {
                type $F = Double;
                $body
            }
            FloatType::F80 => {
                type $F = X87DoubleExtended;
                $body
            }
            FloatType::F128 => {
                type $F = Quad;
                $body
            }
            FloatType::D32 | FloatType::D64 | FloatType::D128 => None,
        }
    };
}

fn floating(value: &Value, depth: usize, flavor: Option<CompilerFlavor>) -> Option<Quad> {
    let depth = depth.checked_sub(1)?;
    let Type::Numeric(NumericType::Float(format)) = value.ty else {
        return None;
    };
    match &value.node.value {
        ValueKind::Constant(Number::FloatBits(bits)) => {
            in_format!(format, F => Some(widen(F::from_bits(*bits))))
        }
        ValueKind::Unary {
            op: UnaryArithOp::Neg,
            operand,
            ..
        } => Some(-floating(operand, depth, flavor)?),
        ValueKind::Arith {
            op, left, right, ..
        } => {
            let left = floating(left, depth, flavor)?;
            let right = floating(right, depth, flavor)?;
            in_format!(format, F => {
                let (left, right) = (narrow::<F>(left), narrow::<F>(right));
                let result = match op {
                    ArithOp::Add => left + right,
                    ArithOp::Sub => left - right,
                    ArithOp::Mul => left * right,
                    ArithOp::Div => left / right,
                    _ => return None,
                };
                Some(widen(result.value))
            })
        }
        ValueKind::Convert {
            kind: ConversionKind::IntToFloat,
            operand,
            ..
        } => {
            let integer = evaluate(operand, depth, flavor)?;
            in_format!(format, F => {
                let converted = match i128::try_from(&integer) {
                    Ok(integer) => F::from_i128(integer),
                    Err(_) => F::from_u128(u128::try_from(&integer).ok()?),
                };
                Some(widen(converted.value))
            })
        }
        ValueKind::Convert {
            kind:
                ConversionKind::FloatWiden | ConversionKind::FloatNarrow | ConversionKind::FloatConvert,
            operand,
            ..
        } => {
            let operand = floating(operand, depth, flavor)?;
            in_format!(format, F => Some(widen(narrow::<F>(operand))))
        }
        ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            let condition = evaluate(condition, depth, flavor)?;
            floating(
                if condition.sign() != Sign::NoSign {
                    then_value
                } else {
                    else_value
                },
                depth,
                flavor,
            )
        }
        _ => None,
    }
}

fn widen<F: FloatConvert<Quad>>(value: F) -> Quad {
    value.convert(&mut false).value
}

fn narrow<F: Float>(value: Quad) -> F
where
    Quad: FloatConvert<F>,
{
    value.convert(&mut false).value
}

fn float_to_integer(
    value: Quad,
    width: u32,
    signed: bool,
    flavor: Option<CompilerFlavor>,
) -> Option<BigInt> {
    // APFloat's integer conversion API is limited to 128 bits.
    if !(1..=128).contains(&width) {
        return None;
    }
    let limit = BigInt::from(1u8) << (width - u32::from(signed));
    let minimum = if signed { -&limit } else { BigInt::from(0u8) };
    match truncated(value, 128) {
        Some(result) if result >= minimum && result < limit => Some(result),
        _ => match flavor {
            // an out-of-range conversion is UB; clang folds it saturated and NaN to 0
            Some(CompilerFlavor::Clang) if value.is_nan() => Some(BigInt::from(0u8)),
            Some(CompilerFlavor::Clang) if value.is_negative() => Some(minimum),
            Some(CompilerFlavor::Clang) => Some(limit - 1),
            // cl wraps the integer part below 2^64 and folds anything larger, inf or NaN to 0
            Some(CompilerFlavor::Msvc) => Some(
                truncated(value, 64)
                    .map_or(BigInt::from(0u8), |result| normalize(result, width, signed)),
            ),
            _ => None,
        },
    }
}

// to_u128 truncates toward zero; INVALID_OP rejects infinities, NaNs and magnitudes of 2^bits or more
fn truncated(value: Quad, bits: usize) -> Option<BigInt> {
    let result = value.abs().to_u128(bits);
    if result.status.contains(Status::INVALID_OP) {
        return None;
    }
    let magnitude = BigInt::from(result.value);
    Some(if value.is_negative() {
        -magnitude
    } else {
        magnitude
    })
}

fn integer_type(ty: &Type) -> Option<(u32, bool)> {
    match ty {
        Type::Bool => Some((1, false)),
        Type::Numeric(NumericType::Integer { width, signed, .. })
            if (1..=MAX_WIDTH).contains(width) =>
        {
            Some((*width, *signed))
        }
        _ => None,
    }
}

fn arithmetic(
    op: ArithOp,
    semantics: ArithSema,
    left: BigInt,
    right: BigInt,
    width: u32,
    signed: bool,
    flavor: Option<CompilerFlavor>,
) -> Option<BigInt> {
    match (op, semantics) {
        (ArithOp::Add | ArithOp::Sub | ArithOp::Mul, ArithSema::Integer { overflow }) => {
            let result = match op {
                ArithOp::Add => left + right,
                ArithOp::Sub => left - right,
                ArithOp::Mul => left * right,
                _ => return None,
            };
            arithmetic_result(result, width, signed, overflow, flavor.is_some())
        }
        (ArithOp::Div | ArithOp::Rem, ArithSema::Division { .. }) => {
            if right.sign() == Sign::NoSign {
                return (flavor == Some(CompilerFlavor::Msvc)).then_some(BigInt::from(0));
            }
            if flavor.is_none()
                && signed
                && right == BigInt::from(-1)
                && left == -(BigInt::from(1u8) << (width - 1))
            {
                return None;
            }
            Some(if op == ArithOp::Div {
                left / right
            } else {
                left % right
            })
        }
        (ArithOp::And, ArithSema::Exact) => Some(left & right),
        (ArithOp::Or, ArithSema::Exact) => Some(left | right),
        (ArithOp::Xor, ArithSema::Exact) => Some(left ^ right),
        (ArithOp::Shl, ArithSema::ShiftLeft { overflow, .. }) => {
            let amount = match flavor {
                Some(CompilerFlavor::Clang) => clang_shift_amount(&right, width),
                Some(CompilerFlavor::Gcc) if right.sign() == Sign::Minus => {
                    return (left.sign() == Sign::NoSign).then_some(BigInt::from(0));
                }
                Some(_) if shift_amount(&right, width).is_none() => return Some(BigInt::from(0)),
                Some(_) | None => shift_amount(&right, width)?,
            };
            if flavor.is_none() && signed && left.sign() == Sign::Minus {
                return None;
            }
            let result = if flavor == Some(CompilerFlavor::Clang) && right.sign() == Sign::Minus {
                left >> amount
            } else {
                left << amount
            };
            arithmetic_result(result, width, signed, overflow, flavor.is_some())
        }
        (ArithOp::Shr, ArithSema::ShiftRight { fill, .. }) => {
            let amount = match flavor {
                Some(CompilerFlavor::Clang) => clang_shift_amount(&right, width),
                Some(_) if shift_amount(&right, width).is_none() => {
                    return Some(if signed && left.sign() == Sign::Minus {
                        BigInt::from(-1)
                    } else {
                        BigInt::from(0)
                    });
                }
                Some(_) | None => shift_amount(&right, width)?,
            };
            let left = normalize(left, width, fill == ShiftFill::SignExtend);
            Some(
                if flavor == Some(CompilerFlavor::Clang) && right.sign() == Sign::Minus {
                    left << amount
                } else {
                    left >> amount
                },
            )
        }
        _ => None,
    }
}

fn shift_amount(value: &BigInt, width: u32) -> Option<u32> {
    let amount = u32::try_from(value).ok()?;
    (amount < width && amount < MAX_WIDTH).then_some(amount)
}

fn clang_shift_amount(value: &BigInt, width: u32) -> u32 {
    let magnitude = if value.sign() == Sign::Minus {
        -value
    } else {
        value.clone()
    };
    u32::try_from(magnitude).unwrap_or(width - 1).min(width - 1)
}

fn arithmetic_result(
    value: BigInt,
    width: u32,
    signed: bool,
    overflow: Overflow,
    constant: bool,
) -> Option<BigInt> {
    if constant || !signed || overflow == Overflow::Wrap {
        return Some(normalize(value, width, signed));
    }
    let limit = BigInt::from(1u8) << (width - 1);
    if value < -&limit || value >= limit {
        None
    } else {
        Some(value)
    }
}

fn normalize(value: BigInt, width: u32, signed: bool) -> BigInt {
    let modulus = BigInt::from(1u8) << width;
    let mut value = value % &modulus;
    if value.sign() == Sign::Minus {
        value += &modulus;
    }
    if signed && value >= (&modulus >> 1u32) {
        value -= modulus;
    }
    value
}
