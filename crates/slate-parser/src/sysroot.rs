use crate::compiler_args::CompilerFlavor;
use crate::target_info::TargetInfo;
use crate::target_registry::SysrootLayout;
use directories::ProjectDirs;
use std::path::{Path, PathBuf};

pub fn path(target: &str) -> PathBuf {
    let root = std::env::var_os("SLATE_SYSROOTS")
        .map(PathBuf::from)
        .or_else(|| {
            ProjectDirs::from("", "", "Slate").map(|dirs| dirs.data_local_dir().join("sysroots"))
        })
        .unwrap_or_default();
    root.join(target)
}

pub fn include_paths(target: &TargetInfo, flavor: CompilerFlavor) -> Vec<PathBuf> {
    let sysroot = path(&target.triple);
    include_paths_at(&sysroot, target, flavor)
}

pub fn include_paths_at(
    sysroot: &Path,
    target: &TargetInfo,
    flavor: CompilerFlavor,
) -> Vec<PathBuf> {
    let layout = match flavor {
        CompilerFlavor::Msvc => SysrootLayout::WindowsKits,
        CompilerFlavor::Gcc | CompilerFlavor::Clang => target.profile.sysroot,
    };
    let candidates = match layout {
        SysrootLayout::WindowsKits => vec![
            sysroot.join("crt/include"),
            sysroot.join("sdk/include/ucrt"),
            sysroot.join("sdk/include/shared"),
            sysroot.join("sdk/include/um"),
            sysroot.join("sdk/include/winrt"),
            sysroot.join("sdk/include/cppwinrt"),
            sysroot.join("include"),
            sysroot.join("usr/include"),
        ],
        SysrootLayout::Unix { multiarch } => {
            let mut candidates = vec![
                sysroot.join("SDK/usr/include"),
                sysroot.join("usr/include"),
                sysroot.join("include"),
            ];
            candidates.extend(
                multiarch.map(|multiarch| sysroot.join("usr").join(multiarch).join("include")),
            );
            candidates
        }
    };
    candidates
        .into_iter()
        .filter(|candidate| candidate.is_dir())
        .collect()
}
