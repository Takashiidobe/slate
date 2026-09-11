use slate_parser::{eval, parser};

fn main() {
    let return_src = "int main() {\n#ifdef _WIN32\nreturn 2;\n#else\nreturn 3;\n#endif\n}\n";
    let return_ast = parser::parse_translation_unit(return_src);
    println!("{return_ast:#?}");
    println!(
        "eval(_WIN32 defined)   = {:#?}",
        eval::eval_translation_unit(&return_ast, &eval::Env::new().define("_WIN32"))
    );
    println!(
        "eval(_WIN32 undefined) = {:#?}",
        eval::eval_translation_unit(&return_ast, &eval::Env::new())
    );

    let typedef_src = "#ifdef _WIN32\ntypedef HANDLE Socket;\n#else\ntypedef int Socket;\n#endif\n";
    let typedef_ast = parser::parse_translation_unit(typedef_src);
    println!("{typedef_ast:#?}");
    println!(
        "eval(_WIN32 defined)   = {:#?}",
        eval::eval_translation_unit(&typedef_ast, &eval::Env::new().define("_WIN32"))
    );
    println!(
        "eval(_WIN32 undefined) = {:#?}",
        eval::eval_translation_unit(&typedef_ast, &eval::Env::new())
    );
}
