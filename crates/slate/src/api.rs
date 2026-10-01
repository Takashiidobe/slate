use crate::backend::{self, rust_ast};
use crate::frontend::{self, directive_translate, preprocess};
use std::path::{Path, PathBuf};
use thiserror::Error as ThisError;

#[derive(Debug, ThisError)]
pub enum Error {
    #[error(transparent)]
    SlateParser(#[from] frontend::Error),
    #[error("read {path}: {source}")]
    Read {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("format generated Rust: {message}")]
    Format { message: String },
    #[error(transparent)]
    Directive(#[from] directive_translate::DirectiveError),
}

pub fn slate_ir_with_args(
    path: &Path,
    compiler_args: &[String],
) -> Result<(slate_parser::ir::Module, slate_parser::files::Files), Error> {
    frontend::parse_module_with_args(path, compiler_args).map_err(Error::from)
}

/// Translates a C source file into Rust.
///
/// # Errors
///
/// Returns [`Error`] when reading, lowering, formatting, or translating the source fails.
pub fn translate(path: &Path) -> Result<String, Error> {
    translate_with_args(path, &[])
}

/// Translates a C source file into Rust with additional compiler arguments.
///
/// # Errors
///
/// Returns [`Error`] when preprocessing, lowering, formatting, or translation fails.
pub fn translate_with_args(path: &Path, extra_args: &[String]) -> Result<String, Error> {
    let (contents, _raw) = preprocess::read_source(path).map_err(|source| Error::Read {
        path: path.to_path_buf(),
        source,
    })?;
    if directive_translate::should_auto_expand(&contents) {
        return directive_translate::translate_directives_with_args(path, extra_args)
            .map_err(Error::Directive);
    }
    let (module, files, diagnostics) = frontend::parse_module_with_source(path, None, extra_args)?;
    let mut program =
        frontend::lower_module(&module, &files, &frontend::lowerer::LowerOptions::default())?;
    directive_translate::insert_directive_items(
        &mut program,
        diagnostics
            .iter()
            .enumerate()
            .flat_map(|(index, diagnostic)| match diagnostic.severity {
                miette::Severity::Warning => directive_translate::warning_items(
                    &diagnostic.text,
                    index,
                    None,
                    directive_translate::WarningBackend::Standalone,
                ),
                _ => vec![rust_ast::Item::Macro {
                    name: "compile_error".into(),
                    args: vec![rust_ast::Expr::Str(diagnostic.text.clone())],
                }],
            })
            .collect(),
    );
    let source = backend::apply_with_target(program, &module.target).emit();
    backend::pretty_rust(&source).map_err(|message| Error::Format { message })
}

pub fn lowered_slate_program_with_args(
    path: &Path,
    extra_args: &[String],
) -> Result<rust_ast::Program, Error> {
    let (module, files) = slate_ir_with_args(path, extra_args)?;
    frontend::lower_module(&module, &files, &frontend::lowerer::LowerOptions::default())
        .map_err(Error::from)
}

/// Translates a C source file for the requested target triples.
///
/// # Errors
///
/// Returns [`Error::Directive`] if target-specific directive translation fails.
pub fn translate_targets_with_args(
    path: &Path,
    extra_args: &[String],
    targets: &[String],
) -> Result<String, Error> {
    directive_translate::translate_targets_with_args(path, extra_args, targets)
        .map_err(Error::Directive)
}
