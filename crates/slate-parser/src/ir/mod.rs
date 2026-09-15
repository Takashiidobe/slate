mod numeric;

use crate::ast::Span;
pub use numeric::{
    ArithOp, ArithSema, Exceptions, FloatType, FloatingSemantics, Number, NumericType, Overflow,
    Rounding,
};
use rustc_apfloat::{
    Float,
    ieee::{Half, Quad, X87DoubleExtended},
};
use std::fmt;

#[derive(Debug, Clone)]
pub struct Value {
    pub ty: NumericType,
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
            ValueKind::Constant(Number::Integer(value)) => write!(f, "const<{}>({value})", self.ty),
            ValueKind::Constant(Number::FloatBits(bits)) => match self.ty {
                NumericType::Float(FloatType::F32) if !f32::from_bits(*bits as u32).is_nan() => {
                    write!(f, "const<{}>({:?})", self.ty, f32::from_bits(*bits as u32))
                }
                NumericType::Float(FloatType::F64) if !f64::from_bits(*bits as u64).is_nan() => {
                    write!(f, "const<{}>({:?})", self.ty, f64::from_bits(*bits as u64))
                }
                NumericType::Float(FloatType::F16) => format_apfloat::<Half>(f, self.ty, *bits),
                NumericType::Float(FloatType::F80) => {
                    format_apfloat::<X87DoubleExtended>(f, self.ty, *bits)
                }
                NumericType::Float(FloatType::F128) => format_apfloat::<Quad>(f, self.ty, *bits),
                _ => write!(f, "const<{}>(bits=0x{bits:x})", self.ty),
            },
            ValueKind::Arith {
                op,
                left,
                right,
                semantics,
            } => self.format_arithmetic(f, *op, left, right, *semantics, show_spans),
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

    fn format_arithmetic(
        &self,
        f: &mut fmt::Formatter<'_>,
        op: ArithOp,
        left: &Value,
        right: &Value,
        semantics: ArithSema,
        show_spans: bool,
    ) -> fmt::Result {
        write!(f, "{op}<{}", self.ty)?;
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
                match properties.exceptions {
                    Exceptions::Ignore => "ignore",
                    Exceptions::Observable => "observable",
                }
            )?,
        }
        write!(
            f,
            ">({}, {})",
            left.display(show_spans),
            right.display(show_spans)
        )
    }
}

fn format_apfloat<T: Float>(
    f: &mut fmt::Formatter<'_>,
    ty: NumericType,
    bits: u128,
) -> fmt::Result {
    let value = T::from_bits(bits);
    if value.is_nan() {
        write!(f, "const<{ty}>(bits=0x{bits:x})")
    } else {
        write!(f, "const<{ty}>({value})")
    }
}
