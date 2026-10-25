use crate::charset::Charset;
use crate::compiler_options::{
    CodegenOptions, CompilerOptions, LayoutOptions, LibraryBuiltins, MathOptions, MathSwitch,
    MicrosoftFlags, MsvcArch, MsvcArchLevel, MsvcArm64Arch, MsvcFloatingModel, MsvcFloatingPoint,
    OperationValues, Optimization, Pic, SourceGraphs, StackProtector, TrivialAutoVarInit,
};
use crate::diagnostics::{DiagnosticOptions, Warning};
use crate::files::SearchPaths;
use crate::ir::{AsmDialect, Contraction, Exceptions, Overflow, Rounding};
use crate::pp::{MacroOption, PreprocessorInputs};
use crate::rules::{Rule, Rules};
use crate::target::aarch64_isa::{AArch64Feature, ArmMarch};
use crate::target::isa::{IsaRequest, March, TargetIsa};
use crate::target::x86_isa::{X86Feature, X86IsaRequest};
use crate::target_info::{LongDoubleFormat, TargetEnvironment, TargetFamily, TargetInfo, TargetOs};
use crate::target_registry::{ArchMode, arch_variant};
use crate::{compiler_headers, native_headers, sysroot};
use std::path::{Path, PathBuf};
use std::str::FromStr;

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq, Hash)]
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
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
    Gnu23,
    C2y,
    Gnu2y,
}

impl LanguageStandard {
    pub const fn default_for(flavor: CompilerFlavor) -> Self {
        match flavor {
            CompilerFlavor::Gcc => Self::Gnu23,
            CompilerFlavor::Clang => Self::Gnu17,
            CompilerFlavor::Msvc => Self::C17,
        }
    }

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

#[derive(Debug, PartialEq, Eq)]
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
    pub compiler: Option<String>,
    pub target: TargetInfo,
}

impl CompilerArgs {
    pub fn search_paths(&self) -> Result<SearchPaths, CompilerArgError> {
        let explicit_root = self.isysroot.as_ref().or(self.sysroot.as_ref());
        let root = explicit_root.map_or_else(|| sysroot::path(&self.target.triple), PathBuf::from);
        let include_dir = |path: &str| match path.strip_prefix('=') {
            Some(relative) => root.join(relative.trim_start_matches('/')),
            None => PathBuf::from(path),
        };
        let mut system: Vec<PathBuf> = self.isystem.iter().map(|path| include_dir(path)).collect();
        if !self.nostdinc {
            system.extend(self.standard_system_paths(explicit_root.is_some(), &root)?);
        }
        system.extend(self.idirafter.iter().map(|path| include_dir(path)));
        Ok(SearchPaths {
            quote: self.iquote.iter().map(|path| include_dir(path)).collect(),
            user: self.include.iter().map(|path| include_dir(path)).collect(),
            system,
        })
    }

    fn standard_system_paths(
        &self,
        explicit_root: bool,
        root: &Path,
    ) -> Result<Vec<PathBuf>, CompilerArgError> {
        let installed = || {
            let mut paths = compiler_headers::include_paths(&self.target, self.flavor);
            if !self.nostdlibinc {
                paths.extend(sysroot::include_paths_at(root, &self.target, self.flavor));
            }
            paths
        };
        if explicit_root || sysroot::pinned() || !native_headers::is_host(&self.target) {
            return Ok(installed());
        }
        let explicit = self.compiler.as_deref().and_then(|compiler| {
            native_headers::discover(&self.target, self.flavor, Some(compiler)).ok()
        });
        match explicit.map_or_else(
            || native_headers::discover(&self.target, self.flavor, None),
            Ok,
        ) {
            Ok(native) => Ok(native.system_paths(!self.nostdlibinc)),
            Err(_) if root.is_dir() => Ok(installed()),
            Err(error) => Err(CompilerArgError::SystemHeaders {
                triple: self.target.triple.clone(),
                reason: Box::new(error),
            }),
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
    #[error(
        "no system headers for {triple}: {reason}; install a toolchain or run `slate sysroot install {triple}`"
    )]
    SystemHeaders {
        triple: String,
        reason: Box<crate::native_headers::NativeHeaderError>,
    },
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
    compiler: Option<String>,
    target: String,
    arch_mode: Option<ArchMode>,
    regparm: Option<u8>,
    hosted: Option<bool>,
    builtin: Option<bool>,
    no_builtin: Vec<String>,
    char_signed: Option<bool>,
    c11_atomics: bool,
    msvc_warning_level: Option<u32>,
    msvc_convention: Option<(crate::ir::CallConv, String)>,
    msvc_source_charset: Option<(Charset, String)>,
    msvc_execution_charset: Option<(Charset, String)>,
    msvc_utf8: bool,
    msvc_floating_point: MsvcFloatingPoint,
    msvc_arch: Vec<String>,
    msvc_features: Vec<String>,
    short_wchar: Option<bool>,
    short_enums: Option<bool>,
    pack_struct: Option<bool>,
    pack_struct_alignment: Option<u32>,
    ms_bitfields: Option<bool>,
    inline: Option<bool>,
    inline_functions: Option<bool>,
    exceptions: Option<bool>,
    ms_anonymous_structs: Option<bool>,
    strict_flex_arrays: Option<u8>,
    pthread: bool,
    openmp: Option<bool>,
    openmp_simd: Option<bool>,
    openmp_runtime: Option<String>,
    openmp_version: Option<String>,
    openmp_date: Option<u32>,
    delete_null_pointer_checks: Option<bool>,
    trivial_auto_var_init: Option<TrivialAutoVarInit>,
    macro_prefix_map: Vec<(String, String)>,
    fixed_registers: Vec<String>,
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
    math: Vec<(MathSwitch, bool)>,
    fast_optimization: Option<usize>,
    fp_contract: Option<(FpContract, usize)>,
    iso_flt_eval_method: bool,
    gnu89_inline: Option<bool>,
    common: Option<bool>,
    trigraphs: Option<bool>,
    digraphs: Option<bool>,
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

    fn default_max_field_alignment(&self) -> Option<u64> {
        let bare = (self.flavor.is_clang() && self.pack_struct == Some(true)).then_some(1);
        self.pack_struct_alignment
            .or(bare)
            .filter(|alignment| *alignment != 0)
            .map(u64::from)
    }

    fn set_fast_optimization(&mut self, fast: bool) {
        if let Some(position) = self.fast_optimization.take() {
            self.math.remove(position);
            if let Some((_, contract_position)) = &mut self.fp_contract
                && *contract_position > position
            {
                *contract_position -= 1;
            }
        }
        if fast {
            self.fast_optimization = Some(self.math.len());
            self.math.push((MathSwitch::FastMath, true));
        }
    }

    fn openmp_date(&self) -> Result<Option<u32>, CompilerArgError> {
        let enabled = self.openmp == Some(true);
        if self.flavor.is_gcc() {
            return Ok(enabled.then_some(202111));
        }
        if !self.flavor.is_clang() {
            return Ok(None);
        }
        let runtime = self.openmp_runtime.as_deref().unwrap_or("libomp");
        if enabled && !matches!(runtime, "libomp" | "libiomp5" | "libgomp") {
            return Err(invalid(
                &format!("-fopenmp={runtime}"),
                "unsupported OpenMP runtime; expected libomp, libiomp5 or libgomp",
            ));
        }
        let openmp = enabled && runtime != "libgomp";
        let simd = !enabled && self.openmp_simd == Some(true);
        let version = match &self.openmp_version {
            Some(value) if openmp || simd => Some(parse_clang_integer(value).ok_or_else(|| {
                invalid(
                    &format!("-fopenmp-version={value}"),
                    "invalid integral value",
                )
            })?),
            _ => None,
        };
        Ok(openmp.then_some(match version {
            Some(31) => 201107,
            Some(40) => 201307,
            Some(45) => 201511,
            Some(50) => 201811,
            Some(52) => 202111,
            Some(60) => 202411,
            _ => 202011,
        }))
    }

    fn no_inline_requested(&self) -> bool {
        let inline = if self.flavor.is_clang() {
            self.inline.or(self.inline_functions)
        } else {
            self.inline
        };
        inline == Some(false)
    }

    fn fp_contract(&self, math: &MathOptions) -> Option<Contraction> {
        let explicit = self.fp_contract.map(|(request, _)| request.contraction());
        if self.flavor.is_gcc() {
            return explicit.or(math.unsafe_math.then_some(Contraction::Fast));
        }
        let mut contract = None;
        let mut last_seen = None;
        for (index, (switch, value)) in self.math.iter().enumerate() {
            if let Some((_, position)) = self.fp_contract
                && position == index
            {
                (contract, last_seen) = (explicit, explicit);
            }
            match (switch, value) {
                (MathSwitch::FastMath | MathSwitch::UnsafeMath, true) => {
                    contract = Some(Contraction::Fast);
                }
                (MathSwitch::FastMath | MathSwitch::UnsafeMath, false) => contract = last_seen,
                _ => {}
            }
        }
        match self.fp_contract {
            Some((_, position)) if position == self.math.len() => explicit,
            _ => contract,
        }
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum FpContract {
    Off,
    On,
    Fast,
    FastHonorPragmas,
}

impl FpContract {
    fn parse(value: &str) -> Option<Self> {
        match value {
            "off" => Some(Self::Off),
            "on" => Some(Self::On),
            "fast" => Some(Self::Fast),
            "fast-honor-pragmas" => Some(Self::FastHonorPragmas),
            _ => None,
        }
    }

    fn contraction(self) -> Contraction {
        match self {
            Self::Off => Contraction::Off,
            Self::On => Contraction::On,
            Self::Fast | Self::FastHonorPragmas => Contraction::Fast,
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
    Ansi,
    Include,
    Iquote,
    Isystem,
    Idirafter,
    Isysroot,
    Sysroot,
    Nostdlibinc,
    Nostdinc,
    Flavor,
    Compiler,
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
    ShortEnums,
    PackStruct,
    PackStructAlignment,
    MsBitfields,
    FpContract,
    Inline,
    InlineFunctions,
    InlineHintFunctions,
    Exceptions,
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
    FastMath,
    UnsafeMath,
    AssociativeMath,
    ReciprocalMath,
    SignedZeros,
    MathErrno,
    FiniteMathOnly,
    SignalingNans,
    CxLimitedRange,
    ExcessPrecision,
    PermittedFltEvalMethods,
    Gnu89Inline,
    Common,
    Trigraphs,
    ClangTrigraphs,
    ClangDigraphs,
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
    CodegenOnly(&'static str),
    Warning,
    Pedantic,
    Optimize,
    Pthread,
    OpenMp,
    OpenMpSimd,
    OpenMpRuntime,
    OpenMpVersion,
    NoWarnings,
    ConformingPreprocessor,
    MsvcUnsignedChar,
    MsvcPack,
    MsvcDefine,
    MsvcUndef,
    MsvcInclude,
    MsvcForceInclude,
    MsvcNoStandardIncludes,
    MsvcStandard,
    MsvcC11Atomics,
    MsvcWarningLevel,
    MsvcNoWarnings,
    MsvcCallingConvention,
    MsvcUtf8,
    MsvcCharset,
    MsvcFloatingPoint,
    MsvcArch,
    MsvcFeature,
    Permissive,
    DeleteNullPointerChecks,
    TrivialAutoVarInit,
    MacroPrefixMap,
    FixedRegister,
    Mllvm,
}

impl std::fmt::Display for Opt {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let name = match self {
            Self::Define => "define",
            Self::Undef => "undef",
            Self::ForceInclude => "include",
            Self::Imacros => "imacros",
            Self::Standard => "std",
            Self::Ansi => "ansi",
            Self::Include => "I",
            Self::Iquote => "iquote",
            Self::Isystem => "isystem",
            Self::Idirafter => "idirafter",
            Self::Isysroot => "isysroot",
            Self::Sysroot => "sysroot",
            Self::Nostdlibinc => "nostdlibinc",
            Self::Nostdinc => "nostdinc",
            Self::Flavor => "flavor",
            Self::Compiler => "compiler",
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
            Self::ShortEnums => "fshort-enums",
            Self::PackStruct => "fpack-struct",
            Self::PackStructAlignment => "fpack-struct=",
            Self::MsBitfields => "mms-bitfields",
            Self::FpContract => "ffp-contract=",
            Self::Inline => "finline",
            Self::InlineFunctions => "finline-functions",
            Self::InlineHintFunctions => "finline-hint-functions",
            Self::Exceptions => "fexceptions",
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
            Self::FastMath => "fast-math",
            Self::UnsafeMath => "unsafe-math-optimizations",
            Self::AssociativeMath => "associative-math",
            Self::ReciprocalMath => "reciprocal-math",
            Self::SignedZeros => "signed-zeros",
            Self::MathErrno => "math-errno",
            Self::FiniteMathOnly => "finite-math-only",
            Self::SignalingNans => "signaling-nans",
            Self::CxLimitedRange => "cx-limited-range",
            Self::ExcessPrecision => "fexcess-precision",
            Self::PermittedFltEvalMethods => "fpermitted-flt-eval-methods",
            Self::Gnu89Inline => "gnu89-inline",
            Self::Common => "common",
            Self::Trigraphs => "trigraphs",
            Self::ClangTrigraphs => "ftrigraphs",
            Self::ClangDigraphs => "fdigraphs",
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
            Self::CodegenOnly(spelling) => spelling.trim_start_matches(['-', '/']),
            Self::Warning => "W",
            Self::Pedantic => "pedantic",
            Self::Optimize => "O",
            Self::Pthread => "pthread",
            Self::OpenMp => "fopenmp",
            Self::OpenMpSimd => "fopenmp-simd",
            Self::OpenMpRuntime => "fopenmp=",
            Self::OpenMpVersion => "fopenmp-version=",
            Self::NoWarnings => "w",
            Self::ConformingPreprocessor => "Zc:preprocessor",
            Self::MsvcUnsignedChar => "J",
            Self::MsvcPack => "Zp",
            Self::MsvcDefine => "D",
            Self::MsvcUndef => "U",
            Self::MsvcInclude => "I",
            Self::MsvcForceInclude => "FI",
            Self::MsvcNoStandardIncludes => "X",
            Self::MsvcStandard => "std:",
            Self::MsvcC11Atomics => "experimental:c11atomics",
            Self::MsvcWarningLevel => "W",
            Self::MsvcNoWarnings => "w",
            Self::MsvcCallingConvention => "Gd",
            Self::MsvcUtf8 => "utf-8",
            Self::MsvcCharset => "source-charset",
            Self::MsvcFloatingPoint => "fp",
            Self::MsvcArch => "arch",
            Self::MsvcFeature => "feature",
            Self::Permissive => "fpermissive",
            Self::DeleteNullPointerChecks => "fdelete-null-pointer-checks",
            Self::TrivialAutoVarInit => "ftrivial-auto-var-init",
            Self::MacroPrefixMap => "fmacro-prefix-map",
            Self::FixedRegister => "ffixed-<reg>",
            Self::Mllvm => "mllvm",
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
            | Self::Compiler
            | Self::Target
            | Self::Warning
            | Self::Pedantic
            | Self::NoWarnings => &[Gcc, Clang, Msvc],
            Self::Wrapv
            | Self::Trapv
            | Self::StrictOverflow
            | Self::RoundingMath
            | Self::TrappingMath
            | Self::FastMath
            | Self::UnsafeMath
            | Self::AssociativeMath
            | Self::ReciprocalMath
            | Self::SignedZeros
            | Self::MathErrno
            | Self::FiniteMathOnly
            | Self::SignalingNans
            | Self::CxLimitedRange
            | Self::ExcessPrecision
            | Self::Gnu89Inline
            | Self::Common
            | Self::Ansi
            | Self::Trigraphs
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
            | Self::ShortEnums
            | Self::PackStruct
            | Self::PackStructAlignment
            | Self::MsBitfields
            | Self::FpContract
            | Self::Inline
            | Self::InlineFunctions
            | Self::Exceptions
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
            | Self::ThreeDNowA
            | Self::Pthread
            | Self::OpenMp
            | Self::OpenMpSimd
            | Self::DeleteNullPointerChecks
            | Self::TrivialAutoVarInit
            | Self::MacroPrefixMap
            | Self::FixedRegister => &[Gcc, Clang],
            Self::PreferredStackBoundary
            | Self::GccStrictFlexArrays
            | Self::PermittedFltEvalMethods
            | Self::StackProtectorExplicit
            | Self::FpRetIn387
            | Self::Permissive => &[Gcc],
            Self::StackAlignment
            | Self::ClangX87
            | Self::AsmBlocks
            | Self::ClangTrigraphs
            | Self::ClangDigraphs
            | Self::MsAnonymousStructs
            | Self::LateParseAttributes
            | Self::InlineHintFunctions
            | Self::OpenMpRuntime
            | Self::OpenMpVersion
            | Self::Mllvm => &[Clang],
            Self::ConformingPreprocessor
            | Self::MsvcUnsignedChar
            | Self::MsvcPack
            | Self::MsvcDefine
            | Self::MsvcUndef
            | Self::MsvcInclude
            | Self::MsvcForceInclude
            | Self::MsvcNoStandardIncludes
            | Self::MsvcStandard
            | Self::MsvcC11Atomics
            | Self::MsvcWarningLevel
            | Self::MsvcNoWarnings
            | Self::MsvcCallingConvention
            | Self::MsvcUtf8
            | Self::MsvcCharset
            | Self::MsvcFloatingPoint
            | Self::MsvcArch
            | Self::MsvcFeature => &[Msvc],
            Self::CodegenOnly(spelling) => {
                codegen_only_flag_named(spelling).map_or(&[], |flag| flag.flavors)
            }
        };
        flavors.contains(&flavor)
    }

    fn negation_accepted_by(self, flavor: CompilerFlavor) -> bool {
        match self {
            Self::Freestanding | Self::Hosted => flavor.is_gcc(),
            Self::FpRetIn387 => !flavor.is_msvc(),
            Self::InlineHintFunctions => false,
            Self::CodegenOnly(spelling) => codegen_only_flag_named(spelling)
                .is_some_and(|flag| flag.negation_flavors.contains(&flavor)),
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
            Self::FastMath => "-ffast-math",
            Self::UnsafeMath => "-funsafe-math-optimizations",
            Self::AssociativeMath => "-fassociative-math",
            Self::ReciprocalMath => "-freciprocal-math",
            Self::SignedZeros => "-fsigned-zeros",
            Self::MathErrno => "-fmath-errno",
            Self::FiniteMathOnly => "-ffinite-math-only",
            Self::SignalingNans => "-fsignaling-nans",
            Self::CxLimitedRange => "-fcx-limited-range",
            Self::Gnu89Inline => "-fgnu89-inline",
            Self::Common => "-fcommon",
            Self::ClangTrigraphs => "-ftrigraphs",
            Self::ClangDigraphs => "-fdigraphs",
            Self::MsExtensions => "-fms-extensions",
            Self::MsCompatibility => "-fms-compatibility",
            Self::AsmBlocks => "-fasm-blocks",
            Self::Freestanding => "-ffreestanding",
            Self::Hosted => "-fhosted",
            Self::Builtin => "-fbuiltin",
            Self::UnsignedChar => "-funsigned-char",
            Self::SignedChar => "-fsigned-char",
            Self::ShortWchar => "-fshort-wchar",
            Self::ShortEnums => "-fshort-enums",
            Self::PackStruct => "-fpack-struct",
            Self::MsBitfields => "-mms-bitfields",
            Self::Inline => "-finline",
            Self::InlineFunctions => "-finline-functions",
            Self::InlineHintFunctions => "-finline-hint-functions",
            Self::Exceptions => "-fexceptions",
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
            Self::DeleteNullPointerChecks => "-fdelete-null-pointer-checks",
            Self::OpenMp => "-fopenmp",
            Self::OpenMpSimd => "-fopenmp-simd",
            Self::CodegenOnly(spelling)
                if codegen_only_flag_named(spelling)
                    .is_some_and(|flag| flag.form == CodegenOnlyForm::Switch) =>
            {
                spelling
            }
            _ => return None,
        })
    }

    fn parse_switch(self, argument: &str) -> Option<bool> {
        parse_switch(self.switch()?, argument)
    }

    fn math_switch(self) -> Option<MathSwitch> {
        Some(match self {
            Self::FastMath => MathSwitch::FastMath,
            Self::UnsafeMath => MathSwitch::UnsafeMath,
            Self::AssociativeMath => MathSwitch::AssociativeMath,
            Self::ReciprocalMath => MathSwitch::ReciprocalMath,
            Self::SignedZeros => MathSwitch::SignedZeros,
            Self::TrappingMath => MathSwitch::TrappingMath,
            Self::MathErrno => MathSwitch::MathErrno,
            Self::FiniteMathOnly => MathSwitch::FiniteMathOnly,
            Self::RoundingMath => MathSwitch::RoundingMath,
            Self::SignalingNans => MathSwitch::SignalingNans,
            Self::CxLimitedRange => MathSwitch::CxLimitedRange,
            _ => return None,
        })
    }
}

pub(crate) fn math_switch_argument(argument: &str) -> Option<(MathSwitch, bool)> {
    SWITCH_OPTS.iter().find_map(|opt| {
        let switch = opt.math_switch()?;
        opt.parse_switch(argument).map(|value| (switch, value))
    })
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

const IGNORED_DRIVER_FLAGS: [&str; 9] = [
    "-c", "-S", "-MD", "-MMD", "-MP", "-MG", "-M", "-MM", "-pipe",
];

const IGNORED_VALUE_OPTIONS: [&str; 5] = ["-o", "-MF", "-MT", "-MQ", "-MJ"];

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum CodegenOnlyForm {
    Switch,
    Value(&'static [&'static str]),
    Exact,
    Toggle,
    Number,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
struct CodegenOnlyFlag {
    spelling: &'static str,
    form: CodegenOnlyForm,
    flavors: &'static [CompilerFlavor],
    negation_flavors: &'static [CompilerFlavor],
    separate: bool,
}

impl CodegenOnlyFlag {
    const fn switch(spelling: &'static str, flavors: &'static [CompilerFlavor]) -> Self {
        Self {
            spelling,
            form: CodegenOnlyForm::Switch,
            flavors,
            negation_flavors: flavors,
            separate: false,
        }
    }

    const fn exact(spelling: &'static str, flavors: &'static [CompilerFlavor]) -> Self {
        Self {
            form: CodegenOnlyForm::Exact,
            ..Self::switch(spelling, flavors)
        }
    }

    const fn toggle(spelling: &'static str, flavors: &'static [CompilerFlavor]) -> Self {
        Self {
            form: CodegenOnlyForm::Toggle,
            ..Self::switch(spelling, flavors)
        }
    }

    const fn number(spelling: &'static str, flavors: &'static [CompilerFlavor]) -> Self {
        Self {
            form: CodegenOnlyForm::Number,
            ..Self::switch(spelling, flavors)
        }
    }

    const fn separate(self) -> Self {
        Self {
            separate: true,
            ..self
        }
    }

    const fn value(spelling: &'static str, flavors: &'static [CompilerFlavor]) -> Self {
        Self {
            form: CodegenOnlyForm::Value(&[]),
            ..Self::switch(spelling, flavors)
        }
    }

    const fn one_of(self, values: &'static [&'static str]) -> Self {
        Self {
            form: CodegenOnlyForm::Value(values),
            ..self
        }
    }

    const fn negated_by(self, flavors: &'static [CompilerFlavor]) -> Self {
        Self {
            negation_flavors: flavors,
            ..self
        }
    }

    fn matches(&self, argument: &str) -> bool {
        match self.form {
            CodegenOnlyForm::Switch => parse_switch(self.spelling, argument).is_some(),
            CodegenOnlyForm::Value(_) => argument.starts_with(self.spelling),
            CodegenOnlyForm::Exact => argument == self.spelling,
            CodegenOnlyForm::Toggle => {
                matches!(argument.strip_prefix(self.spelling), Some("" | "-"))
            }
            CodegenOnlyForm::Number => argument.strip_prefix(self.spelling).is_some_and(|rest| {
                !rest.is_empty() && rest.bytes().all(|byte| byte.is_ascii_digit())
            }),
        }
    }

    fn check_value(&self, argument: &str) -> Result<(), CompilerArgError> {
        match (self.form, argument.strip_prefix(self.spelling)) {
            (CodegenOnlyForm::Value(values), Some(value))
                if !values.is_empty() && !values.contains(&value) =>
            {
                Err(invalid(
                    argument,
                    &format!("expected one of {}", values.join(", ")),
                ))
            }
            _ => Ok(()),
        }
    }
}

const ALL_FLAVORS: &[CompilerFlavor] = &[
    CompilerFlavor::Gcc,
    CompilerFlavor::Clang,
    CompilerFlavor::Msvc,
];
const GNU_FLAVORS: &[CompilerFlavor] = &[CompilerFlavor::Gcc, CompilerFlavor::Clang];
const GCC_ONLY: &[CompilerFlavor] = &[CompilerFlavor::Gcc];
const CLANG_ONLY: &[CompilerFlavor] = &[CompilerFlavor::Clang];
const MSVC_ONLY: &[CompilerFlavor] = &[CompilerFlavor::Msvc];

const CODEGEN_ONLY_FLAGS: [CodegenOnlyFlag; 173] = [
    CodegenOnlyFlag::switch("-fomit-frame-pointer", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-flto", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-ffunction-sections", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-fdata-sections", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-fstrict-aliasing", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-fplt", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-fsemantic-interposition", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-funwind-tables", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-fstack-clash-protection", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-fmerge-all-constants", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-fident", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-faddrsig", ALL_FLAVORS),
    CodegenOnlyFlag::value("-flto=", ALL_FLAVORS),
    CodegenOnlyFlag::value("-fvisibility=", ALL_FLAVORS),
    CodegenOnlyFlag::value("-fdebug-prefix-map=", ALL_FLAVORS),
    CodegenOnlyFlag::switch("-fjump-tables", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-fsplit-stack", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-fstack-check", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-foptimize-sibling-calls", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-fverbose-asm", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-funroll-loops", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-ftree-vectorize", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-ftree-vrp", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-fprefetch-loop-arrays", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-ffat-lto-objects", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-falign-functions", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-falign-loops", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-falign-jumps", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-falign-labels", GNU_FLAVORS),
    CodegenOnlyFlag::value("-falign-functions=", GNU_FLAVORS),
    CodegenOnlyFlag::value("-falign-loops=", GNU_FLAVORS),
    CodegenOnlyFlag::value("-falign-jumps=", GNU_FLAVORS),
    CodegenOnlyFlag::value("-falign-labels=", GNU_FLAVORS),
    CodegenOnlyFlag::value("-fpatchable-function-entry=", GNU_FLAVORS),
    CodegenOnlyFlag::value("-ftls-model=", GNU_FLAVORS).one_of(&[
        "global-dynamic",
        "local-dynamic",
        "initial-exec",
        "local-exec",
    ]),
    CodegenOnlyFlag::value("-finput-charset=", GNU_FLAVORS).one_of(&["utf-8", "UTF-8"]),
    CodegenOnlyFlag::switch("-fdiagnostics-color", GNU_FLAVORS),
    CodegenOnlyFlag::value("-fdiagnostics-color=", GNU_FLAVORS)
        .one_of(&["always", "never", "auto"]),
    CodegenOnlyFlag::switch("-coverage", GNU_FLAVORS).negated_by(&[]),
    CodegenOnlyFlag::switch("-mred-zone", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-mskip-rax-setup", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-mindirect-branch-cs-prefix", GNU_FLAVORS).negated_by(GCC_ONLY),
    CodegenOnlyFlag::value("-mtune=", GNU_FLAVORS),
    CodegenOnlyFlag::value("-mfunction-return=", GNU_FLAVORS).one_of(&[
        "keep",
        "thunk",
        "thunk-inline",
        "thunk-extern",
    ]),
    CodegenOnlyFlag::value("-mstack-protector-guard=", GNU_FLAVORS).one_of(&["tls", "global"]),
    CodegenOnlyFlag::value("-mstack-protector-guard-reg=", GNU_FLAVORS).one_of(&["fs", "gs"]),
    CodegenOnlyFlag::value("-mstack-protector-guard-symbol=", GNU_FLAVORS),
    CodegenOnlyFlag::value("-mstack-protector-guard-offset=", GNU_FLAVORS),
    CodegenOnlyFlag::switch("-fira-hoist-pressure", GCC_ONLY),
    CodegenOnlyFlag::switch("-ftree-ch", GCC_ONLY),
    CodegenOnlyFlag::switch("-ftree-loop-distribute-patterns", GCC_ONLY),
    CodegenOnlyFlag::value("-fira-region=", GCC_ONLY).one_of(&["all", "mixed", "one"]),
    CodegenOnlyFlag::value("-freorder-blocks-algorithm=", GCC_ONLY).one_of(&["simple", "stc"]),
    CodegenOnlyFlag::switch("-moutline", CLANG_ONLY),
    CodegenOnlyFlag::switch("-mretpoline", CLANG_ONLY),
    CodegenOnlyFlag::switch("-fintegrated-as", CLANG_ONLY),
    CodegenOnlyFlag::switch("-fcolor-diagnostics", CLANG_ONLY),
    CodegenOnlyFlag::switch("-fdiagnostics-show-inlining-chain", CLANG_ONLY),
    CodegenOnlyFlag::switch("-Qunused-arguments", CLANG_ONLY).negated_by(&[]),
    CodegenOnlyFlag::value("--warning-suppression-mappings=", CLANG_ONLY),
    CodegenOnlyFlag::exact("/nologo", MSVC_ONLY),
    CodegenOnlyFlag::exact("/c", MSVC_ONLY),
    CodegenOnlyFlag::exact("/MT", MSVC_ONLY),
    CodegenOnlyFlag::value("/O", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/GS", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Gy", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Gw", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/GL", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/GR", MSVC_ONLY),
    CodegenOnlyFlag::exact("/GF", MSVC_ONLY),
    CodegenOnlyFlag::value("/Gs", MSVC_ONLY),
    CodegenOnlyFlag::exact("/Ge", MSVC_ONLY),
    CodegenOnlyFlag::exact("/Gh", MSVC_ONLY),
    CodegenOnlyFlag::exact("/GH", MSVC_ONLY),
    CodegenOnlyFlag::exact("/GT", MSVC_ONLY),
    CodegenOnlyFlag::exact("/Gu", MSVC_ONLY),
    CodegenOnlyFlag::exact("/GA", MSVC_ONLY),
    CodegenOnlyFlag::value("/Qspectre", MSVC_ONLY),
    CodegenOnlyFlag::value("/Qpar", MSVC_ONLY),
    CodegenOnlyFlag::value("/Qvec-report:", MSVC_ONLY),
    CodegenOnlyFlag::exact("/QIntel-jcc-erratum", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/guard:ehcont", MSVC_ONLY),
    CodegenOnlyFlag::exact("/homeparams", MSVC_ONLY),
    CodegenOnlyFlag::exact("/jumptablerdata", MSVC_ONLY),
    CodegenOnlyFlag::value("/vlen", MSVC_ONLY),
    CodegenOnlyFlag::value("/favor:", MSVC_ONLY).one_of(&["blend", "AMD64"]),
    CodegenOnlyFlag::exact("/bigobj", MSVC_ONLY),
    CodegenOnlyFlag::exact("/Z7", MSVC_ONLY),
    CodegenOnlyFlag::exact("/Zi", MSVC_ONLY),
    CodegenOnlyFlag::exact("/ZI", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zo", MSVC_ONLY),
    CodegenOnlyFlag::value("/ZH:", MSVC_ONLY).one_of(&["MD5", "SHA1", "SHA_256"]),
    CodegenOnlyFlag::toggle("/JMC", MSVC_ONLY),
    CodegenOnlyFlag::number("/F", MSVC_ONLY),
    CodegenOnlyFlag::value("/Fa", MSVC_ONLY),
    CodegenOnlyFlag::value("/FA", MSVC_ONLY),
    CodegenOnlyFlag::value("/Fd:", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/Fd", MSVC_ONLY),
    CodegenOnlyFlag::value("/Fe:", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/Fe", MSVC_ONLY),
    CodegenOnlyFlag::value("/Fi:", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/Fi", MSVC_ONLY),
    CodegenOnlyFlag::value("/Fm:", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/Fm", MSVC_ONLY),
    CodegenOnlyFlag::value("/Fo:", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/Fo", MSVC_ONLY),
    CodegenOnlyFlag::value("/Fp:", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/Fp", MSVC_ONLY),
    CodegenOnlyFlag::value("/FR:", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/FR", MSVC_ONLY),
    CodegenOnlyFlag::value("/Fr", MSVC_ONLY),
    CodegenOnlyFlag::exact("/FC", MSVC_ONLY),
    CodegenOnlyFlag::exact("/FS", MSVC_ONLY),
    CodegenOnlyFlag::exact("/MP", MSVC_ONLY),
    CodegenOnlyFlag::number("/MP", MSVC_ONLY),
    CodegenOnlyFlag::exact("/showIncludes", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/validate-charset", MSVC_ONLY),
    CodegenOnlyFlag::exact("/fastfail", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/volatileMetadata", MSVC_ONLY),
    CodegenOnlyFlag::value("/sourcelink:", MSVC_ONLY),
    CodegenOnlyFlag::value("/diagnostics:", MSVC_ONLY).one_of(&["caret", "column", "classic"]),
    CodegenOnlyFlag::value("/external:I", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/external:", MSVC_ONLY),
    CodegenOnlyFlag::value("/analyze:ruleset", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/analyze:log", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/analyze:plugin", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/analyze", MSVC_ONLY),
    CodegenOnlyFlag::value("/wd", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/we", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/wo", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/w1", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/w2", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/w3", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/w4", MSVC_ONLY).separate(),
    CodegenOnlyFlag::toggle("/WX", MSVC_ONLY),
    CodegenOnlyFlag::exact("/WL", MSVC_ONLY),
    CodegenOnlyFlag::value("/Wv:", MSVC_ONLY),
    CodegenOnlyFlag::value("/Y", MSVC_ONLY),
    CodegenOnlyFlag::exact("/TC", MSVC_ONLY),
    CodegenOnlyFlag::value("/Tc", MSVC_ONLY).separate(),
    CodegenOnlyFlag::value("/EH", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/permissive", MSVC_ONLY),
    CodegenOnlyFlag::exact("/presetPadding", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:wchar_t", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:forScope", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:inline", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:auto", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:rvalueCast", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:strictStrings", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:__cplusplus", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:externConstexpr", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:throwingNew", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:sizedDealloc", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:threadSafeInit", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:referenceBinding", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:ternary", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:hiddenFriend", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:lambda", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:noexceptTypes", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:twoPhase", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:alignedNew", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:implicitNoexcept", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:enumTypes", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:templateScope", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:checkGwOdr", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:nrvo", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:char8_t", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:gotoScope", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:zeroSizeArrayNew", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:static_assert", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:u8EscapeEncoding", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:externC", MSVC_ONLY),
    CodegenOnlyFlag::toggle("/Zc:tlsGuards", MSVC_ONLY),
];

fn codegen_only_flag(argument: &str) -> Option<&'static CodegenOnlyFlag> {
    CODEGEN_ONLY_FLAGS
        .iter()
        .find(|flag| flag.matches(argument))
}

fn codegen_only_flag_named(spelling: &str) -> Option<&'static CodegenOnlyFlag> {
    CODEGEN_ONLY_FLAGS
        .iter()
        .find(|flag| flag.spelling == spelling)
}

fn ignored_option(argument: &str) -> Option<IgnoredOption> {
    if IGNORED_VALUE_OPTIONS.contains(&argument) {
        return Some(IgnoredOption::TakesValue);
    }
    let joined_value = IGNORED_VALUE_OPTIONS
        .iter()
        .any(|option| argument.len() > option.len() && argument.starts_with(option));
    (IGNORED_DRIVER_FLAGS.contains(&argument) || argument.starts_with("-g") || joined_value)
        .then_some(IgnoredOption::Alone)
}

const SWITCH_OPTS: [Opt; 51] = [
    Opt::OpenMp,
    Opt::OpenMpSimd,
    Opt::ClangTrigraphs,
    Opt::ClangDigraphs,
    Opt::DeleteNullPointerChecks,
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
    Opt::ShortEnums,
    Opt::PackStruct,
    Opt::MsBitfields,
    Opt::Inline,
    Opt::InlineFunctions,
    Opt::InlineHintFunctions,
    Opt::Exceptions,
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
    Opt::FastMath,
    Opt::UnsafeMath,
    Opt::AssociativeMath,
    Opt::ReciprocalMath,
    Opt::SignedZeros,
    Opt::MathErrno,
    Opt::FiniteMathOnly,
    Opt::SignalingNans,
    Opt::CxLimitedRange,
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
        target.abi.default_convention = match (target.family, raw.msvc_convention.as_ref()) {
            (TargetFamily::X86, Some((convention, _))) => *convention,
            (TargetFamily::X86_64, Some((crate::ir::CallConv::X86Vectorcall, _))) => {
                crate::ir::CallConv::X86Vectorcall
            }
            _ => crate::ir::CallConv::C,
        };
        let (msvc_arch, msvc_apx, msvc_arm64_arch) = if target.family == TargetFamily::AArch64 {
            let arm64 = resolve_msvc_arm64_arch(&raw.msvc_arch, &raw.msvc_features)?;
            (None, false, arm64)
        } else {
            let arch = resolve_msvc_arch(target.family, &raw.msvc_arch)?;
            (
                arch,
                resolve_msvc_apx(target.family, &raw.msvc_features)?,
                None,
            )
        };
        let mut isa = raw.isa.clone();
        if let Some(arch) = msvc_arch {
            request_msvc_arch(arch, &mut isa.x86);
        }
        if let Some(arch) = msvc_arm64_arch {
            isa.march = Some(March::Arm(msvc_arm64_march(arch)));
        }
        target.isa = TargetIsa::resolve(target.family, target.environment, &isa, raw.flavor)
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
            short_enums: raw.short_enums.unwrap_or(false),
            default_max_field_alignment: raw.default_max_field_alignment(),
            pack_records: flavor.is_gcc() && raw.pack_struct == Some(true),
            ms_bitfields: raw.ms_bitfields.unwrap_or(false),
        };
        let mut math = MathOptions::resolve(flavor, &raw.math);
        math.contract = raw.fp_contract(&math);
        let mut options = CompilerOptions::from_values(
            flavor,
            layout,
            raw.diagnostics,
            OperationValues {
                signed_overflow: raw.signed_overflow,
                strict_overflow: raw.strict_overflow,
                math,
            },
        );
        options.iso_flt_eval_method = raw.iso_flt_eval_method;
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
        options.graphs = SourceGraphs {
            trigraphs: raw.trigraphs,
            digraphs: raw.digraphs,
        };
        options.microsoft = MicrosoftFlags {
            extensions: raw.ms_extensions,
            compatibility: raw.ms_compatibility,
            asm_blocks: raw.asm_blocks == Some(true),
            anonymous_structs: raw.ms_anonymous_structs,
            c11_atomics: raw.c11_atomics,
            warning_level: raw.msvc_warning_level,
            source_charset: raw
                .msvc_source_charset
                .map(|(charset, _)| charset)
                .or(raw.msvc_utf8.then_some(Charset::Utf8)),
            execution_charset: raw
                .msvc_execution_charset
                .map(|(charset, _)| charset)
                .or(raw.msvc_utf8.then_some(Charset::Utf8)),
            utf8: raw.msvc_utf8,
            floating_point: raw.msvc_floating_point,
            arch: msvc_arch,
            apx: msvc_apx,
            arm64_arch: msvc_arm64_arch,
        };
        let floating_point = raw.msvc_floating_point;
        if floating_point.model == Some(MsvcFloatingModel::Strict) {
            options.operations.floating.rounding = Rounding::Environment;
        }
        if floating_point.exceptions() {
            options.operations.floating.exceptions = Exceptions::Observable;
        }
        if floating_point.model == Some(MsvcFloatingModel::Fast) {
            options.math.contract = Some(Contraction::Fast);
        } else if floating_point.contract {
            options.math.contract = Some(Contraction::On);
        }
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
        options.openmp = raw.openmp_date.is_some();
        options.strict_flex_arrays = raw.strict_flex_arrays.unwrap_or(0);
        options.macro_prefix_map = raw.macro_prefix_map;
        options.fixed_registers = raw.fixed_registers;
        Ok(CompilerArgs {
            options,
            defines: raw.defines,
            preprocessor_inputs: raw.preprocessor_inputs,
            standard: raw
                .standard
                .unwrap_or(LanguageStandard::default_for(flavor)),
            include: raw.include,
            iquote: raw.iquote,
            isystem: raw.isystem,
            idirafter: raw.idirafter,
            isysroot: raw.isysroot,
            sysroot: raw.sysroot,
            nostdlibinc: raw.nostdlibinc,
            nostdinc: raw.nostdinc,
            flavor,
            compiler: raw.compiler,
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
        target: crate::target_registry::host()
            .map_or("x86_64-unknown-linux-gnu", |spec| spec.triple)
            .into(),
        ..ParsedCompilerArgs::default()
    };
    let mut index = 0;
    while index < arguments.len() {
        let argument = &arguments[index];
        if let Some(ignored) = ignored_option(argument) {
            if ignored == IgnoredOption::TakesValue {
                next_value(arguments, &mut index, argument, "")?;
            }
        } else if let Some(flag) = codegen_only_flag(argument) {
            flag.check_value(argument)?;
            parsed.saw(Opt::CodegenOnly(flag.spelling), argument);
            if flag.separate && argument == flag.spelling {
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
                _ if let Some(switch) = opt.math_switch() => parsed.math.push((switch, value)),
                Opt::Gnu89Inline => parsed.gnu89_inline = Some(value),
                Opt::Common => parsed.common = Some(value),
                Opt::ClangTrigraphs => parsed.trigraphs = Some(value),
                Opt::ClangDigraphs => parsed.digraphs = Some(value),
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
                Opt::ShortEnums => parsed.short_enums = Some(value),
                Opt::PackStruct => parsed.pack_struct = Some(value),
                Opt::MsBitfields => parsed.ms_bitfields = Some(value),
                Opt::Inline => parsed.inline = Some(value),
                Opt::InlineFunctions | Opt::InlineHintFunctions => {
                    parsed.inline_functions = Some(value)
                }
                Opt::OpenMp => parsed.openmp = Some(value),
                Opt::OpenMpSimd => parsed.openmp_simd = Some(value),
                Opt::Exceptions => parsed.exceptions = Some(value),
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
                Opt::DeleteNullPointerChecks => parsed.delete_null_pointer_checks = Some(value),
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
        } else if argument == "-trigraphs" || argument == "--trigraphs" {
            parsed.saw(Opt::Trigraphs, argument);
            parsed.trigraphs = Some(true);
        } else if argument == "-pthread" {
            parsed.saw(Opt::Pthread, argument);
            parsed.pthread = true;
        } else if let Some(runtime) = argument.strip_prefix("-fopenmp=") {
            parsed.saw(Opt::OpenMpRuntime, argument);
            parsed.openmp = Some(true);
            parsed.openmp_runtime = Some(runtime.into());
        } else if let Some(version) = argument.strip_prefix("-fopenmp-version=") {
            parsed.saw(Opt::OpenMpVersion, argument);
            parsed.openmp_version = Some(version.into());
        } else if argument == "-w" {
            parsed.saw(Opt::NoWarnings, argument);
            parsed.diagnostics.ignore_warnings = true;
        } else if argument == "/Zc:preprocessor" {
            parsed.saw(Opt::ConformingPreprocessor, argument);
        } else if argument == "/J" {
            parsed.saw(Opt::MsvcUnsignedChar, argument);
            parsed.char_signed = Some(false);
        } else if let Some(alignment) = argument.strip_prefix("/Zp") {
            parsed.saw(Opt::MsvcPack, argument);
            let alignment = match alignment {
                "" => 1,
                _ => {
                    match parse_value::<u32>(alignment.to_owned(), argument, "numeric argument")? {
                        alignment @ (1 | 2 | 4 | 8 | 16) => alignment,
                        _ => 8,
                    }
                }
            };
            parsed.pack_struct_alignment = Some(alignment);
        } else if argument == "-fpermissive" || argument == "-fno-permissive" {
            parsed.saw(Opt::Permissive, argument);
            parsed.diagnostics.permissive = argument == "-fpermissive";
        } else if let Some(value) = argument.strip_prefix("-fexcess-precision=") {
            parsed.saw(Opt::ExcessPrecision, argument);
            let fast = match value {
                "fast" => true,
                "standard" | "16" => false,
                _ => return Err(invalid(argument, "expected one of fast, standard, 16")),
            };
            parsed.math.push((MathSwitch::FastExcessPrecision, fast));
        } else if let Some(value) = argument.strip_prefix("-fpermitted-flt-eval-methods=") {
            parsed.saw(Opt::PermittedFltEvalMethods, argument);
            if !matches!(value, "c11" | "ts-18661-3") {
                return Err(invalid(argument, "expected one of c11, ts-18661-3"));
            }
            parsed.iso_flt_eval_method = true;
        } else if let Some(value) = argument.strip_prefix("-ftrivial-auto-var-init=") {
            parsed.saw(Opt::TrivialAutoVarInit, argument);
            parsed.trivial_auto_var_init = Some(parse_value(
                value.to_owned(),
                argument,
                "automatic variable initialization",
            )?);
        } else if let Some(mapping) = argument
            .strip_prefix("-fmacro-prefix-map=")
            .or_else(|| argument.strip_prefix("-ffile-prefix-map="))
        {
            parsed.saw(Opt::MacroPrefixMap, argument);
            let (old, new) = mapping
                .split_once('=')
                .ok_or_else(|| invalid(argument, "expected OLD=NEW"))?;
            parsed
                .macro_prefix_map
                .push((old.to_owned(), new.to_owned()));
        } else if let Some(register) = argument
            .strip_prefix("-ffixed-")
            .filter(|register| !register.is_empty())
        {
            parsed.saw(Opt::FixedRegister, argument);
            parsed.fixed_registers.push(register.to_owned());
        } else if argument == "-mllvm" || argument.starts_with("-mllvm=") {
            parsed.saw(Opt::Mllvm, argument);
            if argument == "-mllvm" {
                index += 1;
                if index == arguments.len() {
                    return Err(invalid(argument, "missing value"));
                }
            }
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
        } else if let Some(alignment) = argument.strip_prefix("-fpack-struct=") {
            parsed.saw(Opt::PackStructAlignment, argument);
            parsed.pack_struct_alignment =
                Some(parse_value(alignment.to_owned(), argument, "alignment")?);
        } else if let Some(style) = argument.strip_prefix("-ffp-contract=") {
            parsed.saw(Opt::FpContract, argument);
            let request = FpContract::parse(style).ok_or_else(|| {
                invalid(
                    argument,
                    "expected one of off, on, fast, fast-honor-pragmas",
                )
            })?;
            parsed.fp_contract = Some((request, parsed.math.len()));
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
        } else if let Some(value) = argument.strip_prefix("/D") {
            parsed.saw(Opt::MsvcDefine, argument);
            let define = next_value(arguments, &mut index, argument, value)?.replace('#', "=");
            parsed.defines.push(define.clone());
            parsed
                .preprocessor_inputs
                .macros
                .push(MacroOption::Define(define));
        } else if let Some(value) = argument.strip_prefix("/U") {
            parsed.saw(Opt::MsvcUndef, argument);
            parsed
                .preprocessor_inputs
                .macros
                .push(MacroOption::Undef(next_value(
                    arguments, &mut index, argument, value,
                )?));
        } else if let Some(value) = argument.strip_prefix("/FI") {
            parsed.saw(Opt::MsvcForceInclude, argument);
            parsed
                .preprocessor_inputs
                .includes
                .push(PathBuf::from(next_value(
                    arguments, &mut index, argument, value,
                )?));
        } else if let Some(value) = argument.strip_prefix("/I") {
            parsed.saw(Opt::MsvcInclude, argument);
            parsed
                .include
                .push(next_value(arguments, &mut index, argument, value)?);
        } else if argument == "/X" {
            parsed.saw(Opt::MsvcNoStandardIncludes, argument);
            parsed.nostdlibinc = true;
        } else if let Some(standard) = argument.strip_prefix("/std:") {
            parsed.saw(Opt::MsvcStandard, argument);
            parsed.standard = Some(match standard {
                "c11" => LanguageStandard::C11,
                "c17" => LanguageStandard::C17,
                "clatest" => LanguageStandard::C23,
                _ if standard.starts_with("c++") => {
                    return Err(invalid(argument, "a C++ standard does not apply to C"));
                }
                _ => return Err(invalid(argument, "expected one of c11, c17, clatest")),
            });
        } else if argument == "/experimental:c11atomics" {
            parsed.saw(Opt::MsvcC11Atomics, argument);
            parsed.c11_atomics = true;
        } else if let Some(convention) = msvc_convention(argument) {
            parsed.saw(Opt::MsvcCallingConvention, argument);
            if let Some((previous, spelling)) = &parsed.msvc_convention
                && *previous != convention
            {
                return Err(invalid(argument, &format!("incompatible with {spelling}")));
            }
            parsed.msvc_convention = Some((convention, argument.clone()));
        } else if let Some(mode) = argument.strip_prefix("/fp:") {
            parsed.saw(Opt::MsvcFloatingPoint, argument);
            let floating_point = &mut parsed.msvc_floating_point;
            let conflict = match mode {
                "strict" if floating_point.contract => Some("/fp:contract"),
                "contract" if floating_point.model == Some(MsvcFloatingModel::Strict) => {
                    Some("/fp:strict")
                }
                "fast" if floating_point.except == Some(true) => Some("/fp:except"),
                "except" if floating_point.model == Some(MsvcFloatingModel::Fast) => {
                    Some("/fp:fast")
                }
                "precise" | "strict" | "fast" | "contract" | "except" | "except-" => None,
                _ => {
                    return Err(invalid(
                        argument,
                        "expected one of precise, strict, fast, contract, except, except-",
                    ));
                }
            };
            if let Some(previous) = conflict {
                return Err(invalid(argument, &format!("incompatible with {previous}")));
            }
            match mode {
                "precise" => floating_point.model = Some(MsvcFloatingModel::Precise),
                "strict" => floating_point.model = Some(MsvcFloatingModel::Strict),
                "fast" => {
                    floating_point.model = Some(MsvcFloatingModel::Fast);
                    floating_point.contract = false;
                }
                "contract" => floating_point.contract = true,
                "except" => floating_point.except = Some(true),
                _ => floating_point.except = Some(false),
            }
        } else if argument.starts_with("/arch:") {
            parsed.saw(Opt::MsvcArch, argument);
            parsed.msvc_arch.push(argument.clone());
        } else if argument.starts_with("/feature:") {
            parsed.saw(Opt::MsvcFeature, argument);
            parsed.msvc_features.push(argument.clone());
        } else if argument == "/utf-8" {
            parsed.saw(Opt::MsvcUtf8, argument);
            if let Some((_, spelling)) = parsed
                .msvc_source_charset
                .as_ref()
                .or(parsed.msvc_execution_charset.as_ref())
            {
                return Err(invalid(argument, &format!("incompatible with {spelling}")));
            }
            parsed.msvc_utf8 = true;
        } else if let Some((slot, name)) = argument
            .strip_prefix("/source-charset:")
            .map(|name| (&mut parsed.msvc_source_charset, name))
            .or_else(|| {
                argument
                    .strip_prefix("/execution-charset:")
                    .map(|name| (&mut parsed.msvc_execution_charset, name))
            })
        {
            let charset = Charset::parse_msvc(name).ok_or_else(|| {
                invalid(
                    argument,
                    "expected one of utf-8, windows-1252, .65001, .1252",
                )
            })?;
            *slot = Some((charset, argument.clone()));
            if parsed.msvc_utf8 {
                return Err(invalid(argument, "incompatible with /utf-8"));
            }
            parsed.saw(Opt::MsvcCharset, argument);
        } else if argument == "/w" {
            parsed.saw(Opt::MsvcNoWarnings, argument);
            parsed.msvc_warning_level = parsed
                .msvc_warning_level
                .filter(|level| *level == 100)
                .or(Some(0));
        } else if let Some(value) = argument.strip_prefix("/W") {
            parsed.saw(Opt::MsvcWarningLevel, argument);
            let level = match next_value(arguments, &mut index, argument, value)? {
                level if level == "all" => 100,
                level => match parse_value::<u32>(level, argument, "numeric argument")? {
                    level @ 0..=4 => level,
                    _ => 1,
                },
            };
            if parsed.msvc_warning_level != Some(100) {
                parsed.msvc_warning_level = Some(level);
            }
        } else if ["/TP", "/Tp", "/clr", "/ZW", "/await"]
            .iter()
            .any(|prefix| argument.starts_with(prefix))
        {
            return Err(invalid(argument, "a C++ option does not apply to C"));
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
        } else if matches!(argument.as_str(), "-ansi" | "--ansi") {
            parsed.saw(Opt::Ansi, argument);
            parsed.standard = Some(LanguageStandard::C89);
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
        } else if let Some(value) = option_value(argument, "compiler") {
            parsed.saw(Opt::Compiler, argument);
            parsed.compiler = Some(next_value(arguments, &mut index, argument, value)?);
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
                "" | "1" | "2" | "3" | "g" | "fast" => Optimization::Speed,
                "s" | "z" => Optimization::Size,
                _ => return Err(invalid(argument, "unknown optimization level")),
            });
            parsed.set_fast_optimization(level == "fast");
        } else if let Some(format) = long_double_flag(argument) {
            parsed.saw(Opt::LongDouble, argument);
            parsed.long_double = Some(format);
        } else {
            return Err(invalid(argument, "unknown option"));
        }
        index += 1;
    }
    if let (true, Some(position)) = (parsed.flavor.is_gcc(), parsed.fast_optimization) {
        let entry = parsed.math.remove(position);
        parsed.math.insert(0, entry);
    }
    parsed.signed_overflow = signed_overflow(parsed.flavor, arguments);
    let optimization_macros = match parsed.optimization {
        None | Some(Optimization::None) => Vec::new(),
        Some(Optimization::Speed) => vec!["__OPTIMIZE__"],
        Some(Optimization::Size) => vec!["__OPTIMIZE__", "__OPTIMIZE_SIZE__"],
    };
    if !optimization_macros.is_empty() {
        let keep_no_inline = parsed.no_inline_requested();
        parsed.preprocessor_inputs.macros.splice(
            0..0,
            (!keep_no_inline)
                .then(|| MacroOption::Undef("__NO_INLINE__".into()))
                .into_iter()
                .chain(
                    optimization_macros
                        .into_iter()
                        .map(|name| MacroOption::Define(name.into())),
                ),
        );
    }
    if parsed.exceptions == Some(true) {
        parsed
            .preprocessor_inputs
            .macros
            .insert(0, MacroOption::Define("__EXCEPTIONS".into()));
    }
    parsed.openmp_date = parsed.openmp_date()?;
    if let Some(date) = parsed.openmp_date {
        parsed
            .preprocessor_inputs
            .macros
            .insert(0, MacroOption::Define(format!("_OPENMP={date}")));
    }
    if parsed.pthread || (parsed.flavor.is_gcc() && parsed.openmp_date.is_some()) {
        parsed
            .preprocessor_inputs
            .macros
            .insert(0, MacroOption::Define("_REENTRANT".into()));
    }
    if parsed.flavor.is_gcc() {
        parsed.macro_prefix_map.reverse();
    } else {
        parsed
            .macro_prefix_map
            .sort_by(|left, right| right.0.cmp(&left.0));
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
        delete_null_pointer_checks: raw.delete_null_pointer_checks.unwrap_or(true),
        trivial_auto_var_init: raw.trivial_auto_var_init.unwrap_or_default(),
        optimization: raw.optimization.unwrap_or_default(),
    }
}

fn last_flag(arguments: &[String], opt: Opt) -> Option<(usize, bool)> {
    arguments
        .iter()
        .enumerate()
        .rev()
        .find_map(|(index, argument)| opt.parse_switch(argument).map(|value| (index, value)))
}

fn msvc_convention(argument: &str) -> Option<crate::ir::CallConv> {
    use crate::ir::CallConv;
    Some(match argument {
        "/Gd" => CallConv::C,
        "/Gr" => CallConv::X86Fastcall,
        "/Gz" => CallConv::X86Stdcall,
        "/Gv" => CallConv::X86Vectorcall,
        _ => return None,
    })
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

fn parse_clang_integer(value: &str) -> Option<i32> {
    let (negative, digits) = match value.strip_prefix('-') {
        Some(rest) => (true, rest),
        None => (false, value),
    };
    let (radix, digits) = if let Some(rest) = digits
        .strip_prefix("0x")
        .or_else(|| digits.strip_prefix("0X"))
    {
        (16, rest)
    } else if let Some(rest) = digits
        .strip_prefix("0b")
        .or_else(|| digits.strip_prefix("0B"))
    {
        (2, rest)
    } else if let Some(rest) = digits.strip_prefix("0o") {
        (8, rest)
    } else if digits.len() > 1
        && let Some(rest) = digits.strip_prefix('0')
    {
        (8, rest)
    } else {
        (10, digits)
    };
    if digits.is_empty() || digits.starts_with(['+', '-']) {
        return None;
    }
    let magnitude = i64::from_str_radix(digits, radix).ok()?;
    i32::try_from(if negative { -magnitude } else { magnitude }).ok()
}

fn invalid(argument: &str, reason: &str) -> CompilerArgError {
    CompilerArgError::Invalid {
        argument: argument.into(),
        reason: reason.into(),
    }
}

fn msvc_x86_arch_level(argument: &str) -> Result<(MsvcArchLevel, Option<u32>), CompilerArgError> {
    Ok(match &argument["/arch:".len()..] {
        "IA32" => (MsvcArchLevel::Ia32, None),
        "SSE" => (MsvcArchLevel::Sse, None),
        "SSE2" => (MsvcArchLevel::Sse2, None),
        "SSE4.2" => (MsvcArchLevel::Sse42, None),
        "AVX" => (MsvcArchLevel::Avx, None),
        "AVX2" => (MsvcArchLevel::Avx2, None),
        "AVX512" => (MsvcArchLevel::Avx512, None),
        "AVX10.1" => (MsvcArchLevel::Avx512, Some(1)),
        "AVX10.2" => (MsvcArchLevel::Avx512, Some(2)),
        _ => {
            return Err(invalid(
                argument,
                "expected one of IA32, SSE, SSE2, SSE4.2, AVX, AVX2, AVX512, AVX10.1, AVX10.2",
            ));
        }
    })
}

fn resolve_msvc_apx(family: TargetFamily, features: &[String]) -> Result<bool, CompilerArgError> {
    for argument in features {
        if argument != "/feature:APX" {
            return Err(invalid(argument, "expected APX"));
        }
        if family != TargetFamily::X86_64 {
            return Err(invalid(argument, "only supported on x64"));
        }
    }
    Ok(!features.is_empty())
}

fn resolve_msvc_arm64_arch(
    arches: &[String],
    features: &[String],
) -> Result<Option<MsvcArm64Arch>, CompilerArgError> {
    if arches.is_empty() && features.is_empty() {
        return Ok(None);
    }
    let mut version = (8, 0);
    let mut extensions = Vec::new();
    for argument in arches {
        let mut names = argument["/arch:".len()..].split('+');
        version = names.next().and_then(msvc_arm64_version).ok_or_else(|| {
            invalid(
                argument,
                "expected one of armv8.0 through armv8.9, armv9.0 through armv9.4",
            )
        })?;
        extensions = names
            .map(|name| msvc_arm64_feature(argument, name))
            .collect::<Result<_, _>>()?;
    }
    for argument in features {
        for name in argument["/feature:".len()..].split('+') {
            extensions.push(msvc_arm64_feature(argument, name)?);
        }
    }
    let (major, minor) = version;
    let level = if major == 9 { 5 + minor } else { minor };
    let mut arch = MsvcArm64Arch {
        major,
        minor,
        lse: level >= 1,
        rcpc: u8::from(level >= 3) + u8::from(level >= 4),
    };
    for name in extensions {
        match name {
            "lse" => arch.lse = true,
            "rcpc" => arch.rcpc = arch.rcpc.max(1),
            _ => arch.rcpc = 2,
        }
    }
    Ok(Some(arch))
}

fn msvc_arm64_version(name: &str) -> Option<(u8, u8)> {
    let (major, minor) = name.strip_prefix("armv")?.split_once('.')?;
    let last = match major {
        "8" => b'9',
        "9" => b'4',
        _ => return None,
    };
    match minor.as_bytes() {
        &[digit @ b'0'..=b'9'] if digit <= last => Some((major.parse().ok()?, digit - b'0')),
        _ => None,
    }
}

fn msvc_arm64_feature<'a>(argument: &str, name: &'a str) -> Result<&'a str, CompilerArgError> {
    match name {
        "lse" | "rcpc" | "rcpc2" => Ok(name),
        _ => Err(invalid(argument, "expected one of lse, rcpc, rcpc2")),
    }
}

fn msvc_arm64_march(arch: MsvcArm64Arch) -> ArmMarch {
    let features = [
        (arch.lse, AArch64Feature::Lse),
        (arch.rcpc > 0, AArch64Feature::Rcpc),
    ];
    ArmMarch::with_features(
        arch.major,
        arch.minor,
        &features
            .into_iter()
            .filter_map(|(enabled, feature)| enabled.then_some(feature))
            .collect::<Vec<_>>(),
    )
}

fn resolve_msvc_arch(
    family: TargetFamily,
    options: &[String],
) -> Result<Option<MsvcArch>, CompilerArgError> {
    let mut resolved: Option<MsvcArch> = None;
    let mut avx10: Option<&str> = None;
    for argument in options {
        let (level, version) = &msvc_x86_arch_level(argument)?;
        match family {
            TargetFamily::X86_64 if *level < MsvcArchLevel::Sse2 => {
                return Err(invalid(argument, "only supported on x86"));
            }
            TargetFamily::X86 if *level == MsvcArchLevel::Sse42 => {
                return Err(invalid(argument, "only supported on x64"));
            }
            TargetFamily::X86 | TargetFamily::X86_64 => {}
            _ => return Err(invalid(argument, "only supported on x86, x64 and arm64")),
        }
        if family == TargetFamily::X86 && *level <= MsvcArchLevel::Sse2 {
            if let Some(previous) = avx10 {
                return Err(invalid(argument, &format!("unsupported after {previous}")));
            }
            resolved = Some(MsvcArch {
                level: *level,
                avx10: None,
            });
            continue;
        }
        if version.is_some() {
            avx10 = Some(argument);
        }
        resolved = Some(MsvcArch {
            level: resolved.map_or(*level, |resolved| resolved.level.max(*level)),
            avx10: version.or(resolved.and_then(|resolved| resolved.avx10)),
        });
    }
    Ok(resolved)
}

fn request_msvc_arch(arch: MsvcArch, request: &mut X86IsaRequest) {
    use X86Feature::*;
    let features: &[X86Feature] = match arch.level {
        MsvcArchLevel::Ia32 => return request.set(Sse, false),
        MsvcArchLevel::Sse => return request.set(Sse2, false),
        MsvcArchLevel::Sse2 => &[],
        MsvcArchLevel::Sse42 => &[Sse42],
        MsvcArchLevel::Avx => &[Avx],
        MsvcArchLevel::Avx2 => &[Avx2],
        MsvcArchLevel::Avx512 => &[Avx512f, Avx512cd, Avx512bw, Avx512dq, Avx512vl],
    };
    for &feature in features {
        request.set(feature, true);
    }
    match arch.avx10 {
        Some(1) => request.set(Avx101, true),
        Some(_) => request.set(Avx102, true),
        None => {}
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
        Rule::validate("GCC MS bit-fields", move |args: &ParsedCompilerArgs| {
            if args.ms_bitfields.is_some() && !target.family.is_x86() {
                Err(format!(
                    "MS bit-fields are unsupported for {}",
                    target.triple
                ))
            } else {
                Ok(())
            }
        }),
        Rule::validate(
            "GCC contraction style",
            |args: &ParsedCompilerArgs| match args.fp_contract {
                Some((FpContract::FastHonorPragmas, _)) => {
                    Err("unknown floating point contraction style 'fast-honor-pragmas'".to_owned())
                }
                _ => Ok(()),
            },
        ),
        Rule::validate(
            "GCC structure packing",
            |args: &ParsedCompilerArgs| match args.pack_struct_alignment {
                Some(alignment) if !matches!(alignment, 1 | 2 | 4 | 8 | 16) => Err(format!(
                    "structure alignment must be a small power of two, not {alignment}"
                )),
                _ => Ok(()),
            },
        ),
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
            "Clang structure packing",
            |args: &ParsedCompilerArgs| match args.pack_struct_alignment {
                Some(alignment) if alignment != 0 && !alignment.is_power_of_two() => Err(format!(
                    "a structure packing of {alignment}, which is not a power of two, is not emulated"
                )),
                _ => Ok(()),
            },
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
