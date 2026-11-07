use super::expand::PPToken;
use super::{MacroDef, Preprocessor, tokens_source};
use crate::ast::{
    self, Loc, MacroDefinition, MacroDefinitionId, MacroExpansion, MacroExpansionId,
    MacroInvocations, MacroNesting, Provenance,
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
    open_arguments: Vec<MacroExpansionId>,
}

pub(super) struct Recorded<'r> {
    pub(super) name: &'r str,
    pub(super) definition: &'r Rc<MacroDef>,
    pub(super) provenance: Provenance,
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
        std::mem::take(&mut self.invocations.table)
    }

    fn records_invocation_of(&self, name: &PPToken) -> bool {
        !self.invocations.in_directive
            && self.outermost_system_header.is_none()
            && name.token.macro_origin.is_none()
    }

    pub(super) fn record_object_expansion(
        &mut self,
        name: &PPToken,
        recorded: &Recorded,
    ) -> Option<Arc<MacroNesting>> {
        self.records_invocation_of(name).then(|| {
            self.record_expansion(
                name,
                recorded,
                Some(ast::SpelledCall {
                    site: name.token.expansion,
                    arguments: Vec::new(),
                }),
            )
        })
    }

    pub(super) fn record_function_expansion(
        &mut self,
        name: &PPToken,
        call: SpelledCall,
        recorded: &Recorded,
    ) -> Option<Arc<MacroNesting>> {
        let site = name.token.expansion;
        let close = call.rparen.token.expansion;
        if self.records_invocation_of(name)
            && call.is_spelled_in_source()
            && site.file == close.file
        {
            let spelled = ast::SpelledCall {
                site: site.through(close),
                arguments: call.argument_ranges(),
            };
            return Some(self.record_expansion(name, recorded, Some(spelled)));
        }
        let origin = name.token.macro_origin.as_ref()?;
        if self.invocations.in_directive
            || self.outermost_system_header.is_some()
            || name.token.provenance.system_header.is_some()
            || recorded.provenance.system_header.is_some()
            || call.lparen.token.macro_origin.as_ref() != Some(origin)
            || call.rparen.token.macro_origin.as_ref() != Some(origin)
        {
            return None;
        }
        Some(self.record_expansion(name, recorded, None))
    }

    fn record_expansion(
        &mut self,
        name: &PPToken,
        recorded: &Recorded,
        spelled: Option<ast::SpelledCall>,
    ) -> Arc<MacroNesting> {
        let enclosing = name.token.macro_nesting.clone();
        let parent = enclosing
            .iter()
            .flat_map(|nesting| nesting.iter())
            .find(|link| link.argument.is_none())
            .map(|link| link.expansion)
            .or_else(|| self.invocations.open_arguments.last().copied());
        let definition = self.invocations.definition_id(recorded);
        let expansion = MacroExpansionId(self.invocations.table.expansions.len() as u32);
        self.invocations.table.expansions.push(MacroExpansion {
            definition,
            parent,
            spelled,
            empty: false,
            cover: None,
            argument_uses: Vec::new(),
        });
        Arc::new(MacroNesting {
            expansion,
            argument: None,
            enclosing,
        })
    }

    pub(super) fn mark_expansion_empty(&mut self, id: MacroExpansionId) {
        self.invocations.table.expansions[id.0 as usize].empty = true;
    }

    pub(super) fn enter_argument_of(&mut self, id: Option<MacroExpansionId>) {
        if let Some(id) = id {
            self.invocations.open_arguments.push(id);
        }
    }

    pub(super) fn leave_argument_of(&mut self, id: Option<MacroExpansionId>) {
        if id.is_some() {
            self.invocations.open_arguments.pop();
        }
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
            comments: recorded.definition.comments.clone(),
        });
        self.definitions
            .insert(key, (id, recorded.definition.clone()));
        id
    }
}
