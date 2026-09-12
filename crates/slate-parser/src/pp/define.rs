use super::condition::{difference, intersect, is_satisfiable};
use super::error::{PPErrorKind, PPFailure};
use super::syntax::{Directive, identifier};
use super::{MacroDef, MacroEntry, Preprocessor};
use crate::ast::{Condition, Conditional, Provenance, Span};
use crate::lexer::Token;

#[derive(Debug, Clone)]
pub(super) struct PushedMacro {
    on_stack: Condition,
    saved: Vec<(Condition, Option<MacroEntry>)>,
}

impl Preprocessor<'_> {
    pub(super) fn push_macro(&mut self, directive: &Directive, active: &Condition) {
        let Some(name) = pragma_macro_name(directive) else {
            return;
        };
        let saved = self
            .macro_cases(&name, active)
            .into_iter()
            .map(|(condition, entry)| (condition, entry.cloned()))
            .collect();
        self.pushed_macros
            .entry(name)
            .or_default()
            .push(PushedMacro {
                on_stack: active.clone(),
                saved,
            });
    }

    pub(super) fn pop_macro(&mut self, directive: &Directive, active: &Condition) {
        let Some(name) = pragma_macro_name(directive) else {
            return;
        };
        let pop_provenance = self.provenance(directive.loc);
        let Some(stack) = self.pushed_macros.get_mut(&name) else {
            return;
        };
        let mut unmatched = active.clone();
        let mut restores = Vec::new();
        for pushed in stack.iter_mut().rev() {
            let joint = intersect(&unmatched, &pushed.on_stack);
            if !is_satisfiable(&joint) {
                continue;
            }
            pushed.on_stack = difference(&pushed.on_stack, &joint);
            unmatched = difference(&unmatched, &joint);
            restores.extend(
                pushed
                    .saved
                    .iter()
                    .map(|(condition, entry)| (intersect(&joint, condition), entry.clone())),
            );
            if !is_satisfiable(&unmatched) {
                break;
            }
        }
        stack.retain(|pushed| is_satisfiable(&pushed.on_stack));
        for (condition, entry) in restores {
            if !is_satisfiable(&condition) {
                continue;
            }
            let (definition, provenance) = match entry {
                Some(entry) => (entry.definition, entry.provenance),
                None => (None, pop_provenance),
            };
            self.record_entry(name.clone(), definition, provenance, &condition);
        }
    }

    pub(super) fn macro_name<'d>(
        &self,
        directive: &'d Directive,
        directive_name: &'static str,
    ) -> Result<(String, &'d Span<Token>), PPFailure> {
        let Some(token) = directive.arguments.first() else {
            return Err(PPFailure::at(
                directive.name_loc,
                PPErrorKind::ExpectedMacroName(directive_name),
            ));
        };
        identifier(self.source(directive.loc.file), token)
            .map(|name| (name, token))
            .ok_or_else(|| {
                PPFailure::at(
                    token.spelling,
                    PPErrorKind::ExpectedMacroName(directive_name),
                )
            })
    }

    pub(super) fn record_define(
        &mut self,
        directive: &Directive,
        condition: &Condition,
    ) -> Result<(), PPFailure> {
        let (name, name_token) = self.macro_name(directive, "#define")?;
        let src = self.source(directive.loc.file);
        let rest = &directive.arguments[1..];
        let name_end = name_token.spelling.offset + name_token.spelling.length;
        let (parameters, variadic, replacement) = match rest.first() {
            Some(open) if open.value == Token::LParen && open.spelling.offset == name_end => {
                let close = rest
                    .iter()
                    .position(|token| token.value == Token::RParen)
                    .ok_or_else(|| {
                        PPFailure::at(open.spelling, PPErrorKind::ExpectedParametersClose)
                    })?;
                let mut parameters = Vec::new();
                let mut variadic = false;
                for token in &rest[1..close] {
                    match &token.value {
                        Token::Comma => {}
                        Token::Ellipsis => variadic = true,
                        _ => parameters.extend(identifier(src, token)),
                    }
                }
                (Some(parameters), variadic, &rest[close + 1..])
            }
            _ => (None, false, rest),
        };
        let definition = MacroDef {
            parameters,
            variadic,
            replacement: replacement.to_vec(),
        };
        self.record_macro(name, Some(definition), directive, condition);
        Ok(())
    }

    pub(super) fn record_undef(
        &mut self,
        directive: &Directive,
        condition: &Condition,
    ) -> Result<(), PPFailure> {
        let (name, _) = self.macro_name(directive, "#undef")?;
        self.record_macro(name, None, directive, condition);
        Ok(())
    }

    fn record_macro(
        &mut self,
        name: String,
        definition: Option<MacroDef>,
        directive: &Directive,
        condition: &Condition,
    ) {
        let provenance = self.provenance(directive.loc);
        self.record_entry(name, definition, provenance, condition);
    }

    fn record_entry(
        &mut self,
        name: String,
        definition: Option<MacroDef>,
        provenance: Provenance,
        condition: &Condition,
    ) {
        let entry = MacroEntry {
            definition,
            provenance,
            order: self.macro_order,
        };
        self.macros
            .entry(name)
            .or_insert_with(|| Conditional {
                branches: Vec::new(),
            })
            .branches
            .push((condition.clone(), entry));
        self.macro_order += 1;
    }
}

fn pragma_macro_name(directive: &Directive) -> Option<String> {
    match directive.arguments.get(1..4)? {
        [open, name, close] if open.value == Token::LParen && close.value == Token::RParen => {
            match &name.value {
                Token::StringLit(name) => Some(name.clone()),
                _ => None,
            }
        }
        _ => None,
    }
}
