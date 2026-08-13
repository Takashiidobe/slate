//! Stdlib coverage probes: for each small C program under `tests/stdlib/<header>/`,
//! translate it, compile the generated Rust, run both C and Rust, and require
//! identical stdout + exit code — the same bar as the differential harness.
//!
//! This maps which common libc functions Slate can translate today. Probes that
//! currently fail are listed in `KNOWN_UNSUPPORTED`; each has a tracking bead.
//! When a listed probe starts passing, the test fails and tells you to promote
//! it (drop it from the list) — so coverage only ratchets forward.
//!
//! Run just this suite with:
//!   cargo nextest r --release --test stdlib_coverage --nocapture

mod support;

use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};

struct UnsupportedProbe {
    probe: &'static str,
    bead: &'static str,
}

const KNOWN_UNSUPPORTED: &[UnsupportedProbe] = &[];

fn stdlib_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/stdlib")
}

/// (`header/name`, path) for every `tests/stdlib/<header>/<name>.c`, sorted.
fn probes() -> Vec<(String, PathBuf)> {
    let root = stdlib_dir();
    let mut out = Vec::new();
    for header in std::fs::read_dir(&root).expect("read stdlib dir") {
        let hpath = header.expect("header entry").path();
        if !hpath.is_dir() {
            continue;
        }
        let hname = hpath.file_name().unwrap().to_string_lossy().into_owned();
        for probe in std::fs::read_dir(&hpath).expect("read header dir") {
            let ppath = probe.expect("probe entry").path();
            if ppath.extension().and_then(|e| e.to_str()) != Some("c") {
                continue;
            }
            let pname = ppath.file_stem().unwrap().to_string_lossy().into_owned();
            out.push((format!("{hname}/{pname}"), ppath));
        }
    }
    out.sort_by(|a, b| a.0.cmp(&b.0));
    out
}

#[test]
fn stdlib_coverage() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/stdlib-coverage");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let probes = probes();
    assert!(!probes.is_empty(), "no probes found in {:?}", stdlib_dir());
    let known = known_unsupported_by_probe(&probes);

    let translated = support::parallel_map(&probes, |(id, c_src)| {
        let generated = tmp.join(format!("{}.generated.rs", id.replace('/', "__")));
        support::translate(c_src, &generated).and_then(|()| {
            let config = probe_config(c_src)
                .map_err(|e| format!("load run config for {}: {e}", c_src.display()))?;
            Ok(support::Case {
                name: id.clone(),
                c_src: c_src.clone(),
                rs_src: generated,
                config,
            })
        })
    });
    let mut cases = Vec::new();
    let mut known_cases = Vec::new();
    let mut results: BTreeMap<String, Result<(), String>> = BTreeMap::new();
    for ((id, _), result) in probes.iter().zip(translated) {
        match result {
            Ok(case) => {
                if known.contains_key(id.as_str()) {
                    known_cases.push(case);
                } else {
                    cases.push(case);
                }
            }
            Err(e) => {
                results.insert(id.clone(), Err(e));
            }
        }
    }

    for (name, result) in support::compare_batch(&cases, &tmp) {
        results.insert(name, result);
    }
    for (name, result) in support::compare_batch(&known_cases, &tmp.join("known-unsupported")) {
        results.insert(name, result);
    }

    // Report, grouped by header.
    let mut passed = Vec::new();
    let mut failed = Vec::new();
    eprintln!("\n=== stdlib coverage ===");
    let mut cur_header = String::new();
    for (id, result) in &results {
        let header = id.split('/').next().unwrap().to_string();
        if header != cur_header {
            eprintln!("\n[{header}]");
            cur_header = header;
        }
        let name = id.split('/').nth(1).unwrap();
        match result {
            Ok(()) => {
                eprintln!("  ok    {name}");
                passed.push(id.clone());
            }
            Err(e) => {
                let first = e.lines().next().unwrap_or("");
                eprintln!("  FAIL  {name}  ({first})");
                failed.push(id.clone());
            }
        }
    }
    eprintln!(
        "\n{} passed, {} failed, {} total\n",
        passed.len(),
        failed.len(),
        results.len()
    );

    let mut regressions = Vec::new();
    let mut promotable = Vec::new();
    for (id, result) in &results {
        match (result.is_ok(), known.get(id.as_str())) {
            (false, None) => regressions.push(format!(
                "[{id}] {}",
                result.as_ref().err().unwrap().lines().next().unwrap_or("")
            )),
            (true, Some(bead)) => promotable.push(format!("{id} ({bead})")),
            _ => {}
        }
    }

    let mut msg = String::new();
    if !regressions.is_empty() {
        msg.push_str(&format!(
            "{} probe(s) unexpectedly failed (create a focused bead, add UnsupportedProbe {{ probe, bead }} to KNOWN_UNSUPPORTED, or fix):\n{}\n",
            regressions.len(),
            regressions.join("\n")
        ));
    }
    if !promotable.is_empty() {
        msg.push_str(&format!(
            "{} probe(s) now pass — remove from KNOWN_UNSUPPORTED to lock them in:\n{}\n",
            promotable.len(),
            promotable.join("\n")
        ));
    }
    assert!(msg.is_empty(), "{msg}");
}

fn known_unsupported_by_probe(
    probes: &[(String, PathBuf)],
) -> BTreeMap<&'static str, &'static str> {
    let valid_probes: BTreeSet<&str> = probes.iter().map(|(id, _)| id.as_str()).collect();
    let mut known = BTreeMap::new();
    let mut problems = Vec::new();

    for entry in KNOWN_UNSUPPORTED {
        if !valid_probes.contains(entry.probe) {
            problems.push(format!(
                "KNOWN_UNSUPPORTED probe '{}' does not exist under tests/stdlib",
                entry.probe
            ));
        }
        if !entry.bead.starts_with("slate-") {
            problems.push(format!(
                "KNOWN_UNSUPPORTED probe '{}' has invalid bead id '{}'",
                entry.probe, entry.bead
            ));
        }
        if let Some(previous) = known.insert(entry.probe, entry.bead) {
            problems.push(format!(
                "KNOWN_UNSUPPORTED probe '{}' is listed twice ({previous}, {})",
                entry.probe, entry.bead
            ));
        }
    }

    assert!(
        problems.is_empty(),
        "invalid KNOWN_UNSUPPORTED entries:\n{}",
        problems.join("\n")
    );

    known
}

fn probe_config(c_src: &Path) -> Result<support::RunConfig, String> {
    let mut config = support::RunConfig {
        compare_stderr: true,
        ..support::RunConfig::default()
    };
    let base = c_src.with_extension("");

    let stdin = base.with_extension("stdin");
    if stdin.exists() {
        config.stdin =
            std::fs::read(&stdin).map_err(|e| format!("read {}: {e}", stdin.display()))?;
    }

    let args = base.with_extension("args");
    if args.exists() {
        let text =
            std::fs::read_to_string(&args).map_err(|e| format!("read {}: {e}", args.display()))?;
        config.args = text
            .lines()
            .map(str::trim)
            .filter(|line| !line.is_empty())
            .map(str::to_string)
            .collect();
    }

    let env = base.with_extension("env");
    if env.exists() {
        let text =
            std::fs::read_to_string(&env).map_err(|e| format!("read {}: {e}", env.display()))?;
        for (i, line) in text.lines().enumerate() {
            let line = line.trim();
            if line.is_empty() || line.starts_with('#') {
                continue;
            }
            let Some((key, value)) = line.split_once('=') else {
                return Err(format!("{}:{}: expected KEY=VALUE", env.display(), i + 1));
            };
            config.env.insert(key.to_string(), value.to_string());
        }
    }

    Ok(config)
}
