mod support;

use std::path::{Path, PathBuf};
use std::process::Command;

const WINDOWS_TRIPLE: &str = "x86_64-pc-windows-msvc";
const AARCH64_TRIPLE: &str = "aarch64-unknown-linux-gnu";
const I686_TRIPLE: &str = "i686-unknown-linux-gnu";

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

fn headers_in(dir: &Path) -> Vec<PathBuf> {
    std::fs::read_dir(dir)
        .into_iter()
        .flatten()
        .filter_map(|entry| entry.ok().map(|entry| entry.path()))
        .filter(|path| path.extension().is_some_and(|ext| ext == "h"))
        .collect()
}

#[derive(Default)]
struct FixtureConfig {
    platforms: Option<Vec<String>>,
    args: Vec<String>,
    compiler: Option<String>,
    translate_args: Vec<String>,
}

impl FixtureConfig {
    fn path(fixture: &Path) -> PathBuf {
        fixture.join("dylink.json")
    }

    fn load(fixture: &Path) -> Self {
        let path = Self::path(fixture);
        let Ok(text) = std::fs::read_to_string(&path) else {
            return Self::default();
        };
        let value: serde_json::Value = serde_json::from_str(&text)
            .unwrap_or_else(|error| panic!("{}: {error}", path.display()));
        let strings = |key: &str| {
            value.get(key).map(|list| {
                list.as_array()
                    .unwrap_or_else(|| panic!("{}: `{key}` must be a list", path.display()))
                    .iter()
                    .map(|item| {
                        item.as_str()
                            .unwrap_or_else(|| panic!("{}: `{key}` holds strings", path.display()))
                            .to_owned()
                    })
                    .collect::<Vec<_>>()
            })
        };
        Self {
            platforms: strings("platforms"),
            args: strings("args").unwrap_or_default(),
            compiler: value.get("compiler").map(|compiler| {
                compiler
                    .as_str()
                    .unwrap_or_else(|| panic!("{}: `compiler` must be a string", path.display()))
                    .to_owned()
            }),
            translate_args: strings("translate_args").unwrap_or_default(),
        }
    }

    fn runs_on(&self, platform: &Platform) -> bool {
        match &self.platforms {
            Some(platforms) => platforms.iter().any(|name| name == platform.name),
            None => !platform.opt_in,
        }
    }
}

fn fixtures(platform: &Platform) -> Vec<(String, PathBuf, FixtureConfig)> {
    let selected = std::env::var("SLATE_DIFF_FIXTURE").ok();
    let mut fixtures: Vec<_> = std::fs::read_dir(fixtures_root())
        .expect("read dylink fixtures")
        .filter_map(|entry| {
            let path = entry.ok()?.path();
            let name = path.file_name()?.to_string_lossy().into_owned();
            if !path.join("lib").is_dir()
                || !path.join("host").is_dir()
                || selected.as_ref().is_some_and(|selected| *selected != name)
            {
                return None;
            }
            let config = FixtureConfig::load(&path);
            config.runs_on(platform).then_some((name, path, config))
        })
        .collect();
    fixtures.sort_by(|left, right| left.0.cmp(&right.0));
    fixtures
}

struct Platform {
    name: &'static str,
    triple: &'static str,
    dep_file: &'static str,
    dep_path: &'static str,
    translate_args: Vec<String>,
    cross: Option<support::CrossTarget>,
    msvc_root: Option<PathBuf>,
    opt_in: bool,
}

impl Platform {
    fn linux() -> Self {
        Self {
            name: "linux",
            triple: env!("SLATE_BUILD_TARGET"),
            dep_file: "libdep.so",
            dep_path: "./libdep.so",
            translate_args: vec!["-std=c23".into()],
            cross: None,
            msvc_root: None,
            opt_in: false,
        }
    }

    fn linux_aarch64() -> Self {
        Self {
            name: "linux-aarch64",
            triple: AARCH64_TRIPLE,
            dep_file: "libdep.so",
            dep_path: "./libdep.so",
            translate_args: vec!["-std=c23".into(), format!("--target={AARCH64_TRIPLE}")],
            cross: Some(support::CrossTarget::aarch64_linux()),
            msvc_root: None,
            opt_in: false,
        }
    }

    fn linux_i686() -> Self {
        Self {
            name: "linux-i686",
            triple: I686_TRIPLE,
            dep_file: "libdep.so",
            dep_path: "./libdep.so",
            translate_args: vec!["-std=c23".into(), format!("--target={I686_TRIPLE}")],
            cross: Some(support::CrossTarget::i686_linux()),
            msvc_root: None,
            opt_in: true,
        }
    }

    fn windows(msvc_root: PathBuf) -> Self {
        Self {
            name: "windows",
            triple: WINDOWS_TRIPLE,
            dep_file: "dep.dll",
            dep_path: ".\\\\dep.dll",
            translate_args: vec![format!("--target={WINDOWS_TRIPLE}")],
            cross: Some(support::CrossTarget::windows_msvc_with_root(
                &msvc_root, "x86_64",
            )),
            msvc_root: Some(msvc_root),
            opt_in: false,
        }
    }

    fn is_windows(&self) -> bool {
        self.msvc_root.is_some()
    }

    fn linux_cc(&self) -> (String, Vec<String>) {
        match &self.cross {
            Some(cross) => (cross.cc.clone(), cross.cc_extra_args.clone()),
            None => (support::cc(), Vec::new()),
        }
    }

    fn compile_cached(
        &self,
        fixture: &Path,
        config: &FixtureConfig,
        work: &Path,
        sources: &[PathBuf],
        out: &Path,
        compile: impl FnOnce(&Path) -> Result<std::process::Output, String>,
    ) -> Result<(), String> {
        let mut inputs = sources.to_vec();
        for dir in [
            fixtures_root(),
            fixture.to_path_buf(),
            fixture.join("lib"),
            fixture.join("host"),
        ] {
            inputs.extend(headers_in(&dir));
        }
        inputs.push(work.join("dylink_path.h"));
        inputs.extend(Some(FixtureConfig::path(fixture)).filter(|path| path.is_file()));
        let key =
            serde_json::to_string(&(1, self.triple, self.linux_cc(), sources, &config.args, out))
                .map_err(|e| format!("encode dylink C cache key: {e}"))?;
        let inputs: Vec<&Path> = inputs.iter().map(PathBuf::as_path).collect();
        let what = format!("{} build of {}", self.name, out.display());
        support::compile_c_cached(&inputs, out, &key, &what, compile)
    }

    fn cl_into(
        &self,
        work: &Path,
        built: &str,
        temporary: &Path,
        configure: impl FnOnce(&mut Command),
    ) -> Result<std::process::Output, String> {
        let mut command = self.cl();
        command.current_dir(work);
        configure(&mut command);
        let output = command.output().map_err(|e| format!("spawn cl.exe: {e}"))?;
        if output.status.success() {
            std::fs::rename(work.join(built), temporary)
                .map_err(|e| format!("rename {built}: {e}"))?;
        }
        Ok(output)
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

    fn build_dependency(
        &self,
        fixture: &Path,
        config: &FixtureConfig,
        work: &Path,
    ) -> Result<(), String> {
        let sources = c_sources_in(&fixture.join("lib"));
        let out = work.join(self.dep_file);
        self.compile_cached(fixture, config, work, &sources, &out, |temporary| {
            if self.is_windows() {
                std::fs::create_dir_all(work.join("obj_lib")).map_err(|e| e.to_string())?;
                return self.cl_into(work, self.dep_file, temporary, |command| {
                    command
                        .arg("/LD")
                        .arg(format!("/I{}", fixtures_root().display()))
                        .args(&config.args)
                        .arg(format!("/Fe:{}", self.dep_file))
                        .arg("/Foobj_lib\\")
                        .args(&sources);
                });
            }
            let (cc, cc_args) = self.linux_cc();
            Command::new(&cc)
                .args(cc_args)
                .args(["-O0", "-std=c23", "-shared", "-fPIC", "-fvisibility=hidden"])
                .args(&config.args)
                .arg(format!("-I{}", fixtures_root().display()))
                .arg("-o")
                .arg(temporary)
                .args(&sources)
                .output()
                .map_err(|e| format!("spawn {cc}: {e}"))
        })
    }

    fn build_native_host(
        &self,
        fixture: &Path,
        config: &FixtureConfig,
        work: &Path,
    ) -> Result<PathBuf, String> {
        let sources = c_sources_in(&fixture.join("host"));
        let out = work.join(if self.is_windows() {
            "host_c.exe"
        } else {
            "host_c"
        });
        self.compile_cached(fixture, config, work, &sources, &out, |temporary| {
            if self.is_windows() {
                std::fs::create_dir_all(work.join("obj_host")).map_err(|e| e.to_string())?;
                return self.cl_into(work, "host_c.exe", temporary, |command| {
                    command
                        .arg(format!("/I{}", fixtures_root().display()))
                        .arg(format!("/I{}", work.display()))
                        .args(&config.args)
                        .arg("/Fe:host_c.exe")
                        .arg("/Foobj_host\\")
                        .args(&sources);
                });
            }
            let (cc, cc_args) = self.linux_cc();
            Command::new(&cc)
                .args(cc_args)
                .args(["-O0", "-std=c23"])
                .args(&config.args)
                .arg(format!("-I{}", fixtures_root().display()))
                .arg(format!("-I{}", work.display()))
                .arg("-o")
                .arg(temporary)
                .args(&sources)
                .args(["-ldl", "-lm"])
                .output()
                .map_err(|e| format!("spawn {cc}: {e}"))
        })?;
        Ok(out)
    }

    fn build_translated_host(
        &self,
        fixture: &Path,
        config: &FixtureConfig,
        work: &Path,
    ) -> Result<PathBuf, String> {
        let sources = c_sources_in(&fixture.join("host"));
        let crate_dir = work.join("rust");
        let previous = support::fresh_tree::set_aside(&crate_dir);
        std::fs::create_dir_all(&crate_dir).map_err(|e| e.to_string())?;
        let database = work.join("compile_commands.json");
        let host_dir = fixture.join("host");
        let entries: Vec<_> = sources
            .iter()
            .map(|source| {
                let mut arguments = vec![config.compiler.as_deref().unwrap_or("clang").to_string()];
                arguments.extend(self.translate_args.iter().cloned());
                arguments.extend(config.args.iter().cloned());
                arguments.extend(config.translate_args.iter().cloned());
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
        support::fresh_tree::keep_unchanged_mtimes(&previous, &crate_dir);
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

    fn check(&self, fixture: &Path, name: &str, config: &FixtureConfig) -> Result<(), String> {
        let work = manifest_dir()
            .join("target/difftest-generated/dylink")
            .join(self.triple)
            .join(name);
        std::fs::create_dir_all(&work).map_err(|e| e.to_string())?;
        support::write_if_changed(
            work.join("dylink_path.h"),
            format!("#define DEP_PATH \"{}\"\n", self.dep_path).as_bytes(),
        )
        .map_err(|e| e.to_string())?;
        self.build_dependency(fixture, config, &work)?;
        let native = self.build_native_host(fixture, config, &work)?;
        let translated = self.build_translated_host(fixture, config, &work)?;
        let c = self.run(&native, &work)?;
        if c.exit() != Some(0) || c.stdout().is_empty() {
            return Err(format!(
                "native host is not a usable oracle (exit {:?}):\n{}",
                c.exit(),
                String::from_utf8_lossy(c.stdout())
            ));
        }
        let rust = self.run(&translated, &work)?;
        support::compare_runs(&c, &rust, false)?;
        let crate_dir = work.join("rust");
        std::fs::write(crate_dir.join("runtime-ok"), b"").map_err(|e| e.to_string())?;
        for source in c_sources_in(&fixture.join("host")) {
            let text = std::fs::read_to_string(&source).map_err(|e| e.to_string())?;
            if !text.contains("SLATE-FILECHECK-BEGIN") {
                continue;
            }
            let stem = source.file_stem().unwrap().to_string_lossy();
            let unit = if matches!(&*stem, "main" | "lib") {
                format!("__slate_unit_{stem}")
            } else {
                stem.to_string()
            };
            let generated = Some(crate_dir.join("src").join(format!("{unit}.rs")))
                .filter(|path| path.is_file())
                .unwrap_or_else(|| crate_dir.join("src/main.rs"));
            let generated_rust = std::fs::read_to_string(&generated)
                .map_err(|e| format!("read {}: {e}", generated.display()))?;
            support::filecheck::check_generated_rust(
                &text,
                &generated_rust,
                self.triple,
                &crate_dir.join("filecheck").join(stem.as_ref()),
            )
            .map_err(|e| format!("{}: {e}", source.display()))?;
        }
        Ok(())
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
    let fixtures = fixtures(platform);
    assert!(
        !fixtures.is_empty() || platform.opt_in || std::env::var("SLATE_DIFF_FIXTURE").is_ok(),
        "no dylink fixtures found in {}",
        fixtures_root().display()
    );
    let failures: Vec<_> = support::parallel_map(&fixtures, |(name, path, config)| {
        platform
            .check(path, name, config)
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
fn dylink_linux_aarch64() {
    let missing = [
        (
            "qemu-aarch64-static",
            Command::new("qemu-aarch64-static")
                .arg("--version")
                .output()
                .is_ok(),
        ),
        (
            "/usr/aarch64-linux-gnu",
            Path::new("/usr/aarch64-linux-gnu").is_dir(),
        ),
        (
            "rust target aarch64-unknown-linux-gnu",
            support::rust_target_installed(AARCH64_TRIPLE),
        ),
    ]
    .into_iter()
    .filter_map(|(name, present)| (!present).then_some(name))
    .collect::<Vec<_>>();
    if missing.is_empty() {
        run_platform(&Platform::linux_aarch64());
    } else {
        eprintln!(
            "dylink: skipping linux-aarch64 leg, missing {}",
            missing.join(", ")
        );
    }
}

#[test]
fn dylink_linux_i686() {
    if support::rust_target_installed(I686_TRIPLE) {
        run_platform(&Platform::linux_i686());
    } else {
        eprintln!("dylink: skipping linux-i686 leg, missing rust target {I686_TRIPLE}");
    }
}

#[test]
fn dylink_windows() {
    if let Some(platform) = windows_platform() {
        run_platform(&platform);
    }
}
