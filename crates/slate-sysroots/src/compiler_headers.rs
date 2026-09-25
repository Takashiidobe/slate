use crate::install::{install_staged_at, staging_path};
use crate::{DoctorCheck, Paths, Target};
use std::env;
use std::ffi::OsStr;
use std::fs::{self, File};
use std::io;
use std::path::{Component, Path, PathBuf};
use std::process::Command;

pub const CLANG_VERSION: &str = "22.1.8";
pub const GCC_VERSION: &str = "16.1.0";
const GCC_SHA256: &str = "50efb4d94c3397aff3b0d61a5abd748b4dd31d9d3f2ab7be05b171d36a510f79";

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum CompilerHeaders {
    Clang,
    AppleClang,
    Gcc,
    Msvc(Target),
}

impl CompilerHeaders {
    pub fn name(self) -> &'static str {
        match self {
            Self::Clang => "clang",
            Self::AppleClang => "apple-clang",
            Self::Gcc => "gcc",
            Self::Msvc(_) => "msvc",
        }
    }
}

impl Paths {
    pub fn compiler_header_path(&self, compiler: CompilerHeaders) -> PathBuf {
        match compiler {
            CompilerHeaders::Clang => self
                .data
                .join("compiler-headers")
                .join(format!("clang-{CLANG_VERSION}"))
                .join("include"),
            CompilerHeaders::AppleClang => self
                .data
                .join("compiler-headers/apple-clang-current/include"),
            CompilerHeaders::Gcc => self
                .data
                .join("compiler-headers")
                .join(format!("gcc-{GCC_VERSION}"))
                .join("include"),
            CompilerHeaders::Msvc(target) => self.sysroot_path(target).join("crt/include"),
        }
    }

    pub fn resolve_compiler_headers(&self, compiler: CompilerHeaders) -> io::Result<PathBuf> {
        let path = self.compiler_header_path(compiler);
        validate(compiler, &path)?;
        Ok(path)
    }

    pub fn doctor_compiler_headers(&self, compiler: CompilerHeaders) -> Vec<DoctorCheck> {
        checks(compiler, self.compiler_header_path(compiler))
    }

    pub fn install_compiler_headers(&self, compiler: CompilerHeaders) -> io::Result<PathBuf> {
        match compiler {
            CompilerHeaders::Msvc(target) => {
                if !is_msvc_target(target) {
                    return Err(io::Error::new(
                        io::ErrorKind::InvalidInput,
                        format!("{} is not a Windows MSVC target", target.triple()),
                    ));
                }
                self.install(target)?;
                self.resolve_compiler_headers(compiler)
            }
            CompilerHeaders::AppleClang => {
                install_apple_clang(self, env::consts::OS, OsStr::new("xcrun"))
            }
            CompilerHeaders::Clang | CompilerHeaders::Gcc => {
                let include = self.compiler_header_path(compiler);
                let output = include.parent().expect("compiler header path has a parent");
                install_staged_at(
                    output,
                    |root| validate(compiler, &root.join("include")),
                    |root| match compiler {
                        CompilerHeaders::Clang => install_clang(root),
                        CompilerHeaders::Gcc => install_gcc(&self.cache, root),
                        CompilerHeaders::AppleClang | CompilerHeaders::Msvc(_) => unreachable!(),
                    },
                )?;
                Ok(include)
            }
        }
    }
}

fn is_msvc_target(target: Target) -> bool {
    matches!(
        target,
        Target::I686PcWindowsMsvc | Target::X86_64PcWindowsMsvc | Target::Aarch64PcWindowsMsvc
    )
}

fn checks(compiler: CompilerHeaders, path: PathBuf) -> Vec<DoctorCheck> {
    let (label, files): (&'static str, &[&str]) = match compiler {
        CompilerHeaders::Clang => (
            "Clang resource headers",
            &["stdarg.h", "stddef.h", "immintrin.h"],
        ),
        CompilerHeaders::AppleClang => (
            "Apple Clang resource headers",
            &["stdarg.h", "stddef.h", "immintrin.h"],
        ),
        CompilerHeaders::Gcc => (
            "GCC generic headers",
            &["stdarg.h", "stddef.h", "stdint-gcc.h"],
        ),
        CompilerHeaders::Msvc(_) => ("MSVC compiler headers", &["vcruntime.h", "yvals_core.h"]),
    };
    let license_files: &[&str] = match compiler {
        CompilerHeaders::Clang => &["LICENSE.TXT"],
        CompilerHeaders::AppleClang => &["COMPILER-HEADERS-MANIFEST.txt"],
        CompilerHeaders::Gcc => &["COPYING3", "COPYING.RUNTIME"],
        CompilerHeaders::Msvc(_) => &[],
    };
    let present = path.is_dir()
        && files.iter().all(|file| path.join(file).is_file())
        && license_files.iter().all(|file| {
            path.parent()
                .is_some_and(|parent| parent.join(file).is_file())
        });
    vec![DoctorCheck {
        label,
        path,
        present,
    }]
}

pub(crate) fn install_apple_clang(
    paths: &Paths,
    host_os: &str,
    xcrun: &OsStr,
) -> io::Result<PathBuf> {
    if host_os != "macos" {
        return Err(io::Error::new(
            io::ErrorKind::Unsupported,
            "Apple Clang headers can only be acquired from Xcode or Command Line Tools on macOS",
        ));
    }
    let clang = command_output(xcrun, &["--find", "clang"])?;
    let clang = PathBuf::from(clang.trim());
    let resource = command_output(clang.as_os_str(), &["-print-resource-dir"])?;
    let resource = PathBuf::from(resource.trim());
    let version_info = command_output(clang.as_os_str(), &["--version"])?;
    let version = apple_clang_version(&version_info)?;
    let source = resource.join("include");
    if !source.is_dir() {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!(
                "Apple Clang resource headers are missing at {}",
                source.display()
            ),
        ));
    }
    let parent = paths.data.join("compiler-headers");
    let output = parent.join(format!("apple-clang-{version}"));
    install_staged_at(
        &output,
        |root| validate(CompilerHeaders::AppleClang, &root.join("include")),
        |root| {
            copy_tree(&source, &root.join("include"))?;
            fs::write(
                root.join("COMPILER-HEADERS-MANIFEST.txt"),
                format!(
                    "Compiler: Apple Clang\nVersion: {version}\nCompiler path: {}\nResource directory: {}\nAcquisition: copied from locally installed Xcode or Command Line Tools\nDistribution: local use only; Apple toolchain license applies\n",
                    clang.display(),
                    resource.display()
                ),
            )?;
            Ok(())
        },
    )?;
    let current = parent.join("apple-clang-current");
    let staging = staging_path(&parent, "apple-clang-current")?;
    link_current(&output, &staging)?;
    if let Err(error) = fs::rename(&staging, &current) {
        let _ = fs::remove_file(&staging);
        return Err(error);
    }
    paths.resolve_compiler_headers(CompilerHeaders::AppleClang)
}

#[cfg(unix)]
fn link_current(output: &Path, link: &Path) -> io::Result<()> {
    std::os::unix::fs::symlink(output.file_name().expect("bundle has a name"), link)
}

#[cfg(windows)]
fn link_current(_output: &Path, _link: &Path) -> io::Result<()> {
    Err(io::Error::new(
        io::ErrorKind::Unsupported,
        "Apple Clang requires macOS",
    ))
}

fn command_output(command: &OsStr, args: &[&str]) -> io::Result<String> {
    let output = Command::new(command).args(args).output().map_err(|error| {
        io::Error::new(
            error.kind(),
            format!("could not run {}: {error}", command.to_string_lossy()),
        )
    })?;
    if !output.status.success() {
        return Err(io::Error::other(format!(
            "{} failed with {}",
            command.to_string_lossy(),
            output.status
        )));
    }
    String::from_utf8(output.stdout).map_err(io::Error::other)
}

fn apple_clang_version(info: &str) -> io::Result<String> {
    let line = info.lines().find_map(|line| {
        line.split_once("Apple clang version ")
            .map(|(_, rest)| rest)
    });
    let line = line.ok_or_else(|| {
        io::Error::new(
            io::ErrorKind::InvalidData,
            "could not determine Apple Clang version",
        )
    })?;
    let version = line.split_whitespace().next().unwrap_or_default();
    let build = line
        .split_once("(clang-")
        .and_then(|(_, rest)| rest.split_once(')').map(|(build, _)| build));
    let label = match build {
        Some(build) => format!("{version}-clang-{build}"),
        None => version.to_owned(),
    };
    if label.is_empty()
        || !label
            .chars()
            .all(|c| c.is_ascii_alphanumeric() || "._+-".contains(c))
    {
        return Err(io::Error::new(
            io::ErrorKind::InvalidData,
            "invalid Apple Clang version label",
        ));
    }
    Ok(label)
}

fn validate(compiler: CompilerHeaders, path: &Path) -> io::Result<()> {
    if let CompilerHeaders::Msvc(target) = compiler
        && !is_msvc_target(target)
    {
        return Err(io::Error::new(
            io::ErrorKind::InvalidInput,
            "expected an MSVC target",
        ));
    }
    let check = checks(compiler, path.to_path_buf()).remove(0);
    if check.present {
        Ok(())
    } else {
        Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!("missing {} at {}", check.label, path.display()),
        ))
    }
}

fn install_clang(root: &Path) -> io::Result<()> {
    let source = root.join("source");
    let tag = format!("llvmorg-{CLANG_VERSION}");
    run_git(
        [
            "clone",
            "--quiet",
            "--depth",
            "1",
            "--filter=blob:none",
            "--sparse",
            "--branch",
            &tag,
            "https://github.com/llvm/llvm-project.git",
        ],
        &source,
    )?;
    let status = Command::new("git")
        .arg("-C")
        .arg(&source)
        .args([
            "sparse-checkout",
            "set",
            "--no-cone",
            "/clang/lib/Headers/",
            "/LICENSE.TXT",
            "/NOTICE.TXT",
        ])
        .status()?;
    if !status.success() {
        return Err(io::Error::other(format!(
            "git sparse-checkout failed with {status}"
        )));
    }
    copy_tree(&source.join("clang/lib/Headers"), &root.join("include"))?;
    fs::copy(source.join("LICENSE.TXT"), root.join("LICENSE.TXT"))?;
    if source.join("NOTICE.TXT").is_file() {
        fs::copy(source.join("NOTICE.TXT"), root.join("NOTICE.TXT"))?;
    }
    fs::remove_dir_all(source)?;
    fs::write(
        root.join("COMPILER-HEADERS-MANIFEST.txt"),
        format!(
            "Compiler: upstream Clang\nVersion: {CLANG_VERSION}\nSource repository: https://github.com/llvm/llvm-project.git\nSource tag: {tag}\nSource path: clang/lib/Headers\n"
        ),
    )?;
    Ok(())
}

fn run_git<const N: usize>(args: [&str; N], destination: &Path) -> io::Result<()> {
    let status = Command::new("git")
        .args(["-c", "advice.detachedHead=false"])
        .args(args)
        .arg(destination)
        .status()
        .map_err(|error| io::Error::new(error.kind(), format!("could not run git: {error}")))?;
    if status.success() {
        Ok(())
    } else {
        Err(io::Error::other(format!("git clone failed with {status}")))
    }
}

fn copy_tree(source: &Path, destination: &Path) -> io::Result<()> {
    fs::create_dir_all(destination)?;
    for entry in fs::read_dir(source)? {
        let entry = entry?;
        let target = destination.join(entry.file_name());
        let kind = entry.file_type()?;
        if kind.is_dir() {
            copy_tree(&entry.path(), &target)?;
        } else if kind.is_file() {
            fs::copy(entry.path(), target)?;
        } else if kind.is_symlink() {
            copy_link(&entry.path(), &target)?;
        } else {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "unexpected Clang header entry",
            ));
        }
    }
    Ok(())
}

#[cfg(unix)]
fn copy_link(source: &Path, target: &Path) -> io::Result<()> {
    std::os::unix::fs::symlink(fs::read_link(source)?, target)
}

#[cfg(windows)]
fn copy_link(source: &Path, target: &Path) -> io::Result<()> {
    if source.is_dir() {
        copy_tree(source, target)
    } else {
        fs::copy(source, target).map(|_| ())
    }
}

fn install_gcc(cache: &Path, root: &Path) -> io::Result<()> {
    let filename = format!("gcc-{GCC_VERSION}.tar.xz");
    let url = format!("https://ftp.gnu.org/gnu/gcc/gcc-{GCC_VERSION}/{filename}");
    let archive =
        crate::download::fetch(&cache.join("compiler-headers"), &filename, &url, GCC_SHA256)?;
    let file = File::open(archive)?;
    let decoder = xz2::read::XzDecoder::new(file);
    let mut archive = tar::Archive::new(decoder);
    let prefix = format!("gcc-{GCC_VERSION}/gcc/ginclude/");
    let license_prefix = format!("gcc-{GCC_VERSION}/");
    for entry in archive.entries()? {
        let mut entry = entry?;
        let path = entry.path()?;
        let path = path.as_ref();
        let output = if let Ok(relative) = path.strip_prefix(&prefix) {
            Some(root.join("include").join(relative))
        } else if let Ok(relative) = path.strip_prefix(&license_prefix) {
            match relative.to_str() {
                Some("COPYING3" | "COPYING.RUNTIME") => Some(root.join(relative)),
                _ => None,
            }
        } else {
            None
        };
        let Some(output) = output else { continue };
        if path
            .components()
            .any(|component| !matches!(component, Component::Normal(_)))
        {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "invalid GCC archive path",
            ));
        }
        if entry.header().entry_type().is_dir() {
            fs::create_dir_all(&output)?;
        } else if entry.header().entry_type().is_file() {
            fs::create_dir_all(output.parent().expect("archive entry has a parent"))?;
            let mut file = File::create(output)?;
            io::copy(&mut entry, &mut file)?;
        } else {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "unexpected GCC archive entry",
            ));
        }
    }
    fs::write(
        root.join("COMPILER-HEADERS-MANIFEST.txt"),
        format!(
            "Compiler: GCC\nVersion: {GCC_VERSION}\nSource URL: {url}\nSource path: gcc/ginclude\nScope: generic compiler headers only; target-specific and generated headers omitted\n"
        ),
    )?;
    Ok(())
}
