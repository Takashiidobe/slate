pub mod numeric;
mod validate;

pub use validate::{SemaError, SemaErrors};

use crate::ast::{DeclKind, Expr, StmtKind, TranslationUnit};
use crate::ir::Value;
use numeric::{Context, ResolveError};

pub fn resolve_expression_roots(unit: &TranslationUnit) -> Result<Vec<Value>, ResolveError> {
    let context = Context::new(unit.target);
    let mut expressions: Vec<&Expr> = Vec::new();
    for declaration in &unit.decls {
        match &declaration.value {
            DeclKind::Comment(_) => {}
            DeclKind::Function(function) => {
                if !function.attributes.is_empty() || !function.specifiers.attributes.is_empty() {
                    return Err(ResolveError::Unsupported("function attributes"));
                }
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
