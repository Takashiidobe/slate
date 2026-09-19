use super::FloatType;
use std::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AbiConvention {
    SysV64,
    Win64,
    X86Cdecl,
    Aapcs64,
    WinArm64,
    Aapcs32,
    Aapcs32HardFloat,
}

impl fmt::Display for AbiConvention {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::SysV64 => "sysv64",
            Self::Win64 => "win64",
            Self::X86Cdecl => "x86_cdecl",
            Self::Aapcs64 => "aapcs64",
            Self::WinArm64 => "win_arm64",
            Self::Aapcs32 => "aapcs32",
            Self::Aapcs32HardFloat => "aapcs32_hard_float",
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AbiChunk {
    Integer(u32),
    Float(FloatType),
    FloatPair(FloatType),
}

impl fmt::Display for AbiChunk {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Integer(width) => write!(f, "i{width}"),
            Self::Float(format) => write!(f, "{format}"),
            Self::FloatPair(format) => write!(f, "pair<{format}>"),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum AbiPass {
    Void,
    Scalar,
    Direct,
    NativeC,
    Coerce(Vec<AbiChunk>),
    ByValue { align: u32 },
    ByReference { align: u32 },
    SRet { align: u32 },
}

impl fmt::Display for AbiPass {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Void => f.write_str("void"),
            Self::Scalar => f.write_str("scalar"),
            Self::Direct => f.write_str("direct"),
            Self::NativeC => f.write_str("native_c"),
            Self::Coerce(chunks) => {
                f.write_str("coerce<")?;
                for (index, chunk) in chunks.iter().enumerate() {
                    if index != 0 {
                        f.write_str(", ")?;
                    }
                    write!(f, "{chunk}")?;
                }
                f.write_str(">")
            }
            Self::ByValue { align } => write!(f, "byval<align={align}>"),
            Self::ByReference { align } => write!(f, "byref<align={align}>"),
            Self::SRet { align } => write!(f, "sret<align={align}>"),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct AbiSignature {
    pub convention: AbiConvention,
    pub arguments: Vec<AbiPass>,
    pub result: AbiPass,
}

impl AbiSignature {
    pub fn has_nontrivial_pass(&self) -> bool {
        self.result != AbiPass::Scalar && self.result != AbiPass::Void
            || self.arguments.iter().any(|arg| *arg != AbiPass::Scalar)
    }
}

impl fmt::Display for AbiSignature {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}(", self.convention)?;
        for (index, argument) in self.arguments.iter().enumerate() {
            if index != 0 {
                f.write_str(", ")?;
            }
            write!(f, "{argument}")?;
        }
        write!(f, ") -> {}", self.result)
    }
}
