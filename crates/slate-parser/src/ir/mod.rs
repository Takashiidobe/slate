mod abi;
mod atomic;
mod declarations;
mod module;
mod module_print;
mod names;
mod numeric;

pub use abi::{AbiChunk, AbiConvention, AbiPass, AbiSignature};
pub use atomic::{CompareExchangeForm, FenceScope, MemoryOrder, Weakness};
pub use declarations::{
    Access, AggregateMember, AggregateTarget, ArrayExtent, ArrayParameter, BitFieldAccess,
    BitFieldUnit, Enumerator, Field, Global, Parameter, Parameters, Place, PlaceKind, RecordKind,
    RecordLayout, StorageDuration, TypeDefinition, TypeDefinitionKind, TypeId, Variable,
};
pub use module::{
    DllStorage, Evaluation, Fallthrough, Function, FunctionSemantics, Inlining, Linkage, Metadata,
    Module, Statement, SymbolAttributes, TlsModel, Visibility,
};

pub use names::{Binding, BindingId, BindingKind, NameResolution, Reference};

use crate::ast::Span;
pub use numeric::{
    ArithOp, ArithSema, CompareOp, ConversionKind, ConversionReason, ConversionSema, Exceptions,
    Fits, FloatType, FloatingSemantics, LogicalOp, Number, NumericType, Overflow, Rounding,
    ShiftFill, Type, UbPolicy, UnaryArithOp, VariableExtent,
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
pub enum Callee {
    Direct(BindingId),
    Builtin(String),
    Indirect(Box<Value>),
}

#[derive(Debug, Clone)]
pub enum ValueKind {
    Constant(Number),
    Copy {
        operand: Box<Value>,
        reason: ConversionReason,
    },
    Null,
    LabelAddress(BindingId),
    Void,
    CodeUnits(Vec<u32>),
    Aggregate {
        members: Vec<AggregateMember>,
        zero_fill: bool,
    },
    ArrayDecay {
        place: Place,
        length: Option<u64>,
    },
    FunctionDecay {
        place: Place,
    },
    Store {
        place: Place,
        value: Box<Value>,
        ordering: Option<MemoryOrder>,
    },
    Update {
        place: Place,
        computation: Box<Value>,
        postfix: bool,
        ordering: Option<MemoryOrder>,
    },
    CompareExchange {
        place: Place,
        expected: Box<Value>,
        desired: Box<Value>,
        success: MemoryOrder,
        failure: MemoryOrder,
        weak: Weakness,
        form: CompareExchangeForm,
    },
    Fence {
        ordering: MemoryOrder,
        scope: FenceScope,
    },
    OldValue,
    PointerOffset {
        pointer: Box<Value>,
        amount: Box<Value>,
        subtract: bool,
        element: Type,
        overflow: Overflow,
    },
    PointerDifference {
        left: Box<Value>,
        right: Box<Value>,
        element: Type,
    },
    Conditional {
        condition: Box<Value>,
        then_value: Box<Value>,
        else_value: Box<Value>,
    },
    Sequence {
        left: Box<Value>,
        right: Box<Value>,
    },
    Call {
        callee: Callee,
        signature: Type,
        abi: AbiSignature,
        arguments: Vec<Value>,
    },
    Read {
        place: Place,
        ordering: Option<MemoryOrder>,
    },
    AddressOf(Place),
    VaArg {
        list: Place,
    },
    StatementExpression(Box<Evaluation>),
    Capture {
        id: BindingId,
        extent: Box<Value>,
        value: Box<Value>,
    },
    VaStart {
        list: Place,
    },
    VaEnd {
        list: Place,
    },
    VaCopy {
        destination: Place,
        source: Place,
    },
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
    Overflow {
        op: ArithOp,
        left: Box<Value>,
        right: Box<Value>,
        result: Place,
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
        reason: Option<ConversionReason>,
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
            ValueKind::Copy { operand, reason } => write!(
                f,
                "copy<{}, reason={}>({})",
                self.ty,
                reason,
                operand
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact)
            ),
            ValueKind::CodeUnits(units) => write!(f, "code_units<{}>({units:?})", self.ty),
            ValueKind::Aggregate { members, zero_fill } => {
                write!(f, "aggregate<{}, zero_fill={zero_fill}>(", self.ty)?;
                for (position, member) in members.iter().enumerate() {
                    if position > 0 {
                        f.write_str(", ")?;
                    }
                    write!(
                        f,
                        "{} = {}",
                        member.target,
                        member
                            .value
                            .display_metadata(show_spans, metadata)
                            .with_compact(compact)
                    )?;
                }
                f.write_str(")")
            }
            ValueKind::ArrayDecay { place, length } => {
                write!(
                    f,
                    "array_decay<{}, length={length:?}>({})",
                    self.ty,
                    place.display_mode(compact)
                )
            }
            ValueKind::FunctionDecay { place } => {
                write!(
                    f,
                    "function_decay<{}>({})",
                    self.ty,
                    place.display_mode(compact)
                )
            }
            ValueKind::OldValue => write!(f, "old<{}>", self.ty),
            ValueKind::PointerOffset {
                pointer,
                amount,
                subtract,
                element,
                overflow,
            } => {
                write!(f, "ptr_offset<{}, subtract={subtract}", self.ty)?;
                if !compact {
                    write!(f, ", element={element}")?;
                    format_overflow(f, *overflow)?;
                }
                write!(
                    f,
                    ">({}, {})",
                    pointer
                        .display_metadata(show_spans, metadata)
                        .with_compact(compact),
                    amount
                        .display_metadata(show_spans, metadata)
                        .with_compact(compact)
                )
            }
            ValueKind::PointerDifference {
                left,
                right,
                element,
            } => {
                write!(f, "ptr_diff<{}", self.ty)?;
                if !compact {
                    write!(f, ", element={element}, same_array=required, overflow=ub")?;
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
            ValueKind::Store {
                place,
                value,
                ordering,
            } => {
                write!(f, "store<{}{}", self.ty, place.access)?;
                atomic::format_ordering(f, ordering.as_ref(), compact)?;
                write!(
                    f,
                    ">({}, {})",
                    place.display_mode(compact),
                    value
                        .display_metadata(show_spans, metadata)
                        .with_compact(compact)
                )
            }
            ValueKind::Update {
                place,
                computation,
                postfix,
                ordering,
            } => {
                write!(
                    f,
                    "update<{}, result={}{}",
                    self.ty,
                    if *postfix { "old" } else { "new" },
                    place.access
                )?;
                atomic::format_ordering(f, ordering.as_ref(), compact)?;
                write!(
                    f,
                    ">({}, {})",
                    place.display_mode(compact),
                    computation
                        .display_metadata(show_spans, metadata)
                        .with_compact(compact)
                )
            }
            ValueKind::CompareExchange {
                place,
                expected,
                desired,
                success,
                failure,
                weak,
                form,
            } => write!(
                f,
                "compare_exchange<{}{}, form={form}, weak={}, success={}, failure={}>({}, {}, {})",
                place.ty,
                place.access,
                weak.display_mode(compact),
                success.display_mode(compact),
                failure.display_mode(compact),
                place.display_mode(compact),
                expected
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact),
                desired
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact)
            ),
            ValueKind::Fence { ordering, scope } => write!(
                f,
                "fence<scope={scope}, order={}>",
                ordering.display_mode(compact)
            ),
            ValueKind::Conditional {
                condition,
                then_value,
                else_value,
            } => write!(
                f,
                "conditional<{}>({}, {}, {})",
                self.ty,
                condition
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact),
                then_value
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact),
                else_value
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact)
            ),
            ValueKind::Sequence { left, right } => write!(
                f,
                "sequence<{}>({}, {})",
                self.ty,
                left.display_metadata(show_spans, metadata)
                    .with_compact(compact),
                right
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact)
            ),
            ValueKind::Call {
                callee,
                signature,
                abi,
                arguments,
            } => {
                write!(f, "call<{}", self.ty)?;
                if !compact {
                    write!(f, ", signature={signature}")?;
                }
                if abi.has_nontrivial_pass() {
                    write!(f, ", abi={abi}")?;
                }
                f.write_str(">(")?;
                match callee {
                    Callee::Direct(id) => write!(f, "%{}", id.0)?,
                    Callee::Builtin(name) => f.write_str(name)?,
                    Callee::Indirect(value) => write!(
                        f,
                        "{}",
                        value
                            .display_metadata(show_spans, metadata)
                            .with_compact(compact)
                    )?,
                }
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
            ValueKind::LabelAddress(id) => write!(f, "label_addr<{}>(%{})", self.ty, id.0),
            ValueKind::Void => f.write_str("void"),
            ValueKind::Null => write!(f, "null<{}>", self.ty),
            ValueKind::Read { place, ordering } => {
                write!(f, "read<{}{}", place.ty, place.access)?;
                atomic::format_ordering(f, ordering.as_ref(), compact)?;
                write!(f, ">({})", place.display_mode(compact))
            }
            ValueKind::VaArg { list } => {
                write!(f, "va_arg<{}>({})", self.ty, list.display_mode(compact))
            }
            ValueKind::StatementExpression(evaluation) => write!(
                f,
                "statement_expression<{}, statements={}>({})",
                self.ty,
                evaluation.statements.len(),
                evaluation
                    .value
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact)
            ),
            ValueKind::Capture { id, extent, value } => write!(
                f,
                "capture<%{}>({}, {})",
                id.0,
                extent
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact),
                value
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact)
            ),
            ValueKind::VaStart { list } => write!(f, "va_start({})", list.display_mode(compact)),
            ValueKind::VaEnd { list } => write!(f, "va_end({})", list.display_mode(compact)),
            ValueKind::VaCopy {
                destination,
                source,
            } => write!(
                f,
                "va_copy({}, {})",
                destination.display_mode(compact),
                source.display_mode(compact)
            ),
            ValueKind::AddressOf(place) => {
                write!(f, "addr_of<{}>({})", self.ty, place.display_mode(compact))
            }
            ValueKind::Constant(Number::Bool(value)) => write!(f, "const<{}>({value})", self.ty),
            ValueKind::Constant(Number::Integer(value)) => write!(f, "const<{}>({value})", self.ty),
            ValueKind::Constant(Number::SignedInteger(value)) => {
                write!(f, "const<{}>({value})", self.ty)
            }
            ValueKind::Constant(Number::DecimalFloat(digits)) => {
                write!(f, "const<{}>({digits})", self.ty)
            }
            ValueKind::Constant(Number::FloatBits(bits)) => match self.ty {
                Type::Numeric(NumericType::Float(format)) | Type::Imaginary(format) => match format
                {
                    FloatType::F32 if !f32::from_bits(*bits as u32).is_nan() => {
                        write!(f, "const<{}>({:?})", self.ty, f32::from_bits(*bits as u32))
                    }
                    FloatType::F64 if !f64::from_bits(*bits as u64).is_nan() => {
                        write!(f, "const<{}>({:?})", self.ty, f64::from_bits(*bits as u64))
                    }
                    FloatType::F16 => format_apfloat::<Half>(f, self.ty.clone(), *bits),
                    FloatType::F80 => {
                        format_apfloat::<X87DoubleExtended>(f, self.ty.clone(), *bits)
                    }
                    FloatType::F128 => format_apfloat::<Quad>(f, self.ty.clone(), *bits),
                    _ => write!(f, "const<{}>(bits=0x{bits:x})", self.ty),
                },
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
            ValueKind::Overflow {
                op,
                left,
                right,
                result,
            } => write!(
                f,
                "overflow_{op}<{}>({}, {}, {result})",
                self.ty,
                left.display_metadata(show_spans, metadata)
                    .with_compact(compact),
                right
                    .display_metadata(show_spans, metadata)
                    .with_compact(compact)
            ),
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
                reason,
            } => {
                write!(f, "{op}<{}", left.ty)?;
                if let Type::Vector { .. } = self.ty {
                    write!(f, ", result={}", self.ty)?;
                }
                if let Some(reason) = reason.filter(|_| !compact) {
                    write!(f, ", reason={reason}")?;
                }
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
        if let Type::Vector { .. } = self.ty {
            f.write_str(", elementwise=true")?;
        }
        match semantics {
            ArithSema::Integer { overflow } => format_overflow(f, overflow)?,
            ArithSema::Division {
                by_zero,
                min_by_neg_one,
            } => {
                write!(f, ", by_zero={by_zero}")?;
                if let Some(policy) = min_by_neg_one {
                    write!(f, ", min_by_neg_one={policy}")?;
                }
            }
            ArithSema::ShiftLeft {
                overflow,
                amount_out_of_range,
                negative_left,
            } => {
                format_overflow(f, overflow)?;
                write!(f, ", amount_out_of_range={amount_out_of_range}")?;
                if let Some(policy) = negative_left {
                    write!(f, ", negative_left={policy}")?;
                }
            }
            ArithSema::Floating(properties) => write!(
                f,
                ", rounding={}, exceptions={}",
                match properties.rounding {
                    Rounding::NearestEven => "nearest_even",
                    Rounding::Environment => "environment",
                },
                exceptions_name(properties.exceptions)
            )?,
            ArithSema::ComplexFloating(properties) => write!(
                f,
                ", complex=true, rounding={}, exceptions={}",
                match properties.rounding {
                    Rounding::NearestEven => "nearest_even",
                    Rounding::Environment => "environment",
                },
                exceptions_name(properties.exceptions)
            )?,
            ArithSema::ComplexInteger { overflow, by_zero } => {
                f.write_str(", complex=true")?;
                format_overflow(f, overflow)?;
                if let Some(policy) = by_zero {
                    write!(f, ", by_zero={policy}")?;
                }
            }
            ArithSema::Exact => {}
            ArithSema::ShiftRight {
                fill,
                amount_out_of_range,
            } => write!(
                f,
                ", amount_out_of_range={amount_out_of_range}, fill={}",
                match fill {
                    ShiftFill::SignExtend => "sign_extend",
                    ShiftFill::ZeroExtend => "zero_extend",
                }
            )?,
        }
        f.write_str(">")
    }
}

fn format_overflow(f: &mut fmt::Formatter<'_>, overflow: Overflow) -> fmt::Result {
    write!(
        f,
        ", overflow={}",
        match overflow {
            Overflow::Undefined => "ub",
            Overflow::Wrap => "wrap",
            Overflow::Trap => "trap",
        }
    )
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
