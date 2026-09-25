mod support;

use slate::api;
use slate::backend;
use std::path::{Path, PathBuf};
use support::filecheck::{self, Profile};

#[test]
fn generated_slate_differential() {
    let root = Path::new(env!("CARGO_MANIFEST_DIR"));
    let fixture_dir = root.join("tests/fixtures");
    let work = root.join("target/difftest-slate");
    std::fs::create_dir_all(&work).expect("create slate differential directory");
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut cases = Vec::new();
    let mut found = 0;
    for entry in std::fs::read_dir(&fixture_dir).expect("read fixture directory") {
        let path = entry.expect("fixture entry").path();
        if path.extension().is_none_or(|extension| extension != "c") {
            continue;
        }
        let name = path.file_stem().unwrap().to_string_lossy().into_owned();
        if selected.as_ref().is_some_and(|selected| selected != &name) {
            continue;
        }
        let source = std::fs::read_to_string(&path).expect("read fixture");
        let lowering_checks = filecheck::slate_checks(&source, Profile::Lowering);
        let rewrite_checks = filecheck::slate_checks(&source, Profile::Rewrites);
        if lowering_checks.is_empty() && rewrite_checks.is_empty() {
            continue;
        }
        assert!(
            !lowering_checks.is_empty() && !rewrite_checks.is_empty(),
            "{name}: both slate check prefixes are required"
        );
        found += 1;
        let program = api::lowered_slate_program_with_args(&path, &[])
            .unwrap_or_else(|error| panic!("{name}: lower: {error}"));
        for (profile, generated) in [
            (Profile::Lowering, program.emit()),
            (Profile::Rewrites, backend::apply(program.clone()).emit()),
        ] {
            let variant = match profile {
                Profile::Lowering => "lowering",
                Profile::Rewrites => "rewrites",
            };
            let generated = backend::format_rust(&generated)
                .unwrap_or_else(|error| panic!("{name}: format: {error}"));
            let checks = filecheck::slate_checks(&source, profile);
            filecheck::check_generated_rust(
                &checks,
                &generated,
                profile,
                &work.join("filecheck").join(format!("{name}-{variant}")),
            )
            .unwrap_or_else(|error| panic!("{name} ({variant}): {error}"));
            let output = work.join(format!("{name}-{variant}.rs"));
            std::fs::write(&output, generated).expect("write translated Rust");
            cases.push(support::Case {
                name: format!("{name}-{variant}"),
                c_src: PathBuf::from(&path),
                rs_src: output,
                config: support::RunConfig::default(),
            });
        }
    }
    assert!(found > 0, "no slate frontend fixtures selected");
    let failures = support::compare_batch(&cases, &work)
        .into_iter()
        .filter_map(|(name, result)| result.err().map(|error| format!("{name}: {error}")))
        .collect::<Vec<_>>();
    assert!(failures.is_empty(), "{}", failures.join("\n"));
}
