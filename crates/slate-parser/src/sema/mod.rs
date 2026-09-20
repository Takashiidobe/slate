mod abi;
mod assertion;
mod atomic;
mod builtins;
pub mod ctype;
mod effects;
mod effects_statements;
mod entity;
mod expression;
mod fold;
mod function;
mod initializer;
mod module;
pub mod names;
pub mod numeric;
mod operand;
mod type_of;
pub mod types;
mod validate;

pub use module::resolve_module;
pub use validate::{SemaError, SemaErrors, with_sources};

use crate::ast::{DeclKind, Expr, StmtKind, TranslationUnit};
use crate::ir::Value;
use numeric::{Context, ResolveError};

pub fn resolve_expression_roots(unit: &TranslationUnit) -> Result<Vec<Value>, ResolveError> {
    let context = Context::new(unit.target.clone())
        .with_options(&unit.options)
        .with_features(crate::standard_features::StandardFeatures::new(
            unit.standard,
        ));
    let mut expressions: Vec<&Expr> = Vec::new();
    let mut types = types::TypeResolver::with_tags(context.target.clone(), unit);
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
                        _ => return Err(ResolveError::Unsupported("statement in expression dump")),
                    }
                }
            }
            _ => return Err(ResolveError::Unsupported("declaration in expression dump")),
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
