
use clang_ast::Node;
use serde::Deserialize;
use slate_parser::ast::*;
use slate_parser::eval::{Env, eval_translation_unit};
use slate_parser::parser::parse_translation_unit;

type ClangNode = Node<Clang>;

#[derive(Deserialize)]
enum Clang {
    FunctionDecl(FunctionDecl),
    TypedefDecl(TypedefDecl),
    ReturnStmt,
    IntegerLiteral(IntegerLiteral),
    Other,
}

#[derive(Deserialize)]
struct FunctionDecl {
    name: Option<String>,
    #[serde(rename = "isImplicit", default)]
    is_implicit: bool,
}

#[derive(Deserialize)]
struct TypedefDecl {
    name: String,
    #[serde(rename = "isImplicit", default)]
    is_implicit: bool,
    r#type: QualType,
}

#[derive(Deserialize)]
struct QualType {
    #[serde(rename = "qualType")]
    qual_type: String,
}

#[derive(Deserialize)]
struct IntegerLiteral {
    value: String,
}

#[derive(Debug, PartialEq)]
enum CTypeSummary {
    Int,
    Named(String),
}

#[derive(Debug, PartialEq)]
enum DeclSummary {
    Function { name: String, returns: Vec<i64> },
    Typedef { name: String, ty: CTypeSummary },
}

fn summarize_concrete(tu: &ConcreteTranslationUnit) -> Vec<DeclSummary> {
    tu.decls.iter().map(summarize_concrete_decl).collect()
}

fn summarize_concrete_decl(decl: &ConcreteDecl) -> DeclSummary {
    match decl {
        ConcreteDecl::Function(f) => DeclSummary::Function {
            name: f.name.clone(),
            returns: f
                .body
                .iter()
                .map(|s| {
                    let ConcreteStmt::Return(Expr::IntLit(n)) = s;
                    *n
                })
                .collect(),
        },
        ConcreteDecl::Typedef { name, ty, .. } => DeclSummary::Typedef {
            name: name.clone(),
            ty: match ty {
                CType::Int => CTypeSummary::Int,
                CType::Named(n) => CTypeSummary::Named(n.clone()),
            },
        },
    }
}

fn run_clang_ast(src: &str, defines: &[&str]) -> ClangNode {
    let path = std::env::temp_dir().join(format!(
        "slate_oracle_{}_{}.c",
        std::process::id(),
        std::time::SystemTime::now()
            .duration_since(std::time::UNIX_EPOCH)
            .unwrap()
            .as_nanos()
    ));
    std::fs::write(&path, src).expect("failed to write oracle source file");

    let mut cmd = std::process::Command::new("clang");
    cmd.args(["-Xclang", "-ast-dump=json", "-fsyntax-only"]);
    for d in defines {
        cmd.arg(format!("-D{d}"));
    }
    cmd.arg(&path);

    let output = cmd.output().expect("failed to invoke clang - is it on PATH?");
    let _ = std::fs::remove_file(&path);
    assert!(
        output.status.success(),
        "clang rejected oracle source: {}",
        String::from_utf8_lossy(&output.stderr)
    );

    serde_json::from_slice(&output.stdout).expect("clang did not emit valid AST JSON")
}

fn summarize_clang(root: &ClangNode) -> Vec<DeclSummary> {
    root.inner.iter().filter_map(summarize_clang_decl).collect()
}

fn summarize_clang_decl(node: &ClangNode) -> Option<DeclSummary> {
    match &node.kind {
        Clang::FunctionDecl(f) if !f.is_implicit => Some(DeclSummary::Function {
            name: f.name.clone().expect("FunctionDecl without a name"),
            returns: collect_returns(node),
        }),
        Clang::TypedefDecl(t) if !t.is_implicit => Some(DeclSummary::Typedef {
            name: t.name.clone(),
            ty: if t.r#type.qual_type == "int" {
                CTypeSummary::Int
            } else {
                CTypeSummary::Named(t.r#type.qual_type.clone())
            },
        }),
        _ => None,
    }
}

fn collect_returns(node: &ClangNode) -> Vec<i64> {
    let mut out = Vec::new();
    walk_returns(node, &mut out);
    out
}

fn walk_returns(node: &ClangNode, out: &mut Vec<i64>) {
    if let Clang::ReturnStmt = &node.kind
        && let Some(child) = node.inner.first()
    {
        match &child.kind {
            Clang::IntegerLiteral(lit) => out.push(
                lit.value
                    .parse()
                    .expect("clang IntegerLiteral value wasn't an integer"),
            ),
            _ => panic!("phase 1 oracle only handles integer-literal returns"),
        }
    }
    for child in &node.inner {
        walk_returns(child, out);
    }
}

fn assert_matches_clang(src: &str, defines: &[&str]) {
    let ours = {
        let ast = parse_translation_unit(src);
        let mut env = Env::new();
        for d in defines {
            env = env.define(*d);
        }
        summarize_concrete(&eval_translation_unit(&ast, &env))
    };
    let theirs = summarize_clang(&run_clang_ast(src, defines));
    assert_eq!(
        ours, theirs,
        "our eval() output diverged from clang for defines={defines:?}"
    );
}

#[test]
fn return_example_matches_clang_win32_defined() {
    let src = "int main() {\n#ifdef _WIN32\nreturn 2;\n#else\nreturn 3;\n#endif\n}\n";
    assert_matches_clang(src, &["_WIN32"]);
}

#[test]
fn return_example_matches_clang_win32_undefined() {
    let src = "int main() {\n#ifdef _WIN32\nreturn 2;\n#else\nreturn 3;\n#endif\n}\n";
    assert_matches_clang(src, &[]);
}

#[test]
fn typedef_example_matches_clang_win32_defined() {
    let src = "typedef int HANDLE;\n#ifdef _WIN32\ntypedef HANDLE Socket;\n#else\ntypedef int Socket;\n#endif\n";
    assert_matches_clang(src, &["_WIN32"]);
}

#[test]
fn typedef_example_matches_clang_win32_undefined() {
    let src = "typedef int HANDLE;\n#ifdef _WIN32\ntypedef HANDLE Socket;\n#else\ntypedef int Socket;\n#endif\n";
    assert_matches_clang(src, &[]);
}
