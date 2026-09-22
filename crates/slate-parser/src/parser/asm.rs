use super::decl::matching_paren;
use super::declarator::DeclaratorParser;
use super::{Cursor, Parser, TokenCursor, span_tokens};
use crate::ast::{
    AsmClobber, AsmConstraint, AsmConstraintAlternative, AsmConstraintLocation,
    AsmConstraintModifier, AsmLabel, AsmOperand, AsmOperands, AsmQualifier, AsmTemplatePiece, Expr,
    GnuAsm, Span, StorageClass,
};
use crate::compiler_args::CompilerFlavor;
use crate::error::ParseError;
use crate::lexer::{Keyword, Token};
use crate::target::x86::decode_register;

pub(super) fn is_asm_keyword(token: Option<&Token>) -> bool {
    matches!(token, Some(Token::Ident(name)) if matches!(name.as_str(), "asm" | "__asm" | "__asm__"))
}

fn asm_qualifier(token: Option<&Token>) -> Option<AsmQualifier> {
    match token {
        Some(Token::Keyword(Keyword::Volatile)) => Some(AsmQualifier::Volatile),
        Some(Token::Keyword(Keyword::Inline)) => Some(AsmQualifier::Inline),
        Some(Token::Keyword(Keyword::Goto)) => Some(AsmQualifier::Goto),
        _ => None,
    }
}

fn qualifier_spelling(qualifier: AsmQualifier) -> &'static str {
    match qualifier {
        AsmQualifier::Volatile => "volatile",
        AsmQualifier::Inline => "inline",
        AsmQualifier::Goto => "goto",
    }
}

fn is_string_literal(token: Option<&Token>) -> bool {
    matches!(
        token,
        Some(
            Token::StringLit(_)
                | Token::WideStringLit(_)
                | Token::Utf8StringLit(_)
                | Token::Utf16StringLit(_)
                | Token::Utf32StringLit(_)
        )
    )
}

fn asm_string(tokens: &[Span<Token>], pos: &mut usize) -> Result<Span<String>, String> {
    let start = *pos;
    let mut value = String::new();
    while let Some(token) = tokens.get(*pos) {
        match &token.value {
            Token::StringLit(piece) => value.push_str(piece),
            Token::WideStringLit(_) => {
                return Err("cannot use wide string literal in `asm`".into());
            }
            Token::Utf8StringLit(_) | Token::Utf16StringLit(_) | Token::Utf32StringLit(_) => {
                return Err("cannot use unicode string literal in `asm`".into());
            }
            _ => break,
        }
        *pos += 1;
    }
    if *pos == start {
        return Err("expected string literal in `asm`".into());
    }
    Ok(span_tokens(value, &tokens[start..*pos], None))
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
        if let Some(qualifier) = asm_qualifier(self.peek()) {
            return Err(format!(
                "meaningless `{}` on asm outside function",
                qualifier_spelling(qualifier)
            ));
        }
        if !self.matches(Token::LParen) {
            return Err("expected `(` after `asm`".into());
        }
        let tokens = self.tokens;
        let label = asm_string(tokens, &mut self.pos)?.value;
        if !self.matches(Token::RParen) {
            return Err("expected `)`".into());
        }
        let label = if storage == StorageClass::Register {
            AsmLabel::Register(decode_register(&label))
        } else {
            AsmLabel::Symbol(label)
        };
        Ok(Some(span_tokens(
            label,
            &tokens[start..self.pos],
            self.context,
        )))
    }
}

struct RawOperand {
    name: Option<Span<String>>,
    constraint: Span<String>,
    constraint_pos: usize,
    expr: Expr,
}

impl Parser {
    pub(super) fn parse_file_scope_asm(
        &self,
        tokens: &[Span<Token>],
    ) -> Result<Option<GnuAsm>, ParseError> {
        let mut cursor = TokenCursor::new(self, tokens, 0);
        let Some(asm) = self.parse_asm(&mut cursor, true)? else {
            return Ok(None);
        };
        if cursor.pos != tokens.len() {
            return Err(cursor.error("expected `;` after top-level asm block"));
        }
        Ok(Some(asm))
    }

    pub(super) fn parse_asm_stmt(
        &self,
        cursor: &mut TokenCursor<'_, '_>,
    ) -> Result<Option<GnuAsm>, ParseError> {
        self.parse_asm(cursor, false)
    }

    fn parse_asm(
        &self,
        cursor: &mut TokenCursor<'_, '_>,
        at_file_scope: bool,
    ) -> Result<Option<GnuAsm>, ParseError> {
        if !is_asm_keyword(cursor.peek()) {
            return Ok(None);
        }
        let tokens = cursor.tokens;
        let mut position = cursor.pos + 1;
        let mut qualifiers = Vec::new();
        while let Some(qualifier) = asm_qualifier(tokens.get(position).map(|token| &token.value)) {
            qualifiers.push(span_tokens(
                qualifier,
                &tokens[position..=position],
                Some(self),
            ));
            position += 1;
        }
        if tokens.get(position).map(|token| &token.value) != Some(&Token::LParen) {
            return Ok(None);
        }
        if at_file_scope {
            if !is_string_literal(tokens.get(position + 1).map(|token| &token.value)) {
                return Ok(None);
            }
            if let Some(qualifier) = qualifiers.first() {
                return Err(self.error_at_tokens(
                    tokens,
                    cursor.pos + 1,
                    format!(
                        "meaningless `{}` on asm outside function",
                        qualifier_spelling(qualifier.value)
                    ),
                ));
            }
        }
        let is_goto = qualifiers
            .iter()
            .any(|qualifier| qualifier.value == AsmQualifier::Goto);
        cursor.pos = position + 1;
        let template_pos = cursor.pos;
        let template = asm_string(tokens, &mut cursor.pos).map_err(|error| cursor.error(error))?;
        if is_goto && self.flavor() == CompilerFlavor::Gcc && cursor.peek() == Some(&Token::RParen)
        {
            return Err(cursor.error("expected `:`"));
        }
        if at_file_scope
            && self.flavor() == CompilerFlavor::Clang
            && cursor.peek() != Some(&Token::RParen)
        {
            return Err(cursor.error("expected `)`"));
        }
        if cursor.consume(Token::RParen) {
            cursor.consume(Token::Semi);
            return Ok(Some(GnuAsm {
                qualifiers,
                template,
                operands: None,
            }));
        }

        let mut outputs = Vec::new();
        let mut inputs = Vec::new();
        let mut clobbers = Vec::new();
        let mut labels = Vec::new();
        if cursor.consume(Token::Colon) {
            outputs = self.parse_asm_operands(cursor)?;
        }
        if cursor.consume(Token::Colon) {
            inputs = self.parse_asm_operands(cursor)?;
        }
        if cursor.consume(Token::Colon) && is_string_literal(cursor.peek()) {
            loop {
                let clobber =
                    asm_string(tokens, &mut cursor.pos).map_err(|error| cursor.error(error))?;
                clobbers.push(Span::new(
                    decode_clobber(&clobber.value),
                    clobber.spelling,
                    clobber.expansion,
                ));
                if !cursor.consume(Token::Comma) {
                    break;
                }
            }
        }
        if !is_goto && cursor.peek() != Some(&Token::RParen) {
            return Err(cursor.error("expected `)`"));
        }
        if cursor.consume(Token::Colon) {
            loop {
                let start = cursor.pos;
                let label = cursor.expect_ident("expected identifier")?;
                labels.push(span_tokens(label, &tokens[start..cursor.pos], Some(self)));
                if !cursor.consume(Token::Comma) {
                    break;
                }
            }
        } else if is_goto {
            return Err(cursor.error("expected `:`"));
        }
        cursor.expect(Token::RParen, "expected `)`")?;
        cursor.consume(Token::Semi);

        let output_names = operand_names(&outputs);
        let mut names = output_names.clone();
        names.extend(operand_names(&inputs));
        let operand_count = names.len();
        names.extend(labels.iter().map(|label| Some(label.value.as_str())));
        let pieces = analyze_template(&template.value, &names, operand_count)
            .map_err(|error| self.error_at_tokens(tokens, template_pos, error))?;
        let resolve = |operand: &RawOperand, direction: &str| {
            let constraint = decode_constraint(&operand.constraint.value, &output_names);
            if self.flavor() == CompilerFlavor::Clang
                && constraint.alternatives.iter().any(|alternative| {
                    matches!(alternative.location, AsmConstraintLocation::HardRegister(_))
                })
            {
                return Err(self.error_at_tokens(
                    tokens,
                    operand.constraint_pos,
                    format!(
                        "invalid {direction} constraint '{}' in asm",
                        operand.constraint.value
                    ),
                ));
            }
            Ok(AsmOperand {
                name: operand.name.clone(),
                constraint: Span::new(
                    constraint,
                    operand.constraint.spelling,
                    operand.constraint.expansion,
                ),
                expr: operand.expr.clone(),
            })
        };
        let outputs = outputs
            .iter()
            .map(|operand| resolve(operand, "output"))
            .collect::<Result<_, _>>()?;
        let inputs = inputs
            .iter()
            .map(|operand| resolve(operand, "input"))
            .collect::<Result<_, _>>()?;
        Ok(Some(GnuAsm {
            qualifiers,
            template,
            operands: Some(AsmOperands {
                pieces,
                outputs,
                inputs,
                clobbers,
                labels,
            }),
        }))
    }

    fn parse_asm_operands(
        &self,
        cursor: &mut TokenCursor<'_, '_>,
    ) -> Result<Vec<RawOperand>, ParseError> {
        let mut operands = Vec::new();
        if matches!(cursor.peek(), Some(Token::Colon | Token::RParen)) {
            return Ok(operands);
        }
        let tokens = cursor.tokens;
        loop {
            let name = if cursor.consume(Token::LBracket) {
                let start = cursor.pos;
                let name = cursor.expect_ident("expected identifier")?;
                let name = span_tokens(name, &tokens[start..cursor.pos], Some(self));
                cursor.expect(Token::RBracket, "expected `]`")?;
                Some(name)
            } else {
                None
            };
            let constraint_pos = cursor.pos;
            let constraint =
                asm_string(tokens, &mut cursor.pos).map_err(|error| cursor.error(error))?;
            if cursor.peek() != Some(&Token::LParen) {
                return Err(cursor.error("expected `(` after asm operand"));
            }
            let close =
                matching_paren(tokens, cursor.pos).ok_or_else(|| cursor.error("expected `)`"))?;
            let expr = self.parse_expression(&tokens[cursor.pos + 1..close])?;
            cursor.pos = close + 1;
            operands.push(RawOperand {
                name,
                constraint,
                constraint_pos,
                expr,
            });
            if !cursor.consume(Token::Comma) {
                return Ok(operands);
            }
        }
    }
}

fn operand_names(operands: &[RawOperand]) -> Vec<Option<&str>> {
    operands
        .iter()
        .map(|operand| operand.name.as_ref().map(|name| name.value.as_str()))
        .collect()
}

fn decode_clobber(name: &str) -> AsmClobber {
    match name {
        "memory" => AsmClobber::Memory,
        "cc" => AsmClobber::Cc,
        "unwind" => AsmClobber::Unwind,
        _ => AsmClobber::Register(decode_register(name)),
    }
}

fn analyze_template(
    template: &str,
    names: &[Option<&str>],
    operand_count: usize,
) -> Result<Vec<AsmTemplatePiece>, String> {
    let mut pieces = Vec::new();
    let mut text = String::new();
    let mut rest = template;
    while let Some(percent) = rest.find('%') {
        text.push_str(&rest[..percent]);
        rest = &rest[percent + 1..];
        let mut chars = rest.chars();
        let escaped = chars
            .next()
            .ok_or("invalid % escape in inline assembly string")?;
        let simple = match escaped {
            '%' => Some(AsmTemplatePiece::Percent),
            '{' => Some(AsmTemplatePiece::LBrace),
            '|' => Some(AsmTemplatePiece::Pipe),
            '}' => Some(AsmTemplatePiece::RBrace),
            '=' => Some(AsmTemplatePiece::UniqueId),
            _ => None,
        };
        if !text.is_empty() {
            pieces.push(AsmTemplatePiece::Text(std::mem::take(&mut text)));
        }
        if let Some(piece) = simple {
            pieces.push(piece);
            rest = chars.as_str();
            continue;
        }
        let modifier = escaped.is_ascii_alphabetic().then_some(escaped);
        if modifier.is_some() {
            rest = chars.as_str();
            if escaped == 'c' {
                rest = rest.strip_prefix('c').unwrap_or(rest);
            }
        }
        let digits = rest.bytes().take_while(u8::is_ascii_digit).count();
        let index = if digits > 0 {
            let index = rest[..digits]
                .parse::<usize>()
                .ok()
                .filter(|&index| index < names.len())
                .ok_or("invalid operand number in inline asm string")?;
            rest = &rest[digits..];
            index
        } else if let Some(symbolic) = rest.strip_prefix('[') {
            let close = symbolic
                .find(']')
                .ok_or("unterminated symbolic operand name in inline asm string")?;
            let name = &symbolic[..close];
            if name.is_empty() {
                return Err("empty symbolic operand name in inline asm string".into());
            }
            rest = &symbolic[close + 1..];
            names
                .iter()
                .position(|candidate| *candidate == Some(name))
                .ok_or_else(|| {
                    format!("unknown symbolic operand name in inline asm string: `{name}`")
                })?
        } else {
            return Err("invalid % escape in inline assembly string".into());
        };
        pieces.push(if index >= operand_count {
            AsmTemplatePiece::Label(index - operand_count)
        } else if modifier == Some('l') {
            return Err("`%l` operand isn't a label".into());
        } else {
            AsmTemplatePiece::Operand { index, modifier }
        });
    }
    text.push_str(rest);
    if !text.is_empty() {
        pieces.push(AsmTemplatePiece::Text(text));
    }
    Ok(pieces)
}

fn decode_constraint(spelling: &str, output_names: &[Option<&str>]) -> AsmConstraint {
    AsmConstraint {
        alternatives: spelling
            .split(',')
            .map(|alternative| decode_alternative(alternative, output_names))
            .collect(),
    }
}

fn decode_alternative(spelling: &str, output_names: &[Option<&str>]) -> AsmConstraintAlternative {
    let mut modifiers = Vec::new();
    let mut body = String::new();
    for c in spelling.chars() {
        match c {
            '=' => modifiers.push(AsmConstraintModifier::Overwrite),
            '+' => modifiers.push(AsmConstraintModifier::ReadWrite),
            '&' => modifiers.push(AsmConstraintModifier::EarlyClobber),
            '%' => modifiers.push(AsmConstraintModifier::Commutative),
            '-' => modifiers.push(AsmConstraintModifier::Pic),
            _ => body.push(c),
        }
    }
    let location = if let Some(register) = body
        .strip_prefix('{')
        .and_then(|rest| rest.strip_suffix('}'))
    {
        AsmConstraintLocation::HardRegister(decode_register(register))
    } else if !body.is_empty()
        && body.bytes().all(|byte| byte.is_ascii_digit())
        && let Ok(index) = body.parse()
    {
        AsmConstraintLocation::Matching(index)
    } else if let Some(index) = body
        .strip_prefix('[')
        .and_then(|rest| rest.strip_suffix(']'))
        .and_then(|name| {
            output_names
                .iter()
                .position(|candidate| *candidate == Some(name))
        })
    {
        AsmConstraintLocation::Matching(index)
    } else {
        AsmConstraintLocation::Letters(body)
    };
    AsmConstraintAlternative {
        modifiers,
        location,
    }
}
