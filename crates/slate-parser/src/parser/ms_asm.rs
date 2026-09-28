use super::decl::split_top_level;
use super::input::ParserInput;
use super::{Cursor, Parser, TokenCursor, span_tokens};
use crate::ast::{
    FileId, MsAsm, MsAsmBinaryOp, MsAsmExpr, MsAsmInstruction, MsAsmOperator, MsAsmSegment,
    MsAsmSize, Register, Span,
};
use crate::compiler_args::CompilerFlavor;
use crate::error::ParseError;
use crate::lexer::{Keyword, Token};
use crate::target::x86::decode_register;
use crate::target_info::{TargetEnvironment, TargetFamily};

const PREFIXES: &[&str] = &["lock", "rep", "repe", "repz", "repne", "repnz"];

fn is_ms_asm_keyword(token: Option<&Token>) -> bool {
    matches!(token, Some(Token::Ident(name)) if matches!(name.as_str(), "__asm" | "_asm"))
}

fn starts_ms_asm(tokens: &[Span<Token>], position: usize) -> bool {
    is_ms_asm_keyword(tokens.get(position).map(|token| &token.value))
        && !matches!(
            tokens.get(position + 1).map(|token| &token.value),
            None | Some(
                Token::LParen | Token::Keyword(Keyword::Volatile | Keyword::Inline | Keyword::Goto)
            )
        )
}

// a single-line `__asm` swallows a following line's `__asm`, but not a following line's `__asm {`.
fn continues_on_next_line(tokens: &[Span<Token>], position: usize) -> bool {
    starts_ms_asm(tokens, position)
        && tokens.get(position + 1).map(|token| &token.value) != Some(&Token::LBrace)
}

fn word(token: &Token) -> Option<&str> {
    match token {
        Token::Ident(name) => Some(name),
        Token::Keyword(keyword) => Some((*keyword).into()),
        _ => None,
    }
}

fn instruction_end(tokens: &[Span<Token>], start: usize) -> usize {
    tokens[start..]
        .iter()
        .position(|token| {
            matches!(token.value, Token::Newline | Token::LBrace | Token::RBrace)
                || is_ms_asm_keyword(Some(&token.value))
        })
        .map_or(tokens.len(), |offset| start + offset)
}

impl Parser {
    fn ms_asm_enabled(&self) -> bool {
        match self.flavor() {
            CompilerFlavor::Msvc => self.target.family == TargetFamily::X86,
            CompilerFlavor::Clang => {
                matches!(self.target.family, TargetFamily::X86 | TargetFamily::X86_64)
                    && self.target.environment == TargetEnvironment::Msvc
            }
            CompilerFlavor::Gcc => false,
        }
    }

    fn token_line(&self, token: &Span<Token>) -> (FileId, usize) {
        let loc = token.expansion;
        let line = self.line_starts.get(&loc.file).map_or(0, |starts| {
            starts
                .partition_point(|&start| start <= loc.offset)
                .saturating_sub(1)
        });
        (loc.file, line)
    }

    pub(super) fn mark_ms_asm_lines(&self, input: &mut ParserInput) {
        if !self.ms_asm_enabled()
            || !(0..input.tokens.len()).any(|position| starts_ms_asm(&input.tokens, position))
        {
            return;
        }
        let tokens = &input.tokens;
        let mut marked = Vec::with_capacity(tokens.len());
        let mut positions = Vec::with_capacity(tokens.len() + 1);
        let mut position = 0;
        while position < tokens.len() {
            if starts_ms_asm(tokens, position) {
                position =
                    self.mark_ms_asm_statement(tokens, position, &mut marked, &mut positions);
            } else {
                positions.push(marked.len());
                marked.push(tokens[position].clone());
                position += 1;
            }
        }
        positions.push(marked.len());
        input.replace_tokens(marked, &positions);
    }

    fn mark_ms_asm_statement(
        &self,
        tokens: &[Span<Token>],
        start: usize,
        marked: &mut Vec<Span<Token>>,
        positions: &mut Vec<usize>,
    ) -> usize {
        let braced = tokens.get(start + 1).map(|token| &token.value) == Some(&Token::LBrace);
        let opening = if braced { start + 2 } else { start + 1 };
        for token in &tokens[start..opening] {
            positions.push(marked.len());
            marked.push(token.clone());
        }
        let mut depth = usize::from(braced);
        let mut line = self.token_line(&tokens[opening - 1]);
        let mut position = opening;
        while let Some(token) = tokens.get(position) {
            let token_line = self.token_line(token);
            if token_line != line {
                if depth == 0 && !continues_on_next_line(tokens, position) {
                    break;
                }
                marked.push(token.clone().with_value(Token::Newline));
                line = token_line;
            }
            match token.value {
                Token::Semi => {
                    while tokens
                        .get(position)
                        .is_some_and(|token| self.token_line(token) == line)
                    {
                        positions.push(marked.len());
                        position += 1;
                    }
                    continue;
                }
                Token::LBrace => depth += 1,
                Token::RBrace if depth == 0 => break,
                Token::RBrace => {
                    depth -= 1;
                    if depth == 0 && braced {
                        positions.push(marked.len());
                        marked.push(token.clone());
                        return position + 1;
                    }
                }
                _ => {}
            }
            positions.push(marked.len());
            marked.push(token.clone());
            position += 1;
        }
        if let Some(last) = marked.last() {
            marked.push(last.clone().with_value(Token::Newline));
        }
        position
    }

    pub(super) fn parse_ms_asm_stmt(
        &self,
        cursor: &mut TokenCursor<'_, '_>,
    ) -> Result<Option<MsAsm>, ParseError> {
        let tokens = cursor.tokens;
        if !starts_ms_asm(tokens, cursor.pos) {
            return Ok(None);
        }
        if !self.ms_asm_enabled() {
            return match self.flavor() {
                CompilerFlavor::Msvc => {
                    Err(cursor.error("`__asm` is not supported on this architecture"))
                }
                CompilerFlavor::Clang | CompilerFlavor::Gcc => Ok(None),
            };
        }
        cursor.pos += 1;
        let braced = cursor.consume(Token::LBrace);
        let mut depth = usize::from(braced);
        let mut instructions = Vec::new();
        loop {
            match cursor.peek() {
                None if depth > 0 => return Err(cursor.error("expected `}`")),
                None => break,
                Some(Token::Newline) => {
                    cursor.pos += 1;
                    if depth == 0 && !continues_on_next_line(tokens, cursor.pos) {
                        break;
                    }
                }
                Some(Token::LBrace) => {
                    depth += 1;
                    cursor.pos += 1;
                }
                Some(Token::RBrace) if depth == 0 => break,
                Some(Token::RBrace) => {
                    depth -= 1;
                    cursor.pos += 1;
                    if depth == 0 && braced {
                        break;
                    }
                }
                token if is_ms_asm_keyword(token) => cursor.pos += 1,
                Some(_) => {
                    let end = instruction_end(tokens, cursor.pos);
                    instructions.extend(self.parse_ms_asm_line(&tokens[cursor.pos..end])?);
                    cursor.pos = end;
                }
            }
        }
        Ok(Some(MsAsm { instructions }))
    }

    fn parse_ms_asm_line(
        &self,
        tokens: &[Span<Token>],
    ) -> Result<Vec<Span<MsAsmInstruction>>, ParseError> {
        let mut instructions = Vec::new();
        let mut start = 0;
        let mut label = None;
        while tokens.get(start + 1).map(|token| &token.value) == Some(&Token::Colon)
            && let Some(name) = word(&tokens[start].value)
        {
            let name = span_tokens(name.to_string(), &tokens[start..=start], Some(self));
            if let Some(previous) = label.replace(name) {
                instructions.push(span_tokens(
                    MsAsmInstruction {
                        label: Some(previous),
                        prefixes: Vec::new(),
                        mnemonic: None,
                        operands: Vec::new(),
                    },
                    &tokens[start - 2..start],
                    Some(self),
                ));
            }
            start += 2;
        }
        let label_start = start - label.as_ref().map_or(0, |_| 2);
        let mut position = start;
        let mut prefixes = Vec::new();
        while position + 1 < tokens.len()
            && let Some(prefix) = word(&tokens[position].value)
            && PREFIXES.contains(&prefix.to_ascii_lowercase().as_str())
        {
            prefixes.push(span_tokens(
                prefix.to_string(),
                &tokens[position..=position],
                Some(self),
            ));
            position += 1;
        }
        let mnemonic = match tokens.get(position) {
            None => None,
            Some(token) => {
                let Some(mnemonic) = word(&token.value) else {
                    return Err(self.error_at_tokens(
                        tokens,
                        position,
                        "expected instruction mnemonic in `__asm`",
                    ));
                };
                position += 1;
                Some(span_tokens(
                    mnemonic.to_string(),
                    &tokens[position - 1..position],
                    Some(self),
                ))
            }
        };
        let mut operands = Vec::new();
        if tokens.last().map(|token| &token.value) == Some(&Token::Comma) {
            return Err(self.error_at_tokens(tokens, tokens.len() - 1, "expected `__asm` operand"));
        }
        if position < tokens.len() {
            for operand in split_top_level(&tokens[position..], &Token::Comma) {
                if operand.is_empty() {
                    return Err(self.error_at_tokens(tokens, position, "expected `__asm` operand"));
                }
                operands.push(OperandParser::new(self, operand).parse()?);
            }
        }
        if label.is_some() || mnemonic.is_some() {
            instructions.push(span_tokens(
                MsAsmInstruction {
                    label,
                    prefixes,
                    mnemonic,
                    operands,
                },
                &tokens[label_start..],
                Some(self),
            ));
        }
        Ok(instructions)
    }
}

struct OperandParser<'p, 'a> {
    parser: &'p Parser,
    tokens: &'a [Span<Token>],
    pos: usize,
}

type Operand = Box<Span<MsAsmExpr>>;

impl<'p, 'a> OperandParser<'p, 'a> {
    fn new(parser: &'p Parser, tokens: &'a [Span<Token>]) -> Self {
        Self {
            parser,
            tokens,
            pos: 0,
        }
    }

    fn parse(mut self) -> Result<Span<MsAsmExpr>, ParseError> {
        let operand = if self.peek_word_is("short") {
            self.pos += 1;
            let operand = self.additive()?;
            self.node(
                MsAsmExpr::Operator {
                    operator: MsAsmOperator::Short,
                    operand,
                },
                0,
            )
        } else {
            self.additive()?
        };
        if self.pos != self.tokens.len() {
            return Err(self.error("unexpected token in `__asm` operand"));
        }
        Ok(*operand)
    }

    fn error(&self, message: &str) -> ParseError {
        self.parser.error_at_tokens(self.tokens, self.pos, message)
    }

    fn peek(&self) -> Option<&'a Token> {
        self.tokens.get(self.pos).map(|token| &token.value)
    }

    fn peek_word(&self, offset: usize) -> Option<String> {
        self.tokens
            .get(self.pos + offset)
            .and_then(|token| word(&token.value))
            .map(str::to_ascii_lowercase)
    }

    fn peek_word_is(&self, expected: &str) -> bool {
        self.peek_word(0).as_deref() == Some(expected)
    }

    fn expect(&mut self, token: Token, message: &str) -> Result<(), ParseError> {
        if self.peek() == Some(&token) {
            self.pos += 1;
            Ok(())
        } else {
            Err(self.error(message))
        }
    }

    fn node(&self, value: MsAsmExpr, start: usize) -> Operand {
        Box::new(span_tokens(
            value,
            &self.tokens[start..self.pos],
            Some(self.parser),
        ))
    }

    fn additive(&mut self) -> Result<Operand, ParseError> {
        let start = self.pos;
        let mut lhs = self.multiplicative()?;
        loop {
            let op = match self.peek() {
                Some(Token::Plus) => MsAsmBinaryOp::Add,
                Some(Token::Minus) => MsAsmBinaryOp::Sub,
                _ => return Ok(lhs),
            };
            self.pos += 1;
            let rhs = self.multiplicative()?;
            lhs = self.node(MsAsmExpr::Binary { op, lhs, rhs }, start);
        }
    }

    fn multiplicative(&mut self) -> Result<Operand, ParseError> {
        let start = self.pos;
        let mut lhs = self.unary()?;
        loop {
            let op = match self.peek() {
                Some(Token::Star) => MsAsmBinaryOp::Mul,
                Some(Token::Slash) => MsAsmBinaryOp::Div,
                _ => return Ok(lhs),
            };
            self.pos += 1;
            let rhs = self.unary()?;
            lhs = self.node(MsAsmExpr::Binary { op, lhs, rhs }, start);
        }
    }

    fn unary(&mut self) -> Result<Operand, ParseError> {
        let start = self.pos;
        match self.peek() {
            Some(Token::Minus) => {
                self.pos += 1;
                let operand = self.unary()?;
                Ok(self.node(MsAsmExpr::Negate(operand), start))
            }
            Some(Token::Plus) => {
                self.pos += 1;
                self.unary()
            }
            _ => self.prefixed(),
        }
    }

    fn prefixed(&mut self) -> Result<Operand, ParseError> {
        let start = self.pos;
        if let Some(size) = self.peek_word(0).as_deref().and_then(size)
            && self.peek_word(1).as_deref() == Some("ptr")
        {
            self.pos += 2;
            let operand = self.prefixed()?;
            return Ok(self.node(MsAsmExpr::Ptr { size, operand }, start));
        }
        if let Some(operator) = self.peek_word(0).as_deref().and_then(operator) {
            self.pos += 1;
            if operator == MsAsmOperator::Type
                && self.parser.flavor() == CompilerFlavor::Msvc
                && let Some(Token::Keyword(keyword)) = self.peek()
                && is_type_keyword(*keyword)
            {
                let keyword_start = self.pos;
                self.pos += 1;
                let operand = self.node(MsAsmExpr::TypeKeyword(*keyword), keyword_start);
                return Ok(self.node(MsAsmExpr::Operator { operator, operand }, start));
            }
            let operand = self.prefixed()?;
            return Ok(self.node(MsAsmExpr::Operator { operator, operand }, start));
        }
        if let Some(segment) = self.peek_word(0).as_deref().and_then(segment)
            && self.tokens.get(self.pos + 1).map(|token| &token.value) == Some(&Token::Colon)
        {
            self.pos += 2;
            let operand = self.prefixed()?;
            return Ok(self.node(MsAsmExpr::Segment { segment, operand }, start));
        }
        self.postfix()
    }

    fn postfix(&mut self) -> Result<Operand, ParseError> {
        let start = self.pos;
        let mut base = self.primary()?;
        loop {
            match self.peek() {
                Some(Token::LBracket) => {
                    self.pos += 1;
                    let index = self.additive()?;
                    self.expect(Token::RBracket, "expected `]` in `__asm` operand")?;
                    base = self.node(MsAsmExpr::Index { base, index }, start);
                }
                Some(Token::Dot) => {
                    self.pos += 1;
                    let Some(Token::Ident(field)) = self.peek() else {
                        return Err(self.error("expected field name in `__asm` operand"));
                    };
                    self.pos += 1;
                    let field = span_tokens(
                        field.to_string(),
                        &self.tokens[self.pos - 1..self.pos],
                        Some(self.parser),
                    );
                    base = self.node(MsAsmExpr::Member { base, field }, start);
                }
                Some(Token::Ident(_))
                    if matches!(self.tokens[self.pos - 1].value, Token::RBracket) =>
                {
                    let rhs = self.postfix()?;
                    return Ok(self.node(
                        MsAsmExpr::Binary {
                            op: MsAsmBinaryOp::Add,
                            lhs: base,
                            rhs,
                        },
                        start,
                    ));
                }
                _ => return Ok(base),
            }
        }
    }

    fn primary(&mut self) -> Result<Operand, ParseError> {
        let start = self.pos;
        let Some(token) = self.peek() else {
            return Err(self.error("expected `__asm` operand"));
        };
        let value = match token {
            Token::LBracket => {
                self.pos += 1;
                let inner = self.additive()?;
                self.expect(Token::RBracket, "expected `]` in `__asm` operand")?;
                MsAsmExpr::Bracket(inner)
            }
            Token::LParen => {
                self.pos += 1;
                let inner = self.additive()?;
                self.expect(Token::RParen, "expected `)` in `__asm` operand")?;
                return Ok(inner);
            }
            Token::IntLit(spelling) | Token::FloatLit(spelling) => {
                let Some(value) = number(spelling) else {
                    return Err(self.error("invalid number in `__asm` operand"));
                };
                self.pos += 1;
                MsAsmExpr::Number(value)
            }
            Token::Ident(name) => {
                self.pos += 1;
                let lower = name.to_ascii_lowercase();
                if lower == "st" {
                    self.st()?
                } else if let Some(segment) = segment(&lower) {
                    MsAsmExpr::SegmentRegister(segment)
                } else if let Some(register) = register(name, &lower) {
                    MsAsmExpr::Register(register)
                } else {
                    MsAsmExpr::Name(name.to_string())
                }
            }
            _ => return Err(self.error("expected `__asm` operand")),
        };
        Ok(self.node(value, start))
    }

    fn st(&mut self) -> Result<MsAsmExpr, ParseError> {
        if self.peek() != Some(&Token::LParen) {
            return Ok(MsAsmExpr::St(0));
        }
        self.pos += 1;
        let index = match self.peek() {
            Some(Token::IntLit(spelling)) => number(spelling)
                .and_then(|index| u8::try_from(index).ok())
                .filter(|index| *index < 8),
            _ => None,
        };
        let Some(index) = index else {
            return Err(self.error("expected x87 stack register index 0-7"));
        };
        self.pos += 1;
        self.expect(Token::RParen, "expected `)` after x87 stack register index")?;
        Ok(MsAsmExpr::St(index))
    }
}

fn size(name: &str) -> Option<MsAsmSize> {
    Some(match name {
        "byte" => MsAsmSize::Byte,
        "word" => MsAsmSize::Word,
        "dword" => MsAsmSize::Dword,
        "fword" => MsAsmSize::Fword,
        "qword" => MsAsmSize::Qword,
        "tbyte" => MsAsmSize::Tbyte,
        "mmword" => MsAsmSize::Mmword,
        "xmmword" => MsAsmSize::Xmmword,
        "ymmword" => MsAsmSize::Ymmword,
        "zmmword" => MsAsmSize::Zmmword,
        "oword" => MsAsmSize::Oword,
        "real4" => MsAsmSize::Real4,
        "real8" => MsAsmSize::Real8,
        "real10" => MsAsmSize::Real10,
        _ => return None,
    })
}

fn is_type_keyword(keyword: Keyword) -> bool {
    matches!(
        keyword,
        Keyword::Char
            | Keyword::Short
            | Keyword::Int
            | Keyword::Long
            | Keyword::Int64
            | Keyword::Float
            | Keyword::Double
            | Keyword::Signed
            | Keyword::Unsigned
    )
}

fn operator(name: &str) -> Option<MsAsmOperator> {
    Some(match name {
        "offset" => MsAsmOperator::Offset,
        "type" => MsAsmOperator::Type,
        "length" => MsAsmOperator::Length,
        "size" => MsAsmOperator::Size,
        _ => return None,
    })
}

fn segment(name: &str) -> Option<MsAsmSegment> {
    Some(match name {
        "es" => MsAsmSegment::Es,
        "cs" => MsAsmSegment::Cs,
        "ss" => MsAsmSegment::Ss,
        "ds" => MsAsmSegment::Ds,
        "fs" => MsAsmSegment::Fs,
        "gs" => MsAsmSegment::Gs,
        _ => return None,
    })
}

fn register(spelling: &str, lower: &str) -> Option<Register> {
    match decode_register(lower) {
        Register::X86(mut info) if !(8..=21).contains(&info.number) => {
            info.spelling = spelling.to_string();
            Some(Register::X86(info))
        }
        _ => None,
    }
}

fn number(spelling: &str) -> Option<u64> {
    let lower = spelling.to_ascii_lowercase();
    let c_integer = |digits: &str, radix: u32| {
        u64::from_str_radix(digits.trim_end_matches(['u', 'l']), radix).ok()
    };
    if let Some(hex) = lower.strip_prefix("0x") {
        return c_integer(hex, 16);
    }
    let (digits, suffix) = lower.split_at(lower.len().checked_sub(1)?);
    let radix = match suffix {
        "h" => 16,
        "b" | "y" => 2,
        "o" | "q" => 8,
        "d" | "t" => 10,
        _ if lower.len() > 1 && lower.starts_with('0') => return c_integer(&lower[1..], 8),
        _ => return c_integer(&lower, 10),
    };
    u64::from_str_radix(digits, radix).ok()
}
