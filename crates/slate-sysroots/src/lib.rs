mod download;
mod install;
mod linux_gnu;
mod linux_musl;
mod target;
mod windows_msvc;

pub use target::Target;

use directories::ProjectDirs;
use std::env;
use std::ffi::OsStr;
use std::io;
use std::path::PathBuf;

#[derive(Clone, Debug)]
pub struct Paths {
    pub data: PathBuf,
    pub cache: PathBuf,
}

#[derive(Clone, Debug)]
pub struct DoctorCheck {
    pub label: &'static str,
    pub path: PathBuf,
    pub present: bool,
}

impl Paths {
    pub fn discover() -> io::Result<Self> {
        let dirs = ProjectDirs::from("", "", "Slate")
            .ok_or_else(|| io::Error::other("could not determine Slate directories"))?;
        Ok(Self {
            data: dirs.data_local_dir().to_path_buf(),
            cache: dirs.cache_dir().to_path_buf(),
        })
    }

    pub fn sysroot_path(&self, target: Target) -> PathBuf {
        self.data.join("sysroots").join(target.triple())
    }

    pub fn resolve(&self, target: Target) -> io::Result<PathBuf> {
        let path = self.sysroot_path(target);
        let validation = match target {
            Target::X86_64PcWindowsMsvc | Target::Aarch64PcWindowsMsvc => {
                windows_msvc::validate(&path, target)
            }
            Target::X86_64UnknownLinuxGnu | Target::Aarch64UnknownLinuxGnu => {
                linux_gnu::validate(&path, target)
            }
            Target::X86_64UnknownLinuxMusl | Target::Aarch64UnknownLinuxMusl => {
                linux_musl::validate(&path, target)
            }
        };
        validation.map_err(|error| {
            io::Error::new(
                error.kind(),
                format!(
                    "{} is not installed or is incomplete: {error}",
                    target.triple()
                ),
            )
        })?;
        Ok(path)
    }

    pub fn include_paths(&self, target: Target) -> io::Result<Vec<PathBuf>> {
        let root = self.resolve(target)?;
        Ok(match target {
            Target::X86_64PcWindowsMsvc | Target::Aarch64PcWindowsMsvc => {
                windows_msvc::include_paths(&root)
            }
            Target::X86_64UnknownLinuxGnu | Target::Aarch64UnknownLinuxGnu => {
                linux_gnu::include_paths(&root, target)
            }
            Target::X86_64UnknownLinuxMusl | Target::Aarch64UnknownLinuxMusl => {
                linux_musl::include_paths(&root)
            }
        })
    }

    pub fn doctor(&self, target: Target) -> Vec<DoctorCheck> {
        let root = self.sysroot_path(target);
        match target {
            Target::X86_64PcWindowsMsvc | Target::Aarch64PcWindowsMsvc => {
                windows_msvc::doctor(&root, target)
            }
            Target::X86_64UnknownLinuxGnu | Target::Aarch64UnknownLinuxGnu => {
                linux_gnu::doctor(&root, target)
            }
            Target::X86_64UnknownLinuxMusl | Target::Aarch64UnknownLinuxMusl => {
                linux_musl::doctor(&root, target)
            }
        }
    }

    pub fn install(&self, target: Target) -> io::Result<PathBuf> {
        match target {
            Target::X86_64PcWindowsMsvc | Target::Aarch64PcWindowsMsvc => {
                let xwin = env::var_os("XWIN").unwrap_or_else(|| "xwin".into());
                self.install_with_xwin(target, &xwin)
            }
            Target::X86_64UnknownLinuxGnu | Target::Aarch64UnknownLinuxGnu => {
                linux_gnu::install(self, target)
            }
            Target::X86_64UnknownLinuxMusl | Target::Aarch64UnknownLinuxMusl => {
                linux_musl::install(self, target)
            }
        }
    }

    pub fn install_with_xwin(&self, target: Target, xwin: &OsStr) -> io::Result<PathBuf> {
        match target {
            Target::X86_64PcWindowsMsvc | Target::Aarch64PcWindowsMsvc => {
                windows_msvc::install(self, target, xwin)
            }
            _ => Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                format!("{} is not a Windows MSVC target", target.triple()),
            )),
        }
    }
}

#[cfg(all(test, unix))]
mod tests;
