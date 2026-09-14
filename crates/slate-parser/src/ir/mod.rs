mod numeric;

use crate::ast::Span;
pub use numeric::{
    AddSemantics, Exceptions, FloatingSemantics, Number, NumericType, Overflow, Rounding,
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
    Add {
        left: Box<Value>,
        right: Box<Value>,
        semantics: AddSemantics,
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
            ValueKind::Constant(Number::FloatBits(bits)) => {
                write!(f, "const<{}>(bits=0x{bits:x})", self.ty)
            }
            ValueKind::Add {
                left,
                right,
                semantics,
            } => {
                write!(f, "add<{}", self.ty)?;
                match semantics {
                    AddSemantics::Integer { overflow } => write!(
                        f,
                        ", overflow={}",
                        match overflow {
                            Overflow::Undefined => "undefined",
                            Overflow::Wrap => "wrap",
                            Overflow::Trap => "trap",
                        }
                    )?,
                    AddSemantics::Floating(properties) => write!(
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
}
