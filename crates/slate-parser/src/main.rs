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
    let (ast, files) = parser.parse_file(Path::new(&path))?;
    let defines = compiler_args.defines;
    let mut env = Env::new();
    for define in &defines {
        env = env.define(define.split_once('=').map_or_else(
            || define.trim_start_matches("-D").to_string(),
            |(name, _)| name.trim_start_matches("-D").to_string(),
        ));
    }
    ast.analyze(&defines, &files)?;
    let stdout = io::stdout();
    let mut renderer = Renderer::new(stdout.lock());
    renderer
        .render(&ast, &env)
        .map_err(|error| miette::miette!(error))?;
    Ok(())
}
