use super::lower_program;
use crate::backend::codegen::program_to_string;
use std::sync::atomic::{AtomicUsize, Ordering};

static COUNTER: AtomicUsize = AtomicUsize::new(0);

fn render(src: &str) -> String {
    let id = COUNTER.fetch_add(1, Ordering::Relaxed);
    let path = std::env::temp_dir().join(format!("slate_native_lowerer_test_{id}.c"));
    std::fs::write(&path, src).expect("write temp fixture");
    let tokens = crate::parse::lexer::tokenize_file(&path).expect("tokenize");
    let tokens =
        crate::parse::preprocessor::preprocess(tokens, Vec::new(), Vec::new()).expect("preprocess");
    let program = crate::parse::parser::parse(&tokens).expect("parse");
    std::fs::remove_file(&path).ok();
    let rust_program = lower_program(&program);
    let rust_src = program_to_string(&rust_program);
    compile_check(&rust_src, id);
    rust_src
}

fn compile_check(rust_src: &str, id: usize) {
    let path = std::env::temp_dir().join(format!("slate_native_lowerer_test_{id}.rs"));
    std::fs::write(&path, rust_src).expect("write generated rust");
    let rustc = std::env::var("RUSTC").unwrap_or_else(|_| "rustc".into());
    let output = std::process::Command::new(rustc)
        .args(["--edition=2024", "--crate-type", "lib", "--emit=metadata"])
        .arg("-o")
        .arg(std::env::temp_dir().join(format!("slate_native_lowerer_test_{id}.rmeta")))
        .arg(&path)
        .output()
        .expect("spawn rustc");
    std::fs::remove_file(&path).ok();
    if !output.status.success() {
        panic!(
            "generated Rust failed to compile:\n{}\n--- stderr ---\n{}",
            rust_src,
            String::from_utf8_lossy(&output.stderr)
        );
    }
}

#[test]
fn empty_main() {
    let out = render("int main(void) { return 0; }");
    println!("{out}");
    assert!(out.contains("fn main"));
}

#[test]
fn arithmetic_and_locals() {
    let out = render(
        r#"
        int add(int a, int b) {
            int c = a + b;
            return c * 2;
        }
        "#,
    );
    println!("{out}");
    assert!(out.contains("fn add"));
}

#[test]
fn if_else_and_loop() {
    let out = render(
        r#"
        int sum_to(int n) {
            int total = 0;
            for (int i = 0; i < n; i = i + 1) {
                if (i % 2 == 0) {
                    total = total + i;
                } else {
                    total = total - 1;
                }
            }
            return total;
        }
        "#,
    );
    println!("{out}");
    assert!(out.contains("fn sum_to"));
}

#[test]
fn pointers_and_arrays() {
    let out = render(
        r#"
        int first(int *arr) {
            return arr[0];
        }
        int global_arr[4] = {1, 2, 3, 4};
        "#,
    );
    println!("{out}");
    assert!(out.contains("fn first"));
}

#[test]
fn struct_def_and_field_access() {
    let out = render(
        r#"
        struct point { int x; int y; };
        int getx(struct point *p) {
            return p->x;
        }
        "#,
    );
    println!("{out}");
    assert!(out.contains("struct point") || out.contains("Point") || out.contains("struct"));
}

#[test]
fn switch_stmt() {
    let out = render(
        r#"
        int classify(int x) {
            switch (x) {
                case 0:
                    return 100;
                case 1:
                    return 200;
                default:
                    return -1;
            }
        }
        "#,
    );
    println!("{out}");
    assert!(out.contains("fn classify"));
}

#[test]
fn calls_libc() {
    let out = render(
        r#"
        int puts(const char *s);
        int main(void) {
            puts("hi");
            return 0;
        }
        "#,
    );
    println!("{out}");
    assert!(out.contains("fn main"));
}

#[test]
fn static_local_counter() {
    let out = render(
        r#"
        int next(void) {
            static int counter = 0;
            counter = counter + 1;
            return counter;
        }
        "#,
    );
    println!("{out}");
    assert!(out.contains("fn next"));
}

#[test]
fn pointer_difference() {
    let out = render(
        r#"
        long diff(int *a, int *b) {
            return a - b;
        }
        "#,
    );
    println!("{out}");
    assert!(out.contains("fn diff"));
}

#[test]
fn pointer_arith_both_directions() {
    let out = render(
        r#"
        int at(int *arr, int i) {
            return *(arr + i) + *(i + arr) - *(arr - (-i));
        }
        "#,
    );
    println!("{out}");
    assert!(out.contains("fn at"));
}
