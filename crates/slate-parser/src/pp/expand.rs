use super::{MacroDef, Preprocessor, stringized_source};
use crate::ast::{Loc, MacroOrigin, MacroOriginLink, Span};
use crate::lexer::{Token, TokenSpanExt};
use foldhash::HashSet;
use std::rc::Rc;

fn origin_for_expansion(
    name: &str,
    provenance: crate::ast::Provenance,
    token: &Span<Token>,
) -> Rc<MacroOrigin> {
    match &token.macro_origin {
        Some(existing) => Rc::new(MacroOrigin {
            name: existing.name.clone(),
            definition: existing.definition,
            inner: Some(Rc::new(MacroOriginLink {
                name: Rc::from(name),
                definition: provenance,
                parent: existing.inner.clone(),
            })),
        }),
        None => Rc::new(MacroOrigin {
            name: Rc::from(name),
            definition: provenance,
            inner: None,
        }),
    }
}

struct Stamp {
    expansion: Loc,
    origin: Rc<MacroOrigin>,
}

impl Stamp {
    fn apply(&self, token: &Span<Token>) -> Span<Token> {
        let mut token = token.clone();
        token.expansion = self.expansion;
        token.macro_origin = Some(self.origin.clone());
        token
    }
}

struct Invocation {
    arguments: Vec<Vec<Span<Token>>>,
    commas: Vec<Span<Token>>,
    end: usize,
    from_tail: usize,
}

impl Preprocessor<'_> {
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

    fn may_expand(&self, token: &Span<Token>) -> bool {
        match &token.value {
            Token::Ident(name) => name.starts_with("__") || self.macros.contains_key(name),
            _ => false,
        }
    }

    pub(super) fn expand_macros(
        &self,
        tokens: &[Span<Token>],
        disabled: &mut HashSet<String>,
    ) -> Vec<Span<Token>> {
        self.expand_rescanning(tokens, &[], disabled).0
    }

    // `tail` is what follows `tokens` in the enclosing stream: rescanning a
    // replacement may complete an invocation out of it, and reports how much
    // of it the expansion swallowed
    fn expand_rescanning(
        &self,
        tokens: &[Span<Token>],
        tail: &[Span<Token>],
        disabled: &mut HashSet<String>,
    ) -> (Vec<Span<Token>>, usize) {
        let mut expanded = Vec::new();
        let mut i = 0;
        let mut taken = 0;
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
            let Some(macro_entry) = self.macros.get(name) else {
                expanded.push(token.clone());
                i += 1;
                continue;
            };
            let macro_def = &macro_entry.definition;
            if disabled.contains(name) {
                expanded.push(token.clone());
                i += 1;
                continue;
            }
            let rest = &tokens[i + 1..];
            let stamp = Stamp {
                expansion: token.expansion,
                origin: origin_for_expansion(name, macro_entry.provenance, token),
            };
            let Some(parameters) = &macro_def.parameters else {
                let replacement = macro_def
                    .replacement
                    .iter()
                    .map(|replacement| stamp.apply(replacement))
                    .collect::<Vec<_>>();
                disabled.insert(name.clone());
                let (produced, used) = self.rescan(&replacement, rest, tail, disabled);
                disabled.remove(name);
                expanded.extend(inherit_leading_space(produced, token));
                i += 1 + used.min(rest.len());
                taken += used.saturating_sub(rest.len());
                continue;
            };
            let Some(Invocation {
                mut arguments,
                commas,
                end,
                from_tail,
            }) = self.invocation(rest, tail)
            else {
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
            let prescanned = prescanned_parameters(macro_def, parameters);
            let expanded_arguments = arguments
                .iter()
                .enumerate()
                .map(|(index, argument)| {
                    let prescan = prescanned
                        .get(index)
                        .copied()
                        .unwrap_or(prescanned[parameters.len()]);
                    (prescan && argument.iter().any(|token| self.may_expand(token)))
                        .then(|| self.expand_macros(argument, disabled))
                })
                .collect::<Vec<_>>();
            disabled.insert(name.clone());
            let replacement = self.substitute_function_macro(
                macro_def,
                parameters,
                &Arguments {
                    raw: &arguments,
                    expanded: &expanded_arguments,
                    commas: &commas,
                },
                &stamp,
            );
            let consumed = i + 1 + end - from_tail;
            let (produced, used) = self.rescan(
                &replacement,
                &tokens[consumed..],
                &tail[from_tail..],
                disabled,
            );
            disabled.remove(name);
            expanded.extend(inherit_leading_space(produced, token));
            i = consumed + used.min(tokens.len() - consumed);
            taken += from_tail + used.saturating_sub(tokens.len() - consumed);
        }
        (expanded, taken)
    }

    fn rescan(
        &self,
        replacement: &[Span<Token>],
        rest: &[Span<Token>],
        tail: &[Span<Token>],
        disabled: &mut HashSet<String>,
    ) -> (Vec<Span<Token>>, usize) {
        let isolated = self.expand_macros(replacement, disabled);
        if (rest.is_empty() && tail.is_empty()) || !self.wants_more(&isolated) {
            return (isolated, 0);
        }
        let following = rest.iter().chain(tail).cloned().collect::<Vec<_>>();
        self.expand_rescanning(replacement, &following, disabled)
    }

    // an expansion that ends mid-invocation is the only one worth rescanning
    // against the caller's stream, and expanding in isolation is how we tell
    fn wants_more(&self, expansion: &[Span<Token>]) -> bool {
        let mut depth = 0i32;
        for token in expansion {
            match token.value {
                Token::LParen => depth += 1,
                Token::RParen => depth -= 1,
                _ => {}
            }
        }
        if depth > 0 {
            return true;
        }
        match expansion.last().map(|token| &token.value) {
            Some(Token::Ident(name)) => self
                .macros
                .get(name)
                .is_some_and(|entry| entry.definition.parameters.is_some()),
            _ => false,
        }
    }

    // an invocation may open in `rest` and close in the caller's `tail`
    fn invocation(&self, rest: &[Span<Token>], tail: &[Span<Token>]) -> Option<Invocation> {
        if let Some((arguments, commas, end)) = invocation_arguments(rest, 0) {
            return Some(Invocation {
                arguments,
                commas,
                end,
                from_tail: 0,
            });
        }
        if tail.is_empty() {
            return None;
        }
        let spliced = rest.iter().chain(tail).cloned().collect::<Vec<_>>();
        let (arguments, commas, end) = invocation_arguments(&spliced, 0)?;
        Some(Invocation {
            arguments,
            commas,
            end,
            from_tail: end.saturating_sub(rest.len()),
        })
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
                expanded.extend(self.expand_macros(&tokens[start..end], &mut HashSet::default()));
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
                invocation_arguments(tokens, i + 1).map(|(_, _, end)| end)
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

type SplitArguments = (Vec<Vec<Span<Token>>>, Vec<Span<Token>>, usize);

fn invocation_arguments(tokens: &[Span<Token>], start: usize) -> Option<SplitArguments> {
    if tokens.value_at(start) != Some(&Token::LParen) {
        return None;
    }
    let mut arguments = Vec::new();
    let mut commas = Vec::new();
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
                return Some((arguments, commas, i + 1));
            }
            Token::RParen => {
                depth -= 1;
                current.push(tokens[i].clone());
            }
            Token::Comma if depth == 0 => {
                arguments.push(std::mem::take(&mut current));
                commas.push(tokens[i].clone());
            }
            _ => current.push(tokens[i].clone()),
        }
        i += 1;
    }
    None
}

struct Arguments<'a> {
    raw: &'a [Vec<Span<Token>>],
    expanded: &'a [Option<Vec<Span<Token>>>],
    commas: &'a [Span<Token>],
}

impl Arguments<'_> {
    fn selected(&self, index: usize, prescan: bool) -> &[Span<Token>] {
        match self.expanded.get(index) {
            Some(Some(expanded)) if prescan => expanded,
            _ => self.raw.get(index).map_or(&[], Vec::as_slice),
        }
    }

    fn variadic(&self, fixed: usize, prescan: bool) -> Vec<Span<Token>> {
        let mut output: Vec<Span<Token>> = Vec::new();
        for index in fixed..self.raw.len() {
            if index != fixed {
                output.extend(self.commas.get(index - 1).cloned());
            }
            output.extend_from_slice(self.selected(index, prescan));
        }
        output
    }
}

// one entry per parameter, then one for __VA_ARGS__
fn prescanned_parameters(definition: &MacroDef, parameters: &[String]) -> Vec<bool> {
    let replacement = &definition.replacement;
    if replacement
        .iter()
        .any(|token| matches!(&token.value, Token::Ident(name) if name == "__VA_OPT__"))
    {
        return vec![true; parameters.len() + 1];
    }
    let mut prescanned = vec![false; parameters.len() + 1];
    for (i, token) in replacement.iter().enumerate() {
        let Token::Ident(name) = &token.value else {
            continue;
        };
        let index = if name == "__VA_ARGS__" {
            parameters.len()
        } else if let Some(index) = parameters.iter().position(|parameter| parameter == name) {
            index
        } else {
            continue;
        };
        let operand = |neighbor: Option<&Span<Token>>, operator: &Token| {
            neighbor.is_some_and(|neighbor| &neighbor.value == operator)
        };
        let previous = i
            .checked_sub(1)
            .and_then(|previous| replacement.get(previous));
        if !operand(previous, &Token::Hash)
            && !operand(previous, &Token::HashHash)
            && !operand(replacement.get(i + 1), &Token::HashHash)
        {
            prescanned[index] = true;
        }
    }
    prescanned
}

fn inherit_leading_space(mut tokens: Vec<Span<Token>>, name: &Span<Token>) -> Vec<Span<Token>> {
    if let Some(first) = tokens.first_mut() {
        first.leading_space = name.leading_space;
    }
    tokens
}

impl Preprocessor<'_> {
    fn substitute_function_macro(
        &self,
        definition: &MacroDef,
        parameters: &[String],
        arguments: &Arguments,
        stamp: &Stamp,
    ) -> Vec<Span<Token>> {
        let mut output = Vec::new();
        let mut i = 0;
        while i < definition.replacement.len() {
            let token = &definition.replacement[i];
            if matches!(&token.value, Token::Ident(name) if name == "__VA_OPT__")
                && let Some((optional, _, end)) =
                    invocation_arguments(&definition.replacement, i + 1)
            {
                if arguments
                    .raw
                    .iter()
                    .skip(parameters.len())
                    .any(|argument| !argument.is_empty())
                {
                    for optional_token in optional.into_iter().flatten() {
                        output.extend(replacement_tokens(
                            &optional_token,
                            parameters,
                            arguments,
                            stamp,
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
                    Some(arguments.variadic(parameters.len(), false))
                } else {
                    macro_argument(name, parameters, arguments.raw).map(<[Span<Token>]>::to_vec)
                };
                if let Some(argument) = argument {
                    output.push(
                        stamp
                            .apply(token)
                            .with_value(Token::StringLit(stringized_source(&argument))),
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
                    stamp,
                    false,
                );
                if let Some(right) = right_tokens.first() {
                    let pasted = self.lex(&format!(
                        "{}{}",
                        String::from(&left.value),
                        String::from(&right.value)
                    ));
                    if pasted.len() == 1 {
                        output.push(
                            Span::new(
                                pasted[0].clone(),
                                left.spelling.through(right.spelling),
                                left.expansion.through(right.expansion),
                            )
                            .with_leading_space(left.leading_space),
                        );
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
                stamp,
                i + 1 >= definition.replacement.len()
                    || definition.replacement[i + 1].value != Token::HashHash,
            ));
            i += 1;
        }
        output
    }
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
    arguments: &Arguments,
    stamp: &Stamp,
    prescan: bool,
) -> Vec<Span<Token>> {
    let Token::Ident(name) = &token.value else {
        return vec![stamp.apply(token)];
    };
    if name == "__VA_ARGS__" {
        return inherit_leading_space(arguments.variadic(parameters.len(), prescan), token);
    }
    parameters
        .iter()
        .position(|parameter| parameter == name)
        .filter(|&index| index < arguments.raw.len())
        .map(|index| inherit_leading_space(arguments.selected(index, prescan).to_vec(), token))
        .unwrap_or_else(|| vec![stamp.apply(token)])
}
