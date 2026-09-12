use super::error::{PPErrorKind, PPFailure};
use super::syntax::{Directive, identifier};
use super::{MacroDef, MacroEntry, Preprocessor};
use crate::ast::{Condition, Conditional, Span};
use crate::lexer::Token;

impl Preprocessor<'_> {
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
        let entry = MacroEntry {
            definition,
            provenance: self.provenance(directive.loc),
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
