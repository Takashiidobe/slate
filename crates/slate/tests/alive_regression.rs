//! Opt-in alive-tv translation-validation regression suite for `src/lower.rs`
//! (slate-4us epic). For each fixture under `tests/fixtures/`, compiles the C
//! source and the `SLATE_RAW_LOWER=1` baseline Rust lowering to LLVM IR and
//! asks alive-tv whether the Rust refines the C, function by function.
//!
//! Not part of the default `cargo nextest r --release` run — invoke
//! explicitly via `cargo nextest r --release --test alive_regression`, same
//! posture as the `bnf_fuzz` target. Skips cleanly (rather than failing) when
//! alive-tv isn't available, matching the other toolchain-prerequisite tools
//! in CLAUDE.md's table.
//!
//! Runs every fixture by default; set `SLATE_ALIVE_FIXTURE=<name>` (fixture
//! file stem, e.g. `range_loop_local_array`) to check just one while
//! debugging a specific lowering bug. Some fixtures (large integer types in
//! particular) make alive-tv's solver run arbitrarily long even at a modest
//! unroll bound — those are reported as timeouts, not failures, since a
//! timeout is a tool limitation and not evidence of an incorrect lowering.
//! Only a confirmed "incorrect transformation" or a translate/compile error
//! fails the test.

mod support;

use std::path::{Path, PathBuf};
use std::time::Duration;
use support::alive::{VerifyOutcome, emit_lowering_ir, verify};

fn unroll() -> u32 {
    std::env::var("SLATE_ALIVE_UNROLL")
        .ok()
        .and_then(|v| v.parse().ok())
        .unwrap_or(40)
}

fn timeout() -> Duration {
    let secs = std::env::var("SLATE_ALIVE_TIMEOUT_SECS")
        .ok()
        .and_then(|v| v.parse().ok())
        .unwrap_or(30);
    Duration::from_secs(secs)
}

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

fn fixtures() -> Vec<(String, PathBuf)> {
    let dir = fixtures_dir();
    let mut fixtures = Vec::new();
    for entry in std::fs::read_dir(&dir).expect("read fixtures dir") {
        let path = entry.expect("dir entry").path();
        if path.extension().and_then(|e| e.to_str()) != Some("c") {
            continue;
        }
        let name = path.file_stem().unwrap().to_string_lossy().into_owned();
        fixtures.push((name, path));
    }
    if let Ok(only) = std::env::var("SLATE_ALIVE_FIXTURE") {
        let only = only.strip_suffix(".c").unwrap_or(&only).to_string();
        fixtures.retain(|(name, _)| *name == only);
        assert!(
            !fixtures.is_empty(),
            "no fixture named {only:?} in {:?}",
            fixtures_dir()
        );
    }
    fixtures.sort_by(|a, b| a.0.cmp(&b.0));
    fixtures
}

fn alive_tv_available() -> bool {
    std::process::Command::new(support::alive_tv())
        .arg("--version")
        .output()
        .is_ok()
}

#[test]
fn baseline_lowering_matches_c_semantics() {
    if !alive_tv_available() {
        eprintln!(
            "skipping: alive-tv not found at {} (set SLATE_ALIVE_TV)",
            support::alive_tv()
        );
        return;
    }

    let work_dir = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/alive_regression");
    std::fs::create_dir_all(&work_dir).unwrap();

    let unroll = unroll();
    let timeout = timeout();
    let fixtures = fixtures();

    let mut bugs = Vec::new();
    let mut inconclusive = Vec::new();
    let mut vacuous = Vec::new();

    for (name, fixture) in &fixtures {
        let result = (|| -> Result<VerifyOutcome, String> {
            let ir = emit_lowering_ir(fixture, &work_dir)?;
            verify(&ir.c_ll, &ir.rs_ll, unroll, timeout)
        })();

        match result {
            Ok(VerifyOutcome::Completed(summary)) if summary.ok() => {
                eprintln!("ok           {name}");
            }
            Ok(VerifyOutcome::Completed(summary)) if summary.incorrect > 0 => {
                eprintln!("MISMATCH     {name}");
                bugs.push(format!(
                    "{name}: alive-tv found an incorrect transformation:\n{}",
                    summary.output
                ));
            }
            Ok(VerifyOutcome::Completed(summary)) if summary.vacuous() => {
                eprintln!("vacuous      {name} (no top-level fn besides main)");
                vacuous.push(name.clone());
            }
            Ok(VerifyOutcome::Completed(summary)) => {
                eprintln!("inconclusive {name} (failed to prove)");
                inconclusive.push(format!(
                    "{name}: {} correct, {} failed-to-prove (raise SLATE_ALIVE_UNROLL?)",
                    summary.correct, summary.failed_to_prove
                ));
            }
            Ok(VerifyOutcome::TimedOut) => {
                eprintln!("timeout      {name}");
                inconclusive.push(format!(
                    "{name}: alive-tv exceeded {timeout:?} (common with wide/large-int fixtures; \
                     raise SLATE_ALIVE_TIMEOUT_SECS to force a verdict)"
                ));
            }
            Err(e) => {
                eprintln!("ERROR        {name}");
                bugs.push(format!("{name}: {e}"));
            }
        }
    }

    if !vacuous.is_empty() {
        eprintln!(
            "\n{} fixture(s) had no top-level fn besides main, so nothing was checked \
             (logic lives entirely in main, which this harness can't pair against C's \
             differently-shaped main): {}",
            vacuous.len(),
            vacuous.join(", ")
        );
    }

    if !inconclusive.is_empty() {
        eprintln!(
            "\n{} fixture(s) inconclusive (tool limitation, not a confirmed bug):\n{}",
            inconclusive.len(),
            inconclusive.join("\n")
        );
    }

    assert!(
        bugs.is_empty(),
        "{} of {} fixtures show a lowering bug:\n\n{}",
        bugs.len(),
        fixtures.len(),
        bugs.join("\n\n")
    );
}
