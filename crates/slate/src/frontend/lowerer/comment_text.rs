const TOOL_ANNOTATIONS: &[&str] = &[
    "FALLTHROUGH",
    "NOTREACHED",
    "ARGSUSED",
    "NO_TEST",
    "OPTIMIZATION-IF-TRUE",
    "OPTIMIZATION-IF-FALSE",
];

pub(super) fn normalize(pieces: &[String]) -> Vec<String> {
    let mut lines = Vec::new();
    for piece in pieces {
        let piece_lines = if let Some(body) = piece.strip_prefix("/*") {
            block_lines(body)
        } else {
            line_lines(piece)
        };
        if is_tool_annotation(&piece_lines) {
            continue;
        }
        if !lines.is_empty() && !piece_lines.is_empty() {
            lines.push(String::new());
        }
        lines.extend(piece_lines);
    }
    lines
}

fn is_tool_annotation(lines: &[String]) -> bool {
    let [line] = lines else {
        return false;
    };
    let line = line.trim();
    TOOL_ANNOTATIONS.contains(&line) || line.starts_with("NOLINT")
}

fn line_lines(piece: &str) -> Vec<String> {
    let rest = piece.strip_prefix("//").unwrap_or(piece);
    let rest = rest.strip_prefix(['/', '!']).unwrap_or(rest);
    let rest = rest.strip_prefix('<').unwrap_or(rest);
    let rest = rest.strip_prefix(' ').unwrap_or(rest).trim_end();
    if is_separator(rest) {
        return Vec::new();
    }
    vec![rest.to_owned()]
}

fn block_lines(body: &str) -> Vec<String> {
    let body = body.strip_suffix("*/").unwrap_or(body);
    let body = body.trim_end_matches('*');
    let body = body.trim_start_matches(['*', '!']);
    let body = body.strip_prefix('<').unwrap_or(body);
    let mut raw = body.lines().map(str::to_owned);
    let first = raw.next().unwrap_or_default();
    let rest: Vec<String> = raw.collect();
    let guttered = rest
        .iter()
        .filter(|line| !line.trim().is_empty())
        .all(|line| line.trim_start().starts_with('*'))
        && rest.iter().any(|line| !line.trim().is_empty());
    let rest = if guttered {
        rest.into_iter().map(|line| strip_gutter(&line)).collect()
    } else {
        rest
    };
    let first = first.strip_prefix(' ').unwrap_or(&first).to_owned();
    tidy(rest, first)
}

fn strip_gutter(line: &str) -> String {
    let line = line.trim_start().trim_start_matches('*');
    line.strip_prefix(' ').unwrap_or(line).to_owned()
}

fn tidy(rest: Vec<String>, first: String) -> Vec<String> {
    let mut rest: Vec<String> = rest
        .into_iter()
        .map(|line| line.trim_end().to_owned())
        .filter(|line| !is_separator(line))
        .collect();
    let indent = rest
        .iter()
        .filter(|line| !line.is_empty())
        .map(|line| line.len() - line.trim_start().len())
        .min()
        .unwrap_or(0);
    for line in &mut rest {
        if !line.is_empty() {
            line.drain(..indent);
        }
    }
    let mut lines: Vec<String> = std::iter::once(first.trim_end().to_owned())
        .filter(|first| !is_separator(first))
        .chain(rest)
        .collect();
    while lines.first().is_some_and(String::is_empty) {
        lines.remove(0);
    }
    while lines.last().is_some_and(String::is_empty) {
        lines.pop();
    }
    lines
}

fn is_separator(line: &str) -> bool {
    let line = line.trim();
    line.len() >= 3 && line.chars().all(|c| "*-=_/#~+".contains(c))
}
