mod exprs;
mod globals;
mod items;
mod stmts;
mod types;

use crate::backend::rust_ast::Program as RustProgram;
use crate::parse::clang_ast::Node;
use clang_ast::Id;
use std::collections::HashMap;
use types::RecordRegistry;

pub(crate) struct VarInfo {
    pub(crate) name: String,
}

#[derive(Default)]
pub(crate) struct Ctx {
    pub(crate) records: RecordRegistry,
    pub(crate) vars: HashMap<Id, VarInfo>,
}

#[derive(Clone, Copy)]
pub(crate) struct Env<'a> {
    pub(crate) vars: &'a HashMap<Id, VarInfo>,
    pub(crate) records: &'a RecordRegistry,
    pub(crate) is_main: bool,
}

pub(crate) fn is_present(node: &Node) -> bool {
    node.id != Id::NULL
}

pub(crate) fn lower_program(tu: &Node) -> RustProgram {
    let mut ctx = Ctx::default();
    items::collect_top_level(tu, &mut ctx);
    RustProgram {
        items: items::lower_items(tu, &mut ctx),
    }
}
