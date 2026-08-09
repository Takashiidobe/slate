use serde_json::Value;
use std::path::{Path, PathBuf};

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub struct CompileCommand {
    pub file: PathBuf,
    pub args: Vec<String>,
    pub target: String,
}

pub fn read(paths: &[PathBuf]) -> Result<Vec<CompileCommand>, String> {
    let mut commands = Vec::new();
    for path in paths {
        commands.extend(read_one(path)?);
    }
    commands.sort();
    commands.dedup();
    if commands.is_empty() {
        return Err("compile command inputs contain no C translation units".into());
    }
    Ok(commands)
}

fn read_one(path: &Path) -> Result<Vec<CompileCommand>, String> {
    let bytes = std::fs::read(path)
        .map_err(|error| format!("read compile commands {}: {error}", path.display()))?;
    let entries: Value = serde_json::from_slice(&bytes)
        .map_err(|error| format!("parse compile commands {}: {error}", path.display()))?;
    let entries = entries.as_array().ok_or_else(|| {
        format!(
            "compile commands {} must contain a JSON array",
            path.display()
        )
    })?;
    let database_dir = path.parent().unwrap_or_else(|| Path::new("."));
    let mut commands = Vec::new();
    for (index, entry) in entries.iter().enumerate() {
        let entry = entry.as_object().ok_or_else(|| {
            format!(
                "compile commands {} entry {} must be an object",
                path.display(),
                index
            )
        })?;
        let directory = string_field(entry, "directory", path, index)?;
        let directory = absolute_path(database_dir, Path::new(directory));
        let file = string_field(entry, "file", path, index)?;
        let file = absolute_path(&directory, Path::new(file));
        if file.extension().and_then(|extension| extension.to_str()) != Some("c") {
            continue;
        }
        let words = match entry.get("arguments") {
            Some(Value::Array(arguments)) => arguments
                .iter()
                .map(|argument| {
                    argument.as_str().map(str::to_string).ok_or_else(|| {
                        format!(
                            "compile commands {} entry {} has a non-string argument",
                            path.display(),
                            index
                        )
                    })
                })
                .collect::<Result<Vec<_>, _>>()?,
            Some(_) => {
                return Err(format!(
                    "compile commands {} entry {} field arguments must be an array",
                    path.display(),
                    index
                ));
            }
            None => {
                let command = string_field(entry, "command", path, index)?;
                shlex::split(command).ok_or_else(|| {
                    format!(
                        "compile commands {} entry {} has invalid shell quoting",
                        path.display(),
                        index
                    )
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
    field: &str,
    path: &Path,
    index: usize,
) -> Result<&'a str, String> {
    entry.get(field).and_then(Value::as_str).ok_or_else(|| {
        format!(
            "compile commands {} entry {} requires string field {field}",
            path.display(),
            index
        )
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
) -> Result<(Vec<String>, String), String> {
    let compiler = words.first().ok_or_else(|| {
        format!(
            "compile commands {} entry {} has an empty command",
            database.display(),
            index
        )
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
                return Err(format!(
                    "compile commands {} entry {} option {word} requires a value",
                    database.display(),
                    index
                ));
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
                format!(
                    "compile commands {} entry {} option {word} requires a value",
                    database.display(),
                    index
                )
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
                format!(
                    "compile commands {} entry {} option {word} requires a value",
                    database.display(),
                    index
                )
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
    Ok((args, target.unwrap_or_else(crate::cir::emit::active_target)))
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
