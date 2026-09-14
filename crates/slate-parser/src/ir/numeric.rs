use num_bigint::BigUint;
use std::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum NumericType {
    Integer { width: u32, signed: bool },
    Float { width: u32 },
}

impl fmt::Display for NumericType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Integer { width, signed } => {
                write!(f, "{}{width}", if *signed { 'i' } else { 'u' })
            }
            Self::Float { width } => write!(f, "f{width}"),
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
pub enum AddSemantics {
    Integer { overflow: Overflow },
    Floating(FloatingSemantics),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Number {
    Integer(BigUint),
    FloatBits(u128),
}
