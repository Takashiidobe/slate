use super::validate::{fits_rank, integer_candidates, integer_rank_width};
use crate::ast::{Expr, ExprKind, Span};
use crate::const_expr::{
    BinaryOp, FloatLiteral, FloatSuffix, FloatValue, IntegerSizeSuffix, ResolvedFloat,
    resolve_float,
};
use crate::ir::{
    ArithOp, ArithSema, Exceptions, FloatType, FloatingSemantics, Number, NumericType, Overflow,
    Rounding, ShiftFill, Value, ValueKind,
};
use crate::target_info::TargetInfo;
use thiserror::Error;

#[derive(Debug, Error)]
pub enum ResolveError {
    #[error("unsupported in numeric IR lowering: {0}")]
    Unsupported(&'static str),
    #[error("integer literal `{0}` has no supported target type")]
    IntegerLiteral(String),
    #[error("arithmetic requires conversions not yet implemented: {left} {operator} {right}")]
    Conversion {
        left: NumericType,
        operator: &'static str,
        right: NumericType,
    },
    #[error("invalid operands to binary expression: {left} {operator} {right}")]
    InvalidOperands {
        left: NumericType,
        operator: &'static str,
        right: NumericType,
    },
    #[error(transparent)]
    Literal(#[from] crate::const_expr::ConstExprError),
}

const UNSUPPORTED_EXPRESSION: &str = "expression (expected a number or arithmetic operator)";

pub struct Context {
    pub target: TargetInfo,
    pub signed_overflow: Overflow,
    pub floating: FloatingSemantics,
}

impl Context {
    pub fn with_options(mut self, options: &crate::compiler_options::CompilerOptions) -> Self {
        self.target = options.effective_target(self.target);
        self.signed_overflow = options.operations.signed_overflow;
        self.floating = options.operations.floating;
        self
    }
    pub fn new(target: TargetInfo) -> Self {
        Self {
            target,
            signed_overflow: Overflow::Undefined,
            floating: FloatingSemantics {
                rounding: Rounding::NearestEven,
                exceptions: Exceptions::Ignore,
            },
        }
    }

    pub fn resolve(&self, expression: &Expr) -> Result<Value, ResolveError> {
        let (ty, kind) = match &expression.value {
            ExprKind::IntegerLiteral(literal) => {
                if literal.suffix.size == IntegerSizeSuffix::BitInt {
                    return Err(ResolveError::Unsupported("bit-precise integer literals"));
                }
                let (width, signed) = integer_candidates(literal)
                    .into_iter()
                    .map(|(rank, signed)| (integer_rank_width(rank, &self.target), signed))
                    .find(|(width, signed)| {
                        *width > 0 && fits_rank(&literal.value, *width, *signed)
                    })
                    .ok_or_else(|| ResolveError::IntegerLiteral(literal.spelling.clone()))?;
                (
                    NumericType::Integer { width, signed },
                    ValueKind::Constant(Number::Integer(literal.value.clone())),
                )
            }
            ExprKind::FloatLiteral(literal) => {
                if literal.suffix == FloatSuffix::F64x {
                    return Err(ResolveError::Unsupported("target-dependent f64x literals"));
                }
                if literal.imaginary {
                    return Err(ResolveError::Unsupported("imaginary literals"));
                }
                let (format, bits) = match resolve_float_literal(literal, &self.target)?.value {
                    FloatValue::Half(bits) => (FloatType::F16, u128::from(bits)),
                    FloatValue::Single(value) => (FloatType::F32, u128::from(value.to_bits())),
                    FloatValue::Double(value) => (FloatType::F64, u128::from(value.to_bits())),
                    FloatValue::Quad(bits) => (FloatType::F128, bits),
                    FloatValue::LongDouble(bits) => (FloatType::F80, bits),
                    _ => {
                        return Err(ResolveError::Unsupported("decimal floating literals"));
                    }
                };
                (
                    NumericType::Float(format),
                    ValueKind::Constant(Number::FloatBits(bits)),
                )
            }
            ExprKind::Paren(inner) => return self.resolve(inner),
            ExprKind::Binary { op, left, right } => {
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
                    _ => return Err(ResolveError::Unsupported(UNSUPPORTED_EXPRESSION)),
                };
                let operator = <&'static str>::from(*op);
                let left = self.resolve(left)?;
                let right = self.resolve(right)?;
                let is_shift = matches!(arith, ArithOp::Shl | ArithOp::Shr);
                if !is_shift && left.ty != right.ty {
                    return Err(ResolveError::Conversion {
                        left: left.ty,
                        operator,
                        right: right.ty,
                    });
                }
                let invalid = ResolveError::InvalidOperands {
                    left: left.ty,
                    operator,
                    right: right.ty,
                };
                let semantics = match (left.ty, arith) {
                    (
                        NumericType::Float(_),
                        ArithOp::Add | ArithOp::Sub | ArithOp::Mul | ArithOp::Div,
                    ) => ArithSema::Floating(self.floating),
                    (NumericType::Float(_), _) => return Err(invalid),
                    (_, ArithOp::Shl | ArithOp::Shr)
                        if matches!(right.ty, NumericType::Float(_)) =>
                    {
                        return Err(invalid);
                    }
                    (
                        NumericType::Integer { signed, .. },
                        ArithOp::Add | ArithOp::Sub | ArithOp::Mul,
                    ) => ArithSema::Integer {
                        overflow: if signed {
                            self.signed_overflow
                        } else {
                            Overflow::Wrap
                        },
                    },
                    // clang and gcc apply -fwrapv/-ftrapv to add, sub, and mul only
                    (
                        NumericType::Integer { signed, .. },
                        ArithOp::Div | ArithOp::Rem | ArithOp::Shl,
                    ) => ArithSema::Integer {
                        overflow: if signed {
                            Overflow::Undefined
                        } else {
                            Overflow::Wrap
                        },
                    },
                    (NumericType::Integer { .. }, ArithOp::And | ArithOp::Or | ArithOp::Xor) => {
                        ArithSema::Bitwise
                    }
                    (NumericType::Integer { signed, .. }, ArithOp::Shr) => ArithSema::ShiftRight {
                        fill: if signed {
                            ShiftFill::SignExtend
                        } else {
                            ShiftFill::ZeroExtend
                        },
                    },
                };
                (
                    left.ty,
                    ValueKind::Arith {
                        op: arith,
                        left: Box::new(left),
                        right: Box::new(right),
                        semantics,
                    },
                )
            }
            ExprKind::Assign { .. } => {
                return Err(ResolveError::Unsupported(
                    "assignment (requires place lowering)",
                ));
            }
            _ => return Err(ResolveError::Unsupported(UNSUPPORTED_EXPRESSION)),
        };
        Ok(Value {
            ty,
            node: Span {
                id: expression.id,
                value: kind,
                spelling: expression.spelling,
                expansion: expression.expansion,
                provenance: expression.provenance,
                macro_origin: expression.macro_origin.clone(),
            },
        })
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
