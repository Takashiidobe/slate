use super::error::{PPErrorKind, PPFailure};
use super::syntax::{Directive, identifier};
use super::{MacroDef, Preprocessor};
use crate::ast::{Condition, Conditional};
use crate::lexer::Token;

impl Preprocessor<'_> {
    pub(super) fn record_define(
        &mut self,
        directive: &Directive,
        condition: &Condition,
    ) -> Result<(), PPFailure> {
        let src = self.source(directive.loc.file);
        let expected_name =
            || PPFailure::at(directive.loc, PPErrorKind::ExpectedMacroName("#define"));
        let (name_token, rest) = directive
            .arguments
            .split_first()
            .ok_or_else(expected_name)?;
        let name = identifier(src, name_token).ok_or_else(expected_name)?;
        let name_end = name_token.spelling.offset + name_token.spelling.length;
        let (parameters, variadic, replacement) = match rest.first() {
            Some(open) if open.value == Token::LParen && open.spelling.offset == name_end => {
                let close = rest
                    .iter()
                    .position(|token| token.value == Token::RParen)
                    .ok_or_else(|| {
                        PPFailure::at(directive.loc, PPErrorKind::ExpectedParametersClose)
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
            provenance: self.provenance(directive.loc),
            order: self.macro_order,
        };
        self.macros
            .entry(name)
            .or_insert_with(|| Conditional {
                branches: Vec::new(),
            })
            .branches
            .push((condition.clone(), definition));
        self.macro_order += 1;
        Ok(())
    }
}
