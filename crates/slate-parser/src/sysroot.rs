use crate::compiler_args::CompilerFlavor;
use std::path::{Path, PathBuf};

pub fn path(target: &str) -> PathBuf {
    let root = std::env::var_os("SLATE_SYSROOTS")
        .map(PathBuf::from)
        .unwrap_or_else(|| {
            Path::new(env!("CARGO_MANIFEST_DIR")).join("../slate-sysroots/sysroots")
        });
    root.join(target)
}

pub fn include_paths(target: &str, flavor: CompilerFlavor) -> Vec<PathBuf> {
    let sysroot = path(target);
    let candidates = if flavor == CompilerFlavor::Msvc {
        vec![
            sysroot.join("crt/include"),
            sysroot.join("sdk/include/ucrt"),
            sysroot.join("sdk/include/shared"),
            sysroot.join("sdk/include/um"),
            sysroot.join("sdk/include/winrt"),
            sysroot.join("sdk/include/cppwinrt"),
            sysroot.join("include"),
            sysroot.join("usr/include"),
        ]
    } else {
        let mut candidates = vec![
            sysroot.join("SDK/usr/include"),
            sysroot.join("usr/include"),
            sysroot.join("include"),
        ];
        if let Some(arch) = target.strip_suffix("-unknown-linux-gnu") {
            candidates.push(
                sysroot
                    .join("usr")
                    .join(format!("{arch}-linux-gnu/include")),
            );
        }
        candidates
    };
    candidates
        .into_iter()
        .filter(|candidate| candidate.is_dir())
        .collect()
}
