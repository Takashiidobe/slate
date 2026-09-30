use super::error::{PPErrorKind, PPFailure};
use super::syntax::{Directive, identifier};
use super::{MacroDef, MacroEntry, Preprocessor};
use crate::ast::Span;
use crate::lexer::Token;

impl Preprocessor<'_> {
    pub(super) fn push_macro(&mut self, directive: &Directive) {
        let Some(name) = pragma_macro_name(directive) else {
            return;
        };
        let saved = self.macros.get(&name).cloned();
        self.pushed_macros.entry(name).or_default().push(saved);
    }

    pub(super) fn pop_macro(&mut self, directive: &Directive) {
        let Some(name) = pragma_macro_name(directive) else {
            return;
        };
        let Some(saved) = self.pushed_macros.get_mut(&name).and_then(Vec::pop) else {
            return;
        };
        match saved {
            Some(entry) => {
                self.macros.insert(name, entry);
            }
            None => {
                self.macros.remove(&name);
            }
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

    pub(super) fn record_define(&mut self, directive: &Directive) -> Result<(), PPFailure> {
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
        let mut replacement = replacement.to_vec();
        replacement.dedup_by(|next, previous| {
            next.value == Token::HashHash && previous.value == Token::HashHash
        });
        let definition = MacroDef {
            parameters,
            variadic,
            replacement,
        };
        let provenance = self.provenance(directive.loc);
        self.macros.insert(
            name,
            MacroEntry {
                definition,
                provenance,
            },
        );
        Ok(())
    }

    pub(super) fn record_undef(&mut self, directive: &Directive) -> Result<(), PPFailure> {
        let (name, _) = self.macro_name(directive, "#undef")?;
        self.macros.remove(&name);
        Ok(())
    }
}

fn pragma_macro_name(directive: &Directive) -> Option<String> {
    match directive.arguments.get(1..4)? {
        [open, name, close] if open.value == Token::LParen && close.value == Token::RParen => {
            match &name.value {
                Token::StringLit(name) => Some(name.to_string()),
                _ => None,
            }
        }
        _ => None,
    }
}
