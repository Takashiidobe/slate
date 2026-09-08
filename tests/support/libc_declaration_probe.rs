use std::collections::BTreeSet;
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
) {
    if node.get("kind").and_then(Value::as_str) == Some("FunctionDecl")
        && node.get("name").and_then(Value::as_str) == Some(symbol)
        && let Some(type_spelling) = node
            .pointer("/type/qualType")
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
        types.insert((type_spelling, variadic, storage));
    }
    if let Some(children) = node.get("inner").and_then(Value::as_array) {
        for child in children {
            collect_functions(child, symbol, types);
        }
    }
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

pub fn extract_oracle_function(
    config: &ProbeConfig,
    header: &str,
    symbol: &str,
    output_dir: &Path,
) -> Result<OracleFunction, String> {
    std::fs::create_dir_all(output_dir)
        .map_err(|error| format!("create {}: {error}", output_dir.display()))?;
    let root = header_ast(config, header, &output_dir.join("oracle-header.c"))?;
    let mut types = BTreeSet::new();
    collect_functions(&root, symbol, &mut types);
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
    Ok((
        oracle_name.clone(),
        format!("extern {} {}({parameters};", result.trim_end(), oracle_name),
    ))
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
