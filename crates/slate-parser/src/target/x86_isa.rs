pub use super::x86_isa_tables::X86Feature;
use super::x86_isa_tables::{
    ALL_FEATURES, CPUS, PENTIUM4, X86_64, X86_64_V2, X86_64_V3, X86_64_V4, X86Cpu,
};
use crate::compiler_args::CompilerFlavor;
use crate::ir::TargetFeature;
use crate::target_info::TargetFamily;
use std::str::FromStr;

impl X86Feature {
    fn enables(self, rules: Rules) -> X86Features {
        match rules {
            Rules::Clang => X86Features::of(&[self]).closure(),
            Rules::Gcc => X86Features(self.bit() | self.gcc_enables().unwrap_or_default()),
        }
    }

    fn disables(self, rules: Rules) -> X86Features {
        match rules {
            Rules::Clang => X86Features::dependents(self),
            Rules::Gcc => X86Features(self.bit() | self.gcc_disables()),
        }
    }

    fn macros_for(self, flavor: CompilerFlavor) -> &'static [&'static str] {
        if flavor.is_gcc() {
            self.gcc_macros()
        } else {
            self.macros()
        }
    }

    fn bit(self) -> u128 {
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
                .iter()
                .copied()
                .find(|feature| feature.spelling() == name)?,
        };
        Some((feature, enabled))
    }
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct X86Features(u128);

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
                .iter()
                .copied()
                .filter(|feature| closed.contains(*feature))
                .fold(closed, |set, feature| {
                    set.union(Self(feature.clang_implies()))
                });
            if next == closed {
                return closed;
            }
            closed = next;
        }
    }

    fn dependents(feature: X86Feature) -> Self {
        ALL_FEATURES
            .iter()
            .copied()
            .filter(|candidate| Self::of(&[*candidate]).closure().contains(feature))
            .fold(Self::default(), |set, candidate| {
                set.union(Self::of(&[candidate]))
            })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct X86Arch(usize);

impl FromStr for X86Arch {
    type Err = String;

    fn from_str(name: &str) -> Result<Self, Self::Err> {
        CPUS.iter()
            .position(|cpu| cpu.name == name)
            .map(Self)
            .ok_or_else(|| format!("unsupported x86 architecture: {name}"))
    }
}

impl X86Arch {
    fn default_for(family: TargetFamily, rules: Rules) -> Self {
        match (family, rules) {
            // gcc follows the multilib x86_64 build our i686 snapshot comes from, whose -m32 defaults to x86-64
            (TargetFamily::X86, Rules::Clang) => Self(PENTIUM4),
            _ => Self(X86_64),
        }
    }

    fn cpu(self) -> &'static X86Cpu {
        &CPUS[self.0]
    }

    fn features(self, rules: Rules) -> X86Features {
        let features = X86Features(self.cpu().features);
        match rules {
            Rules::Clang => features,
            Rules::Gcc => features.without(X86Features::of(&[X86Feature::Crc32])),
        }
    }

    fn is_level(self) -> bool {
        [X86_64, X86_64_V2, X86_64_V3, X86_64_V4].contains(&self.0)
    }

    pub fn long_mode(self) -> bool {
        self.cpu().long_mode
    }

    pub fn name(self) -> &'static str {
        self.cpu().name
    }
}

/// clang and gcc imply different features from the same `-m` flags.
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

/// The `-m<feature>`/`-mno-<feature>` flags in command-line order, folded once
/// the flavor is known.
#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub struct X86IsaRequest {
    flags: Vec<(X86Feature, bool)>,
    clang_only: Vec<usize>,
    general_regs_only: Vec<usize>,
    apx: Vec<ApxFlag>,
    pub fpmath: Option<FpMath>,
}

const APX_FEATURES: &[(&str, &str)] = &[
    ("egpr", "__EGPR__"),
    ("push2pop2", "__PUSH2POP2__"),
    ("ppx", "__PPX__"),
    ("ndd", "__NDD__"),
    ("ccmp", "__CCMP__"),
    ("nf", "__NF__"),
    ("cf", "__CF__"),
    ("zu", "__ZU__"),
    ("jmpabs", "__JMPABS__"),
];
const GCC_APX_FEATURES: &[&str] = &[
    "none",
    "all",
    "egpr",
    "push2pop2",
    "ppx",
    "ndd",
    "ccmp",
    "nf",
    "zu",
];
const APX_F_MACRO: &str = "__APX_F__";
const APX_INLINE_ASM_USE_GPR32_MACRO: &str = "__APX_INLINE_ASM_USE_GPR32__";

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum ApxFlag {
    Apxf(bool),
    Features { names: Vec<String>, enabled: bool },
    InlineAsmUseGpr32(bool),
}

impl ApxFlag {
    pub fn parse(argument: &str) -> Option<Self> {
        let name = argument.strip_prefix("-m")?;
        let (name, enabled) = match name.strip_prefix("no-") {
            Some(name) => (name, false),
            None => (name, true),
        };
        match name {
            "apxf" => Some(Self::Apxf(enabled)),
            "apx-inline-asm-use-gpr32" => Some(Self::InlineAsmUseGpr32(enabled)),
            _ => name
                .strip_prefix("apx-features=")
                .map(|names| Self::Features {
                    names: names.split(',').map(String::from).collect(),
                    enabled,
                }),
        }
    }

    fn check(&self, family: TargetFamily, flavor: CompilerFlavor) -> Result<(), String> {
        match self {
            Self::Apxf(true) if family == TargetFamily::X86 => {
                Err("-mapxf is not supported for 32-bit code".into())
            }
            Self::Features { enabled: true, .. } if family == TargetFamily::X86 => {
                Err("-mapx-features= is not supported for 32-bit code".into())
            }
            Self::Features { names, .. } => match names.iter().find(|name| {
                if flavor.is_gcc() {
                    !GCC_APX_FEATURES.contains(&name.as_str())
                } else {
                    !name.is_empty() && apx_bit(name).is_none()
                }
            }) {
                Some(name) => Err(format!("unsupported APX feature '{name}'")),
                None => Ok(()),
            },
            Self::InlineAsmUseGpr32(false) if !flavor.is_gcc() => {
                Err("unknown option -mno-apx-inline-asm-use-gpr32".into())
            }
            _ => Ok(()),
        }
    }
}

fn apx_bit(name: &str) -> Option<u16> {
    APX_FEATURES
        .iter()
        .position(|(spelling, _)| *spelling == name)
        .map(|index| 1 << index)
}

fn apx_f_features() -> u16 {
    APX_FEATURES
        .iter()
        .filter(|(spelling, _)| *spelling != "cf")
        .filter_map(|(spelling, _)| apx_bit(spelling))
        .fold(0, |bits, bit| bits | bit)
}

fn is_apx_macro(name: &str) -> bool {
    name == APX_F_MACRO || APX_FEATURES.iter().any(|(_, apx)| *apx == name)
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct Apx {
    features: u16,
    apx_f: bool,
    inline_asm_use_gpr32: bool,
}

impl Apx {
    fn resolve(cpu: &X86Cpu, flags: &[ApxFlag], rules: Rules) -> Self {
        let mut features = APX_FEATURES
            .iter()
            .filter(|(_, name)| cpu.macros.contains(name))
            .filter_map(|(spelling, _)| apx_bit(spelling))
            .fold(0, |bits, bit| bits | bit);
        let mut apx_f = false;
        let mut inline_asm_use_gpr32 = false;
        for flag in flags {
            let (bits, enabled) = match flag {
                ApxFlag::Apxf(enabled) => {
                    apx_f = *enabled;
                    (apx_f_features(), *enabled)
                }
                ApxFlag::Features { names, enabled } => (
                    names
                        .iter()
                        .filter_map(|name| apx_bit(name))
                        .fold(0, |bits, bit| bits | bit),
                    *enabled,
                ),
                ApxFlag::InlineAsmUseGpr32(enabled) => {
                    inline_asm_use_gpr32 = *enabled;
                    continue;
                }
            };
            features = if enabled {
                features | bits
            } else {
                features & !bits
            };
        }
        match rules {
            Rules::Clang => Self {
                features,
                apx_f: features & apx_f_features() == apx_f_features(),
                inline_asm_use_gpr32: inline_asm_use_gpr32
                    && apx_bit("egpr").is_some_and(|egpr| features & egpr != 0),
            },
            Rules::Gcc => Self {
                features: 0,
                apx_f,
                inline_asm_use_gpr32,
            },
        }
    }

    fn macros(self) -> impl Iterator<Item = &'static str> {
        APX_FEATURES
            .iter()
            .enumerate()
            .filter(move |(index, _)| self.features & (1 << index) != 0)
            .map(|(_, (_, name))| *name)
            .chain(self.apx_f.then_some(APX_F_MACRO))
            .chain(
                self.inline_asm_use_gpr32
                    .then_some(APX_INLINE_ASM_USE_GPR32_MACRO),
            )
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum FpMath {
    Sse,
    X87,
    Both,
}

impl FromStr for FpMath {
    type Err = String;

    fn from_str(value: &str) -> Result<Self, Self::Err> {
        match value {
            "sse" => Ok(Self::Sse),
            "387" => Ok(Self::X87),
            "both" | "sse+387" | "sse,387" | "387+sse" | "387,sse" => Ok(Self::Both),
            _ => Err(format!("unknown FP unit '{value}'")),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct X87Formats {
    pub float: bool,
    pub double: bool,
}

impl X87Formats {
    const NONE: Self = Self {
        float: false,
        double: false,
    };
}

impl X86IsaRequest {
    pub fn set(&mut self, feature: X86Feature, enabled: bool) {
        self.flags.push((feature, enabled));
    }

    pub fn set_apx(&mut self, flag: ApxFlag) {
        self.apx.push(flag);
    }

    pub fn set_for_clang(&mut self, feature: X86Feature, enabled: bool) {
        self.clang_only.push(self.flags.len());
        self.set(feature, enabled);
    }

    pub fn next_position(&self) -> usize {
        self.flags.len()
    }

    pub fn with_general_regs_only(mut self, positions: &[usize]) -> Self {
        self.general_regs_only = positions.to_vec();
        self
    }

    fn included(&self, index: usize, rules: Rules) -> bool {
        rules == Rules::Clang || !self.clang_only.contains(&index)
    }

    pub fn check(
        &self,
        family: TargetFamily,
        arch: Option<X86Arch>,
        flavor: CompilerFlavor,
    ) -> Result<(), String> {
        for flag in &self.apx {
            flag.check(family, flavor)?;
        }
        if let Some(arch) = arch {
            if family == TargetFamily::X86_64 && !arch.long_mode() {
                return Err(format!(
                    "CPU `{}` does not support 64-bit mode",
                    arch.name()
                ));
            }
            if flavor.is_gcc() && !arch.is_level() {
                return Err(format!(
                    "x86 architecture `{}` is only emulated for the clang flavor",
                    arch.name()
                ));
            }
        }
        if family == TargetFamily::X86
            && let Some((feature, _)) = self
                .flags
                .iter()
                .find(|(feature, on)| *on && feature.long_mode_only())
        {
            return Err(format!(
                "x86 feature `{}` is only supported in 64-bit mode",
                feature.spelling()
            ));
        }
        match self
            .flags
            .iter()
            .find(|(feature, _)| flavor.is_gcc() && feature.gcc_enables().is_none())
        {
            Some((feature, _)) => Err(format!(
                "x86 feature `{}` is not accepted by gcc",
                feature.spelling()
            )),
            None => Ok(()),
        }
    }

    fn fold(&self, rules: Rules) -> (X86Features, X86Features) {
        let mut enabled = X86Features::default();
        let mut disabled = X86Features::default();
        for index in 0..=self.flags.len() {
            for _ in self.general_regs_only.iter().filter(|&&at| at == index) {
                let reenabled = |feature: X86Feature| {
                    rules == Rules::Clang
                        && (index..self.flags.len())
                            .any(|later| self.flags[later] == (feature, true))
                };
                (enabled, disabled) = drop_vector_registers(rules, enabled, disabled, reenabled);
            }
            let Some(&(feature, on)) = self
                .flags
                .get(index)
                .filter(|_| self.included(index, rules))
            else {
                continue;
            };
            let cancelled = on
                && (index + 1..self.flags.len()).any(|later| {
                    self.included(later, rules) && self.flags[later] == (feature, false)
                });
            let replaced = !on
                && (index + 1..self.flags.len()).any(|later| self.flags[later] == (feature, true));
            if cancelled || replaced {
                continue;
            }
            if on {
                let added = feature.enables(rules);
                enabled = enabled.union(added);
                disabled = disabled.without(added);
            } else {
                let removed = feature.disables(rules);
                disabled = disabled.union(removed);
                enabled = enabled.without(removed);
            }
        }
        (enabled, disabled)
    }
}

fn drop_vector_registers(
    rules: Rules,
    mut enabled: X86Features,
    mut disabled: X86Features,
    reenabled: impl Fn(X86Feature) -> bool,
) -> (X86Features, X86Features) {
    let kept = if rules == Rules::Gcc && enabled.contains(X86Feature::Sse42) {
        X86Features::of(&[X86Feature::Popcnt, X86Feature::Crc32]).without(disabled)
    } else {
        X86Features::default()
    };
    for feature in [X86Feature::Sse, X86Feature::Mmx, X86Feature::X87] {
        if reenabled(feature) {
            continue;
        }
        let removed = feature.disables(rules);
        disabled = disabled.union(removed);
        enabled = enabled.without(removed);
    }
    (enabled.union(kept), disabled.without(kept))
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct X86Isa {
    pub arch: X86Arch,
    pub features: X86Features,
    pub disabled: X86Features,
    pub fpmath: Option<FpMath>,
    apx: Apx,
}

impl X86Isa {
    pub fn baseline(family: TargetFamily) -> Self {
        Self::resolve(
            family,
            None,
            &X86IsaRequest::default(),
            CompilerFlavor::Clang,
        )
    }

    pub fn resolve(
        family: TargetFamily,
        arch: Option<X86Arch>,
        request: &X86IsaRequest,
        flavor: CompilerFlavor,
    ) -> Self {
        use X86Feature::*;
        let rules = Rules::from(flavor);
        let arch = arch.unwrap_or(X86Arch::default_for(family, rules));
        let (enabled, disabled) = request.fold(rules);
        let mut features = arch.features(rules).union(enabled).without(disabled);
        let late_mmx = (rules == Rules::Gcc).then_some((Sse, Mmx));
        for (trigger, implied) in [(Sse42, Popcnt), (Sse42, Crc32), (Avx, Xsave)]
            .into_iter()
            .chain(late_mmx)
        {
            if features.contains(trigger) && !disabled.contains(implied) {
                features = features.union(X86Features::of(&[implied]));
            }
        }
        Self {
            arch,
            features,
            disabled: disabled.without(features),
            fpmath: request.fpmath,
            apx: Apx::resolve(arch.cpu(), &request.apx, rules),
        }
    }

    pub fn check_fpmath(self, flavor: CompilerFlavor) -> Result<(), String> {
        if flavor.is_gcc() {
            return Ok(());
        }
        let sse = self.features.contains(X86Feature::Sse);
        match self.fpmath {
            Some(FpMath::Sse) if !sse => {
                Err("the 'sse' unit is not supported with this instruction set".into())
            }
            Some(FpMath::X87) if sse => {
                Err("the '387' unit is not supported with this instruction set".into())
            }
            Some(FpMath::Both) => Err("unknown FP unit".into()),
            _ => Ok(()),
        }
    }

    fn wants_sse_math(self, family: TargetFamily, gcc: bool) -> bool {
        match self.fpmath {
            Some(FpMath::X87) => false,
            Some(FpMath::Sse | FpMath::Both) => true,
            None => !(gcc && family == TargetFamily::X86),
        }
    }

    pub fn x87_formats(self, family: TargetFamily) -> X87Formats {
        if !self.features.contains(X86Feature::X87) {
            return X87Formats::NONE;
        }
        if !self.wants_sse_math(family, true) || !self.features.contains(X86Feature::Sse) {
            return X87Formats {
                float: true,
                double: true,
            };
        }
        X87Formats {
            float: false,
            double: !self.features.contains(X86Feature::Sse2),
        }
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

    pub fn function_vector_register_bytes(
        self,
        family: TargetFamily,
        flavor: CompilerFlavor,
        features: &[TargetFeature],
    ) -> u64 {
        let mut arch = None;
        let mut request = X86IsaRequest::default();
        for feature in features {
            let (name, enabled) = match feature {
                TargetFeature::Arch(name) => {
                    arch = name.parse().ok();
                    continue;
                }
                TargetFeature::Enable(name) => (name, true),
                TargetFeature::Disable(name) => (name, false),
                TargetFeature::Tune(_) | TargetFeature::BranchProtection(_) => continue,
            };
            if let Some(feature) = ALL_FEATURES
                .iter()
                .copied()
                .find(|feature| feature.spelling() == name)
            {
                request.set(feature, enabled);
            }
        }
        let function = Self::resolve(family, arch, &request, flavor);
        self.vector_register_bytes()
            .max(function.vector_register_bytes())
    }

    pub fn predefines(self, family: TargetFamily, flavor: CompilerFlavor) -> Vec<String> {
        let gcc = flavor.is_gcc();
        let cpu_macros = match self.arch {
            arch if gcc && arch.is_level() => &["__k8", "__k8__"],
            arch => arch.cpu().macros,
        };
        let mut defines: Vec<String> = cpu_macros
            .iter()
            .copied()
            .filter(|name| !gcc || !name.starts_with("__tune_"))
            .filter(|name| !is_apx_macro(name))
            .chain(self.apx.macros())
            .map(|name| format!("{name}=1"))
            .collect();
        for feature in ALL_FEATURES.iter().copied() {
            let present = match feature {
                X86Feature::Sahf if family == TargetFamily::X86 => true,
                X86Feature::Cx16 if family == TargetFamily::X86 => false,
                _ => self.features.contains(feature),
            };
            if present {
                defines.extend(
                    feature
                        .macros_for(flavor)
                        .iter()
                        .map(|name| format!("{name}=1")),
                );
            }
        }
        let sse_math = self.wants_sse_math(family, gcc);
        for (feature, name) in [
            (X86Feature::Sse, "__SSE_MATH__"),
            (X86Feature::Sse2, "__SSE2_MATH__"),
        ] {
            if sse_math && self.features.contains(feature) {
                defines.push(format!("{name}=1"));
            }
        }
        if gcc {
            let fma = [X86Feature::Fma, X86Feature::Fma4, X86Feature::Avx512f]
                .into_iter()
                .any(|feature| self.features.contains(feature));
            if sse_math && fma {
                defines.extend([
                    "__FP_FAST_FMA=1".into(),
                    "__FP_FAST_FMAF=1".into(),
                    "__FP_FAST_FMAF32=1".into(),
                    "__FP_FAST_FMAF32x=1".into(),
                    "__FP_FAST_FMAF64=1".into(),
                ]);
            }
            if family == TargetFamily::X86_64 && self.features.contains(X86Feature::Sse2) {
                defines.push("__MMX_WITH_SSE__=1".into());
            }
            if !self.features.contains(X86Feature::X87) {
                defines.push("_SOFT_FLOAT=1".into());
            }
            if self.features.contains(X86Feature::Avx512vl) {
                defines.push("__EVEX256__=1".into());
            }
            defines.push(format!(
                "__BIGGEST_ALIGNMENT__={}",
                self.vector_register_bytes()
            ));
        }
        if let Some(method) = self.eval_method(family, flavor) {
            defines.push(format!("__FLT_EVAL_METHOD__={method}"));
            if gcc {
                defines.push(format!("__FLT_EVAL_METHOD_TS_18661_3__={method}"));
            }
        }
        defines
    }

    fn eval_method(self, family: TargetFamily, flavor: CompilerFlavor) -> Option<&'static str> {
        let has = |feature| self.features.contains(feature);
        if flavor.is_gcc() {
            let sse_math = self.wants_sse_math(family, true) && has(X86Feature::Sse);
            match family {
                TargetFamily::X86 if !has(X86Feature::X87) => Some("0"),
                TargetFamily::X86_64 if !has(X86Feature::X87) => None,
                TargetFamily::X86 if self.fpmath.is_none() => None,
                _ if !sse_math => Some("2"),
                _ if !has(X86Feature::Sse2) || self.fpmath == Some(FpMath::Both) => Some("-1"),
                _ if self.fpmath.is_some() => Some("0"),
                _ => None,
            }
        } else if has(X86Feature::Sse) {
            Some("0")
        } else {
            Some("2")
        }
    }

    pub fn disabled_macros(self, flavor: CompilerFlavor) -> Vec<&'static str> {
        let mut names: Vec<&'static str> = ALL_FEATURES
            .iter()
            .copied()
            .filter(|feature| self.disabled.contains(*feature))
            .flat_map(|feature| feature.macros_for(flavor).iter().copied())
            .collect();
        for (feature, name) in [
            (X86Feature::Sse, "__SSE_MATH__"),
            (X86Feature::Sse2, "__SSE2_MATH__"),
        ] {
            if self.disabled.contains(feature) || self.fpmath == Some(FpMath::X87) {
                names.push(name);
            }
        }
        names
    }
}
