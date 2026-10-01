use slate_parser::compiler_args::CompilerArgParser;
use slate_parser::dialect::Dialect;
use slate_parser::files::Files;
use slate_parser::ir::Module;
use slate_parser::parser::Parser;
use slate_parser::pp::DirectiveDiagnostic;
use slate_parser::sema::Sema;
use std::path::{Path, PathBuf};
use thiserror::Error;

pub mod c_shim;
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
    #[label(collection)]
    labels: Vec<miette::LabeledSpan>,
    #[help]
    help: String,
}

fn render_site(
    site: &lowerer::Site,
    files: &Files,
    message: String,
    label: &str,
    ir: String,
    context: &[lowerer::Context],
    clip_to_first_line: bool,
) -> String {
    let mut help = ir;
    if let Some(spelling) = site.spelling(files) {
        help.push_str(&format!("\nspelled at {spelling}"));
    }
    for context in context
        .iter()
        .filter(|context| context.site.expansion.file != site.expansion.file)
    {
        help.push_str(&format!(
            "\n{} at {}",
            context.label,
            context.site.render(files)
        ));
    }
    let path = files.get_path(site.expansion.file);
    let Some((path, source)) = path.and_then(|path| Some((path, std::fs::read(path).ok()?))) else {
        return format!("{message} at {}\n{help}", site.render(files));
    };
    let source = String::from_utf8_lossy(&source).into_owned();
    let first_line = |offset: usize| {
        let end = source
            .get(offset..)
            .and_then(|rest| rest.find('\n'))
            .unwrap_or(0);
        miette::SourceSpan::from((offset, end))
    };
    let labels = std::iter::once(miette::LabeledSpan::new_primary_with_span(
        Some(label.to_owned()),
        if clip_to_first_line {
            let line = first_line(site.expansion.offset);
            (site.expansion.offset, line.len().min(site.expansion.length)).into()
        } else {
            miette::SourceSpan::from((site.expansion.offset, site.expansion.length))
        },
    ))
    .chain(
        context
            .iter()
            .filter(|context| context.site.expansion.file == site.expansion.file)
            .map(|context| {
                miette::LabeledSpan::new_with_span(
                    Some(context.label.clone()),
                    first_line(context.site.expansion.offset),
                )
            }),
    )
    .collect();
    let diagnostic = SiteDiagnostic {
        message,
        code: miette::NamedSource::new(path.display().to_string(), source).with_language("C"),
        labels,
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
    options: &lowerer::LowerOptions,
) -> Result<crate::backend::rust_ast::Program, Error> {
    let lowered = lowerer::lower(module, options).map_err(|invalid| Error::Invalid {
        report: render_site(
            &invalid.site,
            files,
            format!(
                "invalid slate-parser IR{}",
                function_suffix(&invalid.function)
            ),
            "broken IR invariant",
            invalid.invariant.to_string(),
            &invalid.context,
            false,
        ),
        invalid: Box::new(invalid),
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
                &barrier.construct.label(),
                barrier.construct.to_string(),
                &barrier.context,
                matches!(
                    barrier.construct,
                    lowerer::Construct::Function { .. } | lowerer::Construct::Return { .. }
                ),
            ),
            barrier: Box::new(barrier),
        }),
        None => Ok(lowered.program),
    }
}

pub fn parse_module_with_args(path: &Path, args: &[String]) -> Result<(Module, Files), Error> {
    let (module, files, diagnostics) = parse_module_with_source(path, None, args)?;
    reject_directive_errors(path, &diagnostics)?;
    Ok((module, files))
}

pub fn parse_module_with_source(
    path: &Path,
    source: Option<String>,
    args: &[String],
) -> Result<(Module, Files, Vec<DirectiveDiagnostic>), Error> {
    let args = CompilerArgParser::parse(args.iter().cloned())?;
    let search = args.search_paths();
    let dialect = Dialect::new(args.flavor, args.standard, args.target, args.options);
    let mut parser =
        Parser::new(search, dialect).with_preprocessor_inputs(args.preprocessor_inputs);
    let (unit, files) = match source {
        Some(source) => parser.parse_file_with_source(path, source),
        None => parser.parse_file(path),
    }
    .map_err(|error| Error::Parse {
        path: path.to_path_buf(),
        message: error.to_string(),
    })?;
    let mut sema = Sema::new(&unit);
    sema.analyze(&files).map_err(|error| Error::Analyze {
        path: path.to_path_buf(),
        message: error.to_string(),
    })?;
    let (module, _) = sema.lower(&files).map_err(|error| Error::Lower {
        path: path.to_path_buf(),
        message: error.to_string(),
    })?;
    Ok((module, files, parser.directive_diagnostics().to_vec()))
}

pub fn reject_directive_errors(
    path: &Path,
    diagnostics: &[DirectiveDiagnostic],
) -> Result<(), Error> {
    let errors: Vec<_> = diagnostics
        .iter()
        .filter(|diagnostic| diagnostic.severity != miette::Severity::Warning)
        .map(ToString::to_string)
        .collect();
    if errors.is_empty() {
        return Ok(());
    }
    Err(Error::Analyze {
        path: path.to_path_buf(),
        message: errors.join("\n"),
    })
}
