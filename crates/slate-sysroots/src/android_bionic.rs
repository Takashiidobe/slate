use crate::download::fetch;
use crate::install::install_staged;
use crate::{DoctorCheck, Paths, Target};
use std::fs::{self, File};
use std::io::{self, Read};
use std::path::{Path, PathBuf};
use zip::ZipArchive;

const NDK_REVISION: &str = "27.3.13750724";
const NDK_FILENAME: &str = "android-ndk-r27d-linux.zip";
const NDK_URL: &str = "https://dl.google.com/android/repository/android-ndk-r27d-linux.zip";
const NDK_SHA256: &str = "601246087a682d1944e1e16dd85bc6e49560fe8b6d61255be2829178c8ed15d9";
const HEADER_PREFIX: &str =
    "android-ndk-r27d/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include";
const ARCH_DIRS: [&str; 5] = [
    "aarch64-linux-android",
    "arm-linux-androideabi",
    "i686-linux-android",
    "riscv64-linux-android",
    "x86_64-linux-android",
];

pub(crate) fn install(paths: &Paths, target: Target) -> io::Result<PathBuf> {
    install_staged(
        paths,
        target,
        |root| validate(root, target),
        |root| {
            let archive = fetch(
                &paths.cache.join("android"),
                NDK_FILENAME,
                NDK_URL,
                NDK_SHA256,
            )?;
            extract_headers(&archive, target, root)?;
            fs::write(
                root.join("SYSROOT-MANIFEST.txt"),
                format!(
                    "Target: {}\nOperating system: Android (Bionic)\nNDK: r27d ({NDK_REVISION})\nAndroid API level: 21\nContents: C system headers only; link libraries and runtime files omitted\nSource URL: {NDK_URL}\nSource SHA-256: {NDK_SHA256}\n",
                    target.triple()
                ),
            )?;
            Ok(())
        },
    )
}

pub(crate) fn extract_headers(archive: &Path, target: Target, root: &Path) -> io::Result<()> {
    let mut zip = ZipArchive::new(File::open(archive)?).map_err(io::Error::other)?;
    let mut properties = String::new();
    zip.by_name("android-ndk-r27d/source.properties")
        .map_err(io::Error::other)?
        .read_to_string(&mut properties)?;
    if !properties
        .lines()
        .any(|line| line.trim() == format!("Pkg.Revision = {NDK_REVISION}"))
    {
        return Err(io::Error::new(
            io::ErrorKind::InvalidData,
            format!("expected Android NDK r27d ({NDK_REVISION})"),
        ));
    }
    let notice = root.join("licenses/ANDROID-NDK-NOTICE");
    fs::create_dir_all(notice.parent().expect("notice has a parent"))?;
    {
        let mut source = zip
            .by_name("android-ndk-r27d/NOTICE")
            .map_err(io::Error::other)?;
        let mut destination = File::create(&notice)?;
        io::copy(&mut source, &mut destination)?;
    }
    let include = root.join("usr/include");
    for index in 0..zip.len() {
        let mut source = zip.by_index(index).map_err(io::Error::other)?;
        if source.is_dir() {
            continue;
        }
        let Some(name) = source.enclosed_name() else {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "unsafe NDK ZIP path",
            ));
        };
        let Ok(relative) = name.strip_prefix(HEADER_PREFIX) else {
            continue;
        };
        if relative.as_os_str().is_empty() {
            continue;
        }
        let first = relative
            .components()
            .next()
            .expect("relative header path is nonempty");
        if ARCH_DIRS.iter().any(|arch| first.as_os_str() == *arch)
            && first.as_os_str() != target.triple()
        {
            continue;
        }
        let output = include.join(relative);
        fs::create_dir_all(output.parent().expect("header has a parent"))?;
        let mut destination = File::create(output)?;
        io::copy(&mut source, &mut destination)?;
    }
    Ok(())
}

pub(crate) fn include_paths(root: &Path, target: Target) -> Vec<PathBuf> {
    vec![
        root.join("usr/include").join(target.triple()),
        root.join("usr/include"),
    ]
}

pub(crate) fn doctor(root: &Path, target: Target) -> Vec<DoctorCheck> {
    let include = root.join("usr/include");
    let arch = include.join(target.triple());
    vec![
        DoctorCheck {
            label: "Bionic C headers",
            present: include.join("stdio.h").is_file() && include.join("stdlib.h").is_file(),
            path: include,
        },
        DoctorCheck {
            label: "Android architecture headers",
            present: arch.join("asm/types.h").is_file(),
            path: arch,
        },
        DoctorCheck {
            label: "Android NDK notice",
            present: root.join("licenses/ANDROID-NDK-NOTICE").is_file(),
            path: root.join("licenses/ANDROID-NDK-NOTICE"),
        },
    ]
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
