use super::expression::Lowerer;
use super::numeric::ResolveError;
use crate::ast::{Attribute, DeclarationSpecifiers, Declarator, StorageClass};
use crate::compiler_options::InlineSemantics;
use crate::ir::{BindingId, Fallthrough, FunctionSemantics, Inlining, Linkage};

#[derive(Default)]
pub(super) struct FunctionDeclarations {
    inlining: Option<Inlining>,
    semantics_override: Option<InlineSemantics>,
    definition: Option<DefinitionSpecifiers>,
    has_external_declaration: bool,
    noreturn: bool,
    attributes: Vec<Attribute>,
}

enum DefinitionSpecifiers {
    Ordinary,
    Inline,
    ExternInline,
}

pub(super) fn attributes<'a>(
    specifiers: &'a DeclarationSpecifiers,
    declarator: &'a Declarator,
    trailing: &'a [Attribute],
) -> Vec<&'a Attribute> {
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
    pub(super) fn record_function(
        &mut self,
        id: BindingId,
        specifiers: &DeclarationSpecifiers,
        attributes: &[&Attribute],
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
            match attribute {
                Attribute::GnuInline => {
                    state.semantics_override = Some(InlineSemantics::SupressDef)
                }
                Attribute::AlwaysInline | Attribute::NoInline => {
                    let preference = if matches!(attribute, Attribute::AlwaysInline) {
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
                _ => {}
            }
            if !state.attributes.contains(attribute) {
                state.attributes.push((*attribute).clone());
            }
        }
        Ok(())
    }

    pub(super) fn finish_functions(&mut self, mode: InlineSemantics) {
        for function in &mut self.module.functions {
            let Some(state) = self.function_declarations.get(&function.value.id) else {
                continue;
            };
            let mode = state.semantics_override.unwrap_or(mode);
            function.value.semantics = FunctionSemantics {
                inlining: state.inlining,
                inline_only: function.body.is_some()
                    && matches!(function.linkage, Linkage::External)
                    && match (&state.definition, mode) {
                        (Some(DefinitionSpecifiers::ExternInline), InlineSemantics::SupressDef) => {
                            true
                        }
                        (Some(DefinitionSpecifiers::Inline), InlineSemantics::ProvideDef) => {
                            !state.has_external_declaration
                        }
                        _ => false,
                    },
                noreturn: state.noreturn,
            };
            if state.noreturn && function.body.is_some() {
                function.value.fallthrough = Some(Fallthrough::Undefined);
            }
            if !state.attributes.is_empty() {
                self.module
                    .metadata
                    .entry(function.id)
                    .or_default()
                    .push(("c_attributes".into(), format!("{:?}", state.attributes)));
            }
        }
    }
}
