use slate_parser::compiler_args::CompilerFlavor;
use slate_parser::native_headers;
use slate_parser::target_info::TargetInfo;
use slate_sysroots::{CompilerHeaders, GccFamily, Paths, Target};
use std::path::Path;
use std::process::ExitCode;

pub fn run(args: impl IntoIterator<Item = String>) -> std::io::Result<ExitCode> {
    let mut args = args.into_iter();
    let program = "slate sysroot";
    let command = args.next();
    let subject = args.next();
    let usage = format!(
        "usage: {program} <install|remove|path|doctor> <Rust target triple>\n       {program} install <Darwin target> [--sdk <path>]\n       {program} <install|path|doctor> compiler-headers <clang|apple-clang>\n       {program} <install|path|doctor> compiler-headers gcc [x86|aarch64|arm]\n       {program} <install|path|doctor> compiler-headers msvc [MSVC target triple]\n       {program} <path|doctor> native [clang|gcc|msvc]"
    );
    if matches!(command.as_deref(), Some("-h" | "--help")) {
        println!("{usage}");
        return Ok(ExitCode::SUCCESS);
    }
    if !matches!(
        command.as_deref(),
        Some("install" | "remove" | "path" | "doctor")
    ) {
        eprintln!("{usage}");
        return Err(std::io::Error::new(
            std::io::ErrorKind::InvalidInput,
            "expected install, remove, path, or doctor",
        ));
    }
    let paths = Paths::discover()?;
    if subject.as_deref() == Some("compiler-headers") {
        if command.as_deref() == Some("remove") {
            return Err(std::io::Error::new(
                std::io::ErrorKind::InvalidInput,
                "remove applies to target sysroots, not compiler headers",
            ));
        }
        let compiler = args.next().ok_or_else(|| {
            std::io::Error::new(
                std::io::ErrorKind::InvalidInput,
                "expected clang, apple-clang, gcc, or msvc",
            )
        })?;
        let target = args.next();
        if args.next().is_some() {
            eprintln!("{usage}");
            return Err(std::io::Error::new(
                std::io::ErrorKind::InvalidInput,
                "too many arguments",
            ));
        }
        let specs = match compiler.as_str() {
            "clang" if target.is_none() => vec![CompilerHeaders::Clang],
            "apple-clang" if target.is_none() => vec![CompilerHeaders::AppleClang],
            "gcc" => match target {
                Some(family) => vec![CompilerHeaders::Gcc(family.parse::<GccFamily>()?)],
                None => GccFamily::ALL.map(CompilerHeaders::Gcc).to_vec(),
            },
            "msvc" => match target {
                Some(target) => vec![CompilerHeaders::Msvc(target.parse::<Target>()?)],
                None => vec![
                    CompilerHeaders::Msvc(Target::I686PcWindowsMsvc),
                    CompilerHeaders::Msvc(Target::X86_64PcWindowsMsvc),
                    CompilerHeaders::Msvc(Target::Aarch64PcWindowsMsvc),
                    CompilerHeaders::Msvc(Target::Thumbv7aPcWindowsMsvc),
                ],
            },
            _ => {
                return Err(std::io::Error::new(
                    std::io::ErrorKind::InvalidInput,
                    "expected clang or apple-clang without a target, gcc with an optional header family, or msvc with an optional Windows MSVC target",
                ));
            }
        };
        let mut healthy = true;
        for spec in specs {
            match command.as_deref() {
                Some("install") => println!("{}", paths.install_compiler_headers(spec)?.display()),
                Some("path") => println!("{}", paths.resolve_compiler_headers(spec)?.display()),
                Some("doctor") => {
                    for check in paths.doctor_compiler_headers(spec) {
                        let mark = if check.present { '✓' } else { '✗' };
                        println!("{mark} {}: {}", check.label, check.path.display());
                        healthy &= check.present;
                    }
                }
                _ => unreachable!(),
            }
        }
        return Ok(if healthy {
            ExitCode::SUCCESS
        } else {
            ExitCode::FAILURE
        });
    }
    if subject.as_deref() == Some("native") {
        return native(command.as_deref(), args.next(), args.next().is_some());
    }
    let target = subject.ok_or_else(|| {
        std::io::Error::new(
            std::io::ErrorKind::InvalidInput,
            "expected a Rust target triple",
        )
    })?;
    let target = target.parse::<Target>()?;
    let option = args.next();
    let option_path = args.next();
    if args.next().is_some()
        || option.is_some() != option_path.is_some()
        || (option.is_some() && command.as_deref() != Some("install"))
    {
        eprintln!("{usage}");
        return Err(std::io::Error::new(
            std::io::ErrorKind::InvalidInput,
            "invalid target options",
        ));
    }
    match command.as_deref() {
        Some("install") => {
            let installed = match (option.as_deref(), option_path.as_deref()) {
                (None, None) => paths.install(target)?,
                (Some("--sdk"), Some(path)) => {
                    paths.install_darwin_with_sdk(target, Path::new(path))?
                }
                _ => {
                    return Err(std::io::Error::new(
                        std::io::ErrorKind::InvalidInput,
                        "expected --sdk <path> for Darwin",
                    ));
                }
            };
            println!("{}", installed.display());
        }
        Some("path") => println!("{}", paths.resolve(target)?.display()),
        Some("remove") => {
            paths.remove(target)?;
            println!("removed {}", target.triple());
        }
        Some("doctor") => {
            let checks = paths.doctor(target);
            for check in &checks {
                let mark = if check.present { '✓' } else { '✗' };
                println!("{mark} {}: {}", check.label, check.path.display());
            }
            return Ok(if checks.iter().all(|check| check.present) {
                ExitCode::SUCCESS
            } else {
                ExitCode::FAILURE
            });
        }
        _ => {
            return Err(std::io::Error::new(
                std::io::ErrorKind::InvalidInput,
                "expected install, remove, path, or doctor",
            ));
        }
    }
    Ok(ExitCode::SUCCESS)
}

fn native(command: Option<&str>, flavor: Option<String>, extra: bool) -> std::io::Result<ExitCode> {
    let invalid = |message: String| std::io::Error::new(std::io::ErrorKind::InvalidInput, message);
    if extra || !matches!(command, Some("path" | "doctor")) {
        return Err(invalid(
            "native supports `path` and `doctor` with an optional clang, gcc, or msvc".into(),
        ));
    }
    let flavor = flavor
        .as_deref()
        .unwrap_or("clang")
        .parse::<CompilerFlavor>()
        .map_err(invalid)?;
    let host = slate_parser::target_registry::host()
        .ok_or_else(|| invalid("Slate does not support this host as a target".into()))?;
    let target = TargetInfo::for_triple(host.triple).map_err(|error| invalid(error.to_string()))?;
    let headers = native_headers::discover(&target, flavor, None).map_err(std::io::Error::other)?;
    if command == Some("path") {
        for path in &headers.include {
            println!("{}", path.display());
        }
        return Ok(ExitCode::SUCCESS);
    }
    println!("✓ compiler: {} for {}", headers.compiler, target.triple);
    let mut healthy = true;
    for path in &headers.include {
        let present = path.is_dir();
        let mark = if present { '✓' } else { '✗' };
        let label = if headers.is_compiler_path(path) {
            "compiler headers"
        } else {
            "system headers"
        };
        println!("{mark} {label}: {}", path.display());
        healthy &= present;
    }
    let stdio = headers
        .include
        .iter()
        .map(|path| path.join("stdio.h"))
        .find(|path| path.is_file());
    match &stdio {
        Some(path) => println!("✓ stdio.h: {}", path.display()),
        None => println!("✗ stdio.h: not found"),
    }
    Ok(if healthy && stdio.is_some() {
        ExitCode::SUCCESS
    } else {
        ExitCode::FAILURE
    })
}

pub fn main_result(args: impl IntoIterator<Item = String>) -> ExitCode {
    match run(args) {
        Ok(code) => code,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
