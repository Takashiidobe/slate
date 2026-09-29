use super::pragmas::FloatingRegion;
use super::validate::{
    bit_int_literal_width, fits_rank, integer_rank_width, select_integer_candidate,
};
use crate::ast::{Expr, ExprKind, FixedPointKind, FixedPointRank, Loc};
use crate::const_expr::{
    BinaryOp, ConstExprError, FixedPointLiteralSuffix, FloatLiteral, FloatSuffix, FloatValue,
    IntegerSizeSuffix, ResolvedFloat, UnaryOp, resolve_float,
};
use crate::ir::{
    AggregateMember, AggregateTarget, ArithOp, ArithSema, AsmDialect, CompareOp, ConversionKind,
    ConversionReason, ConversionSema, Fits, FixedOverflow, FixedPointType, FixedRounding,
    FloatType, LogicalOp, Number, NumericType, Overflow, ShiftFill, Type, UbPolicy, UnaryArithOp,
    Value, ValueKind,
};
use crate::standard_features::StandardFeatures;
use crate::target_info::TargetInfo;
use num_bigint::BigUint;
use thiserror::Error;

#[derive(Debug, Clone, Error)]
pub enum ResolveError {
    #[error("{0}")]
    Rejected(&'static str),
    #[error("not implemented: {0}")]
    Unimplemented(&'static str),
    #[error("internal error: {0}")]
    Internal(&'static str),
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
    #[error("{error}")]
    Located { loc: Loc, error: Box<ResolveError> },
}

impl ResolveError {
    pub fn loc(&self) -> Option<Loc> {
        match self {
            Self::Names(error) => Some(error.loc()),
            Self::Located { loc, .. } => Some(*loc),
            _ => None,
        }
    }

    pub fn at(self, loc: Loc) -> Self {
        if self.loc().is_some() {
            self
        } else {
            Self::Located {
                loc,
                error: Box::new(self),
            }
        }
    }

    pub(super) fn checked(self) -> Self {
        match self {
            Self::Rejected(reason) => Self::Internal(reason),
            Self::InvalidOperands { .. } => Self::Internal("invalid operands"),
            Self::InvalidOperand { .. } => Self::Internal("invalid operand"),
            error => error,
        }
    }

    pub(super) fn is_rejection(&self) -> bool {
        matches!(
            self,
            Self::Rejected(_) | Self::InvalidOperands { .. } | Self::InvalidOperand { .. }
        )
    }
}

const UNSUPPORTED_EXPRESSION: &str = "expression (expected a number or arithmetic operator)";

pub struct Context {
    pub target: TargetInfo,
    pub features: StandardFeatures,
    pub signed_overflow: Overflow,
    pub pointer_wrap: bool,
    pub region: FloatingRegion,
    pub asm_dialect: AsmDialect,
}

type Resolved = (Type, ValueKind);

impl Context {
    pub fn for_dialect(dialect: &crate::dialect::Dialect) -> Self {
        let options = dialect.options();
        Self {
            target: dialect.target().clone(),
            features: dialect.features(),
            signed_overflow: options.operations.signed_overflow,
            pointer_wrap: options.operations.pointer_wrap,
            region: FloatingRegion {
                floating: options.operations.floating,
                contract: super::pragmas::default_contraction(dialect.flavor(), dialect.standard()),
                ..FloatingRegion::default()
            },
            asm_dialect: options.asm_dialect,
        }
    }

    pub(super) fn floating_arith(&self) -> ArithSema {
        ArithSema::Floating {
            floating: self.region.floating,
            contract: self.region.contract,
        }
    }

    fn complex_floating_arith(&self) -> ArithSema {
        ArithSema::ComplexFloating {
            floating: self.region.floating,
            range: self.region.complex_range,
        }
    }

    pub(super) fn resolve_literal(&self, expression: &Expr) -> Result<Value, ResolveError> {
        let (ty, kind) = match &expression.value {
            ExprKind::IntegerLiteral(literal) => {
                let bit_precise = literal.suffix.size == IntegerSizeSuffix::BitInt;
                let (width, signed, value) = if bit_precise {
                    bit_int_literal_width(literal)
                        .map(|width| {
                            (
                                width,
                                !literal.suffix.unsigned,
                                Number::Integer(literal.value.clone()),
                            )
                        })
                        .ok_or_else(|| ResolveError::IntegerLiteral(literal.spelling.clone()))?
                } else {
                    select_integer_candidate(literal, &self.target, self.features)
                        .map(|selection| {
                            (
                                integer_rank_width(selection.rank, &self.target),
                                selection.signed,
                                selection.value,
                            )
                        })
                        .ok_or_else(|| ResolveError::IntegerLiteral(literal.spelling.clone()))?
                };
                let numeric = NumericType::Integer {
                    width,
                    signed,
                    bit_precise,
                };
                let number = value;
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
                if let Some(suffix) = literal.fixed_suffix {
                    let number = fixed_literal_value(literal, suffix)?;
                    let fract_width = 8u32
                        << match suffix.rank {
                            FixedPointRank::Short => 0,
                            FixedPointRank::Default => 1,
                            FixedPointRank::Long => 2,
                            FixedPointRank::LongLong => 3,
                        };
                    let width = fract_width
                        * if suffix.kind == FixedPointKind::Accum {
                            2
                        } else {
                            1
                        };
                    let signed = !suffix.unsigned;
                    let fixed = FixedPointType {
                        width,
                        scale: fract_width - u32::from(signed),
                        signed,
                        saturating: false,
                    };
                    return Ok(Value {
                        ty: Type::FixedPoint(fixed),
                        node: expression.derive(ValueKind::Constant(number)),
                    });
                }
                let (format, number) = match resolve_float_literal(literal, &self.target)?.value {
                    FloatValue::BFloat16(bits) => {
                        (FloatType::BF16, Number::FloatBits(u128::from(bits)))
                    }
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
            _ => return Err(ResolveError::Internal(UNSUPPORTED_EXPRESSION)),
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
        binary_rule(op, &left.ty, &right.ty, &self.target).map_err(ResolveError::checked)?;
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
        let left_ty = numeric(&left)?;
        let right_ty = numeric(&right)?;
        let invalid = ResolveError::Internal("invalid operands");
        let semantics = match (left_ty, arith) {
            (NumericType::Float(_), ArithOp::Add | ArithOp::Sub | ArithOp::Mul | ArithOp::Div) => {
                self.floating_arith()
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
        unary_rule(op, &operand.ty).map_err(ResolveError::checked)?;
        let arith = match op {
            UnaryOp::Minus => UnaryArithOp::Neg,
            _ => UnaryArithOp::Not,
        };
        if let Type::Vector { element, .. } = operand.ty {
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
                return Err(ResolveError::Internal(
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
                return Err(ResolveError::Internal(
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
                NumericType::Float(_) => self.complex_floating_arith(),
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
            (NumericType::Float(_), UnaryArithOp::Not) => {
                return Err(ResolveError::Internal("invalid operand"));
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
            return Err(ResolveError::Internal("vector arithmetic conversion"));
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
                        .then_some(self.region.floating.exceptions),
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
            _ => return Err(ResolveError::Internal("vector operator")),
        };
        // clang emits no nsw for vector arithmetic, so signed lanes wrap
        let semantics = match (element, arith) {
            (NumericType::Float(_), ArithOp::Add | ArithOp::Sub | ArithOp::Mul | ArithOp::Div) => {
                self.floating_arith()
            }
            (NumericType::Float(_), _) => {
                return Err(ResolveError::Internal(
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
            ) => return Err(ResolveError::Unimplemented("vector operator")),
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
            _ => Err(ResolveError::Internal("vector arithmetic conversion")),
        }
    }

    fn splat(&self, value: Value, to: Type) -> Result<Value, ResolveError> {
        let Type::Vector { element, .. } = to else {
            return Err(ResolveError::Internal("vector arithmetic conversion"));
        };
        if !matches!(value.ty, Type::Numeric(_) | Type::Bool) {
            return Err(ResolveError::Internal(
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
            return Err(ResolveError::Internal(
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
        u32::try_from(bytes * 8).map_err(|_| ResolveError::Internal("vector element width"))
    }

    fn compare(&self, op: CompareOp, left: Value, right: Value) -> Result<Resolved, ResolveError> {
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
            return Err(ResolveError::Internal("unconverted fixed-point operand"));
        };
        let fixed = *fixed;
        if shift {
            if !matches!(right.ty, Type::Numeric(NumericType::Integer { .. })) {
                return Err(ResolveError::Internal(
                    "fixed-point shift amount must be an integer",
                ));
            }
        } else if left.ty != right.ty {
            return Err(ResolveError::Internal("unconverted fixed-point operands"));
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
                return Err(ResolveError::Internal(
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
                    return Err(ResolveError::Internal("unconverted complex components"));
                }
                *a
            }
            _ => return Err(ResolveError::Internal("complex arithmetic conversion")),
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
                        .then_some(self.region.floating.exceptions),
                    reason: None,
                },
            ));
        }
        let arith = match op {
            BinaryOp::Add => ArithOp::Add,
            BinaryOp::Sub => ArithOp::Sub,
            BinaryOp::Mul => ArithOp::Mul,
            BinaryOp::Div => ArithOp::Div,
            _ => return Err(ResolveError::Internal("complex operator")),
        };
        let semantics = match component {
            NumericType::Float(_) => self.complex_floating_arith(),
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
            return Err(ResolveError::Internal("imaginary arithmetic conversion"));
        };
        if a != b {
            return Err(ResolveError::Internal("unconverted imaginary components"));
        }
        let NumericType::Float(format) = a else {
            return Err(ResolveError::Internal("imaginary arithmetic conversion"));
        };
        if [a, b]
            .iter()
            .any(|ty| matches!(ty, NumericType::Float(format) if format.is_decimal()))
        {
            return Err(ResolveError::Internal(
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
                    exceptions: Some(self.region.floating.exceptions),
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
                return Err(ResolveError::Internal(
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
            self.complex_floating_arith()
        } else {
            self.floating_arith()
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
            exceptions: matches!(ty, NumericType::Float(_))
                .then_some(self.region.floating.exceptions),
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
                    ConversionSema::Floating(self.region.floating)
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
                        ConversionSema::Floating(self.region.floating)
                    }
                    (NumericType::Integer { width, signed, .. }, NumericType::Float(format)) => {
                        ConversionSema::IntToFloat {
                            exact: width - u32::from(signed) <= format.exact_integer_bits(),
                            floating: self.region.floating,
                        }
                    }
                    (NumericType::Float(_), NumericType::Integer { .. }) => {
                        ConversionSema::Exceptions(self.region.floating.exceptions)
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
                ConversionSema::Floating(self.region.floating),
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
                        floating: self.region.floating,
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
                        ConversionSema::Floating(self.region.floating),
                    )
                } else {
                    (
                        ConversionKind::FloatNarrow,
                        ConversionSema::Floating(self.region.floating),
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
                    ConversionSema::Exceptions(self.region.floating.exceptions),
                )
            }
            _ => value,
        };
        // an unhandled type pair would otherwise leave the operand at its source
        // type while the caller treats it as converted
        if converted.ty != expected {
            return Err(ResolveError::Unimplemented("arithmetic conversion"));
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

fn fixed_literal_value(
    literal: &FloatLiteral,
    suffix: FixedPointLiteralSuffix,
) -> Result<Number, ResolveError> {
    let lower = literal.spelling.to_ascii_lowercase();
    let marker = lower.rfind(['r', 'k']).ok_or_else(|| {
        ResolveError::Literal(ConstExprError::InvalidFloatLiteral(
            literal.spelling.clone(),
        ))
    })?;
    let mut start = marker;
    while start > 0 && matches!(lower.as_bytes()[start - 1], b'u' | b'h' | b'l') {
        start -= 1;
    }
    let body = &literal.spelling[..start];
    let (mantissa, exponent) = body
        .split_once(['e', 'E'])
        .map_or((body, 0i32), |(mantissa, exponent)| {
            (mantissa, exponent.parse().unwrap_or(i32::MIN))
        });
    if exponent == i32::MIN || mantissa.starts_with("0x") || mantissa.starts_with("0X") {
        return Err(ResolveError::Literal(ConstExprError::InvalidFloatLiteral(
            literal.spelling.clone(),
        )));
    }
    let mut digits = String::new();
    let mut fractional = 0i32;
    let mut after_dot = false;
    for ch in mantissa.chars() {
        match ch {
            '.' => after_dot = true,
            '\'' => {}
            '0'..='9' => {
                digits.push(ch);
                if after_dot {
                    fractional += 1;
                }
            }
            _ => {
                return Err(ResolveError::Literal(ConstExprError::InvalidFloatLiteral(
                    literal.spelling.clone(),
                )));
            }
        }
    }
    let mut numerator = BigUint::parse_bytes(digits.as_bytes(), 10).ok_or_else(|| {
        ResolveError::Literal(ConstExprError::InvalidFloatLiteral(
            literal.spelling.clone(),
        ))
    })?;
    let decimal_scale = fractional - exponent;
    let fract_width = 8u32
        << match suffix.rank {
            FixedPointRank::Short => 0,
            FixedPointRank::Default => 1,
            FixedPointRank::Long => 2,
            FixedPointRank::LongLong => 3,
        };
    let scale = fract_width - u32::from(!suffix.unsigned);
    numerator <<= scale as usize;
    if decimal_scale > 0 {
        numerator /= BigUint::from(10u8).pow(decimal_scale as u32);
    } else if decimal_scale < 0 {
        numerator *= BigUint::from(10u8).pow((-decimal_scale) as u32);
    }
    Ok(Number::Integer(numerator))
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
    numeric_type(&value.ty).map_err(ResolveError::checked)
}

fn numeric_type(ty: &Type) -> Result<NumericType, ResolveError> {
    match *ty {
        Type::Numeric(ty) => Ok(ty),
        Type::Bool => Err(ResolveError::Internal("unpromoted boolean operand")),
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
        | Type::Void => Err(ResolveError::Rejected("non-numeric operand")),
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
    left: &Type,
    right: &Type,
) -> Result<(), ResolveError> {
    let family = |ty: &Type| match ty {
        Type::Numeric(NumericType::Float(format)) | Type::Complex(NumericType::Float(format)) => {
            Some(format.is_decimal())
        }
        _ => None,
    };
    match (family(left), family(right)) {
        (Some(a), Some(b)) if a != b => Err(ResolveError::InvalidOperands {
            left: numeric_type(left)?,
            operator,
            right: numeric_type(right)?,
        }),
        _ => Ok(()),
    }
}

// the operand checks emit_binary relies on, over the converted operand types
pub(super) fn binary_rule(
    op: BinaryOp,
    left: &Type,
    right: &Type,
    target: &TargetInfo,
) -> Result<(), ResolveError> {
    let operator = <&'static str>::from(op);
    if matches!(left, Type::Vector { .. }) || matches!(right, Type::Vector { .. }) {
        return vector_rule(op, left, right, target);
    }
    let comparison = matches!(
        op,
        BinaryOp::Equal
            | BinaryOp::NotEqual
            | BinaryOp::Less
            | BinaryOp::LessEqual
            | BinaryOp::Greater
            | BinaryOp::GreaterEqual
    );
    if matches!(op, BinaryOp::And | BinaryOp::Or) {
        return Ok(());
    }
    if matches!(left, Type::FixedPoint(_)) || matches!(right, Type::FixedPoint(_)) {
        if matches!(op, BinaryOp::ShiftLeft | BinaryOp::ShiftRight) {
            if !matches!(right, Type::Numeric(NumericType::Integer { .. })) {
                return Err(ResolveError::Rejected(
                    "fixed-point shift amount must be an integer",
                ));
            }
        } else if !comparison
            && !matches!(
                op,
                BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div
            )
        {
            return Err(ResolveError::Rejected(
                "operator requires integer or real operands",
            ));
        }
        return Ok(());
    }
    let imaginary = matches!(left, Type::Imaginary(_)) || matches!(right, Type::Imaginary(_));
    let complex = matches!(left, Type::Complex(_)) || matches!(right, Type::Complex(_));
    let equality = matches!(op, BinaryOp::Equal | BinaryOp::NotEqual);
    if imaginary && (equality || !comparison) {
        let decimal = |ty: &Type| match ty {
            Type::Numeric(NumericType::Float(format))
            | Type::Complex(NumericType::Float(format))
            | Type::Imaginary(format) => format.is_decimal(),
            _ => false,
        };
        if decimal(left) || decimal(right) {
            return Err(ResolveError::Rejected(
                "decimal floating operand with imaginary operand",
            ));
        }
        if !equality
            && !matches!(
                op,
                BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div
            )
        {
            return Err(ResolveError::Rejected(
                "operator requires integer or real operands",
            ));
        }
        return Ok(());
    }
    if complex && equality {
        return Ok(());
    }
    if comparison {
        if imaginary {
            return Err(ResolveError::Rejected(
                "relational comparison requires real operands",
            ));
        }
        reject_mixed_decimal(operator, left, right)?;
        numeric_type(left)?;
        return Ok(());
    }
    if complex {
        return match op {
            BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div => Ok(()),
            _ => Err(ResolveError::Rejected("complex operator")),
        };
    }
    reject_mixed_decimal(operator, left, right)?;
    let left_ty = numeric_type(left)?;
    let right_ty = numeric_type(right)?;
    let invalid = ResolveError::InvalidOperands {
        left: left_ty,
        operator,
        right: right_ty,
    };
    match (left_ty, op) {
        (NumericType::Float(_), BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div) => {
            Ok(())
        }
        (NumericType::Float(_), _) => Err(invalid),
        (_, BinaryOp::ShiftLeft | BinaryOp::ShiftRight)
            if matches!(right_ty, NumericType::Float(_)) =>
        {
            Err(invalid)
        }
        _ => Ok(()),
    }
}

fn vector_rule(
    op: BinaryOp,
    left: &Type,
    right: &Type,
    target: &TargetInfo,
) -> Result<(), ResolveError> {
    let element = match (left, right) {
        (Type::Vector { element, .. }, Type::Vector { .. }) => {
            if target.storage_of(left.clone())?.size_bytes
                != target.storage_of(right.clone())?.size_bytes
            {
                return Err(ResolveError::Rejected(
                    "conversion between vector types of different size",
                ));
            }
            element
        }
        (Type::Vector { element, .. }, scalar) | (scalar, Type::Vector { element, .. }) => {
            if !matches!(scalar, Type::Numeric(_) | Type::Bool) {
                return Err(ResolveError::Rejected(
                    "vector operand must be a vector or a scalar",
                ));
            }
            element
        }
        _ => return Err(ResolveError::Internal("vector arithmetic conversion")),
    };
    match op {
        BinaryOp::Equal
        | BinaryOp::NotEqual
        | BinaryOp::Less
        | BinaryOp::LessEqual
        | BinaryOp::Greater
        | BinaryOp::GreaterEqual => Ok(()),
        BinaryOp::And | BinaryOp::Or => Err(ResolveError::Rejected("vector operator")),
        BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div => Ok(()),
        _ if matches!(element, NumericType::Float(_)) => Err(ResolveError::Rejected(
            "operator requires integer vector elements",
        )),
        _ => Ok(()),
    }
}

// the operand checks emit_unary_arith relies on, over the promoted operand type
pub(super) fn unary_rule(op: UnaryOp, operand: &Type) -> Result<(), ResolveError> {
    let complement = op != UnaryOp::Minus;
    match operand {
        Type::Vector {
            element: NumericType::Float(_),
            ..
        } if complement => Err(ResolveError::Rejected(
            "bitwise complement of a floating vector",
        )),
        Type::FixedPoint(_) if complement => Err(ResolveError::Rejected(
            "bitwise complement of a fixed-point operand",
        )),
        Type::Imaginary(_) if complement => Err(ResolveError::Rejected(
            "bitwise complement of imaginary operand",
        )),
        Type::Vector { .. } | Type::FixedPoint(_) | Type::Imaginary(_) | Type::Complex(_) => Ok(()),
        ty => match numeric_type(ty)? {
            ty @ NumericType::Float(_) if complement => Err(ResolveError::InvalidOperand {
                operator: <&'static str>::from(op),
                operand: ty,
            }),
            _ => Ok(()),
        },
    }
}

pub(super) fn resolve_float_literal(
    literal: &FloatLiteral,
    target: &TargetInfo,
) -> Result<ResolvedFloat, crate::const_expr::ConstExprError> {
    let mut literal = literal.clone();
    let format = match literal.suffix {
        FloatSuffix::L | FloatSuffix::W => Some(match target.long_double {
            crate::target_info::LongDoubleFormat::Binary64 => FloatType::F64,
            crate::target_info::LongDoubleFormat::X87 => FloatType::F80,
            crate::target_info::LongDoubleFormat::Binary128 => FloatType::F128,
        }),
        FloatSuffix::F64x => target.float64x_format(),
        _ => None,
    };
    literal.suffix = match format {
        Some(FloatType::F64) => FloatSuffix::F64,
        Some(FloatType::F80) => FloatSuffix::L,
        Some(FloatType::F128) => FloatSuffix::F128,
        _ => literal.suffix,
    };
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
        return Err(ResolveError::Internal("elementwise conversion"));
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
