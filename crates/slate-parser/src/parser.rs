use crate::ast::*;
use crate::error::ParseError;
use crate::files::{Files, SearchPaths};
use crate::lexer::{Keyword, Token, lex};
use crate::pp::{PPConditional, PPNode, Preprocessor};
use crate::reachability::filter_translation_unit;
use std::path::Path;

pub struct Parser {
    search: SearchPaths,
    source_name: String,
    source: String,
}

impl Parser {
    pub fn new(search: SearchPaths) -> Self {
        Self {
            search,
            source_name: "<source>".into(),
            source: String::new(),
        }
    }

    pub fn parse_source(&mut self, src: &str) -> Result<TranslationUnit, ParseError> {
        self.source_name = "<main>".into();
        self.source = src.into();
        let mut pp = Preprocessor::new(&self.search);
        let nodes = pp.parse_str("<main>", src);
        self.parse_nodes(&nodes)
    }

    pub fn parse_file(&mut self, path: &Path) -> Result<(TranslationUnit, Files), ParseError> {
        self.source_name = path.display().to_string();
        self.source = std::fs::read_to_string(path).map_err(|error| {
            ParseError::new(
                self.source_name.clone(),
                "",
                0,
                0,
                format!("failed to read source: {error}"),
            )
        })?;
        let mut pp = Preprocessor::new(&self.search);
        let nodes = pp.parse_file(path);
        let ast = self.parse_nodes(&nodes);
        ast.map(|ast| (ast, pp.files))
    }

    pub fn parse_declaration(&self, code: &str) -> Result<Declaration, ParseError> {
        let tokens = lex(code);
        let mut parser = DeclaratorParser {
            tokens: &tokens,
            pos: 0,
        };
        let storage = if parser.take(Token::Keyword(Keyword::Typedef)) {
            StorageClass::Typedef
        } else {
            StorageClass::None
        };
        let ty = parser.parse_base_type();
        if !matches!(
            parser.peek(),
            Some(&Token::Ident(_)) | Some(&Token::LParen) | Some(&Token::Star)
        ) {
            return Err(self.error_at(
                code,
                code.len().saturating_sub(1),
                1,
                "expected declarator",
            ));
        }
        let declarator = parser.parse_declarator(false);
        if parser.peek() != Some(&Token::Semi) {
            return Err(self.error_at(code, 0, code.len(), "expected `;`"));
        }
        parser.pos += 1;
        if parser.peek().is_some() {
            return Err(self.error_at(code, 0, code.len(), "unexpected tokens after declaration"));
        }
        Ok(Declaration {
            specifiers: DeclarationSpecifiers {
                ty,
                qualifiers: Qualifiers::default(),
                storage,
            },
            declarator,
        })
    }

    fn parse_nodes(&self, nodes: &[PPNode]) -> Result<TranslationUnit, ParseError> {
        let ast = filter_translation_unit(
            &TranslationUnit {
                decls: self.parse_decls(nodes)?,
            },
            FileId(0),
        );
        Ok(ast)
    }

    fn parse_decls(&self, nodes: &[PPNode]) -> Result<Vec<Decl>, ParseError> {
        let mut decls = Vec::new();
        let mut i = 0;
        while i < nodes.len() {
            if matches!(&nodes[i], PPNode::Code { text, .. } if lex(text).is_empty()) {
                i += 1;
                continue;
            }
            let (decl, consumed) = self.parse_top_level_item(&nodes[i..])?;
            decls.push(decl);
            i += consumed;
        }
        Ok(decls)
    }

    fn parse_top_level_item(&self, nodes: &[PPNode]) -> Result<(Decl, usize), ParseError> {
        match &nodes[0] {
            PPNode::Code { text, provenance } if text.trim_start().starts_with("typedef") => {
                let (name, ty) = self.parse_typedef_line(text)?;
                Ok((
                    Decl::Typedef {
                        name,
                        ty,
                        provenance: *provenance,
                    },
                    1,
                ))
            }
            PPNode::Code { .. } => {
                let tokens = lex(self.node_text(&nodes[0]));
                if tokens.contains(&Token::Semi) && !tokens.contains(&Token::LBrace) {
                    return Ok((
                        Decl::Declaration {
                            declaration: self.parse_declaration(self.node_text(&nodes[0]))?,
                            provenance: self.node_provenance(&nodes[0]),
                        },
                        1,
                    ));
                }
                let (func, consumed) = self.parse_function(nodes)?;
                Ok((Decl::Function(func), consumed))
            }
            PPNode::Conditional(cond) => Ok((self.parse_top_level_conditional(cond)?, 1)),
        }
    }

    fn parse_top_level_conditional(&self, cond: &PPConditional) -> Result<Decl, ParseError> {
        let branches = cond
            .branches
            .iter()
            .map(|(c, body)| Ok((c.clone(), self.parse_decls(body)?)))
            .collect::<Result<Vec<_>, ParseError>>()?;
        Ok(Decl::Conditional(Conditional { branches }))
    }

    fn parse_typedef_line(&self, code: &str) -> Result<(String, CType), ParseError> {
        let tokens = lex(code);
        if tokens.first() != Some(&Token::Keyword(Keyword::Typedef)) {
            return Err(self.error_at(code, 0, code.len(), "expected `typedef`"));
        }
        let ty = match tokens.get(1) {
            Some(Token::Keyword(Keyword::Int)) => CType::Int,
            Some(Token::Ident(n)) => CType::Named(n.clone()),
            _ => return Err(self.error_at(code, 0, code.len(), "expected typedef type")),
        };
        let name = match tokens.get(2) {
            Some(Token::Ident(n)) => n.clone(),
            _ => return Err(self.error_at(code, 0, code.len(), "expected typedef name")),
        };
        if tokens.get(3) != Some(&Token::Semi) {
            return Err(self.error_at(code, 0, code.len(), "expected `;`"));
        }
        Ok((name, ty))
    }

    fn error_at(
        &self,
        code: &str,
        offset: usize,
        length: usize,
        message: impl Into<String>,
    ) -> ParseError {
        let base = self.source.find(code).unwrap_or(0);
        ParseError::new(
            self.source_name.clone(),
            self.source.clone(),
            base + offset,
            length.max(1),
            message,
        )
    }

    fn node_text<'a>(&self, node: &'a PPNode) -> &'a str {
        match node {
            PPNode::Code { text, .. } => text,
            PPNode::Conditional(_) => {
                panic!("expected a plain code line, found a conditional region")
            }
        }
    }

    fn node_provenance(&self, node: &PPNode) -> Provenance {
        match node {
            PPNode::Code { provenance, .. } => *provenance,
            PPNode::Conditional(_) => panic!("conditional regions have no single provenance"),
        }
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
                Declarator::Grouped(Box::new(declarator))
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
                    assert!(
                        self.take(Token::RBracket),
                        "expected `]` in array declarator"
                    );
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
        assert!(
            self.take(Token::LParen),
            "expected `(` in function declarator"
        );
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

impl Parser {
    fn parse_function(&self, nodes: &[PPNode]) -> Result<(FunctionDecl, usize), ParseError> {
        let provenance = self.node_provenance(&nodes[0]);
        let code = self.node_text(&nodes[0]);
        let sig_tokens = lex(code);
        if sig_tokens.first() != Some(&Token::Keyword(Keyword::Int)) {
            return Err(self.error_at(code, 0, code.len(), "expected function return type"));
        }
        let name = match sig_tokens.get(1) {
            Some(Token::Ident(n)) => n.clone(),
            _ => return Err(self.error_at(code, 0, code.len(), "expected function name")),
        };
        if sig_tokens.get(2) != Some(&Token::LParen) {
            return Err(self.error_at(code, code.len().saturating_sub(1), 1, "expected `(`"));
        }
        if sig_tokens.get(3) != Some(&Token::RParen) {
            let offset = code.find('{').unwrap_or(code.len().saturating_sub(1));
            return Err(self.error_at(code, offset, 1, "expected `)`"));
        }
        if sig_tokens.get(4) != Some(&Token::LBrace) {
            return Err(self.error_at(
                code,
                code.len().saturating_sub(1),
                1,
                "expected function body",
            ));
        }

        let close_idx = nodes[1..]
            .iter()
            .position(
                |n| matches!(n, PPNode::Code { text, .. } if lex(text) == vec![Token::RBrace]),
            )
            .ok_or_else(|| self.error_at(code, code.len().saturating_sub(1), 1, "expected `}`"))?
            + 1;

        let body = self.parse_stmt_list(&nodes[1..close_idx]);
        Ok((
            FunctionDecl {
                ret_type: Type::Int,
                name,
                body,
                provenance,
            },
            close_idx + 1,
        ))
    }

    fn parse_stmt_list(&self, nodes: &[PPNode]) -> Vec<Stmt> {
        let mut stmts = Vec::new();
        for node in nodes {
            match node {
                PPNode::Code { text, .. } if !lex(text).is_empty() => {
                    stmts.extend(self.parse_stmts_from_code(text))
                }
                PPNode::Code { .. } => {}
                PPNode::Conditional(cond) => {
                    let branches = cond
                        .branches
                        .iter()
                        .map(|(c, body)| (c.clone(), self.parse_stmt_list(body)))
                        .collect();
                    stmts.push(Stmt::Conditional(Conditional { branches }));
                }
            }
        }
        stmts
    }

    fn parse_stmts_from_code(&self, code: &str) -> Vec<Stmt> {
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
}
