use super::{ArmState, MacroEventKind, PPNode, PPNodeKind, PreprocessTrace, TokenTrace};
use crate::ast::{FileId, Loc, Span};
use crate::charset::Charset;
use crate::files::{Files, decode_source_bytes, raw_byte_for_char};
use crate::lexer::{Token, token_spelling};
use std::collections::HashMap;
use std::io::{self, Write};

pub fn write_preprocessed(
    nodes: &[PPNode],
    files: &Files,
    code_page: Option<Charset>,
    out: &mut impl Write,
) -> io::Result<()> {
    let mut spellings = Spellings::new(files);
    for node in nodes {
        let line = match &node.value {
            PPNodeKind::Comment { .. } => continue,
            PPNodeKind::Code { tokens, .. } => spellings.line(tokens),
            PPNodeKind::Pragma { tokens, .. } => format!("#pragma {}", spellings.line(tokens)),
        };
        match code_page {
            Some(charset) => out.write_all(&encode_line(&line, charset))?,
            None => out.write_all(line.as_bytes())?,
        }
        out.write_all(b"\n")?;
    }
    Ok(())
}

pub fn write_trace(
    nodes: &[PPNode],
    files: &Files,
    trace: &PreprocessTrace,
    out: &mut impl Write,
) -> io::Result<()> {
    let site = |loc: Loc| {
        let line = files
            .position(loc.file, loc.offset)
            .map(|(line, _)| line + 1);
        let path = files.path(loc.file);
        let name = if Some(loc.file) == trace.main_file {
            "<main>".to_string()
        } else {
            path.file_name().map_or_else(
                || path.display().to_string(),
                |name| name.to_string_lossy().into_owned(),
            )
        };
        match line {
            Some(line) if name != "<command line>" => format!("{name}:{line}"),
            _ => name,
        }
    };
    let describe = |trace_of: TokenTrace| {
        let context: Vec<String> = trace
            .branches(trace_of.context)
            .iter()
            .map(|branch| {
                let arm = &trace.conditional(branch.conditional).arms[branch.arm as usize];
                format!("{}@{}", arm.directive, site(arm.loc))
            })
            .collect();
        let macros: Vec<String> = trace
            .macros(trace_of.macros)
            .iter()
            .map(|event| {
                let event = trace.event(*event);
                let kind = match event.kind {
                    MacroEventKind::Define => "",
                    MacroEventKind::Undef => "undef ",
                };
                let skipped = if event.skipped { "skipped " } else { "" };
                format!("{skipped}{kind}{}@{}", event.name, site(event.loc))
            })
            .collect();
        let roots: Vec<String> = trace.dependencies(trace_of).roots.into_iter().collect();
        format!(
            "context=[{}] macros=[{}] roots=[{}]",
            context.join(", "),
            macros.join(", "),
            roots.join(", ")
        )
    };
    for conditional in &trace.conditionals {
        for (index, arm) in conditional.arms.iter().enumerate() {
            let state = match arm.state {
                ArmState::Taken => "taken",
                ArmState::NotTaken => "not-taken",
                ArmState::NotEvaluated => "not-evaluated",
            };
            let reads: Vec<String> = arm
                .reads
                .iter()
                .map(|read| {
                    let status = match (read.defined, read.event) {
                        (true, Some(event)) => {
                            format!("defined@{}", site(trace.event(event).loc))
                        }
                        (true, None) => "defined".to_string(),
                        (false, Some(event)) => {
                            format!("undefined@{}", site(trace.event(event).loc))
                        }
                        (false, None) => "undefined".to_string(),
                    };
                    format!("{}:{status}", read.name)
                })
                .collect();
            writeln!(
                out,
                "{} {} {} {state} names=[{}] reads=[{}]",
                if index == 0 { "if" } else { "arm" },
                arm.directive,
                site(arm.loc),
                arm.names.join(", "),
                reads.join(", ")
            )?;
        }
    }
    for expansion in &trace.expansions {
        writeln!(
            out,
            "expansion {} {}{} {}",
            expansion.name,
            site(expansion.site),
            if expansion.empty { " empty" } else { "" },
            describe(expansion.trace)
        )?;
    }
    let mut spellings = Spellings::new(files);
    for node in nodes {
        let (tokens, traces) = match &node.value {
            PPNodeKind::Comment { .. } => continue,
            PPNodeKind::Code { tokens, trace, .. } | PPNodeKind::Pragma { tokens, trace, .. } => {
                (tokens, trace)
            }
        };
        let mut start = 0;
        while start < tokens.len() {
            let current = traces[start];
            let line = site(tokens[start].expansion);
            let length = traces[start..]
                .iter()
                .zip(&tokens[start..])
                .take_while(|(trace, token)| **trace == current && site(token.expansion) == line)
                .count();
            writeln!(
                out,
                "tokens {line} `{}` {}",
                spellings.line(&tokens[start..start + length]),
                describe(current)
            )?;
            start += length;
        }
    }
    Ok(())
}

fn encode_line(line: &str, charset: Charset) -> Vec<u8> {
    let mut units = Vec::new();
    for c in line.chars().filter(|&c| raw_byte_for_char(c).is_none()) {
        charset.encode(u32::from(c), &mut units);
    }
    units.into_iter().map(|unit| unit as u8).collect()
}

struct Spellings<'a> {
    files: &'a Files,
    sources: HashMap<FileId, Option<String>>,
}

impl<'a> Spellings<'a> {
    fn new(files: &'a Files) -> Self {
        Self {
            files,
            sources: HashMap::new(),
        }
    }

    fn line(&mut self, tokens: &[Span<Token>]) -> String {
        let mut line = String::new();
        for (index, token) in tokens.iter().enumerate() {
            let spelling = self.spelling(token);
            if index > 0 && (token.leading_space || would_paste(&line, &spelling)) {
                line.push(' ');
            }
            line.push_str(&spelling);
        }
        line
    }

    fn spelling(&mut self, token: &Span<Token>) -> String {
        let canonical = token_spelling(token);
        if !matches!(
            token.value,
            Token::Keyword(_)
                | Token::Sizeof
                | Token::Alignof
                | Token::GnuAlignof
                | Token::Countof
                | Token::Maxof
                | Token::Minof
        ) {
            return canonical;
        }
        let loc = token.spelling;
        let files = self.files;
        let source = self.sources.entry(loc.file).or_insert_with(|| {
            let path = files.get_path(loc.file)?;
            std::fs::read(path)
                .ok()
                .map(|bytes| decode_source_bytes(&bytes))
        });
        source
            .as_deref()
            .and_then(|source| source.get(loc.offset..loc.offset + loc.length))
            .filter(|text| {
                text.bytes()
                    .all(|byte| byte == b'_' || byte.is_ascii_alphanumeric())
            })
            .map_or(canonical, str::to_string)
    }
}

// adjacent tokens with no leading space (from ## operands, _Pragma strings) must not relex as one
fn would_paste(before: &str, next: &str) -> bool {
    let (Some(last), Some(first)) = (before.chars().last(), next.chars().next()) else {
        return false;
    };
    let word = |c: char| c == '_' || c == '.' || c.is_ascii_alphanumeric();
    let number_before = before
        .rsplit(|c: char| !word(c))
        .next()
        .is_some_and(|tail| tail.starts_with(|c: char| c.is_ascii_digit() || c == '.'));
    (word(last) && word(first))
        || (number_before && matches!(last, 'e' | 'E' | 'p' | 'P') && matches!(first, '+' | '-'))
        || (last.is_ascii_punctuation()
            && first.is_ascii_punctuation()
            && !matches!(
                last,
                '(' | ')' | '[' | ']' | '{' | '}' | ',' | ';' | '"' | '\''
            )
            && !matches!(
                first,
                '(' | ')' | '[' | ']' | '{' | '}' | ',' | ';' | '"' | '\''
            ))
}
