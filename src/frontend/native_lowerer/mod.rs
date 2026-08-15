mod exprs;
mod globals;
mod items;
mod stmts;
mod types;

use crate::backend::rust_ast::Program as RustProgram;
use crate::parse::clang_ast::Node;
use clang_ast::Id;
use std::collections::HashMap;
use std::path::Path;
use thiserror::Error;
use types::RecordRegistry;

#[derive(Debug, Error)]
pub enum LowerError {
    #[error("expected child {index}, found {found} children")]
    MissingChild { index: usize, found: usize },
    #[error("unresolved decl reference {0}")]
    UnresolvedDecl(Id),
    #[error("unsupported expr node kind: {0:?}")]
    UnsupportedExpr(Option<String>),
    #[error("unsupported unary opcode: {0}")]
    UnsupportedUnaryOp(String),
    #[error("unsupported binary opcode: {0}")]
    UnsupportedBinaryOp(String),
    #[error("FunctionDecl {0:?} does not have a function type")]
    NonFunctionType(Option<String>),
    #[error("local variable was not registered during function prescan")]
    UnregisteredLocal,
    #[error("case/default statement has no body")]
    MissingCaseBody,
}

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

pub(crate) type LResult<T> = Result<T, LowerError>;

pub(crate) trait NodeExt {
    fn child(&self, i: usize) -> LResult<&Node>;
}

impl NodeExt for Node {
    fn child(&self, i: usize) -> LResult<&Node> {
        self.inner.get(i).ok_or(LowerError::MissingChild {
            index: i,
            found: self.inner.len(),
        })
    }
}

pub(crate) fn lower_program(tu: &Node, primary: &Path) -> LResult<RustProgram> {
    let mut ctx = Ctx::default();
    items::collect_top_level(tu, &mut ctx);
    Ok(RustProgram {
        items: items::lower_items(tu, &mut ctx, primary)?,
    })
}
