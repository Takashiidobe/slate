use crate::backend::rust_ast::Program;
use std::collections::BTreeMap;
use std::sync::OnceLock;

pub(crate) fn enabled(pass: &str) -> bool {
    static PASSES: OnceLock<Vec<String>> = OnceLock::new();
    PASSES
        .get_or_init(|| {
            std::env::var("SLATE_SHADOW")
                .map(|passes| passes.split(',').map(str::to_owned).collect())
                .unwrap_or_default()
        })
        .iter()
        .any(|enabled| enabled == pass || enabled == "all")
}

type Key<'a> = (usize, &'a str, &'a str);

pub(in crate::backend) fn compare<'a>(
    pass: &str,
    programs: &'a [Program],
    rust: impl IntoIterator<Item = (usize, &'a str, &'a str, String)>,
) {
    if !enabled(pass) {
        return;
    }
    let ir: BTreeMap<Key<'a>, &'a str> = programs
        .iter()
        .enumerate()
        .flat_map(|(unit, program)| {
            program
                .shadow
                .iter()
                .filter(|decision| decision.pass == pass)
                .map(move |decision| {
                    (
                        (unit, decision.function.as_str(), decision.binding.as_str()),
                        decision.decision.as_str(),
                    )
                })
        })
        .collect();
    let rust: BTreeMap<Key<'a>, String> = rust
        .into_iter()
        .map(|(unit, function, binding, decision)| ((unit, function, binding), decision))
        .collect();
    let mut counts: BTreeMap<&str, usize> = BTreeMap::new();
    let mut keys: Vec<_> = rust.keys().chain(ir.keys()).copied().collect();
    keys.sort_unstable();
    keys.dedup();
    for key @ (unit, function, binding) in keys {
        let (kind, rust_side, ir_side) = match (rust.get(&key), ir.get(&key)) {
            (Some(rust), Some(ir)) if rust == ir => {
                *counts.entry("agree").or_default() += 1;
                continue;
            }
            (Some(rust), Some(ir)) => ("differ", rust.as_str(), *ir),
            (Some(rust), None) => ("rust_only", rust.as_str(), "-"),
            (None, Some(ir)) => ("ir_only", "-", *ir),
            (None, None) => continue,
        };
        *counts.entry(kind).or_default() += 1;
        eprintln!(
            "slate-shadow pass={pass} kind={kind} unit={unit} fn={function} binding={binding} rust={rust_side} ir={ir_side}"
        );
    }
    let summary: Vec<String> = ["agree", "differ", "rust_only", "ir_only"]
        .iter()
        .map(|kind| format!("{kind}={}", counts.get(kind).copied().unwrap_or(0)))
        .collect();
    eprintln!("slate-shadow pass={pass} summary {}", summary.join(" "));
}
