
use serde_json::Value;
use slate_parser::eval::{Env, eval_translation_unit};
use slate_parser::parser::parse_translation_unit;
use std::process::Command;

const RETURN_SRC: &str = "int main() {\n#ifdef _WIN32\nreturn 2;\n#else\nreturn 3;\n#endif\n}\n";
const TYPEDEF_SRC: &str =
    "typedef int HANDLE;\n#ifdef _WIN32\ntypedef HANDLE Socket;\n#else\ntypedef int Socket;\n#endif\n";

fn main() {
    compare("return example", RETURN_SRC, &["_WIN32"]);
    compare("return example", RETURN_SRC, &[]);
    compare("typedef example", TYPEDEF_SRC, &["_WIN32"]);
    compare("typedef example", TYPEDEF_SRC, &[]);
}

fn compare(label: &str, src: &str, defines: &[&str]) {
    println!("\n{}", "=".repeat(60));
    println!("{label}  (defines: {defines:?})");
    println!("{}", "=".repeat(60));
    println!("--- source ---\n{src}");

    let ast = parse_translation_unit(src);
    let mut env = Env::new();
    for d in defines {
        env = env.define(*d);
    }
    let concrete = eval_translation_unit(&ast, &env);

    println!("--- our concrete AST (post-eval) ---\n{concrete:#?}\n");
    println!(
        "--- clang AST (non-implicit top-level decls; id/loc/range stripped) ---\n{}",
        clang_ast_trimmed(src, defines)
    );
}

fn clang_ast_trimmed(src: &str, defines: &[&str]) -> String {
    let path = std::env::temp_dir().join(format!("slate_compare_{}.c", std::process::id()));
    std::fs::write(&path, src).expect("failed to write temp source file");

    let mut cmd = Command::new("clang");
    cmd.args(["-Xclang", "-ast-dump=json", "-fsyntax-only"]);
    for d in defines {
        cmd.arg(format!("-D{d}"));
    }
    cmd.arg(&path);

    let output = cmd.output().expect("failed to invoke clang - is it on PATH?");
    let _ = std::fs::remove_file(&path);
    if !output.status.success() {
        return format!("<clang error>\n{}", String::from_utf8_lossy(&output.stderr));
    }

    let mut root: Value = serde_json::from_slice(&output.stdout).expect("clang did not emit valid AST JSON");
    strip_noise(&mut root);

    let empty = Vec::new();
    let visible: Vec<&Value> = root["inner"]
        .as_array()
        .unwrap_or(&empty)
        .iter()
        .filter(|n| !n["isImplicit"].as_bool().unwrap_or(false))
        .collect();

    serde_json::to_string_pretty(&visible).unwrap()
}

fn strip_noise(v: &mut Value) {
    match v {
        Value::Object(map) => {
            for key in ["id", "loc", "range", "mangledName", "typeAliasDeclId"] {
                map.remove(key);
            }
            for val in map.values_mut() {
                strip_noise(val);
            }
        }
        Value::Array(arr) => {
            for val in arr.iter_mut() {
                strip_noise(val);
            }
        }
        _ => {}
    }
}
