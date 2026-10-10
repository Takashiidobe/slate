use super::Preprocessor;
use super::syntax::{Directive, DirectiveName};
use crate::ast::{Loc, Span};
use crate::diagnostics::Warning;
use crate::lexer::Token;

impl Preprocessor<'_> {
    pub(super) fn warn_traditional(&mut self, loc: Loc, message: String) {
        if self.dialect.flavor().is_gcc() {
            self.warn(loc, Warning::Traditional, message, false);
        }
    }

    pub(super) fn warn_traditional_directive(&mut self, directive: &Directive) {
        let name = directive.name;
        if !self.dialect.flavor().is_gcc()
            || matches!(
                name,
                DirectiveName::Null | DirectiveName::LineMarker | DirectiveName::Unknown
            )
        {
            return;
        }
        let spelling = self
            .source(directive.loc.file)
            .get(directive.name_loc.offset..directive.name_loc.offset + directive.name_loc.length)
            .unwrap_or_default()
            .to_owned();
        let message = if name == DirectiveName::Elif {
            "suggest not using '#elif' in traditional C".to_owned()
        } else if name.is_k_and_r() {
            if !directive.indented {
                return;
            }
            format!("traditional C ignores '#{spelling}' with the '#' indented")
        } else {
            if directive.indented {
                return;
            }
            format!("suggest hiding '#{spelling}' from traditional C with an indented '#'")
        };
        self.warn_traditional(directive.name_loc, message);
    }

    pub(super) fn warn_traditional_literal(&mut self, token: &Span<Token>) {
        if !self.dialect.flavor().is_gcc() {
            return;
        }
        let (text, prefix_len) = match &token.value {
            Token::IntLit(text) => {
                if let Some(suffix) = unsigned_suffix(text) {
                    self.warn_traditional(
                        token.expansion,
                        format!("traditional C rejects the '{suffix}' suffix"),
                    );
                }
                return;
            }
            Token::StringLit(text) | Token::CharLit(text, _) => (text, 0),
            Token::Utf16StringLit(text)
            | Token::Utf32StringLit(text)
            | Token::WideStringLit(text)
            | Token::Utf16CharLit(text, _)
            | Token::Utf32CharLit(text, _)
            | Token::WideCharLit(text, _) => (text, 1),
            Token::Utf8StringLit(text) | Token::Utf8CharLit(text, _) => (text, 2),
            _ => return,
        };
        if !text.contains('\\') {
            return;
        }
        let mut chars = text.char_indices();
        while let Some((index, c)) = chars.next() {
            if c != '\\' {
                continue;
            }
            let Some((_, escape)) = chars.next() else {
                break;
            };
            if !matches!(escape, 'x' | 'a' | 'u' | 'U') {
                continue;
            }
            let loc = if token.spelling == token.expansion {
                Loc::new(
                    token.spelling.file,
                    token.spelling.offset + prefix_len + 1 + index,
                    2,
                )
            } else {
                token.expansion
            };
            self.warn_traditional(
                loc,
                format!("the meaning of '\\{escape}' is different in traditional C"),
            );
        }
    }
}

impl DirectiveName {
    fn is_k_and_r(self) -> bool {
        matches!(
            self,
            Self::Define
                | Self::Undef
                | Self::Include
                | Self::If
                | Self::Ifdef
                | Self::Ifndef
                | Self::Else
                | Self::Endif
                | Self::Line
        )
    }
}

fn unsigned_suffix(spelling: &str) -> Option<&str> {
    let start = spelling.find(['u', 'U', 'l', 'L', 'w', 'W', 'z', 'Z'])?;
    let suffix = &spelling[start..];
    suffix.contains(['u', 'U']).then_some(suffix)
}
