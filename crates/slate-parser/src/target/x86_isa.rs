use crate::compiler_args::CompilerFlavor;
use crate::target_info::TargetFamily;
use std::str::FromStr;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum X86Feature {
    Mmx,
    Sse,
    Sse2,
    Sse3,
    Ssse3,
    Sse41,
    Sse42,
    Popcnt,
    Crc32,
    Xsave,
    Fxsr,
    Sahf,
    Cx16,
    Avx,
    Avx2,
    Fma,
    F16c,
    Avx512f,
    Avx512bw,
    Avx512cd,
    Avx512dq,
    Avx512vl,
    Bmi,
    Bmi2,
    Lzcnt,
    Movbe,
}

const ALL_FEATURES: [X86Feature; 26] = [
    X86Feature::Mmx,
    X86Feature::Sse,
    X86Feature::Sse2,
    X86Feature::Sse3,
    X86Feature::Ssse3,
    X86Feature::Sse41,
    X86Feature::Sse42,
    X86Feature::Popcnt,
    X86Feature::Crc32,
    X86Feature::Xsave,
    X86Feature::Fxsr,
    X86Feature::Sahf,
    X86Feature::Cx16,
    X86Feature::Avx,
    X86Feature::Avx2,
    X86Feature::Fma,
    X86Feature::F16c,
    X86Feature::Avx512f,
    X86Feature::Avx512bw,
    X86Feature::Avx512cd,
    X86Feature::Avx512dq,
    X86Feature::Avx512vl,
    X86Feature::Bmi,
    X86Feature::Bmi2,
    X86Feature::Lzcnt,
    X86Feature::Movbe,
];

impl X86Feature {
    fn spelling(self) -> &'static str {
        match self {
            Self::Mmx => "mmx",
            Self::Sse => "sse",
            Self::Sse2 => "sse2",
            Self::Sse3 => "sse3",
            Self::Ssse3 => "ssse3",
            Self::Sse41 => "sse4.1",
            Self::Sse42 => "sse4.2",
            Self::Popcnt => "popcnt",
            Self::Crc32 => "crc32",
            Self::Xsave => "xsave",
            Self::Fxsr => "fxsr",
            Self::Sahf => "sahf",
            Self::Cx16 => "cx16",
            Self::Avx => "avx",
            Self::Avx2 => "avx2",
            Self::Fma => "fma",
            Self::F16c => "f16c",
            Self::Avx512f => "avx512f",
            Self::Avx512bw => "avx512bw",
            Self::Avx512cd => "avx512cd",
            Self::Avx512dq => "avx512dq",
            Self::Avx512vl => "avx512vl",
            Self::Bmi => "bmi",
            Self::Bmi2 => "bmi2",
            Self::Lzcnt => "lzcnt",
            Self::Movbe => "movbe",
        }
    }

    fn macro_name(self) -> &'static str {
        match self {
            Self::Mmx => "__MMX__",
            Self::Sse => "__SSE__",
            Self::Sse2 => "__SSE2__",
            Self::Sse3 => "__SSE3__",
            Self::Ssse3 => "__SSSE3__",
            Self::Sse41 => "__SSE4_1__",
            Self::Sse42 => "__SSE4_2__",
            Self::Popcnt => "__POPCNT__",
            Self::Crc32 => "__CRC32__",
            Self::Xsave => "__XSAVE__",
            Self::Fxsr => "__FXSR__",
            Self::Sahf => "__LAHF_SAHF__",
            Self::Cx16 => "__GCC_HAVE_SYNC_COMPARE_AND_SWAP_16",
            Self::Avx => "__AVX__",
            Self::Avx2 => "__AVX2__",
            Self::Fma => "__FMA__",
            Self::F16c => "__F16C__",
            Self::Avx512f => "__AVX512F__",
            Self::Avx512bw => "__AVX512BW__",
            Self::Avx512cd => "__AVX512CD__",
            Self::Avx512dq => "__AVX512DQ__",
            Self::Avx512vl => "__AVX512VL__",
            Self::Bmi => "__BMI__",
            Self::Bmi2 => "__BMI2__",
            Self::Lzcnt => "__LZCNT__",
            Self::Movbe => "__MOVBE__",
        }
    }

    fn implies(self) -> X86Features {
        match self {
            Self::Sse2 => X86Features::of(&[Self::Sse]),
            Self::Sse3 => X86Features::of(&[Self::Sse2]),
            Self::Ssse3 => X86Features::of(&[Self::Sse3]),
            Self::Sse41 => X86Features::of(&[Self::Ssse3]),
            Self::Sse42 => X86Features::of(&[Self::Sse41]),
            Self::Avx => X86Features::of(&[Self::Sse42]),
            Self::Avx2 | Self::Fma | Self::F16c => X86Features::of(&[Self::Avx]),
            Self::Avx512f => X86Features::of(&[Self::Avx2, Self::Fma, Self::F16c]),
            Self::Avx512bw | Self::Avx512cd | Self::Avx512dq | Self::Avx512vl => {
                X86Features::of(&[Self::Avx512f])
            }
            _ => X86Features::default(),
        }
    }

    fn bit(self) -> u32 {
        1 << self as u32
    }

    pub fn parse_flag(argument: &str) -> Option<(Self, bool)> {
        let name = argument
            .strip_prefix("--m")
            .or_else(|| argument.strip_prefix("-m"))?;
        let (name, enabled) = match name.strip_prefix("no-") {
            Some(name) => (name, false),
            None => (name, true),
        };
        let feature = match name {
            "sse4" if enabled => Self::Sse42,
            "sse4" => Self::Sse41,
            _ => ALL_FEATURES
                .into_iter()
                .find(|feature| feature.spelling() == name)?,
        };
        Some((feature, enabled))
    }
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct X86Features(u32);

impl X86Features {
    fn of(features: &[X86Feature]) -> Self {
        Self(
            features
                .iter()
                .fold(0, |bits, feature| bits | feature.bit()),
        )
    }

    pub fn contains(self, feature: X86Feature) -> bool {
        self.0 & feature.bit() != 0
    }

    fn union(self, other: Self) -> Self {
        Self(self.0 | other.0)
    }

    fn without(self, other: Self) -> Self {
        Self(self.0 & !other.0)
    }

    fn closure(self) -> Self {
        let mut closed = self;
        loop {
            let next = ALL_FEATURES
                .into_iter()
                .filter(|feature| closed.contains(*feature))
                .fold(closed, |set, feature| set.union(feature.implies()));
            if next == closed {
                return closed;
            }
            closed = next;
        }
    }

    fn dependents(feature: X86Feature) -> Self {
        ALL_FEATURES
            .into_iter()
            .filter(|candidate| Self::of(&[*candidate]).closure().contains(feature))
            .fold(Self::default(), |set, candidate| {
                set.union(Self::of(&[candidate]))
            })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum X86Arch {
    Pentium4,
    X86_64,
    X86_64V2,
    X86_64V3,
    X86_64V4,
}

impl FromStr for X86Arch {
    type Err = String;

    fn from_str(name: &str) -> Result<Self, Self::Err> {
        match name {
            "x86-64" => Ok(Self::X86_64),
            "x86-64-v2" => Ok(Self::X86_64V2),
            "x86-64-v3" => Ok(Self::X86_64V3),
            "x86-64-v4" => Ok(Self::X86_64V4),
            _ => Err(format!("unsupported x86 architecture: {name}")),
        }
    }
}

impl X86Arch {
    fn default_for(family: TargetFamily) -> Self {
        match family {
            TargetFamily::X86 => Self::Pentium4,
            _ => Self::X86_64,
        }
    }

    fn features(self) -> X86Features {
        use X86Feature::*;
        let base = X86Features::of(&[Mmx, Sse2, Fxsr]);
        let level = match self {
            Self::Pentium4 | Self::X86_64 => X86Features::default(),
            Self::X86_64V2 => X86Features::of(&[Cx16, Sahf, Popcnt, Crc32, Sse42]),
            Self::X86_64V3 => X86Features::of(&[
                Cx16, Sahf, Popcnt, Crc32, Avx2, Bmi, Bmi2, F16c, Fma, Lzcnt, Movbe, Xsave,
            ]),
            Self::X86_64V4 => X86Features::of(&[
                Cx16, Sahf, Popcnt, Crc32, Avx2, Bmi, Bmi2, F16c, Fma, Lzcnt, Movbe, Xsave,
                Avx512f, Avx512bw, Avx512cd, Avx512dq, Avx512vl,
            ]),
        };
        base.union(level).closure()
    }

    fn cpu_macros(self) -> &'static [&'static str] {
        match self {
            Self::Pentium4 => &["__pentium4", "__pentium4__", "__tune_pentium4__"],
            Self::X86_64 => &["__k8", "__k8__", "__tune_k8__"],
            Self::X86_64V2 | Self::X86_64V3 | Self::X86_64V4 => &[],
        }
    }
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct X86IsaRequest {
    enabled: X86Features,
    disabled: X86Features,
}

impl X86IsaRequest {
    pub fn set(&mut self, feature: X86Feature, enabled: bool) {
        if enabled {
            let added = X86Features::of(&[feature]).closure();
            self.enabled = self.enabled.union(added);
            self.disabled = self.disabled.without(added);
        } else {
            let removed = X86Features::dependents(feature);
            self.disabled = self.disabled.union(removed);
            self.enabled = self.enabled.without(removed);
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct X86Isa {
    pub arch: X86Arch,
    pub features: X86Features,
}

impl X86Isa {
    pub fn baseline(family: TargetFamily) -> Self {
        Self::resolve(family, None, X86IsaRequest::default())
    }

    pub fn resolve(family: TargetFamily, arch: Option<X86Arch>, request: X86IsaRequest) -> Self {
        use X86Feature::*;
        let arch = arch.unwrap_or(X86Arch::default_for(family));
        let mut features = arch
            .features()
            .union(request.enabled)
            .without(request.disabled);
        for (trigger, implied) in [(Sse42, Popcnt), (Sse42, Crc32), (Avx, Xsave)] {
            if features.contains(trigger) && !request.disabled.contains(implied) {
                features = features.union(X86Features::of(&[implied]));
            }
        }
        Self { arch, features }
    }

    pub fn vector_register_bytes(self) -> u64 {
        if self.features.contains(X86Feature::Avx512f) {
            64
        } else if self.features.contains(X86Feature::Avx) {
            32
        } else {
            16
        }
    }

    pub fn predefines(self, family: TargetFamily, flavor: CompilerFlavor) -> Vec<String> {
        let gcc = flavor == CompilerFlavor::Gcc;
        let cpu_macros = match self.arch {
            X86Arch::X86_64V2 | X86Arch::X86_64V3 | X86Arch::X86_64V4 if gcc => &["__k8", "__k8__"],
            arch => arch.cpu_macros(),
        };
        let mut defines: Vec<String> = cpu_macros
            .iter()
            .filter(|name| !gcc || !name.starts_with("__tune_"))
            .map(|name| format!("{name}=1"))
            .collect();
        for feature in ALL_FEATURES {
            let present = match feature {
                X86Feature::Sahf if family == TargetFamily::X86 => true,
                X86Feature::Cx16 if family == TargetFamily::X86 => false,
                _ => self.features.contains(feature),
            };
            if present {
                defines.push(format!("{}=1", feature.macro_name()));
            }
        }
        // gcc defaults to -mfpmath=387 on 32-bit x86
        let sse_math = !(gcc && family == TargetFamily::X86);
        for (feature, name) in [
            (X86Feature::Sse, "__SSE_MATH__"),
            (X86Feature::Sse2, "__SSE2_MATH__"),
        ] {
            if sse_math && self.features.contains(feature) {
                defines.push(format!("{name}=1"));
            }
        }
        if gcc {
            if sse_math && self.features.contains(X86Feature::Fma) {
                defines.extend([
                    "__FP_FAST_FMA=1".into(),
                    "__FP_FAST_FMAF=1".into(),
                    "__FP_FAST_FMAF32=1".into(),
                    "__FP_FAST_FMAF32x=1".into(),
                    "__FP_FAST_FMAF64=1".into(),
                ]);
            }
            if family == TargetFamily::X86_64 {
                defines.push("__MMX_WITH_SSE__=1".into());
            }
            if self.features.contains(X86Feature::Avx512vl) {
                defines.push("__EVEX256__=1".into());
            }
            defines.push(format!(
                "__BIGGEST_ALIGNMENT__={}",
                self.vector_register_bytes()
            ));
        }
        defines
    }
}
