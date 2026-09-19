mod abi;
mod assertion;
mod atomic;
mod builtins;
mod effects;
mod effects_statements;
mod expression;
mod fold;
mod function;
mod initializer;
mod module;
pub mod names;
pub mod numeric;
mod type_of;
pub mod types;
mod validate;

pub use module::resolve_module;
pub use validate::{SemaError, SemaErrors};

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
        .map(|expr| context.resolve(expr))
        .collect()
}
