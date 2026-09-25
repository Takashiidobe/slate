use crate::download::fetch;
use crate::install::install_staged;
use crate::{DoctorCheck, Paths, Target};
use std::fs::{self, File};
use std::io;
use std::path::{Path, PathBuf};
use xz2::read::XzDecoder;

const RELEASE: &str = "20260430";
const BASE_URL: &str = "https://github.com/cross-tools/musl-cross/releases/download/20260430";

pub(crate) fn install(paths: &Paths, target: Target) -> io::Result<PathBuf> {
    install_staged(
        paths,
        target,
        |root| validate(root, target),
        |root| {
            let filename = format!("{}.tar.xz", target.triple());
            let url = format!("{BASE_URL}/{filename}");
            let archive = fetch(
                &paths.cache.join("linux/musl"),
                &filename,
                &url,
                sha256(target),
            )?;
            extract_sysroot(&archive, target, root)?;
            fs::write(
                root.join("SYSROOT-MANIFEST.txt"),
                format!(
                    "Target: {}\nLibc: musl (cross-tools/musl-cross release {RELEASE})\nSource URL: {url}\nSource SHA-256: {}\nContents: prebuilt libc sysroot and license notices\n",
                    target.triple(),
                    sha256(target)
                ),
            )?;
            Ok(())
        },
    )
}

pub(crate) fn include_paths(root: &Path) -> Vec<PathBuf> {
    vec![root.join("usr/include")]
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
    let include = root.join("usr/include");
    let lib = root.join("usr/lib");
    let loader = root.join(format!("lib/ld-musl-{}.so.1", arch(target)));
    vec![
        DoctorCheck {
            label: "musl headers",
            present: include.join("stdio.h").is_file() && include.join("stdlib.h").is_file(),
            path: include.clone(),
        },
        DoctorCheck {
            label: "Linux UAPI headers",
            present: include.join("linux/version.h").is_file(),
            path: include,
        },
        DoctorCheck {
            label: "musl libraries and startup files",
            present: lib.join("libc.a").is_file()
                && lib.join("crt1.o").is_file()
                && loader.is_file(),
            path: lib,
        },
    ]
}

fn arch(target: Target) -> &'static str {
    match target {
        Target::X86_64UnknownLinuxMusl => "x86_64",
        Target::Aarch64UnknownLinuxMusl => "aarch64",
        _ => unreachable!("musl module received a non-musl target"),
    }
}

fn sha256(target: Target) -> &'static str {
    match target {
        Target::X86_64UnknownLinuxMusl => {
            "2495cfe18fc1f406d5cab93d902176af75a78f0ae93137f3e8b2df7708ec32fa"
        }
        Target::Aarch64UnknownLinuxMusl => {
            "9303385fc29f8197004f641f96382196d56e4ec6dd975bff8ddd66641378628d"
        }
        _ => unreachable!("musl module received a non-musl target"),
    }
}

fn extract_sysroot(archive: &Path, target: Target, root: &Path) -> io::Result<()> {
    let staging = root.parent().expect("staged root has a parent");
    let prefix = PathBuf::from(target.triple())
        .join(target.triple())
        .join("sysroot");
    let musl_license = PathBuf::from(target.triple()).join("share/licenses/musl/COPYRIGHT");
    let linux_license = PathBuf::from(target.triple()).join("share/licenses/linux/COPYING");
    let decoder = XzDecoder::new(File::open(archive)?);
    let mut tar = tar::Archive::new(decoder);
    for entry in tar.entries()? {
        let mut entry = entry?;
        let path = entry.path()?;
        if (path.starts_with(&prefix) || path == musl_license || path == linux_license)
            && !entry.header().entry_type().is_dir()
            && !entry.unpack_in(staging)?
        {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                format!("unsafe path in {}", archive.display()),
            ));
        }
    }
    fs::rename(staging.join(&prefix), root)?;
    fs::create_dir_all(root.join("licenses/musl"))?;
    fs::create_dir_all(root.join("licenses/linux"))?;
    fs::copy(
        staging.join(musl_license),
        root.join("licenses/musl/COPYRIGHT"),
    )?;
    fs::copy(
        staging.join(linux_license),
        root.join("licenses/linux/COPYING"),
    )?;
    Ok(())
}
