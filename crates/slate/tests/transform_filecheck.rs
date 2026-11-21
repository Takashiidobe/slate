mod support;

use std::path::{Path, PathBuf};
use std::process::Command;

const DEFAULT_TARGET: &str = "x86_64-unknown-linux-gnu";

fn function_bodies(ir: &str) -> String {
    let mut bodies = String::new();
    let mut inside = false;
    for line in ir.lines() {
        inside |= line.starts_with("    fn ") && line.ends_with('{');
        if inside {
            bodies.push_str(line.strip_prefix("    ").unwrap_or(line));
            bodies.push('\n');
            inside = line != "    }";
        }
    }
    bodies
}

fn targets(text: &str) -> Vec<String> {
    text.lines()
        .find_map(|line| line.strip_prefix("// SLATE-FILECHECK-BEGIN"))
        .map(|targets| targets.split_whitespace().map(str::to_string).collect())
        .filter(|targets: &Vec<String>| !targets.is_empty())
        .unwrap_or_else(|| vec![DEFAULT_TARGET.to_string()])
}

fn emit(fixture: &Path, passes: &str, args: &[String], generated: &Path) -> Result<(), String> {
    let key_path = PathBuf::from(format!("{}.translate-key", generated.display()));
    let key = support::translation_key(fixture, args);
    if generated.is_file()
        && key
            .as_ref()
            .is_some_and(|key| std::fs::read_to_string(&key_path).is_ok_and(|old| old == *key))
    {
        return Ok(());
    }
    let _ = std::fs::remove_file(&key_path);
    let output = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("emit-slate-ir")
        .arg(format!("--passes={passes}"))
        .args(args)
        .arg(fixture)
        .env("SLATE_SYSROOTS", slate_parser::sysroot::root())
        .env_remove("SLATE_TARGET")
        .env_remove("SLATE_CLANG_ARGS")
        .output()
        .map_err(|error| format!("spawn slate emit-slate-ir: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "slate emit-slate-ir failed ({}):\n{}",
            output.status,
            String::from_utf8_lossy(&output.stderr)
        ));
    }
    let bodies = function_bodies(&String::from_utf8_lossy(&output.stdout));
    support::write_if_changed(generated, bodies.as_bytes())
        .map_err(|error| format!("write {}: {error}", generated.display()))?;
    if let Some(key) = key {
        std::fs::write(&key_path, key)
            .map_err(|error| format!("write {}: {error}", key_path.display()))?;
    }
    Ok(())
}

fn check_fixture(fixture: &Path, target: &str) -> Result<(), String> {
    let passes = fixture
        .parent()
        .and_then(Path::file_name)
        .and_then(|name| name.to_str())
        .ok_or("fixture is not in a pass directory")?;
    let stem = fixture.file_stem().unwrap().to_string_lossy();
    let text = std::fs::read_to_string(fixture).map_err(|error| error.to_string())?;
    let mut args = vec![format!("--target={target}")];
    args.extend(
        text.lines()
            .find_map(|line| line.strip_prefix("// SLATE-TRANSFORM-ARGS "))
            .into_iter()
            .flat_map(str::split_whitespace)
            .map(str::to_string),
    );
    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/difftest-generated/transforms")
        .join(target);
    std::fs::create_dir_all(&work).map_err(|error| error.to_string())?;
    let name = format!("{passes}_{stem}");
    let generated = work.join(format!("{name}.generated.rs"));
    let marker = work.join(format!("{name}.runtime-ok"));
    let _ = std::fs::remove_file(&marker);
    emit(fixture, passes, &args, &generated)?;
    std::fs::write(&marker, b"").map_err(|error| error.to_string())?;
    let bodies = std::fs::read_to_string(&generated).map_err(|error| error.to_string())?;
    support::filecheck::check_generated_rust(
        &text,
        &bodies,
        target,
        &work.join("filecheck").join(name),
    )
}

#[test]
fn transform_filecheck() {
    let root = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.transforms");
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let requested = std::env::var("SLATE_TEST_TARGETS").ok();
    let mut fixtures = Vec::<PathBuf>::new();
    for pass in std::fs::read_dir(&root).expect("transform fixture directories") {
        for fixture in std::fs::read_dir(pass.expect("pass directory").path()).expect("fixtures") {
            let path = fixture.expect("fixture").path();
            if path.extension().is_some_and(|extension| extension == "c")
                && selected
                    .as_ref()
                    .is_none_or(|selected| path.file_stem().unwrap() == selected.as_str())
            {
                fixtures.push(path);
            }
        }
    }
    fixtures.sort();
    let jobs: Vec<(PathBuf, String)> = fixtures
        .into_iter()
        .flat_map(|fixture| {
            let text = std::fs::read_to_string(&fixture).expect("read transform fixture");
            targets(&text)
                .into_iter()
                .filter(|target| {
                    requested
                        .as_ref()
                        .is_none_or(|requested| requested.split(',').any(|wanted| wanted == target))
                })
                .map(move |target| (fixture.clone(), target))
                .collect::<Vec<_>>()
        })
        .collect();
    assert!(!jobs.is_empty(), "no transform fixtures selected");
    let failures: Vec<String> = support::parallel_map(&jobs, |(fixture, target)| {
        check_fixture(fixture, target)
            .err()
            .map(|error| format!("{} ({target}): {error}", fixture.display()))
    })
    .into_iter()
    .flatten()
    .collect();
    assert!(failures.is_empty(), "{}", failures.join("\n\n"));
}
