#[derive(Debug, Clone, PartialEq)]
pub enum ConstExpr {
    Integer(i64),
    Unary {
        op: UnaryOp,
        value: Box<Self>,
    },
    Binary {
        op: BinaryOp,
        left: Box<Self>,
        right: Box<Self>,
    },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum UnaryOp {
    Plus,
    Minus,
    BitNot,
    Not,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum BinaryOp {
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Equal,
    NotEqual,
    BitAnd,
    BitXor,
    BitOr,
    And,
    Or,
    ShiftLeft,
    ShiftRight,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ConstExprError(pub String);

pub fn parse(source: &str) -> Result<ConstExpr, ConstExprError> {
    let mut parser = Parser::new(source)?;
    let expression = parser.parse_binary(0)?;
    if parser.peek().is_some() {
        return Err(ConstExprError("unexpected tokens after expression".into()));
    }
    Ok(expression)
}

pub fn evaluate(source: &str) -> Result<i64, ConstExprError> {
    evaluate_expr(&parse(source)?)
}

pub fn evaluate_expr(expression: &ConstExpr) -> Result<i64, ConstExprError> {
    match expression {
        ConstExpr::Integer(value) => Ok(*value),
        ConstExpr::Unary { op, value } => {
            let value = evaluate_expr(value)?;
            match op {
                UnaryOp::Plus => Ok(value),
                UnaryOp::Minus => value
                    .checked_neg()
                    .ok_or_else(|| ConstExprError("integer overflow".into())),
                UnaryOp::BitNot => Ok(!value),
                UnaryOp::Not => Ok((value == 0) as i64),
            }
        }
        ConstExpr::Binary { op, left, right } => {
            let left = evaluate_expr(left)?;
            if *op == BinaryOp::And && left == 0 || *op == BinaryOp::Or && left != 0 {
                return Ok((*op == BinaryOp::Or) as i64);
            }
            let right = evaluate_expr(right)?;
            let value = match op {
                BinaryOp::Add => left.checked_add(right),
                BinaryOp::Sub => left.checked_sub(right),
                BinaryOp::Mul => left.checked_mul(right),
                BinaryOp::Div => left.checked_div(right),
                BinaryOp::Rem => left.checked_rem(right),
                BinaryOp::Less => Some((left < right) as i64),
                BinaryOp::LessEqual => Some((left <= right) as i64),
                BinaryOp::Greater => Some((left > right) as i64),
                BinaryOp::GreaterEqual => Some((left >= right) as i64),
                BinaryOp::Equal => Some((left == right) as i64),
                BinaryOp::NotEqual => Some((left != right) as i64),
                BinaryOp::BitAnd => Some(left & right),
                BinaryOp::BitXor => Some(left ^ right),
                BinaryOp::BitOr => Some(left | right),
                BinaryOp::And => Some(((left != 0) && (right != 0)) as i64),
                BinaryOp::Or => Some(((left != 0) || (right != 0)) as i64),
                BinaryOp::ShiftLeft => left.checked_shl(right as u32),
                BinaryOp::ShiftRight => left.checked_shr(right as u32),
            };
            value.ok_or_else(|| ConstExprError("invalid integer constant expression".into()))
        }
    }
}

struct Parser {
    tokens: Vec<String>,
    position: usize,
}

impl Parser {
    fn new(source: &str) -> Result<Self, ConstExprError> {
        let mut tokens = Vec::new();
        let chars: Vec<char> = source.chars().collect();
        let mut index = 0;
        while index < chars.len() {
            if chars[index].is_whitespace() {
                index += 1;
            } else if chars[index].is_ascii_digit() {
                let start = index;
                while index < chars.len() && chars[index].is_ascii_digit() {
                    index += 1;
                }
                tokens.push(chars[start..index].iter().collect());
            } else if chars[index].is_ascii_alphabetic() || chars[index] == '_' {
                let start = index;
                while index < chars.len()
                    && (chars[index].is_ascii_alphanumeric() || chars[index] == '_')
                {
                    index += 1;
                }
                tokens.push(chars[start..index].iter().collect());
            } else if index + 1 < chars.len()
                && matches!(
                    (chars[index], chars[index + 1]),
                    ('<', '=')
                        | ('>', '=')
                        | ('=', '=')
                        | ('!', '=')
                        | ('&', '&')
                        | ('|', '|')
                        | ('<', '<')
                        | ('>', '>')
                )
            {
                tokens.push(chars[index..index + 2].iter().collect());
                index += 2;
            } else if "+-*/%()!~<>&^|".contains(chars[index]) {
                tokens.push(chars[index].to_string());
                index += 1;
            } else {
                return Err(ConstExprError(format!(
                    "unsupported token `{}`",
                    chars[index]
                )));
            }
        }
        Ok(Self {
            tokens,
            position: 0,
        })
    }

    fn peek(&self) -> Option<&str> {
        self.tokens.get(self.position).map(String::as_str)
    }

    fn take(&mut self) -> Option<String> {
        let token = self.tokens.get(self.position).cloned();
        self.position += token.is_some() as usize;
        token
    }

    fn parse_binary(&mut self, minimum_precedence: u8) -> Result<ConstExpr, ConstExprError> {
        let mut left = self.parse_unary()?;
        while let Some((op, precedence)) = self.binary_operator() {
            if precedence < minimum_precedence {
                break;
            }
            self.take();
            let right = self.parse_binary(precedence + 1)?;
            left = ConstExpr::Binary {
                op,
                left: Box::new(left),
                right: Box::new(right),
            };
        }
        Ok(left)
    }

    fn parse_unary(&mut self) -> Result<ConstExpr, ConstExprError> {
        let op = match self.peek() {
            Some("+") => Some(UnaryOp::Plus),
            Some("-") => Some(UnaryOp::Minus),
            Some("~") => Some(UnaryOp::BitNot),
            Some("!") => Some(UnaryOp::Not),
            _ => None,
        };
        if let Some(op) = op {
            self.take();
            return Ok(ConstExpr::Unary {
                op,
                value: Box::new(self.parse_unary()?),
            });
        }
        if self.take().as_deref() == Some("(") {
            let expression = self.parse_binary(0)?;
            if self.take().as_deref() != Some(")") {
                return Err(ConstExprError("expected `)`".into()));
            }
            return Ok(expression);
        }
        match self.tokens.get(self.position.saturating_sub(1)) {
            Some(value) => value
                .parse()
                .map(ConstExpr::Integer)
                .map_err(|_| ConstExprError(format!("unsupported identifier `{value}`"))),
            None => Err(ConstExprError("expected integer expression".into())),
        }
    }

    fn binary_operator(&self) -> Option<(BinaryOp, u8)> {
        Some(match self.peek()? {
            "*" => (BinaryOp::Mul, 6),
            "/" => (BinaryOp::Div, 6),
            "%" => (BinaryOp::Rem, 6),
            "+" => (BinaryOp::Add, 5),
            "-" => (BinaryOp::Sub, 5),
            "<" => (BinaryOp::Less, 4),
            "<=" => (BinaryOp::LessEqual, 4),
            ">" => (BinaryOp::Greater, 4),
            ">=" => (BinaryOp::GreaterEqual, 4),
            "==" => (BinaryOp::Equal, 3),
            "!=" => (BinaryOp::NotEqual, 3),
            "&" => (BinaryOp::BitAnd, 2),
            "^" => (BinaryOp::BitXor, 1),
            "|" => (BinaryOp::BitOr, 1),
            "&&" => (BinaryOp::And, 1),
            "||" => (BinaryOp::Or, 0),
            "<<" => (BinaryOp::ShiftLeft, 4),
            ">>" => (BinaryOp::ShiftRight, 4),
            _ => return None,
        })
    }
}
