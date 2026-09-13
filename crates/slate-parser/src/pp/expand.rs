use super::{MacroDef, Preprocessor, lex, tokens_source};
use crate::ast::{FileId, Loc, Span};
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

    fn expand_builtin_macro(&self, name: &str, token: &Span<Token>) -> Option<Span<Token>> {
        let loc = token.expansion;
        match name {
            "__LINE__" => {
                let (line, _) = self.presumed_location(loc);
                Some(token.clone().with_value(Token::IntLit(line.to_string())))
            }
            "__FILE__" => {
                let (_, file) = self.presumed_location(loc);
                Some(token.clone().with_value(Token::StringLit(file)))
            }
            "__FILE_NAME__" => {
                let (_, file) = self.presumed_location(loc);
                let name = std::path::Path::new(&file)
                    .file_name()
                    .map_or(file.clone(), |name| name.to_string_lossy().into_owned());
                Some(token.clone().with_value(Token::StringLit(name)))
            }
            "__BASE_FILE__" => {
                let main = self.main_file.unwrap_or(loc.file);
                let file = crate::files::display_path(self.files.path(main));
                Some(token.clone().with_value(Token::StringLit(file)))
            }
            "__INCLUDE_LEVEL__" => {
                let level = self.open_stack.len().saturating_sub(1);
                Some(token.clone().with_value(Token::IntLit(level.to_string())))
            }
            "__COUNTER__" => {
                let value = self.counter.get();
                self.counter.set(value + 1);
                Some(token.clone().with_value(Token::IntLit(value.to_string())))
            }
            "__DATE__" => Some(
                token
                    .clone()
                    .with_value(Token::StringLit(self.build_date())),
            ),
            "__TIME__" => Some(
                token
                    .clone()
                    .with_value(Token::StringLit(self.build_time())),
            ),
            _ => None,
        }
    }

    fn build_date(&self) -> String {
        let (year, month, day, _, _, _) = self.build_calendar_time();
        const MONTHS: [&str; 12] = [
            "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
        ];
        format!("{} {:2} {year:04}", MONTHS[month as usize - 1], day)
    }

    fn build_time(&self) -> String {
        let (_, _, _, hour, minute, second) = self.build_calendar_time();
        format!("{hour:02}:{minute:02}:{second:02}")
    }

    fn build_calendar_time(&self) -> (i64, i64, i64, i64, i64, i64) {
        let seconds = self
            .build_time
            .duration_since(std::time::UNIX_EPOCH)
            .map_or(0, |duration| duration.as_secs() as i64);
        #[cfg(unix)]
        if let Some(local) = local_calendar_time(seconds) {
            return local;
        }
        let days = seconds.div_euclid(86_400);
        let day_seconds = seconds.rem_euclid(86_400);
        let z = days + 719_468;
        let era = if z >= 0 { z } else { z - 146_096 }.div_euclid(146_097);
        let day_of_era = z - era * 146_097;
        let year_of_era = (day_of_era - day_of_era / 1_460 + day_of_era / 36_524
            - day_of_era / 146_096)
            .div_euclid(365);
        let mut year = year_of_era + era * 400;
        let day_of_year = day_of_era - (365 * year_of_era + year_of_era / 4 - year_of_era / 100);
        let month_part = (5 * day_of_year + 2).div_euclid(153);
        let day = day_of_year - (153 * month_part + 2).div_euclid(5) + 1;
        let month = month_part + if month_part < 10 { 3 } else { -9 };
        if month <= 2 {
            year += 1;
        }
        (
            year,
            month,
            day,
            day_seconds / 3_600,
            day_seconds % 3_600 / 60,
            day_seconds % 60,
        )
    }

    pub(super) fn expand_macros(
        &self,
        tokens: &[Span<Token>],
        disabled: &mut HashSet<String>,
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
            if let Some(replacement) = self.expand_builtin_macro(name, token) {
                expanded.push(replacement);
                i += 1;
                continue;
            }
            let Some(macro_def) = self.macros.get(name).map(|entry| entry.definition.clone())
            else {
                expanded.push(token.clone());
                i += 1;
                continue;
            };
            if disabled.contains(name) {
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
                disabled.insert(name.clone());
                expanded.extend(self.expand_macros(&replacement, disabled));
                disabled.remove(name);
                i += 1;
                continue;
            };
            let Some((mut arguments, end)) = invocation_arguments(tokens, i + 1) else {
                expanded.push(token.clone());
                i += 1;
                continue;
            };
            if arguments.is_empty() && parameters.len() == 1 && !macro_def.variadic {
                arguments.push(Vec::new());
            }
            if !macro_def.variadic && arguments.len() != parameters.len()
                || macro_def.variadic && arguments.len() < parameters.len()
            {
                expanded.push(token.clone());
                i += 1;
                continue;
            }
            let expanded_arguments = arguments
                .iter()
                .map(|argument| self.expand_macros(argument, disabled))
                .collect::<Vec<_>>();
            let mut macro_def = macro_def;
            for replacement in &mut macro_def.replacement {
                replacement.expansion = token.expansion;
            }
            let name = name.clone();
            disabled.insert(name.clone());
            let replacement =
                substitute_function_macro(&macro_def, &parameters, &arguments, &expanded_arguments);
            expanded.extend(self.expand_macros(&replacement, disabled));
            disabled.remove(&name);
            i = end;
        }
        expanded
    }

    pub(super) fn expand_condition(&self, tokens: &[Span<Token>]) -> Vec<Span<Token>> {
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
                expanded.extend(self.expand_macros(&tokens[start..end], &mut HashSet::new()));
            }
            start = end;
        }
        expanded
    }
}

#[cfg(unix)]
fn local_calendar_time(seconds: i64) -> Option<(i64, i64, i64, i64, i64, i64)> {
    let timestamp = seconds as libc::time_t;
    let mut local = std::mem::MaybeUninit::<libc::tm>::uninit();
    if unsafe { libc::localtime_r(&timestamp, local.as_mut_ptr()) }.is_null() {
        return None;
    }
    let local = unsafe { local.assume_init() };
    Some((
        i64::from(local.tm_year) + 1900,
        i64::from(local.tm_mon) + 1,
        i64::from(local.tm_mday),
        i64::from(local.tm_hour),
        i64::from(local.tm_min),
        i64::from(local.tm_sec),
    ))
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
    expanded_arguments: &[Vec<Span<Token>>],
) -> Vec<Span<Token>> {
    let mut output = Vec::new();
    let mut i = 0;
    while i < definition.replacement.len() {
        let token = &definition.replacement[i];
        if token.value == Token::Ident("__VA_OPT__".to_string())
            && let Some((optional, end)) = invocation_arguments(&definition.replacement, i + 1)
        {
            if arguments.len() > parameters.len() {
                for optional_token in optional.into_iter().flatten() {
                    output.extend(replacement_tokens(
                        &optional_token,
                        parameters,
                        arguments,
                        expanded_arguments,
                        true,
                    ));
                }
            }
            i = end;
            continue;
        }
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
                expanded_arguments,
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
            expanded_arguments,
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
