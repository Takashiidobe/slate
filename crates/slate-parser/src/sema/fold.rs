use crate::ir::{
    ArithOp, ArithSema, CompareOp, ConversionKind, FloatType, LogicalOp, Number, NumericType,
    Overflow, ShiftFill, Type, UnaryArithOp, Value, ValueKind,
};
use num_bigint::{BigInt, Sign};
use rustc_apfloat::{
    Float, Status,
    ieee::{Double, Half, Quad, Single, X87DoubleExtended},
};

const MAX_WIDTH: u32 = 65_536;
const MAX_DEPTH: usize = 256;

/// Evaluate the executed, side-effect-free integer portion of typed IR.
/// This is not a check of C's syntactic integer-constant-expression rules.
pub(super) fn integer(value: &Value) -> Option<BigInt> {
    evaluate(value, MAX_DEPTH)
}

fn evaluate(value: &Value, depth: usize) -> Option<BigInt> {
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
        } => float_to_integer(operand, width, signed)?,
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
            evaluate(operand, depth)?
        }
        ValueKind::Unary {
            op,
            operand,
            semantics,
        } => {
            let operand = evaluate(operand, depth)?;
            match (op, semantics) {
                (UnaryArithOp::Not, ArithSema::Exact) if value.ty == Type::Bool => {
                    BigInt::from(u8::from(operand.sign() == Sign::NoSign))
                }
                (UnaryArithOp::Not, ArithSema::Exact) => !operand,
                (UnaryArithOp::Neg, ArithSema::Integer { overflow }) => {
                    arithmetic_result(-operand, width, signed, *overflow)?
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
            let left = evaluate(left, depth)?;
            let right = evaluate(right, depth)?;
            arithmetic(*op, *semantics, left, right, width, signed)?
        }
        ValueKind::Compare {
            op, left, right, ..
        } => {
            let left = evaluate(left, depth)?;
            let right = evaluate(right, depth)?;
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
            let left = evaluate(left, depth)?.sign() != Sign::NoSign;
            let result = match op {
                LogicalOp::And => left && evaluate(right, depth)?.sign() != Sign::NoSign,
                LogicalOp::Or => left || evaluate(right, depth)?.sign() != Sign::NoSign,
            };
            BigInt::from(u8::from(result))
        }
        ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            let condition = evaluate(condition, depth)?;
            evaluate(
                if condition.sign() != Sign::NoSign {
                    then_value
                } else {
                    else_value
                },
                depth,
            )?
        }
        ValueKind::Sequence { left, right } => {
            evaluate(left, depth)?;
            evaluate(right, depth)?
        }
        _ => return None,
    };
    Some(normalize(result, width, signed))
}

/// Only immediate floating constants (optionally negated) participate here;
/// this deliberately does not evaluate floating arithmetic or conversions.
fn float_to_integer(value: &Value, width: u32, signed: bool) -> Option<BigInt> {
    let Type::Numeric(NumericType::Float(format)) = value.ty else {
        return None;
    };
    let (constant, negate) = match &value.node.value {
        ValueKind::Unary {
            op: UnaryArithOp::Neg,
            operand,
            semantics: ArithSema::Exact,
        } if operand.ty == value.ty => (operand.as_ref(), true),
        _ => (value, false),
    };
    let ValueKind::Constant(Number::FloatBits(bits)) = constant.node.value else {
        return None;
    };
    // The IR format already incorporates the target's long-double selection.
    match format {
        FloatType::F16 => convert_float::<Half>(bits, negate, width, signed),
        FloatType::F32 => convert_float::<Single>(bits, negate, width, signed),
        FloatType::F64 => convert_float::<Double>(bits, negate, width, signed),
        FloatType::F80 => convert_float::<X87DoubleExtended>(bits, negate, width, signed),
        FloatType::F128 => convert_float::<Quad>(bits, negate, width, signed),
    }
}

fn convert_float<F: Float>(bits: u128, negate: bool, width: u32, signed: bool) -> Option<BigInt> {
    // APFloat's integer conversion API is limited to 128 bits.
    if !(1..=128).contains(&width) {
        return None;
    }
    let value = F::from_bits(bits);
    let value = if negate { -value } else { value };
    // to_u128 truncates toward zero. INEXACT is expected for fractions;
    // INVALID_OP rejects infinities, NaNs, and unrepresentable magnitudes.
    let result = value.abs().to_u128(width as usize);
    if result.status.contains(Status::INVALID_OP) {
        return None;
    }
    let magnitude = BigInt::from(result.value);
    let result = if value.is_negative() {
        -magnitude
    } else {
        magnitude
    };
    let limit = BigInt::from(1u8) << (width - u32::from(signed));
    let minimum = if signed { -&limit } else { BigInt::from(0u8) };
    (result >= minimum && result < limit).then_some(result)
}

fn integer_type(ty: &Type) -> Option<(u32, bool)> {
    match ty {
        Type::Bool => Some((1, false)),
        Type::Numeric(NumericType::Integer { width, signed })
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
) -> Option<BigInt> {
    match (op, semantics) {
        (ArithOp::Add | ArithOp::Sub | ArithOp::Mul, ArithSema::Integer { overflow }) => {
            let result = match op {
                ArithOp::Add => left + right,
                ArithOp::Sub => left - right,
                ArithOp::Mul => left * right,
                _ => return None,
            };
            arithmetic_result(result, width, signed, overflow)
        }
        (ArithOp::Div | ArithOp::Rem, ArithSema::Division { .. }) => {
            if right.sign() == Sign::NoSign
                || (signed
                    && right == BigInt::from(-1)
                    && left == -(BigInt::from(1u8) << (width - 1)))
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
            let amount = shift_amount(&right, width)?;
            if signed && left.sign() == Sign::Minus {
                return None;
            }
            arithmetic_result(left << amount, width, signed, overflow)
        }
        (ArithOp::Shr, ArithSema::ShiftRight { fill, .. }) => {
            let amount = shift_amount(&right, width)?;
            let left = normalize(left, width, fill == ShiftFill::SignExtend);
            Some(left >> amount)
        }
        _ => None,
    }
}

fn shift_amount(value: &BigInt, width: u32) -> Option<u32> {
    let amount = u32::try_from(value).ok()?;
    (amount < width && amount < MAX_WIDTH).then_some(amount)
}

fn arithmetic_result(
    value: BigInt,
    width: u32,
    signed: bool,
    overflow: Overflow,
) -> Option<BigInt> {
    if !signed || overflow == Overflow::Wrap {
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
