use crate::download::fetch;
use crate::install::install_staged;
use crate::{DoctorCheck, Paths, Target};
use std::fs::{self, File};
use std::io;
use std::path::{Path, PathBuf};
use xz2::read::XzDecoder;

const RELEASE: &str = "15.1-RELEASE";

pub(crate) fn install(paths: &Paths, target: Target) -> io::Result<PathBuf> {
    let (arch, sha256) = release(target);
    let url = format!("https://download.freebsd.org/releases/{arch}/{RELEASE}/base.txz");
    install_staged(paths, target, validate, |root| {
        fs::create_dir(root)?;
        let filename = format!("{RELEASE}-{}-base.txz", arch_name(target));
        let archive = fetch(&paths.cache.join("freebsd"), &filename, &url, sha256)?;
        extract_headers(&archive, root)?;
        fs::write(
            root.join("SYSROOT-MANIFEST.txt"),
            format!(
                "Target: {}\nOperating system: FreeBSD {RELEASE}\nContents: headers only, from the FreeBSD base set\nSource URL: {url}\nSource SHA-256: {sha256}\n",
                target.triple()
            ),
        )?;
        Ok(())
    })
}

fn release(target: Target) -> (&'static str, &'static str) {
    match target {
        Target::X86_64UnknownFreebsd => (
            "amd64/amd64",
            "3768988b151c20f965679062b065c63a977d6bbb9f47fd83695ec2c40790c18f",
        ),
        Target::Aarch64UnknownFreebsd => (
            "arm64/aarch64",
            "5b7a46a0abfbe23a1d4454b5600e2efcfce16b705bc7e9c851c37470c035ef98",
        ),
        _ => unreachable!("FreeBSD module received a non-FreeBSD target"),
    }
}

fn arch_name(target: Target) -> &'static str {
    match target {
        Target::X86_64UnknownFreebsd => "amd64",
        Target::Aarch64UnknownFreebsd => "aarch64",
        _ => unreachable!("FreeBSD module received a non-FreeBSD target"),
    }
}

fn extract_headers(archive: &Path, root: &Path) -> io::Result<()> {
    let decoder = XzDecoder::new(File::open(archive)?);
    let mut tar = tar::Archive::new(decoder);
    for entry in tar.entries()? {
        let mut entry = entry?;
        let path = entry.path()?;
        let path = path.strip_prefix(".").unwrap_or(&path);
        if (path == Path::new("COPYRIGHT") || path.starts_with("usr/include"))
            && !entry.header().entry_type().is_dir()
            && !entry.unpack_in(root)?
        {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                format!("unsafe path in {}", archive.display()),
            ));
        }
    }
    Ok(())
}

pub(crate) fn include_paths(root: &Path) -> Vec<PathBuf> {
    vec![root.join("usr/include")]
}

pub(crate) fn doctor(root: &Path) -> Vec<DoctorCheck> {
    let include = root.join("usr/include");
    vec![
        DoctorCheck {
            label: "FreeBSD C headers",
            present: include.join("stdio.h").is_file() && include.join("sys/types.h").is_file(),
            path: include,
        },
        DoctorCheck {
            label: "FreeBSD copyright notice",
            present: root.join("COPYRIGHT").is_file(),
            path: root.join("COPYRIGHT"),
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
