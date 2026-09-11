use slate_parser::compiler_args::CompilerArgParser;
use slate_parser::eval::Env;
use slate_parser::files::SearchPaths;
use slate_parser::parser::Parser;
use slate_parser::render::Renderer;
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
    let mut parser = Parser::new(SearchPaths::default());
    let (ast, _) = parser.parse_file(Path::new(&path))?;
    let mut env = Env::new();
    for define in compiler_args.defines {
        env = env.define(define);
    }
    let stdout = io::stdout();
    let mut renderer = Renderer::new(stdout.lock());
    renderer
        .render(&ast, &env)
        .map_err(|error| miette::miette!(error))?;
    Ok(())
}
