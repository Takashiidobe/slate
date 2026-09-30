use super::error::PPFailure;
use super::hide_set::HideSet;
use super::syntax::{Directive, DirectiveName, Line};
use super::{FileInput, MacroDef, Preprocessor, stringized_source};
use crate::ast::{Loc, MacroOrigin, MacroOriginLink, Span};
use crate::compiler_args::CompilerFlavor;
use crate::lexer::{Token, TokenSpanExt};
use std::rc::Rc;

#[derive(Debug, Clone)]
pub(super) struct PPToken {
    pub(super) token: Span<Token>,
    pub(super) hide: HideSet,
    // where __LINE__ reads its line: the token itself, or the end of the invocation that produced it
    end: Loc,
}

impl From<Span<Token>> for PPToken {
    fn from(token: Span<Token>) -> Self {
        Self {
            end: token.expansion,
            token,
            hide: HideSet::EMPTY,
        }
    }
}

pub(super) struct Stream<'f, 's> {
    pending: Vec<PPToken>,
    file: Option<&'f mut FileInput<'s>>,
}

impl<'f, 's> Stream<'f, 's> {
    pub(super) fn new(tokens: Vec<PPToken>, file: Option<&'f mut FileInput<'s>>) -> Self {
        let mut pending = tokens;
        pending.reverse();
        Self { pending, file }
    }

    fn push_front(&mut self, tokens: Vec<PPToken>) {
        self.pending.extend(tokens.into_iter().rev());
    }

    fn next_is_lparen(&self) -> bool {
        match self.pending.last() {
            Some(token) => token.token.value == Token::LParen,
            None => self
                .file
                .as_ref()
                .and_then(|file| file.source.peek_token())
                .is_some_and(|token| token.value == Token::LParen),
        }
    }
}

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
    end: Loc,
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

pub(super) enum Piece {
    Code(Span<Token>),
    Pragma {
        tokens: Vec<Span<Token>>,
        spelling: Loc,
        expansion: Loc,
    },
}

struct Invocation {
    lparen: PPToken,
    arguments: Vec<Vec<PPToken>>,
    commas: Vec<PPToken>,
    rparen: Option<PPToken>,
}

impl Invocation {
    fn into_tokens(self) -> Vec<PPToken> {
        let mut tokens = vec![self.lparen];
        let mut commas = self.commas.into_iter();
        for (index, argument) in self.arguments.into_iter().enumerate() {
            if index > 0 {
                tokens.extend(commas.next());
            }
            tokens.extend(argument);
        }
        tokens.extend(commas);
        tokens.extend(self.rparen);
        tokens
    }
}

impl Preprocessor<'_> {
    pub(super) fn next_raw(
        &mut self,
        stream: &mut Stream,
        across_lines: bool,
    ) -> Result<Option<PPToken>, PPFailure> {
        loop {
            if let Some(token) = stream.pending.pop() {
                return Ok(Some(token));
            }
            let Some(file) = stream.file.as_deref_mut().filter(|_| across_lines) else {
                return Ok(None);
            };
            match file.source.next_line() {
                None => return Ok(None),
                Some(Line::Directive(directive, comments)) => {
                    file.group.trailing.extend(comments);
                    self.source_position = Some(directive.loc);
                    self.directive(
                        &mut file.source,
                        &mut file.conditionals,
                        directive,
                        &mut file.group.deferred,
                    )?;
                }
                Some(Line::Text(line)) => {
                    file.group.trailing.extend(line.comments);
                    file.group.extend(&line.tokens);
                    if let Some(last) = line.tokens.last() {
                        self.source_position = Some(last.spelling);
                    }
                    stream
                        .pending
                        .extend(line.tokens.into_iter().rev().map(PPToken::from));
                }
            }
        }
    }

    // a final token, or None after pushing a macro's replacement back onto the stream
    pub(super) fn expand_token(
        &mut self,
        token: PPToken,
        stream: &mut Stream,
    ) -> Result<Option<PPToken>, PPFailure> {
        let Token::Ident(name) = &token.token.value else {
            return Ok(Some(token));
        };
        let Some(entry) = self.macros.get(name.as_str()) else {
            return Ok(Some(token));
        };
        if entry.definition.builtin {
            return Ok(Some(
                match self.expand_builtin_macro(name, &token.token, token.end) {
                    Some(value) => PPToken {
                        token: value,
                        ..token
                    },
                    None => token,
                },
            ));
        }
        if self.hide_sets.contains(token.hide, name) {
            return Ok(Some(token));
        }
        let definition = entry.definition.clone();
        let mut stamp = Stamp {
            expansion: token.token.expansion,
            end: token.end,
            origin: origin_for_expansion(name, entry.provenance, &token.token),
        };
        let name = name.clone();
        let Some(parameters) = &definition.parameters else {
            let hide = self.hide_sets.with(token.hide, &name);
            let replacement = self.substitute_object_macro(&definition, &stamp);
            let replacement = self.hide_all(replacement, hide);
            stream.push_front(inherit_leading_space(replacement, &token.token));
            return Ok(None);
        };
        if !stream.next_is_lparen() {
            return Ok(Some(token));
        }
        let Some(lparen) = self.next_raw(stream, true)? else {
            return Ok(Some(token));
        };
        let mut invocation = self.collect_invocation(stream, lparen)?;
        let Some(rparen) = invocation.rparen.as_ref() else {
            stream.push_front(invocation.into_tokens());
            return Ok(Some(token));
        };
        let rparen_hide = rparen.hide;
        // clang reports the end of the invocation's expansion range, gcc its name
        if self.dialect.flavor() == CompilerFlavor::Clang {
            stamp.end = rparen.end;
        }
        if invocation.arguments.is_empty() && parameters.len() == 1 && !definition.variadic {
            invocation.arguments.push(Vec::new());
        }
        let arguments = &invocation.arguments;
        if !definition.variadic && arguments.len() != parameters.len()
            || definition.variadic && arguments.len() < parameters.len()
        {
            stream.push_front(invocation.into_tokens());
            return Ok(Some(token));
        }
        let prescanned = prescanned_parameters(&definition, parameters);
        let mut expanded_arguments = Vec::with_capacity(arguments.len());
        for (index, argument) in arguments.iter().enumerate() {
            let prescan = prescanned
                .get(index)
                .copied()
                .unwrap_or(prescanned[parameters.len()]);
            expanded_arguments.push(
                if prescan && argument.iter().any(|token| self.may_expand(&token.token)) {
                    Some(self.expand_isolated(argument.clone())?)
                } else {
                    None
                },
            );
        }
        let elide_comma =
            self.comma_elision(&definition, parameters, arguments, &expanded_arguments)?;
        let replacement = self.substitute_function_macro(
            &definition,
            parameters,
            &Arguments {
                raw: arguments,
                expanded: &expanded_arguments,
                commas: &invocation.commas,
                elide_comma,
            },
            &stamp,
        );
        let hide = self.hide_sets.intersection(token.hide, rparen_hide);
        let hide = self.hide_sets.with(hide, &name);
        let replacement = self.hide_all(replacement, hide);
        stream.push_front(inherit_leading_space(replacement, &token.token));
        Ok(None)
    }

    fn hide_all(&mut self, mut tokens: Vec<PPToken>, hide: HideSet) -> Vec<PPToken> {
        for token in &mut tokens {
            token.hide = self.hide_sets.union(token.hide, hide);
        }
        tokens
    }

    fn collect_invocation(
        &mut self,
        stream: &mut Stream,
        lparen: PPToken,
    ) -> Result<Invocation, PPFailure> {
        let mut invocation = Invocation {
            lparen,
            arguments: Vec::new(),
            commas: Vec::new(),
            rparen: None,
        };
        let mut current = Vec::new();
        let mut depth = 0usize;
        while let Some(token) = self.next_raw(stream, true)? {
            match token.token.value {
                Token::LParen => {
                    depth += 1;
                    current.push(token);
                }
                Token::RParen if depth == 0 => {
                    if !current.is_empty() || !invocation.arguments.is_empty() {
                        invocation.arguments.push(current);
                    }
                    invocation.rparen = Some(token);
                    return Ok(invocation);
                }
                Token::RParen => {
                    depth -= 1;
                    current.push(token);
                }
                Token::Comma if depth == 0 => {
                    invocation.arguments.push(std::mem::take(&mut current));
                    invocation.commas.push(token);
                }
                _ => current.push(token),
            }
        }
        invocation.arguments.push(current);
        Ok(invocation)
    }

    pub(super) fn expand_isolated(
        &mut self,
        tokens: Vec<PPToken>,
    ) -> Result<Vec<PPToken>, PPFailure> {
        let mut stream = Stream::new(tokens, None);
        let mut expanded = Vec::new();
        while let Some(token) = self.next_raw(&mut stream, false)? {
            expanded.extend(self.expand_token(token, &mut stream)?);
        }
        Ok(expanded)
    }

    pub(super) fn expand_macros(
        &mut self,
        tokens: &[Span<Token>],
    ) -> Result<Vec<Span<Token>>, PPFailure> {
        let tokens = tokens.iter().cloned().map(PPToken::from).collect();
        Ok(self
            .expand_isolated(tokens)?
            .into_iter()
            .map(|token| token.token)
            .collect())
    }

    // `defined` and `__has_*` take their operands unexpanded, even when an expansion produced them
    pub(super) fn expand_condition(
        &mut self,
        tokens: &[Span<Token>],
    ) -> Result<Vec<Span<Token>>, PPFailure> {
        let tokens = tokens.iter().cloned().map(PPToken::from).collect();
        let mut stream = Stream::new(tokens, None);
        let mut expanded = Vec::new();
        while let Some(token) = self.next_raw(&mut stream, false)? {
            let operator = match &token.token.value {
                Token::Ident(name) if name == "defined" => Some(true),
                Token::Ident(name) if name.starts_with("__has_") => Some(false),
                Token::Ident(name)
                    if name == "__is_identifier"
                        && self.dialect.flavor() == CompilerFlavor::Clang =>
                {
                    Some(false)
                }
                _ => None,
            };
            let Some(defined) = operator else {
                expanded.extend(
                    self.expand_token(token, &mut stream)?
                        .map(|token| token.token),
                );
                continue;
            };
            expanded.push(token.token);
            if defined && !stream.next_is_lparen() {
                expanded.extend(stream.pending.pop().map(|token| token.token));
                continue;
            }
            let mut depth = 0usize;
            while stream.next_is_lparen() || depth > 0 {
                let Some(token) = stream.pending.pop() else {
                    break;
                };
                match token.token.value {
                    Token::LParen => depth += 1,
                    Token::RParen => depth -= 1,
                    _ => {}
                }
                expanded.push(token.token);
                if depth == 0 {
                    break;
                }
            }
        }
        Ok(expanded)
    }

    fn next_expanded(
        &mut self,
        stream: &mut Stream,
        across_lines: bool,
    ) -> Result<Option<PPToken>, PPFailure> {
        while let Some(token) = self.next_raw(stream, across_lines)? {
            if let Some(token) = self.expand_token(token, stream)? {
                return Ok(Some(token));
            }
        }
        Ok(None)
    }

    // false once the stream is exhausted
    pub(super) fn read_piece(
        &mut self,
        stream: &mut Stream,
        pieces: &mut Vec<Piece>,
    ) -> Result<bool, PPFailure> {
        let Some(token) = self.next_expanded(stream, false)? else {
            return Ok(false);
        };
        let microsoft = match &token.token.value {
            Token::Ident(name) if name == "_Pragma" => Some(false),
            Token::Ident(name)
                if name == "__pragma" && self.dialect.features().microsoft_extensions =>
            {
                Some(true)
            }
            _ => None,
        };
        let Some(microsoft) = microsoft else {
            pieces.push(Piece::Code(token.token));
            return Ok(true);
        };
        let lparen = match self.next_expanded(stream, true)? {
            Some(lparen) if lparen.token.value == Token::LParen => lparen,
            next => {
                stream.push_front(next.into_iter().collect());
                pieces.push(Piece::Code(token.token));
                return Ok(true);
            }
        };
        let mut consumed = vec![token.token, lparen.token];
        let tokens = if microsoft {
            let mut depth = 1usize;
            while depth > 0
                && let Some(token) = self.next_expanded(stream, true)?
            {
                match token.token.value {
                    Token::LParen => depth += 1,
                    Token::RParen => depth -= 1,
                    _ => {}
                }
                consumed.push(token.token);
            }
            (depth == 0).then(|| consumed[2..consumed.len() - 1].to_vec())
        } else {
            for _ in 0..2 {
                consumed.extend(self.next_expanded(stream, true)?.map(|token| token.token));
            }
            match (consumed.len(), consumed.value_at(2), consumed.value_at(3)) {
                (4, Some(Token::StringLit(value)), Some(Token::RParen)) => {
                    let origin = Span::cover((), &consumed);
                    let decoded = value.replace("\\\"", "\"").replace("\\\\", "\\");
                    Some(
                        self.lex(&decoded)
                            .into_iter()
                            .map(|token| origin.clone().with_value(token))
                            .collect(),
                    )
                }
                _ => None,
            }
        };
        let Some(tokens) = tokens else {
            pieces.extend(consumed.into_iter().map(Piece::Code));
            return Ok(true);
        };
        let origin = Span::cover((), &consumed);
        self.record_pragma(&Directive {
            name: DirectiveName::Pragma,
            arguments: tokens.clone(),
            name_loc: origin.expansion,
            loc: origin.expansion,
        });
        pieces.push(Piece::Pragma {
            tokens,
            spelling: origin.spelling,
            expansion: origin.expansion,
        });
        Ok(true)
    }
}

impl Preprocessor<'_> {
    fn expand_builtin_macro(
        &self,
        name: &str,
        token: &Span<Token>,
        end: Loc,
    ) -> Option<Span<Token>> {
        let loc = token.expansion;
        match name {
            "__LINE__" => {
                // msvc reports how far the source has been read, even for a __LINE__ in an argument
                let end = match self.dialect.flavor() {
                    CompilerFlavor::Msvc => self.source_position.unwrap_or(end),
                    _ => end,
                };
                let (line, _) = self.presumed_location(end);
                Some(
                    token
                        .clone()
                        .with_value(Token::IntLit(line.to_string().into())),
                )
            }
            "__FILE__" => {
                let (_, file) = self.presumed_location(loc);
                Some(token.clone().with_value(Token::StringLit(file.into())))
            }
            "__FILE_NAME__" => {
                let (_, file) = self.presumed_location(loc);
                let name = std::path::Path::new(&file)
                    .file_name()
                    .map_or(file.clone(), |name| name.to_string_lossy().into_owned());
                Some(token.clone().with_value(Token::StringLit(name.into())))
            }
            "__BASE_FILE__" => {
                let main = self.main_file.unwrap_or(loc.file);
                let file = crate::files::display_path(self.files.path(main));
                Some(token.clone().with_value(Token::StringLit(file.into())))
            }
            "__INCLUDE_LEVEL__" => {
                let level = self.open_stack.len().saturating_sub(1);
                Some(
                    token
                        .clone()
                        .with_value(Token::IntLit(level.to_string().into())),
                )
            }
            "__COUNTER__" => {
                let value = self.counter.get();
                self.counter.set(value + 1);
                Some(
                    token
                        .clone()
                        .with_value(Token::IntLit(value.to_string().into())),
                )
            }
            "__TIMESTAMP__" => Some(
                token
                    .clone()
                    .with_value(Token::StringLit(self.file_timestamp(loc.file).into())),
            ),
            "__DATE__" => Some(
                token
                    .clone()
                    .with_value(Token::StringLit(self.build_date().into())),
            ),
            "__TIME__" => Some(
                token
                    .clone()
                    .with_value(Token::StringLit(self.build_time().into())),
            ),
            _ => None,
        }
    }

    fn build_date(&self) -> String {
        let (year, month, day, _, _, _) = calendar_time(self.build_time);
        format!("{} {:2} {year:04}", MONTHS[month as usize - 1], day)
    }

    fn build_time(&self) -> String {
        let (_, _, _, hour, minute, second) = calendar_time(self.build_time);
        format!("{hour:02}:{minute:02}:{second:02}")
    }

    fn file_timestamp(&self, file: crate::ast::FileId) -> String {
        let Some(modified) = self
            .files
            .get_path(file)
            .and_then(|path| std::fs::metadata(path).ok())
            .and_then(|metadata| metadata.modified().ok())
        else {
            return "??? ??? ?? ??:??:?? ????".into();
        };
        let (year, month, day, hour, minute, second) = calendar_time(modified);
        const WEEKDAYS: [&str; 7] = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
        let shifted = if month < 3 { year - 1 } else { year };
        let offsets = [0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4];
        let weekday = (shifted + shifted / 4 - shifted / 100
            + shifted / 400
            + offsets[month as usize - 1]
            + day)
            .rem_euclid(7);
        format!(
            "{} {} {day:2} {hour:02}:{minute:02}:{second:02} {year:04}",
            WEEKDAYS[weekday as usize],
            MONTHS[month as usize - 1]
        )
    }

    fn may_expand(&self, token: &Span<Token>) -> bool {
        match &token.value {
            Token::Ident(name) => self.macros.contains_key(name.as_str()),
            _ => false,
        }
    }
}

const MONTHS: [&str; 12] = [
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
];

fn calendar_time(time: std::time::SystemTime) -> (i64, i64, i64, i64, i64, i64) {
    let seconds = time
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

struct Arguments<'a> {
    raw: &'a [Vec<PPToken>],
    expanded: &'a [Option<Vec<PPToken>>],
    commas: &'a [PPToken],
    elide_comma: ElideComma,
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum ElideComma {
    Never,
    // gnu `, ## __VA_ARGS__` with the variadic argument omitted
    Pasted,
    // msvc's traditional preprocessor, before any empty __VA_ARGS__
    Always,
}

impl Arguments<'_> {
    fn selected(&self, index: usize, prescan: bool) -> &[PPToken] {
        match self.expanded.get(index) {
            Some(Some(expanded)) if prescan => expanded,
            _ => self.raw.get(index).map_or(&[], Vec::as_slice),
        }
    }

    fn variadic(&self, fixed: usize, prescan: bool) -> Vec<PPToken> {
        let mut output = Vec::new();
        for index in fixed..self.raw.len() {
            if index != fixed {
                output.extend(self.commas.get(index - 1).cloned());
            }
            output.extend_from_slice(self.selected(index, prescan));
        }
        output
    }
}

fn inherit_leading_space(mut tokens: Vec<PPToken>, name: &Span<Token>) -> Vec<PPToken> {
    if let Some(first) = tokens.first_mut() {
        first.token.leading_space = name.leading_space;
    }
    tokens
}

impl Stamp {
    fn token(&self, token: &Span<Token>) -> PPToken {
        PPToken {
            end: self.end,
            ..PPToken::from(self.apply(token))
        }
    }
}

impl Preprocessor<'_> {
    fn substitute_function_macro(
        &self,
        definition: &MacroDef,
        parameters: &[String],
        arguments: &Arguments,
        stamp: &Stamp,
    ) -> Vec<PPToken> {
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
                    macro_argument(name, parameters, arguments.raw).map(<[PPToken]>::to_vec)
                };
                if let Some(argument) = argument {
                    let text = stringized_source(argument.iter().map(|token| &token.token));
                    output.push(PPToken::from(
                        stamp.apply(token).with_value(Token::StringLit(text.into())),
                    ));
                    i += 2;
                    continue;
                }
            }
            let elide = match arguments.elide_comma {
                ElideComma::Never => false,
                ElideComma::Pasted => token.value == Token::HashHash,
                ElideComma::Always => true,
            };
            let next = if token.value == Token::HashHash {
                definition.replacement.get(i + 1)
            } else {
                Some(token)
            };
            if elide
                && next.is_some_and(
                    |next| matches!(&next.value, Token::Ident(name) if name == "__VA_ARGS__"),
                )
                && output
                    .last()
                    .is_some_and(|last| last.token.value == Token::Comma)
            {
                output.pop();
                i += if token.value == Token::HashHash { 2 } else { 1 };
                continue;
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
                output.extend(self.paste(left, right_tokens));
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

    fn comma_elision(
        &mut self,
        definition: &MacroDef,
        parameters: &[String],
        arguments: &[Vec<PPToken>],
        expanded: &[Option<Vec<PPToken>>],
    ) -> Result<ElideComma, PPFailure> {
        if !definition.variadic {
            return Ok(ElideComma::Never);
        }
        if self.dialect.flavor() == CompilerFlavor::Msvc {
            for (index, raw) in arguments.iter().enumerate().skip(parameters.len()) {
                let empty = match expanded.get(index) {
                    Some(Some(expanded)) => expanded.is_empty(),
                    _ => raw.is_empty() || self.expand_isolated(raw.clone())?.is_empty(),
                };
                if !empty {
                    return Ok(ElideComma::Never);
                }
            }
            return Ok(ElideComma::Always);
        }
        // iso modes read `H()` of `H(...)` as one empty argument, gnu modes as none
        let omitted = arguments.len() <= parameters.len()
            && (!parameters.is_empty() || self.dialect.standard().is_gnu());
        Ok(if omitted {
            ElideComma::Pasted
        } else {
            ElideComma::Never
        })
    }

    fn substitute_object_macro(&self, definition: &MacroDef, stamp: &Stamp) -> Vec<PPToken> {
        let mut output = Vec::new();
        let mut tokens = definition.replacement.iter();
        while let Some(token) = tokens.next() {
            if token.value == Token::HashHash
                && !output.is_empty()
                && let Some(right) = tokens.next()
                && let Some(left) = output.pop()
            {
                output.extend(self.paste(left, vec![stamp.token(right)]));
                continue;
            }
            output.push(stamp.token(token));
        }
        output
    }

    fn paste(&self, left: PPToken, right_tokens: Vec<PPToken>) -> Vec<PPToken> {
        let Some(right) = right_tokens.first() else {
            return vec![left];
        };
        let pasted = self.lex(&format!(
            "{}{}",
            String::from(&left.token.value),
            String::from(&right.token.value)
        ));
        if pasted.len() != 1 {
            return std::iter::once(left).chain(right_tokens).collect();
        }
        let joined = Span::new(
            pasted[0].clone(),
            left.token.spelling.through(right.token.spelling),
            left.token.expansion.through(right.token.expansion),
        )
        .with_leading_space(left.token.leading_space);
        std::iter::once(PPToken::from(joined))
            .chain(right_tokens.into_iter().skip(1))
            .collect()
    }
}

fn macro_argument<'a>(
    name: &str,
    parameters: &[String],
    arguments: &'a [Vec<PPToken>],
) -> Option<&'a [PPToken]> {
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
) -> Vec<PPToken> {
    let Token::Ident(name) = &token.value else {
        return vec![stamp.token(token)];
    };
    if name == "__VA_ARGS__" {
        return inherit_leading_space(arguments.variadic(parameters.len(), prescan), token);
    }
    parameters
        .iter()
        .position(|parameter| parameter == name)
        .filter(|&index| index < arguments.raw.len())
        .map(|index| inherit_leading_space(arguments.selected(index, prescan).to_vec(), token))
        .unwrap_or_else(|| vec![stamp.token(token)])
}
