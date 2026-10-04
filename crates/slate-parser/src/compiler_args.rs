use crate::compiler_options::{
    CodegenOptions, CompilerOptions, LayoutOptions, LibraryBuiltins, MicrosoftFlags,
    OperationValues, Pic, StackProtector,
};
use crate::diagnostics::{DiagnosticOptions, Warning};
use crate::files::SearchPaths;
use crate::ir::{AsmDialect, Overflow};
use crate::pp::{MacroOption, PreprocessorInputs};
use crate::rules::{Rule, Rules};
use crate::target::isa::{IsaRequest, TargetIsa};
use crate::target::x86_isa::X86Feature;
use crate::target_info::{LongDoubleFormat, TargetEnvironment, TargetFamily, TargetInfo, TargetOs};
use crate::target_registry::{ArchMode, arch_variant};
use crate::{compiler_headers, sysroot};
use std::path::{Path, PathBuf};
use std::str::FromStr;

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum CompilerFlavor {
    Gcc,
    #[default]
    Clang,
    Msvc,
}

impl FromStr for CompilerFlavor {
    type Err = String;

    fn from_str(name: &str) -> Result<Self, Self::Err> {
        match name {
            "gcc" => Ok(Self::Gcc),
            "clang" => Ok(Self::Clang),
            "msvc" => Ok(Self::Msvc),
            _ => Err(format!("unknown compiler flavor: {name}")),
        }
    }
}

impl CompilerFlavor {
    pub const fn is_gcc(self) -> bool {
        matches!(self, Self::Gcc)
    }

    pub const fn is_clang(self) -> bool {
        matches!(self, Self::Clang)
    }

    pub const fn is_msvc(self) -> bool {
        matches!(self, Self::Msvc)
    }
}

impl std::fmt::Display for CompilerFlavor {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        formatter.write_str(match self {
            Self::Gcc => "gcc",
            Self::Clang => "clang",
            Self::Msvc => "msvc",
        })
    }
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum LanguageStandard {
    C89,
    C94,
    Gnu89,
    C99,
    Gnu99,
    C11,
    Gnu11,
    C17,
    Gnu17,
    C23,
    #[default]
    Gnu23,
    C2y,
    Gnu2y,
}

impl LanguageStandard {
    pub fn is_gnu(self) -> bool {
        matches!(
            self,
            Self::Gnu89 | Self::Gnu99 | Self::Gnu11 | Self::Gnu17 | Self::Gnu23 | Self::Gnu2y
        )
    }

    pub fn predefined_stdc_version(self, flavor: CompilerFlavor) -> Option<i64> {
        match (self, flavor) {
            (Self::C2y | Self::Gnu2y, CompilerFlavor::Gcc) => Some(202500),
            _ => self.stdc_version(),
        }
    }

    pub fn stdc_version(self) -> Option<i64> {
        match self {
            Self::C89 | Self::Gnu89 => None,
            Self::C94 => Some(199409),
            Self::C99 | Self::Gnu99 => Some(199901),
            Self::C11 | Self::Gnu11 => Some(201112),
            Self::C17 | Self::Gnu17 => Some(201710),
            Self::C23 | Self::Gnu23 => Some(202311),
            Self::C2y | Self::Gnu2y => Some(202400),
        }
    }

    pub fn at_least_c99(self) -> bool {
        self.stdc_version() >= Some(199901)
    }

    pub fn at_least_c11(self) -> bool {
        self.stdc_version() >= Some(201112)
    }

    pub fn at_least_c23(self) -> bool {
        self.stdc_version() >= Some(202311)
    }
}

impl FromStr for LanguageStandard {
    type Err = String;

    fn from_str(name: &str) -> Result<Self, Self::Err> {
        match name {
            "c89" | "c90" | "iso9899:1990" => Ok(Self::C89),
            "iso9899:199409" => Ok(Self::C94),
            "gnu89" | "gnu90" => Ok(Self::Gnu89),
            "c99" | "c9x" | "iso9899:1999" | "iso9899:199x" => Ok(Self::C99),
            "gnu99" | "gnu9x" => Ok(Self::Gnu99),
            "c11" | "c1x" | "iso9899:2011" => Ok(Self::C11),
            "gnu11" | "gnu1x" => Ok(Self::Gnu11),
            "c17" | "c18" | "iso9899:2017" | "iso9899:2018" => Ok(Self::C17),
            "gnu17" | "gnu18" => Ok(Self::Gnu17),
            "c23" | "c2x" | "iso9899:2024" => Ok(Self::C23),
            "gnu23" | "gnu2x" => Ok(Self::Gnu23),
            "c2y" => Ok(Self::C2y),
            "gnu2y" => Ok(Self::Gnu2y),
            _ => Err(format!("unknown language standard: {name}")),
        }
    }
}

#[derive(Debug, Default, PartialEq, Eq)]
pub struct CompilerArgs {
    pub options: CompilerOptions,
    pub defines: Vec<String>,
    pub preprocessor_inputs: PreprocessorInputs,
    pub standard: LanguageStandard,
    pub include: Vec<String>,
    pub iquote: Vec<String>,
    pub isystem: Vec<String>,
    pub idirafter: Vec<String>,
    pub isysroot: Option<String>,
    pub sysroot: Option<String>,
    pub nostdlibinc: bool,
    pub nostdinc: bool,
    pub flavor: CompilerFlavor,
    pub target: TargetInfo,
}

impl CompilerArgs {
    pub fn search_paths(&self) -> SearchPaths {
        let root = self
            .isysroot
            .as_ref()
            .or(self.sysroot.as_ref())
            .map_or_else(|| sysroot::path(&self.target.triple), PathBuf::from);
        let include_dir = |path: &str| match path.strip_prefix('=') {
            Some(relative) => root.join(relative.trim_start_matches('/')),
            None => PathBuf::from(path),
        };
        let mut system: Vec<PathBuf> = self.isystem.iter().map(|path| include_dir(path)).collect();
        if !self.nostdinc {
            system.extend(compiler_headers::include_paths(&self.target, self.flavor));
        }
        if !self.nostdlibinc && !self.nostdinc {
            system.extend(sysroot::include_paths_at(&root, &self.target, self.flavor));
        }
        system.extend(self.idirafter.iter().map(|path| include_dir(path)));
        SearchPaths {
            quote: self.iquote.iter().map(|path| include_dir(path)).collect(),
            user: self.include.iter().map(|path| include_dir(path)).collect(),
            system,
        }
    }

    fn resolve_paths_in(&mut self, directory: &Path) {
        let resolve = |path: &Path| {
            let path = directory.join(path);
            path.canonicalize().unwrap_or(path)
        };
        let resolve_dir = |path: &mut String| {
            if !path.starts_with('=') {
                *path = resolve(Path::new(path)).to_string_lossy().into_owned();
            }
        };
        self.include
            .iter_mut()
            .chain(&mut self.iquote)
            .chain(&mut self.isystem)
            .chain(&mut self.idirafter)
            .chain(&mut self.isysroot)
            .chain(&mut self.sysroot)
            .for_each(resolve_dir);
        let inputs = &mut self.preprocessor_inputs;
        inputs
            .includes
            .iter_mut()
            .chain(&mut inputs.imacros)
            .for_each(|path| *path = resolve(path));
    }
}

#[derive(Debug, thiserror::Error)]
pub enum CompilerArgError {
    #[error("invalid compiler argument `{argument}`: {reason}")]
    Invalid { argument: String, reason: String },
    #[error(transparent)]
    Target(#[from] crate::target_info::TargetError),
    #[error(transparent)]
    Rule(#[from] crate::rules::RuleError),
}

impl miette::Diagnostic for CompilerArgError {}

#[derive(Debug, Default)]
struct ParsedCompilerArgs {
    defines: Vec<String>,
    preprocessor_inputs: PreprocessorInputs,
    standard: Option<LanguageStandard>,
    include: Vec<String>,
    iquote: Vec<String>,
    isystem: Vec<String>,
    idirafter: Vec<String>,
    isysroot: Option<String>,
    sysroot: Option<String>,
    nostdlibinc: bool,
    nostdinc: bool,
    flavor: CompilerFlavor,
    target: String,
    arch_mode: Option<ArchMode>,
    regparm: Option<u8>,
    hosted: Option<bool>,
    builtin: Option<bool>,
    no_builtin: Vec<String>,
    char_signed: Option<bool>,
    short_wchar: Option<bool>,
    ms_anonymous_structs: Option<bool>,
    strict_flex_arrays: Option<u8>,
    late_parsed_attributes: Option<bool>,
    asynchronous_unwind_tables: Option<bool>,
    pic: Option<Pic>,
    stack_protector: Option<StackProtector>,
    cf_protection: Vec<u8>,
    three_dnow: Option<u8>,
    preferred_stack_boundary: Option<u32>,
    stack_alignment: Option<u32>,
    wrapv: Option<bool>,
    trapv: Option<bool>,
    signed_overflow: Overflow,
    strict_overflow: Option<bool>,
    rounding_math: Option<bool>,
    trapping_math: Option<bool>,
    gnu89_inline: Option<bool>,
    common: Option<bool>,
    ms_extensions: Option<bool>,
    ms_compatibility: Option<bool>,
    asm_blocks: Option<bool>,
    long_double: Option<LongDoubleFormat>,
    asm_dialect: Option<AsmDialect>,
    optimization: Option<Optimization>,
    isa: IsaRequest,
    diagnostics: DiagnosticOptions,
    occurrences: Vec<(Opt, String)>,
}

impl ParsedCompilerArgs {
    fn saw(&mut self, opt: Opt, argument: &str) {
        self.occurrences.push((opt, argument.to_owned()));
    }

    fn has(&self, opt: Opt) -> bool {
        self.occurrences.iter().any(|(seen, _)| *seen == opt)
    }

    fn check_flavor(&self) -> Result<(), CompilerArgError> {
        match self.occurrences.iter().find(|(opt, argument)| {
            if opt.parse_switch(argument) == Some(false) {
                !opt.negation_accepted_by(self.flavor)
            } else {
                !opt.accepted_by(self.flavor)
            }
        }) {
            Some((_, argument)) => Err(invalid(
                argument,
                &format!("unknown option for the {} flavor", self.flavor),
            )),
            None => Ok(()),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
enum Opt {
    Define,
    Undef,
    ForceInclude,
    Imacros,
    Standard,
    Include,
    Iquote,
    Isystem,
    Idirafter,
    Isysroot,
    Sysroot,
    Nostdlibinc,
    Nostdinc,
    Flavor,
    Target,
    ArchMode,
    RegParm,
    Freestanding,
    Hosted,
    Builtin,
    NoBuiltinFunction,
    UnsignedChar,
    SignedChar,
    ShortWchar,
    MsAnonymousStructs,
    StrictFlexArrays,
    GccStrictFlexArrays,
    LateParseAttributes,
    AsynchronousUnwindTables,
    SmallPic,
    LargePic,
    SmallPie,
    LargePie,
    StackProtector,
    StackProtectorLevel,
    StackProtectorExplicit,
    CfProtection,
    CodeModel,
    X87,
    ClangX87,
    FpRetIn387,
    ThreeDNow,
    ThreeDNowA,
    PreferredStackBoundary,
    StackAlignment,
    Wrapv,
    Trapv,
    StrictOverflow,
    RoundingMath,
    TrappingMath,
    Gnu89Inline,
    Common,
    MsExtensions,
    MsCompatibility,
    AsmBlocks,
    LongDouble,
    AsmDialect,
    IsaFeature,
    Arch,
    FloatAbi,
    Fpu,
    Thumb,
    SveVectorBits,
    ClangCodegenOnly,
    Warning,
    Pedantic,
    Optimize,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Optimization {
    None,
    Speed,
    Size,
}

impl std::fmt::Display for Opt {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let name = match self {
            Self::Define => "define",
            Self::Undef => "undef",
            Self::ForceInclude => "include",
            Self::Imacros => "imacros",
            Self::Standard => "std",
            Self::Include => "I",
            Self::Iquote => "iquote",
            Self::Isystem => "isystem",
            Self::Idirafter => "idirafter",
            Self::Isysroot => "isysroot",
            Self::Sysroot => "sysroot",
            Self::Nostdlibinc => "nostdlibinc",
            Self::Nostdinc => "nostdinc",
            Self::Flavor => "flavor",
            Self::Target => "target",
            Self::ArchMode => "m16|m32|m64|mx32",
            Self::RegParm => "mregparm",
            Self::Freestanding => "ffreestanding",
            Self::Hosted => "fhosted",
            Self::Builtin => "fbuiltin",
            Self::NoBuiltinFunction => "fno-builtin-<function>",
            Self::UnsignedChar => "funsigned-char",
            Self::SignedChar => "fsigned-char",
            Self::ShortWchar => "fshort-wchar",
            Self::MsAnonymousStructs => "fms-anonymous-structs",
            Self::StrictFlexArrays => "fstrict-flex-arrays=",
            Self::GccStrictFlexArrays => "fstrict-flex-arrays",
            Self::LateParseAttributes => "fexperimental-late-parse-attributes",
            Self::AsynchronousUnwindTables => "fasynchronous-unwind-tables",
            Self::SmallPic => "fpic",
            Self::LargePic => "fPIC",
            Self::SmallPie => "fpie",
            Self::LargePie => "fPIE",
            Self::StackProtector => "fstack-protector",
            Self::StackProtectorLevel => "fstack-protector-<level>",
            Self::StackProtectorExplicit => "fstack-protector-explicit",
            Self::CfProtection => "fcf-protection",
            Self::CodeModel => "mcmodel",
            Self::X87 => "m80387",
            Self::ClangX87 => "mx87",
            Self::FpRetIn387 => "mfp-ret-in-387",
            Self::ThreeDNow => "m3dnow",
            Self::ThreeDNowA => "m3dnowa",
            Self::PreferredStackBoundary => "preferred-stack-boundary",
            Self::StackAlignment => "stack-alignment",
            Self::Wrapv => "wrapv",
            Self::Trapv => "trapv",
            Self::StrictOverflow => "strict-overflow",
            Self::RoundingMath => "rounding-math",
            Self::TrappingMath => "trapping-math",
            Self::Gnu89Inline => "gnu89-inline",
            Self::Common => "common",
            Self::MsExtensions => "ms-extensions",
            Self::MsCompatibility => "ms-compatibility",
            Self::AsmBlocks => "asm-blocks",
            Self::LongDouble => "long-double",
            Self::AsmDialect => "masm",
            Self::IsaFeature => "m<feature>",
            Self::Arch => "march",
            Self::FloatAbi => "mfloat-abi",
            Self::Fpu => "mfpu",
            Self::Thumb => "mthumb",
            Self::SveVectorBits => "msve-vector-bits",
            Self::ClangCodegenOnly => "m<clang codegen flag>",
            Self::Warning => "W",
            Self::Pedantic => "pedantic",
            Self::Optimize => "O",
        };
        formatter.write_str(name)
    }
}

impl Opt {
    fn accepted_by(self, flavor: CompilerFlavor) -> bool {
        use CompilerFlavor::{Clang, Gcc, Msvc};
        let flavors: &[CompilerFlavor] = match self {
            Self::Define
            | Self::Undef
            | Self::ForceInclude
            | Self::Imacros
            | Self::Standard
            | Self::Include
            | Self::Iquote
            | Self::Isystem
            | Self::Idirafter
            | Self::Isysroot
            | Self::Sysroot
            | Self::Nostdlibinc
            | Self::Nostdinc
            | Self::Flavor
            | Self::Target
            | Self::Warning
            | Self::Pedantic => &[Gcc, Clang, Msvc],
            Self::Wrapv
            | Self::Trapv
            | Self::StrictOverflow
            | Self::RoundingMath
            | Self::TrappingMath
            | Self::Gnu89Inline
            | Self::Common
            | Self::MsExtensions
            | Self::MsCompatibility
            | Self::LongDouble
            | Self::AsmDialect
            | Self::IsaFeature
            | Self::Arch
            | Self::FloatAbi
            | Self::Fpu
            | Self::Thumb
            | Self::SveVectorBits
            | Self::Optimize
            | Self::Freestanding
            | Self::Hosted
            | Self::Builtin
            | Self::NoBuiltinFunction
            | Self::UnsignedChar
            | Self::SignedChar
            | Self::ShortWchar
            | Self::StrictFlexArrays
            | Self::AsynchronousUnwindTables
            | Self::SmallPic
            | Self::LargePic
            | Self::SmallPie
            | Self::LargePie
            | Self::StackProtector
            | Self::StackProtectorLevel
            | Self::CfProtection
            | Self::CodeModel
            | Self::ArchMode
            | Self::RegParm
            | Self::X87
            | Self::ThreeDNow
            | Self::ThreeDNowA => &[Gcc, Clang],
            Self::PreferredStackBoundary
            | Self::GccStrictFlexArrays
            | Self::StackProtectorExplicit
            | Self::FpRetIn387 => &[Gcc],
            Self::StackAlignment
            | Self::ClangX87
            | Self::ClangCodegenOnly
            | Self::AsmBlocks
            | Self::MsAnonymousStructs
            | Self::LateParseAttributes => &[Clang],
        };
        flavors.contains(&flavor)
    }

    fn negation_accepted_by(self, flavor: CompilerFlavor) -> bool {
        match self {
            Self::Freestanding | Self::Hosted => flavor.is_gcc(),
            Self::FpRetIn387 => !flavor.is_msvc(),
            _ => self.accepted_by(flavor),
        }
    }

    fn switch(self) -> Option<&'static str> {
        Some(match self {
            Self::Wrapv => "-fwrapv",
            Self::Trapv => "-ftrapv",
            Self::StrictOverflow => "-fstrict-overflow",
            Self::RoundingMath => "-frounding-math",
            Self::TrappingMath => "-ftrapping-math",
            Self::Gnu89Inline => "-fgnu89-inline",
            Self::Common => "-fcommon",
            Self::MsExtensions => "-fms-extensions",
            Self::MsCompatibility => "-fms-compatibility",
            Self::AsmBlocks => "-fasm-blocks",
            Self::Freestanding => "-ffreestanding",
            Self::Hosted => "-fhosted",
            Self::Builtin => "-fbuiltin",
            Self::UnsignedChar => "-funsigned-char",
            Self::SignedChar => "-fsigned-char",
            Self::ShortWchar => "-fshort-wchar",
            Self::MsAnonymousStructs => "-fms-anonymous-structs",
            Self::GccStrictFlexArrays => "-fstrict-flex-arrays",
            Self::LateParseAttributes => "-fexperimental-late-parse-attributes",
            Self::AsynchronousUnwindTables => "-fasynchronous-unwind-tables",
            Self::SmallPic => "-fpic",
            Self::LargePic => "-fPIC",
            Self::SmallPie => "-fpie",
            Self::LargePie => "-fPIE",
            Self::StackProtector => "-fstack-protector",
            Self::X87 => "-m80387",
            Self::ClangX87 => "-mx87",
            Self::FpRetIn387 => "-mfp-ret-in-387",
            Self::ThreeDNow => "-m3dnow",
            Self::ThreeDNowA => "-m3dnowa",
            _ => return None,
        })
    }

    fn parse_switch(self, argument: &str) -> Option<bool> {
        parse_switch(self.switch()?, argument)
    }
}

fn parse_switch(switch: &str, argument: &str) -> Option<bool> {
    let argument = argument
        .strip_prefix('-')
        .filter(|rest| rest.starts_with('-'))
        .unwrap_or(argument);
    let (prefix, name) = switch.split_at(2);
    let rest = argument.strip_prefix(prefix)?;
    match rest.strip_prefix("no-") {
        Some(negated) if negated == name => Some(false),
        _ if rest == name => Some(true),
        _ => None,
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum IgnoredOption {
    Alone,
    TakesValue,
}

const IGNORED_DRIVER_FLAGS: [&str; 8] = ["-c", "-MD", "-MMD", "-MP", "-MG", "-M", "-MM", "-pipe"];

const IGNORED_VALUE_OPTIONS: [&str; 5] = ["-o", "-MF", "-MT", "-MQ", "-MJ"];

const CODEGEN_ONLY_FLAGS: [&str; 12] = [
    "-fomit-frame-pointer",
    "-flto",
    "-ffunction-sections",
    "-fdata-sections",
    "-fstrict-aliasing",
    "-fplt",
    "-fsemantic-interposition",
    "-funwind-tables",
    "-fstack-clash-protection",
    "-fmerge-all-constants",
    "-fident",
    "-faddrsig",
];

const CODEGEN_ONLY_VALUE_FLAGS: [&str; 3] = ["lto=", "visibility=", "debug-prefix-map="];

const CLANG_CODEGEN_ONLY_M_FLAGS: [&str; 1] = ["-moutline"];

fn clang_codegen_only(argument: &str) -> bool {
    CLANG_CODEGEN_ONLY_M_FLAGS
        .iter()
        .any(|switch| parse_switch(switch, argument).is_some())
}

fn ignored_option(argument: &str) -> Option<IgnoredOption> {
    if IGNORED_VALUE_OPTIONS.contains(&argument) {
        return Some(IgnoredOption::TakesValue);
    }
    let joined_value = IGNORED_VALUE_OPTIONS
        .iter()
        .any(|option| argument.len() > option.len() && argument.starts_with(option));
    let codegen_only = CODEGEN_ONLY_FLAGS
        .iter()
        .any(|switch| parse_switch(switch, argument).is_some())
        || CODEGEN_ONLY_VALUE_FLAGS
            .iter()
            .any(|prefix| argument.starts_with(&format!("-f{prefix}")));
    (IGNORED_DRIVER_FLAGS.contains(&argument)
        || argument.starts_with("-g")
        || joined_value
        || codegen_only)
        .then_some(IgnoredOption::Alone)
}

const SWITCH_OPTS: [Opt; 30] = [
    Opt::SmallPic,
    Opt::LargePic,
    Opt::SmallPie,
    Opt::LargePie,
    Opt::StackProtector,
    Opt::X87,
    Opt::ClangX87,
    Opt::FpRetIn387,
    Opt::ThreeDNow,
    Opt::ThreeDNowA,
    Opt::Freestanding,
    Opt::Hosted,
    Opt::GccStrictFlexArrays,
    Opt::Builtin,
    Opt::UnsignedChar,
    Opt::SignedChar,
    Opt::ShortWchar,
    Opt::MsAnonymousStructs,
    Opt::LateParseAttributes,
    Opt::AsynchronousUnwindTables,
    Opt::Gnu89Inline,
    Opt::Common,
    Opt::MsExtensions,
    Opt::MsCompatibility,
    Opt::AsmBlocks,
    Opt::Wrapv,
    Opt::Trapv,
    Opt::StrictOverflow,
    Opt::RoundingMath,
    Opt::TrappingMath,
];

pub struct CompilerArgParser;

impl CompilerArgParser {
    pub fn parse<I>(args: I) -> Result<CompilerArgs, CompilerArgError>
    where
        I: IntoIterator<Item = String>,
    {
        let arguments = args.into_iter().collect::<Vec<_>>();
        let raw = parse_arguments(&arguments)?;
        raw.check_flavor()?;
        let triple = match raw.arch_mode {
            Some(mode) => arch_variant(&raw.target, mode)?,
            None => raw.target.clone(),
        };
        let mut target = TargetInfo::for_triple_and_flavor(&triple, raw.flavor)?;
        validate_rules(&target).check(&raw)?;
        if target.family == TargetFamily::X86 {
            target.abi.default_regparm = raw.regparm.unwrap_or(0);
        }
        target.isa = TargetIsa::resolve(target.family, target.environment, &raw.isa, raw.flavor)
            .map_err(|reason| invalid(&raw.target, &reason))?;
        let flavor = raw.flavor;
        let codegen = codegen_options(&raw, &target);
        let layout = LayoutOptions {
            long_double: raw.long_double,
            preferred_stack_alignment: raw
                .preferred_stack_boundary
                .map(|exponent| 1u32 << exponent)
                .or(raw.stack_alignment),
            char_signed: raw.char_signed,
            short_wchar: raw.short_wchar,
        };
        let mut options = CompilerOptions::from_values(
            flavor,
            layout,
            raw.diagnostics,
            OperationValues {
                signed_overflow: raw.signed_overflow,
                strict_overflow: raw.strict_overflow,
                rounding_math: raw.rounding_math,
                trapping_math: raw.trapping_math,
            },
        );
        options.inline_semantics = raw.gnu89_inline.map(|enabled| {
            use crate::compiler_options::InlineSemantics;
            if enabled {
                InlineSemantics::SupressDef
            } else {
                InlineSemantics::ProvideDef
            }
        });
        options.codegen = codegen;
        options.common = raw.common.unwrap_or(false);
        options.microsoft = MicrosoftFlags {
            extensions: raw.ms_extensions,
            compatibility: raw.ms_compatibility,
            asm_blocks: raw.asm_blocks == Some(true),
            anonymous_structs: raw.ms_anonymous_structs,
        };
        options.asm_dialect = raw.asm_dialect.unwrap_or_default();
        options.explicit_standard = raw.standard.is_some();
        options.hosted = raw.hosted.unwrap_or(true);
        options.library_builtins = LibraryBuiltins {
            enabled: options.hosted && raw.builtin.unwrap_or(true),
            disabled: raw.no_builtin,
        };
        options.implicit_stdc_predef = options.hosted && !raw.nostdinc;
        options.asynchronous_unwind_tables = raw
            .asynchronous_unwind_tables
            .unwrap_or(options.hosted || !flavor.is_clang());
        options.late_parsed_attributes = raw.late_parsed_attributes == Some(true);
        options.strict_flex_arrays = raw.strict_flex_arrays.unwrap_or(0);
        Ok(CompilerArgs {
            options,
            defines: raw.defines,
            preprocessor_inputs: raw.preprocessor_inputs,
            standard: raw.standard.unwrap_or_default(),
            include: raw.include,
            iquote: raw.iquote,
            isystem: raw.isystem,
            idirafter: raw.idirafter,
            isysroot: raw.isysroot,
            sysroot: raw.sysroot,
            nostdlibinc: raw.nostdlibinc,
            nostdinc: raw.nostdinc,
            flavor,
            target,
        })
    }

    pub fn parse_in<I>(args: I, directory: &Path) -> Result<CompilerArgs, CompilerArgError>
    where
        I: IntoIterator<Item = String>,
    {
        let mut parsed = Self::parse(args)?;
        parsed.resolve_paths_in(directory);
        Ok(parsed)
    }
}

fn parse_pedantic(argument: &str, diagnostics: &mut DiagnosticOptions) -> bool {
    match argument {
        "-pedantic" | "--pedantic" | "-Wpedantic" => diagnostics.pedantic = true,
        "-pedantic-errors" | "--pedantic-errors" => diagnostics.pedantic_errors = true,
        "-Wno-pedantic" => {
            diagnostics.pedantic = false;
            diagnostics.pedantic_errors = false;
        }
        _ => return false,
    }
    true
}

// unrecognized -W names are accepted and ignored, as clang does by default
fn apply_warning_flag(name: &str, diagnostics: &mut DiagnosticOptions) {
    if name == "error" {
        diagnostics.werror = true;
    } else if name == "no-error" {
        diagnostics.werror = false;
    } else if let Some(name) = name.strip_prefix("error=") {
        if let Some(warning) = Warning::from_name(name) {
            diagnostics.set_error(warning, true);
        }
    } else if let Some(name) = name.strip_prefix("no-error=") {
        if let Some(warning) = Warning::from_name(name) {
            diagnostics.set_error(warning, false);
        }
    } else if let Some(name) = name.strip_prefix("no-") {
        if let Some(warning) = Warning::from_name(name) {
            diagnostics.set_enabled(warning, false);
        }
    } else if let Some(warning) = Warning::from_name(name) {
        diagnostics.set_enabled(warning, true);
    }
}

fn parse_arguments(arguments: &[String]) -> Result<ParsedCompilerArgs, CompilerArgError> {
    let mut parsed = ParsedCompilerArgs {
        flavor: CompilerFlavor::Clang,
        target: "x86_64-unknown-linux-gnu".into(),
        ..ParsedCompilerArgs::default()
    };
    let mut index = 0;
    while index < arguments.len() {
        let argument = &arguments[index];
        if let Some(ignored) = ignored_option(argument) {
            if ignored == IgnoredOption::TakesValue {
                next_value(arguments, &mut index, argument, "")?;
            }
        } else if let Some((opt, value)) = SWITCH_OPTS
            .iter()
            .find_map(|opt| opt.parse_switch(argument).map(|value| (*opt, value)))
        {
            parsed.saw(opt, argument);
            match opt {
                Opt::Wrapv => parsed.wrapv = Some(value),
                Opt::Trapv => parsed.trapv = Some(value),
                Opt::StrictOverflow => parsed.strict_overflow = Some(value),
                Opt::RoundingMath => parsed.rounding_math = Some(value),
                Opt::TrappingMath => parsed.trapping_math = Some(value),
                Opt::Gnu89Inline => parsed.gnu89_inline = Some(value),
                Opt::Common => parsed.common = Some(value),
                Opt::MsExtensions => {
                    parsed.ms_extensions = Some(value);
                    parsed.ms_anonymous_structs = Some(value);
                }
                Opt::MsCompatibility => {
                    parsed.ms_compatibility = Some(value);
                    if value {
                        parsed.ms_anonymous_structs = Some(true);
                    }
                }
                Opt::AsmBlocks => parsed.asm_blocks = Some(value),
                Opt::Builtin => parsed.builtin = Some(value),
                Opt::UnsignedChar => parsed.char_signed = Some(!value),
                Opt::SignedChar => parsed.char_signed = Some(value),
                Opt::ShortWchar => parsed.short_wchar = Some(value),
                Opt::MsAnonymousStructs => parsed.ms_anonymous_structs = Some(value),
                Opt::LateParseAttributes => parsed.late_parsed_attributes = Some(value),
                Opt::AsynchronousUnwindTables => parsed.asynchronous_unwind_tables = Some(value),
                Opt::Freestanding => parsed.hosted = Some(!value),
                Opt::Hosted => parsed.hosted = Some(value),
                Opt::GccStrictFlexArrays => {
                    parsed.strict_flex_arrays = Some(if value { 3 } else { 0 })
                }
                Opt::SmallPic | Opt::LargePic | Opt::SmallPie | Opt::LargePie => {
                    parsed.pic = Some(if value {
                        Pic {
                            level: if matches!(opt, Opt::SmallPic | Opt::SmallPie) {
                                1
                            } else {
                                2
                            },
                            executable: matches!(opt, Opt::SmallPie | Opt::LargePie),
                        }
                    } else {
                        Pic::OFF
                    })
                }
                Opt::StackProtector => {
                    parsed.stack_protector = Some(if value {
                        StackProtector::On
                    } else {
                        StackProtector::Off
                    })
                }
                Opt::X87 | Opt::ClangX87 => parsed.isa.x86.set(X86Feature::X87, value),
                Opt::FpRetIn387 => {
                    if !value {
                        parsed.isa.x86.set_for_clang(X86Feature::X87, false);
                    }
                }
                Opt::ThreeDNow => parsed.three_dnow = Some(u8::from(value)),
                Opt::ThreeDNowA => parsed.three_dnow = Some(if value { 2 } else { 0 }),
                _ => return Err(invalid(argument, "unknown flag")),
            }
        } else if let Some(level) = match argument.as_str() {
            "-fstack-protector-strong" => Some((Opt::StackProtectorLevel, StackProtector::Strong)),
            "-fstack-protector-all" => Some((Opt::StackProtectorLevel, StackProtector::All)),
            "-fstack-protector-explicit" => {
                Some((Opt::StackProtectorExplicit, StackProtector::Explicit))
            }
            _ => None,
        } {
            parsed.saw(level.0, argument);
            parsed.stack_protector = Some(level.1);
        } else if argument == "-fcf-protection" || argument.starts_with("-fcf-protection=") {
            parsed.saw(Opt::CfProtection, argument);
            parsed.cf_protection.push(
                match argument.strip_prefix("-fcf-protection").unwrap_or_default() {
                    "" | "=full" => 3,
                    "=none" => 0,
                    "=branch" => 1,
                    "=return" => 2,
                    "=check" => 8,
                    _ => return Err(invalid(argument, "unknown control-flow protection level")),
                },
            );
        } else if let Some(model) = argument.strip_prefix("-mcmodel=") {
            parsed.saw(Opt::CodeModel, argument);
            if !["tiny", "small", "kernel", "medium", "large"].contains(&model) {
                return Err(invalid(argument, "unknown code model"));
            }
        } else if let Some(mode) = match argument.as_str() {
            "-m16" => Some(ArchMode::Code16),
            "-m32" => Some(ArchMode::Bits32),
            "-m64" => Some(ArchMode::Bits64),
            "-mx32" => Some(ArchMode::X32),
            _ => None,
        } {
            parsed.saw(Opt::ArchMode, argument);
            parsed.arch_mode = Some(mode);
        } else if let Some(count) = argument.strip_prefix("-mregparm=") {
            parsed.saw(Opt::RegParm, argument);
            parsed.regparm = Some(parse_value(count.to_owned(), argument, "register count")?);
        } else if let Some(function) = argument
            .strip_prefix("-fno-builtin-")
            .filter(|function| !function.is_empty())
        {
            parsed.saw(Opt::NoBuiltinFunction, argument);
            parsed.no_builtin.push(function.to_owned());
        } else if let Some(level) = argument.strip_prefix("-fstrict-flex-arrays=") {
            parsed.saw(Opt::StrictFlexArrays, argument);
            parsed.strict_flex_arrays = Some(parse_value(level.to_owned(), argument, "level")?);
        } else if let Some(value) = option_value(argument, "D") {
            parsed.saw(Opt::Define, argument);
            let define = next_value(arguments, &mut index, argument, value)?;
            parsed.defines.push(define.clone());
            parsed
                .preprocessor_inputs
                .macros
                .push(MacroOption::Define(define));
        } else if let Some(value) = option_value(argument, "U") {
            parsed.saw(Opt::Undef, argument);
            parsed
                .preprocessor_inputs
                .macros
                .push(MacroOption::Undef(next_value(
                    arguments, &mut index, argument, value,
                )?));
        } else if let Some(value) = option_value(argument, "include") {
            parsed.saw(Opt::ForceInclude, argument);
            parsed
                .preprocessor_inputs
                .includes
                .push(PathBuf::from(next_value(
                    arguments, &mut index, argument, value,
                )?));
        } else if let Some(value) = option_value(argument, "imacros") {
            parsed.saw(Opt::Imacros, argument);
            parsed
                .preprocessor_inputs
                .imacros
                .push(PathBuf::from(next_value(
                    arguments, &mut index, argument, value,
                )?));
        } else if let Some(value) = option_value(argument, "std") {
            parsed.saw(Opt::Standard, argument);
            parsed.standard = Some(parse_value(
                next_value(arguments, &mut index, argument, value)?,
                argument,
                "language standard",
            )?);
        } else if let Some(value) = option_value(argument, "I") {
            parsed.saw(Opt::Include, argument);
            parsed
                .include
                .push(next_value(arguments, &mut index, argument, value)?);
        } else if let Some(value) = option_value(argument, "iquote") {
            parsed.saw(Opt::Iquote, argument);
            parsed
                .iquote
                .push(next_value(arguments, &mut index, argument, value)?);
        } else if let Some(value) = option_value(argument, "isystem") {
            parsed.saw(Opt::Isystem, argument);
            parsed
                .isystem
                .push(next_value(arguments, &mut index, argument, value)?);
        } else if let Some(value) = option_value(argument, "idirafter") {
            parsed.saw(Opt::Idirafter, argument);
            parsed
                .idirafter
                .push(next_value(arguments, &mut index, argument, value)?);
        } else if let Some(value) = option_value(argument, "isysroot") {
            parsed.saw(Opt::Isysroot, argument);
            parsed.isysroot = Some(next_value(arguments, &mut index, argument, value)?);
        } else if let Some(value) = option_value(argument, "sysroot") {
            parsed.saw(Opt::Sysroot, argument);
            parsed.sysroot = Some(next_value(arguments, &mut index, argument, value)?);
        } else if matches!(argument.as_str(), "-nostdlibinc" | "--nostdlibinc") {
            parsed.saw(Opt::Nostdlibinc, argument);
            parsed.nostdlibinc = true;
        } else if argument == "-nostdinc" {
            parsed.saw(Opt::Nostdinc, argument);
            parsed.nostdinc = true;
        } else if let Some(value) = option_value(argument, "flavor") {
            parsed.saw(Opt::Flavor, argument);
            parsed.flavor = parse_value(
                next_value(arguments, &mut index, argument, value)?,
                argument,
                "compiler flavor",
            )?;
        } else if let Some(value) = option_value(argument, "target") {
            parsed.saw(Opt::Target, argument);
            parsed.target = next_value(arguments, &mut index, argument, value)?;
        } else if let Some(value) = option_value(argument, "mpreferred-stack-boundary") {
            parsed.saw(Opt::PreferredStackBoundary, argument);
            parsed.preferred_stack_boundary = Some(parse_value(
                next_value(arguments, &mut index, argument, value)?,
                argument,
                "integer exponent",
            )?);
        } else if let Some(value) = option_value(argument, "mstack-alignment") {
            parsed.saw(Opt::StackAlignment, argument);
            parsed.stack_alignment = Some(parse_value(
                next_value(arguments, &mut index, argument, value)?,
                argument,
                "byte alignment",
            )?);
        } else if let Some(value) = option_value(argument, "long-double") {
            parsed.saw(Opt::LongDouble, argument);
            parsed.long_double = Some(parse_value(
                next_value(arguments, &mut index, argument, value)?,
                argument,
                "long double format",
            )?);
        } else if let Some(value) = option_value(argument, "masm") {
            parsed.saw(Opt::AsmDialect, argument);
            parsed.asm_dialect = Some(parse_value(
                next_value(arguments, &mut index, argument, value)?,
                argument,
                "asm dialect",
            )?);
        } else if let Some(value) = option_value(argument, "march") {
            parsed.saw(Opt::Arch, argument);
            parsed.isa.march = Some(parse_value(
                next_value(arguments, &mut index, argument, value)?,
                argument,
                "architecture",
            )?);
        } else if let Some(value) = option_value(argument, "mfloat-abi") {
            parsed.saw(Opt::FloatAbi, argument);
            parsed.isa.float_abi = Some(parse_value(
                next_value(arguments, &mut index, argument, value)?,
                argument,
                "float ABI",
            )?);
        } else if let Some(value) = option_value(argument, "mfpu") {
            parsed.saw(Opt::Fpu, argument);
            parsed.isa.fpu = Some(parse_value(
                next_value(arguments, &mut index, argument, value)?,
                argument,
                "FPU",
            )?);
        } else if let Some(value) = option_value(argument, "msve-vector-bits") {
            parsed.saw(Opt::SveVectorBits, argument);
            parsed.isa.sve_vector_bits = Some(parse_value(
                next_value(arguments, &mut index, argument, value)?,
                argument,
                "SVE vector length",
            )?);
        } else if clang_codegen_only(argument) {
            parsed.saw(Opt::ClangCodegenOnly, argument);
        } else if let Some(thumb) = thumb_flag(argument) {
            parsed.saw(Opt::Thumb, argument);
            parsed.isa.thumb = Some(thumb);
        } else if let Some((feature, enabled)) = X86Feature::parse_flag(argument) {
            parsed.saw(Opt::IsaFeature, argument);
            parsed.isa.x86.set(feature, enabled);
        } else if parse_pedantic(argument, &mut parsed.diagnostics) {
            parsed.saw(Opt::Pedantic, argument);
        } else if let Some(name) = argument.strip_prefix("-W") {
            parsed.saw(Opt::Warning, argument);
            apply_warning_flag(name, &mut parsed.diagnostics);
        } else if let Some(level) = argument.strip_prefix("-O") {
            parsed.saw(Opt::Optimize, argument);
            parsed.optimization = Some(match level {
                "0" => Optimization::None,
                "" | "1" | "2" | "3" | "g" => Optimization::Speed,
                "s" | "z" => Optimization::Size,
                "fast" => return Err(invalid(argument, "fast-math is not emulated")),
                _ => return Err(invalid(argument, "unknown optimization level")),
            });
        } else if let Some(format) = long_double_flag(argument) {
            parsed.saw(Opt::LongDouble, argument);
            parsed.long_double = Some(format);
        } else {
            return Err(invalid(argument, "unknown option"));
        }
        index += 1;
    }
    parsed.signed_overflow = signed_overflow(parsed.flavor, arguments);
    let optimization_macros = match parsed.optimization {
        None | Some(Optimization::None) => Vec::new(),
        Some(Optimization::Speed) => vec!["__OPTIMIZE__"],
        Some(Optimization::Size) => vec!["__OPTIMIZE__", "__OPTIMIZE_SIZE__"],
    };
    if !optimization_macros.is_empty() {
        parsed.preprocessor_inputs.macros.splice(
            0..0,
            std::iter::once(MacroOption::Undef("__NO_INLINE__".into())).chain(
                optimization_macros
                    .into_iter()
                    .map(|name| MacroOption::Define(name.into())),
            ),
        );
    }
    Ok(parsed)
}

fn signed_overflow(flavor: CompilerFlavor, arguments: &[String]) -> Overflow {
    let wrap = match (
        last_flag(arguments, Opt::Wrapv),
        last_flag(arguments, Opt::StrictOverflow),
    ) {
        (Some(wrap), Some(strict)) if strict.0 > wrap.0 => Some((strict.0, !strict.1)),
        (None, Some(strict)) => Some((strict.0, !strict.1)),
        (wrap, _) => wrap,
    };
    let trap = last_flag(arguments, Opt::Trapv);
    match (
        wrap.filter(|(_, value)| *value),
        trap.filter(|(_, value)| *value),
    ) {
        (_, Some(_)) if flavor.is_clang() => Overflow::Trap,
        (Some(wrap), Some(trap)) if trap.0 > wrap.0 => Overflow::Trap,
        (Some(_), _) => Overflow::Wrap,
        (_, Some(_)) => Overflow::Trap,
        _ => Overflow::Undefined,
    }
}

fn codegen_options(raw: &ParsedCompilerArgs, target: &TargetInfo) -> CodegenOptions {
    let snapshot = target.profile.predefines(raw.flavor);
    let snapshot_has = |name| snapshot.is_some_and(|predefines| predefines.value(name).is_some());
    let default_protector = [
        ("__SSP_ALL__", StackProtector::All),
        ("__SSP_STRONG__", StackProtector::Strong),
        ("__SSP__", StackProtector::On),
    ]
    .into_iter()
    .find_map(|(name, level)| snapshot_has(name).then_some(level))
    .unwrap_or(StackProtector::Off);
    let pic_fixed = target.os == TargetOs::Darwin || target.environment == TargetEnvironment::Msvc;
    let cf_protection = raw.cf_protection.iter().fold(None, |bits, &level| {
        Some(match (raw.flavor, level) {
            (CompilerFlavor::Gcc, 0) => bits.unwrap_or(0) & 8,
            (CompilerFlavor::Gcc, level) => bits.unwrap_or(0) | level,
            (_, level) => level,
        })
    });
    CodegenOptions {
        pic: raw.pic.filter(|_| !pic_fixed),
        stack_protector: raw.stack_protector.map(|level| match level {
            StackProtector::On if raw.flavor.is_clang() => level.max(default_protector),
            level => level,
        }),
        cf_protection,
        three_dnow: raw.three_dnow.unwrap_or(0),
    }
}

fn last_flag(arguments: &[String], opt: Opt) -> Option<(usize, bool)> {
    arguments
        .iter()
        .enumerate()
        .rev()
        .find_map(|(index, argument)| opt.parse_switch(argument).map(|value| (index, value)))
}

fn option_value<'a>(argument: &'a str, name: &str) -> Option<&'a str> {
    let short = format!("-{name}");
    let long = format!("--{name}");
    [short, long].iter().find_map(|spelling| {
        argument.strip_prefix(spelling).and_then(|rest| {
            (rest.is_empty()
                || matches!(
                    name,
                    "I" | "iquote"
                        | "isystem"
                        | "idirafter"
                        | "isysroot"
                        | "D"
                        | "U"
                        | "include"
                        | "imacros"
                ))
            .then_some(rest)
            .or_else(|| rest.strip_prefix('='))
        })
    })
}

fn next_value(
    arguments: &[String],
    index: &mut usize,
    argument: &str,
    attached: &str,
) -> Result<String, CompilerArgError> {
    if !attached.is_empty() {
        return Ok(attached.into());
    }
    *index += 1;
    arguments
        .get(*index)
        .filter(|value| !value.starts_with('-'))
        .cloned()
        .ok_or_else(|| invalid(argument, "missing value"))
}

fn parse_value<T>(value: String, argument: &str, description: &str) -> Result<T, CompilerArgError>
where
    T: FromStr,
    T::Err: std::fmt::Display,
{
    value
        .parse()
        .map_err(|error| invalid(argument, &format!("invalid {description}: {error}")))
}

fn thumb_flag(argument: &str) -> Option<bool> {
    match argument {
        "-mthumb" | "--mthumb" => Some(true),
        "-marm" | "--marm" => Some(false),
        _ => None,
    }
}

fn long_double_flag(argument: &str) -> Option<LongDoubleFormat> {
    match argument {
        "-mlong-double-64" | "--mlong-double-64" => Some(LongDoubleFormat::Binary64),
        "-mlong-double-80" | "--mlong-double-80" => Some(LongDoubleFormat::X87),
        "-mlong-double-128" | "--mlong-double-128" => Some(LongDoubleFormat::Binary128),
        _ => None,
    }
}

fn invalid(argument: &str, reason: &str) -> CompilerArgError {
    CompilerArgError::Invalid {
        argument: argument.into(),
        reason: reason.into(),
    }
}

fn validate_rules<'a>(target: &'a TargetInfo) -> Rule<'a, ParsedCompilerArgs> {
    Rules::pipeline([
        common_rules(),
        strict_flex_arrays_rule(),
        long_double_target_rule(target),
        isa_target_rule(target),
        flavor_rules(target),
    ])
}

fn isa_target_rule<'a>(target: &'a TargetInfo) -> Rule<'a, ParsedCompilerArgs> {
    Rule::validate("target ISA options", move |args: &ParsedCompilerArgs| {
        TargetIsa::resolve(target.family, target.environment, &args.isa, args.flavor)
            .map(drop)
            .map_err(|reason| format!("{reason} for {}", target.triple))
    })
}

fn long_double_target_rule<'a>(target: &'a TargetInfo) -> Rule<'a, ParsedCompilerArgs> {
    Rule::validate(
        "target long double options",
        move |args: &ParsedCompilerArgs| {
            if args.long_double.is_some()
                && matches!(target.family, TargetFamily::AArch64 | TargetFamily::Arm32)
            {
                Err(format!(
                    "long double format options are unsupported for {}",
                    target.triple
                ))
            } else {
                Ok(())
            }
        },
    )
}

fn common_rules<'a>() -> Rule<'a, ParsedCompilerArgs> {
    Rule::validate("stack alignment options", |args: &ParsedCompilerArgs| {
        if args.preferred_stack_boundary.is_some() && args.stack_alignment.is_some() {
            Err("preferred stack boundary and stack alignment are mutually exclusive".into())
        } else {
            Ok(())
        }
    })
}

fn strict_flex_arrays_rule<'a>() -> Rule<'a, ParsedCompilerArgs> {
    Rule::validate(
        "strict flex arrays level",
        |args: &ParsedCompilerArgs| match args.strict_flex_arrays {
            Some(level) if level > 3 => Err(format!("expected a level in 0..=3, found {level}")),
            _ => Ok(()),
        },
    )
}

fn flavor_rules<'a>(target: &'a TargetInfo) -> Rule<'a, ParsedCompilerArgs> {
    Rules::branch(
        |args: &ParsedCompilerArgs| args.flavor,
        [
            (CompilerFlavor::Gcc, gcc_rules(target)),
            (CompilerFlavor::Clang, clang_rules()),
            (CompilerFlavor::Msvc, Rules::pipeline([])),
        ],
    )
}

fn gcc_rules<'a>(target: &'a TargetInfo) -> Rule<'a, ParsedCompilerArgs> {
    Rules::pipeline([
        Rule::validate("GCC asm dialect", move |args: &ParsedCompilerArgs| {
            if args.asm_dialect.is_some() && !target.family.is_x86() {
                Err(format!("asm dialect is unsupported for {}", target.triple))
            } else {
                Ok(())
            }
        }),
        Rule::validate(
            "GCC register parameters",
            |args: &ParsedCompilerArgs| match args.regparm {
                Some(count) if count > 3 => {
                    Err(format!("expected a count in 0..=3, found {count}"))
                }
                _ => Ok(()),
            },
        ),
        Rule::validate("GCC MS modes", |args: &ParsedCompilerArgs| {
            match [Opt::MsExtensions, Opt::MsCompatibility]
                .into_iter()
                .find(|opt| args.has(*opt))
            {
                Some(opt) => Err(format!("`{opt}` is not emulated for GCC")),
                None => Ok(()),
            }
        }),
        Rules::when(
            |args: &ParsedCompilerArgs| args.preferred_stack_boundary.is_some(),
            Rule::validate(
                "GCC preferred stack boundary",
                move |args: &ParsedCompilerArgs| {
                    let value = args.preferred_stack_boundary.unwrap_or_default();
                    if !target.family.is_x86() {
                        return Err(format!(
                            "preferred stack boundary is unsupported for {}",
                            target.triple
                        ));
                    }
                    let minimum = if target.family == TargetFamily::X86_64 {
                        4
                    } else {
                        2
                    };
                    if (minimum..=12).contains(&value) {
                        Ok(())
                    } else {
                        Err(format!(
                            "expected an exponent in {minimum}..=12 for target {}",
                            target.triple
                        ))
                    }
                },
            ),
        ),
    ])
}

fn clang_rules<'a>() -> Rule<'a, ParsedCompilerArgs> {
    Rules::pipeline([
        Rules::when(
            |args: &ParsedCompilerArgs| args.stack_alignment.is_some(),
            Rule::validate("Clang stack alignment", |args: &ParsedCompilerArgs| {
                let value = args.stack_alignment.unwrap_or_default();
                if value.is_power_of_two() {
                    Ok(())
                } else {
                    Err(format!("expected a power of two, found {value}"))
                }
            }),
        ),
        Rule::validate(
            "Clang control-flow protection",
            |args: &ParsedCompilerArgs| {
                if args.cf_protection.contains(&8) {
                    Err("`check` is a gcc-only level".into())
                } else {
                    Ok(())
                }
            },
        ),
    ])
}
