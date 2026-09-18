use super::expression::Lowerer;
use super::numeric::{Context, ResolveError};
use super::types::TypeResolver;
use crate::ast::{
    self, DeclKind, Declarator, Initializer, ParameterList, Span, Stmt, StmtKind, StorageClass,
    TranslationUnit,
};
use crate::ir::*;
use crate::standard_features::StandardFeatures;
use std::collections::HashMap;

/// Lowers an already analyzed unit; `TranslationUnit::analyze` reports the
/// diagnostics, including failed static assertions.
pub fn resolve_module(unit: &TranslationUnit) -> Result<Module, ResolveError> {
    let features = StandardFeatures::new(unit.standard);
    let context = Context::new(unit.target.clone())
        .with_options(&unit.options)
        .with_features(features);
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
        break_targets: Vec::new(),
        continue_targets: Vec::new(),
        switches: Vec::new(),
    };
    for declaration in &unit.decls {
        match &declaration.value {
            DeclKind::Comment(_) | DeclKind::StaticAssert(_) => {}
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
                let body = lower.statements(&function.body, return_type.clone())?;
                let fallthrough = if name == "main"
                    && return_type == Some(lower.context.int_type())
                    && features.main_implicit_return_zero
                {
                    Fallthrough::ReturnZero
                } else if return_type.is_none() {
                    Fallthrough::ReturnVoid
                } else {
                    Fallthrough::UndefinedIfUsed
                };
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
                        fallthrough: Some(fallthrough),
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
    for global in &mut lower.module.globals {
        if global.definition
            && let Type::Array { length, .. } = &mut global.value.variable.ty
            && length.is_none()
        {
            *length = Some(1);
        }
    }
    super::effects_statements::normalize(&mut lower.module, lower.next_id)?;
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
        if matches!(params, ParameterList::Empty) && !self.types.features.empty_parens_are_prototype
        {
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
                || matches!(
                    item.specifiers.ty,
                    ast::TypeSpecifier::Tag(ast::TagSpecifier::Definition(_))
                ))
        {
            return Err(ResolveError::Unsupported(
                "block scoped typedef or tag definition",
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
                        fallthrough: None,
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
                let declared_linkage = linkage(item.specifiers.storage)?;
                let definition = item.specifiers.storage != StorageClass::Extern
                    || variable.initializer.is_some();
                if let Some(existing) = self
                    .module
                    .globals
                    .iter_mut()
                    .find(|global| global.variable.name == name)
                {
                    if existing.value.variable.ty != variable.ty {
                        match (&existing.value.variable.ty, &variable.ty) {
                            (
                                Type::Array {
                                    element: a,
                                    length: None,
                                },
                                Type::Array {
                                    element: b,
                                    length: Some(_),
                                },
                            ) if a == b => existing.value.variable.ty = variable.ty.clone(),
                            (
                                Type::Array {
                                    element: a,
                                    length: Some(_),
                                },
                                Type::Array {
                                    element: b,
                                    length: None,
                                },
                            ) if a == b => {}
                            _ => {
                                return Err(ResolveError::Unsupported(
                                    "incompatible global redeclaration",
                                ));
                            }
                        }
                    }
                    if variable.initializer.is_some() {
                        if existing.value.variable.initializer.is_some() {
                            return Err(ResolveError::Unsupported("multiple global initializers"));
                        }
                        existing.value.variable.initializer = variable.initializer;
                    }
                    existing.value.definition |= definition;
                    if matches!(declared_linkage, Linkage::Internal) {
                        existing.value.linkage = Linkage::Internal;
                    }
                } else {
                    self.module
                        .globals
                        .push(declarator.clone().with_value(Global {
                            variable,
                            linkage: declared_linkage,
                            definition,
                        }));
                }
            } else {
                statements.push(declarator.clone().with_value(Statement::Let(variable)));
            }
        }
        Ok(statements)
    }

    fn case_value(&mut self, expr: &ast::Expr, ty: Type) -> Result<Value, ResolveError> {
        let value = self.expr(expr)?;
        let value = self.convert(value, ty.clone(), ConversionReason::Promotion)?;
        let number = super::fold::integer(&value)
            .ok_or(ResolveError::Unsupported("nonconstant case expression"))?;
        Ok(self.value(expr, ty, ValueKind::Constant(Number::SignedInteger(number))))
    }

    fn loop_body(
        &mut self,
        id: BindingId,
        body: &Stmt,
        return_type: Option<Type>,
    ) -> Result<Vec<Span<Statement>>, ResolveError> {
        self.break_targets.push(id);
        self.continue_targets.push(id);
        let result = self.statements(std::slice::from_ref(body), return_type);
        self.continue_targets.pop();
        self.break_targets.pop();
        result
    }

    fn statements(
        &mut self,
        body: &[Stmt],
        return_type: Option<Type>,
    ) -> Result<Vec<Span<Statement>>, ResolveError> {
        let mut result = Vec::new();
        for statement in body {
            let kind = match &statement.value {
                StmtKind::Comment(_) | StmtKind::StaticAssert(_) => continue,
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
                StmtKind::If {
                    condition,
                    then_branch,
                    else_branch,
                } => {
                    let value = self.expr(condition)?;
                    Statement::If {
                        condition: self.condition(value, None)?,
                        then_body: self
                            .statements(std::slice::from_ref(then_branch), return_type.clone())?,
                        else_body: else_branch
                            .as_ref()
                            .map(|body| {
                                self.statements(std::slice::from_ref(body), return_type.clone())
                            })
                            .transpose()?,
                    }
                }
                StmtKind::While { condition, body } => {
                    let id = self.fresh();
                    let value = self.expr(condition)?;
                    let condition = self.condition(value, None)?;
                    let body = self.loop_body(id, body, return_type.clone())?;
                    Statement::While {
                        id,
                        condition: condition.into(),
                        body,
                    }
                }
                StmtKind::DoWhile { body, condition } => {
                    let id = self.fresh();
                    let body = self.loop_body(id, body, return_type.clone())?;
                    let value = self.expr(condition)?;
                    Statement::DoWhile {
                        id,
                        body,
                        condition: self.condition(value, None)?.into(),
                    }
                }
                StmtKind::For {
                    init,
                    condition,
                    increment,
                    body,
                } => {
                    let id = self.fresh();
                    let init = match init {
                        Some(init) => {
                            self.statements(std::slice::from_ref(init), return_type.clone())?
                        }
                        None => Vec::new(),
                    };
                    let condition = condition
                        .as_ref()
                        .map(|expr| {
                            let value = self.expr(expr)?;
                            self.condition(value, None)
                        })
                        .transpose()?;
                    let increment = increment.as_ref().map(|expr| self.expr(expr)).transpose()?;
                    let body = self.loop_body(id, body, return_type.clone())?;
                    Statement::For {
                        id,
                        init,
                        condition: condition.map(Into::into),
                        increment: increment.map(Into::into),
                        body,
                    }
                }
                StmtKind::Break => Statement::Break(
                    *self
                        .break_targets
                        .last()
                        .ok_or(ResolveError::Unsupported("break outside loop or switch"))?,
                ),
                StmtKind::Continue => Statement::Continue(
                    *self
                        .continue_targets
                        .last()
                        .ok_or(ResolveError::Unsupported("continue outside loop"))?,
                ),
                StmtKind::Switch { discriminant, body } => {
                    let value = self.expr(discriminant)?;
                    let discriminant = self.context.promote(self.enum_integer(value));
                    if !matches!(discriminant.ty, Type::Numeric(NumericType::Integer { .. })) {
                        return Err(ResolveError::Unsupported("noninteger switch discriminant"));
                    }
                    let id = self.fresh();
                    self.break_targets.push(id);
                    self.switches.push((id, discriminant.ty.clone()));
                    let body = self.statements(std::slice::from_ref(body), return_type.clone());
                    self.switches.pop();
                    self.break_targets.pop();
                    Statement::Switch {
                        id,
                        discriminant,
                        body: body?,
                    }
                }
                StmtKind::SwitchLabel { label, body } => {
                    let (switch, ty) = self
                        .switches
                        .last()
                        .cloned()
                        .ok_or(ResolveError::Unsupported("case or default outside switch"))?;
                    match label {
                        ast::SwitchLabel::Default => Statement::Default {
                            switch,
                            body: self
                                .statements(std::slice::from_ref(body), return_type.clone())?,
                        },
                        ast::SwitchLabel::Case(start) => Statement::Case {
                            switch,
                            start: self.case_value(start, ty)?,
                            end: None,
                            body: self
                                .statements(std::slice::from_ref(body), return_type.clone())?,
                        },
                        ast::SwitchLabel::CaseRange { start, end } => Statement::Case {
                            switch,
                            start: self.case_value(start, ty.clone())?,
                            end: Some(self.case_value(end, ty)?),
                            body: self
                                .statements(std::slice::from_ref(body), return_type.clone())?,
                        },
                    }
                }
                StmtKind::Null => Statement::Null,
                StmtKind::Attribute(attributes)
                    if attributes
                        .iter()
                        .all(|a| matches!(a, ast::Attribute::Fallthrough)) =>
                {
                    if self.switches.is_empty() {
                        return Err(ResolveError::Unsupported("fallthrough outside switch"));
                    }
                    self.module
                        .metadata
                        .entry(statement.id)
                        .or_default()
                        .push(("c_attribute".into(), "fallthrough".into()));
                    Statement::Null
                }
                StmtKind::LocalLabelDecl(_) => continue,
                StmtKind::ComputedGoto(expr) => {
                    let value = self.expr(expr)?;
                    if !matches!(value.ty, Type::Pointer { .. }) {
                        return Err(ResolveError::Unsupported("nonpointer computed goto"));
                    }
                    Statement::ComputedGoto(value)
                }
                StmtKind::Goto(label) => Statement::Goto(
                    self.names
                        .references
                        .iter()
                        .find(|r| r.id == label.id)
                        .map(|r| r.binding)
                        .ok_or(ResolveError::Unsupported("missing goto binding"))?,
                ),
                StmtKind::Labeled { label, body } => Statement::Label {
                    id: *self
                        .names
                        .label_definitions
                        .get(&label.id)
                        .ok_or(ResolveError::Unsupported("missing label binding"))?,
                    name: label.value.clone(),
                    body: self.statements(std::slice::from_ref(body), return_type.clone())?,
                },
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
