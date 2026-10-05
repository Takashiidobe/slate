use super::attributes::{Subject, Use};
use super::ctype::QualType;
use super::expression::Lowerer;
use super::initializer::InitializerSource;
use super::numeric::{Context, ResolveError};
use super::operand::Operand;
use super::pragmas::{FloatingPragmas, PragmaPlacement};
use super::typer::Slot;
use super::types::{TypeResolver, is_folded};
use super::validate::{ERROR_LIMIT, with_sources};
use super::{SemaError, SemaErrors};
use crate::ast::{
    self, DeclKind, Declarator, ParameterList, Span, Stmt, StmtKind, StorageClass, TranslationUnit,
};
use crate::compiler_args::CompilerFlavor;
use crate::const_expr::{IntegerLiteral, IntegerSizeSuffix, IntegerSuffix, Radix};
use crate::diagnostics::Warning;
use crate::ir::*;
use crate::standard_features::StandardFeatures;
use crate::target_info::TargetEnvironment;
use num_bigint::Sign;
use std::collections::{HashMap, HashSet};

impl super::Sema<'_> {
    pub fn lower(
        self,
        files: &crate::files::Files,
    ) -> Result<(Module, Vec<SemaError>), SemaErrors> {
        resolve_module(self.unit, self.names, self.items, self.types, files)
    }
}

fn resolve_module(
    unit: &TranslationUnit,
    names: NameResolution,
    items: Vec<super::names::ItemResolution>,
    mut types: TypeResolver,
    files: &crate::files::Files,
) -> Result<(Module, Vec<SemaError>), SemaErrors> {
    let context = Context::for_dialect(&unit.dialect);
    let next_id = names
        .bindings
        .iter()
        .map(|b| b.value.id.0 + 1)
        .max()
        .unwrap_or(0);
    types.entities = super::entity::Entities::default();
    let mut lower = Lowerer {
        types,
        module: Module::new(context.target.clone()),
        names,
        function_declarations: HashMap::new(),
        builtin_declarations: HashMap::new(),
        alias_annotations: HashMap::new(),
        next_id,
        break_targets: Vec::new(),
        continue_targets: Vec::new(),
        switches: Vec::new(),
        in_function: false,
        in_naked_function: false,
        files: files.clone(),
        return_type: None,
        ms_asm_return: Vec::new(),
        floating_pragmas: FloatingPragmas::new(unit.dialect.flavor(), context.region),
        compound_start: false,
        reserved_extents: HashMap::new(),
        bare_weakrefs: HashSet::new(),
        context,
    };
    let mut poisoned = HashSet::new();
    let mut error_count = 0;
    for (declaration, item) in unit.decls.iter().zip(items) {
        if let Some(diagnostics) = lower.types.item_diagnostics.remove(&declaration.id) {
            lower.types.diagnostics.extend(diagnostics);
        }
        let errors: Vec<ResolveError> = if item.errors.is_empty() {
            let Err(error) = lower
                .declare_implicit_functions(item.declared.clone())
                .and_then(|()| lower_item(&mut lower, declaration, unit.dialect.features()))
            else {
                continue;
            };
            lower.reset_after_failed_item();
            let uses_poisoned = lower.names.references[item.references]
                .iter()
                .any(|reference| poisoned.contains(&reference.binding));
            if uses_poisoned {
                vec![]
            } else {
                vec![error.checked()]
            }
        } else {
            item.errors.into_iter().map(ResolveError::Names).collect()
        };
        poisoned.extend(item.declared.map(BindingId));
        for error in &errors {
            lower.types.diagnostics.push(item_error(error, declaration));
        }
        error_count += errors.len();
        if error_count > ERROR_LIMIT {
            break;
        }
    }
    if error_count > 0 {
        with_sources(std::mem::take(&mut lower.types.diagnostics), files)?;
    }
    for definition in &lower.types.definitions {
        let span = if let Some(owner) = lower.types.owners.get(&definition.id) {
            owner.derive(definition.clone())
        } else if let Some(tag) = lower.types.tag_span(definition.id, unit) {
            tag.derive(definition.clone())
        } else if let Some(declaration) = unit.decls.first() {
            declaration.derive(definition.clone())
        } else {
            continue;
        };
        if let Some(entries) = lower.alias_annotations.remove(&definition.id) {
            lower.module.annotate(&span, entries);
        }
        lower.module.types.push(span);
    }
    for global in &mut lower.module.globals {
        if global.definition
            && let Type::Array { length, .. } = &mut global.value.variable.ty
            && length.is_none()
        {
            *length = Some(1);
        }
    }
    lower.finish_module(unit).map_err(|error| SemaErrors {
        errors: vec![SemaError::resolve_error(&error.checked(), files)],
    })?;
    let diagnostics = with_sources(lower.types.diagnostics, files)?;
    Ok((lower.module, diagnostics))
}

fn item_error(error: &ResolveError, item: &ast::Decl) -> SemaError {
    super::validate::error(
        item.provenance,
        error.loc().unwrap_or(item.expansion),
        error.to_string(),
    )
}

impl Lowerer {
    fn finish_module(&mut self, unit: &TranslationUnit) -> Result<(), ResolveError> {
        self.resolve_object_requests(unit)?;
        self.finish_functions(unit.dialect.inline_semantics())?;
        self.complete_declaration_abis()?;
        let declared: Vec<_> = self.types.entities.types().collect();
        let access = declared
            .into_iter()
            .map(|(id, c)| (id, self.types.access_of(c)))
            .collect();
        super::effects_statements::normalize(&mut self.module, self.next_id, access)
    }

    fn complete_declaration_abis(&mut self) -> Result<(), ResolveError> {
        for index in 0..self.module.functions.len() {
            let function = &self.module.functions[index].value;
            if function.abi.is_some() {
                continue;
            }
            let signature = self
                .types
                .entities
                .ty(&function.id)
                .ok_or(ResolveError::Internal("missing function type"))?;
            let ty = self.types.ir_type(signature);
            self.module.functions[index].value.abi = self.declaration_abi(signature, &ty)?;
        }
        Ok(())
    }

    fn declare_implicit_functions(
        &mut self,
        declared: std::ops::Range<u32>,
    ) -> Result<(), ResolveError> {
        for id in declared.map(BindingId) {
            if self.names.implicit_functions.contains(&id) {
                self.declare_implicit_function(id)?;
            }
        }
        Ok(())
    }

    fn declare_implicit_function(&mut self, id: BindingId) -> Result<(), ResolveError> {
        let binding = self
            .names
            .bindings
            .iter()
            .find(|binding| binding.value.id == id)
            .ok_or(ResolveError::Internal("missing declaration binding"))?
            .clone();
        let resolved = self.types.ctypes.implicit_function();
        let ty = self.types.ir_type(resolved);
        let abi = Some(self.c_abi_signature(resolved, &ty, None)?);
        self.types.entities.declare(id, resolved, false);
        self.record_implicit_function(id);
        let mut symbol = SymbolAttributes::default();
        self.types.pragmas.apply(&binding.name, &mut symbol);
        let function = binding.derive(Function {
            id,
            name: binding.name.clone(),
            parameters: Parameters::Unprototyped,
            return_type: Some(self.context.int_type()),
            abi,
            linkage: Linkage::External,
            symbol,
            semantics: Default::default(),
            body: None,
            fallthrough: None,
        });
        let message = format!(
            "implicit declaration of function '{}'; assuming extern returning int",
            binding.name
        );
        self.warn(Warning::ImplicitFunctionDeclaration, &message, &function);
        let mut metadata = vec![("c_implicit".into(), "true".into())];
        metadata.extend(self.types.render(resolved).entries());
        self.module.annotate(&function, metadata);
        self.declare_function(function)
    }

    fn reset_after_failed_item(&mut self) {
        self.break_targets.clear();
        self.continue_targets.clear();
        self.switches.clear();
        self.ms_asm_return.clear();
        self.in_function = false;
        self.in_naked_function = false;
        self.types.function_names = None;
        self.return_type = None;
        self.compound_start = false;
        self.types.owner = None;
    }
}

fn lower_item(
    lower: &mut Lowerer,
    declaration: &ast::Decl,
    features: StandardFeatures,
) -> Result<(), ResolveError> {
    match &declaration.value {
        DeclKind::Comment(_) | DeclKind::StaticAssert(_) => {}
        DeclKind::Attribute(attributes) => {
            if lower.types.flavor().is_gcc() {
                for attribute in attributes {
                    lower.warn(Warning::IgnoredAttributes, "attribute ignored", attribute);
                }
            }
        }
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
            let attributes = function
                .specifiers
                .attributes_with(&function.declarator, &function.attributes)
                .collect::<Vec<_>>();
            lower
                .types
                .check_attributes(attributes.iter().copied(), Subject::Function)
                .map_err(ResolveError::checked)?;
            let mut symbol =
                function_symbol(attributes.iter().copied(), None).map_err(ResolveError::checked)?;
            let name = function
                .declarator
                .name()
                .ok_or(ResolveError::Internal("unnamed function"))?;
            lower.types.pragmas.apply(name, &mut symbol);
            let id = lower.declaration_id(declaration.id, name)?;
            lower
                .record_function(id, &function.specifiers, &attributes, true, true)
                .map_err(ResolveError::checked)?;
            let owner = lower.types.owner.replace(declaration.derive(()));
            let resolved = lower.resolve_type(&function.specifiers, &function.declarator)?;
            let resolved = lower.types.apply_convention(resolved, &function.attributes);
            let resolved = lower.types.inherit_convention(id, resolved);
            let (return_c, ..) = lower
                .types
                .ctypes
                .function_parts(resolved)
                .ok_or(ResolveError::Internal("function definition declarator"))?;
            let c_return = lower.types.render(return_c).spelling;
            lower.types.owner = owner;
            let ty = lower.types.object_type(resolved, "void function type")?;
            let Type::Function { return_type, .. } = &ty else {
                return Err(ResolveError::Internal("function definition declarator"));
            };
            let return_type = return_type.as_ref().map(|ty| (**ty).clone());
            let abi = Some(lower.c_abi_signature(resolved, &ty, None)?);
            lower.redeclared(declaration.id, id)?;
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
                .ok_or(ResolveError::Internal("missing function parameters"))?;
            if matches!(params, ParameterList::IdentifierList { .. }) {
                lower.warn(
                    Warning::DeprecatedNonPrototype,
                    "a function definition without a prototype is deprecated in all versions of C and is not supported in C23",
                    declaration,
                );
            }
            let mut prologue = Vec::new();
            lower.in_function = true;
            lower.in_naked_function = lower.is_naked(id);
            lower.types.function_names = Some(lower.types.function_names(resolved, name));
            lower.return_type = return_type.as_ref().map(|_| return_c);
            let body = lower
                .parameters(params, Some(&mut prologue))
                .and_then(|parameters| {
                    let body = lower.compound(|lower| {
                        lower.statements(&function.body, return_type.as_ref().map(|_| return_c))
                    })?;
                    prologue.extend(body);
                    Ok((parameters, prologue))
                });
            lower.in_function = false;
            lower.in_naked_function = false;
            lower.types.function_names = None;
            lower.return_type = None;
            let (parameters, mut body) = body?;
            let asm_return = lower.finish_ms_asm_return(
                &declaration.derive(()),
                return_type.as_ref(),
                &mut body,
                name == "main"
                    && return_type == Some(lower.context.int_type())
                    && features.main_implicit_return_zero,
            )?;
            let fallthrough = if let Some(value) = asm_return {
                Fallthrough::Return(Box::new(value))
            } else if name == "main"
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
                linkage: linkage(function.specifiers.storage).map_err(ResolveError::checked)?,
                symbol,
                semantics: Default::default(),
                body: Some(body),
                fallthrough: Some(fallthrough),
            });
            lower.module.annotate(&lowered, metadata);
            lower.declare_function(lowered)?;
        }
    }
    Ok(())
}

pub(super) fn linkage(storage: StorageClass) -> Result<Linkage, ResolveError> {
    match storage {
        StorageClass::Static => Ok(Linkage::Internal),
        StorageClass::None | StorageClass::Extern => Ok(Linkage::External),
        _ => Err(ResolveError::Rejected("linkage storage class")),
    }
}

pub(super) fn applies(attribute: &Span<ast::Attribute>, subject: Subject) -> bool {
    !matches!(
        super::attributes::declaration_use(&attribute.value, subject),
        Use::Inapplicable { .. }
    )
}

pub(super) fn symbol_attributes<'a>(
    attributes: impl IntoIterator<Item = &'a Span<ast::Attribute>>,
    asm_label: Option<&Span<ast::AsmLabel>>,
) -> Result<SymbolAttributes, ResolveError> {
    let mut symbol = SymbolAttributes::default();
    let mut weakref = false;
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
                    _ => return Err(ResolveError::Rejected("visibility")),
                });
            }
            ast::Attribute::TlsModel(name) => {
                symbol.tls_model = Some(match name.as_str() {
                    "global-dynamic" => TlsModel::GlobalDynamic,
                    "local-dynamic" => TlsModel::LocalDynamic,
                    "initial-exec" => TlsModel::InitialExec,
                    "local-exec" => TlsModel::LocalExec,
                    _ => return Err(ResolveError::Rejected("tls_model")),
                });
            }
            ast::Attribute::Weak => symbol.weak = true,
            ast::Attribute::Alias(target) => symbol.alias = Some(target.clone()),
            ast::Attribute::Section(name) => symbol.section = Some(name.clone()),
            ast::Attribute::Used => symbol.used = true,
            ast::Attribute::Retain => symbol.retain = true,
            ast::Attribute::DllImport => symbol.dll_storage = Some(DllStorage::Import),
            ast::Attribute::DllExport => symbol.dll_storage = Some(DllStorage::Export),
            ast::Attribute::WeakRef(target) => {
                weakref = true;
                symbol.weakref = symbol.weakref.take().or(target.clone());
            }
            ast::Attribute::Ifunc(resolver) => symbol.ifunc = Some(resolver.clone()),
            ast::Attribute::SelectAny => symbol.selectany = true,
            ast::Attribute::ThreadLocal
            | ast::Attribute::Aligned(_)
            | ast::Attribute::AlignAs(_)
            | ast::Attribute::Common
            | ast::Attribute::NoCommon => {}
            _ => {}
        }
    }
    if weakref {
        let alias = symbol.alias.take();
        symbol.weakref = symbol.weakref.take().or(alias);
    }
    Ok(symbol)
}

fn is_bare_weakref(attribute: &Span<ast::Attribute>) -> bool {
    matches!(attribute.value, ast::Attribute::WeakRef(None))
}

// gcc: a bare weakref takes an alias from any redeclaration; ignored on a definition.
fn weak_reference(mut symbol: SymbolAttributes, bare: bool, defined: bool) -> SymbolAttributes {
    if defined {
        symbol.weakref = None;
    } else if bare && symbol.weakref.is_none() {
        symbol.weakref = symbol.alias.take();
    }
    symbol
}

pub(super) fn function_symbol<'a>(
    attributes: impl IntoIterator<Item = &'a Span<ast::Attribute>> + Clone,
    asm_label: Option<&Span<ast::AsmLabel>>,
) -> Result<SymbolAttributes, ResolveError> {
    if matches!(
        asm_label.map(|label| &label.value),
        Some(ast::AsmLabel::Register(_))
    ) {
        return Err(ResolveError::Rejected("register asm label on function"));
    }
    if attributes
        .clone()
        .into_iter()
        .any(|attribute| matches!(&attribute.value, ast::Attribute::ThreadLocal))
    {
        return Err(ResolveError::Rejected("thread-local function"));
    }
    symbol_attributes(
        attributes.into_iter().filter(|attribute| {
            matches!(
                &attribute.value,
                ast::Attribute::Visibility(_)
                    | ast::Attribute::Weak
                    | ast::Attribute::Alias(_)
                    | ast::Attribute::WeakRef(_)
                    | ast::Attribute::Ifunc(_)
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
            ast::Attribute::Malloc {
                deallocator,
                argument,
            } => ast::Attribute::Malloc {
                deallocator: deallocator.clone(),
                argument: argument
                    .as_ref()
                    .map(|argument| fold(self, argument))
                    .transpose()?,
            },
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

    fn resolve_object_requests(&mut self, unit: &TranslationUnit) -> Result<(), ResolveError> {
        let msvc_target = self.context.target.environment == TargetEnvironment::Msvc;
        for function in &mut self.module.functions {
            let id = function.value.id;
            if let Some(linkage) = self.types.entities.linkage(id) {
                function.value.linkage = linkage;
            }
            if let Some(symbol) = self.types.entities.symbol(id) {
                function.value.symbol = weak_reference(
                    symbol.clone(),
                    self.bare_weakrefs.contains(&id),
                    function.value.body.is_some(),
                );
            }
        }
        for global in &mut self.module.globals {
            let loc = global.expansion;
            let global = &mut global.value;
            let id = global.variable.id;
            if let Some(linkage) = self.types.entities.linkage(id) {
                global.linkage = linkage;
                if let Some(storage) = self.types.entities.storage(id) {
                    global.variable.storage = storage;
                }
                global.definition = self.types.entities.definition(id);
                if let Some(symbol) = self.types.entities.symbol(id) {
                    global.symbol = weak_reference(
                        symbol.clone(),
                        self.bare_weakrefs.contains(&id),
                        global.variable.initializer.is_some(),
                    );
                    global.definition &= global.symbol.weakref.is_none();
                }
            }
            let request = self.types.entities.request(&global.variable.id);
            global.variable.alignment = self
                .types
                .object_alignment_override(
                    &global.variable.ty,
                    self.types.entities.ty(&id),
                    request.alignment,
                )
                .map_err(|error| error.checked().at(loc))?;
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
            global.common = tentative && request.common.unwrap_or(unit.dialect.options().common);
        }
        Ok(())
    }

    fn redeclared(&mut self, node: ast::NodeId, id: BindingId) -> Result<(), ResolveError> {
        if let Some(error) = self.types.unsupported_declarations.get(&node) {
            return Err(error.clone());
        }
        let recorded = *self
            .types
            .declared_types
            .get(&node)
            .ok_or(ResolveError::Internal(
                "declaration type not recorded by the checker",
            ))?;
        self.types.entities.declare(id, recorded, false);
        Ok(())
    }

    fn declare_global(&mut self, global: Span<Global>) -> Result<(), ResolveError> {
        let id = global.value.variable.id;
        self.types.entities.merge_declaration(
            id,
            global.value.linkage,
            Some(global.value.variable.storage),
            global.value.definition,
            global.value.symbol.clone(),
        );
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
            .ok_or(ResolveError::Internal("untyped global redeclaration"))?;
        let merged_ty = self.types.ir_type(merged);
        let merged_quals = self.types.ctypes.quals(merged);
        let merged_access = self.types.access_of(merged);
        let existing = &mut self.module.globals[index];
        if global.value.variable.initializer.is_some()
            || global.value.definition && !existing.value.definition
        {
            existing.spelling = global.spelling;
            existing.expansion = global.expansion;
            existing.provenance = global.provenance;
            existing.macro_origin = global.macro_origin;
            existing.leading_space = global.leading_space;
        }
        let global = global.value;
        let existing = &mut existing.value;
        existing.definition |= global.definition;
        existing.variable.ty = merged_ty;
        existing.variable.restrict = merged_quals.is_restrict;
        existing.variable.is_const = merged_quals.is_const;
        existing.variable.access = merged_access;
        if global.variable.initializer.is_some() {
            if existing.variable.initializer.is_some() {
                return Err(ResolveError::Internal("multiple global initializers"));
            }
            existing.variable.initializer = global.variable.initializer;
        }
        Ok(())
    }

    fn declare_function(&mut self, function: Span<Function>) -> Result<(), ResolveError> {
        let id = function.value.id;
        self.types.entities.merge_declaration(
            id,
            function.value.linkage,
            None,
            function.value.body.is_some(),
            function.value.symbol.clone(),
        );
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
        if function.value.body.is_some() {
            *existing = Span {
                id: existing.id,
                ..function
            };
        } else if existing.value.body.is_none()
            && matches!(existing.value.parameters, Parameters::Unprototyped)
        {
            existing.value = function.value;
        }
        Ok(())
    }

    fn parameters(
        &mut self,
        params: &ParameterList,
        mut prologue: Option<&mut Vec<Span<Statement>>>,
    ) -> Result<Parameters, ResolveError> {
        if matches!(params, ParameterList::Empty)
            && !self.types.features().empty_parens_are_prototype
        {
            return Ok(Parameters::Unprototyped);
        }
        let mut fixed = Vec::new();
        for parameter in params.parameters() {
            let attributes = || {
                parameter
                    .specifiers
                    .attributes_with(&parameter.declarator, &parameter.attributes)
            };
            self.types
                .check_attributes(attributes(), Subject::Parameter)
                .map_err(ResolveError::checked)?;
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
                self.capture_extents(
                    Some(&parameter.specifiers.ty),
                    &parameter.declarator,
                    &anchor,
                    prologue,
                )?;
            }
            let owner = self.types.owner.replace(parameter.derive(()));
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
            self.types.owner = owner;
            let promoted = self.types.promoted_parameters.get(&parameter.id).copied();
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
                let initializer = self.converted_at(parameter, Slot::Parameter, passed)?;
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
            for attribute in &item.specifiers.attributes {
                self.warn(
                    Warning::IgnoredAttributes,
                    "attribute ignored; place it after the tag keyword to apply it to the type",
                    attribute,
                );
            }
            if !self.types.declare_forward_tag(&item.specifiers) {
                self.resolve_type(&item.specifiers, &Declarator::Abstract)?;
            }
        }
        let storage_class = item.specifiers.storage;
        let inferred = matches!(item.specifiers.ty, ast::TypeSpecifier::Inferred);
        if inferred && self.types.flavor().is_gcc() && item.declarators.len() > 1 {
            return Err(ResolveError::Internal(
                "'auto' may only be used with a single declarator",
            ));
        }
        let mut deduced = None;
        let mut statements = Vec::new();
        for (index, declarator) in item.declarators.iter().enumerate() {
            let attributes = item
                .specifiers
                .attributes_with(&declarator.declarator, &declarator.attributes);
            if storage_class == StorageClass::Typedef {
                self.types
                    .check_attributes(attributes.clone(), Subject::Typedef)
                    .map_err(ResolveError::checked)?;
            }
            let thread = item.specifiers.is_thread_local
                || attributes
                    .clone()
                    .any(|attribute| matches!(&attribute.value, ast::Attribute::ThreadLocal));
            let name = declarator
                .declarator
                .name()
                .ok_or(ResolveError::Internal("unnamed declaration"))?;
            if !global {
                let anchor = declarator.derive(());
                self.capture_extents(
                    (index == 0).then_some(&item.specifiers.ty),
                    &declarator.declarator,
                    &anchor,
                    &mut statements,
                )?;
            }
            let owner = self.types.owner.replace(declarator.derive(()));
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
                    return Err(ResolveError::Internal(
                        "'auto' deduced as different types in one declaration",
                    ));
                }
                deduced = Some(placeholder);
                self.types.inferred = Some(base);
                Some(value)
            } else {
                None
            };
            let recorded = self
                .types
                .declarator_types
                .get(&declarator.id)
                .cloned()
                .ok_or(ResolveError::Internal(
                    "declarator type not recorded by the checker",
                ))??;
            let resolved = if self.types.ctypes.has_unbound_extent(recorded) {
                self.resolve_declarator_type(
                    &item.specifiers,
                    &declarator.declarator,
                    &declarator.attributes,
                )?
            } else {
                recorded
            };
            let resolved = match self.declaration_id(declarator.id, name) {
                Ok(id) => self.types.inherit_convention(id, resolved),
                Err(_) => resolved,
            };
            if let Some(value) = value {
                self.types
                    .check_inferred(resolved, value)
                    .map_err(ResolveError::checked)?;
            }
            let mut c_entries = self.types.render(resolved).entries();
            if item.specifiers.storage == StorageClass::Typedef {
                let alias = self.types.define_alias(
                    declarator.id,
                    name.into(),
                    resolved,
                    attributes.clone(),
                )?;
                self.types.owner = owner;
                self.alias_annotations.insert(alias, c_entries);
                continue;
            }
            self.types.owner = owner;
            let qualifiers = self.types.ctypes.quals(resolved);
            if item.specifiers.is_constexpr && declarator.initializer.is_none() {
                return Err(ResolveError::Internal(
                    "constexpr object requires an initializer",
                ));
            }
            let ty = match self.types.layout(resolved) {
                Some(ty) => ty,
                None if storage_class == StorageClass::Extern => self.types.ir_type(resolved),
                None => return Err(ResolveError::Internal("object cannot have type void")),
            };
            let id = self.declaration_id(declarator.id, name)?;
            self.types
                .entities
                .declare(id, resolved, storage_class == StorageClass::Register);
            if let Type::Function {
                return_type,
                parameters: parameter_types,
                variadic,
                prototyped,
                ..
            } = &ty
            {
                if declarator.initializer.is_some() {
                    return Err(ResolveError::Internal("function initializer"));
                }
                if thread {
                    return Err(ResolveError::Internal("thread-local function"));
                }
                if !global && storage_class == StorageClass::Static {
                    return Err(ResolveError::Internal("block scope static function"));
                }
                let attributes = item
                    .specifiers
                    .attributes_with(&declarator.declarator, &declarator.attributes)
                    .collect::<Vec<_>>();
                self.types
                    .check_attributes(attributes.iter().copied(), Subject::Function)
                    .map_err(ResolveError::checked)?;
                let mut symbol =
                    function_symbol(attributes.iter().copied(), declarator.asm_label.as_ref())
                        .map_err(ResolveError::checked)?;
                if attributes.iter().copied().any(is_bare_weakref) {
                    self.bare_weakrefs.insert(id);
                }
                self.types.pragmas.apply(name, &mut symbol);
                self.record_function(id, &item.specifiers, &attributes, false, global)
                    .map_err(ResolveError::checked)?;
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
                let abi = self.declaration_abi(resolved, &ty)?;
                let lowered = declarator.derive(Function {
                    id,
                    name: name.into(),
                    parameters,
                    return_type: return_type.as_ref().map(|ty| (**ty).clone()),
                    abi,
                    linkage: linkage(storage_class).map_err(ResolveError::checked)?,
                    symbol,
                    semantics: Default::default(),
                    body: None,
                    fallthrough: None,
                });
                self.module.annotate(&lowered, c_entries);
                self.redeclared(declarator.id, id)?;
                self.declare_function(lowered)?;
                continue;
            }
            let linked = global || storage_class == StorageClass::Extern;
            let storage = if !linked && storage_class != StorageClass::Static {
                if thread {
                    return Err(ResolveError::Internal("thread-local automatic variable"));
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
            self.types
                .check_attributes(attributes.clone(), subject)
                .map_err(ResolveError::checked)?;
            let attributes = || attributes.clone().filter(|a| applies(a, subject));
            if let Some(metadata) = self.c_attribute_metadata(attributes())? {
                c_entries.push(metadata);
            }
            if !global && linked && declarator.initializer.is_some() {
                return Err(ResolveError::Internal("block scope extern initializer"));
            }
            let mut symbol = symbol_attributes(attributes(), declarator.asm_label.as_ref())
                .map_err(ResolveError::checked)?;
            if attributes().any(is_bare_weakref) {
                self.bare_weakrefs.insert(id);
            }
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
                        return Err(ResolveError::Internal("variable length array initializer"));
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
                    let value = self.initializer_value(
                        declarator.id,
                        resolved,
                        InitializerSource::Initializer(initializer),
                        &anchor,
                    );
                    self.context.region = region;
                    let mut value = value?;
                    if self.types.flavor().is_msvc()
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
                self.types.constants.insert(
                    id,
                    super::operand::Operand {
                        value: value.clone(),
                        c: resolved,
                    },
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
                cleanup: attributes()
                    .find_map(|attribute| match &attribute.value {
                        ast::Attribute::Cleanup(function) => Some(self.reference(function)),
                        _ => None,
                    })
                    .transpose()?,
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
                return Err(ResolveError::Internal(
                    "variable length array with static storage duration",
                ));
            }
            if variable.is_const && !variable.access.volatile {
                match &variable.initializer {
                    Some(value) => self.types.entities.record_constant(id, value.clone()),
                    None if storage != StorageDuration::Automatic
                        && storage_class != StorageClass::Extern
                        && self.types.flavor().is_gcc() =>
                    {
                        let zero = ValueKind::Aggregate {
                            members: Vec::new(),
                            zero_fill: true,
                        };
                        self.types.entities.record_tentative_constant(
                            id,
                            Value {
                                ty: variable.ty.clone(),
                                node: declarator.derive(zero),
                            },
                        );
                    }
                    None => {}
                }
            }
            if linked {
                let declared_linkage = if storage_class == StorageClass::Register {
                    Linkage::External
                } else if item.specifiers.is_constexpr {
                    Linkage::Internal
                } else {
                    linkage(storage_class).map_err(ResolveError::checked)?
                };
                if symbol.weakref.is_some() && !matches!(declared_linkage, Linkage::Internal) {
                    return Err(ResolveError::Internal("weakref without internal linkage"));
                }
                if symbol.selectany && !matches!(declared_linkage, Linkage::External) {
                    return Err(ResolveError::Internal("selectany without external linkage"));
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
                self.redeclared(declarator.id, id)?;
                self.declare_global(global)?;
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
        specifier: Option<&ast::TypeSpecifier>,
        declarator: &Declarator,
        anchor: &Span<()>,
        out: &mut Vec<Span<Statement>>,
    ) -> Result<(), ResolveError> {
        let mut extents = Vec::new();
        if let Some(specifier) = specifier {
            self.typeof_evaluations(specifier, &mut extents)?;
        }
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
                let count = self.expr(expr)?;
                let count = self.converted(expr, Slot::Extent, count)?;
                let extent_type = count.c;
                let id = match self.reserved_extents.remove(&expr.id) {
                    Some(id) => id,
                    None => self.fresh(),
                };
                self.types.entities.declare(id, extent_type, false);
                self.types.extents.insert(expr.id, id);
                out.push((id, count.value));
                Ok(())
            }
        }
    }

    pub(super) fn compound<T>(
        &mut self,
        lower: impl FnOnce(&mut Self) -> Result<T, ResolveError>,
    ) -> Result<T, ResolveError> {
        let region = self.context.region;
        self.compound_start = true;
        let result = lower(self);
        self.context.region = region;
        result
    }

    fn case_value(
        &mut self,
        label: &Stmt,
        slot: Slot,
        expr: &ast::Expr,
    ) -> Result<Value, ResolveError> {
        let value = self.expr(expr)?;
        let value = self.converted_at(label, slot, value)?;
        let Some(number) = super::fold::integer_constant(&value, self.types.fold_target()) else {
            self.types.constant_integer(expr)?;
            return Err(ResolveError::Internal("nonconstant case expression"));
        };
        Ok(self.value(
            expr,
            value.ty.clone(),
            ValueKind::Constant(super::fold::integer_number(&value.ty, number)),
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
        let result = self.statements(std::slice::from_ref(body), return_type);
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
            self.statement(statement, return_type, &mut result)
                .map_err(|error| error.at(statement.expansion))?;
        }
        Ok(result)
    }

    fn statement(
        &mut self,
        statement: &Stmt,
        return_type: Option<QualType>,
        result: &mut Vec<Span<Statement>>,
    ) -> Result<(), ResolveError> {
        match &statement.value {
            StmtKind::Pragma(pragma) => {
                let placement = if self.compound_start {
                    PragmaPlacement::CompoundStart
                } else {
                    PragmaPlacement::Misplaced
                };
                self.floating_pragmas
                    .apply(&mut self.context.region, &pragma.kind, placement)?;
                return Ok(());
            }
            StmtKind::Comment(_) => return Ok(()),
            _ => self.compound_start = false,
        }
        let mut annotations = Vec::new();
        let kind = match &statement.value {
            StmtKind::Comment(_) | StmtKind::StaticAssert(_) | StmtKind::Pragma(_) => return Ok(()),
            StmtKind::Decl(item) => {
                result.extend(self.declaration(item, false)?);
                return Ok(());
            }
            StmtKind::Expr(expr) => Statement::Expression(self.expr(expr)?.value),
            StmtKind::Return(expr) => {
                let value = self.expr(expr)?;
                match return_type {
                    Some(ty) => Statement::Return(Some(
                        self.convert_recorded(expr, value, ty, ConversionReason::Return)?
                            .value,
                    )),
                    None if self.types.ctypes.is_void(value.c) => {
                        result.push(statement.derive(Statement::Expression(value.value)));
                        Statement::Return(None)
                    }
                    None => {
                        return Err(ResolveError::Internal("value return from void function"));
                    }
                }
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
                    then_body: self.statements(std::slice::from_ref(then_branch), return_type)?,
                    else_body: else_branch
                        .as_ref()
                        .map(|body| self.statements(std::slice::from_ref(body), return_type))
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
            } => {
                let id = self.fresh();
                let init = match init {
                    Some(init) => self.statements(std::slice::from_ref(init), return_type)?,
                    None => Vec::new(),
                };
                let condition = condition
                    .as_ref()
                    .map(|expr| {
                        let value = self.expr(expr)?;
                        self.condition(value.value, None)
                    })
                    .transpose()?;
                let increment = increment.as_ref().map(|expr| self.expr(expr)).transpose()?;
                let body = self.loop_body(id, body, return_type)?;
                Statement::For {
                    id,
                    init,
                    condition: condition.map(Into::into),
                    increment: increment.map(|value| value.value.into()),
                    body,
                }
            }
            StmtKind::Break => Statement::Break(
                *self
                    .break_targets
                    .last()
                    .ok_or(ResolveError::Internal("break outside loop or switch"))?,
            ),
            StmtKind::Continue => Statement::Continue(
                *self
                    .continue_targets
                    .last()
                    .ok_or(ResolveError::Internal("continue outside loop"))?,
            ),
            StmtKind::Switch { discriminant, body } => {
                let value = self.expr(discriminant)?;
                let discriminant = self.converted_at(statement, Slot::Discriminant, value)?;
                if !matches!(discriminant.ty, Type::Numeric(NumericType::Integer { .. })) {
                    return Err(ResolveError::Internal("noninteger switch discriminant"));
                }
                let id = self.fresh();
                self.break_targets.push(id);
                self.switches.push(id);
                let body = self.statements(std::slice::from_ref(body), return_type);
                self.switches.pop();
                self.break_targets.pop();
                Statement::Switch {
                    id,
                    discriminant: discriminant.value,
                    body: body?,
                }
            }
            StmtKind::SwitchLabel { label, body } => {
                let switch = *self
                    .switches
                    .last()
                    .ok_or(ResolveError::Internal("case or default outside switch"))?;
                match label {
                    ast::SwitchLabel::Default => Statement::Default {
                        switch,
                        body: self.statements(std::slice::from_ref(body), return_type)?,
                    },
                    ast::SwitchLabel::Case(start) => Statement::Case {
                        switch,
                        start: self.case_value(statement, Slot::CaseStart, start)?,
                        end: None,
                        body: self.statements(std::slice::from_ref(body), return_type)?,
                    },
                    ast::SwitchLabel::CaseRange { start, end } => Statement::Case {
                        switch,
                        start: self.case_value(statement, Slot::CaseStart, start)?,
                        end: Some(self.case_value(statement, Slot::CaseEnd, end)?),
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
                if !self.switches.is_empty() {
                    annotations.push(("c_attribute".into(), "fallthrough".into()));
                } else if !self.types.flavor().is_gcc() {
                    return Err(ResolveError::Internal("fallthrough outside switch"));
                }
                Statement::Null
            }
            StmtKind::Attribute(_) => Statement::Null,
            StmtKind::LocalLabelDecl(_) => return Ok(()),
            StmtKind::ComputedGoto(expr) => {
                let value = self.expr(expr)?;
                if !matches!(value.ty, Type::Pointer { .. }) {
                    return Err(ResolveError::Internal("nonpointer computed goto"));
                }
                Statement::ComputedGoto(value.value)
            }
            StmtKind::Goto(label) => Statement::Goto(
                self.names
                    .references
                    .iter()
                    .find(|r| r.id == label.id)
                    .map(|r| r.binding)
                    .ok_or(ResolveError::Internal("missing goto binding"))?,
            ),
            StmtKind::Labeled { label, body } => Statement::Label {
                id: *self
                    .names
                    .label_definitions
                    .get(&label.id)
                    .ok_or(ResolveError::Internal("missing label binding"))?,
                name: label.value.clone(),
                body: self.statements(std::slice::from_ref(body), return_type)?,
            },
            StmtKind::Asm(asm) => Statement::Asm(Box::new(self.asm_statement(asm)?)),
            StmtKind::MsAsm(asm) => Statement::Asm(Box::new(self.ms_asm(asm)?)),
            StmtKind::Block(body) => {
                Statement::Block(self.compound(|lower| lower.statements(body, return_type))?)
            }
            StmtKind::ReturnVoid => match self.types.flavor() {
                CompilerFlavor::Msvc => Statement::Return(None),
                CompilerFlavor::Gcc if self.types.features().valueless_return_in_nonvoid => {
                    Statement::Return(None)
                }
                _ => {
                    return Err(ResolveError::Internal(
                        "non-void function should return a value",
                    ));
                }
            },
            StmtKind::Attributed { attributes, body } => {
                if !self.types.flavor().is_gcc()
                    && attributes
                        .iter()
                        .any(|a| matches!(&a.value, ast::Attribute::Fallthrough))
                {
                    return Err(ResolveError::Internal(
                        "fallthrough attribute on a non-empty statement",
                    ));
                }
                result.extend(self.statements(std::slice::from_ref(body), return_type)?);
                return Ok(());
            }
            StmtKind::NestedFunction(_) => {
                return Err(if self.types.flavor().is_gcc() {
                    ResolveError::Unimplemented("GNU nested function")
                } else {
                    ResolveError::Internal("function definition is not allowed here")
                });
            }
        };
        let lowered = statement.derive(kind);
        self.module.annotate(&lowered, annotations);
        result.push(lowered);
        Ok(())
    }
}
