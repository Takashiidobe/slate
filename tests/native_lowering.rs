mod support;

use std::path::{Path, PathBuf};

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

fn fixtures() -> Vec<(String, PathBuf)> {
    let dir = fixtures_dir();
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut out = Vec::new();
    let Ok(entries) = std::fs::read_dir(&dir) else {
        return out;
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
        out.push((name, path));
    }
    out.sort_by(|a, b| a.0.cmp(&b.0));
    out
}

#[test]
fn generated_differential_native() {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-native-generated");
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let fixtures = fixtures();
    assert!(
        !fixtures.is_empty(),
        "no fixtures found in {:?}",
        fixtures_dir()
    );

    let mut failures = Vec::new();

    let translated = support::parallel_map(&fixtures, |(name, path)| {
        let generated = tmp.join(format!("{name}.generated.rs"));
        support::translate_native(path, &generated).map(|()| support::Case {
            name: name.clone(),
            c_src: path.clone(),
            rs_src: generated,
            config: support::RunConfig::default(),
        })
    });

    let mut cases = Vec::new();
    for ((name, _), result) in fixtures.iter().zip(translated) {
        match result {
            Ok(case) => cases.push(case),
            Err(e) => {
                eprintln!("FAIL  {name}");
                failures.push(format!("[{name}] {e}"));
            }
        }
    }

    let mut passed = 0usize;
    for (name, result) in support::compare_batch(&cases, &tmp) {
        match result {
            Ok(()) => {
                eprintln!("ok    {name}");
                passed += 1;
            }
            Err(e) => {
                eprintln!("FAIL  {name}");
                failures.push(format!("[{name}] {e}"));
            }
        }
    }

    eprintln!(
        "\nnative lowering parity: {passed}/{} fixtures passing",
        fixtures.len()
    );

    if !failures.is_empty() {
        panic!(
            "{} of {} generated fixtures failed under the native frontend:\n\n{}",
            failures.len(),
            fixtures.len(),
            failures.join("\n\n")
        );
    }
}
