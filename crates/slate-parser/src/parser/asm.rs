use super::decl::matching_paren;
use super::declarator::DeclaratorParser;
use super::{Cursor, Fragment, Parser, span_tokens};
use crate::ast::{
    AsmClobber, AsmConstraint, AsmConstraintAlternative, AsmConstraintLocation,
    AsmConstraintModifier, AsmLabel, AsmOperand, AsmOperands, AsmQualifier, AsmTemplatePiece,
    GnuAsm, Span, SpannedExpr, StorageClass,
};
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
    Ok(span_tokens(value, &tokens[start..*pos]))
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
        Ok(Some(span_tokens(label, &tokens[start..self.pos])))
    }
}

struct RawOperand {
    name: Option<Span<String>>,
    constraint: Span<String>,
    expr: SpannedExpr,
}

impl Parser {
    pub(super) fn parse_asm_stmt(
        &self,
        fragment: &mut Fragment<'_, '_>,
    ) -> Result<Option<GnuAsm>, ParseError> {
        if !is_asm_keyword(fragment.peek()) {
            return Ok(None);
        }
        let tokens = fragment.tokens;
        let mut cursor = fragment.pos + 1;
        let mut qualifiers = Vec::new();
        while let Some(qualifier) = asm_qualifier(tokens.get(cursor).map(|token| &token.value)) {
            qualifiers.push(span_tokens(qualifier, &tokens[cursor..=cursor]));
            cursor += 1;
        }
        if tokens.get(cursor).map(|token| &token.value) != Some(&Token::LParen) {
            return Ok(None);
        }
        fragment.pos = cursor + 1;
        let template_pos = fragment.pos;
        let template =
            asm_string(tokens, &mut fragment.pos).map_err(|error| fragment.error(error))?;
        if fragment.consume(Token::RParen) {
            fragment.consume(Token::Semi);
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
        if fragment.consume(Token::Colon) {
            outputs = self.parse_asm_operands(fragment)?;
        }
        if fragment.consume(Token::Colon) {
            inputs = self.parse_asm_operands(fragment)?;
        }
        if fragment.consume(Token::Colon) && is_string_literal(fragment.peek()) {
            loop {
                let clobber =
                    asm_string(tokens, &mut fragment.pos).map_err(|error| fragment.error(error))?;
                clobbers.push(Span::new(
                    decode_clobber(&clobber.value),
                    clobber.spelling,
                    clobber.expansion,
                ));
                if !fragment.consume(Token::Comma) {
                    break;
                }
            }
        }
        let is_goto = qualifiers
            .iter()
            .any(|qualifier| qualifier.value == AsmQualifier::Goto);
        if !is_goto && fragment.peek() != Some(&Token::RParen) {
            return Err(fragment.error("expected `)`"));
        }
        if fragment.consume(Token::Colon) {
            loop {
                let start = fragment.pos;
                let label = fragment.expect_ident("expected identifier")?;
                labels.push(span_tokens(label, &tokens[start..fragment.pos]));
                if !fragment.consume(Token::Comma) {
                    break;
                }
            }
        } else if is_goto {
            return Err(fragment.error("expected `:`"));
        }
        fragment.expect(Token::RParen, "expected `)`")?;
        fragment.consume(Token::Semi);

        let output_names = operand_names(&outputs);
        let mut names = output_names.clone();
        names.extend(operand_names(&inputs));
        let operand_count = names.len();
        names.extend(labels.iter().map(|label| Some(label.value.as_str())));
        let pieces = analyze_template(&template.value, &names, operand_count)
            .map_err(|error| self.error_at_tokens(tokens, template_pos, error))?;
        let resolve = |operand: &RawOperand| AsmOperand {
            name: operand.name.clone(),
            constraint: Span::new(
                decode_constraint(&operand.constraint.value, &output_names),
                operand.constraint.spelling,
                operand.constraint.expansion,
            ),
            expr: operand.expr.clone(),
        };
        let outputs = outputs.iter().map(resolve).collect();
        let inputs = inputs.iter().map(resolve).collect();
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
        fragment: &mut Fragment<'_, '_>,
    ) -> Result<Vec<RawOperand>, ParseError> {
        let mut operands = Vec::new();
        if matches!(fragment.peek(), Some(Token::Colon | Token::RParen)) {
            return Ok(operands);
        }
        let tokens = fragment.tokens;
        loop {
            let name = if fragment.consume(Token::LBracket) {
                let start = fragment.pos;
                let name = fragment.expect_ident("expected identifier")?;
                let name = span_tokens(name, &tokens[start..fragment.pos]);
                fragment.expect(Token::RBracket, "expected `]`")?;
                Some(name)
            } else {
                None
            };
            let constraint =
                asm_string(tokens, &mut fragment.pos).map_err(|error| fragment.error(error))?;
            if fragment.peek() != Some(&Token::LParen) {
                return Err(fragment.error("expected `(` after asm operand"));
            }
            let close = matching_paren(tokens, fragment.pos)
                .ok_or_else(|| fragment.error("expected `)`"))?;
            let expr = self.parse_expression(fragment.code, &tokens[fragment.pos + 1..close])?;
            fragment.pos = close + 1;
            operands.push(RawOperand {
                name,
                constraint,
                expr,
            });
            if !fragment.consume(Token::Comma) {
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
