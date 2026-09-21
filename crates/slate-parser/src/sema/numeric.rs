use super::validate::{
    bit_int_literal_width, fits_rank, integer_rank_width, select_integer_candidate,
};
use crate::ast::{Expr, ExprKind};
use crate::const_expr::{
    BinaryOp, FloatLiteral, FloatSuffix, FloatValue, IntegerSizeSuffix, ResolvedFloat, UnaryOp,
    resolve_float,
};
use crate::ir::{
    AggregateMember, AggregateTarget, ArithOp, ArithSema, CompareOp, ConversionKind,
    ConversionReason, ConversionSema, Exceptions, Fits, FixedOverflow, FixedRounding, FloatType,
    FloatingSemantics, LogicalOp, Number, NumericType, Overflow, Rounding, ShiftFill, Type,
    UbPolicy, UnaryArithOp, Value, ValueKind,
};
use crate::standard_features::StandardFeatures;
use crate::target_info::TargetInfo;
use num_bigint::BigUint;
use thiserror::Error;

#[derive(Debug, Error)]
pub enum ResolveError {
    #[error("unsupported in numeric IR lowering: {0}")]
    Unsupported(&'static str),
    #[error("invalid in this context: {0}")]
    Invalid(&'static str),
    #[error("integer literal `{0}` has no supported target type")]
    IntegerLiteral(String),
    #[error("missing expression binding for `{0}`")]
    MissingExpressionBinding(String),
    #[error("unsupported Clang builtin `{0}`")]
    UnsupportedBuiltin(String),
    #[error("invalid operands to binary expression: {left} {operator} {right}")]
    InvalidOperands {
        left: NumericType,
        operator: &'static str,
        right: NumericType,
    },
    #[error("invalid argument type to unary expression: {operator}{operand}")]
    InvalidOperand {
        operator: &'static str,
        operand: NumericType,
    },
    #[error(transparent)]
    Names(#[from] super::names::ResolveError),
    #[error(transparent)]
    Literal(#[from] crate::const_expr::ConstExprError),
    #[error(transparent)]
    Layout(#[from] crate::target_info::LayoutError),
}

const UNSUPPORTED_EXPRESSION: &str = "expression (expected a number or arithmetic operator)";

pub struct Context {
    pub target: TargetInfo,
    pub features: StandardFeatures,
    pub signed_overflow: Overflow,
    pub pointer_wrap: bool,
    pub floating: FloatingSemantics,
}

type Resolved = (Type, ValueKind);

impl Context {
    pub fn with_options(mut self, options: &crate::compiler_options::CompilerOptions) -> Self {
        self.target = options.effective_target(self.target);
        self.signed_overflow = options.operations.signed_overflow;
        self.pointer_wrap = options.operations.pointer_wrap;
        self.floating = options.operations.floating;
        self
    }
    pub fn with_features(mut self, features: StandardFeatures) -> Self {
        self.features = features;
        self
    }

    pub fn new(target: TargetInfo) -> Self {
        Self {
            target,
            features: StandardFeatures::default(),
            signed_overflow: Overflow::Undefined,
            pointer_wrap: false,
            floating: FloatingSemantics {
                rounding: Rounding::NearestEven,
                exceptions: Exceptions::Ignore,
            },
        }
    }

    pub(super) fn resolve_literal(&self, expression: &Expr) -> Result<Value, ResolveError> {
        let (ty, kind) = match &expression.value {
            ExprKind::IntegerLiteral(literal) => {
                let bit_precise = literal.suffix.size == IntegerSizeSuffix::BitInt;
                let (width, signed) = if bit_precise {
                    bit_int_literal_width(literal)
                        .map(|width| (width, !literal.suffix.unsigned))
                        .ok_or_else(|| ResolveError::IntegerLiteral(literal.spelling.clone()))?
                } else {
                    select_integer_candidate(literal, &self.target, self.features)
                        .map(|(rank, signed)| (integer_rank_width(rank, &self.target), signed))
                        .ok_or_else(|| ResolveError::IntegerLiteral(literal.spelling.clone()))?
                };
                let numeric = NumericType::Integer {
                    width,
                    signed,
                    bit_precise,
                };
                let number = Number::Integer(literal.value.clone());
                if literal.imaginary {
                    imaginary_literal(
                        expression,
                        numeric,
                        Number::Integer(BigUint::default()),
                        number,
                    )
                } else {
                    (Type::Numeric(numeric), ValueKind::Constant(number))
                }
            }
            ExprKind::FloatLiteral(literal) => {
                if literal.suffix == FloatSuffix::F64x {
                    return Err(ResolveError::Unsupported("target-dependent f64x literals"));
                }
                let (format, number) = match resolve_float_literal(literal, &self.target)?.value {
                    FloatValue::Half(bits) => (FloatType::F16, Number::FloatBits(u128::from(bits))),
                    FloatValue::Single(value) => (
                        FloatType::F32,
                        Number::FloatBits(u128::from(value.to_bits())),
                    ),
                    FloatValue::Double(value) => (
                        FloatType::F64,
                        Number::FloatBits(u128::from(value.to_bits())),
                    ),
                    FloatValue::Quad(bits) => (FloatType::F128, Number::FloatBits(bits)),
                    FloatValue::LongDouble(bits) => (FloatType::F80, Number::FloatBits(bits)),
                    FloatValue::Decimal32(digits) => (FloatType::D32, Number::DecimalFloat(digits)),
                    FloatValue::Decimal64(digits) => (FloatType::D64, Number::DecimalFloat(digits)),
                    FloatValue::Decimal128(digits) => {
                        (FloatType::D128, Number::DecimalFloat(digits))
                    }
                };
                if literal.imaginary {
                    imaginary_literal(
                        expression,
                        NumericType::Float(format),
                        Number::float_zero(format),
                        number,
                    )
                } else {
                    (
                        Type::Numeric(NumericType::Float(format)),
                        ValueKind::Constant(number),
                    )
                }
            }
            ExprKind::BoolLiteral(value) => (Type::Bool, ValueKind::Constant(Number::Bool(*value))),
            _ => return Err(ResolveError::Unsupported(UNSUPPORTED_EXPRESSION)),
        };
        Ok(Value {
            ty,
            node: expression.derive(kind),
        })
    }

    pub(super) fn emit_binary(
        &self,
        op: BinaryOp,
        left: Value,
        right: Value,
    ) -> Result<Resolved, ResolveError> {
        let operator = <&'static str>::from(op);
        if matches!(left.ty, Type::Vector { .. }) || matches!(right.ty, Type::Vector { .. }) {
            return self.vector_binary(op, left, right);
        }
        if (matches!(left.ty, Type::FixedPoint(_)) || matches!(right.ty, Type::FixedPoint(_)))
            && !matches!(op, BinaryOp::And | BinaryOp::Or)
        {
            return self.fixed_binary(op, left, right);
        }
        let arith = match op {
            BinaryOp::Add => ArithOp::Add,
            BinaryOp::Sub => ArithOp::Sub,
            BinaryOp::Mul => ArithOp::Mul,
            BinaryOp::Div => ArithOp::Div,
            BinaryOp::Rem => ArithOp::Rem,
            BinaryOp::BitAnd => ArithOp::And,
            BinaryOp::BitOr => ArithOp::Or,
            BinaryOp::BitXor => ArithOp::Xor,
            BinaryOp::ShiftLeft => ArithOp::Shl,
            BinaryOp::ShiftRight => ArithOp::Shr,
            BinaryOp::Equal | BinaryOp::NotEqual if has_imaginary(&left, &right) => {
                return self.imaginary_binary(op, left, right);
            }
            BinaryOp::Equal | BinaryOp::NotEqual
                if matches!(left.ty, Type::Complex(_)) || matches!(right.ty, Type::Complex(_)) =>
            {
                return self.complex_binary(op, left, right);
            }
            BinaryOp::Equal => return self.compare(CompareOp::Eq, left, right),
            BinaryOp::NotEqual => return self.compare(CompareOp::Ne, left, right),
            BinaryOp::Less => return self.compare(CompareOp::Lt, left, right),
            BinaryOp::LessEqual => return self.compare(CompareOp::Le, left, right),
            BinaryOp::Greater => return self.compare(CompareOp::Gt, left, right),
            BinaryOp::GreaterEqual => return self.compare(CompareOp::Ge, left, right),
            BinaryOp::And => return Ok(self.logical(LogicalOp::And, left, right)),
            BinaryOp::Or => return Ok(self.logical(LogicalOp::Or, left, right)),
        };
        if has_imaginary(&left, &right) {
            return self.imaginary_binary(op, left, right);
        }
        if matches!(left.ty, Type::Complex(_)) || matches!(right.ty, Type::Complex(_)) {
            return self.complex_binary(op, left, right);
        }
        reject_mixed_decimal(operator, &left, &right)?;
        let left_ty = numeric(&left)?;
        let right_ty = numeric(&right)?;
        let invalid = ResolveError::InvalidOperands {
            left: left_ty,
            operator,
            right: right_ty,
        };
        let semantics = match (left_ty, arith) {
            (NumericType::Float(_), ArithOp::Add | ArithOp::Sub | ArithOp::Mul | ArithOp::Div) => {
                ArithSema::Floating(self.floating)
            }
            (NumericType::Float(_), _)
            | (
                NumericType::Integer { .. },
                ArithOp::MinNum
                | ArithOp::MaxNum
                | ArithOp::Minimum
                | ArithOp::Maximum
                | ArithOp::MinimumNum
                | ArithOp::MaximumNum,
            ) => return Err(invalid),
            (_, ArithOp::Shl | ArithOp::Shr) if matches!(right_ty, NumericType::Float(_)) => {
                return Err(invalid);
            }
            (NumericType::Integer { signed, .. }, ArithOp::Add | ArithOp::Sub | ArithOp::Mul) => {
                ArithSema::Integer {
                    overflow: if signed {
                        self.signed_overflow
                    } else {
                        Overflow::Wrap
                    },
                }
            }
            (NumericType::Integer { signed, .. }, ArithOp::Div | ArithOp::Rem) => {
                ArithSema::Division {
                    by_zero: UbPolicy::Undefined,
                    min_by_neg_one: signed.then_some(UbPolicy::Undefined),
                }
            }
            (NumericType::Integer { signed, .. }, ArithOp::Shl) => ArithSema::ShiftLeft {
                overflow: if signed {
                    Overflow::Undefined
                } else {
                    Overflow::Wrap
                },
                amount_out_of_range: UbPolicy::Undefined,
                negative_left: signed.then_some(UbPolicy::Undefined),
            },
            (NumericType::Integer { .. }, ArithOp::And | ArithOp::Or | ArithOp::Xor) => {
                ArithSema::Exact
            }
            (NumericType::Integer { signed, .. }, ArithOp::Shr) => ArithSema::ShiftRight {
                fill: if signed {
                    self.target.signed_right_shift
                } else {
                    ShiftFill::ZeroExtend
                },
                amount_out_of_range: UbPolicy::Undefined,
            },
        };
        Ok((
            left.ty.clone(),
            ValueKind::Arith {
                op: arith,
                left: Box::new(left),
                right: Box::new(right),
                semantics,
            },
        ))
    }

    pub(super) fn emit_unary_arith(
        &self,
        op: UnaryOp,
        operand: Value,
    ) -> Result<Resolved, ResolveError> {
        let arith = match op {
            UnaryOp::Minus => UnaryArithOp::Neg,
            _ => UnaryArithOp::Not,
        };
        if let Type::Vector { element, .. } = operand.ty {
            if arith == UnaryArithOp::Not && matches!(element, NumericType::Float(_)) {
                return Err(ResolveError::Invalid(
                    "bitwise complement of a floating vector",
                ));
            }
            let semantics = match (element, arith) {
                (NumericType::Float(_), _) | (NumericType::Integer { .. }, UnaryArithOp::Not) => {
                    ArithSema::Exact
                }
                (NumericType::Integer { .. }, UnaryArithOp::Neg) => ArithSema::Integer {
                    overflow: Overflow::Wrap,
                },
            };
            return Ok((
                operand.ty.clone(),
                ValueKind::Unary {
                    op: arith,
                    operand: Box::new(operand),
                    semantics,
                },
            ));
        }
        if let Type::FixedPoint(fixed) = operand.ty {
            if arith != UnaryArithOp::Neg {
                return Err(ResolveError::Invalid(
                    "bitwise complement of a fixed-point operand",
                ));
            }
            let semantics = self.fixed_semantics(fixed, None, None);
            return Ok((
                operand.ty.clone(),
                ValueKind::Unary {
                    op: arith,
                    operand: Box::new(operand),
                    semantics,
                },
            ));
        }
        if let Type::Imaginary(_) = operand.ty {
            if arith != UnaryArithOp::Neg {
                return Err(ResolveError::Invalid(
                    "bitwise complement of imaginary operand",
                ));
            }
            return Ok((
                operand.ty.clone(),
                ValueKind::Unary {
                    op: arith,
                    operand: Box::new(operand),
                    semantics: ArithSema::Exact,
                },
            ));
        }
        if let Type::Complex(component) = operand.ty {
            let semantics = match component {
                NumericType::Float(_) => ArithSema::ComplexFloating(self.floating),
                NumericType::Integer { signed, .. } => ArithSema::ComplexInteger {
                    overflow: if signed {
                        self.signed_overflow
                    } else {
                        Overflow::Wrap
                    },
                    by_zero: None,
                },
            };
            return Ok((
                operand.ty.clone(),
                ValueKind::Unary {
                    op: arith,
                    operand: Box::new(operand),
                    semantics,
                },
            ));
        }
        let semantics = match (numeric(&operand)?, arith) {
            (NumericType::Float(_), UnaryArithOp::Neg) => ArithSema::Exact,
            (ty @ NumericType::Float(_), UnaryArithOp::Not) => {
                return Err(ResolveError::InvalidOperand {
                    operator: <&'static str>::from(op),
                    operand: ty,
                });
            }
            (NumericType::Integer { signed, .. }, UnaryArithOp::Neg) => ArithSema::Integer {
                overflow: if signed {
                    self.signed_overflow
                } else {
                    Overflow::Wrap
                },
            },
            (NumericType::Integer { .. }, UnaryArithOp::Not) => ArithSema::Exact,
        };
        Ok((
            operand.ty.clone(),
            ValueKind::Unary {
                op: arith,
                operand: Box::new(operand),
                semantics,
            },
        ))
    }

    fn vector_binary(
        &self,
        op: BinaryOp,
        left: Value,
        right: Value,
    ) -> Result<Resolved, ResolveError> {
        let (left, right) = self.vector_operands(left, right)?;
        let Type::Vector { element, lanes } = left.ty else {
            return Err(ResolveError::Unsupported("vector arithmetic conversion"));
        };
        let compare = match op {
            BinaryOp::Equal => Some(CompareOp::Eq),
            BinaryOp::NotEqual => Some(CompareOp::Ne),
            BinaryOp::Less => Some(CompareOp::Lt),
            BinaryOp::LessEqual => Some(CompareOp::Le),
            BinaryOp::Greater => Some(CompareOp::Gt),
            BinaryOp::GreaterEqual => Some(CompareOp::Ge),
            _ => None,
        };
        if let Some(compare) = compare {
            let width = self.element_bits(element)?;
            return Ok((
                Type::Vector {
                    element: NumericType::integer(width, true),
                    lanes,
                },
                ValueKind::Compare {
                    op: compare,
                    left: Box::new(left),
                    right: Box::new(right),
                    exceptions: matches!(element, NumericType::Float(_))
                        .then_some(self.floating.exceptions),
                    reason: None,
                },
            ));
        }
        let arith = match op {
            BinaryOp::Add => ArithOp::Add,
            BinaryOp::Sub => ArithOp::Sub,
            BinaryOp::Mul => ArithOp::Mul,
            BinaryOp::Div => ArithOp::Div,
            BinaryOp::Rem => ArithOp::Rem,
            BinaryOp::BitAnd => ArithOp::And,
            BinaryOp::BitOr => ArithOp::Or,
            BinaryOp::BitXor => ArithOp::Xor,
            BinaryOp::ShiftLeft => ArithOp::Shl,
            BinaryOp::ShiftRight => ArithOp::Shr,
            _ => return Err(ResolveError::Unsupported("vector operator")),
        };
        // clang emits no nsw for vector arithmetic, so signed lanes wrap
        let semantics = match (element, arith) {
            (NumericType::Float(_), ArithOp::Add | ArithOp::Sub | ArithOp::Mul | ArithOp::Div) => {
                ArithSema::Floating(self.floating)
            }
            (NumericType::Float(_), _) => {
                return Err(ResolveError::Invalid(
                    "operator requires integer vector elements",
                ));
            }
            (
                NumericType::Integer { .. },
                ArithOp::MinNum
                | ArithOp::MaxNum
                | ArithOp::Minimum
                | ArithOp::Maximum
                | ArithOp::MinimumNum
                | ArithOp::MaximumNum,
            ) => return Err(ResolveError::Unsupported("vector operator")),
            (NumericType::Integer { .. }, ArithOp::Add | ArithOp::Sub | ArithOp::Mul) => {
                ArithSema::Integer {
                    overflow: Overflow::Wrap,
                }
            }
            (NumericType::Integer { signed, .. }, ArithOp::Div | ArithOp::Rem) => {
                ArithSema::Division {
                    by_zero: UbPolicy::Undefined,
                    min_by_neg_one: signed.then_some(UbPolicy::Undefined),
                }
            }
            (NumericType::Integer { signed, .. }, ArithOp::Shl) => ArithSema::ShiftLeft {
                overflow: Overflow::Wrap,
                amount_out_of_range: UbPolicy::Undefined,
                negative_left: signed.then_some(UbPolicy::Undefined),
            },
            (NumericType::Integer { signed, .. }, ArithOp::Shr) => ArithSema::ShiftRight {
                fill: if signed {
                    self.target.signed_right_shift
                } else {
                    ShiftFill::ZeroExtend
                },
                amount_out_of_range: UbPolicy::Undefined,
            },
            (NumericType::Integer { .. }, ArithOp::And | ArithOp::Or | ArithOp::Xor) => {
                ArithSema::Exact
            }
        };
        Ok((
            left.ty.clone(),
            ValueKind::Arith {
                op: arith,
                left: Box::new(left),
                right: Box::new(right),
                semantics,
            },
        ))
    }

    // clang's lax vector conversions: the left operand fixes the result type and
    // the right operand is reinterpreted or splatted into it
    fn vector_operands(&self, left: Value, right: Value) -> Result<(Value, Value), ResolveError> {
        match (&left.ty, &right.ty) {
            (Type::Vector { .. }, Type::Vector { .. }) => {
                let ty = left.ty.clone();
                let right = self.vector_convert(right, ty, ConversionReason::UsualArith)?;
                Ok((left, right))
            }
            (Type::Vector { .. }, _) => {
                let ty = left.ty.clone();
                let right = self.splat(right, ty)?;
                Ok((left, right))
            }
            (_, Type::Vector { .. }) => {
                let ty = right.ty.clone();
                let left = self.splat(left, ty)?;
                Ok((left, right))
            }
            _ => Err(ResolveError::Unsupported("vector arithmetic conversion")),
        }
    }

    fn splat(&self, value: Value, to: Type) -> Result<Value, ResolveError> {
        let Type::Vector { element, .. } = to else {
            return Err(ResolveError::Unsupported("vector arithmetic conversion"));
        };
        if !matches!(value.ty, Type::Numeric(_) | Type::Bool) {
            return Err(ResolveError::Invalid(
                "vector operand must be a vector or a scalar",
            ));
        }
        let value = self.emit_arithmetic_conversion(
            value,
            Type::Numeric(element),
            ConversionReason::UsualArith,
        )?;
        Ok(conversion(
            value,
            to,
            ConversionKind::VectorSplat,
            ConversionReason::UsualArith,
            ConversionSema::Exact,
        ))
    }

    pub(super) fn vector_convert(
        &self,
        value: Value,
        to: Type,
        reason: ConversionReason,
    ) -> Result<Value, ResolveError> {
        if value.ty == to {
            return Ok(value);
        }
        let from_bytes = self.target.storage_of(value.ty.clone())?.size_bytes;
        let to_bytes = self.target.storage_of(to.clone())?.size_bytes;
        if from_bytes != to_bytes {
            return Err(ResolveError::Invalid(
                "conversion between vector types of different size",
            ));
        }
        Ok(conversion(
            value,
            to,
            ConversionKind::VectorBitCast,
            reason,
            ConversionSema::Exact,
        ))
    }

    fn element_bits(&self, element: NumericType) -> Result<u32, ResolveError> {
        let bytes = self.target.storage_of(Type::Numeric(element))?.size_bytes;
        u32::try_from(bytes * 8).map_err(|_| ResolveError::Unsupported("vector element width"))
    }

    fn compare(&self, op: CompareOp, left: Value, right: Value) -> Result<Resolved, ResolveError> {
        if has_imaginary(&left, &right) {
            return Err(ResolveError::Invalid(
                "relational comparison requires real operands",
            ));
        }
        let operator = match op {
            CompareOp::Eq => "==",
            CompareOp::Ne => "!=",
            CompareOp::Lt => "<",
            CompareOp::Le => "<=",
            CompareOp::Gt => ">",
            CompareOp::Ge => ">=",
        };
        reject_mixed_decimal(operator, &left, &right)?;
        let left_ty = numeric(&left)?;
        Ok((Type::Bool, self.comparison(op, left_ty, left, right)))
    }

    fn fixed_binary(
        &self,
        op: BinaryOp,
        left: Value,
        right: Value,
    ) -> Result<Resolved, ResolveError> {
        let shift = matches!(op, BinaryOp::ShiftLeft | BinaryOp::ShiftRight);
        let Type::FixedPoint(fixed) = &left.ty else {
            return Err(ResolveError::Unsupported("unconverted fixed-point operand"));
        };
        let fixed = *fixed;
        if shift {
            if !matches!(right.ty, Type::Numeric(NumericType::Integer { .. })) {
                return Err(ResolveError::Invalid(
                    "fixed-point shift amount must be an integer",
                ));
            }
        } else if left.ty != right.ty {
            return Err(ResolveError::Unsupported(
                "unconverted fixed-point operands",
            ));
        }
        let compare = match op {
            BinaryOp::Equal => Some(CompareOp::Eq),
            BinaryOp::NotEqual => Some(CompareOp::Ne),
            BinaryOp::Less => Some(CompareOp::Lt),
            BinaryOp::LessEqual => Some(CompareOp::Le),
            BinaryOp::Greater => Some(CompareOp::Gt),
            BinaryOp::GreaterEqual => Some(CompareOp::Ge),
            _ => None,
        };
        if let Some(op) = compare {
            return Ok((
                Type::Bool,
                ValueKind::Compare {
                    op,
                    left: Box::new(left),
                    right: Box::new(right),
                    exceptions: None,
                    reason: None,
                },
            ));
        }
        let arith = match op {
            BinaryOp::Add => ArithOp::Add,
            BinaryOp::Sub => ArithOp::Sub,
            BinaryOp::Mul => ArithOp::Mul,
            BinaryOp::Div => ArithOp::Div,
            BinaryOp::ShiftLeft => ArithOp::Shl,
            BinaryOp::ShiftRight => ArithOp::Shr,
            _ => {
                return Err(ResolveError::Invalid(
                    "operator requires integer or real operands",
                ));
            }
        };
        Ok((
            left.ty.clone(),
            ValueKind::Arith {
                op: arith,
                left: Box::new(left),
                right: Box::new(right),
                semantics: self.fixed_semantics(
                    fixed,
                    (arith == ArithOp::Div).then_some(UbPolicy::Undefined),
                    shift.then_some(UbPolicy::Undefined),
                ),
            },
        ))
    }

    fn fixed_semantics(
        &self,
        fixed: crate::ir::FixedPointType,
        by_zero: Option<UbPolicy>,
        amount_out_of_range: Option<UbPolicy>,
    ) -> ArithSema {
        ArithSema::FixedPoint {
            overflow: fixed_overflow(fixed),
            rounding: FixedRounding::TowardZero,
            by_zero,
            amount_out_of_range,
        }
    }

    fn complex_binary(
        &self,
        op: BinaryOp,
        left: Value,
        right: Value,
    ) -> Result<Resolved, ResolveError> {
        let component = match (&left.ty, &right.ty) {
            (Type::Complex(a), Type::Complex(b))
            | (Type::Complex(a), Type::Numeric(b))
            | (Type::Numeric(a), Type::Complex(b)) => {
                if a != b {
                    return Err(ResolveError::Unsupported("unconverted complex components"));
                }
                *a
            }
            _ => return Err(ResolveError::Unsupported("complex arithmetic conversion")),
        };
        let result_ty = Type::Complex(component);
        let left_to = if matches!(left.ty, Type::Complex(_)) {
            result_ty.clone()
        } else {
            Type::Numeric(component)
        };
        let right_to = if matches!(right.ty, Type::Complex(_)) {
            result_ty.clone()
        } else {
            Type::Numeric(component)
        };
        let left = self.emit_arithmetic_conversion(left, left_to, ConversionReason::UsualArith)?;
        let right =
            self.emit_arithmetic_conversion(right, right_to, ConversionReason::UsualArith)?;
        if matches!(op, BinaryOp::Equal | BinaryOp::NotEqual) {
            return Ok((
                Type::Bool,
                ValueKind::Compare {
                    op: if op == BinaryOp::Equal {
                        CompareOp::Eq
                    } else {
                        CompareOp::Ne
                    },
                    left: Box::new(left),
                    right: Box::new(right),
                    exceptions: matches!(component, NumericType::Float(_))
                        .then_some(self.floating.exceptions),
                    reason: None,
                },
            ));
        }
        let arith = match op {
            BinaryOp::Add => ArithOp::Add,
            BinaryOp::Sub => ArithOp::Sub,
            BinaryOp::Mul => ArithOp::Mul,
            BinaryOp::Div => ArithOp::Div,
            _ => return Err(ResolveError::Unsupported("complex operator")),
        };
        let semantics = match component {
            NumericType::Float(_) => ArithSema::ComplexFloating(self.floating),
            NumericType::Integer { signed, .. } => ArithSema::ComplexInteger {
                overflow: if signed {
                    self.signed_overflow
                } else {
                    Overflow::Wrap
                },
                by_zero: (arith == ArithOp::Div).then_some(UbPolicy::Undefined),
            },
        };
        Ok((
            result_ty,
            ValueKind::Arith {
                op: arith,
                left: Box::new(left),
                right: Box::new(right),
                semantics,
            },
        ))
    }

    fn imaginary_binary(
        &self,
        op: BinaryOp,
        left: Value,
        right: Value,
    ) -> Result<Resolved, ResolveError> {
        let component = |ty: &Type| match ty {
            Type::Numeric(component) | Type::Complex(component) => Some(*component),
            Type::Imaginary(format) => Some(NumericType::Float(*format)),
            _ => None,
        };
        let (Some(a), Some(b)) = (component(&left.ty), component(&right.ty)) else {
            return Err(ResolveError::Unsupported("imaginary arithmetic conversion"));
        };
        if a != b {
            return Err(ResolveError::Unsupported(
                "unconverted imaginary components",
            ));
        }
        let NumericType::Float(format) = a else {
            return Err(ResolveError::Unsupported("imaginary arithmetic conversion"));
        };
        if [a, b]
            .iter()
            .any(|ty| matches!(ty, NumericType::Float(format) if format.is_decimal()))
        {
            return Err(ResolveError::Invalid(
                "decimal floating operand with imaginary operand",
            ));
        }
        let domain = |value: Value| {
            let to = match value.ty {
                Type::Numeric(_) => Type::Numeric(NumericType::Float(format)),
                Type::Imaginary(_) => Type::Imaginary(format),
                _ => Type::Complex(NumericType::Float(format)),
            };
            self.emit_arithmetic_conversion(value, to, ConversionReason::UsualArith)
        };
        let left = domain(left)?;
        let right = domain(right)?;
        if matches!(op, BinaryOp::Equal | BinaryOp::NotEqual) {
            return Ok((
                Type::Bool,
                ValueKind::Compare {
                    op: if op == BinaryOp::Equal {
                        CompareOp::Eq
                    } else {
                        CompareOp::Ne
                    },
                    left: Box::new(left),
                    right: Box::new(right),
                    exceptions: Some(self.floating.exceptions),
                    reason: None,
                },
            ));
        }
        let arith = match op {
            BinaryOp::Add => ArithOp::Add,
            BinaryOp::Sub => ArithOp::Sub,
            BinaryOp::Mul => ArithOp::Mul,
            BinaryOp::Div => ArithOp::Div,
            _ => {
                return Err(ResolveError::Invalid(
                    "operator requires integer or real operands",
                ));
            }
        };
        let both_imaginary = matches!(
            (&left.ty, &right.ty),
            (Type::Imaginary(_), Type::Imaginary(_))
        );
        let any_complex =
            matches!(left.ty, Type::Complex(_)) || matches!(right.ty, Type::Complex(_));
        let result_ty = match arith {
            _ if any_complex => Type::Complex(NumericType::Float(format)),
            ArithOp::Add | ArithOp::Sub if both_imaginary => Type::Imaginary(format),
            ArithOp::Add | ArithOp::Sub => Type::Complex(NumericType::Float(format)),
            _ if both_imaginary => Type::Numeric(NumericType::Float(format)),
            _ => Type::Imaginary(format),
        };
        let semantics = if matches!(result_ty, Type::Complex(_)) {
            ArithSema::ComplexFloating(self.floating)
        } else {
            ArithSema::Floating(self.floating)
        };
        Ok((
            result_ty,
            ValueKind::Arith {
                op: arith,
                left: Box::new(left),
                right: Box::new(right),
                semantics,
            },
        ))
    }

    fn comparison(&self, op: CompareOp, ty: NumericType, left: Value, right: Value) -> ValueKind {
        ValueKind::Compare {
            op,
            left: Box::new(left),
            right: Box::new(right),
            exceptions: matches!(ty, NumericType::Float(_)).then_some(self.floating.exceptions),
            reason: None,
        }
    }

    fn logical(&self, op: LogicalOp, left: Value, right: Value) -> Resolved {
        (
            Type::Bool,
            ValueKind::Logical {
                op,
                left: Box::new(self.condition(left)),
                right: Box::new(self.condition(right)),
            },
        )
    }

    pub(super) fn int_type(&self) -> Type {
        Type::integer(self.target.int_width, true)
    }

    pub(super) fn elementwise_conversion(
        &self,
        value: Value,
        from: Type,
        to: Type,
        lanes: u32,
        reason: ConversionReason,
    ) -> Result<Value, ResolveError> {
        let probe = Value {
            ty: from,
            node: value.node.clone(),
        };
        let converted = self.emit_arithmetic_conversion(probe, to, reason)?;
        per_lane(converted, value, lanes)
    }

    pub(super) fn emit_arithmetic_conversion(
        &self,
        value: Value,
        to: Type,
        reason: ConversionReason,
    ) -> Result<Value, ResolveError> {
        if value.ty == to {
            return Ok(value);
        }
        if to == Type::Bool {
            let mut value = self.condition(value);
            if let ValueKind::Compare { reason: why, .. } = &mut value.node.value {
                *why = Some(reason);
            }
            return Ok(value);
        }
        if value.ty == Type::Bool {
            let integer = if matches!(to, Type::Numeric(NumericType::Integer { .. })) {
                to.clone()
            } else {
                self.int_type()
            };
            let value = conversion(
                value,
                integer,
                ConversionKind::FromBool,
                reason,
                ConversionSema::Exact,
            );
            return self.emit_arithmetic_conversion(value, to, reason);
        }
        let expected = to.clone();
        let converted = match (value.ty.clone(), to.clone()) {
            (Type::Imaginary(from), Type::Imaginary(target)) => {
                let semantics = if target.widens_from(from) {
                    ConversionSema::Exact
                } else {
                    ConversionSema::Floating(self.floating)
                };
                conversion(
                    value,
                    to,
                    ConversionKind::ImaginaryConvert,
                    reason,
                    semantics,
                )
            }
            (Type::Numeric(_), Type::Imaginary(_)) => conversion(
                value,
                to,
                ConversionKind::RealToImaginary,
                reason,
                ConversionSema::Exact,
            ),
            (Type::Imaginary(_), Type::Numeric(_)) => conversion(
                value,
                to,
                ConversionKind::ImaginaryToReal,
                reason,
                ConversionSema::Exact,
            ),
            (Type::Imaginary(from), Type::Complex(_)) => {
                let value = conversion(
                    value,
                    Type::Complex(NumericType::Float(from)),
                    ConversionKind::ImaginaryToComplex,
                    reason,
                    ConversionSema::Exact,
                );
                self.emit_arithmetic_conversion(value, to, reason)?
            }
            (Type::Complex(source), Type::Imaginary(target)) => {
                let format = match source {
                    NumericType::Float(format) => format,
                    NumericType::Integer { .. } => target,
                };
                let value = self.emit_arithmetic_conversion(
                    value,
                    Type::Complex(NumericType::Float(format)),
                    reason,
                )?;
                let value = conversion(
                    value,
                    Type::Imaginary(format),
                    ConversionKind::ComplexToImaginary,
                    reason,
                    ConversionSema::Exact,
                );
                self.emit_arithmetic_conversion(value, to, reason)?
            }
            (Type::Numeric(_), Type::Complex(component)) => {
                let value =
                    self.emit_arithmetic_conversion(value, Type::Numeric(component), reason)?;
                conversion(
                    value,
                    to,
                    ConversionKind::RealToComplex,
                    reason,
                    ConversionSema::Exact,
                )
            }
            (Type::Complex(source), Type::Numeric(_)) => {
                let value = conversion(
                    value,
                    Type::Numeric(source),
                    ConversionKind::ComplexToReal,
                    reason,
                    ConversionSema::Exact,
                );
                self.emit_arithmetic_conversion(value, to, reason)?
            }
            (Type::Complex(from), Type::Complex(target)) => {
                let semantics = match (from, target) {
                    (NumericType::Float(a), NumericType::Float(b)) if b.widens_from(a) => {
                        ConversionSema::Exact
                    }
                    (NumericType::Float(_), NumericType::Float(_)) => {
                        ConversionSema::Floating(self.floating)
                    }
                    (NumericType::Integer { width, signed, .. }, NumericType::Float(format)) => {
                        ConversionSema::IntToFloat {
                            exact: width - u32::from(signed) <= format.exact_integer_bits(),
                            floating: self.floating,
                        }
                    }
                    (NumericType::Float(_), NumericType::Integer { .. }) => {
                        ConversionSema::Exceptions(self.floating.exceptions)
                    }
                    (
                        NumericType::Integer {
                            width: a,
                            signed: sa,
                            ..
                        },
                        NumericType::Integer {
                            width: b,
                            signed: sb,
                            ..
                        },
                    ) => {
                        if a < b && sa == sb {
                            ConversionSema::Exact
                        } else {
                            ConversionSema::Fits(Fits::Unknown)
                        }
                    }
                };
                conversion(value, to, ConversionKind::ComplexConvert, reason, semantics)
            }
            (Type::FixedPoint(from), Type::FixedPoint(target)) => conversion(
                value,
                to,
                ConversionKind::FixedConvert,
                reason,
                fixed_conversion_sema(from, target),
            ),
            (Type::Numeric(NumericType::Integer { .. }), Type::FixedPoint(target)) => conversion(
                value,
                to,
                ConversionKind::IntToFixed,
                reason,
                ConversionSema::FixedPoint {
                    overflow: fixed_overflow(target),
                    rounding: FixedRounding::TowardZero,
                },
            ),
            (Type::FixedPoint(_), Type::Numeric(NumericType::Integer { .. })) => conversion(
                value,
                to,
                ConversionKind::FixedToInt,
                reason,
                ConversionSema::FixedPoint {
                    overflow: FixedOverflow::Undefined,
                    rounding: FixedRounding::TowardZero,
                },
            ),
            (Type::Numeric(NumericType::Float(_)), Type::FixedPoint(target)) => conversion(
                value,
                to,
                ConversionKind::FloatToFixed,
                reason,
                ConversionSema::FixedPoint {
                    overflow: fixed_overflow(target),
                    rounding: FixedRounding::TowardZero,
                },
            ),
            (Type::FixedPoint(_), Type::Numeric(NumericType::Float(_))) => conversion(
                value,
                to,
                ConversionKind::FixedToFloat,
                reason,
                ConversionSema::Floating(self.floating),
            ),
            (
                Type::Numeric(NumericType::Integer {
                    width: from_width,
                    signed: from_signed,
                    ..
                }),
                Type::Numeric(NumericType::Integer {
                    width,
                    signed,
                    bit_precise,
                }),
            ) => {
                let mut value = value;
                if from_width != width {
                    let intermediate = Type::Numeric(NumericType::Integer {
                        width,
                        signed: from_signed,
                        bit_precise,
                    });
                    let (kind, semantics) = if from_width < width {
                        (ConversionKind::Widen, ConversionSema::Exact)
                    } else {
                        (
                            ConversionKind::Truncate,
                            ConversionSema::Fits(integer_fits(&value, width, from_signed)),
                        )
                    };
                    value = conversion(value, intermediate, kind, reason, semantics);
                }
                if from_signed != signed {
                    let fits = integer_fits(&value, width, signed);
                    value = conversion(
                        value,
                        to,
                        ConversionKind::Reinterpret,
                        reason,
                        ConversionSema::Fits(fits),
                    );
                } else if value.ty != to {
                    // same width and signedness, so only bit-precision distinguishes the two types
                    value = conversion(
                        value,
                        to,
                        ConversionKind::Reinterpret,
                        reason,
                        ConversionSema::Exact,
                    );
                }
                value
            }
            (
                Type::Numeric(NumericType::Integer { width, signed, .. }),
                Type::Numeric(NumericType::Float(format)),
            ) => {
                let exact = width - u32::from(signed) <= format.exact_integer_bits()
                    || matches!(
                        value.node.value,
                        ValueKind::Convert {
                            kind: ConversionKind::FromBool,
                            ..
                        }
                    );
                conversion(
                    value,
                    to,
                    ConversionKind::IntToFloat,
                    reason,
                    ConversionSema::IntToFloat {
                        exact,
                        floating: self.floating,
                    },
                )
            }
            (
                Type::Numeric(NumericType::Float(from)),
                Type::Numeric(NumericType::Float(to_format)),
            ) => {
                let (kind, semantics) = if to_format.widens_from(from) {
                    (ConversionKind::FloatWiden, ConversionSema::Exact)
                } else if from.is_decimal() != to_format.is_decimal() {
                    (
                        ConversionKind::FloatConvert,
                        ConversionSema::Floating(self.floating),
                    )
                } else {
                    (
                        ConversionKind::FloatNarrow,
                        ConversionSema::Floating(self.floating),
                    )
                };
                conversion(value, to, kind, reason, semantics)
            }
            (Type::Numeric(NumericType::Float(_)), Type::Numeric(NumericType::Integer { .. })) => {
                conversion(
                    value,
                    to,
                    ConversionKind::FloatToInt,
                    reason,
                    ConversionSema::Exceptions(self.floating.exceptions),
                )
            }
            _ => value,
        };
        // an unhandled type pair would otherwise leave the operand at its source
        // type while the caller treats it as converted
        if converted.ty != expected {
            return Err(ResolveError::Unsupported("arithmetic conversion"));
        }
        Ok(converted)
    }

    pub(super) fn condition(&self, value: Value) -> Value {
        let ty = match value.ty {
            Type::Numeric(ty) => ty,
            Type::Imaginary(format) => NumericType::Float(format),
            Type::FixedPoint(fixed) => fixed.storage(),
            _ => return value,
        };
        let zero = match ty {
            NumericType::Integer { .. } => Number::Integer(BigUint::default()),
            NumericType::Float(format) => Number::float_zero(format),
        };
        let anchor = value.node.derive(());
        let zero = Value {
            ty: value.ty.clone(),
            node: anchor.derive(ValueKind::Constant(zero)),
        };
        Value {
            ty: Type::Bool,
            node: anchor.with_value(self.comparison(CompareOp::Ne, ty, value, zero)),
        }
    }
}

fn fixed_overflow(fixed: crate::ir::FixedPointType) -> FixedOverflow {
    if fixed.saturating {
        FixedOverflow::Saturate
    } else {
        FixedOverflow::Undefined
    }
}

// a fixed-point conversion that keeps every representable value of the source
// can neither saturate nor drop fractional bits
fn fixed_conversion_sema(
    from: crate::ir::FixedPointType,
    to: crate::ir::FixedPointType,
) -> ConversionSema {
    if to.scale >= from.scale
        && to.integral_bits() >= from.integral_bits()
        && (to.signed || !from.signed)
    {
        return ConversionSema::Exact;
    }
    ConversionSema::FixedPoint {
        overflow: fixed_overflow(to),
        rounding: FixedRounding::TowardZero,
    }
}

fn numeric(value: &Value) -> Result<NumericType, ResolveError> {
    match value.ty {
        Type::Numeric(ty) => Ok(ty),
        Type::Bool => Err(ResolveError::Unsupported("unpromoted boolean operand")),
        Type::Defined(_)
        | Type::Complex(_)
        | Type::Imaginary(_)
        | Type::FixedPoint(_)
        | Type::Vector { .. }
        | Type::Pointer { .. }
        | Type::VaList
        | Type::Array { .. }
        | Type::VariableArray { .. }
        | Type::Function { .. }
        | Type::Void => Err(ResolveError::Unsupported("non-numeric operand")),
    }
}

fn imaginary_literal(
    expression: &Expr,
    component: NumericType,
    zero: Number,
    value: Number,
) -> Resolved {
    let member = |index, number| AggregateMember {
        target: AggregateTarget::Index(index),
        value: Value {
            ty: Type::Numeric(component),
            node: expression.derive(ValueKind::Constant(number)),
        },
    };
    (
        Type::Complex(component),
        ValueKind::Aggregate {
            members: vec![member(0, zero), member(1, value)],
            zero_fill: false,
        },
    )
}

fn has_imaginary(left: &Value, right: &Value) -> bool {
    matches!(left.ty, Type::Imaginary(_)) || matches!(right.ty, Type::Imaginary(_))
}

pub(super) fn reject_mixed_decimal(
    operator: &'static str,
    left: &Value,
    right: &Value,
) -> Result<(), ResolveError> {
    let family = |ty: &Type| match ty {
        Type::Numeric(NumericType::Float(format)) | Type::Complex(NumericType::Float(format)) => {
            Some(format.is_decimal())
        }
        _ => None,
    };
    match (family(&left.ty), family(&right.ty)) {
        (Some(a), Some(b)) if a != b => Err(ResolveError::InvalidOperands {
            left: numeric(left)?,
            operator,
            right: numeric(right)?,
        }),
        _ => Ok(()),
    }
}

pub(super) fn resolve_float_literal(
    literal: &FloatLiteral,
    target: &TargetInfo,
) -> Result<ResolvedFloat, crate::const_expr::ConstExprError> {
    let mut literal = literal.clone();
    if literal.suffix == FloatSuffix::L {
        literal.suffix = match target.long_double {
            crate::target_info::LongDoubleFormat::Binary64 => FloatSuffix::F64,
            crate::target_info::LongDoubleFormat::X87 => FloatSuffix::L,
            crate::target_info::LongDoubleFormat::Binary128 => FloatSuffix::F128,
        };
    }
    resolve_float(&literal)
}

fn conversion(
    value: Value,
    ty: Type,
    kind: ConversionKind,
    reason: ConversionReason,
    semantics: ConversionSema,
) -> Value {
    let anchor = value.node.derive(());
    let node = anchor.with_value(ValueKind::Convert {
        kind,
        operand: Box::new(value),
        reason,
        semantics,
    });
    Value { ty, node }
}

fn integer_fits(value: &Value, width: u32, signed: bool) -> Fits {
    match &value.node.value {
        ValueKind::Constant(Number::Integer(value)) if fits_rank(value, width, signed) => {
            Fits::Always
        }
        ValueKind::Convert {
            kind: ConversionKind::FromBool,
            ..
        } => Fits::Always,
        _ => Fits::Unknown,
    }
}

fn per_lane(converted: Value, operand: Value, lanes: u32) -> Result<Value, ResolveError> {
    let mut node = converted.node;
    let ValueKind::Convert {
        kind,
        operand: inner,
        reason,
        semantics,
    } = std::mem::replace(&mut node.value, ValueKind::Void)
    else {
        return Ok(operand);
    };
    let Type::Numeric(element) = converted.ty else {
        return Err(ResolveError::Unsupported("elementwise conversion"));
    };
    let inner = per_lane(*inner, operand, lanes)?;
    Ok(Value {
        ty: Type::Vector { element, lanes },
        node: node.with_value(ValueKind::Convert {
            kind,
            operand: Box::new(inner),
            reason,
            semantics,
        }),
    })
}
