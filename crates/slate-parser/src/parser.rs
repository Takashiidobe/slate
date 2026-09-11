use crate::ast::*;
use crate::files::{Files, SearchPaths};
use crate::lexer::{Keyword, Token, lex};
use crate::pp::{PPConditional, PPNode, Preprocessor};
use std::path::Path;

pub fn parse_translation_unit(src: &str) -> TranslationUnit {
    let search = SearchPaths::default();
    let mut pp = Preprocessor::new(&search);
    let nodes = pp.parse_str("<main>", src);
    TranslationUnit {
        decls: parse_decls(&nodes),
    }
}

pub fn parse_translation_unit_from_file(path: &Path, search: &SearchPaths) -> (TranslationUnit, Files) {
    let mut pp = Preprocessor::new(search);
    let nodes = pp.parse_file(path);
    let decls = parse_decls(&nodes);
    (TranslationUnit { decls }, pp.files)
}

fn parse_decls(nodes: &[PPNode]) -> Vec<Decl> {
    let mut decls = Vec::new();
    let mut i = 0;
    while i < nodes.len() {
        let (decl, consumed) = parse_top_level_item(&nodes[i..]);
        decls.push(decl);
        i += consumed;
    }
    decls
}

fn node_text(node: &PPNode) -> &str {
    match node {
        PPNode::Code { text, .. } => text,
        PPNode::Conditional(_) => panic!("expected a plain code line, found a conditional region"),
    }
}

fn node_provenance(node: &PPNode) -> Provenance {
    match node {
        PPNode::Code { provenance, .. } => *provenance,
        PPNode::Conditional(_) => panic!("conditional regions have no single provenance"),
    }
}

fn parse_top_level_item(nodes: &[PPNode]) -> (Decl, usize) {
    match &nodes[0] {
        PPNode::Code { text, provenance } if text.trim_start().starts_with("typedef") => {
            let (name, ty) = parse_typedef_line(text);
            (
                Decl::Typedef {
                    name,
                    ty,
                    provenance: *provenance,
                },
                1,
            )
        }
        PPNode::Code { .. } => {
            let (func, consumed) = parse_function(nodes);
            (Decl::Function(func), consumed)
        }
        PPNode::Conditional(cond) => (parse_top_level_conditional(cond), 1),
    }
}

fn parse_top_level_conditional(cond: &PPConditional) -> Decl {
    let branches = cond
        .branches
        .iter()
        .map(|(c, body)| (c.clone(), parse_decls(body)))
        .collect();
    Decl::Conditional(Conditional { branches })
}

fn parse_typedef_line(code: &str) -> (String, CType) {
    let tokens = lex(code);
    assert_eq!(
        tokens.first(),
        Some(&Token::Keyword(Keyword::Typedef)),
        "expected `typedef`"
    );
    let ty = match tokens.get(1) {
        Some(Token::Keyword(Keyword::Int)) => CType::Int,
        Some(Token::Ident(n)) => CType::Named(n.clone()),
        _ => panic!("unsupported type in typedef (phase 1 scope)"),
    };
    let name = match tokens.get(2) {
        Some(Token::Ident(n)) => n.clone(),
        _ => panic!("expected typedef name"),
    };
    assert_eq!(tokens.get(3), Some(&Token::Semi), "expected `;`");
    (name, ty)
}

fn parse_function(nodes: &[PPNode]) -> (FunctionDecl, usize) {
    let provenance = node_provenance(&nodes[0]);
    let sig_tokens = lex(node_text(&nodes[0]));
    assert_eq!(
        sig_tokens.first(),
        Some(&Token::Keyword(Keyword::Int)),
        "phase 0/1 only supports `int`-returning functions"
    );
    let name = match sig_tokens.get(1) {
        Some(Token::Ident(n)) => n.clone(),
        _ => panic!("expected function name"),
    };
    assert_eq!(sig_tokens.get(2), Some(&Token::LParen));
    assert_eq!(sig_tokens.get(3), Some(&Token::RParen));
    assert_eq!(
        sig_tokens.get(4),
        Some(&Token::LBrace),
        "expected `{{` (phase 0/1: single-line signature only)"
    );

    let close_idx = nodes[1..]
        .iter()
        .position(|n| matches!(n, PPNode::Code { text, .. } if lex(text) == vec![Token::RBrace]))
        .expect("missing closing `}` for function body")
        + 1;

    let body = parse_stmt_list(&nodes[1..close_idx]);
    (
        FunctionDecl {
            ret_type: Type::Int,
            name,
            body,
            provenance,
        },
        close_idx + 1,
    )
}

fn parse_stmt_list(nodes: &[PPNode]) -> Vec<Stmt> {
    let mut stmts = Vec::new();
    for node in nodes {
        match node {
            PPNode::Code { text, .. } => stmts.extend(parse_stmts_from_code(text)),
            PPNode::Conditional(cond) => {
                let branches = cond
                    .branches
                    .iter()
                    .map(|(c, body)| (c.clone(), parse_stmt_list(body)))
                    .collect();
                stmts.push(Stmt::Conditional(Conditional { branches }));
            }
        }
    }
    stmts
}

fn parse_stmts_from_code(code: &str) -> Vec<Stmt> {
    let tokens = lex(code);
    let mut stmts = Vec::new();
    let mut i = 0;
    while i < tokens.len() {
        assert_eq!(
            tokens[i],
            Token::Keyword(Keyword::Return),
            "phase 0/1 only supports `return <int>;` statements"
        );
        let value = match tokens.get(i + 1) {
            Some(Token::IntLit(n)) => *n,
            _ => panic!("expected integer literal after `return`"),
        };
        assert_eq!(tokens.get(i + 2), Some(&Token::Semi), "expected `;`");
        stmts.push(Stmt::Return(Expr::IntLit(value)));
        i += 3;
    }
    stmts
}

#[cfg(test)]
mod tests {
    use super::*;

    const MAIN: Provenance = Provenance {
        file: FileId(0),
        kind: HeaderKind::User,
    };

    #[test]
    fn plain_return_no_conditionals() {
        let tu = parse_translation_unit("int main() {\nreturn 3;\n}\n");
        let Decl::Function(f) = &tu.decls[0] else {
            panic!("expected a function decl");
        };
        assert_eq!(f.name, "main");
        assert_eq!(f.body, vec![Stmt::Return(Expr::IntLit(3))]);
        assert_eq!(f.provenance, MAIN);
    }

    #[test]
    fn ifdef_else_produces_conditional_stmt() {
        let src = "int main() {\n#ifdef _WIN32\nreturn 2;\n#else\nreturn 3;\n#endif\n}\n";
        let tu = parse_translation_unit(src);
        let Decl::Function(f) = &tu.decls[0] else {
            panic!("expected a function decl");
        };
        assert_eq!(
            f.body,
            vec![Stmt::Conditional(Conditional {
                branches: vec![
                    (
                        Condition::Defined("_WIN32".into()),
                        vec![Stmt::Return(Expr::IntLit(2))]
                    ),
                    (
                        Condition::Not(Box::new(Condition::Defined("_WIN32".into()))),
                        vec![Stmt::Return(Expr::IntLit(3))]
                    ),
                ]
            })]
        );
    }

    #[test]
    fn conditional_typedef_produces_conditional_decl() {
        let src = "#ifdef _WIN32\ntypedef HANDLE Socket;\n#else\ntypedef int Socket;\n#endif\n";
        let tu = parse_translation_unit(src);
        assert_eq!(
            tu.decls,
            vec![Decl::Conditional(Conditional {
                branches: vec![
                    (
                        Condition::Defined("_WIN32".into()),
                        vec![Decl::Typedef {
                            name: "Socket".into(),
                            ty: CType::Named("HANDLE".into()),
                            provenance: MAIN,
                        }]
                    ),
                    (
                        Condition::Not(Box::new(Condition::Defined("_WIN32".into()))),
                        vec![Decl::Typedef {
                            name: "Socket".into(),
                            ty: CType::Int,
                            provenance: MAIN,
                        }]
                    ),
                ]
            })]
        );
    }

    #[test]
    fn include_tags_system_vs_user_provenance() {
        let dir = std::env::temp_dir().join(format!(
            "slate_parser_include_test_{}_{}",
            std::process::id(),
            std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap()
                .as_nanos()
        ));
        let sys_dir = dir.join("sys");
        let usr_dir = dir.join("usr");
        std::fs::create_dir_all(&sys_dir).unwrap();
        std::fs::create_dir_all(&usr_dir).unwrap();

        std::fs::write(sys_dir.join("limits.h"), "typedef int ULONG_MAX_TYPE;\n").unwrap();
        std::fs::write(usr_dir.join("myconfig.h"), "typedef int MyConfigType;\n").unwrap();

        let main_path = dir.join("main.c");
        std::fs::write(
            &main_path,
            "#include <limits.h>\n#include \"myconfig.h\"\n",
        )
        .unwrap();

        let search = SearchPaths {
            user: vec![usr_dir],
            system: vec![sys_dir],
        };
        let (tu, files) = parse_translation_unit_from_file(&main_path, &search);

        let Decl::Typedef { name: n0, provenance: p0, .. } = &tu.decls[0] else {
            panic!("expected typedef");
        };
        let Decl::Typedef { name: n1, provenance: p1, .. } = &tu.decls[1] else {
            panic!("expected typedef");
        };

        assert_eq!(n0, "ULONG_MAX_TYPE");
        assert_eq!(p0.kind, HeaderKind::System);
        assert!(files.path(p0.file).ends_with("sys/limits.h"));

        assert_eq!(n1, "MyConfigType");
        assert_eq!(p1.kind, HeaderKind::User);
        assert!(files.path(p1.file).ends_with("usr/myconfig.h"));

        let _ = std::fs::remove_dir_all(&dir);
    }
}
