use crate::backend::{self, rust_ast};
use crate::frontend;
use crate::project::matrix::Matrix;
use crate::project::merge::{self, Projection};
use std::collections::BTreeSet;
use std::path::{Path, PathBuf};
use thiserror::Error as ThisError;

#[derive(Debug, ThisError, miette::Diagnostic)]
pub enum Error {
    #[error("{0}")]
    #[diagnostic(transparent)]
    SlateParser(#[from] frontend::Error),
    #[error("read {path}: {source}")]
    Read {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("format generated Rust: {message}")]
    Format { message: String },
    #[error("{0}")]
    Merge(String),
}

pub fn slate_ir_with_args(
    path: &Path,
    compiler_args: &[String],
) -> Result<(slate_parser::ir::Module, slate_parser::files::Files), Error> {
    frontend::parse_module_with_args(path, compiler_args).map_err(Error::from)
}

fn single_file_options(path: &Path) -> frontend::lowerer::LowerOptions {
    frontend::lowerer::LowerOptions {
        unit: path
            .file_stem()
            .and_then(|stem| stem.to_str())
            .map(merge::rust_ident)
            .unwrap_or_default(),
        ..Default::default()
    }
}

pub fn translate(path: &Path) -> Result<String, Error> {
    translate_with_args(path, &[])
}

pub fn translate_with_args(path: &Path, extra_args: &[String]) -> Result<String, Error> {
    let (module, files, diagnostics) = frontend::parse_module_with_source(path, None, extra_args)?;
    let mut program =
        frontend::lower_single_module(&module, &files, &single_file_options(path), "main")?;
    insert_after_crate_attrs(
        &mut program,
        diagnostics
            .iter()
            .enumerate()
            .flat_map(|(index, diagnostic)| match diagnostic.severity {
                miette::Severity::Warning => warning_items(&diagnostic.text, index),
                _ => vec![rust_ast::Item::Macro {
                    name: "compile_error".into(),
                    args: vec![rust_ast::Expr::Str(diagnostic.text.clone())],
                }],
            })
            .collect(),
    );
    format_program(backend::apply_with_target(program, &module.target))
}

pub fn translate_targets_with_args(
    path: &Path,
    extra_args: &[String],
    targets: &[String],
) -> Result<String, Error> {
    match dedup(targets).as_slice() {
        [] => translate_with_args(path, extra_args),
        [target] => translate_with_args(path, &with_target(extra_args, target)),
        targets => merge_targets(path, extra_args, targets),
    }
}

pub fn translate_with_diagnostics(
    path: &Path,
    extra_args: &[String],
    targets: &[String],
) -> (Result<String, Error>, Vec<serde_json::Value>) {
    let targets = dedup(targets);
    let mut diagnostics = Vec::new();
    let result = match targets.as_slice() {
        [] => checked_program(path, extra_args, None, &mut diagnostics).and_then(format_program),
        [target] => checked_program(
            path,
            &with_target(extra_args, target),
            Some(target),
            &mut diagnostics,
        )
        .and_then(format_program),
        _ => targets
            .iter()
            .map(|target| {
                checked_program(
                    path,
                    &with_target(extra_args, target),
                    Some(target),
                    &mut diagnostics,
                )
            })
            .collect::<Vec<_>>()
            .into_iter()
            .find_map(Result::err)
            .map_or_else(|| merge_targets(path, extra_args, &targets), Err),
    };
    if let Err(error) = &result
        && !diagnostics
            .iter()
            .any(|diagnostic| diagnostic["severity"] == "error")
    {
        diagnostics.push(crate::diagnostics::plain(&error.to_string()));
    }
    let mut seen = BTreeSet::new();
    diagnostics.retain(|diagnostic| seen.insert(diagnostic.to_string()));
    (result, diagnostics)
}

fn checked_program(
    path: &Path,
    args: &[String],
    target: Option<&String>,
    diagnostics: &mut Vec<serde_json::Value>,
) -> Result<rust_ast::Program, Error> {
    let start = diagnostics.len();
    let result = (|| {
        let (module, files, directives) =
            frontend::parse_module_with_diagnostics(path, None, args, diagnostics)?;
        frontend::reject_directive_errors(path, &directives)?;
        let program =
            frontend::lower_single_module(&module, &files, &single_file_options(path), "main")
                .inspect_err(|error| {
                    diagnostics.push(crate::diagnostics::frontend(error, &files))
                })?;
        Ok(backend::apply_with_target(program, &module.target))
    })();
    for diagnostic in &mut diagnostics[start..] {
        diagnostic["target"] = serde_json::json!(target);
    }
    result
}

fn merge_targets(path: &Path, extra_args: &[String], targets: &[String]) -> Result<String, Error> {
    let source = std::path::absolute(path).map_err(|source| Error::Read {
        path: path.to_path_buf(),
        source,
    })?;
    let mut flavor = "clang".to_string();
    let args = extra_args
        .iter()
        .filter(|arg| match arg.strip_prefix("--flavor=") {
            Some(value) => {
                value.clone_into(&mut flavor);
                false
            }
            None => true,
        })
        .cloned()
        .collect::<Vec<_>>();
    let matrix = Matrix::single_source(&source, &args, &flavor, targets)
        .map_err(|error| Error::Merge(error.to_string()))?;
    let merged = merge::merge(&matrix, &[], Projection::Skip, &[]).map_err(Error::Merge)?;
    let mut program = merged
        .units
        .into_iter()
        .next()
        .map(|(_, program)| program)
        .unwrap_or_default();
    let mut prologue = vec![merged.target_guard];
    prologue.extend(
        merged
            .shared_macros
            .into_iter()
            .flat_map(|macros| macros.items),
    );
    insert_after_crate_attrs(&mut program, prologue);
    backend::pretty_rust(&program.emit()).map_err(|message| Error::Format { message })
}

fn format_program(mut program: rust_ast::Program) -> Result<String, Error> {
    backend::assign_template_names(std::iter::once(&mut program));
    backend::pretty_rust(&program.emit()).map_err(|message| Error::Format { message })
}

fn dedup(targets: &[String]) -> Vec<String> {
    let mut seen = BTreeSet::new();
    targets
        .iter()
        .filter(|target| seen.insert(target.as_str()))
        .cloned()
        .collect()
}

fn with_target(args: &[String], target: &str) -> Vec<String> {
    let mut args = args.to_vec();
    args.push(format!("--target={target}"));
    args
}

fn warning_items(message: &str, index: usize) -> Vec<rust_ast::Item> {
    vec![
        rust_ast::Item::Const {
            attrs: vec![rust_ast::Attr::Deprecated(Some(message.into()))],
            name: format!("__SLATE_WARNING_{index}"),
            ty: rust_ast::Type::Unit,
            init: rust_ast::Expr::Block(Box::default()),
        },
        rust_ast::Item::Const {
            attrs: Vec::new(),
            name: "_".into(),
            ty: rust_ast::Type::Unit,
            init: rust_ast::Expr::Var(format!("__SLATE_WARNING_{index}").into()),
        },
    ]
}

fn insert_after_crate_attrs(program: &mut rust_ast::Program, items: Vec<rust_ast::Item>) {
    let index = program
        .items
        .iter()
        .take_while(|item| matches!(item, rust_ast::Item::CrateAttrs(_)))
        .count();
    program.items.splice(index..index, items);
}
