use super::condition::{difference, implies, intersect, is_satisfiable, simplify_condition};
use super::{MacroDef, MacroEntry, Preprocessor, lex, tokens_source};
use crate::ast::{Condition, FileId, Loc, Span};
use crate::lexer::{Token, TokenSpanExt};
use std::collections::HashSet;

impl Preprocessor<'_> {
    pub(super) fn visible_entry(&self, name: &str, active: &Condition) -> Option<&MacroEntry> {
        let cases = self.macro_cases(name, active);
        let (_, first) = cases.first()?;
        let definition = first.and_then(|entry| entry.definition.as_ref());
        cases
            .iter()
            .all(|(_, entry)| entry.and_then(|entry| entry.definition.as_ref()) == definition)
            .then_some(*first)
            .flatten()
    }

    fn definition_cases(
        &self,
        name: &str,
        active: &Condition,
    ) -> Vec<(Condition, Option<&MacroDef>)> {
        let mut groups: Vec<(Option<&MacroDef>, Condition)> = Vec::new();
        for (condition, entry) in self.macro_cases(name, active) {
            let definition = entry.and_then(|entry| entry.definition.as_ref());
            match groups
                .iter_mut()
                .find(|(existing, _)| *existing == definition)
            {
                Some((_, merged)) => {
                    *merged = Condition::Or(Box::new(merged.clone()), Box::new(condition));
                }
                None => groups.push((definition, condition)),
            }
        }
        if let [(definition, _)] = groups.as_slice() {
            return vec![(active.clone(), *definition)];
        }
        groups
            .into_iter()
            .map(|(definition, condition)| (simplify_condition(&condition), definition))
            .collect()
    }

    fn refine_cases(
        &self,
        tokens: &[Span<Token>],
        parameters: &[String],
        cases: Vec<Condition>,
        expanding: &mut HashSet<String>,
    ) -> Vec<Condition> {
        let mut cases = cases;
        for (index, token) in tokens.iter().enumerate() {
            let Token::Ident(name) = &token.value else {
                continue;
            };
            if parameters.contains(name) || expanding.contains(name) {
                continue;
            }
            let Some(conditional) = self.macros.get(name) else {
                continue;
            };
            if conditional
                .branches
                .iter()
                .filter_map(|(_, entry)| entry.definition.as_ref())
                .all(|definition| definition.parameters.is_some())
                && tokens.value_at(index + 1) != Some(&Token::LParen)
            {
                continue;
            }
            let mut refined = Vec::new();
            for case in &cases {
                for (condition, definition) in self.definition_cases(name, case) {
                    let Some(definition) = definition else {
                        refined.push(condition);
                        continue;
                    };
                    expanding.insert(name.clone());
                    refined.extend(self.refine_cases(
                        &definition.replacement,
                        definition.parameters.as_deref().unwrap_or_default(),
                        vec![condition],
                        expanding,
                    ));
                    expanding.remove(name);
                }
            }
            cases = refined;
        }
        cases
    }

    pub(super) fn macro_cases(
        &self,
        name: &str,
        active: &Condition,
    ) -> Vec<(Condition, Option<&MacroEntry>)> {
        let mut cases = Vec::new();
        let mut remaining = Some(active.clone());
        let branches = self
            .macros
            .get(name)
            .into_iter()
            .flat_map(|conditional| conditional.branches.iter().rev());
        for (condition, entry) in branches {
            let Some(current) = remaining.take() else {
                break;
            };
            if implies(&current, condition) {
                cases.push((current, Some(entry)));
                break;
            }
            let joint = intersect(&current, condition);
            if !is_satisfiable(&joint) {
                remaining = Some(current);
                continue;
            }
            cases.push((joint, Some(entry)));
            let rest = difference(&current, condition);
            remaining = is_satisfiable(&rest).then_some(rest);
        }
        cases.reverse();
        cases.extend(remaining.map(|condition| (condition, None)));
        cases
    }

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
            let Some(macro_def) = self
                .visible_entry(name, active)
                .and_then(|entry| entry.definition.clone())
            else {
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
        let cases = self.refine_cases(tokens, &[], vec![active.clone()], &mut HashSet::new());
        (cases.len() > 1).then_some(cases)
    }

    pub(super) fn expandable_tokens(tokens: &[Span<Token>]) -> Vec<Span<Token>> {
        let operands = unexpanded_operands(tokens);
        tokens
            .iter()
            .zip(operands)
            .filter(|(_, unexpanded)| !unexpanded)
            .map(|(token, _)| token.clone())
            .collect()
    }

    pub(super) fn expand_condition(
        &self,
        tokens: &[Span<Token>],
        active: &Condition,
    ) -> Vec<Span<Token>> {
        let operands = unexpanded_operands(tokens);
        let mut expanded = Vec::with_capacity(tokens.len());
        let mut start = 0;
        while start < tokens.len() {
            let unexpanded = operands[start];
            let end = operands[start..]
                .iter()
                .position(|&flag| flag != unexpanded)
                .map_or(tokens.len(), |offset| start + offset);
            if unexpanded {
                expanded.extend_from_slice(&tokens[start..end]);
            } else {
                expanded.extend(self.expand_macros(
                    &tokens[start..end],
                    &mut HashSet::new(),
                    active,
                ));
            }
            start = end;
        }
        expanded
    }
}

pub(super) fn defined_operands(tokens: &[Span<Token>]) -> Vec<String> {
    let mut names: Vec<String> = Vec::new();
    for (index, token) in tokens.iter().enumerate() {
        if !matches!(&token.value, Token::Ident(word) if word == "defined") {
            continue;
        }
        let name = match (tokens.value_at(index + 1), tokens.value_at(index + 2)) {
            (Some(Token::Ident(name)), _) | (Some(Token::LParen), Some(Token::Ident(name))) => name,
            _ => continue,
        };
        if !names.contains(name) {
            names.push(name.clone());
        }
    }
    names
}

fn unexpanded_operands(tokens: &[Span<Token>]) -> Vec<bool> {
    let mut operands = vec![false; tokens.len()];
    let mut i = 0;
    while i < tokens.len() {
        let operand_end = match tokens.value_at(i) {
            Some(Token::Ident(name)) if name == "defined" => {
                Some(if tokens.value_at(i + 1) == Some(&Token::LParen) {
                    i + 4
                } else {
                    i + 2
                })
            }
            Some(Token::Ident(name)) if name.starts_with("__has_") => {
                invocation_arguments(tokens, i + 1).map(|(_, end)| end)
            }
            _ => None,
        };
        match operand_end {
            Some(end) => {
                let end = end.min(tokens.len());
                operands[i..end].fill(true);
                i = end;
            }
            None => i += 1,
        }
    }
    operands
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
