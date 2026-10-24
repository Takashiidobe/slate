use crate::download::fetch;
use crate::install::{MANIFEST_FILE, install_staged};
use crate::{DoctorCheck, Paths, Target};
use std::fs::{self, File};
use std::io;
use std::path::{Path, PathBuf};
use xz2::read::XzDecoder;

const SNAPSHOT: &str = "https://snapshot.debian.org/archive/debian/20261015T205142Z/pool/main";
const GLIBC_POOL: &str = "c/cross-toolchain-base";
const GLIBC_PORTS_POOL: &str = "c/cross-toolchain-base-ports";
const LINUX_POOL: &str = "l/linux";

struct Package {
    pool: &'static str,
    filename: &'static str,
    sha256: &'static str,
}

struct Release {
    triplet: &'static str,
    glibc: &'static str,
    linux_headers: &'static str,
    debian: &'static str,
    packages: [Package; 4],
}

const LINUX_UAPI: Package = Package {
    pool: LINUX_POOL,
    filename: "linux-libc-dev_7.2.11-1_all.deb",
    sha256: "816dffbb6bbce8e125c0c8ffa526f3e16433e1e926363f297a24208d124d9923",
};

const X86_64: Release = Release {
    triplet: "x86_64-linux-gnu",
    glibc: "2.43-3",
    linux_headers: "7.2.11-1",
    debian: "Debian Sid",
    packages: [
        Package {
            pool: GLIBC_POOL,
            filename: "libc6-amd64-cross_2.43-3cross8_all.deb",
            sha256: "902d11748eb83e0f3dea4377f1e59af18fafa34c7de56b61b7a0bc9972f724fb",
        },
        Package {
            pool: GLIBC_POOL,
            filename: "libc6-dev-amd64-cross_2.43-3cross8_all.deb",
            sha256: "377d11965ef7b26dc279fdb9b91271916a73a47754a1a7037b01e1bd4be1228f",
        },
        Package {
            pool: LINUX_POOL,
            filename: "linux-libc-dev-amd64-cross_7.2.11-1_all.deb",
            sha256: "49b4c084c0bc41133171ee46ffcc62148469c5836f68e4784899a7f884394a1f",
        },
        LINUX_UAPI,
    ],
};

const AARCH64: Release = Release {
    triplet: "aarch64-linux-gnu",
    glibc: "2.43-3",
    linux_headers: "7.2.11-1",
    debian: "Debian Sid",
    packages: [
        Package {
            pool: GLIBC_POOL,
            filename: "libc6-arm64-cross_2.43-3cross8_all.deb",
            sha256: "1192c1a3de7c48169235ff137c6b6f81ad7af93b0f26b48fbe39b8030388c43a",
        },
        Package {
            pool: GLIBC_POOL,
            filename: "libc6-dev-arm64-cross_2.43-3cross8_all.deb",
            sha256: "f0bb4dc323b0d493cf992df554acfbd47046681a328e105c9bb4707a637c9640",
        },
        Package {
            pool: LINUX_POOL,
            filename: "linux-libc-dev-arm64-cross_7.2.11-1_all.deb",
            sha256: "1169821349e46159a1c2687bf2422bcfb057e7c80739587fb71e1b9e960820bd",
        },
        LINUX_UAPI,
    ],
};

const I686: Release = Release {
    triplet: "i686-linux-gnu",
    glibc: "2.43-3",
    linux_headers: "7.2.11-1",
    debian: "Debian Sid",
    packages: [
        Package {
            pool: GLIBC_POOL,
            filename: "libc6-i386-cross_2.43-3cross8_all.deb",
            sha256: "3aa5c2c33d9ab1f6e2f0b2cc0e5f932cd942da810cba97c8e6fca43f625e5435",
        },
        Package {
            pool: GLIBC_POOL,
            filename: "libc6-dev-i386-cross_2.43-3cross8_all.deb",
            sha256: "0fbf0c61f7a7dfe6e6c96238936edac05262256256275bdc55faf6f8a7575e12",
        },
        Package {
            pool: LINUX_POOL,
            filename: "linux-libc-dev-i386-cross_7.2.11-1_all.deb",
            sha256: "12a2d8558833b4cb2b5febe9ad8431581c72735075e8bcd924a535225e02077b",
        },
        LINUX_UAPI,
    ],
};

const ARMEL: Release = Release {
    triplet: "arm-linux-gnueabi",
    glibc: "2.43-3",
    linux_headers: "7.2.11-1",
    debian: "Debian Sid",
    packages: [
        Package {
            pool: GLIBC_PORTS_POOL,
            filename: "libc6-armel-cross_2.43-3cross8_all.deb",
            sha256: "eea89767b5e390a53a33a3f2856f1c9ddc4817b958c2fac065f1bea49a00b2f6",
        },
        Package {
            pool: GLIBC_PORTS_POOL,
            filename: "libc6-dev-armel-cross_2.43-3cross8_all.deb",
            sha256: "7b255b73e3846ecdd37cbd2a4d09c3070d90bc6831c1ff37fcadde7b2b0a2d8f",
        },
        Package {
            pool: LINUX_POOL,
            filename: "linux-libc-dev-armel-cross_7.2.11-1_all.deb",
            sha256: "6a39635d70750d2641ac1adafcc4fe98494eb39467fe61f4dbe4eb0b90e38f59",
        },
        LINUX_UAPI,
    ],
};

const ARMHF: Release = Release {
    triplet: "arm-linux-gnueabihf",
    glibc: "2.43-3",
    linux_headers: "7.2.11-1",
    debian: "Debian Sid",
    packages: [
        Package {
            pool: GLIBC_POOL,
            filename: "libc6-armhf-cross_2.43-3cross8_all.deb",
            sha256: "41e24b4b2a9507688a95493f2024cbd1baeca4003db3da6d203843414cae2d34",
        },
        Package {
            pool: GLIBC_POOL,
            filename: "libc6-dev-armhf-cross_2.43-3cross8_all.deb",
            sha256: "74f89d7ce010f6889df318d178ac9ffd81b227af3013554ba8810f6ff312de26",
        },
        Package {
            pool: LINUX_POOL,
            filename: "linux-libc-dev-armhf-cross_7.2.11-1_all.deb",
            sha256: "72728621e4b4d58b8fab2c01bccd999a5d1f0b7d2dd3fce098e43f6c323461ff",
        },
        LINUX_UAPI,
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

pub(crate) fn manifest(target: Target) -> String {
    let release = release(target);
    format!(
        "Target: {}\nLibc: glibc {} (Debian cross-toolchain-base)\nLinux UAPI headers: {}\nSource: {} packages from {SNAPSHOT}\nPackages:\n{}",
        target.triple(),
        release.glibc,
        release.linux_headers,
        release.debian,
        release
            .packages
            .iter()
            .map(|package| format!("  {}\n", package.filename))
            .collect::<String>()
    )
}

pub(crate) fn install(paths: &Paths, target: Target) -> io::Result<PathBuf> {
    install_staged(
        paths,
        target,
        Some(&manifest(target)),
        |root| validate(root, target),
        |root| {
            fs::create_dir(root)?;
            let release = release(target);
            let cache = paths.cache.join("linux/debian");
            for package in &release.packages {
                let url = format!("{SNAPSHOT}/{}/{}", package.pool, package.filename);
                let archive = fetch(&cache, package.filename, &url, package.sha256)?;
                extract_deb_data(&archive, root)?;
            }
            fs::write(root.join(MANIFEST_FILE), manifest(target))?;
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
