use num_bigint::BigUint;
use std::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum NumericType {
    Integer { width: u32, signed: bool },
    Float(FloatType),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Overflow {
    Undefined,
    Wrap,
    Trap,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Rounding {
    NearestEven,
    Environment,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Exceptions {
    Ignore,
    Observable,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
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
}

impl fmt::Display for ArithOp {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Add => "add",
            Self::Sub => "sub",
            Self::Mul => "mul",
            Self::Div => "div",
            Self::Rem => "rem",
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ArithSema {
    Integer { overflow: Overflow },
    Floating(FloatingSemantics),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Number {
    Integer(BigUint),
    FloatBits(u128),
}
