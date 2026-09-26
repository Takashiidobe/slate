use super::SemaError;
use super::attributes::{Subject, Use};
use super::ctype::QualType;
use super::expression::Lowerer;
use super::numeric::{Context, ResolveError};
use super::operand::Operand;
use super::pragmas::{FloatingPragmas, PragmaPlacement, default_contraction};
use super::types::{Ordinary, TypeResolver, is_folded};
use crate::ast::{
    self, DeclKind, Declarator, ParameterList, Span, Stmt, StmtKind, StorageClass, TranslationUnit,
};
use crate::attribute_support;
use crate::compiler_args::CompilerFlavor;
use crate::const_expr::{IntegerLiteral, IntegerSizeSuffix, IntegerSuffix, Radix};
use crate::diagnostics::Warning;
use crate::ir::*;
use crate::standard_features::StandardFeatures;
use crate::target_info::TargetEnvironment;
use num_bigint::Sign;
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
        .with_features(features)
        .with_contraction(default_contraction(unit.flavor, unit.standard));
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
        names,
        function_declarations: HashMap::new(),
        builtin_declarations: HashMap::new(),
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
        floating_pragmas: FloatingPragmas::new(unit.flavor, context.region),
        compound_start: false,
        context,
    };
    for declaration in &unit.decls {
        match &declaration.value {
            DeclKind::Comment(_) | DeclKind::StaticAssert(_) => {}
            DeclKind::Pragma(pragma) => {
                lower.floating_pragmas.apply(
                    &mut lower.context.region,
                    &pragma.kind,
                    PragmaPlacement::File,
                )?;
            }
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
                lower.check_attributes(attributes.iter().copied(), Subject::Function)?;
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
                if matches!(params, ParameterList::IdentifierList { .. }) {
                    lower.warn(
                        Warning::DeprecatedNonPrototype,
                        "a function definition without a prototype is deprecated in all versions of C and is not supported in C23",
                        declaration,
                    );
                }
                let mut prologue = Vec::new();
                lower.in_function = true;
                lower.function_name = Some(name.to_string());
                lower.pretty_function_name = Some(lower.types.declaration_spelling(resolved, name));
                lower.return_type = return_type.as_ref().map(|_| return_c);
                let body = lower.scoped(|lower| {
                    let parameters = lower.parameters(params, Some(&mut prologue))?;
                    let body = lower.compound(|lower| {
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
    lower.finish_functions(unit.options.effective_inline_semantics(unit.standard))?;
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
    attributes: impl IntoIterator<Item = &'a Span<ast::Attribute>>,
    subject: Subject,
) -> Result<(), ResolveError> {
    for attribute in attributes {
        match super::attributes::declaration_use(&attribute.value, subject) {
            Use::Unsupported(reason) => return Err(ResolveError::Unsupported(reason)),
            Use::Invalid(reason) => return Err(ResolveError::Invalid(reason)),
            _ => {}
        }
    }
    Ok(())
}

fn applies(attribute: &Span<ast::Attribute>, subject: Subject) -> bool {
    !matches!(
        super::attributes::declaration_use(&attribute.value, subject),
        Use::Inapplicable { .. }
    )
}

fn symbol_attributes<'a>(
    attributes: impl IntoIterator<Item = &'a Span<ast::Attribute>>,
    asm_label: Option<&Span<ast::AsmLabel>>,
) -> Result<SymbolAttributes, ResolveError> {
    let mut symbol = SymbolAttributes::default();
    if let Some(label) = asm_label
        && let ast::AsmLabel::Symbol(name) = &label.value
    {
        symbol.asm_name = Some(name.clone());
    }
    for attribute in attributes {
        match &attribute.value {
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
            _ => {}
        }
    }
    Ok(symbol)
}

fn function_symbol<'a>(
    attributes: impl IntoIterator<Item = &'a Span<ast::Attribute>> + Clone,
    asm_label: Option<&Span<ast::AsmLabel>>,
) -> Result<SymbolAttributes, ResolveError> {
    if matches!(
        asm_label.map(|label| &label.value),
        Some(ast::AsmLabel::Register(_))
    ) {
        return Err(ResolveError::Unsupported("register asm label on function"));
    }
    if attributes
        .clone()
        .into_iter()
        .any(|attribute| matches!(&attribute.value, ast::Attribute::ThreadLocal))
    {
        return Err(ResolveError::Invalid("thread-local function"));
    }
    symbol_attributes(
        attributes.into_iter().filter(|attribute| {
            matches!(
                &attribute.value,
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
    fn c_attribute_metadata<'a>(
        &mut self,
        attributes: impl IntoIterator<Item = &'a Span<ast::Attribute>>,
    ) -> Result<Option<(String, String)>, ResolveError> {
        self.render_c_attributes(attributes.into_iter().map(|attribute| &attribute.value))
    }

    pub(super) fn render_c_attributes<'a>(
        &mut self,
        attributes: impl IntoIterator<Item = &'a ast::Attribute>,
    ) -> Result<Option<(String, String)>, ResolveError> {
        let mut folded = Vec::new();
        for attribute in attributes {
            folded.push(self.fold_c_attribute(attribute)?);
        }
        Ok((!folded.is_empty()).then(|| ("c_attributes".into(), format!("{folded:?}"))))
    }

    fn fold_attribute_expr(&mut self, expression: &ast::Expr) -> Result<ast::Expr, ResolveError> {
        let value = self.types.constant_integer(expression)?;
        let magnitude = value.magnitude().clone();
        let literal = Box::new(
            expression.derive(ast::ExprKind::IntegerLiteral(IntegerLiteral {
                spelling: magnitude.to_string(),
                value: magnitude,
                radix: Radix::Decimal,
                suffix: IntegerSuffix {
                    unsigned: false,
                    size: IntegerSizeSuffix::None,
                },
                imaginary: false,
            })),
        );
        if value.sign() == Sign::Minus {
            Ok(Box::new(expression.derive(ast::ExprKind::Unary {
                op: crate::const_expr::UnaryOp::Minus,
                operand: literal,
            })))
        } else {
            Ok(literal)
        }
    }

    fn fold_c_attribute(
        &mut self,
        attribute: &ast::Attribute,
    ) -> Result<ast::Attribute, ResolveError> {
        let fold = |this: &mut Self, expression: &ast::Expr| this.fold_attribute_expr(expression);
        Ok(match attribute {
            ast::Attribute::AddressSpace(expression) => {
                ast::Attribute::AddressSpace(fold(self, expression)?)
            }
            ast::Attribute::PassObjectSize { size_type, dynamic } => {
                ast::Attribute::PassObjectSize {
                    size_type: fold(self, size_type)?,
                    dynamic: *dynamic,
                }
            }
            ast::Attribute::Aligned(expression) => ast::Attribute::Aligned(fold(self, expression)?),
            ast::Attribute::AlignAs(ast::AlignAsOperand::Expr(expression)) => {
                ast::Attribute::AlignAs(ast::AlignAsOperand::Expr(fold(self, expression)?))
            }
            ast::Attribute::VectorSize(expression) => {
                ast::Attribute::VectorSize(fold(self, expression)?)
            }
            ast::Attribute::AssumeAligned(expressions) => ast::Attribute::AssumeAligned(
                expressions
                    .iter()
                    .map(|expression| fold(self, expression))
                    .collect::<Result<_, _>>()?,
            ),
            ast::Attribute::AllocSize(expressions) => ast::Attribute::AllocSize(
                expressions
                    .iter()
                    .map(|expression| fold(self, expression))
                    .collect::<Result<_, _>>()?,
            ),
            ast::Attribute::AllocAlign(expression) => {
                ast::Attribute::AllocAlign(fold(self, expression)?)
            }
            ast::Attribute::ExtVectorType(expression) => {
                ast::Attribute::ExtVectorType(fold(self, expression)?)
            }
            ast::Attribute::CallingConvention(ast::CallingConvention::RegParm(expression)) => {
                ast::Attribute::CallingConvention(ast::CallingConvention::RegParm(fold(
                    self, expression,
                )?))
            }
            attribute => attribute.clone(),
        })
    }

    fn check_attributes<'a>(
        &mut self,
        attributes: impl IntoIterator<Item = &'a Span<ast::Attribute>>,
        subject: Subject,
    ) -> Result<(), ResolveError> {
        for attribute in attributes {
            match super::attributes::declaration_use(&attribute.value, subject) {
                Use::Unsupported(reason) => return Err(ResolveError::Unsupported(reason)),
                Use::Invalid(reason) => return Err(ResolveError::Invalid(reason)),
                Use::Inapplicable {
                    spelling,
                    applies_to,
                } => {
                    let message = match applies_to {
                        Some(subjects) => {
                            format!("'{spelling}' attribute ignored; it applies only to {subjects}")
                        }
                        None => format!("'{spelling}' attribute ignored"),
                    };
                    self.warn(Warning::IgnoredAttributes, &message, attribute);
                }
                Use::Unknown => {
                    if let ast::Attribute::Unknown { name, .. } = &attribute.value
                        && !attribute_support::spelling_registered(
                            name,
                            self.types.flavor,
                            &self.context.target,
                        )
                    {
                        let message = format!("unknown attribute '{name}' ignored");
                        self.warn(Warning::UnknownAttributes, &message, attribute);
                    }
                }
                Use::UnsupportedDeclspec => {
                    if let ast::Attribute::IgnoredDeclspec { name, .. } = &attribute.value {
                        let message = format!("__declspec attribute '{name}' is not supported");
                        self.warn(Warning::IgnoredAttributes, &message, attribute);
                    }
                }
                Use::Symbol | Use::Layout | Use::Ignored => {}
            }
        }
        Ok(())
    }

    fn resolve_object_requests(&mut self, unit: &TranslationUnit) -> Result<(), ResolveError> {
        let msvc_target = self.context.target.environment == TargetEnvironment::Msvc;
        for function in &mut self.module.functions {
            let id = function.value.id;
            if let Some(linkage) = self.types.entities.linkage(id) {
                function.value.linkage = linkage;
            }
            if let Some(symbol) = self.types.entities.symbol(id) {
                function.value.symbol = symbol.clone();
            }
        }
        for global in &mut self.module.globals {
            let global = &mut global.value;
            let id = global.variable.id;
            if let Some(linkage) = self.types.entities.linkage(id) {
                global.linkage = linkage;
                if let Some(storage) = self.types.entities.storage(id) {
                    global.variable.storage = storage;
                }
                global.definition = self.types.entities.definition(id);
                if let Some(symbol) = self.types.entities.symbol(id) {
                    global.symbol = symbol.clone();
                }
            }
            let request = self.types.entities.request(&global.variable.id);
            global.variable.alignment = self.types.object_alignment_override(
                &global.variable.ty,
                self.types.entities.ty(&id),
                request.alignment,
            )?;
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
        self.types.entities.merge_declaration(
            id,
            global.value.linkage,
            Some(global.value.variable.storage),
            global.value.definition,
            global.value.symbol.clone(),
        );
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
        Ok(())
    }

    fn declare_function(
        &mut self,
        function: Span<Function>,
        previous: Option<QualType>,
    ) -> Result<(), ResolveError> {
        let id = function.value.id;
        self.types.entities.merge_declaration(
            id,
            function.value.linkage,
            None,
            function.value.body.is_some(),
            function.value.symbol.clone(),
        );
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
        let existing = &self.module.functions[index];
        let parameterized = matches!(&function.value.parameters, Parameters::Prototype { fixed, .. } if !fixed.is_empty());
        if parameterized
            && existing.value.body.is_none()
            && matches!(existing.value.parameters, Parameters::Unprototyped)
        {
            let subsequent = if function.value.body.is_some() {
                "definition"
            } else {
                "declaration"
            };
            let message = format!(
                "a function declaration without a prototype is deprecated in all versions of C and is treated as a zero-parameter prototype in C23, conflicting with a subsequent {subsequent}"
            );
            let anchor = existing.derive(());
            self.warn(Warning::DeprecatedNonPrototype, &message, &anchor);
        }
        let existing = &mut self.module.functions[index];
        let function = function.value;
        let replaces = function.body.is_some()
            || (existing.value.body.is_none()
                && matches!(existing.value.parameters, Parameters::Unprototyped));
        if replaces {
            existing.value = function;
        }
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
            let attributes = || {
                parameter
                    .specifiers
                    .attributes
                    .iter()
                    .chain(&parameter.attributes)
            };
            self.check_attributes(attributes(), Subject::Parameter)?;
            let alignment = super::types::requested_alignment(&mut self.types, attributes())?;
            if let Some(alignment) = alignment {
                let rejected_by = if attributes()
                    .any(|attribute| matches!(&attribute.value, ast::Attribute::AlignAs(_)))
                {
                    "clang and gcc"
                } else {
                    "gcc"
                };
                self.warn(
                    Warning::ParameterAlignment,
                    &format!(
                        "alignment of {alignment} on a function parameter is rejected by {rejected_by}"
                    ),
                    parameter,
                );
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
            self.types.entities.merge_request(
                id,
                super::entity::ObjectRequest {
                    alignment,
                    common: None,
                },
            )?;
            for definition in &self.types.definitions[start..] {
                self.type_spans
                    .insert(definition.id, parameter.derive(definition.clone()));
            }
            let promoted = matches!(params, ParameterList::IdentifierList { .. })
                .then(|| self.types.promoted_parameter(resolved))
                .filter(|promoted| {
                    self.types.ctypes.canonical(*promoted).local_unqualified()
                        != self.types.ctypes.canonical(adjusted).local_unqualified()
                });
            if let (Some(promoted), Some(prologue)) = (promoted, prologue.as_deref_mut()) {
                let slot = self.fresh();
                self.types.entities.declare(slot, promoted, false);
                let ty = self.types.ir_type(promoted);
                let passed = Operand {
                    value: Value {
                        ty: ty.clone(),
                        node: parameter.derive(ValueKind::Read {
                            place: Place {
                                ty: ty.clone(),
                                kind: PlaceKind::Binding(slot),
                                access: Access::default(),
                            },
                            ordering: None,
                        }),
                    },
                    c: promoted,
                };
                let unqualified = self.types.ctypes.unqualified(adjusted);
                let initializer = self.convert(passed, unqualified, ConversionReason::Arg)?;
                let local = parameter.derive(Statement::Let(Variable {
                    id,
                    name: name.unwrap_or_default().into(),
                    ty: shape.ty,
                    storage: StorageDuration::Automatic,
                    restrict: shape.qualifiers.is_restrict,
                    is_const: shape.qualifiers.is_const,
                    access: Access {
                        volatile: shape.qualifiers.is_volatile,
                        atomic: shape.qualifiers.is_atomic,
                    },
                    constexpr: false,
                    alignment: None,
                    cleanup: None,
                    register: None,
                    initializer: Some(initializer.value),
                }));
                self.module
                    .annotate(&local, self.types.render(resolved).entries());
                prologue.push(local);
                let slot = parameter.derive(Parameter {
                    id: slot,
                    name: name.map(str::to_owned),
                    ty,
                    restrict: false,
                    is_const: false,
                    access: Access::default(),
                    array: None,
                });
                self.module
                    .annotate(&slot, self.types.render(promoted).entries());
                fixed.push(slot);
                continue;
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
            let mut c_entries = self.types.render(resolved).entries();
            if let Some(metadata) = self.c_attribute_metadata(attributes())? {
                c_entries.push(metadata);
            }
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
            reject_unsupported(&item.specifiers.attributes, Subject::Record)?;
            if !self.types.declare_forward_tag(&item.specifiers) {
                self.resolve_type(&item.specifiers, &Declarator::Abstract)?;
            }
        }
        let storage_class = item.specifiers.storage;
        let inferred = matches!(item.specifiers.ty, ast::TypeSpecifier::Inferred);
        if inferred && self.types.flavor == CompilerFlavor::Gcc && item.declarators.len() > 1 {
            return Err(ResolveError::Invalid(
                "'auto' may only be used with a single declarator",
            ));
        }
        let mut deduced = None;
        let mut statements = Vec::new();
        for declarator in &item.declarators {
            let attributes = item
                .specifiers
                .attributes
                .iter()
                .chain(&declarator.attributes);
            if storage_class == StorageClass::Typedef {
                self.check_attributes(attributes.clone(), Subject::Typedef)?;
            }
            let thread = item.specifiers.is_thread_local
                || attributes
                    .clone()
                    .any(|attribute| matches!(&attribute.value, ast::Attribute::ThreadLocal));
            let name = declarator
                .declarator
                .name()
                .ok_or(ResolveError::Unsupported("unnamed declaration"))?;
            if !global {
                let anchor = declarator.derive(());
                self.capture_extents(&declarator.declarator, &anchor, &mut statements)?;
            }
            let start = self.types.definitions.len();
            let value = if inferred {
                let binding = self.declaration_id(declarator.id, name)?;
                let (base, value) = self.infer_type(
                    &item.specifiers,
                    &declarator.declarator,
                    declarator.initializer.as_ref(),
                    binding,
                )?;
                let canonical = self.types.ctypes.canonical(base);
                let placeholder = canonical
                    .local_unqualified()
                    .with(canonical.quals.without(item.specifiers.qualifiers.into()));
                if deduced.is_some_and(|first| first != placeholder) {
                    return Err(ResolveError::Invalid(
                        "'auto' deduced as different types in one declaration",
                    ));
                }
                deduced = Some(placeholder);
                self.types.inferred = Some(base);
                Some(value)
            } else {
                None
            };
            let resolved = self.resolve_declarator_type(
                &item.specifiers,
                &declarator.declarator,
                &declarator.attributes,
            )?;
            if let Some(value) = value {
                self.check_inferred(resolved, value)?;
            }
            let mut c_entries = self.types.render(resolved).entries();
            if item.specifiers.storage == StorageClass::Typedef {
                self.types
                    .define_alias(name.into(), resolved, attributes.clone())?;
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
                self.check_attributes(attributes.iter().copied(), Subject::Function)?;
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
            let subject = Subject::Object {
                automatic: storage == StorageDuration::Automatic,
            };
            self.check_attributes(attributes.clone(), subject)?;
            let attributes = || attributes.clone().filter(|a| applies(a, subject));
            if let Some(metadata) = self.c_attribute_metadata(attributes())? {
                c_entries.push(metadata);
            }
            if !global && linked && declarator.initializer.is_some() {
                return Err(ResolveError::Invalid("block scope extern initializer"));
            }
            let mut symbol = symbol_attributes(attributes(), declarator.asm_label.as_ref())?;
            self.types.pragmas.apply(name, &mut symbol);
            let request = super::entity::ObjectRequest {
                alignment: super::types::requested_alignment(&mut self.types, attributes())?,
                common: if attributes()
                    .any(|attribute| matches!(&attribute.value, ast::Attribute::Common))
                {
                    Some(true)
                } else if attributes()
                    .any(|attribute| matches!(&attribute.value, ast::Attribute::NoCommon))
                {
                    Some(false)
                } else {
                    None
                },
            };
            self.types.entities.merge_request(id, request)?;
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
                    let region = self.context.region;
                    if storage != StorageDuration::Automatic {
                        self.context.region.floating = FloatingSemantics::default();
                    }
                    let value = self.initializer_value(resolved, initializer, &anchor);
                    self.context.region = region;
                    let mut value = value?;
                    if self.types.flavor == CompilerFlavor::Msvc
                        && (global || storage_class == StorageClass::Static)
                    {
                        super::fold::fold_msvc_static_divisions(&mut value);
                    }
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
            let automatic_alignment = if storage == StorageDuration::Automatic {
                self.types
                    .object_alignment_override(&ty, Some(resolved), request.alignment)?
            } else {
                None
            };
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
                cleanup: attributes().find_map(|attribute| match &attribute.value {
                    ast::Attribute::Cleanup(function) => Some(function.clone()),
                    _ => None,
                }),
                register: declarator
                    .asm_label
                    .as_ref()
                    .and_then(|label| match &label.value {
                        ast::AsmLabel::Register(register) => Some(super::asm::register(register)),
                        _ => None,
                    }),
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
                let declared_linkage = if storage_class == StorageClass::Register {
                    Linkage::External
                } else if item.specifiers.is_constexpr {
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
                self.types.entities.merge_declaration(
                    global.value.variable.id,
                    global.value.linkage,
                    Some(global.value.variable.storage),
                    global.value.definition,
                    global.value.symbol.clone(),
                );
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

    pub(super) fn compound<T>(
        &mut self,
        lower: impl FnOnce(&mut Self) -> Result<T, ResolveError>,
    ) -> Result<T, ResolveError> {
        let region = self.context.region;
        self.compound_start = true;
        let result = self.scoped(lower);
        self.context.region = region;
        result
    }

    fn case_value(&mut self, expr: &ast::Expr, ty: QualType) -> Result<Value, ResolveError> {
        let value = self.expr(expr)?;
        let value = self.convert(value, ty, ConversionReason::Promotion)?;
        let number = super::fold::integer_constant(&value, self.types.flavor)
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
            match &statement.value {
                StmtKind::Pragma(pragma) => {
                    let placement = if self.compound_start {
                        PragmaPlacement::CompoundStart
                    } else {
                        PragmaPlacement::Misplaced
                    };
                    self.floating_pragmas.apply(
                        &mut self.context.region,
                        &pragma.kind,
                        placement,
                    )?;
                    continue;
                }
                StmtKind::Comment(_) => continue,
                _ => self.compound_start = false,
            }
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
                        .any(|a| matches!(&a.value, ast::Attribute::Fallthrough)) =>
                {
                    if self.switches.is_empty() {
                        return Err(ResolveError::Unsupported("fallthrough outside switch"));
                    }
                    annotations.push(("c_attribute".into(), "fallthrough".into()));
                    Statement::Null
                }
                StmtKind::Attribute(_) => Statement::Null,
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
                    Statement::Block(self.compound(|lower| lower.statements(body, return_type))?)
                }
                StmtKind::ReturnVoid => match self.types.flavor {
                    CompilerFlavor::Msvc => Statement::Return(None),
                    CompilerFlavor::Gcc if self.types.features.valueless_return_in_nonvoid => {
                        Statement::Return(None)
                    }
                    _ => {
                        return Err(ResolveError::Invalid(
                            "non-void function should return a value",
                        ));
                    }
                },
                StmtKind::Attributed { attributes, body } => {
                    if self.types.flavor != CompilerFlavor::Gcc
                        && attributes
                            .iter()
                            .any(|a| matches!(&a.value, ast::Attribute::Fallthrough))
                    {
                        return Err(ResolveError::Invalid(
                            "fallthrough attribute on a non-empty statement",
                        ));
                    }
                    result.extend(self.statements(std::slice::from_ref(body), return_type)?);
                    continue;
                }
                StmtKind::NestedFunction(_) => {
                    return Err(if self.types.flavor == CompilerFlavor::Gcc {
                        ResolveError::Unsupported("GNU nested function")
                    } else {
                        ResolveError::Invalid("function definition is not allowed here")
                    });
                }
            };
            let lowered = statement.derive(kind);
            self.module.annotate(&lowered, annotations);
            result.push(lowered);
        }
        Ok(result)
    }
}
