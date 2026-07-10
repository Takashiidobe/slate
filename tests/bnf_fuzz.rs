mod support;

use std::collections::BTreeMap;
use std::path::Path;

#[derive(Debug)]
struct Grammar {
    rules: BTreeMap<String, Vec<Vec<Symbol>>>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
enum Symbol {
    Terminal(String),
    Nonterminal(String),
}

#[derive(Debug, Clone)]
struct Rng {
    state: u64,
}

impl Rng {
    fn new(seed: u64) -> Self {
        Self { state: seed }
    }

    fn next(&mut self) -> u64 {
        self.state = self
            .state
            .wrapping_mul(6364136223846793005)
            .wrapping_add(1442695040888963407);
        self.state
    }

    fn choose(&mut self, len: usize) -> usize {
        (self.next() as usize) % len
    }
}

fn parse_grammar(src: &str) -> Result<Grammar, String> {
    let mut rules = BTreeMap::new();
    for (line_no, raw) in src.lines().enumerate() {
        let line = raw.trim();
        if line.is_empty() || line.starts_with('#') {
            continue;
        }
        let line = line
            .strip_suffix(';')
            .ok_or_else(|| format!("line {}: missing ';'", line_no + 1))?;
        let (lhs, rhs) = line
            .split_once("::=")
            .ok_or_else(|| format!("line {}: missing '::='", line_no + 1))?;
        let lhs = parse_nonterminal(lhs.trim())
            .ok_or_else(|| format!("line {}: invalid lhs", line_no + 1))?;
        let alternatives = split_alternatives(rhs)
            .into_iter()
            .map(parse_symbols)
            .collect::<Result<Vec<_>, _>>()
            .map_err(|e| format!("line {}: {e}", line_no + 1))?;
        rules.insert(lhs, alternatives);
    }
    Ok(Grammar { rules })
}

fn parse_symbols(src: &str) -> Result<Vec<Symbol>, String> {
    let mut symbols = Vec::new();
    let mut chars = src.trim().chars().peekable();
    while let Some(c) = chars.peek().copied() {
        if c.is_whitespace() {
            chars.next();
        } else if c == '"' {
            symbols.push(Symbol::Terminal(parse_string(&mut chars)?));
        } else if c == '<' {
            symbols.push(Symbol::Nonterminal(parse_angle_name(&mut chars)?));
        } else {
            return Err(format!("unexpected character {c:?}"));
        }
    }
    Ok(symbols)
}

fn parse_string<I>(chars: &mut std::iter::Peekable<I>) -> Result<String, String>
where
    I: Iterator<Item = char>,
{
    expect_char(chars, '"')?;
    let mut out = String::new();
    while let Some(c) = chars.next() {
        match c {
            '"' => return Ok(out),
            '\\' => match chars.next() {
                Some('n') => out.push('\n'),
                Some('t') => out.push('\t'),
                Some('\\') => out.push('\\'),
                Some('"') => out.push('"'),
                Some(other) => return Err(format!("unsupported escape \\{other}")),
                None => return Err("unterminated escape".into()),
            },
            other => out.push(other),
        }
    }
    Err("unterminated string".into())
}

fn parse_angle_name<I>(chars: &mut std::iter::Peekable<I>) -> Result<String, String>
where
    I: Iterator<Item = char>,
{
    expect_char(chars, '<')?;
    let mut name = String::new();
    while let Some(c) = chars.next() {
        if c == '>' {
            if name.is_empty() {
                return Err("empty nonterminal".into());
            }
            return Ok(name);
        }
        name.push(c);
    }
    Err("unterminated nonterminal".into())
}

fn expect_char<I>(chars: &mut std::iter::Peekable<I>, expected: char) -> Result<(), String>
where
    I: Iterator<Item = char>,
{
    match chars.next() {
        Some(c) if c == expected => Ok(()),
        Some(c) => Err(format!("expected {expected:?}, got {c:?}")),
        None => Err(format!("expected {expected:?}, got EOF")),
    }
}

fn parse_nonterminal(src: &str) -> Option<String> {
    src.strip_prefix('<')
        .and_then(|s| s.strip_suffix('>'))
        .filter(|s| !s.is_empty())
        .map(str::to_string)
}

fn split_alternatives(src: &str) -> Vec<&str> {
    let mut parts = Vec::new();
    let mut start = 0;
    let mut in_string = false;
    let mut escaped = false;
    for (i, c) in src.char_indices() {
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
        if c == '"' {
            in_string = true;
        } else if c == '|' {
            parts.push(src[start..i].trim());
            start = i + c.len_utf8();
        }
    }
    parts.push(src[start..].trim());
    parts
}

fn expand(grammar: &Grammar, start: &str, rng: &mut Rng) -> Result<String, String> {
    expand_symbol(grammar, &Symbol::Nonterminal(start.into()), rng, 0)
}

fn expand_symbol(
    grammar: &Grammar,
    symbol: &Symbol,
    rng: &mut Rng,
    depth: usize,
) -> Result<String, String> {
    if depth > 64 {
        return Err("grammar expansion exceeded depth limit".into());
    }
    match symbol {
        Symbol::Terminal(text) => Ok(text.clone()),
        Symbol::Nonterminal(name) => {
            let alternatives = grammar
                .rules
                .get(name)
                .ok_or_else(|| format!("unknown nonterminal <{name}>"))?;
            let alt = &alternatives[rng.choose(alternatives.len())];
            let mut out = String::new();
            for symbol in alt {
                out.push_str(&expand_symbol(grammar, symbol, rng, depth + 1)?);
            }
            Ok(out)
        }
    }
}

#[test]
fn parses_subset_bnf() {
    let grammar = parse_grammar(include_str!("../c.bnf")).expect("parse c.bnf");
    assert!(grammar.rules.contains_key("program"));
    assert!(grammar.rules.contains_key("array_fn"));
    assert!(grammar.rules.contains_key("union_decl"));
    assert!(grammar.rules.contains_key("struct_decl"));
    assert!(grammar.rules.contains_key("sizeof_fn"));
    assert!(grammar.rules.contains_key("volatile_fn"));
}

#[test]
fn bnf_generated_differential() {
    let cases = std::env::var("SLATE_FUZZ_CASES")
        .ok()
        .and_then(|s| s.parse().ok())
        .unwrap_or(8);
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR"));
    let tmp = manifest.join("target/bnf-fuzz");
    std::fs::create_dir_all(&tmp).expect("create bnf fuzz dir");
    let grammar = parse_grammar(include_str!("../c.bnf")).expect("parse c.bnf");

    let mut failures = Vec::new();
    for seed in 0..cases {
        let name = format!("bnf_seed_{seed:04}");
        let mut rng = Rng::new(0x5eed_f00d ^ seed as u64);
        let c_src = tmp.join(format!("{name}.c"));
        let rs_src = tmp.join(format!("{name}.generated.rs"));
        let program = expand(&grammar, "program", &mut rng).expect("expand grammar");
        std::fs::write(&c_src, program).expect("write generated c");

        match support::translate(&c_src, &rs_src)
            .and_then(|()| support::compare(&name, &c_src, &rs_src, &tmp))
        {
            Ok(()) => eprintln!("ok    {name}"),
            Err(e) => {
                eprintln!("FAIL  {name}");
                failures.push(format!("[{name}] {e}"));
            }
        }
    }

    if !failures.is_empty() {
        panic!(
            "{} of {} BNF-generated programs failed:\n\n{}",
            failures.len(),
            cases,
            failures.join("\n\n")
        );
    }
}
