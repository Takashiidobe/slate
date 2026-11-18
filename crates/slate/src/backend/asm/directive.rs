use super::{Escaped, Operand};
use std::fmt;

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Symbol {
    Name(String),
    Operand(Operand),
}

impl fmt::Display for Symbol {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Name(name) => write!(f, "{}", Escaped(name)),
            Self::Operand(operand) => write!(f, "{operand}"),
        }
    }
}

impl From<Operand> for Symbol {
    fn from(operand: Operand) -> Self {
        Self::Operand(operand)
    }
}

impl From<&str> for Symbol {
    fn from(name: &str) -> Self {
        Self::Name(name.to_owned())
    }
}

impl From<String> for Symbol {
    fn from(name: String) -> Self {
        Self::Name(name)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum SymbolType {
    IndirectFunction,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum ComdatKind {
    ReadOnly,
    Data,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Section {
    Drectve,
    Comdat { kind: ComdatKind, symbol: String },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum DataWidth {
    Byte,
    Short,
    Long,
    Quad,
}

impl DataWidth {
    pub fn of_bytes(bytes: u64) -> Option<Self> {
        match bytes {
            1 => Some(Self::Byte),
            2 => Some(Self::Short),
            4 => Some(Self::Long),
            8 => Some(Self::Quad),
            _ => None,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum DataValue {
    Literal(u64),
    Operand(Operand),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Directive {
    Globl(Symbol),
    Weak(Symbol),
    Hidden(Symbol),
    Protected(Symbol),
    Type { symbol: Symbol, ty: SymbolType },
    Set { symbol: Symbol, value: Symbol },
    Section(Section),
    Text,
    Ascii(String),
    P2Align(u32),
    Zero(u64),
    Data { width: DataWidth, value: DataValue },
    Label(String),
}

impl fmt::Display for Directive {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Globl(symbol) => write!(f, ".globl {symbol}"),
            Self::Weak(symbol) => write!(f, ".weak {symbol}"),
            Self::Hidden(symbol) => write!(f, ".hidden {symbol}"),
            Self::Protected(symbol) => write!(f, ".protected {symbol}"),
            Self::Type {
                symbol,
                ty: SymbolType::IndirectFunction,
            } => write!(f, ".type {symbol}, @gnu_indirect_function"),
            Self::Set { symbol, value } => write!(f, ".set {symbol}, {value}"),
            Self::Section(Section::Drectve) => f.write_str(".section .drectve,\"yni\""),
            Self::Section(Section::Comdat { kind, symbol }) => {
                let (name, flags) = match kind {
                    ComdatKind::ReadOnly => (".rdata", "dr"),
                    ComdatKind::Data => (".data", "dw"),
                };
                let symbol = Escaped(symbol);
                write!(f, ".section {name}${symbol},\"{flags}\",discard,{symbol}")
            }
            Self::Text => f.write_str(".text"),
            Self::Ascii(text) => write!(f, ".ascii {}", Escaped(&format!("{text:?}"))),
            Self::P2Align(power) => write!(f, ".p2align {power}"),
            Self::Zero(bytes) => write!(f, ".zero {bytes}"),
            Self::Data { width, value } => {
                f.write_str(match width {
                    DataWidth::Byte => ".byte",
                    DataWidth::Short => ".short",
                    DataWidth::Long => ".long",
                    DataWidth::Quad => ".quad",
                })?;
                match value {
                    DataValue::Literal(literal) => write!(f, " {literal}"),
                    DataValue::Operand(operand) => write!(f, " {operand}"),
                }
            }
            Self::Label(name) => write!(f, "{}:", Escaped(name)),
        }
    }
}
