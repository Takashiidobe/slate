use super::validate::{fits_rank, integer_candidates, integer_rank_width};
use crate::ast::{
    Declarator, Expr, ExprKind, FloatingType, IntegerType, Span, TypeName, TypeSpecifier,
};
use crate::const_expr::{
    BinaryOp, FloatLiteral, FloatSuffix, FloatValue, IntegerSizeSuffix, ResolvedFloat, UnaryOp,
    resolve_float,
};
use crate::ir::{
    ArithOp, ArithSema, CompareOp, ConversionKind, ConversionReason, ConversionSema, Exceptions,
    Fits, FloatType, FloatingSemantics, LogicalOp, Number, NumericType, Overflow, Rounding,
    ShiftFill, Type, UnaryArithOp, Value, ValueKind,
};
use crate::target_info::TargetInfo;
use num_bigint::BigUint;
use thiserror::Error;

#[derive(Debug, Error)]
pub enum ResolveError {
    #[error("unsupported in numeric IR lowering: {0}")]
    Unsupported(&'static str),
    #[error("integer literal `{0}` has no supported target type")]
    IntegerLiteral(String),
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
const UNSUPPORTED_INCREMENT: &str = "increment and decrement (requires place lowering)";

pub struct Context {
    pub target: TargetInfo,
    pub signed_overflow: Overflow,
    pub floating: FloatingSemantics,
}

type Resolved = (Type, ValueKind);

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
                    Type::Numeric(NumericType::Integer { width, signed }),
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
                    Type::Numeric(NumericType::Float(format)),
                    ValueKind::Constant(Number::FloatBits(bits)),
                )
            }
            ExprKind::BoolLiteral(value) => (Type::Bool, ValueKind::Constant(Number::Bool(*value))),
            ExprKind::SizeOfType { ty } => {
                let value = self.storage_of(ty)?.size_bytes;
                (
                    self.size_type(),
                    ValueKind::Constant(Number::Integer(BigUint::from(value))),
                )
            }
            ExprKind::AlignOf { ty } => {
                let value = self.storage_of(ty)?.alignment_bytes;
                (
                    self.size_type(),
                    ValueKind::Constant(Number::Integer(BigUint::from(value))),
                )
            }
            ExprKind::Cast { ty, value } => {
                let converted = self.convert(
                    self.resolve(value)?,
                    self.cast_type(ty)?,
                    ConversionReason::Explicit,
                );
                (converted.ty, converted.node.value)
            }
            ExprKind::Paren(inner) => return self.resolve(inner),
            ExprKind::Binary { op, left, right } => {
                let left = self.resolve(left)?;
                let right = self.resolve(right)?;
                self.resolve_binary(*op, left, right)?
            }
            ExprKind::Unary { op, operand } => match op {
                UnaryOp::Plus => {
                    let operand = self.promote(self.resolve(operand)?);
                    return Ok(operand);
                }
                UnaryOp::Not => (
                    Type::Bool,
                    ValueKind::Unary {
                        op: UnaryArithOp::Not,
                        operand: Box::new(self.condition(self.resolve(operand)?)),
                        semantics: ArithSema::Exact,
                    },
                ),
                UnaryOp::Minus | UnaryOp::BitNot => {
                    self.resolve_unary_arith(*op, self.resolve(operand)?)?
                }
                UnaryOp::PreIncrement | UnaryOp::PreDecrement => {
                    return Err(ResolveError::Unsupported(UNSUPPORTED_INCREMENT));
                }
                _ => return Err(ResolveError::Unsupported(UNSUPPORTED_EXPRESSION)),
            },
            ExprKind::Postfix { .. } => {
                return Err(ResolveError::Unsupported(UNSUPPORTED_INCREMENT));
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

    pub(super) fn resolve_binary(
        &self,
        op: BinaryOp,
        left: Value,
        right: Value,
    ) -> Result<Resolved, ResolveError> {
        let operator = <&'static str>::from(op);
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
            BinaryOp::Equal => return self.compare(CompareOp::Eq, left, right),
            BinaryOp::NotEqual => return self.compare(CompareOp::Ne, left, right),
            BinaryOp::Less => return self.compare(CompareOp::Lt, left, right),
            BinaryOp::LessEqual => return self.compare(CompareOp::Le, left, right),
            BinaryOp::Greater => return self.compare(CompareOp::Gt, left, right),
            BinaryOp::GreaterEqual => return self.compare(CompareOp::Ge, left, right),
            BinaryOp::And => return Ok(self.logical(LogicalOp::And, left, right)),
            BinaryOp::Or => return Ok(self.logical(LogicalOp::Or, left, right)),
        };
        let left = self.promote(left);
        let right = self.promote(right);
        let is_shift = matches!(arith, ArithOp::Shl | ArithOp::Shr);
        let (left, right) = if is_shift {
            (left, right)
        } else {
            self.usual_arithmetic(left, right)
        };
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
            (NumericType::Float(_), _) => return Err(invalid),
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
            // clang and gcc apply -fwrapv/-ftrapv to add, sub, and mul only
            (NumericType::Integer { signed, .. }, ArithOp::Div | ArithOp::Rem | ArithOp::Shl) => {
                ArithSema::Integer {
                    overflow: if signed {
                        Overflow::Undefined
                    } else {
                        Overflow::Wrap
                    },
                }
            }
            (NumericType::Integer { .. }, ArithOp::And | ArithOp::Or | ArithOp::Xor) => {
                ArithSema::Exact
            }
            (NumericType::Integer { signed, .. }, ArithOp::Shr) => ArithSema::ShiftRight {
                fill: if signed {
                    ShiftFill::SignExtend
                } else {
                    ShiftFill::ZeroExtend
                },
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

    pub(super) fn resolve_unary_arith(
        &self,
        op: UnaryOp,
        operand: Value,
    ) -> Result<Resolved, ResolveError> {
        let operand = self.promote(operand);
        let arith = match op {
            UnaryOp::Minus => UnaryArithOp::Neg,
            _ => UnaryArithOp::Not,
        };
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

    fn compare(&self, op: CompareOp, left: Value, right: Value) -> Result<Resolved, ResolveError> {
        let left = self.promote(left);
        let right = self.promote(right);
        let (left, right) = self.usual_arithmetic(left, right);
        let left_ty = numeric(&left)?;
        Ok((Type::Bool, self.comparison(op, left_ty, left, right)))
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

    pub(super) fn cast_type(&self, ty: &TypeName) -> Result<Type, ResolveError> {
        let mut declarator = &ty.declarator;
        while let Declarator::Grouped(inner) = declarator {
            declarator = inner;
        }
        if !matches!(declarator, Declarator::Abstract) {
            return Err(ResolveError::Unsupported("non-scalar cast type"));
        }
        let numeric = match &ty.specifiers.ty {
            TypeSpecifier::Bool => return Ok(Type::Bool),
            TypeSpecifier::Integer(IntegerType::Char { signed }) => NumericType::Integer {
                width: 8,
                signed: signed.unwrap_or(self.target.char_signed),
            },
            TypeSpecifier::Integer(IntegerType::Ranked { rank, signed }) => NumericType::Integer {
                width: integer_rank_width(*rank, &self.target),
                signed: *signed,
            },
            TypeSpecifier::Floating(ty) => NumericType::Float(match ty {
                FloatingType::Float16 | FloatingType::Fp16 => FloatType::F16,
                FloatingType::Float => FloatType::F32,
                FloatingType::Double => FloatType::F64,
                FloatingType::Float128 | FloatingType::Float128Ext => FloatType::F128,
                FloatingType::LongDouble => match self.target.long_double {
                    crate::target_info::LongDoubleFormat::Binary64 => FloatType::F64,
                    crate::target_info::LongDoubleFormat::X87 => FloatType::F80,
                    crate::target_info::LongDoubleFormat::Binary128 => FloatType::F128,
                },
                _ => return Err(ResolveError::Unsupported("floating cast type")),
            }),
            _ => return Err(ResolveError::Unsupported("cast type")),
        };
        Ok(Type::Numeric(numeric))
    }

    fn storage_of(&self, ty: &TypeName) -> Result<crate::target_info::StorageLayout, ResolveError> {
        Ok(self.target.storage_of(self.cast_type(ty)?)?)
    }

    fn size_type(&self) -> Type {
        Type::Numeric(NumericType::Integer {
            width: self.target.long_width,
            signed: false,
        })
    }

    pub(super) fn promote(&self, value: Value) -> Value {
        match value.ty {
            Type::Bool => self.convert(value, self.int_type(), ConversionReason::Promotion),
            Type::Numeric(NumericType::Integer { width, .. }) if width < self.target.int_width => {
                self.convert(value, self.int_type(), ConversionReason::Promotion)
            }
            _ => value,
        }
    }

    pub(super) fn usual_arithmetic(&self, left: Value, right: Value) -> (Value, Value) {
        let ty = match (left.ty.clone(), right.ty.clone()) {
            (Type::Numeric(NumericType::Float(a)), Type::Numeric(NumericType::Float(b))) => {
                Type::Numeric(NumericType::Float(a.max(b)))
            }
            (ty @ Type::Numeric(NumericType::Float(_)), _)
            | (_, ty @ Type::Numeric(NumericType::Float(_))) => ty,
            (
                Type::Numeric(NumericType::Integer {
                    width: a,
                    signed: sa,
                }),
                Type::Numeric(NumericType::Integer {
                    width: b,
                    signed: sb,
                }),
            ) => Type::Numeric(NumericType::Integer {
                width: a.max(b),
                signed: if a == b {
                    sa && sb
                } else if a > b {
                    sa
                } else {
                    sb
                },
            }),
            _ => return (left, right),
        };
        (
            self.convert(left, ty.clone(), ConversionReason::UsualArith),
            self.convert(right, ty, ConversionReason::UsualArith),
        )
    }

    pub(super) fn int_type(&self) -> Type {
        Type::Numeric(NumericType::Integer {
            width: self.target.int_width,
            signed: true,
        })
    }

    pub(super) fn convert(&self, value: Value, to: Type, reason: ConversionReason) -> Value {
        if value.ty == to {
            return value;
        }
        if to == Type::Bool {
            let mut value = self.condition(value);
            if let ValueKind::Compare { reason: why, .. } = &mut value.node.value {
                *why = Some(reason);
            }
            return value;
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
            return self.convert(value, to, reason);
        }
        match (value.ty.clone(), to.clone()) {
            (
                Type::Numeric(NumericType::Integer {
                    width: from_width,
                    signed: from_signed,
                }),
                Type::Numeric(NumericType::Integer { width, signed }),
            ) => {
                let mut value = value;
                if from_width != width {
                    let intermediate = Type::Numeric(NumericType::Integer {
                        width,
                        signed: from_signed,
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
                }
                value
            }
            (
                Type::Numeric(NumericType::Integer { width, signed }),
                Type::Numeric(NumericType::Float(format)),
            ) => {
                let precision = match format {
                    FloatType::F16 => 11,
                    FloatType::F32 => 24,
                    FloatType::F64 => 53,
                    FloatType::F80 => 64,
                    FloatType::F128 => 113,
                };
                let exact = width - u32::from(signed) <= precision
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
                let (kind, semantics) = if from < to_format {
                    (ConversionKind::FloatWiden, ConversionSema::Exact)
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
        }
    }

    pub(super) fn condition(&self, value: Value) -> Value {
        let Type::Numeric(ty) = value.ty else {
            return value;
        };
        let zero = match ty {
            NumericType::Integer { .. } => Number::Integer(BigUint::default()),
            NumericType::Float(_) => Number::FloatBits(0),
        };
        let node = derived_span(&value.node, ValueKind::Constant(zero));
        let zero = Value {
            ty: value.ty.clone(),
            node: node.clone(),
        };
        Value {
            ty: Type::Bool,
            node: Span {
                value: self.comparison(CompareOp::Ne, ty, value, zero),
                ..node
            },
        }
    }
}

fn numeric(value: &Value) -> Result<NumericType, ResolveError> {
    match value.ty {
        Type::Numeric(ty) => Ok(ty),
        Type::Bool => Err(ResolveError::Unsupported("unpromoted boolean operand")),
        Type::Defined(_)
        | Type::Pointer { .. }
        | Type::Array { .. }
        | Type::Function { .. }
        | Type::Void => Err(ResolveError::Unsupported("non-numeric operand")),
    }
}

fn derived_span(node: &Span<ValueKind>, value: ValueKind) -> Span<ValueKind> {
    Span {
        id: node.id,
        value,
        spelling: node.spelling,
        expansion: node.expansion,
        provenance: node.provenance,
        macro_origin: node.macro_origin.clone(),
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
    let node = Span {
        id: value.node.id,
        spelling: value.node.spelling,
        expansion: value.node.expansion,
        provenance: value.node.provenance,
        macro_origin: value.node.macro_origin.clone(),
        value: ValueKind::Convert {
            kind,
            operand: Box::new(value),
            reason,
            semantics,
        },
    };
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
