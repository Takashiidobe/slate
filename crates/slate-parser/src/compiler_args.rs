use std::iter::Peekable;
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
            _ => Err(format!(
                "unknown compiler flavor: {name} (expected gcc, clang, or msvc)"
            )),
        }
    }
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum LanguageStandard {
    C89,
    Gnu89,
    C99,
    Gnu99,
    C11,
    Gnu11,
    C17,
    Gnu17,
    #[default]
    C23,
    Gnu23,
}

impl LanguageStandard {
    pub fn stdc_version(self) -> Option<i64> {
        match self {
            Self::C89 | Self::Gnu89 => None,
            Self::C99 | Self::Gnu99 => Some(199901),
            Self::C11 | Self::Gnu11 => Some(201112),
            Self::C17 | Self::Gnu17 => Some(201710),
            Self::C23 | Self::Gnu23 => Some(202311),
        }
    }

    pub fn is_c23_or_later(self) -> bool {
        matches!(self, Self::C23 | Self::Gnu23)
    }

    pub fn allows_implicit_int(self) -> bool {
        matches!(self, Self::C89 | Self::Gnu89)
    }
}

impl FromStr for LanguageStandard {
    type Err = String;

    fn from_str(name: &str) -> Result<Self, Self::Err> {
        match name {
            "c89" => Ok(Self::C89),
            "gnu89" => Ok(Self::Gnu89),
            "c99" => Ok(Self::C99),
            "gnu99" => Ok(Self::Gnu99),
            "c11" => Ok(Self::C11),
            "gnu11" => Ok(Self::Gnu11),
            "c17" => Ok(Self::C17),
            "gnu17" => Ok(Self::Gnu17),
            "c23" => Ok(Self::C23),
            "gnu23" => Ok(Self::Gnu23),
            _ => Err(format!(
                "unknown language standard: {name} (expected c89, gnu89, c99, gnu99, c11, gnu11, c17, gnu17, c23, or gnu23)"
            )),
        }
    }
}

#[derive(Debug, Default, PartialEq, Eq)]
pub struct CompilerArgs {
    pub options: crate::compiler_options::CompilerOptions,
    pub defines: Vec<String>,
    pub standard: LanguageStandard,
    pub isystem: Vec<String>,
    pub flavor: CompilerFlavor,
}

pub struct CompilerArgParser;

impl CompilerArgParser {
    pub fn parse<I>(args: I) -> Result<CompilerArgs, String>
    where
        I: IntoIterator<Item = String>,
    {
        let arguments: Vec<String> = args.into_iter().collect();
        let mut args = arguments.clone().into_iter().peekable();
        let mut parsed = CompilerArgs::default();
        while let Some(arg) = args.next() {
            if let Some(define) = arg.strip_prefix("-D") {
                parsed.defines.push(Self::value(define, &mut args, "-D")?);
            } else if let Some(standard) = arg.strip_prefix("-std=") {
                parsed.standard = standard.parse()?;
            } else if arg == "-std" {
                parsed.standard = Self::next_value(&mut args, "-std")?.parse()?;
            } else if let Some(isystem) = arg.strip_prefix("-isystem") {
                parsed
                    .isystem
                    .push(Self::value(isystem, &mut args, "-isystem")?);
            } else if let Some(flavor) = arg.strip_prefix("--flavor=") {
                parsed.flavor = flavor.parse()?;
            } else if arg == "--flavor" {
                parsed.flavor = Self::next_value(&mut args, "--flavor")?.parse()?;
            } else if crate::compiler_options::CompilerOptions::recognizes(&arg) {
            } else {
                return Err(format!("unsupported argument: {arg}"));
            }
        }
        parsed.options =
            crate::compiler_options::CompilerOptions::resolve(arguments, parsed.flavor)
                .map_err(|error| error.to_string())?;
        Ok(parsed)
    }

    fn value<I>(value: &str, args: &mut Peekable<I>, option: &str) -> Result<String, String>
    where
        I: Iterator<Item = String>,
    {
        if value.is_empty() {
            Self::next_value(args, option)
        } else {
            Ok(value.to_string())
        }
    }

    fn next_value<I>(args: &mut Peekable<I>, option: &str) -> Result<String, String>
    where
        I: Iterator<Item = String>,
    {
        args.next()
            .filter(|value| !value.starts_with('-'))
            .ok_or_else(|| format!("missing value for {option}"))
    }
}
