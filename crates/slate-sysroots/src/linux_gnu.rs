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

struct Release {
    triplet: &'static str,
    glibc: &'static str,
    linux_headers: &'static str,
    debian: &'static str,
    packages: [Package; 3],
}

const X86_64: Release = Release {
    triplet: "x86_64-linux-gnu",
    glibc: "2.43-3",
    linux_headers: "6.12.38-1",
    debian: "Debian Sid",
    packages: [
        Package {
            filename: "libc6-amd64-cross_2.43-3cross8_all.deb",
            sha256: "902d11748eb83e0f3dea4377f1e59af18fafa34c7de56b61b7a0bc9972f724fb",
        },
        Package {
            filename: "libc6-dev-amd64-cross_2.43-3cross8_all.deb",
            sha256: "377d11965ef7b26dc279fdb9b91271916a73a47754a1a7037b01e1bd4be1228f",
        },
        Package {
            filename: "linux-libc-dev-amd64-cross_6.12.38-1cross1_all.deb",
            sha256: "da463ddd3ee96b37c33f16da99c47b03b4e514888d3d853e73253ee73bd0d7d8",
        },
    ],
};

const AARCH64: Release = Release {
    triplet: "aarch64-linux-gnu",
    glibc: "2.36-8",
    linux_headers: "6.1.4-1",
    debian: "Debian Bookworm",
    packages: [
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
    ],
};

const I686: Release = Release {
    triplet: "i686-linux-gnu",
    glibc: "2.43-3",
    linux_headers: "6.12.38-1",
    debian: "Debian Sid",
    packages: [
        Package {
            filename: "libc6-i386-cross_2.43-3cross8_all.deb",
            sha256: "3aa5c2c33d9ab1f6e2f0b2cc0e5f932cd942da810cba97c8e6fca43f625e5435",
        },
        Package {
            filename: "libc6-dev-i386-cross_2.43-3cross8_all.deb",
            sha256: "0fbf0c61f7a7dfe6e6c96238936edac05262256256275bdc55faf6f8a7575e12",
        },
        Package {
            filename: "linux-libc-dev-i386-cross_6.12.38-1cross1_all.deb",
            sha256: "6375cf48f33a25df794c26fe5a4ab0edddf9284d57867a82ed5ad846c2e7a1c5",
        },
    ],
};

const ARMEL: Release = Release {
    triplet: "arm-linux-gnueabi",
    glibc: "2.36-8",
    linux_headers: "6.1.4-1",
    debian: "Debian Bookworm",
    packages: [
        Package {
            filename: "libc6-armel-cross_2.36-8cross1_all.deb",
            sha256: "1f263368de455d680381c6665956770e63f7646084cc4d8393750e23475dde13",
        },
        Package {
            filename: "libc6-dev-armel-cross_2.36-8cross1_all.deb",
            sha256: "c0f6385b372dcdcbd6c4b1628cc3c41c4d114d591665b763f62e33947d032117",
        },
        Package {
            filename: "linux-libc-dev-armel-cross_6.1.4-1cross1_all.deb",
            sha256: "b62b393dbc326f7d45571cfb11ae96863af6640269212b9bdb28a86b7fe05809",
        },
    ],
};

const ARMHF: Release = Release {
    triplet: "arm-linux-gnueabihf",
    glibc: "2.43-3",
    linux_headers: "6.12.38-1",
    debian: "Debian Sid",
    packages: [
        Package {
            filename: "libc6-armhf-cross_2.43-3cross8_all.deb",
            sha256: "41e24b4b2a9507688a95493f2024cbd1baeca4003db3da6d203843414cae2d34",
        },
        Package {
            filename: "libc6-dev-armhf-cross_2.43-3cross8_all.deb",
            sha256: "74f89d7ce010f6889df318d178ac9ffd81b227af3013554ba8810f6ff312de26",
        },
        Package {
            filename: "linux-libc-dev-armhf-cross_6.12.38-1cross1_all.deb",
            sha256: "bf6fa2d938e493a84c34086219b3e3e391aba885d4fb349022eda9fdafa36272",
        },
    ],
};

fn release(target: Target) -> &'static Release {
    match target {
        Target::X86_64UnknownLinuxGnu => &X86_64,
        Target::Aarch64UnknownLinuxGnu => &AARCH64,
        Target::I686UnknownLinuxGnu => &I686,
        Target::Armv7UnknownLinuxGnueabi => &ARMEL,
        Target::Armv7UnknownLinuxGnueabihf => &ARMHF,
        _ => unreachable!("glibc module received a non-glibc target"),
    }
}

pub(crate) fn install(paths: &Paths, target: Target) -> io::Result<PathBuf> {
    install_staged(
        paths,
        target,
        |root| validate(root, target),
        |root| {
            fs::create_dir(root)?;
            let release = release(target);
            let cache = paths.cache.join("linux/debian");
            for package in &release.packages {
                let url = format!("{BASE_URL}/{}", package.filename);
                let archive = fetch(&cache, package.filename, &url, package.sha256)?;
                extract_deb_data(&archive, root)?;
            }
            fs::write(
                root.join("SYSROOT-MANIFEST.txt"),
                format!(
                    "Target: {}\nLibc: glibc {} (Debian cross-toolchain-base)\nLinux UAPI headers: {}\nSource: {} cross-toolchain-base packages\nPackages:\n{}",
                    target.triple(),
                    release.glibc,
                    release.linux_headers,
                    release.debian,
                    release
                        .packages
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
    release(target).triplet
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
