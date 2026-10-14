use crate::ast::TranslationUnit;
use std::io::{self, Write};

#[derive(Debug)]
enum Node {
    Atom(String),
    List(Vec<Node>),
    Vector(Vec<Node>),
    Map(Vec<(Node, Node)>),
}

enum Token {
    Atom(String),
    Quoted(String),
    Punct(char),
}

pub struct SExprRenderer<W> {
    out: W,
}

impl<W: Write> SExprRenderer<W> {
    pub fn new(out: W) -> Self {
        Self { out }
    }

    pub fn render(&mut self, ast: &TranslationUnit) -> io::Result<()> {
        let debug = format!("{ast:?}");
        let mut parser = Parser::new(&debug);
        let mut node = parser
            .value()
            .filter(|_| parser.is_done())
            .unwrap_or_else(|| {
                Node::List(vec![
                    Node::Atom("ast-debug".into()),
                    Node::Atom(format!("{debug:?}")),
                ])
            });
        filter_system_header_nodes(&mut node);
        writeln!(self.out, "{}", render_node(&node, 0))
    }

    pub fn into_inner(self) -> W {
        self.out
    }
}

struct Parser {
    tokens: Vec<Token>,
    index: usize,
}

impl Parser {
    fn new(input: &str) -> Self {
        let mut tokens = Vec::new();
        let mut chars = input.char_indices().peekable();
        while let Some((start, ch)) = chars.next() {
            if ch.is_whitespace() {
                continue;
            }
            if "{}[](),:".contains(ch) {
                tokens.push(Token::Punct(ch));
                continue;
            }
            if matches!(ch, '"' | '\'') {
                let mut escaped = false;
                let mut end = start + ch.len_utf8();
                for (offset, next) in chars.by_ref() {
                    end = offset + next.len_utf8();
                    if escaped {
                        escaped = false;
                    } else if next == '\\' {
                        escaped = true;
                    } else if next == ch {
                        break;
                    }
                }
                tokens.push(Token::Quoted(input[start..end].to_owned()));
                continue;
            }
            let mut end = start + ch.len_utf8();
            while let Some(&(offset, next)) = chars.peek() {
                if next.is_whitespace() || "{}[](),:\"'".contains(next) {
                    break;
                }
                chars.next();
                end = offset + next.len_utf8();
            }
            tokens.push(Token::Atom(input[start..end].to_owned()));
        }
        Self { tokens, index: 0 }
    }

    fn is_done(&self) -> bool {
        self.index == self.tokens.len()
    }

    fn punct(&self, expected: char) -> bool {
        matches!(self.tokens.get(self.index), Some(Token::Punct(value)) if *value == expected)
    }

    fn consume(&mut self, expected: char) -> bool {
        if self.punct(expected) {
            self.index += 1;
            true
        } else {
            false
        }
    }

    fn atom(&mut self) -> Option<String> {
        match self.tokens.get(self.index)? {
            Token::Atom(value) => {
                self.index += 1;
                Some(value.clone())
            }
            _ => None,
        }
    }

    fn value(&mut self) -> Option<Node> {
        match self.tokens.get(self.index)? {
            Token::Quoted(value) => {
                let node = Node::Atom(value.clone());
                self.index += 1;
                Some(node)
            }
            Token::Punct('[') => {
                self.index += 1;
                Some(Node::Vector(self.sequence(']', false)?))
            }
            Token::Punct('{') => {
                self.index += 1;
                Some(Node::Map(self.map()?))
            }
            Token::Atom(name) => {
                let name = name.clone();
                self.index += 1;
                let close = match self.tokens.get(self.index) {
                    Some(Token::Punct('{')) => '}',
                    Some(Token::Punct('(')) => ')',
                    _ => {
                        return Some(if name.chars().next().is_some_and(char::is_uppercase) {
                            Node::List(vec![Node::Atom(name)])
                        } else {
                            Node::Atom(name)
                        });
                    }
                };
                self.index += 1;
                let mut values = vec![Node::Atom(name)];
                values.extend(self.sequence(close, close == '}')?);
                Some(Node::List(values))
            }
            Token::Punct(_) => None,
        }
    }

    fn sequence(&mut self, close: char, fields: bool) -> Option<Vec<Node>> {
        let mut values = Vec::new();
        if self.consume(close) {
            return Some(values);
        }
        loop {
            if fields {
                let name = self.atom()?;
                if !self.consume(':') {
                    return None;
                }
                values.push(Node::List(vec![Node::Atom(name), self.value()?]));
            } else {
                values.push(self.value()?);
            }
            if self.consume(close) {
                break;
            }
            if !self.consume(',') {
                return None;
            }
        }
        Some(values)
    }

    fn map(&mut self) -> Option<Vec<(Node, Node)>> {
        let mut entries = Vec::new();
        if self.consume('}') {
            return Some(entries);
        }
        loop {
            let key = self.value()?;
            if !self.consume(':') {
                return None;
            }
            let value = self.value()?;
            entries.push((key, value));
            if self.consume('}') {
                break;
            }
            if !self.consume(',') {
                return None;
            }
        }
        Some(entries)
    }
}

fn filter_system_header_nodes(node: &mut Node) {
    let Node::List(root) = node else { return };
    if !matches!(root.first(), Some(Node::Atom(name)) if name == "TranslationUnit") {
        return;
    }
    let mut omitted = 0;
    for field in root.iter_mut().skip(1) {
        let Node::List(parts) = field else { continue };
        let Some(Node::Atom(name)) = parts.first() else {
            continue;
        };
        if !matches!(name.as_str(), "decls" | "tags") {
            continue;
        }
        let Some(Node::Vector(values)) = parts.get_mut(1) else {
            continue;
        };
        values.retain(|value| {
            if is_system_node(value) {
                omitted += 1;
                false
            } else {
                true
            }
        });
    }
    root[1..].sort_by_key(|field| match field {
        Node::List(parts) if matches!(parts.first(), Some(Node::Atom(name)) if name == "decls") => 0,
        Node::List(parts) if matches!(parts.first(), Some(Node::Atom(name)) if name == "tags") => 1,
        Node::List(parts)
            if matches!(parts.first(), Some(Node::Atom(name)) if name == "dialect") =>
        {
            3
        }
        _ => 2,
    });
    if omitted > 0 {
        let dialect = root.iter().position(
            |field| matches!(field, Node::List(parts) if matches!(parts.first(), Some(Node::Atom(name)) if name == "dialect")),
        );
        let summary = Node::List(vec![
            Node::Atom("system-header-nodes-omitted".into()),
            Node::Atom(omitted.to_string()),
        ]);
        root.insert(dialect.unwrap_or(root.len()), summary);
    }
}

fn is_system_node(node: &Node) -> bool {
    matches!(node, Node::List(values) if matches!(values.first(), Some(Node::Atom(name)) if name == "Spanned"))
}

fn compact(node: &Node) -> String {
    match node {
        Node::Atom(value) => value.clone(),
        Node::List(values) => format!(
            "({})",
            values.iter().map(compact).collect::<Vec<_>>().join(" ")
        ),
        Node::Vector(values) => format!(
            "[{}]",
            values.iter().map(compact).collect::<Vec<_>>().join(" ")
        ),
        Node::Map(entries) => format!(
            "{{{}}}",
            entries
                .iter()
                .map(|(key, value)| format!("({} {})", compact(key), compact(value)))
                .collect::<Vec<_>>()
                .join(" ")
        ),
    }
}

fn render_node(node: &Node, indent: usize) -> String {
    let limit = 100usize.saturating_sub(indent);
    if flat_len(node, limit) <= limit {
        return compact(node);
    }
    match node {
        Node::Atom(_) => compact(node),
        Node::List(values) if !values.is_empty() => {
            render_container('(', ')', &values[0], &values[1..], indent)
        }
        Node::Vector(values) => render_items('[', ']', values, indent),
        Node::Map(entries) => render_map(entries, indent),
        _ => compact(node),
    }
}

fn render_container(
    open: char,
    close: char,
    head: &Node,
    children: &[Node],
    indent: usize,
) -> String {
    let head = compact(head);
    if children.is_empty() {
        return format!("{open}{head}{close}");
    }
    format!(
        "{open}{head}\n{}\n{}{close}",
        render_children(children, indent + 2),
        " ".repeat(indent)
    )
}

fn render_items(open: char, close: char, values: &[Node], indent: usize) -> String {
    if values.is_empty() {
        return format!("{open}{close}");
    }
    format!(
        "{open}\n{}\n{}{close}",
        render_children(values, indent + 2),
        " ".repeat(indent)
    )
}

fn render_map(entries: &[(Node, Node)], indent: usize) -> String {
    let values = entries
        .iter()
        .map(|(key, value)| {
            format!(
                "({} {})",
                render_node(key, indent + 2),
                render_node(value, indent + 2)
            )
        })
        .collect::<Vec<_>>();
    if values.is_empty() {
        return "{}".into();
    }
    format!(
        "{{\n{}\n{}}}",
        values
            .iter()
            .map(|value| format!("{}{}", " ".repeat(indent + 2), value))
            .collect::<Vec<_>>()
            .join("\n"),
        " ".repeat(indent)
    )
}

fn render_children(values: &[Node], indent: usize) -> String {
    values
        .iter()
        .map(|child| format!("{}{}", " ".repeat(indent), render_node(child, indent)))
        .collect::<Vec<_>>()
        .join("\n")
}

fn flat_len(node: &Node, limit: usize) -> usize {
    match node {
        Node::Atom(value) => value.len(),
        Node::List(values) => delimited_len(values, limit),
        Node::Vector(values) => delimited_len(values, limit),
        Node::Map(entries) => {
            let mut len = 2usize;
            for (index, (key, value)) in entries.iter().enumerate() {
                len = len
                    .saturating_add(usize::from(index != 0))
                    .saturating_add(2)
                    .saturating_add(flat_len(key, limit))
                    .saturating_add(1)
                    .saturating_add(flat_len(value, limit));
                if len > limit {
                    return len;
                }
            }
            len
        }
    }
}

fn delimited_len(values: &[Node], limit: usize) -> usize {
    let mut len = 2usize;
    for (index, value) in values.iter().enumerate() {
        len = len
            .saturating_add(usize::from(index != 0))
            .saturating_add(flat_len(value, limit));
        if len > limit {
            return len;
        }
    }
    len
}
