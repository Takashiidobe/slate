use super::{Condition, Operand};
use std::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Modifier {
    W,
    X,
    B,
    H,
    S,
    D,
    Q,
    V,
}

impl Modifier {
    pub fn letter(self) -> char {
        match self {
            Self::W => 'w',
            Self::X => 'x',
            Self::B => 'b',
            Self::H => 'h',
            Self::S => 's',
            Self::D => 'd',
            Self::Q => 'q',
            Self::V => 'v',
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Reg {
    X(u8),
    Sp,
}

impl Reg {
    pub fn from_canonical(canonical: &str) -> Option<Self> {
        match canonical {
            "sp" => Some(Self::Sp),
            _ => canonical
                .strip_prefix('x')?
                .parse()
                .ok()
                .filter(|number| *number <= 30)
                .map(Self::X),
        }
    }
}

impl fmt::Display for Reg {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::X(number) => write!(f, "x{number}"),
            Self::Sp => f.write_str("sp"),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Arg {
    Reg(Reg),
    Operand(Operand),
}

impl fmt::Display for Arg {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Reg(reg) => write!(f, "{reg}"),
            Self::Operand(operand) => write!(f, "{operand}"),
        }
    }
}

impl From<Reg> for Arg {
    fn from(reg: Reg) -> Self {
        Self::Reg(reg)
    }
}

impl From<Operand> for Arg {
    fn from(operand: Operand) -> Self {
        Self::Operand(operand)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Inst {
    Mov { dst: Arg, src: Arg },
    Cset { dst: Operand, condition: Condition },
    MsrNzcv(Operand),
}

impl Inst {
    pub fn mov(dst: impl Into<Arg>, src: impl Into<Arg>) -> Self {
        Self::Mov {
            dst: dst.into(),
            src: src.into(),
        }
    }
}

impl fmt::Display for Inst {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Mov { dst, src } => write!(f, "mov {dst}, {src}"),
            Self::Cset { dst, condition } => write!(f, "cset {dst}, {condition}"),
            Self::MsrNzcv(src) => write!(f, "msr nzcv, {src}"),
        }
    }
}
