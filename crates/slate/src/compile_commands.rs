use serde_json::Value;
use slate_parser::compiler_args::{
    CompilerArgError, CompilerArgParser, CompilerArgs, CompilerFlavor,
};
use std::cmp::Reverse;
use std::collections::{BTreeMap, BTreeSet};
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
    #[error("unknown build target {name}, the compile commands have: {known}")]
    UnknownBuildTarget { name: String, known: String },
    #[error(
        "compile commands have different arguments for {} and no output paths to choose a build target from",
        file.display()
    )]
    NoBuildTargets { file: PathBuf },
    #[error("build target {target} compiles {} with different arguments", file.display())]
    MultipleCommandsInTarget { file: PathBuf, target: String },
}

#[derive(Debug, PartialEq, Eq)]
pub struct CompileCommand {
    pub file: PathBuf,
    pub args: CompilerArgs,
}

pub struct ReadOptions<'a> {
    pub extra_args: &'a [String],
    pub flavor: Option<CompilerFlavor>,
    pub build_target: Option<&'a str>,
}

pub struct Commands {
    pub commands: Vec<CompileCommand>,
    pub selected: Option<SelectedTarget>,
}

pub struct SelectedTarget {
    pub name: String,
    pub others: Vec<String>,
}

struct Entry {
    command: CompileCommand,
    target: Option<String>,
    position_independent: bool,
}

pub fn read(paths: &[PathBuf], options: &ReadOptions) -> Result<Commands, CompileCommandsError> {
    let mut parsed = Vec::new();
    for path in paths {
        parsed.extend(read_one(path, options)?);
    }
    parsed.sort_by(|left, right| left.command.file.cmp(&right.command.file));
    if parsed.is_empty() {
        return Err(CompileCommandsError::NoCTranslationUnits {
            paths: paths.to_vec(),
        });
    }
    let conflict = parsed
        .windows(2)
        .find(|pair| {
            pair[0].command.file == pair[1].command.file
                && pair[0].command.args != pair[1].command.args
        })
        .map(|pair| pair[0].command.file.clone());
    let (entries, selected) = match (options.build_target, conflict) {
        (None, None) => (parsed, None),
        (requested, conflict) => select_target(parsed, requested, conflict)?,
    };
    let mut commands: Vec<CompileCommand> = Vec::new();
    for Entry { command, .. } in entries {
        match commands.last() {
            Some(earlier) if earlier.file == command.file => {
                if selected.is_some() && earlier.args != command.args {
                    return Err(CompileCommandsError::MultipleCommandsInTarget {
                        file: command.file,
                        target: selected.map_or_else(String::new, |target| target.name),
                    });
                }
            }
            _ => commands.push(command),
        }
    }
    Ok(Commands { commands, selected })
}

fn select_target(
    entries: Vec<Entry>,
    requested: Option<&str>,
    conflict: Option<PathBuf>,
) -> Result<(Vec<Entry>, Option<SelectedTarget>), CompileCommandsError> {
    let mut targets: BTreeMap<String, (BTreeSet<&Path>, bool)> = BTreeMap::new();
    for entry in &entries {
        if let Some(target) = &entry.target {
            let (files, position_independent) = targets.entry(target.clone()).or_default();
            files.insert(&entry.command.file);
            *position_independent |= entry.position_independent;
        }
    }
    let name = match requested {
        Some(requested) if targets.contains_key(requested) => requested.to_string(),
        Some(requested) => {
            return Err(CompileCommandsError::UnknownBuildTarget {
                name: requested.to_string(),
                known: targets.keys().cloned().collect::<Vec<_>>().join(", "),
            });
        }
        None => targets
            .iter()
            .max_by_key(|(name, (files, position_independent))| {
                let is_program = files.iter().any(|file| mentions_main(file));
                (
                    !is_program,
                    files.len(),
                    !position_independent,
                    Reverse(*name),
                )
            })
            .map(|(name, _)| name.clone())
            .ok_or_else(|| CompileCommandsError::NoBuildTargets {
                file: conflict.unwrap_or_default(),
            })?,
    };
    let others = targets
        .keys()
        .filter(|other| **other != name)
        .cloned()
        .collect();
    let selected: Vec<Entry> = entries
        .into_iter()
        .filter(|entry| entry.target.as_deref() == Some(name.as_str()))
        .collect();
    Ok((selected, Some(SelectedTarget { name, others })))
}

fn code_only(source: &[u8]) -> Vec<u8> {
    let mut code = Vec::with_capacity(source.len());
    let mut at = 0;
    while at < source.len() {
        let rest = &source[at..];
        if rest.starts_with(b"//") {
            at += rest
                .iter()
                .position(|byte| *byte == b'\n')
                .unwrap_or(rest.len());
        } else if rest.starts_with(b"/*") {
            at += rest
                .windows(2)
                .skip(2)
                .position(|window| window == b"*/")
                .map_or(rest.len(), |end| end + 4);
            code.push(b' ');
        } else if matches!(rest[0], b'"' | b'\'') {
            let mut end = 1;
            while end < rest.len() && rest[end] != rest[0] && rest[end] != b'\n' {
                end += if rest[end] == b'\\' { 2 } else { 1 };
            }
            at += end + 1;
            code.push(b' ');
        } else {
            code.push(rest[0]);
            at += 1;
        }
    }
    code
}

fn mentions_main(file: &Path) -> bool {
    let Ok(source) = std::fs::read(file) else {
        return false;
    };
    let source = code_only(&source);
    let is_identifier = |byte: &u8| byte.is_ascii_alphanumeric() || *byte == b'_';
    source
        .windows(4)
        .enumerate()
        .filter(|(_, window)| *window == b"main")
        .any(|(at, _)| {
            let after = source[at + 4..]
                .iter()
                .find(|byte| !byte.is_ascii_whitespace());
            after == Some(&b'(')
                && !at
                    .checked_sub(1)
                    .is_some_and(|before| is_identifier(&source[before]))
        })
}

fn build_target_of(output: &str) -> Option<String> {
    let directory = Path::new(output).parent()?.file_name()?.to_str()?;
    let name = directory
        .strip_suffix(".dir")
        .or_else(|| directory.strip_suffix(".p"))
        .unwrap_or(directory);
    Some(name.to_string())
}

fn output_of<'a>(
    entry: &'a serde_json::Map<String, Value>,
    words: &'a [String],
) -> Option<&'a str> {
    if let Some(output) = entry.get("output").and_then(Value::as_str) {
        return Some(output);
    }
    words.iter().enumerate().find_map(|(index, word)| {
        if word == "-o" {
            words.get(index + 1).map(String::as_str)
        } else {
            word.strip_prefix("-o").filter(|path| !path.is_empty())
        }
    })
}

fn read_one(path: &Path, options: &ReadOptions) -> Result<Vec<Entry>, CompileCommandsError> {
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
        commands.push(Entry {
            target: output_of(entry, &words).and_then(build_target_of),
            position_independent: words
                .iter()
                .any(|word| matches!(word.as_str(), "-fPIC" | "-fpic")),
            command: CompileCommand { file, args },
        });
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
    let executable = match Path::new(compiler) {
        path if path.is_relative() && path.components().count() > 1 => {
            absolute_path(directory, path).display().to_string()
        }
        _ => compiler.clone(),
    };
    let mut args = vec![
        format!("--flavor={flavor}"),
        format!("--compiler={executable}"),
    ];
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
