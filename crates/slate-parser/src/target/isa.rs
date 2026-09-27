use crate::compiler_args::CompilerFlavor;
use crate::target::aarch64_isa::{AArch64Isa, ArmMarch, ArmVersion, SveVectorBits};
use crate::target::arm_isa::{ArmFloatAbi, ArmFpu, ArmIsa};
use crate::target::x86_isa::{X86Arch, X86Feature, X86Isa, X86IsaRequest};
use crate::target_info::{TargetEnvironment, TargetFamily};
use std::str::FromStr;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum March {
    X86(X86Arch),
    Arm(ArmMarch),
}

impl FromStr for March {
    type Err = String;

    fn from_str(value: &str) -> Result<Self, Self::Err> {
        if value.starts_with("arm") {
            value.parse().map(Self::Arm)
        } else {
            value.parse().map(Self::X86)
        }
    }
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct IsaRequest {
    pub march: Option<March>,
    pub x86: X86IsaRequest,
    pub float_abi: Option<ArmFloatAbi>,
    pub fpu: Option<ArmFpu>,
    pub thumb: Option<bool>,
    pub sve_vector_bits: Option<SveVectorBits>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TargetIsa {
    X86(X86Isa),
    AArch64(AArch64Isa),
    Arm(ArmIsa),
}

impl TargetIsa {
    pub fn baseline(family: TargetFamily, environment: TargetEnvironment) -> Self {
        match family {
            TargetFamily::X86_64 | TargetFamily::X86 => Self::X86(X86Isa::baseline(family)),
            TargetFamily::AArch64 => Self::AArch64(AArch64Isa::baseline()),
            TargetFamily::Arm32 => Self::Arm(ArmIsa::baseline(environment)),
        }
    }

    pub fn resolve(
        family: TargetFamily,
        environment: TargetEnvironment,
        request: &IsaRequest,
    ) -> Result<Self, String> {
        match family {
            TargetFamily::X86_64 | TargetFamily::X86 => {
                if request.float_abi.is_some() || request.fpu.is_some() {
                    return Err("float ABI and FPU options are Arm options".into());
                }
                if request.thumb.is_some() || request.sve_vector_bits.is_some() {
                    return Err("Thumb and SVE options are Arm options".into());
                }
                let arch = match request.march {
                    None => None,
                    Some(March::X86(arch)) => Some(arch),
                    Some(March::Arm(_)) => return Err("expected an x86 architecture".into()),
                };
                let isa = X86Isa::resolve(family, arch, request.x86);
                if isa.features.contains(X86Feature::Sse2) {
                    Ok(Self::X86(isa))
                } else {
                    Err("disabling SSE or SSE2 is unsupported".into())
                }
            }
            TargetFamily::AArch64 => {
                if request.x86 != X86IsaRequest::default() {
                    return Err("x86 feature options are unsupported".into());
                }
                if request.float_abi.is_some() || request.fpu.is_some() || request.thumb.is_some() {
                    return Err("float ABI, FPU, and Thumb options are 32-bit Arm options".into());
                }
                let march = match request.march {
                    None => None,
                    Some(March::Arm(march)) if march.version >= ArmVersion::V8 => Some(march),
                    Some(_) => return Err("expected an armv8-a or later architecture".into()),
                };
                Ok(Self::AArch64(AArch64Isa::resolve(
                    march,
                    request.sve_vector_bits,
                )))
            }
            TargetFamily::Arm32 => {
                if request.x86 != X86IsaRequest::default() {
                    return Err("x86 feature options are unsupported".into());
                }
                if request.sve_vector_bits.is_some() {
                    return Err("SVE options are AArch64 options".into());
                }
                let version = match request.march {
                    None => None,
                    Some(March::Arm(march))
                        if !march.has_modifiers()
                            && [ArmVersion::V7, ArmVersion::V8].contains(&march.version) =>
                    {
                        Some(march.version)
                    }
                    Some(_) => {
                        return Err(
                            "expected armv7-a or armv8-a without extension modifiers".into()
                        );
                    }
                };
                Ok(Self::Arm(ArmIsa::resolve(
                    environment,
                    version,
                    request.fpu,
                    request.float_abi,
                    request.thumb,
                )))
            }
        }
    }

    pub fn vector_register_bytes(self) -> u64 {
        match self {
            Self::X86(isa) => isa.vector_register_bytes(),
            Self::AArch64(_) | Self::Arm(_) => 16,
        }
    }

    pub fn arm_hard_float(self) -> bool {
        matches!(self, Self::Arm(isa) if isa.hard_float())
    }

    pub fn predefines(self, family: TargetFamily, flavor: CompilerFlavor) -> Vec<String> {
        match self {
            Self::X86(isa) => isa.predefines(family, flavor),
            Self::AArch64(isa) => isa.predefines(flavor),
            Self::Arm(isa) => isa.predefines(flavor),
        }
    }
}
