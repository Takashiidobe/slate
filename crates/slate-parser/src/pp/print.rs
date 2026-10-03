use super::{PPNode, PPNodeKind};
use crate::ast::{FileId, Span};
use crate::files::{Files, decode_source_bytes};
use crate::lexer::Token;
use std::collections::HashMap;
use std::io::{self, Write};

pub fn write_preprocessed(nodes: &[PPNode], files: &Files, out: &mut impl Write) -> io::Result<()> {
    let mut spellings = Spellings::new(files);
    for node in nodes {
        match &node.value {
            PPNodeKind::Comment { .. } => {}
            PPNodeKind::Code { tokens, .. } => {
                writeln!(out, "{}", spellings.line(tokens))?;
            }
            PPNodeKind::Pragma { tokens, .. } => {
                writeln!(out, "#pragma {}", spellings.line(tokens))?;
            }
        }
    }
    Ok(())
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
        let canonical = String::from(&token.value);
        if !matches!(
            token.value,
            Token::Keyword(_) | Token::Sizeof | Token::Alignof | Token::Countof
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
