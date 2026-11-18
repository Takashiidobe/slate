use super::{Condition, Operand};
use std::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Modifier {
    LowHalf,
    HighHalf,
}

impl Modifier {
    pub fn letter(self) -> char {
        match self {
            Self::LowHalf => 'e',
            Self::HighHalf => 'f',
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Reg(u8);

impl Reg {
    pub fn from_canonical(canonical: &str) -> Option<Self> {
        canonical
            .strip_prefix('r')?
            .parse()
            .ok()
            .filter(|number| *number <= 15)
            .map(Self)
    }
}

impl fmt::Display for Reg {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "r{}", self.0)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Arg {
    Reg(Reg),
    Operand(Operand),
    Imm(i32),
}

impl fmt::Display for Arg {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Reg(reg) => write!(f, "{reg}"),
            Self::Operand(operand) => write!(f, "{operand}"),
            Self::Imm(value) => write!(f, "#{value}"),
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
    Mov {
        condition: Option<Condition>,
        dst: Arg,
        src: Arg,
    },
}

impl Inst {
    pub fn mov(dst: impl Into<Arg>, src: impl Into<Arg>) -> Self {
        Self::Mov {
            condition: None,
            dst: dst.into(),
            src: src.into(),
        }
    }

    pub fn mov_if(condition: Condition, dst: impl Into<Arg>, src: impl Into<Arg>) -> Self {
        Self::Mov {
            condition: Some(condition),
            dst: dst.into(),
            src: src.into(),
        }
    }
}

impl fmt::Display for Inst {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Mov {
                condition,
                dst,
                src,
            } => {
                f.write_str("mov")?;
                if let Some(condition) = condition {
                    write!(f, "{condition}")?;
                }
                write!(f, " {dst}, {src}")
            }
        }
    }
}
