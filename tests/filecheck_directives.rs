mod support;

use std::path::Path;

use support::filecheck::{
    Profile, check_generated_rust, check_generated_rust_with_prefixes, has_checks,
};

fn work_dir(name: &str) -> std::path::PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/filecheck-directives")
        .join(name)
}

#[test]
fn enforces_rewrite_checks() {
    assert!(has_checks(
        "// REWRITES: rewritten_output\n",
        Profile::Rewrites
    ));
    assert!(
        check_generated_rust(
            "// REWRITES: rewritten_output\n",
            "raw_lowering_output\n",
            Profile::Rewrites,
            &work_dir("enforced-rewrites-failure"),
        )
        .is_err()
    );
    check_generated_rust(
        "// REWRITES: rewritten_output\n",
        "rewritten_output\n",
        Profile::Rewrites,
        &work_dir("enforced-rewrites-success"),
    )
    .unwrap();
}

#[test]
fn selects_lowering_and_common_checks_without_ordering() {
    let fixture = r#"
// COMMON-DAG: common_second
// COMMON-DAG: common_first
// LOWERING-DAG: lowering_only
// REWRITES-DAG: rewrites_only
"#;
    let lowering = "common_first\nlowering_only\ncommon_second\n";
    check_generated_rust(
        fixture,
        lowering,
        Profile::Lowering,
        &work_dir("lowering-profile"),
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

#[test]
fn applies_global_negative_checks_to_the_entire_generated_file() {
    let fixture = r#"
// LOWERING-DAG: required_assertion
// LOWERING-NOT: forbidden_assertion
"#;

    assert!(
        check_generated_rust(
            fixture,
            "forbidden_assertion();\nrequired_assertion();\nfn main() {}\n",
            Profile::Lowering,
            &work_dir("global-negative-present"),
        )
        .is_err()
    );
    check_generated_rust(
        fixture,
        "required_assertion();\nfn main() {}\n",
        Profile::Lowering,
        &work_dir("global-negative-absent"),
    )
    .unwrap();
}

#[test]
fn preserves_explicit_ordered_checks_inside_one_function() {
    let fixture = r#"
// LOWERING-LABEL: {{^}}fn wanted() {
// LOWERING: first();
// LOWERING-NEXT: second();
// LOWERING: third();
// LOWERING: {{^}}}
"#;
    let ordered = "fn wanted() {\nfirst();\nsecond();\nthird();\n}\n";
    let reordered = "fn wanted() {\nthird();\nfirst();\nsecond();\n}\n";

    check_generated_rust(
        fixture,
        ordered,
        Profile::Lowering,
        &work_dir("ordered-function-check"),
    )
    .unwrap();
    assert!(
        check_generated_rust(
            fixture,
            reordered,
            Profile::Lowering,
            &work_dir("reordered-function-check"),
        )
        .is_err()
    );
}

#[test]
fn attaches_global_next_checks_to_their_predecessor() {
    let fixture = r#"
// LOWERING: #[thread_local]
// LOWERING-NEXT: static mut value: i32 = 1;
"#;

    check_generated_rust(
        fixture,
        "#[thread_local]\nstatic mut value: i32 = 1;\n",
        Profile::Lowering,
        &work_dir("global-next"),
    )
    .unwrap();
    assert!(
        check_generated_rust(
            fixture,
            "#[thread_local]\nother_item();\nstatic mut value: i32 = 1;\n",
            Profile::Lowering,
            &work_dir("global-next-separated"),
        )
        .is_err()
    );
}

#[test]
fn selects_profile_qualified_target_prefixes() {
    let fixture = r#"
// LOWERING-DAG: lowered
// LOWERING-MACOS-DAG: f64_abi
// LOWERING-MSVC-DAG: llp64_abi
"#;

    check_generated_rust_with_prefixes(
        fixture,
        "f64_abi\nlowered\n",
        Profile::Lowering,
        &["LOWERING-MACOS"],
        &work_dir("macos-prefix"),
    )
    .unwrap();
    assert!(
        check_generated_rust_with_prefixes(
            fixture,
            "llp64_abi\nlowered\n",
            Profile::Lowering,
            &["LOWERING-MACOS"],
            &work_dir("wrong-target-prefix"),
        )
        .is_err()
    );
}
