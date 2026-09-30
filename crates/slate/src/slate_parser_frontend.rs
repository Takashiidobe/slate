use slate_parser::compiler_args::CompilerArgParser;
use slate_parser::dialect::Dialect;
use slate_parser::ir::Module;
use slate_parser::parser::Parser;
use slate_parser::sema::Sema;
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
    Ok(module)
}
