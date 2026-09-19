use crate::target::aarch64_isa::ArmVersion;
use crate::target_info::TargetEnvironment;
use std::str::FromStr;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ArmFloatAbi {
    Soft,
    SoftFp,
    Hard,
}

impl FromStr for ArmFloatAbi {
    type Err = String;

    fn from_str(value: &str) -> Result<Self, Self::Err> {
        match value {
            "soft" => Ok(Self::Soft),
            "softfp" => Ok(Self::SoftFp),
            "hard" => Ok(Self::Hard),
            _ => Err(format!("unknown float ABI: {value}")),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ArmFpu {
    None,
    Vfpv3,
    Vfpv3D16,
    Vfpv4,
    Vfpv4D16,
    Neon,
    NeonVfpv4,
    FpArmv8,
    NeonFpArmv8,
    CryptoNeonFpArmv8,
}

impl FromStr for ArmFpu {
    type Err = String;

    fn from_str(value: &str) -> Result<Self, Self::Err> {
        match value {
            "none" => Ok(Self::None),
            "vfpv3" => Ok(Self::Vfpv3),
            "vfpv3-d16" => Ok(Self::Vfpv3D16),
            "vfpv4" => Ok(Self::Vfpv4),
            "vfpv4-d16" => Ok(Self::Vfpv4D16),
            "neon" => Ok(Self::Neon),
            "neon-vfpv4" => Ok(Self::NeonVfpv4),
            "fp-armv8" => Ok(Self::FpArmv8),
            "neon-fp-armv8" => Ok(Self::NeonFpArmv8),
            "crypto-neon-fp-armv8" => Ok(Self::CryptoNeonFpArmv8),
            _ => Err(format!("unsupported FPU: {value}")),
        }
    }
}

impl ArmFpu {
    fn vfp_version(self) -> u8 {
        match self {
            Self::None => 0,
            Self::Vfpv3 | Self::Vfpv3D16 | Self::Neon => 3,
            Self::Vfpv4 | Self::Vfpv4D16 | Self::NeonVfpv4 => 4,
            Self::FpArmv8 | Self::NeonFpArmv8 | Self::CryptoNeonFpArmv8 => 5,
        }
    }

    fn neon(self) -> bool {
        matches!(
            self,
            Self::Neon | Self::NeonVfpv4 | Self::NeonFpArmv8 | Self::CryptoNeonFpArmv8
        )
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ArmIsa {
    pub version: ArmVersion,
    pub fpu: ArmFpu,
    pub float_abi: ArmFloatAbi,
    pub thumb: bool,
}

impl ArmIsa {
    pub fn baseline(environment: TargetEnvironment) -> Self {
        Self::resolve(environment, None, None, None, None)
    }

    pub fn resolve(
        environment: TargetEnvironment,
        version: Option<ArmVersion>,
        fpu: Option<ArmFpu>,
        float_abi: Option<ArmFloatAbi>,
        thumb: Option<bool>,
    ) -> Self {
        let version = version.unwrap_or(ArmVersion::V7);
        let default_fpu = if version >= ArmVersion::V8 {
            ArmFpu::CryptoNeonFpArmv8
        } else {
            ArmFpu::Neon
        };
        let default_float_abi = if environment == TargetEnvironment::GnuEabiHf {
            ArmFloatAbi::Hard
        } else {
            ArmFloatAbi::SoftFp
        };
        Self {
            version,
            fpu: fpu.unwrap_or(default_fpu),
            float_abi: float_abi.unwrap_or(default_float_abi),
            thumb: thumb.unwrap_or(false),
        }
    }

    pub fn hard_float(self) -> bool {
        self.float_abi == ArmFloatAbi::Hard
    }

    pub fn predefines(self) -> Vec<String> {
        let v8 = self.version >= ArmVersion::V8;
        let fpu = if self.float_abi == ArmFloatAbi::Soft {
            ArmFpu::None
        } else {
            self.fpu
        };
        let vfp = fpu.vfp_version();
        let mut defines = vec![
            format!("__ARM_ARCH={}", if v8 { 8 } else { 7 }),
            format!("__ARM_FEATURE_COPROC={}", if v8 { "0x5" } else { "0xf" }),
        ];
        let mut define = |name: &str, present: bool| {
            if present {
                defines.push(format!("{name}=1"));
            }
        };
        define("__ARM_ARCH_7A__", !v8);
        define("__ARM_ARCH_8A__", v8);
        for name in [
            "__ARM_ARCH_EXT_IDIV__",
            "__ARM_FEATURE_IDIV",
            "__ARM_FEATURE_CRC32",
            "__ARM_FEATURE_DIRECTED_ROUNDING",
            "__ARM_FEATURE_NUMERIC_MAXMIN",
        ] {
            define(name, v8);
        }
        for name in [
            "__ARM_FEATURE_AES",
            "__ARM_FEATURE_SHA2",
            "__ARM_FEATURE_CRYPTO",
        ] {
            define(name, v8 && fpu == ArmFpu::CryptoNeonFpArmv8);
        }
        define("__ARM_FEATURE_FMA", vfp >= 4);
        define("__ARM_VFPV2__", vfp >= 3);
        define("__ARM_VFPV3__", vfp >= 3);
        define("__ARM_VFPV4__", vfp >= 4);
        define("__ARM_FPV5__", vfp >= 5);
        define("__ARM_NEON", fpu.neon());
        define("__ARM_NEON__", fpu.neon());
        define("__ARM_PCS_VFP", self.float_abi == ArmFloatAbi::Hard);
        define(
            "__SOFTFP__",
            self.float_abi == ArmFloatAbi::Soft
                || (vfp == 0 && self.float_abi != ArmFloatAbi::Hard),
        );
        for name in ["__thumb__", "__thumb2__", "__THUMBEL__"] {
            define(name, self.thumb);
        }
        if vfp >= 3 {
            defines.push(format!("__ARM_FP={}", if vfp >= 4 { "0xe" } else { "0xc" }));
        }
        if fpu.neon() {
            defines.push(format!(
                "__ARM_NEON_FP={}",
                if vfp >= 4 { "0x6" } else { "0x4" }
            ));
        }
        defines
    }
}
