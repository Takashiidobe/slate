mod numeric;

use crate::ast::Span;
pub use numeric::{
    ArithOp, ArithSema, CompareOp, Exceptions, FloatType, FloatingSemantics, LogicalOp, Number,
    NumericType, Overflow, Rounding, ShiftFill, Type, UnaryArithOp,
};
use rustc_apfloat::{
    Float,
    ieee::{Half, Quad, X87DoubleExtended},
};
use std::fmt;

#[derive(Debug, Clone)]
pub struct Value {
    pub ty: Type,
    pub node: Span<ValueKind>,
}

#[derive(Debug, Clone)]
pub enum ValueKind {
    Constant(Number),
    Arith {
        op: ArithOp,
        left: Box<Value>,
        right: Box<Value>,
        semantics: ArithSema,
    },
    Unary {
        op: UnaryArithOp,
        operand: Box<Value>,
        semantics: ArithSema,
    },
    Compare {
        op: CompareOp,
        left: Box<Value>,
        right: Box<Value>,
        exceptions: Option<Exceptions>,
    },
    Logical {
        op: LogicalOp,
        left: Box<Value>,
        right: Box<Value>,
    },
}

impl fmt::Display for Value {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.format(f, false)
    }
}

pub struct DisplayValue<'a> {
    value: &'a Value,
    show_spans: bool,
}

impl fmt::Display for DisplayValue<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.value.format(f, self.show_spans)
    }
}

impl Value {
    pub fn display(&self, show_spans: bool) -> DisplayValue<'_> {
        DisplayValue {
            value: self,
            show_spans,
        }
    }

    fn format(&self, f: &mut fmt::Formatter<'_>, show_spans: bool) -> fmt::Result {
        match &self.node.value {
            ValueKind::Constant(Number::Bool(value)) => write!(f, "const<{}>({value})", self.ty),
            ValueKind::Constant(Number::Integer(value)) => write!(f, "const<{}>({value})", self.ty),
            ValueKind::Constant(Number::FloatBits(bits)) => match self.ty {
                Type::Numeric(NumericType::Float(FloatType::F32))
                    if !f32::from_bits(*bits as u32).is_nan() =>
                {
                    write!(f, "const<{}>({:?})", self.ty, f32::from_bits(*bits as u32))
                }
                Type::Numeric(NumericType::Float(FloatType::F64))
                    if !f64::from_bits(*bits as u64).is_nan() =>
                {
                    write!(f, "const<{}>({:?})", self.ty, f64::from_bits(*bits as u64))
                }
                Type::Numeric(NumericType::Float(FloatType::F16)) => {
                    format_apfloat::<Half>(f, self.ty, *bits)
                }
                Type::Numeric(NumericType::Float(FloatType::F80)) => {
                    format_apfloat::<X87DoubleExtended>(f, self.ty, *bits)
                }
                Type::Numeric(NumericType::Float(FloatType::F128)) => {
                    format_apfloat::<Quad>(f, self.ty, *bits)
                }
                _ => write!(f, "const<{}>(bits=0x{bits:x})", self.ty),
            },
            ValueKind::Arith {
                op,
                left,
                right,
                semantics,
            } => {
                write!(f, "{op}")?;
                self.format_semantics(f, *semantics)?;
                write!(
                    f,
                    "({}, {})",
                    left.display(show_spans),
                    right.display(show_spans)
                )
            }
            ValueKind::Unary {
                op,
                operand,
                semantics,
            } => {
                write!(f, "{op}")?;
                self.format_semantics(f, *semantics)?;
                write!(f, "({})", operand.display(show_spans))
            }
            ValueKind::Compare {
                op,
                left,
                right,
                exceptions,
            } => {
                write!(f, "{op}<{}", left.ty)?;
                if let Some(exceptions) = exceptions {
                    write!(f, ", exceptions={}", exceptions_name(*exceptions))?;
                }
                write!(
                    f,
                    ">({}, {})",
                    left.display(show_spans),
                    right.display(show_spans)
                )
            }
            ValueKind::Logical { op, left, right } => write!(
                f,
                "{op}<{}>({}, {})",
                self.ty,
                left.display(show_spans),
                right.display(show_spans)
            ),
        }?;
        if show_spans {
            let spelling = self.node.spelling;
            let expansion = self.node.expansion;
            write!(
                f,
                " [spelling={}:{}+{}, expansion={}:{}+{}]",
                spelling.file.0,
                spelling.offset,
                spelling.length,
                expansion.file.0,
                expansion.offset,
                expansion.length
            )?;
        }
        Ok(())
    }

    fn format_semantics(&self, f: &mut fmt::Formatter<'_>, semantics: ArithSema) -> fmt::Result {
        write!(f, "<{}", self.ty)?;
        match semantics {
            ArithSema::Integer { overflow } => write!(
                f,
                ", overflow={}",
                match overflow {
                    Overflow::Undefined => "undefined",
                    Overflow::Wrap => "wrap",
                    Overflow::Trap => "trap",
                }
            )?,
            ArithSema::Floating(properties) => write!(
                f,
                ", rounding={}, exceptions={}",
                match properties.rounding {
                    Rounding::NearestEven => "nearest_even",
                    Rounding::Environment => "environment",
                },
                exceptions_name(properties.exceptions)
            )?,
            ArithSema::Exact => {}
            ArithSema::ShiftRight { fill } => write!(
                f,
                ", fill={}",
                match fill {
                    ShiftFill::SignExtend => "sign_extend",
                    ShiftFill::ZeroExtend => "zero_extend",
                }
            )?,
        }
        f.write_str(">")
    }
}

fn exceptions_name(exceptions: Exceptions) -> &'static str {
    match exceptions {
        Exceptions::Ignore => "ignore",
        Exceptions::Observable => "observable",
    }
}

fn format_apfloat<T: Float>(f: &mut fmt::Formatter<'_>, ty: Type, bits: u128) -> fmt::Result {
    let value = T::from_bits(bits);
    if value.is_nan() {
        write!(f, "const<{ty}>(bits=0x{bits:x})")
    } else {
        write!(f, "const<{ty}>({value})")
    }
}
