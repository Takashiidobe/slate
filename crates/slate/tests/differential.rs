mod support;

use std::path::{Path, PathBuf};

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

struct Fixture {
    name: String,
    path: PathBuf,
}

fn collect_fixtures(dir: &Path, selected: &Option<String>, out: &mut Vec<Fixture>) {
    let Ok(entries) = std::fs::read_dir(dir) else {
        return;
    };
    for entry in entries {
        let path = entry.expect("dir entry").path();
        if path.extension().and_then(|e| e.to_str()) != Some("c") {
            continue;
        }
        let name = path.file_stem().unwrap().to_string_lossy().into_owned();
        if selected.as_ref().is_some_and(|selected| selected != &name) {
            continue;
        }
        out.push(Fixture { name, path });
    }
}

fn fixtures() -> Vec<Fixture> {
    let dir = fixtures_dir();
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut fixtures = Vec::new();
    collect_fixtures(&dir, &selected, &mut fixtures);
    if cfg!(target_arch = "x86_64") {
        collect_fixtures(&dir.join("x86_64"), &selected, &mut fixtures);
    }
    fixtures.sort_by(|a, b| a.name.cmp(&b.name));
    fixtures
}

#[test]
fn generated_differential() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let fixtures = fixtures();
    assert!(
        !fixtures.is_empty(),
        "no fixtures found in {:?}",
        fixtures_dir()
    );

    let mut failures = Vec::new();

    let translated = support::parallel_map(&fixtures, |f| {
        let generated = tmp.join(format!("{}.generated.rs", f.name));
        let dg_options = support::fixture_dg_options(&f.path);
        let mut extra_args = dg_options.clone();
        extra_args.extend(support::fixture_dg_additional_options(&f.path));
        support::translate_slate(&f.path, &generated, &extra_args).map(|()| {
            let mut config = support::RunConfig::default();
            config.c_args.extend(dg_options);
            support::Case {
                name: f.name.clone(),
                c_src: f.path.clone(),
                rs_src: generated,
                config,
            }
        })
    });
    let mut cases = Vec::new();
    for (f, result) in fixtures.iter().zip(translated) {
        match result {
            Ok(case) => cases.push(case),
            Err(e) => {
                eprintln!("FAIL  {}", f.name);
                failures.push(format!("[{}] {e}", f.name));
            }
        }
    }

    for (name, result) in support::compare_batch(&cases, &tmp) {
        match result {
            Ok(()) => eprintln!("ok    {name}"),
            Err(e) => {
                eprintln!("FAIL  {name}");
                failures.push(format!("[{name}] {e}"));
            }
        }
    }

    if !failures.is_empty() {
        panic!(
            "{} of {} generated fixtures failed:\n\n{}",
            failures.len(),
            fixtures.len(),
            failures.join("\n\n")
        );
    }
}
