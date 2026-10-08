use super::builtins::BitBuiltin;
use crate::compiler_args::CompilerFlavor;
use crate::ir::{
    AggregateTarget, ArithOp, ArithSema, BindingId, CompareOp, ConversionKind, FloatType,
    LogicalOp, Number, NumericType, Overflow, Place, PlaceKind, ShiftFill, Type, UnaryArithOp,
    Value, ValueKind,
};
use num_bigint::{BigInt, BigUint, Sign};
use rustc_apfloat::{
    Float, FloatConvert, Status,
    ieee::{BFloat, Double, Half, Quad, Single, X87DoubleExtended},
};
use std::cmp::Ordering;
use std::collections::HashMap;

const MAX_WIDTH: u32 = 65_536;
const MAX_DEPTH: usize = 256;

/// Evaluate the executed, side-effect-free integer portion of typed IR.
/// This is not a check of C's syntactic integer-constant-expression rules.
pub(super) fn integer(value: &Value) -> Option<BigInt> {
    evaluate(value, MAX_DEPTH, Env::default())
}

#[derive(Clone, Copy)]
pub(super) struct Target {
    pub flavor: CompilerFlavor,
    pub pointer_width: u32,
}

pub(super) fn integer_constant(value: &Value, target: Target) -> Option<BigInt> {
    evaluate(value, MAX_DEPTH, Env::constant(target, None))
}

pub(super) fn integer_with_objects(
    value: &Value,
    target: Target,
    objects: &Objects,
) -> Option<BigInt> {
    evaluate(value, MAX_DEPTH, Env::constant(target, Some(objects)))
}

pub(super) fn read_with_objects(
    place: &Place,
    target: Target,
    objects: &Objects,
) -> Option<BigInt> {
    read(place, MAX_DEPTH, Env::constant(target, Some(objects)))
}

pub(super) fn bit_builtin(
    builtin: BitBuiltin,
    argument: &Value,
    fallback: Option<&Value>,
    target: Target,
) -> Option<BigInt> {
    if matches!(builtin, BitBuiltin::RotateLeft | BitBuiltin::RotateRight) {
        let (width, false) = integer_type(&argument.ty)? else {
            return None;
        };
        let bits = normalize(integer_constant(argument, target)?, width, false);
        let count = integer_constant(fallback?, target)?;
        if count.sign() == Sign::Minus && target.flavor.is_gcc() {
            return None;
        }
        let width_value = BigInt::from(width);
        let count = u32::try_from((count % &width_value + &width_value) % &width_value).ok()?;
        if count == 0 {
            return Some(bits);
        }
        let (left, right) = if builtin == BitBuiltin::RotateLeft {
            (count, width - count)
        } else {
            (width - count, count)
        };
        return Some(normalize(
            (bits.clone() << left) | (bits >> right),
            width,
            false,
        ));
    }
    let (width, _) = integer_type(&argument.ty).filter(|(width, _)| *width <= 128)?;
    let bits = u128::try_from(normalize(integer_constant(argument, target)?, width, false)).ok()?;
    let unused = 128 - width;
    let count = match builtin {
        BitBuiltin::Clz | BitBuiltin::Ctz if bits == 0 => {
            return match fallback {
                Some(fallback) => integer_constant(fallback, target),
                None if target.flavor.is_gcc() => Some(width.into()),
                None => None,
            };
        }
        BitBuiltin::Clz => bits.leading_zeros() - unused,
        BitBuiltin::Ctz => bits.trailing_zeros(),
        BitBuiltin::Popcount => bits.count_ones(),
        BitBuiltin::Parity => bits.count_ones() & 1,
        BitBuiltin::Ffs if bits == 0 => 0,
        BitBuiltin::Ffs => bits.trailing_zeros() + 1,
        BitBuiltin::Clrsb => {
            let mask = u128::MAX >> unused;
            let magnitude = if bits >> (width - 1) == 1 {
                !bits & mask
            } else {
                bits
            };
            magnitude.leading_zeros() - unused - 1
        }
        BitBuiltin::Bswap => return Some((bits.swap_bytes() >> unused).into()),
        BitBuiltin::Bitreverse => return Some((bits.reverse_bits() >> unused).into()),
        BitBuiltin::RotateLeft | BitBuiltin::RotateRight => return None,
    };
    Some(count.into())
}

pub(super) fn integer_number(ty: &Type, value: BigInt) -> Number {
    if *ty == Type::Bool {
        return Number::Bool(value.sign() != Sign::NoSign);
    }
    match value.to_biguint() {
        Some(value) => Number::Integer(value),
        None => Number::SignedInteger(value),
    }
}

pub(super) type Objects = HashMap<BindingId, Value>;

#[derive(Clone, Copy, Default)]
struct Env<'a> {
    flavor: Option<CompilerFlavor>,
    pointer_width: Option<u32>,
    objects: Option<&'a Objects>,
}

impl<'a> Env<'a> {
    fn constant(target: Target, objects: Option<&'a Objects>) -> Self {
        Self {
            flavor: Some(target.flavor),
            pointer_width: Some(target.pointer_width),
            objects,
        }
    }
}

enum Stored<'a> {
    Value(&'a Value),
    Integer(BigInt),
}

fn read(place: &Place, depth: usize, env: Env) -> Option<BigInt> {
    let (width, signed) = integer_type(&place.ty)?;
    let value = match stored(place, depth, env)? {
        Stored::Value(value) => evaluate(value, depth, env)?,
        Stored::Integer(value) => value,
    };
    Some(normalize(value, width, signed))
}

fn read_floating(place: &Place, depth: usize, env: Env) -> Option<Quad> {
    match stored(place, depth, env)? {
        Stored::Value(value) if value.ty == place.ty => floating(value, depth, env),
        Stored::Integer(value) if value.sign() == Sign::NoSign => Some(Quad::ZERO),
        _ => None,
    }
}

fn stored<'a>(place: &Place, depth: usize, env: Env<'a>) -> Option<Stored<'a>> {
    let depth = depth.checked_sub(1)?;
    match &place.kind {
        PlaceKind::Binding(binding) => {
            let value = env.objects?.get(binding)?;
            Some(match &value.node.value {
                ValueKind::Aggregate {
                    members,
                    zero_fill: true,
                } if members.is_empty() => Stored::Integer(BigInt::from(0u8)),
                _ => Stored::Value(value),
            })
        }
        PlaceKind::Field { base, index, bits } => {
            let found = member(stored(base, depth, env)?, AggregateTarget::Field(*index))?;
            let Some(bits) = bits else {
                return Some(found);
            };
            let (_, signed) = integer_type(&place.ty)?;
            let value = match found {
                Stored::Value(value) => evaluate(value, depth, env)?,
                Stored::Integer(value) => value,
            };
            Some(Stored::Integer(normalize(value, bits.width, signed)))
        }
        PlaceKind::Index { base, index } => {
            let index = evaluate(index, depth, env)?;
            element(base, index, depth, env)
        }
        PlaceKind::Deref(pointer) => element(pointer, BigInt::from(0u8), depth, env),
        _ => None,
    }
}

fn element<'a>(pointer: &Value, index: BigInt, depth: usize, env: Env<'a>) -> Option<Stored<'a>> {
    let depth = depth.checked_sub(1)?;
    match &pointer.node.value {
        ValueKind::ArrayDecay { place, .. } => {
            let index = u64::try_from(index).ok()?;
            member(stored(place, depth, env)?, AggregateTarget::Index(index))
        }
        ValueKind::AddressOf(place) if index.sign() == Sign::NoSign => stored(place, depth, env),
        ValueKind::PointerOffset {
            pointer,
            amount,
            subtract,
            ..
        } => {
            let amount = evaluate(amount, depth, env)?;
            let index = if *subtract {
                index - amount
            } else {
                index + amount
            };
            element(pointer, index, depth, env)
        }
        ValueKind::Read {
            place,
            ordering: None,
        } => match stored(place, depth, env)? {
            Stored::Value(pointer) => element(pointer, index, depth, env),
            Stored::Integer(_) => None,
        },
        _ => None,
    }
}

fn member(aggregate: Stored<'_>, target: AggregateTarget) -> Option<Stored<'_>> {
    let aggregate = match aggregate {
        Stored::Integer(value) => return Some(Stored::Integer(value)),
        Stored::Value(value) => value,
    };
    match &aggregate.node.value {
        ValueKind::Aggregate { members, zero_fill } => {
            let found = members
                .iter()
                .rev()
                .find(|member| match (member.target, target) {
                    (AggregateTarget::Range { start, end }, AggregateTarget::Index(index)) => {
                        (start..=end).contains(&index)
                    }
                    (found, target) => found == target,
                });
            match found {
                Some(member) => Some(Stored::Value(&member.value)),
                None => zero_fill.then(|| Stored::Integer(BigInt::from(0u8))),
            }
        }
        ValueKind::CodeUnits(units) => {
            let AggregateTarget::Index(index) = target else {
                return None;
            };
            let unit = units.get(usize::try_from(index).ok()?)?;
            Some(Stored::Integer(BigInt::from(*unit)))
        }
        _ => None,
    }
}

pub(super) fn fold_msvc_static_divisions(value: &mut Value) {
    let zero_division = match &mut value.node.value {
        ValueKind::Arith {
            op, left, right, ..
        } => {
            fold_msvc_static_divisions(left);
            fold_msvc_static_divisions(right);
            let msvc = Env {
                flavor: Some(CompilerFlavor::Msvc),
                ..Env::default()
            };
            matches!(op, ArithOp::Div | ArithOp::Rem)
                && evaluate(left, MAX_DEPTH, msvc).is_some()
                && evaluate(right, MAX_DEPTH, msvc).is_some_and(|value| value == 0.into())
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

fn evaluate(value: &Value, depth: usize, env: Env) -> Option<BigInt> {
    let depth = depth.checked_sub(1)?;
    if matches!(value.ty, Type::Defined(_)) {
        return enumerated(value, depth, env);
    }
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
        } => float_to_integer(floating(operand, depth, env)?, width, signed, env.flavor)?,
        ValueKind::Convert {
            kind: ConversionKind::EnumToInt,
            operand,
            ..
        } => enumerated(operand, depth, env)?,
        ValueKind::Convert {
            kind: ConversionKind::PtrToInt,
            operand,
            ..
        } => address(operand, depth, env)?,
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
            evaluate(operand, depth, env)?
        }
        ValueKind::Unary {
            op,
            operand,
            semantics,
        } => {
            let operand = evaluate(operand, depth, env)?;
            match (op, semantics) {
                (UnaryArithOp::Not, ArithSema::Exact) if value.ty == Type::Bool => {
                    BigInt::from(u8::from(operand.sign() == Sign::NoSign))
                }
                (UnaryArithOp::Not, ArithSema::Exact) => !operand,
                (UnaryArithOp::Neg, ArithSema::Integer { overflow }) => {
                    arithmetic_result(-operand, width, signed, *overflow, env.flavor.is_some())?
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
            let left = evaluate(left, depth, env)?;
            let right = evaluate(right, depth, env)?;
            arithmetic(*op, *semantics, left, right, width, signed, env.flavor)?
        }
        ValueKind::Compare {
            op, left, right, ..
        } if matches!(left.ty, Type::Numeric(NumericType::Float(_))) => {
            let order = floating(left, depth, env)?.partial_cmp(&floating(right, depth, env)?);
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
            let (left, right) = if matches!(left.ty, Type::Pointer { .. }) {
                (address(left, depth, env)?, address(right, depth, env)?)
            } else {
                (evaluate(left, depth, env)?, evaluate(right, depth, env)?)
            };
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
            let left = evaluate(left, depth, env)?.sign() != Sign::NoSign;
            let result = match op {
                LogicalOp::And => left && evaluate(right, depth, env)?.sign() != Sign::NoSign,
                LogicalOp::Or => left || evaluate(right, depth, env)?.sign() != Sign::NoSign,
            };
            BigInt::from(u8::from(result))
        }
        ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            let condition = evaluate(condition, depth, env)?;
            evaluate(
                if condition.sign() != Sign::NoSign {
                    then_value
                } else {
                    else_value
                },
                depth,
                env,
            )?
        }
        ValueKind::Sequence { left, right } => {
            evaluate(left, depth, env)?;
            evaluate(right, depth, env)?
        }
        ValueKind::Read {
            place,
            ordering: None,
        } if env.objects.is_some() => read(place, depth, env)?,
        _ => return None,
    };
    Some(normalize(result, width, signed))
}

fn address(value: &Value, depth: usize, env: Env) -> Option<BigInt> {
    let depth = depth.checked_sub(1)?;
    let width = env.pointer_width?;
    match &value.node.value {
        ValueKind::Null => Some(BigInt::from(0u8)),
        ValueKind::Convert {
            kind: ConversionKind::IntToPtr,
            operand,
            ..
        } => Some(normalize(evaluate(operand, depth, env)?, width, false)),
        ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } => address(operand, depth, env),
        _ => None,
    }
}

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

fn floating(value: &Value, depth: usize, env: Env) -> Option<Quad> {
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
        } => Some(-floating(operand, depth, env)?),
        ValueKind::Arith {
            op, left, right, ..
        } => {
            let left = floating(left, depth, env)?;
            let right = floating(right, depth, env)?;
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
            let integer = evaluate(operand, depth, env)?;
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
            let operand = floating(operand, depth, env)?;
            in_format!(format, F => Some(widen(narrow::<F>(operand))))
        }
        ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            let condition = evaluate(condition, depth, env)?;
            floating(
                if condition.sign() != Sign::NoSign {
                    then_value
                } else {
                    else_value
                },
                depth,
                env,
            )
        }
        ValueKind::Read {
            place,
            ordering: None,
        } if env.objects.is_some() => read_floating(place, depth, env),
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

fn enumerated(value: &Value, depth: usize, env: Env) -> Option<BigInt> {
    match &value.node.value {
        ValueKind::Constant(Number::Integer(value)) => Some(BigInt::from(value.clone())),
        ValueKind::Constant(Number::SignedInteger(value)) => Some(value.clone()),
        ValueKind::Constant(Number::Bool(value)) => Some(BigInt::from(u8::from(*value))),
        ValueKind::Convert {
            kind: ConversionKind::IntToEnum,
            operand,
            ..
        } => evaluate(operand, depth, env),
        _ => None,
    }
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
