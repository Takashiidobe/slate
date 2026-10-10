use super::error::{PPErrorKind, PPFailure};
use super::syntax::{Directive, identifier};
use super::{MacroDef, MacroEntry, Preprocessor};
use crate::ast::{HeaderKind, Span};
use crate::compiler_args::CompilerFlavor;
use crate::diagnostics::Warning;
use crate::lexer::{Token, token_spelling};

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
        if self.is_reserved_macro(&name) {
            self.warn(
                name_token.spelling,
                Warning::BuiltinMacroRedefined,
                format!("macro name '{name}' is reserved, '#define' ignored"),
                true,
            );
            return Ok(());
        }
        let src = self.source(directive.loc.file);
        let rest = &directive.arguments[1..];
        let name_end = name_token.spelling.offset + name_token.spelling.length;
        let (parameters, variadic, named_variadic, replacement) = match rest.first() {
            Some(open) if open.value == Token::LParen && open.spelling.offset == name_end => {
                let close = rest
                    .iter()
                    .position(|token| token.value == Token::RParen)
                    .ok_or_else(|| {
                        PPFailure::at(open.spelling, PPErrorKind::ExpectedParametersClose)
                    })?;
                let list = &rest[1..close];
                let mut parameters = Vec::new();
                let mut variadic = false;
                let mut named_variadic = None;
                for (index, token) in list.iter().enumerate() {
                    match &token.value {
                        Token::Comma => {}
                        Token::Ellipsis => variadic = true,
                        _ if list
                            .get(index + 1)
                            .is_some_and(|next| next.value == Token::Ellipsis) =>
                        {
                            named_variadic = identifier(src, token);
                        }
                        _ => parameters.extend(identifier(src, token)),
                    }
                }
                (
                    Some(parameters),
                    variadic,
                    named_variadic,
                    &rest[close + 1..],
                )
            }
            _ => (None, false, None, rest),
        };
        let mut replacement = replacement.to_vec();
        if let Some(named) = named_variadic {
            for token in &mut replacement {
                if identifier(src, token).as_deref() == Some(named.as_str()) {
                    token.value = Token::Ident("__VA_ARGS__".into());
                }
            }
        }
        replacement.dedup_by(|next, previous| {
            next.value == Token::HashHash && previous.value == Token::HashHash
        });
        let definition = MacroDef {
            parameters,
            variadic,
            replacement,
            builtin: false,
        };
        if let Some((warning, message)) = self.redefinition_warning(&name, Some(&definition)) {
            self.warn(name_token.spelling, warning, message, true);
        }
        let provenance = self.provenance(directive.loc);
        self.macros.insert(
            name,
            MacroEntry {
                definition: std::rc::Rc::new(definition),
                provenance,
            },
        );
        Ok(())
    }

    pub(super) fn record_undef(&mut self, directive: &Directive) -> Result<(), PPFailure> {
        let (name, name_token) = self.macro_name(directive, "#undef")?;
        if self.is_reserved_macro(&name) {
            self.warn(
                name_token.spelling,
                Warning::BuiltinMacroRedefined,
                format!("macro name '{name}' is reserved, '#undef' ignored"),
                true,
            );
            return Ok(());
        }
        if let Some((warning, message)) = self.redefinition_warning(&name, None) {
            let pedantic = !self.dialect.flavor().is_gcc();
            self.warn(name_token.spelling, warning, message, pedantic);
        }
        self.macros.remove(&name);
        Ok(())
    }

    fn redefinition_warning(
        &self,
        name: &str,
        definition: Option<&MacroDef>,
    ) -> Option<(Warning, String)> {
        let flavor = self.dialect.flavor();
        let previous = self.macros.get(name);
        let operator = super::is_defined_operator(name, flavor);
        if previous.is_none() && !operator {
            return None;
        }
        let builtin = operator || previous.is_some_and(|entry| entry.definition.builtin);
        let differs = |positional_parameters| match (previous, definition) {
            (Some(previous), Some(definition)) => {
                !same_definition(&previous.definition, definition, positional_parameters)
            }
            _ => false,
        };
        match flavor {
            CompilerFlavor::Gcc => {
                let message = match definition {
                    Some(_) => format!("'{name}' redefined"),
                    None => format!("undefining '{name}'"),
                };
                if builtin && GCC_NAMED_BUILTIN_MACROS.contains(&name) {
                    Some((Warning::BuiltinMacroRedefined, message))
                } else if builtin || gcc_always_warns(name) || differs(false) {
                    Some((Warning::MacroRedefined, message))
                } else {
                    None
                }
            }
            CompilerFlavor::Clang => {
                let predefined = previous.is_some_and(|entry| {
                    entry.provenance.kind == HeaderKind::System
                        && self
                            .files
                            .path(entry.provenance.file)
                            .to_str()
                            .is_some_and(|path| path.starts_with('<'))
                });
                if builtin || predefined && name.starts_with("__STDC") {
                    let verb = if definition.is_some() {
                        "redefining"
                    } else {
                        "undefining"
                    };
                    Some((
                        Warning::BuiltinMacroRedefined,
                        format!("{verb} builtin macro"),
                    ))
                } else if differs(false) {
                    Some((Warning::MacroRedefined, format!("'{name}' macro redefined")))
                } else {
                    None
                }
            }
            CompilerFlavor::Msvc => differs(true).then(|| {
                (
                    Warning::MacroRedefined,
                    format!("'{name}': macro redefinition"),
                )
            }),
        }
    }
}

const GCC_NAMED_BUILTIN_MACROS: [&str; 6] = [
    "__TIME__",
    "__DATE__",
    "__FILE__",
    "__FILE_NAME__",
    "__BASE_FILE__",
    "__TIMESTAMP__",
];

fn gcc_always_warns(name: &str) -> bool {
    name.starts_with("__STDC_")
        && !matches!(
            name,
            "__STDC_FORMAT_MACROS" | "__STDC_LIMIT_MACROS" | "__STDC_CONSTANT_MACROS"
        )
}

fn same_definition(
    previous: &MacroDef,
    definition: &MacroDef,
    positional_parameters: bool,
) -> bool {
    let parameters_match = match (&previous.parameters, &definition.parameters) {
        (Some(previous), Some(definition)) if positional_parameters => {
            previous.len() == definition.len()
        }
        (previous, definition) => previous == definition,
    };
    let spelling = |token: &Span<Token>, parameters: &Option<Vec<String>>| match &token.value {
        Token::Ident(name) if positional_parameters => parameters
            .iter()
            .flatten()
            .position(|parameter| parameter == name.as_str())
            .map_or_else(|| token_spelling(token), |index| format!("#{index}")),
        _ => token_spelling(token),
    };
    parameters_match
        && previous.variadic == definition.variadic
        && previous.replacement.len() == definition.replacement.len()
        && previous
            .replacement
            .iter()
            .zip(&definition.replacement)
            .enumerate()
            .all(|(index, (old, new))| {
                spelling(old, &previous.parameters) == spelling(new, &definition.parameters)
                    && (index == 0 || old.leading_space == new.leading_space)
            })
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
