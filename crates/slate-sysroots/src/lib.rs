mod android_bionic;
mod compiler_headers;
mod darwin;
mod download;
mod freebsd;
mod install;
mod linux_gnu;
mod linux_musl;
mod target;
mod windows_msvc;

pub use compiler_headers::{CLANG_VERSION, CompilerHeaders, GCC_VERSION};
pub use target::Target;

use directories::ProjectDirs;
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
            Target::I686PcWindowsMsvc
            | Target::X86_64PcWindowsMsvc
            | Target::Aarch64PcWindowsMsvc
            | Target::Thumbv7aPcWindowsMsvc => windows_msvc::validate(&path, target),
            Target::X86_64UnknownLinuxGnu | Target::Aarch64UnknownLinuxGnu => {
                linux_gnu::validate(&path, target)
            }
            Target::X86_64UnknownLinuxMusl | Target::Aarch64UnknownLinuxMusl => {
                linux_musl::validate(&path, target)
            }
            Target::X86_64AppleDarwin | Target::Aarch64AppleDarwin => darwin::validate(&path),
            Target::X86_64UnknownFreebsd | Target::Aarch64UnknownFreebsd => {
                freebsd::validate(&path)
            }
            Target::X86_64LinuxAndroid | Target::Aarch64LinuxAndroid => {
                android_bionic::validate(&path, target)
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
            Target::I686PcWindowsMsvc
            | Target::X86_64PcWindowsMsvc
            | Target::Aarch64PcWindowsMsvc
            | Target::Thumbv7aPcWindowsMsvc => windows_msvc::include_paths(&root),
            Target::X86_64UnknownLinuxGnu | Target::Aarch64UnknownLinuxGnu => {
                linux_gnu::include_paths(&root, target)
            }
            Target::X86_64UnknownLinuxMusl | Target::Aarch64UnknownLinuxMusl => {
                linux_musl::include_paths(&root)
            }
            Target::X86_64AppleDarwin | Target::Aarch64AppleDarwin => darwin::include_paths(&root),
            Target::X86_64UnknownFreebsd | Target::Aarch64UnknownFreebsd => {
                freebsd::include_paths(&root)
            }
            Target::X86_64LinuxAndroid | Target::Aarch64LinuxAndroid => {
                android_bionic::include_paths(&root, target)
            }
        })
    }

    pub fn include_paths_with_compiler(
        &self,
        target: Target,
        compiler: CompilerHeaders,
    ) -> io::Result<Vec<PathBuf>> {
        if let CompilerHeaders::Msvc(compiler_target) = compiler
            && compiler_target != target
        {
            return Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "MSVC compiler headers must match the target sysroot",
            ));
        }
        if compiler == CompilerHeaders::AppleClang
            && !matches!(
                target,
                Target::X86_64AppleDarwin | Target::Aarch64AppleDarwin
            )
        {
            return Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "Apple Clang compiler headers require a macOS target",
            ));
        }
        let compiler_path = self.resolve_compiler_headers(compiler)?;
        let mut paths = self.include_paths(target)?;
        if !paths.contains(&compiler_path) {
            paths.insert(0, compiler_path);
        }
        Ok(paths)
    }

    pub fn doctor(&self, target: Target) -> Vec<DoctorCheck> {
        let root = self.sysroot_path(target);
        match target {
            Target::I686PcWindowsMsvc
            | Target::X86_64PcWindowsMsvc
            | Target::Aarch64PcWindowsMsvc
            | Target::Thumbv7aPcWindowsMsvc => windows_msvc::doctor(&root, target),
            Target::X86_64UnknownLinuxGnu | Target::Aarch64UnknownLinuxGnu => {
                linux_gnu::doctor(&root, target)
            }
            Target::X86_64UnknownLinuxMusl | Target::Aarch64UnknownLinuxMusl => {
                linux_musl::doctor(&root, target)
            }
            Target::X86_64AppleDarwin | Target::Aarch64AppleDarwin => darwin::doctor(&root),
            Target::X86_64UnknownFreebsd | Target::Aarch64UnknownFreebsd => freebsd::doctor(&root),
            Target::X86_64LinuxAndroid | Target::Aarch64LinuxAndroid => {
                android_bionic::doctor(&root, target)
            }
        }
    }

    pub fn install(&self, target: Target) -> io::Result<PathBuf> {
        match target {
            Target::I686PcWindowsMsvc
            | Target::X86_64PcWindowsMsvc
            | Target::Aarch64PcWindowsMsvc
            | Target::Thumbv7aPcWindowsMsvc => windows_msvc::install(self, target),
            Target::X86_64UnknownLinuxGnu | Target::Aarch64UnknownLinuxGnu => {
                linux_gnu::install(self, target)
            }
            Target::X86_64UnknownLinuxMusl | Target::Aarch64UnknownLinuxMusl => {
                linux_musl::install(self, target)
            }
            Target::X86_64AppleDarwin | Target::Aarch64AppleDarwin => darwin::install(self, target),
            Target::X86_64UnknownFreebsd | Target::Aarch64UnknownFreebsd => {
                freebsd::install(self, target)
            }
            Target::X86_64LinuxAndroid | Target::Aarch64LinuxAndroid => {
                android_bionic::install(self, target)
            }
        }
    }

    pub fn remove(&self, target: Target) -> io::Result<()> {
        install::remove(self, target)
    }

    pub fn install_darwin_with_sdk(
        &self,
        target: Target,
        sdk: &std::path::Path,
    ) -> io::Result<PathBuf> {
        darwin::install_with_sdk(self, target, sdk)
    }
}

#[cfg(all(test, unix))]
mod tests;
