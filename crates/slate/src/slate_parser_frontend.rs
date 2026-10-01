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
    #[error("{report}")]
    Unsupported {
        barrier: Box<lowerer::Barrier>,
        report: String,
    },
    #[error("{report}")]
    Invalid {
        invalid: Box<lowerer::InvalidIr>,
        report: String,
    },
}

#[derive(Debug, Error, miette::Diagnostic)]
#[error("{message}")]
struct SiteDiagnostic {
    message: String,
    #[source_code]
    code: miette::NamedSource<String>,
    #[label("{label}")]
    span: miette::SourceSpan,
    label: String,
    #[help]
    help: String,
}

fn render_site(
    site: &lowerer::Site,
    files: &Files,
    message: String,
    label: &str,
    ir: String,
) -> String {
    let help = match site.spelling(files) {
        Some(spelling) => format!("{ir}\nspelled at {spelling}"),
        None => ir,
    };
    let path = files.get_path(site.expansion.file);
    let Some((path, source)) = path.and_then(|path| Some((path, std::fs::read(path).ok()?))) else {
        return format!("{message} at {}\n{help}", site.render(files));
    };
    let diagnostic = SiteDiagnostic {
        message,
        code: miette::NamedSource::new(
            path.display().to_string(),
            String::from_utf8_lossy(&source).into_owned(),
        )
        .with_language("C"),
        span: (site.expansion.offset, site.expansion.length).into(),
        label: label.to_owned(),
        help,
    };
    let mut report = String::new();
    let handler = if std::io::IsTerminal::is_terminal(&std::io::stderr()) {
        miette::GraphicalReportHandler::new_themed(miette::GraphicalTheme::unicode())
            .with_syntax_highlighting(miette::highlighters::SyntectHighlighter::default())
    } else {
        miette::GraphicalReportHandler::new_themed(miette::GraphicalTheme::unicode_nocolor())
            .without_syntax_highlighting()
    };
    match handler
        .with_context_lines(2)
        .render_report(&mut report, &diagnostic)
    {
        Ok(()) => report,
        Err(_) => format!(
            "{} at {}\n{}",
            diagnostic.message,
            site.render(files),
            diagnostic.help
        ),
    }
}

fn function_suffix(function: &Option<String>) -> String {
    function
        .as_ref()
        .map(|function| format!(" in {function}"))
        .unwrap_or_default()
}

pub fn lower_module(
    module: &Module,
    files: &Files,
) -> Result<crate::backend::rust_ast::Program, Error> {
    let lowered = lowerer::lower(module, &lowerer::LowerOptions::default()).map_err(|invalid| {
        Error::Invalid {
            report: render_site(
                &invalid.site,
                files,
                format!(
                    "invalid slate-parser IR{}",
                    function_suffix(&invalid.function)
                ),
                "broken IR invariant",
                invalid.invariant.to_string(),
            ),
            invalid: Box::new(invalid),
        }
    })?;
    match lowered.barriers.into_iter().next() {
        Some(barrier) => Err(Error::Unsupported {
            report: render_site(
                &barrier.site,
                files,
                format!(
                    "unsupported slate-parser IR{}",
                    function_suffix(&barrier.function)
                ),
                &format!("cannot lower {} to Rust", barrier.construct.kind()),
                barrier.construct.to_string(),
            ),
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
