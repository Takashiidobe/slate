use super::declarator::DeclaratorParser;
use super::span_tokens;
use crate::ast::{AsmLabel, Span, StorageClass};
use crate::lexer::{Keyword, Token};
use crate::target::x86::decode_register;

pub(super) fn is_asm_keyword(token: Option<&Token>) -> bool {
    matches!(token, Some(Token::Ident(name)) if matches!(name.as_str(), "asm" | "__asm" | "__asm__"))
}

impl DeclaratorParser<'_> {
    pub(super) fn parse_asm_label(
        &mut self,
        storage: StorageClass,
    ) -> Result<Option<Span<AsmLabel>>, String> {
        if !is_asm_keyword(self.peek()) {
            return Ok(None);
        }
        let start = self.pos;
        self.pos += 1;
        if let Some(Token::Keyword(
            qualifier @ (Keyword::Volatile | Keyword::Inline | Keyword::Goto),
        )) = self.peek()
        {
            let qualifier = match qualifier {
                Keyword::Volatile => "volatile",
                Keyword::Inline => "inline",
                _ => "goto",
            };
            return Err(format!("meaningless `{qualifier}` on asm outside function"));
        }
        if !self.matches(Token::LParen) {
            return Err("expected `(` after `asm`".into());
        }
        let first_string = self.pos;
        let mut label = String::new();
        while let Some(token) = self.peek() {
            match token {
                Token::StringLit(value) => label.push_str(value),
                Token::WideStringLit(_) => {
                    return Err("cannot use wide string literal in `asm`".into());
                }
                Token::Utf8StringLit(_) | Token::Utf16StringLit(_) | Token::Utf32StringLit(_) => {
                    return Err("cannot use unicode string literal in `asm`".into());
                }
                _ => break,
            }
            self.pos += 1;
        }
        if self.pos == first_string {
            return Err("expected string literal in `asm`".into());
        }
        if !self.matches(Token::RParen) {
            return Err("expected `)`".into());
        }
        let label = if storage == StorageClass::Register {
            AsmLabel::Register(decode_register(&label))
        } else {
            AsmLabel::Symbol(label)
        };
        Ok(Some(span_tokens(label, &self.tokens[start..self.pos])))
    }
}
