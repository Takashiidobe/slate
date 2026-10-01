use slate_parser::compiler_args::{CompilerArgError, CompilerArgParser, CompilerArgs};
use slate_parser::target_info::{
    LongDoubleFormat, TargetEnvironment, TargetFamily, TargetInfo, TargetOs,
};

pub use slate_parser::target_info::TargetError;

pub fn active_target() -> String {
    std::env::var("SLATE_TARGET")
        .ok()
        .filter(|target| !target.trim().is_empty())
        .unwrap_or_else(|| env!("SLATE_BUILD_TARGET").to_string())
}

pub fn compiler_arguments(extra_args: &[String]) -> Vec<String> {
    std::iter::once(format!("--target={}", active_target()))
        .chain(
            std::env::var("SLATE_CLANG_ARGS")
                .unwrap_or_default()
                .split_whitespace()
                .map(str::to_string),
        )
        .chain(extra_args.iter().cloned())
        .collect()
}

pub fn parse_args(extra_args: &[String]) -> Result<CompilerArgs, CompilerArgError> {
    CompilerArgParser::parse(compiler_arguments(extra_args))
}

pub fn active_info() -> TargetInfo {
    let args = parse_args(&[]).unwrap_or_else(|error| panic!("active target required: {error}"));
    args.options.effective_target(args.target)
}

pub struct TargetConfig {
    pub arch: &'static str,
    pub endian: &'static str,
    pub env: &'static str,
    pub long_bits: u32,
    pub long_double_bits: u32,
    pub os: &'static str,
    pub pointer_width: String,
    pub vendor: &'static str,
}

pub fn target_config(target: &str) -> Result<TargetConfig, TargetError> {
    let info = TargetInfo::for_triple(target)?;
    Ok(TargetConfig {
        arch: match info.family {
            TargetFamily::X86_64 => "x86_64",
            TargetFamily::X86 => "x86",
            TargetFamily::AArch64 => "aarch64",
            TargetFamily::Arm32 => "arm",
        },
        endian: info.endian.as_str(),
        env: match info.environment {
            TargetEnvironment::Gnu | TargetEnvironment::GnuEabi | TargetEnvironment::GnuEabiHf => {
                "gnu"
            }
            TargetEnvironment::Msvc => "msvc",
            TargetEnvironment::Darwin | TargetEnvironment::Android | TargetEnvironment::FreeBsd => {
                ""
            }
        },
        long_bits: info.long_width,
        long_double_bits: match info.long_double {
            LongDoubleFormat::Binary64 => 64,
            LongDoubleFormat::X87 => 80,
            LongDoubleFormat::Binary128 => 128,
        },
        os: match info.os {
            TargetOs::Linux => "linux",
            TargetOs::Windows => "windows",
            TargetOs::Darwin => "macos",
            TargetOs::Android => "android",
            TargetOs::FreeBsd => "freebsd",
        },
        pointer_width: info.pointer_width.to_string(),
        vendor: match info.triple.split('-').nth(1) {
            Some("apple") => "apple",
            Some("pc") => "pc",
            _ => "unknown",
        },
    })
}

pub fn char_is_signed_default(target: &str) -> bool {
    TargetInfo::for_triple(target)
        .unwrap_or_else(|error| panic!("char target mapping required for `{target}`: {error}"))
        .char_signed
}

pub fn long_double_bits(target: &str) -> u32 {
    target_config(target)
        .unwrap_or_else(|error| {
            panic!("long double target mapping required for `{target}`: {error}")
        })
        .long_double_bits
}

pub fn active_long_double_bits() -> u32 {
    match active_info().long_double {
        LongDoubleFormat::Binary64 => 64,
        LongDoubleFormat::X87 => 80,
        LongDoubleFormat::Binary128 => 128,
    }
}

pub fn active_long_bits() -> u32 {
    active_info().long_width
}

pub fn target_has_native_fma(_bits: u32) -> bool {
    let info = active_info();
    info.isa
        .predefines(
            info.family,
            slate_parser::compiler_args::CompilerFlavor::Clang,
            false,
        )
        .iter()
        .any(|definition| {
            definition.starts_with("__FMA__=") || definition.starts_with("__ARM_FEATURE_FMA=")
        })
}

pub fn target_override_args(target: &str) -> Result<Vec<String>, TargetError> {
    let info = TargetInfo::for_triple(target)?;
    Ok(vec!["-target".into(), info.triple])
}
