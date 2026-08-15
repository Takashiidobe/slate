use slate::api;
use std::path::Path;

fn main() {
    let path = std::env::args().nth(1).expect("usage: dump_ast <file.c>");
    let program = api::parse_native(Path::new(&path)).expect("parse failed");
    println!("{program:#?}");
}
