use super::numeric::{Context, ResolveError};
use crate::ast::{
    DeclKind, Declarator, Expr, ExprKind, IntegerRank, IntegerType, Span, Stmt, StmtKind,
    StorageClass, TranslationUnit, TypeName, TypeSpecifier,
};
use crate::ir::{ConversionReason, Function, Linkage, Module, Parameters, Statement, Type};

pub fn resolve_module(unit: &TranslationUnit) -> Result<Module, ResolveError> {
    let context = Context::new(unit.target.clone()).with_options(&unit.options);
    let mut module = Module::new(context.target.clone());
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
        module.metadata.insert(
            declaration.id,
            vec![
                (
                    "c_storage".into(),
                    function.specifiers.storage.as_str().into(),
                ),
                ("c_return".into(), format!("{:?}", function.specifiers.ty)),
            ],
        );
        for statement in &function.body {
            collect_layout_metadata(statement, &mut module.metadata);
        }
        module.functions.push(
            declaration.clone().with_value(Function {
                id: crate::ir::BindingId(
                    u32::try_from(module.functions.len())
                        .map_err(|_| ResolveError::Unsupported("too many functions"))?,
                ),
                name: name.clone(),
                parameters: Parameters::Prototype {
                    fixed: Vec::new(),
                    variadic: false,
                },
                return_type,
                linkage,
                body: Some(lower_statements(&context, &function.body, return_type)?),
            }),
        );
    }
    Ok(module)
}

fn collect_layout_metadata(statement: &Stmt, metadata: &mut crate::ir::Metadata) {
    let expression = match &statement.value {
        StmtKind::Return(expression) | StmtKind::Expr(expression) => Some(expression),
        _ => None,
    };
    if let Some(expression) = expression {
        collect_expression_metadata(expression, metadata);
    }
}

fn collect_expression_metadata(expression: &Expr, metadata: &mut crate::ir::Metadata) {
    match &expression.value {
        ExprKind::SizeOfType { ty } => {
            metadata.insert(
                expression.id,
                vec![("size_of".into(), scalar_type_name(ty))],
            );
        }
        ExprKind::AlignOf { ty } => {
            metadata.insert(
                expression.id,
                vec![("align_of".into(), scalar_type_name(ty))],
            );
        }
        ExprKind::Paren(inner)
        | ExprKind::SizeOfExpr(inner)
        | ExprKind::AlignOfExpr(inner)
        | ExprKind::Unary { operand: inner, .. }
        | ExprKind::Postfix { operand: inner, .. } => collect_expression_metadata(inner, metadata),
        ExprKind::Binary { left, right, .. }
        | ExprKind::Assign {
            target: left,
            value: right,
            ..
        }
        | ExprKind::Comma { left, right } => {
            collect_expression_metadata(left, metadata);
            collect_expression_metadata(right, metadata);
        }
        _ => {}
    }
}

fn scalar_type_name(ty: &TypeName) -> String {
    match &ty.specifiers.ty {
        TypeSpecifier::Floating(crate::ast::FloatingType::LongDouble) => "long double".into(),
        TypeSpecifier::Floating(crate::ast::FloatingType::Double) => "double".into(),
        TypeSpecifier::Floating(crate::ast::FloatingType::Float) => "float".into(),
        TypeSpecifier::Bool => "_Bool".into(),
        TypeSpecifier::Integer(IntegerType::Char { signed: None }) => "char".into(),
        TypeSpecifier::Integer(IntegerType::Char { signed: Some(true) }) => "signed char".into(),
        TypeSpecifier::Integer(IntegerType::Char {
            signed: Some(false),
        }) => "unsigned char".into(),
        TypeSpecifier::Integer(IntegerType::Ranked { rank, signed }) => {
            let name = match rank {
                IntegerRank::Short => "short",
                IntegerRank::Int => "int",
                IntegerRank::Long => "long",
                IntegerRank::LongLong => "long long",
                IntegerRank::Int128 => "__int128",
            };
            if *signed {
                name.into()
            } else {
                format!("unsigned {name}")
            }
        }
        _ => format!("{:?}", ty.specifiers.ty),
    }
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
                StmtKind::Null => Statement::Block(Vec::new()),
                StmtKind::Block(body) => {
                    Statement::Block(lower_statements(context, body, return_type)?)
                }
                _ => return Err(ResolveError::Unsupported("module statement")),
            };
            Ok(statement.clone().with_value(lowered))
        })
        .collect()
}
