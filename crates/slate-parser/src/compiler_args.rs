use crate::compiler_options::{CompilerOptions, LayoutOptions, MicrosoftFlags, OperationValues};
use crate::diagnostics::{DiagnosticOptions, Warning};
use crate::files::SearchPaths;
use crate::ir::{AsmDialect, Overflow};
use crate::pp::{MacroOption, PreprocessorInputs};
use crate::rules::{Rule, Rules};
use crate::target::isa::{IsaRequest, TargetIsa};
use crate::target::x86_isa::X86Feature;
use crate::target_info::{LongDoubleFormat, TargetFamily, TargetInfo};
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
        system.extend(compiler_headers::include_paths(&self.target, self.flavor));
        if !self.nostdlibinc {
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
    flavor: CompilerFlavor,
    target: String,
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
        match self
            .occurrences
            .iter()
            .find(|(opt, _)| !opt.accepted_by(self.flavor))
        {
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
    Flavor,
    Target,
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
            Self::Flavor => "flavor",
            Self::Target => "target",
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
            | Self::Optimize => &[Gcc, Clang],
            Self::PreferredStackBoundary => &[Gcc],
            Self::StackAlignment | Self::ClangCodegenOnly => &[Clang],
        };
        flavors.contains(&flavor)
    }

    fn name(self) -> &'static str {
        match self {
            Self::Wrapv => "wrapv",
            Self::Trapv => "trapv",
            Self::StrictOverflow => "strict-overflow",
            Self::RoundingMath => "rounding-math",
            Self::TrappingMath => "trapping-math",
            Self::Gnu89Inline => "gnu89-inline",
            Self::Common => "common",
            Self::MsExtensions => "ms-extensions",
            Self::MsCompatibility => "ms-compatibility",
            _ => "",
        }
    }

    fn parse_flag(self, argument: &str) -> Option<bool> {
        parse_f_flag(self.name(), argument)
    }
}

fn parse_f_flag(name: &str, argument: &str) -> Option<bool> {
    let flag = argument
        .strip_prefix("--f")
        .or_else(|| argument.strip_prefix("-f"))?;
    match flag.strip_prefix("no-") {
        Some(negated) if negated == name => Some(false),
        _ if flag == name => Some(true),
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

const CODEGEN_ONLY_FLAGS: [&str; 13] = [
    "omit-frame-pointer",
    "lto",
    "function-sections",
    "data-sections",
    "strict-aliasing",
    "plt",
    "semantic-interposition",
    "asynchronous-unwind-tables",
    "unwind-tables",
    "stack-clash-protection",
    "merge-all-constants",
    "ident",
    "addrsig",
];

const CODEGEN_ONLY_VALUE_FLAGS: [&str; 3] = ["lto=", "visibility=", "debug-prefix-map="];

const CLANG_CODEGEN_ONLY_M_FLAGS: [&str; 1] = ["outline"];

fn clang_codegen_only(argument: &str) -> bool {
    argument.strip_prefix("-m").is_some_and(|flag| {
        CLANG_CODEGEN_ONLY_M_FLAGS.contains(&flag.strip_prefix("no-").unwrap_or(flag))
    })
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
        .any(|name| parse_f_flag(name, argument).is_some())
        || CODEGEN_ONLY_VALUE_FLAGS
            .iter()
            .any(|prefix| argument.starts_with(&format!("-f{prefix}")));
    (IGNORED_DRIVER_FLAGS.contains(&argument)
        || argument.starts_with("-g")
        || joined_value
        || codegen_only)
        .then_some(IgnoredOption::Alone)
}

const FLAG_OPTS: [Opt; 9] = [
    Opt::Gnu89Inline,
    Opt::Common,
    Opt::MsExtensions,
    Opt::MsCompatibility,
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
        let mut target = TargetInfo::for_triple_and_flavor(&raw.target, raw.flavor)?;
        validate_rules(&target).check(&raw)?;
        target.isa = TargetIsa::resolve(target.family, target.environment, &raw.isa, raw.flavor)
            .map_err(|reason| invalid(&raw.target, &reason))?;
        let flavor = raw.flavor;
        let layout = LayoutOptions {
            long_double: raw.long_double,
            preferred_stack_alignment: raw
                .preferred_stack_boundary
                .map(|exponent| 1u32 << exponent)
                .or(raw.stack_alignment),
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
        options.common = raw.common.unwrap_or(false);
        options.microsoft = MicrosoftFlags {
            extensions: raw.ms_extensions,
            compatibility: raw.ms_compatibility,
        };
        options.asm_dialect = raw.asm_dialect.unwrap_or_default();
        options.explicit_standard = raw.standard.is_some();
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
        } else if let Some((opt, value)) = FLAG_OPTS
            .iter()
            .find_map(|opt| opt.parse_flag(argument).map(|value| (*opt, value)))
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
                Opt::MsExtensions => parsed.ms_extensions = Some(value),
                Opt::MsCompatibility => parsed.ms_compatibility = Some(value),
                _ => return Err(invalid(argument, "unknown flag")),
            }
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
        last_flag(arguments, "wrapv"),
        last_flag(arguments, "strict-overflow"),
    ) {
        (Some(wrap), Some(strict)) if strict.0 > wrap.0 => Some((strict.0, !strict.1)),
        (None, Some(strict)) => Some((strict.0, !strict.1)),
        (wrap, _) => wrap,
    };
    let trap = last_flag(arguments, "trapv");
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

fn last_flag(arguments: &[String], name: &str) -> Option<(usize, bool)> {
    arguments
        .iter()
        .enumerate()
        .rev()
        .find_map(|(index, argument)| parse_f_flag(name, argument).map(|value| (index, value)))
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
    )
}
