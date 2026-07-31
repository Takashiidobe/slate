use std::path::{Path, PathBuf};
use std::process::Command;

const VERIFY_COMMAND: &str = "verify-incremental-facts";

fn slate() -> &'static str {
    env!("CARGO_BIN_EXE_slate")
}

fn fixture_paths() -> Vec<(String, PathBuf)> {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures");
    let mut fixtures = Vec::new();
    for entry in std::fs::read_dir(&dir).expect("read tests/fixtures") {
        let path = entry.expect("read fixture entry").path();
        if path.extension().and_then(|ext| ext.to_str()) != Some("c") {
            continue;
        }
        let name = path
            .file_name()
            .expect("fixture file name")
            .to_string_lossy()
            .into_owned();
        fixtures.push((name, path));
    }
    fixtures.sort_by(|a, b| a.0.cmp(&b.0));
    fixtures
}

fn selected_fixture_paths() -> Vec<(String, PathBuf)> {
    let Some(filter) = std::env::var("SLATE_INCREMENTAL_FACTS_FIXTURE")
        .ok()
        .filter(|filter| !filter.trim().is_empty())
    else {
        return fixture_paths();
    };
    let filter = filter.trim();
    let selected: Vec<_> = fixture_paths()
        .into_iter()
        .filter(|(name, path)| {
            name == filter || path.file_stem().and_then(|stem| stem.to_str()) == Some(filter)
        })
        .collect();
    assert!(
        !selected.is_empty(),
        "SLATE_INCREMENTAL_FACTS_FIXTURE={filter} did not match any tests/fixtures/*.c file"
    );
    selected
}

fn run_fixture(fixture: &Path) -> Result<(), String> {
    let output = Command::new(slate())
        .arg(VERIFY_COMMAND)
        .arg(fixture)
        .output()
        .map_err(|e| format!("spawn slate {VERIFY_COMMAND}: {e}"))?;
    if output.status.success() {
        return Ok(());
    }

    let stdout = String::from_utf8_lossy(&output.stdout);
    let stderr = String::from_utf8_lossy(&output.stderr);
    Err(format!(
        "slate {VERIFY_COMMAND} {} failed with status {}\nstdout:\n{stdout}\nstderr:\n{stderr}",
        fixture.display(),
        output.status
    ))
}

fn assert_all_selected_fixtures_match() {
    let mut failures = Vec::new();
    let mut passed = 0usize;
    for (name, path) in selected_fixture_paths() {
        match run_fixture(&path) {
            Ok(()) => passed += 1,
            Err(err) => failures.push(format!("[{name}] {err}")),
        }
    }

    if failures.is_empty() {
        return;
    }

    panic!(
        "{} fixture(s) passed; {} fixture(s) failed:\n\n{}",
        passed,
        failures.len(),
        failures.join("\n\n")
    );
}

#[test]
#[ignore = "diagnostic-only: incremental fact splicing vs full facts::analyze; run explicitly"]
fn all_fixtures_incremental_facts_match_full_reanalyze() {
    assert_all_selected_fixtures_match();
}
