use serde_json::Value;
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
    #[error("compile commands {path} entry {index} option {option} requires a value")]
    MissingOptionValue {
        path: PathBuf,
        index: usize,
        option: String,
    },
    #[error("compile command inputs contain no C translation units")]
    NoCTranslationUnits { paths: Vec<PathBuf> },
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub struct CompileCommand {
    pub file: PathBuf,
    pub args: Vec<String>,
    pub target: String,
}

pub fn read(paths: &[PathBuf]) -> Result<Vec<CompileCommand>, CompileCommandsError> {
    let mut commands = Vec::new();
    for path in paths {
        commands.extend(read_one(path)?);
    }
    commands.sort();
    commands.dedup();
    if commands.is_empty() {
        return Err(CompileCommandsError::NoCTranslationUnits {
            paths: paths.to_vec(),
        });
    }
    Ok(commands)
}

fn read_one(path: &Path) -> Result<Vec<CompileCommand>, CompileCommandsError> {
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
        let (args, target) = normalize(words, &directory, &file, path, index)?;
        commands.push(CompileCommand { file, args, target });
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

fn absolute_path(base: &Path, path: &Path) -> PathBuf {
    let path = if path.is_absolute() {
        path.to_path_buf()
    } else {
        base.join(path)
    };
    path.canonicalize().unwrap_or(path)
}

fn normalize(
    words: Vec<String>,
    directory: &Path,
    file: &Path,
    database: &Path,
    index: usize,
) -> Result<(Vec<String>, String), CompileCommandsError> {
    let compiler = words
        .first()
        .ok_or_else(|| CompileCommandsError::EmptyCommand {
            path: database.to_path_buf(),
            index,
        })?;
    let mut args = Vec::new();
    let mut target = compiler_target(compiler);
    let mut word_index = 1;
    while word_index < words.len() {
        let word = &words[word_index];
        if matches!(
            word.as_str(),
            "-c" | "-MD" | "-MMD" | "-MP" | "-MG" | "-M" | "-MM"
        ) {
            word_index += 1;
            continue;
        }
        if matches!(word.as_str(), "-o" | "-MF" | "-MT" | "-MQ" | "-MJ") {
            if word_index + 1 >= words.len() {
                return Err(CompileCommandsError::MissingOptionValue {
                    path: database.to_path_buf(),
                    index,
                    option: word.clone(),
                });
            }
            word_index += 2;
            continue;
        }
        if ["-o", "-MF", "-MT", "-MQ", "-MJ"]
            .iter()
            .any(|prefix| word.starts_with(prefix) && word.len() > prefix.len())
        {
            word_index += 1;
            continue;
        }
        if matches!(
            word.as_str(),
            "-I" | "-isystem"
                | "-iquote"
                | "-idirafter"
                | "-include"
                | "-imacros"
                | "-isysroot"
                | "--sysroot"
        ) {
            let value = words.get(word_index + 1).ok_or_else(|| {
                CompileCommandsError::MissingOptionValue {
                    path: database.to_path_buf(),
                    index,
                    option: word.clone(),
                }
            })?;
            args.push(word.clone());
            args.push(
                absolute_path(directory, Path::new(value))
                    .to_string_lossy()
                    .into_owned(),
            );
            word_index += 2;
            continue;
        }
        if let Some(value) = word.strip_prefix("-I").filter(|value| !value.is_empty()) {
            args.push(format!(
                "-I{}",
                absolute_path(directory, Path::new(value)).display()
            ));
            word_index += 1;
            continue;
        }
        if let Some(value) = word.strip_prefix("--sysroot=") {
            args.push(format!(
                "--sysroot={}",
                absolute_path(directory, Path::new(value)).display()
            ));
            word_index += 1;
            continue;
        }
        if matches!(word.as_str(), "-target" | "--target") {
            let value = words.get(word_index + 1).ok_or_else(|| {
                CompileCommandsError::MissingOptionValue {
                    path: database.to_path_buf(),
                    index,
                    option: word.clone(),
                }
            })?;
            target = Some(value.clone());
            word_index += 2;
            continue;
        }
        if let Some(value) = word
            .strip_prefix("--target=")
            .or_else(|| word.strip_prefix("-target="))
        {
            target = Some(value.to_string());
            word_index += 1;
            continue;
        }
        if !word.starts_with('-') && absolute_path(directory, Path::new(word)) == file {
            word_index += 1;
            continue;
        }
        args.push(word.clone());
        word_index += 1;
    }
    Ok((
        args,
        target.unwrap_or_else(crate::frontend::toolchain::active_target),
    ))
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
