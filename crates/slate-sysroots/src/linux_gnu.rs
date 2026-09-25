use crate::download::fetch;
use crate::install::install_staged;
use crate::{DoctorCheck, Paths, Target};
use std::fs::{self, File};
use std::io;
use std::path::{Path, PathBuf};
use xz2::read::XzDecoder;

const BASE_URL: &str = "https://deb.debian.org/debian/pool/main/c/cross-toolchain-base";

struct Package {
    filename: &'static str,
    sha256: &'static str,
}

const X86_64_PACKAGES: [Package; 3] = [
    Package {
        filename: "libc6-amd64-cross_2.36-8cross1_all.deb",
        sha256: "e410f3d2da35bccf757976d38e6a309d4d94d25c0dfde565a9662e3f75951b2c",
    },
    Package {
        filename: "libc6-dev-amd64-cross_2.36-8cross1_all.deb",
        sha256: "197b497ed91056e6c4093b0972296199260424f24943d7108a8124cc7de750d6",
    },
    Package {
        filename: "linux-libc-dev-amd64-cross_6.1.4-1cross1_all.deb",
        sha256: "b7ec9f95d20bd4669119201ec6a1ac372189fc222d35f21631bb79b723519293",
    },
];

const AARCH64_PACKAGES: [Package; 3] = [
    Package {
        filename: "libc6-arm64-cross_2.36-8cross1_all.deb",
        sha256: "91936cbbee75771c360ed16513f6e734a25ca5517fe7c3e13dfd7835d7553186",
    },
    Package {
        filename: "libc6-dev-arm64-cross_2.36-8cross1_all.deb",
        sha256: "5e4cf6abf0e89e89c4b3006bb8b7dc47aa831df25d94df9134929b8b968cacb4",
    },
    Package {
        filename: "linux-libc-dev-arm64-cross_6.1.4-1cross1_all.deb",
        sha256: "66457f015d16b7d372db6591cff38db0da928362ff2ea1fa75491e28f6904d81",
    },
];

pub(crate) fn install(paths: &Paths, target: Target) -> io::Result<PathBuf> {
    install_staged(
        paths,
        target,
        |root| validate(root, target),
        |root| {
            fs::create_dir(root)?;
            let packages = match target {
                Target::X86_64UnknownLinuxGnu => &X86_64_PACKAGES,
                Target::Aarch64UnknownLinuxGnu => &AARCH64_PACKAGES,
                _ => unreachable!("glibc module received a non-glibc target"),
            };
            let cache = paths.cache.join("linux/debian");
            for package in packages {
                let url = format!("{BASE_URL}/{}", package.filename);
                let archive = fetch(&cache, package.filename, &url, package.sha256)?;
                extract_deb_data(&archive, root)?;
            }
            fs::write(
                root.join("SYSROOT-MANIFEST.txt"),
                format!(
                    "Target: {}\nLibc: glibc 2.36-8 (Debian cross-toolchain-base)\nLinux UAPI headers: 6.1.4-1\nSource: Debian Bookworm cross-toolchain-base packages\nPackages:\n{}",
                    target.triple(),
                    packages
                        .iter()
                        .map(|package| format!("  {}\n", package.filename))
                        .collect::<String>()
                ),
            )?;
            Ok(())
        },
    )
}

pub(crate) fn include_paths(root: &Path, target: Target) -> Vec<PathBuf> {
    vec![root.join(format!("usr/{}/include", debian_triplet(target)))]
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
    let triplet = debian_triplet(target);
    let include = root.join(format!("usr/{triplet}/include"));
    let lib = root.join(format!("usr/{triplet}/lib"));
    [
        DoctorCheck {
            label: "glibc headers",
            present: include.join("stdio.h").is_file() && include.join("stdlib.h").is_file(),
            path: include.clone(),
        },
        DoctorCheck {
            label: "Linux UAPI headers",
            present: include.join("linux/version.h").is_file(),
            path: include,
        },
        DoctorCheck {
            label: "glibc libraries and startup files",
            present: lib.join("libc.a").is_file() && lib.join("crt1.o").is_file(),
            path: lib,
        },
    ]
    .into()
}

fn debian_triplet(target: Target) -> &'static str {
    match target {
        Target::X86_64UnknownLinuxGnu => "x86_64-linux-gnu",
        Target::Aarch64UnknownLinuxGnu => "aarch64-linux-gnu",
        _ => unreachable!("glibc module received a non-glibc target"),
    }
}

fn extract_deb_data(package: &Path, root: &Path) -> io::Result<()> {
    let mut archive = ar::Archive::new(File::open(package)?);
    while let Some(entry) = archive.next_entry() {
        let entry = entry?;
        if entry.header().identifier() == b"data.tar.xz" {
            let decoder = XzDecoder::new(entry);
            tar::Archive::new(decoder).unpack(root)?;
            return Ok(());
        }
    }
    Err(io::Error::new(
        io::ErrorKind::InvalidData,
        format!("{} has no data.tar.xz", package.display()),
    ))
}
