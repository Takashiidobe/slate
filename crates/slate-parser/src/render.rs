use crate::ast::{ConcreteDecl, Decl, TranslationUnit};
use crate::eval::Env;
use std::fmt::Debug;
use std::io::{self, Write};

pub struct Renderer<W> {
    out: W,
}

impl<W: Write> Renderer<W> {
    pub fn new(out: W) -> Self {
        Self { out }
    }

    pub fn render(&mut self, ast: &TranslationUnit, env: &Env) -> io::Result<()> {
        let concrete = ast.eval(env);
        self.line("polyvariant:")?;
        self.render_decls(&ast.decls)?;
        self.line("concrete:")?;
        self.render_concrete_decls(&concrete.decls)
    }

    pub fn into_inner(self) -> W {
        self.out
    }

    fn render_decls(&mut self, decls: &[Decl]) -> io::Result<()> {
        for (index, decl) in decls.iter().enumerate() {
            self.render_debug(index, decl)?;
        }
        Ok(())
    }

    fn render_concrete_decls(&mut self, decls: &[ConcreteDecl]) -> io::Result<()> {
        for (index, decl) in decls.iter().enumerate() {
            self.render_debug(index, decl)?;
        }
        Ok(())
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
