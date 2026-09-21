use super::SemaError;
use super::ctype::QualType;
use super::expression::Lowerer;
use super::numeric::{Context, ResolveError};
use super::types::{Ordinary, TypeResolver, is_folded};
use crate::ast::{
    self, DeclKind, Declarator, ParameterList, Span, Stmt, StmtKind, StorageClass, TranslationUnit,
};
use crate::diagnostics::Warning;
use crate::ir::*;
use crate::standard_features::StandardFeatures;
use crate::target_info::TargetEnvironment;
use std::collections::HashMap;

/// Lowers an already analyzed unit; `TranslationUnit::analyze` reports the
/// diagnostics, including failed static assertions.
pub fn resolve_module(
    unit: &TranslationUnit,
    files: &crate::files::Files,
) -> Result<(Module, Vec<SemaError>), ResolveError> {
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
        function_declarations: HashMap::new(),
        type_spans: HashMap::new(),
        next_id,
        break_targets: Vec::new(),
        continue_targets: Vec::new(),
        switches: Vec::new(),
        in_function: false,
        function_name: None,
        pretty_function_name: None,
        files: files.clone(),
        return_type: None,
        diagnostic_options: unit.options.diagnostics.clone(),
        standard: unit.standard,
        diagnostics: Vec::new(),
    };
    for declaration in &unit.decls {
        match &declaration.value {
            DeclKind::Comment(_) | DeclKind::StaticAssert(_) | DeclKind::Pragma(_) => {}
            DeclKind::Asm(asm) => {
                let lowered = declaration.derive(lower.asm(asm)?);
                lower.module.asm.push(lowered);
            }
            DeclKind::Declaration(item) => {
                lower.declaration(item, true)?;
            }
            DeclKind::Function(function) => {
                let attributes = super::function::attributes(
                    &function.specifiers,
                    &function.declarator,
                    &function.attributes,
                );
                let mut symbol = function_symbol(attributes.iter().copied(), None)?;
                let name = function
                    .declarator
                    .name()
                    .ok_or(ResolveError::Unsupported("unnamed function"))?;
                lower.types.pragmas.apply(name, &mut symbol);
                let id = lower.declaration_id(declaration.id, name)?;
                lower.record_function(id, &function.specifiers, &attributes, true, true)?;
                let start = lower.types.definitions.len();
                let resolved = lower.resolve_type(&function.specifiers, &function.declarator)?;
                let (return_c, ..) = lower
                    .types
                    .ctypes
                    .function_parts(resolved)
                    .ok_or(ResolveError::Unsupported("function definition declarator"))?;
                let c_return = lower.types.render(return_c).spelling;
                for definition in &lower.types.definitions[start..] {
                    lower
                        .type_spans
                        .insert(definition.id, declaration.derive(definition.clone()));
                }
                let ty = lower.types.object_type(resolved, "void function type")?;
                let Type::Function { return_type, .. } = &ty else {
                    return Err(ResolveError::Unsupported("function definition declarator"));
                };
                let return_type = return_type.as_ref().map(|ty| (**ty).clone());
                let abi = lower.c_abi_signature(resolved, &ty, None)?;
                let previous = lower.types.entities.declare(id, resolved, false);
                let mut metadata = vec![
                    (
                        "c_storage".into(),
                        function.specifiers.storage.as_str().into(),
                    ),
                    ("c_return".into(), c_return),
                ];
                metadata.extend(lower.types.render(resolved).entries());
                let params = function
                    .declarator
                    .function_parameters()
                    .ok_or(ResolveError::Unsupported("missing function parameters"))?;
                let mut prologue = Vec::new();
                lower.in_function = true;
                lower.function_name = Some(name.to_string());
                lower.pretty_function_name = Some(lower.types.declaration_spelling(resolved, name));
                lower.return_type = return_type.as_ref().map(|_| return_c);
                let body = lower.scoped(|lower| {
                    let parameters = lower.parameters(params, Some(&mut prologue))?;
                    let body = lower.scoped(|lower| {
                        lower.statements(&function.body, return_type.as_ref().map(|_| return_c))
                    })?;
                    prologue.extend(body);
                    Ok((parameters, prologue))
                });
                lower.in_function = false;
                lower.function_name = None;
                lower.pretty_function_name = None;
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
                let lowered = declaration.derive(Function {
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
                });
                lower.module.annotate(&lowered, metadata);
                lower.declare_function(lowered, previous)?;
            }
        }
    }
    for definition in &lower.types.definitions {
        if let Some(span) = lower.type_spans.get(&definition.id) {
            lower.module.types.push(span.clone());
        } else if let Some(tag) = lower.types.tag_span(definition.id, unit) {
            lower.module.types.push(tag.derive(definition.clone()));
        } else if let Some(declaration) = unit.decls.first() {
            lower
                .module
                .types
                .push(declaration.derive(definition.clone()));
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
    let declared: Vec<_> = lower.types.entities.types().collect();
    let access = declared
        .into_iter()
        .map(|(id, c)| (id, lower.types.access_of(c)))
        .collect();
    super::effects_statements::normalize(&mut lower.module, lower.next_id, access)?;
    Ok((lower.module, lower.diagnostics))
}

fn linkage(storage: StorageClass) -> Result<Linkage, ResolveError> {
    match storage {
        StorageClass::Static => Ok(Linkage::Internal),
        StorageClass::None | StorageClass::Extern => Ok(Linkage::External),
        _ => Err(ResolveError::Unsupported("linkage storage class")),
    }
}

fn reject_unsupported<'a>(
    attributes: impl IntoIterator<Item = &'a ast::Attribute>,
) -> Result<(), ResolveError> {
    reject_with(attributes, super::attributes::unsupported)
}

fn reject_with<'a>(
    attributes: impl IntoIterator<Item = &'a ast::Attribute>,
    classify: impl Fn(&ast::Attribute) -> Option<&'static str>,
) -> Result<(), ResolveError> {
    for attribute in attributes {
        if let Some(reason) = classify(attribute) {
            return Err(ResolveError::Unsupported(reason));
        }
    }
    Ok(())
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
            other => {
                if let Some(reason) = super::attributes::unsupported(other) {
                    return Err(ResolveError::Unsupported(reason));
                }
            }
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
            let request = self.types.entities.request(&global.variable.id);
            if let Some(requested) = request.alignment {
                let natural = u64::from(
                    self.types
                        .storage(global.variable.ty.clone())?
                        .alignment_bytes,
                );
                let effective = self.types.effective_alignment(requested, natural);
                global.variable.alignment = (effective != natural).then_some(effective);
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

    fn declare_global(
        &mut self,
        global: Span<Global>,
        previous: Option<QualType>,
    ) -> Result<(), ResolveError> {
        let id = global.value.variable.id;
        let declared = self
            .types
            .entities
            .ty(&id)
            .ok_or(ResolveError::Unsupported("untyped global redeclaration"))?;
        if let Some(message) = self.types.merge_redeclaration(id, previous, declared)? {
            self.warn(Warning::ConflictingTypes, message, &global);
        }
        let Some(index) = self
            .module
            .globals
            .iter()
            .position(|existing| existing.value.variable.id == id)
        else {
            self.module.globals.push(global);
            return Ok(());
        };
        let merged = self
            .types
            .entities
            .ty(&id)
            .ok_or(ResolveError::Unsupported("untyped global redeclaration"))?;
        let merged_ty = self.types.ir_type(merged);
        let merged_quals = self.types.ctypes.quals(merged);
        let merged_access = self.types.access_of(merged);
        let global = global.value;
        let existing = &mut self.module.globals[index].value;
        existing.variable.ty = merged_ty;
        existing.variable.restrict = merged_quals.is_restrict;
        existing.variable.is_const = merged_quals.is_const;
        existing.variable.access = merged_access;
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

    fn declare_function(
        &mut self,
        function: Span<Function>,
        previous: Option<QualType>,
    ) -> Result<(), ResolveError> {
        let id = function.value.id;
        if let Some(declared) = self.types.entities.ty(&id)
            && let Some(message) = self.types.merge_redeclaration(id, previous, declared)?
        {
            self.warn(Warning::ConflictingTypes, message, &function);
        }
        let Some(index) = self
            .module
            .functions
            .iter()
            .position(|existing| existing.value.id == id)
        else {
            self.module.functions.push(function);
            return Ok(());
        };
        let existing = &mut self.module.functions[index];
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
        Ok(())
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
                let anchor = parameter.derive(());
                self.capture_extents(&parameter.declarator, &anchor, prologue)?;
            }
            let start = self.types.definitions.len();
            let resolved =
                self.resolve_parameter_type(&parameter.specifiers, &parameter.declarator)?;
            let declared_array = parameter.declarator.array_parameter().unwrap_or_default();
            let shape = self.types.parameter_shape(resolved, declared_array)?;
            let adjusted = shape.adjusted;
            let name = parameter.declarator.name();
            let id = match (prologue.is_some(), name) {
                (true, Some(name)) => self.declaration_id(parameter.id, name)?,
                _ => self.fresh(),
            };
            self.types.entities.declare(
                id,
                adjusted,
                parameter.specifiers.storage == StorageClass::Register,
            );
            for definition in &self.types.definitions[start..] {
                self.type_spans
                    .insert(definition.id, parameter.derive(definition.clone()));
            }
            let lowered = parameter.derive(Parameter {
                id,
                name: name.map(str::to_owned),
                ty: shape.ty,
                restrict: shape.qualifiers.is_restrict,
                is_const: shape.qualifiers.is_const,
                access: Access {
                    volatile: shape.qualifiers.is_volatile,
                    atomic: shape.qualifiers.is_atomic,
                },
                array: shape.array,
            });
            let c_entries = self.types.render(resolved).entries();
            self.module.annotate(&lowered, c_entries);
            fixed.push(lowered);
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
        if item.declarators.is_empty() {
            reject_unsupported(&item.specifiers.attributes)?;
            if let ast::TypeSpecifier::Tag(ast::TagSpecifier::Reference {
                kind,
                name,
                fixed_type: None,
            }) = &item.specifiers.ty
            {
                self.types.declare_incomplete_tag(*kind, name);
            } else {
                self.resolve_type(&item.specifiers, &Declarator::Abstract)?;
            }
        }
        let storage_class = item.specifiers.storage;
        let mut statements = Vec::new();
        for declarator in &item.declarators {
            let attributes = item
                .specifiers
                .attributes
                .iter()
                .chain(&declarator.attributes);
            if storage_class == StorageClass::Typedef {
                reject_with(attributes.clone(), super::attributes::typedef_unsupported)?;
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
                let anchor = declarator.derive(());
                self.capture_extents(&declarator.declarator, &anchor, &mut statements)?;
            }
            let start = self.types.definitions.len();
            let resolved = self.resolve_type(&item.specifiers, &declarator.declarator)?;
            let c_entries = self.types.render(resolved).entries();
            if item.specifiers.storage == StorageClass::Typedef {
                self.types.define_alias(name.into(), resolved)?;
                for definition in &self.types.definitions[start..] {
                    let span = declarator.derive(definition.clone());
                    if matches!(definition.kind, TypeDefinitionKind::Alias(_)) {
                        self.module.annotate(&span, c_entries.clone());
                    }
                    self.type_spans.insert(definition.id, span);
                }
                continue;
            }
            for definition in &self.types.definitions[start..] {
                self.type_spans
                    .insert(definition.id, declarator.derive(definition.clone()));
            }
            let qualifiers = self.types.ctypes.quals(resolved);
            if item.specifiers.is_constexpr && declarator.initializer.is_none() {
                return Err(ResolveError::Invalid(
                    "constexpr object requires an initializer",
                ));
            }
            let ty = match self.types.layout(resolved) {
                Some(ty) => ty,
                None if storage_class == StorageClass::Extern => self.types.ir_type(resolved),
                None => return Err(ResolveError::Invalid("object cannot have type void")),
            };
            let id = self.declaration_id(declarator.id, name)?;
            let previous =
                self.types
                    .entities
                    .declare(id, resolved, storage_class == StorageClass::Register);
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
                let mut symbol =
                    function_symbol(attributes.iter().copied(), declarator.asm_label.as_ref())?;
                self.types.pragmas.apply(name, &mut symbol);
                self.record_function(id, &item.specifiers, &attributes, false, global)?;
                let parameters = match declarator.declarator.function_parameters() {
                    Some(params) => self.scoped(|lower| lower.parameters(params, None))?,
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
                                        access: Access::default(),
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
                let abi = self.c_abi_signature(resolved, &ty, None)?;
                let lowered = declarator.derive(Function {
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
                });
                self.module.annotate(&lowered, c_entries);
                self.declare_function(lowered, previous)?;
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
            reject_unsupported(attributes.clone())?;
            if !global && linked && declarator.initializer.is_some() {
                return Err(ResolveError::Invalid("block scope extern initializer"));
            }
            let mut symbol = symbol_attributes(attributes.clone(), declarator.asm_label.as_ref())?;
            self.types.pragmas.apply(name, &mut symbol);
            let request = super::entity::ObjectRequest {
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
            self.types.entities.merge_request(id, request)?;
            let automatic_alignment = match (storage, request.alignment) {
                (StorageDuration::Automatic, Some(requested)) => {
                    let natural = u64::from(self.types.storage(ty.clone())?.alignment_bytes);
                    let effective = self.types.effective_alignment(requested, natural);
                    (effective != natural).then_some(effective)
                }
                _ => None,
            };
            let (ty, initializer) = match &declarator.initializer {
                None => (ty, None),
                Some(initializer) if matches!(ty, Type::VariableArray { .. }) => {
                    if !matches!(initializer, ast::Initializer::List(items) if items.is_empty()) {
                        return Err(ResolveError::Invalid("variable length array initializer"));
                    }
                    let value = Value {
                        ty: ty.clone(),
                        node: declarator.derive(ValueKind::Aggregate {
                            members: Vec::new(),
                            zero_fill: true,
                        }),
                    };
                    (ty, Some(value))
                }
                Some(initializer) => {
                    let anchor = declarator.derive(());
                    let value = self.initializer_value(resolved, initializer, &anchor)?;
                    let ty = match ty {
                        Type::Array { length: None, .. } => value.ty.clone(),
                        ty => ty,
                    };
                    let completed = self.with_length(resolved, &ty);
                    self.types.entities.declare(
                        id,
                        completed,
                        storage_class == StorageClass::Register,
                    );
                    (ty, Some(value))
                }
            };
            if item.specifiers.is_constexpr
                && let Some(value) = &initializer
                && is_folded(value)
            {
                self.types.declare(
                    name,
                    Ordinary::Constant(super::operand::Operand {
                        value: value.clone(),
                        c: resolved,
                    }),
                );
            }
            let variable = Variable {
                id,
                name: name.into(),
                ty,
                storage,
                restrict: qualifiers.is_restrict,
                is_const: qualifiers.is_const,
                access: Access {
                    volatile: qualifiers.is_volatile,
                    atomic: qualifiers.is_atomic,
                },
                constexpr: item.specifiers.is_constexpr,
                alignment: automatic_alignment,
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
                let global = declarator.derive(Global {
                    variable,
                    linkage: declared_linkage,
                    symbol,
                    definition,
                    common: false,
                });
                self.module.annotate(&global, c_entries);
                self.declare_global(global, previous)?;
            } else if storage == StorageDuration::Automatic {
                let binding = declarator.derive(Statement::Let(variable));
                self.module.annotate(&binding, c_entries);
                statements.push(binding);
            } else {
                let global = declarator.derive(Global {
                    variable,
                    linkage: Linkage::Internal,
                    symbol,
                    definition: true,
                    common: false,
                });
                self.module.annotate(&global, c_entries);
                self.module.globals.push(global);
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
            anchor.derive(Statement::Temporary {
                id,
                ty: count.ty.clone(),
                initializer: Some(count),
                unsequenced: false,
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
                let extent_type = self.types.ctypes.size_type(&self.context.target);
                let count = self.expr(expr)?;
                let count = self.convert(count, extent_type, ConversionReason::Assign)?;
                let id = self.fresh();
                self.types.entities.declare(id, extent_type, false);
                self.types.extents.insert(expr.id, id);
                out.push((id, count.value));
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

    fn case_value(&mut self, expr: &ast::Expr, ty: QualType) -> Result<Value, ResolveError> {
        let value = self.expr(expr)?;
        let value = self.convert(value, ty, ConversionReason::Promotion)?;
        let number = super::fold::integer(&value)
            .ok_or(ResolveError::Unsupported("nonconstant case expression"))?;
        Ok(self.value(
            expr,
            self.types.ir_type(ty),
            ValueKind::Constant(Number::SignedInteger(number)),
        ))
    }

    fn loop_body(
        &mut self,
        id: BindingId,
        body: &Stmt,
        return_type: Option<QualType>,
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
        return_type: Option<QualType>,
    ) -> Result<Vec<Span<Statement>>, ResolveError> {
        let mut result = Vec::new();
        for statement in body {
            let mut annotations = Vec::new();
            let kind = match &statement.value {
                StmtKind::Comment(_) | StmtKind::StaticAssert(_) | StmtKind::Pragma(_) => continue,
                StmtKind::Decl(item) => {
                    result.extend(self.declaration(item, false)?);
                    continue;
                }
                StmtKind::Expr(expr) => Statement::Expression(self.expr(expr)?.value),
                StmtKind::Return(expr) => {
                    let ty = return_type
                        .ok_or(ResolveError::Unsupported("value return from void function"))?;
                    let value = self.expr(expr)?;
                    Statement::Return(Some(
                        self.convert_expr(expr, value, ty, ConversionReason::Return)?
                            .value,
                    ))
                }
                StmtKind::ReturnVoid if return_type.is_none() => Statement::Return(None),
                StmtKind::If {
                    condition,
                    then_branch,
                    else_branch,
                } => {
                    let value = self.expr(condition)?;
                    Statement::If {
                        condition: self.condition(value.value, None)?,
                        then_body: self.scoped(|lower| {
                            lower.statements(std::slice::from_ref(then_branch), return_type)
                        })?,
                        else_body: else_branch
                            .as_ref()
                            .map(|body| {
                                self.scoped(|lower| {
                                    lower.statements(std::slice::from_ref(body), return_type)
                                })
                            })
                            .transpose()?,
                    }
                }
                StmtKind::While { condition, body } => {
                    let id = self.fresh();
                    let value = self.expr(condition)?;
                    let condition = self.condition(value.value, None)?;
                    let body = self.loop_body(id, body, return_type)?;
                    Statement::While {
                        id,
                        condition: condition.into(),
                        body,
                    }
                }
                StmtKind::DoWhile { body, condition } => {
                    let id = self.fresh();
                    let body = self.loop_body(id, body, return_type)?;
                    let value = self.expr(condition)?;
                    Statement::DoWhile {
                        id,
                        body,
                        condition: self.condition(value.value, None)?.into(),
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
                        Some(init) => lower.statements(std::slice::from_ref(init), return_type)?,
                        None => Vec::new(),
                    };
                    let condition = condition
                        .as_ref()
                        .map(|expr| {
                            let value = lower.expr(expr)?;
                            lower.condition(value.value, None)
                        })
                        .transpose()?;
                    let increment = increment
                        .as_ref()
                        .map(|expr| lower.expr(expr))
                        .transpose()?;
                    let body = lower.loop_body(id, body, return_type)?;
                    Ok(Statement::For {
                        id,
                        init,
                        condition: condition.map(Into::into),
                        increment: increment.map(|value| value.value.into()),
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
                    let discriminant = self.promote(value)?;
                    if !matches!(discriminant.ty, Type::Numeric(NumericType::Integer { .. })) {
                        return Err(ResolveError::Unsupported("noninteger switch discriminant"));
                    }
                    let id = self.fresh();
                    self.break_targets.push(id);
                    self.switches.push((id, discriminant.c));
                    let body = self
                        .scoped(|lower| lower.statements(std::slice::from_ref(body), return_type));
                    self.switches.pop();
                    self.break_targets.pop();
                    Statement::Switch {
                        id,
                        discriminant: discriminant.value,
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
                            body: self.statements(std::slice::from_ref(body), return_type)?,
                        },
                        ast::SwitchLabel::Case(start) => Statement::Case {
                            switch,
                            start: self.case_value(start, ty)?,
                            end: None,
                            body: self.statements(std::slice::from_ref(body), return_type)?,
                        },
                        ast::SwitchLabel::CaseRange { start, end } => Statement::Case {
                            switch,
                            start: self.case_value(start, ty)?,
                            end: Some(self.case_value(end, ty)?),
                            body: self.statements(std::slice::from_ref(body), return_type)?,
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
                    annotations.push(("c_attribute".into(), "fallthrough".into()));
                    Statement::Null
                }
                StmtKind::LocalLabelDecl(_) => continue,
                StmtKind::ComputedGoto(expr) => {
                    let value = self.expr(expr)?;
                    if !matches!(value.ty, Type::Pointer { .. }) {
                        return Err(ResolveError::Unsupported("nonpointer computed goto"));
                    }
                    Statement::ComputedGoto(value.value)
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
                    body: self.statements(std::slice::from_ref(body), return_type)?,
                },
                StmtKind::Asm(asm) => Statement::Asm(Box::new(self.asm(asm)?)),
                StmtKind::Block(body) => {
                    Statement::Block(self.scoped(|lower| lower.statements(body, return_type))?)
                }
                _ => return Err(ResolveError::Unsupported("module statement")),
            };
            let lowered = statement.derive(kind);
            self.module.annotate(&lowered, annotations);
            result.push(lowered);
        }
        Ok(result)
    }
}
