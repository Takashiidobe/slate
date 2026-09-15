use num_bigint::BigUint;
use std::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Type {
    Bool,
    Numeric(NumericType),
}

impl fmt::Display for Type {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Bool => f.write_str("bool"),
            Self::Numeric(ty) => write!(f, "{ty}"),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum NumericType {
    Integer { width: u32, signed: bool },
    Float(FloatType),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum FloatType {
    F16,
    F32,
    F64,
    F80,
    F128,
}

impl fmt::Display for FloatType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::F16 => "f16",
            Self::F32 => "f32",
            Self::F64 => "f64",
            Self::F80 => "f80",
            Self::F128 => "f128",
        })
    }
}

impl fmt::Display for NumericType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Integer { width, signed } => {
                write!(f, "{}{width}", if *signed { 'i' } else { 'u' })
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
    Integer { overflow: Overflow },
    Floating(FloatingSemantics),
    Exact,
    ShiftRight { fill: ShiftFill },
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
    FromBool,
    IntToFloat,
    FloatWiden,
    FloatNarrow,
    FloatToInt,
}

impl fmt::Display for ConversionKind {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Widen => "widen",
            Self::Truncate => "truncate",
            Self::Reinterpret => "reinterpret",
            Self::FromBool => "from_bool",
            Self::IntToFloat => "int_to_float",
            Self::FloatWiden => "float_widen",
            Self::FloatNarrow => "float_narrow",
            Self::FloatToInt => "float_to_int",
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ConversionReason {
    Return,
    Promotion,
    UsualArith,
    Explicit,
}

impl fmt::Display for ConversionReason {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Return => "return",
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
    FloatBits(u128),
}
