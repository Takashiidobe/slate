use crate::ast::{
    DeclKind, Declaration, Declarator, EnumItemKind, FieldItemKind, FunctionDefinition,
    ParameterList, Stmt, StmtKind, TagBody, TagDefinition, TranslationUnit,
};
use std::fmt::Debug;
use std::io::{self, Write};

pub struct Renderer<W> {
    out: W,
    show_comments: bool,
    show_ids: bool,
}

impl<W: Write> Renderer<W> {
    pub fn new(out: W) -> Self {
        Self {
            out,
            show_comments: false,
            show_ids: false,
        }
    }

    pub fn with_show_comments(mut self, show_comments: bool) -> Self {
        self.show_comments = show_comments;
        self
    }

    pub fn with_show_ids(mut self, show_ids: bool) -> Self {
        self.show_ids = show_ids;
        self
    }

    pub fn render(&mut self, ast: &TranslationUnit) -> io::Result<()> {
        crate::ast::set_show_node_ids(self.show_ids);
        let stripped = if self.show_comments {
            None
        } else {
            let mut ast = ast.clone();
            strip_comments(&mut ast);
            Some(ast)
        };
        let ast = stripped.as_ref().unwrap_or(ast);
        for tag in &ast.tags {
            let label = if self.show_ids {
                format!("tag[{}] #{}", tag.value.id.0, tag.id.0)
            } else {
                format!("tag[{}]", tag.value.id.0)
            };
            self.render_debug(&label, tag)?;
        }
        for (index, decl) in ast.decls.iter().enumerate() {
            let label = if self.show_ids {
                format!("decl[{index}] #{}", decl.id.0)
            } else {
                format!("decl[{index}]")
            };
            self.render_debug(&label, decl)?;
        }
        Ok(())
    }

    pub fn into_inner(self) -> W {
        self.out
    }

    fn render_debug<T: Debug>(&mut self, label: &str, value: &T) -> io::Result<()> {
        let rendered = expand(&format!("{value:?}"));
        for (line_index, line) in rendered.lines().enumerate() {
            if line_index == 0 {
                self.line(&format!("{label}: {line}"))?;
            } else {
                self.line(&format!("  {line}"))?;
            }
        }
        Ok(())
    }

    fn line(&mut self, text: &str) -> io::Result<()> {
        writeln!(self.out, "{text}")
    }
}

// `{:#?}` layers a PadAdapter per nesting level, so a deep AST costs O(bytes x depth);
// re-laying out the flat form ourselves keeps it linear
fn expand(compact: &str) -> String {
    let source = compact.as_bytes();
    let mut out = Vec::with_capacity(source.len() * 2);
    let mut depth = 0usize;
    let mut index = 0;
    while index < source.len() {
        let byte = source[index];
        match byte {
            b'"' | b'\'' => index = copy_literal(source, index, &mut out),
            b'{' | b'[' | b'(' => {
                out.push(byte);
                index += 1;
                let empty = skip_spaces(source, index);
                if source.get(empty) == Some(&closer(byte)) {
                    out.push(closer(byte));
                    index = empty + 1;
                    continue;
                }
                depth += 1;
                indent(&mut out, depth);
                index = empty;
            }
            b'}' | b']' | b')' => {
                while out.last() == Some(&b' ') {
                    out.pop();
                }
                out.push(b',');
                depth -= 1;
                indent(&mut out, depth);
                out.push(byte);
                index += 1;
            }
            b',' => {
                out.push(byte);
                index = skip_spaces(source, index + 1);
                indent(&mut out, depth);
            }
            _ => {
                out.push(byte);
                index += 1;
            }
        }
    }
    String::from_utf8(out).unwrap_or_else(|_| compact.to_owned())
}

fn closer(open: u8) -> u8 {
    match open {
        b'{' => b'}',
        b'[' => b']',
        _ => b')',
    }
}

fn skip_spaces(source: &[u8], mut index: usize) -> usize {
    while source.get(index) == Some(&b' ') {
        index += 1;
    }
    index
}

fn indent(out: &mut Vec<u8>, depth: usize) {
    out.push(b'\n');
    out.extend(std::iter::repeat_n(b' ', depth * 4));
}

fn copy_literal(source: &[u8], start: usize, out: &mut Vec<u8>) -> usize {
    let quote = source[start];
    out.push(quote);
    let mut index = start + 1;
    while let Some(&byte) = source.get(index) {
        out.push(byte);
        index += 1;
        match byte {
            b'\\' => {
                if let Some(&escaped) = source.get(index) {
                    out.push(escaped);
                    index += 1;
                }
            }
            _ if byte == quote => break,
            _ => {}
        }
    }
    index
}

fn strip_comments(unit: &mut TranslationUnit) {
    unit.decls
        .retain(|decl| !matches!(decl.value, DeclKind::Comment(_)));
    for decl in &mut unit.decls {
        strip_decl_comments(&mut decl.value);
    }
    for tag in &mut unit.tags {
        strip_tag_comments(&mut tag.value);
    }
}

fn strip_decl_comments(decl: &mut DeclKind) {
    match decl {
        DeclKind::Function(function) => strip_function_comments(function),
        DeclKind::Declaration(declaration) => strip_declaration_comments(declaration),
        _ => {}
    }
}

fn strip_function_comments(function: &mut FunctionDefinition) {
    strip_declarator_comments(&mut function.declarator);
    strip_stmt_comments(&mut function.body);
}

fn strip_declaration_comments(declaration: &mut Declaration) {
    for declarator in &mut declaration.declarators {
        strip_declarator_comments(&mut declarator.value.declarator);
    }
}

fn strip_declarator_comments(declarator: &mut Declarator) {
    match declarator {
        Declarator::Function { inner, parameters } => {
            if let ParameterList::Prototype { parameters, .. }
            | ParameterList::IdentifierList { parameters } = parameters
            {
                for parameter in parameters {
                    parameter.value.comments.clear();
                    strip_declarator_comments(&mut parameter.value.declarator);
                }
            }
            strip_declarator_comments(inner);
        }
        Declarator::Grouped(inner)
        | Declarator::Attributed { inner, .. }
        | Declarator::Pointer { inner, .. }
        | Declarator::Array { inner, .. } => strip_declarator_comments(inner),
        Declarator::Abstract | Declarator::Name(_) => {}
    }
}

fn strip_tag_comments(tag: &mut TagDefinition) {
    match &mut tag.body {
        TagBody::Record(fields) => {
            fields.retain(|field| !matches!(field.value, FieldItemKind::Comment(_)));
            for field in fields {
                if let FieldItemKind::Field(field) = &mut field.value {
                    for declarator in &mut field.declarators {
                        strip_declarator_comments(&mut declarator.value.declarator);
                    }
                }
            }
        }
        TagBody::Enum { enumerators, .. } => {
            enumerators.retain(|item| !matches!(item.value, EnumItemKind::Comment(_)))
        }
    }
}

fn strip_stmt_comments(stmts: &mut Vec<Stmt>) {
    stmts.retain(|stmt| !matches!(stmt.value, StmtKind::Comment(_)));
    for stmt in stmts {
        strip_stmt_children(&mut stmt.value);
    }
}

fn strip_stmt_children(stmt: &mut StmtKind) {
    match stmt {
        StmtKind::Block(body) => strip_stmt_comments(body),
        StmtKind::Decl(declaration) => strip_declaration_comments(declaration),
        StmtKind::While { body, .. }
        | StmtKind::DoWhile { body, .. }
        | StmtKind::Switch { body, .. } => strip_stmt_children(&mut body.value),
        StmtKind::If {
            then_branch,
            else_branch,
            ..
        } => {
            strip_stmt_children(&mut then_branch.value);
            if let Some(else_branch) = else_branch {
                strip_stmt_children(&mut else_branch.value);
            }
        }
        StmtKind::IfDeclaration {
            declaration,
            then_branch,
            else_branch,
            ..
        } => {
            strip_declaration_comments(&mut declaration.value);
            strip_stmt_children(&mut then_branch.value);
            if let Some(branch) = else_branch {
                strip_stmt_children(&mut branch.value);
            }
        }
        StmtKind::SwitchDeclaration {
            declaration, body, ..
        } => {
            strip_declaration_comments(&mut declaration.value);
            strip_stmt_children(&mut body.value);
        }
        StmtKind::For { init, body, .. } => {
            if let Some(init) = init {
                strip_stmt_children(&mut init.value);
            }
            strip_stmt_children(&mut body.value);
        }
        StmtKind::Labeled { body, .. }
        | StmtKind::SwitchLabel { body, .. }
        | StmtKind::Attributed { body, .. } => {
            strip_stmt_children(&mut body.value);
        }
        StmtKind::NestedFunction(function) => strip_function_comments(function),
        _ => {}
    }
}
