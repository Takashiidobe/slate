use crate::install::install_staged;
use crate::{DoctorCheck, Paths, Target};
use std::ffi::OsStr;
use std::fs;
use std::io;
use std::path::{Path, PathBuf};
use std::process::Command;

pub(crate) fn install(paths: &Paths, target: Target, xwin: &OsStr) -> io::Result<PathBuf> {
    install_staged(
        paths,
        target,
        |root| validate(root, target),
        |root| {
            let xwin_cache = paths.cache.join("xwin");
            fs::create_dir_all(&xwin_cache)?;
            let status = Command::new(xwin)
                .arg("--arch")
                .arg(arch(target))
                .arg("--cache-dir")
                .arg(&xwin_cache)
                .arg("splat")
                .arg("--output")
                .arg(root)
                .status()
                .map_err(|error| {
                    io::Error::new(error.kind(), format!("could not run xwin: {error}"))
                })?;
            if !status.success() {
                return Err(io::Error::other(format!("xwin failed with {status}")));
            }
            validate(root, target)?;
            fs::write(
                root.join("SYSROOT-MANIFEST.txt"),
                format!(
                    "Target: {}\nOperating system: Windows\nABI: MSVC\nSource: Microsoft CRT and Windows SDK, assembled by xwin\nLicense acceptance: handled by xwin\nContents: headers and libraries; local use only\n",
                    target.triple()
                ),
            )?;
            Ok(())
        },
    )
}

pub(crate) fn include_paths(root: &Path) -> Vec<PathBuf> {
    [
        "crt/include",
        "sdk/include/ucrt",
        "sdk/include/shared",
        "sdk/include/um",
        "sdk/include/winrt",
        "sdk/include/cppwinrt",
    ]
    .into_iter()
    .map(|path| root.join(path))
    .collect()
}

pub(crate) fn validate(root: &Path, target: Target) -> io::Result<()> {
    if let Some(check) = doctor(root, target)
        .into_iter()
        .find(|check| !check.present)
    {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!("missing {} at {}", check.label, check.path.display()),
        ));
    }
    Ok(())
}

pub(crate) fn doctor(root: &Path, target: Target) -> Vec<DoctorCheck> {
    let library_dir = format!("crt/lib/{}", arch(target));
    [
        (
            "MSVC compiler headers",
            "crt/include",
            &["vcruntime.h", "yvals_core.h"][..],
            &[][..],
        ),
        (
            "C runtime headers",
            "sdk/include/ucrt",
            &["stdio.h", "stdlib.h"][..],
            &[][..],
        ),
        (
            "Windows SDK headers",
            "sdk/include",
            &["um/windows.h"][..],
            &["shared", "winrt", "cppwinrt"][..],
        ),
        (
            "MSVC import libraries",
            library_dir.as_str(),
            &["msvcrt.lib"][..],
            &[][..],
        ),
    ]
    .into_iter()
    .map(|(label, relative, files, dirs)| {
        let path = root.join(relative);
        let present = path.is_dir()
            && files.iter().all(|name| path.join(name).is_file())
            && dirs.iter().all(|name| path.join(name).is_dir());
        DoctorCheck {
            label,
            path,
            present,
        }
    })
    .collect()
}

fn arch(target: Target) -> &'static str {
    match target {
        Target::I686PcWindowsMsvc => "x86",
        Target::X86_64PcWindowsMsvc => "x86_64",
        Target::Aarch64PcWindowsMsvc => "aarch64",
        _ => unreachable!("Windows MSVC module received a non-Windows target"),
    }
}
