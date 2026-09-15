use super::numeric::{Context, ResolveError};
use crate::ast::{
    DeclKind, Declarator, Span, Stmt, StmtKind, StorageClass, TranslationUnit, TypeName,
    TypeSpecifier,
};
use crate::ir::{ConversionReason, Function, Linkage, Module, Statement, Type};

pub fn resolve_module(unit: &TranslationUnit) -> Result<Module, ResolveError> {
    let context = Context::new(unit.target).with_options(&unit.options);
    let mut module = Module {
        target: context.target,
        functions: Vec::new(),
    };
    for declaration in &unit.decls {
        let function = match &declaration.value {
            DeclKind::Comment(_) => continue,
            DeclKind::Function(function) => function,
            _ => return Err(ResolveError::Unsupported("module declaration")),
        };
        if !function.attributes.is_empty()
            || !function.specifiers.attributes.is_empty()
            || function.specifiers.is_inline
            || function.specifiers.is_noreturn
            || function.specifiers.is_constexpr
        {
            return Err(ResolveError::Unsupported(
                "function attributes or specifiers",
            ));
        }
        let Declarator::Function { inner, parameters } = &function.declarator else {
            return Err(ResolveError::Unsupported("derived function return type"));
        };
        let Declarator::Name(name) = inner.as_ref() else {
            return Err(ResolveError::Unsupported("derived function declarator"));
        };
        if !parameters.parameters().is_empty() || parameters.is_variadic() {
            return Err(ResolveError::Unsupported("function parameters"));
        }
        let return_type = if function.specifiers.ty == TypeSpecifier::Void {
            None
        } else {
            Some(context.cast_type(&TypeName {
                specifiers: function.specifiers.clone(),
                declarator: Declarator::Abstract,
            })?)
        };
        let linkage = match function.specifiers.storage {
            StorageClass::Static => Linkage::Internal,
            StorageClass::None | StorageClass::Extern => Linkage::External,
            _ => return Err(ResolveError::Unsupported("function storage class")),
        };
        module
            .functions
            .push(declaration.clone().with_value(Function {
                name: name.clone(),
                return_type,
                linkage,
                body: lower_statements(&context, &function.body, return_type)?,
            }));
    }
    Ok(module)
}

fn lower_statements(
    context: &Context,
    body: &[Stmt],
    return_type: Option<Type>,
) -> Result<Vec<Span<Statement>>, ResolveError> {
    body.iter()
        .filter(|statement| !matches!(statement.value, StmtKind::Comment(_)))
        .map(|statement| {
            let lowered = match &statement.value {
                StmtKind::Expr(expression) => Statement::Expression(context.resolve(expression)?),
                StmtKind::Return(expression) => {
                    let ty = return_type
                        .ok_or(ResolveError::Unsupported("value return from void function"))?;
                    Statement::Return(Some(context.convert(
                        context.resolve(expression)?,
                        ty,
                        ConversionReason::Return,
                    )))
                }
                StmtKind::ReturnVoid if return_type.is_none() => Statement::Return(None),
                StmtKind::Block(body) => {
                    Statement::Block(lower_statements(context, body, return_type)?)
                }
                _ => return Err(ResolveError::Unsupported("module statement")),
            };
            Ok(statement.clone().with_value(lowered))
        })
        .collect()
}
