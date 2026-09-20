use num_bigint::{BigInt, BigUint};
use std::fmt;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Type {
    Void,
    Bool,
    Numeric(NumericType),
    Complex(NumericType),
    Imaginary(FloatType),
    Vector {
        element: NumericType,
        lanes: u32,
    },
    VaList,
    Defined(super::TypeId),
    Pointer {
        pointee: Box<Type>,
        is_const: bool,
        access: super::Access,
    },
    Array {
        element: Box<Type>,
        length: Option<u64>,
    },
    VariableArray {
        element: Box<Type>,
        extent: VariableExtent,
    },
    Function {
        return_type: Option<Box<Type>>,
        parameters: Vec<Type>,
        variadic: bool,
        prototyped: bool,
    },
}

impl fmt::Display for Type {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Void => f.write_str("void"),
            Self::Defined(id) => write!(f, "@type{}", id.0),
            Self::Bool => f.write_str("bool"),
            Self::VaList => f.write_str("va_list"),
            Self::Numeric(ty) => write!(f, "{ty}"),
            Self::Complex(ty) => write!(f, "complex<{ty}>"),
            Self::Imaginary(ty) => write!(f, "imaginary<{ty}>"),
            Self::Vector { element, lanes } => write!(f, "vector<{element}, {lanes}>"),
            Self::Pointer {
                pointee,
                is_const,
                access,
            } => write!(
                f,
                "ptr<{}{}{pointee}>",
                if *is_const { "const " } else { "" },
                access.prefix()
            ),
            Self::Array { element, length } => match length {
                Some(length) => write!(f, "array<{element}, {length}>"),
                None => write!(f, "array<{element}, incomplete>"),
            },
            Self::VariableArray { element, extent } => match extent {
                VariableExtent::Captured(binding) => write!(f, "vla<{element}, %{}>", binding.0),
                VariableExtent::Unspecified => write!(f, "vla<{element}, *>"),
            },
            Self::Function {
                return_type,
                parameters,
                variadic,
                prototyped,
            } => {
                f.write_str("fn(")?;
                if !prototyped {
                    f.write_str("unprototyped")?;
                } else {
                    for (index, parameter) in parameters.iter().enumerate() {
                        if index != 0 {
                            f.write_str(", ")?;
                        }
                        write!(f, "{parameter}")?;
                    }
                    if *variadic {
                        f.write_str(if parameters.is_empty() {
                            "..."
                        } else {
                            ", ..."
                        })?;
                    }
                }
                write!(
                    f,
                    ") -> {}",
                    return_type
                        .as_ref()
                        .map_or("void".to_owned(), ToString::to_string)
                )
            }
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum VariableExtent {
    Captured(super::BindingId),
    Unspecified,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum NumericType {
    Integer {
        width: u32,
        signed: bool,
        bit_precise: bool,
    },
    Float(FloatType),
}

impl NumericType {
    pub fn integer(width: u32, signed: bool) -> Self {
        Self::Integer {
            width,
            signed,
            bit_precise: false,
        }
    }

    pub fn bit_precise(width: u32, signed: bool) -> Self {
        Self::Integer {
            width,
            signed,
            bit_precise: true,
        }
    }
}

impl Type {
    pub fn integer(width: u32, signed: bool) -> Self {
        Self::Numeric(NumericType::integer(width, signed))
    }

    pub fn bit_precise(width: u32, signed: bool) -> Self {
        Self::Numeric(NumericType::bit_precise(width, signed))
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum FloatType {
    F16,
    F32,
    F64,
    F80,
    F128,
    D32,
    D64,
    D128,
}

impl FloatType {
    pub fn is_decimal(self) -> bool {
        matches!(self, Self::D32 | Self::D64 | Self::D128)
    }

    pub fn exact_integer_bits(self) -> u32 {
        match self {
            Self::F16 => 11,
            Self::F32 => 24,
            Self::F64 => 53,
            Self::F80 => 64,
            Self::F128 => 113,
            Self::D32 => 23,
            Self::D64 => 53,
            Self::D128 => 112,
        }
    }

    pub fn widens_from(self, from: Self) -> bool {
        self.is_decimal() == from.is_decimal() && from < self
    }
}

impl Number {
    pub fn float_zero(format: FloatType) -> Self {
        if format.is_decimal() {
            Self::DecimalFloat("0".to_owned())
        } else {
            Self::FloatBits(0)
        }
    }
}

impl fmt::Display for FloatType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::F16 => "f16",
            Self::F32 => "f32",
            Self::F64 => "f64",
            Self::F80 => "f80",
            Self::F128 => "f128",
            Self::D32 => "d32",
            Self::D64 => "d64",
            Self::D128 => "d128",
        })
    }
}

impl fmt::Display for NumericType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Integer {
                width,
                signed,
                bit_precise,
            } => {
                write!(
                    f,
                    "{}{width}{}",
                    if *signed { 'i' } else { 'u' },
                    if *bit_precise { "b" } else { "" }
                )
            }
            Self::Float(format) => write!(f, "{format}"),
        }
    }
}

#[derive(Default, Debug, Clone, Copy, PartialEq, Eq)]
pub enum Overflow {
    #[default]
    Undefined,
    Wrap,
    Trap,
}

#[derive(Default, Debug, Clone, Copy, PartialEq, Eq)]
pub enum Rounding {
    #[default]
    NearestEven,
    Environment,
}

#[derive(Default, Debug, Clone, Copy, PartialEq, Eq)]
pub enum Exceptions {
    #[default]
    Ignore,
    Observable,
}

#[derive(Default, Debug, Clone, Copy, PartialEq, Eq)]
pub struct FloatingSemantics {
    pub rounding: Rounding,
    pub exceptions: Exceptions,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ArithOp {
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    And,
    Or,
    Xor,
    Shl,
    Shr,
}

impl fmt::Display for ArithOp {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Add => "add",
            Self::Sub => "sub",
            Self::Mul => "mul",
            Self::Div => "div",
            Self::Rem => "rem",
            Self::And => "and",
            Self::Or => "or",
            Self::Xor => "xor",
            Self::Shl => "shl",
            Self::Shr => "shr",
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum UnaryArithOp {
    Neg,
    Not,
}

impl fmt::Display for UnaryArithOp {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Neg => "neg",
            Self::Not => "not",
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CompareOp {
    Eq,
    Ne,
    Lt,
    Le,
    Gt,
    Ge,
}

impl fmt::Display for CompareOp {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Eq => "eq",
            Self::Ne => "ne",
            Self::Lt => "lt",
            Self::Le => "le",
            Self::Gt => "gt",
            Self::Ge => "ge",
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum FloatClassTest {
    Nan,
    Infinite,
    Finite,
    Normal,
    Subnormal,
    Zero,
    Signaling,
    SignBit,
}

impl fmt::Display for FloatClassTest {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Nan => "nan",
            Self::Infinite => "infinite",
            Self::Finite => "finite",
            Self::Normal => "normal",
            Self::Subnormal => "subnormal",
            Self::Zero => "zero",
            Self::Signaling => "signaling",
            Self::SignBit => "sign_bit",
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum LogicalOp {
    And,
    Or,
}

impl fmt::Display for LogicalOp {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::And => "logical_and",
            Self::Or => "logical_or",
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ArithSema {
    Integer {
        overflow: Overflow,
    },
    Division {
        by_zero: UbPolicy,
        min_by_neg_one: Option<UbPolicy>,
    },
    ShiftLeft {
        overflow: Overflow,
        amount_out_of_range: UbPolicy,
        negative_left: Option<UbPolicy>,
    },
    Floating(FloatingSemantics),
    ComplexFloating(FloatingSemantics),
    ComplexInteger {
        overflow: Overflow,
        by_zero: Option<UbPolicy>,
    },
    Exact,
    ShiftRight {
        fill: ShiftFill,
        amount_out_of_range: UbPolicy,
    },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum UbPolicy {
    Undefined,
}

impl fmt::Display for UbPolicy {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Undefined => f.write_str("ub"),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ShiftFill {
    SignExtend,
    ZeroExtend,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ConversionKind {
    Widen,
    Truncate,
    Reinterpret,
    BitCast,
    FromBool,
    IntToFloat,
    FloatWiden,
    FloatNarrow,
    FloatConvert,
    FloatToInt,
    PointerCast,
    RealToComplex,
    ComplexToReal,
    ComplexToImag,
    ComplexConvert,
    RealToImaginary,
    ImaginaryToReal,
    ImaginaryToComplex,
    ComplexToImaginary,
    ImaginaryConvert,
    VectorSplat,
    VectorBitCast,
    EnumToInt,
    IntToEnum,
    PtrToInt,
    IntToPtr,
}

impl fmt::Display for ConversionKind {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Widen => "widen",
            Self::Truncate => "truncate",
            Self::Reinterpret => "reinterpret",
            Self::BitCast => "bit_cast",
            Self::FromBool => "from_bool",
            Self::IntToFloat => "int_to_float",
            Self::FloatWiden => "float_widen",
            Self::FloatNarrow => "float_narrow",
            Self::FloatConvert => "float_convert",
            Self::FloatToInt => "float_to_int",
            Self::PointerCast => "pointer_cast",
            Self::RealToComplex => "real_to_complex",
            Self::ComplexToReal => "complex_to_real",
            Self::ComplexToImag => "complex_to_imag",
            Self::ComplexConvert => "complex_convert",
            Self::RealToImaginary => "real_to_imaginary",
            Self::ImaginaryToReal => "imaginary_to_real",
            Self::ImaginaryToComplex => "imaginary_to_complex",
            Self::ComplexToImaginary => "complex_to_imaginary",
            Self::VectorSplat => "vector_splat",
            Self::VectorBitCast => "vector_bit_cast",
            Self::ImaginaryConvert => "imaginary_convert",
            Self::EnumToInt => "enum_to_int",
            Self::IntToEnum => "int_to_enum",
            Self::PtrToInt => "ptr_to_int",
            Self::IntToPtr => "int_to_ptr",
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ConversionReason {
    Return,
    Assign,
    Arg,
    Vararg,
    Promotion,
    UsualArith,
    Explicit,
}

impl fmt::Display for ConversionReason {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Return => "return",
            Self::Assign => "assign",
            Self::Arg => "arg",
            Self::Vararg => "vararg",
            Self::Promotion => "promotion",
            Self::UsualArith => "usual_arith",
            Self::Explicit => "explicit",
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Fits {
    Always,
    Unknown,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ConversionSema {
    Exact,
    Fits(Fits),
    IntToFloat {
        exact: bool,
        floating: FloatingSemantics,
    },
    Floating(FloatingSemantics),
    Exceptions(Exceptions),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Number {
    Bool(bool),
    Integer(BigUint),
    SignedInteger(BigInt),
    FloatBits(u128),
    DecimalFloat(String),
}
