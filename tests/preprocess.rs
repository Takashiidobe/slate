//! Integration coverage for the `record-cfg` preprocessing oracle (slate-lq0.2).
//!
//! Asserts that the conditional regions of the cfg fixtures are recorded with
//! the `cfg` mappings from `expected_cfgs.json`, with source ranges, and with
//! active/inactive state resolved for a forced macro environment.

use serde_json::Value;
use std::path::{Path, PathBuf};
use std::process::Command;

fn cfg_fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.cfg")
}

fn record_cfg(name: &str, clang_args: &[&str]) -> Value {
    let src = cfg_fixtures_dir().join(name);
    let out = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("record-cfg")
        .arg(&src)
        .args(clang_args)
        .output()
        .expect("run slate record-cfg");
    assert!(
        out.status.success(),
        "record-cfg failed for {name}:\n{}",
        String::from_utf8_lossy(&out.stderr)
    );
    serde_json::from_slice(&out.stdout).expect("record-cfg output is JSON")
}

fn expected_cfgs() -> Value {
    let path = cfg_fixtures_dir().join("expected_cfgs.json");
    let text = std::fs::read_to_string(path).expect("read expected_cfgs.json");
    serde_json::from_str(&text).expect("parse expected_cfgs.json")
}

fn recorded_cfgs(doc: &Value) -> Vec<String> {
    doc["chains"]
        .as_array()
        .unwrap()
        .iter()
        .flat_map(|chain| chain["branches"].as_array().unwrap())
        .filter_map(|branch| branch["rust_cfg"].as_str().map(str::to_string))
        .collect()
}

#[test]
fn records_msvc_slate_target_features() {
    let src = cfg_fixtures_dir().join("slate_msvc_target.c");
    let output = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("record-cfg")
        .arg(src)
        .env("SLATE_TARGET", "x86_64-pc-windows-msvc")
        .output()
        .expect("record MSVC Slate target");
    assert!(
        output.status.success(),
        "record-cfg failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let doc: Value = serde_json::from_slice(&output.stdout).expect("record-cfg output is JSON");
    assert_eq!(doc["directives"][0]["active"], true);
}

#[test]
fn record_cfg_exposes_the_directive_ledger() {
    let doc = record_cfg("feature_nested.c", &[]);
    let directives = doc["directives"].as_array().expect("directives array");

    assert_eq!(directives.len(), 6);
    assert_eq!(directives[0]["name"], "ifdef");
    assert_eq!(directives[0]["raw_payload"], "OUTER_FEATURE");
    assert_eq!(directives[0]["line_start"], 3);
    assert_eq!(directives[0]["line_end"], 3);
    assert_eq!(directives[0]["depth"], 0);
    assert_eq!(directives[0]["condition"], "defined(OUTER_FEATURE)");
    assert_eq!(directives[0]["active"], false);
    assert_eq!(directives[0]["disposition"], "represented-in-rust");

    assert_eq!(directives[1]["name"], "ifdef");
    assert_eq!(directives[1]["depth"], 1);
    assert_eq!(
        directives[1]["condition"],
        "(defined(OUTER_FEATURE)) && (defined(INNER_FEATURE))"
    );
    assert!(
        directives[1]["byte_end"].as_u64().unwrap() > directives[1]["byte_start"].as_u64().unwrap()
    );
}

#[test]
fn records_c23_elifdef_and_elifndef_activity() {
    let doc = record_cfg("../fixtures/elifdef.c", &[]);
    let directives = doc["directives"].as_array().expect("directives array");
    let branches: Vec<_> = directives
        .iter()
        .filter(|directive| matches!(directive["name"].as_str(), Some("elifdef" | "elifndef")))
        .map(|directive| {
            (
                directive["name"].as_str().unwrap(),
                directive["disposition"].as_str().unwrap(),
                directive["active"].as_bool().unwrap(),
            )
        })
        .collect();

    assert_eq!(
        branches,
        vec![
            ("elifdef", "represented-in-rust", true),
            ("elifndef", "represented-in-rust", false),
            ("elifdef", "represented-in-rust", false),
            ("elifndef", "represented-in-rust", true),
            ("elifdef", "represented-in-rust", false),
            ("elifndef", "represented-in-rust", false),
        ]
    );
}

#[test]
fn directive_ledger_exposes_every_disposition() {
    let doc = record_cfg("reject/directive_dispositions.c", &[]);
    let directives = doc["directives"].as_array().expect("directives array");
    let recorded: Vec<_> = directives
        .iter()
        .map(|directive| {
            (
                directive["name"].as_str().unwrap(),
                directive["disposition"].as_str().unwrap(),
            )
        })
        .collect();

    assert_eq!(
        recorded,
        vec![
            ("define", "consumed-by-clang"),
            ("if", "represented-in-rust"),
            ("error", "represented-in-rust"),
            ("endif", "represented-in-rust"),
            ("warning", "diagnostic-only"),
            ("", "no-output"),
            ("pragma", "unsupported-semantic"),
            ("slate_unknown", "unsupported-semantic"),
        ]
    );
}

#[test]
fn common_nonconditional_directives_have_explicit_dispositions() {
    let doc = record_cfg("reject/common_directive_dispositions.c", &[]);
    let recorded: Vec<_> = doc["directives"]
        .as_array()
        .unwrap()
        .iter()
        .map(|directive| {
            (
                directive["name"].as_str().unwrap(),
                directive["disposition"].as_str().unwrap(),
            )
        })
        .collect();

    assert_eq!(
        recorded,
        vec![
            ("define", "consumed-by-clang"),
            ("undef", "consumed-by-clang"),
            ("include", "consumed-by-clang"),
            ("include_next", "consumed-by-clang"),
            ("import", "consumed-by-clang"),
            ("line", "consumed-by-clang"),
            ("ident", "no-output"),
            ("sccs", "no-output"),
            ("embed", "consumed-by-clang"),
            ("", "no-output"),
        ]
    );
}

/// Every predicate mapping in `expected_cfgs.json` (variants and fallback) must
/// appear among the recorded branch cfgs for its source.
#[test]
fn records_expected_cfg_mappings_for_every_fixture() {
    let expected = expected_cfgs();
    for fixture in expected["fixtures"].as_array().unwrap() {
        let source = fixture["source"].as_str().unwrap();
        let recorded = recorded_cfgs(&record_cfg(source, &[]));

        for variant in fixture["variants"].as_array().unwrap() {
            let want = variant["rust_cfg"].as_str().unwrap();
            assert!(
                recorded.iter().any(|c| c == want),
                "{source}: expected recorded cfg {want:?}, got {recorded:?}"
            );
        }
        if let Some(fallback) = fixture.get("fallback") {
            let want = fallback["rust_cfg"].as_str().unwrap();
            assert!(
                recorded.iter().any(|c| c == want),
                "{source}: expected fallback cfg {want:?}, got {recorded:?}"
            );
        }
    }
}

/// Every recorded branch carries a body source range and a directive line, and
/// known fixtures produce no opaque-predicate diagnostics.
#[test]
fn records_source_ranges_without_diagnostics() {
    for source in [
        "os_targets.c",
        "win64_target.c",
        "unix_target.c",
        "arch_targets.c",
        "pointer_width_targets.c",
        "arm_endian_targets.c",
        "ndebug.c",
        "feature_single.c",
        "feature_multiple.c",
        "feature_nested.c",
    ] {
        let doc = record_cfg(source, &[]);
        assert!(
            doc["diagnostics"].as_array().unwrap().is_empty(),
            "{source}: unexpected diagnostics {:?}",
            doc["diagnostics"]
        );
        for chain in doc["chains"].as_array().unwrap() {
            assert!(chain["open_line"].as_u64().unwrap() >= 1);
            assert!(chain["endif_line"].as_u64().unwrap() > chain["open_line"].as_u64().unwrap());
            for branch in chain["branches"].as_array().unwrap() {
                let line = branch["directive_line"].as_u64().unwrap();
                let body_start = branch["body_start"].as_u64().unwrap();
                assert_eq!(
                    body_start,
                    line + 1,
                    "{source}: body starts after directive"
                );
                assert!(branch["rust_cfg"].is_string(), "{source}: known cfg mapped");
            }
        }
    }
}

/// An unknown project macro is reported as a structured `unmapped-macro`
/// diagnostic that names the macro and its source line, distinct from a
/// predicate shape we cannot normalize at all (slate-lq0.6).
#[test]
fn reports_reserved_system_macro_with_kind_and_location() {
    let doc = record_cfg("reject/system_macro_feature.c", &[]);
    let diags = doc["diagnostics"].as_array().expect("diagnostics array");
    let unmapped = diags
        .iter()
        .find(|d| d["kind"] == "unmapped-macro")
        .expect("an unmapped-macro diagnostic");
    assert_eq!(unmapped["line"].as_u64().unwrap(), 1);
    let message = unmapped["message"].as_str().unwrap();
    assert!(
        message.contains("_FILE_OFFSET_BITS"),
        "diagnostic should name the macro: {message}"
    );
    assert!(
        message.contains("uncovered"),
        "inactive unmapped branch should be reported uncovered: {message}"
    );
}

fn active_cfg(doc: &Value) -> Option<String> {
    doc["chains"]
        .as_array()
        .unwrap()
        .iter()
        .flat_map(|chain| chain["branches"].as_array().unwrap())
        .find(|branch| branch["active"] == Value::Bool(true))
        .and_then(|branch| branch["rust_cfg"].as_str().map(str::to_string))
}

/// A forced macro environment selects exactly one active branch, matching the
/// current invocation's config.
#[test]
fn resolves_the_single_active_branch_for_forced_macros() {
    let linux = record_cfg("os_targets.c", &["-D_WIN32", "-U__linux__", "-U__APPLE__"]);
    assert_eq!(active_cfg(&linux).as_deref(), Some("windows"));

    let apple = record_cfg("os_targets.c", &["-U_WIN32", "-U__linux__", "-D__APPLE__"]);
    assert_eq!(
        active_cfg(&apple).as_deref(),
        Some("target_vendor = \"apple\"")
    );

    let release = record_cfg("ndebug.c", &["-DNDEBUG"]);
    assert_eq!(
        active_cfg(&release).as_deref(),
        Some("not(debug_assertions)")
    );

    let debug = record_cfg("ndebug.c", &["-UNDEBUG"]);
    assert_eq!(active_cfg(&debug).as_deref(), Some("debug_assertions"));

    // exactly one active branch per chain.
    for chain in release["chains"].as_array().unwrap() {
        let active = chain["branches"]
            .as_array()
            .unwrap()
            .iter()
            .filter(|b| b["active"] == Value::Bool(true))
            .count();
        assert_eq!(active, 1, "exactly one active branch");
    }
}
