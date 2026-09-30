use crate::compiler_args::CompilerFlavor;
use std::str::FromStr;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AArch64Feature {
    Fp,
    Simd,
    Fp16,
    Fp16fml,
    Crc,
    Lse,
    Rcpc,
    Pauth,
    Dotprod,
    Bti,
    Bf16,
    I8mm,
    Mops,
    Cssc,
    Sve,
    Sve2,
    Sve2p1,
    Aes,
    Sha2,
    Crypto,
    CryptoPair,
    Faminmax,
    Lut,
    Fcma,
    Jscvt,
    Frintts,
}

const ALL_FEATURES: [AArch64Feature; 26] = [
    AArch64Feature::Fp,
    AArch64Feature::Simd,
    AArch64Feature::Fp16,
    AArch64Feature::Fp16fml,
    AArch64Feature::Crc,
    AArch64Feature::Lse,
    AArch64Feature::Rcpc,
    AArch64Feature::Pauth,
    AArch64Feature::Dotprod,
    AArch64Feature::Bti,
    AArch64Feature::Bf16,
    AArch64Feature::I8mm,
    AArch64Feature::Mops,
    AArch64Feature::Cssc,
    AArch64Feature::Sve,
    AArch64Feature::Sve2,
    AArch64Feature::Sve2p1,
    AArch64Feature::Aes,
    AArch64Feature::Sha2,
    AArch64Feature::Crypto,
    AArch64Feature::CryptoPair,
    AArch64Feature::Faminmax,
    AArch64Feature::Lut,
    AArch64Feature::Fcma,
    AArch64Feature::Jscvt,
    AArch64Feature::Frintts,
];

/// clang and gcc resolve the same `-march` string to different feature sets.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Rules {
    Clang,
    Gcc,
}

impl From<CompilerFlavor> for Rules {
    fn from(flavor: CompilerFlavor) -> Self {
        match flavor {
            CompilerFlavor::Gcc => Self::Gcc,
            _ => Self::Clang,
        }
    }
}

const MODIFIERS: [(&str, AArch64Feature); 13] = [
    ("simd", AArch64Feature::Simd),
    ("fp", AArch64Feature::Fp),
    ("fp16", AArch64Feature::Fp16),
    ("crc", AArch64Feature::Crc),
    ("crypto", AArch64Feature::Crypto),
    ("aes", AArch64Feature::Aes),
    ("sha2", AArch64Feature::Sha2),
    ("lse", AArch64Feature::Lse),
    ("dotprod", AArch64Feature::Dotprod),
    ("sve", AArch64Feature::Sve),
    ("sve2", AArch64Feature::Sve2),
    ("bf16", AArch64Feature::Bf16),
    ("i8mm", AArch64Feature::I8mm),
];

impl AArch64Feature {
    fn bit(self) -> u32 {
        1 << self as u32
    }

    fn enables(self, rules: Rules) -> AArch64Features {
        use AArch64Feature::*;
        match self {
            Simd | Fp16 => AArch64Features::of(&[Fp]),
            Sve if rules == Rules::Gcc => AArch64Features::of(&[Fp16, Simd, Fcma]),
            Fcma => AArch64Features::of(&[Simd]),
            Jscvt | Frintts => AArch64Features::of(&[Fp]),
            Sve => AArch64Features::of(&[Fp16]),
            Sve2 => AArch64Features::of(&[Sve]),
            Crypto | Bf16 | I8mm | Dotprod | Aes | Sha2 => AArch64Features::of(&[Simd]),
            Fp16fml => AArch64Features::of(&[Simd, Fp16]),
            _ => AArch64Features::default(),
        }
    }

    fn disables(self, rules: Rules) -> AArch64Features {
        use AArch64Feature::*;
        match (self, rules) {
            (Fp, Rules::Clang) => AArch64Features::of(&[Simd, Fp16]),
            (Fp, Rules::Gcc) => AArch64Features::of(&[Simd, Fp16, Bf16, Jscvt, Frintts]),
            (Simd, Rules::Clang) => {
                AArch64Features::of(&[Dotprod, Bf16, I8mm, Aes, Sha2, Crypto, CryptoPair, Fp16fml])
            }
            // gcc keeps bf16 and fp16fml, and only their vector macros need simd
            (Simd, Rules::Gcc) => AArch64Features::of(&[
                Dotprod, I8mm, Aes, Sha2, Crypto, CryptoPair, Sve, Faminmax, Lut, Fcma,
            ]),
            (feature, _) => feature.disables_under_both(),
        }
    }

    fn disables_under_both(self) -> AArch64Features {
        use AArch64Feature::*;
        match self {
            Fp16 => AArch64Features::of(&[Sve, Fp16fml]),
            Sve => AArch64Features::of(&[Sve2]),
            Sve2 => AArch64Features::of(&[Sve2p1]),
            Aes | Sha2 => AArch64Features::of(&[Crypto, CryptoPair]),
            Crypto | CryptoPair => AArch64Features::of(&[Aes, Sha2, Crypto, CryptoPair]),
            _ => AArch64Features::default(),
        }
    }
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct AArch64Features(u32);

impl AArch64Features {
    fn of(features: &[AArch64Feature]) -> Self {
        Self(
            features
                .iter()
                .fold(0, |bits, feature| bits | feature.bit()),
        )
    }

    pub fn contains(self, feature: AArch64Feature) -> bool {
        self.0 & feature.bit() != 0
    }

    fn union(self, other: Self) -> Self {
        Self(self.0 | other.0)
    }

    fn without(self, other: Self) -> Self {
        Self(self.0 & !other.0)
    }

    fn closure(feature: AArch64Feature, step: impl Fn(AArch64Feature) -> Self) -> Self {
        let mut closed = Self::of(&[feature]);
        let mut pending = vec![feature];
        while let Some(next) = pending.pop() {
            let reached = step(next);
            for candidate in ALL_FEATURES {
                if reached.contains(candidate) && !closed.contains(candidate) {
                    closed = closed.union(Self::of(&[candidate]));
                    pending.push(candidate);
                }
            }
        }
        closed
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct ArmVersion {
    major: u8,
    minor: u8,
}

impl ArmVersion {
    pub const V7: Self = Self { major: 7, minor: 0 };
    pub const V8: Self = Self { major: 8, minor: 0 };

    fn v8_equivalent(self) -> u8 {
        match self.major {
            9 => 5 + self.minor,
            _ => self.minor,
        }
    }

    fn is_v8(self, minimum: u8) -> bool {
        self.major >= 8 && self.v8_equivalent() >= minimum
    }

    fn fp16_implies_fp16fml(self) -> bool {
        self.major == 8 && self.minor >= 4
    }

    fn default_features(self, rules: Rules) -> AArch64Features {
        use AArch64Feature::*;
        let gcc = rules == Rules::Gcc;
        let mut features = AArch64Features::of(&[Fp, Simd]);
        for (minimum, added, present) in [
            (1, &[Crc, Lse][..], true),
            (3, &[Rcpc, Pauth, Fcma, Jscvt], true),
            (4, &[Dotprod], true),
            (5, &[Bti, Frintts], true),
            (6, &[Bf16, I8mm], true),
            (8, &[Mops], !gcc),
            (9, &[Cssc], !gcc),
            (10, &[Faminmax, Lut], gcc),
        ] {
            if present && self.is_v8(minimum) {
                features = features.union(AArch64Features::of(added));
            }
        }
        if self.major == 9 {
            features = features.union(AArch64Features::of(&[Fp16, Sve, Sve2]));
            if gcc {
                features = features.union(AArch64Features::of(&[Fp16fml]));
            }
        }
        if self.major == 9 && self.minor >= 4 {
            features = features.union(AArch64Features::of(&[Sve2p1]));
        }
        features
    }
}

impl FromStr for ArmVersion {
    type Err = String;

    fn from_str(name: &str) -> Result<Self, Self::Err> {
        let version = name
            .strip_prefix("armv")
            .and_then(|rest| rest.strip_suffix("-a"))
            .ok_or_else(|| format!("unsupported Arm architecture: {name}"))?;
        let (major, minor) = version.split_once('.').unwrap_or((version, "0"));
        let parsed = match (major.parse::<u8>(), minor.parse::<u8>()) {
            (Ok(major), Ok(minor)) => Self { major, minor },
            _ => return Err(format!("unsupported Arm architecture: {name}")),
        };
        let supported = match parsed.major {
            7 => parsed.minor == 0,
            8 => parsed.minor <= 9,
            9 => parsed.minor <= 6,
            _ => false,
        };
        if supported {
            Ok(parsed)
        } else {
            Err(format!("unsupported Arm architecture: {name}"))
        }
    }
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
struct Selection {
    enabled: AArch64Features,
    disabled: AArch64Features,
}

impl Selection {
    fn set(&mut self, version: ArmVersion, feature: AArch64Feature, enabled: bool, rules: Rules) {
        if enabled {
            let mut added = AArch64Features::closure(feature, |next| next.enables(rules));
            if added.contains(AArch64Feature::Fp16) && version.fp16_implies_fp16fml() {
                added = added.union(AArch64Features::closure(AArch64Feature::Fp16fml, |next| {
                    next.enables(rules)
                }));
            }
            if self.enabled.union(added).contains(AArch64Feature::Aes)
                && self.enabled.union(added).contains(AArch64Feature::Sha2)
            {
                added = added.union(AArch64Features::of(&[AArch64Feature::CryptoPair]));
            }
            self.enabled = self.enabled.union(added);
            self.disabled = self.disabled.without(added);
        } else {
            let paired = self.enabled.contains(AArch64Feature::Crypto)
                || self.enabled.contains(AArch64Feature::CryptoPair);
            let removed =
                if !paired && matches!(feature, AArch64Feature::Aes | AArch64Feature::Sha2) {
                    AArch64Features::of(&[feature]).union(feature.disables(rules))
                } else {
                    AArch64Features::closure(feature, |next| next.disables(rules))
                };
            self.disabled = self.disabled.union(removed);
            self.enabled = self.enabled.without(removed);
        }
    }

    fn apply(self, defaults: AArch64Features) -> AArch64Features {
        defaults.union(self.enabled).without(self.disabled)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ArmMarch {
    pub version: ArmVersion,
    clang: Selection,
    gcc: Selection,
}

impl ArmMarch {
    fn plain(version: ArmVersion) -> Self {
        Self {
            version,
            clang: Selection::default(),
            gcc: Selection::default(),
        }
    }

    pub fn has_modifiers(self) -> bool {
        self.clang != Selection::default() || self.gcc != Selection::default()
    }

    fn selection(self, rules: Rules) -> Selection {
        match rules {
            Rules::Clang => self.clang,
            Rules::Gcc => self.gcc,
        }
    }
}

impl FromStr for ArmMarch {
    type Err = String;

    fn from_str(value: &str) -> Result<Self, Self::Err> {
        let mut parts = value.split('+');
        let mut march = Self::plain(parts.next().unwrap_or_default().parse()?);
        for modifier in parts {
            let (name, enabled) = match modifier.strip_prefix("no") {
                Some(name) => (name, false),
                None => (modifier, true),
            };
            let feature = MODIFIERS
                .iter()
                .find(|(spelling, _)| *spelling == name)
                .map(|(_, feature)| *feature)
                .ok_or_else(|| format!("unsupported AArch64 extension: {modifier}"))?;
            march
                .clang
                .set(march.version, feature, enabled, Rules::Clang);
            march.gcc.set(march.version, feature, enabled, Rules::Gcc);
        }
        Ok(march)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum SveVectorBits {
    Scalable,
    Fixed(u32),
}

impl FromStr for SveVectorBits {
    type Err = String;

    fn from_str(value: &str) -> Result<Self, Self::Err> {
        if value == "scalable" {
            return Ok(Self::Scalable);
        }
        match value.parse::<u32>() {
            Ok(bits) if (128..=2048).contains(&bits) && bits.is_power_of_two() => {
                Ok(Self::Fixed(bits))
            }
            _ => Err(format!("unsupported SVE vector length: {value}")),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct AArch64Isa {
    pub version: ArmVersion,
    pub features: AArch64Features,
    pub sve_vector_bits: Option<SveVectorBits>,
}

impl AArch64Isa {
    pub fn baseline() -> Self {
        Self::resolve(None, None, CompilerFlavor::Clang)
    }

    pub fn resolve(
        march: Option<ArmMarch>,
        sve_vector_bits: Option<SveVectorBits>,
        flavor: CompilerFlavor,
    ) -> Self {
        let rules = Rules::from(flavor);
        let march = march.unwrap_or(ArmMarch::plain(ArmVersion::V8));
        Self {
            version: march.version,
            features: march
                .selection(rules)
                .apply(march.version.default_features(rules)),
            sve_vector_bits,
        }
    }

    pub fn predefines(self, flavor: CompilerFlavor, strict_iso: bool) -> Vec<String> {
        match flavor {
            CompilerFlavor::Gcc => self.gcc_predefines(strict_iso),
            _ => self.clang_predefines(),
        }
    }

    fn clang_predefines(self) -> Vec<String> {
        use AArch64Feature::*;
        let has = |feature| self.features.contains(feature);
        let version = self.version;
        let mut defines = vec![format!("__ARM_ARCH={}", version.major)];
        let mut define = |name: &str, present: bool| {
            if present {
                defines.push(format!("{name}=1"));
            }
        };
        define("__ARM_FEATURE_QRDMX", version.is_v8(1));
        define("__ARM_FEATURE_COMPLEX", version.is_v8(3));
        define("__ARM_FEATURE_JCVT", version.is_v8(3));
        define("__ARM_FEATURE_FRINT", version.is_v8(5));
        for (feature, name) in [
            (Rcpc, "__ARM_FEATURE_RCPC"),
            (Pauth, "__ARM_FEATURE_PAUTH"),
            (Bti, "__ARM_FEATURE_BTI"),
            (Mops, "__ARM_FEATURE_MOPS"),
            (Cssc, "__ARM_FEATURE_CSSC"),
            (Crc, "__ARM_FEATURE_CRC32"),
            (Lse, "__ARM_FEATURE_ATOMICS"),
            (Dotprod, "__ARM_FEATURE_DOTPROD"),
            (I8mm, "__ARM_FEATURE_MATMUL_INT8"),
            (Sve2, "__ARM_FEATURE_SVE2"),
            (Sve2p1, "__ARM_FEATURE_SVE2p1"),
            (Fp16, "__ARM_FEATURE_FP16_SCALAR_ARITHMETIC"),
        ] {
            define(name, has(feature));
        }
        define("__ARM_NEON", has(Simd));
        define(
            "__ARM_FEATURE_FP16_VECTOR_ARITHMETIC",
            has(Simd) && has(Fp16),
        );
        define("__ARM_FEATURE_FP16_FML", has(Simd) && has(Fp16fml));
        for name in [
            "__ARM_FEATURE_BF16",
            "__ARM_FEATURE_BF16_SCALAR_ARITHMETIC",
            "__ARM_FEATURE_BF16_VECTOR_ARITHMETIC",
            "__ARM_BF16_FORMAT_ALTERNATIVE",
        ] {
            define(name, has(Bf16));
        }
        let sve = has(Sve) && has(Simd);
        define("__ARM_FEATURE_SVE", sve);
        define("__ARM_FEATURE_SVE_BF16", sve && has(Bf16));
        define("__ARM_FEATURE_SVE_MATMUL_INT8", sve && has(I8mm));
        let aes = has(Aes) || has(Crypto);
        let sha2 = has(Sha2) || has(Crypto);
        define("__ARM_FEATURE_AES", aes);
        define("__ARM_FEATURE_SHA2", sha2);
        define("__ARM_FEATURE_CRYPTO", aes && sha2);
        for name in [
            "__ARM_FEATURE_SHA3",
            "__ARM_FEATURE_SHA512",
            "__ARM_FEATURE_SM3",
            "__ARM_FEATURE_SM4",
        ] {
            define(name, has(Crypto) && version.is_v8(4));
        }
        if has(Fp) {
            defines.push("__ARM_FP=0xE".into());
        }
        if has(Simd) {
            defines.push("__ARM_NEON_FP=0xE".into());
        }
        if sve {
            defines.push("__ARM_FEATURE_SVE_VECTOR_OPERATORS=2".into());
        }
        if let Some(SveVectorBits::Fixed(bits)) = self.sve_vector_bits {
            defines.push(format!("__ARM_FEATURE_SVE_BITS={bits}"));
        }
        defines
    }

    fn gcc_predefines(self, strict_iso: bool) -> Vec<String> {
        use AArch64Feature::*;
        let has = |feature| self.features.contains(feature);
        let version = self.version;
        let (fp, simd) = (has(Fp), has(Simd));
        let sve = has(Sve);
        let mut defines = vec![format!("__ARM_ARCH={}", version.major)];
        let mut define = |name: &str, present: bool| {
            if present {
                defines.push(format!("{name}=1"));
            }
        };
        define("__ARM_FEATURE_QRDMX", simd && version.is_v8(1));
        for (feature, name) in [
            (Fcma, "__ARM_FEATURE_COMPLEX"),
            (Jscvt, "__ARM_FEATURE_JCVT"),
            (Frintts, "__ARM_FEATURE_FRINT"),
            (Rcpc, "__ARM_FEATURE_RCPC"),
            (Pauth, "__ARM_FEATURE_PAUTH"),
            (Bti, "__ARM_FEATURE_BTI"),
            (Crc, "__ARM_FEATURE_CRC32"),
            (Lse, "__ARM_FEATURE_ATOMICS"),
            (Dotprod, "__ARM_FEATURE_DOTPROD"),
            (I8mm, "__ARM_FEATURE_MATMUL_INT8"),
            (Sve, "__ARM_FEATURE_SVE"),
            (Sve2, "__ARM_FEATURE_SVE2"),
            (Sve2p1, "__ARM_FEATURE_SVE2p1"),
            (Fp16, "__ARM_FEATURE_FP16_SCALAR_ARITHMETIC"),
            (Bf16, "__ARM_FEATURE_BF16"),
            (Bf16, "__ARM_FEATURE_BF16_SCALAR_ARITHMETIC"),
            (Faminmax, "__ARM_FEATURE_FAMINMAX"),
            (Lut, "__ARM_FEATURE_LUT"),
            (Fp, "__ARM_FEATURE_FMA"),
            (Fp, "__ARM_FP16_ARGS"),
            (Fp, "__ARM_FP16_FORMAT_IEEE"),
            (Fp, "__FP_FAST_FMA"),
            (Fp, "__FP_FAST_FMAF"),
            (Fp, "__FP_FAST_FMAF32"),
            (Fp, "__FP_FAST_FMAF32x"),
            (Fp, "__FP_FAST_FMAF64"),
            (Fp, "__STDC_IEC_559__"),
            (Fp, "__STDC_IEC_559_COMPLEX__"),
            (Simd, "__ARM_NEON"),
            (Simd, "__ARM_FEATURE_NUMERIC_MAXMIN"),
        ] {
            define(name, has(feature));
        }
        define("__ARM_FEATURE_FP16_VECTOR_ARITHMETIC", simd && has(Fp16));
        define("__ARM_FEATURE_FP16_FML", simd && has(Fp16fml));
        define("__ARM_FEATURE_BF16_VECTOR_ARITHMETIC", simd && has(Bf16));
        define("__ARM_FEATURE_SVE_BF16", sve && has(Bf16));
        define("__ARM_FEATURE_SVE_MATMUL_INT8", sve && has(I8mm));
        let aes = has(Aes) || has(Crypto);
        let sha2 = has(Sha2) || has(Crypto);
        define("__ARM_FEATURE_AES", aes);
        define("__ARM_FEATURE_SHA2", sha2);
        define("__ARM_FEATURE_CRYPTO", aes && sha2);
        if fp {
            defines.push("__ARM_FP=14".into());
            defines.push("__STDC_IEC_60559_BFP__=201404L".into());
            defines.push("__STDC_IEC_60559_COMPLEX__=201404L".into());
        }
        let iec_559 = if fp { 2 } else { 0 };
        defines.push(format!("__GCC_IEC_559={iec_559}"));
        defines.push(format!("__GCC_IEC_559_COMPLEX={iec_559}"));
        let eval_method = if has(Fp16) { 16 } else { 0 };
        // strict -std=cNN does not permit the ts-18661 method in __FLT_EVAL_METHOD__ itself
        let iso_eval_method = if strict_iso { 0 } else { eval_method };
        defines.push(format!("__FLT_EVAL_METHOD__={iso_eval_method}"));
        defines.push(format!("__FLT_EVAL_METHOD_C99__={eval_method}"));
        defines.push(format!("__FLT_EVAL_METHOD_TS_18661_3__={eval_method}"));
        let destructive_size = if version.major == 9 { 64 } else { 256 };
        defines.push(format!("__GCC_DESTRUCTIVE_SIZE={destructive_size}"));
        if sve {
            let (bits, operators) = match self.sve_vector_bits {
                Some(SveVectorBits::Fixed(bits)) => (bits, 1),
                _ => (0, 2),
            };
            defines.push(format!("__ARM_FEATURE_SVE_BITS={bits}"));
            defines.push(format!("__ARM_FEATURE_SVE_VECTOR_OPERATORS={operators}"));
            defines.push(format!("__ARM_FEATURE_SVE_PREDICATE_OPERATORS={operators}"));
        }
        defines
    }
}
