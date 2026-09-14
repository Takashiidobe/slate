use crate::backend::{self, rust_ast};
use crate::ctx;
use crate::frontend::{self, c_ast, cir_input, directive_translate, preprocess};
use clang_ir::model::Module;
use std::path::{Path, PathBuf};
use thiserror::Error as ThisError;

#[derive(Debug, ThisError)]
/// Errors returned by [`translate`], [`translate_with_args`], and related API functions.
///
/// ```
/// use std::path::Path;
/// use slate::api::{self, Error};
///
/// let error = api::translate(Path::new("missing.c")).unwrap_err();
/// assert!(matches!(error, Error::Read { .. }));
/// ```
pub enum Error {
    /// The source file could not be read.
    #[error("read {path}: {source}")]
    Read {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    /// Preprocessing the source file failed.
    #[error("preprocess {path}: {source}")]
    Preprocess {
        path: PathBuf,
        #[source]
        source: preprocess::PreprocessError,
    },
    /// A diagnostic's conditional state could not be evaluated.
    #[error(
        "translate: cannot determine whether #{name} at line {line} is active because predicate `{predicate}` cannot be evaluated"
    )]
    UnevaluableDiagnostic {
        path: PathBuf,
        name: String,
        line: usize,
        predicate: String,
    },
    /// Preparing the source for Clang failed.
    #[error("prepare Clang input for {path}: {source}")]
    ClangInput {
        path: PathBuf,
        #[source]
        source: preprocess::PreprocessError,
    },
    /// Loading the generated ClangIR module failed.
    #[error("load CIR for {path}: {source}")]
    Cir {
        path: PathBuf,
        #[source]
        source: cir_input::ModuleError,
    },
    /// Loading the Clang AST failed.
    #[error("load Clang AST for {path}: {source}")]
    Ast {
        path: PathBuf,
        #[source]
        source: c_ast::AstError,
    },
    /// Lowering ClangIR and AST data into Rust failed.
    #[error("lowering failed for {path}:{diagnostics}")]
    Lowering {
        path: PathBuf,
        diagnostics: ctx::Diagnostics,
    },
    /// An active preprocessor directive is unsupported.
    #[error("{context}: {message}")]
    UnsupportedDirective {
        context: String,
        line: usize,
        message: String,
    },
    /// A directive's active state could not be determined.
    #[error("{context}: cannot determine whether {message} is active")]
    IndeterminateDirective {
        context: String,
        line: usize,
        message: String,
    },
    /// A directive's conditional predicate could not be evaluated.
    #[error(
        "{context}: cannot determine whether {message} is active because predicate `{predicate}` cannot be evaluated"
    )]
    UnevaluableDirective {
        context: String,
        line: usize,
        message: String,
        predicate: String,
    },
    /// Formatting the generated Rust failed.
    #[error("format generated Rust: {message}")]
    Format { message: String },
    /// Translating preprocessor directives failed.
    #[error(transparent)]
    Directive(#[from] directive_translate::DirectiveError),
}

/// Translates a C source file into Rust.
///
/// # Errors
///
/// Returns [`Error`] when reading, lowering, formatting, or translating the source fails.
pub fn translate(path: &Path) -> Result<String, Error> {
    translate_with_args(path, &[])
}

/// Translates a C source file into Rust with additional Clang arguments.
///
/// # Errors
///
/// Returns [`Error`] when preprocessing, lowering, formatting, or translation fails.
pub fn translate_with_args(path: &Path, extra_args: &[String]) -> Result<String, Error> {
    let (contents, _raw) = preprocess::read_source(path).map_err(|source| Error::Read {
        path: path.to_path_buf(),
        source,
    })?;
    if frontend::toolchain::target_is_host_default()
        && directive_translate::should_auto_expand(&contents)
    {
        return directive_translate::translate_directives_with_args(path, extra_args)
            .map_err(Error::Directive);
    }
    let (_, program) = lowered_program_with_args(path, extra_args)?;
    let source = backend::apply(program).emit();
    backend::format_rust(&source).map_err(|message| Error::Format { message })
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

/// Lowers a C source file into ClangIR and the baseline Rust program.
///
/// # Errors
///
/// Returns [`Error`] if reading, preprocessing, parsing, or lowering fails.
pub fn lowered_program(path: &Path) -> Result<(Module, rust_ast::Program), Error> {
    lowered_program_with_args(path, &[])
}

/// Lowers a C source file with additional Clang arguments.
///
/// # Errors
///
/// Returns [`Error`] if reading, preprocessing, parsing, or lowering fails.
pub fn lowered_program_with_args(
    path: &Path,
    extra_args: &[String],
) -> Result<(Module, rust_ast::Program), Error> {
    let (source, raw) = preprocess::read_source(path).map_err(|source| Error::Read {
        path: path.to_path_buf(),
        source,
    })?;
    let pp = preprocess::record_translation_unit(path, &source, extra_args).map_err(|source| {
        Error::Preprocess {
            path: path.to_path_buf(),
            source,
        }
    })?;
    reject_active_unsupported(&pp, "translate")?;
    let diagnostics: Vec<_> = pp
        .directives
        .iter()
        .filter(|directive| {
            matches!(
                directive.name,
                preprocess::DirectiveName::Error | preprocess::DirectiveName::Warning
            )
        })
        .collect();
    for directive in &diagnostics {
        if let (Some(condition), None) = (&directive.condition, directive.active) {
            return Err(Error::UnevaluableDiagnostic {
                path: path.to_path_buf(),
                name: directive.name.as_str().to_string(),
                line: directive.line_start,
                predicate: preprocess::predicate_text(condition),
            });
        }
    }
    let input =
        preprocess::clang_input(path, &raw, &diagnostics).map_err(|source| Error::ClangInput {
            path: path.to_path_buf(),
            source,
        })?;
    let all_args: Vec<String> = extra_args
        .iter()
        .cloned()
        .chain(input.extra_args().iter().cloned())
        .collect();
    let module = cir_input::emit_module(path, &all_args).map_err(|source| Error::Cir {
        path: path.to_path_buf(),
        source,
    })?;
    let unit = c_ast::parse_file_with_args(path, &all_args).map_err(|source| Error::Ast {
        path: path.to_path_buf(),
        source,
    })?;

    let mut ctx = ctx::Ctx::default();
    let mut program = frontend::lower(&module, &unit, &mut ctx);
    if ctx.diagnostics.has_errors() {
        return Err(Error::Lowering {
            path: path.to_path_buf(),
            diagnostics: ctx.diagnostics,
        });
    }
    let index = program
        .items
        .iter()
        .take_while(|item| matches!(item, rust_ast::Item::CrateAttrs(_)))
        .count();
    program.items.splice(
        index..index,
        diagnostics
            .into_iter()
            .filter(|directive| directive.active == Some(true))
            .enumerate()
            .flat_map(|(index, directive)| {
                if directive.name == preprocess::DirectiveName::Error {
                    vec![rust_ast::Item::Macro {
                        name: "compile_error".into(),
                        args: vec![rust_ast::Expr::Str(directive.raw_payload.clone())],
                    }]
                } else {
                    directive_translate::warning_items(
                        &directive.raw_payload,
                        index,
                        None,
                        directive_translate::WarningBackend::Standalone,
                    )
                }
            }),
    );

    Ok((module, program))
}

/// Rejects active unsupported directives in recorded preprocessing data.
///
/// # Errors
///
/// Returns [`Error::UnsupportedDirective`], [`Error::IndeterminateDirective`], or
/// [`Error::UnevaluableDirective`] when an unsupported directive cannot be accepted.
pub fn reject_active_unsupported(
    pp: &preprocess::Preprocessing,
    context: &str,
) -> Result<(), Error> {
    for directive in pp.directives.iter().filter(|directive| {
        directive.disposition() == preprocess::DirectiveDisposition::UnsupportedSemantic
    }) {
        if directive.is_clang_resolved_pragma() {
            continue;
        }
        match directive.active {
            Some(false) => {}
            Some(true) => {
                return Err(Error::UnsupportedDirective {
                    context: context.to_string(),
                    line: directive.line_start,
                    message: directive.unsupported_message(),
                });
            }
            None => {
                let message = directive.unsupported_message();
                let Some(condition) = directive.condition.as_ref() else {
                    return Err(Error::IndeterminateDirective {
                        context: context.to_string(),
                        line: directive.line_start,
                        message,
                    });
                };
                return Err(Error::UnevaluableDirective {
                    context: context.to_string(),
                    line: directive.line_start,
                    message,
                    predicate: preprocess::predicate_text(condition),
                });
            }
        }
    }
    Ok(())
}

/// Reads a source file and rejects its active unsupported directives.
///
/// # Errors
///
/// Returns [`Error::Read`], [`Error::Preprocess`], or a directive rejection error from
/// [`reject_active_unsupported`].
pub fn reject_active_unsupported_file(path: &Path, context: &str) -> Result<(), Error> {
    let (source, _raw) = preprocess::read_source(path).map_err(|source| Error::Read {
        path: path.to_path_buf(),
        source,
    })?;
    let pp = preprocess::record_translation_unit(path, &source, &[]).map_err(|source| {
        Error::Preprocess {
            path: path.to_path_buf(),
            source,
        }
    })?;
    reject_active_unsupported(&pp, context)
}
