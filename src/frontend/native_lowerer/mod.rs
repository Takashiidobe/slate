mod builtins;
mod exprs;
mod globals;
mod items;
mod stmts;
mod types;

use crate::backend::rust_ast::{
    ExternDecl, Item, Label, Program as RustProgram, Visibility as RustVisibility,
};
use crate::parse::clang_ast::Node;
use clang_ast::Id;
use std::collections::{HashMap, HashSet};
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
    #[error("unsupported goto/label form in native lowering")]
    UnsupportedGoto,
}

pub(crate) struct VarInfo {
    pub(crate) name: String,
}

#[derive(Default)]
pub(crate) struct Ctx {
    pub(crate) records: RecordRegistry,
    pub(crate) vars: HashMap<Id, VarInfo>,
    pub(crate) enum_values: HashMap<Id, i128>,
    pub(crate) address_taken_fns: HashSet<String>,
    pub(crate) ctor_calls: Vec<String>,
    pub(crate) dtor_calls: Vec<String>,
    pub(crate) intrinsic_passthroughs: HashMap<Id, String>,
    pub(crate) uses_complex_runtime: std::cell::Cell<bool>,
}

#[derive(Clone, Copy)]
pub(crate) struct GotoCtx<'a> {
    pub(crate) state_var: &'a str,
    pub(crate) dispatch_label: &'a Label,
    pub(crate) label_to_state: &'a HashMap<Id, usize>,
    pub(crate) hoisted_vars: &'a HashSet<Id>,
}

#[derive(Clone, Copy)]
pub(crate) struct Env<'a> {
    pub(crate) vars: &'a HashMap<Id, VarInfo>,
    pub(crate) records: &'a RecordRegistry,
    pub(crate) enum_values: &'a HashMap<Id, i128>,
    pub(crate) is_main: bool,
    pub(crate) continue_label: Option<&'a Label>,
    pub(crate) break_label: Option<&'a Label>,
    pub(crate) goto: Option<GotoCtx<'a>>,
    pub(crate) dtor_calls: &'a [String],
    pub(crate) ret_ty: &'a types::CType,
    pub(crate) intrinsic_passthroughs: &'a HashMap<Id, String>,
    pub(crate) uses_complex_runtime: &'a std::cell::Cell<bool>,
}

pub(crate) const VOID_RET: types::CType = types::CType::Void;

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
    items::collect_address_taken_fns(tu, &mut ctx.address_taken_fns);
    ctx.intrinsic_passthroughs = builtins::collect_intrinsic_passthroughs(tu);
    let mut items = items::lower_items(tu, &mut ctx, primary)?;
    if ctx.uses_complex_runtime.get() {
        items.splice(
            0..0,
            crate::frontend::lowerer::runtime_support::complex_prelude(),
        );
    }
    let math_builtin_externs = builtins::collect_math_builtin_externs(tu);
    if !math_builtin_externs.is_empty() {
        items.splice(
            0..0,
            [Item::ExternBlock {
                abi: "C".into(),
                decls: math_builtin_externs
                    .into_iter()
                    .map(ExternDecl::Fn)
                    .collect(),
            }],
        );
    }
    let libc_passthrough_externs = builtins::collect_libc_passthrough_externs(tu);
    if !libc_passthrough_externs.is_empty() {
        items.splice(
            0..0,
            [Item::ExternBlock {
                abi: "C".into(),
                decls: libc_passthrough_externs
                    .into_iter()
                    .map(ExternDecl::Fn)
                    .collect(),
            }],
        );
    }
    if items::program_uses_long_double(&items) {
        items.splice(
            0..0,
            [Item::ExternBlock {
                abi: "C".into(),
                decls: crate::frontend::lowerer::runtime_support::f80_shim_decls()
                    .into_iter()
                    .map(ExternDecl::Fn)
                    .collect(),
            }],
        );
        items.splice(
            0..0,
            crate::frontend::lowerer::runtime_support::long_double_prelude(RustVisibility::Private),
        );
    }
    Ok(RustProgram { items })
}
