use crate::compiler_args::CompilerFlavor;
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
    NeonFp16,
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
            "neon-fp16" => Ok(Self::NeonFp16),
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
            Self::Vfpv3 | Self::Vfpv3D16 | Self::Neon | Self::NeonFp16 => 3,
            Self::Vfpv4 | Self::Vfpv4D16 | Self::NeonVfpv4 => 4,
            Self::FpArmv8 | Self::NeonFpArmv8 | Self::CryptoNeonFpArmv8 => 5,
        }
    }

    fn neon(self) -> bool {
        matches!(
            self,
            Self::Neon
                | Self::NeonFp16
                | Self::NeonVfpv4
                | Self::NeonFpArmv8
                | Self::CryptoNeonFpArmv8
        )
    }

    fn half_precision(self) -> bool {
        self == Self::NeonFp16 || self.vfp_version() >= 4
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
        let windows = environment == TargetEnvironment::Msvc;
        let default_fpu = match (version >= ArmVersion::V8, windows) {
            (true, false) => ArmFpu::CryptoNeonFpArmv8,
            (false, false) | (true, true) => ArmFpu::Neon,
            (false, true) => ArmFpu::NeonFp16,
        };
        let default_float_abi = if windows || environment == TargetEnvironment::GnuEabiHf {
            ArmFloatAbi::Hard
        } else {
            ArmFloatAbi::SoftFp
        };
        Self {
            version,
            fpu: fpu.unwrap_or(default_fpu),
            float_abi: float_abi.unwrap_or(default_float_abi),
            thumb: windows || thumb.unwrap_or(false),
        }
    }

    pub fn hard_float(self) -> bool {
        self.float_abi == ArmFloatAbi::Hard
    }

    pub fn predefines(self, flavor: CompilerFlavor) -> Vec<String> {
        let gcc = flavor.is_gcc();
        let number = |hex: u32| {
            if gcc {
                hex.to_string()
            } else {
                format!("{hex:#x}")
            }
        };
        let v8 = self.version >= ArmVersion::V8;
        let fpu = if self.float_abi == ArmFloatAbi::Soft {
            ArmFpu::None
        } else {
            self.fpu
        };
        let vfp = fpu.vfp_version();
        let mut defines = vec![format!("__ARM_ARCH={}", if v8 { 8 } else { 7 })];
        if !(gcc && v8) {
            defines.push(format!(
                "__ARM_FEATURE_COPROC={}",
                number(if v8 { 0x5 } else { 0xf })
            ));
        }
        let mut define = |name: &str, present: bool| {
            if present {
                defines.push(format!("{name}=1"));
            }
        };
        define("__ARM_ARCH_7A__", !v8);
        define("__ARM_ARCH_8A__", v8);
        define("__ARM_ARCH_EXT_IDIV__", v8);
        define("__ARM_FEATURE_IDIV", v8);
        define("__ARM_FEATURE_CRC32", v8 && !gcc);
        define("__ARM_FEATURE_DIRECTED_ROUNDING", v8 && !gcc);
        define(
            "__ARM_FEATURE_NUMERIC_MAXMIN",
            v8 && (!gcc || (fpu.neon() && vfp >= 5)),
        );
        for name in [
            "__ARM_FEATURE_AES",
            "__ARM_FEATURE_SHA2",
            "__ARM_FEATURE_CRYPTO",
        ] {
            define(name, (v8 || gcc) && fpu == ArmFpu::CryptoNeonFpArmv8);
        }
        define("__ARM_FEATURE_FMA", vfp >= 4);
        define("__ARM_VFPV2__", vfp >= 3 && !gcc);
        define("__ARM_VFPV3__", vfp >= 3 && !gcc);
        define("__ARM_VFPV4__", vfp >= 4 && !gcc);
        define("__ARM_FPV5__", vfp >= 5 && !gcc);
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
        define("__ARM_ASM_SYNTAX_UNIFIED__", gcc && self.thumb);
        define("__ARM_PCS", gcc && self.float_abi != ArmFloatAbi::Hard);
        if vfp >= 3 {
            defines.push(format!(
                "__ARM_FP={}",
                number(if fpu.half_precision() { 0xe } else { 0xc })
            ));
        }
        if fpu.neon() {
            defines.push(format!(
                "__ARM_NEON_FP={}",
                number(if fpu.half_precision() { 0x6 } else { 0x4 })
            ));
        }
        if gcc && vfp >= 4 {
            for suffix in ["", "F", "F32", "F32x", "F64", "L"] {
                defines.push(format!("__FP_FAST_FMA{suffix}=1"));
            }
        }
        if gcc {
            let iec = if self.float_abi == ArmFloatAbi::Soft {
                0
            } else {
                2
            };
            defines.push(format!("__GCC_IEC_559={iec}"));
            defines.push(format!("__GCC_IEC_559_COMPLEX={iec}"));
            if iec > 0 {
                defines.extend([
                    "__STDC_IEC_559__=1".into(),
                    "__STDC_IEC_559_COMPLEX__=1".into(),
                    "__STDC_IEC_60559_BFP__=201404L".into(),
                    "__STDC_IEC_60559_COMPLEX__=201404L".into(),
                ]);
            }
        }
        defines
    }
}
