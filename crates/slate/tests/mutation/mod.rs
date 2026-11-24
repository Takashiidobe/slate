use super::{Flavor, fixture_flavor, fixtures_dir, selected_flavor, support, target_fixtures};
use slate_analysis::transform::{Passes, Slot};
use slate_analysis::{Program, UnitId, analyze};
use slate_mutate::{Mutation, Rule, candidates};
use slate_parser::ast::FileId;
use slate_parser::{files::Files, ir::Module};
use std::fs;
use std::path::{Path, PathBuf};

struct Oracle<'a> {
    config: support::RunConfig,
    cross: Option<&'a support::CrossTarget>,
    msvc_arch: Option<&'static str>,
}

impl Oracle<'_> {
    fn run(&self, seed: &Path, source: &str, dir: &Path) -> Result<support::Run, String> {
        fs::create_dir_all(dir).map_err(|error| error.to_string())?;
        let path = dir.join(seed.file_name().unwrap());
        let filename = seed.to_str().ok_or("source path must be UTF-8")?;
        let filename = filename.replace('\\', "\\\\").replace('"', "\\\"");
        fs::write(&path, format!("#line 1 \"{filename}\"\n{source}"))
            .map_err(|error| error.to_string())?;
        let compiler_dir = tempfile::tempdir().map_err(|error| error.to_string())?;
        let mut flags = self.config.c_args.clone();
        let include_dir = seed.parent().unwrap().display();
        let binary = if let Some(arch) = self.msvc_arch {
            flags.insert(0, format!("/I{include_dir}"));
            let binary = compiler_dir.path().join("oracle.exe");
            support::compile_c_msvc(&path, &binary, &flags, arch)?;
            binary
        } else {
            flags.splice(0..0, ["-iquote".into(), include_dir.to_string()]);
            let binary = compiler_dir.path().join("oracle");
            support::compile_c_with_args_for_target(
                &path,
                &binary,
                &flags,
                self.config.cc.as_deref(),
                self.cross,
            )?;
            binary
        };
        support::run_with_config_for_target(&binary, &self.config, compiler_dir.path(), self.cross)
    }
}

fn explicit_source() -> Option<(PathBuf, Flavor)> {
    let path = PathBuf::from(std::env::var_os("SLATE_MUTATE_SOURCE")?);
    let path = if path.is_absolute() {
        path
    } else {
        Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("../..")
            .join(path)
    };
    let flavor = selected_flavor().unwrap_or_else(|| {
        path.canonicalize()
            .unwrap_or_else(|_| path.clone())
            .strip_prefix(fixtures_dir())
            .map(fixture_flavor)
            .unwrap_or(Flavor::Clang)
    });
    Some((path, flavor))
}

#[test]
fn mutation_differential() {
    assert!(
        std::env::var_os("SLATE_MUTATE_SOURCE").is_some()
            || std::env::var_os("SLATE_DIFF_FIXTURE").is_some()
            || std::env::var("SLATE_MUTATE_ALL").as_deref() == Ok("1"),
        "select SLATE_MUTATE_SOURCE, SLATE_DIFF_FIXTURE, or SLATE_MUTATE_ALL=1"
    );
    let extra = shlex::split(&std::env::var("SLATE_MUTATE_FLAGS").unwrap_or_default())
        .expect("SLATE_MUTATE_FLAGS has unmatched quotes");
    let limit = std::env::var("SLATE_MUTATE_LIMIT")
        .map(|limit| limit.parse::<usize>().expect("invalid SLATE_MUTATE_LIMIT"))
        .unwrap_or(16);
    assert!(limit > 0, "SLATE_MUTATE_LIMIT must be positive");

    let root = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/difftest-mutations");
    fs::create_dir_all(&root).expect("create mutation output directory");
    let build = tempfile::tempdir().expect("create mutation build directory");
    let mut total = 0;
    let mut failures = Vec::new();
    for triple in support::test_targets() {
        let seeds = if let Some(source) = explicit_source() {
            vec![source]
        } else {
            let unsupported =
                super::target_unsupported(&super::unsupported_dir().join(format!("{triple}.txt")));
            target_fixtures(&fixtures_dir(), &triple)
                .into_iter()
                .filter(|fixture| {
                    fixture.flavor != Flavor::Clang || !unsupported.contains(&fixture.name)
                })
                .map(|fixture| (fixture.path, fixture.flavor))
                .collect()
        };
        for (path, flavor) in seeds {
            match run_seed(&path, flavor, &triple, &extra, limit, build.path(), &root) {
                Ok(count) => total += count,
                Err(error) => failures.push(format!("{triple} {}: {error}", path.display())),
            }
        }
    }
    assert!(failures.is_empty(), "{}", failures.join("\n\n"));
    assert!(
        total != 0,
        "no runnable mutants; choose a seed with eligible expressions"
    );
    eprintln!("compared {total} IR mutants");
}

fn run_seed(
    path: &Path,
    flavor: Flavor,
    triple: &str,
    extra: &[String],
    limit: usize,
    build: &Path,
    artifacts: &Path,
) -> Result<usize, String> {
    let cross = if flavor == Flavor::Msvc {
        support::CrossTarget::windows_msvc(triple.split('-').next().unwrap())
    } else {
        support::CrossTarget::for_triple(triple)
    };
    let msvc_arch = support::msvc_oracle_arch(cross.as_ref()).filter(|_| flavor == Flavor::Msvc);
    if cross.as_ref().is_some_and(|cross| cross.build_only)
        || (flavor == Flavor::Msvc && (cross.is_none() || msvc_arch.is_none()))
    {
        eprintln!("skip {}: no runnable oracle for {triple}", path.display());
        return Ok(0);
    }
    let target = cross.as_ref().map_or(triple, |cross| cross.rust_triple);
    let path = path.canonicalize().map_err(|error| error.to_string())?;
    let source = fs::read_to_string(&path).map_err(|error| error.to_string())?;
    let mut flags = support::mutation_fixture_flags(&path, target);
    flags.extend_from_slice(extra);
    let mut parser_args = vec!["-std=c23".into(), format!("-target={target}")];
    match flavor {
        Flavor::Clang => {}
        Flavor::Gcc => parser_args.push("--flavor=gcc".into()),
        Flavor::Msvc => parser_args.push("--flavor=msvc".into()),
    }
    parser_args.extend_from_slice(&flags);
    let (module, files) =
        slate::api::slate_ir_with_passes(&path, &parser_args, &Passes::named("inline-temps")?)
            .map_err(|error| format!("seed parser failed: {error}"))?;
    let allow_non_finite = flavor != Flavor::Msvc
        && !flags.iter().any(|flag| {
            matches!(
                flag.as_str(),
                "-ffast-math" | "-ffinite-math-only" | "-Ofast"
            )
        });
    let variants: Vec<_> = materialize(&path, &source, &module, &files, allow_non_finite)?
        .take(limit)
        .collect();
    if variants.is_empty() {
        return Ok(0);
    }

    let work = tempfile::tempdir_in(artifacts).map_err(|error| error.to_string())?;
    let oracle = Oracle {
        config: support::RunConfig {
            cc: (flavor == Flavor::Gcc).then(|| "gcc".into()),
            c_args: flags,
            timeout_seconds: Some(5),
            env: if flavor == Flavor::Msvc {
                [("WINEDEBUG".into(), "-all".into())].into()
            } else {
                Default::default()
            },
            ..Default::default()
        },
        cross: cross.as_ref(),
        msvc_arch,
    };
    let result = compare_seed_and_mutants(
        &path,
        &source,
        &module,
        &files,
        &oracle,
        Work {
            artifacts: work.path(),
            build,
        },
        &variants,
    );
    if let Err(error) = result {
        let _ = fs::write(
            work.path().join("arguments.json"),
            serde_json::to_string_pretty(&parser_args).unwrap(),
        );
        return Err(format!("{error}\nartifacts: {}", work.keep().display()));
    }
    result
}

fn materialize<'a>(
    path: &Path,
    source: &'a str,
    module: &Module,
    files: &Files,
    allow_non_finite: bool,
) -> Result<impl Iterator<Item = (Mutation, String)> + 'a, String> {
    let file = files
        .paths()
        .position(|file| file == path)
        .map(|index| FileId(index as u32))
        .ok_or("seed source missing from parser file table")?;
    let rules = std::env::var("SLATE_MUTATE_RULES").ok();
    let program = Program::new([(module, files)], |_, _| true, false);
    let facts = analyze(&program);
    let mut mutations = candidates(module, facts.unit(UnitId(0)));
    if let Ok(seed) = std::env::var("SLATE_MUTATE_SEED") {
        let seed = seed
            .parse::<u64>()
            .map_err(|_| "invalid SLATE_MUTATE_SEED")?;
        fastrand::Rng::with_seed(seed).shuffle(&mut mutations);
    }
    Ok(mutations.into_iter().filter_map(move |mutation| {
        if mutation.rule == Rule::FloatSpecial && !allow_non_finite {
            return None;
        }
        if rules.as_ref().is_some_and(|rules| {
            !rules
                .split(',')
                .any(|rule| rule.trim() == mutation.rule.strategy())
        }) {
            return None;
        }
        let source = mutation.oracle_source(file, source)?;
        Some((mutation, source))
    }))
}

struct Work<'a> {
    artifacts: &'a Path,
    build: &'a Path,
}

fn compare_seed_and_mutants(
    path: &Path,
    source: &str,
    module: &Module,
    files: &Files,
    oracle: &Oracle<'_>,
    work: Work<'_>,
    variants: &[(Mutation, String)],
) -> Result<usize, String> {
    let mut runs = Vec::new();
    let mut cases = Vec::new();
    let mut failures = Vec::new();

    for (index, (mutation, c)) in std::iter::once((None, source))
        .chain(
            variants
                .iter()
                .map(|(mutation, source)| (Some(mutation), source.as_str())),
        )
        .enumerate()
    {
        let name = format!("variant_{index}");
        let dir = work.artifacts.join(&name);
        let description = mutation.map_or_else(
            || "seed".into(),
            |mutation| {
                format!(
                    "{name} {:?} at byte {} -> {}",
                    mutation.rule,
                    mutation.location.offset,
                    mutation.description()
                )
            },
        );
        let c_run = match oracle.run(path, c, &dir) {
            Ok(run) if run.exit().is_some() => run,
            Ok(_) if mutation.is_some() => {
                eprintln!("skip {description}: C terminated by signal");
                continue;
            }
            Err(error)
                if mutation.is_some()
                    && (error.starts_with("C compile failed")
                        || error.starts_with("cl.exe compile failed")
                        || error.contains(" timed out after ")) =>
            {
                eprintln!("skip {description}: {error}");
                continue;
            }
            Ok(_) => return Err("C seed terminated by signal".into()),
            Err(error) => return Err(format!("{description}: {error}")),
        };
        let mut input = module.clone();
        if let Some(mutation) = mutation
            && !mutation.apply(&mut input)
        {
            return Err(format!("{description}: failed to apply mutation to IR"));
        }
        Passes::default().run(&mut input, Slot::AfterMacroStrip);
        match slate::api::translate_ir(&input, files, path) {
            Ok(rust) => {
                let rs_src = dir.join("generated.rs");
                fs::write(&rs_src, rust).map_err(|error| error.to_string())?;
                runs.push((description, c_run));
                cases.push(support::RustCase { name, rs_src });
            }
            Err(error) if mutation.is_some() => {
                failures.push(format!("{description}: IR translation failed: {error}"));
            }
            Err(error) => return Err(format!("seed IR translation failed: {error}")),
        }
    }
    let project = work.build.join("batch_cargo");
    let built = support::build_batch_in(
        &cases,
        &project,
        &project.join("src/bin"),
        support::test_jobs(),
        oracle.cross,
        &work.build.join("target"),
    )?;
    let count = cases.len() - 1;
    for (case, (description, expected)) in cases.iter().zip(runs) {
        let result = built
            .executable(&case.name)
            .and_then(|binary| {
                let dir = tempfile::tempdir().map_err(|error| error.to_string())?;
                support::run_with_config_for_target(
                    &binary,
                    &oracle.config,
                    dir.path(),
                    oracle.cross,
                )
            })
            .and_then(|actual| {
                support::compare_runs(&expected, &actual, oracle.config.compare_stderr)
            });
        if let Err(error) = result {
            if case.name == "variant_0" {
                return Err(format!("seed differential failed: {error}"));
            }
            failures.push(format!("{description}: {error}"));
        }
    }
    if failures.is_empty() {
        if count > 0 {
            eprintln!("{}: compared {count} mutants", path.display());
        }
        Ok(count)
    } else {
        Err(failures.join("\n\n"))
    }
}
