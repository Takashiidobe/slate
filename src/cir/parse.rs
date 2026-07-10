//! parse-cir: generic MLIR text -> [`Module`] Op-tree.

use super::ir::{Attr, Block, Module, Op, Region};
use std::collections::BTreeMap;

/// Parse generic-form CIR text into an Op-tree.
pub fn parse_module(text: &str) -> Result<Module, String> {
    Parser::new(text).parse_module()
}

struct Parser<'a> {
    text: &'a str,
    pos: usize,
}

impl<'a> Parser<'a> {
    fn new(text: &'a str) -> Self {
        Self { text, pos: 0 }
    }

    fn parse_module(mut self) -> Result<Module, String> {
        let mut ops = Vec::new();
        let mut aliases = BTreeMap::new();
        while !self.eof() {
            self.skip_ws();
            if self.eof() {
                break;
            }
            if self.peek() == Some('!') && self.line_contains_alias() {
                if let Some((name, value)) = self.parse_alias_line() {
                    aliases.insert(name, value);
                }
                continue;
            }
            ops.push(self.parse_op()?);
        }
        Ok(Module { ops, aliases })
    }

    fn parse_alias_line(&mut self) -> Option<(String, String)> {
        let line = self.take_line();
        let (name, value) = line.split_once('=')?;
        Some((name.trim().to_string(), value.trim().to_string()))
    }

    fn parse_op(&mut self) -> Result<Op, String> {
        self.skip_ws();
        let results = if self.peek() == Some('%') {
            let results = self.parse_results()?;
            self.skip_ws();
            self.expect_char('=')?;
            results
        } else {
            Vec::new()
        };

        self.skip_ws();
        let name = if self.peek() == Some('"') {
            self.parse_string()?
        } else {
            self.parse_bare_word()?
        };

        self.skip_ws();
        let operands = self.parse_operands()?;
        let mut attrs = BTreeMap::new();
        let mut regions = Vec::new();
        let mut ty = None;
        let mut loc = None;

        loop {
            self.skip_horizontal_ws();
            if self.starts_with("<{") {
                let body = self.take_balanced_attr_dict()?;
                attrs.extend(parse_attr_dict(&body));
            } else if self.next_is_region_list() {
                regions = self.parse_region_list()?;
            } else if self.peek() == Some('{') {
                let body = self.take_balanced('{', '}')?;
                attrs.extend(parse_attr_dict(strip_outer(&body)));
            } else if self.starts_with("loc(") {
                loc = Some(self.take_balanced('(', ')')?);
            } else if self.peek() == Some(':') {
                self.bump();
                let tail = self.take_type_tail();
                let (type_text, parsed_loc) = split_type_and_loc(tail.trim());
                if !type_text.is_empty() {
                    ty = Some(type_text.to_string());
                }
                if parsed_loc.is_some() {
                    loc = parsed_loc;
                }
                break;
            } else {
                break;
            }
        }

        Ok(Op {
            results,
            name,
            operands,
            attrs,
            regions,
            ty,
            loc,
        })
    }

    fn parse_results(&mut self) -> Result<Vec<String>, String> {
        let mut results = Vec::new();
        loop {
            self.skip_ws();
            results.push(self.parse_ssa_name()?);
            self.skip_ws();
            match self.peek() {
                Some(',') => {
                    self.bump();
                }
                Some('=') => break,
                _ => return Err(self.error("expected ',' or '=' after SSA result")),
            }
        }
        Ok(results)
    }

    fn parse_operands(&mut self) -> Result<Vec<String>, String> {
        self.skip_ws();
        self.expect_char('(')?;
        let mut operands = Vec::new();
        loop {
            self.skip_ws();
            match self.peek() {
                Some(')') => {
                    self.bump();
                    break;
                }
                Some('%') => operands.push(self.parse_ssa_name()?),
                Some(',') => {
                    self.bump();
                }
                Some(c) => return Err(self.error(&format!("unexpected operand character '{c}'"))),
                None => return Err(self.error("unterminated operand list")),
            }
        }
        Ok(operands)
    }

    fn parse_region_list(&mut self) -> Result<Vec<Region>, String> {
        self.expect_char('(')?;
        let mut regions = Vec::new();
        loop {
            self.skip_ws();
            match self.peek() {
                Some(')') => {
                    self.bump();
                    break;
                }
                Some('{') => regions.push(self.parse_region()?),
                Some(',') => {
                    self.bump();
                }
                Some(c) => return Err(self.error(&format!("unexpected region character '{c}'"))),
                None => return Err(self.error("unterminated region list")),
            }
        }
        Ok(regions)
    }

    fn parse_region(&mut self) -> Result<Region, String> {
        self.expect_char('{')?;
        let mut blocks = Vec::<Block>::new();
        while !self.eof() {
            self.skip_ws();
            if self.peek() == Some('}') {
                self.bump();
                break;
            }

            if self.peek() == Some('^') {
                blocks.push(self.parse_block_header()?);
                continue;
            }

            if blocks.is_empty() {
                blocks.push(Block::default());
            }
            let op = self.parse_op()?;
            blocks.last_mut().unwrap().ops.push(op);
        }
        Ok(Region { blocks })
    }

    fn parse_block_header(&mut self) -> Result<Block, String> {
        self.expect_char('^')?;
        let label = self.parse_ident()?;
        self.skip_ws();
        let args = if self.peek() == Some('(') {
            self.parse_block_args()?
        } else {
            Vec::new()
        };
        self.skip_ws();
        self.expect_char(':')?;
        Ok(Block {
            label: Some(label),
            args,
            ops: Vec::new(),
        })
    }

    fn parse_block_args(&mut self) -> Result<Vec<(String, String)>, String> {
        self.expect_char('(')?;
        let mut args = Vec::new();
        loop {
            self.skip_ws();
            match self.peek() {
                Some(')') => {
                    self.bump();
                    break;
                }
                Some('%') => {
                    let name = self.parse_ssa_name()?;
                    self.skip_ws();
                    self.expect_char(':')?;
                    let ty = self.take_until_top_level(&[',', ')']).trim().to_string();
                    args.push((name, ty));
                }
                Some(',') => {
                    self.bump();
                }
                Some(c) => return Err(self.error(&format!("unexpected block arg character '{c}'"))),
                None => return Err(self.error("unterminated block argument list")),
            }
        }
        Ok(args)
    }

    fn parse_ssa_name(&mut self) -> Result<String, String> {
        self.expect_char('%')?;
        self.parse_ident()
    }

    fn parse_ident(&mut self) -> Result<String, String> {
        let start = self.pos;
        while let Some(c) = self.peek() {
            if c.is_ascii_alphanumeric() || matches!(c, '_' | '.' | '-' | '$') {
                self.bump();
            } else {
                break;
            }
        }
        if self.pos == start {
            Err(self.error("expected identifier"))
        } else {
            Ok(self.text[start..self.pos].to_string())
        }
    }

    fn parse_bare_word(&mut self) -> Result<String, String> {
        let start = self.pos;
        while let Some(c) = self.peek() {
            if c.is_whitespace() || matches!(c, '(' | ')' | '{' | '}' | '<' | '>' | ':' | ',') {
                break;
            }
            self.bump();
        }
        if self.pos == start {
            Err(self.error("expected operation name"))
        } else {
            Ok(self.text[start..self.pos].to_string())
        }
    }

    fn parse_string(&mut self) -> Result<String, String> {
        self.expect_char('"')?;
        let mut s = String::new();
        while let Some(c) = self.bump() {
            match c {
                '"' => return Ok(s),
                '\\' => {
                    s.push(c);
                    if let Some(next) = self.bump() {
                        s.push(next);
                    }
                }
                _ => s.push(c),
            }
        }
        Err(self.error("unterminated string literal"))
    }

    fn take_balanced_attr_dict(&mut self) -> Result<String, String> {
        self.expect_str("<{")?;
        let start = self.pos;
        let mut brace_depth = 1usize;
        let mut paren_depth = 0usize;
        let mut bracket_depth = 0usize;
        let mut angle_depth = 0usize;
        let mut in_string = false;
        let mut escaped = false;

        while let Some(c) = self.bump() {
            if in_string {
                if escaped {
                    escaped = false;
                } else if c == '\\' {
                    escaped = true;
                } else if c == '"' {
                    in_string = false;
                }
                continue;
            }

            match c {
                '"' => in_string = true,
                '{' => brace_depth += 1,
                '}' => {
                    brace_depth -= 1;
                    if brace_depth == 0 && self.peek() == Some('>') {
                        let end = self.pos - c.len_utf8();
                        self.bump();
                        return Ok(self.text[start..end].to_string());
                    }
                }
                '(' => paren_depth += 1,
                ')' => paren_depth = paren_depth.saturating_sub(1),
                '[' => bracket_depth += 1,
                ']' => bracket_depth = bracket_depth.saturating_sub(1),
                '<' => angle_depth += 1,
                '>' if angle_depth > 0 => angle_depth -= 1,
                _ => {
                    let _ = (paren_depth, bracket_depth, angle_depth);
                }
            }
        }
        Err(self.error("unterminated <{...}> attribute dictionary"))
    }

    fn take_balanced(&mut self, open: char, close: char) -> Result<String, String> {
        self.expect_char(open)?;
        let start = self.pos - open.len_utf8();
        let mut depth = 1usize;
        let mut in_string = false;
        let mut escaped = false;
        while let Some(c) = self.bump() {
            if in_string {
                if escaped {
                    escaped = false;
                } else if c == '\\' {
                    escaped = true;
                } else if c == '"' {
                    in_string = false;
                }
                continue;
            }
            match c {
                '"' => in_string = true,
                c if c == open => depth += 1,
                c if c == close => {
                    depth -= 1;
                    if depth == 0 {
                        return Ok(self.text[start..self.pos].to_string());
                    }
                }
                _ => {}
            }
        }
        Err(self.error("unterminated balanced group"))
    }

    fn take_until_top_level(&mut self, terminators: &[char]) -> String {
        let start = self.pos;
        let mut paren = 0usize;
        let mut angle = 0usize;
        let mut bracket = 0usize;
        let mut brace = 0usize;
        let mut in_string = false;
        let mut escaped = false;
        while let Some(c) = self.peek() {
            if in_string {
                self.bump();
                if escaped {
                    escaped = false;
                } else if c == '\\' {
                    escaped = true;
                } else if c == '"' {
                    in_string = false;
                }
                continue;
            }

            if paren == 0 && angle == 0 && bracket == 0 && brace == 0 && terminators.contains(&c) {
                break;
            }

            match c {
                '"' => in_string = true,
                '(' => paren += 1,
                ')' => paren = paren.saturating_sub(1),
                '<' => angle += 1,
                '>' => angle = angle.saturating_sub(1),
                '[' => bracket += 1,
                ']' => bracket = bracket.saturating_sub(1),
                '{' => brace += 1,
                '}' => brace = brace.saturating_sub(1),
                _ => {}
            }
            self.bump();
        }
        self.text[start..self.pos].to_string()
    }

    fn take_type_tail(&mut self) -> String {
        let start = self.pos;
        while let Some(c) = self.peek() {
            if c == '\n' || c == '\r' {
                break;
            }
            self.bump();
        }
        self.text[start..self.pos].to_string()
    }

    fn next_is_region_list(&self) -> bool {
        if self.peek() != Some('(') {
            return false;
        }
        let mut i = self.pos + 1;
        while i < self.text.len() {
            let c = self.text[i..].chars().next().unwrap();
            if c.is_whitespace() {
                i += c.len_utf8();
            } else {
                return c == '{';
            }
        }
        false
    }

    fn line_contains_alias(&self) -> bool {
        self.text[self.pos..]
            .lines()
            .next()
            .is_some_and(|line| line.contains('='))
    }

    fn skip_ws(&mut self) {
        while self.peek().is_some_and(char::is_whitespace) {
            self.bump();
        }
    }

    fn skip_horizontal_ws(&mut self) {
        while self.peek().is_some_and(|c| c == ' ' || c == '\t') {
            self.bump();
        }
    }

    fn skip_line(&mut self) {
        self.take_line();
    }

    fn take_line(&mut self) -> &'a str {
        let start = self.pos;
        while let Some(c) = self.bump() {
            if c == '\n' {
                break;
            }
        }
        self.text[start..self.pos].trim()
    }

    fn expect_char(&mut self, expected: char) -> Result<(), String> {
        match self.bump() {
            Some(c) if c == expected => Ok(()),
            Some(c) => Err(self.error(&format!("expected '{expected}', found '{c}'"))),
            None => Err(self.error(&format!("expected '{expected}', found EOF"))),
        }
    }

    fn expect_str(&mut self, expected: &str) -> Result<(), String> {
        if self.starts_with(expected) {
            self.pos += expected.len();
            Ok(())
        } else {
            Err(self.error(&format!("expected {expected:?}")))
        }
    }

    fn starts_with(&self, s: &str) -> bool {
        self.text[self.pos..].starts_with(s)
    }

    fn peek(&self) -> Option<char> {
        self.text[self.pos..].chars().next()
    }

    fn bump(&mut self) -> Option<char> {
        let c = self.peek()?;
        self.pos += c.len_utf8();
        Some(c)
    }

    fn eof(&self) -> bool {
        self.pos >= self.text.len()
    }

    fn error(&self, msg: &str) -> String {
        let line = self.text[..self.pos]
            .bytes()
            .filter(|b| *b == b'\n')
            .count()
            + 1;
        let col = self.text[..self.pos]
            .rsplit('\n')
            .next()
            .map_or(1, |s| s.chars().count() + 1);
        format!("{msg} at {line}:{col}")
    }
}

fn strip_outer(s: &str) -> &str {
    s.strip_prefix('{')
        .and_then(|s| s.strip_suffix('}'))
        .unwrap_or(s)
}

fn split_type_and_loc(s: &str) -> (&str, Option<String>) {
    if let Some(idx) = find_top_level_loc(s) {
        let ty = s[..idx].trim_end();
        let loc = s[idx..].trim().to_string();
        (ty, Some(loc))
    } else {
        (s, None)
    }
}

fn find_top_level_loc(s: &str) -> Option<usize> {
    let mut paren = 0usize;
    let mut angle = 0usize;
    let mut bracket = 0usize;
    let mut brace = 0usize;
    let mut in_string = false;
    let mut escaped = false;
    for (i, c) in s.char_indices() {
        if in_string {
            if escaped {
                escaped = false;
            } else if c == '\\' {
                escaped = true;
            } else if c == '"' {
                in_string = false;
            }
            continue;
        }
        if paren == 0 && angle == 0 && bracket == 0 && brace == 0 && s[i..].starts_with("loc(") {
            return Some(i);
        }
        match c {
            '"' => in_string = true,
            '(' => paren += 1,
            ')' => paren = paren.saturating_sub(1),
            '<' => angle += 1,
            '>' => angle = angle.saturating_sub(1),
            '[' => bracket += 1,
            ']' => bracket = bracket.saturating_sub(1),
            '{' => brace += 1,
            '}' => brace = brace.saturating_sub(1),
            _ => {}
        }
    }
    None
}

fn parse_attr_dict(s: &str) -> BTreeMap<String, Attr> {
    split_top_level(s, ',')
        .into_iter()
        .filter_map(|entry| {
            let entry = entry.trim();
            if entry.is_empty() {
                return None;
            }
            if let Some(eq) = find_top_level_char(entry, '=') {
                let key = entry[..eq].trim().trim_matches('"').to_string();
                let value = parse_attr_value(entry[eq + 1..].trim());
                Some((key, value))
            } else {
                Some((
                    entry.trim_matches('"').to_string(),
                    Attr::Raw("true".into()),
                ))
            }
        })
        .collect()
}

fn parse_attr_value(s: &str) -> Attr {
    let s = s.trim();
    if s.starts_with('"') && s.ends_with('"') && s.len() >= 2 {
        return Attr::Str(s[1..s.len() - 1].to_string());
    }
    if s.starts_with('!') {
        return Attr::Type(s.to_string());
    }
    if s.starts_with('{') && s.ends_with('}') {
        return Attr::Dict(parse_attr_dict(strip_outer(s)));
    }
    if s.starts_with('[') && s.ends_with(']') {
        let inner = &s[1..s.len() - 1];
        return Attr::Array(
            split_top_level(inner, ',')
                .into_iter()
                .map(|part| parse_attr_value(part.trim()))
                .collect(),
        );
    }
    if let Some(n) = parse_integer_prefix(s) {
        return Attr::Int(n);
    }
    Attr::Raw(s.to_string())
}

fn parse_integer_prefix(s: &str) -> Option<i64> {
    let mut end = 0usize;
    for (i, c) in s.char_indices() {
        if i == 0 && c == '-' {
            end = c.len_utf8();
        } else if c.is_ascii_digit() {
            end = i + c.len_utf8();
        } else {
            break;
        }
    }
    if end == 0 || s[..end].trim() == "-" {
        return None;
    }
    let rest = s[end..].trim_start();
    if rest.is_empty() || rest.starts_with(':') {
        s[..end].parse().ok()
    } else {
        None
    }
}

fn find_top_level_char(s: &str, needle: char) -> Option<usize> {
    let mut paren = 0usize;
    let mut angle = 0usize;
    let mut bracket = 0usize;
    let mut brace = 0usize;
    let mut in_string = false;
    let mut escaped = false;
    for (i, c) in s.char_indices() {
        if in_string {
            if escaped {
                escaped = false;
            } else if c == '\\' {
                escaped = true;
            } else if c == '"' {
                in_string = false;
            }
            continue;
        }
        if paren == 0 && angle == 0 && bracket == 0 && brace == 0 && c == needle {
            return Some(i);
        }
        match c {
            '"' => in_string = true,
            '(' => paren += 1,
            ')' => paren = paren.saturating_sub(1),
            '<' => angle += 1,
            '>' => angle = angle.saturating_sub(1),
            '[' => bracket += 1,
            ']' => bracket = bracket.saturating_sub(1),
            '{' => brace += 1,
            '}' => brace = brace.saturating_sub(1),
            _ => {}
        }
    }
    None
}

fn split_top_level(s: &str, delimiter: char) -> Vec<&str> {
    let mut parts = Vec::new();
    let mut start = 0usize;
    let mut paren = 0usize;
    let mut angle = 0usize;
    let mut bracket = 0usize;
    let mut brace = 0usize;
    let mut in_string = false;
    let mut escaped = false;

    for (i, c) in s.char_indices() {
        if in_string {
            if escaped {
                escaped = false;
            } else if c == '\\' {
                escaped = true;
            } else if c == '"' {
                in_string = false;
            }
            continue;
        }
        if paren == 0 && angle == 0 && bracket == 0 && brace == 0 && c == delimiter {
            parts.push(&s[start..i]);
            start = i + c.len_utf8();
            continue;
        }
        match c {
            '"' => in_string = true,
            '(' => paren += 1,
            ')' => paren = paren.saturating_sub(1),
            '<' => angle += 1,
            '>' => angle = angle.saturating_sub(1),
            '[' => bracket += 1,
            ']' => bracket = bracket.saturating_sub(1),
            '{' => brace += 1,
            '}' => brace = brace.saturating_sub(1),
            _ => {}
        }
    }
    parts.push(&s[start..]);
    parts
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parses_nested_generic_ops_and_attrs() {
        let text = r#"
!s32i = !cir.int<s, 32>
"builtin.module"() <{sym_name = "t.c"}> ({
  "cir.func"() <{function_type = !cir.func<(!s32i) -> !s32i>, sym_name = "f", dso_local}> ({
  ^bb0(%arg0: !s32i):
    %0 = "cir.alloca"() <{alignment = 4 : i64, name = "x"}> : () -> !cir.ptr<!s32i>
    "cir.store"(%arg0, %0) : (!s32i, !cir.ptr<!s32i>) -> ()
  }) {"cir.target-cpu" = "x86-64", nothrow} : () -> ()
}) {cir.lang = #cir.lang<c>, cir.triple = "x86_64-unknown-linux-gnu", dlti.dl_spec = #dlti.dl_spec<i32 = dense<32> : vector<2xi64>>} : () -> ()
"#;

        let module = parse_module(text).unwrap();
        assert_eq!(module.ops.len(), 1);
        assert_eq!(module.aliases["!s32i"], "!cir.int<s, 32>");
        let module_op = &module.ops[0];
        assert_eq!(module_op.name, "builtin.module");
        assert_eq!(module_op.attrs["sym_name"].as_str(), Some("t.c"));
        assert_eq!(
            module_op.attrs["cir.triple"].as_str(),
            Some("x86_64-unknown-linux-gnu")
        );
        assert!(
            module_op.attrs["dlti.dl_spec"]
                .as_str()
                .is_some_and(|spec| spec.contains("i32 = dense<32>"))
        );
        assert_eq!(module_op.regions.len(), 1);

        let func = &module_op.regions[0].blocks[0].ops[0];
        assert_eq!(func.name, "cir.func");
        assert_eq!(func.attrs["sym_name"].as_str(), Some("f"));
        assert_eq!(func.attrs["dso_local"].as_str(), Some("true"));
        assert_eq!(func.regions[0].blocks[0].label.as_deref(), Some("bb0"));
        assert_eq!(
            func.regions[0].blocks[0].args[0],
            ("arg0".into(), "!s32i".into())
        );

        let alloca = &func.regions[0].blocks[0].ops[0];
        assert_eq!(alloca.results, ["0"]);
        assert_eq!(alloca.attrs["alignment"].as_int(), Some(4));
        assert_eq!(alloca.ty.as_deref(), Some("() -> !cir.ptr<!s32i>"));

        let store = &func.regions[0].blocks[0].ops[1];
        assert_eq!(store.operands, ["arg0", "0"]);
    }

    #[test]
    fn retains_trailing_locations() {
        let text = r#"
%0 = "cir.const"() <{value = #cir.int<1> : !s32i}> : () -> !s32i loc("f.c":1:2)
"#;
        let module = parse_module(text).unwrap();
        assert_eq!(module.ops[0].loc.as_deref(), Some(r#"loc("f.c":1:2)"#));
        assert_eq!(module.ops[0].ty.as_deref(), Some("() -> !s32i"));
    }
}
