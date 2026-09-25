use slate_parser::compiler_args::CompilerArgParser;
use slate_parser::ir::Module;
use slate_parser::parser::Parser;
use slate_parser::sema;
use std::path::{Path, PathBuf};
use thiserror::Error;

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
    #[error("unsupported slate-parser IR: {0}")]
    Unsupported(String),
}

pub fn parse_module_with_args(path: &Path, args: &[String]) -> Result<Module, Error> {
    let args = CompilerArgParser::parse(args.iter().cloned())?;
    let mut parser = Parser::new(args.search_paths())
        .with_preprocessor_inputs(args.preprocessor_inputs)
        .with_target(args.target)
        .with_flavor(args.flavor)
        .with_options(args.options)
        .with_standard(args.standard);
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
    unit.analyze(&files).map_err(|error| Error::Analyze {
        path: path.to_path_buf(),
        message: error.to_string(),
    })?;
    let (module, _) = sema::resolve_module(&unit, &files).map_err(|error| Error::Lower {
        path: path.to_path_buf(),
        message: error.to_string(),
    })?;
    Ok(module)
}
