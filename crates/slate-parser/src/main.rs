use slate_parser::render::render_path;
use std::env;
use std::fs;
use std::path::Path;

fn main() {
    let mut args = env::args().skip(1);
    assert_eq!(
        args.next().as_deref(),
        Some("filecheck"),
        "usage: slate-parser filecheck <source.c> [-DNAME]"
    );
    let path = args.next().expect("missing source path");
    let mut defines = Vec::new();
    for arg in args {
        if let Some(define) = arg.strip_prefix("-D") {
            defines.push(define.to_string());
        } else {
            panic!("unsupported argument: {arg}");
        }
    }
    let _ = fs::metadata(Path::new(&path)).expect("read source fixture");
    print!("{}", render_path(Path::new(&path), &defines));
}
