use slate_sysroots::{CompilerHeaders, Paths, Target};
use std::env;
use std::path::Path;
use std::process::ExitCode;

fn run() -> std::io::Result<ExitCode> {
    let mut args = env::args();
    let program = args.next().unwrap_or_else(|| "slate-sysroots".into());
    let command = args.next();
    let subject = args.next();
    let usage = format!(
        "usage: {program} <install|path|doctor> <Rust target triple>\n       {program} install <Darwin target> [--sdk <path>]\n       {program} <install|path|doctor> compiler-headers <clang|apple-clang|gcc|msvc> [MSVC target triple]"
    );
    if !matches!(command.as_deref(), Some("install" | "path" | "doctor")) {
        eprintln!("{usage}");
        return Err(std::io::Error::new(
            std::io::ErrorKind::InvalidInput,
            "expected install, path, or doctor",
        ));
    }
    let paths = Paths::discover()?;
    if subject.as_deref() == Some("compiler-headers") {
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
            "gcc" if target.is_none() => vec![CompilerHeaders::Gcc],
            "msvc" => match target {
                Some(target) => vec![CompilerHeaders::Msvc(target.parse::<Target>()?)],
                None => vec![
                    CompilerHeaders::Msvc(Target::I686PcWindowsMsvc),
                    CompilerHeaders::Msvc(Target::X86_64PcWindowsMsvc),
                    CompilerHeaders::Msvc(Target::Aarch64PcWindowsMsvc),
                ],
            },
            _ => {
                return Err(std::io::Error::new(
                    std::io::ErrorKind::InvalidInput,
                    "expected clang, apple-clang, or gcc without a target, or msvc with an optional Windows MSVC target",
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
                "expected install, path, or doctor",
            ));
        }
    }
    Ok(ExitCode::SUCCESS)
}

fn main() -> ExitCode {
    match run() {
        Ok(code) => code,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
