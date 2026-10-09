use super::names::ItemResolution;
use crate::ast::*;
use crate::compiler_args::CompilerFlavor;
use crate::const_expr::{AssignOp, BinaryOp, UnaryOp};
use crate::diagnostics::Warning;
use crate::ir::{
    BindingId, Linkage, NameResolution, Number, NumericType, SymbolAttributes, Type, Value,
    ValueKind,
};
use crate::visit::{self, Visitor};
use miette::Severity;
use num_bigint::{BigInt, Sign};

use super::attributes::Subject;
use super::ctype::convert::ConversionContext;
use super::ctype::{Extent, QualType};
use super::entity::ObjectRequest;
use super::function::{builtin_deallocator, deallocator_argument};
use super::initializer::{ElementError, InitializerSource};
use super::module::{applies, function_symbol, linkage as declared_linkage, symbol_attributes};
use super::numeric::{Context, ResolveError};
use super::pragmas::{FloatingPragmas, FloatingRegion, PragmaPlacement};
use super::typer::{Choice, Slot};
use super::types::TypeResolver;
use super::validate::{SemaError, error};
use std::collections::{HashMap, HashSet};

pub(super) fn validate(
    unit: &TranslationUnit,
    types: &mut TypeResolver,
    names: &NameResolution,
    items: &[ItemResolution],
) -> Vec<SemaError> {
    let region = Context::for_dialect(&unit.dialect).region;
    let mut checker = Checker {
        unit,
        types,
        errors: Vec::new(),
        context: StatementContext::default(),
        initialized: HashSet::new(),
        inlining: HashMap::new(),
        rejected: HashSet::new(),
        pragmas: FloatingPragmas::new(unit.dialect.flavor(), region),
        region,
        compound_start: false,
        in_function: false,
    };
    for (declaration, item) in unit.decls.iter().zip(items) {
        for id in item.declared.clone().map(BindingId) {
            if names.implicit_functions.contains(&id) {
                let ty = checker.types.ctypes.implicit_function();
                checker.types.entities.declare(id, ty, false);
                checker.types.entities.merge_declaration(
                    id,
                    Linkage::External,
                    None,
                    false,
                    SymbolAttributes::default(),
                );
            }
        }
        match &declaration.value {
            DeclKind::StaticAssert(assertion) => checker.assertion(assertion),
            DeclKind::Pragma(pragma) => {
                checker.pragma(declaration, &pragma.kind, PragmaPlacement::File)
            }
            DeclKind::Declaration(inner) => {
                checker.declaration(&declaration.derive(()), inner, true)
            }
            DeclKind::Function(function) => checker.function(
                declaration.id,
                declaration.derive(()),
                Some(declaration.derive(())),
                function,
            ),
            _ => {}
        }
        let diagnostics = std::mem::take(&mut checker.types.diagnostics);
        if diagnostics.iter().any(|d| d.severity == Severity::Error) {
            checker.errors.extend(diagnostics);
        } else if !diagnostics.is_empty() {
            checker
                .types
                .item_diagnostics
                .insert(declaration.id, diagnostics);
        }
    }
    checker.errors
}

struct Checker<'a> {
    unit: &'a TranslationUnit,
    types: &'a mut TypeResolver,
    errors: Vec<SemaError>,
    context: StatementContext,
    initialized: HashSet<BindingId>,
    inlining: HashMap<BindingId, bool>,
    rejected: HashSet<NodeId>,
    pragmas: FloatingPragmas,
    region: FloatingRegion,
    compound_start: bool,
    in_function: bool,
}

#[derive(Default)]
struct StatementContext {
    returns: Returns,
    loops: usize,
    breakables: usize,
    switches: Vec<Option<QualType>>,
    labels: Vec<String>,
    named_targets: Vec<NamedTarget>,
}

struct NamedTarget {
    labels: Vec<String>,
    statement: NodeId,
    is_loop: bool,
}

#[derive(Default, Clone, Copy)]
enum Returns {
    #[default]
    Unknown,
    Void,
    Value(QualType),
}

impl Checker<'_> {
    fn assertion(&mut self, assertion: &StaticAssert) {
        self.errors
            .extend(static_assertion_error(self.types, assertion));
    }

    fn declare_object<T>(
        &mut self,
        at: &Span<T>,
        node: NodeId,
        ty: QualType,
        alignment: Option<u64>,
        linkage: Option<Linkage>,
        register: bool,
    ) -> Result<(), ResolveError> {
        let Some(&id) = self.types.declarations.get(&node) else {
            return Ok(());
        };
        if let Some(overloads) = self.types.overload_sets.get(&id).cloned() {
            let passes_object_size = self.types.passes_object_size.contains(&id);
            let redeclares = overloads.iter().filter(|&&other| other != id).any(|other| {
                self.types.passes_object_size.contains(other) == passes_object_size
                    && self
                        .types
                        .entities
                        .ty(other)
                        .is_some_and(|earlier| self.types.ctypes.compatible(earlier, ty))
            });
            if redeclares {
                let error =
                    ResolveError::Unimplemented("redeclaration of an overloadable function");
                self.types
                    .unsupported_declarations
                    .insert(node, error.clone());
                return Err(error);
            }
            if matches!(linkage, Some(Linkage::External)) {
                self.types.external_overloads.insert(id);
            }
        }
        let ty = self.types.inherit_convention(id, ty);
        let previous = self.types.entities.ty(&id);
        self.types.entities.declare(id, ty, register);
        let merged = match self.types.merge_redeclaration(id, previous, ty) {
            Ok(Some(message)) => {
                self.types.warn(Warning::ConflictingTypes, message, at);
                Ok(())
            }
            merged => merged.map(drop),
        };
        if let Some(declared) = self.types.entities.ty(&id) {
            self.types.declared_types.insert(node, declared);
        }
        if let Some(linkage) = linkage {
            self.types.entities.merge_declaration(
                id,
                linkage,
                None,
                false,
                SymbolAttributes::default(),
            );
        }
        let _ = self.types.entities.merge_request(
            id,
            ObjectRequest {
                alignment,
                common: None,
            },
        );
        merged
    }

    fn function(
        &mut self,
        node: NodeId,
        at: Span<()>,
        owner: Option<Span<()>>,
        function: &FunctionDefinition,
    ) {
        let enclosing_scope = std::mem::replace(&mut self.in_function, true);
        let owner_linkage = owner.is_some();
        let attributes = function
            .specifiers
            .attributes_with(&function.declarator, &function.attributes)
            .collect::<Vec<_>>();
        self.function_rules(&at, node, &function.specifiers, &attributes, None);
        self.weakref(&at, attributes.iter().copied(), true, false);
        let identifier_list = matches!(
            function.declarator.function_parameters(),
            Some(ParameterList::IdentifierList { .. })
        );
        for parameter in function
            .declarator
            .function_parameters()
            .map_or(&[][..], ParameterList::parameters)
        {
            self.parameter_rules(parameter);
            self.specifier(&parameter.specifiers.ty);
            self.declarator(&parameter.declarator);
            let owner = self.types.owner.replace(parameter.derive(()));
            let provisional = std::mem::replace(&mut self.types.provisional_extents, true);
            self.tag(&parameter.specifiers.ty);
            let resolved = self
                .types
                .resolve(&parameter.specifiers, &parameter.declarator);
            self.types.provisional_extents = provisional;
            self.types.owner = owner;
            self.resolution(parameter, &resolved);
            if let Ok(resolved) = resolved
                && !self.types.ctypes.is_void(resolved)
            {
                let declared_array = parameter.declarator.array_parameter().unwrap_or_default();
                let adjusted = self
                    .types
                    .adjusted_parameter(resolved, declared_array.qualifiers.into());
                if self.types.is_incomplete_record(adjusted) {
                    self.reject(parameter, "variable has incomplete type");
                }
                let register = parameter.specifiers.storage == StorageClass::Register;
                let declared =
                    self.declare_object(parameter, parameter.id, adjusted, None, None, register);
                self.report(parameter, declared);
                if identifier_list
                    && let Err(rejection) = self
                        .types
                        .record_promoted_parameter(parameter, resolved, adjusted)
                {
                    self.reject_with(parameter, rejection);
                }
            }
        }
        let mut names = None;
        let mut returns = Returns::Unknown;
        let owner = std::mem::replace(&mut self.types.owner, owner);
        let resolved = self
            .types
            .resolve(&function.specifiers, &function.declarator);
        self.types.owner = owner;
        self.resolution(&at, &resolved);
        if let Ok(ty) = resolved {
            let ty = self.types.apply_convention(ty, &function.attributes);
            let linkage = owner_linkage.then(|| linkage(function.specifiers.storage));
            let declared = self.declare_object(&at, node, ty, None, linkage.flatten(), false);
            self.report(&at, declared);
            if let Some(id) = self.types.declarations.get(&node)
                && self.types.external_overloads.contains(id)
            {
                self.types.unsupported_declarations.insert(
                    node,
                    ResolveError::Unimplemented("definition of an extern overloadable function"),
                );
            }
            names = function
                .declarator
                .name()
                .map(|name| self.types.function_names(ty, name));
            if let Some((returned, ..)) = self.types.ctypes.function_parts(ty) {
                if self.types.is_incomplete_record(returned) {
                    self.reject(&at, "incomplete result type in function definition");
                }
                returns = if self.types.ctypes.is_void(returned) {
                    Returns::Void
                } else {
                    Returns::Value(returned)
                };
            }
        }
        let enclosing_context = std::mem::replace(
            &mut self.context,
            StatementContext {
                returns,
                ..StatementContext::default()
            },
        );
        let enclosing = std::mem::replace(&mut self.types.function_names, names);
        self.compound(&function.body);
        self.types.function_names = enclosing;
        self.context = enclosing_context;
        self.in_function = enclosing_scope;
    }

    fn declaration(&mut self, at: &Span<()>, declaration: &Declaration, global: bool) {
        if declaration.declarators.is_empty() {
            if !self.types.declare_forward_tag(&declaration.specifiers) {
                self.tag(&declaration.specifiers.ty);
                let resolved = self
                    .types
                    .resolve(&declaration.specifiers, &Declarator::Abstract);
                self.resolution(at, &resolved);
            }
            return;
        }
        let first = declaration.declarators.first();
        let owner = std::mem::replace(
            &mut self.types.owner,
            first.map(|declarator| declarator.derive(())),
        );
        self.specifier(&declaration.specifiers.ty);
        self.tag(&declaration.specifiers.ty);
        self.types.owner = owner;
        let inferred = matches!(declaration.specifiers.ty, TypeSpecifier::Inferred);
        if inferred
            && self.types.flavor().is_gcc()
            && let [_, second, ..] = declaration.declarators.as_slice()
        {
            self.reject(second, "'auto' may only be used with a single declarator");
        }
        let mut deduced = None;
        for declarator in &declaration.declarators {
            self.declarator(&declarator.declarator);
            let Some(name) = declarator.declarator.name() else {
                continue;
            };
            let deduced_value = if inferred {
                self.deduction(declaration, declarator, &mut deduced)
            } else {
                None
            };
            let owner = self.types.owner.replace(declarator.derive(()));
            let mut initialized = None;
            let provisional = std::mem::replace(&mut self.types.provisional_extents, true);
            let resolved = self
                .types
                .declarator_type(&declaration.specifiers, declarator);
            self.types.provisional_extents = provisional;
            self.resolution(at, &resolved);
            self.types
                .declarator_types
                .insert(declarator.id, resolved.clone());
            if let Ok(resolved) = resolved
                && declaration.specifiers.storage == StorageClass::Typedef
            {
                let attributes = declaration
                    .specifiers
                    .attributes_with(&declarator.declarator, &declarator.attributes);
                let result = self
                    .types
                    .attribute_error(attributes.clone(), Subject::Typedef);
                self.report(declarator, result);
                if self.types.ctypes.is_variably_modified(resolved) {
                    self.types
                        .declare_provisional_alias(declarator.id, name.to_owned(), resolved);
                } else {
                    let result = self
                        .types
                        .define_alias(declarator.id, name.to_owned(), resolved, attributes)
                        .map(drop);
                    self.report(declarator, result);
                }
            }
            self.types.owner = owner;
            if let (Ok(resolved), Some(value)) = (&resolved, deduced_value) {
                let resolved = *resolved;
                let result = self.types.check_inferred(resolved, value);
                self.report(declarator, result);
            }
            if let Ok(resolved) = resolved
                && declaration.specifiers.storage != StorageClass::Typedef
            {
                self.object_rules(declaration, declarator, global, resolved);
            }
            if let Ok(resolved) = resolved
                && declaration.specifiers.storage != StorageClass::Typedef
                && (!self.types.ctypes.is_void(resolved)
                    || declaration.specifiers.storage == StorageClass::Extern)
            {
                let sized = self.previously_sized(declarator.id, resolved);
                let completed = self
                    .types
                    .completed_array(sized, declarator.initializer.as_ref());
                let attributes = declaration
                    .specifiers
                    .attributes_with(&declarator.declarator, &declarator.attributes);
                let requested = match super::types::requested_alignment(self.types, attributes) {
                    Ok(requested) => requested,
                    Err(error) => {
                        self.report(declarator, Err(error));
                        None
                    }
                };
                let storage = declaration.specifiers.storage;
                let linkage = if global
                    || storage == StorageClass::Extern
                    || self.types.ctypes.is_function(completed)
                {
                    linkage(storage)
                } else {
                    None
                };
                let register = storage == StorageClass::Register;
                let declared = self.declare_object(
                    declarator,
                    declarator.id,
                    completed,
                    requested,
                    linkage,
                    register,
                );
                self.report(declarator, declared);
                if !self.types.ctypes.is_function(completed) && !self.variable_array(completed) {
                    initialized = Some(completed);
                }
                if declaration.specifiers.is_constexpr
                    && let Some(Initializer::Expr(expr)) = &declarator.initializer
                    && ice_shape(self.types, expr).is_constant()
                    && let Ok(value) = self.types.constant_value(expr)
                {
                    self.types.declare_constant(declarator.id, value);
                }
            }
            if let Some(initializer) = &declarator.initializer {
                if (global || declaration.specifiers.storage == StorageClass::Static)
                    && let Err((expr, reason)) = (InvalidConstantArithmetic {
                        types: self.types,
                        flavor: self.unit.dialect.flavor(),
                    })
                    .visit_initializer(initializer)
                {
                    self.errors.push(error(
                        expr.provenance,
                        expr.expansion,
                        format!("initializer element is not a compile-time constant: {reason}"),
                    ));
                }
                self.initializer(initializer);
                if let Some(to) = initialized {
                    let result = self.types.record_initializer(
                        declarator.id,
                        to,
                        InitializerSource::Initializer(initializer),
                    );
                    self.element(declarator, result);
                }
            }
        }
    }

    fn constexpr_literal_type(&self, c: QualType) -> bool {
        let qualifiers = self.types.ctypes.quals(c);
        if qualifiers.is_volatile || qualifiers.is_atomic || qualifiers.is_restrict {
            return false;
        }
        match self.types.ctypes.canonical_kind(c) {
            super::ctype::CTypeKind::Array { element, .. } => self.constexpr_literal_type(*element),
            super::ctype::CTypeKind::Record { id, .. } => self
                .types
                .record_fields
                .get(id)
                .is_none_or(|fields| fields.iter().all(|c| self.constexpr_literal_type(*c))),
            _ => true,
        }
    }

    fn constant_literal_initializer(&mut self, initializer: &Initializer, constexpr: bool) {
        match initializer {
            Initializer::List(items) => {
                for item in items {
                    self.constant_literal_initializer(&item.value, constexpr);
                }
            }
            Initializer::Expr(expr) => {
                if let Err((at, reason)) = (InvalidConstantArithmetic {
                    types: self.types,
                    flavor: self.unit.dialect.flavor(),
                })
                .visit_expr(expr)
                {
                    self.errors.push(error(
                        at.provenance,
                        at.expansion,
                        format!("initializer element is not a compile-time constant: {reason}"),
                    ));
                }
                if !literal_initializer_constant(self.types, expr, constexpr) {
                    self.reject(
                        expr,
                        "compound literal initializer is not a constant expression",
                    );
                }
            }
        }
    }

    fn previously_sized(&mut self, node: NodeId, resolved: QualType) -> QualType {
        if !matches!(
            self.types.ctypes.element(resolved),
            Some((_, Extent::Incomplete))
        ) {
            return resolved;
        }
        self.types
            .declarations
            .get(&node)
            .and_then(|id| self.types.entities.ty(id))
            .and_then(|previous| self.types.ctypes.composite(previous, resolved))
            .unwrap_or(resolved)
    }

    fn deduction(
        &mut self,
        declaration: &Declaration,
        declarator: &InitDeclarator,
        deduced: &mut Option<QualType>,
    ) -> Option<QualType> {
        let binding = *self.types.declarations.get(&declarator.id)?;
        let expr = self.types.deduced_initializer(
            &declaration.specifiers,
            &declarator.declarator,
            declarator.initializer.as_ref(),
            binding,
        );
        let expr = match expr {
            Ok(expr) => expr,
            Err(error) => {
                self.report(declarator, Err(error));
                return None;
            }
        };
        let value = self.types.typed(expr).ok()?.c;
        let (base, value) = match self.types.inferred_base(&declarator.declarator, value) {
            Ok(inferred) => inferred,
            Err(error) => {
                self.report(declarator, Err(error));
                return None;
            }
        };
        let canonical = self.types.ctypes.canonical(base);
        let placeholder = canonical.local_unqualified().with(
            canonical
                .quals
                .without(declaration.specifiers.qualifiers.into()),
        );
        if deduced.is_some_and(|first| first != placeholder) {
            self.reject(
                declarator,
                "'auto' deduced as different types in one declaration",
            );
        }
        *deduced = Some(placeholder);
        Some(value)
    }

    fn object_rules(
        &mut self,
        declaration: &Declaration,
        declarator: &InitDeclarator,
        global: bool,
        resolved: QualType,
    ) {
        let specifiers = &declaration.specifiers;
        let storage = specifiers.storage;
        let attributes = specifiers.attributes_with(&declarator.declarator, &declarator.attributes);
        let thread = specifiers.is_thread_local
            || attributes
                .clone()
                .any(|attribute| matches!(attribute.value, Attribute::ThreadLocal));
        let initializer = declarator.initializer.as_ref();
        if specifiers.is_constexpr && initializer.is_none() {
            self.reject(declarator, "constexpr object requires an initializer");
        }
        if self.types.ctypes.is_void(resolved) && storage != StorageClass::Extern {
            self.reject(declarator, "object cannot have type void");
            return;
        }
        if self.types.ctypes.is_function(resolved) {
            if initializer.is_some() {
                self.reject(declarator, "function initializer");
            }
            if thread {
                self.reject(declarator, "thread-local function");
            }
            if !global && storage == StorageClass::Static {
                self.reject(declarator, "block scope static function");
            }
            let attributes = specifiers
                .attributes_with(&declarator.declarator, &declarator.attributes)
                .collect::<Vec<_>>();
            self.function_rules(
                declarator,
                declarator.id,
                specifiers,
                &attributes,
                declarator.asm_label.as_ref(),
            );
            self.weakref(declarator, attributes.iter().copied(), false, false);
            for parameter in declarator
                .declarator
                .function_parameters()
                .map_or(&[][..], ParameterList::parameters)
            {
                self.parameter_rules(parameter);
            }
            return;
        }
        let linked = global || storage == StorageClass::Extern;
        let automatic = !linked && storage != StorageClass::Static;
        if automatic && thread {
            self.reject(declarator, "thread-local automatic variable");
        }
        let subject = Subject::Object { automatic };
        let result = self.types.attribute_error(attributes.clone(), subject);
        self.report(declarator, result);
        if automatic {
            for attribute in attributes.clone() {
                if let Attribute::Cleanup(function) = &attribute.value {
                    self.cleanup(function, resolved);
                }
            }
        }
        if !global && linked && initializer.is_some() {
            self.reject(declarator, "block scope extern initializer");
        }
        self.weakref(declarator, attributes.clone(), initializer.is_some(), true);
        let symbol = symbol_attributes(
            attributes.filter(|attribute| applies(attribute, subject)),
            declarator.asm_label.as_ref(),
        );
        let variable_array = matches!(
            self.types.ctypes.element(resolved),
            Some((_, Extent::Variable(_)))
        );
        if variable_array
            && initializer.is_some_and(
                |initializer| !matches!(initializer, Initializer::List(items) if items.is_empty()),
            )
        {
            self.reject(declarator, "variable length array initializer");
        }
        if variable_array && !automatic {
            self.reject(
                declarator,
                "variable length array with static storage duration",
            );
        }
        if !linked {
            self.report(declarator, symbol.map(drop));
            return;
        }
        let linkage = if storage == StorageClass::Register {
            Ok(Linkage::External)
        } else if specifiers.is_constexpr {
            Ok(Linkage::Internal)
        } else {
            declared_linkage(storage)
        };
        match (symbol, linkage) {
            (Ok(symbol), Ok(linkage)) => {
                if symbol.weakref.is_some() && !matches!(linkage, Linkage::Internal) {
                    self.reject(declarator, "weakref without internal linkage");
                }
                if symbol.selectany && !matches!(linkage, Linkage::External) {
                    self.reject(declarator, "selectany without external linkage");
                }
            }
            (symbol, linkage) => {
                self.report(declarator, symbol.map(drop));
                self.report(declarator, linkage.map(drop));
            }
        }
        if initializer.is_some()
            && let Some(&id) = self.types.declarations.get(&declarator.id)
            && !self.initialized.insert(id)
        {
            self.reject(declarator, "multiple global initializers");
        }
    }

    fn cleanup(&mut self, function: &Expr, object: QualType) {
        if !self.types.function_references.contains(&function.id) {
            return self.reject(function, "'cleanup' argument is not a function");
        }
        let Ok(ty) = self.types.expression_type(function) else {
            return;
        };
        let clang = self.types.flavor().is_clang();
        let parameter = match self.types.ctypes.function_parts(ty) {
            Some((_, &[parameter], _, true)) => parameter,
            Some((_, _, _, false)) if !clang => return,
            _ => return self.reject(function, "'cleanup' function must take 1 parameter"),
        };
        let parameter = self.types.ctypes.unqualified(parameter);
        let address = self.types.ctypes.pointer(object);
        let conversion = self.types.ctypes.classify_conversion(
            address,
            parameter,
            ConversionContext::Arg,
            false,
        );
        match conversion {
            Err(reason) => self.errors.push(error(
                function.provenance,
                function.expansion,
                reason.to_string(),
            )),
            Ok(conversion) => match conversion.warning {
                Some((Warning::IncompatiblePointerTypesDiscardsQualifiers, _)) if clang => {}
                Some(_) if clang => self.reject(
                    function,
                    "'cleanup' function parameter type is incompatible with the variable's address",
                ),
                Some((warning, message)) => self.types.warn(warning, message, function),
                None => {}
            },
        }
    }

    fn deallocator(&mut self, function: &Expr, argument: Option<&Expr>) {
        let ty = if self.types.function_references.contains(&function.id) {
            self.types.expression_type(function)
        } else if let Some((_, signature)) = builtin_deallocator(self.types, function) {
            Ok(signature)
        } else {
            return self.reject(function, "'malloc' argument is not a function");
        };
        let result = ty.and_then(|ty| deallocator_argument(self.types, ty, argument));
        if self.types.flavor().is_clang() {
            self.report(function, result.map(drop));
        }
    }

    fn weakref<'a, T>(
        &mut self,
        at: &Span<T>,
        attributes: impl Iterator<Item = &'a Span<Attribute>> + Clone,
        defined: bool,
        object: bool,
    ) {
        if !attributes
            .clone()
            .any(|attribute| matches!(attribute.value, Attribute::WeakRef(_)))
        {
            return;
        }
        let target = attributes.clone().any(|attribute| {
            matches!(
                attribute.value,
                Attribute::WeakRef(Some(_)) | Attribute::Alias(_)
            )
        });
        let clang = self.types.flavor().is_clang();
        if clang && !target {
            self.reject(at, "weakref declaration must also have an alias attribute");
        } else if defined && (clang || (target && object)) {
            self.reject(at, "weakref declaration cannot be a definition");
        }
    }

    fn function_rules<T>(
        &mut self,
        at: &Span<T>,
        node: NodeId,
        specifiers: &DeclarationSpecifiers,
        attributes: &[&Span<Attribute>],
        asm_label: Option<&Span<AsmLabel>>,
    ) {
        let result = self
            .types
            .attribute_error(attributes.iter().copied(), Subject::Function);
        self.report(at, result);
        self.report(
            at,
            function_symbol(attributes.iter().copied(), asm_label).map(drop),
        );
        for attribute in attributes {
            if let Attribute::Malloc {
                deallocator: Some(function),
                argument,
            } = &attribute.value
            {
                self.deallocator(function, argument.as_ref());
            }
        }
        self.report(at, declared_linkage(specifiers.storage).map(drop));
        let Some(&id) = self.types.declarations.get(&node) else {
            return;
        };
        for attribute in attributes {
            let always = match attribute.value {
                Attribute::AlwaysInline => true,
                Attribute::NoInline => false,
                _ => continue,
            };
            if self
                .inlining
                .insert(id, always)
                .is_some_and(|previous| previous != always)
            {
                self.reject(at, "conflicting always_inline and noinline attributes");
            }
        }
    }

    fn parameter_rules(&mut self, parameter: &ParameterDeclaration) {
        let attributes = parameter
            .specifiers
            .attributes_with(&parameter.declarator, &parameter.attributes);
        let result = self.types.attribute_error(attributes, Subject::Parameter);
        self.report(parameter, result);
    }

    fn variable_array(&self, c: QualType) -> bool {
        matches!(self.types.ctypes.element(c), Some((_, Extent::Variable(_))))
    }

    fn element<T>(&mut self, at: &Span<T>, result: Result<(), ElementError>) {
        let Err(ElementError { at: element, error }) = result else {
            return;
        };
        match element {
            Some(element) => self.report(&element, Err(error)),
            None => self.report(at, Err(error)),
        }
    }

    fn resolution<T>(&mut self, at: &Span<T>, result: &Result<QualType, ResolveError>) {
        if let Err(ResolveError::Rejected(reason)) = result
            && self.rejected.insert(at.id)
        {
            self.reject(at, reason);
        }
    }

    fn report<T>(&mut self, at: &Span<T>, result: Result<(), ResolveError>) {
        if let Err(ResolveError::Rejected(reason)) = result {
            self.reject(at, reason);
        }
    }

    fn tag(&mut self, ty: &TypeSpecifier) {
        let TypeSpecifier::Tag(TagSpecifier::Definition(id)) = ty else {
            return;
        };
        let Some(tag) = self.unit.tag(*id) else {
            return;
        };
        if let TagBody::Enum {
            enumerators,
            fixed_type,
        } = &tag.body
        {
            let int_ty = self.types.ctypes.int();
            let fixed = fixed_type
                .as_ref()
                .and_then(|ty| self.types.resolve(&ty.specifiers, &ty.declarator).ok());
            let mut previous: Option<BigInt> = Some((-1).into());
            for item in enumerators {
                let EnumItemKind::Enumerator(enumerator) = &item.value else {
                    continue;
                };
                let value = match &enumerator.value {
                    Some(expr) if ice_shape(self.types, expr).is_constant() => {
                        self.types.constant_integer(expr).ok()
                    }
                    Some(_) => None,
                    None => previous.as_ref().map(|value| value + 1),
                };
                previous = value.clone();
                if let Some(value) = value {
                    let c = fixed.unwrap_or(int_ty);
                    let ty = self.types.ir_type(c);
                    let Type::Numeric(NumericType::Integer { width, signed, .. }) = ty else {
                        continue;
                    };
                    let limit = BigInt::from(1u8) << (width - u32::from(signed));
                    let min = if signed { -&limit } else { BigInt::from(0u8) };
                    if value < min || value >= limit {
                        continue;
                    }
                    self.types.declare_constant(
                        item.id,
                        super::operand::Operand {
                            c,
                            value: Value {
                                ty,
                                node: item
                                    .clone()
                                    .derive(ValueKind::Constant(Number::SignedInteger(value))),
                            },
                        },
                    );
                }
            }
        }
        // Failed dependencies remain unavailable; only a consuming assertion diagnoses them.
        let specifiers = DeclarationSpecifiers {
            ty: ty.clone(),
            qualifiers: Qualifiers::default(),
            storage: StorageClass::None,
            is_thread_local: false,
            is_inline: false,
            is_noreturn: false,
            is_constexpr: false,
            attributes: Vec::new(),
        };
        let _ = self.types.resolve(&specifiers, &Declarator::Abstract);
    }

    fn compound(&mut self, body: &[Stmt]) {
        let region = self.region;
        self.compound_start = true;
        for stmt in body {
            self.statement(stmt);
        }
        self.region = region;
    }

    fn pragma<T>(&mut self, at: &Span<T>, pragma: &PragmaKind, placement: PragmaPlacement) {
        let result = self.pragmas.apply(&mut self.region, pragma, placement);
        self.report(at, result);
    }

    fn statement(&mut self, stmt: &Stmt) {
        match &stmt.value {
            StmtKind::Pragma(pragma) => {
                let placement = if self.compound_start {
                    PragmaPlacement::CompoundStart
                } else {
                    PragmaPlacement::Misplaced
                };
                return self.pragma(stmt, &pragma.kind, placement);
            }
            StmtKind::Comment(_) => return,
            _ => self.compound_start = false,
        }
        let gcc = self.types.flavor().is_gcc();
        let labels = match &stmt.value {
            StmtKind::Labeled { .. } | StmtKind::Attributed { .. } => Vec::new(),
            StmtKind::SwitchLabel { .. } if gcc => Vec::new(),
            _ => std::mem::take(&mut self.context.labels),
        };
        match &stmt.value {
            StmtKind::StaticAssert(assertion) => self.assertion(assertion),
            StmtKind::Decl(declaration) => self.declaration(&stmt.derive(()), declaration, false),
            StmtKind::Block(body) => self.compound(body),
            StmtKind::NestedFunction(function) => {
                if !self.types.flavor().is_gcc() {
                    self.reject(stmt, "function definition is not allowed here");
                }
                self.function(stmt.id, stmt.derive(()), None, function)
            }
            StmtKind::If {
                condition,
                then_branch,
                else_branch,
            } => {
                self.condition(condition);
                self.statement(then_branch);
                if let Some(branch) = else_branch {
                    self.statement(branch);
                }
            }
            StmtKind::DoWhile { condition, body } => {
                self.loop_body(stmt, labels, body);
                self.condition(condition);
            }
            StmtKind::While { condition, body } => {
                self.condition(condition);
                self.loop_body(stmt, labels, body);
            }
            StmtKind::Switch { discriminant, body } => {
                self.expression(discriminant);
                let mut promoted = None;
                if let Ok(ty) = self.types.operand_type(discriminant) {
                    if self.types.ctypes.is_integer(ty) {
                        promoted = self.types.record_switch(stmt, discriminant).ok();
                    } else {
                        self.reject(stmt, "noninteger switch discriminant");
                    }
                }
                self.context.breakables += 1;
                self.context.switches.push(promoted);
                self.named_target(stmt, labels, false, body);
                self.context.switches.pop();
                self.context.breakables -= 1;
            }
            StmtKind::For {
                init,
                condition,
                increment,
                body,
            } => {
                if let Some(init) = init {
                    self.statement(init);
                }
                if let Some(condition) = condition {
                    self.condition(condition);
                }
                if let Some(increment) = increment {
                    self.expression(increment);
                }
                self.loop_body(stmt, labels, body);
            }
            StmtKind::SwitchLabel { label, body } => {
                if self.context.switches.is_empty() {
                    self.reject(stmt, "case or default outside switch");
                }
                match label {
                    SwitchLabel::Case(value) => self.case_value(stmt, Slot::CaseStart, value),
                    SwitchLabel::CaseRange { start, end } => {
                        self.case_value(stmt, Slot::CaseStart, start);
                        self.case_value(stmt, Slot::CaseEnd, end);
                    }
                    SwitchLabel::Default => {}
                }
                self.statement(body);
            }
            StmtKind::Break if self.context.breakables == 0 => {
                self.reject(stmt, "break outside loop or switch")
            }
            StmtKind::Continue if self.context.loops == 0 => {
                self.reject(stmt, "continue outside loop")
            }
            StmtKind::Attribute(attributes)
                if self.context.switches.is_empty()
                    && is_fallthrough(attributes)
                    && !self.types.flavor().is_gcc() =>
            {
                self.reject(stmt, "fallthrough outside switch")
            }
            StmtKind::Asm(asm) => {
                if let Some(operands) = &asm.operands {
                    for (operand, output) in operands
                        .outputs
                        .iter()
                        .map(|operand| (operand, true))
                        .chain(operands.inputs.iter().map(|operand| (operand, false)))
                    {
                        self.expression(&operand.expr);
                        let result = self.types.asm_operand_rule(operand, output);
                        self.report(&operand.expr, result);
                    }
                }
            }
            StmtKind::Attributed { attributes, body } => {
                if !self.types.flavor().is_gcc() && is_fallthrough(attributes) {
                    self.reject(stmt, "fallthrough attribute on a non-empty statement");
                }
                self.statement(body)
            }
            StmtKind::Labeled { label, body } => {
                if !gcc {
                    self.context.labels.clear();
                }
                self.context.labels.push(label.value.clone());
                self.statement(body)
            }
            StmtKind::NamedBreak(label) => self.named_jump(stmt, label, false),
            StmtKind::NamedContinue(label) => self.named_jump(stmt, label, true),
            StmtKind::Return(expr) => {
                self.expression(expr);
                match self.context.returns {
                    Returns::Value(to) => self.convert(expr, to, ConversionContext::Return),
                    Returns::Void => {
                        if let Ok(ty) = self.types.operand_type(expr)
                            && !self.types.ctypes.is_void(ty)
                        {
                            self.reject(stmt, "value return from void function");
                        }
                    }
                    Returns::Unknown => {}
                }
            }
            StmtKind::ReturnVoid => {
                let accepted = match self.types.flavor() {
                    CompilerFlavor::Msvc => true,
                    CompilerFlavor::Gcc => self.types.features().valueless_return_in_nonvoid,
                    _ => false,
                };
                if matches!(self.context.returns, Returns::Value(_)) && !accepted {
                    self.reject(stmt, "non-void function should return a value");
                }
            }
            StmtKind::ComputedGoto(expr) => {
                self.expression(expr);
                if let Ok(ty) = self.types.operand_type(expr)
                    && !self.types.ctypes.is_pointer(ty)
                {
                    self.reject(stmt, "nonpointer computed goto");
                }
            }
            StmtKind::Expr(expr) => self.expression(expr),
            _ => {}
        }
    }

    fn loop_body(&mut self, stmt: &Stmt, labels: Vec<String>, body: &Stmt) {
        self.context.loops += 1;
        self.context.breakables += 1;
        self.named_target(stmt, labels, true, body);
        self.context.breakables -= 1;
        self.context.loops -= 1;
    }

    fn named_target(&mut self, stmt: &Stmt, labels: Vec<String>, is_loop: bool, body: &Stmt) {
        if labels.is_empty() {
            return self.statement(body);
        }
        self.context.named_targets.push(NamedTarget {
            labels,
            statement: stmt.id,
            is_loop,
        });
        self.statement(body);
        self.context.named_targets.pop();
    }

    fn named_jump(&mut self, stmt: &Stmt, label: &Span<String>, is_continue: bool) {
        let target = self
            .context
            .named_targets
            .iter()
            .rev()
            .find(|target| target.labels.contains(&label.value));
        let message = match target {
            Some(target) if !is_continue || target.is_loop => {
                self.types.named_jumps.insert(stmt.id, target.statement);
                return;
            }
            Some(_) => format!(
                "`continue` label `{}` names a switch, not a loop",
                label.value
            ),
            None if is_continue => {
                format!(
                    "`continue` label `{}` does not name an enclosing loop",
                    label.value
                )
            }
            None => format!(
                "`break` label `{}` does not name an enclosing loop or switch",
                label.value
            ),
        };
        self.errors
            .push(error(label.provenance, label.expansion, message));
    }

    fn condition(&mut self, condition: &Expr) {
        self.expression(condition);
        if let Ok(ty) = self.types.operand_type(condition)
            && !self.types.ctypes.is_scalar(ty)
        {
            self.reject(condition, "non-scalar condition");
        }
    }

    fn case_value(&mut self, label: &Stmt, slot: Slot, value: &Expr) {
        self.expression(value);
        if self
            .types
            .constant_integer(value)
            .is_err_and(|error| error.is_rejection())
        {
            self.reject(label, "nonconstant case expression");
        }
        if let Some(Some(switch)) = self.context.switches.last().copied() {
            let recorded = self.types.record_case(label, slot, value, switch);
            self.report(label, recorded);
        }
    }

    fn reject_with<T>(&mut self, at: &Span<T>, rejection: ResolveError) {
        if rejection.is_rejection() {
            self.errors
                .push(error(at.provenance, at.expansion, rejection.to_string()));
        }
    }

    fn reject<T>(&mut self, at: &Span<T>, reason: &'static str) {
        self.errors.push(error(
            at.provenance,
            at.expansion,
            ResolveError::Rejected(reason).to_string(),
        ));
    }

    fn initializer(&mut self, initializer: &Initializer) {
        match initializer {
            Initializer::Expr(expr) => self.expression(expr),
            Initializer::List(items) => {
                for item in items {
                    self.initializer(&item.value);
                }
            }
        }
    }

    fn comparison_warning(&mut self, expr: &Expr, left: &Expr, right: &Expr) {
        let (Ok(lc), Ok(rc)) = (
            self.types.operand_type(left),
            self.types.operand_type(right),
        ) else {
            return;
        };
        if self.types.ctypes.is_nullptr(lc) || self.types.ctypes.is_nullptr(rc) {
            return;
        }
        match (
            self.types.ctypes.is_pointer(lc),
            self.types.ctypes.is_pointer(rc),
        ) {
            (true, true) => {
                if self.types.ctypes.is_void(lc) || self.types.ctypes.is_void(rc) {
                    return;
                }
                if self
                    .types
                    .ctypes
                    .merge_pointer(lc, rc, super::PointerMerge::EXACT)
                    .is_none()
                {
                    self.types.warn(
                        Warning::CompareDistinctPointerTypes,
                        "comparison of distinct pointer types",
                        expr,
                    );
                }
            }
            (true, false) | (false, true) => {
                let (other, oc) = if self.types.ctypes.is_pointer(lc) {
                    (right, rc)
                } else {
                    (left, lc)
                };
                if !(self.types.ctypes.is_integer(oc) && self.types.integer_constant_zero(other)) {
                    self.types.warn(
                        Warning::PointerIntegerCompare,
                        "comparison between pointer and integer",
                        expr,
                    );
                }
            }
            (false, false) => {}
        }
    }

    fn expression(&mut self, expr: &Expr) {
        self.subexpressions(expr);
        self.types.rejected_at = None;
        if let Err(rejection) = self.types.typed(expr)
            && rejection.is_rejection()
            && let Some(at) = self.types.rejected_at.take()
            && self.rejected.insert(at.id)
        {
            self.errors
                .push(error(at.provenance, at.expansion, rejection.to_string()));
        }
        if constexpr_literal_root(expr)
            && let Ok(c) = self.types.expression_type(expr)
            && self.types.ctypes.element(c).is_none()
            && let Ok(value) = self.types.constant_value(expr)
        {
            self.types.constexpr_values.insert(expr.id, value);
        }
    }

    fn subexpressions(&mut self, expr: &Expr) {
        match &expr.value {
            ExprKind::StatementExpression(body) => self.compound(body),
            ExprKind::Cast { ty, value } => {
                self.type_name(ty);
                self.expression(value);
                self.cast(expr, ty, value);
            }
            ExprKind::BitCast { ty, value } | ExprKind::ConvertVector { ty, value } => {
                self.type_name(ty);
                self.expression(value);
            }
            ExprKind::VaArg { list, ty } => {
                self.expression(list);
                self.type_name(ty);
            }
            ExprKind::SizeOfType { ty }
            | ExprKind::AlignOf { ty }
            | ExprKind::CountOfType { ty }
            | ExprKind::MaxOf { ty }
            | ExprKind::MinOf { ty } => self.type_name(ty),
            ExprKind::OffsetOf { ty, member } => {
                self.type_name(ty);
                self.expression(member);
            }
            ExprKind::TypesCompatible { left_ty, right_ty } => {
                self.type_name(left_ty);
                self.type_name(right_ty);
            }
            ExprKind::Paren(expr)
            | ExprKind::Unary { operand: expr, .. }
            | ExprKind::Postfix { operand: expr, .. }
            | ExprKind::SizeOfExpr(expr)
            | ExprKind::AlignOfExpr(expr)
            | ExprKind::CountOfExpr(expr)
            | ExprKind::Member { base: expr, .. } => self.expression(expr),
            ExprKind::Assign { op, target, value } => {
                let target_expr = target;
                self.expression(target);
                self.expression(value);
                if *op == AssignOp::Assign
                    && let Ok(target) = self.types.typed(target)
                    && target.lvalue
                    && self.types.require_modifiable_lvalue(target.c).is_ok()
                {
                    self.convert(value, target.c, ConversionContext::Assign);
                } else if *op != AssignOp::Assign
                    && let Ok(target) = self.types.typed(target)
                    && target.lvalue
                    && let Ok(from) = self.types.operand_type(value)
                {
                    let updated = self.types.ctypes.unqualified(target.c);
                    if let Ok(conversion) =
                        self.types
                            .compound_conversion(*op, target_expr, from, updated)
                        && let Some((warning, message)) = conversion.warning
                    {
                        self.types.warn(warning, message, expr);
                    }
                }
            }
            ExprKind::Binary {
                op:
                    BinaryOp::Equal
                    | BinaryOp::NotEqual
                    | BinaryOp::Less
                    | BinaryOp::LessEqual
                    | BinaryOp::Greater
                    | BinaryOp::GreaterEqual,
                left,
                right,
            } => {
                self.expression(left);
                self.expression(right);
                self.comparison_warning(expr, left, right);
            }
            ExprKind::Binary { left, right, .. }
            | ExprKind::Comma { left, right }
            | ExprKind::Index {
                base: left,
                index: right,
            } => {
                self.expression(left);
                self.expression(right);
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                self.expression(condition);
                if let Some(expr) = then_value {
                    self.expression(expr);
                }
                self.expression(else_value);
                let then_value: &Expr = match then_value {
                    Some(then_value) => then_value,
                    None => condition,
                };
                if let (Ok(left), Ok(right)) = (
                    self.types.operand_type(then_value),
                    self.types.operand_type(else_value),
                ) && self.types.ctypes.is_pointer(left)
                    && self.types.ctypes.is_pointer(right)
                    && let Ok((_, Some(warning))) = self
                        .types
                        .conditional_pointers(then_value, left, else_value, right)
                {
                    self.types.warn(
                        warning,
                        "pointer type mismatch in conditional expression",
                        expr,
                    );
                }
            }
            ExprKind::Call { callee, arguments } => {
                self.expression(callee);
                for argument in arguments {
                    self.expression(argument);
                }
                self.arguments(callee, arguments);
            }
            ExprKind::CompoundLiteral { ty, initializer } => {
                self.type_name(ty);
                let global = !self.in_function;
                let specifiers = &ty.specifiers;
                let storage = if specifiers.is_thread_local {
                    crate::ir::StorageDuration::Thread
                } else if global || specifiers.storage == StorageClass::Static {
                    crate::ir::StorageDuration::Static
                } else {
                    crate::ir::StorageDuration::Automatic
                };
                self.types.compound_storage.insert(expr.id, storage);
                if global && specifiers.storage == StorageClass::Register {
                    self.reject(expr, "register compound literal at file scope");
                }
                if specifiers.is_thread_local
                    && ((!global && specifiers.storage != StorageClass::Static)
                        || specifiers.storage == StorageClass::Register
                        || specifiers.is_constexpr)
                {
                    self.reject(
                        expr,
                        "invalid storage-class combination for thread-local compound literal",
                    );
                }
                for item in initializer {
                    self.initializer(&item.value);
                }
                if let Ok(literal) = self.types.typed(expr) {
                    if specifiers.is_constexpr && !self.constexpr_literal_type(literal.c) {
                        self.reject(expr, "constexpr compound literal has volatile, atomic, or restrict-qualified type");
                    }
                    if self.variable_array(literal.c) {
                        self.reject(expr, "compound literal has variable length array type");
                    } else {
                        let result = self.types.record_initializer(
                            expr.id,
                            literal.c,
                            InitializerSource::CompoundLiteral(initializer),
                        );
                        self.element(expr, result);
                        if specifiers.is_constexpr
                            || storage != crate::ir::StorageDuration::Automatic
                        {
                            for item in initializer {
                                self.constant_literal_initializer(
                                    &item.value,
                                    specifiers.is_constexpr,
                                );
                            }
                        }
                    }
                }
            }
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                match controlling {
                    GenericControl::Expr(expr) => self.expression(expr),
                    GenericControl::Type { ty } => self.type_name(ty),
                }
                for association in associations {
                    match association {
                        GenericAssociation::Type { ty, value } => {
                            self.type_name(ty);
                            self.expression(value);
                        }
                        GenericAssociation::Default(value) => self.expression(value),
                    }
                }
            }
            ExprKind::StaticAssert(assertion) => self.assertion(assertion),
            _ => {}
        }
    }

    fn convert(&mut self, expr: &Expr, to: QualType, context: ConversionContext) {
        if let Ok(from) = self.types.operand_type(expr) {
            self.convert_at(expr, expr, from, to, context);
        }
    }

    fn convert_at(
        &mut self,
        at: &Expr,
        expr: &Expr,
        from: QualType,
        to: QualType,
        context: ConversionContext,
    ) {
        if let Err(reason) = self.types.record_conversion(expr, from, to, context) {
            self.errors
                .push(error(at.provenance, at.expansion, reason.to_string()));
        }
    }

    fn cast(&mut self, cast: &Expr, ty: &TypeName, value: &Expr) {
        let provisional = std::mem::replace(&mut self.types.provisional_extents, true);
        let to = self.types.resolve_type_name(ty);
        self.types.provisional_extents = provisional;
        let (Ok(to), Ok(from)) = (to, self.types.operand_type(value)) else {
            return;
        };
        let to = self.types.ctypes.unqualified(to);
        match self.types.union_cast_member(to, from) {
            Ok(Some(member)) => {
                self.types.choices.insert(cast.id, Choice::Member(member));
                return;
            }
            Ok(None) if !self.types.ctypes.is_void(to) => {}
            _ => return,
        }
        self.convert_at(cast, value, from, to, ConversionContext::Cast);
    }

    fn arguments(&mut self, callee: &Expr, arguments: &[Expr]) {
        if let Some(builtin) = super::atomic::atomic_builtin(callee) {
            for (object, value) in builtin.operands(arguments).values {
                if let Ok(pointer) = self.types.operand_type(object)
                    && let Some(pointee) = self.types.ctypes.pointee(pointer)
                {
                    self.convert(value, pointee, ConversionContext::Arg);
                }
            }
            return;
        }
        let Ok(Some(signature)) = self.types.argument_signature(callee, arguments) else {
            return;
        };
        let Some((_, parameters, ..)) = self.types.ctypes.function_parts(signature) else {
            return;
        };
        let parameters = parameters.to_vec();
        for (argument, &parameter) in arguments.iter().zip(&parameters) {
            let Ok(from) = self.types.operand_type(argument) else {
                continue;
            };
            let mut to = self.types.ctypes.adjust_parameter(parameter);
            if let Some((index, member)) = self.types.transparent_member(to, argument, from) {
                self.types
                    .transparent_arguments
                    .insert(argument.id, (index, member));
                to = member;
            }
            self.convert_at(argument, argument, from, to, ConversionContext::Arg);
        }
    }

    fn declarator(&mut self, declarator: &Declarator) {
        match declarator {
            Declarator::Grouped(inner)
            | Declarator::Attributed { inner, .. }
            | Declarator::Pointer { inner, .. } => self.declarator(inner),
            Declarator::Array { inner, size, .. } => {
                self.declarator(inner);
                if let ArraySize::Expression(size) = size {
                    self.expression(size);
                    if let Err(rejection) = self.types.record_extent(size) {
                        self.reject_with(size, rejection);
                    }
                }
            }
            Declarator::Function { inner, parameters } => {
                self.declarator(inner);
                let enclosing_scope = std::mem::replace(&mut self.in_function, true);
                for parameter in parameters.parameters() {
                    self.specifier(&parameter.specifiers.ty);
                    self.declarator(&parameter.declarator);
                }
                self.in_function = enclosing_scope;
            }
            Declarator::Abstract | Declarator::Name(_) => {}
        }
    }

    fn specifier(&mut self, ty: &TypeSpecifier) {
        match ty {
            TypeSpecifier::TypeOf(TypeOfOperand::Expression(operand))
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(operand)) => {
                self.expression(operand);
                let result = self.types.typeof_expression(operand).map(drop);
                self.report(operand, result);
            }
            TypeSpecifier::TypeOf(TypeOfOperand::Type(operand))
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Type(operand)) => self.type_name(operand),
            _ => {}
        }
    }

    fn type_name(&mut self, ty: &TypeName) {
        self.specifier(&ty.specifiers.ty);
        self.declarator(&ty.declarator);
        self.tag(&ty.specifiers.ty);
        let _ = self.types.resolve_type_name(ty);
        if !self.types.flavor().is_gcc() {
            for attribute in &ty.specifiers.attributes {
                if matches!(attribute.value, Attribute::Aligned(_)) {
                    self.types.warn(
                        Warning::IgnoredAttributes,
                        "'aligned' attribute ignored when parsing type",
                        attribute,
                    );
                }
            }
        }
    }
}

struct InvalidConstantArithmetic<'a> {
    types: &'a mut TypeResolver,
    flavor: crate::compiler_args::CompilerFlavor,
}

impl Visitor for InvalidConstantArithmetic<'_> {
    type Error = (Expr, &'static str);

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), Self::Error> {
        match &expr.value {
            ExprKind::Binary {
                op: BinaryOp::Div | BinaryOp::Rem,
                right,
                ..
            } if self.flavor != crate::compiler_args::CompilerFlavor::Msvc
                && self
                    .types
                    .constant_integer(right)
                    .is_ok_and(|value| value == 0.into()) =>
            {
                Err((expr.clone(), "division by zero"))
            }
            ExprKind::Binary {
                op: BinaryOp::ShiftLeft,
                left,
                right,
                ..
            } if self.flavor == crate::compiler_args::CompilerFlavor::Gcc
                && self
                    .types
                    .constant_integer(right)
                    .is_ok_and(|value| value.sign() == Sign::Minus)
                && self
                    .types
                    .constant_integer(left)
                    .is_ok_and(|value| value.sign() != Sign::NoSign) =>
            {
                Err((expr.clone(), "negative shift count"))
            }
            ExprKind::Binary {
                op: BinaryOp::And | BinaryOp::Or,
                left,
                right,
            } => {
                self.visit_expr(left)?;
                let truth = self
                    .types
                    .constant_integer(left)
                    .ok()
                    .map(|value| value.sign() != Sign::NoSign);
                if truth
                    != Some(matches!(
                        expr.value,
                        ExprKind::Binary {
                            op: BinaryOp::Or,
                            ..
                        }
                    ))
                {
                    self.visit_expr(right)?;
                }
                Ok(())
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                self.visit_expr(condition)?;
                match self
                    .types
                    .constant_integer(condition)
                    .ok()
                    .map(|value| value.sign() != Sign::NoSign)
                {
                    Some(true) => {
                        if let Some(value) = then_value {
                            self.visit_expr(value)?;
                        }
                    }
                    Some(false) => self.visit_expr(else_value)?,
                    None => {
                        if let Some(value) = then_value {
                            self.visit_expr(value)?;
                        }
                        self.visit_expr(else_value)?;
                    }
                }
                Ok(())
            }
            ExprKind::SizeOfExpr(_) | ExprKind::AlignOfExpr(_) | ExprKind::CountOfExpr(_) => Ok(()),
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                if let Ok(selected) = self.types.generic_selection(controlling, associations) {
                    self.visit_expr(selected)?;
                }
                Ok(())
            }
            _ => visit::walk_expr(self, expr),
        }
    }
}

// Fold evaluates executed IR only, so enforce the supported ICE syntax separately.
#[derive(Clone, Copy)]
enum Shape<'e> {
    Constant,
    Skip,
    NotConstant(&'e Expr),
}

impl<'e> Shape<'e> {
    fn and(self, other: Self) -> Self {
        match (self, other) {
            (Shape::NotConstant(expr), _) | (_, Shape::NotConstant(expr)) => {
                Shape::NotConstant(expr)
            }
            (Shape::Constant, Shape::Constant) => Shape::Constant,
            _ => Shape::Skip,
        }
    }

    fn opaque(self) -> Self {
        self.and(Shape::Skip)
    }

    fn unevaluated(self) -> Self {
        match self {
            Shape::NotConstant(_) => Shape::Skip,
            shape => shape,
        }
    }

    fn is_constant(self) -> bool {
        matches!(self, Shape::Constant)
    }
}

fn evaluated_shape<'e>(types: &mut TypeResolver, expr: &'e Expr, evaluated: bool) -> Shape<'e> {
    let shape = ice_shape(types, expr);
    if evaluated {
        shape
    } else {
        shape.unevaluated()
    }
}

fn constant_truth(types: &mut TypeResolver, expr: &Expr, shape: Shape) -> Option<bool> {
    if !shape.is_constant() {
        return None;
    }
    types
        .constant_integer(expr)
        .ok()
        .map(|value| value.sign() != Sign::NoSign)
}

fn call_shape<'e>(expr: &'e Expr, callee: &Expr, arguments: &'e [Expr]) -> Shape<'e> {
    if super::expression::constant_p_operand(callee, arguments).is_some()
        || super::builtins::BitBuiltin::of_call(callee, arguments).is_some()
    {
        return Shape::Constant;
    }
    if super::expression::choose_expr_operands(callee, arguments).is_some() {
        return Shape::Skip;
    }
    let mut callee = callee;
    while let ExprKind::Paren(inner) = &callee.value {
        callee = inner;
    }
    match &callee.value {
        ExprKind::Identifier(name) if super::builtins::is_foldable_builtin(name) => Shape::Skip,
        _ => Shape::NotConstant(expr),
    }
}

pub(super) fn static_assertion_error(
    types: &mut TypeResolver,
    assertion: &StaticAssert,
) -> Option<SemaError> {
    let condition = &assertion.condition;
    match ice_shape(types, condition) {
        Shape::Constant => {}
        Shape::Skip => return None,
        Shape::NotConstant(call) => {
            return Some(error(
                call.provenance,
                call.expansion,
                "static assertion requires an integer constant expression: call to a function that cannot be constant folded",
            ));
        }
    }
    let target = types.fold_target();
    let result = types
        .constant_value(condition)
        .and_then(|value| match value.ty {
            Type::Bool | Type::Numeric(NumericType::Integer { .. }) => {
                super::fold::integer_constant(&value, target).ok_or(ResolveError::Rejected(
                    "nonconstant or undefined integer expression",
                ))
            }
            _ => Err(ResolveError::Rejected("non-integer constant expression")),
        })
        .map_err(|error| error.to_string());
    let message = match result {
        Ok(value) if value.sign() != Sign::NoSign => return None,
        Ok(_) => assertion.message.as_ref().map_or_else(
            || "static assertion failed".to_owned(),
            |message| format!("static assertion failed: {message}"),
        ),
        Err(reason) => {
            format!("static assertion requires an integer constant expression: {reason}")
        }
    };
    Some(error(condition.provenance, condition.expansion, message))
}

fn constexpr_literal_root(expr: &Expr) -> bool {
    match &expr.value {
        ExprKind::Paren(inner) => constexpr_literal_root(inner),
        ExprKind::Member {
            base, arrow: false, ..
        } => constexpr_literal_root(base),
        ExprKind::CompoundLiteral { ty, .. } => ty.specifiers.is_constexpr,
        _ => false,
    }
}

fn literal_initializer_constant(types: &mut TypeResolver, expr: &Expr, constexpr: bool) -> bool {
    if types.constant_value(expr).is_ok() {
        return true;
    }
    match &expr.value {
        ExprKind::Paren(inner) | ExprKind::Cast { value: inner, .. } => {
            literal_initializer_constant(types, inner, constexpr)
        }
        ExprKind::StringLiteral(_) => true,
        ExprKind::Call { callee, arguments } => {
            !matches!(call_shape(expr, callee, arguments), Shape::NotConstant(_))
        }
        ExprKind::CompoundLiteral { initializer, .. } if !constexpr => initializer
            .iter()
            .all(|item| literal_initializer_list(types, &item.value, false)),
        ExprKind::Unary {
            op: UnaryOp::AddrOf,
            operand,
        } if !constexpr => constant_literal_address(types, operand),
        ExprKind::Identifier(_) if !constexpr => {
            types
                .expression_type(expr)
                .is_ok_and(|c| types.ctypes.is_function(c) || types.ctypes.element(c).is_some())
                && constant_literal_address(types, expr)
        }
        ExprKind::Unary {
            op: UnaryOp::Plus | UnaryOp::Minus | UnaryOp::Not | UnaryOp::BitNot,
            operand,
        } => literal_initializer_constant(types, operand, constexpr),
        ExprKind::Binary { left, right, .. } => {
            literal_initializer_constant(types, left, constexpr)
                && literal_initializer_constant(types, right, constexpr)
        }
        ExprKind::Conditional {
            condition,
            then_value,
            else_value,
        } => match types.constant_integer(condition) {
            Ok(value) if value.sign() == Sign::NoSign => {
                literal_initializer_constant(types, else_value, constexpr)
            }
            Ok(_) => then_value
                .as_ref()
                .is_none_or(|value| literal_initializer_constant(types, value, constexpr)),
            Err(_) => false,
        },
        _ => false,
    }
}

fn literal_initializer_list(
    types: &mut TypeResolver,
    initializer: &Initializer,
    constexpr: bool,
) -> bool {
    match initializer {
        Initializer::Expr(expr) => literal_initializer_constant(types, expr, constexpr),
        Initializer::List(items) => items
            .iter()
            .all(|item| literal_initializer_list(types, &item.value, constexpr)),
    }
}

fn constant_literal_address(types: &mut TypeResolver, expr: &Expr) -> bool {
    match &expr.value {
        ExprKind::Paren(inner) => constant_literal_address(types, inner),
        ExprKind::Identifier(_) => types.references.get(&expr.id).is_some_and(|id| {
            types.entities.storage(*id) == Some(crate::ir::StorageDuration::Static)
                || types.entities.linkage(*id).is_some()
        }),
        ExprKind::StringLiteral(_) => true,
        ExprKind::Member {
            base, arrow: false, ..
        } => constant_literal_address(types, base),
        ExprKind::Index { base, index } => {
            constant_literal_address(types, base) && types.constant_integer(index).is_ok()
        }
        ExprKind::CompoundLiteral { .. } => {
            types.compound_storage.get(&expr.id) == Some(&crate::ir::StorageDuration::Static)
        }
        _ => false,
    }
}

fn ice_shape<'e>(types: &mut TypeResolver, expr: &'e Expr) -> Shape<'e> {
    match &expr.value {
        ExprKind::CompoundLiteral { ty, .. } if ty.specifiers.is_constexpr => Shape::Constant,
        ExprKind::Member { .. } | ExprKind::Index { .. } if types.constant_value(expr).is_ok() => {
            Shape::Constant
        }
        ExprKind::IntegerLiteral(_)
        | ExprKind::FloatLiteral(_)
        | ExprKind::CharLiteral(_)
        | ExprKind::BoolLiteral(_)
        | ExprKind::Identifier(_)
        | ExprKind::SizeOfType { .. }
        | ExprKind::AlignOf { .. }
        | ExprKind::SizeOfExpr(_)
        | ExprKind::AlignOfExpr(_)
        | ExprKind::MaxOf { .. }
        | ExprKind::MinOf { .. }
        | ExprKind::CountOfType { .. }
        | ExprKind::CountOfExpr(_)
        | ExprKind::OffsetOf { .. }
        | ExprKind::TypesCompatible { .. } => Shape::Constant,
        ExprKind::Call { callee, arguments } => call_shape(expr, callee, arguments),
        ExprKind::Paren(expr) | ExprKind::Cast { value: expr, .. } => ice_shape(types, expr),
        ExprKind::Unary { op, operand } => {
            let shape = ice_shape(types, operand);
            if matches!(
                op,
                UnaryOp::Plus | UnaryOp::Minus | UnaryOp::Not | UnaryOp::BitNot
            ) {
                shape
            } else {
                shape.opaque()
            }
        }
        ExprKind::Binary {
            op: op @ (BinaryOp::And | BinaryOp::Or),
            left,
            right,
        } => {
            let left_shape = ice_shape(types, left);
            let short_circuits = constant_truth(types, left, left_shape)
                .map(|truth| truth == matches!(op, BinaryOp::Or));
            left_shape.and(evaluated_shape(types, right, short_circuits == Some(false)))
        }
        ExprKind::Binary { left, right, .. } => ice_shape(types, left).and(ice_shape(types, right)),
        ExprKind::Comma { left, right } => {
            ice_shape(types, left).and(ice_shape(types, right)).opaque()
        }
        ExprKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            let condition_shape = ice_shape(types, condition);
            let truth = constant_truth(types, condition, condition_shape);
            let then_shape = then_value.as_ref().map_or(Shape::Constant, |value| {
                evaluated_shape(types, value, truth == Some(true))
            });
            let else_shape = evaluated_shape(types, else_value, truth == Some(false));
            condition_shape.and(then_shape).and(else_shape)
        }
        ExprKind::Generic {
            controlling,
            associations,
        } => types
            .generic_selection(controlling, associations)
            .map_or(Shape::Skip, |selected| ice_shape(types, selected)),
        _ => Shape::Skip,
    }
}

fn is_fallthrough(attributes: &[Span<Attribute>]) -> bool {
    attributes
        .iter()
        .any(|attribute| matches!(attribute.value, Attribute::Fallthrough))
}

fn linkage(storage: StorageClass) -> Option<Linkage> {
    match storage {
        StorageClass::Static => Some(Linkage::Internal),
        StorageClass::None | StorageClass::Extern => Some(Linkage::External),
        _ => None,
    }
}
