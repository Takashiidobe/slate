use super::builtins::BuiltinAttribute;
use super::builtins::ClangBuiltin;
use super::ctype::QualType;
use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::types::TypeResolver;
use crate::ast::{Attribute, DeclarationSpecifiers, Expr, ExprKind, Span, StorageClass};
use crate::compiler_args::CompilerFlavor;
use crate::compiler_options::InlineSemantics;
use crate::ir::{
    BindingId, Deallocator, Fallthrough, FunctionSemantics, Inlining, Linkage, MemoryEffects,
    TargetFeature, Type,
};
use crate::target_info::TargetEnvironment;

#[derive(Default)]
pub(super) struct FunctionDeclarations {
    inlining: Option<Inlining>,
    semantics_override: Option<InlineSemantics>,
    definition: Option<DefinitionSpecifiers>,
    has_external_declaration: bool,
    noreturn: bool,
    naked: bool,
    memory: Option<MemoryEffects>,
    deallocators: Vec<Deallocator>,
    target: Vec<TargetFeature>,
    attributes: Vec<Attribute>,
}

fn target_features(spec: &str) -> Vec<TargetFeature> {
    spec.split(',')
        .map(str::trim)
        .filter_map(|entry| {
            if let Some(arch) = entry.strip_prefix("arch=") {
                Some(TargetFeature::Arch(arch.into()))
            } else if let Some(tune) = entry.strip_prefix("tune=") {
                Some(TargetFeature::Tune(tune.into()))
            } else if let Some(protection) = entry.strip_prefix("branch-protection=") {
                Some(TargetFeature::BranchProtection(protection.into()))
            } else if entry.is_empty() {
                None
            } else if let Some(feature) = entry.strip_prefix("no-") {
                Some(TargetFeature::Disable(feature.into()))
            } else {
                Some(TargetFeature::Enable(entry.into()))
            }
        })
        .collect()
}

impl FunctionDeclarations {
    fn restrict_memory(&mut self, memory: MemoryEffects) {
        self.memory = Some(self.memory.map_or(memory, |current| current.min(memory)));
    }
}

/// An undeclared builtin named as a `malloc` deallocator, with its signature.
pub(super) fn builtin_deallocator(
    types: &mut TypeResolver,
    function: &Expr,
) -> Option<(&'static ClangBuiltin, QualType)> {
    let ExprKind::Identifier(name) = &function.value else {
        return None;
    };
    if types.references.contains_key(&function.id) {
        return None;
    }
    let builtin =
        super::builtins::clang_builtin(name, types.compiler_flavor(), types.target_info().family)?;
    Some((builtin, types.builtin_signature(builtin)?))
}

/// The 0-based parameter of a `malloc` deallocator of type `ty` that takes the pointer.
pub(super) fn deallocator_argument(
    types: &mut TypeResolver,
    ty: QualType,
    argument: Option<&Expr>,
) -> Result<u32, ResolveError> {
    const OUT_OF_BOUNDS: ResolveError =
        ResolveError::Rejected("'malloc' attribute parameter is out of bounds");
    let index = match argument {
        Some(argument) => u32::try_from(types.constant_integer(argument)?)
            .ok()
            .and_then(|position| position.checked_sub(1))
            .ok_or(OUT_OF_BOUNDS)?,
        None => 0,
    };
    match types.ctypes.function_parts(ty) {
        Some((_, parameters, _, true)) => match parameters.get(index as usize) {
            Some(&parameter) if types.ctypes.is_pointer(parameter) => Ok(index),
            Some(_) => Err(ResolveError::Rejected(
                "'malloc' argument refers to a non-pointer parameter",
            )),
            None => Err(OUT_OF_BOUNDS),
        },
        _ => Ok(index),
    }
}

enum DefinitionSpecifiers {
    Ordinary,
    Inline,
    ExternInline,
}

impl Lowerer {
    pub(super) fn record_implicit_function(&mut self, id: BindingId) {
        self.function_declarations
            .entry(id)
            .or_default()
            .has_external_declaration = true;
    }

    pub(super) fn record_function(
        &mut self,
        id: BindingId,
        specifiers: &DeclarationSpecifiers,
        attributes: &[&Span<Attribute>],
        definition: bool,
        file_scope: bool,
    ) -> Result<(), ResolveError> {
        let mut deallocators = Vec::new();
        for attribute in attributes {
            let Attribute::Malloc {
                deallocator: Some(function),
                argument,
            } = &attribute.value
            else {
                continue;
            };
            let (binding, ty) = match builtin_deallocator(&mut self.types, function) {
                Some((builtin, signature)) => (
                    self.builtin_declaration(function, builtin, signature)?,
                    signature,
                ),
                None => (
                    self.reference(function)?,
                    self.types.expression_type(function)?,
                ),
            };
            if let Ok(argument) = deallocator_argument(&mut self.types, ty, argument.as_ref()) {
                deallocators.push(Deallocator {
                    function: binding,
                    argument,
                });
            }
        }
        let state = self.function_declarations.entry(id).or_default();
        for deallocator in deallocators {
            if !state.deallocators.contains(&deallocator) {
                state.deallocators.push(deallocator);
            }
        }
        if specifiers.is_inline && state.inlining.is_none() {
            state.inlining = Some(Inlining::Hint);
        }
        state.noreturn |= specifiers.is_noreturn;
        state.has_external_declaration |=
            file_scope && (!specifiers.is_inline || specifiers.storage == StorageClass::Extern);
        if definition {
            state.definition = Some(if !specifiers.is_inline {
                DefinitionSpecifiers::Ordinary
            } else if specifiers.storage == StorageClass::Extern {
                DefinitionSpecifiers::ExternInline
            } else {
                DefinitionSpecifiers::Inline
            });
        }
        for attribute in attributes {
            match &attribute.value {
                Attribute::GnuInline => {
                    state.semantics_override = Some(InlineSemantics::SupressDef)
                }
                Attribute::AlwaysInline | Attribute::NoInline => {
                    let preference = if matches!(&attribute.value, Attribute::AlwaysInline) {
                        Inlining::Always
                    } else {
                        Inlining::Never
                    };
                    if matches!(
                        (state.inlining, preference),
                        (Some(Inlining::Always), Inlining::Never)
                            | (Some(Inlining::Never), Inlining::Always)
                    ) {
                        return Err(ResolveError::Internal(
                            "conflicting always_inline and noinline attributes",
                        ));
                    }
                    state.inlining = Some(preference);
                }
                Attribute::NoReturn => state.noreturn = true,
                Attribute::Naked => state.naked = true,
                Attribute::Target(spec) => state.target = target_features(spec),
                Attribute::Const => state.restrict_memory(MemoryEffects::None),
                Attribute::Pure => state.restrict_memory(MemoryEffects::Read),
                _ => {}
            }
            if !state.attributes.contains(&attribute.value) {
                state.attributes.push(attribute.value.clone());
            }
        }
        Ok(())
    }

    pub(super) fn is_naked(&self, id: BindingId) -> bool {
        self.function_declarations
            .get(&id)
            .is_some_and(|state| state.naked)
    }

    pub(super) fn finish_functions(&mut self, mode: InlineSemantics) -> Result<(), ResolveError> {
        let named_builtins: Vec<_> = self
            .module
            .functions
            .iter()
            .filter_map(|function| {
                let builtin = super::builtins::clang_builtin(
                    &function.name,
                    self.types.compiler_flavor(),
                    self.types.target_info().family,
                )?;
                Some((function.id, function.value.id, builtin))
            })
            .collect();
        for (node, id, builtin) in named_builtins {
            if !self.types.declares_builtin(id, builtin) {
                if self.types.compiler_flavor() == CompilerFlavor::Clang
                    && builtin.has(BuiltinAttribute::NoReturn)
                    && matches!(self.types.entities.linkage(id), Some(Linkage::External))
                {
                    self.function_declarations.entry(id).or_default().noreturn = true;
                }
                continue;
            }
            self.module
                .metadata
                .entry(node)
                .or_default()
                .push(("c_builtin".into(), builtin.name.into()));
            let state = self.function_declarations.entry(id).or_default();
            state.noreturn |= builtin.noreturn(self.types.compiler_flavor());
            if let Some(memory) = builtin.memory_effects(self.types.compiler_flavor()) {
                state.restrict_memory(memory);
            }
        }
        let retained_attributes: Vec<_> = self
            .function_declarations
            .iter()
            .map(|(id, state)| (*id, state.attributes.clone()))
            .collect();
        let mut rendered_attributes = Vec::new();
        for (id, attributes) in retained_attributes {
            rendered_attributes.push((id, self.render_c_attributes(attributes.iter())?));
        }
        let microsoft_abi = self.context.target.environment == TargetEnvironment::Msvc;
        for function in &mut self.module.functions {
            let Some(state) = self.function_declarations.get(&function.value.id) else {
                continue;
            };
            let mode = match state.semantics_override {
                None if microsoft_abi => None,
                semantics => Some(semantics.unwrap_or(mode)),
            };
            function.value.semantics = FunctionSemantics {
                inlining: state.inlining,
                inline_only: function.body.is_some()
                    && matches!(function.linkage, Linkage::External)
                    && match (&state.definition, mode) {
                        (
                            Some(DefinitionSpecifiers::ExternInline),
                            Some(InlineSemantics::SupressDef),
                        ) => true,
                        (Some(DefinitionSpecifiers::Inline), Some(InlineSemantics::ProvideDef)) => {
                            !state.has_external_declaration
                        }
                        _ => false,
                    },
                noreturn: state.noreturn,
                naked: state.naked,
                memory: state.memory,
                deallocators: if matches!(function.return_type, Some(Type::Pointer { .. })) {
                    state.deallocators.clone()
                } else {
                    Vec::new()
                },
                target: state.target.clone(),
            };
            // a naked function has no epilogue: clang ends its body in `unreachable`.
            if (state.noreturn || state.naked) && function.body.is_some() {
                function.value.fallthrough = Some(Fallthrough::Undefined);
            }
            if let Some(Some(metadata)) = rendered_attributes
                .iter()
                .find(|(id, _)| *id == function.value.id)
                .map(|(_, metadata)| metadata)
            {
                self.module
                    .metadata
                    .entry(function.id)
                    .or_default()
                    .push(metadata.clone());
            }
        }
        Ok(())
    }
}
