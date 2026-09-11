use std::iter::Peekable;

#[derive(Debug, Default, PartialEq, Eq)]
pub struct CompilerArgs {
    pub defines: Vec<String>,
    pub standard: Option<String>,
    pub isystem: Vec<String>,
}

pub struct CompilerArgParser;

impl CompilerArgParser {
    pub fn parse<I>(args: I) -> Result<CompilerArgs, String>
    where
        I: IntoIterator<Item = String>,
    {
        let mut args = args.into_iter().peekable();
        let mut parsed = CompilerArgs::default();
        while let Some(arg) = args.next() {
            if let Some(define) = arg.strip_prefix("-D") {
                parsed.defines.push(Self::value(define, &mut args, "-D")?);
            } else if let Some(standard) = arg.strip_prefix("-std=") {
                parsed.standard = Some(standard.to_string());
            } else if arg == "-std" {
                parsed.standard = Some(Self::next_value(&mut args, "-std")?);
            } else if let Some(isystem) = arg.strip_prefix("-isystem") {
                parsed.isystem.push(Self::value(isystem, &mut args, "-isystem")?);
            } else {
                return Err(format!("unsupported argument: {arg}"));
            }
        }
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
