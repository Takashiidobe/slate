mod abi;
mod asm;
mod assertion;
mod atomic;
mod attributes;
mod builtins;
pub mod ctype;
mod effects;
mod effects_statements;
mod entity;
mod expression;
mod fold;
pub(crate) mod function;
mod initializer;
mod module;
mod ms_asm;
mod ms_asm_effects;
mod ms_asm_return;
pub mod names;
pub mod numeric;
mod operand;
pub mod pragmas;
mod sequencing;
mod type_of;
pub mod types;
mod validate;

pub use ctype::compat::PointerMerge;
pub use validate::{SemaError, SemaErrors};

use crate::ast::{DeclKind, Expr, StmtKind, TranslationUnit};
use crate::ir::{NameResolution, Value};
use numeric::{Context, ResolveError};

pub struct Sema<'u> {
    unit: &'u TranslationUnit,
    names: NameResolution,
    items: Vec<names::ItemResolution>,
}

impl<'u> Sema<'u> {
    pub fn new(unit: &'u TranslationUnit) -> Self {
        let (names, items) = names::resolve_items(unit);
        Self { unit, names, items }
    }

    pub fn names(&self) -> Result<&NameResolution, &names::ResolveError> {
        match self.items.iter().find_map(|item| item.errors.first()) {
            Some(error) => Err(error),
            None => Ok(&self.names),
        }
    }
}

pub fn resolve_expression_roots(unit: &TranslationUnit) -> Result<Vec<Value>, ResolveError> {
    let context = Context::for_dialect(&unit.dialect);
    let mut expressions: Vec<&Expr> = Vec::new();
    let mut types = types::TypeResolver::with_tags(unit);
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
