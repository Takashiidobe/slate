mod support;

use std::path::{Path, PathBuf};

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

fn unsupported_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.unsupported")
}

struct Fixture {
    name: String,
    relative: PathBuf,
    path: PathBuf,
}

fn collect_fixtures(root: &Path, sub: &Path, selected: &Option<String>, out: &mut Vec<Fixture>) {
    let Ok(entries) = std::fs::read_dir(root.join(sub)) else {
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
        let relative = sub.join(path.file_name().unwrap());
        out.push(Fixture {
            name,
            relative,
            path,
        });
    }
}

fn fixtures(root: &Path) -> Vec<Fixture> {
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut fixtures = Vec::new();
    collect_fixtures(root, Path::new(""), &selected, &mut fixtures);
    if cfg!(target_arch = "x86_64") {
        collect_fixtures(root, Path::new("x86_64"), &selected, &mut fixtures);
    }
    fixtures.sort_by(|a, b| a.name.cmp(&b.name));
    fixtures
}

fn run_cases(group: &str, fixtures: &[Fixture]) -> Vec<(String, Result<(), String>)> {
    run_cases_with_mode(group, fixtures, false)
}

fn run_cases_with_mode(
    group: &str,
    fixtures: &[Fixture],
    project: bool,
) -> Vec<(String, Result<(), String>)> {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/difftest-generated")
        .join(group);
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let translated = support::parallel_map(fixtures, |f| {
        let generated = tmp.join(format!("{}.generated.rs", f.name));
        let dg_options = support::fixture_dg_options(&f.path);
        let mut extra_args = dg_options.clone();
        extra_args.extend(support::fixture_dg_additional_options(&f.path));
        let result = if project {
            let crate_dir = tmp.join(&f.name);
            translate_fixture_project(std::slice::from_ref(&f.path), &crate_dir, extra_args, &[])
                .and_then(|()| {
                    std::fs::copy(crate_dir.join("src/main.rs"), &generated)
                        .map(|_| ())
                        .map_err(|e| e.to_string())
                })
        } else {
            support::translate_slate(&f.path, &generated, &extra_args)
        };
        result.map(|()| {
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
    let mut results = Vec::new();
    for (f, result) in fixtures.iter().zip(translated) {
        match result {
            Ok(case) => cases.push(case),
            Err(e) => results.push((f.name.clone(), Err(e))),
        }
    }
    results.extend(support::compare_batch(&cases, &tmp));
    results.sort_by(|a, b| a.0.cmp(&b.0));
    results
}

fn translate_fixture_project(
    sources: &[PathBuf],
    crate_dir: &Path,
    extra_args: Vec<String>,
    translate_args: &[&str],
) -> Result<(), String> {
    std::fs::create_dir_all(crate_dir).expect("create project directory");
    let database = crate_dir.join("compile_commands.json");
    let directory = sources[0].parent().unwrap();
    let entries: Vec<_> = sources
        .iter()
        .map(|source| {
            let mut arguments = vec!["clang".to_string(), "-std=c23".into()];
            arguments.extend(extra_args.iter().cloned());
            arguments.push(source.display().to_string());
            serde_json::json!({
                "directory": directory, "file": source, "arguments": arguments,
            })
        })
        .collect();
    std::fs::write(&database, serde_json::to_vec(&entries).unwrap()).unwrap();
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-project")
        .args(translate_args)
        .arg("--compile-commands")
        .arg(database)
        .arg(directory)
        .arg(crate_dir)
        .output()
        .unwrap();
    if output.status.success() {
        Ok(())
    } else {
        Err(String::from_utf8_lossy(&output.stderr).into_owned())
    }
}

fn first_barrier(error: &str) -> (&'static str, &str) {
    let detail = error
        .lines()
        .map(str::trim)
        .find(|line| line.starts_with("error"))
        .unwrap_or_else(|| error.lines().next().unwrap_or_default());
    let class = if error.starts_with("slate translate failed") {
        if detail.contains("unsupported slate-parser IR") {
            "unsupported lowering"
        } else {
            "parse/sema"
        }
    } else if error.starts_with("Rust batch build failed") {
        "rustc"
    } else if error.starts_with("exit code differs")
        || error.starts_with("stdout differs")
        || error.starts_with("stderr differs")
    {
        "runtime mismatch"
    } else {
        "harness"
    };
    (class, detail)
}

#[test]
fn generated_differential() {
    let fixtures = fixtures(&fixtures_dir());
    assert!(
        !fixtures.is_empty() || std::env::var("SLATE_DIFF_FIXTURE").is_ok(),
        "no fixtures found in {:?}",
        fixtures_dir()
    );

    let mut failures = Vec::new();
    for (name, result) in run_cases("supported", &fixtures) {
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

#[test]
fn project_control_flow_differential() {
    let fixtures: Vec<_> = fixtures(&fixtures_dir())
        .into_iter()
        .filter(|f| f.name.starts_with("goto_") || f.name.starts_with("switch_"))
        .collect();
    let failures: Vec<_> = run_cases_with_mode("project-control-flow", &fixtures, true)
        .into_iter()
        .filter_map(|(name, result)| result.err().map(|error| format!("[{name}] {error}")))
        .collect();
    assert!(failures.is_empty(), "{}", failures.join("\n\n"));
}

#[test]
fn release_build_differential() {
    let work = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated/release");
    let root = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.release");
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut projects: Vec<(String, Vec<PathBuf>)> = std::fs::read_dir(&root)
        .expect("read release fixtures")
        .filter_map(|entry| {
            let path = entry.ok()?.path();
            let name = path.file_stem()?.to_string_lossy().into_owned();
            let mut sources: Vec<PathBuf> = if path.is_dir() {
                std::fs::read_dir(&path)
                    .ok()?
                    .filter_map(|entry| entry.ok().map(|entry| entry.path()))
                    .filter(|source| source.extension().is_some_and(|ext| ext == "c"))
                    .collect()
            } else if path.extension().is_some_and(|ext| ext == "c") {
                vec![path]
            } else {
                return None;
            };
            sources.sort();
            (selected.as_ref().is_none_or(|selected| *selected == name) && !sources.is_empty())
                .then_some((name, sources))
        })
        .collect();
    projects.sort();
    let failures: Vec<_> = support::parallel_map(&projects, |(name, sources)| {
        let crate_dir = work.join(name);
        let result = (|| {
            translate_fixture_project(
                sources,
                &crate_dir,
                support::fixture_dg_options(&sources[0]),
                &[],
            )?;
            let rs_bin = support::build_project_release(&crate_dir)?;
            let c_bin = work.join(format!("{name}_c"));
            support::compile_c_multi_with_std_include_and_args(
                sources,
                &c_bin,
                "c23",
                sources[0].parent(),
                &[],
            )?;
            let config = support::RunConfig::default();
            let c = support::run_with_config(&c_bin, &config, &work)?;
            let r = support::run_with_config(&rs_bin, &config, &work)?;
            support::compare_runs(&c, &r, false)
        })();
        result.err().map(|error| format!("[{name}] {error}"))
    })
    .into_iter()
    .flatten()
    .collect();
    assert!(failures.is_empty(), "{}", failures.join("\n\n"));
}

fn c_sources_in(dir: &Path) -> Vec<PathBuf> {
    let mut sources: Vec<PathBuf> = std::fs::read_dir(dir)
        .into_iter()
        .flatten()
        .filter_map(|entry| entry.ok().map(|entry| entry.path()))
        .filter(|source| source.extension().is_some_and(|ext| ext == "c"))
        .collect();
    sources.sort();
    sources
}

#[test]
fn library_differential() {
    let work = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated/library");
    let root = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.library");
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut projects: Vec<(String, PathBuf)> = std::fs::read_dir(&root)
        .expect("read library fixtures")
        .filter_map(|entry| {
            let path = entry.ok()?.path();
            let name = path.file_name()?.to_string_lossy().into_owned();
            (path.join("src").is_dir() && selected.as_ref().is_none_or(|s| *s == name))
                .then_some((name, path))
        })
        .collect();
    projects.sort();
    let failures: Vec<_> = support::parallel_map(&projects, |(name, project)| {
        let src = project.join("src");
        let sources = c_sources_in(&src);
        let crate_dir = work.join(name);
        let result = (|| {
            translate_fixture_project(
                &sources,
                &crate_dir,
                Vec::new(),
                &["--crate-type", "staticlib"],
            )?;
            let archive = support::build_project_staticlib(&crate_dir)?;
            for driver in c_sources_in(&project.join("tests")) {
                let stem = driver.file_stem().unwrap().to_string_lossy().into_owned();
                let c_bin = work.join(format!("{name}_{stem}_c"));
                let rs_bin = work.join(format!("{name}_{stem}_rs"));
                let mut native = sources.clone();
                native.push(driver.clone());
                support::compile_c_multi_with_std_include_and_args(
                    &native,
                    &c_bin,
                    "c23",
                    Some(&src),
                    &[],
                )?;
                support::link_c_with_archive(
                    std::slice::from_ref(&driver),
                    &archive,
                    &rs_bin,
                    Some(&src),
                )?;
                let config = support::RunConfig::default();
                let c = support::run_with_config(&c_bin, &config, &work)?;
                let r = support::run_with_config(&rs_bin, &config, &work)?;
                support::compare_runs(&c, &r, false).map_err(|e| format!("{stem}: {e}"))?;
            }
            Ok::<(), String>(())
        })();
        result.err().map(|error| format!("[{name}] {error}"))
    })
    .into_iter()
    .flatten()
    .collect();
    assert!(failures.is_empty(), "{}", failures.join("\n\n"));
}

#[test]
fn fixtures_unsupported_tests_still_fail() {
    let fixtures = fixtures(&unsupported_dir());
    let results = run_cases("unsupported", &fixtures);
    let unexpected_passes: Vec<String> = results
        .into_iter()
        .filter(|(_, result)| result.is_ok())
        .filter_map(|(name, _)| fixtures.iter().find(|f| f.name == name))
        .map(|f| {
            format!(
                "  git mv tests/fixtures.unsupported/{0} tests/fixtures/{0}",
                f.relative.display()
            )
        })
        .collect();
    assert!(
        unexpected_passes.is_empty(),
        "fixture(s) now pass end-to-end -- promote them:\n{}",
        unexpected_passes.join("\n")
    );
}

#[test]
#[ignore]
fn fixtures_unsupported_triage_report() {
    let fixtures = fixtures(&unsupported_dir());
    for (name, result) in run_cases("unsupported", &fixtures) {
        match result {
            Ok(()) => println!("PASS {name}"),
            Err(error) => {
                let (class, detail) = first_barrier(&error);
                println!("FAIL {name} [{class}] {detail}");
            }
        }
    }
}

#[test]
fn translation_is_self_hosted() {
    let source = fixtures_dir().join("atoi_atol_prelude_dynamic.c");
    let work = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated/self-hosted");
    std::fs::create_dir_all(&work).expect("create self-hosted test directory");
    let commands = work.join("compile_commands.json");
    std::fs::write(
        &commands,
        serde_json::to_vec(&serde_json::json!([{
            "directory": env!("CARGO_MANIFEST_DIR"),
            "file": source,
            "arguments": ["clang", "--target=x86_64-unknown-linux-gnu", "-c", source],
        }]))
        .expect("encode compile commands"),
    )
    .expect("write compile commands");
    let target = "--target=x86_64-unknown-linux-gnu";
    let inputs = [
        vec!["translate", target, source.to_str().unwrap()],
        vec!["record-cfg", source.to_str().unwrap(), target],
        vec![
            "translate-project",
            "--compile-commands",
            commands.to_str().unwrap(),
            env!("CARGO_MANIFEST_DIR"),
            work.to_str().unwrap(),
        ],
    ];
    for args in inputs {
        let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
            .args(&args)
            .env("PATH", "")
            .env("SLATE_TARGET", "invalid-default-target")
            .env_remove("SLATE_CLANG_ARGS")
            .output()
            .expect("run Slate without external tools");
        assert!(
            output.status.success(),
            "{} failed without external tools:\n{}",
            args[0],
            String::from_utf8_lossy(&output.stderr)
        );
    }
}
