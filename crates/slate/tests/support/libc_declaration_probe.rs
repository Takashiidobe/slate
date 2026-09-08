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
) {
    if node.get("kind").and_then(Value::as_str) == Some("FunctionDecl")
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
            collect_functions(child, symbol, types, aliases);
        }
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

fn canonicalize_type(type_spelling: &str, aliases: &BTreeMap<String, String>) -> String {
    let mut result = String::new();
    let mut identifier = String::new();
    for character in type_spelling.chars() {
        if character.is_ascii_alphanumeric() || character == '_' {
            identifier.push(character);
        } else {
            if !identifier.is_empty() {
                result.push_str(aliases.get(&identifier).map_or(&identifier, String::as_str));
                identifier.clear();
            }
            result.push(character);
        }
    }
    if !identifier.is_empty() {
        result.push_str(aliases.get(&identifier).map_or(&identifier, String::as_str));
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
        private: name.starts_with("__"),
    })
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
    collect_functions(&root, symbol, &mut types, &aliases);
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
    collect_all_functions(&root, &mut by_name, &aliases);
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

fn collect_all_functions(
    node: &Value,
    functions: &mut BTreeMap<String, Vec<(String, bool, FunctionStorage)>>,
    aliases: &BTreeMap<String, String>,
) {
    if node.get("kind").and_then(Value::as_str) == Some("FunctionDecl")
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
            collect_all_functions(child, functions, aliases);
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
