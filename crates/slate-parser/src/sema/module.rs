use super::expression::Lowerer;
use super::numeric::{Context, ResolveError};
use super::types::TypeResolver;
use crate::ast::{
    self, DeclKind, Declarator, Initializer, ParameterList, Span, Stmt, StmtKind, StorageClass,
    TranslationUnit,
};
use crate::ir::*;
use std::collections::HashMap;

pub fn resolve_module(unit: &TranslationUnit) -> Result<Module, ResolveError> {
    let context = Context::new(unit.target.clone()).with_options(&unit.options);
    let names = super::names::resolve(unit)?;
    let next_id = names
        .bindings
        .iter()
        .map(|b| b.value.id.0 + 1)
        .max()
        .unwrap_or(0);
    let mut lower = Lowerer {
        types: TypeResolver::with_tags(context.target.clone(), unit),
        module: Module::new(context.target.clone()),
        context,
        names,
        bindings: HashMap::new(),
        type_spans: HashMap::new(),
        next_id,
    };
    for declaration in &unit.decls {
        match &declaration.value {
            DeclKind::Comment(_) => {}
            DeclKind::Declaration(item) => {
                lower.declaration(item, true)?;
            }
            DeclKind::Function(function) => {
                check_specifiers(&function.specifiers)?;
                if !function.attributes.is_empty() {
                    return Err(ResolveError::Unsupported("function attributes"));
                }
                let name = function
                    .declarator
                    .name()
                    .ok_or(ResolveError::Unsupported("unnamed function"))?;
                let id = lower.declaration_id(declaration.id, name)?;
                let c_return = lower
                    .types
                    .resolve(&function.specifiers, &Declarator::Abstract)?
                    .c
                    .spelling;
                let start = lower.types.definitions.len();
                let resolved = lower
                    .types
                    .resolve(&function.specifiers, &function.declarator)?;
                for definition in &lower.types.definitions[start..] {
                    lower.type_spans.insert(
                        definition.id,
                        declaration.clone().with_value(definition.clone()),
                    );
                }
                let ty = resolved
                    .ty
                    .ok_or(ResolveError::Unsupported("void function type"))?;
                let Type::Function { return_type, .. } = &ty else {
                    return Err(ResolveError::Unsupported("function definition declarator"));
                };
                let return_type = return_type.as_ref().map(|ty| (**ty).clone());
                lower.bindings.insert(id, ty);
                let mut metadata = vec![
                    (
                        "c_storage".into(),
                        function.specifiers.storage.as_str().into(),
                    ),
                    ("c_return".into(), c_return),
                ];
                metadata.extend(resolved.c.entries());
                lower.module.metadata.insert(declaration.id, metadata);
                let params = function
                    .declarator
                    .function_parameters()
                    .ok_or(ResolveError::Unsupported("missing function parameters"))?;
                let parameters = lower.parameters(params, true)?;
                let mut body = lower.statements(&function.body, return_type.clone())?;
                if name == "main"
                    && return_type == Some(lower.context.int_type())
                    && !matches!(body.last().map(|s| &s.value), Some(Statement::Return(_)))
                {
                    let zero = lower.value(
                        declaration,
                        lower.context.int_type(),
                        ValueKind::Constant(Number::Integer(0u32.into())),
                    );
                    body.push(
                        declaration
                            .clone()
                            .with_value(Statement::Return(Some(zero))),
                    );
                }
                lower
                    .module
                    .functions
                    .push(declaration.clone().with_value(Function {
                        id,
                        name: name.into(),
                        parameters,
                        return_type,
                        linkage: linkage(function.specifiers.storage)?,
                        body: Some(body),
                    }));
            }
            _ => return Err(ResolveError::Unsupported("module declaration")),
        }
    }
    for definition in &lower.types.definitions {
        if let Some(span) = lower.type_spans.get(&definition.id) {
            lower.module.types.push(span.clone());
        } else if let Some(tag) = lower.types.tag_span(definition.id, unit) {
            lower
                .module
                .types
                .push(tag.clone().with_value(definition.clone()));
        } else if let Some(declaration) = unit.decls.first() {
            lower
                .module
                .types
                .push(declaration.clone().with_value(definition.clone()));
        }
    }
    Ok(lower.module)
}

fn linkage(storage: StorageClass) -> Result<Linkage, ResolveError> {
    match storage {
        StorageClass::Static => Ok(Linkage::Internal),
        StorageClass::None | StorageClass::Extern => Ok(Linkage::External),
        _ => Err(ResolveError::Unsupported("linkage storage class")),
    }
}

fn check_specifiers(specifiers: &ast::DeclarationSpecifiers) -> Result<(), ResolveError> {
    if !specifiers.attributes.is_empty()
        || specifiers.is_inline
        || specifiers.is_noreturn
        || specifiers.is_constexpr
        || specifiers.qualifiers.is_volatile
        || specifiers.qualifiers.is_atomic
    {
        return Err(ResolveError::Unsupported(
            "attributes, function specifiers, volatile or atomic access",
        ));
    }
    Ok(())
}

impl Lowerer {
    fn parameters(
        &mut self,
        params: &ParameterList,
        definition: bool,
    ) -> Result<Parameters, ResolveError> {
        if matches!(params, ParameterList::Empty) {
            return Ok(Parameters::Unprototyped);
        }
        let mut fixed = Vec::new();
        for parameter in params.parameters() {
            check_specifiers(&parameter.specifiers)?;
            let start = self.types.definitions.len();
            let resolved = self
                .types
                .resolve(&parameter.specifiers, &parameter.declarator)?;
            let mut ty = resolved
                .ty
                .ok_or(ResolveError::Unsupported("void parameter"))?;
            ty = match ty {
                Type::Array { element, .. } => self.pointer(*element, false),
                function @ Type::Function { .. } => self.pointer(function, false),
                other => other,
            };
            let name = parameter.declarator.name();
            let id = if definition {
                self.declaration_id(
                    parameter.id,
                    name.ok_or(ResolveError::Unsupported("unnamed definition parameter"))?,
                )?
            } else {
                self.fresh()
            };
            self.bindings.insert(id, ty.clone());
            self.module
                .metadata
                .insert(parameter.id, resolved.c.entries());
            for definition in &self.types.definitions[start..] {
                self.type_spans.insert(
                    definition.id,
                    parameter.clone().with_value(definition.clone()),
                );
            }
            fixed.push(parameter.clone().with_value(Parameter {
                id,
                name: name.map(str::to_owned),
                ty,
            }));
        }
        Ok(Parameters::Prototype {
            fixed,
            variadic: params.is_variadic(),
        })
    }

    fn declaration(
        &mut self,
        item: &ast::Declaration,
        global: bool,
    ) -> Result<Vec<Span<Statement>>, ResolveError> {
        check_specifiers(&item.specifiers)?;
        if !global
            && (item.specifiers.storage == StorageClass::Typedef
                || matches!(item.specifiers.ty, ast::TypeSpecifier::Tag(_)))
        {
            return Err(ResolveError::Unsupported(
                "block scoped typedef or tag declaration",
            ));
        }
        if item.declarators.is_empty() {
            self.types
                .resolve(&item.specifiers, &Declarator::Abstract)?;
        }
        let mut statements = Vec::new();
        for declarator in &item.declarators {
            if !declarator.attributes.is_empty() || declarator.asm_label.is_some() {
                return Err(ResolveError::Unsupported(
                    "declarator attributes or asm label",
                ));
            }
            let name = declarator
                .declarator
                .name()
                .ok_or(ResolveError::Unsupported("unnamed declaration"))?;
            let start = self.types.definitions.len();
            let resolved = self
                .types
                .resolve(&item.specifiers, &declarator.declarator)?;
            self.module
                .metadata
                .insert(declarator.id, resolved.c.entries());
            if item.specifiers.storage == StorageClass::Typedef {
                self.types.define_alias(name.into(), resolved)?;
                for definition in &self.types.definitions[start..] {
                    self.type_spans.insert(
                        definition.id,
                        declarator.clone().with_value(definition.clone()),
                    );
                }
                continue;
            }
            for definition in &self.types.definitions[start..] {
                self.type_spans.insert(
                    definition.id,
                    declarator.clone().with_value(definition.clone()),
                );
            }
            let ty = resolved
                .ty
                .ok_or(ResolveError::Unsupported("void object"))?;
            let id = self.declaration_id(declarator.id, name)?;
            self.bindings.insert(id, ty.clone());
            if let Type::Function { return_type, .. } = &ty {
                if !global || declarator.initializer.is_some() {
                    return Err(ResolveError::Unsupported(
                        "local function prototype or function initializer",
                    ));
                }
                let params = declarator
                    .declarator
                    .function_parameters()
                    .ok_or(ResolveError::Unsupported("missing prototype"))?;
                let parameters = self.parameters(params, false)?;
                self.module
                    .functions
                    .push(declarator.clone().with_value(Function {
                        id,
                        name: name.into(),
                        parameters,
                        return_type: return_type.as_ref().map(|ty| (**ty).clone()),
                        linkage: linkage(item.specifiers.storage)?,
                        body: None,
                    }));
                continue;
            }
            let storage = if global {
                StorageDuration::Static
            } else {
                match item.specifiers.storage {
                    StorageClass::None | StorageClass::Auto | StorageClass::Register => {
                        StorageDuration::Automatic
                    }
                    _ => return Err(ResolveError::Unsupported("nonautomatic local")),
                }
            };
            let initializer = match &declarator.initializer {
                None => None,
                Some(Initializer::Expr(expr)) => {
                    if global {
                        return Err(ResolveError::Unsupported("global initializer"));
                    }
                    let value = self.expr(expr)?;
                    Some(self.convert_expr(expr, value, ty.clone(), ConversionReason::Assign)?)
                }
                _ => return Err(ResolveError::Unsupported("aggregate initializer")),
            };
            let variable = Variable {
                id,
                name: name.into(),
                ty,
                storage,
                initializer,
            };
            if global {
                self.module
                    .globals
                    .push(declarator.clone().with_value(Global {
                        variable,
                        linkage: linkage(item.specifiers.storage)?,
                        definition: item.specifiers.storage != StorageClass::Extern,
                    }));
            } else {
                statements.push(declarator.clone().with_value(Statement::Let(variable)));
            }
        }
        Ok(statements)
    }

    fn statements(
        &mut self,
        body: &[Stmt],
        return_type: Option<Type>,
    ) -> Result<Vec<Span<Statement>>, ResolveError> {
        let mut result = Vec::new();
        for statement in body {
            let kind = match &statement.value {
                StmtKind::Comment(_) => continue,
                StmtKind::Decl(item) => {
                    result.extend(self.declaration(item, false)?);
                    continue;
                }
                StmtKind::Expr(expr) => Statement::Expression(self.expr(expr)?),
                StmtKind::Return(expr) => {
                    let ty = return_type
                        .clone()
                        .ok_or(ResolveError::Unsupported("value return from void function"))?;
                    let value = self.expr(expr)?;
                    Statement::Return(Some(self.convert_expr(
                        expr,
                        value,
                        ty,
                        ConversionReason::Return,
                    )?))
                }
                StmtKind::ReturnVoid if return_type.is_none() => Statement::Return(None),
                StmtKind::Null => Statement::Block(Vec::new()),
                StmtKind::Block(body) => {
                    Statement::Block(self.statements(body, return_type.clone())?)
                }
                _ => return Err(ResolveError::Unsupported("module statement")),
            };
            result.push(statement.clone().with_value(kind));
        }
        Ok(result)
    }
}
