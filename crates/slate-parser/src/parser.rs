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
        if matches!(&nodes[i], PPNode::Code { text, .. } if lex(text).is_empty()) {
            i += 1;
            continue;
        }
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
            let tokens = lex(node_text(&nodes[0]));
            if tokens.contains(&Token::Semi) && !tokens.contains(&Token::LBrace) {
                return (
                    Decl::Declaration {
                        declaration: parse_declaration(node_text(&nodes[0])),
                        provenance: node_provenance(&nodes[0]),
                    },
                    1,
                );
            }
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

pub fn parse_declaration(code: &str) -> Declaration {
    let tokens = lex(code);
    let mut parser = DeclaratorParser { tokens: &tokens, pos: 0 };
    let storage = if parser.take(Token::Keyword(Keyword::Typedef)) {
        StorageClass::Typedef
    } else {
        StorageClass::None
    };
    let ty = parser.parse_base_type();
    let declarator = parser.parse_declarator(false);
    assert_eq!(parser.peek(), Some(&Token::Semi), "expected `;`");
    parser.pos += 1;
    assert_eq!(parser.peek(), None, "unexpected tokens after declaration");
    Declaration {
        specifiers: DeclarationSpecifiers {
            ty,
            qualifiers: Qualifiers::default(),
            storage,
        },
        declarator,
    }
}

struct DeclaratorParser<'a> {
    tokens: &'a [Token],
    pos: usize,
}

impl<'a> DeclaratorParser<'a> {
    fn peek(&self) -> Option<&Token> {
        self.tokens.get(self.pos)
    }

    fn take(&mut self, expected: Token) -> bool {
        if self.peek() == Some(&expected) {
            self.pos += 1;
            true
        } else {
            false
        }
    }

    fn parse_base_type(&mut self) -> CType {
        let token = self.peek().cloned().expect("expected declaration type");
        self.pos += 1;
        match token {
            Token::Keyword(Keyword::Char) => CType::Char,
            Token::Keyword(Keyword::Int) => CType::Int,
            Token::Keyword(Keyword::Void) => CType::Void,
            Token::Ident(name) => CType::Named(name),
            other => panic!("expected declaration type, found {other:?}"),
        }
    }

    fn parse_declarator(&mut self, allow_abstract: bool) -> Declarator {
        let mut pointer_count = 0;
        while self.take(Token::Star) {
            pointer_count += 1;
        }

        let mut declarator = match self.peek().cloned() {
            Some(Token::Ident(name)) => {
                self.pos += 1;
                Declarator::Name(name)
            }
            Some(Token::LParen) => {
                self.pos += 1;
                let declarator = self.parse_declarator(allow_abstract);
                assert!(self.take(Token::RParen), "expected `)` in declarator");
                declarator
            }
            _ if allow_abstract => Declarator::Abstract,
            _ => panic!("expected declarator"),
        };

        for _ in 0..pointer_count {
            declarator = Declarator::Pointer {
                qualifiers: Qualifiers::default(),
                inner: Box::new(declarator),
            };
        }

        loop {
            declarator = match self.peek() {
                Some(Token::LBracket) => {
                    self.pos += 1;
                    let size = match self.peek().cloned() {
                        Some(Token::RBracket) => ArraySize::Unspecified,
                        Some(Token::IntLit(value)) => {
                            self.pos += 1;
                            ArraySize::Expression(Box::new(Expr::IntLit(value)))
                        }
                        other => panic!("unsupported array bound: {other:?}"),
                    };
                    assert!(self.take(Token::RBracket), "expected `]` in array declarator");
                    Declarator::Array {
                        inner: Box::new(declarator),
                        size,
                    }
                }
                Some(Token::LParen) => {
                    let (parameters, variadic) = self.parse_parameters();
                    Declarator::Function {
                        inner: Box::new(declarator),
                        parameters,
                        variadic,
                    }
                }
                _ => break,
            };
        }
        declarator
    }

    fn parse_parameters(&mut self) -> (Vec<Parameter>, bool) {
        assert!(self.take(Token::LParen), "expected `(` in function declarator");
        if self.take(Token::RParen) {
            return (vec![], false);
        }
        if self.peek() == Some(&Token::Keyword(Keyword::Void))
            && self.tokens.get(self.pos + 1) == Some(&Token::RParen)
        {
            self.pos += 2;
            return (vec![], false);
        }

        let mut parameters = Vec::new();
        let mut variadic = false;
        loop {
            if self.take(Token::Ellipsis) {
                variadic = true;
                assert!(self.take(Token::RParen), "expected `)` after `...`");
                break;
            }
            let ty = self.parse_base_type();
            let declarator = match self.peek() {
                Some(Token::Comma) | Some(Token::RParen) => None,
                _ => Some(self.parse_declarator(true)),
            };
            parameters.push(Parameter { ty, declarator });
            if self.take(Token::RParen) {
                break;
            }
            assert!(self.take(Token::Comma), "expected `,` between parameters");
        }
        (parameters, variadic)
    }
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
            PPNode::Code { text, .. } if !lex(text).is_empty() => {
                stmts.extend(parse_stmts_from_code(text))
            }
            PPNode::Code { .. } => {}
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
