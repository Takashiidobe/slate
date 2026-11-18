use super::{AsmLine, Condition, Operand};
use std::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Syntax {
    Intel,
    Att,
}

impl Syntax {
    pub fn line(self, inst: Inst) -> AsmLine {
        AsmLine::X86 { syntax: self, inst }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Modifier {
    Byte,
    HighByte,
    Word,
    Dword,
    Qword,
    Xmm,
    Ymm,
    Zmm,
}

impl Modifier {
    pub fn letter(self) -> char {
        match self {
            Self::Byte => 'l',
            Self::HighByte => 'h',
            Self::Word | Self::Xmm => 'x',
            Self::Dword => 'e',
            Self::Qword => 'r',
            Self::Ymm => 'y',
            Self::Zmm => 'z',
        }
    }

    pub fn for_width(bits: u64) -> Option<Self> {
        match bits {
            8 => Some(Self::Byte),
            16 => Some(Self::Word),
            32 => Some(Self::Dword),
            64 => Some(Self::Qword),
            _ => None,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Width {
    Byte,
    Word,
    Dword,
    Qword,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Gpr {
    Ax,
    Bx,
    Cx,
    Dx,
    Si,
    Di,
    Bp,
    Sp,
    R(u8),
}

impl Gpr {
    pub fn from_canonical(canonical: &str) -> Option<Self> {
        Some(match canonical {
            "ax" => Self::Ax,
            "bx" => Self::Bx,
            "cx" => Self::Cx,
            "dx" => Self::Dx,
            "si" => Self::Si,
            "di" => Self::Di,
            "bp" => Self::Bp,
            "sp" => Self::Sp,
            _ => Self::R(canonical.strip_prefix('r')?.parse().ok()?),
        })
    }

    fn base(self) -> &'static str {
        match self {
            Self::Ax => "ax",
            Self::Bx => "bx",
            Self::Cx => "cx",
            Self::Dx => "dx",
            Self::Si => "si",
            Self::Di => "di",
            Self::Bp => "bp",
            Self::Sp => "sp",
            Self::R(_) => "r",
        }
    }

    pub fn name(self, width: Width) -> String {
        let base = self.base();
        match (self, width) {
            (Self::R(number), Width::Byte) => format!("r{number}b"),
            (Self::R(number), Width::Word) => format!("r{number}w"),
            (Self::R(number), Width::Dword) => format!("r{number}d"),
            (Self::R(number), Width::Qword) => format!("r{number}"),
            (Self::Ax | Self::Bx | Self::Cx | Self::Dx, Width::Byte) => format!("{}l", &base[..1]),
            (_, Width::Byte) => format!("{base}l"),
            (_, Width::Word) => base.to_owned(),
            (_, Width::Dword) => format!("e{base}"),
            (_, Width::Qword) => format!("r{base}"),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Reg {
    Gpr(Gpr, Width),
    St(u8),
    Mm(u8),
}

impl Reg {
    pub fn spelling(self, syntax: Syntax) -> String {
        let sigil = match syntax {
            Syntax::Intel => "",
            Syntax::Att => "%",
        };
        match self {
            Self::Gpr(gpr, width) => format!("{sigil}{}", gpr.name(width)),
            Self::St(0) => format!("{sigil}st"),
            Self::St(position) => format!("{sigil}st({position})"),
            Self::Mm(number) => format!("{sigil}mm{number}"),
        }
    }

    fn print(self, syntax: Syntax, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(&self.spelling(syntax))
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum MemSize {
    Dword,
    Qword,
    Tbyte,
}

impl MemSize {
    fn keyword(self) -> &'static str {
        match self {
            Self::Dword => "dword",
            Self::Qword => "qword",
            Self::Tbyte => "tbyte",
        }
    }

    fn x87_suffix(self) -> char {
        match self {
            Self::Dword => 's',
            Self::Qword => 'l',
            Self::Tbyte => 't',
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Mem {
    pub size: MemSize,
    pub base: Operand,
}

impl Mem {
    fn print(self, syntax: Syntax, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match syntax {
            Syntax::Intel => write!(f, "{} ptr [{}]", self.size.keyword(), self.base),
            Syntax::Att => write!(f, "({})", self.base),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Arg {
    Reg(Reg),
    Operand(Operand),
    Mem(Mem),
}

impl Arg {
    fn print(self, syntax: Syntax, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Reg(reg) => reg.print(syntax, f),
            Self::Operand(operand) => write!(f, "{operand}"),
            Self::Mem(mem) => mem.print(syntax, f),
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

impl From<Mem> for Arg {
    fn from(mem: Mem) -> Self {
        Self::Mem(mem)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Inst {
    Mov { dst: Arg, src: Arg },
    Xchg { dst: Arg, src: Arg },
    Movd { dst: Arg, src: Arg },
    Movq { dst: Arg, src: Arg },
    Set { condition: Condition, dst: Operand },
    Fld(Mem),
    Fstp(Mem),
    FstpSt(u8),
}

impl Inst {
    pub fn mov(dst: impl Into<Arg>, src: impl Into<Arg>) -> Self {
        Self::Mov {
            dst: dst.into(),
            src: src.into(),
        }
    }

    pub fn xchg(dst: impl Into<Arg>, src: impl Into<Arg>) -> Self {
        Self::Xchg {
            dst: dst.into(),
            src: src.into(),
        }
    }

    pub(super) fn print(self, syntax: Syntax, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let two = |f: &mut fmt::Formatter<'_>, mnemonic: &str, dst: Arg, src: Arg| {
            let (first, second) = match syntax {
                Syntax::Intel => (dst, src),
                Syntax::Att => (src, dst),
            };
            write!(f, "{mnemonic} ")?;
            first.print(syntax, f)?;
            f.write_str(", ")?;
            second.print(syntax, f)
        };
        match self {
            Self::Mov { dst, src } => two(f, "mov", dst, src),
            Self::Xchg { dst, src } => two(f, "xchg", dst, src),
            Self::Movd { dst, src } => two(f, "movd", dst, src),
            Self::Movq { dst, src } => two(f, "movq", dst, src),
            Self::Set { condition, dst } => write!(f, "set{condition} {dst}"),
            Self::Fld(mem) | Self::Fstp(mem) => {
                f.write_str(if matches!(self, Self::Fld(_)) {
                    "fld"
                } else {
                    "fstp"
                })?;
                if syntax == Syntax::Att {
                    write!(f, "{}", mem.size.x87_suffix())?;
                }
                f.write_str(" ")?;
                mem.print(syntax, f)
            }
            Self::FstpSt(position) => {
                f.write_str("fstp ")?;
                if syntax == Syntax::Att {
                    f.write_str("%")?;
                }
                write!(f, "st({position})")
            }
        }
    }
}
