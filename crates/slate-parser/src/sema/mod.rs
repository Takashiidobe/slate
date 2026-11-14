mod abi;
mod asm;
mod assertion;
mod atomic;
mod attributes;
mod block;
mod builtins;
pub(crate) use builtins::{
    gcc_aarch64_tuple_declarations, is_gcc_aarch64_tuple_name, is_gcc_target_type_name,
};
mod counted_loops;
pub mod ctype;
mod effects;
mod effects_statements;
mod entity;
mod expression;
mod fold;
pub(crate) mod function;
mod initializer;
mod macro_structure;
mod module;
mod ms_asm;
mod ms_asm_effects;
mod ms_asm_return;
pub mod names;
mod neon_builtins;
mod nested;
pub mod numeric;
mod object_size;
mod openmp;
mod operand;
pub mod pragmas;
mod seh_scopes;
mod sequencing;
mod type_of;
mod typer;
pub mod types;
mod validate;
mod vector_builtins;
mod x86_vector_builtins;

pub use ctype::compat::PointerMerge;
pub use module::MacroKeep;
pub use validate::{SemaError, SemaErrors};

use crate::ast::{DeclKind, Expr, StmtKind, TranslationUnit};
use crate::ir::{NameResolution, Value};
use numeric::{Context, ResolveError};

const STACK_RED_ZONE: usize = 512 << 10;
const STACK_GROWTH: usize = 16 << 20;

pub(crate) fn deep<T>(recurse: impl FnOnce() -> T) -> T {
    stacker::maybe_grow(STACK_RED_ZONE, STACK_GROWTH, recurse)
}

pub struct Sema<'u> {
    unit: &'u TranslationUnit,
    names: NameResolution,
    items: Vec<names::ItemResolution>,
    types: types::TypeResolver,
    macro_structure: bool,
    macro_keep: Option<module::MacroKeep>,
}

impl<'u> Sema<'u> {
    pub fn new(unit: &'u TranslationUnit) -> Self {
        let (names, items) = names::resolve_items(unit);
        let types = types::TypeResolver::with_names(unit, &names);
        Self {
            unit,
            names,
            items,
            types,
            macro_structure: false,
            macro_keep: None,
        }
    }

    pub fn with_macro_structure(mut self, enabled: bool) -> Self {
        self.macro_structure = enabled;
        self
    }

    pub fn with_macro_keep(mut self, keep: module::MacroKeep) -> Self {
        self.macro_keep = Some(keep);
        self
    }

    pub fn names(&self) -> Result<&NameResolution, &names::ResolveError> {
        match self.items.iter().find_map(|item| item.errors.first()) {
            Some(error) => Err(error),
            None => Ok(&self.names),
        }
    }

    pub fn type_module(&self) -> Result<crate::ir::Module, ResolveError> {
        types::resolve_type_module(self.unit, &self.names)
    }

    pub fn expression_roots(&self) -> Result<Vec<Value>, ResolveError> {
        resolve_expression_roots(self.unit, &self.names)
    }
}

fn resolve_expression_roots(
    unit: &TranslationUnit,
    names: &NameResolution,
) -> Result<Vec<Value>, ResolveError> {
    let context = Context::for_dialect(&unit.dialect);
    let mut expressions: Vec<&Expr> = Vec::new();
    let mut types = types::TypeResolver::with_names(unit, names);
    for declaration in &unit.decls {
        match &declaration.value {
            DeclKind::Comment(_) => {}
            DeclKind::Function(function) => {
                for statement in &function.body {
                    match &statement.value {
                        StmtKind::Expr(expression) | StmtKind::Return(expression) => {
                            expressions.push(expression);
                        }
                        StmtKind::Comment(_) | StmtKind::ReturnVoid => {}
                        _ => {
                            return Err(ResolveError::Unimplemented(
                                "statement in expression dump",
                            ));
                        }
                    }
                }
            }
            _ => {
                return Err(ResolveError::Unimplemented(
                    "declaration in expression dump",
                ));
            }
        }
    }
    expressions
        .into_iter()
        .map(|expr| {
            types
                .constant_value_with_context(&context, expr)
                .map(|operand| operand.value)
        })
        .collect()
}
