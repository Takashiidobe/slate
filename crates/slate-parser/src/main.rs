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
            "usage: slate-parser parse <source.c> [-DNAME] [--flavor=gcc|clang|msvc] [-std=c89|gnu89|c99|gnu99|c11|gnu11|c17|gnu17|c23|gnu23] [--show-comments] [--show-ids] [--dump-ir-expressions] [--show-spans]"
        ));
    }
    let path = args
        .next()
        .ok_or_else(|| miette::miette!("missing source path"))?;
    let mut show_comments = false;
    let mut show_ids = false;
    let mut dump_ir_expressions = false;
    let mut show_spans = false;
    let remaining: Vec<String> = args
        .filter(|arg| {
            if arg == "--show-spans" {
                show_spans = true;
                false
            } else if arg == "--dump-ir-expressions" {
                dump_ir_expressions = true;
                false
            } else if arg == "--show-comments" {
                show_comments = true;
                false
            } else if arg == "--show-ids" {
                show_ids = true;
                false
            } else {
                true
            }
        })
        .collect();
    let compiler_args =
        CompilerArgParser::parse(remaining).map_err(|error| miette::miette!(error))?;
    if show_spans && !dump_ir_expressions {
        return Err(miette::miette!(
            "--show-spans requires --dump-ir-expressions"
        ));
    }
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
        .with_flavor(compiler_args.flavor)
        .with_options(compiler_args.options)
        .with_standard(compiler_args.standard);
    let parsed = parser.parse_file(Path::new(&path));
    report_directives(parser.directive_diagnostics())?;
    let (ast, files) = parsed?;
    ast.analyze(&files)?;
    if dump_ir_expressions {
        let expressions = slate_parser::sema::resolve_expression_roots(&ast)
            .map_err(|error| miette::miette!("{error}"))?;
        for expression in expressions {
            println!("{}", expression.display(show_spans));
        }
        return Ok(());
    }
    let stdout = io::stdout();
    let mut renderer = Renderer::new(stdout.lock())
        .with_show_comments(show_comments)
        .with_show_ids(show_ids);
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
