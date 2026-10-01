use slate_parser::compiler_args::CompilerArgParser;
use slate_parser::dialect::Dialect;
use slate_parser::files::Files;
use slate_parser::ir::Module;
use slate_parser::parser::Parser;
use slate_parser::sema::Sema;
use std::path::{Path, PathBuf};
use thiserror::Error;

pub(crate) mod long_double;
pub mod lowerer;

#[derive(Debug, Error)]
pub enum Error {
    #[error("invalid slate-parser arguments: {0}")]
    Arguments(#[from] slate_parser::compiler_args::CompilerArgError),
    #[error("parse {path}: {message}")]
    Parse { path: PathBuf, message: String },
    #[error("analyze {path}: {message}")]
    Analyze { path: PathBuf, message: String },
    #[error("lower {path} to slate-parser IR: {message}")]
    Lower { path: PathBuf, message: String },
    #[error("unsupported slate-parser IR at {location}: {barrier}")]
    Unsupported {
        barrier: Box<lowerer::Barrier>,
        location: String,
    },
    #[error("invalid slate-parser IR at {location}: {invalid}")]
    Invalid {
        invalid: Box<lowerer::InvalidIr>,
        location: String,
    },
}

pub fn lower_module(
    module: &Module,
    files: &Files,
) -> Result<crate::backend::rust_ast::Program, Error> {
    let lowered = lowerer::lower(module, &lowerer::LowerOptions::default()).map_err(|invalid| {
        Error::Invalid {
            location: invalid.site.render(files),
            invalid: Box::new(invalid),
        }
    })?;
    match lowered.barriers.into_iter().next() {
        Some(barrier) => Err(Error::Unsupported {
            location: barrier.site.render(files),
            barrier: Box::new(barrier),
        }),
        None => Ok(lowered.program),
    }
}

pub fn parse_module_with_args(path: &Path, args: &[String]) -> Result<(Module, Files), Error> {
    let args = CompilerArgParser::parse(args.iter().cloned())?;
    let search = args.search_paths();
    let dialect = Dialect::new(args.flavor, args.standard, args.target, args.options);
    let mut parser =
        Parser::new(search, dialect).with_preprocessor_inputs(args.preprocessor_inputs);
    let (unit, files) = parser.parse_file(path).map_err(|error| Error::Parse {
        path: path.to_path_buf(),
        message: error.to_string(),
    })?;
    let diagnostics: Vec<_> = parser
        .directive_diagnostics()
        .iter()
        .filter(|diagnostic| diagnostic.severity != miette::Severity::Warning)
        .collect();
    if !diagnostics.is_empty() {
        return Err(Error::Analyze {
            path: path.to_path_buf(),
            message: diagnostics
                .iter()
                .map(ToString::to_string)
                .collect::<Vec<_>>()
                .join("\n"),
        });
    }
    let mut sema = Sema::new(&unit);
    sema.analyze(&files).map_err(|error| Error::Analyze {
        path: path.to_path_buf(),
        message: error.to_string(),
    })?;
    let (module, _) = sema.lower(&files).map_err(|error| Error::Lower {
        path: path.to_path_buf(),
        message: error.to_string(),
    })?;
    Ok((module, files))
}
