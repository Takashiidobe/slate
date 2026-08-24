mod support;

use std::path::Path;

use support::filecheck::{Profile, check_generated_rust};

fn work_dir(name: &str) -> std::path::PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/filecheck-directives")
        .join(name)
}

#[test]
fn selects_profile_checks_and_matches_globals_without_ordering() {
    let fixture = r#"
// COMMON-DAG: common_second
// COMMON-DAG: common_first
// LOWERING-DAG: lowering_only
// REWRITES-DAG: rewrites_only
"#;
    let lowering = "common_first\nlowering_only\ncommon_second\n";
    let rewrites = "rewrites_only\ncommon_first\ncommon_second\n";

    check_generated_rust(
        fixture,
        lowering,
        Profile::Lowering,
        &work_dir("lowering-profile"),
    )
    .unwrap();
    check_generated_rust(
        fixture,
        rewrites,
        Profile::Rewrites,
        &work_dir("rewrites-profile"),
    )
    .unwrap();
}

#[test]
fn bounds_an_independent_label_block_to_one_function() {
    let fixture = r#"
// LOWERING-LABEL: {{^}}fn wanted() {
// LOWERING-DAG: wanted_assertion
// LOWERING-NOT: forbidden_assertion
// LOWERING: {{^}}}
"#;
    let misplaced = r#"fn wanted() {
}

fn other() {
    wanted_assertion();
}
"#;
    let contained = r#"fn other() {
    wanted_assertion();
}

fn wanted() {
    wanted_assertion();
}
"#;

    assert!(
        check_generated_rust(
            fixture,
            misplaced,
            Profile::Lowering,
            &work_dir("misplaced-function-check"),
        )
        .is_err()
    );
    check_generated_rust(
        fixture,
        contained,
        Profile::Lowering,
        &work_dir("contained-function-check"),
    )
    .unwrap();
}
