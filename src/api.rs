use crate::backend::{self, rust_ast};
use crate::frontend::{self, c_ast, directive_translate, preprocess};
use crate::{cir, ctx, parse};
use std::path::{Path, PathBuf};
use thiserror::Error as ThisError;

#[derive(Debug, ThisError)]
pub enum Error {
    #[error("read {path}: {source}")]
    Read {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("preprocess {path}: {source}")]
    Preprocess {
        path: PathBuf,
        #[source]
        source: preprocess::PreprocessError,
    },
    #[error(
        "translate: cannot determine whether #{name} at line {line} is active because predicate `{predicate}` cannot be evaluated"
    )]
    UnevaluableDiagnostic {
        path: PathBuf,
        name: String,
        line: usize,
        predicate: String,
    },
    #[error("prepare Clang input for {path}: {source}")]
    ClangInput {
        path: PathBuf,
        #[source]
        source: preprocess::PreprocessError,
    },
    #[error("load CIR for {path}: {source}")]
    Cir {
        path: PathBuf,
        #[source]
        source: cir::ModuleError,
    },
    #[error("load Clang AST for {path}: {source}")]
    Ast {
        path: PathBuf,
        #[source]
        source: c_ast::AstError,
    },
    #[error("lowering failed for {path}:{diagnostics}")]
    Lowering {
        path: PathBuf,
        diagnostics: ctx::Diagnostics,
    },
    #[error("{context}: {message}")]
    UnsupportedDirective {
        context: String,
        line: usize,
        message: String,
    },
    #[error("{context}: cannot determine whether {message} is active")]
    IndeterminateDirective {
        context: String,
        line: usize,
        message: String,
    },
    #[error(
        "{context}: cannot determine whether {message} is active because predicate `{predicate}` cannot be evaluated"
    )]
    UnevaluableDirective {
        context: String,
        line: usize,
        message: String,
        predicate: String,
    },
    #[error("SLATE_SKIP_PASS is not valid Unicode: {source}")]
    SkipPassEnvironment {
        #[source]
        source: std::env::VarError,
    },
    #[error("unknown SLATE_SKIP_PASS: {name}")]
    UnknownSkipPass { name: String },
    #[error("parse {path} with native frontend: {source}")]
    NativeParse {
        path: PathBuf,
        #[source]
        source: parse::error::CompileError,
    },
    #[error("{path}: native frontend parsed successfully but lowering is not yet implemented")]
    NativeLoweringUnimplemented { path: PathBuf },
    #[error("resolve target for {path}: {source}")]
    Target {
        path: PathBuf,
        #[source]
        source: cir::TargetError,
    },
}

pub fn translate(path: &Path) -> Result<String, Error> {
    translate_with_args(path, &[])
}

pub fn translate_with_args(path: &Path, extra_args: &[String]) -> Result<String, Error> {
    let (_, program) = lowered_program_with_args(path, extra_args)?;
    Ok(backend::apply_with(program, &skip_set_from_env()?).emit())
}

pub fn lowered_program(path: &Path) -> Result<(cir::ir::Module, rust_ast::Program), Error> {
    lowered_program_with_args(path, &[])
}

pub fn lowered_program_with_args(
    path: &Path,
    extra_args: &[String],
) -> Result<(cir::ir::Module, rust_ast::Program), Error> {
    let source = std::fs::read_to_string(path).map_err(|source| Error::Read {
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
    let input = preprocess::clang_input(path, &source, &diagnostics).map_err(|source| {
        Error::ClangInput {
            path: path.to_path_buf(),
            source,
        }
    })?;
    let all_args: Vec<String> = extra_args
        .iter()
        .cloned()
        .chain(input.extra_args().iter().cloned())
        .collect();
    let module = cir::emit_module(path, &all_args).map_err(|source| Error::Cir {
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

pub fn reject_active_unsupported_file(path: &Path, context: &str) -> Result<(), Error> {
    let source = std::fs::read_to_string(path).map_err(|source| Error::Read {
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

pub fn parse_native(path: &Path) -> Result<parse::ast::Program, Error> {
    if let Some(dir) = cir::emit::libc_shim_dir() {
        parse::preprocessor::set_include_paths(vec![PathBuf::from(dir)]);
    }
    let target = cir::emit::active_target();
    let defines = cir::emit::target_define_pairs(&target).map_err(|source| Error::Target {
        path: path.to_path_buf(),
        source,
    })?;
    let tokens = parse::lexer::tokenize_file(path).map_err(|source| Error::NativeParse {
        path: path.to_path_buf(),
        source,
    })?;
    let tokens =
        parse::preprocessor::preprocess(tokens, defines, Vec::new()).map_err(|source| {
            Error::NativeParse {
                path: path.to_path_buf(),
                source,
            }
        })?;
    parse_native_tokens(tokens).map_err(|source| Error::NativeParse {
        path: path.to_path_buf(),
        source,
    })
}

/// Recursive-descent expression/statement parsing can nest deep enough on
/// pathological (e.g. heavily macro-expanded) input to overflow the default
/// thread stack, so run it on a thread with a much larger one.
fn parse_native_tokens(
    tokens: Vec<parse::lexer::Token>,
) -> Result<parse::ast::Program, parse::error::CompileError> {
    std::thread::Builder::new()
        .stack_size(256 * 1024 * 1024)
        .spawn(move || parse::parser::parse(&tokens))
        .expect("spawn native-parser thread")
        .join()
        .expect("native-parser thread panicked")
}

pub fn translate_native(path: &Path) -> Result<String, Error> {
    let program = parse_native(path)?;
    let rust_program = frontend::lower_program(&program);
    Ok(backend::apply_with(rust_program, &skip_set_from_env()?).emit())
}

pub fn lowered_program_native(path: &Path) -> Result<rust_ast::Program, Error> {
    let program = parse_native(path)?;
    Ok(frontend::lower_program(&program))
}

pub fn skip_set_from_env() -> Result<backend::SkipSet, Error> {
    match std::env::var("SLATE_SKIP_PASS") {
        Ok(name) if !name.trim().is_empty() => backend::Pass::parse(name.trim())
            .map(backend::SkipSet::skip)
            .ok_or(Error::UnknownSkipPass { name }),
        Ok(_) | Err(std::env::VarError::NotPresent) => Ok(backend::SkipSet::none()),
        Err(source) => Err(Error::SkipPassEnvironment { source }),
    }
}
