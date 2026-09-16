mod declarations;
mod module;
mod module_print;
mod names;
mod numeric;

pub use declarations::{
    BitFieldUnit, Enumerator, Field, Global, Parameter, Parameters, Place, RecordKind,
    RecordLayout, StorageDuration, TypeDefinition, TypeDefinitionKind, TypeId, Variable,
};
pub use module::{Function, Linkage, Metadata, Module, Statement};

pub use names::{Binding, BindingId, BindingKind, NameResolution, Reference};

use crate::ast::Span;
pub use numeric::{
    ArithOp, ArithSema, CompareOp, ConversionKind, ConversionReason, ConversionSema, Exceptions,
    Fits, FloatType, FloatingSemantics, LogicalOp, Number, NumericType, Overflow, Rounding,
    ShiftFill, Type, UnaryArithOp,
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
    Null,
    Bytes(Vec<u8>),
    ArrayDecay(Place),
    Call {
        function: BindingId,
        arguments: Vec<Value>,
    },
    Read(Place),
    AddressOf(Place),
    Convert {
        kind: ConversionKind,
        operand: Box<Value>,
        reason: ConversionReason,
        semantics: ConversionSema,
    },
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
        self.format(f, false, None, false)
    }
}

pub struct DisplayValue<'a> {
    value: &'a Value,
    show_spans: bool,
    metadata: Option<&'a Metadata>,
    compact: bool,
}

impl fmt::Display for DisplayValue<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.value
            .format(f, self.show_spans, self.metadata, self.compact)
    }
}

impl DisplayValue<'_> {
    pub(crate) fn with_compact(mut self, compact: bool) -> Self {
        self.compact = compact;
        self
    }
}

impl Value {
    pub fn display(&self, show_spans: bool) -> DisplayValue<'_> {
        DisplayValue {
            value: self,
            show_spans,
            metadata: None,
            compact: false,
        }
    }

    pub fn display_metadata<'a>(
        &'a self,
        show_spans: bool,
        metadata: Option<&'a Metadata>,
    ) -> DisplayValue<'a> {
        DisplayValue {
            value: self,
            show_spans,
            metadata,
            compact: false,
        }
    }

    fn format(
        &self,
        f: &mut fmt::Formatter<'_>,
        show_spans: bool,
        metadata: Option<&Metadata>,
        compact: bool,
    ) -> fmt::Result {
        match &self.node.value {
            ValueKind::Bytes(bytes) => write!(f, "bytes<{}>({bytes:?})", self.ty),
            ValueKind::ArrayDecay(place) => {
                write!(f, "array_decay<{}>(%{})", self.ty, place.binding.0)
            }
            ValueKind::Call {
                function,
                arguments,
            } => {
                write!(f, "call<{}>(%{}", self.ty, function.0)?;
                for argument in arguments {
                    write!(
                        f,
                        ", {}",
                        argument
                            .display_metadata(show_spans, metadata)
                            .with_compact(compact)
                    )?;
                }
                f.write_str(")")
            }
            ValueKind::Null => write!(f, "null<{}>", self.ty),
            ValueKind::Read(place) => write!(f, "read<{}>(%{})", place.ty, place.binding.0),
            ValueKind::AddressOf(place) => write!(f, "addr_of<{}>(%{})", self.ty, place.binding.0),
            ValueKind::Constant(Number::Bool(value)) => write!(f, "const<{}>({value})", self.ty),
            ValueKind::Constant(Number::Integer(value)) => write!(f, "const<{}>({value})", self.ty),
            ValueKind::Constant(Number::SignedInteger(value)) => {
                write!(f, "const<{}>({value})", self.ty)
            }
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
            ValueKind::Convert {
                kind,
                operand,
                reason,
                semantics,
            } => {
                write!(f, "{kind}<{}", self.ty)?;
                if !compact {
                    write!(f, ", reason={reason}")?;
                }
                match semantics {
                    _ if compact => {}
                    ConversionSema::Exact => {}
                    ConversionSema::Fits(fits) => write!(
                        f,
                        ", fits={}",
                        match fits {
                            Fits::Always => "always",
                            Fits::Unknown => "unknown",
                        }
                    )?,
                    ConversionSema::IntToFloat { exact, floating } => {
                        write!(f, ", exact={exact}")?;
                        format_floating(f, *floating)?;
                    }
                    ConversionSema::Floating(floating) => format_floating(f, *floating)?,
                    ConversionSema::Exceptions(exceptions) => write!(
                        f,
                        ", out_of_range=ub, exceptions={}",
                        exceptions_name(*exceptions)
                    )?,
                }
                write!(
                    f,
                    ">({})",
                    operand
                        .display_metadata(show_spans, metadata)
                        .with_compact(compact)
                )
            }
            ValueKind::Arith {
                op,
                left,
                right,
                semantics,
            } => {
                write!(f, "{op}")?;
                self.format_semantics(f, *semantics, compact)?;
                write!(
                    f,
                    "({}, {})",
                    left.display_metadata(show_spans, metadata)
                        .with_compact(compact),
                    right
                        .display_metadata(show_spans, metadata)
                        .with_compact(compact)
                )
            }
            ValueKind::Unary {
                op,
                operand,
                semantics,
            } => {
                write!(f, "{op}")?;
                self.format_semantics(f, *semantics, compact)?;
                write!(
                    f,
                    "({})",
                    operand
                        .display_metadata(show_spans, metadata)
                        .with_compact(compact)
                )
            }
            ValueKind::Compare {
                op,
                left,
                right,
                exceptions,
            } => {
                write!(f, "{op}<{}", left.ty)?;
                if let Some(exceptions) = exceptions.filter(|_| !compact) {
                    write!(f, ", exceptions={}", exceptions_name(exceptions))?;
                }
                write!(
                    f,
                    ">({}, {})",
                    left.display_metadata(show_spans, metadata)
                        .with_compact(compact),
                    right
                        .display_metadata(show_spans, metadata)
                        .with_compact(compact)
                )
            }
            ValueKind::Logical { op, left, right } => write!(
                f,
                "{op}<{}>({}, {})",
                self.ty,
                left.display_metadata(show_spans, metadata)
                    .with_compact(compact),
                right
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact)
            ),
        }?;
        module_print::metadata(f, metadata, self.node.id)?;
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

    fn format_semantics(
        &self,
        f: &mut fmt::Formatter<'_>,
        semantics: ArithSema,
        compact: bool,
    ) -> fmt::Result {
        write!(f, "<{}", self.ty)?;
        if compact {
            return f.write_str(">");
        }
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

fn format_floating(f: &mut fmt::Formatter<'_>, floating: FloatingSemantics) -> fmt::Result {
    write!(
        f,
        ", rounding={}, exceptions={}",
        match floating.rounding {
            Rounding::NearestEven => "nearest_even",
            Rounding::Environment => "environment",
        },
        exceptions_name(floating.exceptions)
    )
}
