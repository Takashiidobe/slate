use miette::Severity;
use slate_parser::compiler_args::CompilerArgParser;
use slate_parser::files::SearchPaths;
use slate_parser::parser::Parser;
use slate_parser::pp::{DirectiveDiagnostic, DirectiveErrors};
use slate_parser::render::Renderer;
use std::env;
use std::fs;
use std::io;
use std::path::{Path, PathBuf};

fn main() -> miette::Result<()> {
    let mut args = env::args().skip(1);
    if args.next().as_deref() != Some("parse") {
        return Err(miette::miette!(
            "usage: slate-parser parse <source.c> [-DNAME] [--flavor=gcc|clang|msvc]"
        ));
    }
    let path = args
        .next()
        .ok_or_else(|| miette::miette!("missing source path"))?;
    let compiler_args = CompilerArgParser::parse(args).map_err(|error| miette::miette!(error))?;
    fs::metadata(Path::new(&path)).map_err(|error| miette::miette!(error))?;
    let mut system: Vec<PathBuf> = compiler_args.isystem.iter().map(PathBuf::from).collect();
    if let Some(home) = env::var_os("HOME") {
        system.push(Path::new(&home).join("Projects/slate/libc-shim/include"));
    }
    let search = SearchPaths {
        system,
        ..SearchPaths::default()
    };
    let mut parser = Parser::new(search)
        .with_defines(compiler_args.defines)
        .with_flavor(compiler_args.flavor);
    let parsed = parser.parse_file(Path::new(&path));
    report_directives(parser.directive_diagnostics())?;
    let (ast, files) = parsed?;
    ast.analyze(&files)?;
    let stdout = io::stdout();
    let mut renderer = Renderer::new(stdout.lock());
    renderer
        .render(&ast)
        .map_err(|error| miette::miette!(error))?;
    Ok(())
}

fn report_directives(diagnostics: &[DirectiveDiagnostic]) -> miette::Result<()> {
    let mut errors = Vec::new();
    for diagnostic in diagnostics {
        if diagnostic.severity == Severity::Warning {
            eprintln!("{:?}", miette::Report::new(diagnostic.clone()));
        } else {
            errors.push(diagnostic.clone());
        }
    }
    match errors.len() {
        0 => Ok(()),
        1 => Err(miette::Report::new(errors.remove(0))),
        _ => Err(miette::Report::new(DirectiveErrors { errors })),
    }
}
