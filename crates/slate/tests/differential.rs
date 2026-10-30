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
    flavor: Flavor,
}

#[derive(Clone, Copy, PartialEq)]
enum Flavor {
    Clang,
    Gcc,
    Msvc,
}

const FLAVOR_FIXTURES: [(&str, Flavor); 2] = [("gcc", Flavor::Gcc), ("msvc", Flavor::Msvc)];

fn fixture_flavor(sub: &Path) -> Flavor {
    FLAVOR_FIXTURES
        .iter()
        .find(|(dir, _)| sub.starts_with(dir))
        .map_or(Flavor::Clang, |&(_, flavor)| flavor)
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
            flavor: fixture_flavor(sub),
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

fn target_unsupported(list: &Path) -> std::collections::BTreeSet<String> {
    std::fs::read_to_string(list)
        .unwrap_or_default()
        .lines()
        .map(str::trim)
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn selected_flavor() -> Option<Flavor> {
    let flavor = std::env::var("SLATE_DIFF_FLAVOR").ok()?;
    match flavor.as_str() {
        "clang" => Some(Flavor::Clang),
        "gcc" => Some(Flavor::Gcc),
        "msvc" => Some(Flavor::Msvc),
        _ => panic!("SLATE_DIFF_FLAVOR={flavor}: expected clang, gcc or msvc"),
    }
}

fn target_fixtures(root: &Path, triple: &str) -> Vec<Fixture> {
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let flavor = selected_flavor();
    let runs = |candidate: Flavor| flavor.is_none_or(|flavor| flavor == candidate);
    let arch = support::CrossTarget::fixture_arch(triple);
    let mut fixtures = Vec::new();
    if runs(Flavor::Clang) {
        collect_fixtures(root, Path::new(""), &selected, &mut fixtures);
        collect_fixtures(root, Path::new(arch), &selected, &mut fixtures);
    }
    for (dir, _) in FLAVOR_FIXTURES
        .into_iter()
        .filter(|&(_, dir_flavor)| runs(dir_flavor))
    {
        collect_fixtures(root, Path::new(dir), &selected, &mut fixtures);
        collect_fixtures(root, &Path::new(dir).join(arch), &selected, &mut fixtures);
    }
    fixtures.sort_by(|a, b| a.name.cmp(&b.name));
    fixtures
}

fn run_cases_with_mode(
    group: &str,
    fixtures: &[Fixture],
    project: bool,
) -> Vec<(String, Result<(), String>)> {
    run_cases_for_target(group, fixtures, project, None)
}

fn run_cases_for_target(
    group: &str,
    fixtures: &[Fixture],
    project: bool,
    cross: Option<&support::CrossTarget>,
) -> Vec<(String, Result<(), String>)> {
    let (msvc, rest): (Vec<&Fixture>, Vec<&Fixture>) =
        fixtures.iter().partition(|f| f.flavor == Flavor::Msvc);
    let triple = cross.map_or(env!("SLATE_BUILD_TARGET"), |cross| cross.rust_triple);
    let windows = (!msvc.is_empty())
        .then(|| support::CrossTarget::windows_msvc(triple.split('-').next().unwrap()))
        .flatten();
    let Some(windows) = windows else {
        return run_batch(group, &fixtures.iter().collect::<Vec<_>>(), project, cross);
    };
    let mut results = run_batch(group, &rest, project, cross);
    results.extend(run_batch(
        &format!("{group}-windows"),
        &msvc,
        project,
        Some(&windows),
    ));
    results.sort_by(|a, b| a.0.cmp(&b.0));
    results
}

fn run_batch(
    group: &str,
    fixtures: &[&Fixture],
    project: bool,
    cross: Option<&support::CrossTarget>,
) -> Vec<(String, Result<(), String>)> {
    let tmp = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/difftest-generated")
        .join(group);
    std::fs::create_dir_all(&tmp).expect("create tmp dir");

    let translated = support::parallel_map(fixtures, |f| {
        let generated = tmp.join(format!("{}.generated.rs", f.name));
        let dg_options = support::fixture_dg_options(&f.path);
        let mut extra_args = match f.flavor {
            Flavor::Clang => Vec::new(),
            Flavor::Gcc => vec!["--flavor=gcc".into()],
            Flavor::Msvc => vec!["--flavor=msvc".into()],
        };
        extra_args.extend(dg_options.iter().cloned());
        extra_args.extend(support::fixture_dg_additional_options(&f.path));
        if let Some(cross) = cross {
            extra_args.push(format!("-target={}", cross.rust_triple));
        }
        let filecheck_target = (!project && !group.starts_with("unsupported"))
            .then(|| support::translation_target(&extra_args));
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
            let config = support::RunConfig {
                c_args: dg_options,
                msvc: f.flavor == Flavor::Msvc,
                env: [("WINEDEBUG".to_string(), "-all".to_string())]
                    .into_iter()
                    .filter(|_| f.flavor == Flavor::Msvc)
                    .collect(),
                filecheck_target,
                cc: (f.flavor == Flavor::Gcc).then(|| "gcc".to_string()),
                ..support::RunConfig::default()
            };
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
    results.extend(support::compare_batch_with_jobs_for_target(
        &cases,
        &tmp,
        support::test_jobs(),
        cross,
    ));
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
    let directory = sources[0].parent().unwrap();
    let checked_in = directory.join("compile_commands.json");
    let database = if checked_in.is_file() {
        checked_in
    } else {
        let database = crate_dir.join("compile_commands.json");
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
        database
    };
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .env("SLATE_SYSROOTS", slate_parser::sysroot::root())
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

#[test]
fn generated_differential() {
    let mut failures = Vec::new();
    let mut total = 0;
    for triple in support::test_targets() {
        let fixtures = target_fixtures(&fixtures_dir(), &triple);
        assert!(
            !fixtures.is_empty() || std::env::var("SLATE_DIFF_FIXTURE").is_ok(),
            "no fixtures found in {:?} for {triple}",
            fixtures_dir()
        );
        total += fixtures.len();
        let list = unsupported_dir().join(format!("{triple}.txt"));
        let mut expected_failures = target_unsupported(&list);
        let cross = support::CrossTarget::for_triple(&triple);
        for (name, result) in run_cases_for_target(&triple, &fixtures, false, cross.as_ref()) {
            let expected = expected_failures.remove(&name);
            match result {
                Err(e) if expected && !e.starts_with("FileCheck failed") => {
                    eprintln!("xfail {triple} {name}");
                }
                Ok(()) | Err(_) if expected => {
                    eprintln!("FAIL  {triple} {name}");
                    failures.push((
                        format!("{triple}/{name}"),
                        format!("now passes on {triple}; remove it from {}", list.display()),
                    ));
                }
                Ok(()) => eprintln!("ok    {triple} {name}"),
                Err(e) => {
                    eprintln!("FAIL  {triple} {name}");
                    failures.push((format!("{triple}/{name}"), e));
                }
            }
        }
        if std::env::var("SLATE_DIFF_FIXTURE").is_err()
            && selected_flavor().is_none_or(|flavor| flavor == Flavor::Clang)
        {
            for name in expected_failures {
                failures.push((
                    format!("{triple}/{name}"),
                    format!(
                        "listed in {} but no such fixture runs on {triple}",
                        list.display()
                    ),
                ));
            }
        }
    }

    if !failures.is_empty() {
        panic!(
            "{} of {} generated fixtures failed:\n\n{}",
            failures.len(),
            total,
            support::failure_report(&failures)
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
        let runtime_ok = crate_dir.join("runtime-ok");
        let _ = std::fs::remove_file(&runtime_ok);
        let result = (|| {
            let options = support::fixture_dg_options(&sources[0]);
            let target = support::translation_target(&options);
            let mut c_args = options.clone();
            c_args.push("-O2".to_owned());
            translate_fixture_project(sources, &crate_dir, options, &[])?;
            let rs_bin = support::build_project_release(&crate_dir)?;
            let c_bin = work.join(format!("{name}_c"));
            support::compile_c_multi_with_std_include_and_args(
                sources,
                &c_bin,
                "c23",
                sources[0].parent(),
                &c_args,
            )?;
            let config = support::RunConfig::default();
            let c = support::run_with_config(&c_bin, &config, &work)?;
            let r = support::run_with_config(&rs_bin, &config, &work)?;
            support::compare_runs(&c, &r, false)?;
            std::fs::write(&runtime_ok, b"")
                .map_err(|e| format!("write {}: {e}", runtime_ok.display()))?;
            for source in sources {
                let fixture = std::fs::read_to_string(source)
                    .map_err(|e| format!("read {}: {e}", source.display()))?;
                if !fixture.contains("SLATE-FILECHECK-BEGIN") {
                    continue;
                }
                let generated = release_unit_output(&crate_dir, source);
                let rust = std::fs::read_to_string(&generated)
                    .map_err(|e| format!("read {}: {e}", generated.display()))?;
                let stem = source.file_stem().unwrap().to_string_lossy();
                support::filecheck::check_generated_rust(
                    &fixture,
                    &rust,
                    &target,
                    &crate_dir.join("filecheck").join(stem.as_ref()),
                )
                .map_err(|e| format!("{}: {e}", source.display()))?;
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

fn release_unit_output(crate_dir: &Path, source: &Path) -> PathBuf {
    let stem = source.file_stem().unwrap().to_string_lossy();
    let unit = if stem == "main" {
        "__slate_unit_main".to_string()
    } else {
        stem.into_owned()
    };
    let path = crate_dir.join("src").join(format!("{unit}.rs"));
    if path.is_file() {
        path
    } else {
        crate_dir.join("src/main.rs")
    }
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

type FixtureResults = Vec<(Fixture, Result<(), String>)>;

fn unsupported_results() -> Vec<(String, FixtureResults)> {
    support::test_targets()
        .into_iter()
        .map(|triple| {
            let fixtures = target_fixtures(&unsupported_dir(), &triple);
            let cross = support::CrossTarget::for_triple(&triple);
            let results = run_cases_for_target(
                &format!("unsupported/{triple}"),
                &fixtures,
                false,
                cross.as_ref(),
            );
            let paired = fixtures
                .into_iter()
                .zip(results)
                .map(|(fixture, (name, result))| {
                    assert_eq!(fixture.name, name);
                    (fixture, result)
                })
                .collect();
            (triple, paired)
        })
        .collect()
}

#[test]
fn fixtures_unsupported_tests_still_fail() {
    let mut passed = std::collections::BTreeMap::<PathBuf, Vec<String>>::new();
    let mut failed = std::collections::BTreeMap::<PathBuf, Vec<String>>::new();
    for (triple, results) in unsupported_results() {
        for (fixture, result) in results {
            let bucket = if result.is_ok() {
                &mut passed
            } else {
                &mut failed
            };
            bucket
                .entry(fixture.relative)
                .or_default()
                .push(triple.clone());
        }
    }
    let promotions: Vec<String> = passed
        .iter()
        .map(|(relative, triples)| {
            let name = relative.file_stem().unwrap().to_string_lossy();
            let mut steps = vec![
                format!("{} passes on {}:", relative.display(), triples.join(", ")),
                format!(
                    "  git mv crates/slate/tests/fixtures.unsupported/{0} crates/slate/tests/fixtures/{0}",
                    relative.display()
                ),
                format!(
                    "  python3 crates/slate/tools/update_filecheck.py --target {} crates/slate/tests/fixtures/{}",
                    triples.join(" --target "),
                    relative.display()
                ),
            ];
            for triple in failed.get(relative).into_iter().flatten() {
                steps.push(format!(
                    "  echo {name} >> crates/slate/tests/fixtures.unsupported/{triple}.txt"
                ));
            }
            steps.join("\n")
        })
        .collect();
    assert!(
        promotions.is_empty(),
        "fixture(s) now pass end-to-end -- promote them:\n{}",
        promotions.join("\n")
    );
}

#[test]
#[ignore]
fn fixtures_unsupported_triage_report() {
    for (triple, results) in unsupported_results() {
        let results: Vec<_> = results
            .into_iter()
            .map(|(fixture, result)| (fixture.relative.display().to_string(), result))
            .collect();
        support::print_triage(&format!("{triple} "), &results);
    }
}

#[test]
fn translation_uses_builtin_frontend() {
    let output = std::process::Command::new("rustc")
        .args(["--print", "sysroot"])
        .output()
        .expect("locate Rust toolchain");
    assert!(output.status.success());
    let sysroot = String::from_utf8(output.stdout).unwrap();
    let rustfmt = Path::new(sysroot.trim()).join("bin/rustfmt");
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
            .env("SLATE_SYSROOTS", slate_parser::sysroot::root())
            .env("SLATE_RUSTFMT", &rustfmt)
            .env("SLATE_TARGET", "invalid-default-target")
            .env_remove("SLATE_CLANG_ARGS")
            .output()
            .expect("run Slate without external C tools");
        assert!(
            output.status.success(),
            "{} failed without external C tools:\n{}",
            args[0],
            String::from_utf8_lossy(&output.stderr)
        );
    }
}

fn matrix_fixture(name: &str) -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/fixtures.matrix")
        .join(name)
}

fn translate_matrix(
    work: &str,
    matrix: &Path,
    project: &Path,
    selection: &[&str],
) -> (PathBuf, Result<(), String>) {
    let crate_dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/difftest-generated/matrix")
        .join(work);
    let _ = std::fs::remove_dir_all(&crate_dir);
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-project")
        .arg("--matrix")
        .arg(matrix)
        .args(selection)
        .arg(project)
        .arg(&crate_dir)
        .env("SLATE_SYSROOTS", slate_parser::sysroot::root())
        .env_remove("SLATE_TARGET")
        .env_remove("SLATE_CLANG_ARGS")
        .output()
        .expect("run slate translate-project");
    let result = if output.status.success() {
        Ok(())
    } else {
        Err(String::from_utf8_lossy(&output.stderr).into_owned())
    };
    (crate_dir, result)
}

fn run_cargo_binary(crate_dir: &Path) -> String {
    let output = std::process::Command::new(std::env::var("SLATE_CARGO").unwrap_or("cargo".into()))
        .args(["run", "--quiet", "--release"])
        .current_dir(crate_dir)
        .output()
        .expect("run cargo");
    assert!(
        output.status.success(),
        "generated crate failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    String::from_utf8_lossy(&output.stdout).into_owned()
}

fn run_clang_binary(project: &Path, work: &str, args: &[&str]) -> String {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/difftest-generated/matrix")
        .join(format!("{work}-c"));
    std::fs::create_dir_all(&dir).expect("create C output directory");
    let binary = dir.join("program");
    let status = std::process::Command::new("clang")
        .current_dir(project)
        .args(args)
        .args(["main.c", "value.c", "-o"])
        .arg(&binary)
        .status()
        .expect("run clang");
    assert!(status.success(), "clang failed for {args:?}");
    let output = std::process::Command::new(&binary)
        .output()
        .expect("run C program");
    String::from_utf8_lossy(&output.stdout).into_owned()
}

#[test]
fn matrix_selection_matches_c_with_same_configuration() {
    let project = matrix_fixture("generated");
    let matrix = project.join("matrix.json");
    let base = ["-DPROJECT_BASE=100", "-Iinclude", "-Igen-linux"];
    for (work, selection, extra) in [
        ("plain", vec![], vec![]),
        ("fast", vec!["--features", "fast"], vec!["-DFAST"]),
        (
            "level",
            vec!["--features", "level-two,small"],
            vec!["-DLEVEL=2", "-DHAS_LEVEL", "-DSMALL"],
        ),
    ] {
        let mut selected = vec!["--matrix-target", "linux-x86_64"];
        selected.extend(&selection);
        let (crate_dir, result) = translate_matrix(work, &matrix, &project, &selected);
        result.unwrap_or_else(|error| panic!("{work}: {error}"));
        let c_args: Vec<&str> = base.iter().chain(&extra).copied().collect();
        assert_eq!(
            run_cargo_binary(&crate_dir),
            run_clang_binary(&project, work, &c_args),
            "{work}"
        );
    }
}

#[test]
fn matrix_targets_use_their_own_generated_headers_and_args() {
    let project = matrix_fixture("generated");
    let matrix = project.join("matrix.json");
    for (target, constant) in [
        ("linux-x86_64", "100 + 1 + 0"),
        ("linux-x86_64-gcc", "100 + 1 + 5"),
        ("windows-x86_64", "100 + 2 + 0"),
    ] {
        let (crate_dir, result) =
            translate_matrix(target, &matrix, &project, &["--matrix-target", target]);
        result.unwrap_or_else(|error| panic!("{target}: {error}"));
        let value = std::fs::read_to_string(crate_dir.join("src/value.rs")).expect("read value.rs");
        assert!(value.contains(constant), "{target}: {value}");
    }
}

#[test]
fn matrix_rejects_invalid_input() {
    let project = matrix_fixture("generated");
    let matrix = project.join("matrix.json");
    let write = |name: &str, contents: serde_json::Value| {
        let dir = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated/matrix");
        std::fs::create_dir_all(&dir).expect("create matrix directory");
        let path = dir.join(format!("{name}.json"));
        std::fs::write(&path, contents.to_string()).expect("write matrix");
        path
    };
    let target = serde_json::json!({
        "name": "a", "triple": "x86_64-unknown-linux-gnu", "flavor": "clang"
    });
    let selected = ["--matrix-target", "a"];
    for (name, contents, selection, expected) in [
        (
            "no_target_selected",
            None,
            vec![],
            "--matrix requires --matrix-target",
        ),
        (
            "unknown_target",
            None,
            vec!["--matrix-target", "nope"],
            "unknown matrix target nope",
        ),
        (
            "unknown_feature",
            None,
            vec!["--matrix-target", "linux-x86_64", "--features", "nope"],
            "unknown matrix feature nope",
        ),
        (
            "invalid_combination",
            None,
            vec![
                "--matrix-target",
                "linux-x86_64",
                "--features",
                "small,fast",
            ],
            "matrix features fast, small cannot be combined",
        ),
        (
            "unsupported_triple",
            Some(serde_json::json!({"sources": ["main.c"], "targets": [
                {"name": "a", "triple": "mips-unknown-linux-gnu", "flavor": "clang"}
            ]})),
            selected.to_vec(),
            "matrix target a: unsupported target triple mips-unknown-linux-gnu",
        ),
        (
            "flavor_mismatch",
            Some(serde_json::json!({"sources": ["main.c"], "targets": [
                {"name": "a", "triple": "x86_64-unknown-linux-musl", "flavor": "gcc"}
            ]})),
            selected.to_vec(),
            "matrix target a: x86_64-unknown-linux-musl does not support flavor gcc",
        ),
        (
            "duplicate_target",
            Some(serde_json::json!({"sources": ["main.c"], "targets": [target, target]})),
            selected.to_vec(),
            "duplicate matrix target a",
        ),
        (
            "bad_feature_name",
            Some(
                serde_json::json!({"sources": ["main.c"], "targets": [target],
                "features": [{"name": "not valid", "defines": ["X"]}]}),
            ),
            selected.to_vec(),
            "matrix feature \"not valid\" is not a valid Cargo feature name",
        ),
        (
            "feature_without_defines",
            Some(
                serde_json::json!({"sources": ["main.c"], "targets": [target],
                "features": [{"name": "x", "defines": []}]}),
            ),
            selected.to_vec(),
            "matrix feature x has no defines",
        ),
        (
            "invalid_names_unknown_feature",
            Some(
                serde_json::json!({"sources": ["main.c"], "targets": [target],
                "features": [{"name": "x", "defines": ["X"]}],
                "invalid_features": [["x", "y"]]}),
            ),
            selected.to_vec(),
            "invalid_features names unknown matrix feature y",
        ),
        (
            "empty_invalid_group",
            Some(
                serde_json::json!({"sources": ["main.c"], "targets": [target],
                "invalid_features": [[]]}),
            ),
            selected.to_vec(),
            "invalid_features entries need at least two features",
        ),
        (
            "unknown_field",
            Some(serde_json::json!({"sources": ["main.c"], "targets": [target], "typo": 1})),
            selected.to_vec(),
            "unknown field `typo`",
        ),
        (
            "no_sources",
            Some(serde_json::json!({"sources": [], "targets": [target]})),
            selected.to_vec(),
            "matrix has no sources",
        ),
    ] {
        let path = contents.map_or(matrix.clone(), |contents| write(name, contents));
        let (_, result) = translate_matrix(name, &path, &project, &selection);
        let error = result.expect_err(name);
        assert!(error.contains(expected), "{name}: {error}");
    }

    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate-project", "--matrix"])
        .arg(&matrix)
        .args([
            "--matrix-target",
            "linux-x86_64",
            "--compile-commands",
            "x.json",
        ])
        .arg(&project)
        .arg("unused")
        .output()
        .expect("run slate translate-project");
    let stderr = String::from_utf8_lossy(&output.stderr);
    assert!(
        stderr.contains("--matrix cannot be combined with --compile-commands"),
        "{stderr}"
    );
}

#[test]
fn translation_uses_native_headers_without_sysroot() {
    let work =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated/native-headers");
    let empty = work.join("empty-data");
    std::fs::create_dir_all(&empty).expect("create native header test directory");
    let source = work.join("hello.c");
    std::fs::write(
        &source,
        "#include <stdio.h>\n#include <stdlib.h>\nint main(void) { printf(\"%d\\n\", abs(-3)); return 0; }\n",
    )
    .expect("write native header source");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate")
        .arg(&source)
        .env_remove("SLATE_SYSROOTS")
        .env_remove("SLATE_COMPILER_HEADERS")
        .env_remove("SLATE_TARGET")
        .env_remove("SLATE_CLANG_ARGS")
        .env("XDG_DATA_HOME", &empty)
        .env("HOME", &empty)
        .env("LOCALAPPDATA", &empty)
        .env("APPDATA", &empty)
        .output()
        .expect("run Slate with native headers");
    assert!(
        output.status.success(),
        "translate failed without an installed sysroot:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    assert!(String::from_utf8_lossy(&output.stdout).contains("fn printf"));
}

#[cfg(unix)]
#[test]
fn translate_project_uses_compile_database_compiler_headers() {
    use std::os::unix::fs::PermissionsExt;
    let work =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-generated/compiler-headers");
    let headers = work.join("wrapper-include");
    let source_dir = work.join("src");
    std::fs::create_dir_all(&headers).expect("create wrapper include directory");
    std::fs::create_dir_all(&source_dir).expect("create source directory");
    std::fs::write(
        headers.join("slate_wrapper_marker.h"),
        "#define SLATE_WRAPPER_MARKER 42\n",
    )
    .expect("write marker header");
    let compiler = work.join("wrapper-cc");
    std::fs::write(
        &compiler,
        format!(
            "#!/bin/sh\nexec clang -isystem '{}' \"$@\"\n",
            headers.display()
        ),
    )
    .expect("write wrapper compiler");
    std::fs::set_permissions(&compiler, std::fs::Permissions::from_mode(0o755))
        .expect("make wrapper compiler executable");
    let source = source_dir.join("main.c");
    std::fs::write(
        &source,
        "#include <stdio.h>\n#include <slate_wrapper_marker.h>\nint main(void) { printf(\"%d\\n\", SLATE_WRAPPER_MARKER); return 0; }\n",
    )
    .expect("write source");
    let database = work.join("compile_commands.json");
    std::fs::write(
        &database,
        serde_json::to_vec(&serde_json::json!([{
            "directory": source_dir,
            "file": source,
            "arguments": [compiler, "-c", source],
        }]))
        .expect("encode compile commands"),
    )
    .expect("write compile commands");
    let output = std::process::Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-project")
        .arg("--compile-commands")
        .arg(&database)
        .arg(&source_dir)
        .arg(work.join("out"))
        .env_remove("SLATE_SYSROOTS")
        .env_remove("SLATE_TARGET")
        .env_remove("SLATE_CLANG_ARGS")
        .output()
        .expect("run Slate with a compile database compiler");
    assert!(
        output.status.success(),
        "translate-project did not use the compile database compiler's headers:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
}
