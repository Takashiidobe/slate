use std::collections::HashMap;

#[derive(Debug, thiserror::Error)]
pub enum MatchError {
    #[error("no {prefix} checks in the fixture")]
    NoChecks { prefix: String },
    #[error("check line {line}: {directive} is not supported by the harness matcher")]
    UnsupportedDirective { line: usize, directive: String },
    #[error("check line {line}: {construct} is not supported by the harness matcher")]
    UnsupportedConstruct { line: usize, construct: String },
    #[error("check line {line}: unterminated {open}")]
    Unterminated { line: usize, open: &'static str },
    #[error("check line {line}: variable {name} is used before it is defined")]
    Undefined { line: usize, name: String },
    #[error("check line {line}: the first check cannot be a -NEXT check")]
    NextFirst { line: usize },
    #[error(
        "check line {line}: expected on output line {output_line}:\n  {pattern}\nfound:\n  {found}"
    )]
    NextMismatch {
        line: usize,
        output_line: usize,
        pattern: String,
        found: String,
    },
    #[error("check line {line}: no output line after line {after} matches:\n  {pattern}")]
    NotFound {
        line: usize,
        after: usize,
        pattern: String,
    },
}

#[derive(Debug)]
enum Piece {
    Literal(String),
    Digits,
    DigitList,
    Any,
    Define(String),
    Use(String),
}

#[derive(Clone, Copy, PartialEq)]
enum Kind {
    Plain,
    Next,
}

struct Check {
    line: usize,
    kind: Kind,
    text: String,
    pieces: Vec<Piece>,
}

pub fn check(fixture: &str, prefix: &str, output: &str) -> Result<(), MatchError> {
    let checks = parse_checks(fixture, prefix)?;
    let lines: Vec<String> = output.lines().map(collapse_whitespace).collect();
    let mut variables: HashMap<String, String> = HashMap::new();
    let mut last: Option<usize> = None;
    for check in &checks {
        check_uses(check, &variables)?;
        let mut bound = Vec::new();
        let matched = match check.kind {
            Kind::Next => {
                let target = last.ok_or(MatchError::NextFirst { line: check.line })? + 1;
                let found = lines
                    .get(target)
                    .map(String::as_str)
                    .unwrap_or("<end of output>");
                if !(target < lines.len()
                    && matches(&check.pieces, &lines[target], &variables, &mut bound))
                {
                    return Err(MatchError::NextMismatch {
                        line: check.line,
                        output_line: target + 1,
                        pattern: check.text.clone(),
                        found: found.to_string(),
                    });
                }
                target
            }
            Kind::Plain => {
                let start = last.map_or(0, |line| line + 1);
                (start..lines.len())
                    .find(|&index| {
                        bound.clear();
                        matches(&check.pieces, &lines[index], &variables, &mut bound)
                    })
                    .ok_or_else(|| MatchError::NotFound {
                        line: check.line,
                        after: start,
                        pattern: check.text.clone(),
                    })?
            }
        };
        variables.extend(bound);
        last = Some(matched);
    }
    Ok(())
}

fn parse_checks(fixture: &str, prefix: &str) -> Result<Vec<Check>, MatchError> {
    let mut checks = Vec::new();
    for (index, line) in fixture.lines().enumerate() {
        let line_number = index + 1;
        let Some(rest) = line
            .trim_start()
            .strip_prefix("//")
            .and_then(|rest| rest.trim_start().strip_prefix(prefix))
        else {
            continue;
        };
        let Some((suffix, text)) = rest.split_once(':') else {
            continue;
        };
        let kind = match suffix {
            "" => Kind::Plain,
            "-NEXT" => Kind::Next,
            "-SAME" | "-NOT" | "-DAG" | "-LABEL" | "-EMPTY" => {
                return Err(MatchError::UnsupportedDirective {
                    line: line_number,
                    directive: format!("{prefix}{suffix}"),
                });
            }
            _ if suffix.starts_with("-COUNT-") => {
                return Err(MatchError::UnsupportedDirective {
                    line: line_number,
                    directive: format!("{prefix}{suffix}"),
                });
            }
            _ => continue,
        };
        let text = collapse_whitespace(text);
        let pieces = parse_pattern(&text, line_number)?;
        checks.push(Check {
            line: line_number,
            kind,
            text,
            pieces,
        });
    }
    if checks.is_empty() {
        return Err(MatchError::NoChecks {
            prefix: prefix.to_string(),
        });
    }
    Ok(checks)
}

fn parse_pattern(text: &str, line: usize) -> Result<Vec<Piece>, MatchError> {
    let mut pieces = Vec::new();
    let mut rest = text;
    loop {
        let regex = rest.find("{{");
        let variable = rest.find("[[");
        let (start, is_regex) = match (regex, variable) {
            (Some(r), Some(v)) if r < v => (r, true),
            (_, Some(v)) => (v, false),
            (Some(r), None) => (r, true),
            (None, None) => {
                push_literal(&mut pieces, rest);
                return Ok(pieces);
            }
        };
        push_literal(&mut pieces, &rest[..start]);
        let (close, open) = if is_regex { ("}}", "{{") } else { ("]]", "[[") };
        let body_start = start + 2;
        let body_end = rest[body_start..]
            .find(close)
            .ok_or(MatchError::Unterminated { line, open })?
            + body_start;
        let body = &rest[body_start..body_end];
        let unsupported = || MatchError::UnsupportedConstruct {
            line,
            construct: format!("{open}{body}{close}"),
        };
        if is_regex {
            match body {
                "[0-9]+" => pieces.push(Piece::Digits),
                ".*" => pieces.push(Piece::Any),
                r"\[\[" => push_literal(&mut pieces, "[["),
                r"\{\{" => push_literal(&mut pieces, "{{"),
                "[}][}]" => push_literal(&mut pieces, "}}"),
                r"\[[0-9, ]+\]" => {
                    push_literal(&mut pieces, "[");
                    pieces.push(Piece::DigitList);
                    push_literal(&mut pieces, "]");
                }
                _ => return Err(unsupported()),
            }
        } else {
            let (numeric, body) = match body.strip_prefix('#') {
                Some(body) => (true, body),
                None => (false, body),
            };
            let (name, definition) = match body.split_once(':') {
                Some((name, pattern)) => (name, Some(pattern)),
                None => (body, None),
            };
            let valid_name = !name.is_empty()
                && name
                    .bytes()
                    .all(|byte| byte.is_ascii_alphanumeric() || byte == b'_');
            let piece = match definition {
                _ if !valid_name => return Err(unsupported()),
                None => Piece::Use(name.to_string()),
                Some("") if numeric => Piece::Define(name.to_string()),
                Some("[0-9]+") if !numeric => Piece::Define(name.to_string()),
                Some(_) => return Err(unsupported()),
            };
            pieces.push(piece);
        }
        rest = &rest[body_end + 2..];
    }
}

fn push_literal(pieces: &mut Vec<Piece>, text: &str) {
    if text.is_empty() {
        return;
    }
    if let Some(Piece::Literal(literal)) = pieces.last_mut() {
        literal.push_str(text);
    } else {
        pieces.push(Piece::Literal(text.to_string()));
    }
}

fn check_uses(check: &Check, variables: &HashMap<String, String>) -> Result<(), MatchError> {
    let mut defined_here: Vec<&str> = Vec::new();
    for piece in &check.pieces {
        match piece {
            Piece::Define(name) => defined_here.push(name),
            Piece::Use(name)
                if !variables.contains_key(name) && !defined_here.contains(&name.as_str()) =>
            {
                return Err(MatchError::Undefined {
                    line: check.line,
                    name: name.clone(),
                });
            }
            _ => {}
        }
    }
    Ok(())
}

fn matches(
    pieces: &[Piece],
    text: &str,
    variables: &HashMap<String, String>,
    bound: &mut Vec<(String, String)>,
) -> bool {
    let Some((piece, rest)) = pieces.split_first() else {
        return text.is_empty();
    };
    match piece {
        Piece::Literal(literal) => text
            .strip_prefix(literal.as_str())
            .is_some_and(|text| matches(rest, text, variables, bound)),
        Piece::Use(name) => {
            let remainder = bound
                .iter()
                .rev()
                .find(|(bound_name, _)| bound_name == name)
                .map(|(_, value)| value)
                .or_else(|| variables.get(name))
                .and_then(|value| text.strip_prefix(value.as_str()));
            remainder.is_some_and(|text| matches(rest, text, variables, bound))
        }
        Piece::Digits | Piece::Define(_) => {
            let run = text.bytes().take_while(u8::is_ascii_digit).count();
            (1..=run).rev().any(|length| {
                let Piece::Define(name) = piece else {
                    return matches(rest, &text[length..], variables, bound);
                };
                bound.push((name.clone(), text[..length].to_string()));
                if matches(rest, &text[length..], variables, bound) {
                    return true;
                }
                bound.pop();
                false
            })
        }
        Piece::DigitList => {
            let run = text
                .bytes()
                .take_while(|byte| byte.is_ascii_digit() || *byte == b',' || *byte == b' ')
                .count();
            (1..=run)
                .rev()
                .any(|length| matches(rest, &text[length..], variables, bound))
        }
        Piece::Any => (0..=text.len())
            .rev()
            .filter(|&length| text.is_char_boundary(length))
            .any(|length| matches(rest, &text[length..], variables, bound)),
    }
}

fn collapse_whitespace(line: &str) -> String {
    let mut collapsed = String::with_capacity(line.len());
    for word in line.split([' ', '\t']).filter(|word| !word.is_empty()) {
        if !collapsed.is_empty() {
            collapsed.push(' ');
        }
        collapsed.push_str(word);
    }
    collapsed
}
