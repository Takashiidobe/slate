use super::builtins::BuiltinAttribute;
use super::expression::Lowerer;
use super::numeric::ResolveError;
use crate::ast::{Attribute, DeclarationSpecifiers, Declarator, Span, StorageClass};
use crate::compiler_args::CompilerFlavor;
use crate::compiler_options::InlineSemantics;
use crate::ir::{BindingId, Fallthrough, FunctionSemantics, Inlining, Linkage, MemoryEffects};
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
    attributes: Vec<Attribute>,
}

impl FunctionDeclarations {
    fn restrict_memory(&mut self, memory: MemoryEffects) {
        self.memory = Some(self.memory.map_or(memory, |current| current.min(memory)));
    }
}

enum DefinitionSpecifiers {
    Ordinary,
    Inline,
    ExternInline,
}

pub(crate) fn attributes<'a>(
    specifiers: &'a DeclarationSpecifiers,
    declarator: &'a Declarator,
    trailing: &'a [Span<Attribute>],
) -> Vec<&'a Span<Attribute>> {
    let mut attributes = specifiers
        .attributes
        .iter()
        .chain(trailing)
        .collect::<Vec<_>>();
    let mut current = Some(declarator);
    while let Some(declarator) = current {
        if let Declarator::Attributed {
            attributes: nested, ..
        } = declarator
        {
            attributes.extend(nested);
        }
        current = match declarator {
            Declarator::Grouped(inner)
            | Declarator::Attributed { inner, .. }
            | Declarator::Pointer { inner, .. }
            | Declarator::Array { inner, .. }
            | Declarator::Function { inner, .. } => Some(inner),
            Declarator::Name(_) | Declarator::Abstract => None,
        };
    }
    attributes
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
        let state = self.function_declarations.entry(id).or_default();
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
                        return Err(ResolveError::Invalid(
                            "conflicting always_inline and noinline attributes",
                        ));
                    }
                    state.inlining = Some(preference);
                }
                Attribute::NoReturn => state.noreturn = true,
                Attribute::Naked => state.naked = true,
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
                let builtin = super::builtins::clang_builtin(&function.name, self.types.flavor)?;
                Some((function.id, function.value.id, builtin))
            })
            .collect();
        for (node, id, builtin) in named_builtins {
            if !self.declares_builtin(id, builtin) {
                if self.types.flavor == CompilerFlavor::Clang
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
            state.noreturn |= builtin.noreturn(self.types.flavor);
            if let Some(memory) = builtin.memory_effects(self.types.flavor) {
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
