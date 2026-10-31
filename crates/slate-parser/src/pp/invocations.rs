use super::expand::PPToken;
use super::{MacroDef, Preprocessor, tokens_source};
use crate::ast::{
    Loc, MacroDefinition, MacroDefinitionId, MacroInvocation, MacroInvocations, Provenance,
};
use crate::lexer::TokenSpanExt;
use std::collections::HashMap;
use std::rc::Rc;
use std::sync::Arc;

#[derive(Default)]
pub(super) struct InvocationRecorder {
    table: MacroInvocations,
    definitions: HashMap<*const MacroDef, (MacroDefinitionId, Rc<MacroDef>)>,
    pub(super) in_directive: bool,
}

pub(super) struct Recorded<'r> {
    pub(super) name: &'r str,
    pub(super) definition: &'r Rc<MacroDef>,
    pub(super) provenance: Provenance,
    pub(super) empty: bool,
}

pub(super) struct SpelledCall<'c> {
    pub(super) lparen: &'c PPToken,
    pub(super) arguments: &'c [Vec<PPToken>],
    pub(super) commas: &'c [PPToken],
    pub(super) rparen: &'c PPToken,
}

impl SpelledCall<'_> {
    fn is_spelled_in_source(&self) -> bool {
        [self.lparen, self.rparen]
            .into_iter()
            .chain(self.arguments.iter().flatten())
            .all(|token| token.token.macro_origin.is_none())
    }

    fn argument_ranges(&self) -> Vec<Loc> {
        let mut preceding = self.lparen.token.expansion;
        let mut ranges = Vec::with_capacity(self.arguments.len());
        for (index, argument) in self.arguments.iter().enumerate() {
            ranges.push(match (argument.first(), argument.last()) {
                (Some(first), Some(last)) => first.token.expansion.through(last.token.expansion),
                _ => Loc::new(preceding.file, preceding.end(), 0),
            });
            if let Some(comma) = self.commas.get(index) {
                preceding = comma.token.expansion;
            }
        }
        ranges
    }
}

impl Preprocessor<'_> {
    pub(crate) fn take_macro_invocations(&mut self) -> MacroInvocations {
        let mut table = std::mem::take(&mut self.invocations.table);
        table.nest();
        table
    }

    fn records_invocation_of(&self, name: &PPToken) -> bool {
        !self.invocations.in_directive
            && self.outermost_system_header.is_none()
            && name.token.macro_origin.is_none()
    }

    pub(super) fn record_object_invocation(&mut self, name: &PPToken, recorded: Recorded) {
        if self.records_invocation_of(name) {
            self.record_invocation(recorded, name.token.expansion, Vec::new());
        }
    }

    pub(super) fn record_function_invocation(
        &mut self,
        name: &PPToken,
        call: SpelledCall,
        recorded: Recorded,
    ) {
        let site = name.token.expansion;
        let close = call.rparen.token.expansion;
        if self.records_invocation_of(name)
            && call.is_spelled_in_source()
            && site.file == close.file
        {
            self.record_invocation(recorded, site.through(close), call.argument_ranges());
        }
    }

    fn record_invocation(&mut self, recorded: Recorded, site: Loc, arguments: Vec<Loc>) {
        let definition = self.invocations.definition_id(&recorded);
        self.invocations.table.invocations.push(MacroInvocation {
            definition,
            site,
            arguments,
            parent: None,
            empty: recorded.empty,
        });
    }
}

impl InvocationRecorder {
    fn definition_id(&mut self, recorded: &Recorded) -> MacroDefinitionId {
        let key = Rc::as_ptr(recorded.definition);
        if let Some((id, _)) = self.definitions.get(&key) {
            return *id;
        }
        let id = MacroDefinitionId(self.table.definitions.len() as u32);
        self.table.definitions.push(MacroDefinition {
            name: Arc::from(recorded.name),
            parameters: recorded.definition.parameters.clone(),
            variadic: recorded.definition.variadic,
            replacement: tokens_source(recorded.definition.replacement.values()),
            provenance: recorded.provenance,
        });
        self.definitions
            .insert(key, (id, recorded.definition.clone()));
        id
    }
}
