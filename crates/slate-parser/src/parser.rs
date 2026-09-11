use crate::ast::*;
use crate::const_expr;
use crate::error::{FrontendError, ParseError};
use crate::files::{Files, SearchPaths, display_path};
use crate::lexer::{Keyword, Token, lex};
use crate::pp::{PPConditional, PPNode, Preprocessor};
use crate::reachability::filter_translation_unit;
use std::collections::HashSet;
use std::path::Path;

#[derive(Clone)]
pub struct Parser {
    search: SearchPaths,
    source_name: String,
    source: String,
    typedef_names: HashSet<String>,
}

impl Parser {
    pub fn new(search: SearchPaths) -> Self {
        Self {
            search,
            source_name: "<source>".into(),
            source: String::new(),
            typedef_names: HashSet::new(),
        }
    }

    pub fn parse_source(&mut self, src: &str) -> Result<TranslationUnit, FrontendError> {
        self.source_name = "<main>".into();
        self.source = src.into();
        let search = self.search.clone();
        let mut pp = Preprocessor::new(&search);
        let nodes = pp.parse_str("<main>", src).map_err(FrontendError::PP)?;
        self.parse_nodes(&nodes).map_err(FrontendError::Parse)
    }

    pub fn parse_file(&mut self, path: &Path) -> Result<(TranslationUnit, Files), FrontendError> {
        self.source_name = display_path(path);
        self.source = std::fs::read_to_string(path)
            .map_err(|error| {
                ParseError::new(
                    self.source_name.clone(),
                    "",
                    0,
                    0,
                    format!("failed to read source: {error}"),
                )
            })
            .map_err(FrontendError::Parse)?;
        let search = self.search.clone();
        let mut pp = Preprocessor::new(&search);
        let nodes = pp.parse_file(path).map_err(FrontendError::PP)?;
        let ast = self.parse_nodes(&nodes);
        ast.map(|ast| (ast, pp.files)).map_err(FrontendError::Parse)
    }

    pub fn parse_declaration(&self, code: &str) -> Result<Declaration, ParseError> {
        let tokens = lex(code);
        let mut parser = DeclaratorParser {
            tokens: &tokens,
            pos: 0,
        };
        let (mut attributes, position) = parse_attribute_groups(parser.tokens, parser.pos)
            .map_err(|error| self.error_at(code, 0, code.len(), error))?;
        parser.pos = position;
        let mut qualifiers = Qualifiers::default();
        let mut storage = StorageClass::None;
        let mut is_inline = false;
        let mut is_noreturn = false;
        loop {
            if let Some(qualifier) = parser.take_qualifier() {
                match qualifier {
                    Keyword::Const => qualifiers.is_const = true,
                    Keyword::Volatile => qualifiers.is_volatile = true,
                    Keyword::Restrict => qualifiers.is_restrict = true,
                    Keyword::Atomic => qualifiers.is_atomic = true,
                    _ => unreachable!(),
                }
                continue;
            }
            if parser.matches(Token::Keyword(Keyword::Inline)) {
                is_inline = true;
                continue;
            }
            if parser.matches(Token::Keyword(Keyword::Noreturn)) {
                is_noreturn = true;
                continue;
            }
            if let Some(Token::Keyword(keyword)) = parser.peek() {
                let next_storage = match *keyword {
                    Keyword::Typedef => StorageClass::Typedef,
                    Keyword::Extern => StorageClass::Extern,
                    Keyword::Static => StorageClass::Static,
                    Keyword::Auto => StorageClass::Auto,
                    Keyword::Register => StorageClass::Register,
                    Keyword::ThreadLocal => StorageClass::ThreadLocal,
                    _ => break,
                };
                if storage != StorageClass::None {
                    return Err(self.error_at(code, 0, code.len(), "multiple storage classes"));
                }
                storage = next_storage;
                parser.matches(Token::Keyword(*keyword));
            } else {
                break;
            }
        }
        let ty = parser.parse_base_type();
        let declarator = if parser.peek() == Some(&Token::Semi) {
            Declarator::Abstract
        } else {
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
            parser.parse_declarator(false)
        };
        let (trailing_attributes, position) = parse_attribute_groups(parser.tokens, parser.pos)
            .map_err(|error| self.error_at(code, 0, code.len(), error))?;
        parser.pos = position;
        attributes.extend(trailing_attributes);
        let initializer = if parser.matches(Token::Equal) {
            Some(parser.parse_initializer())
        } else {
            None
        };
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
                qualifiers,
                storage,
                is_inline,
                is_noreturn,
            },
            declarator,
            initializer,
            attributes,
        })
    }

    fn parse_nodes(&mut self, nodes: &[PPNode]) -> Result<TranslationUnit, ParseError> {
        let ast = filter_translation_unit(
            &TranslationUnit {
                decls: self.parse_decls(nodes)?,
            },
            FileId(0),
        );
        Ok(ast)
    }

    fn parse_decls(&mut self, nodes: &[PPNode]) -> Result<Vec<Decl>, ParseError> {
        let mut decls = Vec::new();
        let mut i = 0;
        while i < nodes.len() {
            if matches!(&nodes[i], PPNode::Code { text, .. } if lex(text).is_empty()) {
                i += 1;
                continue;
            }
            let (decl, consumed) = if let [
                PPNode::Code { text, provenance },
                PPNode::Conditional(cond),
                ..,
            ] = &nodes[i..]
                && text.trim_end().ends_with('=')
            {
                (
                    self.parse_conditional_initializer(text, *provenance, cond)?,
                    2,
                )
            } else {
                self.parse_top_level_item(&nodes[i..])?
            };
            self.record_typedefs(&decl);
            decls.push(decl);
            i += consumed;
        }
        Ok(decls)
    }

    fn parse_conditional_initializer(
        &self,
        prefix: &str,
        provenance: Provenance,
        conditional: &PPConditional,
    ) -> Result<Decl, ParseError> {
        let declaration = self.parse_declaration(&format!("{prefix} 0;"))?;
        let initializer = Initializer::Conditional(Conditional {
            branches: conditional
                .branches
                .iter()
                .map(|(condition, nodes)| {
                    let Some(PPNode::Code { text, .. }) = nodes.first() else {
                        panic!("conditional initializer branch is empty")
                    };
                    let expression = text.trim_end_matches(';').trim();
                    let mut parser = DeclaratorParser {
                        tokens: &lex(expression),
                        pos: 0,
                    };
                    (condition.clone(), Box::new(parser.parse_initializer()))
                })
                .collect(),
        });
        Ok(Decl::Declaration {
            declaration: Declaration {
                initializer: Some(initializer),
                ..declaration
            },
            provenance,
        })
    }

    fn parse_top_level_item(&mut self, nodes: &[PPNode]) -> Result<(Decl, usize), ParseError> {
        match &nodes[0] {
            PPNode::Code { text, provenance } if text.trim_start().starts_with("typedef") => {
                let (name, ty, attributes) = self.parse_typedef_line(text)?;
                Ok((
                    Decl::Typedef {
                        name,
                        ty,
                        provenance: *provenance,
                        attributes,
                    },
                    1,
                ))
            }
            PPNode::Code { .. } => {
                let tokens = lex(self.node_text(&nodes[0]));
                if matches!(
                    tokens.first(),
                    Some(Token::Keyword(
                        Keyword::Struct | Keyword::Union | Keyword::Enum
                    ))
                ) && tokens.contains(&Token::LBrace)
                    && !tokens.contains(&Token::Equal)
                {
                    return self.parse_tag_definition(nodes);
                }
                if tokens.contains(&Token::Semi)
                    && (!tokens.contains(&Token::LBrace) || tokens.contains(&Token::Equal))
                {
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

    fn parse_tag_definition(&self, nodes: &[PPNode]) -> Result<(Decl, usize), ParseError> {
        let code = self.node_text(&nodes[0]);
        let tokens = lex(code);
        let kind = match tokens.first() {
            Some(Token::Keyword(Keyword::Struct)) => TagKind::Struct,
            Some(Token::Keyword(Keyword::Union)) => TagKind::Union,
            Some(Token::Keyword(Keyword::Enum)) => TagKind::Enum,
            _ => return Err(self.error_at(code, 0, code.len(), "expected record or enum")),
        };
        let (mut attributes, name_index) = parse_record_attributes(&tokens)
            .map_err(|error| self.error_at(code, 0, code.len(), error))?;
        let name = match tokens.get(name_index) {
            Some(Token::Ident(name)) => Some(name.clone()),
            Some(Token::LBrace) => None,
            _ => return Err(self.error_at(code, 0, code.len(), "expected tag name or `{`")),
        };
        let close = nodes[1..]
            .iter()
            .position(|node| matches!(node, PPNode::Code { text, .. } if lex(text).first() == Some(&Token::RBrace)))
            .ok_or_else(|| self.error_at(code, 0, code.len(), "expected `}`"))?
            + 1;
        if let PPNode::Code { text, .. } = &nodes[close] {
            let closing_tokens = lex(text);
            if closing_tokens.first() == Some(&Token::RBrace) {
                let (trailing, _) = parse_attribute_groups(&closing_tokens, 1)
                    .map_err(|error| self.error_at(text, 0, text.len(), error))?;
                attributes.extend(trailing);
            }
        }
        let consumed = close + 1;
        let provenance = self.node_provenance(&nodes[0]);
        if kind == TagKind::Enum {
            let mut enumerators = Vec::new();
            for node in &nodes[1..close] {
                let text = self.node_text(node);
                let tokens = lex(text);
                if tokens.is_empty() {
                    continue;
                }
                let Some(Token::Ident(name)) = tokens.first() else {
                    return Err(self.error_at(text, 0, text.len(), "expected enumerator"));
                };
                let value = match tokens.get(1) {
                    Some(Token::Comma) | None => None,
                    Some(Token::Equal) => {
                        let end = tokens
                            .iter()
                            .position(|token| token == &Token::Comma)
                            .unwrap_or(tokens.len());
                        let value =
                            const_expr::Parser::evaluate(&tokens[2..end]).map_err(|error| {
                                self.error_at(text, 0, text.len(), error.to_string())
                            })?;
                        Some(Expr::IntLit(value))
                    }
                    _ => {
                        return Err(self.error_at(
                            text,
                            0,
                            text.len(),
                            "expected enumerator value",
                        ));
                    }
                };
                enumerators.push(Enumerator {
                    name: name.clone(),
                    value,
                });
            }
            Ok((
                Decl::Enum(EnumDecl {
                    name,
                    enumerators,
                    provenance,
                }),
                consumed,
            ))
        } else {
            let mut fields = Vec::new();
            for node in &nodes[1..close] {
                let text = self.node_text(node);
                if lex(text).is_empty() {
                    continue;
                }
                fields.push(FieldDecl {
                    declaration: self.parse_declaration(text)?,
                    provenance: self.node_provenance(node),
                });
            }
            Ok((
                Decl::Record(RecordDecl {
                    kind,
                    name,
                    fields,
                    provenance,
                    attributes,
                }),
                consumed,
            ))
        }
    }

    fn parse_top_level_conditional(&mut self, cond: &PPConditional) -> Result<Decl, ParseError> {
        let base_typedefs = self.typedef_names.clone();
        let mut branch_typedefs = Vec::new();
        let mut branches = Vec::new();
        for (condition, body) in &cond.branches {
            let mut branch_parser = self.clone();
            branch_parser.typedef_names = base_typedefs.clone();
            let decls = branch_parser.parse_decls(body)?;
            branch_typedefs.push(branch_parser.typedef_names);
            branches.push((condition.clone(), decls));
        }
        if let Some(common) = branch_typedefs.into_iter().reduce(|mut common, names| {
            common.retain(|name| names.contains(name));
            common
        }) {
            self.typedef_names = common;
        }
        Ok(Decl::Conditional(Conditional { branches }))
    }

    fn record_typedefs(&mut self, decl: &Decl) {
        if let Decl::Typedef { name, .. } = decl {
            self.typedef_names.insert(name.clone());
        }
    }

    fn parse_typedef_line(
        &self,
        code: &str,
    ) -> Result<(String, CType, Vec<Attribute>), ParseError> {
        let declaration = self.parse_declaration(code)?;
        let name = declaration
            .declarator
            .name()
            .ok_or_else(|| self.error_at(code, 0, code.len(), "expected typedef name"))?;
        let ty = if declaration.specifiers.qualifiers == Qualifiers::default() {
            declaration.specifiers.ty
        } else {
            CType::Qualified {
                qualifiers: declaration.specifiers.qualifiers,
                ty: Box::new(declaration.specifiers.ty),
            }
        };
        Ok((name.to_string(), ty, declaration.attributes))
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

fn parse_record_attributes(tokens: &[Token]) -> Result<(Vec<Attribute>, usize), String> {
    parse_attribute_groups(tokens, 1)
}

fn parse_attribute_groups(
    tokens: &[Token],
    mut position: usize,
) -> Result<(Vec<Attribute>, usize), String> {
    let mut attributes = Vec::new();
    loop {
        if tokens.get(position) == Some(&Token::Ident("__attribute__".into())) {
            position += 1;
            if tokens.get(position) != Some(&Token::LParen)
                || tokens.get(position + 1) != Some(&Token::LParen)
            {
                return Err("expected `((` after __attribute__".into());
            }
            position += 2;
            loop {
                let Some(Token::Ident(name)) = tokens.get(position) else {
                    return Err("expected attribute name".into());
                };
                let name = name.clone();
                position += 1;
                let arguments = if tokens.get(position) == Some(&Token::LParen) {
                    position += 1;
                    let start = position;
                    let mut depth = 0;
                    while let Some(token) = tokens.get(position) {
                        match token {
                            Token::LParen => depth += 1,
                            Token::RParen if depth == 0 => break,
                            Token::RParen => depth -= 1,
                            _ => {}
                        }
                        position += 1;
                    }
                    if tokens.get(position) != Some(&Token::RParen) {
                        return Err("expected `)` after attribute arguments".into());
                    }
                    let arguments = tokens[start..position].to_vec();
                    position += 1;
                    arguments
                } else {
                    Vec::new()
                };
                attributes.push(parse_attribute(&name, &arguments)?);
                if tokens.get(position) == Some(&Token::Comma) {
                    position += 1;
                    continue;
                }
                if tokens.get(position) != Some(&Token::RParen)
                    || tokens.get(position + 1) != Some(&Token::RParen)
                {
                    return Err("expected `))` after attributes".into());
                }
                position += 2;
                break;
            }
        } else if tokens.get(position) == Some(&Token::LBracket)
            && tokens.get(position + 1) == Some(&Token::LBracket)
        {
            position += 2;
            loop {
                let Some(Token::Ident(first)) = tokens.get(position) else {
                    return Err("expected C23 attribute name".into());
                };
                let mut name = first.clone();
                position += 1;
                if tokens.get(position) == Some(&Token::Colon) {
                    position += 1;
                    if tokens.get(position) != Some(&Token::Colon) {
                        return Err("expected `::` in attribute name".into());
                    }
                    position += 1;
                    let Some(Token::Ident(last)) = tokens.get(position) else {
                        return Err("expected attribute name after `::`".into());
                    };
                    name.push_str("::");
                    name.push_str(last);
                    position += 1;
                }
                let arguments = if tokens.get(position) == Some(&Token::LParen) {
                    position += 1;
                    let start = position;
                    while tokens.get(position) != Some(&Token::RParen) {
                        if tokens.get(position).is_none() {
                            return Err("expected `)` after attribute arguments".into());
                        }
                        position += 1;
                    }
                    let arguments = tokens[start..position].to_vec();
                    position += 1;
                    arguments
                } else {
                    Vec::new()
                };
                attributes.push(parse_attribute(&name, &arguments)?);
                if tokens.get(position) == Some(&Token::Comma) {
                    position += 1;
                    continue;
                }
                if tokens.get(position) != Some(&Token::RBracket)
                    || tokens.get(position + 1) != Some(&Token::RBracket)
                {
                    return Err("expected `]]` after C23 attributes".into());
                }
                position += 2;
                break;
            }
        } else {
            break;
        }
    }
    Ok((attributes, position))
}

fn parse_attribute(name: &str, arguments: &[Token]) -> Result<Attribute, String> {
    let canonical_name = name
        .strip_prefix("__")
        .and_then(|name| name.strip_suffix("__"))
        .unwrap_or(name);
    let single_string = || match arguments {
        [Token::StringLit(value)] => Some(value.clone()),
        _ => None,
    };
    let single_ident = || match arguments {
        [Token::Ident(value)] => Some(value.clone()),
        _ => None,
    };
    let single_int = || match arguments {
        [Token::IntLit(value)] => Some(*value),
        _ => None,
    };
    let integers = || {
        arguments
            .split(|token| *token == Token::Comma)
            .map(|tokens| match tokens {
                [Token::IntLit(value)] => Ok(*value),
                _ => Err(()),
            })
            .collect::<Result<Vec<_>, _>>()
            .ok()
    };
    match canonical_name {
        "packed" if arguments.is_empty() => Ok(Attribute::Packed),
        "aligned" => Ok(match single_int() {
            Some(value) => Attribute::Aligned(const_expr::ConstExpr::Integer(value)),
            None if !arguments.is_empty() => {
                Attribute::Aligned(parse_attribute_expression(arguments)?)
            }
            None => invalid_attribute(name, arguments),
        }),
        "vector_size" => Ok(match single_int() {
            Some(value) => Attribute::VectorSize(const_expr::ConstExpr::Integer(value)),
            None if !arguments.is_empty() => {
                Attribute::VectorSize(parse_attribute_expression(arguments)?)
            }
            None => invalid_attribute(name, arguments),
        }),
        "mode" => Ok(single_ident()
            .map(Attribute::Mode)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "visibility" => Ok(single_string()
            .map(Attribute::Visibility)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "section" => Ok(single_string()
            .map(Attribute::Section)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "annotate" => Ok(single_string()
            .map(Attribute::Annotate)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "target" => Ok(single_string()
            .map(Attribute::Target)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "alias" => Ok(single_string()
            .map(Attribute::Alias)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "weakref" => Ok(single_string()
            .map(Attribute::WeakRef)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "nonnull" => Ok(integers()
            .map(Attribute::NonNull)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "assume_aligned" => Ok(
            match arguments
                .split(|token| *token == Token::Comma)
                .map(parse_attribute_expression)
                .collect::<Result<Vec<_>, _>>()
            {
                Ok(values) if !values.is_empty() => Attribute::AssumeAligned(values),
                _ => invalid_attribute(name, arguments),
            },
        ),
        "alloc_size" => Ok(
            match arguments
                .split(|token| *token == Token::Comma)
                .map(parse_attribute_expression)
                .collect::<Result<Vec<_>, _>>()
            {
                Ok(values) if !values.is_empty() => Attribute::AllocSize(values),
                _ => invalid_attribute(name, arguments),
            },
        ),
        "alloc_align" => Ok(match parse_attribute_expression(arguments) {
            Ok(value) if !arguments.is_empty() => Attribute::AllocAlign(value),
            _ => invalid_attribute(name, arguments),
        }),
        "cleanup" => Ok(single_ident()
            .map(Attribute::Cleanup)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "weak" if arguments.is_empty() => Ok(Attribute::Weak),
        "used" if arguments.is_empty() => Ok(Attribute::Used),
        "retain" if arguments.is_empty() => Ok(Attribute::Retain),
        "noinline" if arguments.is_empty() => Ok(Attribute::NoInline),
        "always_inline" if arguments.is_empty() => Ok(Attribute::AlwaysInline),
        "noreturn" if arguments.is_empty() => Ok(Attribute::NoReturn),
        "constructor" if arguments.is_empty() => Ok(Attribute::Constructor),
        "destructor" if arguments.is_empty() => Ok(Attribute::Destructor),
        "malloc" if arguments.is_empty() => Ok(Attribute::Malloc),
        "returns_nonnull" if arguments.is_empty() => Ok(Attribute::ReturnsNonNull),
        "warn_unused_result" if arguments.is_empty() => Ok(Attribute::WarnUnusedResult),
        "sentinel" if arguments.is_empty() => Ok(Attribute::Sentinel(None)),
        "sentinel" => Ok(single_int()
            .map(|value| Attribute::Sentinel(Some(value)))
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "cold" if arguments.is_empty() => Ok(Attribute::Cold),
        "flatten" if arguments.is_empty() => Ok(Attribute::Flatten),
        "hot" if arguments.is_empty() => Ok(Attribute::Hot),
        "leaf" if arguments.is_empty() => Ok(Attribute::Leaf),
        "noipa" if arguments.is_empty() => Ok(Attribute::NoIpa),
        "noclone" if arguments.is_empty() => Ok(Attribute::NoClone),
        "optimize" if !arguments.is_empty() => Ok(Attribute::Optimize(
            arguments.iter().map(String::from).collect(),
        )),
        "naked" if arguments.is_empty() => Ok(Attribute::Naked),
        "interrupt" if arguments.is_empty() => Ok(Attribute::Interrupt),
        "no_split_stack" if arguments.is_empty() => Ok(Attribute::NoSplitStack),
        "returns_twice" if arguments.is_empty() => Ok(Attribute::ReturnsTwice),
        "cpu_dispatch" => Ok(Attribute::CpuDispatch(attribute_arguments(arguments))),
        "cpu_specific" => Ok(Attribute::CpuSpecific(attribute_arguments(arguments))),
        "target_clones" => Ok(Attribute::TargetClones(attribute_arguments(arguments))),
        "ifunc" => Ok(single_string()
            .map(Attribute::Ifunc)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "dllimport" if arguments.is_empty() => Ok(Attribute::DllImport),
        "weak_import" if arguments.is_empty() => Ok(Attribute::WeakImport),
        "tls_model" => Ok(single_string()
            .map(Attribute::TlsModel)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "ms_struct" if arguments.is_empty() => Ok(Attribute::MsStruct),
        "stdcall" if arguments.is_empty() => Ok(Attribute::Stdcall),
        "nomips16" if arguments.is_empty() => Ok(Attribute::NoMips16),
        "availability" => Ok(Attribute::Availability(attribute_arguments(arguments))),
        "ext_vector_type" => Ok(match parse_attribute_expression(arguments) {
            Ok(value) => Attribute::ExtVectorType(value),
            Err(_) => invalid_attribute(name, arguments),
        }),
        "scalar_storage_order" => Ok(single_string()
            .map(Attribute::ScalarStorageOrder)
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "transparent_union" if arguments.is_empty() => Ok(Attribute::TransparentUnion),
        "format" => Ok(Attribute::Format(attribute_arguments(arguments))),
        "format_arg" => Ok(Attribute::FormatArg(attribute_arguments(arguments))),
        "gcc_struct" if arguments.is_empty() => Ok(Attribute::GccStruct),
        "common" if arguments.is_empty() => Ok(Attribute::Common),
        "nocommon" if arguments.is_empty() => Ok(Attribute::NoCommon),
        "pure" if arguments.is_empty() => Ok(Attribute::Pure),
        "const" if arguments.is_empty() => Ok(Attribute::Const),
        "may_alias" if arguments.is_empty() => Ok(Attribute::MayAlias),
        "deprecated" if arguments.is_empty() => Ok(Attribute::Deprecated(None)),
        "deprecated" => Ok(single_string()
            .map(|value| Attribute::Deprecated(Some(value)))
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "nodiscard" if arguments.is_empty() => Ok(Attribute::NoDiscard(None)),
        "nodiscard" => Ok(single_string()
            .map(|value| Attribute::NoDiscard(Some(value)))
            .unwrap_or_else(|| invalid_attribute(name, arguments))),
        "maybe_unused" if arguments.is_empty() => Ok(Attribute::MaybeUnused),
        "fallthrough" if arguments.is_empty() => Ok(Attribute::Fallthrough),
        _ if canonical_name.is_attribute_name() => Ok(invalid_attribute(name, arguments)),
        _ => Ok(Attribute::Unknown {
            name: name.into(),
            arguments: arguments.iter().map(String::from).collect(),
        }),
    }
}

fn invalid_attribute(name: &str, arguments: &[Token]) -> Attribute {
    Attribute::Invalid {
        name: name.into(),
        arguments: arguments.iter().map(String::from).collect(),
    }
}

fn attribute_arguments(arguments: &[Token]) -> Vec<String> {
    arguments
        .split(|token| *token == Token::Comma)
        .map(|tokens| {
            tokens
                .iter()
                .map(String::from)
                .collect::<Vec<_>>()
                .join(" ")
        })
        .collect()
}

fn parse_attribute_expression(arguments: &[Token]) -> Result<const_expr::ConstExpr, String> {
    const_expr::Parser::parse(arguments).map_err(|error| error.to_string())
}

trait AttributeName {
    fn is_attribute_name(&self) -> bool;
}

impl AttributeName for str {
    fn is_attribute_name(&self) -> bool {
        matches!(
            self,
            "aligned"
                | "vector_size"
                | "mode"
                | "visibility"
                | "section"
                | "annotate"
                | "target"
                | "alias"
                | "weakref"
                | "nonnull"
                | "weak"
                | "used"
                | "retain"
                | "noinline"
                | "always_inline"
                | "noreturn"
                | "constructor"
                | "destructor"
                | "malloc"
                | "assume_aligned"
                | "alloc_size"
                | "alloc_align"
                | "cleanup"
                | "returns_nonnull"
                | "warn_unused_result"
                | "sentinel"
                | "cold"
                | "flatten"
                | "hot"
                | "leaf"
                | "noipa"
                | "noclone"
                | "optimize"
                | "naked"
                | "interrupt"
                | "no_split_stack"
                | "returns_twice"
                | "cpu_dispatch"
                | "cpu_specific"
                | "target_clones"
                | "ifunc"
                | "dllimport"
                | "weak_import"
                | "tls_model"
                | "ms_struct"
                | "stdcall"
                | "nomips16"
                | "availability"
                | "ext_vector_type"
                | "scalar_storage_order"
                | "transparent_union"
                | "format"
                | "format_arg"
                | "gcc_struct"
                | "common"
                | "nocommon"
                | "pure"
                | "const"
                | "may_alias"
                | "deprecated"
                | "nodiscard"
                | "maybe_unused"
                | "fallthrough"
        )
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

    fn matches(&mut self, expected: Token) -> bool {
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
            Token::Keyword(Keyword::Bool) => CType::Bool,
            Token::Keyword(Keyword::BFloat16) => CType::Floating(FloatingType::BFloat16),
            Token::Keyword(Keyword::Char) => CType::Integer(IntegerType::Char { signed: None }),
            Token::Keyword(Keyword::Double) => {
                if self.matches(Token::Keyword(Keyword::Complex)) {
                    CType::Complex(Box::new(CType::Floating(FloatingType::Double)))
                } else {
                    CType::Floating(FloatingType::Double)
                }
            }
            Token::Keyword(Keyword::Float) => {
                if self.matches(Token::Keyword(Keyword::Complex)) {
                    CType::Complex(Box::new(CType::Floating(FloatingType::Float)))
                } else {
                    CType::Floating(FloatingType::Float)
                }
            }
            Token::Keyword(Keyword::Float16) => CType::Floating(FloatingType::Float16),
            Token::Keyword(Keyword::Fp16) => CType::Floating(FloatingType::Fp16),
            Token::Keyword(Keyword::Float64x) => CType::Floating(FloatingType::Float64x),
            Token::Keyword(Keyword::Float128) => CType::Floating(FloatingType::Float128),
            Token::Keyword(Keyword::Float128Ext) => CType::Floating(FloatingType::Float128Ext),
            Token::Keyword(Keyword::Int) => CType::Integer(IntegerType::Ranked {
                rank: IntegerRank::Int,
                signed: true,
            }),
            Token::Keyword(Keyword::Int128) => CType::Integer(IntegerType::Ranked {
                rank: IntegerRank::Int128,
                signed: true,
            }),
            Token::Keyword(Keyword::Long) => {
                if self.matches(Token::Keyword(Keyword::Long)) {
                    self.matches(Token::Keyword(Keyword::Int));
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::LongLong,
                        signed: true,
                    })
                } else if self.matches(Token::Keyword(Keyword::Double)) {
                    if self.matches(Token::Keyword(Keyword::Complex)) {
                        CType::Complex(Box::new(CType::Floating(FloatingType::LongDouble)))
                    } else {
                        CType::Floating(FloatingType::LongDouble)
                    }
                } else {
                    self.matches(Token::Keyword(Keyword::Int));
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Long,
                        signed: true,
                    })
                }
            }
            Token::Keyword(Keyword::Short) => {
                self.matches(Token::Keyword(Keyword::Int));
                CType::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Short,
                    signed: true,
                })
            }
            Token::Keyword(Keyword::Signed) => match self.peek() {
                Some(Token::Keyword(Keyword::Char)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Char { signed: Some(true) })
                }
                Some(Token::Keyword(Keyword::Short)) => {
                    self.pos += 1;
                    self.matches(Token::Keyword(Keyword::Int));
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Short,
                        signed: true,
                    })
                }
                Some(Token::Keyword(Keyword::Long)) => {
                    self.pos += 1;
                    if self.matches(Token::Keyword(Keyword::Long)) {
                        self.matches(Token::Keyword(Keyword::Int));
                        CType::Integer(IntegerType::Ranked {
                            rank: IntegerRank::LongLong,
                            signed: true,
                        })
                    } else {
                        self.matches(Token::Keyword(Keyword::Int));
                        CType::Integer(IntegerType::Ranked {
                            rank: IntegerRank::Long,
                            signed: true,
                        })
                    }
                }
                Some(Token::Keyword(Keyword::Int128)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Int128,
                        signed: true,
                    })
                }
                Some(Token::Keyword(Keyword::BitInt)) => {
                    self.pos += 1;
                    self.parse_bit_int(false)
                }
                Some(Token::Keyword(Keyword::Int)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Int,
                        signed: true,
                    })
                }
                _ => CType::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Int,
                    signed: true,
                }),
            },
            Token::Keyword(Keyword::Unsigned) => match self.peek() {
                Some(Token::Keyword(Keyword::Char)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Char {
                        signed: Some(false),
                    })
                }
                Some(Token::Keyword(Keyword::Short)) => {
                    self.pos += 1;
                    self.matches(Token::Keyword(Keyword::Int));
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Short,
                        signed: false,
                    })
                }
                Some(Token::Keyword(Keyword::Long)) => {
                    self.pos += 1;
                    if self.matches(Token::Keyword(Keyword::Long)) {
                        self.matches(Token::Keyword(Keyword::Int));
                        CType::Integer(IntegerType::Ranked {
                            rank: IntegerRank::LongLong,
                            signed: false,
                        })
                    } else {
                        self.matches(Token::Keyword(Keyword::Int));
                        CType::Integer(IntegerType::Ranked {
                            rank: IntegerRank::Long,
                            signed: false,
                        })
                    }
                }
                Some(Token::Keyword(Keyword::Int128)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Int128,
                        signed: false,
                    })
                }
                Some(Token::Keyword(Keyword::BitInt)) => {
                    self.pos += 1;
                    self.parse_bit_int(true)
                }
                Some(Token::Keyword(Keyword::Int)) => {
                    self.pos += 1;
                    CType::Integer(IntegerType::Ranked {
                        rank: IntegerRank::Int,
                        signed: false,
                    })
                }
                _ => CType::Integer(IntegerType::Ranked {
                    rank: IntegerRank::Int,
                    signed: false,
                }),
            },
            Token::Keyword(Keyword::Void) => CType::Void,
            Token::Keyword(Keyword::Complex) => {
                CType::Complex(Box::new(CType::Floating(FloatingType::Double)))
            }
            Token::Keyword(Keyword::BitInt) => self.parse_bit_int(false),
            Token::Keyword(Keyword::Struct) => {
                let name = match self.tokens.get(self.pos) {
                    Some(Token::Ident(name)) => name.clone(),
                    _ => panic!("expected struct tag name"),
                };
                self.pos += 1;
                CType::Tagged {
                    kind: TagKind::Struct,
                    name: Some(name),
                }
            }
            Token::Keyword(Keyword::Union) => {
                let name = match self.tokens.get(self.pos) {
                    Some(Token::Ident(name)) => name.clone(),
                    _ => panic!("expected union tag name"),
                };
                self.pos += 1;
                CType::Tagged {
                    kind: TagKind::Union,
                    name: Some(name),
                }
            }
            Token::Keyword(Keyword::Enum) => {
                let name = match self.tokens.get(self.pos) {
                    Some(Token::Ident(name)) => name.clone(),
                    _ => panic!("expected enum tag name"),
                };
                self.pos += 1;
                CType::Tagged {
                    kind: TagKind::Enum,
                    name: Some(name),
                }
            }
            Token::Ident(name) => CType::Named(name),
            other => panic!("expected declaration type, found {other:?}"),
        }
    }

    fn parse_bit_int(&mut self, is_unsigned: bool) -> CType {
        assert!(self.matches(Token::LParen), "expected `(` after _BitInt");
        let start = self.pos;
        while self.peek() != Some(&Token::RParen) {
            assert!(self.peek().is_some(), "expected `)` after _BitInt width");
            self.pos += 1;
        }
        let width = const_expr::Parser::parse(&self.tokens[start..self.pos])
            .expect("invalid _BitInt width expression");
        self.pos += 1;
        CType::Integer(IntegerType::BitInt {
            width,
            signed: !is_unsigned,
        })
    }

    fn parse_initializer(&mut self) -> Initializer {
        if self.matches(Token::LBrace) {
            let mut items = Vec::new();
            while !self.matches(Token::RBrace) {
                let mut designators = Vec::new();
                loop {
                    if self.matches(Token::LBracket) {
                        let Some(Token::IntLit(index)) = self.peek().cloned() else {
                            panic!("array designator must be an integer literal")
                        };
                        self.pos += 1;
                        assert!(self.matches(Token::RBracket), "expected `]` in designator");
                        designators.push(Designator::Array(index));
                    } else if self.matches(Token::Dot) {
                        let Some(Token::Ident(name)) = self.peek().cloned() else {
                            panic!("field designator must name a field")
                        };
                        self.pos += 1;
                        designators.push(Designator::Field(name));
                    } else {
                        break;
                    }
                }
                if !designators.is_empty() {
                    assert!(self.matches(Token::Equal), "expected `=` after designator");
                }
                items.push(InitializerItem {
                    designators,
                    value: self.parse_initializer(),
                });
                if !self.matches(Token::Comma) {
                    assert!(
                        self.peek() == Some(&Token::RBrace),
                        "expected `,` in initializer"
                    );
                }
            }
            Initializer::List(items)
        } else {
            let expr = match self.peek().cloned() {
                Some(Token::IntLit(value)) => Expr::IntLit(value),
                Some(Token::StringLit(value)) => Expr::StringLit(value),
                other => panic!("unsupported initializer expression: {other:?}"),
            };
            self.pos += 1;
            Initializer::Expr(expr)
        }
    }

    fn parse_declarator(&mut self, allow_abstract: bool) -> Declarator {
        let mut pointer_qualifiers = Vec::new();
        while self.matches(Token::Star) {
            pointer_qualifiers.push(self.take_qualifiers());
        }
        let mut declarator = match self.peek().cloned() {
            Some(Token::Ident(name)) => {
                self.pos += 1;
                Declarator::Name(name)
            }
            Some(Token::LParen) => {
                self.pos += 1;
                let declarator = self.parse_declarator(allow_abstract);
                assert!(self.matches(Token::RParen), "expected `)` in declarator");
                Declarator::Grouped(Box::new(declarator))
            }
            _ if allow_abstract => Declarator::Abstract,
            _ => panic!("expected declarator"),
        };

        for qualifiers in pointer_qualifiers {
            declarator = Declarator::Pointer {
                qualifiers,
                inner: Box::new(declarator),
            };
        }

        loop {
            declarator = match self.peek() {
                Some(Token::LBracket) => {
                    self.pos += 1;
                    let size = if self.peek() == Some(&Token::RBracket) {
                        ArraySize::Unspecified
                    } else {
                        let start = self.pos;
                        while self.peek() != Some(&Token::RBracket) {
                            assert!(self.peek().is_some(), "expected `]` in array declarator");
                            self.pos += 1;
                        }
                        let value = const_expr::Parser::evaluate(&self.tokens[start..self.pos])
                            .unwrap_or_else(|error| panic!("invalid array bound: {error}"));
                        ArraySize::Expression(Box::new(Expr::IntLit(value)))
                    };
                    assert!(
                        self.matches(Token::RBracket),
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

    fn take_qualifiers(&mut self) -> Qualifiers {
        let mut qualifiers = Qualifiers::default();
        while let Some(qualifier) = self.take_qualifier() {
            match qualifier {
                Keyword::Const => qualifiers.is_const = true,
                Keyword::Volatile => qualifiers.is_volatile = true,
                Keyword::Restrict => qualifiers.is_restrict = true,
                Keyword::Atomic => qualifiers.is_atomic = true,
                _ => unreachable!(),
            }
        }
        qualifiers
    }

    fn take_qualifier(&mut self) -> Option<Keyword> {
        let Some(Token::Keyword(keyword)) = self.peek() else {
            return None;
        };
        if !matches!(
            keyword,
            Keyword::Const | Keyword::Volatile | Keyword::Restrict | Keyword::Atomic
        ) {
            return None;
        }
        let keyword = *keyword;
        self.matches(Token::Keyword(keyword));
        Some(keyword)
    }

    fn parse_parameters(&mut self) -> (Vec<Parameter>, bool) {
        assert!(
            self.matches(Token::LParen),
            "expected `(` in function declarator"
        );
        if self.matches(Token::RParen) {
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
            if self.matches(Token::Ellipsis) {
                variadic = true;
                assert!(self.matches(Token::RParen), "expected `)` after `...`");
                break;
            }
            let ty = self.parse_base_type();
            let declarator = match self.peek() {
                Some(Token::Comma) | Some(Token::RParen) => None,
                _ => Some(self.parse_declarator(true)),
            };
            let attributes = parse_attribute_groups(self.tokens, self.pos)
                .map(|(attributes, position)| {
                    self.pos = position;
                    attributes
                })
                .unwrap_or_else(|error| panic!("{error}"));
            parameters.push(Parameter {
                ty,
                declarator,
                attributes,
            });
            if self.matches(Token::RParen) {
                break;
            }
            assert!(
                self.matches(Token::Comma),
                "expected `,` between parameters"
            );
        }
        (parameters, variadic)
    }
}

impl Parser {
    fn parse_function(&self, nodes: &[PPNode]) -> Result<(FunctionDecl, usize), ParseError> {
        let provenance = self.node_provenance(&nodes[0]);
        let code = self.node_text(&nodes[0]);
        let sig_tokens = lex(code);
        let (mut attributes, mut index) = parse_attribute_groups(&sig_tokens, 0)
            .map_err(|error| self.error_at(code, 0, code.len(), error))?;
        let mut qualifiers = Qualifiers::default();
        let mut storage = StorageClass::None;
        let mut is_inline = false;
        let mut is_noreturn = false;
        loop {
            match sig_tokens.get(index) {
                Some(Token::Keyword(Keyword::Const)) => qualifiers.is_const = true,
                Some(Token::Keyword(Keyword::Volatile)) => qualifiers.is_volatile = true,
                Some(Token::Keyword(Keyword::Restrict)) => qualifiers.is_restrict = true,
                Some(Token::Keyword(Keyword::Atomic)) => qualifiers.is_atomic = true,
                Some(Token::Keyword(Keyword::Inline)) => is_inline = true,
                Some(Token::Keyword(Keyword::Noreturn)) => is_noreturn = true,
                Some(Token::Keyword(keyword)) => {
                    let next_storage = match keyword {
                        Keyword::Extern => StorageClass::Extern,
                        Keyword::Static => StorageClass::Static,
                        Keyword::ThreadLocal => StorageClass::ThreadLocal,
                        _ => break,
                    };
                    if storage != StorageClass::None {
                        return Err(self.error_at(code, 0, code.len(), "multiple storage classes"));
                    }
                    storage = next_storage;
                }
                _ => break,
            }
            index += 1;
        }
        let mut return_type_parser = DeclaratorParser {
            tokens: &sig_tokens,
            pos: index,
        };
        let ret_type = return_type_parser.parse_base_type();
        let name_index = return_type_parser.pos;
        let name = match sig_tokens.get(name_index) {
            Some(Token::Ident(n)) => n.clone(),
            _ => return Err(self.error_at(code, 0, code.len(), "expected function name")),
        };
        if sig_tokens.get(name_index + 1) != Some(&Token::LParen) {
            return Err(self.error_at(code, code.len().saturating_sub(1), 1, "expected `(`"));
        }
        let Some(parameter_close) = sig_tokens[name_index + 2..]
            .iter()
            .position(|token| *token == Token::RParen)
            .map(|position| name_index + 2 + position)
        else {
            return Err(self.error_at(code, code.find('{').unwrap_or(0), 1, "expected `)`"));
        };
        if sig_tokens[name_index + 2..parameter_close].contains(&Token::LBrace) {
            return Err(self.error_at(code, code.find('{').unwrap_or(0), 1, "expected parameter"));
        }
        let mut declarator_parser = DeclaratorParser {
            tokens: &sig_tokens,
            pos: name_index + 1,
        };
        let (parameters, variadic) = declarator_parser.parse_parameters();
        let (signature_attributes, body_index) =
            parse_attribute_groups(&sig_tokens, declarator_parser.pos)
                .map_err(|error| self.error_at(code, 0, code.len(), error))?;
        attributes.extend(signature_attributes);
        if sig_tokens.get(body_index) != Some(&Token::LBrace) {
            return Err(self.error_at(
                code,
                code.len().saturating_sub(1),
                1,
                "expected function body",
            ));
        }

        let close_idx = nodes[1..]
            .iter()
            .position(|n| {
                matches!(n, PPNode::Code { text, .. } if lex(text).first() == Some(&Token::RBrace))
            })
            .ok_or_else(|| self.error_at(code, code.len().saturating_sub(1), 1, "expected `}`"))?
            + 1;

        if let PPNode::Code { text, .. } = &nodes[close_idx] {
            let closing_tokens = lex(text);
            let (trailing_attributes, _) = parse_attribute_groups(&closing_tokens, 1)
                .map_err(|error| self.error_at(text, 0, text.len(), error))?;
            attributes.extend(trailing_attributes);
        }

        let body = self.parse_stmt_list(&nodes[1..close_idx])?;
        Ok((
            FunctionDecl {
                ret_type,
                name,
                parameters,
                variadic,
                body,
                provenance,
                qualifiers,
                storage,
                is_inline,
                is_noreturn,
                attributes,
            },
            close_idx + 1,
        ))
    }

    fn parse_stmt_list(&self, nodes: &[PPNode]) -> Result<Vec<Stmt>, ParseError> {
        let mut stmts = Vec::new();
        for node in nodes {
            match node {
                PPNode::Code { text, .. } if !lex(text).is_empty() => {
                    stmts.extend(self.parse_stmts_from_code(text)?)
                }
                PPNode::Code { .. } => {}
                PPNode::Conditional(cond) => {
                    let branches = cond
                        .branches
                        .iter()
                        .map(|(c, body)| self.parse_stmt_list(body).map(|body| (c.clone(), body)))
                        .collect::<Result<_, _>>()?;
                    stmts.push(Stmt::Conditional(Conditional { branches }));
                }
            }
        }
        Ok(stmts)
    }

    fn parse_stmts_from_code(&self, code: &str) -> Result<Vec<Stmt>, ParseError> {
        let tokens = lex(code);
        self.parse_stmts_from_tokens(code, &tokens)
    }

    fn parse_stmts_from_tokens(
        &self,
        code: &str,
        tokens: &[Token],
    ) -> Result<Vec<Stmt>, ParseError> {
        let mut stmts = Vec::new();
        let mut i = 0;
        while i < tokens.len() {
            if tokens.get(i..i + 2) == Some(&[Token::LParen, Token::LBrace]) {
                let mut depth = 1;
                let mut end = i + 2;
                while end < tokens.len() && depth != 0 {
                    match tokens[end] {
                        Token::LBrace => depth += 1,
                        Token::RBrace => depth -= 1,
                        _ => {}
                    }
                    end += 1;
                }
                if depth != 0 {
                    return Err(self.error_at(code, 0, code.len(), "expected `}`"));
                }
                if tokens.get(end) != Some(&Token::RParen) {
                    return Err(self.error_at(code, 0, code.len(), "expected `)`"));
                }
                stmts.push(Stmt::Expr(Expr::StatementExpression(
                    self.parse_stmts_from_tokens(code, &tokens[i + 2..end - 1])?,
                )));
                i = end + 1;
                if tokens.get(i) == Some(&Token::Semi) {
                    i += 1;
                }
                continue;
            }
            if let Some(
                [
                    Token::LParen,
                    Token::Ident(callee),
                    Token::RParen,
                    Token::LParen,
                    Token::Ident(argument),
                    Token::RParen,
                    Token::Semi,
                ],
            ) = tokens.get(i..i.saturating_add(7))
            {
                let expression = if self.typedef_names.contains(callee) {
                    Expr::Cast {
                        ty: callee.clone(),
                        expression: argument.clone(),
                    }
                } else {
                    Expr::Call {
                        callee: callee.clone(),
                        argument: argument.clone(),
                    }
                };
                stmts.push(Stmt::Expr(expression));
                i += 7;
                continue;
            }
            if tokens[i] == Token::Keyword(Keyword::Return) {
                let end = tokens[i + 1..]
                    .iter()
                    .position(|token| *token == Token::Semi)
                    .map_or(tokens.len(), |position| i + 1 + position);
                if end == tokens.len() {
                    return Err(self.error_at(code, 0, code.len(), "expected `;`"));
                }
                stmts.push(Stmt::Return(
                    self.parse_expression(code, &tokens[i + 1..end])?,
                ));
                i = end + 1;
                continue;
            }
            let end = tokens[i..]
                .iter()
                .position(|token| *token == Token::Semi)
                .map_or(tokens.len(), |position| i + position);
            if end == tokens.len() {
                return Err(self.error_at(code, 0, code.len(), "expected `;`"));
            }
            stmts.push(Stmt::Expr(self.parse_expression(code, &tokens[i..end])?));
            i = end + 1;
        }
        Ok(stmts)
    }

    fn parse_expression(&self, code: &str, tokens: &[Token]) -> Result<Expr, ParseError> {
        if tokens.is_empty() {
            return Err(self.error_at(code, 0, code.len(), "expected expression"));
        }
        if let Some(Token::Ident(name)) = tokens.first()
            && name == "_Generic"
        {
            if tokens.get(1) != Some(&Token::LParen) {
                return Err(self.error_at(code, 0, code.len(), "expected `(` after `_Generic`"));
            }
            let Some(close) = tokens.len().checked_sub(1) else {
                return Err(self.error_at(code, 0, code.len(), "expected `)` after `_Generic`"));
            };
            if tokens.get(close) != Some(&Token::RParen) {
                return Err(self.error_at(code, 0, code.len(), "expected `)` after `_Generic`"));
            }
            let Some(comma) = tokens[2..close]
                .iter()
                .position(|token| *token == Token::Comma)
                .map(|position| position + 2)
            else {
                return Err(self.error_at(code, 0, code.len(), "expected `,` in `_Generic`"));
            };
            let controlling = self.parse_expression(code, &tokens[2..comma])?;
            let mut associations = Vec::new();
            let mut start = comma + 1;
            while start < close {
                let Some(colon) = tokens[start..close]
                    .iter()
                    .position(|token| *token == Token::Colon)
                    .map(|position| start + position)
                else {
                    return Err(self.error_at(
                        code,
                        0,
                        code.len(),
                        "expected `:` in `_Generic` association",
                    ));
                };
                let expression_end = tokens[colon + 1..close]
                    .iter()
                    .position(|token| *token == Token::Comma)
                    .map_or(close, |position| colon + 1 + position);
                let type_name = match tokens[start] {
                    Token::Ident(ref name) if name == "default" => None,
                    _ => Some(
                        tokens[start..colon]
                            .iter()
                            .map(String::from)
                            .collect::<Vec<_>>()
                            .join(" "),
                    ),
                };
                associations.push(GenericAssociation {
                    type_name,
                    expression: self.parse_expression(code, &tokens[colon + 1..expression_end])?,
                });
                start = expression_end + 1;
            }
            return Ok(Expr::Generic {
                controlling: Box::new(controlling),
                associations,
            });
        }
        match tokens {
            [Token::IntLit(value)] => Ok(Expr::IntLit(*value)),
            [Token::StringLit(value)] => Ok(Expr::StringLit(value.clone())),
            [Token::Ident(name)] => Ok(Expr::Identifier(name.clone())),
            _ => Err(self.error_at(code, 0, code.len(), "unsupported expression")),
        }
    }
}
