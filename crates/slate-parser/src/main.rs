use miette::Severity;
use slate_parser::compiler_args::CompilerArgParser;
use slate_parser::dialect::Dialect;
use slate_parser::parser::Parser;
use slate_parser::pp::{DirectiveDiagnostic, DirectiveErrors, write_preprocessed};
use slate_parser::render::Renderer;
use std::env;
use std::fs;
use std::io;
use std::path::Path;

// deeply nested sources outgrow the default main-thread stack long before NESTING_LIMIT
const STACK_SIZE: usize = 256 << 20;

fn main() -> miette::Result<()> {
    std::thread::Builder::new()
        .stack_size(STACK_SIZE)
        .spawn(run)
        .map_err(|error| miette::miette!(error))?
        .join()
        .map_err(|_| miette::miette!("parser thread panicked"))?
}

fn run() -> miette::Result<()> {
    let mut args = env::args().skip(1);
    let command = args.next();
    if !matches!(command.as_deref(), Some("parse" | "ir" | "pp")) {
        return Err(miette::miette!(
            "usage: slate-parser <parse|ir|pp> <source.c> [-DNAME] [-UNAME] [-include <file>] [-imacros <file>] [-target=<triple>|-target <triple>] [--flavor=gcc|clang|msvc] [-std=<C standard>] [-I<dir>] [-iquote <dir>] [-isystem <dir>] [-idirafter <dir>] [-nostdlibinc] [-isysroot <dir>|--sysroot=<dir>] [--show-comments] [--show-ids] [--dump-ir] [--dump-ir-types] [--dump-ir-expressions] [--dump-ir-names] [--show-spans] [--show-metadata] [--compact-ir]"
        ));
    }
    let path = args
        .next()
        .ok_or_else(|| miette::miette!("missing source path"))?;
    let mut show_comments = false;
    let mut show_ids = false;
    let mut dump_ir_expressions = false;
    let mut dump_ir_names = false;
    let mut dump_ir_types = false;
    let mut dump_ir = command.as_deref() == Some("ir");
    let mut show_spans = false;
    let mut show_metadata = false;
    let mut compact_ir = false;
    let remaining: Vec<String> = args
        .filter(|arg| {
            if arg == "--show-metadata" {
                show_metadata = true;
                false
            } else if arg == "--compact-ir" {
                compact_ir = true;
                false
            } else if arg == "--dump-ir" {
                dump_ir = true;
                false
            } else if arg == "--dump-ir-types" {
                dump_ir_types = true;
                false
            } else if arg == "--show-spans" {
                show_spans = true;
                false
            } else if arg == "--dump-ir-expressions" {
                dump_ir_expressions = true;
                false
            } else if arg == "--dump-ir-names" {
                dump_ir_names = true;
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
    let compiler_args = CompilerArgParser::parse(remaining).map_err(miette::Report::new)?;
    if u8::from(dump_ir)
        + u8::from(dump_ir_types)
        + u8::from(dump_ir_names)
        + u8::from(dump_ir_expressions)
        > 1
    {
        return Err(miette::miette!("IR dump modes are mutually exclusive"));
    }
    if show_spans && !(dump_ir || dump_ir_expressions) {
        return Err(miette::miette!(
            "--show-spans requires ir, --dump-ir, or --dump-ir-expressions"
        ));
    }
    if show_metadata && !(dump_ir || dump_ir_types) {
        return Err(miette::miette!(
            "--show-metadata requires ir, --dump-ir, or --dump-ir-types"
        ));
    }
    if compact_ir && !(dump_ir || dump_ir_types) {
        return Err(miette::miette!(
            "--compact-ir requires ir, --dump-ir, or --dump-ir-types"
        ));
    }
    fs::metadata(Path::new(&path)).map_err(|error| miette::miette!(error))?;
    let search = compiler_args.search_paths();
    let dialect = Dialect::new(
        compiler_args.flavor,
        compiler_args.standard,
        compiler_args.target,
        compiler_args.options,
    );
    let mut parser =
        Parser::new(search, dialect).with_preprocessor_inputs(compiler_args.preprocessor_inputs);
    if command.as_deref() == Some("pp") {
        let preprocessed = parser.preprocess_file(Path::new(&path));
        report_directives(parser.directive_diagnostics())?;
        let (nodes, files) = preprocessed?;
        let stdout = io::stdout();
        return write_preprocessed(&nodes, &files, &mut io::BufWriter::new(stdout.lock()))
            .map_err(|error| miette::miette!(error));
    }
    let parsed = parser.parse_file(Path::new(&path));
    report_directives(parser.directive_diagnostics())?;
    let (ast, files) = parsed?;
    let mut sema = slate_parser::sema::Sema::new(&ast);
    for warning in sema.analyze(&files)? {
        eprintln!("{:?}", miette::Report::new(warning));
    }
    if dump_ir || dump_ir_types {
        let module = if dump_ir_types {
            sema.type_module()
                .map_err(|error| miette::miette!("{error}"))?
        } else {
            let (module, diagnostics) = sema.lower(&files)?;
            for warning in diagnostics {
                eprintln!("{:?}", miette::Report::new(warning));
            }
            module
        };
        let display = module
            .display(show_metadata)
            .with_spans(show_spans)
            .with_comments(show_comments);
        print!(
            "{}",
            if compact_ir {
                display.compact()
            } else {
                display
            }
        );
        return Ok(());
    }
    if dump_ir_names {
        let resolution = sema.names().map_err(|error| miette::miette!("{error}"))?;
        print!("{resolution}");
        return Ok(());
    }
    if dump_ir_expressions {
        let expressions = sema
            .expression_roots()
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
