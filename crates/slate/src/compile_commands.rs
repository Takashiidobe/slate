use serde_json::Value;
use slate_parser::compiler_args::{
    CompilerArgError, CompilerArgParser, CompilerArgs, CompilerFlavor,
};
use std::path::{Path, PathBuf};
use thiserror::Error;

#[derive(Debug, Error)]
pub enum CompileCommandsError {
    #[error("read compile commands {path}: {source}")]
    Read {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("parse compile commands {path}: {source}")]
    Parse {
        path: PathBuf,
        #[source]
        source: serde_json::Error,
    },
    #[error("compile commands {path} must contain a JSON array")]
    ExpectedArray { path: PathBuf },
    #[error("compile commands {path} entry {index} must be an object")]
    ExpectedObject { path: PathBuf, index: usize },
    #[error("compile commands {path} entry {index} requires string field {field}")]
    StringField {
        path: PathBuf,
        index: usize,
        field: &'static str,
    },
    #[error("compile commands {path} entry {index} cannot contain both arguments and command")]
    ConflictingCommandForms { path: PathBuf, index: usize },
    #[error("compile commands {path} entry {index} field arguments must be an array")]
    ArgumentsNotArray { path: PathBuf, index: usize },
    #[error(
        "compile commands {path} entry {index} has a non-string argument at position {argument}"
    )]
    NonStringArgument {
        path: PathBuf,
        index: usize,
        argument: usize,
    },
    #[error("compile commands {path} entry {index} has invalid shell quoting")]
    InvalidShellQuoting { path: PathBuf, index: usize },
    #[error("compile commands {path} entry {index} has an empty command")]
    EmptyCommand { path: PathBuf, index: usize },
    #[error("compile commands {path} entry {index} ({}): {source}", file.display())]
    Arguments {
        path: PathBuf,
        index: usize,
        file: PathBuf,
        #[source]
        source: CompilerArgError,
    },
    #[error("compile command inputs contain no C translation units")]
    NoCTranslationUnits { paths: Vec<PathBuf> },
}

#[derive(Debug, PartialEq, Eq)]
pub struct CompileCommand {
    pub file: PathBuf,
    pub args: CompilerArgs,
}

pub struct ReadOptions<'a> {
    pub extra_args: &'a [String],
    pub flavor: Option<CompilerFlavor>,
}

pub fn read(
    paths: &[PathBuf],
    options: &ReadOptions,
) -> Result<Vec<CompileCommand>, CompileCommandsError> {
    let mut parsed = Vec::new();
    for path in paths {
        parsed.extend(read_one(path, options)?);
    }
    parsed.sort_by(|left, right| left.file.cmp(&right.file));
    let mut commands: Vec<CompileCommand> = Vec::new();
    for command in parsed {
        let duplicate = commands
            .iter()
            .rev()
            .take_while(|earlier| earlier.file == command.file)
            .any(|earlier| *earlier == command);
        if !duplicate {
            commands.push(command);
        }
    }
    if commands.is_empty() {
        return Err(CompileCommandsError::NoCTranslationUnits {
            paths: paths.to_vec(),
        });
    }
    Ok(commands)
}

fn read_one(
    path: &Path,
    options: &ReadOptions,
) -> Result<Vec<CompileCommand>, CompileCommandsError> {
    let bytes = std::fs::read(path).map_err(|source| CompileCommandsError::Read {
        path: path.to_path_buf(),
        source,
    })?;
    let entries: Value =
        serde_json::from_slice(&bytes).map_err(|source| CompileCommandsError::Parse {
            path: path.to_path_buf(),
            source,
        })?;
    let entries = entries
        .as_array()
        .ok_or_else(|| CompileCommandsError::ExpectedArray {
            path: path.to_path_buf(),
        })?;
    let database_dir = path.parent().unwrap_or_else(|| Path::new("."));
    let mut commands = Vec::new();
    for (index, entry) in entries.iter().enumerate() {
        let entry = entry
            .as_object()
            .ok_or_else(|| CompileCommandsError::ExpectedObject {
                path: path.to_path_buf(),
                index,
            })?;
        let directory = string_field(entry, "directory", path, index)?;
        let directory = absolute_path(database_dir, Path::new(directory));
        let file = string_field(entry, "file", path, index)?;
        let file = absolute_path(&directory, Path::new(file));
        if file.extension().and_then(|extension| extension.to_str()) != Some("c") {
            continue;
        }
        if entry.contains_key("arguments") && entry.contains_key("command") {
            return Err(CompileCommandsError::ConflictingCommandForms {
                path: path.to_path_buf(),
                index,
            });
        }
        let words = match entry.get("arguments") {
            Some(Value::Array(arguments)) => arguments
                .iter()
                .enumerate()
                .map(|(argument_index, argument)| {
                    argument.as_str().map(str::to_string).ok_or_else(|| {
                        CompileCommandsError::NonStringArgument {
                            path: path.to_path_buf(),
                            index,
                            argument: argument_index,
                        }
                    })
                })
                .collect::<Result<Vec<_>, _>>()?,
            Some(_) => {
                return Err(CompileCommandsError::ArgumentsNotArray {
                    path: path.to_path_buf(),
                    index,
                });
            }
            None => {
                let command = string_field(entry, "command", path, index)?;
                shlex::split(command).ok_or_else(|| CompileCommandsError::InvalidShellQuoting {
                    path: path.to_path_buf(),
                    index,
                })?
            }
        };
        if words.is_empty() {
            return Err(CompileCommandsError::EmptyCommand {
                path: path.to_path_buf(),
                index,
            });
        }
        let args = CompilerArgParser::parse_in(
            command_arguments(&words, &directory, &file, options),
            &directory,
        )
        .map_err(|source| CompileCommandsError::Arguments {
            path: path.to_path_buf(),
            index,
            file: file.clone(),
            source,
        })?;
        commands.push(CompileCommand { file, args });
    }
    Ok(commands)
}

fn string_field<'a>(
    entry: &'a serde_json::Map<String, Value>,
    field: &'static str,
    path: &Path,
    index: usize,
) -> Result<&'a str, CompileCommandsError> {
    entry
        .get(field)
        .and_then(Value::as_str)
        .ok_or_else(|| CompileCommandsError::StringField {
            path: path.to_path_buf(),
            index,
            field,
        })
}

const PATH_OPTIONS: [&str; 8] = [
    "-isystem",
    "-iquote",
    "-idirafter",
    "-include",
    "-imacros",
    "-isysroot",
    "--sysroot=",
    "-I",
];

pub const INCLUDE_DIR_OPTIONS: [&str; 4] = ["-I", "-isystem", "-iquote", "-idirafter"];

pub enum PathOption<'a> {
    Separate(&'a str),
    Joined(&'a str, &'a str),
}

pub fn path_option(word: &str) -> Option<PathOption<'_>> {
    if word == "--sysroot" {
        return Some(PathOption::Separate(word));
    }
    PATH_OPTIONS.iter().find_map(|option| {
        let value = word.strip_prefix(option)?;
        match value {
            "" if option.ends_with('=') => None,
            "" => Some(PathOption::Separate(option)),
            value if value.starts_with('-') => None,
            value => Some(PathOption::Joined(option, value)),
        }
    })
}

pub fn absolute_path(base: &Path, path: &Path) -> PathBuf {
    let path = if path.is_absolute() {
        path.to_path_buf()
    } else {
        base.join(path)
    };
    path.canonicalize().unwrap_or(path)
}

fn command_arguments(
    words: &[String],
    directory: &Path,
    file: &Path,
    options: &ReadOptions,
) -> Vec<String> {
    let compiler = &words[0];
    let flavor = options.flavor.unwrap_or_else(|| compiler_flavor(compiler));
    let mut args = vec![format!("--flavor={flavor}")];
    args.extend(compiler_target(compiler).map(|target| format!("--target={target}")));
    args.extend(
        words[1..]
            .iter()
            .filter(|word| {
                word.starts_with('-') || absolute_path(directory, Path::new(word)) != file
            })
            .cloned(),
    );
    args.extend(options.extra_args.iter().cloned());
    crate::target::compiler_arguments(&args)
}

fn compiler_flavor(compiler: &str) -> CompilerFlavor {
    let name = Path::new(compiler)
        .file_name()
        .and_then(|name| name.to_str())
        .unwrap_or(compiler);
    let name = name.strip_suffix(".exe").unwrap_or(name);
    let gcc = name == "gcc"
        || name.ends_with("-gcc")
        || name.starts_with("gcc-")
        || name.contains("-gcc-");
    if name == "cl" {
        CompilerFlavor::Msvc
    } else if gcc && !name.contains("clang") {
        CompilerFlavor::Gcc
    } else {
        CompilerFlavor::Clang
    }
}

fn compiler_target(compiler: &str) -> Option<String> {
    let name = Path::new(compiler).file_name()?.to_str()?;
    for suffix in ["-clang++", "-clang", "-g++", "-gcc", "-cc"] {
        if let Some(target) = name
            .strip_suffix(suffix)
            .filter(|target| target.contains('-'))
        {
            return Some(target.to_string());
        }
    }
    None
}
