use crate::ast::{
    Decl, EnumItem, FieldItem, FunctionDefinition, SpannedStmt, Stmt, TagBody, TagDefinition,
    TranslationUnit,
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
            self.render_debug(&label, &tag.value)?;
        }
        for (index, decl) in ast.decls.iter().enumerate() {
            let label = if self.show_ids {
                format!("decl[{index}] #{}", decl.id.0)
            } else {
                format!("decl[{index}]")
            };
            self.render_debug(&label, &decl.value)?;
        }
        Ok(())
    }

    pub fn into_inner(self) -> W {
        self.out
    }

    fn render_debug<T: Debug>(&mut self, label: &str, value: &T) -> io::Result<()> {
        let rendered = format!("{value:#?}");
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

fn strip_comments(unit: &mut TranslationUnit) {
    unit.decls
        .retain(|decl| !matches!(decl.value, Decl::Comment(_)));
    for decl in &mut unit.decls {
        strip_decl_comments(&mut decl.value);
    }
    for tag in &mut unit.tags {
        strip_tag_comments(&mut tag.value);
    }
}

fn strip_decl_comments(decl: &mut Decl) {
    if let Decl::Function(function) = decl {
        strip_function_comments(function);
    }
}

fn strip_function_comments(function: &mut FunctionDefinition) {
    strip_stmt_comments(&mut function.body);
}

fn strip_tag_comments(tag: &mut TagDefinition) {
    match &mut tag.body {
        TagBody::Record(fields) => {
            fields.retain(|field| !matches!(field.value, FieldItem::Comment(_)))
        }
        TagBody::Enum { enumerators, .. } => {
            enumerators.retain(|item| !matches!(item.value, EnumItem::Comment(_)))
        }
    }
}

fn strip_stmt_comments(stmts: &mut Vec<SpannedStmt>) {
    stmts.retain(|stmt| !matches!(stmt.value, Stmt::Comment(_)));
    for stmt in stmts {
        strip_stmt_children(&mut stmt.value);
    }
}

fn strip_stmt_children(stmt: &mut Stmt) {
    match stmt {
        Stmt::Block(body)
        | Stmt::While { body, .. }
        | Stmt::DoWhile { body, .. }
        | Stmt::Switch { body, .. } => strip_stmt_comments(body),
        Stmt::If {
            then_branch,
            else_branch,
            ..
        } => {
            strip_stmt_comments(then_branch);
            if let Some(else_branch) = else_branch {
                strip_stmt_comments(else_branch);
            }
        }
        Stmt::For { init, body, .. } => {
            if let Some(init) = init {
                strip_stmt_children(&mut init.value);
            }
            strip_stmt_comments(body);
        }
        Stmt::Labeled { body, .. } | Stmt::SwitchLabel { body, .. } => {
            strip_stmt_children(&mut body.value);
        }
        Stmt::NestedFunction(function) => strip_function_comments(function),
        _ => {}
    }
}
