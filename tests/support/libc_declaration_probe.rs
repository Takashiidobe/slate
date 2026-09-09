use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::Command;

use serde_json::Value;

use super::libc_probe::ProbeConfig;
use super::libc_shim::libc_shim_dir;

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub enum FunctionStorage {
    External,
    Static,
    Other(String),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum MacroKind {
    ObjectLike,
    FunctionLike,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct OracleMacro {
    pub header: String,
    pub name: String,
    pub kind: MacroKind,
    pub parameters: Vec<String>,
    pub replacement: String,
    pub definition_file: String,
    pub private: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct OracleFunction {
    pub header: String,
    pub name: String,
    pub type_spelling: String,
    pub variadic: bool,
    pub storage: FunctionStorage,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct OracleObject {
    pub header: String,
    pub name: String,
    pub type_spelling: String,
    pub thread_local: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct OracleTypedef {
    pub name: String,
    pub underlying_type: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct OracleField {
    pub name: String,
    pub type_spelling: String,
    pub bit_width: Option<String>,
    pub offset: Option<usize>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct OracleRecord {
    pub tag: String,
    pub is_union: bool,
    pub fields: Vec<OracleField>,
    pub size: Option<usize>,
    pub align: Option<usize>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct OracleEnumerator {
    pub name: String,
    pub type_spelling: String,
    pub value: Option<String>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct OracleEnum {
    pub tag: String,
    pub enumerators: Vec<OracleEnumerator>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct OracleTypeSurface {
    pub typedefs: Vec<OracleTypedef>,
    pub records: Vec<OracleRecord>,
    pub enums: Vec<OracleEnum>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct GeneratedProbe {
    pub source: PathBuf,
    pub object: PathBuf,
    pub executable: PathBuf,
}

fn command_output(mut command: Command, label: &str) -> Result<String, String> {
    let output = command
        .output()
        .map_err(|error| format!("{label}: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "{label} failed ({}):\nstdout:\n{}\nstderr:\n{}",
            output.status,
            String::from_utf8_lossy(&output.stdout),
            String::from_utf8_lossy(&output.stderr),
        ));
    }
    Ok(String::from_utf8_lossy(&output.stdout).into_owned())
}

fn collect_functions(
    node: &Value,
    symbol: &str,
    types: &mut BTreeSet<(String, bool, FunctionStorage)>,
    aliases: &BTreeMap<String, String>,
    is_public: &dyn Fn(&Value) -> bool,
) {
    if node.get("kind").and_then(Value::as_str) == Some("FunctionDecl")
        && is_public(node)
        && node.get("name").and_then(Value::as_str) == Some(symbol)
        && let Some(type_spelling) = node
            .pointer("/type/desugaredQualType")
            .or_else(|| node.pointer("/type/qualType"))
            .and_then(Value::as_str)
            .map(str::to_string)
    {
        let variadic = node
            .get("variadic")
            .and_then(Value::as_bool)
            .unwrap_or(false);
        let storage = match node.get("storageClass").and_then(Value::as_str) {
            Some("extern") | None => FunctionStorage::External,
            Some("static") => FunctionStorage::Static,
            Some(other) => FunctionStorage::Other(other.to_string()),
        };
        types.insert((
            canonicalize_type(&type_spelling, aliases),
            variadic,
            storage,
        ));
    }
    if let Some(children) = node.get("inner").and_then(Value::as_array) {
        for child in children {
            collect_functions(child, symbol, types, aliases, is_public);
        }
    }
}

fn declaration_file(node: &Value) -> Option<&str> {
    [
        "/loc/file",
        "/loc/spellingLoc/file",
        "/loc/expansionLoc/file",
        "/range/begin/file",
        "/range/begin/spellingLoc/file",
        "/range/begin/expansionLoc/file",
    ]
    .iter()
    .find_map(|path| node.pointer(path).and_then(Value::as_str))
}

fn is_public_header_file(file: &str, header: &str) -> bool {
    let file = file.replace('\\', "/");
    let header = header.trim_start_matches("./");
    file == header
        || file.ends_with(&format!("/{header}"))
        || file.split('/').any(|component| component == "bits")
        || header == "float.h"
            && file
                .rsplit('/')
                .next()
                .is_some_and(|name| name == "float.h" || name.starts_with("__float_"))
}

fn node_is_public_header_declaration(node: &Value, header: &str) -> bool {
    declaration_file(node).is_some_and(|file| is_public_header_file(file, header))
        || declaration_file(node).is_none()
            && node
                .pointer("/loc/includedFrom/file")
                .and_then(Value::as_str)
                .is_some_and(|file| file.ends_with("/oracle-header.c"))
            && node.get("kind").and_then(Value::as_str) == Some("VarDecl")
            && node.get("name").and_then(Value::as_str) == Some("environ")
}

fn is_public_shim_header_file(file: &str) -> bool {
    let file = file.replace('\\', "/");
    let shim_root = libc_shim_dir().to_string_lossy().replace('\\', "/");
    file.starts_with(&shim_root)
        && file
            .rsplit('/')
            .next()
            .is_some_and(|name| !name.starts_with("__"))
}

fn node_is_public_shim_declaration(node: &Value) -> bool {
    declaration_file(node).is_some_and(is_public_shim_header_file)
}

fn is_public_declaration_file(file: &str) -> bool {
    let file = file.replace('\\', "/");
    file.ends_with(".h")
        && !file.split('/').any(|component| component == "bits")
        && file
            .rsplit('/')
            .next()
            .is_some_and(|name| !name.starts_with("__"))
}

fn header_relative_name(file: &str) -> String {
    let file = file.replace('\\', "/");
    let components: Vec<&str> = file.split('/').collect();
    match components.len() {
        0 => String::new(),
        1 => components[0].to_string(),
        _ => format!(
            "{}/{}",
            components[components.len() - 2],
            components[components.len() - 1]
        ),
    }
}

fn collect_declaration_files(node: &Value, files: &mut BTreeSet<String>) {
    if let Some(file) = declaration_file(node)
        && is_public_declaration_file(file)
    {
        files.insert(header_relative_name(file));
    }
    if let Some(children) = node.get("inner").and_then(Value::as_array) {
        for child in children {
            collect_declaration_files(child, files);
        }
    }
}

pub fn extract_oracle_header_files(
    config: &ProbeConfig,
    header: &str,
    output_dir: &Path,
) -> Result<BTreeSet<String>, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let root = header_ast(config, header, &output_dir.join("oracle-header.c"))?;
    let mut files = BTreeSet::new();
    collect_declaration_files(&root, &mut files);
    Ok(files)
}

pub fn extract_shim_header_files(
    config: &ProbeConfig,
    header: &str,
    output_dir: &Path,
) -> Result<BTreeSet<String>, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let root = shim_header_ast(config, header, &output_dir.join("shim-header.c"))?;
    let mut files = BTreeSet::new();
    collect_declaration_files(&root, &mut files);
    Ok(files)
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct HeaderFileDiff {
    pub extra_in_shim: Vec<String>,
    pub missing_from_shim: Vec<String>,
}

impl HeaderFileDiff {
    pub fn is_empty(&self) -> bool {
        self.extra_in_shim.is_empty() && self.missing_from_shim.is_empty()
    }
}

pub fn diff_header_files(oracle: &BTreeSet<String>, shim: &BTreeSet<String>) -> HeaderFileDiff {
    HeaderFileDiff {
        extra_in_shim: shim.difference(oracle).cloned().collect(),
        missing_from_shim: oracle.difference(shim).cloned().collect(),
    }
}

fn collect_type_aliases(node: &Value, aliases: &mut BTreeMap<String, String>) {
    if node.get("kind").and_then(Value::as_str) == Some("TypedefDecl")
        && let (Some(name), Some(type_spelling)) = (
            node.get("name").and_then(Value::as_str),
            node.pointer("/type/desugaredQualType")
                .or_else(|| node.pointer("/type/qualType"))
                .and_then(Value::as_str),
        )
    {
        aliases.insert(name.to_string(), type_spelling.to_string());
    }
    if let Some(children) = node.get("inner").and_then(Value::as_array) {
        for child in children {
            collect_type_aliases(child, aliases);
        }
    }
}

fn type_spelling(node: &Value, aliases: &BTreeMap<String, String>) -> Option<String> {
    node.pointer("/type/desugaredQualType")
        .or_else(|| node.pointer("/type/qualType"))
        .and_then(Value::as_str)
        .map(|spelling| canonicalize_type(spelling, aliases))
}

fn constant_value(node: &Value) -> Option<String> {
    node.get("value")
        .and_then(Value::as_str)
        .map(str::to_string)
        .or_else(|| {
            node.get("inner")
                .and_then(Value::as_array)
                .and_then(|children| children.iter().find_map(constant_value))
        })
}

fn collect_type_surface(
    node: &Value,
    aliases: &BTreeMap<String, String>,
    surface: &mut OracleTypeSurface,
    is_public: &dyn Fn(&Value) -> bool,
) {
    match node.get("kind").and_then(Value::as_str) {
        Some("TypedefDecl") => {
            if is_public(node)
                && let (Some(name), Some(underlying_type)) = (
                    node.get("name").and_then(Value::as_str),
                    type_spelling(node, aliases),
                )
            {
                surface.typedefs.push(OracleTypedef {
                    name: name.to_string(),
                    underlying_type,
                });
            }
        }
        Some("RecordDecl")
            if node.get("completeDefinition").and_then(Value::as_bool) == Some(true) =>
        {
            if !is_public(node) {
                return;
            }
            let Some(tag) = node
                .get("name")
                .and_then(Value::as_str)
                .filter(|tag| !tag.is_empty())
            else {
                return;
            };
            let fields = node
                .get("inner")
                .and_then(Value::as_array)
                .into_iter()
                .flatten()
                .filter(|child| child.get("kind").and_then(Value::as_str) == Some("FieldDecl"))
                .filter_map(|field| {
                    Some(OracleField {
                        name: field.get("name")?.as_str()?.to_string(),
                        type_spelling: type_spelling(field, aliases)?,
                        bit_width: field
                            .get("isBitfield")
                            .and_then(Value::as_bool)
                            .filter(|value| *value)
                            .and_then(|_| constant_value(field)),
                        offset: None,
                    })
                })
                .collect();
            surface.records.push(OracleRecord {
                tag: tag.to_string(),
                is_union: node.get("tagUsed").and_then(Value::as_str) == Some("union"),
                fields,
                size: None,
                align: None,
            });
        }
        Some("EnumDecl") => {
            if !is_public(node) {
                return;
            }
            let Some(tag) = node
                .get("name")
                .and_then(Value::as_str)
                .filter(|tag| !tag.is_empty())
            else {
                return;
            };
            let enumerators = node
                .get("inner")
                .and_then(Value::as_array)
                .into_iter()
                .flatten()
                .filter(|child| {
                    child.get("kind").and_then(Value::as_str) == Some("EnumConstantDecl")
                })
                .filter_map(|enumerator| {
                    Some(OracleEnumerator {
                        name: enumerator.get("name")?.as_str()?.to_string(),
                        type_spelling: type_spelling(enumerator, aliases)?,
                        value: constant_value(enumerator),
                    })
                })
                .collect();
            surface.enums.push(OracleEnum {
                tag: tag.to_string(),
                enumerators,
            });
        }
        _ => {}
    }
    if let Some(children) = node.get("inner").and_then(Value::as_array) {
        for child in children {
            collect_type_surface(child, aliases, surface, is_public);
        }
    }
}

fn canonicalize_type(type_spelling: &str, aliases: &BTreeMap<String, String>) -> String {
    let mut result = String::new();
    let mut identifier = String::new();
    for character in type_spelling.chars() {
        if character.is_ascii_alphanumeric() || character == '_' {
            identifier.push(character);
        } else {
            if !identifier.is_empty() {
                if let Some(replacement) = aliases.get(&identifier).filter(|replacement| {
                    !replacement
                        .split(|character: char| {
                            !character.is_ascii_alphanumeric() && character != '_'
                        })
                        .any(|token| token == identifier.as_str())
                }) {
                    result.push_str(replacement);
                } else {
                    result.push_str(&identifier);
                }
                identifier.clear();
            }
            result.push(character);
        }
    }
    if !identifier.is_empty() {
        if let Some(replacement) = aliases.get(&identifier).filter(|replacement| {
            !replacement
                .split(|character: char| !character.is_ascii_alphanumeric() && character != '_')
                .any(|token| token == identifier.as_str())
        }) {
            result.push_str(replacement);
        } else {
            result.push_str(&identifier);
        }
    }
    result
}

fn header_ast(config: &ProbeConfig, header: &str, source: &Path) -> Result<Value, String> {
    std::fs::write(source, format!("#include <{header}>\n"))
        .map_err(|error| format!("write {}: {error}", source.display()))?;

    let mut command = Command::new(&config.compiler);
    command.args(&config.compiler_args);
    command.arg(format!("--target={}", config.target));
    command.arg(format!("--sysroot={}", config.sysroot.display()));
    command.args([
        "-std=gnu23",
        "-D_GNU_SOURCE",
        "-Xclang",
        "-ast-dump=json",
        "-fsyntax-only",
    ]);
    command.args(&config.defines);
    command.arg(source);
    let ast = command_output(command, &format!("extract oracle declaration {header}"))?;
    serde_json::from_str(&ast)
        .map_err(|error| format!("parse Clang oracle AST for {header}: {error}"))
}

fn shim_header_ast(config: &ProbeConfig, header: &str, source: &Path) -> Result<Value, String> {
    std::fs::write(source, format!("#include <{header}>\n"))
        .map_err(|error| format!("write {}: {error}", source.display()))?;

    let mut command = Command::new(&config.compiler);
    command.args(&config.compiler_args);
    command.arg(format!("--target={}", config.target));
    command.arg("-nostdlibinc");
    command.arg("-isystem").arg(libc_shim_dir());
    command.arg("-D__SLATE_LIBC_SHIM");
    command.args([
        "-std=gnu23",
        "-D_GNU_SOURCE",
        "-Xclang",
        "-ast-dump=json",
        "-fsyntax-only",
    ]);
    command.args(&config.defines);
    command.arg(source);
    let ast = command_output(command, &format!("extract shim declaration {header}"))?;
    serde_json::from_str(&ast)
        .map_err(|error| format!("parse Clang shim AST for {header}: {error}"))
}

fn record_layout_dump(config: &ProbeConfig, source: &Path) -> Result<String, String> {
    let mut command = Command::new(&config.compiler);
    command.args(&config.compiler_args);
    command.arg(format!("--target={}", config.target));
    command.arg(format!("--sysroot={}", config.sysroot.display()));
    command.args([
        "-std=gnu23",
        "-D_GNU_SOURCE",
        "-Xclang",
        "-fdump-record-layouts-complete",
        "-fsyntax-only",
    ]);
    command.args(&config.defines);
    command.arg(source);
    let output = command
        .output()
        .map_err(|error| format!("extract oracle record layouts: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "extract oracle record layouts failed: {}",
            String::from_utf8_lossy(&output.stderr)
        ));
    }
    let mut dump = String::from_utf8_lossy(&output.stdout).into_owned();
    dump.push_str(&String::from_utf8_lossy(&output.stderr));
    Ok(dump)
}

fn shim_record_layout_dump(config: &ProbeConfig, source: &Path) -> Result<String, String> {
    let mut command = Command::new(&config.compiler);
    command.args(&config.compiler_args);
    command.arg(format!("--target={}", config.target));
    command.arg("-nostdlibinc");
    command.arg("-isystem").arg(libc_shim_dir());
    command.arg("-D__SLATE_LIBC_SHIM");
    command.args([
        "-std=gnu23",
        "-D_GNU_SOURCE",
        "-Xclang",
        "-fdump-record-layouts-complete",
        "-fsyntax-only",
    ]);
    command.args(&config.defines);
    command.arg(source);
    let output = command
        .output()
        .map_err(|error| format!("extract shim record layouts: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "extract shim record layouts failed: {}",
            String::from_utf8_lossy(&output.stderr)
        ));
    }
    let mut dump = String::from_utf8_lossy(&output.stdout).into_owned();
    dump.push_str(&String::from_utf8_lossy(&output.stderr));
    Ok(dump)
}

fn merge_record_layouts(surface: &mut OracleTypeSurface, dump: &str) {
    let mut current: Option<usize> = None;
    for line in dump.lines() {
        if let Some((left, right)) = line.split_once('|') {
            let label = right.trim();
            if let Some((is_union, tag)) = label
                .strip_prefix("struct ")
                .map(|tag| (false, tag))
                .or_else(|| label.strip_prefix("union ").map(|tag| (true, tag)))
            {
                current = surface
                    .records
                    .iter()
                    .position(|record| record.is_union == is_union && record.tag == tag);
                continue;
            }
            if let Some(index) = current
                && right.starts_with("   ")
                && !right.starts_with("    ")
                && let Some(name) = label.split_whitespace().last()
                && let Ok(offset) = left.trim().split(':').next().unwrap_or_default().parse()
                && let Some(field) = surface.records[index]
                    .fields
                    .iter_mut()
                    .find(|field| field.name == name)
            {
                field.offset = Some(offset);
            }
        }
        if let Some(index) = current
            && let Some(layout) = line.trim().strip_prefix("| [sizeof=")
            && let Some((size, align)) = layout
                .strip_suffix(']')
                .and_then(|text| text.split_once(", align="))
            && let (Ok(size), Ok(align)) = (size.parse(), align.parse())
        {
            surface.records[index].size = Some(size);
            surface.records[index].align = Some(align);
            current = None;
        }
    }
}

fn header_preprocessor_output(
    config: &ProbeConfig,
    header: &str,
    source: &Path,
) -> Result<String, String> {
    std::fs::write(source, format!("#include <{header}>\n"))
        .map_err(|error| format!("write {}: {error}", source.display()))?;

    let mut command = Command::new(&config.compiler);
    command.args(&config.compiler_args);
    command.arg(format!("--target={}", config.target));
    command.arg(format!("--sysroot={}", config.sysroot.display()));
    command.args(["-std=gnu23", "-D_GNU_SOURCE", "-E", "-dD"]);
    command.args(&config.defines);
    command.arg(source);
    command_output(command, &format!("extract oracle macros {header}"))
}

fn shim_header_preprocessor_output(
    config: &ProbeConfig,
    header: &str,
    source: &Path,
) -> Result<String, String> {
    std::fs::write(source, format!("#include <{header}>\n"))
        .map_err(|error| format!("write {}: {error}", source.display()))?;

    let mut command = Command::new(&config.compiler);
    command.args(&config.compiler_args);
    command.arg(format!("--target={}", config.target));
    command.arg("-nostdlibinc");
    command.arg("-isystem").arg(libc_shim_dir());
    command.arg("-D__SLATE_LIBC_SHIM");
    command.args(["-std=gnu23", "-D_GNU_SOURCE", "-E", "-dD"]);
    command.args(&config.defines);
    command.arg(source);
    command_output(command, &format!("extract shim macros {header}"))
}

fn line_marker_path(line: &str) -> Option<String> {
    let marker = line.strip_prefix("# ")?;
    let start = marker.find('"')? + 1;
    let end = marker[start..].find('"')? + start;
    Some(marker[start..end].to_string())
}

fn macro_name_end(text: &str) -> usize {
    text.bytes()
        .take_while(|byte| byte.is_ascii_alphanumeric() || *byte == b'_')
        .count()
}

fn parse_macro_definition(
    header: &str,
    definition_file: &str,
    directive: &str,
) -> Option<OracleMacro> {
    let definition = directive.strip_prefix("#define ")?;
    let name_end = macro_name_end(definition);
    if name_end == 0 {
        return None;
    }
    let name = &definition[..name_end];
    let rest = &definition[name_end..];
    let (kind, parameters, replacement) = if let Some(rest) = rest.strip_prefix('(') {
        let end = rest.find(')')?;
        let parameters = rest[..end]
            .split(',')
            .filter_map(|parameter| {
                let parameter = parameter.trim();
                (!parameter.is_empty()).then(|| parameter.to_string())
            })
            .collect();
        (MacroKind::FunctionLike, parameters, rest[end + 1..].trim())
    } else {
        (MacroKind::ObjectLike, Vec::new(), rest.trim())
    };
    Some(OracleMacro {
        header: header.to_string(),
        name: name.to_string(),
        kind,
        parameters,
        replacement: replacement.to_string(),
        definition_file: definition_file.to_string(),
        private: name.starts_with('_'),
    })
}

fn parse_macro_directives(
    header: &str,
    output: &str,
    source: &Path,
    is_public: &dyn Fn(&str) -> bool,
) -> Result<Vec<OracleMacro>, String> {
    let mut macros = BTreeMap::new();
    let mut definition_file = String::new();
    let mut lines = output.lines();
    while let Some(line) = lines.next() {
        if let Some(path) = line_marker_path(line) {
            definition_file = path;
            continue;
        }
        if let Some(name) = line.strip_prefix("#undef ") {
            macros.remove(name.trim());
            continue;
        }
        if !line.starts_with("#define ")
            || definition_file.starts_with('<')
            || !is_public(&definition_file)
            || definition_file == source.to_string_lossy()
        {
            continue;
        }
        let mut directive = line.to_string();
        while directive.ends_with('\\') {
            let Some(continuation) = lines.next() else {
                return Err(format!(
                    "unterminated macro definition in {definition_file}"
                ));
            };
            directive.push('\n');
            directive.push_str(continuation);
        }
        if let Some(macro_definition) = parse_macro_definition(header, &definition_file, &directive)
        {
            macros.insert(macro_definition.name.clone(), macro_definition);
        }
    }
    Ok(macros.into_values().collect())
}

pub fn extract_oracle_header_macros(
    config: &ProbeConfig,
    header: &str,
    output_dir: &Path,
) -> Result<Vec<OracleMacro>, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let source = output_dir.join("oracle-macros.c");
    let output = header_preprocessor_output(config, header, &source)?;
    let raw = output_dir.join("oracle-macros.dD");
    std::fs::write(&raw, &output).map_err(|error| format!("write {}: {error}", raw.display()))?;
    parse_macro_directives(header, &output, &source, &|file| {
        is_public_header_file(file, header)
    })
}

pub fn extract_shim_header_macros(
    config: &ProbeConfig,
    header: &str,
    output_dir: &Path,
) -> Result<Vec<OracleMacro>, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let source = output_dir.join("shim-macros.c");
    let output = shim_header_preprocessor_output(config, header, &source)?;
    let raw = output_dir.join("shim-macros.dD");
    std::fs::write(&raw, &output).map_err(|error| format!("write {}: {error}", raw.display()))?;
    parse_macro_directives(header, &output, &source, &is_public_shim_header_file)
}

pub fn write_oracle_macro_manifest(
    macros: &[OracleMacro],
    output_dir: &Path,
) -> Result<PathBuf, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let entries: Vec<_> = macros
        .iter()
        .map(|macro_definition| {
            serde_json::json!({
                "header": macro_definition.header,
                "name": macro_definition.name,
                "kind": match macro_definition.kind {
                    MacroKind::ObjectLike => "object",
                    MacroKind::FunctionLike => "function",
                },
                "parameters": macro_definition.parameters,
                "replacement": macro_definition.replacement,
                "definition_file": macro_definition.definition_file,
                "private": macro_definition.private,
            })
        })
        .collect();
    let path = output_dir.join("oracle-macros.json");
    let contents = serde_json::to_string_pretty(&entries)
        .map_err(|error| format!("serialize {}: {error}", path.display()))?;
    std::fs::write(&path, format!("{contents}\n"))
        .map_err(|error| format!("write {}: {error}", path.display()))?;
    Ok(path)
}

pub fn render_header_macro_presence_probe(macros: &[OracleMacro]) -> Result<String, String> {
    let public: Vec<_> = macros
        .iter()
        .filter(|macro_definition| !macro_definition.private)
        .collect();
    let Some(header) = public
        .first()
        .map(|macro_definition| &macro_definition.header)
    else {
        return Err("oracle header has no public macros to verify".to_string());
    };
    if public
        .iter()
        .any(|macro_definition| macro_definition.header != *header)
    {
        return Err("macro probe entries must originate from one header".to_string());
    }
    let checks = public
        .iter()
        .map(|macro_definition| {
            format!(
                "#ifndef {}\n#error \"{}:{} macro is missing from libc-shim\"\n#endif",
                macro_definition.name, macro_definition.header, macro_definition.name
            )
        })
        .collect::<Vec<_>>();
    Ok(format!(
        "#include <{header}>\n\n{}\n\nint main(void) {{ return 0; }}\n",
        checks.join("\n\n")
    ))
}

pub fn write_header_macro_presence_probe(
    macros: &[OracleMacro],
    output_dir: &Path,
) -> Result<GeneratedProbe, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let source = output_dir.join("shim-macro-presence.c");
    std::fs::write(&source, render_header_macro_presence_probe(macros)?)
        .map_err(|error| format!("write {}: {error}", source.display()))?;
    Ok(GeneratedProbe {
        object: output_dir.join("shim-macro-presence.o"),
        executable: output_dir.join("shim-macro-presence"),
        source,
    })
}

fn render_object_macro_value_checks(macro_definition: &OracleMacro) -> Result<String, String> {
    if macro_definition.private {
        return Err(format!(
            "{}:{} is private and has no public value probe",
            macro_definition.header, macro_definition.name
        ));
    }
    if macro_definition.kind != MacroKind::ObjectLike {
        return Err(format!(
            "{}:{} is function-like and needs a call strategy",
            macro_definition.header, macro_definition.name
        ));
    }
    if macro_definition.replacement.is_empty() {
        return Err(format!(
            "{}:{} has no replacement tokens to compare",
            macro_definition.header, macro_definition.name
        ));
    }
    Ok(format!(
        "#ifndef {}\n#error \"{}:{} macro is missing from libc-shim\"\n#endif\n\n_Static_assert(\n    __builtin_types_compatible_p(__typeof__({}), __typeof__(({}))),\n    \"{}:{} macro type differs from oracle\");\n\n_Static_assert(\n    ({}) == ({}),\n    \"{}:{} macro value differs from oracle\");",
        macro_definition.name,
        macro_definition.header,
        macro_definition.name,
        macro_definition.name,
        macro_definition.replacement,
        macro_definition.header,
        macro_definition.name,
        macro_definition.name,
        macro_definition.replacement,
        macro_definition.header,
        macro_definition.name,
    ))
}

pub fn render_object_macro_value_probe(macro_definition: &OracleMacro) -> Result<String, String> {
    Ok(format!(
        "#include <{}>\n\n{}\n\nint main(void) {{ return 0; }}\n",
        macro_definition.header,
        render_object_macro_value_checks(macro_definition)?
    ))
}

pub fn write_object_macro_value_probe(
    macro_definition: &OracleMacro,
    output_dir: &Path,
) -> Result<GeneratedProbe, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let source = output_dir.join(format!(
        "shim-macro-{}.c",
        identifier(&macro_definition.name)
    ));
    std::fs::write(&source, render_object_macro_value_probe(macro_definition)?)
        .map_err(|error| format!("write {}: {error}", source.display()))?;
    Ok(GeneratedProbe {
        object: output_dir.join(format!(
            "shim-macro-{}.o",
            identifier(&macro_definition.name)
        )),
        executable: output_dir.join(format!("shim-macro-{}", identifier(&macro_definition.name))),
        source,
    })
}

fn oracle_compiles_source(
    config: &ProbeConfig,
    source: &Path,
    object: &Path,
) -> Result<bool, String> {
    let mut command = Command::new(&config.compiler);
    command.args(&config.compiler_args);
    command.arg(format!("--target={}", config.target));
    command.arg(format!("--sysroot={}", config.sysroot.display()));
    command.args(["-std=gnu23", "-D_GNU_SOURCE"]);
    command.args(&config.defines);
    command.arg("-c").arg(source).arg("-o").arg(object);
    let output = command
        .output()
        .map_err(|error| format!("classify oracle macro {}: {error}", source.display()))?;
    Ok(output.status.success())
}

pub fn select_oracle_object_macro_value_probes(
    config: &ProbeConfig,
    macros: &[OracleMacro],
    output_dir: &Path,
) -> Result<Vec<OracleMacro>, String> {
    let classification_dir = output_dir.join("oracle-macro-value-classification");
    std::fs::create_dir_all(&classification_dir)
        .map_err(|error| format!("create {}: {error}", classification_dir.display()))?;
    let mut selected = Vec::new();
    for macro_definition in macros {
        let Ok(source_text) = render_object_macro_value_probe(macro_definition) else {
            continue;
        };
        let stem = identifier(&macro_definition.name);
        let source = classification_dir.join(format!("{stem}.c"));
        let object = classification_dir.join(format!("{stem}.o"));
        std::fs::write(&source, source_text)
            .map_err(|error| format!("write {}: {error}", source.display()))?;
        if oracle_compiles_source(config, &source, &object)? {
            selected.push(macro_definition.clone());
        }
    }
    Ok(selected)
}

pub fn write_header_object_macro_value_probe(
    macros: &[OracleMacro],
    output_dir: &Path,
) -> Result<GeneratedProbe, String> {
    let Some(header) = macros
        .first()
        .map(|macro_definition| &macro_definition.header)
    else {
        return Err("oracle header has no object macros with value strategies".to_string());
    };
    if macros
        .iter()
        .any(|macro_definition| macro_definition.header != *header)
    {
        return Err("macro value probe entries must originate from one header".to_string());
    }
    let checks = macros
        .iter()
        .map(render_object_macro_value_checks)
        .collect::<Result<Vec<_>, _>>()?;
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let source = output_dir.join("shim-macro-values.c");
    std::fs::write(
        &source,
        format!(
            "#include <{header}>\n\n{}\n\nint main(void) {{ return 0; }}\n",
            checks.join("\n\n")
        ),
    )
    .map_err(|error| format!("write {}: {error}", source.display()))?;
    Ok(GeneratedProbe {
        object: output_dir.join("shim-macro-values.o"),
        executable: output_dir.join("shim-macro-values"),
        source,
    })
}

pub fn extract_oracle_function(
    config: &ProbeConfig,
    header: &str,
    symbol: &str,
    output_dir: &Path,
) -> Result<OracleFunction, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let root = header_ast(config, header, &output_dir.join("oracle-header.c"))?;
    let mut aliases = BTreeMap::new();
    collect_type_aliases(&root, &mut aliases);
    let mut types = BTreeSet::new();
    collect_functions(&root, symbol, &mut types, &aliases, &|node| {
        node_is_public_header_declaration(node, header)
    });
    let values: Vec<_> = types.into_iter().collect();
    match values.as_slice() {
        [(type_spelling, variadic, storage)] => Ok(OracleFunction {
            header: header.to_string(),
            name: symbol.to_string(),
            type_spelling: type_spelling.clone(),
            variadic: *variadic,
            storage: storage.clone(),
        }),
        [] => Err(format!(
            "{header}:{symbol} is not a function declaration in the oracle AST"
        )),
        _ => Err(format!(
            "{header}:{symbol} has conflicting oracle function declarations: {values:?}"
        )),
    }
}

pub fn extract_oracle_header_functions(
    config: &ProbeConfig,
    header: &str,
    output_dir: &Path,
) -> Result<Vec<OracleFunction>, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let root = header_ast(config, header, &output_dir.join("oracle-header.c"))?;
    let mut aliases = BTreeMap::new();
    collect_type_aliases(&root, &mut aliases);
    let mut by_name = BTreeMap::new();
    collect_all_functions(&root, &mut by_name, &aliases, &|node| {
        node_is_public_header_declaration(node, header)
    });
    by_name
        .into_iter()
        .map(|(name, types)| match types.as_slice() {
            [(type_spelling, variadic, storage)] => Ok(OracleFunction {
                header: header.to_string(),
                name,
                type_spelling: type_spelling.clone(),
                variadic: *variadic,
                storage: storage.clone(),
            }),
            _ => Err(format!(
                "{header}:{name} has conflicting oracle function declarations: {types:?}"
            )),
        })
        .collect()
}

pub fn extract_shim_header_functions(
    config: &ProbeConfig,
    header: &str,
    output_dir: &Path,
) -> Result<Vec<OracleFunction>, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let root = shim_header_ast(config, header, &output_dir.join("shim-header.c"))?;
    let mut aliases = BTreeMap::new();
    collect_type_aliases(&root, &mut aliases);
    let mut by_name = BTreeMap::new();
    collect_all_functions(
        &root,
        &mut by_name,
        &aliases,
        &node_is_public_shim_declaration,
    );
    by_name
        .into_iter()
        .map(|(name, types)| match types.as_slice() {
            [(type_spelling, variadic, storage)] => Ok(OracleFunction {
                header: header.to_string(),
                name,
                type_spelling: type_spelling.clone(),
                variadic: *variadic,
                storage: storage.clone(),
            }),
            _ => Err(format!(
                "{header}:{name} has conflicting shim function declarations: {types:?}"
            )),
        })
        .collect()
}

fn collect_objects(
    node: &Value,
    aliases: &BTreeMap<String, String>,
    objects: &mut BTreeMap<String, OracleObject>,
    header: &str,
    is_public: &dyn Fn(&Value) -> bool,
) {
    if node.get("kind").and_then(Value::as_str) == Some("VarDecl")
        && is_public(node)
        && node.get("storageClass").and_then(Value::as_str) == Some("extern")
        && let (Some(name), Some(type_spelling)) = (
            node.get("name").and_then(Value::as_str),
            type_spelling(node, aliases),
        )
        && !name.starts_with("__")
    {
        objects.entry(name.to_string()).or_insert(OracleObject {
            header: header.to_string(),
            name: name.to_string(),
            type_spelling,
            thread_local: node
                .get("tlsKind")
                .and_then(Value::as_str)
                .is_some_and(|kind| kind != "none"),
        });
    }
    if let Some(children) = node.get("inner").and_then(Value::as_array) {
        for child in children {
            collect_objects(child, aliases, objects, header, is_public);
        }
    }
}

pub fn extract_oracle_header_objects(
    config: &ProbeConfig,
    header: &str,
    output_dir: &Path,
) -> Result<Vec<OracleObject>, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let root = header_ast(config, header, &output_dir.join("oracle-header.c"))?;
    let mut aliases = BTreeMap::new();
    collect_type_aliases(&root, &mut aliases);
    let mut objects = BTreeMap::new();
    collect_objects(&root, &aliases, &mut objects, header, &|node| {
        node_is_public_header_declaration(node, header)
    });
    Ok(objects.into_values().collect())
}

pub fn extract_shim_header_objects(
    config: &ProbeConfig,
    header: &str,
    output_dir: &Path,
) -> Result<Vec<OracleObject>, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let root = shim_header_ast(config, header, &output_dir.join("shim-header.c"))?;
    let mut aliases = BTreeMap::new();
    collect_type_aliases(&root, &mut aliases);
    let mut objects = BTreeMap::new();
    collect_objects(
        &root,
        &aliases,
        &mut objects,
        header,
        &node_is_public_shim_declaration,
    );
    Ok(objects.into_values().collect())
}

pub fn render_object_probe(object: &OracleObject) -> Result<String, String> {
    if !simple_type(&object.type_spelling) {
        return Err(format!("{} has non-simple type", object.name));
    }
    let oracle = format!("slate_oracle_{}", identifier(&object.name));
    let tls = if object.thread_local {
        "_Thread_local "
    } else {
        ""
    };
    Ok(format!(
        "#include <{}>\n\nextern {tls}{} {oracle};\n\n_Static_assert(__builtin_types_compatible_p(__typeof__({oracle}), __typeof__({})), \"{} object type differs from oracle\");\n\nstatic __typeof__({}) *const slate_reference_{} = &{};\n\nint main(void) {{ return 0; }}\n",
        object.header,
        object.type_spelling,
        object.name,
        object.name,
        object.name,
        identifier(&object.name),
        object.name
    ))
}

pub fn write_object_probe(
    object: &OracleObject,
    output_dir: &Path,
) -> Result<GeneratedProbe, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let source = output_dir.join(format!("shim-object-{}.c", identifier(&object.name)));
    std::fs::write(&source, render_object_probe(object)?)
        .map_err(|error| format!("write {}: {error}", source.display()))?;
    Ok(GeneratedProbe {
        object: output_dir.join(format!("shim-object-{}.o", identifier(&object.name))),
        executable: output_dir.join(format!("shim-object-{}", identifier(&object.name))),
        source,
    })
}

pub fn extract_oracle_type_surface(
    config: &ProbeConfig,
    header: &str,
    output_dir: &Path,
) -> Result<OracleTypeSurface, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let root = header_ast(config, header, &output_dir.join("oracle-header.c"))?;
    let mut aliases = BTreeMap::new();
    collect_type_aliases(&root, &mut aliases);
    let mut surface = OracleTypeSurface {
        typedefs: Vec::new(),
        records: Vec::new(),
        enums: Vec::new(),
    };
    collect_type_surface(&root, &aliases, &mut surface, &|node| {
        node_is_public_header_declaration(node, header)
    });
    let dump = record_layout_dump(config, &output_dir.join("oracle-header.c"))?;
    std::fs::write(output_dir.join("oracle-record-layouts.txt"), &dump)
        .map_err(|error| format!("write record layouts: {error}"))?;
    merge_record_layouts(&mut surface, &dump);
    surface
        .typedefs
        .sort_by(|left, right| left.name.cmp(&right.name));
    surface
        .records
        .sort_by(|left, right| left.tag.cmp(&right.tag));
    surface
        .enums
        .sort_by(|left, right| left.tag.cmp(&right.tag));
    Ok(surface)
}

pub fn extract_shim_type_surface(
    config: &ProbeConfig,
    header: &str,
    output_dir: &Path,
) -> Result<OracleTypeSurface, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let root = shim_header_ast(config, header, &output_dir.join("shim-header.c"))?;
    let mut aliases = BTreeMap::new();
    collect_type_aliases(&root, &mut aliases);
    let mut surface = OracleTypeSurface {
        typedefs: Vec::new(),
        records: Vec::new(),
        enums: Vec::new(),
    };
    collect_type_surface(
        &root,
        &aliases,
        &mut surface,
        &node_is_public_shim_declaration,
    );
    let dump = shim_record_layout_dump(config, &output_dir.join("shim-header.c"))?;
    std::fs::write(output_dir.join("shim-record-layouts.txt"), &dump)
        .map_err(|error| format!("write record layouts: {error}"))?;
    merge_record_layouts(&mut surface, &dump);
    surface
        .typedefs
        .sort_by(|left, right| left.name.cmp(&right.name));
    surface
        .records
        .sort_by(|left, right| left.tag.cmp(&right.tag));
    surface
        .enums
        .sort_by(|left, right| left.tag.cmp(&right.tag));
    Ok(surface)
}

pub fn write_oracle_type_manifest(
    surface: &OracleTypeSurface,
    output_dir: &Path,
) -> Result<PathBuf, String> {
    let path = output_dir.join("oracle-types.json");
    let contents = serde_json::json!({
        "typedefs": surface.typedefs.iter().map(|typedef| serde_json::json!({ "name": typedef.name, "underlying_type": typedef.underlying_type })).collect::<Vec<_>>(),
        "records": surface.records.iter().map(|record| serde_json::json!({ "tag": record.tag, "kind": if record.is_union { "union" } else { "struct" }, "size": record.size, "align": record.align, "fields": record.fields.iter().map(|field| serde_json::json!({ "name": field.name, "type": field.type_spelling, "bit_width": field.bit_width, "offset": field.offset })).collect::<Vec<_>>() })).collect::<Vec<_>>(),
        "enums": surface.enums.iter().map(|enumeration| serde_json::json!({ "tag": enumeration.tag, "enumerators": enumeration.enumerators.iter().map(|enumerator| serde_json::json!({ "name": enumerator.name, "type": enumerator.type_spelling, "value": enumerator.value })).collect::<Vec<_>>() })).collect::<Vec<_>>(),
    });
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    std::fs::write(
        &path,
        format!(
            "{}\n",
            serde_json::to_string_pretty(&contents).expect("serialize type manifest")
        ),
    )
    .map_err(|error| format!("write {}: {error}", path.display()))?;
    Ok(path)
}

fn simple_type(type_spelling: &str) -> bool {
    !type_spelling.contains(['(', '['])
}

pub fn render_type_surface_probe(
    header: &str,
    surface: &OracleTypeSurface,
) -> Result<String, String> {
    let mut checks = Vec::new();
    for typedef in &surface.typedefs {
        if !typedef.name.starts_with('_') && simple_type(&typedef.underlying_type) {
            let oracle_name = format!("slate_oracle_typedef_{}", identifier(&typedef.name));
            checks.push(format!(
                "typedef {} {oracle_name};\n_Static_assert(__builtin_types_compatible_p({oracle_name}, {}), \"typedef {} differs from oracle\");",
                typedef.underlying_type, typedef.name, typedef.name
            ));
        }
    }
    for record in &surface.records {
        if record.tag.starts_with('_') {
            continue;
        }
        let tag = if record.is_union { "union" } else { "struct" };
        if let Some(size) = record.size {
            checks.push(format!("_Static_assert(sizeof({tag} {}) == {size}, \"{tag} {} size differs from oracle\");", record.tag, record.tag));
        }
        if let Some(align) = record.align {
            checks.push(format!("_Static_assert(_Alignof({tag} {}) == {align}, \"{tag} {} alignment differs from oracle\");", record.tag, record.tag));
        }
        for field in &record.fields {
            if let Some(offset) = field.offset.filter(|_| field.bit_width.is_none()) {
                checks.push(format!("_Static_assert(__builtin_offsetof({tag} {}, {}) == {offset}, \"{tag} {}.{} offset differs from oracle\");", record.tag, field.name, record.tag, field.name));
            }
            if simple_type(&field.type_spelling) && field.bit_width.is_none() {
                let oracle_name = format!(
                    "slate_oracle_{}_{}_{}",
                    if record.is_union { "union" } else { "struct" },
                    identifier(&record.tag),
                    identifier(&field.name)
                );
                checks.push(format!(
                    "typedef {} {oracle_name};\n_Static_assert(__builtin_types_compatible_p(__typeof__((( {tag} {} *)0)->{}), {oracle_name}), \"{tag} {}.{} field type differs from oracle\");",
                    field.type_spelling, record.tag, field.name, record.tag, field.name
                ));
            }
        }
    }
    for enumeration in &surface.enums {
        if enumeration.tag.starts_with('_') {
            continue;
        }
        for enumerator in &enumeration.enumerators {
            if enumerator.name.starts_with('_') {
                continue;
            }
            if simple_type(&enumerator.type_spelling) {
                checks.push(format!(
                    "_Static_assert(__builtin_types_compatible_p(__typeof__({}), __typeof__(({})0)), \"enum {} type differs from oracle\");",
                    enumerator.name, enumerator.type_spelling, enumerator.name
                ));
            }
            if let Some(value) = &enumerator.value {
                checks.push(format!(
                    "_Static_assert({} == ({}), \"enum {} value differs from oracle\");",
                    enumerator.name, value, enumerator.name
                ));
            }
        }
    }
    if checks.is_empty() {
        return Err(format!("{header} has no safe type-surface checks"));
    }
    Ok(format!(
        "#include <{header}>\n\n{}\n\nint main(void) {{ return 0; }}\n",
        checks.join("\n\n")
    ))
}

pub fn write_type_surface_probe(
    header: &str,
    surface: &OracleTypeSurface,
    output_dir: &Path,
) -> Result<GeneratedProbe, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let source = output_dir.join("shim-type-surface.c");
    std::fs::write(&source, render_type_surface_probe(header, surface)?)
        .map_err(|error| format!("write {}: {error}", source.display()))?;
    Ok(GeneratedProbe {
        object: output_dir.join("shim-type-surface.o"),
        executable: output_dir.join("shim-type-surface"),
        source,
    })
}

fn probe_body(source: String, header: &str) -> String {
    let prefix = format!("#include <{header}>\n\n");
    source
        .strip_prefix(&prefix)
        .unwrap_or(&source)
        .strip_suffix("\n\nint main(void) { return 0; }\n")
        .unwrap_or(&source)
        .to_string()
}

pub fn write_header_matrix_probe(
    header: &str,
    functions: &[OracleFunction],
    objects: &[OracleObject],
    surface: &OracleTypeSurface,
    macros: &[OracleMacro],
    output_dir: &Path,
) -> Result<GeneratedProbe, String> {
    let mut bodies = Vec::new();
    let public_functions: Vec<_> = functions
        .iter()
        .filter(|function| !function.name.starts_with('_'))
        .filter(|function| !(function.header == "alloca.h" && function.name == "alloca"))
        .cloned()
        .collect();
    if !public_functions.is_empty() {
        bodies.push(probe_body(
            render_header_shim_probe(&public_functions)?,
            header,
        ));
    }
    for object in objects {
        if !object.name.starts_with('_')
            && let Ok(source) = render_object_probe(object)
        {
            bodies.push(probe_body(source, header));
        }
    }
    if let Ok(source) = render_type_surface_probe(header, surface) {
        bodies.push(probe_body(source, header));
    }
    if let Ok(source) = render_header_macro_presence_probe(macros) {
        bodies.push(probe_body(source, header));
    }
    if bodies.is_empty() {
        return Err(format!("{header} has no matrix probe strategy"));
    }
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let source = output_dir.join("shim-header-matrix.c");
    std::fs::write(
        &source,
        format!(
            "#include <{header}>\n\n{}\n\nint main(void) {{ return 0; }}\n",
            bodies.join("\n\n")
        ),
    )
    .map_err(|error| format!("write {}: {error}", source.display()))?;
    Ok(GeneratedProbe {
        object: output_dir.join("shim-header-matrix.o"),
        executable: output_dir.join("shim-header-matrix"),
        source,
    })
}

fn collect_all_functions(
    node: &Value,
    functions: &mut BTreeMap<String, Vec<(String, bool, FunctionStorage)>>,
    aliases: &BTreeMap<String, String>,
    is_public: &dyn Fn(&Value) -> bool,
) {
    if node.get("kind").and_then(Value::as_str) == Some("FunctionDecl")
        && is_public(node)
        && let (Some(name), Some(type_spelling)) = (
            node.get("name").and_then(Value::as_str),
            node.pointer("/type/desugaredQualType")
                .or_else(|| node.pointer("/type/qualType"))
                .and_then(Value::as_str),
        )
    {
        let variadic = node
            .get("variadic")
            .and_then(Value::as_bool)
            .unwrap_or(false);
        let storage = match node.get("storageClass").and_then(Value::as_str) {
            Some("extern") | None => FunctionStorage::External,
            Some("static") => FunctionStorage::Static,
            Some(other) => FunctionStorage::Other(other.to_string()),
        };
        let entry = functions.entry(name.to_string()).or_default();
        let value = (canonicalize_type(type_spelling, aliases), variadic, storage);
        if !entry.contains(&value) {
            entry.push(value);
        }
    }
    if let Some(children) = node.get("inner").and_then(Value::as_array) {
        for child in children {
            collect_all_functions(child, functions, aliases, is_public);
        }
    }
}

fn identifier(name: &str) -> String {
    name.chars()
        .map(|character| {
            if character.is_ascii_alphanumeric() || character == '_' {
                character
            } else {
                '_'
            }
        })
        .collect()
}

fn renamed_declaration(function: &OracleFunction) -> Result<(String, String), String> {
    let Some((result, parameters)) = function.type_spelling.split_once('(') else {
        return Err(format!(
            "cannot render oracle function type for {}: {:?}",
            function.name, function.type_spelling
        ));
    };
    let oracle_name = format!("slate_oracle_{}", identifier(&function.name));
    let declaration = if let Some(pointer_parameters) = parameters.strip_prefix("*(") {
        format!(
            "extern {} (*{}({pointer_parameters};",
            result.trim_end(),
            oracle_name
        )
    } else {
        format!("extern {} {}({parameters};", result.trim_end(), oracle_name)
    };
    Ok((oracle_name, declaration))
}

pub fn render_oracle_declaration(function: &OracleFunction) -> Result<String, String> {
    renamed_declaration(function).map(|(_, declaration)| declaration)
}

pub fn write_oracle_declarations(
    functions: &[OracleFunction],
    output_dir: &Path,
) -> Result<PathBuf, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let declarations = functions
        .iter()
        .filter(|function| function.storage == FunctionStorage::External)
        .map(render_oracle_declaration)
        .collect::<Result<Vec<_>, _>>()?;
    let path = output_dir.join("oracle-declarations.h");
    std::fs::write(&path, format!("{}\n", declarations.join("\n")))
        .map_err(|error| format!("write {}: {error}", path.display()))?;
    Ok(path)
}

pub fn render_shim_probe(function: &OracleFunction) -> Result<String, String> {
    let (oracle_name, declaration) = renamed_declaration(function)?;
    let reference = format!("slate_reference_{}", identifier(&function.name));
    Ok(format!(
        "#include <{}>\n\n{}\n\n_Static_assert(\n    __builtin_types_compatible_p(__typeof__({}), __typeof__({})),\n    \"{}:{} declaration differs from oracle\");\n\nstatic __typeof__({}) *const {} = &{};\n\nint main(void) {{ return {} == 0; }}\n",
        function.header,
        declaration,
        oracle_name,
        function.name,
        function.header,
        function.name,
        function.name,
        reference,
        function.name,
        reference,
    ))
}

pub fn render_header_shim_probe(functions: &[OracleFunction]) -> Result<String, String> {
    let external: Vec<_> = functions
        .iter()
        .filter(|function| function.storage == FunctionStorage::External)
        .collect();
    let Some(header) = external.first().map(|function| &function.header) else {
        return Err("oracle header has no externally linked functions to verify".to_string());
    };
    if external.iter().any(|function| function.header != *header) {
        return Err("header probe functions must originate from one header".to_string());
    }
    let mut declarations = Vec::new();
    let mut assertions = Vec::new();
    let mut references = Vec::new();
    for function in external {
        let (oracle_name, declaration) = renamed_declaration(function)?;
        let reference = format!("slate_reference_{}", identifier(&function.name));
        declarations.push(declaration);
        assertions.push(format!(
            "_Static_assert(\n    __builtin_types_compatible_p(__typeof__({oracle_name}), __typeof__({})),\n    \"{}:{} declaration differs from oracle\");",
            function.name, function.header, function.name
        ));
        references.push(format!(
            "static __typeof__({}) *const {reference} = &{};",
            function.name, function.name
        ));
    }
    Ok(format!(
        "#include <{header}>\n\n{}\n\n{}\n\n{}\n\nint main(void) {{ return 0; }}\n",
        declarations.join("\n"),
        assertions.join("\n\n"),
        references.join("\n"),
    ))
}

pub fn write_header_shim_probe(
    functions: &[OracleFunction],
    output_dir: &Path,
) -> Result<GeneratedProbe, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let source = output_dir.join("shim-header-compatibility.c");
    std::fs::write(&source, render_header_shim_probe(functions)?)
        .map_err(|error| format!("write {}: {error}", source.display()))?;
    Ok(GeneratedProbe {
        object: output_dir.join("shim-header-compatibility.o"),
        executable: output_dir.join("shim-header-compatibility"),
        source,
    })
}

pub fn write_shim_probe(
    function: &OracleFunction,
    output_dir: &Path,
) -> Result<GeneratedProbe, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let source = output_dir.join("shim-compatibility.c");
    std::fs::write(&source, render_shim_probe(function)?)
        .map_err(|error| format!("write {}: {error}", source.display()))?;
    Ok(GeneratedProbe {
        object: output_dir.join("shim-compatibility.o"),
        executable: output_dir.join("shim-compatibility"),
        source,
    })
}

pub fn compile_and_link_shim_probe(
    config: &ProbeConfig,
    probe: &GeneratedProbe,
) -> Result<(), String> {
    let mut compile = Command::new(&config.compiler);
    compile.args(&config.compiler_args);
    compile.arg(format!("--target={}", config.target));
    compile.arg(format!("--sysroot={}", config.sysroot.display()));
    compile.args([
        "-std=gnu23",
        "-D_GNU_SOURCE",
        "-Werror=implicit-function-declaration",
    ]);
    compile.arg("-nostdlibinc");
    compile.arg("-isystem").arg(libc_shim_dir());
    compile.arg("-D__SLATE_LIBC_SHIM");
    compile.args(&config.defines);
    compile
        .arg("-c")
        .arg(&probe.source)
        .arg("-o")
        .arg(&probe.object);
    command_output(
        compile,
        &format!("compile shim declaration probe {}", probe.source.display()),
    )?;

    let mut link = Command::new(&config.linker);
    link.args(&config.linker_args);
    link.arg(&probe.object);
    link.args(&config.linker_post_args);
    link.arg("-o").arg(&probe.executable);
    command_output(
        link,
        &format!("link shim declaration probe {}", probe.source.display()),
    )?;
    Ok(())
}

pub fn compile_and_link_oracle_probe(
    config: &ProbeConfig,
    probe: &GeneratedProbe,
) -> Result<(), String> {
    let mut compile = Command::new(&config.compiler);
    compile.args(&config.compiler_args);
    compile.arg(format!("--target={}", config.target));
    compile.arg(format!("--sysroot={}", config.sysroot.display()));
    compile.args([
        "-std=gnu23",
        "-D_GNU_SOURCE",
        "-Werror=implicit-function-declaration",
    ]);
    compile.args(&config.defines);
    compile
        .arg("-c")
        .arg(&probe.source)
        .arg("-o")
        .arg(&probe.object);
    command_output(
        compile,
        &format!(
            "compile oracle declaration probe {}",
            probe.source.display()
        ),
    )?;

    let mut link = Command::new(&config.linker);
    link.args(&config.linker_args);
    link.arg(&probe.object);
    link.args(&config.linker_post_args);
    link.arg("-o").arg(&probe.executable);
    command_output(
        link,
        &format!("link oracle declaration probe {}", probe.source.display()),
    )?;
    Ok(())
}
