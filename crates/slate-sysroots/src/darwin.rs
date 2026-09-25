use crate::install::install_staged;
use crate::{DoctorCheck, Paths, Target};
use std::env;
use std::ffi::OsStr;
use std::fs;
use std::io;
use std::path::{Path, PathBuf};
use std::process::Command;

pub(crate) fn install(paths: &Paths, target: Target) -> io::Result<PathBuf> {
    if paths.sysroot_path(target).exists() {
        return paths.resolve(target);
    }
    let sdk = env::var_os("OSX_CROSS_SDK");
    let sdk = find_sdk(
        env::consts::OS,
        sdk.as_deref().map(Path::new),
        OsStr::new("xcrun"),
    )?;
    install_with_sdk(paths, target, &sdk)
}

pub(crate) fn find_sdk(
    host_os: &str,
    supplied_sdk: Option<&Path>,
    xcrun: &OsStr,
) -> io::Result<PathBuf> {
    if let Some(sdk) = supplied_sdk {
        return Ok(sdk.to_path_buf());
    }
    if host_os != "macos" {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!(
                "automatic macOS SDK discovery is unavailable on {host_os}; on a Mac, install Xcode Command Line Tools with `xcode-select --install`, or supply an existing SDK with --sdk <path> or OSX_CROSS_SDK"
            ),
        ));
    }
    let output = Command::new(xcrun)
        .args(["--sdk", "macosx", "--show-sdk-path"])
        .output()
        .map_err(|error| {
            io::Error::new(
                error.kind(),
                format!("could not run xcrun: {error}; install Xcode Command Line Tools with `xcode-select --install`"),
            )
        })?;
    if !output.status.success() {
        return Err(io::Error::other(format!(
            "xcrun failed with {}; install Xcode Command Line Tools with `xcode-select --install`",
            output.status
        )));
    }
    let path = String::from_utf8(output.stdout).map_err(io::Error::other)?;
    if path.trim().is_empty() {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            "xcrun returned no macOS SDK path; install Xcode Command Line Tools with `xcode-select --install`",
        ));
    }
    Ok(PathBuf::from(path.trim()))
}

pub(crate) fn install_with_sdk(paths: &Paths, target: Target, sdk: &Path) -> io::Result<PathBuf> {
    if !matches!(
        target,
        Target::X86_64AppleDarwin | Target::Aarch64AppleDarwin
    ) {
        return Err(io::Error::new(
            io::ErrorKind::InvalidInput,
            format!("{} is not a macOS target", target.triple()),
        ));
    }
    let sdk = fs::canonicalize(sdk)?;
    if !sdk.join("usr/include/stdio.h").is_file() {
        return Err(io::Error::new(
            io::ErrorKind::InvalidInput,
            format!(
                "{} is not a macOS SDK (missing usr/include/stdio.h)",
                sdk.display()
            ),
        ));
    }
    install_staged(paths, target, validate, |root| {
        fs::create_dir(root)?;
        link_sdk(&sdk, &root.join("SDK"))?;
        fs::write(
            root.join("SYSROOT-MANIFEST.txt"),
            format!(
                "Target: {}\nOperating system: macOS\nSDK path: {}\nSDK version: {}\nSource: user-provided Xcode, Command Line Tools, or OSXCross SDK\nContents: SDK referenced by symlink; not copied or distributed by Slate\n",
                target.triple(),
                sdk.display(),
                sdk.file_name().unwrap_or_default().to_string_lossy()
            ),
        )?;
        Ok(())
    })
}

#[cfg(unix)]
fn link_sdk(source: &Path, destination: &Path) -> io::Result<()> {
    std::os::unix::fs::symlink(source, destination)
}

#[cfg(windows)]
fn link_sdk(source: &Path, destination: &Path) -> io::Result<()> {
    std::os::windows::fs::symlink_dir(source, destination)
}

pub(crate) fn include_paths(root: &Path) -> Vec<PathBuf> {
    vec![root.join("SDK/usr/include")]
}

pub(crate) fn doctor(root: &Path) -> Vec<DoctorCheck> {
    let sdk = root.join("SDK");
    vec![
        DoctorCheck {
            label: "macOS SDK link",
            present: sdk.is_dir()
                && fs::symlink_metadata(&sdk).is_ok_and(|m| m.file_type().is_symlink()),
            path: sdk.clone(),
        },
        DoctorCheck {
            label: "macOS SDK headers",
            present: sdk.join("usr/include/stdio.h").is_file()
                && sdk.join("usr/include/sys/types.h").is_file(),
            path: sdk.join("usr/include"),
        },
    ]
}

pub(crate) fn validate(root: &Path) -> io::Result<()> {
    if let Some(check) = doctor(root).into_iter().find(|check| !check.present) {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!("missing {} at {}", check.label, check.path.display()),
        ));
    }
    Ok(())
}
