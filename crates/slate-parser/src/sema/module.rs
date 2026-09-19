use super::expression::Lowerer;
use super::numeric::{Context, ResolveError};
use super::types::TypeResolver;
use crate::ast::{
    self, DeclKind, Declarator, ParameterList, Span, Stmt, StmtKind, StorageClass, TranslationUnit,
};
use crate::compiler_args::CompilerFlavor;
use crate::ir::*;
use crate::standard_features::StandardFeatures;
use crate::target_info::TargetEnvironment;
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
    let mut types = TypeResolver::with_tags(context.target.clone(), unit);
    types.references = names.references.iter().map(|r| (r.id, r.binding)).collect();
    let mut lower = Lowerer {
        types,
        module: Module::new(context.target.clone()),
        context,
        names,
        c_types: HashMap::new(),
        function_declarations: HashMap::new(),
        type_spans: HashMap::new(),
        object_requests: HashMap::new(),
        next_id,
        break_targets: Vec::new(),
        continue_targets: Vec::new(),
        switches: Vec::new(),
        in_function: false,
        return_type: None,
    };
    for declaration in &unit.decls {
        match &declaration.value {
            DeclKind::Comment(_) | DeclKind::StaticAssert(_) => {}
            DeclKind::Declaration(item) => {
                lower.declaration(item, true)?;
            }
            DeclKind::Function(function) => {
                let attributes = super::function::attributes(
                    &function.specifiers,
                    &function.declarator,
                    &function.attributes,
                );
                let symbol = function_symbol(attributes.iter().copied(), None)?;
                let name = function
                    .declarator
                    .name()
                    .ok_or(ResolveError::Unsupported("unnamed function"))?;
                let id = lower.declaration_id(declaration.id, name)?;
                lower.record_function(id, &function.specifiers, &attributes, true, true)?;
                let c_return = lower
                    .resolve_type(&function.specifiers, &Declarator::Abstract)?
                    .c
                    .spelling;
                let start = lower.types.definitions.len();
                let resolved = lower.resolve_type(&function.specifiers, &function.declarator)?;
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
                let abi = lower.abi_signature(&ty, None)?;
                lower.types.bindings.insert(id, ty);
                lower.c_types.insert(id, resolved.c.clone());
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
                let mut prologue = Vec::new();
                lower.in_function = true;
                lower.return_type = return_type.clone();
                let parameters = lower.parameters(params, Some(&mut prologue));
                let body = parameters.and_then(|parameters| {
                    let body = lower
                        .scoped(|lower| lower.statements(&function.body, return_type.clone()))?;
                    prologue.extend(body);
                    Ok((parameters, prologue))
                });
                lower.in_function = false;
                lower.return_type = None;
                let (parameters, body) = body?;
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
                lower.declare_function(declaration.clone().with_value(Function {
                    id,
                    name: name.into(),
                    parameters,
                    return_type,
                    abi,
                    linkage: linkage(function.specifiers.storage)?,
                    symbol,
                    semantics: Default::default(),
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
    lower.resolve_object_requests(unit)?;
    lower.finish_functions(unit.options.effective_inline_semantics(unit.standard));
    super::effects_statements::normalize(&mut lower.module, lower.next_id, lower.types.access)?;
    Ok(lower.module)
}

#[derive(Debug, Default, Clone, Copy)]
pub(super) struct ObjectRequest {
    alignment: Option<u64>,
    common: Option<bool>,
}

impl ObjectRequest {
    fn merge(&mut self, later: Self) {
        self.alignment = self.alignment.max(later.alignment);
        // clang lets `common` on any declaration win over `nocommon`
        self.common = self.common.max(later.common);
    }
}

fn linkage(storage: StorageClass) -> Result<Linkage, ResolveError> {
    match storage {
        StorageClass::Static => Ok(Linkage::Internal),
        StorageClass::None | StorageClass::Extern => Ok(Linkage::External),
        _ => Err(ResolveError::Unsupported("linkage storage class")),
    }
}

fn is_vector_attribute(attribute: &ast::Attribute) -> bool {
    matches!(
        attribute,
        ast::Attribute::VectorSize(_) | ast::Attribute::ExtVectorType(_)
    )
}

fn symbol_attributes<'a>(
    attributes: impl IntoIterator<Item = &'a ast::Attribute>,
    asm_label: Option<&Span<ast::AsmLabel>>,
) -> Result<SymbolAttributes, ResolveError> {
    let mut symbol = SymbolAttributes::default();
    if let Some(label) = asm_label {
        let ast::AsmLabel::Symbol(name) = &label.value else {
            return Err(ResolveError::Unsupported("register asm label"));
        };
        symbol.asm_name = Some(name.clone());
    }
    for attribute in attributes {
        match attribute {
            ast::Attribute::Visibility(name) => {
                symbol.visibility = Some(match name.as_str() {
                    "default" => Visibility::Default,
                    "hidden" => Visibility::Hidden,
                    "protected" => Visibility::Protected,
                    "internal" => Visibility::Internal,
                    _ => return Err(ResolveError::Invalid("visibility")),
                });
            }
            ast::Attribute::TlsModel(name) => {
                symbol.tls_model = Some(match name.as_str() {
                    "global-dynamic" => TlsModel::GlobalDynamic,
                    "local-dynamic" => TlsModel::LocalDynamic,
                    "initial-exec" => TlsModel::InitialExec,
                    "local-exec" => TlsModel::LocalExec,
                    _ => return Err(ResolveError::Invalid("tls_model")),
                });
            }
            ast::Attribute::Weak => symbol.weak = true,
            ast::Attribute::Alias(target) => symbol.alias = Some(target.clone()),
            ast::Attribute::Section(name) => symbol.section = Some(name.clone()),
            ast::Attribute::Used => symbol.used = true,
            ast::Attribute::Retain => symbol.retain = true,
            ast::Attribute::DllImport => symbol.dll_storage = Some(DllStorage::Import),
            ast::Attribute::DllExport => symbol.dll_storage = Some(DllStorage::Export),
            ast::Attribute::WeakRef(target) => symbol.weakref = Some(target.clone()),
            ast::Attribute::SelectAny => symbol.selectany = true,
            ast::Attribute::ThreadLocal
            | ast::Attribute::Aligned(_)
            | ast::Attribute::AlignAs(_)
            | ast::Attribute::Common
            | ast::Attribute::NoCommon => {}
            _ => return Err(ResolveError::Unsupported("declaration attribute")),
        }
    }
    Ok(symbol)
}

fn function_symbol<'a>(
    attributes: impl IntoIterator<Item = &'a ast::Attribute> + Clone,
    asm_label: Option<&Span<ast::AsmLabel>>,
) -> Result<SymbolAttributes, ResolveError> {
    if attributes
        .clone()
        .into_iter()
        .any(|attribute| matches!(attribute, ast::Attribute::ThreadLocal))
    {
        return Err(ResolveError::Invalid("thread-local function"));
    }
    symbol_attributes(
        attributes.into_iter().filter(|attribute| {
            matches!(
                attribute,
                ast::Attribute::Visibility(_)
                    | ast::Attribute::Weak
                    | ast::Attribute::Alias(_)
                    | ast::Attribute::WeakRef(_)
                    | ast::Attribute::Section(_)
                    | ast::Attribute::Used
                    | ast::Attribute::Retain
                    | ast::Attribute::DllImport
                    | ast::Attribute::DllExport
            )
        }),
        asm_label,
    )
}

impl Lowerer {
    fn resolve_object_requests(&mut self, unit: &TranslationUnit) -> Result<(), ResolveError> {
        let msvc_target = self.context.target.environment == TargetEnvironment::Msvc;
        for global in &mut self.module.globals {
            let global = &mut global.value;
            let request = self
                .object_requests
                .get(&global.variable.id)
                .copied()
                .unwrap_or_default();
            if let Some(requested) = request.alignment {
                let natural = u64::from(
                    self.types
                        .storage(global.variable.ty.clone())?
                        .alignment_bytes,
                );
                // clang honors an alignment attribute on a variable even below the type's
                let effective = if unit.flavor == CompilerFlavor::Clang {
                    requested
                } else {
                    requested.max(natural)
                };
                global.alignment = (effective != natural).then_some(effective);
            }
            let symbol = &global.symbol;
            let tentative = global.definition
                && global.variable.initializer.is_none()
                && matches!(global.linkage, Linkage::External)
                && global.variable.storage == StorageDuration::Static
                && symbol.alias.is_none()
                && symbol.section.is_none()
                && !symbol.weak
                && !symbol.selectany
                && !(msvc_target && request.alignment.is_some());
            global.common = tentative && request.common.unwrap_or(unit.options.common);
        }
        Ok(())
    }

    fn declare_global(&mut self, global: Span<Global>) -> Result<(), ResolveError> {
        let Some(existing) = self
            .module
            .globals
            .iter_mut()
            .find(|existing| existing.value.variable.id == global.value.variable.id)
        else {
            self.module.globals.push(global);
            return Ok(());
        };
        let global = global.value;
        let existing = &mut existing.value;
        if existing.variable.ty != global.variable.ty {
            match (&existing.variable.ty, &global.variable.ty) {
                (
                    Type::Array {
                        element: a,
                        length: None,
                    },
                    Type::Array {
                        element: b,
                        length: Some(_),
                    },
                ) if a == b => existing.variable.ty = global.variable.ty.clone(),
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
        self.types
            .bindings
            .insert(existing.variable.id, existing.variable.ty.clone());
        if global.variable.initializer.is_some() {
            if existing.variable.initializer.is_some() {
                return Err(ResolveError::Unsupported("multiple global initializers"));
            }
            existing.variable.initializer = global.variable.initializer;
        }
        if global.variable.storage == StorageDuration::Thread {
            existing.variable.storage = StorageDuration::Thread;
        }
        existing.definition |= global.definition;
        if matches!(global.linkage, Linkage::Internal) {
            existing.linkage = Linkage::Internal;
        }
        existing.symbol.merge(global.symbol);
        Ok(())
    }

    fn declare_function(&mut self, function: Span<Function>) {
        let Some(existing) = self
            .module
            .functions
            .iter_mut()
            .find(|existing| existing.value.id == function.value.id)
        else {
            self.module.functions.push(function);
            return;
        };
        let function = function.value;
        let linkage = match (existing.value.linkage, function.linkage) {
            (Linkage::External, Linkage::External) => Linkage::External,
            _ => Linkage::Internal,
        };
        let mut symbol = std::mem::take(&mut existing.value.symbol);
        symbol.merge(function.symbol.clone());
        let replaces = function.body.is_some()
            || (existing.value.body.is_none()
                && matches!(existing.value.parameters, Parameters::Unprototyped));
        if replaces {
            existing.value = function;
        }
        existing.value.linkage = linkage;
        existing.value.symbol = symbol;
    }

    fn parameters(
        &mut self,
        params: &ParameterList,
        mut prologue: Option<&mut Vec<Span<Statement>>>,
    ) -> Result<Parameters, ResolveError> {
        if matches!(params, ParameterList::Empty) && !self.types.features.empty_parens_are_prototype
        {
            return Ok(Parameters::Unprototyped);
        }
        let mut fixed = Vec::new();
        for parameter in params.parameters() {
            if !parameter.specifiers.attributes.is_empty() {
                return Err(ResolveError::Unsupported("parameter attributes"));
            }
            if let Some(prologue) = prologue.as_deref_mut() {
                let anchor = parameter.clone().with_value(());
                self.capture_extents(&parameter.declarator, &anchor, prologue)?;
            }
            let start = self.types.definitions.len();
            let resolved =
                self.resolve_parameter_type(&parameter.specifiers, &parameter.declarator)?;
            let mut ty = resolved
                .ty
                .clone()
                .ok_or(ResolveError::Unsupported("void parameter"))?;
            let element_access = super::types::access(resolved.c.qualifiers);
            let declared_array = parameter.declarator.array_parameter().unwrap_or_default();
            let array = match &ty {
                Type::Array { length, .. } => Some(ArrayParameter {
                    extent: length.map_or(ArrayExtent::Unspecified, ArrayExtent::Fixed),
                    guaranteed: declared_array.is_static,
                }),
                Type::VariableArray { extent, .. } => Some(ArrayParameter {
                    extent: ArrayExtent::Variable(*extent),
                    guaranteed: declared_array.is_static,
                }),
                _ => None,
            };
            let qualifiers = match ty {
                Type::Array { .. } | Type::VariableArray { .. } => declared_array.qualifiers,
                _ => resolved.c.qualifiers,
            };
            let element_const = resolved.c.qualifiers.is_const;
            ty = match ty {
                Type::Array { element, .. } | Type::VariableArray { element, .. } => {
                    self.qualified_pointer(*element, element_const, element_access)
                }
                function @ Type::Function { .. } => self.pointer(function, false),
                other => other,
            };
            let name = parameter.declarator.name();
            let id = match (prologue.is_some(), name) {
                (true, Some(name)) => self.declaration_id(parameter.id, name)?,
                _ => self.fresh(),
            };
            self.types.bindings.insert(id, ty.clone());
            self.c_types
                .insert(id, TypeResolver::parameter_c(&resolved, qualifiers));
            self.types
                .access
                .insert(id, super::types::access(qualifiers));
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
                restrict: qualifiers.is_restrict,
                is_const: qualifiers.is_const,
                array,
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
        if !global
            && matches!(
                item.specifiers.ty,
                ast::TypeSpecifier::Tag(ast::TagSpecifier::Definition(_))
            )
        {
            return Err(ResolveError::Unsupported("block scoped tag definition"));
        }
        if item.declarators.is_empty() {
            if item
                .specifiers
                .attributes
                .iter()
                .any(|attribute| !is_vector_attribute(attribute))
            {
                return Err(ResolveError::Unsupported("declaration attributes"));
            }
            self.resolve_type(&item.specifiers, &Declarator::Abstract)?;
        }
        let storage_class = item.specifiers.storage;
        let mut statements = Vec::new();
        for declarator in &item.declarators {
            let attributes = item
                .specifiers
                .attributes
                .iter()
                .chain(&declarator.attributes);
            let has_attributes = attributes
                .clone()
                .any(|attribute| !is_vector_attribute(attribute));
            if storage_class == StorageClass::Typedef
                && (has_attributes || declarator.asm_label.is_some())
            {
                return Err(ResolveError::Unsupported("typedef attributes or asm label"));
            }
            let thread = item.specifiers.is_thread_local
                || attributes
                    .clone()
                    .any(|attribute| matches!(attribute, ast::Attribute::ThreadLocal));
            let name = declarator
                .declarator
                .name()
                .ok_or(ResolveError::Unsupported("unnamed declaration"))?;
            if !global {
                let anchor = declarator.clone().with_value(());
                self.capture_extents(&declarator.declarator, &anchor, &mut statements)?;
            }
            let start = self.types.definitions.len();
            let resolved = self.resolve_type(&item.specifiers, &declarator.declarator)?;
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
            let qualifiers = resolved.c.qualifiers;
            if item.specifiers.is_constexpr && declarator.initializer.is_none() {
                return Err(ResolveError::Invalid(
                    "constexpr object requires an initializer",
                ));
            }
            let ty = resolved
                .ty
                .ok_or(ResolveError::Unsupported("void object"))?;
            let id = self.declaration_id(declarator.id, name)?;
            self.types.bindings.insert(id, ty.clone());
            self.c_types.insert(id, resolved.c.clone());
            self.types
                .access
                .insert(id, super::types::access(qualifiers));
            if let Type::Function {
                return_type,
                parameters: parameter_types,
                variadic,
                prototyped,
            } = &ty
            {
                if declarator.initializer.is_some() {
                    return Err(ResolveError::Invalid("function initializer"));
                }
                if thread {
                    return Err(ResolveError::Invalid("thread-local function"));
                }
                if !global && storage_class == StorageClass::Static {
                    return Err(ResolveError::Invalid("block scope static function"));
                }
                let attributes = super::function::attributes(
                    &item.specifiers,
                    &declarator.declarator,
                    &declarator.attributes,
                );
                let symbol =
                    function_symbol(attributes.iter().copied(), declarator.asm_label.as_ref())?;
                self.record_function(id, &item.specifiers, &attributes, false, global)?;
                let parameters = match declarator.declarator.function_parameters() {
                    Some(params) => self.parameters(params, None)?,
                    None if !prototyped => Parameters::Unprototyped,
                    None => Parameters::Prototype {
                        fixed: parameter_types
                            .iter()
                            .map(|ty| {
                                Span::new(
                                    Parameter {
                                        id: self.fresh(),
                                        name: None,
                                        ty: ty.clone(),
                                        restrict: false,
                                        is_const: false,
                                        array: None,
                                    },
                                    declarator.spelling,
                                    declarator.expansion,
                                )
                                .with_provenance(declarator.provenance)
                            })
                            .collect(),
                        variadic: *variadic,
                    },
                };
                let abi = self.abi_signature(&ty, None)?;
                self.declare_function(declarator.clone().with_value(Function {
                    id,
                    name: name.into(),
                    parameters,
                    return_type: return_type.as_ref().map(|ty| (**ty).clone()),
                    abi,
                    linkage: linkage(storage_class)?,
                    symbol,
                    semantics: Default::default(),
                    body: None,
                    fallthrough: None,
                }));
                continue;
            }
            let linked = global || storage_class == StorageClass::Extern;
            let storage = if !linked && storage_class != StorageClass::Static {
                if thread {
                    return Err(ResolveError::Invalid("thread-local automatic variable"));
                }
                StorageDuration::Automatic
            } else if thread {
                StorageDuration::Thread
            } else {
                StorageDuration::Static
            };
            if storage == StorageDuration::Automatic
                && (has_attributes || declarator.asm_label.is_some())
            {
                return Err(ResolveError::Unsupported(
                    "automatic variable attributes or asm label",
                ));
            }
            if !global && linked && declarator.initializer.is_some() {
                return Err(ResolveError::Invalid("block scope extern initializer"));
            }
            let symbol = symbol_attributes(attributes.clone(), declarator.asm_label.as_ref())?;
            let request = ObjectRequest {
                alignment: super::types::requested_alignment(&mut self.types, attributes.clone())?,
                common: if attributes
                    .clone()
                    .any(|attribute| matches!(attribute, ast::Attribute::Common))
                {
                    Some(true)
                } else if attributes
                    .clone()
                    .any(|attribute| matches!(attribute, ast::Attribute::NoCommon))
                {
                    Some(false)
                } else {
                    None
                },
            };
            self.object_requests.entry(id).or_default().merge(request);
            let (ty, initializer) = match &declarator.initializer {
                None => (ty, None),
                Some(initializer) if matches!(ty, Type::VariableArray { .. }) => {
                    if !matches!(initializer, ast::Initializer::List(items) if items.is_empty()) {
                        return Err(ResolveError::Invalid("variable length array initializer"));
                    }
                    let value = Value {
                        ty: ty.clone(),
                        node: declarator.clone().with_value(ValueKind::Aggregate {
                            members: Vec::new(),
                            zero_fill: true,
                        }),
                    };
                    (ty, Some(value))
                }
                Some(initializer) => {
                    let anchor = declarator.clone().with_value(());
                    let value = self.initializer_value(&ty, initializer, &anchor)?;
                    let ty = match ty {
                        Type::Array { length: None, .. } => value.ty.clone(),
                        ty => ty,
                    };
                    self.types.bindings.insert(id, ty.clone());
                    self.c_types
                        .insert(id, super::type_of::with_length(resolved.c.clone(), &ty));
                    (ty, Some(value))
                }
            };
            let variable = Variable {
                id,
                name: name.into(),
                ty,
                storage,
                restrict: qualifiers.is_restrict,
                is_const: qualifiers.is_const,
                constexpr: item.specifiers.is_constexpr,
                initializer,
            };
            if storage != StorageDuration::Automatic
                && matches!(variable.ty, Type::VariableArray { .. })
            {
                return Err(ResolveError::Invalid(
                    "variable length array with static storage duration",
                ));
            }
            if linked {
                let declared_linkage = if item.specifiers.is_constexpr {
                    Linkage::Internal
                } else {
                    linkage(storage_class)?
                };
                if symbol.weakref.is_some() && !matches!(declared_linkage, Linkage::Internal) {
                    return Err(ResolveError::Invalid("weakref without internal linkage"));
                }
                if symbol.selectany && !matches!(declared_linkage, Linkage::External) {
                    return Err(ResolveError::Invalid("selectany without external linkage"));
                }
                let definition = (storage_class != StorageClass::Extern
                    || variable.initializer.is_some()
                    || symbol.alias.is_some())
                    && symbol.weakref.is_none();
                self.declare_global(declarator.clone().with_value(Global {
                    variable,
                    linkage: declared_linkage,
                    symbol,
                    definition,
                    alignment: None,
                    common: false,
                }))?;
            } else if storage == StorageDuration::Automatic {
                statements.push(declarator.clone().with_value(Statement::Let(variable)));
            } else {
                self.module
                    .globals
                    .push(declarator.clone().with_value(Global {
                        variable,
                        linkage: Linkage::Internal,
                        symbol,
                        definition: true,
                        alignment: None,
                        common: false,
                    }));
            }
        }
        Ok(statements)
    }

    fn capture_extents(
        &mut self,
        declarator: &Declarator,
        anchor: &Span<()>,
        out: &mut Vec<Span<Statement>>,
    ) -> Result<(), ResolveError> {
        let mut extents = Vec::new();
        self.extents(declarator, &mut extents)?;
        out.extend(extents.into_iter().map(|(id, count)| {
            anchor.clone().with_value(Statement::Temporary {
                id,
                ty: count.ty.clone(),
                initializer: Some(count),
            })
        }));
        Ok(())
    }

    pub(super) fn extents(
        &mut self,
        declarator: &Declarator,
        out: &mut Vec<(BindingId, Value)>,
    ) -> Result<(), ResolveError> {
        match declarator {
            Declarator::Abstract | Declarator::Name(_) => Ok(()),
            Declarator::Grouped(inner)
            | Declarator::Attributed { inner, .. }
            | Declarator::Pointer { inner, .. }
            | Declarator::Function { inner, .. } => self.extents(inner, out),
            Declarator::Array { inner, size, .. } => {
                self.extents(inner, out)?;
                let ast::ArraySize::Expression(expr) = size else {
                    return Ok(());
                };
                if self.types.constant_integer(expr).is_ok() {
                    return Ok(());
                }
                let extent_type = Type::integer(self.context.target.pointer_width, false);
                let count = self.expr(expr)?;
                let count = self.convert(count, extent_type.clone(), ConversionReason::Assign)?;
                let id = self.fresh();
                self.types.bindings.insert(id, extent_type);
                self.types.extents.insert(expr.id, id);
                out.push((id, count));
                Ok(())
            }
        }
    }

    pub(super) fn scoped<T>(
        &mut self,
        lower: impl FnOnce(&mut Self) -> Result<T, ResolveError>,
    ) -> Result<T, ResolveError> {
        self.types.push_scope();
        let result = lower(self);
        self.types.pop_scope();
        result
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
        let result = self.scoped(|lower| lower.statements(std::slice::from_ref(body), return_type));
        self.continue_targets.pop();
        self.break_targets.pop();
        result
    }

    pub(super) fn statements(
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
                        then_body: self.scoped(|lower| {
                            lower.statements(std::slice::from_ref(then_branch), return_type.clone())
                        })?,
                        else_body: else_branch
                            .as_ref()
                            .map(|body| {
                                self.scoped(|lower| {
                                    lower
                                        .statements(std::slice::from_ref(body), return_type.clone())
                                })
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
                } => self.scoped(|lower| {
                    let id = lower.fresh();
                    let init = match init {
                        Some(init) => {
                            lower.statements(std::slice::from_ref(init), return_type.clone())?
                        }
                        None => Vec::new(),
                    };
                    let condition = condition
                        .as_ref()
                        .map(|expr| {
                            let value = lower.expr(expr)?;
                            lower.condition(value, None)
                        })
                        .transpose()?;
                    let increment = increment
                        .as_ref()
                        .map(|expr| lower.expr(expr))
                        .transpose()?;
                    let body = lower.loop_body(id, body, return_type.clone())?;
                    Ok(Statement::For {
                        id,
                        init,
                        condition: condition.map(Into::into),
                        increment: increment.map(Into::into),
                        body,
                    })
                })?,
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
                    let body = self.scoped(|lower| {
                        lower.statements(std::slice::from_ref(body), return_type.clone())
                    });
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
                StmtKind::Block(body) => Statement::Block(
                    self.scoped(|lower| lower.statements(body, return_type.clone()))?,
                ),
                _ => return Err(ResolveError::Unsupported("module statement")),
            };
            result.push(statement.clone().with_value(kind));
        }
        Ok(result)
    }
}
