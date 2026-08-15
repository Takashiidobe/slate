mod exprs;
mod globals;
mod items;
mod stmts;
mod types;

#[cfg(test)]
mod tests;

use crate::backend::rust_ast::Program as RustProgram;
use crate::parse::ast::Program;

pub(crate) fn lower_program(program: &Program) -> RustProgram {
    RustProgram {
        items: items::lower_items(program),
    }
}
