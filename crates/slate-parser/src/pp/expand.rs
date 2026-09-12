use super::condition::is_statically_true;
use super::{MacroDef, Preprocessor, lex, tokens_source};
use crate::ast::{Condition, FileId, Loc, Span};
use crate::lexer::{Token, TokenSpanExt};
use std::collections::HashSet;

impl Preprocessor<'_> {
    pub(super) fn strip_pragma_operator(tokens: &[Span<Token>]) -> Vec<Span<Token>> {
        let mut result = Vec::with_capacity(tokens.len());
        let mut i = 0;
        while i < tokens.len() {
            if tokens.value_at(i) == Some(&Token::Ident("_Pragma".to_string()))
                && tokens.value_at(i + 1) == Some(&Token::LParen)
                && matches!(tokens.value_at(i + 2), Some(Token::StringLit(_)))
                && tokens.value_at(i + 3) == Some(&Token::RParen)
            {
                i += 4;
                continue;
            }
            result.push(tokens[i].clone());
            i += 1;
        }
        result
    }

    pub(super) fn expand_macros(
        &self,
        tokens: &[Span<Token>],
        disabled: &mut HashSet<String>,
        active: &Condition,
    ) -> Vec<Span<Token>> {
        let mut expanded = Vec::new();
        let mut i = 0;
        while i < tokens.len() {
            let token = &tokens[i];
            let Token::Ident(name) = &token.value else {
                expanded.push(token.clone());
                i += 1;
                continue;
            };
            let Some(macro_def) = self.macros.get(name).and_then(|conditional| {
                conditional
                    .branches
                    .iter()
                    .rev()
                    .find(|(condition, _)| condition == active || is_statically_true(condition))
                    .map(|(_, definition)| definition.clone())
            }) else {
                expanded.push(token.clone());
                i += 1;
                continue;
            };
            if !disabled.insert(name.clone()) {
                expanded.push(token.clone());
                i += 1;
                continue;
            }
            let Some(parameters) = macro_def.parameters.clone() else {
                let replacement = macro_def
                    .replacement
                    .iter()
                    .cloned()
                    .map(|mut replacement| {
                        replacement.expansion = token.expansion;
                        replacement
                    })
                    .collect::<Vec<_>>();
                expanded.extend(self.expand_macros(&replacement, disabled, active));
                disabled.remove(name);
                i += 1;
                continue;
            };
            let Some((arguments, end)) = invocation_arguments(tokens, i + 1) else {
                disabled.remove(name);
                expanded.push(token.clone());
                i += 1;
                continue;
            };
            if !macro_def.variadic && arguments.len() != parameters.len()
                || macro_def.variadic && arguments.len() < parameters.len()
            {
                disabled.remove(name);
                expanded.push(token.clone());
                i += 1;
                continue;
            }
            let mut macro_def = macro_def;
            for replacement in &mut macro_def.replacement {
                replacement.expansion = token.expansion;
            }
            let replacement = substitute_function_macro(
                &macro_def,
                &parameters,
                &arguments,
                self,
                disabled,
                active,
            );
            expanded.extend(self.expand_macros(&replacement, disabled, active));
            disabled.remove(name);
            i = end;
        }
        expanded
    }

    pub(super) fn divergent_macro_conditions(
        &self,
        tokens: &[Span<Token>],
        active: &Condition,
    ) -> Option<Vec<Condition>> {
        for (index, token) in tokens.iter().enumerate() {
            let Token::Ident(name) = &token.value else {
                continue;
            };
            let Some(conditional) = self.macros.get(name) else {
                continue;
            };
            if conditional
                .branches
                .iter()
                .any(|(condition, _)| condition == active)
            {
                continue;
            }
            if conditional
                .branches
                .iter()
                .all(|(_, definition)| definition.parameters.is_some())
                && tokens.value_at(index + 1) != Some(&Token::LParen)
            {
                continue;
            }
            let mut conditions = Vec::new();
            for (condition, _) in &conditional.branches {
                if !conditions.contains(condition) {
                    conditions.push(condition.clone());
                }
            }
            if conditions.len() > 1 {
                return Some(conditions);
            }
        }
        None
    }
}

fn invocation_arguments(
    tokens: &[Span<Token>],
    start: usize,
) -> Option<(Vec<Vec<Span<Token>>>, usize)> {
    if tokens.value_at(start) != Some(&Token::LParen) {
        return None;
    }
    let mut arguments = Vec::new();
    let mut current = Vec::new();
    let mut depth = 0;
    let mut i = start + 1;
    while i < tokens.len() {
        match &tokens[i].value {
            Token::LParen => {
                depth += 1;
                current.push(tokens[i].clone());
            }
            Token::RParen if depth == 0 => {
                if !current.is_empty() || !arguments.is_empty() {
                    arguments.push(current);
                }
                return Some((arguments, i + 1));
            }
            Token::RParen => {
                depth -= 1;
                current.push(tokens[i].clone());
            }
            Token::Comma if depth == 0 => {
                arguments.push(std::mem::take(&mut current));
            }
            _ => current.push(tokens[i].clone()),
        }
        i += 1;
    }
    None
}

fn substitute_function_macro(
    definition: &MacroDef,
    parameters: &[String],
    arguments: &[Vec<Span<Token>>],
    preprocessor: &Preprocessor<'_>,
    disabled: &mut HashSet<String>,
    active: &Condition,
) -> Vec<Span<Token>> {
    let expanded_arguments = arguments
        .iter()
        .map(|argument| preprocessor.expand_macros(argument, disabled, active))
        .collect::<Vec<_>>();
    let mut output = Vec::new();
    let mut i = 0;
    while i < definition.replacement.len() {
        let token = &definition.replacement[i];
        if token.value == Token::Hash
            && i + 1 < definition.replacement.len()
            && let Token::Ident(name) = &definition.replacement[i + 1].value
        {
            let argument = if name == "__VA_ARGS__" {
                Some(variadic_tokens(arguments, parameters.len()))
            } else {
                macro_argument(name, parameters, arguments).map(<[Span<Token>]>::to_vec)
            };
            if let Some(argument) = argument {
                output.push(
                    token
                        .clone()
                        .with_value(Token::StringLit(tokens_source(argument.values()))),
                );
                i += 2;
                continue;
            }
        }
        if token.value == Token::HashHash && i + 1 < definition.replacement.len() {
            let Some(left) = output.pop() else {
                i += 1;
                continue;
            };
            let right_tokens = replacement_tokens(
                &definition.replacement[i + 1],
                parameters,
                arguments,
                &expanded_arguments,
                false,
            );
            if let Some(right) = right_tokens.first() {
                let pasted = lex(&format!(
                    "{}{}",
                    String::from(&left.value),
                    String::from(&right.value)
                ));
                if pasted.len() == 1 {
                    output.push(Span::new(
                        pasted[0].clone(),
                        left.spelling.through(right.spelling),
                        left.expansion.through(right.expansion),
                    ));
                    output.extend(right_tokens.into_iter().skip(1));
                } else {
                    output.push(left);
                    output.extend(right_tokens);
                }
            } else {
                output.push(left);
            }
            i += 2;
            continue;
        }
        output.extend(replacement_tokens(
            token,
            parameters,
            arguments,
            &expanded_arguments,
            i + 1 >= definition.replacement.len()
                || definition.replacement[i + 1].value != Token::HashHash,
        ));
        i += 1;
    }
    output
}

fn macro_argument<'a>(
    name: &str,
    parameters: &[String],
    arguments: &'a [Vec<Span<Token>>],
) -> Option<&'a [Span<Token>]> {
    if name == "__VA_ARGS__" {
        return None;
    }
    parameters
        .iter()
        .position(|parameter| parameter == name)
        .and_then(|index| arguments.get(index).map(Vec::as_slice))
}

fn replacement_tokens(
    token: &Span<Token>,
    parameters: &[String],
    arguments: &[Vec<Span<Token>>],
    expanded_arguments: &[Vec<Span<Token>>],
    prescan: bool,
) -> Vec<Span<Token>> {
    let Token::Ident(name) = &token.value else {
        return vec![token.clone()];
    };
    if name == "__VA_ARGS__" {
        return variadic_tokens(
            if prescan {
                expanded_arguments
            } else {
                arguments
            },
            parameters.len(),
        );
    }
    parameters
        .iter()
        .position(|parameter| parameter == name)
        .and_then(|index| {
            if prescan {
                expanded_arguments.get(index).cloned()
            } else {
                arguments.get(index).cloned()
            }
        })
        .unwrap_or_else(|| vec![token.clone()])
}

fn variadic_tokens(arguments: &[Vec<Span<Token>>], fixed: usize) -> Vec<Span<Token>> {
    arguments
        .iter()
        .skip(fixed)
        .enumerate()
        .flat_map(|(index, argument)| {
            let separator = (index != 0).then(|| {
                argument.first().cloned().map_or_else(
                    || {
                        let loc = Loc::new(FileId(0), 0, 0);
                        Span::new(Token::Comma, loc, loc)
                    },
                    |token| token.with_value(Token::Comma),
                )
            });
            separator.into_iter().chain(argument.iter().cloned())
        })
        .collect()
}
