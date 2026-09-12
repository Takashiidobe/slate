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
        for (index, decl) in ast.decls.iter().enumerate() {
            self.render_debug(index, &decl.value)?;
        }
        Ok(())
    }

    pub fn into_inner(self) -> W {
        self.out
    }

    fn render_debug<T: Debug>(&mut self, index: usize, value: &T) -> io::Result<()> {
        let rendered = format!("{value:#?}");
        for (line_index, line) in rendered.lines().enumerate() {
            if line_index == 0 {
                self.line(&format!("decl[{index}]: {line}"))?;
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
