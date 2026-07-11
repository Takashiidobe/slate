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
//!   cargo test --test stdlib_coverage -- --nocapture

mod support;

use std::collections::BTreeMap;
use std::path::{Path, PathBuf};

/// `header/name` probe ids that do not yet translate+run correctly. Each entry
/// is tracked by a bead; remove it once the probe passes.
///   slate-61j  extern globals + FILE streams (stdout/stderr)
///   slate-aeo  struct-returning libc fns + libc aggregate typedefs (div_t)
const KNOWN_UNSUPPORTED: &[&str] = &[
    // stdio/* — FILE streams stdout/stderr (slate-61j)
    "stdio/fprintf",
    "stdio/fputs",
    // stdlib/*
    "stdlib/div", // slate-aeo (div_t)
];

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

    // Translate first; translate failures are recorded directly.
    let mut cases = Vec::new();
    let mut results: BTreeMap<String, Result<(), String>> = BTreeMap::new();
    for (id, c_src) in &probes {
        let generated = tmp.join(format!("{}.generated.rs", id.replace('/', "__")));
        match support::translate(c_src, &generated) {
            Ok(()) => cases.push(support::Case {
                name: id.clone(),
                c_src: c_src.clone(),
                rs_src: generated,
            }),
            Err(e) => {
                results.insert(id.clone(), Err(format!("translate: {e}")));
            }
        }
    }

    for (name, result) in support::compare_batch(&cases, &tmp) {
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

    // Ratchet: listed-unsupported probes may fail; everything else must pass.
    let known: std::collections::HashSet<&str> = KNOWN_UNSUPPORTED.iter().copied().collect();
    let mut regressions = Vec::new();
    let mut promotable = Vec::new();
    for (id, result) in &results {
        let listed = known.contains(id.as_str());
        match (result.is_ok(), listed) {
            (false, false) => regressions.push(format!(
                "[{id}] {}",
                result.as_ref().err().unwrap().lines().next().unwrap_or("")
            )),
            (true, true) => promotable.push(id.clone()),
            _ => {}
        }
    }

    let mut msg = String::new();
    if !regressions.is_empty() {
        msg.push_str(&format!(
            "{} probe(s) unexpectedly failed (add a bead + list in KNOWN_UNSUPPORTED, or fix):\n{}\n",
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
