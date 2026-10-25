mod support;

use std::path::{Path, PathBuf};
use std::process::Command;

const WINDOWS_TRIPLE: &str = "x86_64-pc-windows-msvc";

fn manifest_dir() -> &'static Path {
    Path::new(env!("CARGO_MANIFEST_DIR"))
}

fn fixtures_root() -> PathBuf {
    manifest_dir().join("tests/fixtures.dylink")
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

fn fixtures() -> Vec<(String, PathBuf)> {
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut fixtures: Vec<(String, PathBuf)> = std::fs::read_dir(fixtures_root())
        .expect("read dylink fixtures")
        .filter_map(|entry| {
            let path = entry.ok()?.path();
            let name = path.file_name()?.to_string_lossy().into_owned();
            (path.join("lib").is_dir()
                && path.join("host").is_dir()
                && selected.as_ref().is_none_or(|selected| *selected == name))
            .then_some((name, path))
        })
        .collect();
    fixtures.sort();
    fixtures
}

fn run_checked(command: &mut Command, what: &str) -> Result<(), String> {
    let output = command
        .output()
        .map_err(|error| format!("spawn {what}: {error}"))?;
    if output.status.success() {
        Ok(())
    } else {
        Err(format!(
            "{what} failed ({}):\n{}{}",
            output.status,
            String::from_utf8_lossy(&output.stdout),
            String::from_utf8_lossy(&output.stderr)
        ))
    }
}

struct Platform {
    name: &'static str,
    dep_file: &'static str,
    dep_path: &'static str,
    translate_args: Vec<String>,
    cross: Option<support::CrossTarget>,
    msvc_root: Option<PathBuf>,
}

impl Platform {
    fn linux() -> Self {
        Self {
            name: "linux",
            dep_file: "libdep.so",
            dep_path: "./libdep.so",
            translate_args: vec!["-std=c23".into()],
            cross: None,
            msvc_root: None,
        }
    }

    fn windows(msvc_root: PathBuf) -> Self {
        Self {
            name: "windows",
            dep_file: "dep.dll",
            dep_path: ".\\\\dep.dll",
            translate_args: vec![format!("--target={WINDOWS_TRIPLE}")],
            cross: Some(support::CrossTarget::windows_msvc_with_root(
                &msvc_root, "x86_64",
            )),
            msvc_root: Some(msvc_root),
        }
    }

    fn is_windows(&self) -> bool {
        self.cross.is_some()
    }

    fn run_config(&self) -> support::RunConfig {
        let mut config = support::RunConfig {
            timeout_seconds: Some(120),
            ..Default::default()
        };
        if self.is_windows() {
            config.env.insert("WINEDEBUG".into(), "-all".into());
        }
        config
    }

    fn cl(&self) -> Command {
        let mut command = Command::new(manifest_dir().join("../slate-parser/tools/cl.exe"));
        command
            .env(
                "MSVC_WINE_ROOT",
                self.msvc_root.as_ref().expect("msvc root"),
            )
            .env("WINEDEBUG", "-all")
            .arg("/nologo");
        command
    }

    fn build_dependency(&self, fixture: &Path, work: &Path) -> Result<(), String> {
        let sources = c_sources_in(&fixture.join("lib"));
        if self.is_windows() {
            std::fs::create_dir_all(work.join("obj_lib")).map_err(|e| e.to_string())?;
            let mut command = self.cl();
            command
                .current_dir(work)
                .arg("/LD")
                .arg(format!("/I{}", fixtures_root().display()))
                .arg(format!("/Fe:{}", self.dep_file))
                .arg("/Foobj_lib\\")
                .args(&sources);
            run_checked(&mut command, "cl.exe /LD")
        } else {
            run_checked(
                Command::new(support::cc())
                    .args(["-O0", "-std=c23", "-shared", "-fPIC", "-fvisibility=hidden"])
                    .arg(format!("-I{}", fixtures_root().display()))
                    .arg("-o")
                    .arg(work.join(self.dep_file))
                    .args(&sources),
                "clang -shared",
            )
        }
    }

    fn build_native_host(&self, fixture: &Path, work: &Path) -> Result<PathBuf, String> {
        let sources = c_sources_in(&fixture.join("host"));
        if self.is_windows() {
            std::fs::create_dir_all(work.join("obj_host")).map_err(|e| e.to_string())?;
            let mut command = self.cl();
            command
                .current_dir(work)
                .arg(format!("/I{}", fixtures_root().display()))
                .arg(format!("/I{}", work.display()))
                .arg("/Fe:host_c.exe")
                .arg("/Foobj_host\\")
                .args(&sources);
            run_checked(&mut command, "cl.exe host")?;
            Ok(work.join("host_c.exe"))
        } else {
            let out = work.join("host_c");
            support::compile_c_multi_with_std_include_and_args(
                &sources,
                &out,
                "c23",
                Some(&fixtures_root()),
                &[format!("-I{}", work.display()), "-ldl".into()],
            )?;
            Ok(out)
        }
    }

    fn build_translated_host(&self, fixture: &Path, work: &Path) -> Result<PathBuf, String> {
        let sources = c_sources_in(&fixture.join("host"));
        let crate_dir = work.join("rust");
        std::fs::create_dir_all(&crate_dir).map_err(|e| e.to_string())?;
        let database = crate_dir.join("compile_commands.json");
        let host_dir = fixture.join("host");
        let entries: Vec<_> = sources
            .iter()
            .map(|source| {
                let mut arguments = vec!["clang".to_string()];
                arguments.extend(self.translate_args.iter().cloned());
                arguments.push(format!("-I{}", fixtures_root().display()));
                arguments.push(format!("-I{}", work.display()));
                arguments.push(source.display().to_string());
                serde_json::json!({
                    "directory": host_dir, "file": source, "arguments": arguments,
                })
            })
            .collect();
        std::fs::write(&database, serde_json::to_vec(&entries).unwrap())
            .map_err(|e| e.to_string())?;
        let output = Command::new(env!("CARGO_BIN_EXE_slate"))
            .env("SLATE_SYSROOTS", slate_parser::sysroot::root())
            .arg("translate-project")
            .arg("--compile-commands")
            .arg(&database)
            .arg(&host_dir)
            .arg(&crate_dir)
            .output()
            .map_err(|e| format!("spawn slate translate-project: {e}"))?;
        if !output.status.success() {
            let stderr = String::from_utf8_lossy(&output.stderr);
            assert!(
                !stderr.contains("invalid slate-parser IR"),
                "{}: {stderr}",
                fixture.display()
            );
            return Err(format!("slate translate-project failed:\n{stderr}"));
        }
        support::build_project_release_for_target(&crate_dir, self.cross.as_ref())
    }

    fn run(&self, binary: &Path, work: &Path) -> Result<support::Run, String> {
        support::run_with_config_for_target(binary, &self.run_config(), work, self.cross.as_ref())
    }

    fn check(&self, fixture: &Path, name: &str) -> Result<(), String> {
        let work = manifest_dir()
            .join("target/difftest-generated/dylink")
            .join(self.name)
            .join(name);
        std::fs::create_dir_all(&work).map_err(|e| e.to_string())?;
        support::write_if_changed(
            work.join("dylink_path.h"),
            format!("#define DEP_PATH \"{}\"\n", self.dep_path).as_bytes(),
        )
        .map_err(|e| e.to_string())?;
        self.build_dependency(fixture, &work)?;
        let native = self.build_native_host(fixture, &work)?;
        let translated = self.build_translated_host(fixture, &work)?;
        let c = self.run(&native, &work)?;
        if c.exit() != Some(0) || c.stdout().is_empty() {
            return Err(format!(
                "native host is not a usable oracle (exit {:?}):\n{}",
                c.exit(),
                String::from_utf8_lossy(c.stdout())
            ));
        }
        let rust = self.run(&translated, &work)?;
        support::compare_runs(&c, &rust, false)
    }
}

fn windows_platform() -> Option<Platform> {
    let required = std::env::var_os("SLATE_DYLINK_REQUIRE_WINDOWS").is_some();
    let root = std::env::var_os("MSVC_WINE_ROOT")
        .map(PathBuf::from)
        .or_else(|| {
            std::env::var_os("HOME")
                .map(|home| PathBuf::from(home).join(".local/share/msvc-wine/opt/msvc"))
        })
        .filter(|root| root.join("bin/x64/cl.exe").is_file());
    let missing = [
        (
            "wine",
            Command::new("wine").arg("--version").output().is_ok(),
        ),
        (
            "lld-link",
            Command::new("lld-link").arg("--version").output().is_ok(),
        ),
        ("msvc-wine (MSVC_WINE_ROOT)", root.is_some()),
        (
            "rust target x86_64-pc-windows-msvc",
            support::rust_target_installed(WINDOWS_TRIPLE),
        ),
    ]
    .into_iter()
    .filter_map(|(name, present)| (!present).then_some(name))
    .collect::<Vec<_>>();
    if missing.is_empty() {
        return Some(Platform::windows(root.unwrap()));
    }
    let message = format!(
        "dylink: skipping windows leg, missing {}",
        missing.join(", ")
    );
    assert!(!required, "{message}");
    eprintln!("{message}");
    None
}

fn run_platform(platform: &Platform) {
    let fixtures = fixtures();
    assert!(
        !fixtures.is_empty() || std::env::var("SLATE_DIFF_FIXTURE").is_ok(),
        "no dylink fixtures found in {}",
        fixtures_root().display()
    );
    let failures: Vec<_> = support::parallel_map(&fixtures, |(name, path)| {
        platform
            .check(path, name)
            .err()
            .map(|error| format!("[{}/{name}] {error}", platform.name))
    })
    .into_iter()
    .flatten()
    .collect();
    assert!(failures.is_empty(), "{}", failures.join("\n\n"));
}

#[test]
fn dylink_linux() {
    run_platform(&Platform::linux());
}

#[test]
fn dylink_windows() {
    if let Some(platform) = windows_platform() {
        run_platform(&platform);
    }
}
