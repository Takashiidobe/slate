use miette::Severity;
use slate_parser::compiler_args::CompilerArgParser;
use slate_parser::eval::Env;
use slate_parser::files::SearchPaths;
use slate_parser::parser::Parser;
use slate_parser::pp::{DirectiveDiagnostic, DirectiveErrors};
use slate_parser::render::Renderer;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::io;
use std::path::Path;

fn main() -> miette::Result<()> {
    let mut args = env::args().skip(1);
    if args.next().as_deref() != Some("parse") {
        return Err(miette::miette!(
            "usage: slate-parser parse <source.c> [-DNAME]"
        ));
    }
    let path = args
        .next()
        .ok_or_else(|| miette::miette!("missing source path"))?;
    let compiler_args = CompilerArgParser::parse(args).map_err(|error| miette::miette!(error))?;
    fs::metadata(Path::new(&path)).map_err(|error| miette::miette!(error))?;
    let search = SearchPaths {
        system: compiler_args
            .isystem
            .iter()
            .map(std::path::PathBuf::from)
            .collect(),
        ..SearchPaths::default()
    };
    let mut parser = Parser::new(search);
    let parsed = parser.parse_file(Path::new(&path));
    let env = parser
        .predefined_macros()
        .iter()
        .cloned()
        .chain(
            compiler_args
                .defines
                .iter()
                .map(|define| define_name(define)),
        )
        .fold(Env::new(), |env, name| env.define(name));
    report_directives(parser.directive_diagnostics(), &env)?;
    let (ast, files) = parsed?;
    ast.analyze(&env, &files)?;
    let stdout = io::stdout();
    let mut renderer = Renderer::new(stdout.lock());
    renderer
        .render(&ast, &env)
        .map_err(|error| miette::miette!(error))?;
    Ok(())
}

fn define_name(define: &str) -> String {
    let define = define.trim_start_matches("-D");
    define
        .split_once('=')
        .map_or(define, |(name, _)| name)
        .to_string()
}

fn report_directives(diagnostics: &[DirectiveDiagnostic], env: &Env) -> miette::Result<()> {
    let mut seen = HashSet::new();
    let mut errors = Vec::new();
    for diagnostic in diagnostics {
        if !diagnostic.condition.eval(env)
            || !seen.insert((diagnostic.loc.file, diagnostic.loc.offset))
        {
            continue;
        }
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
