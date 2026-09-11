use std::path::{Path, PathBuf};
use std::process::Command;

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

fn filecheck() -> String {
    std::env::var("SLATE_FILECHECK").unwrap_or_else(|_| "FileCheck".into())
}

fn configurations(source: &str) -> Vec<(String, Vec<String>)> {
    source
        .lines()
        .filter_map(|line| {
            let rest = line.trim().strip_prefix("// SLATE-FILECHECK-DEFINES ")?;
            let mut fields = rest.split_whitespace();
            let prefix = fields.next()?;
            Some((prefix.to_string(), fields.map(str::to_string).collect()))
        })
        .collect()
}

fn run_fixture(fixture: &Path, prefix: &str, defines: &[String], slot: usize) {
    let mut command = Command::new(env!("CARGO_BIN_EXE_slate-parser"));
    command.arg("filecheck").arg(fixture);
    for define in defines {
        command.arg(format!("-D{}", define.trim_start_matches("-D")));
    }
    let rendered = command
        .output()
        .expect("run slate-parser filecheck renderer");
    assert!(
        rendered.status.success(),
        "renderer failed for {}:\n{}",
        fixture.display(),
        String::from_utf8_lossy(&rendered.stderr)
    );

    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/filecheck")
        .join(format!(
            "{}.{}.{}",
            fixture.file_stem().unwrap().to_string_lossy(),
            std::process::id(),
            slot
        ));
    std::fs::create_dir_all(&work).expect("create FileCheck work directory");
    let input = work.join("rendered.txt");
    std::fs::write(&input, rendered.stdout).expect("write rendered AST");

    let result = Command::new(filecheck())
        .arg(fixture)
        .arg(format!("--check-prefix={prefix}"))
        .arg("--input-file")
        .arg(&input)
        .arg("--dump-input=fail")
        .output()
        .expect("run FileCheck");
    assert!(
        result.status.success(),
        "FileCheck failed for {} ({prefix}):\n{}{}",
        fixture.display(),
        String::from_utf8_lossy(&result.stdout),
        String::from_utf8_lossy(&result.stderr)
    );
}

#[test]
fn fixtures_are_filechecked() {
    let mut fixtures = std::fs::read_dir(fixtures_dir())
        .expect("read fixture directory")
        .map(|entry| entry.expect("read fixture entry").path())
        .filter(|path| path.extension().and_then(|extension| extension.to_str()) == Some("c"))
        .collect::<Vec<_>>();
    fixtures.sort();
    assert!(!fixtures.is_empty(), "no C fixtures found");

    let mut checked = 0;
    for fixture in fixtures {
        let source = std::fs::read_to_string(&fixture).expect("read fixture");
        let configs = configurations(&source);
        assert!(
            !configs.is_empty(),
            "fixture has no FileCheck configurations: {}",
            fixture.display()
        );
        for (slot, (prefix, defines)) in configs.iter().enumerate() {
            run_fixture(&fixture, prefix, defines, slot);
            checked += 1;
        }
    }
    assert!(checked > 0, "no FileCheck configurations found");
}
