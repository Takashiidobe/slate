use crate::ast::TranslationUnit;
use std::fmt::Debug;
use std::io::{self, Write};

pub struct Renderer<W> {
    out: W,
}

impl<W: Write> Renderer<W> {
    pub fn new(out: W) -> Self {
        Self { out }
    }

    pub fn render(&mut self, ast: &TranslationUnit) -> io::Result<()> {
        for tag in &ast.tags {
            self.render_debug(&format!("tag[{}]", tag.value.id.0), &tag.value)?;
        }
        for (index, decl) in ast.decls.iter().enumerate() {
            self.render_debug(&format!("decl[{index}]"), &decl.value)?;
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
