use slate_sysroots::{Paths, Target};
use std::env;
use std::process::ExitCode;

fn run() -> std::io::Result<ExitCode> {
    let mut args = env::args();
    let program = args.next().unwrap_or_else(|| "slate-sysroots".into());
    let command = args.next();
    let target = args.next();
    if args.next().is_some() || target.is_none() {
        eprintln!("usage: {program} <install|path|doctor> <Rust target triple>");
        return Err(std::io::Error::new(
            std::io::ErrorKind::InvalidInput,
            "expected a command and a Rust target triple",
        ));
    }

    let target = target.unwrap().parse::<Target>()?;
    let paths = Paths::discover()?;
    match command.as_deref() {
        Some("install") => println!("{}", paths.install(target)?.display()),
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
