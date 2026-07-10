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

/// `c.bnf` is a reference grammar for Slate's supported C subset, not a program
/// generator (that is `support::cgen`). These tests keep it honest: it must
/// parse, expose the expected top-level rules, and be internally closed — every
/// nonterminal referenced on a right-hand side must be defined somewhere.
#[test]
fn parses_subset_bnf() {
    let grammar = parse_grammar(include_str!("../c.bnf")).expect("parse c.bnf");
    for rule in [
        "program",
        "function",
        "stmt",
        "expr",
        "struct_decl",
        "union_decl",
        "enum_decl",
        "global_decl",
        "for_loop",
        "while_loop",
        "sizeof",
    ] {
        assert!(grammar.rules.contains_key(rule), "missing rule <{rule}>");
    }
}

#[test]
fn grammar_references_are_defined() {
    let grammar = parse_grammar(include_str!("../c.bnf")).expect("parse c.bnf");
    let mut undefined = Vec::new();
    for (lhs, alternatives) in &grammar.rules {
        for alt in alternatives {
            for symbol in alt {
                if let Symbol::Nonterminal(name) = symbol {
                    if !grammar.rules.contains_key(name) {
                        undefined.push(format!("<{name}> (used in <{lhs}>)"));
                    }
                }
            }
        }
    }
    assert!(
        undefined.is_empty(),
        "undefined nonterminals in c.bnf:\n{}",
        undefined.join("\n")
    );
}

/// A random `u64`, seeded per call from the process's `RandomState` keys (which
/// are themselves randomized per process). No external RNG crate needed.
fn random_seed() -> u64 {
    use std::hash::{BuildHasher, Hasher};
    std::collections::hash_map::RandomState::new()
        .build_hasher()
        .finish()
}

/// Primary fuzzer: generate multi-function C programs with scoped variables and
/// composed expressions, then require the translated Rust to match the original
/// C on stdout and exit code.
///
/// By default each run uses fresh random seeds, so repeated runs explore
/// different programs. Set `SLATE_FUZZ_SEED=<n>` to make the run deterministic
/// (seeds `n, n+1, ...`); a failure reports the exact seed, so
/// `SLATE_FUZZ_SEED=<seed> SLATE_FUZZ_CASES=1` replays that one program. The
/// generated `.c`/`.generated.rs` are also left under `target/cgen-fuzz/`.
#[test]
fn generator_differential() {
    let cases_n: u64 = std::env::var("SLATE_FUZZ_CASES")
        .ok()
        .and_then(|s| s.parse().ok())
        .unwrap_or(8);
    let base: Option<u64> = std::env::var("SLATE_FUZZ_SEED")
        .ok()
        .and_then(|s| s.parse().ok());
    match base {
        Some(b) => eprintln!("seeds: deterministic from SLATE_FUZZ_SEED={b}"),
        None => eprintln!("seeds: random (set SLATE_FUZZ_SEED=<n> to replay a reported seed)"),
    }

    let manifest = Path::new(env!("CARGO_MANIFEST_DIR"));
    let tmp = manifest.join("target/cgen-fuzz");
    std::fs::create_dir_all(&tmp).expect("create cgen fuzz dir");

    let mut cases = Vec::new();
    let mut seeds = Vec::new();
    let mut failures = Vec::new();
    for i in 0..cases_n {
        let seed = match base {
            Some(b) => b.wrapping_add(i),
            None => random_seed(),
        };
        let name = format!("cgen_seed_{seed:016x}");
        let c_src = tmp.join(format!("{name}.c"));
        let rs_src = tmp.join(format!("{name}.generated.rs"));
        let program = support::cgen::generate(seed);
        std::fs::write(&c_src, program).expect("write generated c");

        match support::translate(&c_src, &rs_src) {
            Ok(()) => {
                seeds.push(seed);
                cases.push(support::Case {
                    name,
                    c_src,
                    rs_src,
                });
            }
            Err(e) => {
                eprintln!("FAIL  {name}  (replay with SLATE_FUZZ_SEED={seed} SLATE_FUZZ_CASES=1)");
                failures.push(format!("[seed {seed}] {e}"));
            }
        }
    }

    for ((name, result), seed) in support::compare_batch(&cases, &tmp).into_iter().zip(&seeds) {
        match result {
            Ok(()) => eprintln!("ok    {name}"),
            Err(e) => {
                eprintln!("FAIL  {name}  (replay with SLATE_FUZZ_SEED={seed} SLATE_FUZZ_CASES=1)");
                failures.push(format!("[seed {seed}] {e}"));
            }
        }
    }

    if !failures.is_empty() {
        panic!(
            "{} of {} generated programs failed:\n\n{}",
            failures.len(),
            cases_n,
            failures.join("\n\n")
        );
    }
}
