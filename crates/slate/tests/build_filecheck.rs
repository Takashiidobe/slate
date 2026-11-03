mod support;

use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::Command;

fn run(command: &mut Command) -> Result<(), String> {
    let output = command
        .output()
        .map_err(|error| format!("{command:?}: {error}"))?;
    if output.status.success() {
        Ok(())
    } else {
        Err(format!(
            "{command:?}:\n{}{}",
            String::from_utf8_lossy(&output.stdout),
            String::from_utf8_lossy(&output.stderr)
        ))
    }
}

fn build_fixture(fixture: &Path, target: &str, text: &str) -> Result<(), String> {
    let root = Path::new(env!("CARGO_MANIFEST_DIR"));
    let stem = fixture.file_stem().unwrap().to_str().unwrap();
    let platform = fixture
        .parent()
        .unwrap()
        .file_name()
        .unwrap()
        .to_str()
        .unwrap();
    let name = format!("{platform}_{stem}");
    let work = root.join("target/difftest-generated/build").join(target);
    std::fs::create_dir_all(&work).map_err(|error| error.to_string())?;
    let generated = work.join(format!("{name}.generated.rs"));
    let marker = work.join(format!("{name}.build-ok"));
    let _ = std::fs::remove_file(&marker);
    let args = support::fixture_dg_options(fixture);
    let sysroot = slate_parser::sysroot::root().join(target);
    let mut clang = Command::new("clang");
    clang.args(["-c", "-std=c23", "-target", target]);
    if target.contains("-apple-") {
        clang.arg("-isysroot").arg(sysroot.join("SDK"));
    } else {
        clang.arg(format!("--sysroot={}", sysroot.display()));
    }
    let info = slate_parser::target_info::TargetInfo::for_triple(target)
        .map_err(|error| error.to_string())?;
    for include in slate_parser::sysroot::include_paths(
        &info,
        slate_parser::compiler_args::CompilerFlavor::Clang,
    ) {
        clang.arg("-isystem").arg(include);
    }
    run(clang
        .args(&args)
        .arg(fixture)
        .arg("-o")
        .arg(work.join(format!("{name}.o"))))?;
    let matrix = work.join(format!("{name}.json"));
    std::fs::write(
        &matrix,
        serde_json::json!({
            "targets": [{"name": "build", "triple": target, "flavor": "clang", "args": args}],
            "sources": [fixture.file_name().unwrap().to_str().unwrap()]
        })
        .to_string(),
    )
    .map_err(|error| error.to_string())?;
    let project = work.join(&name);
    run(Command::new(env!("CARGO_BIN_EXE_slate"))
        .args(["translate-project", "--matrix"])
        .arg(&matrix)
        .args(["--matrix-target", "build"])
        .arg(fixture.parent().unwrap())
        .arg(&project)
        .env("SLATE_SYSROOTS", slate_parser::sysroot::root())
        .env_remove("SLATE_TARGET")
        .env_remove("SLATE_CLANG_ARGS"))?;
    let unit = project.join("src").join(format!("{stem}.rs"));
    let unit = if unit.is_file() {
        unit
    } else {
        project.join("src/lib.rs")
    };
    let source =
        std::fs::read_to_string(&unit).map_err(|error| format!("{}: {error}", unit.display()))?;
    std::fs::write(&generated, &source).map_err(|error| error.to_string())?;
    let assertions = fixture.with_extension("rs");
    if assertions.is_file() {
        let assertions = std::fs::read_to_string(assertions).map_err(|error| error.to_string())?;
        let mut library = std::fs::OpenOptions::new()
            .append(true)
            .open(project.join("src/lib.rs"))
            .map_err(|error| error.to_string())?;
        writeln!(library, "\n{assertions}").map_err(|error| error.to_string())?;
    }
    run(
        Command::new(std::env::var_os("SLATE_CARGO").unwrap_or_else(|| "cargo".into()))
            .args([
                "build",
                "--lib",
                "--quiet",
                "--target",
                target,
                "--manifest-path",
            ])
            .arg(project.join("Cargo.toml"))
            .env("CARGO_TARGET_DIR", root.join("target/build-fixtures-cache"))
            .env("RUSTFLAGS", "-Awarnings")
            .env_remove("CARGO_ENCODED_RUSTFLAGS"),
    )?;
    std::fs::write(marker, "").map_err(|error| error.to_string())?;
    support::filecheck::check_generated_rust(
        text,
        &source,
        target,
        &work.join("filecheck").join(name),
    )
}

#[test]
fn build_filecheck() {
    let root = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.build");
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let targets = std::env::var("SLATE_TEST_TARGETS").ok();
    let mut fixtures = Vec::<PathBuf>::new();
    for platform in std::fs::read_dir(&root).expect("build fixture platforms") {
        for fixture in
            std::fs::read_dir(platform.expect("platform").path()).expect("build fixtures")
        {
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
    let mut failures = Vec::new();
    let mut count = 0;
    for fixture in fixtures {
        let text = std::fs::read_to_string(&fixture).expect("read build fixture");
        let declared = text
            .lines()
            .find_map(|line| line.strip_prefix("// SLATE-BUILD-TARGETS:"))
            .expect("SLATE-BUILD-TARGETS metadata");
        for target in declared.split_whitespace().filter(|target| {
            targets
                .as_ref()
                .is_none_or(|targets| targets.split(',').any(|selected| selected == *target))
        }) {
            count += 1;
            if let Err(error) = build_fixture(&fixture, target, &text) {
                failures.push(format!("{} ({target}): {error}", fixture.display()));
            }
        }
    }
    assert!(count != 0, "no build fixtures selected");
    assert!(failures.is_empty(), "{}", failures.join("\n\n"));
}
